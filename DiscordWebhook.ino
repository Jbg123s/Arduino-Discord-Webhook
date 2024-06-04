#include "WiFiS3.h"
#include "WiFiSSLClient.h"
#include "DiscordWebhook.h"

DiscordWebhook Discordwebhook;

// secrets...
char ssid[] = "";  // your network SSID (name)
char pass[] = "";  // your network password (use for WPA, or use as key for WEP)

int status = WL_IDLE_STATUS;

WiFiSSLClient client;

String incoming;

void setup() {
  //Initialize serial
  Serial.begin(9600);

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);

    status = WiFi.begin(ssid, pass);

    delay(5000);
  }

  Discordwebhook.connect();
  Discordwebhook.APIurl("/api/webhooks/.../...");
  Discordwebhook.send_embed_message("title", "description", "HEX", "imageUrl", "thumbnailUrl", "footerText", "footerImageUrl");

}

void loop() {

}