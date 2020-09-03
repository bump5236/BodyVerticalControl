/*
* メインループと送信のループの周期を変えたいとき
* ループ処理を250Hzで行って、およそ50Hz周期（20ms毎）で64byte分のシリアル送信
*/

uint32_t loopTime;
uint32_t serialTime;
uint32_t dt;

void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.print("LoopTime: ");
  Serial.print(dt);
  Serial.println(" us");

  if (micros() - serialTime > 20000) {
    serialTime = micros();
    for (int i = 0 ; i < 64 ; i++)Serial.print('A');
    Serial.println();
  }

  while (micros() - loopTime < 4000);   //4000us(250Hz)
  dt  = micros() - loopTime; //us
  loopTime = micros();
}