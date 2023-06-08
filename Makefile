kvm_server:
	@sudo make -C qemu qemu

clean:
	@make -C kernel clean

kvm_ssh:
	@make -C qemu ssh

kvm_client: clean
	@echo "Copy files to qemu"
	@rsync -r kernel qemu:~
	@rsync -r driver qemu:~
	@rsync -r include qemu:~
	@rsync qemu/Makefile.qemu qemu:Makefile
	@echo "Compile in qemu"
	@ssh qemu 'make' || true
	@echo "Copy built files back"
	@scp -r qemu:kernel/build/* kernel/build/
	@cp kernel/build/kernel.elf kernel/build/remote.elf
	@make -C kernel locate_remote_entry
	@make -C kernel locate_remote_header


gdb_server:
	@make -C qemu gdb

gdb_client:
	@make -C kernel gdb

