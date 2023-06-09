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
    movq %rsp, .linux_rsp(%rip)

    // switch to the new stack (first arg)
    movq %rdi, %rsp

    call switch_linux

    // restore the stack pointer
    movq .linux_rsp(%rip), %rsp

    pop %r15
    pop %r14
    pop %r13
    pop %r12
    pop %rbx
    pop %rbp

    ret

.linux_rsp:
    .quad 0

    .globl linux_cr3
linux_cr3:
    .quad 0
