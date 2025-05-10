#ifndef _MONLIB_H_
#define _MONLIB_H_

#include "common.h"

typedef struct Dummy6
{
    char pad[16];
    int unknown;
} Dummy6; // not from types.h

unsigned long MON_GetTime(Instance *instance);
void MON_PlayRandomIdle(Instance *instance, int mode);
void MON_TurnOnBodySpheres(Instance *instance);
void MON_PlayAnimFromList(Instance *instance, char *animList, int animtype, int mode);
void MON_SwitchState(Instance *instance, enum MonsterState state);
int MON_TurnToPosition(Instance *instance, Position *position, short turnspeed);
void MON_KillMonster(Instance *instance);
void MON_TurnOffBodySpheres(Instance *instance);
int MON_GetRandomDestinationInWorld(Instance *instance, Position *in, short r);
void MON_StartSpecialFade(Instance *instance, int fadeLevel, int fadeTime);
MonsterAnim *MON_GetAnim(Instance *instance, char *animList, int index);
void MON_PlayAnim(Instance *instance, enum MonsterAnim animtype, int mode);
int MON_AnimPlayingFromList(Instance *instance, char *animList, int animtype);
void MON_ApplyPhysics(Instance *instance);
void MON_BirthMana(Instance *instance);
void MON_DisableHeadMove(Instance *instance);
void MON_EnableHeadMove(Instance *instance);
void MON_CheckEnvironment(Instance *instance);
void MON_PlayAnimID(Instance *instance, int index, int mode);
void MON_TurnOffWeaponSpheres(Instance *instance);
void MON_ProcessLookAt(Instance *instance);
void MON_ProcessSpecialFade(Instance *instance);
void MON_ChangeBehavior(Instance *instance, int behavior);
void MON_ProcessIntro(Instance *instance);
void MON_TurnOnAllSpheres(Instance *instance);
void MON_AnimInit(Instance *instance);
int MON_ValidUnit(Instance *instance, unsigned long unitId);
int MON_ValidPosition(Instance *instance);
Intro *MON_TestForTerrainImpale(Instance *instance, Terrain *terrain);
void MON_CheckTerrainAndRespond(Instance *instance, BSPTree *bsp, TFace *tface);
void MON_SetUpSaveInfo(Instance *instance, MonsterSaveInfo *saveData);
void MON_GetSaveInfo(Instance *instance, MonsterSaveInfo *saveData);
void MON_SwitchStateDoEntry(Instance *instance, enum MonsterState state);
void MON_RelocateCoords(Instance *instance, SVector *offset);
void MON_LookInDirection(Instance *instance, short tx, short tz);
short MON_FacingOffset(Instance *instance, Instance *target);
long MON_AnimCallback(G2Anim *anim, int sectionID, G2AnimCallbackMsg message, long messageDataA, long messageDataB, void *data);
unsigned long MON_CheckTerrain(Instance *instance, BSPTree *bsp, TFace *tface);
void MON_SetDefaults(Instance *instance);
int MON_SetVelocityTowardsImpalingObject(Instance *instance, int checkOrientation);
void MON_TurnOffSphereCollisions(Instance *instance);
void MON_TurnOnSphereCollisions(Instance *instance);
void MON_TurnOffAllSpheres(Instance *instance);
void MON_BirthSoul(Instance *instance, int link);
void MON_SoulSucked(Instance *instance);
void MON_ChangeHumanOpinion(Instance *instance);
void MON_DropAllObjects(Instance *instance);
int MON_AnimPlaying(Instance *instance, enum MonsterAnim animtype);
void MON_PlayCombatIdle(Instance *instance, int mode);
int MON_ShouldIFlee(Instance *instance);
void MON_GetPlanSlot(MonsterVars *mv);
int MON_DefaultPlanMovement(Instance *instance, int anim, long distance);
int MON_ShouldIAmbushEnemy(Instance *instance);
void MON_UnlinkFromRaziel(Instance *instance);
int MON_SetUpKnockBack(Instance *instance, Instance *enemy, evMonsterHitData *data);
int MON_OnGround(Instance *instance);
void MON_DoDrainEffects(Instance *instance, Instance *ei);
int MON_ShouldIAttackInstance(Instance *instance, Instance *ei);
int MON_CheckConditions(Instance *instance, MonsterIR *mir, char *probArray);
int MON_CheckPointSuitability(Instance *instance, Position *origin, Position *destination);
int MON_TakeDamage(Instance *instance, int damage, int type);
void MON_MoveInstanceToImpalePoint(Instance *instance);
void MON_SetFXHitData(Instance *instance, evFXHitData *data, int type, int amount);
void MON_LaunchMonster(Instance *instance, int zDirection, int power, int loft);
void MON_MoveForward(Instance *instance);
void MON_TurnOffWeaponSphere(Instance *instance, int segment);
void MON_TurnOnWeaponSphere(Instance *instance, int segment);
void MON_BurnInAir(Instance *instance, int currentState);
int MON_TransNodeAnimation(Instance *instance);
int MON_ReachableIntro(Instance *instance, Position *pos, Position *introPos, Rotation *introRot, int checkOrientation);

#endif
