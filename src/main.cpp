#include <Arduino.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>

#define SERIAL_BAUDRATE 115200

#define AUTOCONNECT_SSID "MQTT-Home-Monitor"
#define AUTOCONNECT_PASSWORD "87654321"

void setup()
{
  Serial.begin(SERIAL_BAUDRATE);

  // Wi-Fi connection setup.
  Serial.println(F("Establishing Wi-Fi connection..."));

  WiFiManager wifiManager;
  wifiManager.autoConnect(AUTOCONNECT_SSID, AUTOCONNECT_PASSWORD);

  Serial.println(F("Wi-Fi connection established"));
}

void loop()
{
  Serial.println(F("Idle..."));
  delay(1000);
}
