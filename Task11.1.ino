#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>

// ========== CONFIGURATION ==========

// Wi-Fi credentials
#define WIFI_SSID "Vyshnavi’s iphone "
#define WIFI_PASS "Ponn2004"

// IFTTT Webhooks Key
#define IFTTT_KEY "uFHY0p3i1iuzWdXIcYWJUCHC5BB1tjDHSNZaX8di8k"

// Pin Definitions
#define TRIG_PIN 9
#define ECHO_PIN 8
#define SOUND_PIN 7
#define BUZZER_PIN 6
#define LED_PIN 5
#define SWITCH_PIN 4

// IFTTT Host and Port
const char* ifttt = "maker.ifttt.com";
const int port = 80;
WiFiClient wifi;
HttpClient client = HttpClient(wifi, ifttt, port);

// Status flag
bool alertSent = false;

// ========== SETUP ==========
void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(SOUND_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT);

  connectToWiFi();
}

// ========== MAIN LOOP ==========
void loop() {
  if (digitalRead(SWITCH_PIN) == HIGH) {
    long distance = getDistance();
    bool soundDetected = digitalRead(SOUND_PIN);

    if (distance < 30 && !alertSent) {
      triggerIFTTT("intruder_detected");
      activateAlarm();
      alertSent = true;
    } else if (soundDetected && !alertSent) {
      triggerIFTTT("sound_detected");
      activateAlarm();
      alertSent = true;
    } else if (distance >= 30 && !soundDetected) {
      deactivateAlarm();
      alertSent = false;
    }
  } else {
    deactivateAlarm();
    alertSent = false;
  }

  delay(500);
}

// ========== DISTANCE FUNCTION ==========
long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.034 / 2;
}

// ========== ALARM CONTROL ==========
void activateAlarm() {
  digitalWrite(BUZZER_PIN, HIGH);
  digitalWrite(LED_PIN, HIGH);
}

void deactivateAlarm() {
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
}

// ========== WIFI CONNECTION ==========
void connectToWiFi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(WIFI_SSID);

  while (WiFi.begin(WIFI_SSID, WIFI_PASS) != WL_CONNECTED) {
    delay(2000);
    Serial.print(".");
    int status = WiFi.status();
    Serial.print(" Status: ");
    Serial.println(status);
  }

  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

// ========== IFTTT TRIGGER ==========
void triggerIFTTT(String eventName) {
  String url = "/trigger/" + eventName + "/with/key/" + IFTTT_KEY;
  client.get(url);
  Serial.println("IFTTT Triggered: " + eventName);
}
