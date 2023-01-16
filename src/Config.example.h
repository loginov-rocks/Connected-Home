#ifndef CONFIG_H
#define CONFIG_H

#define SERIAL_BAUDRATE 115200
#define LOOP_DELAY 5000

#define AUTOCONNECT_SSID "MQTT-Home-Monitor"
#define AUTOCONNECT_PASSWORD "password"

#define ADAFRUIT_IO_SERVER "io.adafruit.com"
#define ADAFRUIT_IO_PORT 8883
#define ADAFRUIT_IO_FINGERPRINT "18 C0 C2 3D BE DD 81 37 73 40 E7 E4 36 61 CB 0A DF 96 AD 25"
#define ADAFRUIT_IO_USERNAME "username"
#define ADAFRUIT_IO_KEY "key"

#define MQTT_TEMPERATURE_FEED "username/feeds/dht11.temperature"
#define MQTT_HUMIDITY_FEED "username/feeds/dht11.humidity"

#define MQTT_CONNECTION_RETRIES 3
#define MQTT_CONNECTION_DELAY 5000

#endif
