obj-m = simple_char_driver.o

KVERSION = $(shell uname -r)

all:
		make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules

clean:
		make -C /lib/modules/$(KVERSION)/build M=$(PWD) clean

app:
		gcc app.c -o app
