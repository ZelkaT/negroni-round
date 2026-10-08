#pragma once
// Board: Guition ESP32-2424S012 (ESP32-C3, 1.28" round 240x240 GC9A01, CST816D touch)
// If your board differs, only this file (and the platformio env) should need changes.

#define LCD_WIDTH   240
#define LCD_HEIGHT  240

#define PIN_LCD_SCLK 6
#define PIN_LCD_MOSI 7
#define PIN_LCD_DC   2
#define PIN_LCD_CS   10
#define PIN_LCD_RST  -1
#define PIN_LCD_BL   3   // backlight; driven by PWM

#define LCD_INVERT   true
