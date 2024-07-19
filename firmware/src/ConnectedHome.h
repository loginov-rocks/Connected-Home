#ifndef CONNECTEDHOME_H
#define CONNECTEDHOME_H

#include <ArduinoJson.h>
#include <Log.h>

// DHT11
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

class ConnectedHome
{
public:
  ConnectedHome(int, int);
  ConnectedHome &setLogLevel(Log::Level);
  ConnectedHome &setPublishMessageCallback(const std::function<void(const char *)> &);
  void setup();
  void loop();

private:
  Log log;
  int ledPin;
  DHT_Unified dhtSensor;

  std::function<void(const char *)> publishMessageCallback;

  unsigned long previousMillis = 0;
  long interval = 10000;

  void publishMessage();
};

#endif
