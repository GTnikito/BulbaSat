#include <SoftwareSerial.h>
SoftwareSerial gsmSerial(2, 3); // RX, TX

void initGsm(int baudRate) {
  gsmSerial.begin(baudRate);
  delay(1000);
  gsmSerial.println("AT+IPR=" + String(baudRate));  //Set baud rate in GSM Module
  delay(1000);
  gsmSerial.println("AT"); //Check module status
  delay(1000);
  gsmSerial.println("AT+CMGF=1"); //Select SMS message format 
  delay(1000);
  gsmSerial.println("AT+CSCS=\"GSM\""); //Select TE Character Set
  delay(1000);
  gsmSerial.print((char)27);
  delay(1000);
}

void deinitGsm() {
  Serial.println("deinit gsm");
  gsmSerial.println("AT+IPR=0");  //Set auto baud rate in GSM Module
  delay(1000);
}

void setup() {
  Serial.begin(9600);  //Set baud rate in debug connection with PC
  Serial.println("Start!");
  initGsm(9600);
}

void sms(String text, String phone) {
  Serial.println("SMS send started");
  gsmSerial.println("AT+CMGS=\"" + phone + "\"");
  delay(1000);
  gsmSerial.print(text);
  delay(1000);
  gsmSerial.print((char)26);
  delay(1000);
  Serial.println("SMS send finish");
  delay(3000);
}

void loop() { 
    long k = 0;
    while (k < 1000000) {
      if (gsmSerial.available())
        Serial.write(gsmSerial.read());
      if (Serial.available())
        gsmSerial.write(Serial.read());
      k++;
    }

  sms(String("BulbaSat revival! 9600 baud rate. More more symbols, azaza"), String("+375291000000"));
  deinitGsm();

  while (true) {
    if (gsmSerial.available())
      Serial.write(gsmSerial.read());
    if (Serial.available())
      gsmSerial.write(Serial.read());
  }
}
