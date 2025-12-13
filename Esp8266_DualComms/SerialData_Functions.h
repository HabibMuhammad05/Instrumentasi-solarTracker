#include <SoftwareSerial.h>
SoftwareSerial nanoSerial(13, 15); // d7 RX, d8 TX

void StartSerial(){
  nanoSerial.begin(9600);
  Serial.println("Serial Communication Init..");
}

void SerialRW(){
  if (nanoSerial.available() >= sizeof(rx)) {
    nanoSerial.readBytes((char*)&rx, sizeof(rx));

    Serial.print("Telemetry: Panel=");
    Serial.print(rx.panel_mv);
    Serial.print("mV Current=");
    Serial.print(rx.panel_ma);
    Serial.print("mA Battery=");
    Serial.print(rx.battery_mv);
    Serial.println("mV");
  }

  // --- SEND CONTROL PACKET ---
  if (millis() - lastSend >= 200) {
    lastSend = millis();

    tx.manual_pan = 10;
    tx.manual_tilt = -5;
    tx.manual_mode = 1;
    tx.load_cmd = 0;

    nanoSerial.write((uint8_t*)&tx, sizeof(tx));
    Serial.println("Data Sent to Arduino");
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
}
