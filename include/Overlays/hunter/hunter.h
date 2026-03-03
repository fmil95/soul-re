#ifndef _HUNTER_H_
#define _HUNTER_H_

#include "common.h"

// size: 0x2C
typedef struct _HunterVars {
    // offset: 0x0000
    short startIdx;
    // offset: 0x0002
    short endIdx;
    // offset: 0x0004
    int rem_time;
    // offset: 0x0008
    int timer;
    // offset: 0x000C
    short angles[16];
} HunterVars;

// size: 4
typedef struct _HunterAttributes
{
    // offset: 0x0000
    int timeToFire;
} HunterAttributes;
#endif
