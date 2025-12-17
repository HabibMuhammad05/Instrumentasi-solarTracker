#include <BlynkSimpleEsp8266.h>
char auth[] = "9zC48oWa7z8m-9i-QN_4hnz72n-ehqEa"; 

void blynkSetup(){
  Blynk.begin(auth, ssid, password, "iot.serangkota.go.id", 8080);
  Blynk.syncVirtual(V15, V16, V17, V18, V19, V20);
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

BLYNK_WRITE(V15){ tx.panManual = param.asInt();  DEBUG_PRINTLN("V15 RECEIVED"); }
BLYNK_WRITE(V16){ tx.tiltManual   = (int16_t)param.asInt(); DEBUG_PRINTLN("V16 RECEIVED");}
BLYNK_WRITE(V17){ 
  tx.overrideStat = (uint8_t)param.asInt(); 
  tx.overrideStatUpdated = (uint8_t)param.asInt(); 
  DEBUG_PRINTLN("V17 RECEIVED");
}
BLYNK_WRITE(V18){ tx.PJU1Control  = (uint8_t)param.asInt(); DEBUG_PRINTLN("V18 RECEIVED");}
BLYNK_WRITE(V19){ tx.PJU2Control  = (uint8_t)param.asInt(); DEBUG_PRINTLN("V19 RECEIVED");}
BLYNK_WRITE(V20){ tx.treeControl  = (uint8_t)param.asInt(); DEBUG_PRINTLN("V20 RECEIVED");}


void handleWiFi() {
  static bool wasConnected = false;
  static unsigned long wifiLastTry = 0;

  bool connected = (WiFi.status() == WL_CONNECTED);

  // ---- detect status change ----
  if (connected && !wasConnected) {
    wifiConnect = true;

    DEBUG_PRINTLN("");
    DEBUG_PRINTLN("[WIFI] Connected!");
    DEBUG_PRINT("IP ESP: ");
    DEBUG_PRINTLN(WiFi.localIP());
    blynkSetup();
    Blynk.syncVirtual(V15, V16, V17, V18, V19, V20);
  }

  if (!connected && wasConnected) {
    wifiConnect = false;

    DEBUG_PRINTLN("[WIFI] Disconnected!");
  }

  wasConnected = connected;

  // ---- auto reconnect (non-blocking) ----
  if (!wifiConnect) {
    if (millis() - wifiLastTry < 5000) return;

    wifiLastTry = millis();

    DEBUG_PRINTLN("");
    DEBUG_PRINTLN("[WIFI] Reconnecting...");
    WiFi.begin(ssid, password);
  }
}
