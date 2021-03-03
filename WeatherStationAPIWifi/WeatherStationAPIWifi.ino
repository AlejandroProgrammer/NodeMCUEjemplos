#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char *ssid = "WIFI NAME";
const char *password = "PASSWORD";

const int ledAzul = D1;
const int ledAmarillo = D2;
const int ledRojo = D3;

String Location = "Santiago, CL";
String API_Key = "API KEY";

void setup() {
  Serial.begin(9600);
  delay(1000);

  pinMode(ledAzul, OUTPUT);
  pinMode(ledAmarillo, OUTPUT);
  pinMode(ledRojo, OUTPUT);
 
  WiFi.begin(ssid, password);
 
  Serial.print("Conectando.");
  while ( WiFi.status() != WL_CONNECTED )
  {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("connected");
  delay(1000);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;
    http.begin("http://api.openweathermap.org/data/2.5/weather?q=" + Location + "&APPID=" + API_Key);
    int httpCode = http.GET();
    if (httpCode > 0)
    {
      String payload = http.getString();
      DynamicJsonBuffer jsonBuffer(512);
      JsonObject& root = jsonBuffer.parseObject(payload);
      
      if (!root.success()) {
        Serial.println(F("Parsing failed!"));
        return;
      }
      
      float temp = (float)(root["main"]["temp"]) - 273.15;
      int humidity = root["main"]["humidity"];
      float wind_speed = root["wind"]["speed"];
      const char* country = root["sys"]["country"];

      Serial.printf("Temperatura = %.2f°C\r\n", temp);
      if(temp <= 15){
        
        digitalWrite(ledAzul,HIGH);
        digitalWrite(ledAmarillo,LOW);
        digitalWrite(ledRojo,LOW);
        
      }else if(temp <= 26){
        
        digitalWrite(ledAmarillo, HIGH);
        digitalWrite(ledAzul,LOW);
        digitalWrite(ledRojo,LOW);
        
      }else if(temp > 26){

        digitalWrite(ledRojo, HIGH);
        digitalWrite(ledAmarillo, LOW);
        digitalWrite(ledAzul,LOW);
        
      }
      
      Serial.printf("Humedad = %d %%\r\n", humidity);
      Serial.printf("Velocidad Viento = %.1f m/s\r\n", wind_speed); 
      Serial.printf(country);
         
     }
      http.end();
  }
  delay(60000);
}
