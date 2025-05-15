#ifndef _ENMYPLAN_H_
#define _ENMYPLAN_H_

#include "common.h"

void ENMYPLAN_RelocatePlanPositions(int slotID, Position *offset);
void ENMYPLAN_ReleasePlanningWorkspace(int slotID);
int ENMYPLAN_GetInitializedPlanningWorkspaceFinal();
void ENMYPLAN_InitEnemyPlanPool(void *enemyPlanPool);
int ENMYPLAN_MoveToTargetFinal(Instance *instance, Position *outputPos, int slotID, Position *targetPos, int validNodeTypes);

#endif
