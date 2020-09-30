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
#define DIGITAL_INPUT_SYNC 4

unsigned long timer[3], t;
bool exit_tf = false;

float Kp = 0.03
int16_t tgt_cur_1, tgt_cur_2, add_cur_1, add_cur_2;
int16_t euler_z;

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
  
  rmd1.writePID(40, 40, 50, 40, 10, 20);
  rmd2.writePID(40, 40, 50, 40, 10, 20);

  delay(1000);

  rmd1.serialWriteTerminator();
  // rmd2.serialWriteTerminator();
  timer[0] = millis();
}

void loop()
{
  while (exit_tf==false)
  {
//     int sync_flag = digitalRead(DIGITAL_INPUT_SYNC);
    uint8_t sync_flag = PIND & _BV(4);

    timer[1] = millis();
    t = timer[1] - timer[0];

    int16_t A = 9*2000/12.5/3.3;
    int16_t base_cur_1 = A * cos(2 * 3.14 * 0.7 * (timer[1] - timer[0]) * 0.001);
    int16_t base_cur_2 = A * cos(2 * 3.14 * 0.7 * (timer[1] - timer[0]) * 0.001);

    // 振り出し
    if (base_cur_1 < 0)
    {
      base_cur_1 = base_cur_1*0.05;
      add_cur_1 = 0;
    }
    
    if (base_cur_2 > 0)
    {
      base_cur_2 = base_cur_2*0.05;
      add_cur_2 = 0;
    }

    // 振り戻し
    if (base_cur_1 > 350)
    {
      add_cur_1 = 100;
    }
    
    if (base_cur_2 < -350)
    {
      add_cur_2 = 100;
    }

    tgt_cur_1 = base_cur_1 + add_cur_1;
    tgt_cur_2 = base_cur_2 + add_cur_2;

    // 制限
    if (tgt_cur_1 > 600)
    {
        tgt_cur_1 = 600;
    }

    else if (tgt_cur_1 < -600)
    {
        tgt_cur_1 = -600;
    }

    // 制限
    if (tgt_cur_2 > 600)
    {
        tgt_cur_2 = 600;
    }

    else if (tgt_cur_2 < -600)
    {
        tgt_cur_2 = -600;
    }

    rmd1.writeCurrent(tgt_cur_1);
    rmd1.readPosition();

    rmd2.writeCurrent(tgt_cur_2);
    rmd2.readPosition();
  
    // SerialCommunication ---------------------
    SERIAL.print(t);
    SERIAL.print(",");
    SERIAL.print(sync_flag);
    SERIAL.print(",");
    SERIAL.print(euler_z);
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
    SERIAL.print(rmd1.present_position);
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
    SERIAL.println(rmd2.present_position);

//    SERIAL.print(",");
//    SERIAL.print(tgt_cur_1);
//    SERIAL.print(",");
//    SERIAL.println(tgt_cur_2);

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
