#ifndef CONNECTEDHOME_H
#define CONNECTEDHOME_H

#include <ArduinoJson.h>

// DHT11
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

enum class LogLevel
{
  NONE,
  ERROR,
  WARNING,
  INFO
};

class ConnectedHome
{
public:
  ConnectedHome(int, int);
  ConnectedHome &setLogLevel(LogLevel);
  ConnectedHome &setPublishMessageCallback(const std::function<void(const char *)> &);
  void setup();
  void loop();

private:
  int ledPin;
  DHT_Unified dhtSensor;

  LogLevel logLevel = LogLevel::NONE;
  std::function<void(const char *)> publishMessageCallback;

  unsigned long previousMillis = 0;
  long interval = 10000;

  void log(LogLevel, const char *);
  void logln(LogLevel, const char *);
  void logln(LogLevel, const char *, boolean);
  void logln(LogLevel, unsigned long, boolean);
  void logln(LogLevel, float, boolean);
  void logln(LogLevel, time_t, boolean);

  void publishMessage();
};

#endif
