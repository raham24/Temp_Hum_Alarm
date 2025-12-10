#include <Arduino.h>
#include <ArduinoJson.h>
#include "DHT.h"
#include "temp.h"
#include "../api/server.h"

// Global FreeRTOS queue for sharing sensor readings
QueueHandle_t sensorDataQueue = nullptr;

// Static DHT instance for utility functions (like heat index calculation)
DHT dht(DHTPIN, DHTTYPE);

// JSON document and string for temperature data
JsonDocument temp;
String jsonTemp;

unsigned long lastTempReadTime = 0;
const unsigned long TEMP_READ_INTERVAL = 5000; // 5 seconds

// SensorTask Constructor
SensorTask::SensorTask()
    : _dht(nullptr),         // DHT sensor object pointer
      _handle(nullptr),      // RTOS task handle
      _temperature(0.0f),    // initialize temperature
      _humidity(0.0f),       // initialize humidity
      _valid(false)          // initialize validity flag
{
}

// Initialize sensor and start FreeRTOS task
void SensorTask::begin() {
    // Create DHT instance on defined pin and type
    _dht = new DHT(DHTPIN, DHTTYPE);
    _dht->begin(); // initialize sensor
    Serial.println("DHT sensor initialized");

    // Create queue to hold 1 sensor reading (float[3]: temp, hum, valid)
    // Queue length must be 1 when using xQueueOverwrite()
    sensorDataQueue = xQueueCreate(1, 3 * sizeof(float));

    // Create FreeRTOS task for periodic sensor polling
    xTaskCreate(
        taskFunction,        // task function pointer
        "TEMP_SENSOR_TASK",  // task name
        4096,                // stack size (bytes)
        this,                // pass this object as parameter
        1,                   // task priority
        &_handle             // task handle
    );
}

// RTOS task function for polling DHT11
void SensorTask::taskFunction(void* pvParameters) {
    SensorTask* self = static_cast<SensorTask*>(pvParameters); // cast parameter to object

    while (true) { // infinite RTOS loop
        // read humidity and temperature from DHT
        float h = self->_dht->readHumidity();
        float t = self->_dht->readTemperature();

        // check for valid readings
        if (isnan(h) || isnan(t)) {
            self->_valid = false; // invalid reading
            Serial.println("Failed to read from DHT sensor!");
        } else {
            self->_temperature = t; // store temperature
            self->_humidity = h;    // store humidity
            self->_valid = true;    // mark reading valid
        }

        // Push data to queue as float array [temp, hum, valid]
        float data[3] = { self->_temperature, self->_humidity, self->_valid ? 1.0f : 0.0f };
        xQueueOverwrite(sensorDataQueue, &data);

        // Wait 2 seconds before next reading
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

// Initialize temperature sensor task
void initTemp() {
    sensorTask.begin();
}

// Get latest temperature readings and populate JSON
void getTemp() {
    // Get readings from sensor task
    float t = sensorTask.getTemperature();
    float h = sensorTask.getHumidity();
    bool valid = sensorTask.isValid();

    // Check if reading is valid
    if (!valid) {
        temp["error"] = "Failed to read from DHT sensor";
        return;
    }

    // Compute temperature in Fahrenheit
    float f = (t * 9.0 / 5.0) + 32.0;

    // Compute heat index in Fahrenheit
    float hif = dht.computeHeatIndex(f, h);

    // Compute heat index in Celsius
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