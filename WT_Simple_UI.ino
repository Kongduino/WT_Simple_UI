#include <TFT_eSPI.h> //include TFT LCD library 
#include "lcd_backlight.hpp"
#include <cstdint>
#include "Free_Fonts.h" //include free fonts library 

TFT_eSPI tft; //initialize TFT LCD
uint8_t line = 0;
float r0 = .75, r1 = .5;
uint32_t offsetX = 75, offsetY0 = 45, offsetY1 = 120, barWidth = 160, barHeight = 20;
uint8_t maxBrightness = 100;
static LCDBackLight backLight;

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("\n\nSimple UI...");
  delay(500);
  pinMode(WIO_5S_UP, INPUT); //set switch pin up as input
  pinMode(WIO_5S_DOWN, INPUT); //set switch pin down as input
  pinMode(WIO_5S_LEFT, INPUT); //set switch pin left as input
  pinMode(WIO_5S_RIGHT, INPUT); //set switch pin right as input
  pinMode(WIO_5S_PRESS, INPUT); //set switch pin press as input
  tft.begin(); //start TFT LCD
  tft.setRotation(1); //set screen rotation
  tft.fillScreen(tft.color565(200, 200, 200)); //fill background
  drawSliders(2);
  Serial.println("pinMode");
  pinMode(WIO_5S_UP, INPUT); //set switch pin up as input
  pinMode(WIO_5S_DOWN, INPUT); //set switch pin down as input
  pinMode(WIO_5S_LEFT, INPUT); //set switch pin left as input
  pinMode(WIO_5S_RIGHT, INPUT); //set switch pin right as input
  pinMode(WIO_5S_PRESS, INPUT); //set switch pin press as input
  backLight.initialize();
  backLight.setBrightness(100 * r0);
  reDrawFocus();
}

void loop() {
  if (digitalRead(WIO_5S_UP) == LOW) {
    line -= 1;
    if (line == 255) line = 1;
    reDrawFocus();
  } else if (digitalRead(WIO_5S_DOWN) == LOW) {
    line += 1;
    if (line == 2) line = 0;
    reDrawFocus();
  } else if (digitalRead(WIO_5S_RIGHT) == LOW) {
    // minus
    // tft.setRotation(1); so we're upside down
    if (line == 0) r0 -= .01;
    else r1 -= .01;
    if (r0 < 0.0) r0 = 0.0;
    if (r1 < 0.0) r1 = 0.0;
    drawSliders(line);
  } else if (digitalRead(WIO_5S_LEFT) == LOW) {
    // plus
    // tft.setRotation(1); so we're upside down
    if (line == 0) r0 += .01;
    else r1 += .01;
    if (r0 > 1.0) r0 = 1.0;
    if (r1 > 1.0) r1 = 1.0;
    drawSliders(line);
  } else if (digitalRead(WIO_5S_PRESS) == LOW) {
  }
}

void reDrawFocus() {
  uint16_t c0, c1;
  if (line == 0) {
    c0 = TFT_RED;
    c1 = TFT_DARKGREY;
  } else {
    c0 = TFT_DARKGREY;
    c1 = TFT_RED;
  }
  tft.drawRoundRect(71, 41, 168, 28, 4, c0);
  tft.drawRoundRect(70, 40, 170, 30, 4, c0);
  tft.drawRoundRect(71, 116, 168, 28, 4, c1);
  tft.drawRoundRect(70, 115, 170, 30, 4, c1);
  delay(300);
  // debounce
}

void drawSliders(uint8_t which) {
  // 0 --> first slider
  // 1 --> second slider
  // 2 --> both
  if (which == 0 || which == 2) {
    //Drawing for brightness
    backLight.setBrightness(maxBrightness * r0);
    tft.setFreeFont(&FreeSansBold12pt7b); //set font type
    tft.setTextColor(TFT_BLACK); //set text color
    tft.drawString("Brightness", 90, 10); //draw text string
    uint8_t i, j;
    uint16_t lm = 160 * r0;
    tft.startWrite();
    tft.setAddrWindow(offsetX - 1, offsetY0 - 1, barWidth + 2, barHeight + 2);
    tft.pushColor(TFT_BLUE, barWidth + 2);
    // top border
    for (j = 0; j < barHeight; j++) {
      tft.pushColor(TFT_BLUE, lm + 1); // left border + ratio
      tft.pushColor(TFT_WHITE, barWidth - lm); // remainder
      tft.pushColor(TFT_BLUE, 1); // right border
    }
    tft.pushColor(TFT_BLUE, barWidth + 2); // bottom border
    tft.endWrite();
  }
  if (which == 1 || which == 2) {
    //Drawing for Volume
    tft.setFreeFont(&FreeSerifBoldItalic12pt7b);
    tft.setTextColor(TFT_BLACK);
    tft.drawString("Volume", 110, 90);
    uint8_t i, j;
    uint16_t lm = 160 * r1;
    tft.startWrite();
    tft.setAddrWindow(offsetX - 1, offsetY1 - 1, barWidth + 2, barHeight + 2);
    tft.pushColor(TFT_CYAN, barWidth + 2);
    for (j = 0; j < barHeight; j++) {
      tft.pushColor(TFT_CYAN, lm + 1);
      tft.pushColor(TFT_WHITE, barWidth - lm);
      tft.pushColor(TFT_CYAN, 1);
    }
    tft.pushColor(TFT_CYAN, barWidth + 2);
    tft.endWrite();
  }
  delay(10);
}
