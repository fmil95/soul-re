#ifndef _PHYSOBS_H_
#define _PHYSOBS_H_

#include "common.h"

evObjectBirthProjectileData *PHYSOB_BirthProjectile(Instance *parent, int joint, int type);
void CollidePhysicalObject(Instance *instance, GameTracker *gameTracker);
void ProcessPhysicalObject(Instance *instance, GameTracker *gameTracker);
void PhysicalObjectPost(Instance *instance, unsigned long Message, unsigned long Data);
void TurnOffCollisionPhysOb(Instance *instance, int coll);
void TurnOnCollisionPhysOb(Instance *instance, int coll);
Instance *PHYSOB_BirthCollectible(Instance *parent, int x, int y, int z, int type, int lifeTime);
int CheckPhysOb(Instance *instance);
int CheckPhysObFamily(Instance *instance, unsigned short family);
void PHYSOB_NormalToAxis(short *x, short *y);
int PHYSOB_CheckSlide2(Instance *instance, int x, int y, evPhysicsSlideData **data);
int PHYSOB_CheckDroppedLineCollision(Instance *instance, Instance *parent);
int PHYSOB_ReAlignFalling(Instance *instance, int zEndOff);
int CheckPhysObAbility(Instance *instance, unsigned short ability);
void ResetOrientation(Instance *instance);
void PhysOb_AlignPush(Instance *instance, int x, int y, int path, PhysObData *Data);
Instance *PHYSOBS_IsAPushBlockAttached(Instance *block);
int PHYSOB_CheckDirectedLineCollision(Instance *instance, int xoffset, int yoffset, int startZOffset);
void FinishPush(Instance *instance);
PhysObWeaponAttributes *PhysObGetWeapon(Instance *instance);
PhysObLight *PhysObGetLight(Instance *instance);
int PushPhysOb(Instance *instance, short x, short y, short PathNumber, Instance *Force);
int FlipPhysOb(Instance *instance, short x, short y, Instance *Force);
int PickUpPhysOb(Instance *instance, short Steps, Instance *Force, int LinkNode);
void ExecuteGravitate(Instance *instance);
int CanBePickedUp(Instance *instance, Instance *Force, int LinkNode);
void StopPhysOb(Instance *instance);
void DropPhysOb(Instance *instance, int flags);
void ThrowPhysOb(Instance *instance, evObjectThrowData *throwData);
int SwitchPhysOb(Instance *instance);
void ResetSwitchPhysOb(Instance *instance);
int InteractPhysOb(Instance *instance, Instance *Force, int LinkNode, int Action);
void PHYSOB_EndBurning(Instance *instance, PhysObLight *pLight);
void PHYSOB_StopBurning(Instance *instance, PhysObLight *pLight);
void PHYSOB_StartBurning(Instance *instance, PhysObLight *pLight);
long PHYSOBS_CheckForValidMove(Instance *instance);
void PHYSOB_StartLighting(Instance *instance, PhysObLight *pLight);
Instance *BirthProjectilePhysOb(Instance *instance, int grabJoint, int type);
void PHYSOB_SetLightTable(PhysObLight *pLight, LightInstance *li, short burnAmplitude);
long PhysobAnimCallback(G2Anim *anim, int sectionID, G2AnimCallbackMsg message, long messageDataA, long messageDataB, void *data);
void ExecuteFlip(Instance *instance);
void ExecuteDrag(Instance *instance);
void ExecuteSlideToStop(Instance *instance);
void ExecuteThrow(Instance *instance);
void ExecuteFollow(Instance *instance);
int PHYSOB_CheckSlide(Instance *instance, int x, int y, evPhysicsSlideData **data);
int PHYSOB_CheckDropOnSlope(Instance *instance, int x, int y, evPhysicsSlideData **data);
int PHYSOBS_CheckObjectAxisAlignment(MATRIX *m0, MATRIX *m1, SVector *axis);
PhysObSplinter *PhysObGetSplinter(Instance *instance);
int PHYSOB_CheckSlideNormal(Instance *instance, int x, int y, evPhysicsSlideData **data);

#endif
