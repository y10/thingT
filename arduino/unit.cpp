#include "unit.h"

#include <WiFi.h>
#include <esp_wifi.h>
#include <WsConsole.h>

static WsConsole console("main");

UnitControl::UnitControl() {
}

const String UnitControl::wifissid(bool persisted) {
  if (persisted) {
    wifi_config_t conf;
    esp_wifi_get_config(WIFI_IF_STA, &conf);
    return String(reinterpret_cast<const char *>(conf.sta.ssid));
  }
  return SSID;
}

const String UnitControl::wifipass(bool persisted) {
  if (persisted) {
    wifi_config_t conf;
    esp_wifi_get_config(WIFI_IF_STA, &conf);
    return String(reinterpret_cast<char *>(conf.sta.password));
  }
  return SKEY;
}

void UnitControl::fireEvent(const char *eventType, const char *evenDescription) {
  for (auto &event : onEventHandlers[eventType]) {
    event(evenDescription);
  }
}

void UnitControl::on(const char *eventType, OnEvent event) {
  onEventHandlers[eventType].push_back(event);
}

void UnitControl::motor(unsigned int index, unsigned int position = 0) {
  if (index < sizeof(Servos) && position >= 0 && position <= 360) {
    console.println("Servo motor " + (String)index + " changed postition to: " + (String)position);
    Servos[index]->write(position);
    fireEvent("state", "{ \"servos\": { \"" + (String)index + "\": " + (String)position + "} }");
  }
}

bool UnitControl::fromJSON(JsonObject json) {
  if (json.containsKey("name")) {
    Config.dispname(json["name"].as<char *>());
  }

  if (json.containsKey("host")) {
    Config.hostname(json["host"].as<char *>());
  }

  Config.save();

  if (json.containsKey("ssid")) {
    // SSID = "hata";
    // SKEY = "Slava Ukraini";
    SSID = json["ssid"].as<char *>();
    if (json.containsKey("wifiPassword")) {
      SKEY = json["wifiPassword"].as<char *>();
    }
  }

  return true;
}

String UnitControl::toJSON() {
  return (String) "{ \"name\": \"" + Config.dispname() + "\", \"host\": \"" + Config.hostname() + "\", \"ssid\": \"" + wifissid(true) + "\" }";
}

String UnitControl::toJSON(unsigned int index) {
  if (index < sizeof(Servos)) {
    int position = Servos[index]->read();

    return (String) "{ \"servos\": { \"" + (String)index + "\": " + (String)position + "} }";
  }

  return (String) "{ \"servos\": { \"" + (String)index + "\": \"n/a\" } }";
}

void UnitControl::attach() {
  uint8_t pins[] = {SR1_PIN, SR2_PIN, SR3_PIN, SR4_PIN, SR5_PIN};
  for (uint8_t i = 0; i < sizeof(pins); i++) {
    Servo *servo = Servos[i] = new Servo();
    servo->setPeriodHertz(50);  // standard 50 hz servo
    servo->attach(pins[i]);     //, 1000, 2000);
  }
}

void UnitControl::load() {
  Config.load();
}

void UnitControl::save() {
  Config.save();
}

void UnitControl::reset() {
  console.warn("Factory reset requested.");
  Config.clear();

  WiFi.mode(WIFI_AP_STA);  // cannot erase if not in STA mode !
  WiFi.persistent(true);
  WiFi.disconnect(true, true);
  WiFi.persistent(false);

  ESP.restart();
}

void UnitControl::restart() {
  console.warn("Restart requested.");
  ESP.restart();
}

UnitControl Unit = UnitControl();