#include <WiFiNINA.h>
#include <DHT.h>
#include <ThingSpeak.h>

// DHT Sensor Setup
#define DHTPIN 2          // DHT11 Data pin connected to D2
#define DHTTYPE DHT11     // DHT11 Sensor Type
DHT dht(DHTPIN, DHTTYPE);

// WiFi Credentials
char ssid[] = "8Uganda";        
char pass[] = "Lmam2022";     

// ThingSpeak Setup
WiFiClient client;
unsigned long myChannelNumber = 2948038; 
const char * myWriteAPIKey = "C7E2PIRVZ07C169D"; 

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, pass);

  // Wait for Wi-Fi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi!");
  
  // Start DHT sensor
  dht.begin();

  // Initialize ThingSpeak
  ThingSpeak.begin(client);
}

void loop() {
  // Read temperature and humidity
  float temp = dht.readTemperature(); // Celsius
  float hum = dht.readHumidity();     // Percentage

  // Check if readings are valid
  if (isnan(temp) || isnan(hum)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print readings to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print(" °C | Humidity: ");
  Serial.print(hum);
  Serial.println(" %");

  // Send data to ThingSpeak
  ThingSpeak.setField(1, temp);
  ThingSpeak.setField(2, hum);

  int response = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (response == 200) {
    Serial.println("Data sent to ThingSpeak!");
  } else {
    Serial.println("Failed to send data. Error code: " + String(response));
  }

  // Update every 60 seconds
  delay(60000);
}
