#ifndef _HUMAN_H_
#define _HUMAN_H_

#include "common.h"

Instance *HUMAN_CreateWeapon(Instance *instance, int weaponid, int segment);
void HUMAN_Init(Instance *instance);
int HUMAN_TypeOfHuman(Instance *instance);
void HUMAN_StunnedEntry(Instance *instance);
void HUMAN_Stunned(Instance *instance);
void HUMAN_DeadEntry(Instance *instance);
void HUMAN_Dead(Instance *instance);
void HUMAN_EmbraceEntry(Instance *instance);
void HUMAN_Embrace(Instance *instance);
void HUMAN_CleanUp(Instance *instance);
uintptr_t HUMAN_Query(Instance *instance, unsigned long query);
void HUMAN_IdleEntry(Instance *instance);
void HUMAN_Idle(Instance *instance);
void HUMAN_Flee(Instance *instance);
void HUMAN_GetAngry();

extern MonsterFunctionTable HUMAN_FunctionTable;
extern MonsterStateChoice HUMAN_StateChoiceTable[];

#endif
