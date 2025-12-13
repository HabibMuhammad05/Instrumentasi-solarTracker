const int panelCurPin = A7;
float currentVal;

const uint8_t panelVoltPin = A6;
float panelVoltVal;
float panelPower;

const uint8_t battVoltPin = A4;
float battVal;

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

  Serial.print("                                           | PV Current: ");
  Serial.print(currentVal, 2);
  Serial.print(" A");
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

  panelPower = panelVoltVal * currentVal;
  
  Serial.print(" | PV Voltage: ");
  Serial.print(panelVoltVal, 2);
  Serial.print(" V");
  Serial.print(" | PV Power: ");
  Serial.print(panelVoltVal, 2);
  Serial.print(" W");
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
  battVal =  avg * 5.0 / 1023.0; 
  
  Serial.print(" | Batt Voltage: ");
  Serial.print(battVal, 2);
  Serial.print(" V");
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

  Serial.print(" | Panel Temp: ");
  Serial.print(tempC, 2);
  Serial.println(" °C");
}
