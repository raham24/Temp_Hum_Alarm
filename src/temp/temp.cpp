#include <Arduino.h>
#include <ArduinoJson.h>
#include "DHT.h"
#include "temp.h"
#include "../api/server.h"

DHT dht(DHTPIN,DHTTYPE);

JsonDocument temp;
String jsonTemp;

void getTemp() {

    // Read Humidity
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f = dht.readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    }

    // else print info
    // Compute heat index in Fahrenheit (the default)
    float hif = dht.computeHeatIndex(f, h);
    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);

    temp["tempC"] = t;
    temp["tempF"] = f;
    temp["humidity"] = h;
    temp["heatC"] = hic;
    temp["heatF"] = hif;

}

void handleTempRoute() {
    Serial.begin(9600);

    // Add CORS headers
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    
    getTemp();
    serializeJson(temp, jsonTemp);
    
    server.send(200, "temp", jsonTemp);
    Serial.println("Temp route accessed");
}

void handleTempOptions() {
    // Handle preflight OPTIONS request
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(204);
}

void registerTempRoute() {
    Serial.begin(9600);
    server.on("/temp", HTTP_GET, handleTempRoute);
    server.on("/temp", HTTP_OPTIONS, handleTempOptions);
    Serial.println("Test routes registered: /temp");
}
