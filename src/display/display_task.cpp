#include "display_task.h"
#include "temp.h"

DisplayTask::DisplayTask()
    : _enabled(true),     // display starts enabled
      _handle(nullptr),   // RTOS task handle
      _u8g2(nullptr)      // OLED object pointer
{
}

void DisplayTask::begin() {
    // Create U8G2 OLED object
    _u8g2 = new U8G2_SSD1306_128X64_NONAME_1_HW_I2C(
        U8G2_R0,
        U8X8_PIN_NONE
    );

    _u8g2->begin();                 // initialize display
    _u8g2->enableUTF8Print();       // allow UTF-8 printing
    _u8g2->setFont(u8g2_font_ncenB08_tr); // set visible font

    // Create FreeRTOS task for periodic display updates
    xTaskCreate(
        taskFunction,
        "DISPLAY_TASK",
        4096,
        this,
        1,
        &_handle
    );
}

// Enable/disable display output
void DisplayTask::setEnabled(bool enable) {
    _enabled = enable;
}

// FreeRTOS task function
void DisplayTask::taskFunction(void* pvParameters) {
    DisplayTask* self = static_cast<DisplayTask*>(pvParameters);
    U8G2_SSD1306_128X64_NONAME_1_HW_I2C* u8g2 = self->_u8g2;

    // buffer for queue data: [temperature, humidity, validFlag]
    float data[3];

    while (true) {

        if (self->_enabled) {

            // Read latest sensor data (non-blocking)
            if (xQueueReceive(sensorDataQueue, &data, 0) == pdTRUE) {

                float temperature = data[0];
                float humidity = data[1];
                bool valid = (data[2] == 1.0f);

                if (valid) {
                    char tempBuf[20];
                    char humBuf[20];

                    sprintf(tempBuf, "Temp: %.1f C", temperature);
                    sprintf(humBuf,  "Humidity: %.1f%%", humidity);

                    // Draw using U8G2 page-by-page method
                    u8g2->firstPage();
                    do {
                        u8g2->drawStr(0, 14, tempBuf);
                        u8g2->drawStr(0, 32, humBuf);
                    } while (u8g2->nextPage());
                }
            }
        }

        vTaskDelay(pdMS_TO_TICKS(200)); // wait 200 ms
    }
}