#include "authorizeSpotify.h"
#include "config.h"
#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

String base64Encode(String str);
String parseAccessToken(String response);

String requestUserAuthorization(void) {
    String state = generateRandomString(16); //Generate a random string to protect against CSRF attacks

    String url = "https://accounts.spotify.com/authorize?"; //Base URL, followed by query parameters

    url += "client_id=" + String(CLIENT_ID);
    url += "&response_type=" + String(RESPONSE_TYPE);
    url += "&redirect_uri=" + String(REDIRECT_URI);
    url += "&state=" + state;
    url += "&scope=" + String(SCOPE);

    //The link must be manually accessed by the user, the ESP32 is unable to emulate a browser and automatically follow the redirect
    Serial.println("Click the following link to authorize with the Spotify API:");
    Serial.println(url);

    return url;
}

String requestAccessToken(String code) {
    String url = "https://accounts.spotify.com/api/token"; //URL for the Spotify API token endpoint

    WiFiClientSecure client;
    client.setInsecure(); //Ignore SSL certificate

    HTTPClient https;
    String response;
    if (https.begin(client, url)) {
        //Setup headers for POST request
        String payload = "code=" + code +
                         "&redirect_uri=" + String(REDIRECT_URI) +
                         "&grant_type=" + String(GRANT_TYPE);
        
        String authHeader = "Basic " + base64Encode(String(CLIENT_ID) + ":" + String(CLIENT_SECRET));

        //Add headers to http request
        https.addHeader("Authorization", authHeader);
        https.addHeader("Content-Type", "application/x-www-form-urlencoded");

        //Execute the POST request
        int httpCode = https.POST(payload);

        if (httpCode > 0) {
            if (httpCode == HTTP_CODE_OK) {
                response = https.getString();
            }
        } else {
            Serial.printf("HTTP POST request failed, error: %s\n", https.errorToString(httpCode).c_str());
        }
        https.end();
    } else {
        Serial.println("Unable to connect to Spotify API");
    }
    return parseAccessToken(response);
}

String generateRandomString(uint8_t len) {
   const char characters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                              "abcdefghijklmnopqrstuvwxyz"
                              "0123456789"; 

    const int charSetSize = sizeof(characters) - 1; // exclude null terminator
    String randomString = "";

    for (int i = 0; i < len; i++) {
        int index = random(0, charSetSize);
        randomString += characters[index];
    }

    return randomString;
}

String base64Encode(String str) {
    const char* encodeTable = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    String encoded = "";
    unsigned char charArray3[3];
    unsigned char charArray4[4];
    int i = 0, j = 0;

    for (size_t n = 0; n < str.length(); n++) {
        charArray3[i++] = str[n];
        if (i == 3) {
            charArray4[0] = (charArray3[0] & 0xfc) >> 2;
            charArray4[1] = ((charArray3[0] & 0x03) << 4) + ((charArray3[1] & 0xf0) >> 4);
            charArray4[2] = ((charArray3[1] & 0x0f) << 2) + ((charArray3[2] & 0xc0) >> 6);
            charArray4[3] = charArray3[2] & 0x3f;

            for (i = 0; i < 4; i++) {
                encoded += encodeTable[charArray4[i]];
            }
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 3; j++) {
            charArray3[j] = '\0';
        }

        charArray4[0] = (charArray3[0] & 0xfc) >> 2;
        charArray4[1] = ((charArray3[0] & 0x03) << 4) + ((charArray3[1] & 0xf0) >> 4);
        charArray4[2] = ((charArray3[1] & 0x0f) << 2) + ((charArray3[2] & 0xc0) >> 6);

        for (j = 0; j < i + 1; j++) {
            encoded += encodeTable[charArray4[j]];
        }

        while (i++ < 3) {
            encoded += '=';
        }
    }

    return encoded;
}

String parseAccessToken(String response) {
    DynamicJsonDocument doc(200); //Create a JSON document to parse the response

    DeserializationError error = deserializeJson(doc, response); //Deserialize the response

    if (error) {
        Serial.print("Failed to parse access token response: ");
        Serial.println(error.c_str());
        return "";
    }

    const char* accessToken = doc["access_token"]; //Extract the access token from the JSON response

    Serial.println("Parsed access token:");
    Serial.println(accessToken);

    return String(accessToken);
}
