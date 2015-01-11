#include <TFT.h>

typedef void (*draw_func)(int,int);
typedef void* (*state_func)();

const int CENTER_X = (MAX_X - MIN_X) / 2;
const int CENTER_Y = (MAX_Y - MIN_Y) / 2;

const unsigned int COLORS[] = {WHITE, BLUE, RED, YELLOW, GREEN};
static unsigned int CURRENT_COLOR = -1;

void drawLineFromCenter(unsigned int x,unsigned int y, unsigned int color) {
  Tft.drawLine(CENTER_X,CENTER_Y, x, y, color);
}

struct int_stream {
  int init;
  int last;
  int interval;
  int current;
};

void create_int_stream(struct int_stream* input, int init, int last, int interval) {
  input->init = init;
  input->last = last;
  input->interval = interval;
  input->current = init;initial.func = &start;initial.func = &start;
}

int next(struct int_stream* input) {
  int result = input->current;
  if (!has_next(input)) {
    return -1;
  }
  input->current += input->interval;
  return result;
}

bool has_next(struct int_stream* input) {
  if (input->interval > 0) {
    return input->current < input->last;
  } else {
    return input->current > input->last;
  }
}

void setup() {
  Tft.init();
  //Serial.begin(9600);
}

void loop() {
  static state_func state = &do_top;
  state = (state_func) state();
}

void* do_top() {
  //Serial.println("top");
  static struct int_stream x_stream;
  
  if (!has_next(&x_stream)) {
    create_int_stream(&x_stream,MIN_X,MAX_X+2,2);
    return (void*)do_right;
  }
  int x = next(&x_stream);
  drawLineFromCenter(x,MIN_Y, CURRENT_COLOR);
  drawLineFromCenter(x+1, MIN_Y, BLACK);
  return (void *)do_top;
}

void* do_right() {
  //Serial.println("right");
  static struct int_stream y_stream;
  if (!has_next(&y_stream)) {
    create_int_stream(&y_stream,MIN_Y,MAX_Y+2,2);
    return (void*)do_bottom;
  }
  int y = next(&y_stream);
  drawLineFromCenter(MAX_X,y,CURRENT_COLOR);
  drawLineFromCenter(MAX_X,y+1,BLACK);
  return (void *)do_right;
}

void* do_bottom() {
  //Serial.println("bottom");
  static struct int_stream x_stream;
  if (!has_next(&x_stream)) {
    create_int_stream(&x_stream,MAX_X,MIN_X-2,-2);
    return (void*)do_left;
  }
  int x = next(&x_stream);
  drawLineFromCenter(x+1,MAX_Y, CURRENT_COLOR);
  drawLineFromCenter(x, MAX_Y, BLACK);
  return (void *)do_bottom;
}

void* do_left() {
  //Serial.println("left");
  static struct int_stream y_stream;
  
  if (!has_next(&y_stream)) {
    create_int_stream(&y_stream,MAX_Y,MIN_Y-2,-2);
    set_current_color();
    return (void*)do_top;
  }
  int y = next(&y_stream);
  drawLineFromCenter(MIN_X,y+1,CURRENT_COLOR);
  drawLineFromCenter(MIN_X,y,BLACK);
  return (void *)do_left;
}

void set_current_color() {
  static unsigned int color_index = -1;
  color_index += 1;
  if (color_index >= (sizeof(COLORS) / sizeof(COLORS[0]))) {
    color_index = 0;
  }
  CURRENT_COLOR = COLORS[color_index];
}
