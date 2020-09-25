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

unsigned long timer[3], t;
bool exit_tf = false;
int16_t tgt_cur, add_cur;
float euler_z;

/*
1:右モータ
2:左モータ
*/
const uint16_t MOTOR_ADDRESS_1 = 0x141; //0x140 + ID(1~32)
const uint16_t MOTOR_ADDRESS_2 = 0x142; //0x140 + ID(1~32)
const int SPI_CS_PIN = 9;

MCP_CAN CAN(SPI_CS_PIN); //set CS PIN
SoftwareSerial MCSerial(8, 9); // RX, TX

RMDx8Arduino rmd1(CAN, MOTOR_ADDRESS_1);
RMDx8Arduino rmd2(CAN, MOTOR_ADDRESS_2);

void setup()
{
  SERIAL.begin(115200);
  while (!Serial) ;

  MCSerial.begin(57600);
  
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
    timer[1] = millis();
    t = timer[1] - timer[0];

     if (MCSerial.available() > 0)
     {
         String str_euler_z = MCSerial.readStringUntil('\1');
         euler_z = str_euler_z.toFloat();

         String str_add_cur = MCSerial.readStringUntil('\2');
         add_cur_1 = str_add_cur.toInt();
         add_cur_2 = str_add_cur.toInt();
     }

    int16_t A = 9/3.3*2000/12.5;
    int16_t base_cur_1 = A * cos(2 * 3.14 * 0.7 * (timer[1] - timer[0]) * 0.001);
    int16_t base_cur_2 = A * sin(2 * 3.14 * 0.7 * (timer[1] - timer[0]) * 0.001);

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

    tgt_cur_1 = base_cur_1 + add_cur_1;
    tgt_cur_2 = base_cur_2 + add_cur_2;

    // 制限
    if (tgt_cur_1 > 500)
    {
        tgt_cur_1 = 500;
    }

    else if (tgt_cur_1 < -500)
    {
        tgt_cur_1 = -500;
    }

    // 制限
    if (tgt_cur_2 > 500)
    {
        tgt_cur_3 = 500;
    }

    else if (tgt_cur_2 < -500)
    {
        tgt_cur_2 = -500;
    }

    rmd1.writeCurrent(tgt_cur_1);
    rmd1.readPosition();

    rmd2.writeCurrent(tgt_cur_2);
    rmd2.readPosition();
  
    // SerialCommunication ---------------------
    SERIAL.print(t);
    SERIAL.print(",");
    SERIAL.print(euler_z);
    SERIAL.print(",");
    SERIAL.print(rmd1.temperature);
    SERIAL.print(",");
    SERIAL.print(add_cur_1);
    SERIAL.print(",");
    SERIAL.print(tgt_cur_1);
    SERIAL.print(",");
    SERIAL.print(rmd1.current);
    SERIAL.print(",");
    SERIAL.print(rmd1.velocity);
    SERIAL.print(",");
    SERIAL.print(rmd1.position);
    SERIAL.print(",");
    
    SERIAL.print(rmd2.temperature);
    SERIAL.print(",");
    SERIAL.print(add_cur_2);
    SERIAL.print(",");
    SERIAL.print(tgt_cur_2);
    SERIAL.print(",");
    SERIAL.print(rmd2.current);
    SERIAL.print(",");
    SERIAL.print(rmd2.velocity);
    SERIAL.print(",");
    SERIAL.println(rmd2.position);

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
