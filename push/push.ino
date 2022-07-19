#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

/* based on helloserver
this is in the public domain

This sketch sends data to a website that receives it in an event.
I would call this 'pseudo-push'
See push.html for the client side
*/

const char* ssid = "System";
const char* password = "point123";

ESP8266WebServer server(80);

const int led = 5;
int i;

void handleRoot() {
 
  server.send(200, "text/plain", "hello from esp8266!");
  digitalWrite(led, 0);
  
}

void handleNotFound(){
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 1);
}

void setup(void){
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  

server.on("/zend", [](){
  
  char str[80];
   sprintf(str, "data: Send %d \n\n", i);
  server.sendHeader("Cache-Control", "no-cache");
  server.send(200, "text/event-stream", str);

});

  server.onNotFound(handleNotFound);
  server.enableCORS(true);
  
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
i++;
if (i>600000) i=0;
 
}
