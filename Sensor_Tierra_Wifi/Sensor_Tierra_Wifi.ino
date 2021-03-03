#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
 
const char ssid[] = "WIFI NAME";
const char pass[] = "PASSWORD";
 
const uint16_t port = 80;

ESP8266WebServer server(port);
StaticJsonBuffer<200> jsonBuffer;

const int sensorPinAnalogico = A0;
const int sensorPinDigital = D1;

String estado;
int valor;
   
void setup()
{
    pinMode(sensorPinDigital, INPUT);
    
    Serial.begin(74880);
    
    Serial.printf("Connecting to %s ", ssid);
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.printf(" Connected\n");
    Serial.printf("IP address: %s\n", WiFi.localIP().toString().c_str());
 
    server.on("/", HTTP_GET, handleRoot);
    server.begin();
    Serial.printf("Web server started at port %d\n", port);
}
 
void loop()
{
    server.handleClient();
}

void handleRoot()
{
  int humedad = digitalRead(sensorPinDigital);
   if (humedad == HIGH)
   {
      estado = "Necesario regar";
   }else{
      estado = "Tierra Humeda";
   }
   
   valor = analogRead(sensorPinAnalogico);
   delay(1000);
   
  server.send ( 200, "application/json", createJsonResponse(estado, valor));
}

String createJsonResponse(String estadoSensor, int valorSensor) {
  StaticJsonBuffer<500> jsonBuffer;
  JsonObject &root = jsonBuffer.createObject();
  JsonArray &tempValues = root.createNestedArray("Estado");
  tempValues.add(estadoSensor);
  JsonArray &humiValues = root.createNestedArray("Valor");
  humiValues.add(valorSensor);
  String json;
  root.prettyPrintTo(json);
  return json;
}
