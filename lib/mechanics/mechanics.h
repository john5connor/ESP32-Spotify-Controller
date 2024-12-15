#ifndef MECHANICS_H
#define MECHANICS_H

#include <Arduino.h>

extern const uint8_t PREVIOUS_BUTTON_PIN;
extern const uint8_t PLAY_PAUSE_BUTTON_PIN;
extern const uint8_t NEXT_BUTTON_PIN;

bool previousButtonPressed(void);
bool playPauseButtonPressed(void);
bool nextButtonPressed(void);

#endif