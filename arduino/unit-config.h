#ifndef SKETCH_DEVICE_H
#define SKETCH_DEVICE_H

#include <WiFi.h>
#include <EEPROM.h>
#include <WsConsole.h>

#include "includes/files.h"

#define EEPROM_SIZE 4096

class UnitConfig
{

private:
  String host_name;
  String disp_name;
  String full_name;
  uint8_t version;

public:
  UnitConfig();

  const char * builtDate(time_t * dt) const;

  const String safename();

  const String dispname() const { return disp_name; }

  const String dispname(const char *name);

  const String hostname() const { return host_name; }

  const String hostname(const char *name);

  const String fullname() const { return full_name; }

  void load();

  void save();

  void clear();
};

#endif