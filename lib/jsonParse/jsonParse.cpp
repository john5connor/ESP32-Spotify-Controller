#include "jsonParse.h"
#include "config.h"
#include <ArduinoJson.h>

String imageUrl;
String artists;
String song;
String deviceId;

/*
void doDeserializeJson(String json) {
    doc.clear();

    DeserializationError error = deserializeJson(doc, json);

    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
    }
}
*/

void parseAccessToken(String response) {
    StaticJsonDocument<100> doc;

    deserializeJson(doc, response);

    if (doc["access_token"] == nullptr) {
        Serial.println("Access token is null");
    }

    SPOTIFY_ACCESS_TOKEN = String((const char*)doc["access_token"]);
}

void parseImageUrl(String playbackStateJson) {
    StaticJsonDocument<100> doc;

    deserializeJson(doc, playbackStateJson);

    imageUrl = (const char*)doc["item"]["album"]["images"][2]["url"];
}

void parseArtists(String playbackStateJson) {
    StaticJsonDocument<100> doc;
    deserializeJson(doc, playbackStateJson);
    /*
    uint8_t artistCount = doc["item"]["artists"].size(); //Get the number of artists on the track

    if (artistCount == 1) {
        return String((const char*)doc["item"]["artists"][0]["name"]);
    } else {
        String artists = "";
        for (uint8_t i = 1; i < artistCount; i++) {
            artists += String((const char*)doc["item"]["artists"][i]["name"]);
            if (i < artistCount) {
                artists += ", ";
            }
        }
        return artists;
    }
    */
    artists = String((const char*)doc["item"]["artists"][0]["name"]);
}

void parseSong(String playbackStateJson) {
    StaticJsonDocument<100> doc;
    deserializeJson(doc, playbackStateJson);
    song = String((const char*)doc["item"]["name"]);
}

void parseAvailableDevices(String devicesJson) {
    StaticJsonDocument<100> doc;
    deserializeJson(doc, devicesJson);

    if (doc["devices"].size() > 1) {
        for (int i = 0; i < doc["devices"].size(); i++) {
            if (String((const char*)doc["devices"][i]["name"]) == "iPhone") {
                deviceId = String((const char*)doc["devices"][i]["id"]);
            }
        }
    } else {
        deviceId = String((const char*)doc["devices"][0]["id"]);
    }

    Serial.println("Device ID: " + deviceId);
}

String parseLastSong(String playbackStateJson) {
    StaticJsonDocument<50> doc;
    deserializeJson(doc, playbackStateJson);
    return String((const char*)doc["item"]["name"]);
}