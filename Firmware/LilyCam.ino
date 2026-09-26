// DIY Camera — Seeed XIAO ESP32-S3 Sense + 1.3" ST7735 TFT
// SW1 (GPIO1) = Sleep/Wake   
// SW2 (GPIO2) = Shutter
// SW3 (GPIO6) = Filter cycle
#include "esp_camera.h"
#include "SD.h"
#include "SPI.h"
#include "Arduino_GFX_Library.h"
#include "driver/rtc_io.h"

//Pin map (XIAO ESP32-S3)
#define BTN_SLEEP   1  
#define BTN_SHUTTER 2 
#define BTN_FILTER  6  

// Display (SPI)
#define TFT_CS    21   
#define TFT_SCLK  43   
#define TFT_MOSI  44  
#define TFT_RST   4 
#define TFT_DC    5   
#define SD_CS     21    

// Camera pins (Sense board)
#define PWDN -1
#define RESET -1
#define XCLK  10
#define SIOD  40
#define SIOC  39
#define Y9    48
#define Y8    11
#define Y7    12
#define Y6    14
#define Y5    16
#define Y4    18
#define Y3    17
#define Y2    15
#define VSYNC 38
#define HREF  47
#define PCLK  13

// Display
Arduino_GFX *gfx = new Arduino_ST7735(
    TFT_CS /*or -1 if CS tied to GND*/, TFT_DC, TFT_RST,
    SPI_MODE2, 160, 128, TFT_SCLK, TFT_MOSI);

// State
enum Filter { NORMAL, GRAYSCALE, NEGATIVE, SEPIA, NIGHTGREEN };
const char* filterNames[] = {"NORMAL","GRAY","NEGATIVE","SEPIA","NIGHT"};
int filter = NORMAL;
bool sleeping = false;
uint16_t frameBuf[160 * 128];

void startCamera() {
  camera_config_t cfg;
  cfg.ledc_channel = LEDC_CHANNEL_0;
  cfg.ledc_timer   = LEDC_TIMER_0;
  cfg.pin_d0 = Y2; cfg.pin_d1 = Y3; cfg.pin_d2 = Y4; cfg.pin_d3 = Y5;
  cfg.pin_d4 = Y6; cfg.pin_d5 = Y7; cfg.pin_d6 = Y8; cfg.pin_d7 = Y9;
  cfg.pin_xclk = XCLK; cfg.pin_pclk = PCLK;
  cfg.pin_vsync = VSYNC; cfg.pin_href = HREF;
  cfg.pin_sscb_sda = SIOD; cfg.pin_sscb_scl = SIOC;
  cfg.pin_pwdn = PWDN; cfg.pin_reset = RESET;
  cfg.xclk_freq_hz = 20000000;
  cfg.pixel_format = PIXFORMAT_RGB565;  
  cfg.frame_size   = FRAMESIZE_QQVGA;    
  cfg.fb_count = 1;
  cfg.grab_mode = CAMERA_GRAB_LATEST;
  esp_camera_init(&cfg);
}

void applyFilter(uint16_t &px) {
  int r = (px >> 11) & 0x1F, g = (px >> 5) & 0x3F, b = px & 0x1F;
  switch (filter) {
    case GRAYSCALE: {
      uint8_t y = (r*8 + g*10 + b*2) / 4;
      r = g = b = min(31, y >> 1); g = min(63, y); break;
    }
    case NEGATIVE: r = 31 - r; g = 63 - g; b = 31 - b; break;
    case SEPIA: {
      int rr = min(31,(r*13 + g*6)>>4), gg = min(63,(r*5 + g*10 + b*1)>>4);
      r = rr; g = gg; b = min(31, gg >> 2); break;
    }
    case NIGHTGREEN: g = 63 - g; r = 0; b = 0; break;
    default: return;
  }
  px = (r << 11) | (g << 5) | b;
}

void captureAndShow() {
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) return;
  uint16_t *src = (uint16_t*)fb->buf;
  for (int y = 0; y < 120; y++)
    for (int x = 0; x < 160; x++) {
      uint16_t px = src[y * 160 + x];
      px = (px >> 8) | (px << 8);
      applyFilter(px);
      frameBuf[(y + 4) * 160 + x] = px;
    }
  esp_camera_fb_return(fb);
  gfx->draw16bitRGBBitmap(0, 0, frameBuf, 160, 128);
  // HUD
  gfx->setTextColor(RGB565_WHITE);
  gfx->setCursor(2, 2); gfx->print(filterNames[filter]);
}

bool saveToSD() {
  SD.begin(SD_CS);
  File f = SD.open("/shot.raw", FILE_WRITE);
  if (!f) return false;
  f.write((uint8_t*)frameBuf, sizeof(frameBuf));
  f.close();
  return true;
}

void goToSleep() {
  gfx->fillScreen(BLACK);
  esp_camera_deinit();
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_1, 0); // SW1 press (LOW) wakes
  esp_deep_sleep_start();
}

void setup() {
  pinMode(BTN_SLEEP, INPUT_PULLUP);
  pinMode(BTN_SHUTTER, INPUT_PULLUP);
  pinMode(BTN_FILTER, INPUT_PULLUP);

  esp_sleep_enable_ext0_wakeup(GPIO_NUM_1, 0);
  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0)
    filter = NORMAL; // woke up

  gfx->begin();
  gfx->fillScreen(BLACK);
  gfx->setTextColor(RGB565_WHITE);
  gfx->setCursor(20, 56); gfx->print("DIY CAM booting...");
  startCamera();
  captureAndShow();
}

void loop() {
  static uint32_t lastBtn = 0;
  if (millis() - lastBtn < 250) return;

  if (digitalRead(BTN_SLEEP) == LOW) {
    lastBtn = millis();
    goToSleep();
  }
  if (digitalRead(BTN_SHUTTER) == LOW) {
    lastBtn = millis();
    captureAndShow();
    if (saveToSD()) {
      gfx->fillRect(2, 118, 80, 10, BLACK);
      gfx->setCursor(2, 118); gfx->print("SAVED!");
    }
  }
  if (digitalRead(BTN_FILTER) == LOW) {
    lastBtn = millis();
    filter = (filter + 1) % 5;
    captureAndShow(); // re-show last view with new filter
  }
}
