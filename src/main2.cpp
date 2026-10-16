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
#include "wearableNet.h"


Adafruit_ST7789 tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
Adafruit_DRV2605 drv;
AlarmDisplay alarmDisplay(tft);

volatile bool ButtonOne = false;
// volatile bool ButtonTwo = false;

//flags
bool isAlarmDetected = false; //if alarm is detected, this flag will be set to true
bool isAlarmActive = false; //if the alarm is active, this flag will be set to true

const Room* currentRoom = nullptr;

void IRAM_ATTR ButtonOneInterrupt() {
    ButtonOne = true;
}

// void IRAM_ATTR fireAlarmScreenTwo() {
//     ButtonTwo = true;
// }

void dismissCountdown() {
    alarmDisplay.showCountdownFrame();
    
    // vibrate the motor slightly for each countdown
    for (int i = COUNTDOWN_TIMES; i >= 0; i--) 
    {
        drv.setWaveform(0,9);
        drv.setWaveform(1,0);
        alarmDisplay.showCountdownDigit(i);
        drv.go();
        delay(COUNTDOWN_DURATION_MS);
    }

    //reset the button flag and active flag after countdown is done
    ButtonOne = false;
    isAlarmActive = false; 
}

void setup() {
    Serial.begin(115200);

    attachInterrupt(BUTTON_ONE_PIN, ButtonOneInterrupt, RISING);
    // attachInterrupt(BUTTON_TWO_PIN, fireAlarmScreenTwo, RISING);
    wearable_net_begin();
    Wire.begin(SDA_PIN, SCL_PIN);
    alarmDisplay.begin();

    if (!drv.begin()) {
    Serial.println("Could not find DRV2605");
    while (1) delay(10);
    }

    drv.selectLibrary(1);
    drv.setMode(DRV2605_MODE_INTTRIG);

    alarmDisplay.sleepIn();  // put the display to sleep initially
}

unsigned long lastToggle = 0;
bool alarmPhaseOn = true;
unsigned long lastEscalation = 0;
bool escalated = false;

void startAlarm(const Room* room) {
    currentRoom = room;
    ButtonOne =  false;
    escalated = false;
    alarmDisplay.sleepOut();  // wake up the display
    alarmDisplay.showAlarm(currentRoom->name);  

    drv.setWaveform(0, 9);
    drv.setWaveform(1, 9);
    drv.setWaveform(2, 9);
    drv.setWaveform(3, 0);

    alarmPhaseOn = true;
    lastToggle = millis();
    lastEscalation = millis();
    alarmDisplay.setInverted(true);
    drv.go();
}

void startescalation() {
    escalated = true;
    drv.setWaveform(0, 16);
    drv.setWaveform(1, 16);
    drv.setWaveform(2, 16);
    drv.setWaveform(3, 0);

    alarmPhaseOn = true;
    lastToggle = millis();
    alarmDisplay.setInverted(true);
    drv.go();
}


void loop() {
    unsigned long now = millis();
    Serial.println(wearable_alert_active());
    isAlarmActive = wearable_alert_active();
    if (isAlarmActive && !ButtonOne) {

        if(!escalated && now - lastEscalation >= ESCALATION_INTERVAL_MS) {
            startescalation();
        }
        unsigned long toggleInterval = escalated ? ALARM_DURATION_MS / 4 : ALARM_DURATION_MS;
        
        if (now - lastToggle >= toggleInterval) {
                alarmPhaseOn = !alarmPhaseOn;
                lastToggle = now;
                if (alarmPhaseOn) {
                    alarmDisplay.setInverted(true);
                    drv.go();
                }
                else{
                    alarmDisplay.setInverted(false);
                    if( !escalated) drv.stop();
                }
            }
    }
    else if (ButtonOne) {
    drv.stop();
    alarmDisplay.setInverted(true);
    dismissCountdown();
    alarmDisplay.animationOut();
    isAlarmDetected = false;
    }

    if (!isAlarmActive) {
    // alarmDisplay.setInverted(false); 

    alarmDisplay.sleepIn();
    wearable_alert_clear();
    }
    
    if (isAlarmActive && !isAlarmDetected) {
         const Room* room = findRoom('b');
        if (room) startAlarm(room);
        isAlarmDetected = true;
        }

    
    }

    //       tft.enableDisplay(isDisplayVisible);

