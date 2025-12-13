#include "SolarTracker.h"
#include "VoltCurrent.h"
#include "LampControl.h"

static unsigned long lastPrintTime = 0;

void setup() {
  Serial.begin(9600);
  ServoSetup();
  inputSetup();
  outputSetup();
}

void loop() {
  updateSolarTracker();
  fadeLED(PJU1, PJU1Stat);
  fadeLED(PJU2, PJU2Stat);
  updateTreeEffect();
  
  if (millis() - lastPrintTime >= 500) {
    panelCurrent();
    panelVoltage();
    batteryVoltage();
    readLM35();
    lastPrintTime = millis();
  }
}
