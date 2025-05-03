#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include "common.h"

void PHYSICS_CheckLineInWorldMask(Instance *instance, PCollideInfo *pcollideInfo);
void PhysicsMove(Instance *instance, Position *position, long time);
void PhysicsSetVelFromRot(Instance *instance, Rotation *rot, long magnitude);
int PhysicsCheckGravity(Instance *instance, intptr_t Data, short Mode);
int PhysicsCheckSwim(Instance *instance, intptr_t Data, short Mode);
int PhysicsCheckDropHeight(Instance *instance, intptr_t Data, int Mode);
void PhysicsSetVelFromZRot(Instance *instance, short angle, long magnitude);
void PhysicsDefaultLinkedMoveResponse(Instance *instance, evPhysicsLinkedMoveData *Data, int updateTransforms);
void PhysicsDefaultGravityResponse(Instance *instance, evPhysicsGravityData *Data);
void PhysicsMoveLocalZClamp(Instance *instance, long segment, long time, long clamp);
void PHYSICS_StopIfCloseToTarget(Instance *instance, int x, int y, int z);
long PHYSICS_FindAFromDAndT(long d, long t);
long PHYSICS_FindVFromAAndD(long a, long d);
void PHYSICS_SetVAndAFromRot(Instance *instance, Rotation *rot, long v, long a);
void PHYSICS_CheckLineInWorld(Instance *instance, PCollideInfo *pcollideInfo);
int PHYSICS_CheckFaceStick(PCollideInfo *CInfo);
void PHYSICS_GenericLineCheck(Instance *instance, MATRIX *transMat, MATRIX *rotMat, PCollideInfo *cInfo);
void PHYSICS_GenericLineCheckMask(Instance *instance, MATRIX *transMat, MATRIX *rotMat, PCollideInfo *cInfo);
int PhysicsCheckSliding(Instance *instance, intptr_t Data, short Mode);
int PhysicsCheckLOS(Instance *instance, intptr_t Data, int Mode);
int PhysicsDefaultCheckSwimResponse(Instance *instance, evPhysicsSwimData *Data);
void PHYSICS_GenericLineCheckSetup(short x, short y, short z, SVECTOR *inVec);
int PHYSICS_CheckForTerrainCollide(Instance *instance, SVECTOR *startVec, SVECTOR *endVec, int segment);
int PhysicsUpdateTface(Instance *instance, intptr_t Data);
void PhysicsForceSetWater(Instance *instance, int *Time, int Depth, int rate, int maxAmplitude);
int PHYSICS_CheckDontGrabEdge(PCollideInfo *CInfo);
void PhysicsDefaultEdgeGrabResponse(Instance *instance, evPhysicsEdgeData *Data, int blockFlag);
int PHYSICS_CheckForValidMove(Instance *instance, SVECTOR *startVec, SVECTOR *endVec, int segment);
int PhysicsCheckEdgeGrabbing(Instance *instance, GameTracker *gameTracker, intptr_t Data, short Mode);
int PHYSICS_CheckForObjectCollide(Instance *instance, SVECTOR *startVec, SVECTOR *endVec, int segment);
int PhysicsCheckDropOff(Instance *instance, intptr_t Data, short Mode);
int PhysicsCheckBlockers(Instance *instance, GameTracker *gameTracker, intptr_t Data, short Mode);
int PhysicsFollowWall(Instance *instance, GameTracker *gameTracker, intptr_t Data, short Mode);
int PhysicsCheckLinkedMove(Instance *instance, intptr_t Data, short Mode);

#endif
