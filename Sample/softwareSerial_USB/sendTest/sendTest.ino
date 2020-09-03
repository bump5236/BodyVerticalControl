#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX

void setup() {
  Serial.begin(57600); // ハードウェアシリアルを準備
  while (!Serial) {
    ; // シリアルポートの準備ができるのを待つ(Leonardoのみ必要)
  }
  Serial.println("Ready");
  mySerial.begin(4800); // ソフトウェアシリアルの初期化
  mySerial.println("Hello, world?");
}

void loop() {
  if (mySerial.available()) Serial.write(mySerial.read());
  if (Serial.available()) mySerial.write(Serial.read());
}
