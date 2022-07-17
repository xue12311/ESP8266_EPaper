#include <Arduino.h>
#include <WiFiManager.h>

WiFiManager wifi_manager;

void setup() {
    Serial.begin(9600);
    //通过 已保存本地的wifi配置 进行wifi连接
    if (wifi_manager.onConnectWiFiConfigJson()) {
        Serial.println("连接 wifi 成功.");
    } else {
        Serial.println("连接 wifi 失败.");
    }

}

void loop() {
}