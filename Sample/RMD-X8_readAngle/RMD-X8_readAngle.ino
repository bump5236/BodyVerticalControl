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

RMDx8Arduino rmd1(CAN, MOTOR_ADDRESS_1);
RMDx8Arduino rmd2(CAN, MOTOR_ADDRESS_2);

void setup()
{
  SERIAL.begin(115200);
  while (!Serial) ;
  
  rmd1.canSetup();
  rmd2.canSetup();
  delay(1000);

  rmd1.clearState();
  rmd2.clearState();
  rmd1.writePID(40, 40, 50, 40, 10, 20);
  rmd2.writePID(40, 40, 50, 40, 10, 20);
  delay(1000);

  rmd1.serialWriteTerminator();
  timer[0] = millis();
}

uint32_t angle;
int32_t angle1, angle2;

void loop()
{
  rmd1.writeCurrent(0);
  rmd2.writeCurrent(0);
  rmd1.readAngle(1);
  rmd2.readAngle(1);
  angle = ((uint32_t)rmd1.pos_buf[4] << 24) + ((uint32_t)rmd1.pos_buf[3] << 16) + ((uint32_t)rmd1.pos_buf[2] << 8) + rmd1.pos_buf[1];
  if (angle > 2147483648)
  {
    angle1 = angle - 4294967296;
  }
  else
  {
    angle1 = angle;
  }

  
  angle = ((uint32_t)rmd2.pos_buf[4] << 24) + ((uint32_t)rmd2.pos_buf[3] << 16) + ((uint32_t)rmd2.pos_buf[2] << 8) + rmd2.pos_buf[1];
  if (angle > 2147483648)
  {
    angle2 = angle - 4294967296;
  }
  else
  {
    angle2 = angle;
  }
  
   SERIAL.print(angle1/600);
   SERIAL.print(",");
   SERIAL.print(angle2/600);
   SERIAL.println("");


//   SERIAL.print(rmd1.pos_buf[0]);
//   SERIAL.print(",");
//   SERIAL.print(rmd1.pos_buf[1]);
//   SERIAL.print(",");
//   SERIAL.print((unsigned long)rmd1.pos_buf[2] << 8);
//   SERIAL.print(",");
//   SERIAL.print((unsigned long)rmd1.pos_buf[3] << 16);
//   SERIAL.print(",");
//   SERIAL.print((unsigned long)rmd1.pos_buf[4] << 24);
//   SERIAL.println("");
}
