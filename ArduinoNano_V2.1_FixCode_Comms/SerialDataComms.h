#include <SoftwareSerial.h>
SoftwareSerial espSerial(8, 7); // RX, TX

struct TelemetryPacket {
  float panelVoltage;
  float panelCurrent;
  float panelPower;
  float batteryPercent;
  float lampPower;
  uint8_t lampDimmer;
  float maxLDR;
  float panelTemperature;
  int16_t panDegreeRead;
  int16_t tiltDegreeRead;

  float batteryVoltage;
  uint8_t chargeStatus;
  uint8_t PJU1State;
  uint8_t PJU2State;
  uint8_t treeState;

  uint8_t eventCode;
} __attribute__((packed));

TelemetryPacket tx;

struct ControlPacket {
  int16_t panManual;
  int16_t tiltManual;

  uint8_t overrideStat = false;
  uint8_t PJU1Control;
  uint8_t PJU2Control;
  uint8_t treeControl;
} __attribute__((packed));

ControlPacket rx;


void startSerial(){
  espSerial.begin(9600);
  DEBUG_PRINTLN("Serial Communication Init..");
}

void sendTelemetry() {
  uint8_t *data = (uint8_t*)&tx;
  uint8_t len = sizeof(TelemetryPacket);
  uint8_t checksum = 0;

  espSerial.write(0xAA);
  espSerial.write(0x55);
  espSerial.write(len);

  for (uint8_t i = 0; i < len; i++) {
    espSerial.write(data[i]);
    checksum ^= data[i];
  }

  espSerial.write(checksum);
}

bool readControl() {
  static uint8_t state = 0;
  static uint8_t len = 0;
  static uint8_t index = 0;
  static uint8_t checksum = 0;
  static uint8_t buf[32];

  while (espSerial.available()) {
    uint8_t b = espSerial.read();

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
        if (len > 32) { state = 0; break; }
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

void serialWrite(){
    tx.panelVoltage = panelVoltVal;
    tx.panelCurrent = currentVal;
    tx.panelPower = powerVal;
    tx.batteryPercent = battPerc;
    tx.lampPower = powerVal;
    tx.lampDimmer = lampDimmingval;
    tx.maxLDR = peakLDR;
    tx.panelTemperature = tempC;
    tx.panDegreeRead = panPos;
    tx.tiltDegreeRead = tiltPos;
    
    tx.batteryVoltage = battVal;
    tx.chargeStatus = powerValState;
    tx.PJU1State = PJU1Stat;
    tx.PJU2State = PJU2Stat;
    tx.treeState = treeEffect;
    tx.eventCode = 4;

    sendTelemetry();  
    
    DEBUG_PRINTLN("\n==== TELEMETRY SENT TO ESP ====");

    DEBUG_PRINT("panelVoltage      : "); DEBUG_PRINTLN(tx.panelVoltage, 4);
    DEBUG_PRINT("panelCurrent      : "); DEBUG_PRINTLN(tx.panelCurrent, 4);
    DEBUG_PRINT("panelPower        : "); DEBUG_PRINTLN(tx.panelPower, 4);
    DEBUG_PRINT("batteryPercent    : "); DEBUG_PRINTLN(tx.batteryPercent, 2);
    DEBUG_PRINT("lampPower         : "); DEBUG_PRINTLN(tx.lampPower, 2);
    DEBUG_PRINT("lampDimmer        : "); DEBUG_PRINTLN(tx.lampDimmer);
    DEBUG_PRINT("maxLDR            : "); DEBUG_PRINTLN(tx.maxLDR, 2);
    DEBUG_PRINT("panelTemperature  : "); DEBUG_PRINTLN(tx.panelTemperature, 2);
    DEBUG_PRINT("panDegreeRead     : "); DEBUG_PRINTLN(tx.panDegreeRead);
    DEBUG_PRINT("tiltDegreeRead    : "); DEBUG_PRINTLN(tx.tiltDegreeRead);
  
    DEBUG_PRINT("batteryVoltage    : "); DEBUG_PRINTLN(tx.batteryVoltage, 4);
    DEBUG_PRINT("chargeStatus      : "); DEBUG_PRINTLN(tx.chargeStatus);
    DEBUG_PRINT("PJU1State         : "); DEBUG_PRINTLN(tx.PJU1State);
    DEBUG_PRINT("PJU2State         : "); DEBUG_PRINTLN(tx.PJU2State);
    DEBUG_PRINT("treeState         : "); DEBUG_PRINTLN(tx.treeState);
  
    DEBUG_PRINT("eventCode         : "); DEBUG_PRINTLN(tx.eventCode);
  
    DEBUG_PRINTLN("=========================================");
  
    DEBUG_PRINT(" || Data Sent to ESP with size: ");
    DEBUG_PRINTLN(sizeof(TelemetryPacket));
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void serialDataRead(){
  if (readControl()) {
    DEBUG_PRINTLN("\n==== CONTROL RECEIVED FROM ESP ====");
  
    DEBUG_PRINT("panManual   : "); DEBUG_PRINTLN(rx.panManual);
    DEBUG_PRINT("tiltManual  : "); DEBUG_PRINTLN(rx.tiltManual);
  
    DEBUG_PRINT("overrideStat: "); DEBUG_PRINTLN(rx.overrideStat);
    DEBUG_PRINT("PJU1Control : "); DEBUG_PRINTLN(rx.PJU1Control);
    DEBUG_PRINT("PJU2Control : "); DEBUG_PRINTLN(rx.PJU2Control);
    DEBUG_PRINT("treeControl : "); DEBUG_PRINTLN(rx.treeControl);
  
    DEBUG_PRINTLN("=========================================");
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
}
