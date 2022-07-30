#include "utils.h"

void msleep(int tms) {
  struct timeval tv;
  tv.tv_sec = tms / 1000;
  tv.tv_usec = (tms % 1000) * 1000;
  select(0, NULL, NULL, NULL, &tv);
}

int chartoint(char *str, int numbase) {
  long num = strtol(str, NULL, numbase);
  return (int)num;
}

struct tm get_time() {
  time_t s = 1;
  struct tm *current_time;
  s = time(NULL);
  current_time = localtime(&s);

  return *current_time;
}

void repeat(char ch, int times) {
  for (int i = 0; i < times; i++) {
    printf("%c", ch);
  }
}
