#include <SoftwareSerial.h>

//RX pino 2, TX pino 3
SoftwareSerial esp8266(2, 3);
int incomingByte = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  esp8266.begin(19200);
}

void loop() {
  if (esp8266.available())
  {
    incomingByte = esp8266.read();
    Serial.write(incomingByte);
  }
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    esp8266.write(incomingByte);
  }
}
