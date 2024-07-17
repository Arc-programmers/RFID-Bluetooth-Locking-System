#include <SPI.h>
#include <MFRC522.h>
#include <BluetoothSerial.h>
#include "esp_task_wdt.h" // Include ESP32 Watchdog Library

#define SS_PIN 5  
#define RST_PIN 3 
#define SCK_PIN 15 
#define MOSI_PIN 19 
#define MISO_PIN 18

#define BUZZER_PIN 23  
#define RELAY_PIN 4 
#define LED_PIN 22    

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance
BluetoothSerial SerialBT;

// Array to store allowed card UIDs
String allowedCards[] = {}; // Populate with allowed card UIDs
int numAllowedCards = sizeof(allowedCards) / sizeof(allowedCards[0]);

void setup() {
  Serial.begin(9600); // Initialize serial communication
  SerialBT.begin("ARC_LOCK"); // Initialize Bluetooth serial
  
  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN); // Initialize SPI bus with specified pins
  mfrc522.PCD_Init(); // Initialize MFRC522

  pinMode(BUZZER_PIN, OUTPUT); // Set buzzer pin as output
  pinMode(RELAY_PIN, OUTPUT);  // Set relay pin as output
  pinMode(LED_PIN, OUTPUT);    // Set LED pin as output
  
  Serial.println("RFID Ready");

  // Configure watchdog timer
  esp_task_wdt_init(10, true); // Set timeout to 10 seconds
  esp_task_wdt_add(NULL); // Add current task to watchdog
}

void loop() {
  // Reset watchdog timer
  esp_task_wdt_reset();

  // Turn on LED when Bluetooth is connected, turn it off when disconnected
  if (SerialBT.hasClient()) {
    digitalWrite(LED_PIN, HIGH); // Turn on LED
  } else {
    digitalWrite(LED_PIN, LOW); // Turn off LED
  }

  // Look for new cards
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.println("RFID card detected!");

    // Get the UID of the card
    String cardUID = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      cardUID += (mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
      cardUID += String(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.print("Card UID: ");
    Serial.println(cardUID);

    // Check if the card is allowed
    bool accessGranted = false;
    for (int i = 0; i < numAllowedCards; i++) {
      if (cardUID == allowedCards[i]) {
        accessGranted = true;
        break;
      }
    }

    if (accessGranted) {
      // Access granted
      digitalWrite(RELAY_PIN, HIGH); // Turn on relay
      digitalWrite(LED_PIN, HIGH);   // Turn on LED
      for (int i = 0; i < 2; i++) {
        digitalWrite(BUZZER_PIN, HIGH); // Turn on buzzer
        delay(100); // Beep duration
        digitalWrite(BUZZER_PIN, LOW);  // Turn off buzzer
        delay(100); // Time between beeps
      }
      delay(3000); // Access granted duration
      digitalWrite(RELAY_PIN, LOW); // Turn off relay
      digitalWrite(LED_PIN, LOW);   // Turn off LED
    } else {
      // Access denied
      digitalWrite(RELAY_PIN, LOW); // Ensure relay is off
      digitalWrite(LED_PIN, LOW);   // Ensure LED is off
      digitalWrite(BUZZER_PIN, HIGH); // Turn on buzzer
      delay(1000); // Beep duration
      digitalWrite(BUZZER_PIN, LOW);  // Turn off buzzer
      delay(1000); // Time between beeps
    }
  
    // Halt PICC
    mfrc522.PICC_HaltA();
  }

  // Bluetooth control
  if (SerialBT.available()) {
    char command = SerialBT.read();
    if (command == '$') {
      // Access granted via Bluetooth command
      digitalWrite(RELAY_PIN, HIGH); // Turn on relay
      digitalWrite(LED_PIN, HIGH);    // Turn on LED
      for (int i = 0; i < 2; i++) {
        digitalWrite(BUZZER_PIN, HIGH); // Turn on buzzer
        delay(100); // Beep duration
        digitalWrite(BUZZER_PIN, LOW);  // Turn off buzzer
        delay(100); // Time between beeps
      }
      delay(3000); // Access duration
      digitalWrite(RELAY_PIN, LOW);  // Turn off relay
      digitalWrite(LED_PIN, LOW);    // Turn off LED
    }
  }
}
