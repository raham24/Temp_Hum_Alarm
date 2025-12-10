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


// Temperature monitoring - controls display error message and alarm condition
void checkTemperatureAlarm() {
  // Get current temperature from sensor task
  float temperature = sensorTask.getTemperature();
  bool sensorValid = sensorTask.isValid();

  if (!sensorValid) {
    return; // Skip if sensor reading is invalid
  }

  // Check temperature thresholds
  if (temperature > 30.0 || temperature < 20.0) {
    // Temperature is out of range
    // Set alarm condition active - buzzer/LED will only work if also enabled by user
    alarmTask.setAlarmCondition(true);
    ledTask.setAlarmCondition(true);

    // Show error message on display
    if (temperature > 30.0) {
      displayTask.setErrorMessage("Temp too high!");
    } else {
      displayTask.setErrorMessage("Temp too low!");
    }
  } else {
    // Temperature is normal
    // Deactivate alarm condition - buzzer/LED will stop even if enabled
    alarmTask.setAlarmCondition(false);
    ledTask.setAlarmCondition(false);

    // Clear error message
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
  handleClient();
  updateTempReadings();
  checkTemperatureAlarm();

  delay(10);
}