# Задание
## Скомпилировать символьный драйвер

<img width="713" height="298" alt="image" src="https://github.com/user-attachments/assets/f9c5424a-7a6d-422a-bb6a-5ec1515e3055" />

## Загрузить модуль

<img width="439" height="158" alt="image" src="https://github.com/user-attachments/assets/6a91efab-d5bb-4f60-aff9-20ca07418608" />

## Создать файл устройства

<img width="505" height="43" alt="image" src="https://github.com/user-attachments/assets/79d0d0c3-2527-454a-ae4a-0d55658f1a48" />

## Напиcать приложение для открытия спец. файла устройства

```
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

```

<img width="418" height="57" alt="image" src="https://github.com/user-attachments/assets/42c9df9b-e8bc-497a-a98e-fd6e8aacfa90" />
