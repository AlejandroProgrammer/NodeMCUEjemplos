#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
 
const char ssid[] = "WIFI NAME";
const char pass[] = "PASSWORD";
 
// Web server port
const uint16_t port = 80;

// Web server object declaration
ESP8266WebServer server(port);

int rled = D1; // Pin PWN D1 RED
int gled = D2; // Pin PWM D2 GREEN
int bled = D3; // Pin PWM D3 BLUE

//Rgb Color
int r = 000;
int g = 000;
int b = 000;
   
void setup()
{

  // Setup serial communication
    Serial.begin(74880);

    pinMode(rled, OUTPUT);
    pinMode(bled, OUTPUT);
    pinMode(gled, OUTPUT);
 
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
    if(server.arg("red") != "" && server.arg("green") != "" && server.arg("blue") != ""){
      r = server.arg("red").toInt(); 
      g = server.arg("green").toInt(); 
      b = server.arg("blue").toInt(); 
      analogWrite(rled, r);
      analogWrite(gled, g);
      analogWrite(bled, b);
    }
        
    String webPage;
  
    webPage += "<html>\r\n";
    webPage += "<head>\r\n";
    webPage += "<title>RGB-Controller</title>\r\n";
    webPage += "</head>\r\n";
    webPage += "<body>\r\n";
    webPage += "<form method=\"get\">\r\n";
    webPage += "<h1>R:</h1>\r\n";
    webPage += "<br>\r\n";
    webPage += "<input type=\"range\" value=" + (String)r + " name=\"red\" min=\"0\" max=\"255\">\r\n";
    webPage += "<br>\r\n";
    webPage += "<h1>G:</h1>\r\n";
    webPage += "<br>\r\n";
    webPage += "<input type=\"range\" value=" + (String)g + " name=\"green\" min=\"0\" max=\"255\">\r\n";
    webPage += "<br>\r\n";
    webPage += "<h1>B:</h1>\r\n";
    webPage += "<br>\r\n";
    webPage += "<input type=\"range\" value=" + (String)b + " name=\"blue\" min=\"0\" max=\"255\">\r\n";
    webPage += "<br>\r\n";
    webPage += "<input type=\"submit\" name=\"Cambiar Color\">\r\n";
    webPage += "</form>\r\n";
    webPage += "<br>\r\n";
    webPage += "<h1 style=background-color:rgb(" + (String)r + "," + (String)g + "," + (String)b + ");>Color</h1>\r\n";
    webPage += "</body>\r\n";
    webPage += "</html>\r\n";
    
    server.send(200, "text/html", webPage);
}
