byte var;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
}

void loop() {
  var = Serial.read();

  if (var == '1'){
    digitalWrite(13,HIGH);
//    Serial.println("1");
  }
  else if (var == '2'){
    digitalWrite(13,LOW);
//    Serial.println("2");
  }
//  else if (var == '0'){
//    digitalWrite(13,LOW);
//    Serial.println("0");
//  }

}
