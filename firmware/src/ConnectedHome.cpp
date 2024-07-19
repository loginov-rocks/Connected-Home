#include "ConnectedHome.h"

ConnectedHome::ConnectedHome(int ledPin, int dhtSensorPin)
    : ledPin(ledPin),
      dhtSensor(dhtSensorPin, DHT11)
{
  //
}

ConnectedHome &ConnectedHome::setLogLevel(LogLevel logLevel)
{
  this->logLevel = logLevel;

  return *this;
}

ConnectedHome &ConnectedHome::setPublishMessageCallback(const std::function<void(const char *)> &callback)
{
  this->publishMessageCallback = callback;

  return *this;
}

void ConnectedHome::setup()
{
  this->logln(LogLevel::INFO, "Setting up Connected Home...");

  pinMode(this->ledPin, OUTPUT);

  this->logln(LogLevel::INFO, "Setting up DHT11 sensor...");

  this->dhtSensor.begin();

  this->logln(LogLevel::INFO, "DHT11 sensor setup was successful!");
  this->logln(LogLevel::INFO, "Connected Home setup was successful!");
}

void ConnectedHome::loop()
{
  unsigned long currentMillis = millis();

  if (currentMillis >= this->previousMillis + this->interval)
  {
    this->logln(LogLevel::INFO, "Executing task...");

    digitalWrite(this->ledPin, HIGH);

    this->previousMillis = currentMillis;

    this->log(LogLevel::INFO, "Current millis: ");
    this->logln(LogLevel::INFO, currentMillis, false);

    this->publishMessage();

    digitalWrite(this->ledPin, LOW);

    this->logln(LogLevel::INFO, "Task successfully executed!");
  }
}

void ConnectedHome::log(LogLevel logLevel, const char *message)
{
  Serial.print("[ConnectedHome] ");
  Serial.print(message);
}

void ConnectedHome::logln(LogLevel logLevel, const char *message)
{
  Serial.print("[ConnectedHome] ");
  Serial.println(message);
}

void ConnectedHome::logln(LogLevel logLevel, const char *message, boolean prefix)
{
  if (prefix)
  {
    Serial.print("[ConnectedHome] ");
  }

  Serial.println(message);
}

void ConnectedHome::logln(LogLevel logLevel, unsigned long number, boolean prefix)
{
  if (prefix)
  {
    Serial.print("[ConnectedHome] ");
  }

  Serial.println(number);
}

void ConnectedHome::logln(LogLevel logLevel, float number, boolean prefix)
{
  if (prefix)
  {
    Serial.print("[ConnectedHome] ");
  }

  Serial.println(number);
}

void ConnectedHome::logln(LogLevel logLevel, time_t timestamp, boolean prefix)
{
  if (prefix)
  {
    Serial.print("[ConnectedHome] ");
  }

  Serial.println(timestamp);
}

void ConnectedHome::publishMessage()
{
  boolean isDataAvailable = false;
  // @see https://arduinojson.org/v6/how-to/determine-the-capacity-of-the-jsondocument/
  DynamicJsonDocument json(128);

  sensors_event_t event;

  // Humidity
  this->logln(LogLevel::INFO, "Reading humidity...");

  this->dhtSensor.humidity().getEvent(&event);
  if (isnan(event.relative_humidity))
  {
    this->logln(LogLevel::WARNING, "Error reading humidity!");
  }
  else
  {
    isDataAvailable = true;
    json["humidity"] = event.relative_humidity;

    this->log(LogLevel::INFO, "Humidity: ");
    this->logln(LogLevel::INFO, event.relative_humidity, false);
  }

  // Temperature
  this->logln(LogLevel::INFO, "Reading temperature...");

  this->dhtSensor.temperature().getEvent(&event);
  if (isnan(event.temperature))
  {
    this->logln(LogLevel::WARNING, "Error reading temperature!");
  }
  else
  {
    isDataAvailable = true;
    json["temperature"] = event.temperature;

    this->log(LogLevel::INFO, "Temperature: ");
    this->logln(LogLevel::INFO, event.temperature, false);
  }

  if (!isDataAvailable)
  {
    this->logln(LogLevel::WARNING, "No data is available, skipping sending a message!");

    return;
  }

  // Timestamp
  time_t now = time(nullptr);
  json["timestamp"] = now;

  this->log(LogLevel::INFO, "Timestamp: ");
  this->logln(LogLevel::INFO, now, false);

  char message[128];
  serializeJson(json, message);

  this->log(LogLevel::INFO, "Message serialized: ");
  this->logln(LogLevel::INFO, message, false);
  this->logln(LogLevel::INFO, "Publishing message...");

  this->publishMessageCallback(message);

  this->logln(LogLevel::INFO, "Message successfully published!");
}
