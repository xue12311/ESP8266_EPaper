//
// wifi 扫描 配置参数
//

#ifndef ESP8266_EPAPER_WIFISCANCONFIGUREPARAMETRIC_H
#define ESP8266_EPAPER_WIFISCANCONFIGUREPARAMETRIC_H


#include "WString.h"

class WiFiScanConfigureParametric {
private:
    //wifi 名称
    char *wifi_ssid;
    //wifi 信号强度
    int wifi_rssi;
public:
    WiFiScanConfigureParametric();

    WiFiScanConfigureParametric(char *ssid, int rssi);

    WiFiScanConfigureParametric(String ssid, int rssi);

    /**
     * 是否有效
     * @return true 有效 false 无效
     */
    bool isValid();

/**
 * 获取 wifi 名称
 */
    char *getSSID();

/**
 * 获取 wifi 信号强度
 */
    int getRSSI();

/**
 * 获取 wifi 名称
 */
    String getSSIDString();

    /**
     * 获取 wifi 信号强度
     */
    String getRSSIString();
};


#endif //ESP8266_EPAPER_WIFISCANCONFIGUREPARAMETRIC_H
