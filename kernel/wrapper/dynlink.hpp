#pragma once

#include "header.hpp"

typedef uint64_t off_t;
typedef uint64_t size_t;

struct _IO_FILE {
    unsigned flags;
    unsigned char *rpos, *rend;
    int (*close)(_IO_FILE *);
    unsigned char *wend, *wpos;
    unsigned char *mustbezero_1;
    unsigned char *wbase;
    size_t (*read)(_IO_FILE *, unsigned char *, size_t);
    size_t (*write)(_IO_FILE *, const unsigned char *, size_t);
    off_t (*seek)(_IO_FILE *, off_t, int);
    unsigned char *buf;
    size_t buf_size;
    _IO_FILE *prev, *next;
    int fd;
    int pipe_pid;
    long lockcount;
    int mode;
    volatile int lock;
    int lbf;
    void *cookie;
    off_t off;
    char *getln_buf;
    void *mustbezero_2;
    unsigned char *shend;
    off_t shlim, shcnt;
    _IO_FILE *prev_locked, *next_locked;
    struct __locale_struct *locale;
};

// extern _IO_FILE __stdin_FILE;
extern _IO_FILE __stdout_FILE;
extern _IO_FILE __stderr_FILE;
extern _IO_FILE *stdout;
extern _IO_FILE *stderr;
extern _IO_FILE *__stdout_used;
extern _IO_FILE *__stderr_used;
extern "C" void (*const __init_array_start)(void);
extern "C" void (*const __init_array_end)(void);
extern "C" void (*const __fini_array_start)(void);
extern "C" void (*const __fini_array_end)(void);
extern void *__traps[256];

inline void dynlink_ptr(uintptr_t *ptr, uintptr_t offset) {
    if (*ptr) *ptr += offset;
}

void fake_dynlink(uintptr_t offset) {
#define DYNLINK_PTR(x) dynlink_ptr((uintptr_t *)(x), offset)
#define DYNLINK(x) DYNLINK_PTR(&(x))
    uintptr_t a = (uintptr_t)&__init_array_start;
    for (; a < (uintptr_t)&__init_array_end; a += sizeof(void (*)()))
        DYNLINK_PTR(a);

    a = (uintptr_t)&__fini_array_start;
    for (; a < (uintptr_t)&__fini_array_end; a += sizeof(void (*)()))
        DYNLINK_PTR(a);

    // DYNLINK(__stdin_FILE.buf);
    // DYNLINK(__stdin_FILE.close);
    // DYNLINK(__stdin_FILE.read);
    // DYNLINK(__stdin_FILE.write);
    // DYNLINK(__stdin_FILE.seek);
    // DYNLINK(stdin);
    // DYNLINK(__stdin_used);

    DYNLINK(__stdout_FILE.buf);
    DYNLINK(__stdout_FILE.close);
    DYNLINK(__stdout_FILE.read);
    DYNLINK(__stdout_FILE.write);
    DYNLINK(__stdout_FILE.seek);
    DYNLINK(stdout);
    DYNLINK(__stdout_used);

    DYNLINK(__stderr_FILE.buf);
    DYNLINK(__stderr_FILE.close);
    DYNLINK(__stderr_FILE.read);
    DYNLINK(__stderr_FILE.write);
    DYNLINK(__stderr_FILE.seek);
    DYNLINK(stderr);
    DYNLINK(__stderr_used);

    for (int i = 0; i < 256; ++i)
        DYNLINK(__traps[i]);

#undef DYNLINK
#undef DYNLINK_PTR
}