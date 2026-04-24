
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include "HX711.h"

// Blynk credentials
char auth[] = "YOUR_BLYNK_AUTH_TOKEN";
char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";

// HX711 pins
#define DOUT  4
#define CLK   5

HX711 scale;

float calibration_factor = -7050; // Adjust this value
float weight;
float maxWeight = 2000; // grams (set your bin limit)

bool alertSent = false;

void setup() {
  Serial.begin(9600);

  scale.begin(DOUT, CLK);
  scale.set_scale(calibration_factor);
  scale.tare(); // Reset scale to 0

  Blynk.begin(auth, ssid, pass);
}

void loop() {
  Blynk.run();

  weight = scale.get_units(5);

  Serial.print("Weight: ");
  Serial.print(weight);
  Serial.println(" g");

  // Send weight to Blynk (Virtual Pin V0)
  Blynk.virtualWrite(V0, weight);

  // Check if bin is full
  if (weight >= maxWeight) {
    Serial.println("Bin Full!");

    if (!alertSent) {
      Blynk.logEvent("bin_full", "🗑️ Garbage Bin is FULL!");
      alertSent = true;
    }
  } else {
    alertSent = false;
  }

  delay(1000);
}
