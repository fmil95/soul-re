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

// Near match, but has $s1 and $s3 registers swapped (98.17%)
#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/Game/PSX/SUPPORT", vsprintf);
#else 
// int vsprintf(char *str, char *format, void *arg)
int vsprintf(char *str, const char *format, va_list arg)
{
    int scopy;     
    const char *fmt; 

    scopy = 0;
    
    for (fmt = format; *fmt != '\0'; ) 
    {
        int fsize;      
        int pad;       
        int left_just;  
        int ts_len;     
        char *temp_str;
        char buf[16];   

        if (*fmt != '%') 
        {
            str[scopy++] = *fmt++;
            continue;
        } 
        
        fmt++;

        if (*fmt == '%') 
        {
            str[scopy++] = '%';
            
            fmt++;
            continue;
        }
        
        if (*fmt == '-') 
        {
            fmt++;
            
            left_just = 1;
        } 
        else 
        {
            left_just = 0;
        }
        
        if (*fmt == '0')
        {
            pad = '0';
            
            fmt++;
        }
        else 
        {
            pad = ' ';
        }
        
        fsize = 0;
        
        while ((*fmt >= '0') && (*fmt <= '9')) 
        {
            fsize = ((fsize * 10) + *fmt++) - '0';
        }

        while ((*fmt == 'N') || (*fmt == 'F') || (*fmt == 'h') || (*fmt == 'l') || (*fmt == 'L')) 
        {
            fmt++;
        }
        
        if (*fmt == 'c') 
        {
            temp_str = buf;
            
            buf[0] = va_arg(arg, char);
            buf[1] = 0;
        } 
        else if (*fmt == 's') 
        {
            temp_str = va_arg(arg, char*);
        } 
        else if ((*fmt == 'x') || (*fmt == 'X')) 
        {
            temp_str = my_itoa(va_arg(arg, int), buf, 16);
        } 
        else 
        {
            temp_str = my_itoa(va_arg(arg, int), buf, 10);
        }

        fmt++;
        
        ts_len = strlen(temp_str);
        
        if ((fsize != 0) && (ts_len < fsize)) 
        {
            if (left_just != 0) 
            {
                strcpy(&str[scopy], temp_str);
                scopy += ts_len;
                
                memset(&str[scopy], pad, fsize - ts_len);
                scopy += fsize - ts_len;
                
                continue;
            } 
            else
            {
                memset(&str[scopy], pad, fsize - ts_len);
                scopy += fsize - ts_len;
            }
        } 

        strcpy(&str[scopy], temp_str);
        scopy += ts_len;
    }
    
    str[scopy] = 0;
    
    return scopy;
}
#endif

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
        memcpy(str, "0", 2);
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
