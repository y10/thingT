#ifndef SKETCH_WIFI_H
#define SKETCH_WIFI_H

#include <ESPAsyncDNSServer.h>
#include <WiFi.h>
#include <WsConsole.h>
#include <esp_wifi.h>

#include "Unit.h"

IPAddress localIP(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 0);
IPAddress subnet(255, 255, 255, 0);

AsyncDNSServer dnsServer;

String getSSID() {
  wifi_config_t conf;
  esp_wifi_get_config(WIFI_IF_STA, &conf);
  return String(reinterpret_cast<const char*>(conf.sta.ssid));
}

String getPSK() {
  // only if wifi is init
  if (WiFiGenericClass::getMode() == WIFI_MODE_NULL) {
    return String();
  }
  wifi_config_t conf;
  esp_wifi_get_config(WIFI_IF_STA, &conf);
  return String(reinterpret_cast<char*>(conf.sta.password));
}

void disableWifi() {
  // disable wifi if already on
  if (WiFi.getMode() & WIFI_STA) {
    WiFi.mode(WIFI_OFF);
    int timeout = millis() + 1200;
    // async loop for mode change
    while (WiFi.getMode() != WIFI_OFF && millis() < timeout) {
      delay(0);
    }
  }
}

bool enableWifi() {
  String hostname = Unit.hostname();
  WiFi.hostname(hostname);

  Serial.print("[wifi] ");
  Serial.println(hostname);

  WiFi.softAPConfig(localIP, gateway, subnet);
  if (WiFi.softAP(hostname.c_str())) {
    Serial.print("[wifi] ");
    Serial.println(WiFi.softAPIP());
    Serial.println("[wifi] Started.");
    return true;
  } else {
    Serial.println("[wifi] Could not start AP!");
    return false;
  }
}

bool connectWifi() {
  if (WiFi.enableSTA(true) && WiFi.begin() && WiFi.waitForConnectResult() == WL_CONNECTED) {
    Serial.print("[wifi] ");
    Serial.println(WiFi.localIP());
    Serial.println("[wifi] Connected.");
    return true;
  }

  return false;
}

bool connectWifi(String ssid, String pass = "") {
  Serial.print("[wifi] connecting to '");
  Serial.print(ssid);
  Serial.println("' wifi");

  if (WiFi.begin(ssid.c_str(), pass.c_str()) && WiFi.waitForConnectResult() == WL_CONNECTED) {
    WiFi.enableSTA(true);
    Serial.print("[wifi] ");
    Serial.println(WiFi.localIP());
    Serial.println("[wifi] Connected.");
    return true;
  }

  return false;
}

void setupWifi() {
  disableWifi();
  if (!connectWifi()) {
    if (!enableWifi()) {
      ESP.restart();
    }
  }
}

void setupDNSD() {
  if (WiFi.getMode() & WIFI_AP) {
    dnsServer.setErrorReplyCode(AsyncDNSReplyCode::NoError);
    if (dnsServer.start(53, "*", WiFi.softAPIP())) {
      Serial.println("[dnsd] Started.");
    } else {
      Serial.println("[dnsd] Could not start Captive DNS Server!");
      ESP.restart();
    }
  }
}

void loopWifi() {
  if (Unit.wifissid().length() > 0 && WiFi.getMode() & WIFI_AP) {
    connectWifi(Unit.wifissid(), Unit.wifipass());
  }
}

#endif
