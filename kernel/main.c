#include "../include/header.h"

extern int entry(unsigned int);

struct judge_penguin_header __attribute__((section(".header")))
header = {.signature = "JPenguin", .entry = entry, .magic = 0xdeadbeef};

int main(void) {
  header.magic++;
  return 0;
}