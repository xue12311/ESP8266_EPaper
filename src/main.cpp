#include <Arduino.h>
#include <WiFiManager.h>
#include <UserConfigureInfo.h>

/**
 * wifi 连接
 */
WiFiManager wifi_manager;

/**
 * 用户配置信息
 */
UserConfigureInfo user_info;

void setup() {
    Serial.begin(9600);
    //设置 内置LED引脚 为 输出模式
    pinMode(LED_BUILTIN, OUTPUT);
    //通过 已保存本地的wifi配置 进行wifi连接
    if (wifi_manager.onConnectWiFiConfigJson()) {
        Serial.println("缓存 连接 wifi 成功.");
        //连接mqtt服务器
        wifi_manager.onConnectMqttService();
    } else {//缓存 连接 wifi 失败
        Serial.println("缓存 连接 wifi 失败.");
        //wifi连接
        if (wifi_manager.onSmartConfigWiFi()) {
            //连接mqtt服务器
            wifi_manager.onConnectMqttService();
        }
    }
}

void loop() {
    user_info.onUserConfigureLoop();
}

/**
  * 连接 mqtt 服务
  * @return true 连接成功 false 连接失败
  */
bool WiFiManager::onConnectMqttService() {
    //连接 mqtt 服务器
    bool isSuccess = user_info.onConnectMQTTServer();
    if (isSuccess) {
        //订阅 mqtt 主题
        isSuccess = user_info.onMQTTSubscribeTopics();
    }
    if (!isSuccess) {
        //创建web服务
        user_info.onCreateWebServer();
    }
    return isSuccess;
}
