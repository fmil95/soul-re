#include "Overlays/skinbos/skinbos.h"
#include "Game/MATH3D.h"
#include "Game/PLAN/PLANAPI.h"
#include "Game/STREAM.h"

// TODO: double-check that SKINBOS_CheckInsideMasher and SKINBOS_ShouldEscapeJail aren't swapped

// this conditional is for the objdiff report
#ifndef SKIP_ASM

int SKINBOS_Turn(Instance *instance, Position *target, int limit)
{

    int angle; // not from debug symbols

    if (((MATH3D_AngleFromPosToPos(&instance->position, target) - instance->rotation.z) & 0xFFF) > 2048)
    {
        angle = ((MATH3D_AngleFromPosToPos(&instance->position, target) - instance->rotation.z) & 0xFFF) - 4096;
    }
    else
    {
        angle = (MATH3D_AngleFromPosToPos(&instance->position, target) - instance->rotation.z) & 0xFFF;
    }

    if (limit >= angle)
    {
        return (angle < -limit) * 2;
    }

    return 1;
}

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_GateDrop);

int SKINBOS_CheckPointInsideMasher(Instance *instance, Position *position, int which_border)
{
    Position center;
    Position radius;
    int radialDist; // not from debug symbols
    int pointDist; // not from debug symbols
    int markerID; // not from debug symbols
    StreamUnit *su; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    SkinbosAttributes *attrs; // not from debug symbols

    ma = (MonsterAttributes *)instance->data;
    attrs = (SkinbosAttributes *)ma->tunData;

    su = STREAM_GetStreamUnitWithID(instance->currentStreamUnitID);
    PLANAPI_FindNodePositionInUnit(su, &center, attrs->masher_center_marker, 5);

    markerID = which_border == 0 ? attrs->masher_radius_marker_inner : attrs->masher_radius_marker_outer;
    PLANAPI_FindNodePositionInUnit(su, &radius, markerID, 5);


    radialDist = MATH3D_LengthXYZ(center.x - radius.x, center.y - radius.y, center.z - radius.z);
    pointDist = MATH3D_LengthXYZ(center.x - position->x, center.y - position->y, center.z - position->z);
    return pointDist < radialDist;

}

void SKINBOS_CheckInsideMasher(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (SKINBOS_CheckPointInsideMasher(instance, &instance->position, 0))
    {
        mv->auxFlags |= 8;
    }
    else
    {
        mv->auxFlags &= ~8;
    }
}

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_DoPhaseFade);

INCLUDE_RODATA("asm/nonmatchings/Overlays/skinbos/skinbos", D_88000000);

INCLUDE_RODATA("asm/nonmatchings/Overlays/skinbos/skinbos", D_88000020);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_HandleOneShotAnims);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_DoPhasingOutInit);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_CheckPhaseIn);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_ShouldEscapeJail);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_ProcessGateHitBlood);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_StartVertexBlood);

INCLUDE_RODATA("asm/nonmatchings/Overlays/skinbos/skinbos", D_88000060);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_Collide);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_DamageEffect);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_Message);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_Query);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_Init);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_CleanUp);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_IdleEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_Idle);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_FindRandomNodeInUnit);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_WanderEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_Wander);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_PursueEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_Pursue);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_HitEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_Hit);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_CombatEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_Combat);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_LandOnFeetEntry);

void SKINBOS_LandOnFeet(void) {};

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_AttackEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_Attack);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_DeadEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_Dead);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_FleeEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_Flee);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_StunnedEntry);

#else

int SKINBOS_Turn(Instance *instance, Position *target, int limit)
{

    int angle; // not from debug symbols

    if (((MATH3D_AngleFromPosToPos(&instance->position, target) - instance->rotation.z) & 0xFFF) > 2048)
    {
        angle = ((MATH3D_AngleFromPosToPos(&instance->position, target) - instance->rotation.z) & 0xFFF) - 4096;
    }
    else
    {
        angle = (MATH3D_AngleFromPosToPos(&instance->position, target) - instance->rotation.z) & 0xFFF;
    }

    if (limit >= angle)
    {
        return (angle < -limit) * 2;
    }

    return 1;
}

void SKINBOS_GateDrop(void) {};

int SKINBOS_CheckPointInsideMasher(Instance *instance, Position *position, int which_border)
{
    Position center;
    Position radius;
    int radialDist; // not from debug symbols
    int pointDist; // not from debug symbols
    int markerID; // not from debug symbols
    StreamUnit *su; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    SkinbosAttributes *attrs; // not from debug symbols

    ma = (MonsterAttributes *)instance->data;
    attrs = (SkinbosAttributes *)ma->tunData;

    su = STREAM_GetStreamUnitWithID(instance->currentStreamUnitID);
    PLANAPI_FindNodePositionInUnit(su, &center, attrs->masher_center_marker, 5);

    markerID = which_border == 0 ? attrs->masher_radius_marker_inner : attrs->masher_radius_marker_outer;
    PLANAPI_FindNodePositionInUnit(su, &radius, markerID, 5);


    radialDist = MATH3D_LengthXYZ(center.x - radius.x, center.y - radius.y, center.z - radius.z);
    pointDist = MATH3D_LengthXYZ(center.x - position->x, center.y - position->y, center.z - position->z);
    return pointDist < radialDist;

}

void SKINBOS_CheckInsideMasher(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (SKINBOS_CheckPointInsideMasher(instance, &instance->position, 0))
    {
        mv->auxFlags |= 8;
    }
    else
    {
        mv->auxFlags &= ~8;
    }
}

void SKINBOS_DoPhaseFade(void) {};

void SKINBOS_HandleOneShotAnims(void) {};

void SKINBOS_DoPhasingOutInit(void) {};

void SKINBOS_CheckPhaseIn(void) {};

void SKINBOS_ShouldEscapeJail(void) {};

void SKINBOS_ProcessGateHitBlood(void) {};

void SKINBOS_StartVertexBlood(void) {};

void SKINBOS_Collide(void) {};

void SKINBOS_DamageEffect(void) {};

void SKINBOS_Message(void) {};

void SKINBOS_Query(void) {};

void SKINBOS_Init(void) {};

void SKINBOS_CleanUp(void) {};

void SKINBOS_IdleEntry(void) {};

void SKINBOS_Idle(void) {};

void SKINBOS_FindRandomNodeInUnit(void) {};

void SKINBOS_WanderEntry(void) {};

void SKINBOS_Wander(void) {};

void SKINBOS_PursueEntry(void) {};

void SKINBOS_Pursue(void) {};

void SKINBOS_HitEntry(void) {};

void SKINBOS_Hit(void) {};

void SKINBOS_CombatEntry(void) {};

void SKINBOS_Combat(void) {};

void SKINBOS_LandOnFeetEntry(void) {};

void SKINBOS_LandOnFeet(void) {};

void SKINBOS_AttackEntry(void) {};

void SKINBOS_Attack(void) {};

void SKINBOS_DeadEntry(void) {};

void SKINBOS_Dead(void) {};

void SKINBOS_FleeEntry(void) {};

void SKINBOS_Flee(void) {};

void SKINBOS_StunnedEntry(void) {};

#endif
