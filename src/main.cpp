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
    //初始化 mqtt 服务器
    mqtt_manager.initMqttServer();
    //通过 已保存本地的wifi配置 进行wifi连接
    if (wifi_manager.onConnectWiFiConfigJson()) {
        Serial.println("缓存 连接 wifi 成功.");
        //连接 mqtt 成功
        if (wifi_manager.onConnectMqttService()) {
            //订阅 mqtt 主题
            wifi_manager.onSubscribeMqttTopic();
        }
    } else {//缓存 连接 wifi 失败
        Serial.println("缓存 连接 wifi 失败.");
        //wifi连接
        if (wifi_manager.onSmartConfigWiFi()) {
            //连接 mqtt 成功
            if (wifi_manager.onConnectMqttService()) {
                //订阅 mqtt 主题
                wifi_manager.onSubscribeMqttTopic();
            }
        }
    }
}

void loop() {
    //wifi 连接成功
    if (wifi_manager.isWiFiConnected()) {
        mqtt_manager.onMQTTServerLoop();
    }
}

/**
  * 连接 mqtt 服务
  * @return true 连接成功 false 连接失败
  */
bool WiFiManager::onConnectMqttService() {
    return mqtt_manager.onConnectMqttServer();
}

/**
 * 订阅 mqtt主题
 */
void WiFiManager::onSubscribeMqttTopic() {
    mqtt_manager.onSubscribeMqttTopic();
}