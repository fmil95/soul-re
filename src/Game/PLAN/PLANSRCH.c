#include "Game/PLAN/PLANSRCH.h"
#include "Game/PLAN/PLAN.h"
#include "Game/PLAN/PLANAPI.h"
#include "Game/PLAN/PLANPOOL.h"
#include "Game/MATH3D.h"

PlanningNode *PLANSRCH_FindNodeToExpand(PlanningNode *planningPool, PlanningNode *goalNode, int validNodeTypes)
{
    int i;
    PlanningNode *nodeToExpand;
    unsigned long bestValueSoFar;
    unsigned long valueForCurrentNode;
    PlanningNode *currentNode;

    bestValueSoFar = -1;

    nodeToExpand = NULL;

    for (currentNode = planningPool, i = 0; i < poolManagementData->numNodesInPool; currentNode++, i++)
    {
        if (((validNodeTypes >> currentNode->nodeType) & 0x1) && ((currentNode->flags & 0x1) && (!(currentNode->flags & 0x2))))
        {
            valueForCurrentNode = currentNode->cost + MATH3D_LengthXYZ(currentNode->pos.x - goalNode->pos.x, currentNode->pos.y - goalNode->pos.y, currentNode->pos.z - goalNode->pos.z);

            if (valueForCurrentNode <= bestValueSoFar)
            {
                nodeToExpand = currentNode;

                bestValueSoFar = valueForCurrentNode;
            }
        }
    }

    return nodeToExpand;
}

void PLANSRCH_ExpandNode(PlanningNode *planningPool, PlanningNode *nodeToExpand)
{
    int i;
    int connectionStatus;
    int connections;
    int nodeToExpandIndex;

    connectionStatus = nodeToExpand->connectionStatus;
    connections = nodeToExpand->connections;

    nodeToExpandIndex = nodeToExpand - planningPool;

    nodeToExpand->flags |= 0x2;

    for (i = 0; i < poolManagementData->numNodesInPool; i++)
    {
        if (((connectionStatus & 0x1)) && ((connections & 0x1)) && (&planningPool[i] != nodeToExpand))
        {
            long newCost;

            newCost = nodeToExpand->cost + (short)poolManagementData->distanceMatrix[nodeToExpandIndex][i];

            if ((!(planningPool[i].flags & 0x1)) || (newCost < planningPool[i].cost))
            {
                planningPool[i].parent = nodeToExpand - planningPool;

                if (newCost < -0x7FFF)
                {
                    newCost = -0x7FFF;
                }

                if (newCost > 0x7FFF)
                {
                    newCost = 0x7FFF;
                }

                planningPool[i].cost = (unsigned short)newCost;

                planningPool[i].flags |= 0x1;
            }
        }

        connectionStatus >>= 1;
        connections >>= 1;
    }
}

void PLANSRCH_InitNodesForSearch(PlanningNode *planningPool)
{
    int i;

    for (i = 0; i < poolManagementData->numNodesInPool; i++)
    {
        planningPool[i].cost = 0;

        planningPool[i].parent = 0;

        planningPool[i].flags &= ~0x3;
    }
}

PlanningNode *PLANSRCH_FindPathInGraph(PlanningNode *planningPool, PlanningNode *startNode, PlanningNode *goalNode, int validNodeTypes)
{
    PlanningNode *nodeToExpand;

    if (!(goalNode->connections & ~(1 << (goalNode - planningPool))))
    {
        return NULL;
    }

    PLANSRCH_InitNodesForSearch(planningPool);

    startNode->parent = startNode - planningPool;

    startNode->cost = 0;

    startNode->flags |= 0x1;

    if (PLANPOOL_AreTwoNodesConnected(startNode, goalNode, planningPool) != 0)
    {
        goalNode->parent = startNode - planningPool;

        return goalNode;
    }

    while (1)
    {
        nodeToExpand = PLANSRCH_FindNodeToExpand(planningPool, goalNode, validNodeTypes);

        if (nodeToExpand == NULL)
        {
            return NULL;
        }
        else if (nodeToExpand != goalNode)
        {
            PLANSRCH_ExpandNode(planningPool, nodeToExpand);
        }
        else
        {
            return nodeToExpand;
        }
    }
}
