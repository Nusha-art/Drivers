#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include "../ioctl/ioctl.h"

int main(void)
{
	int fd;
	int value;
	int read_value;
	int i;

	fd = open("/dev/app_lab_1", O_RDWR);
	if (fd < 0) {
		perror("open");
		return 1;
	}

	for (i = 0; i < 1000; i++) {
		value = i;

		if (write(fd, &value, sizeof(int)) != sizeof(int)) {
			perror("write");
			break;
		}

		while (read(fd, &read_value, sizeof(int)) < 0) {
			if (errno != EAGAIN) {
				perror("read");
				goto out;
			}
		}
	}

	ioctl(fd, IOC_PRINT_HIST);

out:
	close(fd);
	return 0;
}
