#include <Arduino.h>
#include <ArduinoJson.h>
#include "DHT.h"
#include "temp.h"
#include "../api/server.h"

DHT dht(DHTPIN,DHTTYPE);

JsonDocument temp;
String jsonTemp;
unsigned long lastTempReadTime = 0;
const unsigned long TEMP_READ_INTERVAL = 5000; // 5 seconds

void initTemp() {
    dht.begin();
    Serial.println("DHT sensor initialized");
}

void getTemp() {

    delay(500);
    // Read Humidity
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f = dht.readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f)) {
        Serial.println(F("Failed to read from DHT sensor!"));
        temp["error"] = "Failed to read from DHT sensor";
        return;
    }

    // else print info
    // Compute heat index in Fahrenheit (the default)
    float hif = dht.computeHeatIndex(f, h);
    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);

    temp.clear();
    temp["tempC"] = t;
    temp["tempF"] = f;
    temp["humidity"] = h;
    temp["heatC"] = hic;
    temp["heatF"] = hif;

}

void handleTempOptions() {
    // Handle preflight OPTIONS request - Needed for all SSE
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(200, "text/plain", "");
}

void handleTempGet() {
    // Get current temperature reading
    getTemp(); // change this to the sensor function defined for the sensor
    serializeJson(temp, jsonTemp);

    // Send JSON response with CORS headers - Needed for React Native
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(200, "application/json", jsonTemp);

    Serial.println("Temperature data sent: " + jsonTemp);
}

// Every 5 seconds, get new information from the sensor and send it to the front end
void updateTempReadings() {
    unsigned long currentTime = millis();

    // Update cached temperature reading every 5 seconds
    if (currentTime - lastTempReadTime >= TEMP_READ_INTERVAL) {
        getTemp();
        lastTempReadTime = currentTime;
        Serial.println("Temperature reading updated");
    }
}

void registerTempRoute() {
    server.on("/temp", HTTP_GET, handleTempGet);
    server.on("/temp", HTTP_OPTIONS, handleTempOptions);
    Serial.println("Temp routes registered: /temp (polling endpoint)");
}