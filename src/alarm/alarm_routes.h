#ifndef ALARM_ROUTES_H
#define ALARM_ROUTES_H

#include <Arduino.h>
#include <ArduinoJson.h>

extern JsonDocument alarmDoc;
extern String alarmJson;

void registerAlarmRoute();
void handleAlarmGet();
void handleAlarmPost();
void handleAlarmOptions();

#endif
