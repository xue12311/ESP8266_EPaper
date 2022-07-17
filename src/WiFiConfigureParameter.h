//
// wifi配置
//
#ifndef ESP8266_EPAPER_WIFICONFIGUREPARAMETER_H
#define ESP8266_EPAPER_WIFICONFIGUREPARAMETER_H

#include "WString.h"

class WiFiConfigureParameter {

public:
    WiFiConfigureParameter(char *ssid, char *password);

    WiFiConfigureParameter(String ssid, String password);

    WiFiConfigureParameter();

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
 * 获取 wifi 密码
 */
    char *getPassword();

/**
 * 获取 wifi 名称
 */
    String getSSIDString();

/**
 * 获取 wifi 密码
 */
    String getPasswordString();


private:
    char *wifi_ssid;
    char *wifi_password;
};


#endif //ESP8266_EPAPER_WIFICONFIGUREPARAMETER_H
