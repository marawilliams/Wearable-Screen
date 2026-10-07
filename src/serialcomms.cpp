#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include <Fonts/FreeSerifBold24pt7b.h>
#include <Fonts/FreeSansBold24pt7b.h>
#include <Fonts/FreeSerif12pt7b.h>
#include <Wire.h>

#define USERLED 21

void setup() {
  Serial.begin(115200);
  pinMode(USERLED, OUTPUT); 
  digitalWrite(USERLED, LOW); 
}

void loop() {
  if (Serial.available() > 0) {
    char signal = Serial.read(); 
    if (signal == '1') {
      digitalWrite(USERLED, HIGH); 
      Serial.println("LED is ON");
    } 
    else if (signal == '0') {
      digitalWrite(USERLED, LOW);
      Serial.println("LED is OFF");
    }
  }
}
