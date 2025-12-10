#include <Arduino.h>
#include "display/display_task.h"
#include "wifi/wifi.h"
#include "server/server.h"
#include "temp/temp.h"
#include "alarm/alarm_task.h"
#include "led/led_task.h"
#include "pins.h"

#include "esp_sleep.h"
#include <WiFi.h>

// Tasks
SensorTask sensorTask;      
DisplayTask displayTask;    
LedTask ledTask(LED_PIN);
AlarmTask alarmTask(BUZZER_PIN);

// Temperature monitoring - controls display error message and alarm condition
void checkTemperatureAlarm() {
  float temperature = sensorTask.getTemperature();
  bool sensorValid = sensorTask.isValid();

  if (!sensorValid) {
    return; // Skip if sensor reading is invalid
  }

  // Check temperature thresholds
  if (temperature > 30.0 || temperature < 20.0) {
    // Out of range → alarm active
    alarmTask.setAlarmCondition(true);
    ledTask.setAlarmCondition(true);

    if (temperature > 30.0) {
      displayTask.setErrorMessage("Temp too high!");
    } else {
      displayTask.setErrorMessage("Temp too low!");
    }
  } else {
    // Normal temperature
    alarmTask.setAlarmCondition(false);
    ledTask.setAlarmCondition(false);
    displayTask.setErrorMessage(nullptr);
  }
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

  Serial.println("All tasks initialized");
}

void loop() {
  // --- Normal periodic operations ---
  handleClient();
  updateTempReadings();
  checkTemperatureAlarm();

  // --- Enter light sleep instead of busy waiting (delay) ---
  // Don't sleep if alarm is actively sounding
  if (!alarmTask.isAlarmCondition()) {
    WiFi.setSleep(true);  // Allow WiFi modem-sleep

    // Wake up after ~100 ms
    esp_sleep_enable_timer_wakeup(100 * 1000);

    // Enter light sleep (keeps RAM, tasks, WiFi)
    esp_light_sleep_start();
  } else {
    // If alarm is active, avoid sleep so LED/Buzzer respond instantly
    delay(10);
  }
}