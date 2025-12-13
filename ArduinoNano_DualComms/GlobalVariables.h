struct TelemetryPacket {
  int16_t pan_deg;
  int16_t tilt_deg;
  uint16_t panel_mv;
  uint16_t panel_ma;
  uint16_t battery_mv;
  int16_t air_temp_c;
  uint16_t air_humidity;
  int16_t panel_temp_c;
  uint8_t load_state;
  uint8_t event_code;
  uint8_t system_status;
  uint8_t battery_percent;
};
TelemetryPacket tx;

struct ControlPacket {
  int16_t manual_pan;
  int16_t manual_tilt;
  uint8_t manual_mode;
  uint8_t load_cmd;
};
ControlPacket rx;

unsigned long lastSend = 0;
