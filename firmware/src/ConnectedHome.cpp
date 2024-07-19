#include "ConnectedHome.h"

ConnectedHome::ConnectedHome(int ledPin, int dhtSensorPin)
    : log(),
      ledPin(ledPin),
      dhtSensor(dhtSensorPin, DHT11)
{
  this->log.setPrefix("[ConnectedHome]");
}

ConnectedHome &ConnectedHome::setLogLevel(Log::Level level)
{
  this->log.setLevel(level);

  return *this;
}

ConnectedHome &ConnectedHome::setPublishMessageCallback(const std::function<void(const char *)> &callback)
{
  this->publishMessageCallback = callback;

  return *this;
}

void ConnectedHome::setup()
{
  this->log.println("Setting up Connected Home...");

  pinMode(this->ledPin, OUTPUT);

  this->log.println("Setting up DHT11 sensor...");

  this->dhtSensor.begin();

  this->log.println("DHT11 sensor setup was successful!");
  this->log.println("Connected Home setup was successful!");
}

void ConnectedHome::loop()
{
  unsigned long currentMillis = millis();

  if (currentMillis >= this->previousMillis + this->interval)
  {
    this->log.println("Executing task...");

    digitalWrite(this->ledPin, HIGH);

    this->previousMillis = currentMillis;

    this->log.print("Current millis: ");
    this->log.println(currentMillis);

    this->publishMessage();

    digitalWrite(this->ledPin, LOW);

    this->log.println("Task successfully executed!");
  }
}

void ConnectedHome::publishMessage()
{
  boolean isDataAvailable = false;
  // @see https://arduinojson.org/v6/how-to/determine-the-capacity-of-the-jsondocument/
  DynamicJsonDocument json(128);

  sensors_event_t event;

  // Humidity
  this->log.println("Reading humidity...");

  this->dhtSensor.humidity().getEvent(&event);
  if (isnan(event.relative_humidity))
  {
    this->log.warnln("Error reading humidity!");
  }
  else
  {
    isDataAvailable = true;
    json["humidity"] = event.relative_humidity;

    this->log.print("Humidity: ");
    this->log.println(event.relative_humidity);
  }

  // Temperature
  this->log.println("Reading temperature...");

  this->dhtSensor.temperature().getEvent(&event);
  if (isnan(event.temperature))
  {
    this->log.warnln("Error reading temperature!");
  }
  else
  {
    isDataAvailable = true;
    json["temperature"] = event.temperature;

    this->log.print("Temperature: ");
    this->log.println(event.temperature);
  }

  if (!isDataAvailable)
  {
    this->log.warnln("No data is available, skipping sending a message!");

    return;
  }

  // Timestamp
  time_t now = time(nullptr);
  json["timestamp"] = now;

  this->log.print("Timestamp: ");
  this->log.println(now);

  char message[128];
  serializeJson(json, message);

  this->log.print("Message serialized: ");
  this->log.println(message);
  this->log.println("Publishing message...");

  this->publishMessageCallback(message);

  this->log.println("Message successfully published!");
}
