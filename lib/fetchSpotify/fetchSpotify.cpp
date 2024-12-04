#include "fetchSpotify.h"
#include "config.h"
#include <HTTPClient.h>

String fetchSpotifyPlaylist(String accessToken) {
  String url = String(SPOTIFY_API_URL); //URL for the Spotify API playlist endpoint

  HTTPClient https;
  String response;
  if (https.begin(url)) {
    //Setup headers for GET request
    String authHeader = "Bearer " + accessToken;

    //Add headers to http request
    https.addHeader("Authorization", authHeader);

    //Execute the GET request
    int httpCode = https.GET();

    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK) {
        response = https.getString();
        Serial.println("Response:");
        Serial.println(response);
      }
    } else {
      Serial.printf("HTTP GET request failed, error: %s\n", https.errorToString(httpCode).c_str());
    }
    https.end();
  } else {
    Serial.println("Failed to connect to Spotify API");
  }

    return response;
}
