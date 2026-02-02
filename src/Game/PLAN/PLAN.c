#include "Game/PLAN/PLAN.h"
#include "Game/PLAN/PLANAPI.h"
#include "Game/PLAN/PLANPOOL.h"
#include "Game/PLAN/PLANCOLL.h"
#include "Game/MATH3D.h"
#include "Game/STREAM.h"
#include "Game/COLLIDE.h"
#include "Game/GAMELOOP.h"
#include "Game/MEMPACK.h"
#include "Game/PSX/MAIN.h"

long PLAN_CalcMinDistFromExistingNodes(Position *pos, PlanningNode *planningPool, int distanceType)
{
    PlanningNode *closestNode;
    long minDist;

    // TODO: needs to match via including the proper definition for the function below
    closestNode = (PlanningNode *)(intptr_t)PLANPOOL_GetClosestNode(pos, planningPool, distanceType);

    if (closestNode != NULL)
    {
        if (distanceType == 0)
        {
            minDist = MATH3D_LengthXY(pos->x - closestNode->pos.x, pos->y - closestNode->pos.y);
        }
        else
        {
            minDist = MATH3D_LengthXYZ(pos->x - closestNode->pos.x, pos->y - closestNode->pos.y, pos->z - closestNode->pos.z);
        }
    }
    else
    {
        minDist = 0x7FFFFFFF;
    }

    return minDist;
}

void PLAN_UpdatePlanMkrNodes(PlanningNode *planningPool, Position *playerPos)
{
    int i;
    int d;
    long zDiff;
    StreamUnit *streamUnit;
    int temp; // not from decls.h

    streamUnit = &StreamTracker.StreamList[0];

    for (d = 0; d < 16; d++, streamUnit++)
    {
        if ((streamUnit->used == 2) && (MEMPACK_MemoryValidFunc((char *)streamUnit->level) != 0))
        {
            int numPlanMkrs;
            PlanMkr *planMkr;
            long suID;

            planMkr = streamUnit->level->PlanMarkerList;

            numPlanMkrs = streamUnit->level->NumberOfPlanMarkers;

            suID = streamUnit->StreamUnitID;

            if (numPlanMkrs != 0)
            {
                for (i = 0; i < numPlanMkrs; i++, planMkr++)
                {
                    long nodeType;
                    long nodeID;

                    temp = MATH3D_LengthXY(planMkr->pos.x - playerPos->x, planMkr->pos.y - playerPos->y);

                    zDiff = abs(playerPos->z - planMkr->pos.z);

                    if ((temp < 8000) && (zDiff < 4000))
                    {
                        nodeID = planMkr->id & ~0xF000;

                        if (!(planMkr->id & 0x1000))
                        {
                            if ((planMkr->id & 0x8000))
                            {
                                nodeType = 12;
                            }
                            else if ((planMkr->id & 0x4000))
                            {
                                nodeType = 28;
                            }
                            else if ((planMkr->id & 0x2000))
                            {
                                nodeType = 20;
                            }
                            else
                            {
                                nodeType = 4;
                            }

                            if (PLANPOOL_GetNodeWithID(planningPool, nodeType, nodeID, suID) == 0)
                            {
                                PLANPOOL_AddNodeToPool(&planMkr->pos, planningPool, (short)nodeType, (short)nodeID, streamUnit->StreamUnitID);
                            }
                        }
                    }
                }
            }
        }
    }

    for (i = 0; i < poolManagementData->numNodesInPool; i++)
    {
        if ((planningPool[i].nodeType & 0x7) == 4)
        {
            temp = MATH3D_LengthXY(planningPool[i].pos.x - playerPos->x, planningPool[i].pos.y - playerPos->y);

            zDiff = abs(playerPos->z - planningPool[i].pos.z);

            if ((temp > 10000) || (zDiff > 5000))
            {
                PLANPOOL_DeleteNodeFromPool(&planningPool[i], planningPool);
            }
        }
    }
}

void PLAN_UpdatePlayerNode(PlanningNode *planningPool, Position *playerPos)
{
    PlanningNode *playerNode;
    int nodePlacement;
    int foundHit;
    PlanCollideInfo pci;

    playerNode = PLANPOOL_GetFirstNodeOfSource(planningPool, 1);

    if (playerNode != NULL)
    {
        foundHit = PLANCOLL_CheckUnderwaterPoint(playerPos);

        if (foundHit != -1)
        {
            PLANPOOL_ChangeNodePosition(playerPos, playerNode, planningPool);

            playerNode->nodeType = 25;

            playerNode->streamUnitID = foundHit;
            return;
        }

        COPY_SVEC(Position, &pci.collidePos, Position, playerPos);

        if (PLANCOLL_FindTerrainHitFinal(&pci, &nodePlacement, 256, -640, 0, 5) != 0)
        {
            PLANPOOL_ChangeNodePosition(&pci.collidePos, playerNode, planningPool);

            playerNode->nodeType = ((nodePlacement & 0x3) << 3) | 0x1;

            playerNode->streamUnitID = pci.StreamUnitID;
        }
        else if (MATH3D_LengthXYZ(playerPos->x - playerNode->pos.x, playerPos->y - playerNode->pos.y, playerPos->z - playerNode->pos.z) > 12000)
        {
            PLANPOOL_ChangeNodePosition(playerPos, playerNode, planningPool);

            playerNode->nodeType = 1;

            playerNode->streamUnitID = gameTrackerX.playerInstance->currentStreamUnitID;
        }
    }
}

void PLAN_AddRandomNode(PlanningNode *planningPool, Position *playerPos)
{
    int i;
    PlanCollideInfo pci;
    int successFlag;

    successFlag = 0;

    for (i = 0; i < 5; i++)
    {
        COPY_SVEC(Position, &pci.collidePos, Position, playerPos);

        pci.collidePos.x += (rand() % 24000) - 12000;
        pci.collidePos.y += (rand() % 24000) - 12000;

        if ((MATH3D_LengthXYZ(playerPos->x - pci.collidePos.x, playerPos->y - pci.collidePos.y, playerPos->z - pci.collidePos.z) < 12000) && (PLAN_CalcMinDistFromExistingNodes(&pci.collidePos, planningPool, 0) > 1000))
        {
            successFlag = 1;
            break;
        }
    }

    if ((successFlag == 1) && ((PLANCOLL_FindTerrainHitFinal(&pci, NULL, 256, -2000, 0, 0) != 0) || (PLANCOLL_FindTerrainHitFinal(&pci, NULL, 2000, 0, 0, 0) != 0)))
    {
        SVector normal;

        COLLIDE_GetNormal(pci.tFace->normal, (short *)STREAM_GetLevelWithID(pci.StreamUnitID)->terrain->normalList, &normal);

        if (normal.z > 2048)
        {
            PLANPOOL_AddNodeToPool(&pci.collidePos, planningPool, 0, 0, pci.StreamUnitID);
        }
    }
}

void PLAN_DeleteRandomNode(PlanningNode *planningPool)
{
    PLANPOOL_DeleteNodeFromPool(PLANPOOL_GetFirstNodeOfSource(planningPool, 0), planningPool);
}

void PLAN_DeleteOutOfRangeNodesOfSource(PlanningNode *planningPool, Position *playerPos, int nodeSourceToCheck, long removeDist)
{
    int i;

    for (i = 0; i < poolManagementData->numNodesInPool; i++)
    {
        if (((planningPool[i].nodeType & 0x7) == nodeSourceToCheck) && (removeDist < MATH3D_LengthXYZ(playerPos->x - planningPool[i].pos.x, playerPos->y - planningPool[i].pos.y, playerPos->z - planningPool[i].pos.z)))
        {
            PLANPOOL_DeleteNodeFromPool(&planningPool[i], planningPool);
        }
    }
}

void PLAN_AddOrRemoveRandomNodes(PlanningNode *planningPool, Position *playerPos)
{
    int numNodeError;

    numNodeError = poolManagementData->numNodesInPool - 16;

    if (numNodeError < 0)
    {
        PLAN_AddRandomNode(planningPool, playerPos);
    }
    else if (numNodeError > 0)
    {
        PLAN_DeleteRandomNode(planningPool);
    }
}

void PLAN_AddInitialNodes(PlanningNode *planningPool, Instance *player)
{
    PlanCollideInfo pci;

    COPY_SVEC(Position, &pci.collidePos, Position, &player->position);

    PLANCOLL_FindTerrainHitFinal(&pci, NULL, 256, -1024, 0, 0);

    PLANPOOL_AddNodeToPool(&pci.collidePos, planningPool, 1, 0, player->currentStreamUnitID);

    PLAN_UpdatePlanMkrNodes(planningPool, &player->position);

    poolManagementData->playerPosAtLastPlanMkrUpdate = player->position;
}

void PLAN_AddOrRemoveNodes(PlanningNode *planningPool, Instance *player)
{
    if (MATH3D_LengthXYZ(player->position.x - poolManagementData->playerPosAtLastPlanMkrUpdate.x, player->position.y - poolManagementData->playerPosAtLastPlanMkrUpdate.y, player->position.z - poolManagementData->playerPosAtLastPlanMkrUpdate.z) > 500)
    {
        PLAN_UpdatePlayerNode(planningPool, &player->position);
        PLAN_UpdatePlanMkrNodes(planningPool, &player->position);

        PLAN_DeleteOutOfRangeNodesOfSource(planningPool, &player->position, 0, 12000);
        PLAN_DeleteOutOfRangeNodesOfSource(planningPool, &player->position, 2, 12000);
        PLAN_DeleteOutOfRangeNodesOfSource(planningPool, &player->position, 3, 12000);

        poolManagementData->playerPosAtLastPlanMkrUpdate = player->position;
    }

    PLAN_AddOrRemoveRandomNodes(planningPool, &player->position);
}

PlanningNode *PLAN_FindNodeMostInNeedOfConnectivityExpansion(PlanningNode *planningPool)
{
    int i;
    int numConnections;
    int minNumConnections;
    PlanningNode *nodeToReturn;

    minNumConnections = 65535;

    nodeToReturn = NULL;

    for (i = 0; i < poolManagementData->numNodesInPool; i++)
    {
        numConnections = PLANPOOL_NumConnectionsForNode(&planningPool[i]);

        if ((numConnections < minNumConnections) && (PLANPOOL_NumConnectionsExaminedForNode(&planningPool[i]) != poolManagementData->numNodesInPool))
        {
            minNumConnections = numConnections;

            nodeToReturn = &planningPool[i];
        }
    }

    return nodeToReturn;
}
