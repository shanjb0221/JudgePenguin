#include "header.hpp"

int main(void) {
  header.magic++;

  char *p = (char *)0x10902cull; // magic address in JPenguin mode
  const char *q = "Hello, world!";
  for (int i = 0; i < 13; i++) {
    p[i] = q[i];
  }
  p[13] = '\0';

  return 0;
}
