#ifndef TEMP_H
#define TEMP_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include "DHT.h"

#define DHTPIN 19
#define DHTTYPE DHT11

extern JsonDocument temp;
extern unsigned long lastTempReadTime;

void initTemp();
void getTemp();

void registerTempRoute();
void handleTempGet();
void updateTempReadings();

#endif
