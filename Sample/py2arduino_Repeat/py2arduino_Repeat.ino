#include <SoftwareSerial.h>

SoftwareSerial pySerial(10, 11); // RX, TX
String str1, str2, str3, str4, str5, str6 = "";
uint32_t loopTime;
uint32_t serialTime;
uint32_t dt;

void setup() 
{
    Serial.begin(115200); // ハードウェアシリアルを準備
    while (!Serial) ; // wait for Arduino Serial Monitor to open

    pySerial.begin(57600); // ソフトウェアシリアルの初期化
}


void loop()
{
    // Serial.print("LoopTime: ");
    // Serial.print(dt);
    // Serial.println(" ms");
    
    // 送信側のsleepの量によっては通信可能
    // 結局どっちも間に合わないことがあるので、周期確認
    // 6つ受け取りはきつそう
    if (pySerial.available() > 0)
    {
        str1 = pySerial.readStringUntil('\1');
        str2 = pySerial.readStringUntil('\2');
        str3 = pySerial.readStringUntil('\3');
        // str4 = pySerial.readStringUntil('\4');
        // str5 = pySerial.readStringUntil('\5');
        // str6 = pySerial.readStringUntil('\6');

        Serial.print(str1);
        Serial.print(',');
        Serial.print(str2);
        Serial.print(',');
        Serial.print(str3);
        Serial.print(',');
        Serial.print(255);
        Serial.print(',');
        Serial.print(255);
        Serial.print(',');
        Serial.println(255);
    }

    while (millis() - loopTime < 4);   //4ms(250Hz)
    dt  = millis() - loopTime; //ms
    loopTime = millis();
}


// シリアル割り込み処理？
// ハードウェアシリアルに通信が来たときのみ？
// void serialEvent()
// {
//     if (Serial.available() > 0)
//     {
//         str1 = Serial.readStringUntil('\1');
//         str2 = Serial.readStringUntil('\2');
//         str3 = Serial.readStringUntil('\3');
//         str4 = Serial.readStringUntil('\4');
//         str5 = Serial.readStringUntil('\5');
//         str6 = Serial.readStringUntil('\6');
//     }
// }
