//
// wifi配置
//

#include "WiFiConfigureParameter.h"

WiFiConfigureParameter::WiFiConfigureParameter(char *ssid, char *password) {
    wifi_ssid = ssid;
    wifi_password = password;
}

WiFiConfigureParameter::WiFiConfigureParameter(String ssid, String password) {
    //wifi名称
    char c_wifi_ssid[ssid.length() + 1];
    strcpy(c_wifi_ssid, ssid.c_str());
    wifi_ssid = c_wifi_ssid;
    //密码
    char c_wifi_password[password.length() + 1];
    strcpy(c_wifi_password, password.c_str());
    wifi_password = c_wifi_password;
}

WiFiConfigureParameter::WiFiConfigureParameter() {
    wifi_ssid = NULL;
    wifi_password = NULL;
}

bool WiFiConfigureParameter::isValid() {
    if (wifi_ssid == NULL || wifi_password == NULL) {
        return false;
    }
    return true;
}

/**
  * 获取 wifi 名称
  */
char *WiFiConfigureParameter::getSSID() {
    return wifi_ssid;
}

/**
  * 获取 wifi 名称
  */
String WiFiConfigureParameter::getSSIDString() {
    return String(wifi_ssid);
}

/**
* 获取 wifi 密码
*/
char *WiFiConfigureParameter::getPassword() {
    return wifi_password;
}
/**
* 获取 wifi 密码
*/
String WiFiConfigureParameter::getPasswordString() {
    return String(wifi_password);
}

