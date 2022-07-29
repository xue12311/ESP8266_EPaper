//
// WiFi 管理
//
#include <WiFiConfigureParameter.h>
#include <WiFiManager.h>
#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <list>

/**
 * AP 模式  WiFi 的 ip地址
 */
IPAddress wifi_ap_local_ip(192, 168, 4, 22);
/**
 * AP 模式  WiFi 的 网关IP地址
 */
IPAddress wifi_ap_gateway(192, 168, 4, 9);
/**
 * AP 模式  WiFi 的 子网掩码
 */
IPAddress wifi_ap_subnet(255, 255, 255, 0);

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

//wifi 连接 超时时间 为 15 秒
const long wifi_connect_timed_out_time = 15 * 1000;

// 建立ESP8266WebServer对象，对象名称为esp8266_server
// 括号中的数字是网路服务器响应http请求的端口号
// 网络服务器标准http端口号为80，因此这里使用80为端口号
ESP8266WebServer webServer(80);

// ESP8266 WebServer 是否已经启用
bool isWebServerEnable = false;

//本地 wifi 配置
WiFiConfigureParameter mWiFiConfig = WiFiConfigureParameter();

/**
 * 本地 wifi 配置 连接
  * @return  true 连接成功 false 连接失败
  */
bool WiFiManager::onConnectWiFiConfigJson() {
//    //判断 wifi 配置 是否有效
//    if (onReadWiFiConfigJsonString() && mWiFiConfig.isValid()) {
//        //设置为 STA 模式
//        WiFi.mode(WIFI_STA);
//        //连接 wifi
//        return onConnectionWiFiString(mWiFiConfig.getSSIDString(), mWiFiConfig.getPasswordString());
//    }
    getWiFiScanListJson();
    return false;
}

/**
 * 循环 wifi 配置
 */
void WiFiManager::onWebServerLoop() {
    if (isWebServerEnable) {
        // 检查http服务器访问
        webServer.handleClient();
    }
}

/**
 * 设置 wifi 为 AP 模式,并启用网络服务
 * @return
 */
bool WiFiManager::onStartWiFiAPAndWebServer() {
    isWebServerEnable = false;
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
    //设置为 AP_STA 模式
    WiFi.mode(WIFI_AP_STA);
    //配置接入点的IP，网关IP，子网掩码
    WiFi.softAPConfig(wifi_ap_local_ip, wifi_ap_gateway, wifi_ap_subnet);
    bool isSuccess = WiFi.softAP(ap_wifi_ssid, ap_wifi_password);
    Serial.println("wifi 名称 : " + String(ap_wifi_ssid));
    Serial.println("wifi 密码 : " + String(ap_wifi_password));
    Serial.println("wifi Ip地址 : " + WiFi.softAPIP().toString());
    Serial.println("网络IP :" + wifi_ap_local_ip.toString());
    Serial.println("网关IP :" + wifi_ap_gateway.toString());
    Serial.println("子网掩码 :" + wifi_ap_subnet.toString());
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
        return onJsonWiFiConfig(jsonString);
    } else {
        return false;
    }
}

/**
 * 解析 wifi 配置 json
 * @param json
 * @return
 */
bool WiFiManager::onJsonWiFiConfig(String json) {
    StaticJsonDocument<500> doc;
    DeserializationError error = deserializeJson(doc, json);
    if (error) {
        Serial.println("json 解析失败 : ");
        Serial.println(error.f_str());
        return false;
    }
    //请求返回 状态码
    int code = doc["code"].as<int>(); // 200
    Serial.println("json 解析  code 状态码 : " + String(code));
    //请求成功
    if (code == 200) {
        //数据详情
        JsonObject data = doc["data"];
        if (!doc["data"].isNull()) {
            String data_wifi_ssid = data["wifi_ssid"].as<String>();
            String data_wifi_password = data["wifi_password"].as<String>();
            //设置 wifi 配置
            mWiFiConfig.saveWiFiConfigure(data_wifi_ssid, data_wifi_password);
            Serial.println("json 解析 wifi名称 : " + data_wifi_ssid);
            Serial.println("json 解析 wifi密码 : " + data_wifi_password);
            return true;
        }
    }
    return false;
}

/**
 * 判断 是否扫描到 指定 wifi
 * @param wifi_ssid  指定 wifi 名称
 * @return true 扫描到指定 wifi false 没有扫描到指定 wifi
 */
bool WiFiManager::isSuccessfulScanWiFi(String wifi_ssid) {
    int n = WiFi.scanNetworks();
    if (n > 0) {
        for (int i = 0; i < n; i++) {
            if (wifi_ssid != nullptr && wifi_ssid.length() > 0) {
                if (WiFi.SSID(i).equals(wifi_ssid)) {
                    return true;
                }
            }
        }
    }
    return false;
}

/**
 * 获取当前扫描到的 wifi 列表
 * @return  扫描到的wifi列表
 */
String WiFiManager::getWiFiScanListJson() {
    int n = WiFi.scanNetworks();
    DynamicJsonDocument doc(1024);
    if (n <= 0) {
        doc["code"] = 201;
        doc["msg"] = "未扫描到附件WiFi!";
    } else {
        doc["code"] = 200;
        doc["msg"] = "扫描成功!";
        JsonObject data = doc.createNestedObject("data");
        JsonArray wifi_list = data.createNestedArray("wifi_list");
        JsonObject wifi_scann = wifi_list.createNestedObject();
        for (int i = 0; i < n; i++) {
            String wifi_ssid = WiFi.SSID(i);
            int wifi_rssi = WiFi.RSSI(i);
            int wifi_grade = 0;
            //信号等级
            if (wifi_rssi < -100) {
                wifi_grade = 0;
            } else if (wifi_rssi < -88) {
                wifi_grade = 1;
            } else if (wifi_rssi < -78) {
                wifi_grade = 2;
            } else if (wifi_rssi < -67) {
                wifi_grade = 3;
            } else if (wifi_rssi < 0) {
                wifi_grade = 4;
            }
            if (wifi_ssid != nullptr && wifi_ssid.length() > 0) {
//                Serial.println("WiFi 扫描 SSID: " + wifi_ssid + " RSSI: " + String(wifi_rssi));
                wifi_scann["wifi_ssid"] = wifi_ssid;
                wifi_scann["wifi_rssi"] = wifi_rssi;
                wifi_scann["wifi_grade"] = wifi_grade;
                wifi_list.add(wifi_scann);
            }
        }
    }
    String str_json;
    serializeJsonPretty(doc, str_json);
    doc.clear();
    Serial.println(str_json);
    return str_json;
}


/**
 * 获取 wifi 当前状态
 * @return
 */
String WiFiManager::getWiFiStatusString() {
    if (WiFi.status() == WL_CONNECTED) {
        return "wifi 连接成功";
    } else if (WiFi.status() == WL_CONNECT_FAILED) {
        return "wifi 连接失败";
    } else if (WiFi.status() == WL_CONNECTION_LOST) {
        return "wifi 连接丢失";
    } else if (WiFi.status() == WL_DISCONNECTED) {
        return "wifi 未连接";
    } else if (WiFi.status() == WL_NO_SSID_AVAIL) {
        return "wifi 没有找到设定的SSID的网络";
    } else if (WiFi.status() == WL_IDLE_STATUS) {
        return "wifi 正在尝试连接";
    } else if (WiFi.status() == WL_SCAN_COMPLETED) {
        return "wifi 网络扫描完毕";
    } else if (WiFi.status() == WL_NO_SHIELD) {
        return "wifi 没有找到可用设备";
    } else {
        return "wifi 未知状态";
    }
}

/**
 * wifi 连接
 * @param wifi_ssid wifi 名称
 * @param wifi_password wifi 密码
 * @return true 连接成功 false 连接失败
 */
bool WiFiManager::onConnectionWiFiChar(const char *wifi_ssid, const char *wifi_password) {
//    if (!isSuccessfulScanWiFi(wifi_ssid)) {
//        Serial.println("未扫描到指定 wifi");
//        return false;
//    }
    // 连接 wifi
    WiFi.begin(wifi_ssid, wifi_password);
    Serial.print("wifi 连接中");
    int count_time = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        count_time += 500;
        Serial.print(".");
        if (count_time > wifi_connect_timed_out_time) {
            Serial.println("");
            Serial.println("wifi 连接失败");
            return false;
        }
    }
    Serial.println("");
    Serial.println("wifi 连接成功");
    return true;
}

/**
 * wifi 连接
 * @param wifi_ssid wifi 名称
 * @param wifi_password wifi 密码
 * @return true 连接成功 false 连接失败
 */
bool WiFiManager::onConnectionWiFiString(String wifi_ssid, String wifi_password) {
    return onConnectionWiFiChar(wifi_ssid.c_str(), wifi_password.c_str());
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
            Serial.println("关闭 wifi 网络服务");
            //关闭 wifi 接入点模式
            WiFi.softAPdisconnect(true);
            //停用 Web Server
            webServer.stop();
            //停用 Web Server
            isWebServerEnable = false;
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
    isWebServerEnable = true;
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
        json += "\"msg\":\"";
        json += getWiFiStatusString();
        json += ",请重试!\",";
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

/**
 * 保存 wifi 配置
 * @param json_wifi_config
 * @return
 */
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
    return true;
}