#include <Arduino.h>
#include "display_task.h"
#include "led_task.h"
#include "alarm_task.h"
#include "UI_task.h"

#include "wifi/wifi.h"
#include "api/server.h"

#include "temp/temp.h"
//#include "sensor_task.h"

SensorTask sensorTask;      
DisplayTask displayTask;    
LedTask ledTask;
AlarmTask alarmTask;

UI_task UI_t("UI_task", 4096, 2);
UI_task UI_t2("UI_task2", 4096, 2);
//Sensor sensor("sensor_task", 4096, 2);

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
}

void loop() {
  handleClient();
  updateTempReadings();
  delay(10);
  /*
  display("hello world");
  delay(100);
  display("This is a change from Raham not Naresh");
  */
}