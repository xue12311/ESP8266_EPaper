//
// WiFi管理
//

#ifndef ESP8266_EPAPER_WIFIMANAGER_H
#define ESP8266_EPAPER_WIFIMANAGER_H

#include "ArduinoJson.h"
#include "WString.h"

class WiFiManager {
public:
    /**
     * 连接 本地缓存中保存的 wifi
     * @return true 连接成功 false 连接失败
     */
    bool onConnectWiFiConfigJson();

    /**
     * wifi 是否已经连接
     * @return true 已连接wifi  false 未连接wifi
     */
    bool isWiFiConnected();

    /**
     * 连接 mqtt 服务
     * @return true 连接成功 false 连接失败
     */
    static bool onConnectMqttService();

    /**
     * 订阅 mqtt主题
     */
    void onSubscribeMqttTopic();

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
    bool onConnectionWiFiString(String wifi_ssid, String wifi_password);

    /**
        * 连接 wifi
        * @param wifi_ssid wifi 名称
        * @param wifi_password wifi 密码
        * @return true 连接成功 false 连接失败
        */
    bool onConnectionWiFiChar(const char *wifi_ssid, const char *wifi_password);

    /**
     * 能否成功扫描到指定 wifi
     * @return true 扫描到指定wifi ; false 没有扫描到指定wifi
     */
    bool isSuccessfulScanWiFi(String wifi_ssid);

    /**
     * 获取当前 wifi连接状态
     * @return
     */
    String getWiFiStatusString();

    String getWiFiStatusString(int status);


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

    /**
     * 获取当前扫描到的 wifi 列表
     * @return  扫描到的wifi列表
     */
    String getWiFiScanListJson();

};


#endif //ESP8266_EPAPER_WIFIMANAGER_H
