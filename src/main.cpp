#include <Arduino.h>
#include <SPI.h>

// Wi-Fi
#include <ESP8266WiFi.h>
#include <WiFiManager.h>

// Adafruit MQTT
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

// DHT11
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// Project headers
#include "Config.h"

WiFiClientSecure client;
Adafruit_MQTT_Client mqttClient(&client, ADAFRUIT_IO_SERVER, ADAFRUIT_IO_PORT, ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY);

Adafruit_MQTT_Publish temperatureFeed = Adafruit_MQTT_Publish(&mqttClient, MQTT_TEMPERATURE_FEED);
Adafruit_MQTT_Publish humidityFeed = Adafruit_MQTT_Publish(&mqttClient, MQTT_HUMIDITY_FEED);

DHT_Unified dht(D4, DHT11);

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
  client.setFingerprint(ADAFRUIT_IO_FINGERPRINT);

  // DHT11 setup.
  Serial.println(F("Setting up DHT11 sensor..."));
  dht.begin();
  Serial.println(F("DHT11 sensor set up"));
}

void loop()
{
  keepMqttClient();

  sensors_event_t event;

  dht.temperature().getEvent(&event);
  if (isnan(event.temperature))
  {
    Serial.println(F("Error reading temperature!"));
  }
  else
  {
    // Publish temperature value.
    Serial.print(F("Sending temperature "));
    Serial.print(event.temperature);
    Serial.print(F("... "));

    if (temperatureFeed.publish(event.temperature))
    {
      Serial.println(F("sent"));
    }
    else
    {
      Serial.println(F("failed"));
    }
  }

  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity))
  {
    Serial.println(F("Error reading humidity!"));
  }
  else
  {
    // Publish humidity value.
    Serial.print(F("Sending humidity "));
    Serial.print(event.relative_humidity);
    Serial.print(F("... "));

    if (humidityFeed.publish(event.relative_humidity))
    {
      Serial.println(F("sent"));
    }
    else
    {
      Serial.println(F("failed"));
    }
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
