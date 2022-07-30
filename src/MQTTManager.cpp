//
// MQTT连接配置
//

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <MQTTManager.h>


//MQTT 服务器 地址
const char *mqtt_server = "test.ranye-iot.net";
const uint16_t mqtt_server_port = 1883;

//开发板 mac 地址
const String str_device_mac_address = WiFi.macAddress();

//开发版 chip id
const String str_device_chip_id = String(ESP.getChipId());

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
//        String str_payload;
//        for (unsigned int i = 0; i < length; i++) {
//            str_payload += (char) payload[i];
//        }
//        onHandleMqttMessage(String(topic), str_payload, length);
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
}

/**
 * 连接 MQTT 服务器
 */
bool MQTTManager::onConnectMqttServer() {
    // 根据ESP8266的MAC地址生成客户端ID（避免与其它ESP8266的客户端ID重名）
    String clientId = "esp8266-" + str_device_chip_id + "-" + str_device_mac_address;
    // 连接MQTT服务器
    if (mqttClient.connect(clientId.c_str())) {
        // 连接成功
        Serial.println("MQTT连接成功");
        Serial.println("服务器地址: " + String(mqtt_server));
        Serial.println("客户端Id: " + clientId);
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
    }
}


