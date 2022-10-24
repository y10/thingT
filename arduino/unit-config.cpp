#include <WsConsole.h>
#include <ESP32Servo.h>
#include "unit-config.h"

Servo myservo;  
static WsConsole console("esp");

struct UnitInfo
{
  uint8_t version;
  uint8_t loglevel;
  char full_name[50];
  char host_name[50];
  char disp_name[50];
  UnitInfo(): version(0), full_name({0}), host_name({0}), disp_name({0}) {}
};

#ifdef ESP8266
#define getChipId() ESP.getChipId() 
#elif defined(ESP32)
#define getChipId() (uint32_t)ESP.getEfuseMac()
#endif

UnitConfig::UnitConfig() {
  disp_name = "Thing T.";
  host_name = "ThingT-" + String(getChipId(), HEX);
  full_name = "ThingT-v" + (String)SKETCH_VERSION_MAJOR + "." + (String)SKETCH_VERSION_MINOR + "." + (String)SKETCH_VERSION_RELEASE + "_" + String(getChipId(), HEX);
  ver = 0;
}

const String UnitConfig::hostname(const char *name) {
  if (strlen(name) > 0) {
    if (!host_name.equals(name)) {
      host_name = name;
    }
  }

  return host_name;
}

const String UnitConfig::dispname(const char *name) {
  if (strlen(name) > 0) {
    if (!disp_name.equals(name)) {
      disp_name = name;
    }
  }

  return disp_name;
}

void UnitConfig::load() {
  EEPROM.begin(EEPROM_SIZE);

  UnitInfo cfg;
  EEPROM.get(0, cfg);

  if (full_name.equals(cfg.full_name)) {
    console.print("[unit] disp. name: ");
    console.println(cfg.disp_name);
    disp_name = cfg.disp_name;
    console.print("[unit] host. name: ");
    console.println(cfg.host_name);
    host_name = cfg.host_name;
    console.print("[unit] revision: ");
    console.println(cfg.version);
    ver = cfg.version;
  } else {
    console.println("[unit] no config found.");
  }
}

void UnitConfig::save() {
  UnitInfo cfg;
  strcpy(cfg.disp_name, disp_name.c_str());
  strcpy(cfg.host_name, host_name.c_str());
  strcpy(cfg.full_name, full_name.c_str());
  cfg.version = ver + 1;
  console.println("[unit] save");
  EEPROM.put(0, cfg);
  EEPROM.commit();
}

void UnitConfig::clear() {
  console.println("[unit] clear");
  for (int i = 0; i < EEPROM.length(); i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
}
