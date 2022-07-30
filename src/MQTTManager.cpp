//
// MQTT连接配置
//

#include <MQTTManager.h>
#include <BasicConfigure.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <LittleFS.h>


WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);


/**
 * 初始化 MQTT 服务器
*/
void MQTTManager::initMqttServer() {
    // 设置MQTT服务器和端口号
    mqttClient.setServer(mqtt_server, mqtt_server_port);
// 设置MQTT订阅回调函数
    mqttClient.setCallback([](char *topic, byte *payload, unsigned int length) {
        String str_payload;
        for (int i = 0; i < length; i++) {
            str_payload += (char) payload[i];
        }
        onHandleMqttMessage(String(topic), str_payload, length);
    });
}


/**
* 处理 收到的 MQTT 消息
* @param topic_name 主题名称
* @param message 消息内容
* @param length 消息长度
*/
void MQTTManager::onHandleMqttMessage(String topic_name, String message, unsigned int length) {
    Serial.println("收到 MQTT 消息 [ " + topic_name + " ] : " + message);
    //清除 wifi 配置
    if (str_topic_device_remove_wifi_configure.equals(topic_name)) {
        // 清除 wifi 配置
        if (onClearLocalWifiConfigure()) {
            Serial.println("清除 wifi 配置成功");
            //重置并重试
            ESP.restart();
        } else {
            Serial.println("清除 wifi 配置失败");
        }
    }
}

/**
 * 连接 MQTT 服务器
 */
bool MQTTManager::onConnectMqttServer() {
    // 连接MQTT服务器
    if (mqttClient.connect(device_id.c_str())) {
        // 连接成功
        Serial.println("MQTT连接成功");
        Serial.println("服务器地址: " + String(mqtt_server));
        Serial.println("客户端Id: " + device_id);
        return true;
    } else {
        // 连接失败
        Serial.println("MQTT连接失败, 错误码: " + String(mqttClient.state()));
        return false;
    }
}

/**
 * 订阅 MQTT 主题
 */
void MQTTManager::onSubscribeMqttTopic() {
    // 订阅主题 --- 清除wifi配置
    onSubscribeTopicsRemoveWifiConfigure();
}


/**
 * 循环 mqtt 配置
 */
void MQTTManager::onMQTTServerLoop() {
    //mqtt 连接成功
    if (mqttClient.connected()) {
        //mqtt 保持心跳连接
        mqttClient.loop();
    } else {
        delay(30 * 1000);
        //mqtt 连接失败
        if (onConnectMqttServer()) {
            //mqtt 连接成功
            Serial.println("MQTT重新连接成功");
        } else {
            //mqtt 连接失败
            Serial.println("MQTT重新连接失败");
        }
    }
}

/**
 * 订阅 MQTT 主题  ----   清除wifi配置
 * @return  true 订阅成功  false 订阅失败
 */
bool MQTTManager::onSubscribeTopicsRemoveWifiConfigure() {
    if (mqttClient.subscribe(str_topic_device_remove_wifi_configure.c_str())) {
        Serial.println("订阅主题成功 : " + str_topic_device_remove_wifi_configure);
        return true;
    } else {
        Serial.println("订阅主题失败 : " + str_topic_device_remove_wifi_configure);
        return false;
    }
}


/**
 * 读取 本地缓存 wifi 配置
 * @return true 读取成功 false 读取失败
 */
bool MQTTManager::onClearLocalWifiConfigure() {
    // 启动 LittleFS
    if (!LittleFS.begin()) {
        Serial.println("LittleFS 启用失败.");
        return false;
    }
    // 检查文件是否存在
    if (!LittleFS.exists(save_wifi_config_file)) {
        Serial.println("wifi配置 文件不存在.");
        return true;
    }
    // 删除文件
    return LittleFS.remove(save_wifi_config_file);
}