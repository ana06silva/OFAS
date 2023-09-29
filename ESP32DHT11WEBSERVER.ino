#include "WiFi.h"
#include "ESPAsyncWebSrv.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <SPIFFS.h>

// Replace with your network credentials
const char* ssid = "Maria";
const char* password = "maria2350";

#define DHTPIN 5     // Digital pin connected to the DHT sensor

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String readDHTTemperature() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  //float t = dht.readTemperature(true);
  // Check if any reads failed and exit early (to try again).
  if (isnan(t)) {    
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(t);
    return String(t);
  }
}

String readDHTHumidity() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(h);
    return String(h);
  }
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  dht.begin();
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Serve files from the "data" folder on the web server
  server.serveStatic("/", SPIFFS, "/data");
  
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/data/index.html", String(), false);
  });

  // Route for temperature data
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    String temperature = readDHTTemperature();
    request->send(200, "text/plain", temperature);
  });

  // Route for humidity data
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    String humidity = readDHTHumidity();
    request->send(200, "text/plain", humidity);
  });

  // Start server
  server.begin();
}
 
void loop(){
  // Your loop code here
}
