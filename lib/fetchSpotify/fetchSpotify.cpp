#include "fetchSpotify.h"
#include "config.h"
#include <HTTPClient.h>
#include <WiFiClient.h>

uint8_t* imageBuffer = nullptr;
size_t imageSize = 0;

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
  // Change later ?
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
      } else {
        Serial.printf("Bad code via HTTP request for playback-state failed with error: %s\n", https.errorToString(httpCode).c_str());
      }
    } else {
      Serial.printf("HTTP request for playback-state failed with error: %s\n", https.errorToString(httpCode).c_str());
    }
    https.end();
  } 
  return response;
}

bool fetchAndStoreImage(const char* imageUrl) {
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