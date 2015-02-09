#include <TFT.h>

/*
 * Using seeedstudio TFT 2.8 version 1
 * Playing around with C++ and making algorthimic art
 */
float DEGREE = 3.1415926 / 180;

class Person {
  float x;
  float y;
  unsigned int color;
  float dir;
  float dir_step;
  
  Person* check_bounds();
  
public:
  Person(float x, float y, float dir, unsigned int color);
  Person* move();
  Person* paint();
  Person* clear();
};

Person::Person(float x,float y, float dir, unsigned int color) {
  this->x=x;
  this->y=y;
  this->dir=dir;
  this->color=color;
}

Person* Person::check_bounds() {
  if (x < 0 || x >= MAX_X || y < 0 || y >= MAX_Y) {
    x = MAX_X / 2;
    y = MAX_Y / 2;
  }
  return this;
}

Person* Person::move() {
  x += cos(dir);
  y += sin(dir);
  check_bounds();
  float rand = random(-30,31);
  dir += rand * DEGREE;
  return this;
}

Person* Person::paint() {
  Tft.setPixel(x,y, color);
  return this;
}

Person* Person::clear() {
  //delay(10);
  //Tft.setPixel(x,y, BLACK);
  return this;
}

int rgb(byte r, byte g, byte b) {
  //5/6/5
  float fr=(float)r / 255;
  float fg=(float)g / 255;
  float fb=(float)b / 255;
  return ((int)(fr * 31) << 11) + ((int)(fg * 63) << 5) + ((int)(fb * 31));
}


unsigned int MAX_PEOPLE = 16;
Person **people;

void setup() {
  Serial.begin(9600);
  Tft.init();
  Tft.fillRectangle(MIN_X,MIN_Y, MAX_X,MAX_Y,BLACK);
  randomSeed(analogRead(0));
  
  people = new Person*[MAX_PEOPLE];
  float deg_step = 360 / MAX_PEOPLE;
  float color_step = 256 / MAX_PEOPLE;
  for (int i=0; i < MAX_PEOPLE; i++) {
    int color_content = color_step * i;
    unsigned int color = rgb(0, 0, color_content);
    people[i]=new Person(MAX_X/2,MAX_Y/2, i * deg_step * DEGREE, color);
  }
}

void loop() {
  for (int i=0; i < MAX_PEOPLE; i++) {
    people[i]->clear()->move()->paint();
  }
  
}

