#include <linux/firmware.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/printk.h>

#include "../include/header.h"
#include "device.h"
#include "firmware.h"
#include "memory.h"

MODULE_FIRMWARE(FIRMWARE_NAME);

int load_firmware(void) {
    pr_info("load firmware begin.\n");
    int err;

    const struct firmware *kernel;
    err = request_firmware(&kernel, FIRMWARE_NAME, jpenguin_dev);
    if (err) {
        pr_err("request firmware failed.\n");
        return err;
    }

    const struct jpenguin_kernel_header *header = (struct jpenguin_kernel_header *)kernel->data;
    if (memcmp(header->signature, "JPenguin", sizeof(header->signature))) {
        pr_err("kernel signature validation failed!\n");
        err = -EINVAL;
        goto release_fw;
    } else {
        pr_info("kernel signature validation success.\n");
    }

    if (kernel->size > KERNEL_SIZE - KERNEL_STACK_SIZE - page_table_size) {
        pr_err("kernel size too large.\n");
        err = -ENOMEM;
        goto release_fw;
    }

    memset((void *)kernel_base_linux, 0, KERNEL_OFFSET);
    memcpy((void *)kernel_base_linux + KERNEL_OFFSET, kernel->data, kernel->size);
    memset((void *)kernel_base_linux + KERNEL_OFFSET + kernel->size, 0, KERNEL_SIZE - kernel->size - KERNEL_OFFSET);

    int num_pages = (kernel->size + KERNEL_OFFSET + PAGE_SIZE - 1) / PAGE_SIZE;
    phys_addr_t kernel_base_phys = MEM_START + page_table_size;
    for (int i = 0; i < num_pages; i++)
        map_page(kernel_base_phys + i * PAGE_SIZE, kernel_base_linux + i * PAGE_SIZE);

    int num_stack_pages = KERNEL_STACK_SIZE / PAGE_SIZE;
    virt_addr_t kernel_stack_base_linux = kernel_stack_top_linux - KERNEL_STACK_SIZE;
    phys_addr_t kernel_stack_base_phys = MEM_START + KERNEL_SIZE - KERNEL_STACK_SIZE;
    for (int i = 0; i < num_stack_pages; i++)
        map_page(kernel_stack_base_phys + i * PAGE_SIZE, kernel_stack_base_linux + i * PAGE_SIZE);

    release_firmware(kernel);

    pr_info("load firmware end.\n");
    return 0;

release_fw:
    release_firmware(kernel);
    return err;
}