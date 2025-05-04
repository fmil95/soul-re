#ifndef _MONSENSE_H_
#define _MONSENSE_H_

#include "common.h"

MonsterIR *MONSENSE_SetEnemy(Instance *instance, Instance *newenemy);
MonsterIR *MONSENSE_GetMonsterIR(MonsterVars *mv);
void MONSENSE_DoSenses(Instance *instance);
int MONSENSE_GetDistanceInDirection(Instance *instance, short angle);
void MONSENSE_RemoveSenses(Instance *instance);
void MONSENSE_SetupSenses(Instance *instance);
void MONSENSE_StartMonsterIRList(Instance *instance);
void MONSENSE_SenseInstance(Instance *instance, evCollideInstanceStatsData *data);
int MONSENSE_See(Instance *instance, evCollideInstanceStatsData *data);
void MONSENSE_SetupMIR(Instance *instance, evCollideInstanceStatsData *data, int flags);
void MONSENSE_Radar(Instance *instance);
void MONSENSE_ProcessIRList(Instance *instance);
short MONSENSE_GetClosestFreeDirection(Instance *instance, short angle, long range);
MonsterIR *MONSENSE_FindIR(MonsterVars *mv, Instance *instance);
MonsterIR *MONSENSE_FirstSense(Instance *instance, Instance *sensed);

#endif
