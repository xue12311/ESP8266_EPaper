#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "FS.h"

#define LED 2


void ConnectServer();

bool WiFiWatchDog();

void BaseConfig();

void SmartConfig();


struct Config {
    String SSID;
    String Passwd;
    String Server = "esaylinker";
    String Token = "0000";
};
Config _config;

bool LoadConfig() {
    File configFile = SPIFFS.open("/config.json", "r");
    if (!configFile) {
        Serial.println("Failed to open config file");
        return false;
    }
    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, configFile);
    if (error) {
        Serial.println("Failed to read file, using default configuration");
        return false;
    }
    _config.SSID = doc["SSID"] | "fail";
    _config.Passwd = doc["Passwd"] | "fail";
    if (_config.SSID == "fail" || _config.Passwd == "fail") {
        return false;
    } else {
        Serial.println("Load wifi config from spiffs successful.");
        Serial.print("Loaded ssid: ");
        Serial.println(_config.SSID);
        Serial.print("Loaded passwd: ");
        Serial.println(_config.Passwd);
        return true;
    }
}

bool SaveConfig() {
    DynamicJsonDocument doc(1024);
    JsonObject root = doc.to<JsonObject>();
    root["SSID"] = _config.SSID;
    root["Passwd"] = _config.Passwd;
    root["Server"] = _config.Server;
    root["Token"] = _config.Token;

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
        Serial.println("Failed to open config file for writing");
        return false;
    }
    if (serializeJson(doc, configFile) == 0) {
        Serial.println("Failed to write to file");
        return false;
    }
    return true;
}

void setup() {
    Serial.begin(9600);
    //Set LED
    pinMode(LED, OUTPUT);
    //Mount FS
    Serial.println("Mounting FS...");
    if (!SPIFFS.begin()) {
        Serial.println("Failed to mount file system");
        return;
    }
    //Connect WIFI
    ConnectServer();
}

void loop() {
    //Wifi watch dog,if wifi not connect, it will auto reconnect.
    WiFiWatchDog();

    Serial.print("Looks like working. now:");
    Serial.println(millis() / 1000);
    delay(1000);
}

void ConnectServer() {
    if (LoadConfig()) {
        BaseConfig();
    } else {
        SmartConfig();
    }
    digitalWrite(LED, LOW);
}

bool WiFiWatchDog() {
    if (WiFi.status() != WL_CONNECTED) {
        BaseConfig();
    }
    return true;
}

void BaseConfig() {
    Serial.println("Use base config to connect wifi.");
    WiFi.mode(WIFI_STA);
    WiFi.begin(_config.SSID, _config.Passwd);
    //连接超时时间，30s后没有连接将会转入SmartConfig
    int timeout = 30000;
    while (WiFi.status() != WL_CONNECTED) {
        Serial.println("Wait to connect wifi...");
        digitalWrite(LED, LOW);
        delay(150);
        digitalWrite(LED, HIGH);
        delay(150);
        timeout = timeout - 300;
        if (timeout <= 0) {
            Serial.println("Wifi connect timeout, use smart config to connect...");
            SmartConfig();
            return;
        }
    }
    Serial.println("WiFi connected by base config.");
    Serial.print("SSID:");
    Serial.println(WiFi.SSID());
    Serial.print("IP Address:");
    Serial.println(WiFi.localIP());
}

void SmartConfig() {
    Serial.println("Use smart config to connect wifi.");
    WiFi.mode(WIFI_STA);
    WiFi.beginSmartConfig();
    while (1) {
        Serial.println("Wait to connect wifi...");
        digitalWrite(LED, LOW);
        delay(1000);
        digitalWrite(LED, HIGH);
        delay(1000);
        if (WiFi.smartConfigDone()) {
            Serial.println("WiFi connected by smart config.");
            Serial.print("SSID:");
            Serial.println(WiFi.SSID());
            Serial.print("IP Address:");
            Serial.println(WiFi.localIP());
            _config.SSID = WiFi.SSID();
            _config.Passwd = WiFi.psk();
            if (!SaveConfig()) {
                Serial.println("Failed to save config");
            } else {
                Serial.println("Config saved");
            }
            break;
        }
    }
}