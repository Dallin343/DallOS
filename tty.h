#include "sys.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

size_t strlen(const char* str);
void terminal_initialize(void);
void update_cursor(int x, int y);
void terminal_setcolor(uint8_t color);
void terminal_shiftlineup(void);
void terminal_delentryat(size_t x, size_t y);
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);
void terminal_putchar(char c);
void terminal_write (const char* data, size_t size);
void terminal_writestring(const char* data);
