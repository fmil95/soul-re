#ifndef _SUPPORT_H_
#define _SUPPORT_H_

#include "common.h"

int strcmpi(char *s1, char *s2);
#ifdef TARGET_PSX
int sprintf(char *string, const char *fmt, ...);
#endif

#endif
