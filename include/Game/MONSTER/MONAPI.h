#ifndef _MONAPI_H_
#define _MONAPI_H_

#include "common.h"

typedef struct Dummy3
{
    char pad[24];
    int unknown;
} Dummy3; // not from types.h

typedef struct Dummy4
{
    char pad[12];
    int unknown;
    int unknown2;
} Dummy4; // not from types.h

typedef void (*MONTABLE_DamageEffectFunc)(Instance *, int);

void MonsterProcess(Instance *instance, GameTracker *gameTracker);
uintptr_t MonsterQuery(Instance *instance, unsigned long query);
void MONAPI_AddToGenerator(Instance *instance);
void MONAPI_ResetLookAround(Instance *instance);
void MONAPI_SetLookAround(Instance *instance);
long MONAPI_OkToLookAround(Instance *instance);
void MONAPI_TurnHead(Instance *instance, short *rotx, short *rotz, GameTracker *gameTracker);
void MonsterMessage(Instance *instance, unsigned long message, unsigned long data);
void MONAPI_CheckGenerator(Instance *instance);
void MonsterTranslateAnim(Object *object);
void MONAPI_ProcessGenerator();
void MonsterInit(Instance *instance, GameTracker *gameTracker);
void MonsterCollide(Instance *instance, GameTracker *gameTracker);
void MonsterAdditionalCollide(Instance *instance, GameTracker *gameTracker);
void MonsterRelocateTune(Object *object, long offset);
void MonsterRelocateInstanceObject(Instance *instance, long offset);

#endif
