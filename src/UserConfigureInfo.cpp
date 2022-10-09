//
// 用户配置信息
//

#include "UserConfigureInfo.h"
#include "MQTTManager.h"
#include "BasicConfigure.h"
#include "UserConfigureParameter.h"
#include "ArduinoJson.h"

//用户配置信息
UserConfigureParameter mUserConfig = UserConfigureParameter();
//MQTT管理器
MQTTManager mqttManager;

/**
 * 连接 MQTT 服务
 */
bool UserConfigureInfo::onConnectMQTTServer() {
    //读取 本地 用户配置信息
    if (onReadUserConfigureInfoJsonString()) {
        //设置 配置信息
        mqttManager.setUserConfig(mUserConfig);
        //初始化  MQTT 服务
        if (mqttManager.initMqttServer()) {
            isMqttConnected = mqttManager.onConnectMqttServer();

            return isMqttConnected;
        }
    }
    isMqttConnected = false;
    return isMqttConnected;
}

/**
 * 订阅 MQTT 主题
 */
bool UserConfigureInfo::onMQTTSubscribeTopics() {
    if (isMqttConnected) {
        bool isSubscribeSuccess = mqttManager.onSubscribeTopics(mqtt_subscribe_topict);
        return isSubscribeSuccess;
    } else {
        Serial.println("MQTT 未连接成功 : ");
        return false;
    }
}


/**
* 处理 收到的 MQTT 消息
* @param topic_name 主题名称
* @param message 消息内容
* @param length 消息长度
*/
void MQTTManager::onHandleMqttMessage(String topic_name, String message, unsigned int length) {
    Serial.println("收到 MQTT 消息 : ");
    Serial.println("主题 : " + topic_name);
    Serial.println("消息 : " + message);
    Serial.println("长度 : " + String(length));
    Serial.println("--------------------------------------------------");
    //判断 主题 是否是 本设备的 主题
    if (mqttManager.getSubscribeTopicsName(mqtt_subscribe_topict).equals(topic_name) && !message.isEmpty()) {
        Serial.println("json : " + message);
    }
}

/**
 * 读取 本地 用户配置信息 的 json 数据
 */
bool UserConfigureInfo::onReadUserConfigureInfoJsonString() {
    // 读取文件内容
    String jsonString = onReadLocalCacheJsonString(save_user_config_info_file);
    if (jsonString.length() > 0) {
        return onJsonUserConfig(jsonString);
    } else {
        return false;
    }
}


/**
 * 用户配置 解析 json
 */
bool UserConfigureInfo::onJsonUserConfig(String json) {
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
        if (!data.isNull()) {
            JsonObject mqtt_configure = data["mqtt_configure"];
            if (!mqtt_configure.isNull()) {
                String server = mqtt_configure["mqtt_server"].as<String>();
                String str_port = mqtt_configure["mqtt_port"].as<String>();
                uint16_t port = strtol(str_port.c_str(), NULL, 0);
                String user_name = mqtt_configure["mqtt_username"].as<String>();
                String pass_word = mqtt_configure["mqtt_password"].as<String>();
                String client_id = mqtt_configure["mqtt_client_id"].as<String>();
                String topic_prefix = mqtt_configure["mqtt_topic_prefix"].as<String>();
                mUserConfig.saveMQTTConfigure(server, port, user_name, pass_word, client_id, topic_prefix);
            }
        }
        return mUserConfig.isValid();
    }
    return false;
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
        //设置 用户配置信息
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
    if (isMqttConnected) {
        mqttManager.onMQTTServerLoop();
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