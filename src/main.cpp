#include <Arduino.h>
#include "display.h"
#include "UI_task.h"
#include "wifi/wifi.h"
#include "api/server.h"

// UI_task UI_t("UI_task", 4096, 2);
// UI_task UI_t2("UI_task2", 4096, 2);

void setup() {
  Serial.begin(9600);
  Serial.print("Starting Setup");
  connect_wifi();
  startServer();
  // Serial.println("hello world");
  // Serial.print(UI_t.isConstructed());
  // Serial.print(UI_t2.isConstructed());
  // Serial.println("UI taskHandle = "+String((uint32_t)UI_t.getTaskHandle()));
  // UI_t.run();
  // delay(10000);
}

void loop() {
  handleClient();
  delay(10);
  /*
  display("hello world");
  delay(100);
  display("This is a change from Raham not Naresh");
  */
}