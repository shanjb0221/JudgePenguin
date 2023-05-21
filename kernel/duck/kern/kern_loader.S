    .text
    .globl kern_loader
kern_loader:
	pushq $0  # auxv end
	pushq $0  # envp end
	pushq $0  # argv end
	pushq $0  # argc
	
	# callq _start
	
	popq %rdi
	popq %rdi
	popq %rdi
	popq %rdi

	ret
