#include "Overlays/skinner/skinner.h"
#include "Game/COLLIDE.h"
#include "Game/GAMELOOP.h"
#include "Game/MONSTER/MONLIB.h"
#include "Game/MONSTER/MONSENSE.h"
#include "Game/STREAM.h"

// this conditional is for the objdiff report
#ifndef SKIP_ASM

INCLUDE_ASM("asm/nonmatchings/Overlays/skinner/skinner", MON_GetRandomGroundPoint);

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

    instance->halvePlane.d = -(((instance->halvePlane.a * translation.x) + (instance->halvePlane.b * translation.y) + (instance->halvePlane.c * translation.z)) >> 0xC);
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

INCLUDE_ASM("asm/nonmatchings/Overlays/skinner/skinner", SKINNER_GetBurrowDest);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinner/skinner", SKINNER_CalcBurrowingMove);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinner/skinner", SKINNER_PupateEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinner/skinner", SKINNER_Pupate);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinner/skinner", SKINNER_PursueEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinner/skinner", SKINNER_Pursue);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinner/skinner", SKINNER_HideEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinner/skinner", SKINNER_Hide);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinner/skinner", SKINNER_SurpriseAttackEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinner/skinner", SKINNER_SurpriseAttack);

INCLUDE_RODATA("asm/nonmatchings/Overlays/skinner/skinner", D_88000000);

#else

void MON_GetRandomGroundPoint(void) {};

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

    instance->halvePlane.d = -(((instance->halvePlane.a * translation.x) + (instance->halvePlane.b * translation.y) + (instance->halvePlane.c * translation.z)) >> 0xC);
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

void SKINNER_GetBurrowDest(void) {};

void SKINNER_CalcBurrowingMove(void) {};

void SKINNER_PupateEntry(void) {};

void SKINNER_Pupate(void) {};

void SKINNER_PursueEntry(void) {};

void SKINNER_Pursue(void) {};

void SKINNER_HideEntry(void) {};

void SKINNER_Hide(void) {};

void SKINNER_SurpriseAttackEntry(void) {};

void SKINNER_SurpriseAttack(void) {};

#endif
