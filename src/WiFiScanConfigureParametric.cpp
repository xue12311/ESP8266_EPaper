//
// wifi 扫描 配置参数
//


#include "WiFiScanConfigureParametric.h"

WiFiScanConfigureParametric::WiFiScanConfigureParametric() {
    wifi_ssid = NULL;
    wifi_rssi = -1;
}

WiFiScanConfigureParametric::WiFiScanConfigureParametric(char *ssid, int rssi) {
    wifi_ssid = ssid;
    wifi_rssi = rssi;
}

WiFiScanConfigureParametric::WiFiScanConfigureParametric(String ssid, int rssi) {
    //wifi名称
    char c_wifi_ssid[ssid.length() + 1];
    strcpy(c_wifi_ssid, ssid.c_str());
    wifi_ssid = c_wifi_ssid;
    //wifi 信号强度
    wifi_rssi = rssi;
}
/**
 * 是否有效
 * @return true 有效 false 无效
 */
bool WiFiScanConfigureParametric::isValid() {
    if (wifi_ssid == NULL || wifi_rssi < 0) {
        return false;
    }
    return true;
}
/**
 * 获取 wifi 名称
 */
char *WiFiScanConfigureParametric::getSSID() {
    return wifi_ssid;
}

/**
 * 获取 wifi 信号强度
 */
int WiFiScanConfigureParametric::getRSSI() {
    return wifi_rssi;
}
/**
 * 获取 wifi 名称
 */
String WiFiScanConfigureParametric::getSSIDString() {
    return String(wifi_ssid);
}

String WiFiScanConfigureParametric::getRSSIString() {
    return String(wifi_rssi);
}