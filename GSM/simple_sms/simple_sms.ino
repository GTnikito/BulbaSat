#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // RX, TX

void setup() {
  Serial.begin(1200);  //Скорость порта для связи Arduino с компьютером
  Serial.println("Start!");
  mySerial.begin(115200);  //Скорость порта для связи Arduino с GSM модулем
  mySerial.println("AT+IPR=1200");
  mySerial.begin(1200);
  delay(1000);
  mySerial.println("AT");
  delay(1000);
  mySerial.println("AT+CMGF=1");
  delay(1000);
  mySerial.println("AT+CSCS=\"GSM\"");
  delay(1000);
  mySerial.print((char)27);
}

void sms(String text, String phone) {
  Serial.println("SMS send started");
  mySerial.println("AT+CMGS=\"" + phone + "\"");
  delay(1000);
  mySerial.print(text);
  delay(1000);
  mySerial.print((char)26);
  delay(1000);
  Serial.println("SMS send finish");
  delay(3000);
}

void loop() {    
    long k = 0;
    while (k < 1000000) {
      if (mySerial.available())
        Serial.write(mySerial.read());
      if (Serial.available())
        mySerial.write(Serial.read());
      k++;
    }
  sms(String("Hey Mottafuccka, it is me, BulbaSat!"),String("+375296289065"));

  while (true) {
    if (mySerial.available())
      Serial.write(mySerial.read());
    if (Serial.available())
      mySerial.write(Serial.read());
  }
}
