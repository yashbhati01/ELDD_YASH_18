obj-m := file.o

KDIR = /lib/modules/$(shell uname -r)/build

PWD = $(shell pwd)

default:
	make -C $(KDIR) M=$(PWD) modules
	gcc -o app app.c
	gcc -o app2 app2.c
	gcc -o read read.c
	gcc -o read2 read2.c
clean:
	make -C $(KDIR) M=$(PWD) clean
	rm -rf app read app2 read2
