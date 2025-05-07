#ifndef _PLANAPI_H_
#define _PLANAPI_H_

#include "common.h"

void PLANAPI_DeleteNodeFromPoolByUnit(long streamUnitID);
void PLANAPI_InitPlanMkrList(StreamUnit *streamUnit);
int PLANAPI_NumNodesInPool(void *planningPool);
int PLANAPI_FindNodePositionInUnit(StreamUnit *streamUnit, Position *pos, int id, int type);
void PLANAPI_DeleteNodesFromPoolByType(int nodeSource);
int PLANAPI_FindPathBetweenNodes(PlanningNode *startNode, PlanningNode *goalNode, EnemyPlanData *planData, int validNodeTypes);
int PLANAPI_AddNodeOfTypeToPool(Position *pos, int type);
int PLANAPI_FindPathInGraphToTarget(Position *startPos, EnemyPlanData *planData, int validNodeTypes);
void PLANAPI_InitPlanning(void *planningPool);
void PLANAPI_UpdatePlanningDatabase(GameTracker *gameTracker, Instance *player);

extern PoolManagementData *poolManagementData;

#endif
