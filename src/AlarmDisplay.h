#pragma once
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <Fonts/FreeSerifBold24pt7b.h>
#include <Fonts/FreeSansBold24pt7b.h>
#include <Fonts/FreeSerif12pt7b.h>
#include "Config.h"

class AlarmDisplay {
public:
  explicit AlarmDisplay(Adafruit_ST7789& tft) : _tft(tft) {}

  void begin() { _tft.init(240, 280, SPI_MODE3); }

  void setInverted(bool on) { _tft.invertDisplay(on); }

  void showIdle() {
    _tft.fillScreen(ST77XX_BLACK);
    _tft.setTextSize(1);
    _tft.setFont(&FreeSerifBold24pt7b);
    _tft.setTextColor(COLOR_GRAY);
    printCentered("No Alarm", 115);
    printCentered("Detected", 175);
  }

  void showAlarm(const char* roomName) {
    _tft.fillScreen(ST77XX_WHITE);
    _tft.fillCircle(_tft.width() / 2, _tft.height() / 2, 50, ST77XX_BLACK);
    _tft.setTextSize(1);

    _tft.setFont(&FreeSansBold24pt7b);
    _tft.setTextColor(ST77XX_RED);
    printCentered("FIRE", 65);
    printCentered("ALARM", 250);

    _tft.setFont(&FreeSerif12pt7b);
    _tft.setTextColor(ST77XX_WHITE);
    printCentered(roomName, _tft.height() / 2 + 6);   // inside the circle
  }

  void showCountdownFrame() {
    _tft.fillScreen(ST77XX_BLACK);
    _tft.setTextSize(1);

    _tft.setFont(&FreeSerifBold24pt7b);
    _tft.setTextColor(ST77XX_RED);
    printCentered("Fire Alarm", 60);

    _tft.setFont(&FreeSerif12pt7b);
    _tft.setTextColor(COLOR_GRAY);
    printCentered("Dismissed in:", 100);
    printCentered("seconds", 260);
  }

  void showCountdownDigit(int n) {
    _tft.fillRect(0, 110, _tft.width(), 115, ST77XX_BLACK);  // erase old digit
    _tft.setFont(&FreeSerif12pt7b);
    _tft.setTextColor(COLOR_GRAY);
    _tft.setTextSize(6);
    char buf[8];
    snprintf(buf, sizeof(buf), "%d", n);
    printCentered(buf, 220);
    _tft.setTextSize(1);
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