#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include <Wire.h>
#include <BH1750.h>

const char* ssid = "iPhone";
const char* pass = "ipoparayam";

WiFiClient wifi;
HttpClient client = HttpClient(wifi, "maker.ifttt.com", 80);

String IFTTT_Key = "";

BH1750 lightMeter;
bool sunlightActive = false;
float threshold = 100.0;  // Adjust this lux threshold for your environment

void setup() {
  Serial.begin(9600);
  Wire.begin();
  lightMeter.begin();

  Serial.println("Initializing WiFi...");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi........");
}

void loop() {
  float lux = lightMeter.readLightLevel();
  Serial.print("Lux: ");
  Serial.println(lux);

  if (lux > threshold && !sunlightActive) {
    triggerIFTTT("sunlight_started");
    sunlightActive = true;
  } 
  else if (lux <= threshold && sunlightActive) {
    triggerIFTTT("sunlight_stopped");
    sunlightActive = false;
  }

  delay(10000); // Check every 10 seconds
}

void triggerIFTTT(String event) {
  String requestURL = "/trigger/" + event + "/with/key/" + IFTTT_Key;
  client.get(requestURL);

  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
}


