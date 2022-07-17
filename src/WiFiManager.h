//
// WiFi管理
//

#ifndef ESP8266_EPAPER_WIFIMANAGER_H
#define ESP8266_EPAPER_WIFIMANAGER_H

#include "WString.h"

/**
 * AP 模式  WiFi 名称
 */
const char *ap_wifi_ssid = "ESP8266 e-Paper";

/**
 * AP 模式  WiFi 密码
 */
const char *ap_wifi_password = "12345678";


// wifi 信息 保存地址
const char *save_wifi_config_file = "/wifi_config.json";

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
//
//    void onSettingsWifiAP();
//
//    void onCreateWebServer();
//
//    void handleSettingsWiFiRoot();
//
//    void onConnectionWiFi(String wifi_ssid, String wifi_password);

};


#endif //ESP8266_EPAPER_WIFIMANAGER_H
