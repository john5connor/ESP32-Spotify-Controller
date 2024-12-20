#include "player.h"
#include "config.h"
#include "jsonParse.h"
#include <ArduinoJson.h>
#include <HTTPClient.h>

uint16_t currentVolumeValue;

void previousSong() {
    const String url = "https://api.spotify.com/v1/me/player/previous";

    HTTPClient http;
    if (http.begin(url)) {
        http.addHeader("Authorization", "Bearer " + SPOTIFY_ACCESS_TOKEN);

        uint8_t httpCode = http.POST(deviceId);
        if (httpCode == HTTP_CODE_OK) {
            Serial.println("Previous song");
        } else {
            Serial.printf("HTTP request for PREVIOUS song failed with error: %d\n", httpCode);
        }
    }
    http.end();
} 

void playPauseSong(String playbackStateJson) {
    StaticJsonDocument <50> doc;

    deserializeJson(doc, playbackStateJson); 

    parseIsPlaying(playbackStateJson);
    Serial.println("isPlaying in playPauseSong: " + String(isPlaying));
    
    HTTPClient http;

    if (isPlaying) {
        http.begin("https://api.spotify.com/v1/me/player/pause");
        Serial.println("Pausing song");
        isPlaying = false;
    } else {
        http.begin("https://api.spotify.com/v1/me/player/play");
        Serial.println("Playing song");
        isPlaying = true;
    }

    http.addHeader("Authorization", "Bearer " + SPOTIFY_ACCESS_TOKEN);
    http.addHeader("Content-Type", "application/json");

    int httpCode = http.PUT("{}");
    if (httpCode == HTTP_CODE_NO_CONTENT || httpCode == HTTP_CODE_OK) {
        Serial.println("Playback paused/resumed");
    } else {
        Serial.printf("Failed to pause/resume playback: %d\n", httpCode);
    }

   http.end();
}

void nextSong() {
    const String url = "https://api.spotify.com/v1/me/player/next";

    HTTPClient http;
    if (http.begin(url)) {
        http.addHeader("Authorization", "Bearer " + SPOTIFY_ACCESS_TOKEN);

        uint8_t httpCode = http.POST(deviceId);
        if (httpCode == HTTP_CODE_OK) {
            Serial.println("Next song");
        } else {
            Serial.printf("HTTP request for NEXT song failed with error: %d\n", httpCode);
        }
    }
    http.end();
}

void putVolumeChange(uint16_t mappedInputValue) {
    Serial.println("Changing volume to: " + String(mappedInputValue));
    const String url = "https://api.spotify.com/v1/me/player/volume?volume_percent=" + String(mappedInputValue);

    HTTPClient http;
    if (http.begin(url)) {
        http.addHeader("Authorization", "Bearer " + SPOTIFY_ACCESS_TOKEN);

        uint8_t httpCode = http.PUT("{}");
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_NO_CONTENT) {
            Serial.println("Volume changed");
        } else {
            Serial.printf("HTTP request for changing volume failed with error: %d\n", httpCode);
        }
    }
    http.end();
}

bool songChange(String lastSong, String playbackStateJson) {
    StaticJsonDocument <50> doc;

    deserializeJson(doc, playbackStateJson);

    String currentSong = doc["item"]["name"];

    if (currentSong != lastSong) {
        return true;
    } else {
        return false;
    }
}

uint16_t volumeChange(uint16_t lastVolumeValue, uint16_t currentVolumeValue) {

    if (currentVolumeValue != lastVolumeValue) {
        return currentVolumeValue;
    } else {
        return lastVolumeValue;
    }
    
}