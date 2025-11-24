#include <WiFi.h>
#include "esp_wpa2.h"
#include "config.h"
#include "wifi.h"

void connect_wifi() {
    Serial.begin(9600);
    Serial.println("Connecting to WiFi...");

    WiFi.disconnect(true);
    WiFi.mode(WIFI_STA);

    esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY));
    esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY));
    esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD));
    esp_wifi_sta_wpa2_ent_enable();

    WiFi.begin(SSID);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nConnected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}