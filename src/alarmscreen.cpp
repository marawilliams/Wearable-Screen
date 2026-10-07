#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include <Fonts/FreeSerifBold24pt7b.h>
#include <Fonts/FreeSansBold24pt7b.h>
#include <Fonts/FreeSerif12pt7b.h>
#include <Wire.h>
#include <Adafruit_DRV2605.h>

#define TFT_CS        1
#define TFT_RST        2
#define TFT_DC         3

#define TFT_MOSI 9  // Data out
#define TFT_SCLK 7  // Clock out

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);


void BlinkScreenWhite(String type, String location){
  tft.fillScreen(ST77XX_WHITE);
  tft.fillCircle(tft.width()/2, tft.height()/2, 50, ST77XX_BLACK);
  tft.setFont(&FreeSansBold24pt7b);
  tft.setTextColor(ST77XX_RED);
  tft.setCursor(65, 65);
  tft.print(type);
  tft.setCursor(40, 250);
  tft.print(location);

}

//refacotring code:

//warning screen should notify where the alarm is triggered
//

//color and text preferrably or symbol
// make the screen modular depedning on what room it comes from
// question: how to handle that carrier