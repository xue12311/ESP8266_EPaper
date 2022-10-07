//
// WiFi 管理
//
#include <WiFiConfigureParameter.h>
#include <WiFiManager.h>
#include <BasicConfigure.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

//本地 wifi 配置
WiFiConfigureParameter mWiFiConfig = WiFiConfigureParameter();

/**
 * 配置 wifi
 */
bool WiFiManager::onSmartConfigWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.beginSmartConfig();
    Serial.println("wifi配置中");
    int count_time = 0;
    while (WiFi.status() != WL_CONNECTED) {
        // 改变LED的点亮或者熄灭状态
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        delay(500);
        count_time += 500;
        Serial.print(".");
        if (WiFi.smartConfigDone()) {
            Serial.println("");
            Serial.println("已收到 wifi 配置信息");
            String ssid = WiFi.SSID();
            String password = WiFi.psk();
            Serial.println("wifi ssid: " + ssid);
            Serial.println("wifi password: " + password);
            break;
        } else if (count_time > wifi_smart_config_timed_out_time) {
            Serial.println("");
            Serial.println("wifi 配置失败");
            //熄灭 LED
            digitalWrite(LED_BUILTIN, HIGH);
            //停止配网
            WiFi.stopSmartConfig();
            return false;
        }
    }
    count_time = 0;
    Serial.print("wifi 连接中");
    while (WiFi.status() != WL_CONNECTED) {
        // 改变LED的点亮或者熄灭状态
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        delay(500);
        count_time += 500;
        Serial.print(".");
        if (count_time > wifi_connect_timed_out_time) {
            Serial.println("");
            Serial.println("wifi 连接失败");
            //熄灭 LED
            digitalWrite(LED_BUILTIN, HIGH);
            //停止配网
            WiFi.stopSmartConfig();
            return false;
        }
    }
    if (WiFi.status() == WL_CONNECTED) {
        //写入缓存
        onWriteWiFiConfigJsonString();
    }
    //熄灭 LED
    digitalWrite(LED_BUILTIN, HIGH);
    //停止配网
    WiFi.stopSmartConfig();
    return true;
}


/**
 * 本地 wifi 配置 连接
  * @return  true 连接成功 false 连接失败
  */
bool WiFiManager::onConnectWiFiConfigJson() {
    //判断 wifi 配置 是否有效
    if (onReadWiFiConfigJsonString() && mWiFiConfig.isValid()) {
        //设置为 STA 模式
        WiFi.mode(WIFI_STA);
        //连接 wifi
        return onConnectionWiFiString(mWiFiConfig.getSSIDString(), mWiFiConfig.getPasswordString());
    }
    return false;
}


/**
 * 读取 本地缓存 wifi 配置
 * @return true 读取成功 false 读取失败
 */
bool WiFiManager::onReadWiFiConfigJsonString() {
    // 读取文件内容
    String jsonString = onReadLocalCacheJsonString(save_wifi_config_file);
    if (jsonString.length() > 0) {
        return onJsonWiFiConfig(jsonString);
    } else {
        return false;
    }
}

/**
 * 写入 本地 wifi 配置的 json 数据
 * @return true 写入成功  false 写入失败
 */
bool WiFiManager::onWriteWiFiConfigJsonString() {
    String ssid = WiFi.SSID();
    String password = WiFi.psk();
    String status = getWiFiStatusString(WiFi.status());
    String local_ip = WiFi.localIP().toString();
    Serial.println("");
    Serial.println("wifi ssid: " + ssid);
    Serial.println("wifi password: " + password);
    Serial.println("wifi status: " + status);
    Serial.println("wifi local_ip: " + local_ip);
    DynamicJsonDocument doc(500);
    doc["code"] = 200;
    doc["msg"] = "WiFi 连接成功!";
    JsonObject data = doc.createNestedObject("data");
    data["wifi_status"] = WiFi.status();
    data["wifi_status_name"] = status;
    data["wifi_ssid"] = ssid;
    data["wifi_password"] = password;
    data["wifi_local_ip"] = local_ip;
    data["device_mac"] = WiFi.macAddress();
    data["device_chip_id"] = ESP.getChipId();
    String str_json;
    serializeJsonPretty(doc, str_json);
    doc.clear();
    //保存 wifi 配置
    return onSaveWiFiConfigJson(str_json);
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
 * 获取 wifi 当前状态
 * @return
 */
String WiFiManager::getWiFiStatusString() {
    return getWiFiStatusString(WiFi.status());
}

/**
 * 获取 wifi 当前状态
 * @return
 */
String WiFiManager::getWiFiStatusString(int status) {
    if (status == WL_CONNECTED) {
        return "wifi 连接成功";
    } else if (status == WL_CONNECT_FAILED) {
        return "wifi 连接失败";
    } else if (status == WL_CONNECTION_LOST) {
        return "wifi 连接丢失";
    } else if (status == WL_DISCONNECTED) {
        return "wifi 未连接";
    } else if (status == WL_NO_SSID_AVAIL) {
        return "wifi 没有找到设定的SSID的网络";
    } else if (status == WL_IDLE_STATUS) {
        return "wifi 正在尝试连接";
    } else if (status == WL_SCAN_COMPLETED) {
        return "wifi 网络扫描完毕";
    } else if (status == WL_NO_SHIELD) {
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
    // 连接 wifi
    WiFi.begin(wifi_ssid, wifi_password);
    Serial.print("wifi 连接中");
    int count_time = 0;
    while (WiFi.status() != WL_CONNECTED) {
        // 改变LED的点亮或者熄灭状态
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        delay(500);
        count_time += 500;
        Serial.print(".");
        if (count_time > wifi_connect_timed_out_time) {
            Serial.println("");
            Serial.println("wifi 连接失败");
            //熄灭 LED
            digitalWrite(LED_BUILTIN, HIGH);
            return false;
        }
    }
    Serial.println("");
    Serial.println("wifi 连接成功");
    Serial.println("wifi ip地址: " + WiFi.localIP().toString());
    //熄灭 LED
    digitalWrite(LED_BUILTIN, HIGH);
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
 * 保存 wifi 配置
 * @param json_wifi_config
 * @return
 */
bool WiFiManager::onSaveWiFiConfigJson(String json_wifi_config) {
    return onSaveLocalCacheJsonString(save_wifi_config_file, json_wifi_config);
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
        doc["msg"] = "WiFi扫描成功!";
        JsonObject data = doc.createNestedObject("data");
        data["device_mac"] = WiFi.macAddress();
        data["device_chip_id"] = ESP.getChipId();
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
    return str_json;
}