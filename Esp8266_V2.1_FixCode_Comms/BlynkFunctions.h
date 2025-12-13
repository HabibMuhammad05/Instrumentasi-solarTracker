#include <BlynkSimpleEsp8266.h>
#define BLYNK_PRINT Serial
char auth[] = "9zC48oWa7z8m-9i-QN_4hnz72n-ehqEa"; 

void blynkSetup(){
  Blynk.begin(auth, ssid, password, "iot.serangkota.go.id", 8080);
}

void blynkSend(){
  Blynk.virtualWrite(V0, rx.panelVoltage);
  Blynk.virtualWrite(V1, rx.panelCurrent);
  Blynk.virtualWrite(V2, rx.panelPower);
  Blynk.virtualWrite(V3, rx.batteryPercent);
  Blynk.virtualWrite(V4, rx.lampPower);
  Blynk.virtualWrite(V5, rx.lampDimmer);
  Blynk.virtualWrite(V6, rx.maxLDR);
  Blynk.virtualWrite(V7, rx.panelTemperature);
  Blynk.virtualWrite(V8, rx.panDegreeRead);
  Blynk.virtualWrite(V9, rx.tiltDegreeRead);
  Blynk.virtualWrite(V10, rx.batteryVoltage);
  Blynk.virtualWrite(V11, rx.chargeStatus);
  Blynk.virtualWrite(V12, rx.PJU1State);
  Blynk.virtualWrite(V13, rx.PJU2State);
  Blynk.virtualWrite(V14, rx.treeState);
}
