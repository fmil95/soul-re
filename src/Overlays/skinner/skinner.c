#include "Overlays/skinner/skinner.h"
#include "Game/COLLIDE.h"
#include "Game/GAMELOOP.h"
#include "Game/GAMEPAD.h"
#include "Game/MONSTER/MONLIB.h"
#include "Game/MONSTER/MONMSG.h"
#include "Game/MONSTER/MONSENSE.h"
#include "Game/MONSTER/MONSTER.h"
#include "Game/STATE.h"
#include "Game/STREAM.h"

// this conditional is for the objdiff report
#ifndef SKIP_ASM

int MON_GetRandomGroundPoint(Instance *instance, Position *out, Position *in, short r0, short r1, short theta0, short theta1, short h)
{

    SVECTOR bot;
    SVECTOR top;
    PCollideInfo pcollideInfo;
    Level *level; // not from debug symbols
    int radius; // not from debug symbols
    int angle; // not from debug symbols
    int rc; // not from debug symbols

    level = STREAM_GetLevelWithID(instance->currentStreamUnitID);
    rc = 0;

    radius = r0 + ((rand() * (r1 - r0)) / 32768);
    angle = theta0 + ((rand() * (theta1 - theta0)) / 32768);

    bot.vx = top.vx = in->x + ((radius * rcos(angle)) >> 12);
    bot.vy = top.vy = in->y + ((radius * rsin(angle)) >> 12);

    bot.vz = in->z + (h / 2);
    top.vz = in->z - (h / 2);

    pcollideInfo.collideType = 1;

    pcollideInfo.instance = NULL;
    pcollideInfo.inst = NULL;

    pcollideInfo.oldPoint = &bot;
    pcollideInfo.newPoint = &top;

    COLLIDE_PointAndWorld(&pcollideInfo, level);


    if (pcollideInfo.collideType & 1 && pcollideInfo.prim != NULL && level == (Level *)pcollideInfo.inst)
    {

        if (instance != NULL)
        {
            instance->oldTFace = instance->tface;
            instance->tface = (TFace *)pcollideInfo.prim;
            instance->tfaceLevel = level;
            instance->bspTree = pcollideInfo.segment;
        }

        out->x = top.vx;
        out->y = top.vy;
        out->z = top.vz;

        rc = 1;
    }

    return rc;
}

int MONSENSE_DetectPlayer(Instance *instance)
{
    return MONSENSE_FindIR((MonsterVars *)instance->extraData, gameTrackerX.playerInstance) != NULL;
}

void FX_StartInstanceBurrow(Instance *instance, Level *level, TFace *tface)
{

    SVector normal;
    SVector translation; // not from debug symbols
    Vertex *vertex; // not from debug symbols
    Position *offset; // not from debug symbols

    COLLIDE_GetNormal(tface->normal, (short *)level->terrain->normalList, &normal);

    instance->halvePlane.a = normal.x;
    instance->halvePlane.b = normal.y;
    instance->halvePlane.c = normal.z;

    vertex = &level->terrain->vertexList[tface->face.v0].vertex;
    offset = &level->terrain->BSPTreeArray[instance->bspTree].globalOffset;

    translation.x = vertex->x + offset->x;
    translation.y = vertex->y + offset->y;
    translation.z = vertex->z + offset->z;

    instance->halvePlane.d = -(((instance->halvePlane.a * translation.x) + (instance->halvePlane.b * translation.y) + (instance->halvePlane.c * translation.z)) >> 12);
    instance->halvePlane.flags = 1;
}

void FX_StopInstanceBurrow(Instance *instance)
{
    instance->halvePlane.flags = 0;
}

int SKINNER_BurrowInEntry(Instance *instance)
{
    Level *level; // not from debug symbols
    int rc;// not from debug symbols
    MonsterVars *mv;// not from debug symbols
    MonsterAttributes *ma;// not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    rc = 0;

    if (instance->tface != NULL &&
        instance->tface->textoff != 0xFFFF &&
        ((TextureFT3 *)((char *)((Level *)instance->tfaceLevel)->terrain->StartTextureList + instance->tface->textoff))->attr & 0x100)
    {
        level = STREAM_GetLevelWithID(instance->currentStreamUnitID);
        mv->auxFlags |= 1;
        MON_TurnOffBodySpheres(instance);
        MON_PlayAnimFromList(instance, ma->auxAnimList, SKINNER_ANIM_BURROW_IN, 1);
        FX_StartInstanceBurrow(instance, level, instance->tface);
        mv->avoidMask &= ~0x10;
        rc = 1;
    }

    return rc;
}

int SKINNER_BurrowIn(Instance *instance)
{

    int rc; // not from debug symbols
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    rc = 0;

    if (instance->flags2 & 0x10)
    {
        FX_StopInstanceBurrow(instance);
        mv->auxFlags &= ~1;
        mv->auxFlags |= 4;
        instance->flags |= 0x800;
        MON_TurnOffWeaponSpheres(instance);
        rc = 1;
    }

    return rc;
}

int SKINNER_GetBurrowDest(Instance *instance, Position *enemyPos)
{

    Position target;
    TFace *tface; // not from debug symbols
    int bspTree; // not from debug symbols
    int rc; // not from debug symbols
    MonsterVars *mv; // not from debug symbols

    tface = instance->tface;
    bspTree = instance->bspTree;
    rc = 0;

    if (MON_GetRandomGroundPoint(instance, &target, enemyPos, 128, 204, 0, 4096, 682) != 0)
    {
        if (instance->tface->textoff != 0xFFFF)
        {
            if (((TextureFT3 *)((char *)((Level *)instance->tfaceLevel)->terrain->StartTextureList + instance->tface->textoff))->attr & 0x100)
            {
                if ((target.z - STREAM_GetWaterZLevel(STREAM_GetLevelWithID(instance->currentStreamUnitID), instance)) > 0)
                {
                    mv = (MonsterVars *)instance->extraData;
                    mv->destination = target;
                    rc = 1;
                }
            }
        }
    }

    instance->tface = tface;
    instance->bspTree = bspTree;
    return rc;
}

void SKINNER_CalcBurrowingMove(Instance *instance, Position *position)
{

    int time; // not from debug symbols
    int deltaX; // not from debug symbols
    int deltaY; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    Position *dest; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    dest = &mv->destination;
    time = MON_GetTime(instance);

    deltaX = abs(dest->x - instance->position.x);
    deltaY = abs(dest->y - instance->position.y);

    mv->generalTimer = time + (((deltaX + deltaY) * 1000) / 2560);
    instance->position = mv->destination;
    MON_TurnToPosition(instance, position, 4096);

}

void SKINNER_PupateEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    MON_PupateEntry(instance);
    MON_TurnOffBodySpheres(instance);
    mv->auxFlags |= 4;
}

void SKINNER_Pupate(Instance *instance)
{
    MON_Pupate(instance);

    if (!(instance->flags & 0x800))
    {
        MON_OnGround(instance);
        MON_SwitchState(instance, MONSTER_STATE_SURPRISEATTACK);
    }
}

void SKINNER_PursueEntry(Instance *instance)
{
    MON_PursueEntry(instance);
}

void SKINNER_Pursue(Instance *instance)
{

    int startedBurrow; // not from debug symbols
    int destInRange; // not from debug symbols
    MonsterIR *enemy; // not from debug symbols
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    enemy = mv->enemy;

    if (enemy == NULL)
    {
        MON_Pursue(instance);
        return;
    }

    if (!(mv->auxFlags & 7))
    {
        startedBurrow = 0;
        if (mv->age != 0)
        {
            if (enemy->distance > 2048)
            {
                if (enemy->distance < 10240 && SKINNER_GetBurrowDest(instance, &enemy->instance->position) != 0)
                {
                    startedBurrow = SKINNER_BurrowInEntry(instance);
                }
            }
        }

        if (startedBurrow == 0)
        {
            MON_Pursue(instance);
        }
    }
    else if (mv->auxFlags & 1)
    {
        if (SKINNER_BurrowIn(instance) != 0)
        {
            SKINNER_CalcBurrowingMove(instance, &enemy->instance->position);
        }
    }
    else if (mv->auxFlags & 4 && mv->generalTimer < MON_GetTime(instance))
    {
        destInRange = 0;
        if (((unsigned short)enemy->distance - 205U) < 10035U && SKINNER_GetBurrowDest(instance, &enemy->instance->position) != 0)
        {
            SKINNER_CalcBurrowingMove(instance, &enemy->instance->position);
            destInRange = 1;
        }

        if (destInRange == 0)
        {
            MON_SwitchState(instance, MONSTER_STATE_SURPRISEATTACK);
        }
    }

    while (DeMessageQueue(&mv->messageQueue) != NULL) {};
}

void SKINNER_HideEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (mv->age == 0)
    {
        MON_HideEntry(instance);
    }
    else
    {
        SKINNER_BurrowInEntry(instance);
    }
}

void SKINNER_Hide(Instance *instance)
{

    MonsterIR *enemy; // not from debug symbols
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (mv->age == 0)
    {
        MON_Hide(instance);
        return;
    }

    if (mv->auxFlags & 1)
    {
        SKINNER_BurrowIn(instance);
    }
    else if (mv->auxFlags & 4)
    {
        if (MONSENSE_DetectPlayer(instance) != 0)
        {
            if (mv->behaviorState == MONSTER_STATE_FALL)
            {
                MON_ChangeBehavior(instance, mv->triggeredBehavior);
            }
            else if (mv->behaviorState == MONSTER_STATE_HIT)
            {
                enemy = mv->enemy;
                MON_TurnToPosition(instance, &enemy->instance->position, mv->subAttr->speedPivotTurn);
                if (enemy->distance < mv->ambushRange)
                {
                    MON_SwitchState(instance, MONSTER_STATE_SURPRISEATTACK);
                }
            }
        }
    }
    else
    {
        MON_SwitchState(instance, MONSTER_STATE_WANDER);
    }

    MON_DefaultQueueHandler(instance);
}

void SKINNER_SurpriseAttackEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    if (mv->auxFlags & 7)
    {

        mv->auxFlags &= ~4;
        mv->auxFlags |= 2;
        instance->flags &= ~0x800;

        if (instance->tface != NULL)
        {
            FX_StartInstanceBurrow(instance, STREAM_GetLevelWithID(instance->currentStreamUnitID), instance->tface);
        }

        GAMEPAD_Shock1(64, 122880);
    }

    MON_SurpriseAttackEntry(instance);
}

INCLUDE_ASM("asm/nonmatchings/Overlays/skinner/skinner", SKINNER_SurpriseAttack);

INCLUDE_RODATA("asm/nonmatchings/Overlays/skinner/skinner", D_88000000);

#else

int MON_GetRandomGroundPoint(Instance *instance, Position *out, Position *in, short r0, short r1, short theta0, short theta1, short h)
{

    SVECTOR bot;
    SVECTOR top;
    PCollideInfo pcollideInfo;
    Level *level; // not from debug symbols
    int radius; // not from debug symbols
    int angle; // not from debug symbols
    int rc; // not from debug symbols

    level = STREAM_GetLevelWithID(instance->currentStreamUnitID);
    rc = 0;

    radius = r0 + ((rand() * (r1 - r0)) / 32768);
    angle = theta0 + ((rand() * (theta1 - theta0)) / 32768);

    bot.vx = top.vx = in->x + ((radius * rcos(angle)) >> 12);
    bot.vy = top.vy = in->y + ((radius * rsin(angle)) >> 12);

    bot.vz = in->z + (h / 2);
    top.vz = in->z - (h / 2);

    pcollideInfo.collideType = 1;

    pcollideInfo.instance = NULL;
    pcollideInfo.inst = NULL;

    pcollideInfo.oldPoint = &bot;
    pcollideInfo.newPoint = &top;

    COLLIDE_PointAndWorld(&pcollideInfo, level);


    if (pcollideInfo.collideType & 1 && pcollideInfo.prim != NULL && level == (Level *)pcollideInfo.inst)
    {

        if (instance != NULL)
        {
            instance->oldTFace = instance->tface;
            instance->tface = (TFace *)pcollideInfo.prim;
            instance->tfaceLevel = level;
            instance->bspTree = pcollideInfo.segment;
        }

        out->x = top.vx;
        out->y = top.vy;
        out->z = top.vz;

        rc = 1;
    }

    return rc;
}

int MONSENSE_DetectPlayer(Instance *instance)
{
    return MONSENSE_FindIR((MonsterVars *)instance->extraData, gameTrackerX.playerInstance) != NULL;
}

void FX_StartInstanceBurrow(Instance *instance, Level *level, TFace *tface)
{

    SVector normal;
    SVector translation; // not from debug symbols
    Vertex *vertex; // not from debug symbols
    Position *offset; // not from debug symbols

    COLLIDE_GetNormal(tface->normal, (short *)level->terrain->normalList, &normal);

    instance->halvePlane.a = normal.x;
    instance->halvePlane.b = normal.y;
    instance->halvePlane.c = normal.z;

    vertex = &level->terrain->vertexList[tface->face.v0].vertex;
    offset = &level->terrain->BSPTreeArray[instance->bspTree].globalOffset;

    translation.x = vertex->x + offset->x;
    translation.y = vertex->y + offset->y;
    translation.z = vertex->z + offset->z;

    instance->halvePlane.d = -(((instance->halvePlane.a * translation.x) + (instance->halvePlane.b * translation.y) + (instance->halvePlane.c * translation.z)) >> 12);
    instance->halvePlane.flags = 1;
}

void FX_StopInstanceBurrow(Instance *instance)
{
    instance->halvePlane.flags = 0;
}

int SKINNER_BurrowInEntry(Instance *instance)
{
    Level *level; // not from debug symbols
    int rc;// not from debug symbols
    MonsterVars *mv;// not from debug symbols
    MonsterAttributes *ma;// not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    rc = 0;

    if (instance->tface != NULL &&
        instance->tface->textoff != 0xFFFF &&
        ((TextureFT3 *)((char *)((Level *)instance->tfaceLevel)->terrain->StartTextureList + instance->tface->textoff))->attr & 0x100)
    {
        level = STREAM_GetLevelWithID(instance->currentStreamUnitID);
        mv->auxFlags |= 1;
        MON_TurnOffBodySpheres(instance);
        MON_PlayAnimFromList(instance, ma->auxAnimList, SKINNER_ANIM_BURROW_IN, 1);
        FX_StartInstanceBurrow(instance, level, instance->tface);
        mv->avoidMask &= ~0x10;
        rc = 1;
    }

    return rc;
}

int SKINNER_BurrowIn(Instance *instance)
{

    int rc; // not from debug symbols
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    rc = 0;

    if (instance->flags2 & 0x10)
    {
        FX_StopInstanceBurrow(instance);
        mv->auxFlags &= ~1;
        mv->auxFlags |= 4;
        instance->flags |= 0x800;
        MON_TurnOffWeaponSpheres(instance);
        rc = 1;
    }

    return rc;
}

int SKINNER_GetBurrowDest(Instance *instance, Position *enemyPos)
{

    Position target;
    TFace *tface; // not from debug symbols
    int bspTree; // not from debug symbols
    int rc; // not from debug symbols
    MonsterVars *mv; // not from debug symbols

    tface = instance->tface;
    bspTree = instance->bspTree;
    rc = 0;

    if (MON_GetRandomGroundPoint(instance, &target, enemyPos, 128, 204, 0, 4096, 682) != 0)
    {
        if (instance->tface->textoff != 0xFFFF)
        {
            if (((TextureFT3 *)((char *)((Level *)instance->tfaceLevel)->terrain->StartTextureList + instance->tface->textoff))->attr & 0x100)
            {
                if ((target.z - STREAM_GetWaterZLevel(STREAM_GetLevelWithID(instance->currentStreamUnitID), instance)) > 0)
                {
                    mv = (MonsterVars *)instance->extraData;
                    mv->destination = target;
                    rc = 1;
                }
            }
        }
    }

    instance->tface = tface;
    instance->bspTree = bspTree;
    return rc;
}

void SKINNER_CalcBurrowingMove(Instance *instance, Position *position)
{

    int time; // not from debug symbols
    int deltaX; // not from debug symbols
    int deltaY; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    Position *dest; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    dest = &mv->destination;
    time = MON_GetTime(instance);

    deltaX = abs(dest->x - instance->position.x);
    deltaY = abs(dest->y - instance->position.y);

    mv->generalTimer = time + (((deltaX + deltaY) * 1000) / 2560);
    instance->position = mv->destination;
    MON_TurnToPosition(instance, position, 4096);

}

void SKINNER_PupateEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    MON_PupateEntry(instance);
    MON_TurnOffBodySpheres(instance);
    mv->auxFlags |= 4;
}

void SKINNER_Pupate(Instance *instance)
{
    MON_Pupate(instance);

    if (!(instance->flags & 0x800))
    {
        MON_OnGround(instance);
        MON_SwitchState(instance, MONSTER_STATE_SURPRISEATTACK);
    }
}

void SKINNER_PursueEntry(Instance *instance)
{
    MON_PursueEntry(instance);
}

void SKINNER_Pursue(Instance *instance)
{

    int startedBurrow; // not from debug symbols
    int destInRange; // not from debug symbols
    MonsterIR *enemy; // not from debug symbols
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    enemy = mv->enemy;

    if (enemy == NULL)
    {
        MON_Pursue(instance);
        return;
    }

    if (!(mv->auxFlags & 7))
    {
        startedBurrow = 0;
        if (mv->age != 0)
        {
            if (enemy->distance > 2048)
            {
                if (enemy->distance < 10240 && SKINNER_GetBurrowDest(instance, &enemy->instance->position) != 0)
                {
                    startedBurrow = SKINNER_BurrowInEntry(instance);
                }
            }
        }

        if (startedBurrow == 0)
        {
            MON_Pursue(instance);
        }
    }
    else if (mv->auxFlags & 1)
    {
        if (SKINNER_BurrowIn(instance) != 0)
        {
            SKINNER_CalcBurrowingMove(instance, &enemy->instance->position);
        }
    }
    else if (mv->auxFlags & 4 && mv->generalTimer < MON_GetTime(instance))
    {
        destInRange = 0;
        if (((unsigned short)enemy->distance - 205U) < 10035U && SKINNER_GetBurrowDest(instance, &enemy->instance->position) != 0)
        {
            SKINNER_CalcBurrowingMove(instance, &enemy->instance->position);
            destInRange = 1;
        }

        if (destInRange == 0)
        {
            MON_SwitchState(instance, MONSTER_STATE_SURPRISEATTACK);
        }
    }

    while (DeMessageQueue(&mv->messageQueue) != NULL) {};
}

void SKINNER_HideEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (mv->age == 0)
    {
        MON_HideEntry(instance);
    }
    else
    {
        SKINNER_BurrowInEntry(instance);
    }
}

void SKINNER_Hide(Instance *instance)
{

    MonsterIR *enemy; // not from debug symbols
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (mv->age == 0)
    {
        MON_Hide(instance);
        return;
    }

    if (mv->auxFlags & 1)
    {
        SKINNER_BurrowIn(instance);
    }
    else if (mv->auxFlags & 4)
    {
        if (MONSENSE_DetectPlayer(instance) != 0)
        {
            if (mv->behaviorState == MONSTER_STATE_FALL)
            {
                MON_ChangeBehavior(instance, mv->triggeredBehavior);
            }
            else if (mv->behaviorState == MONSTER_STATE_HIT)
            {
                enemy = mv->enemy;
                MON_TurnToPosition(instance, &enemy->instance->position, mv->subAttr->speedPivotTurn);
                if (enemy->distance < mv->ambushRange)
                {
                    MON_SwitchState(instance, MONSTER_STATE_SURPRISEATTACK);
                }
            }
        }
    }
    else
    {
        MON_SwitchState(instance, MONSTER_STATE_WANDER);
    }

    MON_DefaultQueueHandler(instance);
}

void SKINNER_SurpriseAttackEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    if (mv->auxFlags & 7)
    {

        mv->auxFlags &= ~4;
        mv->auxFlags |= 2;
        instance->flags &= ~0x800;

        if (instance->tface != NULL)
        {
            FX_StartInstanceBurrow(instance, STREAM_GetLevelWithID(instance->currentStreamUnitID), instance->tface);
        }

        GAMEPAD_Shock1(64, 122880);
    }

    MON_SurpriseAttackEntry(instance);
}

void SKINNER_SurpriseAttack(void) {};

#endif
