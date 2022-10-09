//
// MQTT连接配置
//

#include <MQTTManager.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

/**
 * 设置 配置信息
 */
void MQTTManager::setUserConfig(UserConfigureParameter user_config) {
    userConfig = user_config;
}

/**
 * 初始化 MQTT 服务器
*/
bool MQTTManager::initMqttServer() {
    if (!userConfig.isValid()) {
        // 连接失败
        Serial.println("MQTT初始化失败,请检查配置信息");
        return false;
    } else {
        // 设置MQTT服务器和端口号
        mqttClient.setServer(userConfig.getMqttServer(), userConfig.getMqttPort());
        // 设置MQTT订阅回调函数
        mqttClient.setCallback([](char *topic, byte *payload, unsigned int length) {
            String str_payload;
            for (int i = 0; i < length; i++) {
                str_payload += (char) payload[i];
            }
            onHandleMqttMessage(String(topic), str_payload, length);
        });
        return true;
    }
}


/**
* 处理 收到的 MQTT 消息
* @param topic_name 主题名称
* @param message 消息内容
* @param length 消息长度
*/
//void MQTTManager::onHandleMqttMessage(String topic_name, String message, unsigned int length) {
//    Serial.println("收到 MQTT 消息 [ " + topic_name + " ] : " + message);
//    //清除 wifi 配置
//    if (str_topic_device_remove_wifi_configure.equals(topic_name)) {
//        // 清除 wifi 配置
//        if (onClearLocalWifiConfigure()) {
//            Serial.println("清除 wifi 配置成功");
//            //重置并重试
//            ESP.restart();
//        } else {
//            Serial.println("清除 wifi 配置失败");
//        }
//    }
//}

/**
 * 连接 MQTT 服务器
 */
bool MQTTManager::onConnectMqttServer() {
    if (!userConfig.isValid()) {
        // 连接失败
        Serial.println("MQTT连接失败,请检查配置信息");
        return false;
    }
    // 连接MQTT服务器
    if (mqttClient.connect(userConfig.getMqttClientId(), userConfig.getMqttUserName(), userConfig.getMqttPassWord())) {
        // 连接成功
        Serial.println("MQTT连接成功");
        return true;
    } else {
        // 连接失败
        Serial.println("MQTT连接失败, 错误码: " + String(mqttClient.state()));
        return false;
    }
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
 * 订阅 MQTT 主题
 * @return  true 订阅成功  false 订阅失败
 */
bool MQTTManager::onSubscribeTopics(String topic) {
    String str_topic = getSubscribeTopicsName(topic);
    if (str_topic.isEmpty()) {
        Serial.println("订阅主题 : " + str_topic + " 失败");
        return false;
    }
    if (mqttClient.subscribe(str_topic.c_str())) {
        Serial.println("订阅主题成功 : " + str_topic);
        return true;
    } else {
        Serial.println("订阅主题失败 : " + str_topic);
        return false;
    }
}


/**
 * 订阅 主题 名称
 */
String MQTTManager::getSubscribeTopicsName(String str_topic_name) {
    if (str_topic_name == nullptr || str_topic_name.isEmpty()) {
        return "";
    }
    String str_topic;
    String topic_name_prefix = userConfig.getMqttTopicPrefix();
    if (topic_name_prefix != nullptr && !topic_name_prefix.isEmpty()) {
        str_topic = topic_name_prefix;
    }
    if (str_topic.endsWith("/")) {
        str_topic = str_topic + String(ESP.getChipId());
    } else {
        str_topic = str_topic + "/" + String(ESP.getChipId());
    }
    str_topic = str_topic + "/" + str_topic_name;
    return str_topic;
}


