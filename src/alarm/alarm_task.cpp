#include "alarm_task.h"

AlarmTask::AlarmTask(uint8_t pin)
    : _pin(pin), _enabled(false), _alarmCondition(false), _handle(nullptr) // initialize members
{
}

void AlarmTask::begin() {
    pinMode(_pin, OUTPUT);        // set buzzer pin as output
    digitalWrite(_pin, LOW);      // ensure buzzer is off

    // create FreeRTOS task for pulsing alarm
    xTaskCreate(taskFunction, "ALARM_TASK", 2048, this, 1, &_handle);
}

void AlarmTask::setEnabled(bool enable) {
    _enabled = enable;             // update buzzer state
}

void AlarmTask::setAlarmCondition(bool active) {
    _alarmCondition = active;      // update alarm condition state
}

void AlarmTask::taskFunction(void* pvParameters) {
    AlarmTask* self = static_cast<AlarmTask*>(pvParameters); // cast to object

    while (true) {                 // infinite loop
        // Only sound buzzer when BOTH enabled AND alarm condition is active
        if (self->_enabled && self->_alarmCondition) {
            digitalWrite(self->_pin, HIGH); // turn buzzer on
            vTaskDelay(pdMS_TO_TICKS(200)); // beep duration
            digitalWrite(self->_pin, LOW);  // turn buzzer off
            vTaskDelay(pdMS_TO_TICKS(200)); // off duration
        } else {
            digitalWrite(self->_pin, LOW);  // keep buzzer off if disabled
            vTaskDelay(pdMS_TO_TICKS(300)); // sleep 300 ms
        }
    }
}