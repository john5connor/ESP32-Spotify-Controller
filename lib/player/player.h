#ifndef PLAYER_H
#define PLAYER_H

#include <Arduino.h>

void previousSong();
void playPauseSong(String playbackStateJson);
void nextSong();
bool songChange(String lastSong, String playbackStateJson);
uint16_t volumeChange(uint16_t lastVolumeValue, uint16_t currentVolumeValue);
void putVolumeChange(uint16_t currentVolumeValue);

#endif