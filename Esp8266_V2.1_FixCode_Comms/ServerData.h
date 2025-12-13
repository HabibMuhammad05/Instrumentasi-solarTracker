#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

WiFiClient sendClient;


void startWifi(){
  DEBUG_PRINTLN();
  DEBUG_PRINTLN("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    DEBUG_PRINT(".");
  }

  DEBUG_PRINTLN("");
  DEBUG_PRINT("Connected! IP ESP: ");
  DEBUG_PRINTLN(WiFi.localIP());
}


enum SendState {
  SEND_IDLE,
  SEND_CONNECTING,
  SEND_SENDING,
  SEND_WAIT_RESPONSE
};

SendState sendState = SEND_IDLE;
unsigned long sendTimeout = 0;

String postData = "";

void startSendData() {
  DEBUG_PRINTLN("");
  DEBUG_PRINTLN("[SEND] Preparing JSON data...");

  // prepare JSON
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

  serializeJson(doc, postData);

//  DEBUG_PRINT("[SEND] JSON size: ");
//  DEBUG_PRINTLN(postData.length());
//  DEBUG_PRINTLN("[SEND] JSON content:");
//  DEBUG_PRINTLN(postData);

  sendState = SEND_CONNECTING;
  sendTimeout = millis();
}

void sendDataNonBlocking() {
  switch (sendState) {

    case SEND_IDLE:
      return;  // nothing to do

    // ---------------------------------------------------------
    case SEND_CONNECTING:
      DEBUG_PRINTLN("[SEND] Connecting to server...");
      
      if (sendClient.connect(serverName, 80)) {
        DEBUG_PRINTLN("[SEND] Connected to server!");
        sendState = SEND_SENDING;
      } 
      else if (millis() - sendTimeout > 1500) {
        DEBUG_PRINTLN("[SEND] ERROR: Connection timeout");
        sendState = SEND_IDLE;
      }
      break;

    // ---------------------------------------------------------
    case SEND_SENDING: {
      DEBUG_PRINTLN("[SEND] Sending POST request...");

      String request =
        "POST / HTTP/1.1\r\n"
        "Host: " + String(serverName) + "\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: " + String(postData.length()) + "\r\n"
        "Connection: close\r\n"
        "\r\n" +
        postData;

      sendClient.print(request);

      DEBUG_PRINTLN("[SEND] POST sent!");

      sendTimeout = millis();
      sendState = SEND_WAIT_RESPONSE;
      break;
    }

    // ---------------------------------------------------------
    case SEND_WAIT_RESPONSE:
      if (sendClient.available()) {
        DEBUG_PRINTLN("[SEND] Response received:");

        while (sendClient.available()) {
          char c = sendClient.read();
          DEBUG_PRINT(c);
        }

        DEBUG_PRINTLN("\n[SEND] Closing connection.");
        sendClient.stop();
        sendState = SEND_IDLE;
      }

      if (millis() - sendTimeout > 3000) {
        DEBUG_PRINTLN("[SEND] ERROR: Response timeout.");
        sendClient.stop();
        sendState = SEND_IDLE;
      }
      break;
  }
}
