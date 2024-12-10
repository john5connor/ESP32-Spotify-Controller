#include "wifiSetup.h"
#include "config.h"
#include <WiFi.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2 //Pin number for the onboard LED
#endif

WebServer server(80);
String spotifyCode;
bool authComplete = false;
String IP_ADDRESS = "";
String REDIRECT_URI;

void setupWifi(void) {

  pinMode(LED_BUILTIN, OUTPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); //Connect to the WiFi network
  Serial.println("Connecting to: " + String(WIFI_SSID));

  while (WiFi.status() != WL_CONNECTED) { //Flash LED while we wait to connect
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
  }

  Serial.printf("Signal strength: %d dBm\n", WiFi.RSSI());
  digitalWrite(LED_BUILTIN, HIGH); //Turn on LED when we are connected

  IP_ADDRESS = WiFi.localIP().toString();
  Serial.println("IP address: " + IP_ADDRESS);
  REDIRECT_URI = String("http://") + IP_ADDRESS + "/callback"; //Update the redirect URI with the IP address
}

void setupWebServerForAuth(void) {
  server.on("/callback", handleAuthCallback); //Set the ESP32 to listen for the callback
  server.begin();
  Serial.println("Server started");
}

void handleAuthCallback(void) {
  Serial.println("Handling auth callback...");
  if (server.hasArg("code")) { //Check if the code parameter is present
    spotifyCode = server.arg("code"); //Store the code parameter 
    server.send(200, "text/plain", "Success! You can now close this window.");
    Serial.println("Authorization complete.");
    authComplete = true;
  } else {
    server.send(400, "text/plain", "Error: no code provided.");
  }
}