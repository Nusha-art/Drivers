#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/ktime.h>
#include <linux/types.h>
#include "../ioctl/ioctl.h"

#define DRIVER_NAME "driver_lab_1"
#define DEVICE_NAME "app_lab_1"
#define CLASS_NAME "driver_lab_1_class"

#define HIST_BINS 50
#define BIN_WIDTH_US 10

static dev_t dev_no;
static struct cdev dev;
static struct class *dev_class;
static struct device *device;

static int value;
static int has_data;

static ktime_t start_time;
static unsigned int histogram[HIST_BINS];

static int device_open(struct inode *inode, struct file *file)
{
    return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
    return 0;
}

static ssize_t device_write(struct file *file, const char __user *user_buf,
                            size_t count, loff_t *offset)
{
    if (count != sizeof(int))
        return -EINVAL;

    if (copy_from_user(&value, user_buf, sizeof(int)))
        return -EFAULT;

    if (!has_data) {
        start_time = ktime_get();
    }

    has_data = 1;
    return sizeof(int);
}

static ssize_t device_read(struct file *file, char __user *user_buf,
                           size_t count, loff_t *offset)
{
    ktime_t now;
    s64 delta_us;
    int bin;

    if (!has_data)
        return -EAGAIN;

    if (count != sizeof(int))
        return -EINVAL;

    now = ktime_get();
    delta_us = ktime_us_delta(now, start_time);

    bin = delta_us / BIN_WIDTH_US;
    if (bin >= HIST_BINS)
        bin = HIST_BINS - 1;

    histogram[bin]++;

    if (copy_to_user(user_buf, &value, sizeof(int)))
        return -EFAULT;

    return sizeof(int);
}

static long device_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int i;

    switch (cmd) {
    case IOC_PRINT_HIST:
        for (i = 0; i < HIST_BINS; i++)
            pr_info(DRIVER_NAME ": bin %d [%d-%d us]: %u\n",
                    i,
                    i * BIN_WIDTH_US,
                    (i + 1) * BIN_WIDTH_US,
                    histogram[i]);
        break;

    case IOC_CLEAR_HIST:
        for (i = 0; i < HIST_BINS; i++)
            histogram[i] = 0;
        has_data = 0;
        break;

    default:
        return -ENOTTY;
    }

    return 0;
}

static const struct file_operations fops = {
    .open = device_open,
    .release = device_release,
    .read = device_read,
    .write = device_write,
    .unlocked_ioctl = device_ioctl,
};

static int __init driver_lab_1_init(void)
{
    int res;

    res = alloc_chrdev_region(&dev_no, 0, 1, DEVICE_NAME);
    if (res < 0)
        return res;

    cdev_init(&dev, &fops);
    dev.owner = THIS_MODULE;

    res = cdev_add(&dev, dev_no, 1);
    if (res < 0)
        goto err_cdev;

    dev_class = class_create(CLASS_NAME);
    if (IS_ERR(dev_class)) {
        res = PTR_ERR(dev_class);
        goto err_class;
    }

    device = device_create(dev_class, NULL, dev_no, NULL, DEVICE_NAME);
    if (IS_ERR(device)) {
        res = PTR_ERR(device);
        goto err_device;
    }

    return 0;

err_device:
    class_destroy(dev_class);
err_class:
    cdev_del(&dev);
err_cdev:
    unregister_chrdev_region(dev_no, 1);
    return res;
}

static void __exit driver_lab_1_exit(void)
{
    device_destroy(dev_class, dev_no);
    class_destroy(dev_class);
    cdev_del(&dev);
    unregister_chrdev_region(dev_no, 1);
}

module_init(driver_lab_1_init);
module_exit(driver_lab_1_exit);

MODULE_LICENSE("GPL");
