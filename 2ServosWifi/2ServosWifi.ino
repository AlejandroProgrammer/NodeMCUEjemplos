#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
 
const char ssid[] = "WIFI NAME";
const char pass[] = "PASSWORD";
 
// Web server port
const uint16_t port = 80;

#include <Servo.h>
Servo servo1;
Servo servo2;

int grados1 = 0;
int grados2 = 0;
 
// Web server object declaration
ESP8266WebServer server(port);

   
void setup()
{
servo1.attach(D2);
servo2.attach(D3);

servo1.write(0);
servo2.write(0);

delay(2000);

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
  if (server.arg("servo1") == "0"){
      servo1.write(0);
      grados1 = 0;
    }
    else if (server.arg("servo1") == "90"){
      servo1.write(90);
      grados1 = 90;
    }
    else if (server.arg("servo1") == "180"){
      servo1.write(180);
      grados1 = 180;
    }
    else if (server.arg("servo2") == "0"){
      servo2.write(0);
      grados2 = 0;
    }
    else if (server.arg("servo2") == "90"){
      servo2.write(90);
      grados2 = 90;
    }
    else if (server.arg("servo2") == "180"){
      servo2.write(180);
      grados2 = 180;
      
    }else if (server.arg("servo3") != ""){
      Serial.println("Servo 3 llamando");
      servo1.write(server.arg("servo3").toInt());
      grados1 = server.arg("servo3").toInt();
      
    }else if (server.arg("servo4") != ""){
      Serial.println("Servo 4 llamando");
      servo2.write(server.arg("servo4").toInt());
      grados2 = server.arg("servo4").toInt();
    }
    
    String webPage;
  
    webPage += "<html>\r\n";
    webPage += "<head>\r\n";
    webPage += "<title>Servo</title>\r\n";
    webPage += "</head>\r\n";
    webPage += "<body>\r\n";
    webPage += "<form method=\"get\">\r\n";
    if(grados1 == 0){
      webPage += "<h1>Servo Motor 1: 0 grados</h1>\r\n";
    }else if(grados1 == 90){
      webPage += "<h1>Servo Motor 1: 90 grados</h1>\r\n";
    }else if(grados1 == 180){
      webPage += "<h1>Servo Motor 1: 180 grados</h1>\r\n";
    }else{
      webPage += "<h1>Servo Motor 1:" + (String)grados1 + "grados</h1>\r\n";
    }
    webPage += "<input type=\"submit\" name=\"servo1\" value=\"0\">\r\n";
    webPage += "<input type=\"submit\" name=\"servo1\" value=\"90\">\r\n";
    webPage += "<input type=\"submit\" name=\"servo1\" value=\"180\">\r\n";
    webPage += "<br>\r\n";
    if(grados2 == 0){
      webPage += "<h1>Servo Motor 2: 0 grados</h1>\r\n";
    }else if(grados2 == 90){
      webPage += "<h1>Servo Motor 2: 90 grados</h1>\r\n";
    }else if(grados2 == 180){
      webPage += "<h1>Servo Motor 2: 180 grados</h1>\r\n";
    }else{
      webPage += "<h1>Servo Motor 2:" + (String)grados2 + "grados</h1>\r\n";
    }
    webPage += "<input type=\"submit\" name=\"servo2\" value=\"0\">\r\n";
    webPage += "<input type=\"submit\" name=\"servo2\" value=\"90\">\r\n";
    webPage += "<input type=\"submit\" name=\"servo2\" value=\"180\">\r\n";
    webPage += "</form>\r\n";
    webPage += "</body>\r\n";
    webPage += "</html>\r\n";
    
    server.send(200, "text/html", webPage);
}
