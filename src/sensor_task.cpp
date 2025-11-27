#include "sensor_task.h"

// Global FreeRTOS queue for sharing sensor readings
QueueHandle_t sensorDataQueue = nullptr;

SensorTask::SensorTask()
    : _dht(nullptr),         // DHT sensor object pointer
      _handle(nullptr),      // RTOS task handle
      _temperature(0.0f),    // initialize temperature
      _humidity(0.0f),       // initialize humidity
      _valid(false)          // initialize validity flag
{
}

void SensorTask::begin() {
    // Create DHT instance on defined pin and type
    _dht = new DHT(DHT_PIN, DHT_TYPE);
    _dht->begin(); // initialize sensor

    // Create queue to hold 5 sensor readings (float[3]: temp, hum, valid)
    sensorDataQueue = xQueueCreate(5, 3 * sizeof(float));

    // Create FreeRTOS task for periodic sensor polling
    xTaskCreate(
        taskFunction,        // task function pointer
        "SENSOR_TASK",       // task name
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
        } else {
            self->_temperature = t; // store temperature
            self->_humidity = h;    // store humidity
            self->_valid = true;    // mark reading valid
        }

        // Optional: push data to queue as float array [temp, hum, valid]
        float data[3] = { self->_temperature, self->_humidity, self->_valid ? 1.0f : 0.0f };
        xQueueOverwrite(sensorDataQueue, &data);

        // Wait 2 seconds before next reading
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}