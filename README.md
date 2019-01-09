# MQTT Home Monitor

## Requirements

### Hardware

- NodeMCU
- AM2320 temperature and humidity sensor
- Breadboard, wires and jumpers

### MQTT

This project uses [Adafruit IO](https://io.adafruit.com/) MQTT server, create account to get Adafruit IO
username and key and configure following feeds:

- Temperature feed
- Humidity feed

## Quick start

1. Wiring scheme available
[here](https://raw.githubusercontent.com/loginov-rocks/MQTT-Home-Monitor/master/extras/Scheme.png).
2. Clone `src/Config.example.h` to `src/Config.h` and update constants as needed.
3. Connect NodeMCU to the USB port.
4. Open the project in [PlatformIO](https://platformio.org) and upload to the device.

## Public feeds

- [Temperature](https://io.adafruit.com/loginov/feeds/home-monitor-am2320.temperature)
- [Humidity](https://io.adafruit.com/loginov/feeds/home-monitor-am2320.humidity)

Started from [NodeMCU-Arduino-Platform](https://github.com/loginov-rocks/NodeMCU-Arduino-PlatformIO).
