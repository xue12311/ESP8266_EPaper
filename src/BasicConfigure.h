//
// 基本 配置 参数
//

#ifndef ESP8266_EPAPER_BASICCONFIGURE_H
#define ESP8266_EPAPER_BASICCONFIGURE_H

#include <ESP8266WiFi.h>

// wifi 信息 保存地址
extern const char *save_wifi_config_file;

//wifi 连接 超时时间 为 15 秒
extern const long wifi_connect_timed_out_time;

//wifi 配置 超时时间 为 30 秒
extern const long wifi_smart_config_timed_out_time;


// 用户配置信息 保存地址
extern const char *save_user_config_info_file;


//  MQTT 订阅主题
extern const String mqtt_subscribe_topict;


/**
 * 获取 本地缓存数据
 */
String onReadLocalCacheJsonString(const char *file_path);

/**
 * 保存 本地缓存数据
 */
bool onSaveLocalCacheJsonString(const char *file_path, String jsonString);

/**
 * 删除 本地缓存数据
 */
bool onRemoveLocalCacheJsonString(const char *file_path);

#endif //ESP8266_EPAPER_BASICCONFIGURE_H
