#ifndef TEMP_H
#define TEMP_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include "DHT.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "../pins.h"

#define DHTPIN DHT_PIN
#define DHTTYPE DHT11

extern JsonDocument temp;
extern String jsonTemp;
extern unsigned long lastTempReadTime;

void initTemp();
void getTemp();

void registerTempRoute();
void handleTempGet();
void updateTempReadings();

extern QueueHandle_t sensorDataQueue; // Queue to share readings

class SensorTask {
public:
    SensorTask();  // Constructor
    void begin();  // Initialize sensor and start FreeRTOS task

    // Getter functions for latest readings
    float getTemperature() { return _temperature; }
    float getHumidity() { return _humidity; }
    bool isValid() { return _valid; }

private:
    static void taskFunction(void* pvParameters); // FreeRTOS task function

    DHT* _dht;                // DHT sensor object
    TaskHandle_t _handle;     // RTOS task handle

    // Sensor data stored directly in the task
    float _temperature;       // Latest temperature
    float _humidity;          // Latest humidity
    bool _valid;              // True if reading is valid
};

extern SensorTask sensorTask;  // Global sensor task instance

#endif
