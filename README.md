# Solar Tracker and Monitoring System

This project is a solar tracking and monitoring system designed to adjust the position of a solar panel based on light intensity and monitor several electrical and environmental parameters.

The system uses two microcontrollers, an Arduino Nano and an ESP8266. The Arduino Nano handles the tracking mechanism, sensor readings, and output controls, while the ESP8266 manages serial communication, Wi-Fi connectivity, data transmission, and the OLED display.

The repository contains several program versions and test examples from the development process. The latest program set available in this repository is **Arduino Nano V2.1** and **ESP8266 V2.2**.

## Main Features

- Automatic solar panel positioning based on four LDR sensors.
- Manual positioning control through serial commands.
- Horizontal and vertical panel movement using two servo motors.
- Solar panel voltage, current, and power monitoring.
- Battery voltage and estimated battery level monitoring.
- Panel temperature measurement.
- Control of two PJU lighting outputs.
- LED output effects for tree lighting.
- OLED display for system status and monitoring information.
- Wi-Fi connection and HTTP data transmission to a configured server.
- Serial communication between the Arduino Nano and ESP8266.

## System Overview

The system is divided into two main parts.

### Arduino Nano

The Arduino Nano is responsible for the hardware control and data acquisition:

- Reads four LDR sensors to determine the direction of the strongest light.
- Adjusts the solar panel position through pan and tilt servo motors.
- Supports automatic and manual tracking modes.
- Measures solar panel voltage and current.
- Calculates solar panel power.
- Reads battery voltage and estimates its percentage.
- Reads panel temperature using an LM35 sensor.
- Controls two PJU outputs and four additional LED outputs.
- Sends monitoring data to the ESP8266 and receives control data through serial communication.

### ESP8266 (Wemos D1)

The ESP8266 handles communication and monitoring functions:

- Exchanges telemetry and control data with the Arduino Nano through serial communication.
- Connects to a Wi-Fi network.
- Sends monitoring data to a configured HTTP server in JSON format.
- Displays system information through a 128×64 OLED display.

## Hardware Overview

The main hardware components used in the project include:

- Arduino Nano.
- ESP8266 / Wemos D1 mini.
- Solar panel.
- Four LDR sensors.
- Two servo motors for horizontal and vertical movement.
- Voltage sensing circuits for the solar panel and battery.
- Current sensor for solar panel current measurement.
- LM35 temperature sensor.
- 128×64 OLED display using the SSD1306 controller.
- PJU lighting outputs and additional LED indicators.
- Power supply, battery, wiring, and supporting electronic components.
- Custom PCB for system integration.

Component specifications and wiring may vary depending on the hardware assembly. Refer to the schematic, PCB files, and pin definitions included in the repository before building or modifying the system.

## Software and Libraries

The programs are developed using the Arduino IDE.

The main libraries used in the latest program set include:

- **Servo2** – controls the pan and tilt servo motors on the Arduino Nano.
- **SoftwareSerial** – provides serial communication between the Arduino Nano and ESP8266.
- **U8g2** – controls the SSD1306 OLED display.
- **ESP8266WiFi** – manages the ESP8266 Wi-Fi connection.
- **ESP8266HTTPClient** – handles HTTP requests to the server.
- **ArduinoJson** – formats monitoring data into JSON.

Some earlier programs and examples use additional libraries. Install the libraries required by the specific sketch before compiling it.

## Repository Contents

The repository contains several program versions and supporting files:

| Folder | Description |
|---|---|
| `ArduinoNano_DualComms` | Arduino Nano program for an earlier dual-communication development version. |
| `ArduinoNano_FixCode_Comms` | Revised Arduino Nano program with separated control and monitoring functions. |
| `ArduinoNano_V2.1_FixCode_Comms` | Latest Arduino Nano program included in this repository. |
| `Esp8266_DualComms` | ESP8266 program for an earlier dual-communication development version. |
| `Esp8266_FixCode_Comms` | Revised ESP8266 communication program. |
| `Esp8266_V2.1_FixCode_Comms` | ESP8266 program with earlier monitoring and communication features. |
| `Esp8266_V2.2_FixCode_Comms` | Latest ESP8266 program included in this repository. |
| `SolarTracker_Example` | Basic solar tracking program for testing the LDR sensors and servo motors. |
| `SolarTracker_VoltCurrent_Example` | Example combining solar tracking, electrical measurements, and lighting control. |
| `Libraries` | Supporting library archives used during development. |
| `Other Files & Documentation` | Schematics, PCB design files, pin definitions, and other project documentation. |

The latest program set uses the Arduino Nano V2.1 and ESP8266 V2.2 folders. The older versions and standalone examples are retained as references and should not be mixed with the latest program files unless their compatibility has been checked.

## Getting Started

1. Install the Arduino IDE.
2. Install Arduino Nano and ESP8266 board support.
3. Install the libraries required by each program.
4. Download or clone this repository.
5. Open `ArduinoNano_V2.1_FixCode_Comms.ino` from the `ArduinoNano_V2.1_FixCode_Comms` folder.
6. Open `Esp8266_V2.2_FixCode_Comms.ino` from the `Esp8266_V2.2_FixCode_Comms` folder.
7. Select the correct board and port for each microcontroller.
8. Check the pin assignments, wiring, and communication connections.
9. Configure the Wi-Fi network and server address in the ESP8266 source files.
10. Compile and upload each program to its corresponding board.

Keep each sketch and its associated header files in the same folder. The Arduino Nano and ESP8266 programs must be uploaded separately to their respective boards.

## Configuration

Before operating the system, check the following settings:

- **Pin assignments:** Refer to the source code and `pn-Definitions.txt` in `Other Files & Documentation`.
- **Servo positions:** Check the pan and tilt limits in `SolarTracker.h`.
- **Tracking response:** Adjust the dead zone, movement step, and sensor smoothing settings to suit the mechanical assembly and lighting conditions.
- **Wi-Fi connection:** Set the correct SSID and password in the ESP8266 program.
- **Server address:** Update `serverName` in `GlobalVariables.h` to match the HTTP endpoint used by the monitoring server.
- **Sensor calibration:** Verify voltage and current readings against appropriate measuring instruments before using the displayed values.

## Hardware Documentation

Additional hardware design files are available in `Other Files & Documentation`, including:

- PCB layout and board files.
- Circuit schematic files.
- Schematic PDF documents.
- Pin definition reference.

These files can be used as references when assembling, checking, or modifying the hardware.

## Notes

- The latest program set in this repository is Arduino Nano V2.1 and ESP8266 V2.2.
- The Arduino Nano and ESP8266 use separate sketches and communicate through a serial connection.
- The ESP8266 requires a working Wi-Fi network and a reachable server endpoint for HTTP data transmission.
- Sensor readings and calculated electrical values should be calibrated according to the actual sensor modules and circuit configuration.
- Servo movement limits and tracking parameters may need adjustment based on the mechanical design.
- Check the power requirements of the servo motors, lighting outputs, and microcontroller boards before connecting the system.

---

This repository contains the source code and supporting design files for the development of the Solar Tracker and Monitoring System.
