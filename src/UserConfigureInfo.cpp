//
// 用户配置信息
//

#include "UserConfigureInfo.h"
#include "BasicConfigure.h"
#include <ArduinoJson.h>

/**
 * 连接 MQTT 服务
 */
bool UserConfigureInfo::onConnectMQTTServer() {
    //读取 本地 用户配置信息
    if (onReadUserConfigureInfoJsonString()) {
    }
    return false;
}

/**
 * 读取 本地 用户配置信息 的 json 数据
 */
bool UserConfigureInfo::onReadUserConfigureInfoJsonString() {
    // 读取文件内容
    String jsonString = onReadLocalCacheJsonString(save_user_config_info_file);
    if (jsonString.length() > 0) {
//        return onJsonWiFiConfig(jsonString);
        return true;
    } else {
        return false;
    }
}

/**
 * 写入 用户配置信息 的 json 数据
 */
bool UserConfigureInfo::onWriteUserConfigureInfoJsonString() {
    DynamicJsonDocument doc(500);
    doc["code"] = 200;
    doc["msg"] = "获取 用户配置信息成功!";
    JsonObject data = doc.createNestedObject("data");
    JsonObject mqtt_configure = data.createNestedObject("mqtt_configure");
    mqtt_configure["mqtt_server"] = webServer.arg("mqtt_server");
    mqtt_configure["mqtt_port"] = webServer.arg("mqtt_port");
    mqtt_configure["mqtt_username"] = webServer.arg("mqtt_username");
    mqtt_configure["mqtt_password"] = webServer.arg("mqtt_password");
    mqtt_configure["mqtt_client_id"] = webServer.arg("mqtt_client_id");
    mqtt_configure["mqtt_topic_prefix"] = webServer.arg("mqtt_topic_prefix");
    String str_json;
    serializeJsonPretty(doc, str_json);
    doc.clear();
    Serial.println("保存json : " + str_json);
    return onSaveLocalCacheJsonString(save_user_config_info_file, str_json);
}

/**
 * 配置 web 服务 用来 接收 网络请求
 */
void UserConfigureInfo::onCreateWebServer() {
    // WebServer 未启用
    if (!isWebServerEnable) {
        //获取 用户配置信息
        webServer.on("/api/user_configure", HTTP_POST, [this]() {
            bool isSuccessSave = onWebResponseUserConfigureInfo();
            if (isSuccessSave) {
                onReadUserConfigureInfoJsonString();
            }
        });

        //处理404情况
        webServer.onNotFound([this]() {
            String json = "{\"code\":404,\"msg\":\"404错误 请求失败,请重试!\"}";
            webServer.send(404, "application/json", json);
        });
        // 启用 WebServer
        webServer.begin(webServerPort);
        // 设置 WebServer 已启用
        isWebServerEnable = true;
        Serial.println("网络服务器启动成功.");
    }

}

/**
 * Loop 方法
 */
void UserConfigureInfo::onUserConfigureLoop() {
    // WebServer 已启用
    if (isWebServerEnable) {
        // 处理 WebServer 请求
        webServer.handleClient();
    }
}

/**
 * Web 服务 响应 用户配置信息
 */
bool UserConfigureInfo::onWebResponseUserConfigureInfo() {
    //保存 用户配置信息
    bool isSave = onWriteUserConfigureInfoJsonString();
    DynamicJsonDocument doc(500);
    if (isSave) {
        doc["code"] = 200;
        doc["msg"] = "用户信息配置 保存成功！";
    } else {
        doc["code"] = 201;
        doc["msg"] = "用户信息配置 保存失败！";
    }
    JsonObject data = doc.createNestedObject("data");
    data["wifi_local_ip"] = WiFi.localIP().toString();
    data["device_mac"] = WiFi.macAddress();
    data["device_chip_id"] = ESP.getChipId();
    String str_json;
    serializeJsonPretty(doc, str_json);
    doc.clear();
    webServer.send(200, "application/json", str_json);
    return isSave;
}