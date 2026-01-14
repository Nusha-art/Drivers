# Задание
## Создать символьный драйвер с автоматическим созданием спец. файла устройства. Реализовать чтение и запись в глобальный буфер. Реализовать ioctl для сброса содержимого буфера и получения информации чист ли буфер
```
static int __init aut_init(void) {
	int res = 0;

	res = alloc_chrdev_region(&dev_no, 0, 1, DEVICE_NAME);
    if (res < 0) {
        pr_err(DEVICE_NAME ": failed to allocate device numbers with error %d\n", res);
        return res;
    }

    cdev_init(&dev, &fops);
    dev.owner = THIS_MODULE;

	res = cdev_add(&dev, dev_no, 1);
    if (res < 0) {
        pr_err(DEVICE_NAME ": failed to add character device with error %d\n", res);
        unregister_chrdev_region(dev_no, 1);
        return res;
    }

	dev_class = class_create(CLASS_NAME);
    if (IS_ERR(dev_class)) {
        pr_err(DEVICE_NAME ": failed to create device class\n");
        res = PTR_ERR(dev_class);
        cdev_del(&dev);
        unregister_chrdev_region(dev_no, 1);
        return res;
    }

	device = device_create(dev_class, NULL, dev_no, NULL, DEVICE_NAME);
    if (IS_ERR(device)) {
        pr_err(DEVICE_NAME ": Failed to create device node\n");
        res = PTR_ERR(device);
        class_destroy(dev_class);
        cdev_del(&dev);
        unregister_chrdev_region(dev_no, 1);
        return res;
    }

	pr_info(DEVICE_NAME ": registered %s\n", DEVICE_NAME);
	return SUCCESS;
}

```

```
static ssize_t device_read(struct file* flip, char __user* user_buf, size_t count, loff_t* offset) {
    pr_info(DEVICE_NAME ": device_read: count=%zu, offset=%lld, buffer_len=%zu\n", count, *offset, buf_len);

    if (wait_event_interruptible(wq, buf_len != 0)) {
        pr_info(DEVICE_NAME ": device_read: interrupted");
        return -ERESTARTSYS;
    }

    if (*offset >= buf_len) {
        pr_info(DEVICE_NAME ": device_read: End Of File");
        return 0;
    }

    const size_t readable_bytes_len = min(count, buf_len - *offset);

    if (copy_to_user(user_buf, buf + *offset, readable_bytes_len)) {
        pr_err(DEVICE_NAME ": device_read: copy_to_user failed");
        return -EFAULT;
    }

    *offset += readable_bytes_len;

    pr_info(DEVICE_NAME ": device_read: successfully read %lu bytes", readable_bytes_len);

    return readable_bytes_len;
}

static ssize_t device_write(struct file* flip, const char __user* user_buf, size_t count, loff_t* offset) {
    pr_info(DEVICE_NAME ": device_write: count=%zu, offset=%lld, buffer_len=%zu\n", count, *offset, buf_len);

    if (buf_len + count > BUF_LEN) {
        pr_err(DEVICE_NAME ": device_write: buffer is full");
        return -ENOSPC;
    }

    if (copy_from_user(buf + *offset, user_buf, count)) {
        pr_err(DEVICE_NAME ": device_write: copy_from_user failed");
        return -EFAULT;
    }

    buf_len += count;
    wake_up_interruptible(&wq);
    *offset = buf_len;

    pr_info(DEVICE_NAME ": device_write: successfully wrote %lu bytes", count);

    return count;
}
```

```
#include <linux/ioctl.h>

#define DRIVER_IOCTLID 0x11

#define DRIVER_BUF_IS_EMPTY _IOR(DRIVER_IOCTLID, 1, int)
#define DRIVER_CLEAR_BUF _IO(DRIVER_IOCTLID, 0)

```

### Результат:
<img width="709" height="360" alt="image" src="https://github.com/user-attachments/assets/eb3d527a-8704-4ebc-acd7-a21f1bba1bc8" />

<img width="434" height="145" alt="image" src="https://github.com/user-attachments/assets/a48c34c9-d13e-4c31-8fe0-a77ebb62a8c5" />


