#include "Overlays/skinner/skinner.h"
#include "Game/COLLIDE.h"

// this conditional is for the objdiff report
#ifndef SKIP_ASM

INCLUDE_ASM("asm/nonmatchings/Overlays/skinner/skinner", MON_GetRandomGroundPoint);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinner/skinner", MONSENSE_DetectPlayer);

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

INCLUDE_ASM("asm/nonmatchings/Overlays/skinner/skinner", SKINNER_BurrowInEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinner/skinner", SKINNER_BurrowIn);

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

void MONSENSE_DetectPlayer(void) {};

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

void SKINNER_BurrowInEntry(void) {};

void SKINNER_BurrowIn(void) {};

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
