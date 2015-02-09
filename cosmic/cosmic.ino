#include <TFT.h>

/*
 * Using seeedstudio TFT 2.8 version 1
 * 
 */
float DEGREE = 3.1415926 / 180;



int rgb(byte r, byte g, byte b) {
  //5/6/5
  float fr=(float)r / 255;
  float fg=(float)g / 255;
  float fb=(float)b / 255;
  return ((int)(fr * 31) << 11) + ((int)(fg * 63) << 5) + ((int)(fb * 31));
}

void setup() {
  Serial.begin(9600);
  Tft.init();
  Tft.fillRectangle(MIN_X,MIN_Y, MAX_X,MAX_Y,BLACK);
  //randomSeed(analogRead(0));

}

float START = 0;

void loop() {
  float circle = 360 * DEGREE;
  int color = YELLOW;
  int radius = max(MAX_X,MAX_Y);
  for (float deg = START; deg < circle+START; deg+=DEGREE*4) {
    float var = 0;
    float dir = DEGREE / 2;
    float max_dir = DEGREE * 5;
    color = color == YELLOW ? BLUE : YELLOW;
    for (float i=0; i <= radius; i++) {
      var += dir;
      dir = abs(var) >= max_dir ? -dir : dir;
      int x = MAX_X/2 + cos(deg+var)*i;
      int y = MAX_Y/2 + sin(deg+var)*i;
      if (x < MIN_X || y < MIN_Y || x > MAX_X || y > MAX_Y) {
        break;
      }
      Tft.setPixel(x,y,color);
    }
  }
  START += DEGREE;
}
