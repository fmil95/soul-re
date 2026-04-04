// #include "Game/PLAN/PLANPOOL.h" // TODO: find out why including this header causes errors
#include "Game/PLAN/PLAN.h"
#include "Game/PLAN/PLANAPI.h"
#include "Game/MATH3D.h"

int PLANPOOL_NumNodesInPlan(PlanningNode *goalNode, PlanningNode *planningPool)
{
    PlanningNode *currentNode;
    int i;

    for (i = 0, currentNode = &planningPool[goalNode->parent]; goalNode != currentNode; i++)
    {
        goalNode = &planningPool[goalNode->parent];
        currentNode = &planningPool[goalNode->parent];
    }

    return i + 1;
}

int PLANPOOL_CountBitsSetInLong(long mylong)
{
    int numBitsSet;

    for (numBitsSet = 0; mylong != 0; numBitsSet++)
    {
        mylong &= mylong - 1;
    }

    return numBitsSet;
}

int PLANPOOL_NumConnectionsExaminedForNode(PlanningNode *node)
{
    return PLANPOOL_CountBitsSetInLong(node->connectionStatus);
}

int PLANPOOL_NumConnectionsForNode(PlanningNode *node)
{
    return PLANPOOL_CountBitsSetInLong(node->connections);
}

PlanningNode *PLANPOOL_GetFirstNodeOfSource(PlanningNode *planningPool, char nodeSource)
{
    int i;

    for (i = 0; i < poolManagementData->numNodesInPool; i++, planningPool++)
    {
        if ((planningPool->nodeType & 0x7) == nodeSource)
        {
            return planningPool;
        }
    }

    return NULL;
}

int PLANPOOL_NumberOfNodesOfType(PlanningNode *planningPool, char nodeType)
{
    int i;
    int numNodesOfType;

    numNodesOfType = 0;

    for (i = 0; i < poolManagementData->numNodesInPool; i++, planningPool++)
    {
        if (planningPool->nodeType == nodeType)
        {
            numNodesOfType++;
        }
    }

    return numNodesOfType;
}

int PLANPOOL_NumberOfNodesOfSource(PlanningNode *planningPool, char nodeSource)
{
    int i;
    int numNodesOfSource;

    numNodesOfSource = 0;

    for (i = 0; i < poolManagementData->numNodesInPool; i++, planningPool++)
    {
        if ((planningPool->nodeType & 0x7) == nodeSource)
        {
            numNodesOfSource++;
        }
    }

    return numNodesOfSource;
}

PlanningNode *PLANPOOL_GetNodeWithID(PlanningNode *planningPool, int type, int id, long suID)
{
    int i;

    for (i = 0; i < poolManagementData->numNodesInPool; i++)
    {
        if (planningPool[i].nodeType == type && planningPool[i].id == id && planningPool[i].streamUnitID == suID)
        {
            return &planningPool[i];
        }
    }

    return NULL;
}

PlanningNode *PLANPOOL_GetNodeByPosition(Position *currentPos, PlanningNode *planningPool)
{
    int i;
    unsigned long zDiff;
    unsigned long minZDiff;
    PlanningNode *returnNode;

    minZDiff = -1;

    returnNode = NULL;

    for (i = 0; i < poolManagementData->numNodesInPool; i++)
    {
        if (MATH3D_LengthXY(currentPos->x - planningPool[i].pos.x, currentPos->y - planningPool[i].pos.y) < 11)
        {
            zDiff = abs(currentPos->z - planningPool[i].pos.z);

            if (zDiff < minZDiff)
            {
                returnNode = &planningPool[i];
            }
        }
    }

    return returnNode;
}

PlanningNode *PLANPOOL_GetClosestNode(Position *pos, PlanningNode *planningPool, char distanceType)
{
    int i;
    long dist;
    long minDist;
    PlanningNode *closestNode;

    minDist = 0x7FFFFFFF;

    closestNode = NULL;

    for (i = 0; i < poolManagementData->numNodesInPool; i++)
    {
        if (distanceType == TWO_D_DISTANCE)
        {
            dist = MATH3D_LengthXY(pos->x - planningPool[i].pos.x, pos->y - planningPool[i].pos.y);
        }
        else
        {
            dist = MATH3D_LengthXYZ(pos->x - planningPool[i].pos.x, pos->y - planningPool[i].pos.y, pos->z - planningPool[i].pos.z);
        }

        if (dist < minDist)
        {
            minDist = dist;

            closestNode = &planningPool[i];
        }
    }

    return closestNode;
}

int PLANPOOL_AppropriatePair(PlanningNode *node1, PlanningNode *node2)
{
    PlanningNode *temp;
    int res;
    int src;
    int plc;
    short type1;
    short type2;
    short placement1;
    short placement2;
    short source1;
    short source2;

    type1 = node1->nodeType;
    type2 = node2->nodeType;

    placement1 = (type1 >> 3) & 0x3;
    placement2 = (type2 >> 3) & 0x3;

    source1 = type1 & 0x7;
    source2 = type2 & 0x7;

    if (placement2 < placement1)
    {
        placement1 ^= placement2;
        placement2 ^= placement1;
        placement1 ^= placement2;

        temp = node1;
        node1 = node2;
        node2 = temp;
    }

    if (source2 < source1)
    {
        source1 ^= source2;
        source2 ^= source1;
        source1 ^= source2;
    }

    plc = placement2 | (placement1 << 8);
    src = source2 | (source1 << 8);

    res = 1;

    switch (plc)
    {
    case ((FLOOR_NODE << 8) | CEILING_NODE):
        res = 0;
        break;
    case ((FLOOR_NODE << 8) | WATER_NODE):
        if (src != ((PLANMKR_NODE << 8) | PLANMKR_NODE))
        {
            res = 0;
        }

        break;
    case ((FLOOR_NODE << 8) | WALL_NODE):
    case ((WALL_NODE << 8) | CEILING_NODE):
        if (src != ((PLANMKR_NODE << 8) | PLANMKR_NODE) || node1->pos.z >= node2->pos.z || node2->pos.z >= (node1->pos.z + 800) || MATH3D_LengthXY(node1->pos.x - node2->pos.x, node1->pos.y - node2->pos.y) > 800)
        {
            res = 0;
        }

        break;
    case ((WALL_NODE << 8) | WATER_NODE):
    case ((CEILING_NODE<< 8) | WATER_NODE):
        res = 0;
        break;
    }

    return res;
}

int PLANPOOL_AreTwoNodesConnected(PlanningNode *node1, PlanningNode *node2, PlanningNode *planningPool)
{
    long node2Mask;

    node2Mask = 1 << (node2 - planningPool);

    if (((node1->connectionStatus & node2Mask)) && ((node1->connections & node2Mask)))
    {
        return 1;
    }

    return 0;
}

void PLANPOOL_MarkTwoNodesAsConnected(PlanningNode *node1, PlanningNode *node2, PlanningNode *planningPool)
{
    int node1Index;
    int node2Index;
    int temp; // not from decls.h

    node1Index = node1 - planningPool;
    node2Index = node2 - planningPool;

    temp = MATH3D_DistanceBetweenPositions(&node1->pos, &node2->pos);

    if (temp < 0)
    {
        temp = 0;
    }

    if (temp > 65536)
    {
        temp = 65536;
    }

    poolManagementData->distanceMatrix[node1Index][node2Index] = temp;

    node1->connectionStatus |= 1 << node2Index;
    node1->connections |= 1 << node2Index;
}

void PLANPOOL_MarkTwoNodesAsNotConnected(PlanningNode *node1, PlanningNode *node2, PlanningNode *planningPool)
{
    node1->connectionStatus |= 1 << (node2 - planningPool);
    node1->connections &= ~(1 << (node2 - planningPool));
}

PlanningNode *PLANPOOL_GetClosestUnexploredValidNeighbor(PlanningNode *startNode, PlanningNode *planningPool)
{
    int i;
    unsigned long connectionStatus;
    unsigned long dist;
    unsigned long minDist;
    PlanningNode *returnNode;

    minDist = -1;

    returnNode = NULL;

    if (startNode == NULL)
    {
        return NULL;
    }

    connectionStatus = startNode->connectionStatus;

    for (i = 0; i < poolManagementData->numNodesInPool; i++)
    {
        if (!(connectionStatus & 0x1))
        {
            if (PLANPOOL_AppropriatePair(startNode, &planningPool[i]) != 0)
            {
                dist = MATH3D_LengthXYZ(startNode->pos.x - planningPool[i].pos.x, startNode->pos.y - planningPool[i].pos.y, startNode->pos.z - planningPool[i].pos.z);

                if (dist < minDist)
                {
                    minDist = dist;

                    returnNode = &planningPool[i];
                }
            }
            else
            {
                PLANPOOL_MarkTwoNodesAsNotConnected(startNode, &planningPool[i], planningPool);
            }
        }

        connectionStatus >>= 1;
    }

    return returnNode;
}

void PLANPOOL_ChangeNodePosition(Position *newPos, PlanningNode *nodeToChange, PlanningNode *planningPool)
{
    int i;
    unsigned char nodeToChangeMask;
    int temp; // not from decls.h

    temp = nodeToChange - planningPool;

    if (nodeToChange != NULL)
    {
        nodeToChange->pos = *newPos;

        nodeToChange->connectionStatus = 0;
        nodeToChange->connections = 0;

        nodeToChangeMask = 1 << temp;

        for (i = 0; i < poolManagementData->numNodesInPool; i++)
        {
            planningPool[i].connectionStatus &= ~nodeToChangeMask;
            planningPool[i].connections &= ~nodeToChangeMask;
        }
    }
}

PlanningNode *PLANPOOL_AddNodeToPool(Position *pos, PlanningNode *planningPool, short nodeType, short nodeID, long streamUnitID)
{
    PlanningNode *nextAvailableNode;
    int i;

    nextAvailableNode = NULL;

    if (poolManagementData->numNodesInPool < 32)
    {
        nextAvailableNode = &planningPool[poolManagementData->numNodesInPool];

        COPY_SVEC(Position, &nextAvailableNode->pos, Position, pos);

        nextAvailableNode->connectionStatus = 1 << poolManagementData->numNodesInPool;
        nextAvailableNode->connections = 0;

        nextAvailableNode->flags = 0;

        nextAvailableNode->nodeType = nodeType;

        nextAvailableNode->id = nodeID;
        nextAvailableNode->streamUnitID = streamUnitID;

        poolManagementData->numNodesInPool++;

        for (i = 0; i < poolManagementData->numNodesInPool; i++)
        {
            poolManagementData->distanceMatrix[poolManagementData->numNodesInPool - 1][i] = 0;
            poolManagementData->distanceMatrix[i][poolManagementData->numNodesInPool - 1] = 0;
        }
    }

    return nextAvailableNode;
}

void PLANPOOL_DeleteNodeFromPool(PlanningNode *nodeToDelete, PlanningNode *planningPool)
{
    int indexOfNodeToDelete;
    int lastNodeIndex;
    long oldConnectionMask;
    long newConnectionMask;
    int i;
    PlanningNode *lastNode;

    if (nodeToDelete == NULL)
    {
        return;
    }

    lastNodeIndex = poolManagementData->numNodesInPool - 1;

    lastNode = &planningPool[lastNodeIndex];

    *nodeToDelete = *lastNode;

    indexOfNodeToDelete = nodeToDelete - planningPool;

    newConnectionMask = 1 << indexOfNodeToDelete;
    oldConnectionMask = 1 << lastNodeIndex;

    for (i = 0; i < (poolManagementData->numNodesInPool - 1); i++)
    {
        if ((planningPool[i].connectionStatus & oldConnectionMask))
        {
            planningPool[i].connectionStatus |= newConnectionMask;
        }
        else
        {
            planningPool[i].connectionStatus &= ~newConnectionMask;
        }

        planningPool[i].connectionStatus &= ~oldConnectionMask;

        if ((planningPool[i].connections & oldConnectionMask))
        {
            planningPool[i].connections |= newConnectionMask;
        }
        else
        {
            planningPool[i].connections &= ~newConnectionMask;
        }

        planningPool[i].connections &= ~oldConnectionMask;
    }

    for (i = 0; i < (poolManagementData->numNodesInPool - 1); i++)
    {
        poolManagementData->distanceMatrix[indexOfNodeToDelete][i] = poolManagementData->distanceMatrix[lastNodeIndex][i];
        poolManagementData->distanceMatrix[i][indexOfNodeToDelete] = poolManagementData->distanceMatrix[i][lastNodeIndex];
    }

    poolManagementData->numNodesInPool--;
}
