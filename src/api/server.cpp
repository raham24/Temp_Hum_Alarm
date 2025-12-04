#include "server.h"
#include "test.h"
#include "../temp/temp.h"
#include <Arduino.h>

WebServer server(80);

void startServer() {
    Serial.begin(9600);
    // Register routes
    registerTestRoutes();
    registerTempRoute();

    // Start the server
    server.begin();
    Serial.println("HTTP server started on port 80");
    Serial.print("Server IP: ");
    Serial.println(WiFi.localIP());
}

void handleClient() {
    server.handleClient();
}
