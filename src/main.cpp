#include <Arduino.h>
#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>


const char *ap_wifi_ssid = "ESP8266 e-Paper";
const char *ap_wifi_password = "12345678";

// 建立ESP8266WebServer对象，对象名称为esp8266_server
// 括号中的数字是网路服务器响应http请求的端口号
// 网络服务器标准http端口号为80，因此这里使用80为端口号
ESP8266WebServer webServer(80);

// wifi 信息 保存地址
const char *save_wifi_file = "/wifi_config.json";

bool isBeginWebServer = false;

/**
 * 处理 wifi 配置信息
 */
void handleSettingsWiFiRoot();

/**
 * 连接wifi
 * @param wifi_ssid
 * @param wifi_password
 */
void onConnectionWiFi(String wifi_ssid, String wifi_password);

/**
 * 读取 本地保存的 wifi信息
 */
bool onReadWiFiJson() {
    isBeginWebServer = false;
    if (!LittleFS.begin()) { // 启动 LittleFS
        Serial.println("LittleFS 启用失败.");
        return false;
    }
    // 检查文件是否存在
    if (!LittleFS.exists(save_wifi_file)) {
        Serial.println("文件不存在.");
        return false;
    }
    // 打开文件
    File file = LittleFS.open(save_wifi_file, "r");
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

/**
 * 设置 wifi 为 AP模式 接入点模式
 */
void onSettingsWifiAP() {
    //设置为 AP 模式
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ap_wifi_ssid, ap_wifi_password);
    Serial.println("wifi 名称 : " + String(ap_wifi_ssid));
    Serial.println("wifi 密码 : " + String(ap_wifi_password));
    Serial.println("wifi Ip地址 : " + WiFi.softAPIP().toString());
}


void onCreateWebServer() {
    //启动网络服务功能
    webServer.begin();
    webServer.on("/settings_wifi", handleSettingsWiFiRoot);
    webServer.begin();
    isBeginWebServer = true;
    Serial.println("网络服务器启动成功.");
}

void handleSettingsWiFiRoot() {
    String ssid = webServer.arg("wifi_ssid");
    String password = webServer.arg("wifi_password");
    onConnectionWiFi(ssid, password);
    Serial.println("收到请求参数 ssid: " + ssid + " password: " + password);
    String json = "{\"code\":200,";
    json += "\"msg\":\"succeed\",";
    json += "\"data\":{";
    json += "\"wifi_ssid\":\"" + ssid + "\",";
    json += "\"wifi_password\":\"" + password + "\",";
    json += "\"wifi_local_ip\":\"" + WiFi.localIP().toString() + "\",";
    json += "\"device_mac\":\"" + WiFi.macAddress() + "\"";
    json += "}}";
    webServer.send(200, "application/json", json);
}

/**
 * 连接 wifi
 */
void onConnectionWiFi(String wifi_ssid, String wifi_password) {
    //关闭 AP 模式
    WiFi.softAPdisconnect(false);
    //设置为 STA 模式
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());
    Serial.print("WiFi连接中");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
}

/**
 * 循环 wifi 配置
 */
void onWiFiConfigLoop() {
    if (isBeginWebServer) {
        // 检查http服务器访问
        webServer.handleClient();
    }
}

void setup() {
    Serial.begin(9600);
    bool isSuccess = onReadWiFiJson();
    if (!isSuccess) {
        onSettingsWifiAP();
        onCreateWebServer();
    } else {
    }

}

void loop() {
    onWiFiConfigLoop();
}