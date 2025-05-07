#ifndef _VWRAITH_H_
#define _VWRAITH_H_

#include "common.h"

extern MonsterFunctionTable VWRAITH_FunctionTable;

typedef struct Dummy5
{
    short unk0;
    short unk2;
    short unk4;
    short unk6;
    short unk8;
    short unkA;
    short unkC;
    char unkE;
    char unkF;
    char unk10;
} Dummy5;

int VWRAITH_ShouldISwoop(Instance *instance);
void VWRAITH_VerticalMove(Instance *instance);
void VWRAITH_PursueEntry(Instance *instance);
void VWRAITH_CombatEntry(Instance *instance);
void VWRAITH_Combat(Instance *instance);
void VWRAITH_Pursue(Instance *instance);
void VWRAITH_EmbraceEntry(Instance *instance);
void VWRAITH_Embrace(Instance *instance);

#endif
