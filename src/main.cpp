#include <Arduino.h>
#include <WiFiManager.h>
#include <MQTTManager.h>
/**
 * wifi 连接
 */
WiFiManager wifi_manager;
/**
 * mqtt 连接
 */
MQTTManager mqtt_manager;

void setup() {
    Serial.begin(9600);
    //通过 已保存本地的wifi配置 进行wifi连接
    if (wifi_manager.onConnectWiFiConfigJson()) {
        Serial.println("缓存 连接 wifi 成功.");
        wifi_manager.onConnectMqttService();
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
    mqtt_manager.onMQTTServerLoop();
}

/**
  * 连接 mqtt 服务
  * @return true 连接成功 false 连接失败
  */
bool WiFiManager::onConnectMqttService() {
    //初始化 mqtt 服务器
    mqtt_manager.initMqttServer();
    //连接 mqtt 服务器
    bool isSucceeded =mqtt_manager.onConnectMqttServer();
    return isSucceeded;
}
