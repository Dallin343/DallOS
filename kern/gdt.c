#include "../includes/sys.h"
#include "../includes/tty.h"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct gdt_entry
{
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_mid;
	uint8_t access;
	uint8_t granularity;
	uint8_t base_hi;
}__attribute__((packed));

struct gdt_pointer
{
	uint16_t limit;
	uint32_t base;
}__attribute__((packed));

struct gdt_entry gdt[3];
struct gdt_pointer gdt_p;

extern void gdt_flush();

void gdt_set_entry(size_t num, uint32_t limit, uint32_t base, uint8_t access, uint8_t gran)
{
	//sets the base for the GDT entry based on masking the base parameter
	gdt[num].base_low = (base & 0xFFFF);
	gdt[num].base_mid = ((base >> 16) & 0xFF);
	gdt[num].base_hi = ((base >> 24) & 0xFF);

	//Sets the limit.
	gdt[num].limit_low = (limit & 0xFFFF);
	gdt[num].granularity = ((limit >> 16) & 0x0F);

	//set gran and access flags.
	gdt[num].access = access;
	gdt[num].granularity |= (gran & 0xF0);
}

void gdt_install()
{
	terminal_writestring("Here\n");
	gdt_p.limit = (sizeof(struct gdt_entry) * 3) - 1;
	gdt_p.base = (uint32_t)&gdt;

	//Sets NULL descriptor
	gdt_set_entry(0,0,0,0,0);

	//Sets Code Segment Descriptor. 4 GiB limit, 0 base, 4 KiB granularity and 32 bit opcodes.
	gdt_set_entry(1, 0xFFFFFFFF, 0, 0x9A, 0xCF);

	//Sets Data Segment Descriptor.  Exact same as above but its a Data Segment.
	gdt_set_entry(2, 0xFFFFFFFF, 0, 0x92, 0xCF);

	gdt_flush();
}
