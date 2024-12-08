#include <ArduinoJson.h>

String parseAccessToken(String response) {
    StaticJsonDocument<100> doc;

    deserializeJson(doc, response); //Deserialize the response

    //const char* accessToken = doc["access_token"]; //Extract the access token from the JSON response

    return String((const char*)doc["access_token"]);
}

String parseImageUrl(String playbackState) {
    StaticJsonDocument<100> doc; //Create a JSON document to parse the response

    deserializeJson(doc, playbackState); //Deserialize the response

    //const char* imageUrl = doc["item"]["album"]["images"][2]["url"]; //Extract the image URL from the JSON response

    return String((const char*)doc["item"]["album"]["images"][2]["url"]);
}

String parseArtists(String playbackState) {
    StaticJsonDocument<100> doc; //Create a JSON document to parse the response

    deserializeJson(doc, playbackState); //Deserialize the response

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

    return String((const char*)doc["item"]["artists"][0]["name"]);
}