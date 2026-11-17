#ifndef _MBMISS_H_
#define _MBMISS_H_

#include "common.h"

typedef struct Dummy {
    char pad[40];
    int unknown;
} Dummy; // not from types.h

typedef struct Dummy2 {
    int pad;
    int unknown;
} Dummy2; // not from types.h

void WCBEGG_CommonPostProcess2(Instance *instance, GameTracker *gameTracker);
void WCBEGG_Process(Instance *instance, GameTracker *gameTracker);
void WCBEGG_ExplodeCollide(Instance *instance, GameTracker *gameTracker);
void WCBEGG_ExplodeProcess(Instance *instance, GameTracker *gameTracker);
void WCBEGG_SplitProcess(Instance *instance, GameTracker *gameTracker);
void WCBEGG_CommonPostProcess(Instance *instance, GameTracker *gameTracker);
void WCBEGG_Collide(Instance *instance, GameTracker *gameTracker);
void WCBEGG_Message(Instance *instance, unsigned long message, unsigned long data);
long WALBOSC_AnimCallback(G2Anim *anim, int sectionID, G2AnimCallbackMsg message, long messageDataA, long messageDataB, Instance *instance);
void WALBOSC_Collide(Instance *instance, GameTracker *gameTracker);

#endif
