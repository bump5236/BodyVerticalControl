#include <SoftwareSerial.h>

SoftwareSerial pySerial(10, 11); // RX, TX

String str1, str2, str3 = "";

void setup() 
{
    Serial.begin(115200); // ハードウェアシリアルを準備
    while (!Serial) ; // wait for Arduino Serial Monitor to open

    pySerial.begin(115200); // ソフトウェアシリアルの初期化
}


void loop()
{
    if (pySerial.available() > 0)
    {
        str1 = pySerial.readStringUntil('\1');
        str2 = pySerial.readStringUntil('\2');
        str3 = pySerial.readStringUntil('\3');
        Serial.print(str1);
        Serial.print('\t');
        Serial.print(str2);
        Serial.print('\t');
        Serial.print(str3);
        Serial.print('\n');
    }
}


// //シリアル割り込み処理
// void serialEvent()
// {

// }
