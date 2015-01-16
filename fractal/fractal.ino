#include <TFT.h>

//http://en.wikipedia.org/wiki/HSL_and_HSV#Converting_to_RGB

int rgb(byte r, byte g, byte b) {
  //5/6/5
  float fr=(float)r / 255;
  float fg=(float)g / 255;
  float fb=(float)b / 255;
  return ((int)(fr * 31) << 11) + ((int)(fg * 63) << 5) + ((int)(fb * 31));
}

float toRad(short deg) {
  return deg / 180 * PI;
}

void drawCircle(int x, int y, float radius) {
  static float phi = 1.61803 / 2;
  Tft.drawCircle(x,y,radius, BLACK);
  if(radius > 2) {
    radius *= 0.5;
    drawCircle(x + radius, y, radius);
    drawCircle(x - radius, y, radius);
  }
}

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  Tft.init();
  Tft.fillRectangle(MIN_X,MIN_Y,MAX_X - MIN_X, MAX_Y - MIN_Y, WHITE);
}

void loop() {
  drawCircle(MAX_X / 2, MAX_Y / 2, MAX_X / 2);
}


