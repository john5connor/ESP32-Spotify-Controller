#include "mechanics.h"

const uint8_t PREVIOUS_BUTTON_PIN = 21;
const uint8_t PLAY_PAUSE_BUTTON_PIN = 19;
const uint8_t NEXT_BUTTON_PIN = 25;
const uint8_t POTENTIOMETER_PIN = 33;

uint16_t analogInputValue;

bool previousButtonPressed(void) {
    //static unsigned long lastDebounceTime = 0;
    //const unsigned long debounceDelay = 100;

    static uint8_t lastState = HIGH;
    uint8_t currentState = digitalRead(PREVIOUS_BUTTON_PIN);
    //Serial.printf("Current state: %d", currentState);
    
    /*
    if (currentState != lastState) {
        lastDebounceTime = millis();
    }
    
    if ((millis() - lastDebounceTime) > debounceDelay) {
        if (lastState == HIGH && currentState == LOW) {
            lastState = currentState;
            return true;
        } 
    }
    */ 
    
    if (lastState == HIGH && currentState == LOW) {
        lastState = currentState;
        return true;
    }

    lastState = currentState;
    return false;
}

bool playPauseButtonPressed(void) {
    //static unsigned long lastDebounceTime = 0;
    //const unsigned long debounceDelay = 100;

    static uint8_t lastState = HIGH;
    uint8_t currentState = digitalRead(PLAY_PAUSE_BUTTON_PIN);
    //Serial.printf("Current state: %d", currentState);
    
    /*
    if (currentState != lastState) {
        lastDebounceTime = millis();
    }
    
    if ((millis() - lastDebounceTime) > debounceDelay) {
        if (lastState == HIGH && currentState == LOW) {
            lastState = currentState;
            return true;
        } 
    }
    */ 
    
    if (lastState == HIGH && currentState == LOW) {
        lastState = currentState;
        return true;
    }

    lastState = currentState;
    return false;
}

bool nextButtonPressed(void) {
    //static unsigned long lastDebounceTime = 0;
    //const unsigned long debounceDelay = 100;

    static uint8_t lastState = HIGH;
    uint8_t currentState = digitalRead(NEXT_BUTTON_PIN);
    //Serial.printf("Current state: %d", currentState);
    
    /*
    if (currentState != lastState) {
        lastDebounceTime = millis();
    }
    
    if ((millis() - lastDebounceTime) > debounceDelay) {
        if (lastState == HIGH && currentState == LOW) {
            lastState = currentState;
            return true;
        } 
    }
    */ 
    
    if (lastState == HIGH && currentState == LOW) {
        lastState = currentState;
        return true;
    }

    lastState = currentState;
    return false;
}

uint16_t readAndMapVolumeValue(void) {
    analogInputValue = map(analogRead(POTENTIOMETER_PIN), 0, 4095, 0, 100);

    return analogInputValue;
}
