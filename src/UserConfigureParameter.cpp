//
// 用户配置 信息详情
//

#include "UserConfigureParameter.h"

UserConfigureParameter::UserConfigureParameter() {
    mqtt_server = "";
    mqtt_port = 0;
    mqtt_username = "";
    mqtt_password = "";
    mqtt_client_id = "";
    mqtt_topic_prefix = "";
}

/**
     * 保存 MQTT配置信息
     * @param server MQTT服务器地址
     * @param port MQTT服务器端口
     * @param username MQTT登录用户名
     * @param password MQTT登录密码
     * @param client_id MQTT客户端ID
     * @param topic_prefix MQTT主题前缀
     */
void UserConfigureParameter::saveMQTTConfigure(
        String server, uint16_t port, String username,
        String password, String client_id, String topic_prefix
) {
    mqtt_server = server;
    mqtt_port = port;
    mqtt_username = username;
    mqtt_password = password;
    mqtt_client_id = client_id;
    mqtt_topic_prefix = topic_prefix;
}

/**
 * 是否有效
 * @return true 有效 false 无效
 */
bool UserConfigureParameter::isValid() {
    if (mqtt_server == nullptr || mqtt_server.length() == 0 || mqtt_port <= 0) {
        return false;
    }
    return true;
}


/**
* MQTT 地址
*/
const char *UserConfigureParameter::getMqttServer() {
    return mqtt_server.c_str();
}

/**
 * MQTT 端口
 */
uint16_t UserConfigureParameter::getMqttPort() {
    return mqtt_port;
}

/**
 * MQTT 登录账号
 */
const char *UserConfigureParameter::getMqttUserName() {
    return mqtt_username.c_str();
}

/**
 * MQTT 登录密码
 */
const char *UserConfigureParameter::getMqttPassWord() {
    return mqtt_password.c_str();
}

/**
 * MQTT 客户端 ID
 */
const char *UserConfigureParameter::getMqttClientId() {
    return mqtt_client_id.c_str();
}

/**
 * MQTT 主题前缀
 */
String UserConfigureParameter::getMqttTopicPrefix() {
    return mqtt_topic_prefix;
}
