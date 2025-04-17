#include <Servo.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Component Pins
#define MQ_PIN A0
#define LED_PIN 8
#define BUZZER_PIN 7
#define SERVO_PIN 9

Servo gasServo;
SoftwareSerial gsm(6, 5); // GSM TX → D6, RX ← D5

int gasThreshold = 35; // Adjust based on calibration
bool alertSent = false;

void setup() {
  Serial.begin(9600);
  gsm.begin(9600);
  delay(3000); // Let GSM module initialize

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(MQ_PIN, INPUT);

  gasServo.attach(SERVO_PIN);
  gasServo.write(0); // Valve open at start

  // OLED initialization
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED init failed"));
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("LPG Detector Starting...");
  display.display();
  delay(2000);
  display.clearDisplay();

  Serial.println("System Ready.");
}

void loop() {
  int gasLevel = analogRead(MQ_PIN);
  Serial.print("Gas Level: ");
  Serial.println(gasLevel);

  // OLED update
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Gas Level: ");
  display.println(gasLevel);

  if (gasLevel > gasThreshold) {
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    gasServo.write(90); // Close valve

    display.println("Status: ALERT!");
    if (!alertSent) {
      display.println("Sending SMS...");
      display.display(); // Show status immediately
      sendSMS(gasLevel); // ✅ Send with gas level
      alertSent = true;
    } else {
      display.println("Alert Sent");
    }
  } else {
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    gasServo.write(0); // Valve open

    display.println("Status: Safe");
    alertSent = false;
  }

  display.display();
  delay(1000); // Loop delay
}

// ✅ Final SMS function with proper formatting and gas level
void sendSMS(int gasValue) {
  Serial.println("Sending SMS...");

  gsm.println("AT+CMGF=1"); // Set SMS to text mode
  delay(1000);

  gsm.print("AT+CMGS=\"+919595598100\"\r"); // Your number
  delay(3000); // Wait for '>' prompt

  gsm.print("LPG LEAK DETECTED! Gas Level: ");
  gsm.print(gasValue); // Append gas level
  gsm.print(". Take action.");
  delay(500);

  gsm.write(26); // Ctrl+Z to send
  delay(5000);   // Wait for GSM to complete

  Serial.println("SMS with gas level sent.");
}
