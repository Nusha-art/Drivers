#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/ioctl.h>

#define DRIVER_IOCTLID 0x11

#define DRIVER_BUF_IS_EMPTY _IOR(DRIVER_IOCTLID, 1, int)
#define DRIVER_CLEAR_BUF _IO(DRIVER_IOCTLID, 0)

#define DEVICE_NAME "testchrdrv"
#define CLASS_NAME "testchrdrv_class"

void fdread(int fd, size_t len) {
    char buf[100];
    ssize_t read_len = read(fd, buf, len);
    if (read_len < 0) {
        printf("fdread: Failed: %ld\n", read_len);
        return;
    }
    buf[read_len] = 0;
    printf("fdread: \"%s\"\n", buf);
}

void fdwrite(int fd, const char str[], size_t len) {
    ssize_t wrote_len = write(fd, str, len);
    if (wrote_len < 0) {
        printf("fdwrite: Failed: %ld\n", wrote_len);
        return;
    }
    printf("Written \"%s\" to dev\n", str);
}

int main() {
	static const char dev_path[] = "/dev/" DEVICE_NAME;

	int wfd, rfd;
	wfd = open(dev_path, O_WRONLY);
	if (wfd < 0) {
		printf("open write err %d\n", wfd);
		return -1;
	}
	rfd = open(dev_path, O_RDONLY);
	if (rfd < 0) {
		printf("open write err %d\n", rfd);
		return -1;
	}

	fdwrite(wfd, "Test write", 11);
	fdread(rfd, 5);

	unsigned char buf_is_empty = 0;
    if (ioctl(wfd, DRIVER_BUF_IS_EMPTY, &buf_is_empty) != 0) {
        printf("Failed to ioctl DRIVER_BUF_IS_EMPTY\n");
        return -1;
    } else {
        printf("Buffer is empty: %s\n", buf_is_empty ? "true" : "false");
    }

    if (ioctl(wfd, DRIVER_CLEAR_BUF) != 0) {
        printf("Failed to ioctl DRIVER_CLEAR_BUF\n");
        return -1;
    }

    if (ioctl(wfd, DRIVER_BUF_IS_EMPTY, &buf_is_empty) != 0) {
        printf("Failed to ioctl DRIVER_BUF_IS_EMPTY\n");
        return -1;
    } else {
        printf("Buffer is empty after DRIVER_CLEAR_BUF: %s\n", buf_is_empty ? "true" : "false");
    }

	close(wfd);
	close(rfd);
	return 0;
}
