#include "led_task.h"

LedTask::LedTask(uint8_t pin)
    : _pin(pin), _enabled(false), _handle(nullptr) // initialize members
{
}

void LedTask::begin() {
    pinMode(_pin, OUTPUT);        // set LED pin as output
    digitalWrite(_pin, LOW);      // ensure LED starts off

    // create the FreeRTOS task
    xTaskCreate(taskFunction, "LED_TASK", 2048, this, 1, &_handle);
}

void LedTask::setEnabled(bool enable) {
    _enabled = enable;             // update LED state
}

void LedTask::taskFunction(void* pvParameters) {
    LedTask* self = static_cast<LedTask*>(pvParameters); // cast task parameter to object

    while (true) {
        digitalWrite(self->_pin, self->_enabled ? HIGH : LOW); // turn LED on/off
        vTaskDelay(pdMS_TO_TICKS(100));  // sleep 100 ms to avoid busy-wait
    }
}
