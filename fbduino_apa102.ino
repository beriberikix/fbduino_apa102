// needed until fastled can do pinmapping
#define FASTLED_ESP8266_RAW_PIN_ORDER
// use version in master, managed library not up to date
#include "FastLED.h"
#include "patterns.h"

// FastLED configuration
#define NUM_LEDS 108
#define DATA_PIN 5 // D1 on wemos d1 mini
#define CLOCK_PIN 4 // D2 on wemos d1 mini
CRGB leds[NUM_LEDS];

#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
#include "credentials.h"

void setup() {
  Serial.begin(9600);

  // APA102 has not-uncommon reversed RGB
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS);

  // connect to wifi
  WiFi.begin(SSID, PASS);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(PROJECT, SECRET);
}

void loop() {
  if( (bool)Firebase.get("/patio/enabled") ) {
    Serial.println("true");
    blink(500);
    //cylon();
  } else {
    Serial.println("false");
    clear();
  }
}

void blink(uint8_t wait) {
  // Turn the LED on, then pause
  leds[0] = CRGB::Red;
  FastLED.show();
  delay(wait);
  // Now turn the LED off, then pause
  leds[0] = CRGB::Black;
  FastLED.show();
  delay(wait);
}

void clear() {
  FastLED.clear();
  FastLED.show();
}

void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }

void cylon() {
  static uint8_t hue = 0;
  Serial.print("x");
  // First slide the led in one direction
  for(int i = 0; i < NUM_LEDS; i++) {
    // Set the i'th led to red 
    leds[i] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show(); 
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(10);
  }
  Serial.print("x");

  // Now go in the other direction.  
  for(int i = (NUM_LEDS)-1; i >= 0; i--) {
    // Set the i'th led to red 
    leds[i] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show();
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(10);
  }
}
