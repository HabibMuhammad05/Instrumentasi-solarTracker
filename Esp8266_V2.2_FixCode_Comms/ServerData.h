#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

WiFiClient client;

void startWifi(){
  Serial.println();
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected! IP ESP: ");
  Serial.println(WiFi.localIP());
}

void sendData() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi Disconnected!");
    return;
  }
  
  HTTPClient http;
  http.begin(client, serverName);
  http.addHeader("Content-Type", "application/json");

  StaticJsonDocument<200> doc;
  doc["pv_voltage"]    = rx.panelVoltage;
  doc["pv_current"]    = rx.panelCurrent;
  doc["pv_power"]      = rx.panelPower;
  doc["battery_power"] = rx.batteryPercent;
  doc["lamp_power"]    = rx.lampPower;
  doc["lamp_dimmer"]   = rx.lampDimmer;
  doc["rssi_signal"]   = WiFi.RSSI();
  doc["ldr_value"]     = rx.maxLDR;
  doc["panel_temp"]    = rx.panelTemperature;
  doc["azimuth"]       = rx.panDegreeRead;
  doc["elevation"]     = rx.tiltDegreeRead; 

  String requestBody;
  serializeJson(doc, requestBody);
  int httpCode = http.POST(requestBody);

  if (httpCode > 0) {
    Serial.print("HTTP Response: %d\n", httpCode);
    Serial.println(http.getString());
  } else {
    Serial.print("HTTP Request error: ");
    Serial.println(httpCode);
  }
  http.end();
}
