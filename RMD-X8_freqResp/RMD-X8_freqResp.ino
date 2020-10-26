#include <mcp_can.h>
#include <SPI.h>
#include <RMDx8Arduino.h>

/*SAMD core*/
#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
#define SERIAL serialUSB
#else
#define SERIAL Serial
#endif


unsigned long timer[3];
bool exit_tf = false;
float ang_1[2];
int16_t tgt;
int A = 2;
float f = 0.2;

const uint16_t MOTOR_ADDRESS_1 = 0x141; //0x140 + ID(1~32)
const int SPI_CS_PIN = 9;

MCP_CAN CAN(SPI_CS_PIN); //set CS PIN

RMDx8Arduino rmd1(CAN, MOTOR_ADDRESS_1);

void setup()
{
  SERIAL.begin(115200);
  while (!Serial) ;
  
  rmd1.canSetup();
  delay(1000);

  rmd1.clearState();
  
  rmd1.writePID(40, 40, 50, 40, 60, 20);
  delay(1000);

  rmd1.serialWriteTerminator();
  rmd1.readPosition();
  ang_1[0] = rmd1.present_position / 600; // 基準の角度 [deg]
}

void loop()
{
  for (int i = 0; i <= 12; i++)
  {
    rmd1.writeCurrent(0);
    delay(1500);
    timer[0] = millis();
    timer[1] = 0;
    while (timer[1] < 5000)
    {
      timer[1] = millis() - timer[0];
  
      rmd1.readPosition();
      ang_1[1] = rmd1.present_position / 600; // モータ角度 [deg]
      
      tgt = A * 2000 / 12.5 / 3.3 * sin(2 * 3.14 * f * pow(sqrt(2), i) * timer[1] * 0.001);
      
      rmd1.writeCurrent(tgt);
    
      // SerialCommunication ---------------------
      SERIAL.print(timer[1]);
      SERIAL.print(",");
      SERIAL.print(i);
      SERIAL.print(",");
      SERIAL.print(tgt);
      SERIAL.print(",");
      SERIAL.print(rmd1.present_current);
      SERIAL.print(",");
      SERIAL.print(rmd1.present_velocity);
      SERIAL.print(",");
      SERIAL.println(ang_1[1]);
      
      rmd1.serialWriteTerminator();
      // ------------------------------------------
    }
  }
  rmd1.clearState();
  delay(500);
  SERIAL.println("Program finish!");
  while (true)
  {
    delay(100);
  }
}
