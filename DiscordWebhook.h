#ifndef DiscordWebhook_h
#define DiscordWebhook_h

#include "WiFiS3.h"
#include "WiFiSSLClient.h"

class DiscordWebhook {
public:
  void connect();
  void APIurl(String APIurl);
  void send_message(String messageToSend);
  void send_embed_message(String embedTitle, String embedDescription, String embedColor, String imageUrl, String thumbnailUrl, String footerText, String footerImageUrl);
  void edit_message(String newContent);
};

#endif