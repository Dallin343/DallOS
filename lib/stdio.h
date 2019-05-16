#include <stdarg.h>
#include <stddef.h>
#include <stdbool.h>
#include "../includes/tty.h"

extern int k_printf(const char* format, ...);
extern int k_vsnprintf(char *str, size_t size, const char *format, va_list args);
extern void swap(char *x, char *y);
extern char* reverse(char *buffer, int i, int j);
extern int k_itoa(int num, int idx, char *str, int base);