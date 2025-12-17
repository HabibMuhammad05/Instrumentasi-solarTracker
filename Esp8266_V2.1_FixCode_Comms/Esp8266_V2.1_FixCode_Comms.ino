#include <Arduino.h>

// Define DEBUG to enable debugging; comment it out to disable
#define DEBUG

#ifdef DEBUG
  #define DEBUG_PRINT(...) Serial.print(__VA_ARGS__)
  #define DEBUG_PRINTLN(...) Serial.println(__VA_ARGS__)
  #define DEBUG_BEGIN(baud) Serial.begin(baud)
  #define BLYNK_PRINT Serial
#else
  #define DEBUG_PRINT(...)    // Do nothing
  #define DEBUG_PRINTLN(...)  // Do nothing
  #define DEBUG_BEGIN(baud)   // Do nothing
#endif

#include "GlobalVariables.h"
#include "SerialData_Functions.h"
#include "oledDisplay.h"
#include "ServerData.h"
#include "BlynkFunctions.h"

void setup() {
  DEBUG_BEGIN(9600);
  StartSerial();
  oledBegin();
  handleWiFi();
//  blynkSetup();
//  startWifi();
  
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  handleWiFi();
  SerialReadData();
  serialWrite();
  oledDataUpdate();
//  sendDataNonBlocking();
  Blynk.run();


  if (millis() - lastSendTime >= 2000) {
    lastSendTime = millis();
    blynkSend();

    sendTelemetryData();
    getControlStatus();
    
  }
}


//    sendData();
//    startSendData();
