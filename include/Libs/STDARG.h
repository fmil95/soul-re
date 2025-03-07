#ifndef _STDARG_H_
#define _STDARG_H_

#define __va_rounded_size(TYPE) \
    (((sizeof(TYPE) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))

#define va_start(AP, LASTARG) \
    (AP = ((char *)&(LASTARG) + __va_rounded_size(LASTARG)))

#define va_end(AP) AP = (char *)NULL

#define va_arg(AP, TYPE) \
    (AP = ((char *)(AP)) += __va_rounded_size(TYPE), \
     *((TYPE *)((char *)(AP) - __va_rounded_size(TYPE))))

typedef char *va_list;

#endif
