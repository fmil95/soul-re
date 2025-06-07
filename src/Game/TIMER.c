#include "Game/TIMER.h"
#include "Game/HASM.h"

long gTimerEnabled = 0;

unsigned long TIMER_GetTimeMS()
{
    unsigned long ticks;
    unsigned long mticks;

    EnterCriticalSection();

    ticks = GetRCnt(0xF2000000) & 0xFFFF;

    mticks = gameTimer;

    ExitCriticalSection();

    return ((mticks >> 16) * 126819) + ((ticks | mticks << 16) / 33869);
}

unsigned long TIMER_TimeDiff(unsigned long x)
{
    unsigned long intrs;
    unsigned long ticks;
    unsigned long prevIntrs;
    unsigned long prevTicks;
    // unsigned long diffIntrs; // unused
    unsigned long diffTicks;
    unsigned long timeDiff;

    ticks = GetRCnt(0xF2000000) & 0xFFFF;

    prevIntrs = x >> 16;

    intrs = gameTimer;

    intrs &= 0xFFFF;

    prevTicks = x & 0xFFFF;

    if (intrs < prevIntrs)
    {
        intrs -= prevIntrs - 65536;
    }
    else
    {
        intrs -= prevIntrs;
    }

    if (ticks < prevTicks)
    {
        diffTicks = ticks - (prevTicks - 0xFFFF);

        intrs--;
    }
    else
    {
        diffTicks = ticks - prevTicks;
    }

    if (intrs >= 2259)
    {
        timeDiff = 4293263;
    }
    else
    {
        timeDiff = ((diffTicks * 29) + (intrs * 1900515)) / 1000;
    }

    if (gTimerEnabled == 0)
    {
        timeDiff = 0;
    }

    return timeDiff;
}
