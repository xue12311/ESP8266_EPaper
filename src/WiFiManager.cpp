//
// WiFi 管理
//
#include <WiFiConfigureParameter.h>
#include <WiFiManager.h>
#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

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

//wifi 连接 超时时间 为 10 秒
const long wifi_connect_timed_out_time = 5 * 1000;

// 建立ESP8266WebServer对象，对象名称为esp8266_server
// 括号中的数字是网路服务器响应http请求的端口号
// 网络服务器标准http端口号为80，因此这里使用80为端口号
ESP8266WebServer webServer(80);

//本地 wifi 配置
WiFiConfigureParameter mWiFiConfig = WiFiConfigureParameter();

/**
 * 本地 wifi 配置 连接
  * @return  true 连接成功 false 连接失败
  */
bool WiFiManager::onConnectWiFiConfigJson() {
    //判断 wifi 配置 是否有效
    if (onReadWiFiConfigJsonString() && mWiFiConfig.isValid()) {
        //连接 wifi
        return onConnectionWiFiChar(mWiFiConfig.getSSID(), mWiFiConfig.getPassword());
    }
    return false;
}

/**
 * 循环 wifi 配置
 */
void WiFiManager::onWebServerLoop() {
    if (WiFi.getMode() == WIFI_AP && !WiFi.isConnected()) {
        // 检查http服务器访问
        webServer.handleClient();
    }
}

/**
 * 设置 wifi 为 AP 模式,并启用网络服务
 * @return
 */
bool WiFiManager::onStartWiFiAPAndWebServer() {
    //设置 wifi 为 AP 模式
    if (onSettingsWifiAP()) {
        //启用 网络服务 用来 接收 wifi 配置 的 http 请求
        return onCreateWebServer();
    }
    return false;
}

/**
     * 设置 wifi 为 AP模式 接入点模式
     * 设置 wifi名称 密码
     * @return 是否设置成功  true 设置成功 false 设置失败
     */
bool WiFiManager::onSettingsWifiAP() {
    //设置为 AP 模式
    WiFi.mode(WIFI_AP);
    bool isSuccess = WiFi.softAP(ap_wifi_ssid, ap_wifi_password);
    Serial.println("wifi 名称 : " + String(ap_wifi_ssid));
    Serial.println("wifi 密码 : " + String(ap_wifi_password));
    Serial.println("wifi Ip地址 : " + WiFi.softAPIP().toString());
    return isSuccess;
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
    // 读取文件内容
    String jsonString = file.readString();
    Serial.println("wifi 缓存 配置 JSON : " + jsonString);
    file.close();
    if (jsonString.length() > 0) {
        return onJsonWiFiConfigAndConnectionWiFi(jsonString);
    } else {
        return false;
    }
}

/**
   * 解析 本地 json 并 尝试连接 wifi
   * @param json 本地缓存
   * @return true 连接成功 false 连接失败
   */
bool WiFiManager::onJsonWiFiConfigAndConnectionWiFi(String json) {
    return true;
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
    //等待 WiFi 连接结果
    if (WiFi.waitForConnectResult(wifi_connect_timed_out_time) != WL_CONNECTED) {
        Serial.println("wifi 连接失败.");
        return false;
    }
    Serial.println("WiFi 连接成功.");
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

/**
  * 启用 网络服务 用来 接收 wifi 配置
  */
bool WiFiManager::onCreateWebServer() {
    //启动网络服务功能
    webServer.begin();
//    webServer.keepAlive(true);
    //设置wifi配置
    webServer.on("/settings_wifi", HTTP_GET, [this]() {
        //接收 接口中的 wifi 配置信息
        String ssid = webServer.arg("wifi_ssid");
        String password = webServer.arg("wifi_password");
        Serial.println("收到请求参数 ssid: " + ssid + " password: " + password);
        //wifi 连接成功
        bool isWiFiConnectionSucceeded = onConnectionWiFiString(ssid, password);
        //请求响应 json
        String json = getWiFiConnectWebRequestJson(ssid, password, isWiFiConnectionSucceeded);
        Serial.println("请求响应 :" + json);
        webServer.send(200, "application/json", json);
        //保存 wifi 配置
        if (isWiFiConnectionSucceeded) {
            webServer.stop();
            //保存 wifi 配置
            onSaveWiFiConfigJson(json);
        }
    });
    //处理404情况
    webServer.onNotFound([]() {
        String json = "{\"code\":404,\"msg\":\"404错误 请求失败,请重试!\"}";
        Serial.println("请求响应 :" + json);
        webServer.send(404, "application/json", json);
    });
    webServer.begin();
    Serial.println("网络服务器启动成功.");
    return true;
}


/**
  * 响应 wifi连接 请求
  * @param wifi_ssid  wifi 名称
  * @param wifi_password   wifi 密码
  * @param isSuccess  true: wifi 连接成功 false: wifi 连接失败
  * @return json
  */
String WiFiManager::getWiFiConnectWebRequestJson(String wifi_ssid, String wifi_password, bool isSuccess) {
    String json = "{";
    if (isSuccess) {
        json += "\"code\":200,";
        json += "\"msg\":\"wifi 连接成功\",";
    } else {
        json += "\"code\":201,";
        json += "\"msg\":\"wifi 连接失败,请重试!\",";
    }
    json += "\"data\":{";
    json += "\"wifi_ssid\":\"" + wifi_ssid + "\",";
    json += "\"wifi_password\":\"" + wifi_password + "\",";
    if (isSuccess) {
        json += "\"wifi_local_ip\":\"" + WiFi.localIP().toString() + "\",";
    }
    json += "\"device_mac\":\"" + WiFi.macAddress() + "\"";
    json += "}}";
    return json;
}

bool WiFiManager::onSaveWiFiConfigJson(String json_wifi_config) {
    // 启动 LittleFS
    if (!LittleFS.begin()) {
        Serial.println("LittleFS 启用失败.");
        return false;
    }
    File file = LittleFS.open(save_wifi_config_file, "w");
    file.write(json_wifi_config.c_str());
    file.close();
    Serial.println("wifi 配置保存成功.");
//
//    StaticJsonDocument<384> doc;
//
//    DeserializationError error = deserializeJson(doc, input);
//
//    if (error) {
//        Serial.print(F("deserializeJson() failed: "));
//        Serial.println(error.f_str());
//        return;
//    }
//
//    int code = doc["code"]; // 200
//    const char* msg = doc["msg"]; // "wifi 连接成功"
//
//    JsonObject data = doc["data"];
//    const char* data_wifi_ssid = data["wifi_ssid"]; // "public void main () {}"
//    const char* data_wifi_password = data["wifi_password"]; // "zhangjiaxue"
//    const char* data_wifi_local_ip = data["wifi_local_ip"]; // "192.168.0.11"
//    const char* data_device_mac = data["device_mac"]; // "84:F3:EB:A5:D0:44"

    return true;
}