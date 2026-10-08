#include "display.h"

#include <LovyanGFX.hpp>
#include "board_config.h"

// ---- LovyanGFX setup for the GC9A01 round panel -------------------------
class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_GC9A01 _panel;
  lgfx::Bus_SPI _bus;
  lgfx::Light_PWM _light;

public:
  LGFX() {
    {
      auto cfg = _bus.config();
      cfg.spi_host = SPI2_HOST;
      cfg.spi_mode = 0;
      cfg.freq_write = 40000000;
      cfg.freq_read = 16000000;
      cfg.spi_3wire = true;
      cfg.use_lock = true;
      cfg.dma_channel = SPI_DMA_CH_AUTO;
      cfg.pin_sclk = PIN_LCD_SCLK;
      cfg.pin_mosi = PIN_LCD_MOSI;
      cfg.pin_miso = -1;
      cfg.pin_dc = PIN_LCD_DC;
      _bus.config(cfg);
      _panel.setBus(&_bus);
    }
    {
      auto cfg = _panel.config();
      cfg.pin_cs = PIN_LCD_CS;
      cfg.pin_rst = PIN_LCD_RST;
      cfg.pin_busy = -1;
      cfg.panel_width = LCD_WIDTH;
      cfg.panel_height = LCD_HEIGHT;
      cfg.offset_x = 0;
      cfg.offset_y = 0;
      cfg.readable = false;
      cfg.invert = LCD_INVERT;
      cfg.rgb_order = false;
      cfg.dlen_16bit = false;
      cfg.bus_shared = false;
      _panel.config(cfg);
    }
    {
      auto cfg = _light.config();
      cfg.pin_bl = PIN_LCD_BL;
      cfg.invert = false;
      cfg.freq = 44100;
      cfg.pwm_channel = 7;
      _light.config(cfg);
      _panel.setLight(&_light);
    }
    setPanel(&_panel);
  }
};

static LGFX tft;

// ---- Poster palette (taken from the web poster's CSS) -------------------
static const uint32_t C_PAPER = 0xF9F6F2;  // poster background
static const uint32_t C_INK   = 0x1A1614;  // title / text
static const uint32_t C_FRAME = 0xE8E2DB;  // illustration frame
static const uint32_t C_RED   = 0xC8434E;  // campari red band
static const uint32_t C_CREAM = 0xF4EBE3;  // circle

void displayInit() {
  tft.init();
  tft.setBrightness(200);
  tft.setRotation(0);
  tft.fillScreen(C_PAPER);

  const int W = LCD_WIDTH;
  const int cx = W / 2;

  // Thin ink ring just inside the bezel, like a poster border
  tft.drawCircle(cx, LCD_HEIGHT / 2, W / 2 - 4, C_INK);

  // Title
  tft.setTextDatum(lgfx::middle_center);
  tft.setTextColor(C_INK, C_PAPER);
  tft.setFont(&fonts::FreeSerifBold18pt7b);
  tft.drawString("Negroni", cx, 34);

  // Illustration frame, centred on the screen both ways
  const int fs = 120;                       // frame size
  const int fx = cx - fs / 2;
  const int fy = LCD_HEIGHT / 2 - fs / 2;
  const int pad = fs * 30 / 380 + 2;        // inner padding
  const int ix = fx + pad, iy = fy + pad, is = fs - 2 * pad;
  tft.fillRect(fx, fy, fs, fs, C_FRAME);

  // Red band: full width, bottom 45%
  const int bandH = is * 45 / 100;
  tft.fillRect(ix, iy + is - bandH, is, bandH, C_RED);

  // Cream circle: 68.75% of the frame, bottom-aligned, horizontally centred
  const int d = is * 6875 / 10000;
  const int r = d / 2;
  const int ccx = cx;
  const int ccy = iy + is - r;
  tft.fillCircle(ccx, ccy, r, C_CREAM);
}
