#ifndef _WALBOSS_H_
#define _WALBOSS_H_

#include "common.h"

// size: 0x24
typedef struct _WalbossVars
{
    // offset: 0x0000 (6 bytes)
    Position correctedLookAt;
    // offset: 0x0006
    short animState;
    // offset: 0x0008
    short lastDirection;
    // offset: 0x000A
    short numEggHits;
    // offset: 0x000C
    int thornTimer;
    // offset: 0x0010
    Instance *thornInst;
    // offset: 0x0014 (6 bytes)
    Position thornTarget;
    // offset: 0x001A
    short thornState;
    // offset: 0x001C
    int tauntTimer;
    // offset: 0x0020
    int tauntState;
} WalbossVars;

void FX_StartInstanceBurrow(Instance *instance, Level *level, TFace *tface);
void WALBOSS_Attack(Instance *instance);
void WALBOSS_AttackEntry(Instance *instance);
void WALBOSS_BirthEgg(Instance *instance);
void WALBOSS_BirthEggEntry(Instance *instance);
int WALBOSS_ChooseAttack(Instance *instance, MonsterIR *enemy);
void WALBOSS_CleanUp(Instance *instance);
void WALBOSS_Collide(Instance *instance, GameTracker *gameTracker);
void WALBOSS_Combat(Instance *instance);
void WALBOSS_CombatEntry(Instance *instance);
void WALBOSS_CorrectLookAT(Instance *instance, Position *lookAtPos);
Instance *WALBOSS_CreateEgg(Instance *parent);
void WALBOSS_DamageEffect(Instance *instance, evFXHitData *data);
void WALBOSS_Dead(Instance *instance);
void WALBOSS_DeadEntry(Instance *instance);
void WALBOSS_DoNothingEntry(Instance *instance);
void WALBOSS_EjectEgg(Instance *parent);
Instance *WALBOSS_FindEgg();
int WALBOSS_HandleFade(Instance *instance);
void WALBOSS_HandleThorn(Instance *instance);
void WALBOSS_Hit(Instance *instance);
void WALBOSS_HitByEgg(Instance *instance);
void WALBOSS_HitByEggEntry(Instance *instance);
void WALBOSS_HitEntry(Instance *instance);
void WALBOSS_Idle(Instance *instance);
void WALBOSS_IdleEntry(Instance *instance);
void WALBOSS_Init(Instance *instance);
void WALBOSS_InterpSegmentYaw(Instance *instance, int yaw);
void WALBOSS_LegHurt(Instance *instance);
void WALBOSS_LegHurtEntry(Instance *instance);
int WALBOSS_LivingLegs();
void WALBOSS_Message(Instance *instance, unsigned long message, unsigned long data);
void WALBOSS_MessageToLegs(int message);
void WALBOSS_Notice(Instance *instance);
void WALBOSS_NoticeEntry(Instance *instance);
unsigned long WALBOSS_Query(Instance *instance, unsigned long query);
void WALBOSS_SetSegmentYaw(Instance *instance, int yaw);
int WALBOSS_ShouldIAttack(Instance *instance, MonsterIR *enemy, int attack);
void WALBOSS_TurnToPosition(Instance *instance, Position *target);
void WALBOSS_DoNothing();

#endif
