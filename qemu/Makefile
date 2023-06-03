QEMU ?= qemu-system-x86_64

#  -accel kvm \
#  -cpu host,-kvm-asyncpf,-kvm-pv-eoi,-kvm-pv-ipi,-kvm-pv-sched-yield,-kvm-pv-unhalt,-kvm-steal-time,-kvmclock \

# -cpu Skylake-Client \
#   -gdb tcp::1234 -S \

cpu_disable_features := -fma,-pcid,-x2apic,-tsc-deadline,-avx,-f16c,-hle,-avx2,-invpcid,-rtm,-rdseed,-3dnowprefetch,-xsavec,-kvm-asyncpf,-kvm-pv-eoi,-kvm-pv-ipi,-kvm-pv-sched-yield,-kvm-pv-unhalt,-kvm-steal-time,-kvmclock

qemu_image := focal-server-cloudimg-amd64.img
qemu_args_base := \
  -smp 1 -m 4G -nographic \
  -no-reboot -no-shutdown \
  -D qemu.log \
  -d guest_errors,cpu_reset \
  -machine q35,kernel_irqchip=split \
  -cpu Skylake-Client,$(cpu_disable_features) \
  -drive file=$(qemu_image) \
  -net user,id=net,hostfwd=tcp::2333-:22 -net nic,model=e1000e \
  -serial mon:stdio

qemu_args_kvm := $(qemu_args_base) \
  -accel kvm

qemu_args_gdb := $(qemu_args_base) \
  -gdb tcp::1234 -S

$(qemu_image):
	wget https://mirrors.tuna.tsinghua.edu.cn/ubuntu-cloud-images/focal/20230530/$(qemu_image)

.ONESHELL:
image: $(qemu_image)
	cat >user-data <<EOF
	#cloud-config
	password: guest
	chpasswd: { expire: False }
	ssh_pwauth: True
	EOF
	cloud-localds user-data.img user-data
	qemu-img resize $(qemu_image) +64G
	$(QEMU) $(qemu_args_kvm) -drive file=user-data.img,format=raw

qemu: $(qemu_image)
	@$(QEMU) $(qemu_args_kvm)

gdb: $(qemu_image)
	@echo "Run 'target remote localhost:1234' in gdb to connect."
	@$(QEMU) $(qemu_args_gdb)	

ssh:
	ssh qemu
