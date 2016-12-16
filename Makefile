

ARCH=arm
CROSS_COMPILE=arm-linux-gnueabihf-
KERNEL_DIR=~/BPI-M3-bsp/linux-sunxi


obj-m := ledtrig-temperature.o

all:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNEL_DIR) M=$(PWD) clean






