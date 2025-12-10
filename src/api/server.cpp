#include "server.h"
#include "test.h"
#include "../temp/temp.h"
#include "../alarm/alarm_routes.h"
#include "../led/led_routes.h"
#include "../display/display_routes.h"
#include <Arduino.h>

WebServer server(80);

void startServer() {
    Serial.begin(9600);
    // Register routes
    registerTestRoutes();
    registerTempRoute();
    registerAlarmRoute();
    registerLedRoute();
    registerDisplayRoute();

    // Start the server
    server.begin();
    Serial.println("HTTP server started on port 80");
    Serial.print("Server IP: ");
    Serial.println(WiFi.localIP());
}

void handleClient() {
    server.handleClient();
}
