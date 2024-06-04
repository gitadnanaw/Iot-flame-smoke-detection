#include <ESP8266WiFi.h>
#include "ThingSpeak.h"

const char* ssid = "Hahaha";  
const char* password = "anakbaik";  
const char* apiKey = "K3W20VJXNIAFYJ4T";  // Replace with your ThingSpeak API key

const int flame = D0; 
const int buzz = D1;
const int smoke = A0;  // Pin A0 for MQ-2
const int smokeThreshold = 180; // For smoke sensitivity

WiFiClient client;  // Define wifi client as client

unsigned long myChannelNumber = 2568318;  // Replace with your ThingSpeak Channel number
const char* myWriteAPIKey = "K3W20VJXNIAFYJ4T";  // Replace with your ThingSpeak Write API key

void setup() {
  pinMode(flame, INPUT);
  pinMode(buzz, OUTPUT);
  pinMode(smoke, INPUT);

  Serial.begin(115200);         
  Serial.println("");
  Serial.print("Connecting To: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);     

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
  int flameStatus = digitalRead(flame);
  int smokeValue = analogRead(smoke);

  Serial.print("Flame Status: ");
  Serial.print(flameStatus);
  Serial.print(" | Smoke Value: ");
  Serial.println(smokeValue);

  if (flameStatus == 0 || smokeValue > smokeThreshold) {           
    digitalWrite(buzz, HIGH);
    if (flameStatus == 0) {
      Serial.println("Flame detected! Buzzer activated.");
    }
    if (smokeValue > smokeThreshold) {
      Serial.println("Smoke detected! Buzzer activated.");
    }
  } else {
    digitalWrite(buzz, LOW);
  }

  // Write data to ThingSpeak
  ThingSpeak.setField(1, flameStatus);
  ThingSpeak.setField(2, smokeValue);
  
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Channel update successful.");
  } else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  delay(500);
}
