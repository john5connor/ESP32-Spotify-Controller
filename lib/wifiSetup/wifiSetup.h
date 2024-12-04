#ifndef WIFI_SETUP_H
#define WIFI_SETUP_H

#include <Arduino.h>
#include <WebServer.h>

extern WebServer server;
extern String spotifyCode;
extern bool authComplete;

void setupWifi(void);
void setupWebServerForAuth(void);
void handleAuthCallback(void);

#endif