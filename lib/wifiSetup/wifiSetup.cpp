#include "wifiSetup.h"
#include "config.h"
#include <WiFi.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2 //Pin number for the onboard LED
#endif

WebServer server(80);
String spotifyCode = "";
bool authComplete = false;

void setupWifi(void) {
  

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.println("Connecting to");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); //Connect to the WiFi network

  while (WiFi.status() != WL_CONNECTED) { //Flash LED while we wait to connect
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
  }

  digitalWrite(LED_BUILTIN, HIGH); //Turn on LED when we are connected

  Serial.println("Connected! IP address: ");
  Serial.println(WiFi.localIP());
}

void setupWebServerForAuth(void) {
  server.begin();
  server.on("/callback", handleAuthCallback); //Set the ESP32 to listen for the callback
  Serial.println("Server started, waiting for Spotify callback...");
}

void handleAuthCallback(void) {
  if (server.hasArg("code")) { //Check if the code parameter is present
    spotifyCode = server.arg("code"); //Store the code parameter 
    server.send(200, "text/plain", "Success! You can now close this window.");
    authComplete = true;
  } else {
    server.send(400, "text/plain", "Error: no code provided.");
  }
}