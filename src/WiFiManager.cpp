//
// WiFi 管理
//
#include <WiFiConfigureParameter.h>
#include <WiFiManager.h>
#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

//本地 wifi 配置
WiFiConfigureParameter mWiFiConfig = WiFiConfigureParameter();

/**
 * 本地 wifi 配置 连接
  * @return  true 连接成功 false 连接失败
  */
bool WiFiManager::onConnectWiFiConfigJson() {
    //判断 wifi 配置 是否有效
    if (onReadWiFiConfigJsonString() && mWiFiConfig.isValid()) {
        return true;
    }
    return false;
}

/**
 * 读取 本地缓存 wifi 配置
 * @return true 读取成功 false 读取失败
 */
bool WiFiManager::onReadWiFiConfigJsonString() {
    // 启动 LittleFS
    if (!LittleFS.begin()) {
        Serial.println("LittleFS 启用失败.");
        return false;
    }
    // 检查文件是否存在
    if (!LittleFS.exists(save_wifi_config_file)) {
        Serial.println("文件不存在.");
        return false;
    }
    // 打开文件
    File file = LittleFS.open(save_wifi_config_file, "r");
    if (!file) {
        Serial.println("文件打开失败.");
        return false;
    }
    file.readString();
    // char *json;
    while (file.available()) {
//        json += file.read();
        Serial.write(file.read());
    }
    file.close();
//    Serial.println("wifi json 为 : " );
//    if (json.size() > 0) {
//    } else {
    return false;
//    }
}
