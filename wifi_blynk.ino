#include <WiFi.h>
#include <WiFiClient.h>
#define BLYNK_TEMPLATE_ID "TMPL3XGYqRMBy"
#define BLYNK_TEMPLATE_NAME "Distance MEasure"
#define BLYNK_AUTH_TOKEN "HGwAXJ8VGOw15Tt8ge2vTjncMwYFt2lj"
#include <BlynkSimpleEsp32.h>

#define trig 5
#define echo 18

long duration;
int distance;

char auth[] = "HGwAXJ8VGOw15Tt8ge2vTjncMwYFt2lj";
char ssid[] = "Airtel_Harshit s wifi";  // Replace with your WiFi SSID
char pass[] = "Prame1234";  // Replace with your WiFi password`````` 

BlynkTimer timer;

void setup()
{
  // Initialize serial communication
  Serial.begin(9600);
  Serial.println("Starting ESP32");

  // Pin setup
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  // WiFi and Blynk connection
  Serial.println("Connecting to WiFi and Blynk...");
  Blynk.begin(auth, ssid, pass);

  // Timer for sending sensor data
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
}

void sendSensor()
{
  // Trigger the ultrasonic sensor
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  // Measure the echo pulse duration
  duration = pulseIn(echo, HIGH);
  distance = duration * 0.034 / 2;

  // Output distance to Serial Monitor
  Serial.print("Distance = ");
  Serial.println(distance);

  // If the distance is less than or equal to 5 cm, send a notification
  if (distance <= 5)
  {
    Serial.println("Distance too close! Sending notification...");
    Blynk.logEvent("distance_warning", "Distance too close! Check the sensor.");
  }

  // Write the distance to a Blynk virtual pin
  Blynk.virtualWrite(V0, distance);
}
