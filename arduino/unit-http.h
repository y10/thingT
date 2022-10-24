#ifndef SKETCH_HTTP_H
#define SKETCH_HTTP_H

#include <AsyncJson.h>
#include <AsyncWebSocket.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <TimeLib.h>

#include "includes/StreamString.h"
#include "includes/files.h"
#include "unit.h"

AsyncWebServer http(80);
AsyncWebSocket ws("/ws");

String toStringIp(IPAddress ip) {
  String res = "";
  for (int i = 0; i < 3; i++) {
    res += String((ip >> (8 * i)) & 0xFF) + ".";
  }
  res += String(((ip >> 8 * 3)) & 0xFF);
  return res;
}

boolean isIp(String str) {
  for (unsigned int i = 0; i < str.length(); i++) {
    int c = str.charAt(i);
    if (c != '.' && (c < '0' || c > '9')) {
      return false;
    }
  }
  return true;
}

void ok(AsyncWebServerRequest *request) {
  request->send(200);
}

void ok(AsyncWebServerRequest *request, const String text) {
  request->send(200, "text/html", text);
}

void error(AsyncWebServerRequest *request, const String text) {
  request->send(500, "text/html", text);
}

void json(AsyncWebServerRequest *request, const String text) {
  request->send(200, "application/json", text);
}

void gzip(AsyncWebServerRequest *request, const char *contentType, const unsigned char *content, size_t contentLength) {
  if (!request->header("If-Modified-Since").equals(Unit.builtDate())) {
    AsyncWebServerResponse *response = request->beginResponse_P(200, contentType, content, contentLength);
    response->addHeader("Content-Encoding", "gzip");
    response->addHeader("Last-Modified", Unit.builtDate());
    request->send(response);
  } else {
    request->send(304);
  }
}

/**
 * HTTPD redirector
 * Redirect to captive portal if we got a request for another domain.
 * Return true in that case so the page handler do not try to handle the request again.
 */
boolean captivePortal(AsyncWebServerRequest *request) {
  if (!isIp(request->host())) {
    String location = String("http://") + toStringIp(request->client()->localIP());
    Serial.println("[http] Redirect to: " + location);
    AsyncWebServerResponse *response = request->beginResponse(302, "text/plain", "");
    response->addHeader("Location", location);
    request->send(response);
    return true;
  }

  return false;
}

void setupHttp() {
  http.on("/", [&](AsyncWebServerRequest *request) { gzip(request, "text/html", SKETCH_INDEX_HTML_GZ, sizeof(SKETCH_INDEX_HTML_GZ)); });
  http.on("/favicon.svg", [&](AsyncWebServerRequest *rqt) { gzip(rqt, "image/svg", SKETCH_FAVICON_SVG_GZ, sizeof(SKETCH_FAVICON_SVG_GZ)); });
  http.on("/favicon.jpg", [&](AsyncWebServerRequest *rqt) { gzip(rqt, "image/jpg", SKETCH_FAVICON_JPG_GZ, sizeof(SKETCH_FAVICON_JPG_GZ)); });
  http.on("/manifest.json", [&](AsyncWebServerRequest *rqt) { gzip(rqt, "application/json", SKETCH_MANIFEST_JSON_GZ, sizeof(SKETCH_MANIFEST_JSON_GZ)); });
  http.on("/js/setup.js", [&](AsyncWebServerRequest *rqt) { gzip(rqt, "application/javascript", SKETCH_SETUP_JS_GZ, sizeof(SKETCH_SETUP_JS_GZ)); });

  http.on("/api/state", ASYNC_HTTP_GET, [&](AsyncWebServerRequest *request) {
    json(request, Unit.toJSON());
  });

  http.on("/api/servo/{}/state", ASYNC_HTTP_GET, [&](AsyncWebServerRequest *request) {
    uint8_t idx = request->pathArg(0).toInt();
    json(request, Unit.toJSON(idx));
  });


  http.on("/api/servo/all/position", ASYNC_HTTP_GET, [&](AsyncWebServerRequest *request) {
    uint8_t pos = request->hasArg("v") ? request->arg("v").toInt() : 0;
    Serial.println("GET: /api/servo/all/position?v=" + (String)pos);
    Unit.motor(pos);
    json(request, Unit.toJSON());
  });

  http.on("/api/servo/{}/position", ASYNC_HTTP_GET, [&](AsyncWebServerRequest *request) {
    uint8_t idx = request->pathArg(0).toInt();
    uint8_t pos = request->hasArg("v") ? request->arg("v").toInt() : 0;
    Serial.println("GET: /api/servo/" + (String)idx + "/position?v=" + (String)pos);
    Unit.motor(idx, pos);
    json(request, Unit.toJSON(idx));
  });

  http.on("/api/settings", ASYNC_HTTP_GET, [&](AsyncWebServerRequest *request) {
    json(request, Unit.toJSON());
  });

  http.addHandler(new AsyncCallbackJsonWebHandler(
      "/api/settings", [&](AsyncWebServerRequest *request, JsonVariant &jsonDoc) {
        JsonObject jsonObj = jsonDoc.as<JsonObject>();
        Serial.println("POST: /api/settings");
        if (!Unit.fromJSON(jsonObj)) {
          error(request, "Failed to save settings");
        } else {
          json(request, Unit.toJSON());
        }
      },
      4096));

  http.on("/esp/restart", ASYNC_HTTP_POST, [&](AsyncWebServerRequest *request) {
    Unit.restart();
  });

  http.on("/esp/reset", ASYNC_HTTP_POST, [&](AsyncWebServerRequest *request) {
    Unit.reset();
  });

  http.onNotFound([](AsyncWebServerRequest *request) {
    Serial.println("[http] (404): " + request->url());
    if (!captivePortal(request)) {
      AsyncResponseStream *response = request->beginResponseStream("text/html");
      response->print("<!DOCTYPE html><html><head><title>URI Not Found</title></head><body>");
      response->printf("<p>You were trying to reach: http://%s%s</p>", request->host().c_str(), request->url().c_str());
      response->printf("<p>Try opening <a href='http://%s'>this link</a> instead</p>", WiFi.softAPIP().toString().c_str());
      response->print("</body></html>");
      request->send(response);
    }
  });

  /*
  ws.onEvent([&](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    IPAddress ip = client->remoteIP();
    uint32_t id = client->id();
    String url = server->url();
    switch (type) {
      case WS_EVT_CONNECT:
        Serial.printf("[wsct] (%u) Connected from %d.%d.%d.%d url: %s\n", id, ip[0], ip[1], ip[2], ip[3], url.c_str());
        server->text(id, "{\"connection\": \"Connected\"}");
        Console.attach(&ws);
        break;
      case WS_EVT_DISCONNECT:
        Serial.printf("[wsct] (%u) Disconnected!\n", id);
        break;
      case WS_EVT_PONG:
        Serial.printf("[wsct] (%u) Pong [%u]: %s\n", id, len, (len) ? (char *)data : "");
        break;
      case WS_EVT_ERROR:
        Serial.printf("[wsct] (%u) Error (%u): %s\n", id, *((uint16_t *)arg), (char *)data);
        break;
    }
  });
  http.addHandler(&ws);
  */

  http.begin();
  Serial.println("[http] Started.");

  if (MDNS.begin(Unit.hostname().c_str())) {
    Serial.println("[mdsn] Started.");
  }
}

#endif