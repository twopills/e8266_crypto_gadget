#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <EasyButton.h>

// Arduino pin where the button is connected to.
#define BUTTON_PIN 0
// Instance of the button.
EasyButton button(BUTTON_PIN);


void setup()
{
  Serial.begin(115200);

  initWifi();
  onPressed();
  // rqShibaAssets();
  
}


void loop() {
  
}



// Callback function to be called when the button is pressed.
void onPressed() {
    // Initialize the button.
    button.begin();
    // Add the callback function to be called when the button is pressed.
    button.onPressed(Serial.println("Button has been pressed!"));
    
}

void rqShibaAssets() {
  const char* host = "https://data.messari.io/api/v1/assets/shiba-inu/metrics/market-data"; // only google.com not https://google.com

  if(WiFi.status() == WL_CONNECTED){
    Serial.println("SIAMO CONNESSI RAGA");
    
    WiFiClientSecure client;
    HTTPClient http;
    
    client.setInsecure(); //the magic line, use with caution
    client.connect(host, 403);
      
    http.begin(client, host);
    
    String payload;
    if (http.GET() == HTTP_CODE_OK)    
        payload = http.getString();   
        Serial.println(payload);
   }  
}

void initWifi() {
  WiFi.begin();

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }  
}
