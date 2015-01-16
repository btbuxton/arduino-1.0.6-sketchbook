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

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  Tft.init();
}

void loop() {
  byte r;
  byte g;
  byte b;

  static float x = MAX_X / 2;
  static float y = MAX_Y / 2;
  static float phi = 1.61803;
  static bool use_black = false;
  float prevX = x;
  float prevY = y;
  

  //int c = rgb(random(128,255),random(128,255),random(128,255));
  static unsigned int c_now = 1;
  unsigned int c = c_now;
  c_now+=9;
  if (c_now > 65535) {
    c_now = 1;
  }
  if (use_black) {
    //c = BLACK;
  } 
  use_black = !use_black;
  //Serial.println(c);
  for (float r = 0; r < 2*PI; r += PI / 180) {
    Tft.drawLine(prevX, prevY, x, y, c);
    prevX = x;
    prevY = y;
    x += cos(r) + phi / 4;
    y += sin(r) + phi / 4;
    while (x < 0) {
      x+= MAX_X;
      prevX = x;
    }
    while (y < 0) {
      y+= MAX_Y;
      prevY = y;
    }
    while (x > MAX_X) { 
      x -= MAX_X; 
      prevX = x;
    }
    while (y > MAX_Y) { 
      y -= MAX_Y; 
      prevY = y;
    }
  }
  //Tft.fillRectangle(MIN_X,MIN_Y,(MAX_X-MIN_X),(MAX_Y-MIN_Y),c);
}

