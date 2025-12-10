#include <Arduino.h>
#include <ArduinoJson.h>
#include "alarm_routes.h"
#include "alarm_task.h"
#include "../server/server.h"

// JSON document and string for alarm state
JsonDocument alarmDoc;
String alarmJson;

// External reference to the global alarm task instance from main.cpp
extern AlarmTask alarmTask;

void handleAlarmOptions() {
    // Handle preflight OPTIONS request for CORS
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(200, "text/plain", "");
}

void handleAlarmGet() {
    // Get current alarm state
    bool enabled = alarmTask.getEnabled();

    alarmDoc.clear();
    alarmDoc["active"] = enabled;

    serializeJson(alarmDoc, alarmJson);

    // Send JSON response with CORS headers
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(200, "application/json", alarmJson);

    Serial.println("Alarm state sent: " + alarmJson);
}

void handleAlarmPost() {
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
            alarmTask.setEnabled(active);

            // Send success response
            alarmDoc.clear();
            alarmDoc["active"] = active;
            alarmDoc["status"] = "success";

            serializeJson(alarmDoc, alarmJson);

            server.sendHeader("Access-Control-Allow-Origin", "*");
            server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
            server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
            server.send(200, "application/json", alarmJson);

            Serial.print("Alarm set to: ");
            Serial.println(active ? "ACTIVE" : "INACTIVE");
        } else {
            server.send(400, "application/json", "{\"error\":\"Missing 'active' field\"}");
        }
    } else {
        server.send(400, "application/json", "{\"error\":\"No body provided\"}");
    }
}

void registerAlarmRoute() {
    server.on("/alarm", HTTP_GET, handleAlarmGet);
    server.on("/alarm", HTTP_POST, handleAlarmPost);
    server.on("/alarm", HTTP_OPTIONS, handleAlarmOptions);
    Serial.println("Alarm routes registered: /alarm (GET, POST)");
}
