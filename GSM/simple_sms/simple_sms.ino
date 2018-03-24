#include <SoftwareSerial.h>
SoftwareSerial gsmSerial(2, 3); // RX, TX

void initGsm(int baudRate) {
  gsmSerial.begin(baudRate);
  delay(1000);
  configGsmBaudRate(); // need this to set auto baud rate to the BAUDRATE value
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

int configGsmBaudRate() {
  String readStr;
  long iteration = 0, maxIteration = 10;

   while (iteration < maxIteration) {
    gsmSerial.println("AT");
    delay(1000);
    
    if (gsmSerial.available() > 0) {
      readStr = gsmSerial.readStringUntil('\n');
      
      if (strcmp(readStr.c_str(), "OK\r") == 0) {
        break;
      }
    }
    
    iteration++;
  }

  // add here second branch in case of fault
}

void deinitGsm() {
  Serial.println("deinit gsm");
  gsmSerial.println("AT+IPR=0");  //Set auto baud rate in GSM Module
  delay(1000);
}

const int BAUDRATE = 9600;

void setup() {
  Serial.begin(BAUDRATE);  //Set baud rate in debug connection with PC
  Serial.println("Start!");
  initGsm(BAUDRATE);
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
  delay(1000);
  
  sms(String("ty slowpoke\n-BulbaSat"), String("+375440000000"));
  deinitGsm();

  while (true) {
    if (gsmSerial.available())
      Serial.write(gsmSerial.read());
    if (Serial.available())
      gsmSerial.write(Serial.read());
  }
}
