#include "test.h"
#include "server.h"
#include <Arduino.h>

void handleTestRoute() {
    Serial.begin(9600);

    // Add CORS headers
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");

    String message = "Hello from ESP32 API! Test route is working.";
    server.send(200, "text/plain", message);
    Serial.println("Test route accessed");
}

void handleTestOptions() {
    // Handle preflight OPTIONS request
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(204);
}

void registerTestRoutes() {
    Serial.begin(9600);
    server.on("/test", HTTP_GET, handleTestRoute);
    server.on("/test", HTTP_OPTIONS, handleTestOptions);
    Serial.println("Test routes registered: /test");
}
