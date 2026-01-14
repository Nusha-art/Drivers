# Задание:
1. Написать программу для чтения данных с устройства dev/zero
## Код:
```#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(){
	char buf[100];
	int fd = open("/dev/zero", O_RDONLY);
	read (fd, buf, 100);
	close (fd);
	
	return 0;
}
```
2. При помощи strace найти системные вызовы для работы с файловой и подсистемой управления памяти
## Результат:
<img width="944" height="651" alt="image" src="https://github.com/user-attachments/assets/e3dfbe15-935d-42e6-be3a-82030c833098" />

Вызовы для работы с файловой системой:
 ```access, openat, read, pread64, fstat, close```

Вызовы для работы с памятью:
```brk, mmap, mprotect, munmap, arch_prctl, prlimit64```
