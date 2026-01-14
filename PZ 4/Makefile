obj-m += driver.o

PWD := $(CURDIR)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc app.c -o app

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm app
