#include <Arduino.h>
#include "display_task.h"
#include "led_task.h"
#include "alarm_task.h"
#include "wifi/wifi.h"
#include "api/server.h"
#include "temp/temp.h"
#include "alarm_task.h"
#include "led_task.h"
#include "pins.h"

SensorTask sensorTask;      
DisplayTask displayTask;    
LedTask ledTask;
AlarmTask alarmTask;

UI_task UI_t("UI_task", 4096, 2);
UI_task UI_t2("UI_task2", 4096, 2);
//Sensor sensor("sensor_task", 4096, 2);
// Alarm task test - using centralized pin definition
AlarmTask alarmTask(BUZZER_PIN);

LedTask ledTask(LED_PIN);

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
  // Serial.println("hello world");
  // Serial.print(UI_t.isConstructed());
  // Serial.print(UI_t2.isConstructed());
  // Serial.println("UI taskHandle = "+String((uint32_t)UI_t.getTaskHandle()));
  // UI_t.run();
  // delay(10000);
  //initSerial(9600);
  Serial.println("hello world");
  Serial.print(UI_t.isConstructed());
  Serial.print(UI_t2.isConstructed());
  Serial.println("UI taskHandle = "+String((uint32_t)UI_t.getTaskHandle()));
  UI_t.run();
  delay(10000);

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