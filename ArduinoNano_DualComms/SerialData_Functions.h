#include <SoftwareSerial.h>
SoftwareSerial espSerial(8, 7); // RX, TX

void StartSerial(){
  espSerial.begin(9600);
  Serial.println("Serial Communication Init..");
}

void SerialRW(){
  // --- SEND TELEMETRY TO ESP ---
  if (millis() - lastSend >= 150) {
    lastSend = millis();

    tx.pan_deg = 30;
    tx.tilt_deg = -10;
    tx.panel_mv = 5100;
    tx.panel_ma = 320;
    tx.battery_mv = 7800;
    tx.air_temp_c = 26;
    tx.air_humidity = 60;
    tx.panel_temp_c = 37;
    tx.load_state = 1;
    tx.event_code = 4;
    tx.system_status = 1;
    tx.battery_percent = 88;

    espSerial.write((uint8_t*)&tx, sizeof(tx));
    Serial.println("Data Sent to Arduino");
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }

  // --- RECEIVE CONTROL FROM ESP ---
  if (espSerial.available() >= sizeof(rx)) {
    espSerial.readBytes((char*)&rx, sizeof(rx));

    Serial.print("ESP Control → pan: ");
    Serial.print(rx.manual_pan);
    Serial.print(" tilt: ");
    Serial.print(rx.manual_tilt);
    Serial.print(" mode: ");
    Serial.print(rx.manual_mode);
    Serial.print(" load: ");
    Serial.println(rx.load_cmd);
  }
}
