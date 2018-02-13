#ifndef __SYS_H
#define __SYS_H
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* MAIN.C */
extern uint8_t *memcpy(uint8_t *dest, const uint8_t *src, int count);
extern uint8_t *memset(uint8_t *dest, uint8_t val, int count);
extern uint16_t *memsetw(uint16_t *dest, uint16_t val, int count);
extern size_t strlen(const char *str);
extern uint8_t inb (uint16_t port);
extern void outb (uint16_t port, uint8_t val);
extern void terminal_putchar(char c);
extern void terminal_writestring(const char* data);

//GDT Functions
extern void gdt_install();

//IDT Functions

#endif
