#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
	const char *dev = "/dev/foo_device";
	
	int fd = open(dev, O_RDWR);
	
	if (fd < 0) {
		printf ("failed to open '%s' \n", dev);
		return fd;
	}
	printf ("opened  '%s' device with fd: '%d'", dev, fd);
	
	close(fd);
	
}
