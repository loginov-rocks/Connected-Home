#ifndef CONFIG_H
#define CONFIG_H

#define SERIAL_BAUDRATE 115200
#define LOOP_DELAY 5000

#define AUTOCONNECT_SSID "MQTT-Home-Monitor"
#define AUTOCONNECT_PASSWORD "password"

#define ADAFRUIT_IO_SERVER "io.adafruit.com"
#define ADAFRUIT_IO_PORT 8883
#define ADAFRUIT_IO_USERNAME "username"
#define ADAFRUIT_IO_KEY "key"

#define MQTT_TEMPERATURE_FEED "username/feeds/am2320.temperature"
#define MQTT_HUMIDITY_FEED "username/feeds/am2320.humidity"

#define MQTT_CONNECTION_RETRIES 3
#define MQTT_CONNECTION_DELAY 5000

#endif
