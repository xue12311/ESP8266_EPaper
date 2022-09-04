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
     * 配置 wifi
     * @return true 配置成功 false 配置失败
     */
    bool onSmartConfigWiFi();

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

private:

    /**
     * 读取 本地 wifi 配置的 json 数据
     * @return true 读取成功  false 读取失败
     */
    bool onReadWiFiConfigJsonString();

    /**
     * 写入 本地 wifi 配置的 json 数据
     * @return true 写入成功  false 写入失败
     */
    bool onWriteWiFiConfigJsonString();

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
     * 获取当前 wifi连接状态
     * @return
     */
    String getWiFiStatusString();

    String getWiFiStatusString(int status);

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
