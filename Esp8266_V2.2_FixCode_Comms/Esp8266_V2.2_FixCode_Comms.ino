#include <Arduino.h>

// Define DEBUG to enable debugging; comment it out to disable
//#define DEBUG
//
//#ifdef DEBUG
//  #define Serial.print(...)   Serial.print(__VA_ARGS__)
//  #define Serial.println(...) Serial.println(__VA_ARGS__)
//  #define DEBUG_BEGIN(baud)  Serial.begin(baud)
//#else
//  #define Serial.print(...)    // Do nothing
//  #define Serial.println(...)  // Do nothing
//  #define DEBUG_BEGIN(baud)   // Do nothing
//#endif

#include "GlobalVariables.h"
#include "SerialData_Functions.h"
#include "oledDisplay.h"
#include "ServerData.h"

void setup() {
  Serial.begin(9600);
  StartSerial();
  oledBegin();
  startWifi();
  
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  SerialReadData();
  serialWrite();
  oledDataUpdate();

  if (millis() - lastSendTime >= 2000) {
    lastSendTime = millis();
    sendData();
  }
}
