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

const uint16_t MOTOR_ADDRESS = 0x143; //0x140 + ID(1~32)
const int SPI_CS_PIN = 10;

unsigned long timer[3];
float ang[4];

bool exit_tf = false;
int16_t tgt_cur; // 目標電流 [-]
float tgt_torq_1; // 目標トルク
float gear_ratio = 2.5; // 減速比

int run_mode = 0; // 0:振り戻し, 1:振り出し, 2:, 3:?

float rangeA = 0.5; // トルク増減の定数値（振り戻し）
float rangeB = 0.65; // トルク増減の定数値（振り出し）
float torq_const = 3.3; // RMD X8 Pro

// 新明和(左足)
float AA[10] = {7.76866245e-13, 2.49914818e-10, 3.30915904e-08, 2.31075404e-06, 9.04958779e-05, 1.96488968e-03, 2.31391008e-02, 1.65275818e-01, 3.87948034e-02, 4.69136431};    
float BB[6] = {-2.86180404e-08, -5.03733032e-06, -2.85386130e-04, -3.57590797e-03, 1.45259802e-01, 2.64048368};

// NSKモーター(右足)
/*
CC[10] = {2.08507152e-12, 6.21331094e-10, 7.55432406e-08, 4.78670255e-06, 1.66457257e-04, 3.02208174e-03, 2.36530654e-02, 4.83321692e-02, -2.21910362e-01, 1.14103111e+01};
DD[6] = {1.82279504e-07, 3.67404520e-05, 2.83278951e-03, 1.04788154e-01, 1.85489973, 1.23883044e+01};
*/

MCP_CAN CAN(SPI_CS_PIN); // set CS PIN
RMDx8Arduino rmd(CAN, MOTOR_ADDRESS); // インスタンスの作成

void setup()
{
  SERIAL.begin(115200);
  while (!Serial) ;

  rmd.canSetup();
  delay(1000);

  rmd.clearState(); // 初期化
  
  rmd.writePID(40, 40, 50, 40, 20, 20); // PID gainの設定
  delay(1000);

  rmd.serialWriteTerminator();

  timer[0] = millis(); // t0

  rmd.readPosition();
  ang[0] = rmd.present_position / 600; // 基準の角度 [deg]
}

void loop()
{
  while (exit_tf == false)
  {
    timer[1] = millis() - timer[0]; // 現在の時刻 [ms]

    rmd.readPosition(); // 現在の位置(角度)を取得
    ang[1] = (rmd.present_position / 600 - ang[0]) / gear_ratio; // モータ角度 [deg]
    
    // 振り出し,振り戻し切り替え
    if (ang[1] <= -70 && run_mode == 0)
    {
      run_mode = 1;
    }

    if (ang[1] >= -5 && run_mode == 1)
    {
      run_mode = 0;
    }

    // 目標トルク定義
    if (run_mode == 0) // 0:振り戻し
    {
      tgt_torq_1 = AA[0] * pow(ang[1],9) + AA[1] * pow(ang[1],8) + AA[2] * pow(ang[1],7) + AA[3] * pow(ang[1],6) + AA[4] * pow(ang[1],5) + AA[5] * pow(ang[1],4) + AA[6] * pow(ang[1],3) + AA[7] * ang[1] * ang[1] + AA[8] * ang[1] + AA[9];
      tgt_torq_1 = - rangeA * tgt_torq_1;
    }
    else if (run_mode == 1) // 1:振り出し
    {
      tgt_torq_1 = BB[0] * pow(ang[1],5) + BB[1] * pow(ang[1],4) + BB[2] * pow(ang[1],3) + BB[3] * ang[1] * ang[1] + BB[4] * ang[1] + BB[5];
      tgt_torq_1 = rangeB * tgt_torq_1 + 0.5;
    }

    // トルク制限（-1~16.8 [Nm]）
    if (tgt_torq_1 <= -16.8)
    {
      tgt_torq_1 = -16.8;
    }

    if (tgt_torq_1 >= 1)
    {
      tgt_torq_1 = 1;
    }

    // 角度制限（-75~0 [deg]）
    if (ang[1] < -85)
    {
      tgt_torq_1 = 0;
      exit_tf = true;
    }
    
    if (ang[1] > 3)
    {
      tgt_torq_1 = 0;
      exit_tf = true;
    }

    // 目標電流値（整数値） [-]
    tgt_cur = (2000 / 12.5 / torq_const) * tgt_torq_1;

    rmd.writeCurrent(tgt_cur); // 電流値指令 & 現在の温度、電流値、速度を取得

//    SERIAL.print(timer[1]);
//    SERIAL.print(",");
//    SERIAL.print(tgt_cur);
//    SERIAL.print(",");
//    SERIAL.print(ang[1]);
//    SERIAL.print(",");
//    SERIAL.println(run_mode);
//    
//    delay(100);

    SERIAL.print(timer[1]);
    SERIAL.print(",");
    SERIAL.print(tgt_cur);
    SERIAL.print(",");
    SERIAL.print(rmd.temperature);
    SERIAL.print(",");
    SERIAL.print(rmd.present_current);
    SERIAL.print(",");
    SERIAL.print(rmd.present_velocity);
    SERIAL.print(",");
    SERIAL.print(ang[1]);
    SERIAL.print(",");
    SERIAL.print(tgt_torq_1);
    SERIAL.print(",");
    SERIAL.println(run_mode);
  }

  rmd.clearState(); // 初期化

  while (true)
  {
    delay(1000);
  }

  
}
