//
// 基本 配置 参数
//

#include <LittleFS.h>
#include <BasicConfigure.h>

// wifi 信息 保存地址
extern const char *save_wifi_config_file = "/wifi_config.json";

//wifi 连接 超时时间 为 15 秒
extern const long wifi_connect_timed_out_time = 15 * 1000;

//wifi 配置 超时时间 为 30 秒
extern const long wifi_smart_config_timed_out_time = 60 * 1000;


// 用户配置信息 保存地址
extern const char *save_user_config_info_file = "/user_config_info.json";


//  MQTT 订阅主题
extern const String mqtt_subscribe_topict = "esp8266_api";

// api : 获取开发版信息
extern const String api_obtain_esp8266_configure = "/api/obtain_esp8266_configure";

// api : 设置开发版配置信息（wifi配置信息，用户配置信息 ）
extern const String api_setting_device_configure = "/api/api_setting_device_configure";

// api : 删除配置信息（wifi配置信息，用户配置信息 ）
extern const String api_clear_device_configure = "/api/clear_device_configure";

/**
 * 获取 本地缓存数据
 */
String onReadLocalCacheJsonString(const char *file_path) {
    // 启动 LittleFS
    if (!LittleFS.begin()) {
        Serial.println("LittleFS 启用失败.");
        return "";
    }
    // 检查文件是否存在
    if (!LittleFS.exists(file_path)) {
        Serial.println("配置文件 不存在.");
        return "";
    }
    // 打开文件
    File file = LittleFS.open(file_path, "r");
    if (!file) {
        Serial.println("配置文件 打开失败.");
        return "";
    }
    // 读取文件内容
    String jsonString = file.readString();
    Serial.println("缓存配置 JSON : " + jsonString);
    file.close();
    if (jsonString.length() > 0) {
        return jsonString;
    } else {
        return "";
    }
}


/**
 * 保存 本地缓存数据
 */
bool onSaveLocalCacheJsonString(const char *file_path, String jsonString) {
    // 启动 LittleFS
    if (!LittleFS.begin()) {
        Serial.println("LittleFS 启用失败.");
        return false;
    }
    File file = LittleFS.open(file_path, "w");
    file.write(jsonString.c_str());
    file.close();
    Serial.println("配置文件 保存成功.");
    return true;
}


/**
 * 删除 本地缓存数据
 */
bool onRemoveLocalCacheJsonString(const char *file_path) {
    // 启动 LittleFS
    if (!LittleFS.begin()) {
        Serial.println("LittleFS 启用失败.");
        return false;
    }
    // 检查文件是否存在
    if (!LittleFS.exists(file_path)) {
        Serial.println("配置文件不存在.");
        return true;
    }
    // 删除文件
    return LittleFS.remove(file_path);
}