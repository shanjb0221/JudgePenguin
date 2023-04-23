#ifndef JP_HEADER_H
#define JP_HEADER_H

struct judge_penguin_header {
  char signature[8];
  int (*entry)(unsigned int);
  int magic;
  int output[100];
} __attribute__((packed));

#endif