#include <Arduino.h>
#include <SPI.h>

// Wi-Fi
#include <ESP8266WiFi.h>
#include <WiFiManager.h>

// Adafruit MQTT
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

// AM2320
#include <Adafruit_Sensor.h>
#include <Adafruit_AM2320.h>

// Project headers
#include "Config.h"

WiFiClientSecure client;
Adafruit_MQTT_Client mqttClient(&client, ADAFRUIT_IO_SERVER, ADAFRUIT_IO_PORT, ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY);

static const char *fingerprint PROGMEM = "18 C0 C2 3D BE DD 81 37 73 40 E7 E4 36 61 CB 0A DF 96 AD 25";

Adafruit_MQTT_Publish temperatureFeed = Adafruit_MQTT_Publish(&mqttClient, MQTT_TEMPERATURE_FEED);
Adafruit_MQTT_Publish humidityFeed = Adafruit_MQTT_Publish(&mqttClient, MQTT_HUMIDITY_FEED);

Adafruit_AM2320 am2320 = Adafruit_AM2320();

void keepMqttClient();

void setup()
{
  Serial.begin(SERIAL_BAUDRATE);

  // Wi-Fi connection setup.
  Serial.println(F("Establishing Wi-Fi connection..."));
  WiFiManager wifiManager;
  wifiManager.autoConnect(AUTOCONNECT_SSID, AUTOCONNECT_PASSWORD);
  Serial.println(F("Wi-Fi connection established"));

  // check the fingerprint of io.adafruit.com's SSL cert
  client.setFingerprint(fingerprint);

  // AM2320 setup.
  Serial.println(F("Setting up AM2320 sensor..."));
  am2320.begin();
  Serial.println(F("AM2320 sensor set up"));
}

void loop()
{
  keepMqttClient();

  float temperature = am2320.readTemperature();
  float humidity = am2320.readHumidity();

  // Publish temperature value.
  Serial.print(F("Sending temperature "));
  Serial.print(temperature);
  Serial.print(F("... "));

  if (temperatureFeed.publish(temperature))
  {
    Serial.println(F("sent"));
  }
  else
  {
    Serial.println(F("failed"));
  }

  // Publish humidity value.
  Serial.print(F("Sending humidity "));
  Serial.print(humidity);
  Serial.print(F("... "));

  if (humidityFeed.publish(humidity))
  {
    Serial.println(F("sent"));
  }
  else
  {
    Serial.println(F("failed"));
  }

  delay(LOOP_DELAY);
}

void keepMqttClient()
{
  // Stop if already connected.
  if (mqttClient.connected())
  {
    return;
  }

  Serial.println(F("Establishin MQTT connection... "));

  int8_t errorCode;
  uint8_t retries = MQTT_CONNECTION_RETRIES;

  // Connect will return 0 if connected.
  while ((errorCode = mqttClient.connect()) != 0)
  {
    Serial.println(mqttClient.connectErrorString(errorCode));

    mqttClient.disconnect();
    retries--;

    if (retries == 0)
    {
      // Basically die and wait for WDT to reset the device.
      while (1)
        ;
    }

    Serial.print(F("Retrying in "));
    Serial.print(MQTT_CONNECTION_DELAY);
    Serial.println(F(" milliseconds..."));

    delay(MQTT_CONNECTION_DELAY);
  }

  Serial.println(F("MQTT connection established"));
}
