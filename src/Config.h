#pragma once
#include <Arduino.h>

// display pins
constexpr uint8_t TFT_CS = 1;
constexpr uint8_t TFT_RST = 2;
constexpr uint8_t TFT_DC = 3;
constexpr uint8_t TFT_MOSI = 9;
constexpr uint8_t TFT_SCLK = 7;  

// motor driver pins
constexpr uint8_t SDA_PIN = 5;
constexpr uint8_t SCL_PIN = 6;

// button pins
constexpr uint8_t BUTTON_ONE_PIN = 44;
constexpr uint8_t BUTTON_TWO_PIN = 43;

// timing 
constexpr uint32_t ALARM_DURATION_MS = 1000;
constexpr uint32_t COUNTDOWN_DURATION_MS = 1000;
constexpr uint32_t COUNTDOWN_TIMES = 5;
constexpr uint32_t ESCALATION_INTERVAL_MS = 30000; 
// colors
constexpr uint16_t COLOR_RED = ST77XX_RED;
constexpr uint16_t COLOR_GREEN = ST77XX_GREEN;
constexpr uint16_t COLOR_BLUE = ST77XX_BLUE;
constexpr uint16_t COLOR_WHITE = ST77XX_WHITE;
constexpr uint16_t COLOR_BLACK = ST77XX_BLACK;
constexpr uint16_t COLOR_GRAY = 0x9e9e9e;
