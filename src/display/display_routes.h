#ifndef DISPLAY_ROUTES_H
#define DISPLAY_ROUTES_H

#include <Arduino.h>
#include <ArduinoJson.h>

extern JsonDocument displayDoc;
extern String displayJson;

void registerDisplayRoute();
void handleDisplayGet();
void handleDisplayPost();
void handleDisplayOptions();

#endif
