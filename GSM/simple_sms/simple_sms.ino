#include <SoftwareSerial.h>
SoftwareSerial gsmSerial(2, 3); // RX, TX

void initGsm(int baudRate) {
  bool inited = false;
  long iteration = 0;
  long maxIteration = 5;

  while (!inited && iteration < maxIteration) {
    int oldBaudRate = bustBaudRate(); // need this to set auto baud rate to the BAUDRATE value
    gsmSerial.begin(oldBaudRate);
    delay(1000);
    gsmSerial.println("AT+IPR=" + String(baudRate));  //Set baud rate in GSM Module
    delay(1000);
    gsmSerial.begin(baudRate);
    delay(1000);
    inited = checkGsmStatus(baudRate); // Check Gsm Module Status
    delay(1000);
    ++iteration;
  }
  
  if (!inited) {
    Serial.println("SetBaudRateError: GSM haven't answered.");
    return;
  }
  
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

bool checkGsmStatus(const int baudRate) {
  gsmSerial.begin(baudRate);
  delay(1000);

  long maxIteration = 10;
  
  for (int i=0; i<maxIteration; ++i) {
    gsmSerial.println("AT");
    delay(1000);
    
    if (gsmSerial.available() > 0) {
      String readStr = gsmSerial.readStringUntil('\n');
      
      if (strcmp(readStr.c_str(), "OK\r") == 0) {
        return true;
      }
    }
  }
}

int bustBaudRate() {
  const int POSSIBLE_BAUD_RATES [] = {1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200};
  const int NUMBER_OF_BR = 8;
 
  for (int i=0; i < NUMBER_OF_BR; ++i) {
    int current_baud_rate = POSSIBLE_BAUD_RATES[i];
    bool ok_received = checkGsmStatus(current_baud_rate);
    
    Serial.println("Tried baudRate = ");
    Serial.println(String(current_baud_rate));
    Serial.println(", received ");
    
    if (ok_received) {
      Serial.println(" found ");
      return current_baud_rate;
    }
    else {
       Serial.println("not found ");
    }
  }
}

const int BAUD_RATE = 9600;

void setup() {
  Serial.begin(BAUD_RATE);  //Set baud rate in debug connection with PC
  Serial.println("Start!");
  initGsm(BAUD_RATE);
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
