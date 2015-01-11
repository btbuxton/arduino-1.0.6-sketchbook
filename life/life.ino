/*
 * Choose Arduino Nano w/ ATmega328 for board
 * Seeedstudio raindowbuino shield
 */

#include <Rainbowduino.h>

#define SIZE 8

#define ON_VALUE 64
#define OFF_VALUE 0

unsigned char RED_ON = OFF_VALUE;
unsigned char GREEN_ON = OFF_VALUE;
unsigned char BLUE_ON = ON_VALUE;

#define RED_OFF OFF_VALUE
#define GREEN_OFF OFF_VALUE
#define BLUE_OFF OFF_VALUE

unsigned char world[2][SIZE][SIZE];
unsigned char steps;

void init_life(unsigned char life[SIZE][SIZE]) {
  for (unsigned char x=0; x<SIZE; x++) {
    for (unsigned char y=0; y<SIZE; y++) {
      life[x][y]=0;
    }
  }
}

unsigned char alive_amount(unsigned char life[SIZE][SIZE]) {
  unsigned char result = 0;
  for (unsigned char x=0; x<SIZE; x++) {
    for (unsigned char y=0; y<SIZE; y++) {
      if (life[x][y]) {
        result++;
      }
    }
  }
  return result;
}
bool all_dead(unsigned char life[SIZE][SIZE]) {
  for (unsigned char x=0; x<SIZE; x++) {
    for (unsigned char y=0; y<SIZE; y++) {
      if (life[x][y]) {
        return false;
      }
    }
  }
  return true;
}

bool all_same(unsigned char life[SIZE][SIZE], unsigned char another[SIZE][SIZE]) {
  for (unsigned char x=0; x<SIZE; x++) {
    for (unsigned char y=0; y<SIZE; y++) {
      if (life[x][y] != another[x][y]) {
        return false;
      }
    }
  }
  return true;
}

void draw_life(unsigned char life[SIZE][SIZE], unsigned char prev[SIZE][SIZE]) {
  float l_r = RED_OFF;
  float l_g = GREEN_OFF;
  float l_b = BLUE_OFF;
  float d_r = RED_ON;
  float d_g = GREEN_ON;
  float d_b = BLUE_ON;
  float each_step = (float)1 / 64;
  
  
  for (unsigned char scale=0; scale<65;scale++) {
    for (unsigned char x=0; x<SIZE; x++) {
      for (unsigned char y=0; y<SIZE; y++) {
        if (prev[x][y] != life[x][y]) {
          if (life[x][y] > 0) {
            Rb.setPixelXY(x,y,l_r,l_g,l_b);
          } else {
            Rb.setPixelXY(x,y,d_r,d_g,d_b);
          }
        }
      }
    }
    l_r += (RED_ON - RED_OFF) * each_step;
    l_g += (GREEN_ON - GREEN_OFF) * each_step;
    l_b += (BLUE_ON - BLUE_OFF) * each_step;
    d_r -= (RED_ON - RED_OFF) * each_step;
    d_g -= (GREEN_ON - GREEN_OFF) * each_step;
    d_b -= (BLUE_ON - BLUE_OFF) * each_step;
    delay(5);
  }
}

unsigned char count_neighbors(unsigned char x, unsigned char y, unsigned char input[SIZE][SIZE]) {
  unsigned char result = 0;
  unsigned char minus_x = x - 1;
  unsigned char minus_y = y - 1;
  if (minus_x == 255) {
    minus_x = SIZE - 1;
  }
  if (minus_y == 255) {
    minus_y = SIZE - 1;
  }
  unsigned char plus_x = x + 1;
  unsigned char plus_y = y + 1;
  if (plus_x == 8) {
    plus_x = 0;
  }
  if (plus_y == 8) {
    plus_y = 0;
  }
  result += input[minus_x][minus_y];
  result += input[minus_x][y];
  result += input[minus_x][plus_y];
  result += input[x][minus_y];
  result += input[x][plus_y];
  result += input[plus_x][minus_y];
  result += input[plus_x][y];
  result += input[plus_x][plus_y];
  return result;
}

void update_life(unsigned char input[SIZE][SIZE], unsigned char output[SIZE][SIZE]) {
  for (unsigned char x=0; x<SIZE; x++) {
    for (unsigned char y=0; y<SIZE; y++) {
      unsigned char neighbors = count_neighbors(x,y,input);
      if (input[x][y]) {
        output[x][y]=neighbors == 2 || neighbors == 3;
      } else {
        output[x][y]=neighbors == 3;
      }
    }
  }
}

void randomize(unsigned char input[SIZE][SIZE]) {
  for (unsigned char x=0; x<SIZE; x++) {
    for (unsigned char y=0; y<SIZE; y++) {
      input[x][y] = random(0,3) > 0 ? 0:1;
    }
  }
}

void start_glider(unsigned char input[SIZE][SIZE]) {
  input[1][0] = 1;
  input[2][1] = 1;
  input[0][2] = 1;
  input[1][2] = 1;
  input[2][2] = 1;
  
  input[6][5] = 1;
  input[4][6] = 1;
  input[5][6] = 1;
  input[4][4] = 1;
  input[5][4] = 1;
  input[3][5] = 1;
  
}

void start(unsigned char input[SIZE][SIZE]) {
  input[3][2] = 1;
  input[3][3] = 1;
  input[3][4] = 1;
  
  input[4][4] = 1;
  input[4][5] = 1;
  input[4][6] = 1;
}

void setup() {
  randomSeed(analogRead(0));
  Rb.init();
  init_life(world[0]);
  init_life(world[1]);
  
  start_glider(world[0]);
}

void loop() {
  steps++;
  draw_life(world[0],world[1]);
  update_life(world[0], world[1]);
  delay(320);
  draw_life(world[1],world[0]);
  update_life(world[1], world[0]);
  delay(320);
  if (steps == 64 || alive_amount(world[0]) < 4 || all_same(world[0], world[1])) {
    steps = 0;
    randomize(world[0]);
    if (BLUE_ON > 0) {
      RED_ON = ON_VALUE;
      BLUE_ON = OFF_VALUE;
    } else {
      RED_ON = OFF_VALUE;
      BLUE_ON = ON_VALUE;
    }
  }
}
