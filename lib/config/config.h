#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// WiFi credentials
extern const char* WIFI_SSID;
extern const char* WIFI_PASSWORD;

// Spotify credentials for authorization
extern const char* CLIENT_ID;
extern const char* CLIENT_SECRET;
extern String REDIRECT_URI;
extern const char* RESPONSE_TYPE;
extern const char* SCOPE;
extern const char* SPOTIFY_API_URL;
extern String SPOTIFY_ACCESS_TOKEN;
extern const char* GRANT_TYPE;
extern String deviceId;
extern String authorizeUrl;

#endif