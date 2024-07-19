#include <Arduino.h>

#include <AwsIotWiFiClient.h>
#include <ESP8266WiFi.h>

// Uncomment the following to use the WiFiManager library.
// #include <DNSServer.h>
// #include <ESP8266WebServer.h>
// #include <WiFiManager.h>

#include "ConnectedHome.h"
#include "Secrets.h"

const int dhtPin = D4;
const char ssid[] = "ssid";
const char password[] = "password";

AwsIotWiFiClient awsIotWiFiClient;

BearSSL::X509List trustAnchorCertificate(rootCaCertificate);
BearSSL::X509List clientCertificate(deviceCertificate);
BearSSL::PrivateKey clientPrivateKey(privateKeyFile);

ConnectedHome connectedHome(LED_BUILTIN, dhtPin);

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

void publishMessage(const char *message)
{
  Serial.print("Publishing message: ");
  Serial.println(message);

  awsIotWiFiClient.publishMessage(publishTopicName, message);

  Serial.println("Message successfully published!");
}

void receiveMessage(const char *topic, const byte *payload, const unsigned int length)
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

void setupConnectedHome()
{
  Serial.println("Setting up Connected Home...");

  connectedHome.setLogLevel(Log::Level::LOG)
      .setPublishMessageCallback(publishMessage)
      .setup();

  Serial.println("Connected Home setup was successful!");
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Setup...");

  connectWiFi();
  // Comment the previous line and uncomment the following to use the WiFiManager library.
  // WiFiManager wifiManager;
  // wifiManager.autoConnect("Connected-Home");
  // Serial.println("Connected!");

  setupAwsIotWiFiClient();
  setupConnectedHome();

  Serial.println("Setup was successful!");
}

void loop()
{
  connectedHome.loop();
  awsIotWiFiClient.loop();
}
