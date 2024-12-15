#include "config.h"
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

void updateScreen(String playbackStateJson);
void displayJPEG();
uint8_t fetchCount = 0;

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

  Serial.println(playbackStateJson);
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
  parseAvailableDevices(fetchAvailableDevices());

  playbackStateJson = fetchPlaybackState(); //Fetch the playback state from the Spotify API
  lastFetchTime = millis();
  Serial.println("Playback state json from setup code: " + playbackStateJson);

  if (playbackStateJson == "") {
    Serial.println("Playback state is empty, transferring playback to device");
    transferPlayback(deviceId);
  }

  updateScreen(playbackStateJson);

  pinMode(PREVIOUS_BUTTON_PIN, INPUT_PULLUP);
  pinMode(PLAY_PAUSE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(NEXT_BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  static unsigned long lastPlayPauseTime = 0;
  static unsigned long lastPreviousTime = 0;
  static unsigned long lastNextTime = 0;
  const unsigned long apiCooldown = 500; // Cooldown period in milliseconds

  playbackStateJson = getCachedPlaybackState(); // Constantly update/cache the playback state from the Spotify API

  if (previousButtonPressed()) {
      unsigned long currentTime = millis();
      if (currentTime - lastPreviousTime > apiCooldown) {
          previousSong();
          playbackStateJson = fetchPlaybackState(); // Get updated playback-state when song is changed
          updateScreen(playbackStateJson);
          lastPreviousTime = currentTime;
      } else {
          Serial.println("API call ignored due to cooldown");
      }
  }

  if (playPauseButtonPressed()) {
      unsigned long currentTime = millis();
      if (currentTime - lastPlayPauseTime > apiCooldown) {
          playPauseSong(playbackStateJson);
          updateScreen(playbackStateJson);
          lastPlayPauseTime = currentTime;
      } else {
          Serial.println("API call ignored due to cooldown");
      }
  }

  if (nextButtonPressed()) {
      unsigned long currentTime = millis();
      if (currentTime - lastNextTime > apiCooldown) {
          nextSong();
          playbackStateJson = fetchPlaybackState(); // Get updated playback-state when song is changed
          updateScreen(playbackStateJson);
          lastNextTime = currentTime;
      } else {
          Serial.println("API call ignored due to cooldown");
      }
  }
  
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

void updateScreen(String playbackStateJson) {

  //parseAvailableDevices(fetchAvailableDevices());
  tft.fillScreen(TFT_BLACK);
  parseImageUrl(playbackStateJson);

  if (fetchAndStoreImage(imageUrl)) {
    displayJPEG();
  }

  parseSong(playbackStateJson);
  parseArtists(playbackStateJson);
  
  //TODO: Put this functionality in the loop so it updates the display when the song is changed
  tft.drawString(song, (tft.width() / 2) - (song.length() / 2) * 6, tft.height() - 55);
  tft.drawString(artists, (tft.width() / 2) - (artists.length() / 2) * 6, tft.height() - 43);
}