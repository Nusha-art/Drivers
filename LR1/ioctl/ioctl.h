#ifndef LAB1_IOCTL_H
#define LAB1_IOCTL_H

#include <linux/ioctl.h>

#define MAGIC_NUM 0xF1

#define IOC_PRINT_HIST _IO(MAGIC_NUM, 0)
#define IOC_CLEAR_HIST _IO(MAGIC_NUM, 1)

#endif
