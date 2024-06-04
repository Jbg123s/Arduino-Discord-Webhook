#include "DiscordWebhook.h"
#include "WiFiS3.h"
#include "WiFiSSLClient.h"

WiFiSSLClient wificlient;

char discordServer[] = "discord.com";
String apiURL;
String lastMessageId;
bool isConnected = false;

void DiscordWebhook::connect() {
  if (WiFi.status() == WL_CONNECTED) {
    if (wificlient.connect(discordServer, 443)) {
      Serial.println("Connected to Discord server");
      isConnected = true;
    }
  } else {
    Serial.println("No internet connection found");
  }
}

void DiscordWebhook::APIurl(String APIurl) {
  apiURL = APIurl;
}

void DiscordWebhook::send_message(String messageToSend) {
  // if (isConnected) {
  if (wificlient.connect(discordServer, 443)) {
    // message and content as JSON
    String message = "{\"content\": \"" + messageToSend + "\"}";
    const char* char_message = message.c_str();

    // HTTP request
    wificlient.println("POST " + apiURL + "?wait=true HTTP/1.1");
    wificlient.println("Host: discord.com");
    wificlient.println("Content-Type: application/json");
    wificlient.print("Content-Length: ");
    wificlient.println(strlen(char_message));
    wificlient.println("Connection: close");
    wificlient.println();
    wificlient.println(char_message);
    wificlient.println();

    // Read the response to get the message ID
    while (wificlient.connected()) {
      String line = wificlient.readStringUntil('\n');
      if (line.startsWith("{\"id\":\"")) {
        // Assuming the response contains the message ID in the format '{"id": "your_message_id"}'
        int start = line.indexOf("{\"id\":\"") + 7;
        int end = line.indexOf("\"}", start) + -527;
        lastMessageId = line.substring(start, end);
        Serial.println(lastMessageId);
      }
    }

    while (wificlient.available()) wificlient.read();
    wificlient.stop();

  } else {
    Serial.println("No connection to Discord found");
  }
}

void DiscordWebhook::send_embed_message(String embedTitle, String embedDescription, String embedColor, String imageUrl, String thumbnailUrl, String footerText, String footerImageUrl) {
  // if (isConnected) {
  if (wificlient.connect(discordServer, 443)) {
    unsigned long colorValue;
    if (embedColor == "") {
      colorValue = 0;
    } else {
      // Convert hex to int
      colorValue = strtoul(embedColor.c_str(), NULL, 16);
    }

    // Construct the embed message in JSON format with hex color and image URL
    String embedMessage = "{\"embeds\": [{\"title\": \"" + embedTitle + 
    "\", \"description\": \"" + embedDescription + 
    "\", \"color\": " + String(colorValue) + 
    ", \"image\": {\"url\": \"" + imageUrl + 
    "\"}, \"thumbnail\": {\"url\": \"" + thumbnailUrl + 
    "\"}, \"footer\": {\"text\": \"" + footerText + 
    "\", \"icon_url\": \"" + footerImageUrl + "\"}}]}";

    const char* char_embed_message = embedMessage.c_str();

    // HTTP request
    wificlient.println("POST " + apiURL + " HTTP/1.1");
    wificlient.println("Host: discord.com");
    wificlient.println("Content-Type: application/json");
    wificlient.print("Content-Length: ");
    wificlient.println(strlen(char_embed_message));
    wificlient.println("Connection: close");
    wificlient.println();
    wificlient.println(char_embed_message);
    while (wificlient.available()) wificlient.read();
    wificlient.stop();

  } else {
    Serial.println("No connection to Discord found");
  }
}

void DiscordWebhook::edit_message(String newContent) {
  if (wificlient.connect(discordServer, 443)) {
    // Construct the JSON payload for editing the message
    String editMessage = "{\"content\": \"" + newContent + "\"}";
    const char* char_edit_message = editMessage.c_str();

    // HTTP request
    wificlient.println("PATCH " + apiURL + "/messages/" + lastMessageId + " HTTP/1.1");
    wificlient.println("Host: discord.com");
    wificlient.println("Content-Type: application/json");
    wificlient.print("Content-Length: ");
    wificlient.println(strlen(char_edit_message));
    wificlient.println();
    wificlient.println(char_edit_message);

    //delay(1000); // Add a delay before closing the connection

    wificlient.stop(); // Close the connection
  } else {
    Serial.println("No connection to Discord found");
  }
}