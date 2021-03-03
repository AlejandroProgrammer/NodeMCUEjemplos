#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
 
const char ssid[] = "WIFI NAME";
const char pass[] = "PASSWORD";
 
const uint16_t port = 80;
String bombaEstado = "ON";
const int bomba = D2;
ESP8266WebServer server(port);

   
void setup()
{
    pinMode(bomba, OUTPUT);
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
    if (server.arg("bomba") == "ON"){
      digitalWrite(bomba, LOW);
      bombaEstado = "ON";
    }
    else if (server.arg("bomba") == "OFF"){
      digitalWrite(bomba, HIGH);
      bombaEstado = "OFF";
    }

  String webPage;
  
    webPage += "<html>\r\n";
    webPage += "<head>\r\n";
    webPage += "<title>Alejandro Bomba Control</title>\r\n";
    webPage += "</head>\r\n";
    webPage += "<body>\r\n";
    if(bombaEstado == "ON"){
      webPage += "<h1 id=\"estado\">BOMBA: ON</h1>\r\n";
    }else
    {
      webPage += "<h1 id=\"estado\">BOMBA: OFF</h1>\r\n";
    }
    webPage += "<form method=\"get\">\r\n";
    webPage += "<input type=\"submit\" name=\"bomba\" value=\"ON\">\r\n";
    webPage += "<input type=\"submit\" name=\"bomba\" value=\"OFF\">\r\n";
    webPage += "</form>\r\n";
    webPage += "</body>\r\n";
    webPage += "</html>\r\n";
    
    server.send(200, "text/html", webPage);
}
