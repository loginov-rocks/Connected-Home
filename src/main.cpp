#include <Arduino.h>

// Wi-Fi
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

// AM2320
#include <Adafruit_Sensor.h>
#include <Adafruit_AM2320.h>

#define SERIAL_BAUDRATE 115200

#define AUTOCONNECT_SSID "MQTT-Home-Monitor"
#define AUTOCONNECT_PASSWORD "87654321"

Adafruit_AM2320 am2320 = Adafruit_AM2320();

void setup()
{
  Serial.begin(SERIAL_BAUDRATE);

  // Wi-Fi connection setup.
  Serial.println(F("Establishing Wi-Fi connection..."));
  WiFiManager wifiManager;
  wifiManager.autoConnect(AUTOCONNECT_SSID, AUTOCONNECT_PASSWORD);
  Serial.println(F("Wi-Fi connection established"));

  // AM2320 setup.
  Serial.println(F("Setting up AM2320 sensor..."));
  am2320.begin();
  Serial.println(F("AM2320 sensor set up"));

  // Print headline.
  Serial.println(F("T, C\tH, %"));
}

void loop()
{
  float temperature = am2320.readTemperature();
  float humidity = am2320.readHumidity();

  Serial.print(temperature);
  Serial.print(F("\t"));
  Serial.println(humidity);

  delay(2000);
}
