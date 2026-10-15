

#include <Adafruit_GFX.h>    
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include <Fonts/FreeSerifBold24pt7b.h>
#include <Fonts/FreeSansBold24pt7b.h>
#include <Fonts/FreeSerif12pt7b.h>
#include <Wire.h>
#include <Adafruit_DRV2605.h>
#include <WiFi.h>
#include "Config.h"

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
Adafruit_DRV2605 drv;

bool dismissedOne = false;
bool dismissedTwo = false;
bool dismissedDone = false;

void idleScreen(){
  tft.fillScreen(COLOR_BLACK);
  tft.setTextColor(COLOR_GRAY);
  tft.setFont(&FreeSerifBold24pt7b); 
  tft.setTextSize(1);
  tft.setCursor(20, 115);
  tft.print("No Alarm");

  tft.setCursor(30, 175);
  tft.print("Detected");
}

void BlinkScreenWhite(){
  tft.setTextSize(1);
  tft.fillScreen(COLOR_WHITE);
  tft.fillCircle(tft.width()/2, tft.height()/2, 50, ST77XX_BLACK);
  tft.setFont(&FreeSansBold24pt7b);
  tft.setTextColor(COLOR_RED);
  tft.setCursor(65, 65);
  tft.print("FIRE");
  tft.setCursor(40, 250);
  tft.print("ALARM");

}

void dismissCountdown(){
  tft.setTextSize(1);
  tft.fillScreen(COLOR_BLACK);
  tft.setTextColor(COLOR_RED);
  tft.setFont(&FreeSerifBold24pt7b);

  tft.setCursor(10, 60);
  tft.print("Fire Alarm");

  tft.setCursor(50, 100);
  tft.setFont(&FreeSerif12pt7b);
  tft.setTextColor(COLOR_GRAY);
  tft.print("Dismissed in:");

  tft.setCursor(80, 260);
  tft.print("seconds");
  tft.setTextSize(6);
 


  for(int i = 5; i >= 0; i--){
    drv.setWaveform(0, 9);
    drv.setWaveform(1, 0);
    tft.setCursor(80, 220);
    tft.setTextColor(COLOR_GRAY);
    tft.print(i);
    drv.go();
    delay(800);
    tft.setCursor(80, 220);
    tft.setTextColor(COLOR_BLACK);
    tft.print(i);
    delay(200);
  }
  tft.fillScreen(COLOR_BLACK);
  tft.setTextSize(1);
}

void IRAM_ATTR fireAlarmScreenOne(){
  dismissedOne = true;
}
  
void IRAM_ATTR fireAlarmScreenTwo(){
  dismissedTwo = true;
}

void setup(void) {
  Serial.begin(115200);
  attachInterrupt(44, fireAlarmScreenOne, RISING);
  attachInterrupt(43, fireAlarmScreenTwo, RISING);
  Wire.begin(5, 6);
  tft.init(240, 280, SPI_MODE3); 

  if (! drv.begin()) {
    Serial.println("Could not find DRV2605");
    while (1) delay(10);
  }        

  drv.selectLibrary(1);
  drv.setMode(DRV2605_MODE_INTTRIG);
  tft.sendCommand(0x10); //sleep in
}

bool alarmDetected = false;
bool idle = true;

void loop() {
  if (!dismissedOne && !dismissedTwo && !dismissedDone && !idle){
    if (alarmDetected){
      tft.sendCommand(0x11); //sleep out
      BlinkScreenWhite();
      alarmDetected = false;
      idle = false;
    }
      drv.setWaveform(0, 47);  // play effect
      drv.setWaveform(1, 47);
      drv.setWaveform(2, 47); 
      drv.setWaveform(3,0);
      drv.go();
      tft.invertDisplay(true);
      delay(1000);  
      drv.stop();
      tft.invertDisplay(false);
      delay(1000);
  } 
  else if (dismissedOne && dismissedTwo && !dismissedDone) {
    tft.invertDisplay(true);
    dismissCountdown();
    dismissedDone = true;
  }
  
  if (dismissedDone && !idle){
    idleScreen();
    idle = true;
  }
  else{
    if (Serial.available() > 0) {
    char signal = Serial.read(); 
    if (signal == 'a') {
      dismissedOne = false;
      dismissedTwo = false;
      dismissedDone = false;
      alarmDetected = true;
      idle = false;
    } 
  }
  }

}
