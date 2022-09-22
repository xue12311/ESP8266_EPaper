//
// 用户配置信息
//

#ifndef ESP8266_EPAPER_USERCONFIGUREPARAMETER_H
#define ESP8266_EPAPER_USERCONFIGUREPARAMETER_H


#include "WString.h"

class UserConfigureParameter {
public:
    UserConfigureParameter();

     /**
      * 保存 MQTT配置信息
      * @param mqtt_server MQTT服务器地址
      * @param mqtt_port MQTT服务器端口
      * @param mqtt_username MQTT登录用户名
      * @param mqtt_password MQTT登录密码
      * @param mqtt_client_id MQTT客户端ID
      * @param mqtt_topic_prefix MQTT主题前缀
      */
    void saveMQTTConfigure(String mqtt_server, uint16_t mqtt_port, String mqtt_username, String mqtt_password,
                           String mqtt_client_id, String mqtt_topic_prefix);


    /**
     * 是否有效
     * @return true 有效 false 无效
     */
    bool isValid();

    /**
    * MQTT 地址
    */
    const char * getMqttServer();
    /**
     * MQTT 端口
     */
    uint16_t getMqttPort();
    /**
     * MQTT 登录账号
     */
    const char * getMqttUserName();
    /**
     * MQTT 登录密码
     */
    const char * getMqttPassWord();
    /**
     * MQTT 客户端 ID
     */
    const char * getMqttClientId();
    /**
     * MQTT 主题前缀
     */
    String getMqttTopicPrefix();

private:
    /**
     * MQTT 地址
     */
    String mqtt_server;
    /**
     * MQTT 端口
     */
    uint16_t mqtt_port;
    /**
     * MQTT 登录账号
     */
    String mqtt_username;
    /**
     * MQTT 登录密码
     */
    String mqtt_password;
    /**
     * MQTT 客户端 ID
     */
    String mqtt_client_id;
    /**
     * MQTT 主题前缀
     */
    String mqtt_topic_prefix;
};


#endif //ESP8266_EPAPER_USERCONFIGUREPARAMETER_H
