#ifndef FETCH_SPOTIFY_H
#define FETCH_SPOTIFY_H

#include <Arduino.h>

extern uint8_t* imageBuffer;
extern size_t imageSize;

String fetchSpotifyPlaylist();
String fetchPlaybackState();
bool fetchAndStoreImage(String imageUrl);

#endif