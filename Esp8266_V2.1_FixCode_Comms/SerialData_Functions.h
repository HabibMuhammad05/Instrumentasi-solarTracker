#include <SoftwareSerial.h>
SoftwareSerial nanoSerial(13, 15); // d7 RX, d8 TX

void StartSerial(){
  nanoSerial.begin(9600);
  DEBUG_PRINTLN("Serial Communication Init..");
}

void sendControl() {
  uint8_t *buf = (uint8_t*)&tx;
  uint8_t len = sizeof(ControlPacket);
  uint8_t checksum = 0;

  nanoSerial.write(0xAA);
  nanoSerial.write(0x55);
  nanoSerial.write(len);

  for (uint8_t i = 0; i < len; i++) {
    nanoSerial.write(buf[i]);
    checksum ^= buf[i];
  }

  nanoSerial.write(checksum);
}

bool readTelemetry() {
  static uint8_t state = 0;
  static uint8_t len = 0;
  static uint8_t index = 0;
  static uint8_t checksum = 0;
  static uint8_t buf[64];

  while (nanoSerial.available()) {
    uint8_t b = nanoSerial.read();

    switch (state) {
      case 0:
        if (b == 0xAA) state = 1;
        break;
      case 1:
        if (b == 0x55) state = 2;
        else state = 0;
        break;
      case 2:
        len = b;
        if (len > 64) { state = 0; break; }
        index = 0;
        checksum = 0;
        state = 3;
        break;
      case 3:
        buf[index++] = b;
        checksum ^= b;
        if (index >= len) state = 4;
        break;
      case 4:
        if (checksum == b) {
          memcpy(&rx, buf, len);
          state = 0;
          return true;
        }
        state = 0;
        break;
    }
  }
  return false;
}

void SerialReadData(){
  if (readTelemetry()) {
    DEBUG_PRINTLN("\n==== TELEMETRY RECEIVED FROM ARDUINO ====");
//    DEBUG_PRINT("panelVoltage      : "); DEBUG_PRINTLN(rx.panelVoltage, 4);
//    DEBUG_PRINT("panelCurrent      : "); DEBUG_PRINTLN(rx.panelCurrent, 4);
//    DEBUG_PRINT("panelPower        : "); DEBUG_PRINTLN(rx.panelPower, 4);
//    DEBUG_PRINT("batteryPercent    : "); DEBUG_PRINTLN(rx.batteryPercent, 2);
//    DEBUG_PRINT("lampPower         : "); DEBUG_PRINTLN(rx.lampPower, 2);
//    DEBUG_PRINT("lampDimmer        : "); DEBUG_PRINTLN(rx.lampDimmer);
//    DEBUG_PRINT("maxLDR            : "); DEBUG_PRINTLN(rx.maxLDR, 2);
//    DEBUG_PRINT("panelTemperature  : "); DEBUG_PRINTLN(rx.panelTemperature, 2);
//    DEBUG_PRINT("panDegreeRead     : "); DEBUG_PRINTLN(rx.panDegreeRead);
//    DEBUG_PRINT("tiltDegreeRead    : "); DEBUG_PRINTLN(rx.tiltDegreeRead);
//  
//    DEBUG_PRINT("batteryVoltage    : "); DEBUG_PRINTLN(rx.batteryVoltage, 4);
//    DEBUG_PRINT("chargeStatus      : "); DEBUG_PRINTLN(rx.chargeStatus);
//    DEBUG_PRINT("PJU1State         : "); DEBUG_PRINTLN(rx.PJU1State);
//    DEBUG_PRINT("PJU2State         : "); DEBUG_PRINTLN(rx.PJU2State);
//    DEBUG_PRINT("treeState         : "); DEBUG_PRINTLN(rx.treeState);
//  
//    DEBUG_PRINT("eventCode         : "); DEBUG_PRINTLN(rx.eventCode);
  
    DEBUG_PRINTLN("=========================================");
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
}

void serialWrite(){
  int delaytime;
  if(tx.overrideStat) delaytime = 1000;
  else delaytime = 100;
  
  if (millis() - lastSend >= delaytime) {
//  if (millis() - lastSend >= 800) {
    lastSend = millis();

//    tx.panManual = 10;
//    tx.tiltManual = -5;
//    tx.overrideStat = 0;
//    tx.PJU1Control = 1;
//    tx.PJU2Control = 0;
//    tx.treeControl = 0;

    sendControl();

    DEBUG_PRINT("tx.panManual      : "); DEBUG_PRINTLN(tx.panManual);
    DEBUG_PRINT("tx.tiltManual     : "); DEBUG_PRINTLN(tx.tiltManual);
    DEBUG_PRINT("tx.overrideStat   : "); DEBUG_PRINTLN(tx.overrideStat);
    DEBUG_PRINT("tx.PJU1Control    : "); DEBUG_PRINTLN(tx.PJU1Control);
    DEBUG_PRINT("tx.PJU2Control    : "); DEBUG_PRINTLN(tx.PJU2Control);
    DEBUG_PRINT("tx.treeControl    : "); DEBUG_PRINTLN(tx.treeControl);
    DEBUG_PRINT("overrideStatUpdate: "); DEBUG_PRINTLN(tx.overrideStatUpdated);
    DEBUG_PRINT(" || Data Sent to Arduino with size: ");
    DEBUG_PRINTLN(sizeof(ControlPacket));
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
}
