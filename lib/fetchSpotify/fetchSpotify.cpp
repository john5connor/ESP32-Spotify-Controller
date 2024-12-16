#include "fetchSpotify.h"
#include "config.h"
#include "jsonParse.h"
#include <HTTPClient.h>
#include <WiFiClient.h>

uint8_t* imageBuffer = nullptr;
size_t imageSize = 0;
String playbackStateJson;
unsigned long lastFetchTime = 0;
const unsigned long cacheDuration = 3500; // Cooldown period in milliseconds

String fetchSpotifyPlaylist() {
  String url = String(SPOTIFY_API_URL); //URL for the Spotify API playlist endpoint

  HTTPClient https;
  String response;
  if (https.begin(url)) {
    //Setup headers for GET request
    String authHeader = "Bearer " + SPOTIFY_ACCESS_TOKEN;

    //Add headers to http request
    https.addHeader("Authorization", authHeader);

    //Execute the GET request
    int httpCode = https.GET();

    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK) {
        response = https.getString();
      }
    } else {
      Serial.printf("HTTP request for playlist failed with error: %s\n", https.errorToString(httpCode).c_str());
    }
    https.end();
  } 
    return response;
}

String fetchPlaybackState() {
  String url = "https://api.spotify.com/v1/me/player"; //URL for the Spotify API player endpoint

  HTTPClient https;
  String response;
  if (https.begin(url)) {
    //Setup headers for GET request
    String authHeader = "Bearer " + SPOTIFY_ACCESS_TOKEN;

    //Add headers to http request
    https.addHeader("Authorization", authHeader);

    //Execute the GET request
    int httpCode = https.GET();
    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK) {
        response = https.getString();
        playbackStateJson = response;
      } else if (httpCode == HTTP_CODE_NO_CONTENT) {
        Serial.printf("No active playback session, using cached state.\n");
        transferPlayback(deviceId);
        response = playbackStateJson;
      }
    } else {
      Serial.printf("HTTP request for playback-state failed with error: %s\n", https.errorToString(httpCode).c_str());
    }
    https.end();
  } 
  return response;
}

String getCachedPlaybackState() {
  if (millis() - lastFetchTime > cacheDuration) {
    playbackStateJson = fetchPlaybackState();
    if (playbackStateJson == "") {
      Serial.println("PlaybackStateJson is empty from cache update.");
    } else {
      Serial.println("Playback state cache updated.");
    }
    lastFetchTime = millis();
  }
  return playbackStateJson;
}

bool fetchAndStoreImage(String imageUrl) {
  HTTPClient https;

  if (https.begin(imageUrl)) {
    int httpCode = https.GET();

    if (httpCode == HTTP_CODE_OK) {
      imageSize = https.getSize();

      if (imageSize > 0) {
        imageBuffer = (uint8_t*)malloc(imageSize);

        if (imageBuffer) {
          WiFiClient* stream = https.getStreamPtr();
          size_t bytesRead = 0;

          while (bytesRead < imageSize) {
            bytesRead += stream->read(imageBuffer + bytesRead, imageSize - bytesRead);
          }
          https.end();

          Serial.println("Got image");

          return true;
        }
      }
    } else {
      Serial.println("Bad request for image.");
    }
    https.end();
  } 
  return false;
}

String fetchAvailableDevices() {
  const String url = "https://api.spotify.com/v1/me/player/devices";

  HTTPClient https;
  String response;
  if (https.begin(url)) {
    String authHeader = "Bearer " + SPOTIFY_ACCESS_TOKEN;

    https.addHeader("Authorization", authHeader);

    uint8_t httpCode = https.GET();
    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK) {
        response = https.getString();
      }
    } else {
      Serial.printf("HTTP request for devices failed with error: %s\n", https.errorToString(httpCode).c_str());
    }
    https.end();
  }
  Serial.println("Devices response: " + response);
  return response;
}

void transferPlayback(String deviceId) {
    const String url = "https://api.spotify.com/v1/me/player";

    HTTPClient https;
    if (https.begin(url)) {
        https.addHeader("Authorization", "Bearer " + SPOTIFY_ACCESS_TOKEN);
        https.addHeader("Content-Type", "application/json");

        String data = "{\"device_ids\": [\"" + deviceId + "\"], \"play\": true}";

        uint8_t httpCode = https.PUT(data);

        if (httpCode > 0) {
          if (httpCode == HTTP_CODE_NO_CONTENT) {
            Serial.println("Playback transferred to device");
          } else {
            Serial.printf("Failed to transfer playback with error: %d\n", httpCode);
          }
        } else {
          Serial.printf("HTTP request for transfer playback failed: %s\n", https.errorToString(httpCode).c_str());
        }
        https.end(); 
    }
}