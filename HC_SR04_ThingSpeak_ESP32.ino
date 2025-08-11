#include <WiFi.h>
#include <HTTPClient.h>

// Define your Wi-Fi credentials
const char* ssid = "GITAM";            // Replace with your Wi-Fi SSID
const char* password = "Gitam$$123";    // Replace with your Wi-Fi password

// ThingSpeak API details
const char* server = "https://api.thingspeak.com/update?api_key=LTYPZR0G856C1YK1&field1=0";
String apiKey = "LTYPZR0G856C1YK1";            // Replace with your ThingSpeak write API key

// Define GPIO pins for HC-SR04
const int trigPin = 5;
const int echoPin = 18;

// Variables for distance calculation
long duration;
float distanceCm;

void setup() {
  Serial.begin(115200);

  // Set HC-SR04 pins as output and input
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to Wi-Fi");
}

void loop() {
  // Trigger the sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echo time
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate distance in cm
  distanceCm = (duration * 0.0343) / 2;
  
  Serial.print("Distance: ");
  Serial.print(distanceCm);
  Serial.println(" cm");
  
  // Send data to ThingSpeak
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String url = String(server) + "/update?api_key=" + apiKey + "&field1=" + String(distanceCm);
    http.begin(url);  // Specify the URL
    int httpResponseCode = http.GET();  // Send the request

    if (httpResponseCode > 0) {
      Serial.print("Data sent to ThingSpeak. Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error sending data. HTTP Response code: ");
      Serial.println(httpResponseCode);
    }

    http.end();  // Free resources
  }

  // Wait 20 seconds before the next reading
  delay(20000);
}
