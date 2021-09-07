#include <WiFi.h>
#include <HTTPClient.h>
void setup() {

  Serial.begin(115200);
  WiFi.begin("Lan Dat", "88888888");
  while (WiFi.status() != WL_CONNECTED);
  Serial.println("Connected to WiFi");

  HTTPClient client;
  client.begin("https://esp32-deviot-default-rtdb.firebaseio.com/.json");
  client.addHeader("Content-Type", "application/json");
  int HTTPCode =  client.GET();
  if (HTTPCode > 0) {
    Serial.println(HTTPCode);
    String payload = client.getString();
    Serial.println(payload);
  }
  else {
    Serial.println("Error in WiFi connection");
  }
  client.end();
}

void loop(){
  
}
