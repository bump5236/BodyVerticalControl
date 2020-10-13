#include <mcp_can.h>
#include <SPI.h>
#include <RMDx8Arduino.h>
#include <SoftwareSerial.h>

/*SAMD core*/
#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
#define SERIAL serialUSB
#else
#define SERIAL Serial
#endif

#define DIGITAL_INPUT_SYNC 4

/*
1:右モータ
2:左モータ
*/
const uint16_t MOTOR_ADDRESS_R = 0x141; //0x140 + ID(1~32)
const uint16_t MOTOR_ADDRESS_L = 0x142; //0x140 + ID(1~32)
const int SPI_CS_PIN = 9;
float torq_const = 3.3; // RMD X8 Pro

unsigned long timer[3];
float ang_R[2], ang_L[2];
uint16_t cnt;

bool exit_tf = false;
int16_t tgt_cur_R, tgt_cur_L; // 目標電流 [-]
float tgt_torq_R, tgt_torq_L; // 目標トルク

int8_t mode_R = 0; // 0:振り戻し, 1:振り出し, 2:, 3:?
int8_t mode_L = 0; // 0:振り戻し, 1:振り出し, 2:, 3:?

int16_t max_torq = 13;
int16_t min_torq = 1;
int8_t max_ang = 70;
uint8_t min_ang = -5;

// 左足
float rangeA = 0.3; // トルク増減の定数値（振り戻し）
float rangeB = 0.65; // トルク増減の定数値（振り出し）
// 右足
float rangeC = 0.3; // トルク増減の定数値（振り戻し）
float rangeD = 0.65; // トルク増減の定数値（振り出し）

// 新明和(左足)
float AA[10] = {7.76866245e-13, 2.49914818e-10, 3.30915904e-08, 2.31075404e-06, 9.04958779e-05, 1.96488968e-03, 2.31391008e-02, 1.65275818e-01, 3.87948034e-02, 4.69136431};    
float BB[6] = {-2.86180404e-08, -5.03733032e-06, -2.85386130e-04, -3.57590797e-03, 1.45259802e-01, 2.64048368};

// NSKモーター(右足)
float CC[10] = {2.08507152e-12, 6.21331094e-10, 7.55432406e-08, 4.78670255e-06, 1.66457257e-04, 3.02208174e-03, 2.36530654e-02, 4.83321692e-02, -2.21910362e-01, 1.14103111e+01};
float DD[6] = {1.82279504e-07, 3.67404520e-05, 2.83278951e-03, 1.04788154e-01, 1.85489973, 1.23883044e+01};

MCP_CAN CAN(SPI_CS_PIN); // set CS PIN
RMDx8Arduino rmd_R(CAN, MOTOR_ADDRESS_R); // インスタンスの作成
RMDx8Arduino rmd_L(CAN, MOTOR_ADDRESS_L); // インスタンスの作成

void setup()
{
  SERIAL.begin(115200);
  while (!Serial) ;

  pinMode(4, INPUT);

  rmd_R.canSetup();
  rmd_L.canSetup();
  delay(1000);

  rmd_R.clearState(); // 初期化
  rmd_L.clearState(); // 初期化
  
  rmd_R.writePID(40, 40, 50, 40, 20, 20); // PID gainの設定
  rmd_L.writePID(40, 40, 50, 40, 20, 20); // PID gainの設定
  delay(1000);

  timer[0] = millis(); // t0

  rmd_R.readPosition();
  rmd_L.readPosition();
  ang_R[0] = rmd_R.present_position / 600; // 基準の角度 [deg]
  ang_L[0] = rmd_L.present_position / 600; // 基準の角度 [deg]

  SERIAL.println("Waiting to set position ...");
  while (ang_R[1] > - 63)
  {
    if (cnt < 50)
    {
      cnt = cnt + 1;
    }
    rmd_R.readPosition();
    ang_R[1] = - ((rmd_R.present_position / 600) + 5  - ang_R[0]);
    rmd_R.writeCurrent(10 * cnt);
    SERIAL.print("cnt : ");
    SERIAL.println(cnt);
    SERIAL.print("ang : ");
    SERIAL.println(ang_R[1]);
    delay(50);
  }
  rmd_R.serialWriteTerminator();
}

void loop()
{
  while (exit_tf == false)
  {
    uint8_t sync_flag = PIND & _BV(4);

    timer[1] = millis() - timer[0]; // 現在の時刻 [ms]

    rmd_R.readPosition(); // 現在の位置(角度)を取得
    rmd_L.readPosition(); // 現在の位置(角度)を取得
    ang_R[1] = - ((rmd_R.present_position / 600) + 5 - ang_R[0]); // モータ角度 [deg]
    ang_L[1] = (rmd_L.present_position / 600) - 5 - ang_L[0]; // モータ角度 [deg]
    
    // 振り出し,振り戻し切り替え
    if (ang_R[1] <= -65 && mode_R == 0)
    {
      mode_R = 1;
    }
    else if (ang_R[1] >= -5 && mode_R == 1)
    {
      mode_R = 0;
    }

    if (ang_L[1] <= -65 && mode_L == 0)
    {
      mode_L = 1;
    }
    else if (ang_L[1] >= - 5 && mode_L == 1)
    {
      mode_L = 0;
    }

    // 目標トルク定義
    if (mode_R == 0) // 0:振り戻し
    {
      tgt_torq_R = CC[0] * pow(ang_R[1],9) + CC[1] * pow(ang_R[1],8) + CC[2] * pow(ang_R[1],7) + CC[3] * pow(ang_R[1],6) + CC[4] * pow(ang_R[1],5) + CC[5] * pow(ang_R[1],4) + CC[6] * pow(ang_R[1],3) + CC[7] * ang_R[1] * ang_R[1] + CC[8] * ang_R[1] + CC[9];
//      tgt_torq_R = AA[0] * pow(ang_R[1],9) + AA[1] * pow(ang_R[1],8) + AA[2] * pow(ang_R[1],7) + AA[3] * pow(ang_R[1],6) + AA[4] * pow(ang_R[1],5) + AA[5] * pow(ang_R[1],4) + AA[6] * pow(ang_R[1],3) + AA[7] * ang_R[1] * ang_R[1] + AA[8] * ang_R[1] + AA[9];
      tgt_torq_R =  rangeC * tgt_torq_R;
    }
    else if (mode_R == 1) // 1:振り出し
    {
      tgt_torq_R = DD[0] * pow(ang_R[1],5) + DD[1] * pow(ang_R[1],4) + DD[2] * pow(ang_R[1],3) + DD[3] * ang_R[1] * ang_R[1] + DD[4] * ang_R[1] + DD[5];
//      tgt_torq_R = BB[0] * pow(ang_R[1],5) + BB[1] * pow(ang_R[1],4) + BB[2] * pow(ang_R[1],3) + BB[3] * ang_R[1] * ang_R[1] + BB[4] * ang_R[1] + BB[5];
      tgt_torq_R = - rangeD * tgt_torq_R - 0.5;
//      tgt_torq_R = - 0.05;
    }

    if (mode_L == 0) // 0:振り戻し
    {
      tgt_torq_L = AA[0] * pow(ang_L[1],9) + AA[1] * pow(ang_L[1],8) + AA[2] * pow(ang_L[1],7) + AA[3] * pow(ang_L[1],6) + AA[4] * pow(ang_L[1],5) + AA[5] * pow(ang_L[1],4) + AA[6] * pow(ang_L[1],3) + AA[7] * ang_L[1] * ang_L[1] + AA[8] * ang_L[1] + AA[9];
      tgt_torq_L = - rangeA * tgt_torq_L;
    }
    else if (mode_L == 1) // 1:振り出し
    {
      tgt_torq_L = BB[0] * pow(ang_L[1],5) + BB[1] * pow(ang_L[1],4) + BB[2] * pow(ang_L[1],3) + BB[3] * ang_L[1] * ang_L[1] + BB[4] * ang_L[1] + BB[5];
      tgt_torq_L = rangeB * tgt_torq_L + 0.5;
    }

    // トルク制限
    if (tgt_torq_R >= max_torq)
    {
      tgt_torq_R = max_torq;
    }

    else if (tgt_torq_R <= - min_torq)
    {
      tgt_torq_R = - min_torq;
    }

    if (tgt_torq_L <= - max_torq)
    {
      tgt_torq_L = - max_torq;
    }

    else if (tgt_torq_L >= min_torq)
    {
      tgt_torq_L = min_torq;
    }

    // 角度制限
//    if (ang_R[1] > max_ang)
//    {
//      SERIAL.println("A");
//      tgt_torq_R = 0;
//      tgt_torq_L = 0;
//      exit_tf = true;
//    }
//    
//    else if (ang_R[1] < min_ang)
//    {
//      SERIAL.println("B");
//      tgt_torq_R = 0;
//      tgt_torq_L = 0;
//      exit_tf = true;
//    }
//
//    if (ang_L[1] < - max_ang)
//    {
//      SERIAL.println("C");
//      tgt_torq_R = 0;
//      tgt_torq_L = 0;
//      exit_tf = true;
//    }
//    
//    else if (ang_L[1] > - min_ang)
//    {
//      SERIAL.println("D");
//      tgt_torq_R = 0;
//      tgt_torq_L = 0;
//      exit_tf = true;
//    }

    // 目標電流値（整数値） [-]
    tgt_cur_R = (2000 / 12.5 / torq_const) * tgt_torq_R;
    tgt_cur_L = (2000 / 12.5 / torq_const) * tgt_torq_L;

    rmd_R.writeCurrent(0); // 電流値指令 & 現在の温度、電流値、速度を取得
    rmd_L.writeCurrent(0); // 電流値指令 & 現在の温度、電流値、速度を取得


//    SERIAL.print(tgt_cur_R);
//    SERIAL.print("\t");
//    SERIAL.print(mode_R);
//    SERIAL.print("\t");
//    SERIAL.print(tgt_cur_L);
//    SERIAL.print("\t");
//    SERIAL.println(mode_L);

//        SERIAL.println(ang_R[1]);

    SERIAL.print(timer[1]);
    SERIAL.print(",");
    SERIAL.print(sync_flag);
    SERIAL.print(",");
    SERIAL.print(rmd_R.temperature);
    SERIAL.print(",");
    SERIAL.print(tgt_cur_R);
    SERIAL.print(",");
    SERIAL.print(rmd_R.present_current);
    SERIAL.print(",");
    SERIAL.print(rmd_R.present_velocity);
    SERIAL.print(",");
    SERIAL.print(ang_R[1]);
    SERIAL.print(",");
    SERIAL.print(mode_R);
    SERIAL.print(",");

    SERIAL.print(rmd_L.temperature);
    SERIAL.print(",");
    SERIAL.print(tgt_cur_L);
    SERIAL.print(",");
    SERIAL.print(rmd_L.present_current);
    SERIAL.print(",");
    SERIAL.print(rmd_L.present_velocity);
    SERIAL.print(",");
    SERIAL.print(ang_L[1]);
    SERIAL.print(",");
    SERIAL.println(mode_L);

    rmd_R.serialWriteTerminator();
  }

  rmd_R.clearState(); // 初期化
  rmd_L.clearState(); // 初期化

  SERIAL.println("Finish ...");
  while (true)
  {
    delay(1000);
  }

  
}
