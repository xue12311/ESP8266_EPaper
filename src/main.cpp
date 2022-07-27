#include <Arduino.h>
#include <WiFiManager.h>
#include <WiFiConfigureParameter.h>

WiFiManager wifi_manager;

//wifi 连接 超时时间 为 15 秒
const long wifi_connect_timed_out_time = 15 * 1000;

void setup() {
    Serial.begin(9600);
    //通过 已保存本地的wifi配置 进行wifi连接
    if (wifi_manager.onConnectWiFiConfigJson()) {
        Serial.println("缓存 连接 wifi 成功.");
    } else {
        Serial.println("缓存 连接 wifi 失败.");
        if (wifi_manager.onStartWiFiAPAndWebServer()) {
            Serial.println("启用网络服务成功.");
        } else {
            Serial.println("启用网络服务失败.");
        }
    }

}

void loop() {
    wifi_manager.onWebServerLoop();
}

