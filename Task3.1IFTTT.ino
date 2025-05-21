#include <Wire.h>
#include <BH1750.h>
#include <WiFiNINA.h>
#include "secrets.h"  // Contains your Wi-Fi credentials and IFTTT keys

BH1750 lightMeter;
WiFiClient client;

String server = "maker.ifttt.com";
String url = "/trigger/" + String(IFTTT_EVENT_NAME) + "/with/key/" + String(IFTTT_API_KEY);

const int sunlightThreshold = 50;  // Adjust as needed
bool sunlightState = false;        // Track previous state (false = no sunlight, true = sunlight detected)

void setup() {
  Serial.begin(9600);
  
  Wire.begin();
  if (lightMeter.begin()) {
    Serial.println("BH1750 initialized successfully");
  } else {
    Serial.println("Error initializing BH1750 — check wiring or address");
  }

  WiFi.begin(SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  uint16_t lightLevel = lightMeter.readLightLevel();
  Serial.print("Light Level: ");
  Serial.println(lightLevel);

  // Check if sunlight threshold is crossed
  if (lightLevel > sunlightThreshold && !sunlightState) {
    Serial.println("Sunlight just detected! Sending email...");
    sendIFTTTTrigger("Sunlight Detected");
    sunlightState = true;
  } else if (lightLevel <= sunlightThreshold && sunlightState) {
    Serial.println("Sunlight just disappeared! Sending email...");
    sendIFTTTTrigger("Sunlight Stopped");
    sunlightState = false;
  }

  delay(10000);  // Check every 10 seconds
}

void sendIFTTTTrigger(String trigger) {
  if (client.connect(server.c_str(), 80)) {
    String fullUrl = url + "?value1=" + trigger;
    client.println("GET " + fullUrl + " HTTP/1.1");
    client.println("Host: " + server);
    client.println("Connection: close");
    client.println();

    // Optional: Read and print response from IFTTT server
    while (client.available()) {
      String response = client.readString();
      Serial.println(response);
    }

    client.stop();
  } else {
    Serial.println("Failed to connect to IFTTT server.");
  }
}



