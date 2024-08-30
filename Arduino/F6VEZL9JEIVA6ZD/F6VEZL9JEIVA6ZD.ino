// J5 Project. Uses libraries Adafruit_GFX and Adafruit_ST7735. Please read license
// and give proper credit accordingly

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>


#define TFT_CS     10
#define TFT_RST    9  
#define TFT_DC     8

#define BLACK   0x0000
#define RED     0x001F
#define BLUE    0xF800
#define GREEN   0x07E0
#define YELLOW  0x07FF
#define PURPLE  0xF81F
#define CYAN    0xFFE0  
#define WHITE   0xFFFF

// Option 1 (recommended): must use the hardware SPI pins
// (for UNO thats sclk = 13 and sid = 11) and pin 10 must be
// an output. This is much faster - also required if you want
// to use the microSD card (see the image drawing example)
// Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

// Option 2: use any pins but a little slower!
#define TFT_SCLK 13   // set these to be whatever pins you like!
#define TFT_MOSI 11   // set these to be whatever pins you like!
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

int MS = 9;  //count deci-seconds
int S = 59; // count seconds 
int M = 10; // count minutes
int H = 3; // count hours



float p = 3.1415926;

void setup(void) {

  // Use this initializer if you're using TFT
  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
  tft.fillScreen(ST7735_BLACK);

  print_labels();
  draw_buttons();
  delay(1000);
 
}

void loop() {
  
  tft.invertDisplay(false);

  print_ms(MS,BLACK);
  MS--;
  
  if(MS<0){
  print_sec(S,BLACK);
  S--;  
  MS = 9;    
  }
 
  if(S<0){
  print_min(M,BLACK);
  M--;
  S=59;
  }

  if(M<0){
  print_hour(H,BLACK);
  H--;
  M=59;
  }
  
  if(H<0){
  H=23;
  M=59;
  S=59;  
  }

  print_hour(H,RED);
  print_min(M,RED);
  print_sec(S,RED);
  print_ms(MS,RED);
  delay(100);
 
}


void print_labels(){

  tft.fillScreen(ST7735_BLACK);
  tft.setTextSize(1);

  //System label
  tft.setTextColor(YELLOW);
  tft.setCursor(48,35);
  tft.println("SYSTEM");
  tft.setCursor(50,43);
  tft.println("TEST");

  //Warning label
  tft.setTextColor(RED);
  tft.setCursor(30,60);
  tft.println("TIME TO TOTAL");
  tft.setCursor(25,70);
  tft.println("MEMORY FAILURE");
  
  //Time label
  tft.setTextColor(RED);
  tft.setCursor(20,110);
  tft.println("HR.");

  tft.setCursor(50,110);
  tft.println("MIN.");

  tft.setCursor(80,110);
  tft.println("SEC.");

  //ON and OFF labels
  tft.setTextSize(2);
  tft.setTextColor(GREEN);
  tft.setCursor(15,140);
  tft.println("ON");

  tft.setTextColor(RED);
  tft.setCursor(83,140);
  tft.println("OFF");

}


void draw_buttons(){

  //Red button left of System Test label
  tft.fillRect(25, 35, 13, 15, RED);

  //Red and yellow buttons right of System Test label
  tft.fillRect(95, 35, 13, 6, RED);
  tft.fillRect(95, 43, 13, 6, YELLOW); 

  //Rectangle around left red button and system test label
  tft.drawLine(10,32,10,52, RED);
  tft.drawLine(10,52,88,52, RED);
  tft.drawLine(88,52,88,32, RED);
  tft.drawLine(88,32,10,32, RED);

  //Rectangle around counter
  tft.drawLine(10,55,10,125, WHITE);
  tft.drawLine(10,125,122,125, WHITE);
  tft.drawLine(122,125,122,55, WHITE);
  tft.drawLine(122,55,10,55, WHITE);

  //shape around ON label
  tft.drawLine(10,135,10,157, WHITE);
  tft.drawLine(10,157,45,157, WHITE);
  tft.drawLine(45,157,42,145, WHITE);
  tft.drawLine(42,145,45,135, WHITE);
  tft.drawLine(45,135,10,135, WHITE);

  //shape around OFF label
  tft.drawLine(122,135,122,157, WHITE);
  tft.drawLine(122,157,74,157, WHITE);
  tft.drawLine(74,157,77,145, WHITE);
  tft.drawLine(77,145,74,135, WHITE);
  tft.drawLine(74,135,122,135, WHITE);

  //Red button bewteen ON and OFF
  tft.fillCircle(60, 145, 10, RED);
  
}

void print_ms(uint8_t ms, uint16_t color){

  tft.setTextColor(color);
  tft.setTextSize(2);
  //print colon followed by deci-seconds
  tft.setCursor(102,90);
  tft.println(":");
  tft.setCursor(110,90);
  tft.println(ms);
  //delay(10);

}

void print_sec(uint8_t s, uint16_t color){

  tft.setTextColor(color);
  tft.setTextSize(2);
  //print colon followed by seconds
  tft.setCursor(70,90);
  tft.println(":");

  if(s>9){
    tft.setCursor(80,90);
    tft.println(s);
  }
  else{
    tft.setCursor(80,90);
    tft.println("0");
    tft.setCursor(95,90);
    tft.println(s);
  }

}

void print_min(uint8_t m, uint16_t color){

  tft.setTextColor(color);
  tft.setTextSize(2);
  //print colon followed by minutes
  tft.setCursor(40,90);
  tft.println(":");
  
  if(m>9){
    tft.setCursor(50,90);
    tft.println(m);
  }

  else{
    tft.setCursor(50,90);
    tft.println("0");
    tft.setCursor(62,90);
    tft.println(m);
  }
}

void print_hour(uint8_t h, uint16_t color){

  tft.setTextColor(color);
  tft.setTextSize(2);
  //print hour

  if(h > 9){
   tft.setCursor(20,90);
   tft.println(h);
  }
  else{
   tft.setCursor(15,90);
   tft.println("0");
   tft.setCursor(30,90);
   tft.println(h);
  }
}
