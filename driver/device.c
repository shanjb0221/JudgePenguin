#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/miscdevice.h>

#include "device.h"
#include "version.h"
#include "../include/ioctl.h"

struct device* jpenguin_dev;

MODULE_FIRMWARE(FIRMWARE_NAME);

static long jpenguin_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    pr_info("JudgePenguin: ioctl cmd: %d, arg: %ld\n", cmd, arg);
    switch (cmd) {
        case JP_IOCTL_VERSION:
            return JPENGUIN_VERSION << 8 | JPENGUIN_SUBVERSION << 4 | JPENGUIN_PATCHLEVEL;
        default:
            return -EINVAL;
    }
}

static int jpenguin_console_open(struct inode *inode, struct file *file) {
    return 0;
}

static int jpenguin_console_release(struct inode *inode, struct file *file) {
    return 0;
}

static ssize_t jpenguin_console_read(struct file *file, char __user *buf, size_t count, loff_t *ppos) {
    return 0;
}

static const struct file_operations jpenguin_fops = {
    .owner = THIS_MODULE,
    .unlocked_ioctl = jpenguin_ioctl,
    .compat_ioctl = jpenguin_ioctl,
    .llseek = noop_llseek,
    .open = jpenguin_console_open,
    .release = jpenguin_console_release,
    .read = jpenguin_console_read,
};

static struct miscdevice jpenguin_miscdev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "JudgePenguin",
    .fops = &jpenguin_fops,
};

int device_init(void) {
    pr_info("JudgePenguin: device init\n");
    int err;

    jpenguin_dev = root_device_register("JudgePenguin");
    if (IS_ERR(jpenguin_dev)) {
        pr_err("Failed to register JudgePenguin device\n");
        return PTR_ERR(jpenguin_dev);
    }

    err = misc_register(&jpenguin_miscdev);
    if (err) goto unreg_dev;

    return 0;
unreg_dev:
    root_device_unregister(jpenguin_dev);
    return err;
}

void device_exit(void) {
    pr_info("JudgePenguin: device exit\n");
    misc_deregister(&jpenguin_miscdev);
    root_device_unregister(jpenguin_dev);
}