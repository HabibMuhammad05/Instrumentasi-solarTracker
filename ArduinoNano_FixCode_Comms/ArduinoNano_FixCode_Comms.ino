// Define DEBUG to enable debugging; comment it out to disable
//#define DEBUG

#ifdef DEBUG
  #define DEBUG_PRINT(...) Serial.print(__VA_ARGS__)
  #define DEBUG_PRINTLN(...) Serial.println(__VA_ARGS__)
  #define DEBUG_BEGIN(baud) Serial.begin(baud)
#else
  #define DEBUG_PRINT(...)    // Do nothing
  #define DEBUG_PRINTLN(...)  // Do nothing
  #define DEBUG_BEGIN(baud)   // Do nothing
#endif

#include "SolarTracker.h"
#include "VoltCurrent.h"
#include "LampControl.h"
#include "SerialDataComms.h"

static unsigned long lastPrintTime = 0;

void setup() {
  DEBUG_BEGIN(9600);
  startSerial();
  ServoSetup();
  inputSetup();
  outputSetup();
}

void loop() {
  updateSolarTracker();
  fadeLED(PJU1, PJU1Stat);
  fadeLED(PJU2, PJU2Stat);
  updateTreeEffect();
  serialDataRead();
  
  if (millis() - lastPrintTime >= 150) {
    panelCurrent();
    panelVoltage();
    batteryVoltage();
    readLM35();
    serialWrite();
    lastPrintTime = millis();
  }
}
