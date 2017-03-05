# Comment/uncomment the following line to disable/enable debugging
#DEBUG = y


# Add your debugging flag (or not) to CFLAGS
ifeq ($(DEBUG),y)
  DEBFLAGS = -O -g -DSHORT_DEBUG # "-O" is needed to expand inlines
else
  DEBFLAGS = -O2
endif

INCDIR=/home/thwisha/Downloads/my_intr_test/sim_intr.h
EXTRA_CFLAGS += $(DEBFLAGS) 
EXTRA_CFLAGS += -I.. 

ifneq ($(KERNELRELEASE),)
# call from kernel build system

obj-m	:= sim_intr_new.o 

else

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD       := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

endif


clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions

depend .depend dep:
	$(CC) $(EXTRA_CFLAGS) -M *.c > .depend


ifeq (.depend,$(wildcard .depend))
include .depend
endif
