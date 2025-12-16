#include <Arduino.h>
#include "display/display_task.h"
#include "wifi/wifi.h"
#include "server/server.h"
#include "temp/temp.h"
#include "alarm/alarm_task.h"
#include "led/led_task.h"
#include "pins.h"

#include "esp_sleep.h"
#include "esp_wifi.h"
#include "esp_pm.h"
#include <WiFi.h>

// Tasks
SensorTask sensorTask;      
DisplayTask displayTask;    
LedTask ledTask(LED_PIN);
AlarmTask alarmTask(BUZZER_PIN);

// Temperature monitoring - controls display error message and alarm condition
// Debounce variables to prevent false alarms from sensor noise
static int outOfRangeCount = 0;
static int inRangeCount = 0;
const int DEBOUNCE_THRESHOLD = 2; // Require 2 consecutive readings (~4 seconds)

void checkTemperatureAlarm() {
  float temperature = sensorTask.getTemperature();
  bool sensorValid = sensorTask.isValid();

  if (!sensorValid) {
    // If sensor fails, clear the alarm after some time to prevent stuck alarms
    outOfRangeCount = 0;
    inRangeCount++;

    if (inRangeCount >= DEBOUNCE_THRESHOLD) {
      alarmTask.setAlarmCondition(false);
      ledTask.setAlarmCondition(false);
      displayTask.setErrorMessage(nullptr);
    }
    return;
  }

  // Check temperature thresholds with hysteresis
  // Trigger at 30°C, but don't clear until 29°C (1 degree hysteresis)
  bool isOutOfRange = (temperature > 30.0 || temperature < 20.0);
  bool isSafeRange = (temperature <= 29.0 && temperature >= 21.0);

  if (isOutOfRange) {
    // Temperature is out of range
    outOfRangeCount++;
    inRangeCount = 0;

    // Only trigger alarm after consecutive out-of-range readings (debounce)
    if (outOfRangeCount >= DEBOUNCE_THRESHOLD) {
      alarmTask.setAlarmCondition(true);
      ledTask.setAlarmCondition(true);

      // Show error message on display
      if (temperature > 30.0) {
        displayTask.setErrorMessage("Temp too high!");
      } else {
        displayTask.setErrorMessage("Temp too low!");
      }
    }
  } else if (isSafeRange) {
    // Temperature is safely in normal range (with hysteresis)
    inRangeCount++;
    outOfRangeCount = 0;

    // Only clear alarm after consecutive in-range readings (debounce)
    if (inRangeCount >= DEBOUNCE_THRESHOLD) {
      alarmTask.setAlarmCondition(false);
      ledTask.setAlarmCondition(false);
      displayTask.setErrorMessage(nullptr);
    }
  }
  // If temperature is in the hysteresis zone (29-30 or 20-21), don't change counts
}

void setup() {
  Serial.begin(9600);
  Serial.println("Starting Setup");

  // === LOW POWER CONFIGURATION ===

  // 1. Reduce CPU frequency to save power (240MHz -> 80MHz saves ~40% power)
  setCpuFrequencyMhz(80);
  Serial.print("CPU Frequency set to: ");
  Serial.print(getCpuFrequencyMhz());
  Serial.println(" MHz");

  connect_wifi();

  // 2. Enable WiFi power save mode (modem sleeps between DTIM beacons)
  // WIFI_PS_MIN_MODEM: Balance between power saving and responsiveness
  WiFi.setSleep(WIFI_PS_MIN_MODEM);
  Serial.println("WiFi Power Save Mode enabled");

  // 3. Configure automatic light sleep for ESP32
  // This allows light sleep during delay() and idle periods
  esp_pm_config_esp32_t pm_config = {
    .max_freq_mhz = 80,
    .min_freq_mhz = 10,  // Scale down to 10MHz when idle
    .light_sleep_enable = true  // Enable automatic light sleep
  };
  esp_pm_configure(&pm_config);
  Serial.println("Power management configured");

  startServer();

  sensorTask.begin();     // DHT11 readings
  displayTask.begin();    // OLED display
  ledTask.begin();        // LED indicator
  alarmTask.begin();      // Alarm buzzer control

  Serial.println("All tasks initialized with low-power mode");
}

void loop() {
  // --- Normal periodic operations ---
  handleClient();
  updateTempReadings();
  checkTemperatureAlarm();

  // --- Power-saving delay ---
  // With automatic light sleep enabled, delay() will enter light sleep
  // WiFi wakes up automatically for beacon reception and incoming packets
  if (alarmTask.isAlarmCondition()) {
    // If alarm is active, use shorter delay for responsive LED/buzzer
    delay(10);
  } else {
    // Normal operation: longer delay allows deeper power savings
    // WiFi remains connected and wakes for incoming requests
    delay(50);
  }
}