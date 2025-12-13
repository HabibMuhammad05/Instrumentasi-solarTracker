#include <Servo.h>
Servo panServo;   
Servo tiltServo; 

int panPos  = 90;
int tiltPos = 90;
const int PAN_MIN  = 0;
const int PAN_MAX  = 180;
const int TILT_MIN = 0;
const int TILT_MAX = 180;

const int LDR_TR = A2, LDR_TL = A3, LDR_BR = A1, LDR_BL = A0;
int smoothTL = 0, smoothTR = 0, smoothBL = 0, smoothBR = 0;

const int DEADZONE = 50;
const int STEP     = 2;
const int SMOOTH_N = 10;
const unsigned long TRACK_INTERVAL = 3;  

unsigned long lastTrack = 0;


void ServoSetup(){
  panServo.attach(9);
  tiltServo.attach(10);

  panServo.write(panPos);
  tiltServo.write(tiltPos);
}


int getSmooth(int pin, int &buffer) {
  buffer = (buffer * (SMOOTH_N - 1) + analogRead(pin)) / SMOOTH_N;
  return buffer;
}

void updateSolarTracker() {
  unsigned long now = millis();
  if (now - lastTrack < TRACK_INTERVAL) return; 

  lastTrack = now;

  int TL = getSmooth(LDR_TL, smoothTL);
  int TR = getSmooth(LDR_TR, smoothTR);
  int BL = getSmooth(LDR_BL, smoothBL);
  int BR = getSmooth(LDR_BR, smoothBR);

  int topAvg    = (TL + TR) / 2;
  int bottomAvg = (BL + BR) / 2;
  int leftAvg   = (TL + BL) / 2;
  int rightAvg  = (TR + BR) / 2;

  int diffV = topAvg - bottomAvg;
  if (abs(diffV) > DEADZONE) {
    if (diffV < 0 && tiltPos < TILT_MAX) tiltPos += STEP;
    if (diffV > 0 && tiltPos > TILT_MIN) tiltPos -= STEP;
    tiltServo.write(tiltPos);
  }

  int diffH = leftAvg - rightAvg;
  if (abs(diffH) > DEADZONE) {
    if (diffH < 0 && panPos < PAN_MAX) panPos += STEP;
    if (diffH > 0 && panPos > PAN_MIN) panPos -= STEP;
    panServo.write(panPos);
  }

//  /*Debug
  Serial.print("Pan: "); Serial.print(panPos);
  Serial.print("  Tilt: "); Serial.print(tiltPos);
  Serial.print(" | TL "); Serial.print(TL);
  Serial.print(" TR "); Serial.print(TR);
  Serial.print(" BL "); Serial.print(BL);
  Serial.print(" BR "); Serial.println(BR);
//  */

}
