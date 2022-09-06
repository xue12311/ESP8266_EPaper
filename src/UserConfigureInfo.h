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
     * 配置 web 服务 用来 接收 网络请求
     */
    void onCreateWebServer();

    /**
     * 保持 WebServer 持续运行
     */
    void onUserConfigureLoop();

private:

    // 建立ESP8266WebServer对象
    ESP8266WebServer webServer;
    // WebServer 端口
    int webServerPort = 8087;
    // ESP8266 WebServer 是否已经启用
    bool isWebServerEnable = false;

    /**
     * 读取 本地 用户配置信息 的 json 数据
     */
    bool onReadUserConfigureInfoJsonString();

    /**
     * 写入 本地 用户配置信息 的 json 数据
     */
    bool onWriteUserConfigureInfoJsonString();

    /**
     * Web 服务 响应 用户配置信息
     */
    String onWebResponseUserConfigureInfo();

};


#endif //ESP8266_EPAPER_USERCONFIGUREINFO_H
