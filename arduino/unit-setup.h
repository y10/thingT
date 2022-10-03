#ifndef SKETCH_SETUP_H
#define SKETCH_SETUP_H

#include "unit.h"

void setupPins() {
  pinMode(LED_PIN, OUTPUT);
  Unit.load();
}

void setupUnit() {
  Unit.on("state", [](const char *event) {
    Serial.println((String) "(state): " + (String)(strlen(event) ? event : "null"));
  });
  Unit.attach();
}

#endif