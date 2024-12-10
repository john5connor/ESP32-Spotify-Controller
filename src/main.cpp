#include "wifiSetup.h"
#include "authorizeSpotify.h"
#include "fetchSpotify.h"
#include "landingPage.h"
#include "jsonParse.h"
#include "player.h"
#include "mechanics.h"
#include <TFT_eSPI.h>
#include <SPI.h>
#include <TJpg_Decoder.h>

TFT_eSPI tft = TFT_eSPI(); // Create an instance of TFT_eSPI
//TFT_eSprite scroller = TFT_eSprite(&tft); // Create an instance of TFT_eSprite

/*
const char* message = "Text message for scroll";
int msgWidth;
int scrollX;
*/
String playbackStateJson;

void displayJPEG();

void setup() {
  Serial.begin(115200);
  while (!Serial) { // Wait for serial to initialize
    delay(100);
  }

  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextFont(1);
  tft.setTextSize(1);
  /*
  tft.setTextWrap(false);

  msgWidth = tft.textWidth(message);

  scroller.setColorDepth(8);
  scroller.createSprite(msgWidth * 2, tft.fontHeight());
  scroller.fillSprite(TFT_BLACK);
  scroller.setTextColor(TFT_WHITE, TFT_BLACK);
  scroller.setTextDatum(TL_DATUM);

  scroller.drawString(message, 0, 0);
  scroller.drawString(message, msgWidth, 0);

  scrollX = 0;
  */


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

  requestAccessToken(spotifyCode); //Request an access token from the Spotify API
  playbackStateJson = fetchPlaybackState(); //Fetch the playback state from the Spotify API

  parseAvailableDevices(fetchAvailableDevices());
  parseImageUrl(playbackStateJson);

  
  if (fetchAndStoreImage(imageUrl)) {
    displayJPEG();
  }

  parseSong(playbackStateJson);
  parseArtists(playbackStateJson);

  if (song != nullptr) {
    Serial.println("Song not null!");
  }
  //TODO: Put this functionality in the loop so it updates the display when the song is changed
  tft.drawString(song, (tft.width() / 2) - (song.length() / 2) * 6, tft.height() - 55);
  tft.drawString(artists, (tft.width() / 2) - (artists.length() / 2) * 6, tft.height() - 43);
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  if (playPauseButtonPressed()) {
    playPauseSong(fetchPlaybackState());
  }
  //playPauseSong(fetchPlaybackState());
  /*
  tft.fillScreen(TFT_BLACK);
  int spriteY = (tft.height() - tft.fontHeight()) / 2; // Vertically center the sprite

  scroller.pushSprite(-scrollX, spriteY);

  scrollX++;
  if (scrollX >= msgWidth) {
    scrollX = 0;
  }
  
  delay(25);
  */
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
    TJpgDec.drawJpg(tft.width() / 4, tft.height() / 10, imageBuffer, imageSize);

    free(imageBuffer);
    imageBuffer = nullptr;
  } 
}