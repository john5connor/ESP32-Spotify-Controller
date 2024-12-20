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
//TFT_eSprite play = TFT_eSprite(&tft); // Create an instance of TFT_eSprite for the play symbol
TFT_eSprite playPauseImg = TFT_eSprite(&tft); // Create an instance of TFT_eSprite for the pause symbol
int spriteX = 15;
int spriteY = 15;

void playBox(int x, int y, int spriteX, int spriteY, bool isPlaying);
void updateScreen(String playbackStateJson);
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

  setupWifi(); //Setup WiFi connection with ESP32

  requestUserAuthorization(); //Request user authorization from the Spotify API

  configureLandingPage(); //Configure landing page for user to navigate to to authorize the app
  tft.drawString("Type the following" , 10, 10);
  tft.drawString("IP address into your", 10, 20);
  tft.drawString("browser: ", 10, 30);
  tft.drawString(IP_ADDRESS, 10, 50); 

  setupWebServerForAuth(); //Setup the web server

  while (!authComplete) { //Wait for the user to authorize the app
    server.handleClient();
  }

  requestAccessToken(spotifyCode); //Request an access token from the Spotify API
  parseAvailableDevices(fetchAvailableDevices());

  //Set up the buttons
  pinMode(PREVIOUS_BUTTON_PIN, INPUT_PULLUP);
  pinMode(PLAY_PAUSE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(NEXT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(POTENTIOMETER_PIN, INPUT_PULLUP);
}

void loop() {
  static unsigned long lastPlayPauseTime = 0; // Cooldown period in milliseconds
  static unsigned long lastPreviousTime = 0;
  static unsigned long lastNextTime = 0;
  static unsigned long lastVolumeChangeTime = 0;

  static uint16_t lastVolumeValue = 100; // previously
  static String lastSong = "";
  const unsigned long apiCooldown = 500; // Cooldown period in milliseconds

  playbackStateJson = getCachedPlaybackState(); // Constantly update/cache the playback state from the Spotify API

  if (songChange(lastSong, playbackStateJson)) {
    updateScreen(playbackStateJson);
  }

  currentVolumeValue = readAndMapVolumeValue();

  if (abs(lastVolumeValue - currentVolumeValue) > 10) {
    putVolumeChange(currentVolumeValue); //Make request to spotify to change the volume
    lastVolumeValue = currentVolumeValue; //Update the last volume value
  }

  lastSong = parseLastSong(playbackStateJson);

  if (previousButtonPressed()) {
      unsigned long currentTime = millis();
      if (currentTime - lastPreviousTime > apiCooldown) {
          previousSong();
          playbackStateJson = fetchPlaybackState(); // Get updated playback-state when song is changed
          lastPreviousTime = currentTime;
      } else {
          Serial.println("API call ignored due to cooldown");
      }
  }

  if (playPauseButtonPressed()) {
      unsigned long currentTime = millis();
      if (currentTime - lastPlayPauseTime > apiCooldown) {
          playPauseSong(playbackStateJson);
          playBox(tft.width() / 2 - 10, tft.height() - 30, spriteX, spriteY, isPlaying);
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
          lastNextTime = currentTime;
      } else {
          Serial.println("API call ignored due to cooldown");
      }
  }
  
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

void playBox(int x, int y, int spritex, int spritey, bool isPlaying) {
  Serial.println("isPlaying in playBox: " + String(isPlaying));
  playPauseImg.createSprite(spritex, spritey);

  if (isPlaying) {
    playPauseImg.fillSprite(TFT_BLACK);

    uint8_t rectWidth = spritex / 5;
    uint8_t rectSpacing = spritex / 8;

    playPauseImg.fillRect(rectSpacing, 0, spritey / 3, spritey, TFT_WHITE);
    playPauseImg.fillRect(spritex - rectWidth - rectSpacing, 0, spritey / 3, spritey, TFT_WHITE);
  } else {
    playPauseImg.fillSprite(TFT_BLACK);
    playPauseImg.fillTriangle(0, 0, 0, spritex, spritey, spritey / 2, TFT_WHITE);
  }

  playPauseImg.pushSprite(x, y);
  playPauseImg.deleteSprite();
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
  
  tft.drawString(song, (tft.width() / 2) - (song.length() / 2) * 6, tft.height() - 55);
  tft.drawString(artists, (tft.width() / 2) - (artists.length() / 2) * 6, tft.height() - 43);
  playBox(tft.width() / 2 - 10, tft.height() - 30, spriteX, spriteY, isPlaying);
}