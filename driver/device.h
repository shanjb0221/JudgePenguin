#ifndef JP_DEVICE_H
#define JP_DEVICE_H

#include <linux/device.h>

#define FIRMWARE_NAME "jpenguin_kernel.bin"

extern struct device *jpenguin_dev;

extern int device_init(void);
extern void device_exit(void);

#endif // JP_DEVICE_H