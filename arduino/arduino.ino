#include <Ticker.h>
#include <TimeLib.h>
#include <WsConsole.h>

#include "unit-http.h"
#include "unit-setup.h"
#include "unit-wifi.h"
#include "unit.h"

Ticker ticker;

void begin() {
  Console.println("Starting...");
  ticker.attach(0.6, tick);
  Console.begin(115200);
}

void setup() {
  begin();

  setupPins();
  setupWifi();
  setupHttp();
  setupDNSD();
  setupUnit();

  end();
}

void loop() {
  loopWifi();
}

void tick() {
  int state = digitalRead(LED_PIN);
  digitalWrite(LED_PIN, !state);
}

void end() {
  digitalWrite(LED_PIN, LOW);
  ticker.detach();
  Console.println("[unit] Started.");
}