// Define the LED pin (on Arduino Nano 33 IoT, it is pin 13)
const int ledPin = 13;

// Morse code lookup table for A-Z
const String morseCode[] = { 
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.." }; // A to Z

void setup() {
  // Start Serial communication at 9600 baud
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);

  // Print a prompt asking for the name input
  Serial.println("Enter your name:");
}

void loop() {
  // Check if there's any data available in the Serial Monitor
  if (Serial.available() > 0) {
    // Read the name from Serial Monitor
    String name = Serial.readStringUntil('\n');
    name.trim();  // Remove any leading/trailing whitespace

    // Display the entered name
    Serial.println("Your name is: " + name);

    // Loop through each character of the name
    for (int i = 0; i < name.length(); i++) {
      char c = toupper(name[i]);  // Convert to uppercase

      if (c >= 'A' && c <= 'Z') {
        // Get the index of the character in the alphabet (A=0, B=1, ..., Z=25)
        int index = c - 'A';

        // Print the character to Serial Monitor
        Serial.print("Now blinking: ");
        Serial.println(c);

        // Blink the LED for the Morse code of the current letter
        blinkMorseCode(morseCode[index]);
      }

      // Delay before blinking the next letter
      delay(1000);  // Adjust this delay as needed
    }
  }
}

// Function to blink the LED in Morse code
void blinkMorseCode(String code) {
  for (int i = 0; i < code.length(); i++) {
    if (code[i] == '.') {
      blinkDot();
    } else if (code[i] == '-') {
      blinkDash();
    }

    // Small space between symbols
    delay(250);  // Adjust delay as needed
  }

  // Delay before blinking the next character
  delay(1000);  // Adjust delay as needed for the next cycle
}

// Function to blink the LED for a dot
void blinkDot() {
  digitalWrite(ledPin, HIGH);
  delay(250);  // Duration of a dot
  digitalWrite(ledPin, LOW);
  delay(250);  // Duration of a symbol space
}

// Function to blink the LED for a dash
void blinkDash() {
  digitalWrite(ledPin, HIGH);
  delay(750);  // Duration of a dash
  digitalWrite(ledPin, LOW);
  delay(250);  // Duration of a symbol space
}
