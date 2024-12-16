#include "config.h"
#include "landingPage.h"
#include "wifiSetup.h"
#include "Preferences.h"

Preferences persistantStorage;
String input = "";

const char CONFIG_PAGE[] PROGMEM = R"=====( 
<!DOCTYPE html>
<html>
<head>
    <title>Spotify Authorization</title>
</head>
<body>
    <h1>Click the following link to authorize your Spotify device:</h1>
    <a href="{authorizeUrl}">Authorize</a>
</body>
</html>
)=====";


void handleLandingPage(void) {
    String page = CONFIG_PAGE;
    page.replace("{authorizeUrl}", authorizeUrl);

    server.send(200, "text/html", page);
}

void configureLandingPage(void) {
    persistantStorage.begin("spotify", false);

    server.on("/", handleLandingPage);
}