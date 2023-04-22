    .text
    .globl entry
entry:
    push %rbp
    push %rbx
    push %r12
    push %r13
    push %r14
    push %r15

    // save the stack pointer
    movq %rsp, .stack_pointer(%rip)

    // switch to the new stack
    leaq .stack_end(%rip), %rsp

    // call the main function
    call main

    // restore the stack pointer
    movq .stack_pointer(%rip), %rsp

    pop %r15
    pop %r14
    pop %r13
    pop %r12
    pop %rbx
    pop %rbp

    ret

// define a variable to store the stack pointer
.stack_pointer:
    .quad 0

// allocate space for the stack
.stack:
    .skip 1024
.stack_end:
