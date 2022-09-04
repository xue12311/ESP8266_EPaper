//
// wifi配置
//

#include "WiFiConfigureParameter.h"

WiFiConfigureParameter::WiFiConfigureParameter(String ssid, String password) {
    //wifi名称
    wifi_ssid = ssid;
    //密码
    wifi_password = password;
}

WiFiConfigureParameter::WiFiConfigureParameter() {
    wifi_ssid = "";
    wifi_password = "";
}

void WiFiConfigureParameter::saveWiFiConfigure(String ssid, String password) {
    //wifi名称
    wifi_ssid = ssid;
    //密码
    wifi_password = password;

}

bool WiFiConfigureParameter::isValid() {
    if (wifi_ssid == nullptr || wifi_password == nullptr || wifi_ssid.length() <= 0 || wifi_password.length() <= 0) {
        return false;
    }
    return true;
}

/**
  * 获取 wifi 名称
  */
const char *WiFiConfigureParameter::getSSID() {
    return wifi_ssid.c_str();
}

/**
  * 获取 wifi 名称
  */
String WiFiConfigureParameter::getSSIDString() {
    return wifi_ssid;
}

/**
* 获取 wifi 密码
*/
const char *WiFiConfigureParameter::getPassword() {
    return wifi_password.c_str();
}

/**
* 获取 wifi 密码
*/
String WiFiConfigureParameter::getPasswordString() {
    return wifi_password;
}

