// Define DEBUG to enable debugging; comment it out to disable
#define DEBUG

#ifdef DEBUG
  #define DEBUG_PRINT(...) Serial.print(__VA_ARGS__)
  #define DEBUG_PRINTLN(...) Serial.println(__VA_ARGS__)
  #define DEBUG_BEGIN(baud) Serial.begin(baud)
#else
  #define DEBUG_PRINT(...)    // Do nothing
  #define DEBUG_PRINTLN(...)  // Do nothing
  #define DEBUG_BEGIN(baud)   // Do nothing
#endif

#include "GlobalVariables.h"
#include "SerialData_Functions.h"
#include "oledDisplay.h"



void setup() {
  DEBUG_BEGIN(9600);
  StartSerial();
  oledBegin();
  
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  SerialReadData();
  serialWrite();
  displaySend();
}
