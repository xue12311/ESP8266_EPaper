//
// WiFi 管理
//
#include <WiFiConfigureParameter.h>
#include <WiFiManager.h>
#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/**
 * AP 模式  WiFi 名称
 */
const char *ap_wifi_ssid = "ESP8266 e-Paper";

/**
 * AP 模式  WiFi 密码
 */
const char *ap_wifi_password = "12345678";


// wifi 信息 保存地址
const char *save_wifi_config_file = "/wifi_config.json";

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
        Serial.println("wifi配置 文件不存在.");
        return false;
    }
    // 打开文件
    File file = LittleFS.open(save_wifi_config_file, "r");
    if (!file) {
        Serial.println("wifi配置 文件打开失败.");
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

/**
 * wifi 连接
 * @param wifi_ssid wifi 名称
 * @param wifi_password wifi 密码
 * @return true 连接成功 false 连接失败
 */
bool WiFiManager::onConnectionWiFiChar(char *wifi_ssid, char *wifi_password) {
    // 连接 wifi
    WiFi.begin(wifi_ssid, wifi_password);
    // 等待连接
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("连接成功.");
    return true;
}
/**
 * wifi 连接
 * @param wifi_ssid wifi 名称
 * @param wifi_password wifi 密码
 * @return true 连接成功 false 连接失败
 */
bool WiFiManager::onConnectionWiFiString(String wifi_ssid, String wifi_password) {
    //wifi名称
    char c_wifi_ssid[wifi_ssid.length() + 1];
    strcpy(c_wifi_ssid, wifi_ssid.c_str());
    //密码
    char c_wifi_password[wifi_password.length() + 1];
    strcpy(c_wifi_password, wifi_password.c_str());
    //连接wifi
    return onConnectionWiFiChar(c_wifi_ssid, c_wifi_password);
}
