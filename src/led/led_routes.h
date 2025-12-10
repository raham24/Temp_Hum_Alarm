#ifndef LED_ROUTES_H
#define LED_ROUTES_H

#include <Arduino.h>
#include <ArduinoJson.h>

extern JsonDocument ledDoc;
extern String ledJson;

void registerLedRoute();
void handleLedGet();
void handleLedPost();
void handleLedOptions();

#endif
