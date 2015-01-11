#include <TFT.h>

struct state {
  void (*func)(struct state *);
  struct state *next;
};

struct state initial;
struct state after_initial;

void setup() {
  initial.func = &start;
  initial.next = &initial;
  
  after_initial.func = &after_start;
  after_initial.next = &after_initial;
  Serial.begin(9600);
}

void start(struct state *current) {
  static unsgined int times = 0;
  times++;
  Serial.println("start");
  if (times > 3) {
    current->next = &after_start
  }
}

void after_start(struct state *current) {
  Serial.println("after start");
}

void loop() {
  static struct state *current = &initial;
  if (NULL != current) {
    current->func();
    current = current->next;
  }
  
}
