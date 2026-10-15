#pragma once
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <Fonts/FreeSerifBold24pt7b.h>
#include <Fonts/FreeSansBold24pt7b.h>
#include <Fonts/FreeSerif12pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSerif18pt7b.h>
#include "Config.h"

class AlarmDisplay {
public:
  explicit AlarmDisplay(Adafruit_ST7789& tft) : _tft(tft) {}

  void begin() { _tft.init(240, 280, SPI_MODE3); 
                _tft.setSPISpeed(60000000);}

  void setInverted(bool on) { _tft.invertDisplay(on); }

  void showIdle() {
    _tft.fillScreen(ST77XX_WHITE);
    _tft.setTextSize(1);
    _tft.setFont(&FreeSerifBold24pt7b);
    _tft.setTextColor(COLOR_GRAY);
    printCentered("No Alarm", 115);
    printCentered("Detected", 175);
  }

  void sleepIn() { _tft.sendCommand(0x10); }  // sleep in
  void sleepOut() { _tft.sendCommand(0x11); }  // sleep out

  void showAlarm(const char* roomName) {
    _tft.fillScreen(ST77XX_WHITE);
    _tft.fillCircle(_tft.width() / 2, 2 * _tft.height() / 3, 65, ST77XX_RED);
    _tft.setTextSize(1);

    _tft.setFont(&FreeSansBold18pt7b);
    _tft.setTextColor(ST77XX_RED);
    printCentered("FIRE ALARM", 60);
    //printCentered("ALARM", 250);

    _tft.setFont(&FreeSerif18pt7b);
    _tft.setTextColor(ST77XX_BLACK);
    printCentered(("in " + String(roomName)).c_str(), 100);   // inside the circle

    _tft.setFont(&FreeSerifBold24pt7b);
    _tft.setTextColor(ST77XX_WHITE);
    _tft.setTextSize(2);
    printCentered("!", 2 * _tft.height() / 3 + 30);   // inside the circle
    
  }

  void showCountdownFrame() {
    _tft.fillRect(0, 0, _tft.width(), _tft.height() * 0.4, ST77XX_WHITE);
    _tft.setTextSize(1);
    _tft.setFont(&FreeSans18pt7b);
    _tft.setTextColor(ST77XX_BLACK);
    printCentered("Dismissing", 60);
    printCentered("in...", 100);

    _tft.fillCircle(_tft.width() / 2, 2 * _tft.height() / 3, 65, ST77XX_RED);
  }

  void showCountdownDigit(int n) {
    uint16_t circle;

    switch (n) {
      case 0:
      case 1: circle = 0x4800; break;
      case 2: circle = 0x6800; break;
      case 3: circle = 0x9000; break;
      case 4: circle = 0xc800; break;
      case 5: circle = ST77XX_RED; break;
      default: circle = ST77XX_RED; break;
    }

    _tft.fillCircle(_tft.width() / 2, 2 * _tft.height() / 3, 65, circle);
    _tft.setFont(&FreeSerif18pt7b);
    _tft.setTextColor(COLOR_WHITE);
    _tft.setTextSize(3  );
    char buf[8];
    snprintf(buf, sizeof(buf), "%d", n);
    printCentered(buf, 220);
    _tft.setTextSize(1);
  }

  void animationOut(){
    // for (int i = 0; i < 330; i++) {
      // _tft.drawCircle(_tft.width() / 2, 2 * _tft.height() / 3, i, 0x4800);
    // }
    _tft.fillCircle(_tft.width() / 2, 2 * _tft.height() / 3, 330, 0x4800);
  }

private:
  Adafruit_ST7789& _tft;

  void printCentered(const char* text, int baselineY) {
    int16_t x1, y1;
    uint16_t w, h;
    _tft.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    _tft.setCursor((_tft.width() - w) / 2 - x1, baselineY);
    _tft.print(text);
  }
};