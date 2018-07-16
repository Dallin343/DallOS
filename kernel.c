#include "sys.h"
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

void outb(uint16_t port, uint8_t val)
{
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


enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

size_t strlen(const char* str)
{
	size_t len = 0;
	while(str[len])
		len++;
	return len;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;

volatile uint16_t* terminal_buffer;

void terminal_initialize(void)
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_GREEN, VGA_COLOR_DARK_GREY);
	terminal_buffer = (uint16_t *)0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}

void terminal_shiftlineup(void)
{
	for (size_t y = 1; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			char c = terminal_buffer[y * VGA_WIDTH + x];
			terminal_putchar(c);
		}
	}
}

void terminal_delentryat(size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(' ', terminal_color);
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c)
{
	switch (c) {
		case 0xA:
		case 0xD:
			terminal_row++;
			terminal_column = 0;
			break;
		case 0x8:
			//terminal_writestring("\nIn the case\n");
			terminal_delentryat(terminal_row, --terminal_column);
			if (terminal_column == 0) {
				terminal_column = VGA_WIDTH-1;
				terminal_row--;
			}
			break;
		default:
			terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
			if (++terminal_column == VGA_WIDTH) {
				terminal_column = 0;
				if  (++terminal_row == VGA_HEIGHT) {
					terminal_shiftlineup();
					terminal_row--;
				}
			}
			break;

	}
}

void terminal_write (const char* data, size_t size)
{
	for (size_t i = 0; i < size; i++) {
		terminal_putchar(data[i]);
	}
}

void terminal_writestring(const char* data)
{
	terminal_write(data, strlen(data));
}

void kernel_main(void)
{
	terminal_initialize();

	terminal_writestring("This is a test of the Keyboard:\n");
	asm volatile("cli");
	gdt_install();
	idt_install();
	PIC_remap(0x20,0x28);
	//asm volatile("int $5");
	terminal_writestring("This is a test of the Keyboard:\n");
	while(1){
		//terminal_putchar(getScancode());
	}
}
