#include <Arduino.h>
#include <ArduinoJson.h>
#include "display_routes.h"
#include "display_task.h"
#include "../api/server.h"

// JSON document and string for display state
JsonDocument displayDoc;
String displayJson;

// External reference to the global display task instance from main.cpp
extern DisplayTask displayTask;

void handleDisplayOptions() {
    // Handle preflight OPTIONS request for CORS
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(200, "text/plain", "");
}

void handleDisplayGet() {
    // Get current display state
    bool enabled = displayTask.getEnabled();

    displayDoc.clear();
    displayDoc["active"] = enabled;

    serializeJson(displayDoc, displayJson);

    // Send JSON response with CORS headers
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(200, "application/json", displayJson);

    Serial.println("Display state sent: " + displayJson);
}

void handleDisplayPost() {
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
            displayTask.setEnabled(active);

            // Send success response
            displayDoc.clear();
            displayDoc["active"] = active;
            displayDoc["status"] = "success";

            serializeJson(displayDoc, displayJson);

            server.sendHeader("Access-Control-Allow-Origin", "*");
            server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
            server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
            server.send(200, "application/json", displayJson);

            Serial.print("Display set to: ");
            Serial.println(active ? "ACTIVE" : "INACTIVE");
        } else {
            server.send(400, "application/json", "{\"error\":\"Missing 'active' field\"}");
        }
    } else {
        server.send(400, "application/json", "{\"error\":\"No body provided\"}");
    }
}

void registerDisplayRoute() {
    server.on("/display", HTTP_GET, handleDisplayGet);
    server.on("/display", HTTP_POST, handleDisplayPost);
    server.on("/display", HTTP_OPTIONS, handleDisplayOptions);
    Serial.println("Display routes registered: /display (GET, POST)");
}
