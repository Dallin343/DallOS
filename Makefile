CC=i686-elf-gcc
AS=i686-elf-as
CFLAGS= -std=gnu99 -ffreestanding -Wall -Wextra -ggdb
LFLAGS= -ffreestanding -O2 -nostdlib
OUTFLAG = | log.txt
OBJS=kernel.o boot.o gdt.o idt.o pic.o tty.o stdio.o
HDRS=includes/sys.h includes/tty.h lib/stdio.h
K = kern/kernel.c
B = kern/boot.s

DallOS:
	$(MAKE) boot
	$(MAKE) kernel
	$(CC) -T kern/linker.ld -o DallOS.bin $(LFLAGS) $(OBJS) $(HDRS) -lgcc;
	mkdir -p isodir/boot/grub;
	cp DallOS.bin isodir/boot/DallOS.bin;
	cp grub.cfg isodir/boot/grub/grub.cfg;
	grub-mkrescue /usr/lib/grub/i386-pc -o DallOS.iso isodir;
	$(MAKE) finish
boot:
	$(AS) $(B) -o boot.o -g
kernel:
	$(CC) -c $(K) -o kernel.o $(CFLAGS)
	$(CC) -c kern/gdt.c -o gdt.o $(CFLAGS)
	$(CC) -c kern/idt.c -o idt.o $(CFLAGS)
	$(CC) -c drivers/pic.c -o pic.o $(CFLAGS)
	$(CC) -c drivers/tty.c -o tty.o $(CFLAGS)
	$(CC) -c lib/stdio.c -o stdio.o $(CFLAGS)

log:
	$(AS) $(B) -o boot.o -g > log.txt
	$(CC) -c $(K) -o kernel.o $(CFLAGS) $(OUTFLAG)
	$(CC) -c gdt.c -o gdt.o $(CFLAGS) $(OUTFLAG)
	$(CC) -c idt.c -o idt.o $(CFLAGS) $(OUTFLAG)
	$(CC) -T linker.ld -o DallOS.bin $(LFLAGS) $(OBJS) $(HDRS) -lgcc $(OUTFLAG);
	mkdir -p isodir/boot/grub;
	cp DallOS.bin isodir/boot/DallOS.bin $(OUTFLAG);
	cp grub.cfg isodir/boot/grub/grub.cfg; $(OUTFLAG)
	grub-mkrescue /usr/lib/grub/i386-pc -o DallOS.iso isodir $(OUTFLAG);
	$(MAKE) finish
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
	rm *.o
	rm -R isodir
