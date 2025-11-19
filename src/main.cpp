#include <Arduino.h>
#include "display.h"

void setup() {
  
  Serial.begin(9600);

}

void loop() {
  
  display("hello world");
  delay(100);
  display("This is a change from Raham not Naresh");
}