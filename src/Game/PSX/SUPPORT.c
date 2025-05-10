#include "Game/PSX/SUPPORT.h"

#define PRT_BUF_SIZE	0x100

// int printf(char *fmt)
int printf(const char *format, ...)
{
    int len;
    char string[PRT_BUF_SIZE];
    va_list ap; // not from decls.h

    va_start(ap, format);

    len = vsprintf(string, format, ap);

    puts(string);

    return len;
}

// int sprintf(char *string, char *fmt)
int sprintf(char *str, const char *format, ...)
{
    va_list ap; // not from decls.h

    va_start(ap, format);

    return vsprintf(str, format, ap);
}

INCLUDE_ASM("asm/nonmatchings/Game/PSX/SUPPORT", vsprintf);

extern char D_800D1A74[];
//char *my_itoa(unsigned long value, char *str, int radix)
char *my_itoa(int value, char *str, int base)
{
    char buf[31 + 1];
    char *pBuf;
    char *tStr;
    unsigned int radix;
    unsigned char v;

    radix = base;
    pBuf = (char *)&buf;
    tStr = str;
    if (value == 0)
    {
        memcpy(str, D_800D1A74, 2);
        return str;
    }

    if (radix > 36 || radix <= 1)
    {
        radix = 10;
    }

    if ((radix == 10) && (value < 0))
    {
        *str = '-';
        tStr = str + 1;
        value = -value;
    }

    while (value != 0)
    {
        *pBuf = value % radix;
        value = (value / radix);
        pBuf++;
    }

    pBuf--;
    while ((uintptr_t)buf <= (uintptr_t)pBuf)
    {
        v = *pBuf;
        if (v < 10)
        {
            *tStr = v + '0';
        }
        else
        {
            *tStr = v + 'a' - 10;
        }
        pBuf--;
        tStr++;
    }

    *tStr = 0;
    return str;
}

// int atoi(char *str)
int atoi(const char *str)
{
    int val;
    int neg;

    val = 0;

    neg = 0;

    if (*str == '-')
    {
        str++;

        neg = 1;
    }

    while (*str != 0)
    {
        val *= 10;
        val += *str - '0';

        str++;
    }

    if (neg != 0)
    {
        return -val;
    }
    else
    {
        return val;
    }
}

int mytolower(int c)
{
    if ((c >= 'A') && (c <= 'Z'))
    {
        c += 32;
    }

    return c;
}

int strcmpi(char *s1, char *s2)
{
    int c1;
    int c2;

    do
    {
        c1 = mytolower(*s1++);
        c2 = mytolower(*s2++);

        if (c1 != c2)
        {
            return c1 - c2;
        }
    } while (c1 != 0);

    return 0;
}
