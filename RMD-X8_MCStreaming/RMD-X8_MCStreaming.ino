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

// Define Joystick connection pins
#define UP    A1
#define DOWN  A3
#define LEFT  A2
#define RIGHT A5
#define CLICK A4

#define StepValue 10

#define BAUDRATE 115200 //シリアル通信がボトルネックにならないよう，速めに設定しておく
#define LOOPTIME 5 //[ms] 

unsigned long timer[3], t;
bool exit_tf = false;
int32_t tgt_pos = 0;

const uint16_t MOTOR_ADDRESS = 0x141; //0x140 + ID(1~32)
const int SPI_CS_PIN = 10;

MCP_CAN CAN(SPI_CS_PIN); //set CS PIN
SoftwareSerial MCSerial(10, 11); // RX, TX

RMDx8Arduino rmd(CAN);

void setup()
{
  SERIAL.begin(BAUDRATE);
  while (!Serial) ;
  MCSerial.begin(57600);
  rmd.canSetup();
  delay(1000);

  rmd.clearState(MOTOR_ADDRESS);
  rmd.writePID(MOTOR_ADDRESS, 40, 100, 50, 40, 10, 50);

  delay(1000);

  // Initialize pins as necessary
  pinMode(UP, INPUT);
  pinMode(DOWN, INPUT);
  pinMode(CLICK, INPUT);

  // Pull analog pins high to enable reading of joystick movements
  digitalWrite(UP, HIGH);
  digitalWrite(DOWN, HIGH);
  digitalWrite(CLICK, HIGH);

  rmd.serialWriteTerminator();
  timer[0] = millis();
}

void loop()
{
  while (exit_tf==false)
  {
    timer[1] = millis();
    t = timer[1] - timer[0];

    if (digitalRead(CLICK)==LOW)
    {
      exit_tf = true;
    }

    if (pySerial.available() > 0)
    {
        String str_tgt_pos = pySerial.readStringUntil('\1');
        tgt_pos = str_tgt_pos.toInt();
    }

    rmd.writePosition(MOTOR_ADDRESS, tgt_pos);
  
    // SerialCommunication ---------------------
    SERIAL.print(t);
    SERIAL.print(",");
    SERIAL.print(tgt_pos);
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
    SERIAL.print(rmd.pos_buf[7]);

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

