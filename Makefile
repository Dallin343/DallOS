CC=i686-elf-gcc
AS=i686-elf-as
CFLAGS= -std=gnu99 -ffreestanding -Wall -Wextra -ggdb
LFLAGS= -ffreestanding -O2 -nostdlib
OBJS=kernel.o boot.o gdt.o
HDRS=sys.h
K = kernel.c
B = boot.s

DallOS:
	$(MAKE) boot
	$(MAKE) kernel
	$(CC) -T linker.ld -o DallOS.bin $(LFLAGS) $(OBJS) $(HDRS) -lgcc;
	mkdir -p isodir/boot/grub;
	cp DallOS.bin isodir/boot/DallOS.bin;
	cp grub.cfg isodir/boot/grub/grub.cfg;
	grub-mkrescue /usr/lib/grub/i386-pc -o DallOS.iso isodir;
	$(MAKE) finish
boot:
	$(AS) $(B) -o boot.o -g
kernel:
	$(CC) -c $(K) -o kernel.o $(CFLAGS)
	$(CC) -c gdt.c -o gdt.o $(CFLAGS)

debug:
	$(MAKE) DallOS
	qemu-system-i386 -s -S -cdrom DallOS.iso -serial stdio -curses


run:
	$(MAKE) DallOS
	qemu-system-i386 -cdrom DallOS.iso -curses

.PHONY:clean

clean:
	rm *.o *.bin *.iso
	rm -R isodir
finish:
	#rm *.o *.bin
	rm -R isodir

