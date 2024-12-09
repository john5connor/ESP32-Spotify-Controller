#ifndef JSONPARSE_H
#define JSONPARSE_H
#endif

#include <Arduino.h>

extern const char* imageUrl;
extern String artists;
extern String song;
extern String deviceId;

void doDeserializeJson(String json);
void parseAccessToken(String response);
void parseImageUrl(String playbackStateJson);
void parseArtists(String playbackStateJson);
void parseSong(String playbackStateJson);
void parseAvailableDevices(String devicesJson);