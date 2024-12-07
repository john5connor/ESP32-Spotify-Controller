#ifndef WIFI_SETUP_H
#define WIFI_SETUP_H

#include <Arduino.h>
#include <WebServer.h>

extern WebServer server;
extern String spotifyCode;
extern bool authComplete;
extern String IP_ADDRESS;

void setupWifi(void);
void setupWebServerForAuth(void);
void handleAuthCallback(void);

#endif