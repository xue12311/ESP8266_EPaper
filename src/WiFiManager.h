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

private:
    /**
     * 读取 本地 wifi 配置的 json 数据
     * @return true 读取成功  false 读取失败
     */
    bool onReadWiFiConfigJsonString();

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

//    void onSettingsWifiAP();
//
//    void onCreateWebServer();
//
//    void handleSettingsWiFiRoot();
//

};


#endif //ESP8266_EPAPER_WIFIMANAGER_H
