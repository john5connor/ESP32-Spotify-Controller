#include "wifiSetup.h"
#include "authorizeSpotify.h"
#include "fetchSpotify.h"
#include "landingPage.h"
#include "jsonParse.h"
#include <TFT_eSPI.h>
#include <SPI.h>
#include <TJpg_Decoder.h>

TFT_eSPI tft = TFT_eSPI(); // Create an instance of TFT_eSPI

void displayJPEG();

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

  // The following two lines may be be used in the future when implementing the landing page.
  // Currently unsure how it will be implemented.
  //configureLandingPage();
  //tft.drawString(IP_ADDRESS, 10, 10); 

  requestUserAuthorization();
  setupWebServerForAuth(); //Setup the web server

  while (!authComplete) { //Wait for the user to authorize the app
    server.handleClient();
  }

  String accessToken = requestAccessToken(spotifyCode);
  //String playlist = fetchSpotifyPlaylist();
  String playbackState = fetchPlaybackState();

  if (fetchAndStoreImage(parseImageUrl(playbackState))) {
    displayJPEG();
  }

  tft.drawString(parseArtists(playbackState), tft.width() / 8, tft.height() - 30);

}

void loop() {
  // do nothing for now
}

bool outputTFT(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap) {
 if (x > tft.width() || y > tft.height()) {
   return false;
 }
 tft.pushImage(x, y, w, h, bitmap);
 return true;
}

void displayJPEG() {
  if (imageBuffer) {
    TJpgDec.setCallback(outputTFT);
    TJpgDec.setSwapBytes(true);
    TJpgDec.drawJpg(tft.width() / 4, tft.height() / 4, imageBuffer, imageSize);

    free(imageBuffer);
    imageBuffer = nullptr;
  } 
}