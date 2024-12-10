#include "player.h"
#include "config.h"
#include <ArduinoJson.h>
#include <HTTPClient.h>

void playPauseSong(String playbackStateJson) {
    StaticJsonDocument <50> doc;

    deserializeJson(doc, playbackStateJson); 

    bool isPlaying = doc["is_playing"];

    Serial.println("Is playing: ");
    Serial.println(isPlaying);

    String payload = R"({
        "context_uri": "{CONTEXT_URI}",
        "position_ms": {POSITION_MS}
    })";

    payload.replace(String("{CONTEXT_URI}"), String(doc["item"]["album"]["uri"].as<const char*>()));
    payload.replace(String("{POSITION_MS}"), String(doc["progress_ms"].as<long>()));

    HTTPClient http;

    if (isPlaying) {
        http.begin("https://api.spotify.com/v1/me/player/pause");
    } else {
        http.begin("https://api.spotify.com/v1/me/player/play");
    }

    http.addHeader("Authorization", "Bearer " + SPOTIFY_ACCESS_TOKEN);
    http.addHeader("Content-Type", "application/json");

    Serial.println("Pausing/playing song!");
    uint8_t httpCode = http.PUT(payload);

    http.end();
}