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


//===============================JSON DATA SEND/READ================================//
bool wifiConnect = false;
const char* ssid = "JWS ";
const char* password = "12345678";
String serverName = "http://192.168.107.225:3000/api/iot/update";
unsigned long lastSendTime = 0;
