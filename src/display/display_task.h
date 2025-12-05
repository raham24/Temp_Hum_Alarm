#ifndef DISPLAY_TASK_H
#define DISPLAY_TASK_H

#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

extern QueueHandle_t sensorDataQueue;

class DisplayTask {
public:
    DisplayTask();               // Constructor: initialize members
    void begin();                // Initialize OLED and create FreeRTOS task
    void setEnabled(bool enable);// Enable/disable display

private:
    static void taskFunction(void* pvParameters); // RTOS task function

    bool _enabled;               // Display enabled/disabled state
    TaskHandle_t _handle;        // FreeRTOS task handle

    U8G2_SSD1306_128X64_NONAME_1_HW_I2C* _u8g2; // OLED display object
};

#endif