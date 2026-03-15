#ifndef _KAIN_H_
#define _KAIN_H_

#include "common.h"

enum KainTeleportState
{
    K_NORMAL = 0,
    K_PHASING_OUT = 1,
    K_TELEPORT_PAUSE = 2,
    K_TELEPORT_HOLD = 3,
    K_MOVE_DONE = 4,
    K_PHASING_IN = 5,
    K_NO_TELEPORT = 6,
};

// size: 0x38
typedef struct _KainVars
{
    // offset: 0x0000
    short numHits;
    // offset: 0x0002
    short teleportState;
    // offset: 0x0004
    int timer;
    // offset: 0x0008
    int beamTimer;
    // offset: 0x000C
    FXForceFieldEffect *field;
    // offset: 0x0010
    FXLightning *zap[3];
    // offset: 0x001C
    Instance *zapTarget;
    // offset: 0x0020
    char hitSegment[3];
    // offset: 0x0023
    char numBeams;
    // offset: 0x0024
    char changeBeam;
    // offset: 0x0025
    char tier;
    // offset: 0x0026
    int soundHandle;
    // offset: 0x002A
    Position beamTarget;
    // offset: 0x0030
    Position teleportTarget;
} KainVars;

// size: 0x1E
typedef struct _kainAttributes
{
    // offset: 0x0000
    short maxHits;
    // offset: 0x0002
    short timeToFadeOut;
    // offset: 0x0004
    short timeFadePause;
    // offset: 0x0006
    short timeToFadeIn;
    // offset: 0x0008
    short attackLength;
    // offset: 0x000A
    short timeToHit;
    // offset: 0x000C
    short handHitDist;
    // offset: 0x000E
    short outsideOfRoom;
    // offset: 0x0010
    short maxBoltDist;
    // offset: 0x0012
    short timeToBolt[6];
} KainAttributes;

void KAIN_Init(Instance *instance);
void KAIN_CleanUp(Instance *instance);
void KAIN_DamageEffect(Instance *instance, evFXHitData *data);
unsigned long KAIN_Query(Instance *instance, unsigned long query);
void KAIN_PursueEntry(Instance *instance);
void KAIN_Pursue(Instance *instance);
void KAIN_CombatEntry(Instance *instance);
void KAIN_Combat(Instance *instance);
void KAIN_IdleEntry(Instance *instance);
void KAIN_Idle(Instance *instance);
void KAIN_AttackEntry(Instance *instance);
void KAIN_Attack(Instance *instance);
void KAIN_HitEntry(Instance *instance);
void KAIN_Hit(Instance *instance);
void KAIN_DoNothingEntry(Instance *instance);
void KAIN_DoNothing(Instance *instance);
void KAIN_Message(Instance *instance, unsigned long message, unsigned long data);

#endif
