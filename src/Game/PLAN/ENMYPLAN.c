#include "Game/PLAN/ENMYPLAN.h"
#include "Game/PLAN/PLANAPI.h"
#include "Game/GAMELOOP.h"
#include "Game/MATH3D.h"
#include "Game/PLAN/PLANCOLL.h"
#include "Game/PSX/MAIN.h"

void ENMYPLAN_InitEnemyPlanPool(void *enemyPlanPool)
{
    int i;

    for (i = 0; i < 10; i++)
    {
        ((EnemyPlanSlotData *)enemyPlanPool)[i].slotInUse = 0;
    }
}

int ENMYPLAN_GetNextAvailablePlanningSlot()
{
    int slotID;
    int i;

    slotID = -1;

    for (i = 0; i < 10; i++)
    {
        if ((signed char)((EnemyPlanSlotData *)gameTrackerX.enemyPlanPool)[i].slotInUse == 0)
        {
            slotID = i;
            break;
        }
    }

    return slotID;
}

int ENMYPLAN_GetInitializedPlanningWorkspaceFinal()
{
    EnemyPlanSlotData *pool;
    int slotID;

    pool = (EnemyPlanSlotData *)gameTrackerX.enemyPlanPool;

    slotID = ENMYPLAN_GetNextAvailablePlanningSlot();

    if (slotID != -1)
    {
        pool[slotID].slotInUse = 1;

        pool[slotID].state = 0;
    }

    return slotID;
}

void ENMYPLAN_ReleasePlanningWorkspace(int slotID)
{
    EnemyPlanSlotData *pool;

    pool = (EnemyPlanSlotData *)gameTrackerX.enemyPlanPool;

    if (slotID >= 0)
    {
        if (slotID < 10)
        {
            PLANAPI_DeleteNodesFromPoolByType(2);
            PLANAPI_DeleteNodesFromPoolByType(3);

            pool[slotID].slotInUse = 0;
        }
    }
}

int ENMYPLAN_WayPointSkipped(Position *currentPos, Position *targetPos, Position *nextTargetPos)
{
    long range[2];
    Position vector[2];

    SUB_SVEC(Position, &vector[0], Position, targetPos, Position, currentPos);

    range[0] = MATH3D_LengthXYZ(vector[0].x, vector[0].y, vector[0].z);

    SUB_SVEC(Position, &vector[1], Position, nextTargetPos, Position, targetPos);

    range[1] = MATH3D_LengthXYZ(vector[1].x, vector[1].y, vector[1].z);

    if (range[0] < range[1])
    {
        range[0] = range[0] ^ range[1];
        range[1] = range[1] ^ range[0];
        range[0] = range[0] ^ range[1];
    }

    return ((((range[0] * 724) >> 10) * range[1]) < ((vector[0].x * vector[1].x) + (vector[0].y * vector[1].y) + (vector[0].z * vector[1].z))) ^ 1;
}

int ENMYPLAN_WayPointReached(Position *currentPos, Position *oldCurrentPos, Position *targetPos)
{
    long range[2];

    range[0] = MATH3D_LengthXYZ(targetPos->x - currentPos->x, targetPos->y - currentPos->y, targetPos->z - currentPos->z);
    range[1] = MATH3D_LengthXYZ(targetPos->x - oldCurrentPos->x, targetPos->y - oldCurrentPos->y, targetPos->z - oldCurrentPos->z);

    if (((range[0] > range[1]) && (range[0] < 400)) || (range[0] < 50))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void ENMYPLAN_Replan(EnemyPlanSlotData *planSlot)
{
    planSlot->state = 0;

    PLANAPI_DeleteNodesFromPoolByType(2);
    PLANAPI_DeleteNodesFromPoolByType(3);
}

int ENMYPLAN_PathClear(Position *pos, Position *target)
{
    int len;
    int oldPCO;
    Position *tpos;
    Position newTarget;
    int temp; // not from decls.h

    oldPCO = gameTrackerX.plan_collide_override;

    gameTrackerX.plan_collide_override = 1;

    newTarget.x = target->x - pos->x;
    newTarget.y = target->y - pos->y;
    newTarget.z = target->z - pos->z;

    len = MATH3D_LengthXYZ(newTarget.x, newTarget.y, newTarget.z);

    if (len > 1280)
    {
        newTarget.x = (((newTarget.x << 12) / len) * 1280) / 4096;
        newTarget.y = (((newTarget.y << 12) / len) * 1280) / 4096;
        newTarget.z = (((newTarget.z << 12) / len) * 1280) / 4096;

        newTarget.x += pos->x;
        newTarget.y += pos->y;

        tpos = &newTarget;

        newTarget.z += pos->z;
    }
    else
    {
        tpos = target;
    }

    temp = PLANCOLL_DoesStraightLinePathExist(pos, tpos, 0);

    gameTrackerX.plan_collide_override = oldPCO;

    return temp;
}

int ENMYPLAN_MoveToTargetFinal(Instance *instance, Position *outputPos, int slotID, Position *targetPos, int validNodeTypes)
{
    EnemyPlanSlotData *pool;
    EnemyPlanSlotData *planSlot;
    EnemyPlanData *planData;
    int nextWayPointType;
    int nextNextWayPointType;
    int pathFound;
    int nextWayPointIdx;
    int nextNextWayPointIdx;
    // int nextWayPointSkip; unused
    int i;
    Position *nextWayPointPos;
    Position *nextNextWayPointPos;
    int temp; // not from decls.h

    pool = (EnemyPlanSlotData *)gameTrackerX.enemyPlanPool;

    gameTrackerX.plan_collide_override = 1;

    *outputPos = *targetPos;

    if ((slotID == -1) || (slotID >= 10))
    {
        gameTrackerX.plan_collide_override = 1;

        return 2;
    }

    planSlot = &pool[slotID];
    planData = &planSlot->planData;

    switch (planSlot->state)
    {
    case 0:
        planSlot->startPos = instance->position;
        planSlot->goalPos = *targetPos;

        planSlot->timer = 0;

        if (PLANAPI_AddNodeOfTypeToPool(&planSlot->goalPos, 3) != 0)
        {
            PLANAPI_AddNodeOfTypeToPool(&planSlot->startPos, 2);

            planSlot->state = 1;
        }

        break;
    case 1:
        temp = PLANAPI_FindPathInGraphToTarget(&planSlot->startPos, planData, validNodeTypes);

        planSlot->timer++;

        if (temp != 0)
        {
            planSlot->state = 2;

            planSlot->wayPointBeingServoedTo = 1;
        }
        else if (planSlot->timer > 100)
        {
            planSlot->state = 4;
        }

        break;
    case 2:
        nextWayPointIdx = planSlot->wayPointBeingServoedTo;
        nextWayPointPos = &planData->wayPointArray[nextWayPointIdx];

        switch (planData->nodeSkipArray[nextWayPointIdx])
        {
        case 2:
            if (ENMYPLAN_PathClear(&instance->position, nextWayPointPos) == 0)
            {
                if (planData->nodeSkipArray[nextWayPointIdx - 1] != 1)
                {
                    ENMYPLAN_Replan(planSlot);
                }
                else
                {
                    for (i = nextWayPointIdx; i > 0;)
                    {
                        i--;

                        if ((planData->nodeSkipArray[i] != 1) || (i == 0))
                        {
                            planSlot->wayPointBeingServoedTo = i;

                            planData->nodeSkipArray[i + 1] = 2;
                            break;
                        }
                    }
                }
            }

            break;
        default:
            nextWayPointType = (planData->nodeTypeArray[nextWayPointIdx] >> 3) & 0x3;

            nextNextWayPointIdx = nextWayPointIdx + 1;
            nextNextWayPointPos = &planData->wayPointArray[nextNextWayPointIdx];
            nextNextWayPointType = (planData->nodeTypeArray[nextNextWayPointIdx] >> 3) & 0x3;

            while ((nextWayPointType == nextNextWayPointType) && (planData->nodeSkipArray[nextWayPointIdx] != 2) && (nextWayPointType == 0) && (ENMYPLAN_WayPointSkipped(&instance->position, nextWayPointPos, nextNextWayPointPos) != 0) && (planSlot->wayPointBeingServoedTo < (planSlot->planData.numWayPoints - 2)))
            {
                planData->nodeSkipArray[nextWayPointIdx] = 1;

                nextWayPointIdx++;
                nextWayPointPos = nextNextWayPointPos;
                nextWayPointType = nextNextWayPointType;

                nextNextWayPointIdx++;

                planSlot->wayPointBeingServoedTo++;

                nextNextWayPointPos = &planData->wayPointArray[nextNextWayPointIdx];
                nextNextWayPointType = (planData->nodeTypeArray[nextNextWayPointIdx] >> 3) & 0x3;
            }

            planData->nodeSkipArray[nextWayPointIdx] = 2;
        }

        if (ENMYPLAN_WayPointReached(&instance->position, &planSlot->oldCurrentPos, nextWayPointPos) != 0)
        {
            planSlot->wayPointBeingServoedTo++;

            if (planSlot->wayPointBeingServoedTo >= (planSlot->planData.numWayPoints - 1))
            {
                planSlot->state = 3;

                PLANAPI_DeleteNodesFromPoolByType(2);
                PLANAPI_DeleteNodesFromPoolByType(3);
            }
        }

        nextWayPointIdx = planSlot->wayPointBeingServoedTo;
        nextWayPointPos = &planData->wayPointArray[nextWayPointIdx];

        *outputPos = *nextWayPointPos;
        break;
    case 3:
        if (ENMYPLAN_PathClear(&instance->position, targetPos) == 0)
        {
            ENMYPLAN_Replan(planSlot);
        }
        else
        {
            temp = MATH3D_LengthXYZ(targetPos->x - instance->position.x, targetPos->y - instance->position.y, targetPos->z - instance->position.z);

            nextWayPointPos = &planData->wayPointArray[(int)planSlot->wayPointBeingServoedTo];

            if (temp > 1024)
            {
                temp = MATH3D_LengthXYZ(targetPos->x - nextWayPointPos->x, targetPos->y - nextWayPointPos->y, targetPos->z - nextWayPointPos->z);

                if (temp > 1024)
                {
                    ENMYPLAN_Replan(planSlot);
                }
            }
        }

        *outputPos = *targetPos;
        break;
    }

    planSlot->oldCurrentPos = instance->position;

    switch (planSlot->state)
    {
    case 0:
    case 1:
        pathFound = 0;
        break;
    case 2:
        pathFound = 1;
        break;
    case 3:
        pathFound = 2;
        break;
    case 4:
    default:
        pathFound = 3;
    }

    gameTrackerX.plan_collide_override = 0;

    return pathFound;
}

int ValidSlotAndState(EnemyPlanSlotData *pool, int slotID)
{
    if ((slotID == -1) || (slotID >= 10))
    {
        return 0;
    }

    return (pool[slotID].state == 2) || (pool[slotID].state == 3);
}

int ENMYPLAN_GetNodeTypeOfNextWaypoint(int slotID)
{
    EnemyPlanSlotData *pool;
    EnemyPlanSlotData *planSlot;

    pool = (EnemyPlanSlotData *)gameTrackerX.enemyPlanPool;

    if (ValidSlotAndState(pool, slotID) != 0)
    {
        planSlot = &pool[slotID];

        return planSlot->planData.nodeTypeArray[(int)planSlot->wayPointBeingServoedTo];
    }

    return 64;
}

int ENMYPLAN_GetPosOfNextWaypoint(int slotID, Position *pos)
{
    EnemyPlanSlotData *pool;
    EnemyPlanSlotData *planSlot;

    pool = (EnemyPlanSlotData *)gameTrackerX.enemyPlanPool;

    if (ValidSlotAndState(pool, slotID) != 0)
    {
        planSlot = &pool[slotID];

        COPY_SVEC(Position, pos, Position, &planSlot->planData.wayPointArray[(int)planSlot->wayPointBeingServoedTo]);

        return 1;
    }

    return 0;
}

void ENMYPLAN_RelocatePlanPositions(int slotID, Position *offset)
{
    EnemyPlanSlotData *pool;
    EnemyPlanSlotData *planSlot;
    Position *pos;
    int i;
    int numWayPoints;

    pool = (EnemyPlanSlotData *)gameTrackerX.enemyPlanPool;

    planSlot = &pool[slotID];

    numWayPoints = planSlot->planData.numWayPoints;

    if (((slotID != -1) && (slotID < 10)) && (numWayPoints <= 7))
    {
        for (i = numWayPoints; i > 0;)
        {
            i--;

            pos = &planSlot->planData.wayPointArray[i];

            ADD_SVEC(Position, pos, Position, pos, Position, offset);
        }
    }
}
