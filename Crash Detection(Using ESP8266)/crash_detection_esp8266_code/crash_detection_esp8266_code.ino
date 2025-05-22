#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <MPU6050.h>

//Wifi Details
const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASSWORD";

//Discord Webhook URL
const char* webhookUrl = "ADD_DISCORD_WEBHOOK_URL";

//ThingSpeak Details
const char* thingspeakApiKey = "ADD_THINGSPEAK_API_KEY";

MPU6050 mpu;

const int buzzerPin = D6;
const int ledPin = D5;
const int buttonPin = D7;

bool crashDetected = false;
unsigned long crashTime = 0;
const unsigned long alertWindow = 30000; // 30 second alert gap

void setup() {
  Serial.begin(115200);
  Wire.begin(D2, D1); // SDA => D2, SCL => D1
  mpu.initialize();

  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected! IP: " + WiFi.localIP().toString());
}

void loop() {
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  // Calculate acceleration magnitude in g's
  float acceleration = sqrt(ax * ax + ay * ay + az * az) / 16384.0;

  // Crash detection
  if (acceleration > 2.5 && !crashDetected) {
    triggerCrashAlert(acceleration);
  }

  // Alert handling
  if (crashDetected) {
    // Check for button press to cancel
    if (digitalRead(buttonPin) == LOW) {
      cancelAlert();
    }
    // Timeout after 30 seconds
    else if (millis() - crashTime > alertWindow) {
      confirmCrash(acceleration);
    }
  }

  delay(100); // Reduced delay for more responsive button
}

void triggerCrashAlert(float acceleration) {
  crashDetected = true;
  crashTime = millis();
  Serial.println("Accident Detected!");
  digitalWrite(buzzerPin, HIGH);
  digitalWrite(ledPin, HIGH);
  
  sendDiscordAlert("⚠️ Potential accident detected! 30s countdown started...");
  sendThingSpeakData(acceleration, 1); // Status 1 for detected
}

void cancelAlert() {
  Serial.println("Alert cancelled by user");
  crashDetected = false;
  digitalWrite(buzzerPin, LOW);
  digitalWrite(ledPin, LOW);
  sendDiscordAlert("✅ Alert cancelled by user");
  sendThingSpeakData(0, 0); // Status 0 for cancelled
  delay(500); // Debounce delay
}

void confirmCrash(float acceleration) {
  Serial.println("Accident confirmed!");
  crashDetected = false;
  digitalWrite(buzzerPin, LOW); // Keep LED on as indicator
  sendDiscordAlert("🚨 ACCIDENT CONFIRMED! Immediate action required!");
  sendThingSpeakData(acceleration, 2); // Status 2 for confirmed
}

void sendDiscordAlert(const char* message) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Cannot send Discord alert - WiFi disconnected");
    return;
  }

  HTTPClient http;
  http.begin(webhookUrl);
  http.addHeader("Content-Type", "application/json");
  
  // Create proper JSON payload
  DynamicJsonDocument doc(256);
  doc["content"] = message;
  doc["username"] = "ESP8266 Crash Detector";
  
  String payload;
  serializeJson(doc, payload);
  
  int httpCode = http.POST(payload);
  
  if (httpCode == HTTP_CODE_OK) {
    Serial.println("Discord alert sent");
  } else {
    Serial.printf("Discord failed: %d\n", httpCode);
  }
  http.end();
}

void sendThingSpeakData(float acceleration, int status) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Cannot send to ThingSpeak - WiFi disconnected");
    return;
  }

  HTTPClient http;
  String url = "http://api.thingspeak.com/update?api_key=" + String(thingspeakApiKey);
  url += "&field1=" + String(acceleration, 2); // Acceleration with 2 decimal places
  url += "&field2=" + String(status);         // Status code
  
  http.begin(url);
  int httpCode = http.GET();
  
  if (httpCode == HTTP_CODE_OK) {
    Serial.println("ThingSpeak updated");
  } else {
    Serial.printf("ThingSpeak failed: %d\n", httpCode);
  }
  http.end();
}