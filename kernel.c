#include "sys.h"
#include "tty.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

uint8_t *memcpy(uint8_t *dest,  const uint8_t *src, int count)
{
	for (int i = 0; i < count; i++) {
		dest[i] = src[i];
	}
	return dest;
}

uint8_t *memset(uint8_t *dest, uint8_t val, int count)
{
	while (count--)
		dest[count] = val;
	return dest;
}

uint16_t *memsetw(uint16_t *dest, uint16_t val, int count)
{
	while(count--)
		dest[count] = val;
	return dest;
}

void outb(uint16_t port, uint8_t val) {
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
    /* There's an outb %al, $imm8  encoding, for compile-time constant port numbers that fit in 8b.  (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

char getScancode()
{
    while (!(inb(0x64) & 1));
    return inb(0x60);
}

unsigned char scancode[128] =
{
};


char getChar() {
    return getScancode();//scancode[getScancode()];
}

void kernel_main(void)
{
	terminal_initialize();

	terminal_writestring("This is a test of the Keyboard:\n");
	asm volatile("cli");
	gdt_install();
	idt_install();
	PIC_remap(0x20,0x28);
	terminal_writestring("This is a test of the Keyboard:\n");
	while(1){
		//terminal_putchar(getScancode());
	}
}
