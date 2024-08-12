// Code to show a .bmp image in an ILI9488 screen (red, chinese).
// Not all the code is mine, only part of the drawing function.

// Libraries
#include <SPI.h>          // f.k. for Arduino-1.5.2
#include <SD.h>
#include <Arduino_GFX_Library.h> // Library for screen
#include <avr/pgmspace.h>

// SD pins and variables
#define SD_CS 5

// Screen pins variables
#define TFT_CS    10
#define TFT_RESET 9
#define TFT_DC    8
#define TFT_MOSI  11
#define TFT_SCK   13
#define TFT_LED   7
#define TFT_MISO  -1  // not used for TFT
#define GFX_BL TFT_LED // backlight pin

Arduino_DataBus *bus = new Arduino_HWSPI(TFT_DC, TFT_CS);
Arduino_GFX *gfx = new Arduino_ILI9488_18bit(bus, TFT_RESET, 1 /* rotation */, false /* IPS */);

// SD card reader helper functions
uint16_t read16(File f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(File f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}

#define BUFFPIXEL 1
void bmpDraw(char *filename, int x, int y) 
{
  File 	bmpFile;
  int		bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t	bmpDepth;			   // Bit depth (currently must be 24)
  uint32_t bmpImageoffset; 	   // Start of image data in file
  bool goodBmp = false;	   // Set to true on valid header parse
  uint32_t  rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*BUFFPIXEL];  // pixel in buffer (R+G+B per pixel)
  uint16_t lcdbuffer[BUFFPIXEL];  // pixel  out buffer (16-bit per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current  position in sdbuffer
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime  = millis();
  uint8_t  lcdidx = 0;
  boolean  first = true;

  Serial.println();
  Serial.print("Loading image '");
  Serial.print(filename);
  Serial.println('\'');
  // Open requested file on SD card
  if ((bmpFile = SD.open(filename)) == NULL) {
    Serial.print("File not found");
    return;
  }

  // Parse BMP header
  if(read16(bmpFile) == 0x4D42) { // BMP signature
    Serial.print(F("File size: ")); Serial.println(read32(bmpFile));
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    Serial.print(F("Image Offset: ")); Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    Serial.print(F("Header size: ")); Serial.println(read32(bmpFile));
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if(read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      Serial.print(F("Bit Depth: ")); Serial.println(bmpDepth);

      if((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed
        goodBmp = true; // Supported BMP format -- proceed!
        Serial.print(F("Image size: "));
        Serial.print(bmpWidth);
        Serial.print('x');
        Serial.println(bmpHeight);

        rowSize = (bmpWidth * 3 + 3) & ~3;

        w = bmpWidth;
        h = bmpHeight;
        if((x+w-1)  >= gfx->width())  w = gfx->width()  - x;
        if((y+h-1) >= gfx->height()) h  = gfx->height() - y;

        for (row=0; row<h;  row++) { // For each scanline...
          // Seek to start of scan line.  It  might seem labor-
          // intensive to be doing this on every line, but  this
          // method covers a lot of gritty details like cropping
          //  and scanline padding.  Also, the seek only takes
          // place if the file  position actually needs to change
          // (avoids a lot of cluster math  in SD library).
          pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          if(bmpFile.position() != pos) { // Need seek?
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force  buffer reload
          }

          for (col=0; col<w; col++) { // For  each column...
            // Time to read more pixel data?
            if  (buffidx >= sizeof(sdbuffer)) { // Indeed
              // Push LCD buffer to  the display first
              if(lcdidx > 0) {
                gfx->drawPixel(col, row, gfx->color565(r,g,b));
                lcdidx = 0;
                first  = false;
              }
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx  = 0; // Set index to beginning
            }

            // Convert pixel  from BMP to TFT format
            b = sdbuffer[buffidx++];
            g  = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            lcdbuffer[lcdidx++]  = gfx->color565(r,g,b);
          } // end pixel
        } // end scanline
        // Write any remaining data to LCD
        if(lcdidx > 0) {
          gfx->drawPixel(col, row, gfx->color565(r,g,b));
        } 

        Serial.print(F("Loaded in "));
        Serial.print(millis() - startTime);
        Serial.println(" ms");
      } // end goodBmp

    }
  }

  bmpFile.close();
  if(!goodBmp) Serial.println("BMP format not recognized.");
}

void setup()
{
  Serial.begin(9600);
  Serial.print("Show BMP files on TFT with ID:0x");

  // Init Display
  gfx->begin();
  gfx->fillScreen(BLACK);

  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);

  SD.begin(SD_CS);
  bmpDraw("img.bmp", 0, 0);
}

void loop() {}