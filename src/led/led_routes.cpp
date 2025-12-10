#include <Arduino.h>
#include <ArduinoJson.h>
#include "led_routes.h"
#include "led_task.h"
#include "../api/server.h"

// JSON document and string for LED state
JsonDocument ledDoc;
String ledJson;

// External reference to the global LED task instance from main.cpp
extern LedTask ledTask;

void handleLedOptions() {
    // Handle preflight OPTIONS request for CORS
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(200, "text/plain", "");
}

void handleLedGet() {
    // Get current LED state
    bool enabled = ledTask.getEnabled();

    ledDoc.clear();
    ledDoc["active"] = enabled;

    serializeJson(ledDoc, ledJson);

    // Send JSON response with CORS headers
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(200, "application/json", ledJson);

    Serial.println("LED state sent: " + ledJson);
}

void handleLedPost() {
    // Parse JSON body
    if (server.hasArg("plain")) {
        String body = server.arg("plain");

        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, body);

        if (error) {
            server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
            return;
        }

        if (doc.containsKey("active")) {
            bool active = doc["active"];
            ledTask.setEnabled(active);

            // Send success response
            ledDoc.clear();
            ledDoc["active"] = active;
            ledDoc["status"] = "success";

            serializeJson(ledDoc, ledJson);

            server.sendHeader("Access-Control-Allow-Origin", "*");
            server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
            server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
            server.send(200, "application/json", ledJson);

            Serial.print("LED set to: ");
            Serial.println(active ? "ACTIVE" : "INACTIVE");
        } else {
            server.send(400, "application/json", "{\"error\":\"Missing 'active' field\"}");
        }
    } else {
        server.send(400, "application/json", "{\"error\":\"No body provided\"}");
    }
}

void registerLedRoute() {
    server.on("/led", HTTP_GET, handleLedGet);
    server.on("/led", HTTP_POST, handleLedPost);
    server.on("/led", HTTP_OPTIONS, handleLedOptions);
    Serial.println("LED routes registered: /led (GET, POST)");
}
