#include "mechanics.h"

const uint8_t BUTTON_PIN = 27;

bool playPauseButtonPressed(void) {
    static unsigned long lastDebounceTime = 0;
    const unsigned long debounceDelay = 100;

    static uint8_t lastState = HIGH;
    uint8_t currentState = digitalRead(BUTTON_PIN);

    if (currentState != lastState) {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
        if (lastState == HIGH && currentState == LOW) {
            lastState = currentState;
            return true;
        } 
    }

    lastState = currentState;
    return false;
}