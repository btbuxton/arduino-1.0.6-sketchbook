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
  x = x < 0 ? MAX_X : x;
  x = x > MAX_X ? 0 : x;
  y = y < 0 ? MAX_Y : y;
  y = y > MAX_Y ? 0 : y;
  return this;
}

Person* Person::move() {
  x += cos(dir);
  y += sin(dir);
  check_bounds();
  float rand = random(-1,2);
  dir += rand * 5 * DEGREE;
  return this;
}

Person* Person::paint() {
  Tft.setPixel(x,y, color);
  return this;
}

Person* Person::clear() {
  delay(10);
  //Tft.setPixel(x,y, BLACK);
  return this;
}

unsigned int MAX_PEOPLE = 4;
Person **people;

void setup() {
  Serial.begin(9600);
  Tft.init();
  Tft.fillRectangle(MIN_X,MIN_Y, MAX_X,MAX_Y,BLACK);
  randomSeed(analogRead(0));
  
  people = new Person*[MAX_PEOPLE];
  people[0]=new Person(MAX_X/2,MAX_Y/2, 0, BLUE);
  people[1]=new Person(MAX_X/2,MAX_Y/2, 90 * DEGREE, YELLOW);
  people[2]=new Person(MAX_X/2,MAX_Y/2, 180 * DEGREE, RED);
  people[3]=new Person(MAX_X/2,MAX_Y/2, 270 * DEGREE, GREEN);
}

void loop() {
  for (int i=0; i < MAX_PEOPLE; i++) {
    people[i]->clear()->move()->paint();
  }
  
}

