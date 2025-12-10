#include "display_task.h"
#include "temp/temp.h"

DisplayTask::DisplayTask()
    : _enabled(true),     // display starts enabled
      _handle(nullptr),   // RTOS task handle
      _hasError(false),   // no error initially
      _u8g2(nullptr)      // OLED object pointer
{
    _errorMessage[0] = '\0'; // empty error message
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

// Set error message to display
void DisplayTask::setErrorMessage(const char* message) {
    if (message != nullptr && message[0] != '\0') {
        strncpy(_errorMessage, message, sizeof(_errorMessage) - 1);
        _errorMessage[sizeof(_errorMessage) - 1] = '\0'; // ensure null termination
        _hasError = true;
    } else {
        _errorMessage[0] = '\0';
        _hasError = false;
    }
}

// FreeRTOS task function
void DisplayTask::taskFunction(void* pvParameters) {
    DisplayTask* self = static_cast<DisplayTask*>(pvParameters);
    U8G2_SSD1306_128X64_NONAME_1_HW_I2C* u8g2 = self->_u8g2;

    // buffer for queue data: [temperature, humidity, validFlag]
    float data[3];
    bool wasEnabled = true; // Track previous state to know when to clear

    while (true) {

        if (self->_enabled) {
            wasEnabled = true;

            // Check if there's an error message to display
            if (self->_hasError) {
                // Display error message
                u8g2->firstPage();
                do {
                    u8g2->drawStr(0, 20, "ERROR!");
                    u8g2->drawStr(0, 40, self->_errorMessage);
                } while (u8g2->nextPage());
            } else {
                // Read latest sensor data (non-blocking)
                if (xQueueReceive(sensorDataQueue, &data, 0) == pdTRUE) {

                    float temperature = data[0];
                    float humidity = data[1];
                    bool valid = (data[2] == 1.0f);

                    if (valid) {
                        char tempCBuf[20];
                        char tempFBuf[20];
                        char humBuf[20];

                        // Convert temperature to Fahrenheit
                        float temperatureF = (temperature * 9.0 / 5.0) + 32.0;

                        sprintf(tempCBuf, "Temp: %.1f C", temperature);
                        sprintf(tempFBuf, "      %.1f F", temperatureF);
                        sprintf(humBuf,  "Humidity: %.1f%%", humidity);

                        // Draw using U8G2 page-by-page method
                        u8g2->firstPage();
                        do {
                            u8g2->drawStr(0, 14, tempCBuf);
                            u8g2->drawStr(0, 26, tempFBuf);
                            u8g2->drawStr(0, 44, humBuf);
                        } while (u8g2->nextPage());
                    }
                }
            }
        } else {
            // Display is disabled - clear the screen once
            if (wasEnabled) {
                u8g2->clearDisplay();
                wasEnabled = false;
            }
        }

        vTaskDelay(pdMS_TO_TICKS(200)); // wait 200 ms
    }
}