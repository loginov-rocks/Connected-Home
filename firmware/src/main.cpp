#include "Arduino.h"

#include "ArduinoJson.h"
#include "AwsIotWiFiClient.h"
#include "ESP8266WiFi.h"

// Uncomment the following to use the WiFiManager library.
// #include "DNSServer.h"
// #include "ESP8266WebServer.h"
// #include "WiFiManager.h"

// DHT11
#include "Adafruit_Sensor.h"
#include "DHT.h"
#include "DHT_U.h"

#include "Secrets.h"

const char ssid[] = "SSID";
const char password[] = "Password";

AwsIotWiFiClient awsIotWiFiClient;

BearSSL::X509List trustAnchorCertificate(rootCaCertificate);
BearSSL::X509List clientCertificate(deviceCertificate);
BearSSL::PrivateKey clientPrivateKey(privateKeyFile);

DHT_Unified dht(D4, DHT11);

unsigned long previousMillis = 0;
const long interval = 10000;

void connectWiFi()
{
  Serial.print("Connecting to Wi-Fi \"");
  Serial.print(ssid);
  Serial.print("\"...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print('.');
  }

  Serial.println();
  Serial.print("Local IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Wi-Fi connection was successful!");
}

void publishMessage()
{
  Serial.println("Publishing message...");

  // @see https://arduinojson.org/v6/how-to/determine-the-capacity-of-the-jsondocument/
  DynamicJsonDocument json(128);

  sensors_event_t event;

  // Humidity
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity))
  {
    Serial.println("Error reading humidity!");
  }
  else
  {
    json["humidity"] = event.relative_humidity;
  }

  // Temperature
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature))
  {
    Serial.println("Error reading temperature!");
  }
  else
  {
    json["temperature"] = event.temperature;
  }

  // Timestamp
  time_t now = time(nullptr);
  json["timestamp"] = now;

  char message[128];
  serializeJson(json, message);

  awsIotWiFiClient.publishMessage(publishTopicName, message);

  Serial.print("Message successfully published: ");
  Serial.println(message);
}

void receiveMessage(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message received: ");
  Serial.write(payload, length);
  Serial.println();
}

void setupAwsIotWiFiClient()
{
  Serial.println("Setting up AWS IoT Wi-Fi Client...");

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

  Serial.begin(9600);
  Serial.println("Setup...");

  connectWiFi();
  // Comment the previous line and uncomment the following to use the WiFiManager library.
  // WiFiManager wifiManager;
  // wifiManager.autoConnect("Connected-Home");
  // Serial.println("Connected!");

  setupAwsIotWiFiClient();

  // DHT11
  Serial.println("Setting up DHT11 sensor...");
  dht.begin();
  Serial.println("DHT11 sensor setup was successful!");

  Serial.println("Setup was successful!");
}

void loop()
{
  unsigned long currentMillis = millis();

  if (currentMillis > previousMillis + interval)
  {
    digitalWrite(LED_BUILTIN, HIGH);

    previousMillis = currentMillis;
    publishMessage();

    digitalWrite(LED_BUILTIN, LOW);
  }

  awsIotWiFiClient.loop();
}
