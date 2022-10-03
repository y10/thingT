#ifndef SKETCH_H
#define SKETCH_H

#include <ArduinoJson.h>
#include <ESP32Servo.h>
#include <Ticker.h>

#include <functional>
#include <map>
#include <vector>

#include "unit-config.h"
#include "unit-pins.h"

class UnitControl {
 protected:
  UnitConfig Config;
  Servo *Servos[5];
  String SSID = "";
  String SKEY = "";

 public:
  UnitControl();

  const char *builtDate() const { return __DATE__ " " __TIME__ " GMT"; }

  const String safename() { return Config.safename(); }
  const String dispname() const { return Config.dispname(); }
  const String dispname(const char *name) { return Config.dispname(name); }
  const String hostname() const { return Config.hostname(); }
  const String hostname(const char *name) { return Config.hostname(name); }
  const String wifissid(bool persisted = false);
  const String wifipass(bool persisted = false);

  String toJSON();
  String toJSON(unsigned int index);
  bool fromJSON(JsonObject json);

  void motor(unsigned int index, unsigned int position);

  void attach();
  void load();
  void save();
  void reset();
  void restart();

  typedef std::function<void(const char *)> OnEvent;
  void on(const char *eventType, OnEvent event);

 protected:
  void fireEvent(const char *eventType) { fireEvent(eventType, ""); }
  void fireEvent(const char *eventType, const String evenDescription) { fireEvent(eventType, evenDescription.c_str()); }
  void fireEvent(const char *eventType, const char *evenDescription);

 private:
  std::map<const char *, std::vector<OnEvent>> onEventHandlers;
};

extern UnitControl Unit;
#endif