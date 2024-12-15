#ifndef PLAYER_H
#define PLAYER_H

#include <Arduino.h>

void previousSong();
void playPauseSong(String playbackStateJson);
void nextSong();

#endif