#ifndef _COMMON_H_
#define _COMMON_H_

#ifdef TARGET_PSX
typedef unsigned long uintptr_t;
typedef long intptr_t;
#ifdef USE_PSYQ
#include <sys/types.h>
#else
typedef unsigned long size_t;
typedef unsigned long u_long;
typedef unsigned short u_short;
typedef unsigned char u_char;
#endif
#define NULL (void *)0
#include "Libs/STDARG.h"
#include "Libs/STRING.h"
#else
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#endif

#include "include_asm.h"
#ifdef USE_PSYQ
#include <libgte.h>
#include <libgpu.h>
#include <libetc.h>
#include <libcd.h>
#include <libspu.h>
#ifndef TARGET_PSX
/* host syntax check: libapi's file I/O prototypes clash with the host libc's */
#define rename psyq_rename
#endif
#include <libapi.h>
#ifndef TARGET_PSX
#undef rename
#endif
#include <libpad.h>
#endif

#include "Game/TYPES.h"
#include "Game/EXTRA.h"
#include "Libs/SDK.h"

#define ARRAY_COUNT(arr) (sizeof(arr) / sizeof(arr[0]))

#define EXTERN extern
#define STATIC

#endif
