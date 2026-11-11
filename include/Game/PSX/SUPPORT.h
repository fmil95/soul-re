#ifndef _SUPPORT_H_
#define _SUPPORT_H_

#include "common.h"

// TODO: the prototypes here don't really coincide with those on the SYM file, make them more accurate

int printf(const char *format, ...);
int sprintf(char *string, const char *fmt, ...);
int vsprintf(char *str, const char *format, va_list arg);
char *my_itoa(int value, char *str, int base);
int atoi(const char *str);
int mytolower(int c);
int strcmpi(char *s1, char *s2);

#endif
