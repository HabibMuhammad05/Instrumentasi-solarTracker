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

#include "GlobalVariables.h"
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
  if(rx.overrideStatUpdated) updateSolarTracker();
  else trackerManual(); 
  
  fadeLED(PJU1, PJU1Stat);
  fadeLED(PJU2, PJU2Stat);
  updateTreeEffect();
  serialDataRead();
  
  if (millis() - lastPrintTime >= 1000) {
    panelCurrent();
    panelVoltage();
    batteryVoltage();
    readLM35();
    serialWrite();
    if(rx.overrideStat) DEBUG_PRINTLN("AUTO MODE");
    else DEBUG_PRINTLN("MANUAL MODE");
    lastPrintTime = millis();
  }
}
