/*
#include "landingPage.h"
#include "wifiSetup.h"
#include "Preferences.h"

Preferences persistantStorage;
String input = "";

const char CONFIG_PAGE[] PROGMEM = R"=====( 
<!DOCTYPE html>
<html>
<head>
    <title>ESP32 Configuration</title>
</head>
<body>
    <h1>Configure Redirect input</h1>
    <form action="/save" method="POST">
        <label for="input">input:</label><br>
        <input type="text" id="input" name="input" value="{INPUT}" size="50"><br><br>
        <input type="submit" value="Save">
    </form>
</body>
</html>
)=====";


void handleLandingPage(void) {
    String page = CONFIG_PAGE;
    page.replace("{INPUT}", input);

    server.send(200, "text/html", page);
}

void handleSave(void) {
    if (server.hasArg("input")) {
        input = server.arg("input");
        persistantStorage.putString("input", input);
        server.send(200, "text/plain", "Saved!");
    } else {
        server.send(400, "text/plain", "Error: no input provided.");
    }

    String testString = persistantStorage.getString("input", "default value for getString()");
    Serial.println("test string: " + testString);

}

void configureLandingPage(void) {
    persistantStorage.begin("spotify", false);

    server.on("/", handleLandingPage);
    server.on("/save", handleSave);
}
*/