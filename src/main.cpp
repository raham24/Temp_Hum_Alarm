#include <Arduino.h>
#include "display.h"
#include "wifi/wifi.h"
#include "api/server.h"
#include "temp/temp.h"
#include "alarm_task.h"
#include "led_task.h"
#include "pins.h"

// Alarm task test - using centralized pin definition
AlarmTask alarmTask(BUZZER_PIN);

LedTask ledTask(LED_PIN);

void setup() {
  Serial.begin(9600);
  Serial.println("Starting Setup");
  connect_wifi();
  initTemp();
  startServer();

  // Initialize alarm task
  alarmTask.begin();
  ledTask.begin();
  Serial.println("Alarm Task initialized");
  Serial.println("LED Task initialized");
}

void loop() {
  handleClient();
  updateTempReadings();

  // Test alarm: Turn on for 5 seconds, off for 5 seconds
  static unsigned long lastToggle = 0;
  static bool alarmState = false;

  if (millis() - lastToggle > 5000) {
    alarmState = !alarmState;
    alarmTask.setEnabled(alarmState);
    Serial.print("Alarm ");
    Serial.println(alarmState ? "ON" : "OFF");
    lastToggle = millis();
  }

  // Test LED: Toggle every 1 second
  static unsigned long lastLedToggle = 0;
  static bool ledState = false;

  if (millis() - lastLedToggle > 1000) {
    ledState = !ledState;
    ledTask.setEnabled(ledState);
    Serial.print("LED ");
    Serial.println(ledState ? "ON" : "OFF");
    lastLedToggle = millis();
  }

  delay(10);
}