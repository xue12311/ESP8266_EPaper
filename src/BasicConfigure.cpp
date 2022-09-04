//
// 基本 配置 参数
//

#include <BasicConfigure.h>
/**
 * AP 模式  WiFi 的 ip地址
 */
IPAddress wifi_ap_local_ip(192, 168, 4, 22);

/**
 * AP 模式  WiFi 的 网关IP地址
 */
IPAddress wifi_ap_gateway(192, 168, 4, 9);

/**
 * AP 模式  WiFi 的 子网掩码
 */
IPAddress wifi_ap_subnet(255, 255, 255, 0);

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

//wifi 连接 超时时间 为 15 秒
const long wifi_connect_timed_out_time = 15 * 1000;
//wifi 配置 超时时间 为 30 秒
const long wifi_smart_config_timed_out_time = 30 * 1000;


//MQTT 服务器 地址
const char *mqtt_server = "test.ranye-iot.net";

//MQTT 服务器 端口
const uint16_t mqtt_server_port = 1883;

// 设备 唯一值（避免与其它 ESP8266的客户端ID重名）
const String device_id = "ay_esp8266-" + String(ESP.getChipId()) + "-" + WiFi.macAddress();

//主题名称  ---  清除 wifi 配置
const String str_topic_device_remove_wifi_configure = device_id + "/device/remove_wifi_configure";

