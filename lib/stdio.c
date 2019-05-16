#include "stdio.h"

int k_printf(const char* format, ...) {
    
    va_list args;
    char str[100];
    va_start(args, format);
    int numChars = k_vsnprintf(str, (size_t)100, format, args);
    va_end(args);
    terminal_write(str, numChars);
    return numChars;
}

int k_vsnprintf(char *str, size_t size, const char *format, va_list args) {
    int i = 0;
    
    while (*format) {
        if (*format != '%') {
            str[i++] = *format;
            format++;
            continue;
        } else {
            format++;
        }
        
        switch(*format) {
            case 'c':
                str[i++] = (char)va_arg(args, int);
                break;
            case 'd':
                i = k_itoa(va_arg(args, int), i, str, 10);
                break;
            case 'e':
                break;
            case 'f':
                break;
            case 'i':
                i = k_itoa(va_arg(args, int), i, str, 10);
                break;
            case 'o':
                i = k_itoa(va_arg(args, int), i, str, 8);
                break;
            case 's': {
                const char *s = va_arg(args, char *);
                while ((*s != 0)) {
                    str[i++] = *(s++);
                }
                break;
            }
            
            case 'u':
                break;
            case 'x':
                i = k_itoa(va_arg(args, int), i, str, 16);
                break;
            case '%':
                str[i++] = '%';
                break;
            default:
                break;
        }
        format++;
    }
    str[i] = '\0';
    return i;
}

int k_itoa(int num, int idx, char *str, int base) {
    bool isNeg = false;
    int i = idx;
    
    if (num == 0) {
        str[i++] = '0';
        return i;
    }
    
    if (num < 0 && base == 10) {
        isNeg = true;
        num = -num;
    }
    
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem-10) + 'a' : rem + '0';
        num /= base;
    }
    
    if (isNeg) {
        str[i++] = '-';
    }
    reverse(str, idx, i-1);
    
    return i;
}

char* reverse(char *buffer, int i, int j) {
    if (i == j)
        return buffer;
    
    while (i < j) {
        swap(&buffer[i++], &buffer[j--]);
    }
    return buffer;
}

void swap(char *x, char *y) {
    char t = *x; *x = *y; *y = t;
}
