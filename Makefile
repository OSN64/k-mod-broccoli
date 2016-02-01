
MODNAME = green

obj-m += $(MODNAME).o

all:
		make -C /lib/modules/$(shell uname -r)/build M=$(PWD)  modules

clean:
		make -C /lib/modules/$(shell uname -r)/build M=$(PWD)  clean

uninstall:
	sudo rmmod $(MODNAME) || true

reload:	all
		sudo rmmod $(MODNAME) || true
		sudo insmod ./$(MODNAME).ko


# view
# dmesg | tail
