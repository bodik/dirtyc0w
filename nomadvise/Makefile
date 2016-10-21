obj-m += m.o

KERNEL_VER=$(shell uname -r)

SYSCALLSADDR   := $(shell grep " sys_call_table" /boot/System.map-$(KERNEL_VER) | awk '{ print $$1; }')
SYSCALLSADDR32 := $(shell grep "ia32_sys_call_table" /boot/System.map-$(KERNEL_VER) | awk '{ print $$1; }')

EXTRA_CFLAGS += -Dsys_call_table_addr="((void**)0x$(SYSCALLSADDR))"
EXTRA_CFLAGS += -Dia32_sys_call_table_addr="((void**)0x$(SYSCALLSADDR32))" 

all:
	@echo "Building with " . $(EXTRA_CFLAGS)
	make -C /lib/modules/$(KERNEL_VER)/build M=$(PWD)
	cp m.ko /afs/.ics.muni.cz/software/meta-admin/dirtycow/m.`sh kern-build.sh`.ko

clean:
	make -C /lib/modules/$(KERNEL_VER)/build M=$(PWD) clean
