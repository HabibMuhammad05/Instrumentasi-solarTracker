#include "GlobalVariables.h"
#include "SerialData_Functions.h"



void setup() {
  Serial.begin(9600);
  StartSerial();
  
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  SerialRW();
}
