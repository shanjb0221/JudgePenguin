    .text
    .globl duck_entry
duck_entry:
    // callee saved registers
    pushq %rbx
    pushq %rbp
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15
    movq %rsp, .kern_old_rsp(%rip)
    jmp kern_loader

    .globl duck_exit
duck_exit:
    movq .kern_old_rsp(%rip), %rsp
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %rbp
    popq %rbx
    ret
	
.kern_old_rsp:
    .quad 0