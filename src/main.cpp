#include <Arduino.h>
#include "display/display_task.h"
#include "wifi/wifi.h"
#include "api/server.h"
#include "temp/temp.h"
#include "alarm/alarm_task.h"
#include "led/led_task.h"
#include "pins.h"

// Tasks
SensorTask sensorTask;      
DisplayTask displayTask;    
LedTask ledTask(LED_PIN);
AlarmTask alarmTask(BUZZER_PIN);

void setAlarmEnabled(bool enable) {
  alarmTask.setEnabled(enable);
  Serial.print("Alarm ");
  Serial.println(enable ? "ENABLED" : "DISABLED");
}

void setLedEnabled(bool enable) {
  ledTask.setEnabled(enable);
  Serial.print("LED ");
  Serial.println(enable ? "ENABLED" : "DISABLED");
}

void setDisplayEnabled(bool enable) {
  displayTask.setEnabled(enable);
  Serial.print("Display ");
  Serial.println(enable ? "ENABLED" : "DISABLED");
}

void setup() {
  Serial.begin(9600);
  Serial.println("Starting Setup");

  connect_wifi();
  initTemp();
  startServer();

  sensorTask.begin();     // DHT11 readings
  displayTask.begin();    // OLED display
  ledTask.begin();        // LED indicator
  alarmTask.begin();      // Alarm buzzer control

  // Initialize alarm task
  alarmTask.begin();
  Serial.println("Alarm Task initialized");

  // Initialize LED task
  ledTask.begin();
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