#include <SoftwareSerial.h>
SoftwareSerial nanoSerial(13, 15); // d7 RX, d8 TX

void StartSerial(){
  nanoSerial.begin(9600);
  Serial.println("Serial Communication Init..");
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
    Serial.println("\n==== TELEMETRY RECEIVED FROM ARDUINO ====");
    Serial.print("panelVoltage      : "); Serial.println(rx.panelVoltage);
    Serial.print("panelCurrent      : "); Serial.println(rx.panelCurrent);
    Serial.print("panelPower        : "); Serial.println(rx.panelPower);
    Serial.print("batteryPercent    : "); Serial.println(rx.batteryPercent);
    Serial.print("lampPower         : "); Serial.println(rx.lampPower);
    Serial.print("lampDimmer        : "); Serial.println(rx.lampDimmer);
    Serial.print("maxLDR            : "); Serial.println(rx.maxLDR);
    Serial.print("panelTemperature  : "); Serial.println(rx.panelTemperature);
    Serial.print("panDegreeRead     : "); Serial.println(rx.panDegreeRead);
    Serial.print("tiltDegreeRead    : "); Serial.println(rx.tiltDegreeRead);
  
    Serial.print("batteryVoltage    : "); Serial.println(rx.batteryVoltage);
    Serial.print("chargeStatus      : "); Serial.println(rx.chargeStatus);
    Serial.print("PJU1State         : "); Serial.println(rx.PJU1State);
    Serial.print("PJU2State         : "); Serial.println(rx.PJU2State);
    Serial.print("treeState         : "); Serial.println(rx.treeState);
  
    Serial.print("eventCode         : "); Serial.println(rx.eventCode);
  
    Serial.println("=========================================");
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
}

void serialWrite(){
  if (millis() - lastSend >= 200) {
    lastSend = millis();

    tx.panManual = 10;
    tx.tiltManual = -5;
    tx.overrideStat = 0;
    tx.PJU1Control = 1;
    tx.PJU2Control = 0;
    tx.treeControl = 0;

    sendControl();
    Serial.print(" || Data Sent to Arduino with size: ");
    Serial.println(sizeof(ControlPacket));

    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
}
