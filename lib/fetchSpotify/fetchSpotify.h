#ifndef FETCH_SPOTIFY_H
#define FETCH_SPOTIFY_H

#include <Arduino.h>

extern uint8_t* imageBuffer;
extern size_t imageSize;
extern String playbackStateJson;
extern unsigned long lastFetchTime;

String fetchSpotifyPlaylist();
String fetchPlaybackState();
bool fetchAndStoreImage(String imageUrl);
String fetchAvailableDevices();
String getCachedPlaybackState();
void transferPlayback(String deviceId);

#endif