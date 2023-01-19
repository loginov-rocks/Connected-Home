#include <Arduino.h>
#include <ArduinoJson.h>

// Wi-Fi
#include <ESP8266WiFi.h>
#include <WiFiManager.h>

#include <AwsIotWiFiClient.h>

// DHT11
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// Project headers
#include "Config.h"
#include "Secrets.h"

AwsIotWiFiClient awsIotWiFiClient;

BearSSL::X509List trustAnchorCertificate(rootCaCertificate);
BearSSL::X509List clientCertificate(deviceCertificate);
BearSSL::PrivateKey clientPrivateKey(privateKeyFile);

DHT_Unified dht(D4, DHT11);

void receiveMessage(char *topic, byte *payload, unsigned int length)
{
  Serial.println("Some message received");
}

void setupAwsIotWiFiClient()
{
  Serial.println("Setup AWS IoT Wi-Fi Client...");

  awsIotWiFiClient.setDebugOutput(true)
      .setCertificates(&trustAnchorCertificate, &clientCertificate, &clientPrivateKey)
      .setEndpoint(endpoint)
      .setReceiveMessageCallback(receiveMessage)
      .setClientId(clientId)
      .setSubscribeTopicFilter(subscribeTopicFilter)
      .connect();

  Serial.println("AWS IoT Wi-Fi Client setup was successful!");
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(SERIAL_BAUDRATE);

  // Wi-Fi connection setup.
  Serial.println(F("Establishing Wi-Fi connection..."));
  WiFiManager wifiManager;
  wifiManager.autoConnect(AUTOCONNECT_SSID, AUTOCONNECT_PASSWORD);
  Serial.println(F("Wi-Fi connection established"));

  setupAwsIotWiFiClient();

  // DHT11 setup.
  Serial.println(F("Setting up DHT11 sensor..."));
  dht.begin();
  Serial.println(F("DHT11 sensor set up"));
}

void loop()
{
  digitalWrite(LED_BUILTIN, HIGH);

  awsIotWiFiClient.loop();

  // @see https://arduinojson.org/v6/how-to/determine-the-capacity-of-the-jsondocument/
  DynamicJsonDocument json(1024);
  json["millis"] = millis();

  struct tm timeinfo;
  time_t now = time(nullptr);
  gmtime_r(&now, &timeinfo);
  json["time"] = asctime(&timeinfo);

  sensors_event_t event;

  dht.temperature().getEvent(&event);
  if (isnan(event.temperature))
  {
    Serial.println(F("Error reading temperature!"));
  }
  else
  {
    json["temperature"] = event.temperature;
  }

  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity))
  {
    Serial.println(F("Error reading humidity!"));
  }
  else
  {
    json["humidity"] = event.relative_humidity;
  }

  char message[1024];
  serializeJson(json, message);

  awsIotWiFiClient.publishMessage(publishTopicName, message);

  Serial.println(message);

  digitalWrite(LED_BUILTIN, LOW);

  delay(LOOP_DELAY);
}
