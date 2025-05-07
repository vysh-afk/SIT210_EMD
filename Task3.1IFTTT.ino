#include <Wire.h>
#include <BH1750.h>
#include <WiFiNINA.h>
#include "secrets.h"  // Include your Wi-Fi credentials and IFTTT API key

// Create an instance of the BH1750 light sensor
BH1750 lightMeter;

// WiFi client
WiFiClient client;
String server = "maker.ifttt.com";  // IFTTT Webhooks server
String url = "/trigger/" + String(IFTTT_EVENT_NAME) + "/with/key/" + String(IFTTT_API_KEY);

// Pin Setup for I2C
const int lightSensorPin = A0;  // Not used for I2C-based sensor, but kept for any potential analog sensor

// Threshold for sunlight detection (adjust based on your environment)
const int sunlightThreshold = 50;  // Threshold for sunlight intensity, adjust as needed

void setup() {
  // Start serial communication
  Serial.begin(9600);

  // Initialize the light sensor (BH1750)
  Wire.begin();
  lightMeter.begin();

  // Connect to Wi-Fi
  WiFi.begin(SSID, WIFI_PASSWORD);  // Your Wi-Fi credentials from secrets.h
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Successfully connected to Wi-Fi
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Read light level from the BH1750 sensor
  uint16_t lightLevel = lightMeter.readLightLevel();
  Serial.print("Light Level: ");
  Serial.println(lightLevel);

  // Check if sunlight is detected (light level greater than the threshold)
  if (lightLevel > sunlightThreshold) {
    Serial.println("Sunlight detected!");
    sendIFTTTTrigger("sunlight_on");  // Trigger IFTTT notification for sunlight
  } else {
    Serial.println("No sunlight detected.");
    sendIFTTTTrigger("sunlight_off");  // Trigger IFTTT notification when sunlight stops
  }

  // Wait 10 seconds before checking again
  delay(10000);
}

// Function to send an IFTTT trigger
void sendIFTTTTrigger(String trigger) {
  // Connect to the IFTTT Webhooks server
  if (client.connect(server.c_str(), 80)) {
    // Send the HTTP GET request to IFTTT with the trigger event
    String fullUrl = url + "&value1=" + trigger;
    client.println("GET " + fullUrl + " HTTP/1.1");
    client.println("Host: " + server);
    client.println("Connection: close");
    client.println();

    // Print the response from IFTTT (optional)
    String response = "";
    while (client.available()) {
      response = client.readString();
      Serial.println(response);
    }

    client.stop(); // Close the connection after sending
  } else {
    Serial.println("Connection failed.");
  }
}

