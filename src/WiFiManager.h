//
// WiFi管理
//

#ifndef ESP8266_EPAPER_WIFIMANAGER_H
#define ESP8266_EPAPER_WIFIMANAGER_H

#include "WString.h"

class WiFiManager {
public:
    /**
     * 连接 本地缓存中保存的 wifi
     * @return true 连接成功 false 连接失败
     */
    bool onConnectWiFiConfigJson();

    /**
     * 设置 wifi 为 AP 模式,并启用网络服务
     * @return true 设置成功，false 设置失败
     */
    bool onStartWiFiAPAndWebServer();


    void onWebServerLoop();

private:
    /**
     * 读取 本地 wifi 配置的 json 数据
     * @return true 读取成功  false 读取失败
     */
    bool onReadWiFiConfigJsonString();

    /**
     * WiFi配置 解析 json
     * @param json
     * @return
     */
    bool onJsonWiFiConfig(String json);

    /**
     * 连接 wifi
     * @param wifi_ssid wifi 名称
     * @param wifi_password wifi 密码
     * @return true 连接成功 false 连接失败
     */
    bool onConnectionWiFiChar(char *wifi_ssid, char *wifi_password);

    /**
        * 连接 wifi
        * @param wifi_ssid wifi 名称
        * @param wifi_password wifi 密码
        * @return true 连接成功 false 连接失败
        */
    bool onConnectionWiFiString(String wifi_ssid, String wifi_password);


    /**
     * 设置 wifi 为 AP模式 接入点模式
     * 设置 wifi名称 密码
     * @return 是否设置成功  true 设置成功 false 设置失败
     */
    bool onSettingsWifiAP();

    /**
     * 启用 网络服务 用来 接收 wifi 配置
     */
    bool onCreateWebServer();

/**
  * 响应 wifi连接 请求
  * @param wifi_ssid  wifi 名称
  * @param wifi_password   wifi 密码
  * @param isSuccess  true: wifi 连接成功 false: wifi 连接失败
  * @return json
  */
    String getWiFiConnectWebRequestJson(String wifi_ssid, String wifi_password, bool isSuccess);

    /**
     * 保存 wifi 配置
     * @param json_wifi_config wifi配置
     */
    bool onSaveWiFiConfigJson(String json_wifi_config);


};


#endif //ESP8266_EPAPER_WIFIMANAGER_H
