/*******************************************************************************
 * Start of Arduino_GFX setting
 ******************************************************************************/
#include <Arduino_GFX_Library.h>

#define TFT_CS    10
#define TFT_RESET 9
#define TFT_DC    8
#define TFT_MOSI  11
#define TFT_SCK   5
#define TFT_LED   7
#define TFT_MISO  -1  // not used for TFT

#define GFX_BL TFT_LED // backlight pin

/* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
Arduino_DataBus *bus = new Arduino_HWSPI(TFT_DC, TFT_CS);

/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
Arduino_GFX *gfx = new Arduino_ILI9488_18bit(bus, TFT_RESET, 0 /* rotation */, false /* IPS */);

/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

/* more fonts at: https://github.com/moononournation/ArduinoFreeFontFile.git */
#include "FreeMono8pt7b.h"
#include "FreeSansBold10pt7b.h"
#include "FreeSerifBoldItalic12pt7b.h"

void setup(void)
{
  Serial.begin(2700000);
  // Serial.setDebugOutput(true);
  // while(!Serial);
  Serial.println("Arduino_GFX Hello World Gfxfont example");

#ifdef GFX_EXTRA_PRE_INIT
  GFX_EXTRA_PRE_INIT();
#endif

  // Init Display
  #ifdef GFX_BL
    pinMode(GFX_BL, OUTPUT);
    digitalWrite(GFX_BL, HIGH);
  #endif
  if (!gfx->begin())
  {
    Serial.println("gfx->begin() failed!");
  }
  gfx->fillScreen(BLACK);


  gfx->setCursor(10, 10);
  gfx->setFont(&FreeMono8pt7b);
  gfx->setTextColor(BLUE);
  gfx->println("Hello World!");

  delay(5000); // 5 seconds
}

void loop()
{
  gfx->setCursor(random(gfx->width()), random(gfx->height()));
  gfx->setTextColor(random(0xffff));
  uint8_t textSize = random(3);
  switch (textSize)
  {
  case 1:
    gfx->setFont(&FreeMono8pt7b);
    break;
  case 2:
    gfx->setFont(&FreeSansBold10pt7b);
    break;
  default:
    gfx->setFont(&FreeSerifBoldItalic12pt7b);
    break;
  }

  gfx->println("Hello World!");

  delay(1000); // 1 second
}
