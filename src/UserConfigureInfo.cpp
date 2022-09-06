//
// 用户配置信息
//

#include "UserConfigureInfo.h"
#include "BasicConfigure.h"


/**
 * 连接 MQTT 服务
 */
bool UserConfigureInfo::onConnectMQTTServer() {
    //读取 本地 用户配置信息
    if (onReadUserConfigureInfoJsonString()) {
    }
    return false;
}

/**
 * 读取 本地 用户配置信息 的 json 数据
 */
bool UserConfigureInfo::onReadUserConfigureInfoJsonString() {
    // 读取文件内容
    String jsonString = onReadLocalCacheJsonString(save_user_config_info_file);
    if (jsonString.length() > 0) {
//        return onJsonWiFiConfig(jsonString);
        return true;
    } else {
        return false;
    }
}

/**
 * 保持 用户配置信息 的 json 数据
 */
bool UserConfigureInfo::onWriteUserConfigureInfoJsonString() {
    return false;
}

/**
 * 配置 web 服务 用来 接收 网络请求
 */
void UserConfigureInfo::onCreateWebServer() {
    // WebServer 未启用
    if (!isWebServerEnable) {
        //获取 用户配置信息
        webServer.on("/api/user_configure", HTTP_POST, [this]() {
            String json = onWebResponseUserConfigureInfo();
            webServer.send(200, "application/json", json);
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
}

/**
 * Web 服务 响应 用户配置信息
 */
String UserConfigureInfo::onWebResponseUserConfigureInfo() {
    String json = "{\"code\":200,\"msg\":\"请求成功\"}";
    return json;
}