#ifndef JP_DEVICE_H
#define JP_DEVICE_H

#include <linux/device.h>

extern struct device *jpenguin_dev;

extern int init_device(void);
extern void exit_device(void);

#endif // JP_DEVICE_H