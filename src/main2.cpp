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
#include "AlarmDisplay.h"
#include "Rooms.h"

Adafruit_ST7789 tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
Adafruit_DRV2605 drv;
AlarmDisplay alarmDisplay(tft);

volatile bool dismissedOne = false;
volatile bool dismissedTwo = false;

bool dismissedDone = false;
bool alarmDetected = false;
bool idle = true;

const Room* currentRoom = nullptr;

void IRAM_ATTR fireAlarmScreenOne() {
  dismissedOne = true;
}
 
void IRAM_ATTR fireAlarmScreenTwo() {
  dismissedTwo = true;
}

void dismissCountdown() {
    alarmDisplay.showCountdownFrame();
    
    for (int i = COUNTDOWN_TIMES; i >= 0; i--) 
    {
       drv.setWaveform(0,9);
       drv.setWaveform(1,0);
       alarmDisplay.showCountdownDigit(i);
       drv.go();
       delay(COUNTDOWN_DURATION_MS);
    }   
}

void setup() {
  Serial.begin(115200);
 
  attachInterrupt(BUTTON_ONE_PIN, fireAlarmScreenOne, RISING);
  attachInterrupt(BUTTON_TWO_PIN, fireAlarmScreenTwo, RISING);
 
  Wire.begin(SDA_PIN, SCL_PIN);
  alarmDisplay.begin();
 
  if (!drv.begin()) {
    Serial.println("Could not find DRV2605");
    while (1) delay(10);
  }
 
  drv.selectLibrary(1);
  drv.setMode(DRV2605_MODE_INTTRIG);
 
  alarmDisplay.showIdle();
}
 

void loop() {
  if (!dismissedOne && !dismissedTwo && !dismissedDone && !idle) {
    if (alarmDetected) {
      alarmDisplay.showAlarm(currentRoom->name);
      alarmDetected = false;
    }
 
    drv.setWaveform(0, 47);  // play effect
    drv.setWaveform(1, 47);
    drv.setWaveform(2, 47);
    drv.setWaveform(3, 0);
    drv.go();
    alarmDisplay.setInverted(true);
    delay(ALARM_DURATION_MS);
 
    drv.stop();
    alarmDisplay.setInverted(false);
    delay(ALARM_DURATION_MS);
  }
  else if (dismissedOne && dismissedTwo && !dismissedDone) {
    alarmDisplay.setInverted(true);
    dismissCountdown();
    dismissedDone = true;
  }
 
  if (dismissedDone && !idle) {
    // alarmDisplay.setInverted(false); 
    alarmDisplay.showIdle();
    idle = true;
  }
  else {
    if (Serial.available() > 0) {
      const Room* room = findRoom(Serial.read());
      if (room) {
        currentRoom = room;
        dismissedOne = false;
        dismissedTwo = false;
        dismissedDone = false;
        alarmDetected = true;
        idle = false;
      }
    }
  }
}
