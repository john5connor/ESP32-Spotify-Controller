#ifndef AUTHORIZE_SPOTIFY_H
#define AUTHORIZE_SPOTIFY_H

#include <stdint.h>
#include <Arduino.h>

void requestUserAuthorization(void);
void requestAccessToken(String code); 
String generateRandomString(uint8_t state); 

#endif