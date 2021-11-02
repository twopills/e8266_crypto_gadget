#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <EasyButton.h>
#include <ArduinoJson.h>
#include "./config/myHelpers.h"
#define BUTTON_PIN 0
EasyButton button(BUTTON_PIN);

void setup()
{
  Serial.begin(115200);

  initWifi();

  button.begin();
  button.onPressed(rqShibaAssets);
  rqShibaAssets();
}

void loop()
{
  button.read();
}

void rqShibaAssets()
{
  const char *host = apiShiba;

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("SIAMO CONNESSI RAGA!");

    WiFiClientSecure client;
    HTTPClient http;

    client.setInsecure();
    client.connect(host, 403);

    http.begin(client, host);

    String payload;
    if (http.GET() == HTTP_CODE_OK)
    {
      payload = http.getString();
      // Serial.println(payload);
      DynamicJsonDocument doc(1024);

      char *payload1 = (char *)malloc(http.getSize() + 1);
      http.getString().toCharArray(payload1, http.getSize() + 1);
      Serial.println("PAYLOAD:");
      Serial.println(payload1);
      http.end();

      DeserializationError error = deserializeJson(doc, payload1);
      free(payload1);

      if (error)
      {
        Serial.print(F("deserializeJson() failed with code "));
        Serial.println(error.c_str());
      }

      Serial.println("Eccoci:");
      JsonObject obj = doc.as<JsonObject>();

      if (obj.containsKey("data"))
      {
        Serial.println("Contiene data:");
        for (JsonPair kv : obj)
        {
          Serial.println("KEY: ");
          Serial.println(kv.key().c_str());

          //            for(JsonVariant value: kv.key().c_str()){ // nope, WIP
          //              Serial.println(value.as<char*>())
          //            }

          Serial.println("VALUE: ");
          Serial.println(kv.value().as<char *>());
        }
      }
      // serializeJsonPretty(doc, Serial);
    }
  }
}

void initWifi()
{
  WiFi.begin(ssid, pwd);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
}
