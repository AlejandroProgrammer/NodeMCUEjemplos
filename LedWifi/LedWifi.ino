#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
 
const char ssid[] = "WIFI NAME";
const char pass[] = "PASSWORD";
 
// Web server port
const uint16_t port = 80;
boolean ledStatus = false;
unsigned char status_led=0;
 
// Web server object declaration
ESP8266WebServer server(port);

   
void setup()
{
    // Set pin as output
    pinMode(D2, OUTPUT);
    // Setup serial communication
    Serial.begin(74880);
 
    // *** Connect to a WiFi acces point ***
    Serial.printf("Connecting to %s ", ssid);
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.printf(" Connected\n");
    Serial.printf("IP address: %s\n", WiFi.localIP().toString().c_str());
 
    // *** Start web server ***
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
      // *** Turn on or off the LED ***
    if (server.arg("led") == "ON"){
      digitalWrite(D2, HIGH);
      status_led=1;
    }
    else if (server.arg("led") == "OFF"){
      digitalWrite(D2, LOW);
      status_led=0;
    }

  String webPage;
  
    webPage += "<html>\r\n";
    webPage += "<head>\r\n";
    webPage += "<title>Alejandro Control</title>\r\n";
    webPage += "</head>\r\n";
    webPage += "<body>\r\n";
    if(status_led == 1){
      webPage += "<h1>LED: ON</h1>\r\n";
    }else{
      webPage += "<h1>LED: OFF</h1>\r\n";
    }
    webPage += "<form method=\"get\">\r\n";
    webPage += "<input type=\"submit\" name=\"led\" value=\"ON\">\r\n";
    webPage += "<input type=\"submit\" name=\"led\" value=\"OFF\">\r\n";
    webPage += "</form>\r\n";
    webPage += "</body>\r\n";
    webPage += "</html>\r\n";
    
    server.send(200, "text/html", webPage);
}
