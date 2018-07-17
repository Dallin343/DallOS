#include "sys.h"
#include "tty.h"

struct idt_entry {
	uint16_t base_lo;
	uint16_t selector;
	uint8_t always0;
	uint8_t flags;
	uint16_t base_hi;
}__attribute__((packed));

struct idt_pointer {
	uint16_t limit;
	uint32_t base;
}__attribute__((packed));

struct idt_entry idt[256];
struct idt_pointer idt_p;

void idt_set_entry(uint8_t num, uint32_t base, uint16_t selector, uint8_t flags);
extern void idt_flush();

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void isr32();
extern void isr33();
extern void isr34();
void getcharacter();

unsigned char keycode[128] =
{
	0, 27, '1', '2', '3', '4', '5', '6', '7', '8',
	'9', '0', '-', '=', '\b',
	'\t',
	'q', 'w', 'e', 'r',
	't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
	0,
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
	'\'', '`', 0,
	'\\', 'z', 'x', 'c', 'v', 'b', 'n',
	'm', ',', '.', '/', 0,
	'*',
	0,
	' ',
	0,
	0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0,
	0,
	0,
	0,
	0,
	0,
	'-',
	0,
	0,
	0,
	'+',
	0,
	0,
	0,
	0,
	0,
	0, 0, 0,
	0,
	0,
	0,
};


void isr_install()
{
	idt_set_entry(0, (uint32_t)isr0, 0x08, 0x8E);
	idt_set_entry(1, (uint32_t)isr1, 0x08, 0x8E);
	idt_set_entry(2, (uint32_t)isr2, 0x08, 0x8E);
	idt_set_entry(3, (uint32_t)isr3, 0x08, 0x8E);
	idt_set_entry(4, (uint32_t)isr4, 0x08, 0x8E);
	idt_set_entry(5, (uint32_t)isr5, 0x08, 0x8E);
	idt_set_entry(6, (unsigned)isr6, 0x08, 0x8E);
	idt_set_entry(7, (unsigned)isr7, 0x08, 0x8E);
	idt_set_entry(8, (unsigned)isr8, 0x08, 0x8E);
	idt_set_entry(9, (unsigned)isr9, 0x08, 0x8E);
	idt_set_entry(10, (unsigned)isr10, 0x08, 0x8E);
	idt_set_entry(11, (unsigned)isr11, 0x08, 0x8E);
	idt_set_entry(12, (unsigned)isr12, 0x08, 0x8E);
	idt_set_entry(13, (unsigned)isr13, 0x08, 0x8E);
	idt_set_entry(14, (unsigned)isr14, 0x08, 0x8E);
	idt_set_entry(15, (unsigned)isr15, 0x08, 0x8E);
	idt_set_entry(16, (unsigned)isr16, 0x08, 0x8E);
	idt_set_entry(17, (unsigned)isr17, 0x08, 0x8E);
	idt_set_entry(18, (unsigned)isr18, 0x08, 0x8E);
	idt_set_entry(19, (unsigned)isr19, 0x08, 0x8E);
	idt_set_entry(20, (unsigned)isr20, 0x08, 0x8E);
	idt_set_entry(21, (unsigned)isr21, 0x08, 0x8E);
	idt_set_entry(22, (unsigned)isr22, 0x08, 0x8E);
	idt_set_entry(23, (unsigned)isr23, 0x08, 0x8E);
	idt_set_entry(24, (unsigned)isr24, 0x08, 0x8E);
	idt_set_entry(25, (unsigned)isr25, 0x08, 0x8E);
	idt_set_entry(26, (unsigned)isr26, 0x08, 0x8E);
	idt_set_entry(27, (unsigned)isr27, 0x08, 0x8E);
	idt_set_entry(28, (unsigned)isr28, 0x08, 0x8E);
	idt_set_entry(29, (unsigned)isr29, 0x08, 0x8E);
	idt_set_entry(30, (unsigned)isr30, 0x08, 0x8E);
	idt_set_entry(31, (unsigned)isr31, 0x08, 0x8E);
	idt_set_entry(32, (unsigned)isr32, 0x08, 0x8E);
	idt_set_entry(33, (unsigned)isr33, 0x08, 0x8E);
	idt_set_entry(34, (unsigned)isr34, 0x08, 0x8E);
}

void fault_handler(struct regs *r)
{
	if (r->int_no < 32) {
		terminal_writestring("Caught Exception\nHalting...\n");
		for (;;);
	}
	else if (r->int_no >= 32) {
		if (r->int_no == 33) {
			//Keyboard
			getcharacter();
			outb(0x20, 0x20);
		}
		else {
			outb(0x20, 0x20);
		}
	}
}

void getcharacter() {
	//char c = 0;
  //do {
  	unsigned char scancode = inb(0x60);
		if (!(scancode & 0x80)) {
			terminal_putchar(keycode[scancode]);
			return;
		}
//  } while(1);
}

void idt_set_entry(uint8_t num, uint32_t base, uint16_t selector, uint8_t flags)
{
	idt[num].base_lo = base & 0xFFFF;

	idt[num].selector = selector;
	idt[num].always0 = 0x00;
	idt[num].flags = flags;
	idt[num].base_hi = (base >> 16) & 0xFFFF;
}

void idt_install(void)
{
	idt_p.base = (uint32_t)&idt;
	idt_p.limit = ((sizeof(struct idt_entry) * 256) - 1);

	memset(&idt, 0, sizeof(struct idt_entry) * 256);

	isr_install();

	idt_flush();
}
