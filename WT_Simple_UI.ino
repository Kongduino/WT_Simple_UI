#include "TFT_eSPI.h" //include TFT LCD library 
#include "Free_Fonts.h" //include free fonts library 

TFT_eSPI tft; //initialize TFT LCD
uint8_t line = 0;
float r0 = .75, r1 = .5;

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
  Serial.println("done");
  tft.begin(); //start TFT LCD
  tft.setRotation(1); //set screen rotation
  tft.fillScreen(TFT_WHITE); //fill background
  drawSliders(2);
  pinMode(WIO_5S_UP, INPUT); //set switch pin up as input
  pinMode(WIO_5S_DOWN, INPUT); //set switch pin down as input
  pinMode(WIO_5S_LEFT, INPUT); //set switch pin left as input
  pinMode(WIO_5S_RIGHT, INPUT); //set switch pin right as input
  pinMode(WIO_5S_PRESS, INPUT); //set switch pin press as input
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
    tft.setFreeFont(&FreeSansBold12pt7b); //set font type
    tft.setTextColor(TFT_BLACK); //set text color
    tft.drawString("Brightness", 90, 10); //draw text string
    tft.fillRect(75, 45, 160, 20, TFT_WHITE); //draw rectangle with border
    tft.drawRect(75, 45, 160, 20, TFT_NAVY); //draw rectangle with border
    tft.fillRect(75, 45, 160 * r0, 20, TFT_NAVY); //fill rectangle with color
  }
  if (which == 1 || which == 2) {
    //Drawing for Volume
    tft.setFreeFont(&FreeSerifBoldItalic12pt7b);
    tft.setTextColor(TFT_BLACK);
    tft.drawString("Volume", 110, 90);
    tft.fillRect(75, 120, 160, 20, TFT_WHITE);
    tft.drawRect(75, 120, 160, 20, TFT_CYAN);
    tft.fillRect(75, 120, 160 * r1, 20, TFT_CYAN);
  }
  delay(10);
}
