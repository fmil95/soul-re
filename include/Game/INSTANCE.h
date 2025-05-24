#ifndef _INSTANCE_H_
#define _INSTANCE_H_

#include "common.h"

void INSTANCE_UpdateFamilyStreamUnitID(Instance *instance);
Instance *INSTANCE_Find(long introUniqueID);
Instance *INSTANCE_IntroduceInstance(Intro *intro, short streamUnitID);
void INSTANCE_UnlinkFromParent(Instance *instance);
Instance *INSTANCE_BirthObject(Instance *parent, Object *object, int modelNum);
void INSTANCE_Post(Instance *Inst, int Message, intptr_t Data);
unsigned long INSTANCE_DefaultAnimCallback(G2Anim *anim, int sectionID, G2AnimCallbackMsg message, long messageDataA, long messageDataB, Instance *instance);
uintptr_t INSTANCE_Query(Instance *Inst, int Query);
void INSTANCE_LinkToParent(Instance *instance, Instance *parent, int node);
void INSTANCE_KillInstance(Instance *instance);
void INSTANCE_Broadcast(Instance *sender, long whatAmIMask, int Message, intptr_t Data);
void INSTANCE_PlainDeath(Instance *instance);
int INSTANCE_Linked(Instance *instance1, Instance *instance2);
void INSTANCE_ReallyRemoveInstance(InstanceList *list, Instance *instance, long reset);
Instance *INSTANCE_GetChildLinkedToSegment(Instance *instance, int segment);
int INSTANCE_SetStatsData(Instance *instance, Instance *checkee, Vector *checkPoint, evCollideInstanceStatsData *data, MATRIX *mat);
void INSTANCE_ReallyRemoveAllChildren(Instance *instance);
long INSTANCE_GetSplineFrameNumber(Instance *instance, MultiSpline *spline);
void INSTANCE_BuildStaticShadow();
int INSTANCE_InPlane(Instance *instance, int plane);
void INSTANCE_UnlinkChildren(Instance *instance);
void INSTANCE_DefaultInit(Instance *instance, Object *object, int modelNum);
int INSTANCE_GetFadeValue(Instance *instance);
Intro *INSTANCE_FindIntro(long areaID, long introUniqueID);
INICommand *INSTANCE_FindIntroCommand(Instance *instance, int cmd);
Instance *INSTANCE_IntroduceSavedInstance(SavedIntro *savedIntro, StreamUnit *streamUnit, int *deleted);
Instance *INSTANCE_IntroduceSavedInstanceWithIntro(SavedIntroWithIntro *savedIntro, StreamUnit *streamUnit, int *deleted);
void INSTANCE_InitInstanceList(InstanceList *list, InstancePool *pool);
void INSTANCE_SpatialRelationships(InstanceList *instanceList);
void INSTANCE_ProcessFunctions(InstanceList *instanceList);
void INSTANCE_CleanUpInstanceList(InstanceList *list, long reset);
void INSTANCE_DeactivateFarInstances(GameTracker *gameTracker);
void INSTANCE_AdditionalCollideFunctions(InstanceList *instanceList);
Instance *INSTANCE_FindWithName(long areaID, char *instanceName, Instance *startInstance);

#endif
