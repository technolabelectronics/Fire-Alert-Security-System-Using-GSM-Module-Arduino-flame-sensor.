#include <SoftwareSerial.h>

SoftwareSerial sim900(9, 10); // RX, TX pins for connecting to the SIM900A module
int flameSensorPin = 12; // Flame sensor connected to digital pin 2
int buzzerPin = 8; // Buzzer connected to digital pin 3
const char* phoneNumber = "+91XXXXXXXXXXX"; // Enter the phone number you want to alert

void setup() {
  pinMode(flameSensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  sim900.begin(9600); // Start communication with the GSM module
  Serial.begin(9600); // Start serial communication for debugging
  delay(1000); // Allow time for GSM module to initialize
  // Turn off echo
    sim900.println("ATE0");
    delay(1000);
  sim900.print("AT+CMGF=1\r"); // Set GSM to SMS mode
  delay(100);
}

void loop() {
  if (digitalRead(flameSensorPin) == LOW) { // Check if the flame sensor detects fire
    Serial.println("Fire detected!");
    Serial.println(flameSensorPin);
    digitalWrite(buzzerPin, HIGH); // Turn on the buzzer
    delay(1000); // Buzzer on for 1 second
    digitalWrite(buzzerPin, LOW); // Turn off the buzzer
    sendSMS(); // Send alert SMS
    delay(3000);
    makeMissedCall(); // Make a missed call
    while(digitalRead(flameSensorPin) == LOW); // Wait here as long as the sensor detects fire
  }
}

void sendSMS() {
  Serial.println("Sending SMS...");
  sim900.println("AT+CMGF=1"); // Set GSM to SMS mode
  delay(1000);
  sim900.println("AT+CMGS=\"" + String(phoneNumber) + "\"");
  delay(1000);
  sim900.println("Fire Alert! Please check immediately."); // The SMS text
  delay(1000);
  sim900.println((char)26); // ASCII code of CTRL+Z to send the SMS
  delay(100);
  Serial.println("SMS sent.");
}

void makeMissedCall() {
  Serial.println("Making a missed call...");
  sim900.println("ATD" + String(phoneNumber) + ";");
  delay(20000); // Wait for 20 seconds before disconnecting the call
  sim900.println("ATH"); // Hang up the call
  Serial.println("Missed call made.");
}
