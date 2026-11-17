#ifndef _WALLCR_H_
#define _WALLCR_H_

#include "common.h"

// size: 0x4
typedef struct _WallcrAttributes
{
    // offset: 0x0000
    int surprise_attack_time;
} WallcrAttributes;

// size: 0x14
typedef struct _WallcrVars
{
    // offset: 0x0000
    short yaw_speed;
    // offset: 0x0002
    short pitch_speed;
    // offset: 0x0004
    short roll_speed;
    // offset: 0x0006
    short roll;
    // offset: 0x0008
    int correct_roll_speed;
    // offset: 0x000C
    int time;
    // offset: 0x0010
    short left_over_time;
    // offset: 0x0012
    short wallcrFlags;
} WallcrVars;

int WALLCR_AngleFromPosToPos(Position *from, Position *to, Rotation *rot);
void WALLCR_ApplySpin(Instance *instance, int timeMult);
void WALLCR_Attach(Instance *instance);
void WALLCR_AttachEntry(Instance *instance, Position *point, Normal *norm);
void WALLCR_CleanUp(Instance *instance);
void *WALLCR_CollideFace(Level *level, Position *start, Position *end, Position *hit, Normal *norm);
void WALLCR_Detach(Instance *instance);
void WALLCR_DetachEntry(Instance *instance, Position *position);
void WALLCR_DisableControllers(Instance *instance);
void WALLCR_EnableControllers(Instance *instance);
void WALLCR_Fall(Instance *instance);
void WALLCR_FallEntry(Instance *instance);
int WALLCR_FindJumpOnPoint(Instance *instance, Position *point, Normal *norm);
int WALLCR_FollowWall(Instance *instance);
void WALLCR_GeneralDeath(Instance *instance);
void WALLCR_GeneralDeathEntry(Instance *instance);
void WALLCR_Hit(Instance *instance);
void WALLCR_HitEntry(Instance *instance);
void WALLCR_Idle(Instance *instance);
void WALLCR_IdleEntry(Instance *instance);
void WALLCR_Impact(Instance *instance);
void WALLCR_ImpactEntry(Instance *instance);
void WALLCR_Init(Instance *instance);
void WALLCR_Message(Instance *instance, unsigned long message, unsigned long data);
void WALLCR_MissileHit(Instance *instance);
void WALLCR_MissileHitEntry(Instance *instance);
int WALLCR_MoveForward(Instance *instance, Position *pos);
int WALLCR_MoveToDestination(Instance *instance, Position *target);
int WALLCR_NearFloor(Instance *instance);
void WALLCR_Notice(Instance *instance);
void WALLCR_NoticeEntry(Instance *instance);
void WALLCR_Pupate(Instance *instance);
void WALLCR_PupateEntry(Instance *instance);
void WALLCR_Pursue(Instance *instance);
void WALLCR_PursueEntry(Instance *instance);
unsigned long WALLCR_Query(Instance *instance, unsigned long query);
int WALLCR_SeekPath(Instance *instance, Position *pos, Position *targetPos, int targetType, int validNodes);
void WALLCR_SetFlex(Instance *instance, short flex);
int WALLCR_ShouldIAmbushPlayer(Instance *instance);
int WALLCR_ShouldJumpOff(Instance *instance);
int WALLCR_ShouldJumpOn(Instance *instance);
int WALLCR_ShouldSurpriseAttack(Instance *instance);
void WALLCR_Stunned(Instance *instance);
void WALLCR_StunnedEntry(Instance *instance);
void WALLCR_SurpriseAttack(Instance *instance);
void WALLCR_SurpriseAttackEntry(Instance *instance);
void WALLCR_Surprised(Instance *instance);
void WALLCR_SurprisedEntry(Instance *instance);
void WALLCR_Thrown(Instance *instance);
void WALLCR_ThrownEntry(Instance *instance);
void WALLCR_Wander(Instance *instance);
void WALLCR_WanderEntry(Instance *instance);

#endif
