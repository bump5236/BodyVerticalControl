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

#define LOOPTIME 5 //[ms] 

unsigned long timer[3];
unsigned long cnt = 1;
bool exit_tf = false;

int16_t tgt_cur_1, tgt_cur_2, add_cur_1, add_cur_2;
float ang_1[2], ang_2[2];

int8_t mode_1 = 0; // 0:振り戻し, 1:振り出し, 2:, 3:?
int8_t mode_2 = 0; // 0:振り戻し, 1:振り出し, 2:, 3:?

int16_t max_torq = 13;
int16_t min_torq = 1;
int8_t max_ang = 80;
uint8_t min_ang = -5;
float f = 0.77;

/*
1:右モータ
2:左モータ
*/
const uint16_t MOTOR_ADDRESS_1 = 0x141; //0x140 + ID(1~32)
const uint16_t MOTOR_ADDRESS_2 = 0x142; //0x140 + ID(1~32)
const int SPI_CS_PIN = 9;

MCP_CAN CAN(SPI_CS_PIN); //set CS PIN

RMDx8Arduino rmd1(CAN, MOTOR_ADDRESS_1);
RMDx8Arduino rmd2(CAN, MOTOR_ADDRESS_2);

void setup()
{
  SERIAL.begin(115200);
  while (!Serial) ;
  
  pinMode(4, INPUT);

  rmd1.canSetup();
  rmd2.canSetup();
  delay(1000);

  rmd1.clearState();
  rmd2.clearState();
  
  rmd1.writePID(40, 40, 50, 40, 20, 250);
  rmd2.writePID(40, 40, 50, 40, 20, 250);

  delay(1000);

  rmd1.serialWriteTerminator();
  // rmd2.serialWriteTerminator();
  timer[0] = millis();
  rmd1.readPosition();
  rmd2.readPosition();
  ang_1[0] = rmd1.present_position / 600; // 基準の角度 [deg]
  ang_2[0] = rmd2.present_position / 600; // 基準の角度 [deg]
}

void loop()
{
  while (exit_tf==false)
  {
    uint8_t sync_flag = PIND & _BV(2);

    timer[1] = millis() - timer[0];
    if ((timer[1]*0.001) > (1/f * cnt))
    {
      cnt = cnt + 1;
    }

    rmd1.readPosition();
    rmd2.readPosition();
    ang_1[1] = rmd1.present_position / 600 - ang_1[0]; // モータ角度 [deg]
    ang_2[1] = rmd2.present_position / 600 - ang_2[0]; // モータ角度 [deg]

    int16_t A = 4 * 2000 / 12.5 / 3.3;
    int16_t base_cur_1 = A * cos(2 * 3.14 * f * timer[1] * 0.001);
    int16_t base_cur_2 = A * cos(2 * 3.14 * f * timer[1] * 0.001);

    // モード判定
    if (base_cur_1 > 0)
    {
      mode_1 = 0;
    }

    else if (base_cur_1 < 0)
    {
      mode_1 = 1;
    }

    if (base_cur_2 < 0)
    {
      mode_2 = 0;
    }

    else if (base_cur_2 > 0)
    {
      mode_2 = 1;
    }


    // 振り戻し
    if (mode_1 == 0)
    {
      add_cur_1 = 0.73 * ang_1[1] + 131;
    }
    // 振り出し
    else if (mode_1 == 1)
    {
      base_cur_1 = base_cur_1*0.02;
      add_cur_1 = 0;
    }
    
    // 振り戻し
    if (mode_2 == 0)
    {
      // if (ang_2[1] < -60) {add_cur_2 = -150;}
      add_cur_2 = 0.73 * ang_2[1] - 131;
    }
    // 振り出し
    else if (mode_2 == 1)
    {
      base_cur_2 = base_cur_2*0.02;
      add_cur_2 = 0;
    }

    tgt_cur_1 = base_cur_1 + add_cur_1;
    tgt_cur_2 = base_cur_2 + add_cur_2;

    // 制限
    if (tgt_cur_1 > 700)
    {
        tgt_cur_1 = 700;
    }

    else if (tgt_cur_1 < -700)
    {
        tgt_cur_1 = -700;
    }

    // 制限
    if (tgt_cur_2 > 700)
    {
        tgt_cur_2 = 700;
    }

    else if (tgt_cur_2 < -700)
    {
        tgt_cur_2 = -700;
    }

    rmd1.writeCurrent(tgt_cur_1);
    rmd2.writeCurrent(tgt_cur_2);
  
    // SerialCommunication ---------------------
    SERIAL.print(timer[1]);
    SERIAL.print(",");
    SERIAL.print(sync_flag);
    SERIAL.print(",");
    SERIAL.print(rmd1.temperature);
    SERIAL.print(",");
    SERIAL.print(add_cur_1);
    SERIAL.print(",");
    SERIAL.print(tgt_cur_1);
    SERIAL.print(",");
    SERIAL.print(rmd1.present_current);
    SERIAL.print(",");
    SERIAL.print(rmd1.present_velocity);
    SERIAL.print(",");
    SERIAL.print(ang_1[1]);
    SERIAL.print(",");
    
    SERIAL.print(rmd2.temperature);
    SERIAL.print(",");
    SERIAL.print(add_cur_2);
    SERIAL.print(",");
    SERIAL.print(tgt_cur_2);
    SERIAL.print(",");
    SERIAL.print(rmd2.present_current);
    SERIAL.print(",");
    SERIAL.print(rmd2.present_velocity);
    SERIAL.print(",");
    SERIAL.println(ang_2[1]);

    // SERIAL.print(",");
    // SERIAL.print(cnt*100);
    // SERIAL.print(",");
    // SERIAL.println(base_cur_1);

    rmd1.serialWriteTerminator();
    // ------------------------------------------
    
    timer[2] = millis() - timer[1];
    if (timer[2] < LOOPTIME)
    {
      delay(LOOPTIME - timer[2]);
    }
  }

  rmd1.clearState();
  rmd2.clearState();
  delay(500);
  SERIAL.println("Program finish!");
  while (true)
  {
    delay(100);
  }
}
