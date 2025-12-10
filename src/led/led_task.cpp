#include "led_task.h"

LedTask::LedTask(uint8_t pin)
    : _pin(pin), _enabled(false), _alarmCondition(false), _handle(nullptr) // initialize members
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

void LedTask::setAlarmCondition(bool active) {
    _alarmCondition = active;      // update alarm condition state
}

void LedTask::taskFunction(void* pvParameters) {
    LedTask* self = static_cast<LedTask*>(pvParameters); // cast task parameter to object

    while (true) {
        // Only flash LED when BOTH enabled AND alarm condition is active
        if (self->_enabled && self->_alarmCondition) {
            digitalWrite(self->_pin, HIGH); // turn LED on
            vTaskDelay(pdMS_TO_TICKS(200)); // on duration
            digitalWrite(self->_pin, LOW);  // turn LED off
            vTaskDelay(pdMS_TO_TICKS(200)); // off duration
        } else {
            digitalWrite(self->_pin, LOW);  // keep LED off if disabled
            vTaskDelay(pdMS_TO_TICKS(100)); // sleep 100 ms
        }
    }
}
