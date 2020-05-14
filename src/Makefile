obj-m := simple_read_cDriver.o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD   := $(shell pwd)


modules:
		$(MAKE) -C $(KERNELDIR) M=$(PWD)

clean:
		@rm -fr *.o  *.a *.mod *.ko modules.order Module.symvers simple_char_driver.mod.c