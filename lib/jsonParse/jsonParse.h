#ifndef JSONPARSE_H
#define JSONPARSE_H
#endif

#include <Arduino.h>

String parseAccessToken(String response);
String parseImageUrl(String playbackState);
String parseArtists(String playbackState);