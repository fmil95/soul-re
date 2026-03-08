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

// size: 0x50
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
    // offset: 0x0010
    struct _FXForceFieldEffect *field;
    // offset: 0x0018
    struct _FXLightning *zap[3];
    // offset: 0x0030
    struct _Instance *zapTarget;
    // offset: 0x0038
    char hitSegment[3];
    // offset: 0x003B
    char numBeams;
    // offset: 0x003C
    char changeBeam;
    // offset: 0x003D
    char tier;
    // offset: 0x0040
    int soundHandle;
    // offset: 0x0044
    struct _Position beamTarget;
    // offset: 0x004A
    struct _Position teleportTarget;
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

#endif
