const byte treePins[] = {2, 4, 5, 6};    
const byte ledCount = sizeof(treePins);
unsigned long nextTime[4];  
bool ledState[4];            
const byte PJU1 = 3;
const byte PJU2 = 11;

uint8_t lampDimmingval = 255;

bool PJU1Stat = false;  
bool PJU2Stat = false;  

bool treeEffect = false; 

void outputSetup() {
  pinMode(PJU1, OUTPUT);
  pinMode(PJU2, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  
  for (byte i = 0; i < ledCount; i++) {
    pinMode(treePins[i], OUTPUT);
    digitalWrite(treePins[i], LOW);
    ledState[i] = false;
    nextTime[i] = millis() + random(100, 500);
  }
}

void fadeLED(byte pin, bool turnOn) {

  static int brightness[13] = {0};        
  static int fadeDir[13] = {0};            // +1 fade in, -1 fade out, 0 idle
  static unsigned long nextTime[13] = {0}; 

  unsigned long now = millis();
  if (turnOn) {
    fadeDir[pin] = 1;    
  } else {
    fadeDir[pin] = -1;   // fade OUT
  }

  if (now >= nextTime[pin]) {
    brightness[pin] += fadeDir[pin];
    if (brightness[pin] > 255) brightness[pin] = 255;
    if (brightness[pin] < 0)   brightness[pin] = 0;
    analogWrite(pin, brightness[pin]);
    lampDimmingval = brightness[pin];
    nextTime[pin] = now + 5;   
  }
}

void updateTreeEffect() {
  if (!treeEffect) {
    for (byte i = 0; i < ledCount; i++) {
      digitalWrite(treePins[i], LOW);
    }
    return;
  }

  for (byte i = 0; i < ledCount; i++) {
    if (millis() >= nextTime[i]) {
      ledState[i] = !ledState[i];
      digitalWrite(treePins[i], ledState[i]);
      nextTime[i] = millis() + random(100, 400);  
    }
  }
}
