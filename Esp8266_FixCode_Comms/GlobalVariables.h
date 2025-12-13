struct TelemetryPacket {
  int16_t panDegreeRead;
  int16_t tiltDegreeRead;

  float panelVoltage;
  float panelCurrent;
  float batteryVoltage;
  float batteryPercent;

  float panelTemperature;

  uint8_t chargeStatus;
  uint8_t PJU1State;
  uint8_t PJU2State;
  uint8_t treeState;

  uint8_t eventCode;
} __attribute__((packed));

TelemetryPacket rx;
  
struct ControlPacket {
  int16_t panManual;
  int16_t tiltManual;

  uint8_t overrideStat;
  uint8_t PJU1Control;
  uint8_t PJU2Control;
  uint8_t treeControl;
} __attribute__((packed));

ControlPacket tx;

unsigned long lastSend = 0;
