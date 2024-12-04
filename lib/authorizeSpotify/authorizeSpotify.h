#ifndef AUTHORIZE_SPOTIFY_H
#define AUTHORIZE_SPOTIFY_H

#include <stdint.h>
#include <Arduino.h>

String requestUserAuthorization(void);
String requestAccessToken(String code); 
String generateRandomString(uint8_t state); 

#endif