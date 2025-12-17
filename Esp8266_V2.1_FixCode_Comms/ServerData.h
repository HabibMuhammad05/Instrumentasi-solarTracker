#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

WiFiClient client;


// ================= KONFIGURASI WIFI & SERVER =================
//const char* ssid = "NAMA_WIFI_ANDA";       // Ganti dengan nama WiFi
//const char* password = "PASSWORD_WIFI";    // Ganti dengan password WiFi

// Ganti dengan IP Address Laptop/PC tempat server.js berjalan
// Cara cek IP di Windows: Buka CMD -> ketik "ipconfig" -> lihat IPv4 Address
const char* serverIp = "192.168.175.225";      
const int serverPort = 3000;

// URL Endpoint sesuai server.js
String apiUrlUpdate = "http://" + String(serverIp) + ":" + String(serverPort) + "/api/iot/update";
String apiUrlStatus = "http://" + String(serverIp) + ":" + String(serverPort) + "/api/iot/status";


// ================= FUNGSI KIRIM DATA (POST) =================
void sendTelemetryData() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(client, apiUrlUpdate);   // ESP8266 FIX
    http.addHeader("Content-Type", "application/json");

    // Buat JSON Object menggunakan ArduinoJson
    StaticJsonDocument<512> doc;

    // Masukkan data sesuai kunci di server.js (req.body)
    doc["panel_voltage"] = rx.panelVoltage;
    doc["panel_current"] = rx.panelCurrent;
    doc["panel_power"] = rx.panelPower;
    doc["panel_temp"] = rx.panelTemperature; // Contoh statis (bisa pakai DHT11)
    
    doc["battery_voltage"] = rx.batteryVoltage;
    doc["battery_percent"] = rx.batteryPercent;
    doc["charge_status"] = rx.chargeStatus;

    doc["lamp_power"] = rx.lampPower;
    doc["lamp_dimmer"] = rx.lampDimmer;
    
    doc["max_ldr"] = rx.maxLDR; // Contoh nilai LDR
    doc["rssi_signal"] = WiFi.RSSI();
    doc["event_code"] = rx.eventCode; // 0 = Normal

    doc["pan_degree"] = rx.panDegreeRead; // Posisi servo saat ini
    doc["tilt_degree"] = rx.tiltDegreeRead;

    // Serialize JSON ke String
    String requestBody;
    serializeJson(doc, requestBody);

    // Kirim POST Request
    int httpResponseCode = http.POST(requestBody);

    if (httpResponseCode > 0) {
      String response = http.getString();
      DEBUG_PRINT("[POST] Sukses: ");
      DEBUG_PRINT(httpResponseCode);
      DEBUG_PRINTLN(response); // Respon dari server: {"status":"ok", "msg":"Data Saved"}
    } else {
      DEBUG_PRINT("[POST] Gagal, Error code: ");
      DEBUG_PRINTLN(httpResponseCode);
    }
    http.end();
  } else {
    DEBUG_PRINTLN("WiFi Terputus!");
  }
}

// ================= FUNGSI TERIMA KONTROL (GET) =================
void getControlStatus() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(client, apiUrlStatus);   // ESP8266 FIX
    
    int httpCode = http.GET();
    
    if (httpCode > 0) {
      String payload = http.getString();
      
      // Parse JSON dari server
      StaticJsonDocument<256> doc;
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
        const char* mode = doc["mode"]; // "AUTO" atau "MANUAL"
        float targetAzimuth = doc["target_azimuth"];
        float targetElevation = doc["target_elevation"];

//        Serial.print("[GET] Mode: ");
//        Serial.print(mode);
//        Serial.print(" | Target Pan: ");
//        Serial.println(targetAzimuth);

        // LOGIKA KONTROL DISINI
        if (strcmp(mode, "MANUAL") == 0) {
            // Gerakkan Servo ke targetAzimuth & targetElevation
            // servoPan.write(targetAzimuth);
        } else {
            // Jalankan algoritma solar tracker otomatis
        }
      }
    }
    http.end();
  }
}
