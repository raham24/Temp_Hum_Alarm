#include "test.h"
#include "server.h"
#include <Arduino.h>

void handleTestRoute() {
    Serial.begin(9600);
    String message = "Hello from ESP32 API! Test route is working.";
    server.send(200, "text/plain", message);
    Serial.println("Test route accessed");
}

void registerTestRoutes() {
    Serial.begin(9600);
    server.on("/test", HTTP_GET, handleTestRoute);
    Serial.println("Test routes registered: /test");
}
