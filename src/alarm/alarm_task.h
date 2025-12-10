#ifndef ALARM_TASK_H
#define ALARM_TASK_H

#include <Arduino.h>

class AlarmTask {
public:
    AlarmTask(uint8_t pin);       // Constructor: assign buzzer GPIO pin
    void begin();                 // Initialize GPIO and create FreeRTOS task
    void setEnabled(bool enable); // Enable/disable buzzer (remote control)
    bool getEnabled() { return _enabled; } // Get current alarm state
    void setAlarmCondition(bool active); // Set whether alarm condition (temp out of bounds) is active

private:
    static void taskFunction(void* pvParameters); // RTOS task function

    uint8_t _pin;          // GPIO pin for buzzer
    bool _enabled;         // Current alarm state (user controlled)
    bool _alarmCondition;  // Whether alarm condition (temp out of bounds) is active
    TaskHandle_t _handle;  // FreeRTOS task handle
};

#endif