#include <Servo.h>

// -------------------- CONFIG --------------------
Servo panServo;   // Horizontal movement
Servo tiltServo;  // Vertical movement

// LDR pins
const int LDR_TL = A0, LDR_TR = A1, LDR_BL = A2, LDR_BR = A3;

// Servo limits
int panPos  = 90;
int tiltPos = 90;
const int PAN_MIN  = 0;
const int PAN_MAX  = 180;
const int TILT_MIN = 40;
const int TILT_MAX = 150;

// Tuning
const int DEADZONE = 50;
const int STEP     = 1;
const int SMOOTH_N = 10;
const unsigned long TRACK_INTERVAL = 30;  // ms (servo update rate)

// Smoothing buffers
int smoothTL = 0, smoothTR = 0, smoothBL = 0, smoothBR = 0;

// Timing
unsigned long lastTrack = 0;


void setup() {
  Serial.begin(9600);

  panServo.attach(9);
  tiltServo.attach(8);

  panServo.write(panPos);
  tiltServo.write(tiltPos);
}

void loop() {
  updateSolarTracker();  // <── ONE CALL ONLY
}



// -------------------- FUNCTIONS --------------------
int getSmooth(int pin, int &buffer) {
  buffer = (buffer * (SMOOTH_N - 1) + analogRead(pin)) / SMOOTH_N;
  return buffer;
}

// ---- CALL THIS FUNCTION IN LOOP() ----
void updateSolarTracker() {
  unsigned long now = millis();
  if (now - lastTrack < TRACK_INTERVAL) return;  // Non-blocking timing
  lastTrack = now;

  // Read LDRs (smoothed)
  int TL = getSmooth(LDR_TL, smoothTL);
  int TR = getSmooth(LDR_TR, smoothTR);
  int BL = getSmooth(LDR_BL, smoothBL);
  int BR = getSmooth(LDR_BR, smoothBR);

  // Calculate averages
  int topAvg    = (TL + TR) / 2;
  int bottomAvg = (BL + BR) / 2;
  int leftAvg   = (TL + BL) / 2;
  int rightAvg  = (TR + BR) / 2;

  // ---------------- VERTICAL CONTROL ----------------
  int diffV = topAvg - bottomAvg;
  if (abs(diffV) > DEADZONE) {
    if (diffV < 0 && tiltPos < TILT_MAX) tiltPos += STEP;
    if (diffV > 0 && tiltPos > TILT_MIN) tiltPos -= STEP;
    tiltServo.write(tiltPos);
  }

  // ---------------- HORIZONTAL CONTROL ----------------
  int diffH = leftAvg - rightAvg;
  if (abs(diffH) > DEADZONE) {
    if (diffH < 0 && panPos < PAN_MAX) panPos += STEP;
    if (diffH > 0 && panPos > PAN_MIN) panPos -= STEP;
    panServo.write(panPos);
  }

  // Debug printing (optional)
  /*
  Serial.print("Pan: "); Serial.print(panPos);
  Serial.print("  Tilt: "); Serial.print(tiltPos);
  Serial.print(" | TL "); Serial.print(TL);
  Serial.print(" TR "); Serial.print(TR);
  Serial.print(" BL "); Serial.print(BL);
  Serial.print(" BR "); Serial.println(BR);
  */
}
