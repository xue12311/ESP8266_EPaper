//
// 用户配置信息
//

#ifndef ESP8266_EPAPER_USERCONFIGUREINFO_H
#define ESP8266_EPAPER_USERCONFIGUREINFO_H


#include "ESP8266WebServer.h"
#include "WString.h"

class UserConfigureInfo {
public:
    /**
     * 连接 MQTT 服务
     */
    bool onConnectMQTTServer();

    /**
     * 订阅 MQTT 主题
     */
    bool onMQTTSubscribeTopics();

    /**
     * 配置 web 服务 用来 接收 网络请求
     */
    void onCreateWebServer();

    /**
     * 保持 WebServer 持续运行
     */
    void onUserConfigureLoop();

private:

    //mqtt 是否已经 连接成功
    bool isMqttConnected = false;

    // 建立ESP8266WebServer对象
    ESP8266WebServer webServer;
    // WebServer 端口
    int webServerPort = 8081;

    // ESP8266 WebServer 是否已经启用
    bool isWebServerEnable = false;

    /**
     * 读取 本地 用户配置信息 的 json 数据
     */
    bool onReadUserConfigureInfoJsonString();

    /**
     * 用户配置 解析 json
     * @param json
     * @return
     */
    bool onJsonUserConfig(String json);
    /**
     * 写入 本地 用户配置信息 的 json 数据
     */
    bool onWriteUserConfigureInfoJsonString();

    /**
     * Web 服务 响应 --- 用户配置信息
     */
    bool onWebResponseUserConfigureInfo();

};


#endif //ESP8266_EPAPER_USERCONFIGUREINFO_H