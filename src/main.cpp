#include "wifiSetup.h"
#include "authorizeSpotify.h"
#include "fetchSpotify.h"
#include "landingPage.h"
#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI(); // Create an instance of TFT_eSPI

void setup() {
  Serial.begin(115200);
  while (!Serial) { // Wait for serial to initialize
    delay(100);
  }

  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1); 

  setupWifi(); //Setup WiFi connection with ESP32
  configureLandingPage(); //Setup the landing page

  tft.drawString(IP_ADDRESS, 10, 10); 

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
  // do nothing for now
}
