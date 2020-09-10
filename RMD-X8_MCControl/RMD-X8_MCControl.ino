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

#define StepValue 10

#define BAUDRATE 115200 //シリアル通信がボトルネックにならないよう，速めに設定しておく
#define LOOPTIME 5 //[ms] 

unsigned long timer[3], t;
bool exit_tf = false;
int16_t tgt_cur, add_cur;
float euler_z;

const uint16_t MOTOR_ADDRESS = 0x141; //0x140 + ID(1~32)
const int SPI_CS_PIN = 9;

MCP_CAN CAN(SPI_CS_PIN); //set CS PIN
SoftwareSerial MCSerial(8, 9); // RX, TX

RMDx8Arduino rmd(CAN);

void setup()
{
  SERIAL.begin(BAUDRATE);
  while (!Serial) ;

  MCSerial.begin(57600);
  
  rmd.canSetup();
  delay(1000);

  rmd.clearState(MOTOR_ADDRESS);
  rmd.writePID(MOTOR_ADDRESS, 40, 40, 50, 40, 10, 20);

  delay(1000);

  rmd.serialWriteTerminator();
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
         add_cur = str_add_cur.toInt();
     }

    int16_t A = 10/3.3*2000/12.5;
  //  tgt_cur = A * sin(2 * 3.14 * 0.7 * (timer[1] - timer[0]) * 0.001);

    int16_t base_cur = A * sin(2 * 3.14 * 0.7 * (timer[1] - timer[0]) * 0.001);

    tgt_cur = base_cur + add_cur;

    if (tgt_cur > 400)
    {
        tgt_cur = 400;
    }

    else if (tgt_cur < -400)
    {
        tgt_cur = -400;
    }
    rmd.writeCurrent(MOTOR_ADDRESS, tgt_cur);
  
    // SerialCommunication ---------------------
    SERIAL.print(t);
    SERIAL.print(",");
    SERIAL.print(euler_z);
    SERIAL.print(",");
    SERIAL.print(add_cur);
    SERIAL.print(",");
    SERIAL.print(tgt_cur);
    SERIAL.print(",");
    SERIAL.print(rmd.reply_buf[0]);
    SERIAL.print(",");
    SERIAL.print(rmd.reply_buf[1]);
    SERIAL.print(",");
    SERIAL.print(rmd.reply_buf[2]);
    SERIAL.print(",");
    SERIAL.print(rmd.reply_buf[3]);
    SERIAL.print(",");
    SERIAL.print(rmd.reply_buf[4]);
    SERIAL.print(",");
    SERIAL.print(rmd.reply_buf[5]);
    SERIAL.print(",");
    SERIAL.print(rmd.reply_buf[6]);
    SERIAL.print(",");
    SERIAL.print(rmd.reply_buf[7]);
    SERIAL.print(",");
    SERIAL.print(rmd.pos_buf[0]);
    SERIAL.print(",");
    SERIAL.print(rmd.pos_buf[1]);
    SERIAL.print(",");
    SERIAL.print(rmd.pos_buf[2]);
    SERIAL.print(",");
    SERIAL.print(rmd.pos_buf[3]);
    SERIAL.print(",");
    SERIAL.print(rmd.pos_buf[4]);
    SERIAL.print(",");
    SERIAL.print(rmd.pos_buf[5]);
    SERIAL.print(",");
    SERIAL.print(rmd.pos_buf[6]);
    SERIAL.print(",");
    SERIAL.println(rmd.pos_buf[7]);

    rmd.serialWriteTerminator();
    // ------------------------------------------
    
    timer[2] = millis() - timer[1];
    if (timer[2] < LOOPTIME)
    {
      delay(LOOPTIME - timer[2]);
    }
  }

  rmd.clearState(MOTOR_ADDRESS);
  delay(500);
  SERIAL.println("Program finish!");
  while (true)
  {
    delay(100);
  }
}
