obj-m := simple_read_cDriver.o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD   := $(shell pwd)


modules:
		$(MAKE) -C $(KERNELDIR) M=$(PWD)

app:
		$ gcc app.c -o app

clean:
		@rm -fr .*.cmd *.o   *.a *.mod *.ko modules.order Module.symvers simple_read_cDriver.mod.c app
