//
// MQTT连接配置
//

#ifndef ESP8266_EPAPER_MQTTMANAGER_H
#define ESP8266_EPAPER_MQTTMANAGER_H


#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <UserConfigureParameter.h>

class MQTTManager {

public:
    /**
     * 初始化 MQTT 服务器
    */
    bool initMqttServer();

    /**
    * 处理 收到的 MQTT 消息
    * @param topic_name 主题名称
    * @param message 消息内容
    * @param length 消息长度
    */
    void onHandleMqttMessage(String topic_name, String message, unsigned int length);

    /**
     * MQTT 消息 解析 json
     */
    void onMqttMessageJsonResolve(String json);


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


    /**
     * 订阅 主题
     * @return  true:订阅成功  false:订阅失败
     */
    bool onSubscribeTopics(String str_topic_name);

    /**
     * 订阅 主题 名称
     */
    String getSubscribeTopicsName(String str_topic_name);

    /**
    * mqtt 保持 心跳连接
    */
    void onMQTTServerLoop();

    /**
     * 设置 配置信息
     */
    void setUserConfig(UserConfigureParameter userConfig);

private:

    //用户 配置
    UserConfigureParameter userConfig;


    /**
     * 清除配置信息
      * @param type 0: 清除全部配置信息 1:清除 WIFI 配置信息  2: 清除 MQTT 配置信息
      * @return
      */
    bool onClearDeviceConfigureInfo(int type);
};


#endif //ESP8266_EPAPER_MQTTMANAGER_H
