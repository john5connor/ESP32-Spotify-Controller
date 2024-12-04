#include "wifiSetup.h"
#include "authorizeSpotify.h"
#include "fetchSpotify.h"


void setup() {
  setupWifi(); //Setup WiFi connection with ESP32
  
  requestUserAuthorization();
  setupWebServerForAuth(); //Setup the web server

  while (!authComplete) { //Wait for the user to authorize the app
    server.handleClient();
  }

  String accessToken = requestAccessToken(spotifyCode);
  String playlist = fetchSpotifyPlaylist(accessToken);
  
  Serial.println("Playlist JSON...");
  Serial.println(playlist);
}

void loop() {
  server.handleClient(); //Handle incoming requests
}
