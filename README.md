# IoT-gas-detector-ESP8266
CNG (Methane) Gas detector system with ESP8266 board and MQ4 sensor

## Description
ESP8266 firmware for MQ-4 gas sensor project: Calibrates sensor in clean air (Rs/Ro ratio), reads ADC values, estimates methane PPM, activates buzzer on threshold exceed, and sends JSON data via HTTP POST to server for mobile alerts.

## My Role
Full development of this ESP8266 code (written by me).

## Technologies
- ESP8266
- MQ-4 Sensor (methane/CNG detection)
- HTTP POST (JSON)
- Arduino IDE

## Installation
1. Upload to ESP8266 via Arduino IDE.
2. Calibrate in clean air.
3. Connect to WiFi; monitor serial for PPM and alerts.

## Key Features
- PPM calculation: Approximate methane concentration.
- Alert: Buzzer + HTTP send if PPM > threshold.
