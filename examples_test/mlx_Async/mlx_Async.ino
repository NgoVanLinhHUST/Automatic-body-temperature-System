#include <WiFi.h>
#include <Wire.h>
#include <SPI.h>
#include "ESPAsyncWebServer.h"
#include <AsyncTCP.h>
#include <Adafruit_MLX90614.h>

AsyncWebServer server(80);
const char *ssid = "Lan Dat";
const char *pass = "88888888";
int contrastValue = 60; /* Default Contrast Value */
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
float t_object=mlx.readObjectTempC();
int count = 0;


const char html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<body>
<p style="color:blue;text-align:center; font-size: 100px">Your Temp: </p>
<span id="presscount" style="color:blue; font-size: 100px"><center>  %t_object%</center><span>
  <script>
            setInterval(function() {
            xhttp1("GET", "/presscount", true);
            xhttp1.send()
        }, 1000);
//        
//      setInterval(function () {
//          var xhttp = new XMLHttpRequest();
//          xhttp.onreadystatechange = function() {
//              if (this.readyState == 4 && this.status == 200) {
//                  document.getElementById("presscount").innerHTML =
//                      this.responseText;
//              }
//          };
//          xhttp.open("GET", "/presscount", true);
//          xhttp.send();
//      },100);
    </script>
</body>
</html>
)rawliteral";


String processor(const String& var)
{
if(var == "t_object")
return String(mlx.readObjectTempC());
}

void setup() {
// put your setup code here, to run once:
  Serial.begin(115200);
  mlx.begin();
  pinMode(0,INPUT);
  WiFi.begin(ssid,pass);
  while(WiFi.status()!=WL_CONNECTED);
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](WebServerRequest *request){
  request->send_P(200, "text/html", html,processor);
  });
  server.on("/presscount", HTTP_GET, [](WebServerRequest *request){
    request->send(200, "text/plain", String(mlx.readObjectTempC()));
  });
  server.begin();
}
void setup(){
  Serial.begin(115200);
  dht.begin();
  
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(t).c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(h).c_str());
  });

  // Start server
  server.begin();
}

void loop() {
  Serial.println(mlx.readObjectTempC());
  delay (3000);
}
