const int panelCurPin = A7;
float currentVal;

const uint8_t panelVoltPin = A6;
float panelVoltVal;
float panelPower;
bool panelPowerIn;

const uint8_t battVoltPin = A4;
float battVal;
float battPerc;

const byte tempSensorPin = A5;
float tempC;


void inputSetup(){
  pinMode(LDR_TR, INPUT);
  pinMode(LDR_TL, INPUT);
  pinMode(LDR_BR, INPUT);
  pinMode(LDR_BL, INPUT);
  pinMode(panelCurPin, INPUT);
  pinMode(panelVoltPin, INPUT);
  pinMode(battVoltPin, INPUT);
  pinMode(tempSensorPin, INPUT);
}

void panelCurrent() {
  const int sensitivitas = 185;         
  const int teganganoffset = 2500;     
  const byte N = 20;                  
  const unsigned long interval = 20;    

  static int buf[N];
  static long sum = 0;
  static byte idx = 0;
  static bool filled = false;
  static unsigned long lastRead = 0;

  if (millis() - lastRead >= interval) {
    lastRead = millis();
    sum -= buf[idx];
    buf[idx] = analogRead(panelCurPin);
    sum += buf[idx];
    idx++;
    if (idx >= N) { idx = 0; filled = true; }
  }

  float avgADC = filled ? sum / (float)N : sum / (float)idx;
  float tegangan = (avgADC / 1024.0) * 5000.0;
  currentVal = (tegangan - teganganoffset) / sensitivitas;

//  DEBUG_PRINT("PV Current: ");
//  DEBUG_PRINT(currentVal, 2);
//  DEBUG_PRINT(" A");
}

float panelVoltage() {
  const byte N = 20;
  const unsigned long interval = 50;

  static int buf[N];
  static long sum = 0;
  static byte idx = 0;
  static bool filled = false;
  static unsigned long lastRead = 0;

  if (millis() - lastRead >= interval) {
    lastRead = millis();
    sum -= buf[idx];
    buf[idx] = analogRead(panelVoltPin);
    sum += buf[idx];
    idx++;
    if (idx >= N) { idx = 0; filled = true; }
  }

  float avg = filled ? sum / (float)N : sum / (float)idx;
  panelVoltVal =  (avg * 5.0 / 1023.0) * 2; 

  if(panelVoltVal >= 5.0) panelPowerIn = true;
  else panelPowerIn = false;
  panelPower = panelVoltVal * currentVal;
  
//  DEBUG_PRINT(" | PV Voltage: ");
//  DEBUG_PRINT(panelVoltVal, 2);
//  DEBUG_PRINT(" V");
//  DEBUG_PRINT(" | PV Power: ");
//  DEBUG_PRINT(panelVoltVal, 2);
//  DEBUG_PRINT(" W");
}

float batteryVoltage() {
  const byte N = 20;
  const unsigned long interval = 50;
  static int buf[N];
  static long sum = 0;
  static byte idx = 0;
  static bool filled = false;
  static unsigned long lastRead = 0;

  if (millis() - lastRead >= interval) {
    lastRead = millis();
    sum -= buf[idx];
    buf[idx] = analogRead(battVoltPin);
    sum += buf[idx];
    idx++;
    if (idx >= N) { idx = 0; filled = true; }
  }

  float avg = filled ? sum / (float)N : sum / (float)idx;
  battVal = avg * 5.0 / 1023.0;     

  float v = battVal;
  if (v < 3.0) v = 3.0;
  if (v > 4.2) v = 4.2;
  battPerc = (v - 3.0) / (4.2 - 3.0) * 100.0;
  if (battPerc < 0) battPerc = 0;
  if (battPerc > 100) battPerc = 100;

//  DEBUG_PRINT(" | Batt Voltage: ");
//  DEBUG_PRINT(battVal, 2);
//  DEBUG_PRINT(" V");
//
//  DEBUG_PRINT(" (");
//  DEBUG_PRINT(battPerc, 0);
//  DEBUG_PRINT("%)");
}


void readLM35() {
  const byte N = 20;               // moving average size
  const unsigned long interval = 50;

  static int buf[N];
  static long sum = 0;
  static byte idx = 0;
  static bool filled = false;
  static unsigned long lastRead = 0;

  // non-blocking ADC update
  if (millis() - lastRead >= interval) {
    lastRead = millis();

    sum -= buf[idx];
    buf[idx] = analogRead(tempSensorPin);
    sum += buf[idx];

    idx++;
    if (idx >= N) { idx = 0; filled = true; }
  }

  // filtered ADC value
  float avg = filled ? sum / (float)N : sum / (float)idx;

  // LM35: 10 mV per °C → ADC→V→°C
  float voltage = (avg * 5.0 / 1023.0);   // volts
  tempC = voltage * 100.0;          // convert to °C

//  DEBUG_PRINT(" | Panel Temp: ");
//  DEBUG_PRINT(tempC, 2);
//  DEBUG_PRINT(" °C");
}
