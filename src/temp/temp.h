#ifndef TEMP_H
#define TEMP_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include "DHT.h"

#define DHTPIN 19
#define DHTTYPE DHT11

extern JsonDocument temp;

void getTemp();

void registerTempRoute();

#endif
