#include "Game/GAMELOOP.h"
#include "Game/MONSTER/MONAPI.h"
#include "Game/RAZIEL/RAZIEL.h"
#include "Game/INSTANCE.h"

void PLAYER_TurnHead(Instance *instance, short *rotx, short *rotz, GameTracker *gameTracker)
{
    if ((INSTANCE_Query(instance, 1) & 0x1))
    {
        RAZIEL_TurnHead(instance, rotx, rotz, gameTracker);
    }
    else
    {
        MONAPI_TurnHead(instance, rotx, rotz, gameTracker);
    }
}

long PLAYER_OkToLookAround(Instance *instance)
{
    if ((INSTANCE_Query(instance, 1) & 0x1))
    {
        return RAZIEL_OkToLookAround(instance);
    }
    else
    {
        return MONAPI_OkToLookAround(instance);
    }
}

void PLAYER_SetLookAround(Instance *instance)
{
    if ((INSTANCE_Query(instance, 1) & 0x1))
    {
        RAZIEL_SetLookAround(instance);
    }
    else
    {
        MONAPI_SetLookAround(instance);
    }
}

void PLAYER_ReSetLookAround(Instance *instance)
{
    if ((INSTANCE_Query(instance, 1) & 0x1))
    {
        RAZIEL_ResetLookAround(instance);
    }
    else
    {
        MONAPI_ResetLookAround(instance);
    }
}
