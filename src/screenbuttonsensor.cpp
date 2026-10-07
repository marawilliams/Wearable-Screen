
//The 1.69" TFT breakout
    ///----> https://www.adafruit.com/product/5206

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include <Fonts/FreeSerifBold24pt7b.h>
#include <Fonts/FreeSansBold24pt7b.h>
#include <Fonts/FreeSerif12pt7b.h>
#include <Wire.h>
#include <Adafruit_DRV2605.h>
#include <WiFi.h>


const char* ssid = "CRISSYLAPTOP 0085";
const char* password = "44p,02C2";

WiFiServer server(10000);

#define TFT_CS        1
#define TFT_RST        2
#define TFT_DC         3

#define TFT_MOSI 9  // Data out
#define TFT_SCLK 7  // Clock out

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
Adafruit_DRV2605 drv;

bool dismissed = false;
bool dismissedDone = false;


float p = 3.1415926;

void idleScreen(){
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(0x9e9e9e);
  tft.setFont(&FreeSerifBold24pt7b); 
  tft.setTextSize(1);
  tft.setCursor(20, 115);
  tft.print("No Alarm");

  tft.setCursor(30, 175);
  tft.print("Detected");
}

void BlinkScreenWhite(){
  tft.fillScreen(ST77XX_WHITE);
  tft.fillCircle(tft.width()/2, tft.height()/2, 50, ST77XX_BLACK);
  tft.setFont(&FreeSansBold24pt7b);
  tft.setTextColor(ST77XX_RED);
  tft.setCursor(65, 65);
  tft.print("FIRE");
  tft.setCursor(40, 250);
  tft.print("ALARM");

}

void dismissCountdown(){
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_RED);
  tft.setFont(&FreeSerifBold24pt7b);

  tft.setCursor(10, 60);
  tft.print("Fire Alarm");

  tft.setCursor(50, 100);
  tft.setFont(&FreeSerif12pt7b);
  tft.setTextColor(0x9e9e9e);
  tft.print("Dismissed in:");

  tft.setCursor(80, 260);
  tft.print("seconds");
  tft.setTextSize(6);
 


  for(int i = 5; i >= 0; i--){
    drv.setWaveform(0, 9);
    drv.setWaveform(1, 0);
    tft.setCursor(80, 220);
    tft.setTextColor(0x9e9e9e);
    tft.print(i);
    drv.go();
    delay(800);
    tft.setCursor(80, 220);
    tft.setTextColor(ST77XX_BLACK);
    tft.print(i);
    delay(200);
  }
  tft.fillScreen(ST77XX_BLACK);
}

void IRAM_ATTR fireAlarmScreen(){
  dismissed = true;
}

void setup(void) {
  
  attachInterrupt(44, fireAlarmScreen, RISING);
  Serial.begin(9600);
  Wire.begin(5, 6);
  tft.init(240, 280, SPI_MODE3); 

  if (! drv.begin()) {
    Serial.println("Could not find DRV2605");
    while (1) delay(10);
  }        

  drv.selectLibrary(1);
  drv.setMode(DRV2605_MODE_INTTRIG);
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  BlinkScreenWhite();
}

bool alarmDetected = true;

void loop() {
  if (!dismissed && !dismissedDone){
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
  else if (dismissed && !dismissedDone) {
    tft.invertDisplay(true);
    dismissCountdown();
    dismissedDone = true;
  }
  
  if (dismissedDone && alarmDetected){
    idleScreen();
    alarmDetected = false;
  }
}


// void loop() {
//   drv.setWaveform(0, 47);  // play effect
//   drv.setWaveform(1, 47);
//   drv.setWaveform(2, 47); 
//   drv.setWaveform(3,0);
//   drv.go();
//   tft.invertDisplay(true);
//   delay(1000);  
//   drv.stop();
//   tft.invertDisplay(false);
//   delay(1000);
// }


// void BlinkScreenBlack(){
//   tft.fillScreen(ST77XX_BLACK);
//   tft.fillCircle(tft.width()/2, tft.height()/2, 50, ST77XX_WHITE);
//   tft.setFont(&FreeSansBold24pt7b);
//   tft.setTextColor(ST77XX_RED);
//   tft.setCursor(65, 65);
//   tft.print("FIRE");
//   tft.setCursor(40, 250);
//   tft.print("ALARM");}
// void BlinkScreen(){
//   BlinkScreenWhite();
// }