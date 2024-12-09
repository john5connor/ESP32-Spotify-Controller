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
      }
    }
    https.end();
  } 
  return response;
}

bool fetchAndStoreImage(const char* imageUrl) {
  HTTPClient https;

  if (https.begin(imageUrl)) {
    Serial.println("Getting url");
    int httpCode = https.GET();

    if (httpCode == HTTP_CODE_OK) {
      imageSize = https.getSize();

      Serial.println("Image size: " + String(imageSize));

      if (imageSize > 0) {
        imageBuffer = (uint8_t*)malloc(imageSize);

        if (imageBuffer) {
          WiFiClient* stream = https.getStreamPtr();
          Serial.println("Reading image as pointer...");
          size_t bytesRead = 0;

          while (bytesRead < imageSize) {
            bytesRead += stream->read(imageBuffer + bytesRead, imageSize - bytesRead);
          }
          https.end();
          Serial.println("Got image");
          return true;
        }
        https.end();
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
  Serial.printf("Free heap before HTTP request: %d bytes\n", ESP.getFreeHeap());
  if (https.begin(url.c_str())) {
  Serial.printf("Free heap after HTTP request: %d bytes\n", ESP.getFreeHeap());
    String authHeader = "Bearer " + SPOTIFY_ACCESS_TOKEN;

    https.addHeader("Authorization", authHeader);

    uint8_t httpCode = https.GET();
    Serial.printf("Http code for devices: %d\n" ,httpCode);

    if (httpCode == HTTP_CODE_OK) {
      response = https.getString();
    }
    https.end();
  }
  Serial.println("Devices response: " + response);
  return response;
}