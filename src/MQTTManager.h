//
// MQTT连接配置
//

#ifndef ESP8266_EPAPER_MQTTMANAGER_H
#define ESP8266_EPAPER_MQTTMANAGER_H


#include <Arduino.h>

class MQTTManager {

public:
    /**
     * 初始化 MQTT 服务器
    */
    void initMqttServer();

    /**
    * 处理 收到的 MQTT 消息
    * @param topic_name 主题名称
    * @param message 消息内容
    * @param length 消息长度
    */
    static void onHandleMqttMessage(String topic_name, String message, unsigned int length);

    /**
    * 连接 MQTT 服务器
    * @return true 连接成功 false 连接失败
    */
    bool onConnectMqttServer();

//    /**
//     * 发送 mqtt 消息 绑定 开发板设备
//     * @return  true 发送成功  false 发送失败
//     */
//    bool onPushMqttMessage();

//    /**
//     * 订阅 MQTT 主题
//     */
//    void onSubscribeMqttTopic();

    /**
    * mqtt 保持 心跳连接
    */
    void onMQTTServerLoop();

private:
};


#endif //ESP8266_EPAPER_MQTTMANAGER_H
