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
    movq %rsp, .wrapper_rsp(%rip)
    
    // switch kernel stack pointer to JPenguin low virtual address space 
    addq %rdi, %rsp
    // calculate the address of the duck kernel loader in JPenguin low virtual address space
    leaq kern_loader(%rip), %r15
    addq %rdi, %r15
    jmp *%r15

    .globl duck_exit
duck_exit:
    movq .wrapper_rsp(%rip), %rsp
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %rbp
    popq %rbx
    movq %rdi, %rax
    ret
	
.wrapper_rsp:
    .quad 0