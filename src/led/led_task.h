#ifndef LED_TASK_H
#define LED_TASK_H

#include <Arduino.h>

class LedTask {
public:
    LedTask(uint8_t pin);       // Constructor: assign GPIO pin
    void begin();                // Initialize GPIO and create FreeRTOS task
    void setEnabled(bool enable);// Enable/disable LED (remote control)
    bool getEnabled() { return _enabled; } // Get current LED state
    void setAlarmCondition(bool active); // Set whether alarm condition (temp out of bounds) is active

private:
    static void taskFunction(void* pvParameters); // RTOS task function

    uint8_t _pin;          // GPIO pin connected to LED
    bool _enabled;         // Current LED state (user controlled)
    bool _alarmCondition;  // Whether alarm condition (temp out of bounds) is active
    TaskHandle_t _handle;  // FreeRTOS task handle
};

#endif