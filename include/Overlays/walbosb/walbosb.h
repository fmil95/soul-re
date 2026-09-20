#ifndef _WALBOSB_H_
#define _WALBOSB_H_

#include "common.h"

// size: 0x10
typedef struct _WalbosbVars
{
    // offset: 0x0000
    int tauntTimer;
    // offset: 0x0004
    short tauntState;
    // offset: 0x0006
    short pitch;
    // offset: 0x0008
    short abortedAttacks;
    // offset: 0x000A
    short junk;
    // offset: 0x000C
    Instance *autofaceInst;
} WalbosbVars;

// size: 0x14
typedef struct _walbosbAttributes {
    // offset: 0x0000
    short maxYawAngle;
    // offset: 0x0002
    short maxPitchAngle;
    // offset: 0x0004
    short lengthOfTense;
    // offset: 0x0006
    short lengthOfStuck;
    // offset: 0x0008
    short lengthOfStrikeTrack;
    // offset: 0x000A
    short lengthOfStrikeWait;
    // offset: 0x000C
    short allowedAbortedAttacks;
    // offset: 0x000E
    short junk;
    // offset: 0x0010
    char attackDeltas[3];
} WalbosbAttributes;

void WALBOSB_IdleEntry(Instance *instance);
void WALBOSB_Idle(Instance *instance);
void WALBOSB_CombatEntry(Instance *instance);
void WALBOSB_Combat(Instance *instance);
void WALBOSB_AttackEntry(Instance *instance);
void WALBOSB_Attack(Instance *instance);
void WALBOSB_HitEntry(Instance *instance);
void WALBOSB_Hit(Instance *instance);
void WALBOSB_HitEntry(Instance *instance);
void WALBOSB_Hit(Instance *instance);
void WALBOSB_DoNothingEntry(Instance *instance);
void WALBOSB_DoNothing(Instance *instance);
void WALBOSB_DoNothingEntry(Instance *instance);
void WALBOSB_DoNothing(Instance *instance);
void WALBOSB_DoNothingEntry(Instance *instance);
void WALBOSB_DoNothing(Instance *instance);
void WALBOSB_DoNothingEntry(Instance *instance);
void WALBOSB_DoNothing(Instance *instance);
void WALBOSB_DoNothingEntry(Instance *instance);
void WALBOSB_DoNothing(Instance *instance);
void WALBOSB_GeneralDeathEntry(Instance *instance);
void WALBOSB_GeneralDeath(Instance *instance);
void WALBOSB_DeadEntry(Instance *instance);
void WALBOSB_Dead(Instance *instance);

#endif
