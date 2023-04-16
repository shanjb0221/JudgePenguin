# JudgePenguin

Yet another [JudgeDuck](https://github.com/JudgeDuck/JudgeDuck-OS): Stable and Accurate Judge System on Linux

Working in [Progress](https://github.com/shanjb0221/JudgePenguin/tree/docs)... (see branch [`docs`](https://github.com/shanjb0221/JudgePenguin/tree/docs))

## Requirments

Currently developing with:

- `Ubuntu 20.04`
- Linux kernel version `5.4.0-139-generic`
- gcc version `9.4.0`


```bash
sudo apt install gcc linux-headers-5.4.0-139-generic
```

## Preparation

Modify `/etc/default/grub`, append `memmap=0x20000000\\\$0x40000000` to `GRUB_CMDLINE_LINUX`. Then `sudo update-grub` and reboot.

(optional, but suggested) Use `sudo sysctl -w kernel.watchdog=0` to disable Linux Kernel's Hard Lockup Watchdog. If not doing this, our module may be interrupted about 20 seconds after startup. 

## Build

```bash
cd src
make
```

## Run

<u>**Warning**</u>: **This may broke your system! This may broke your system! This may broke your system!**

Please be awared of above, or test this in QEMU.

```bash
make load # for the first time
make remove # remove manually
make reload # remove then load

make trace # print recent kernel message
```
