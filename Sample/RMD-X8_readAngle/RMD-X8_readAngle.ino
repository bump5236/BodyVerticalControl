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
const int SPI_CS_PIN = 9;

MCP_CAN CAN(SPI_CS_PIN); //set CS PIN
SoftwareSerial MCSerial(8, 9); // RX, TX

RMDx8Arduino rmd1(CAN, MOTOR_ADDRESS_1);

void setup()
{
  SERIAL.begin(115200);
  while (!Serial) ;

  MCSerial.begin(57600);
  
  rmd1.canSetup();
  delay(1000);

  rmd1.clearState();
  rmd1.writePID(40, 40, 50, 40, 10, 20);
  delay(1000);

  rmd1.serialWriteTerminator();
  timer[0] = millis();
}

void loop()
{
  rmd1.readAngle(1);
  long long angle =rmd1.reply_buf[3] << 24 + rmd1.reply_buf[2] << 16 + rmd1.reply_buf[1] << 8 + rmd1.reply_buf[0];

  if (angle > 2147483648)
  {
    angle = angle - 4294967296;
  }
  
  SERIAL.print(angle);
}