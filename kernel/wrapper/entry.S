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
    movq %rsp, .old_rsp(%rip)

    // switch to the new stack (first arg)
    movq %rdi, %rsp

    // call the _main function
    call _main

    // restore the stack pointer
    movq .old_rsp(%rip), %rsp

    pop %r15
    pop %r14
    pop %r13
    pop %r12
    pop %rbx
    pop %rbp

    ret

.old_rsp:
    .quad 4
