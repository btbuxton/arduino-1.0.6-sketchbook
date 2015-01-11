#include <TFT.h>
#include <TouchScreen.h>

unsigned int CENTER_X = (MAX_X - MIN_X) / 2;
unsigned int CENTER_Y = (MAX_Y - MIN_Y) / 2;
const float CIRCLE = 2 * PI;
const unsigned int MAX_INT = 32767;
const unsigned int MAX_QUARTER = MAX_INT / 4;

struct star {
  unsigned int rad;
  unsigned int speed;
  int x;
  int y;
};

void init_star(struct star *new_star) {
  float degrees = random(0,359);
  float rad = degrees * 3.1415927 / 180;
  new_star->rad = (rad / CIRCLE) * MAX_INT;
  float radius = random(0,min(MAX_X,MAX_Y) / 4);
  float x = CENTER_X; //+ (radius * cos(rad));
  float y = CENTER_Y; //+ (radius * sin(rad));
  new_star->x = (x / MAX_X) * MAX_INT;
  new_star->y = (y / MAX_Y) * MAX_INT;
  new_star->speed = ((float)random(10,30) / 10) * MAX_QUARTER;
}

void update_star(struct star *inst) {
  float speed = (float)inst->speed / MAX_QUARTER;
  float rad = (float)inst->rad / MAX_INT * CIRCLE;
  float x = ((float)inst->x / MAX_INT) * MAX_X;
  float y = ((float)inst->y / MAX_INT) * MAX_Y;

  x += cos(rad) * speed;
  y += sin(rad) * speed;
  inst->x = (x / MAX_X) * MAX_INT;
  inst->y = (y / MAX_Y) * MAX_INT;
}

bool out_of_bounds(struct star *inst) {
  float x = ((float)inst->x / MAX_INT) * MAX_X;
  float y = ((float)inst->y / MAX_INT) * MAX_Y;
  if (x < MIN_X)
    return true;
  if (x > MAX_X)
    return true;
  if (y < MIN_Y)
    return true;
  if (y > MAX_Y)
    return true;
  return false;
}

const int MAX = 128; //192;
struct star ALL[MAX];

#define TS_MINX 140
#define TS_MAXX 900
#define TS_MINY 120
#define TS_MAXY 940
TouchScreen TS = TouchScreen(17, A2, A1, 14, 300);

void setup() {
  //Serial.begin(9600);
  Tft.init();
  randomSeed(analogRead(0));
  for (int i=0; i < MAX; i++) {
    init_star(&ALL[i]);
  }
}

void adjust_center() {
  Point p = TS.getPoint();
  p.x = map(p.x, TS_MINX, TS_MAXX, 240, 0);
  p.y = map(p.y, TS_MINY, TS_MAXY, 320, 0);
  if (p.z > TS.pressureThreshhold) {
    CENTER_X = p.x;
    CENTER_Y = p.y;
  }
}

void loop() {
  for (int i=0; i < MAX; i++) {
    float x = ((float)ALL[i].x / MAX_INT) * MAX_X;
    float y = ((float)ALL[i].y / MAX_INT) * MAX_Y;
    Tft.setPixel(x, y, BLACK);
    update_star(&ALL[i]);
    if (out_of_bounds(&ALL[i])) {
      init_star(&ALL[i]);
    } 
    x = ((float)ALL[i].x / MAX_INT) * MAX_X;
    y = ((float)ALL[i].y / MAX_INT) * MAX_Y;
    unsigned int value = random(32,63);
    unsigned color = 0x0001 | (((0x001F & value) << 11) + ((0x001F & value) << 6) + value);
    Tft.setPixel(x, y, color);

    adjust_center();
  }
}

