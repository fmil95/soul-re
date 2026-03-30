#include "Overlays/alukabss/alukabss.h"
#include "Game/G2/ANMCTRLR.h"
#include "Game/MONSTER/MONLIB.h"
#include "Game/PLAN/PLANAPI.h"
#include "Game/GAMELOOP.h"
#include "Game/MATH3D.h"
#include "Game/STREAM.h"

// this conditional is for the objdiff report
#ifndef SKIP_ASM

void ALUKABSS_SetTwist(Instance *instance, int angle)
{

    G2SVector3 extraRot;

    extraRot.y = 0;
    extraRot.x = 0;
    extraRot.z = angle / 2;

    G2Anim_SetController_Vector(&instance->anim, 8, G2ANIM_CTRLRTYPE_ADD_LOCALROT, &extraRot);
    extraRot.z = angle - extraRot.z;
    G2Anim_SetController_Vector(&instance->anim, 6, G2ANIM_CTRLRTYPE_ADD_LOCALROT, &extraRot);
}


INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_RotateToFace);

int ALUKABSS_RazTimeAtMarker(Instance *instance)
{

    Position markerPos;

    int dist; // not from debug symbols
    MonsterIR *enemy; // not from debug symbols
    AlukabssVars *vars; // not from debug symbols
    AlukabssAttributes *attrs; // not from debug symbols
    Level *level; // not from debug symbols
    StreamUnit *streamUnit; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (AlukabssVars *)mv->extraVars;
    attrs = (AlukabssAttributes *)ma->tunData;

    enemy = mv->enemy;
    streamUnit = STREAM_GetStreamUnitWithID(instance->currentStreamUnitID);
    level = streamUnit->level;

    if (enemy != NULL)
    {
        PLANAPI_FindClosestNodePositionInUnit(streamUnit, &enemy->instance->position, &markerPos, 0, 32767, 5, 0);
        dist = MATH3D_LengthXY(enemy->instance->position.x - markerPos.x, enemy->instance->position.y - markerPos.y);

        if (vars->markerx == markerPos.x && vars->markery == markerPos.y &&
            dist < attrs->raz_dist_from_marker && level->waterZLevel < enemy->instance->position.z)
        {
            return MON_GetTime(instance) - vars->raz_time_at_marker;
        }

        vars->raz_time_at_marker = MON_GetTime(instance);
        vars->markerx = markerPos.x;
        vars->markery = markerPos.y;
    }

    return 0;
}

int ALUKABSS_TimeSinceSpit(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols
    AlukabssVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (AlukabssVars *)mv->extraVars;

    if (mv->enemy != NULL && gameTrackerX.gameData.asmData.MorphTime == 1000)
    {
        return MON_GetTime(instance) - vars->time_since_spit;
    }

    vars->time_since_spit = MON_GetTime(instance);
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_ShouldAttack);

void ALUKABSS_InitCircle(Instance *instance)
{

    Position rootPos;
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukabssVars *vars; // not from debug symbols
    AlukabssAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    attrs = (AlukabssAttributes *)ma->tunData;
    vars = (AlukabssVars *)mv->extraVars;

    mv->speed = attrs->init_circle_speed;

    rootPos.x = instance->matrix[1].t[0];
    rootPos.y = instance->matrix[1].t[1];
    rootPos.z = 0;

    vars->last_angle = MATH3D_AngleFromPosToPos(&instance->position, &rootPos);
    vars->accumulated_angle = 0;
}

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_Circle);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_SetUpWaterPlaneClip);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_Query);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_Message);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_Init);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_CleanUp);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_DamageEffect);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_IdleEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_Idle);

void ALUKABSS_PursueEntry() {};

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_Pursue);

void ALUKABSS_CombatEntry() {};

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_Combat);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_AttackEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_Attack);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_LandInWaterEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_LandInWater);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_ProjectileEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_Projectile);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_DeadEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_Dead);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_DoNothingEntry);

void ALUKABSS_DoNothing() {};

INCLUDE_RODATA("asm/nonmatchings/Overlays/alukabss/alukabss", D_88000000);

INCLUDE_RODATA("asm/nonmatchings/Overlays/alukabss/alukabss", D_88000020);

#else 

void ALUKABSS_SetTwist(Instance *instance, int angle)
{

    G2SVector3 extraRot;

    extraRot.y = 0;
    extraRot.x = 0;
    extraRot.z = angle / 2;

    G2Anim_SetController_Vector(&instance->anim, 8, G2ANIM_CTRLRTYPE_ADD_LOCALROT, &extraRot);
    extraRot.z = angle - extraRot.z;
    G2Anim_SetController_Vector(&instance->anim, 6, G2ANIM_CTRLRTYPE_ADD_LOCALROT, &extraRot);
}


void ALUKABSS_RotateToFace(void) {};

int ALUKABSS_RazTimeAtMarker(Instance *instance)
{

    Position markerPos;

    int dist; // not from debug symbols
    MonsterIR *enemy; // not from debug symbols
    AlukabssVars *vars; // not from debug symbols
    AlukabssAttributes *attrs; // not from debug symbols
    Level *level; // not from debug symbols
    StreamUnit *streamUnit; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (AlukabssVars *)mv->extraVars;
    attrs = (AlukabssAttributes *)ma->tunData;

    enemy = mv->enemy;
    streamUnit = STREAM_GetStreamUnitWithID(instance->currentStreamUnitID);
    level = streamUnit->level;

    if (enemy != NULL)
    {
        PLANAPI_FindClosestNodePositionInUnit(streamUnit, &enemy->instance->position, &markerPos, 0, 32767, 5, 0);
        dist = MATH3D_LengthXY(enemy->instance->position.x - markerPos.x, enemy->instance->position.y - markerPos.y);

        if (vars->markerx == markerPos.x && vars->markery == markerPos.y &&
            dist < attrs->raz_dist_from_marker && level->waterZLevel < enemy->instance->position.z)
        {
            return MON_GetTime(instance) - vars->raz_time_at_marker;
        }

        vars->raz_time_at_marker = MON_GetTime(instance);
        vars->markerx = markerPos.x;
        vars->markery = markerPos.y;
    }

    return 0;
}

int ALUKABSS_TimeSinceSpit(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols
    AlukabssVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (AlukabssVars *)mv->extraVars;

    if (mv->enemy != NULL && gameTrackerX.gameData.asmData.MorphTime == 1000)
    {
        return MON_GetTime(instance) - vars->time_since_spit;
    }

    vars->time_since_spit = MON_GetTime(instance);
    return 0;
}

void ALUKABSS_ShouldAttack(void) {};

void ALUKABSS_InitCircle(Instance *instance)
{

    Position rootPos;
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukabssVars *vars; // not from debug symbols
    AlukabssAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    attrs = (AlukabssAttributes *)ma->tunData;
    vars = (AlukabssVars *)mv->extraVars;

    mv->speed = attrs->init_circle_speed;

    rootPos.x = instance->matrix[1].t[0];
    rootPos.y = instance->matrix[1].t[1];
    rootPos.z = 0;

    vars->last_angle = MATH3D_AngleFromPosToPos(&instance->position, &rootPos);
    vars->accumulated_angle = 0;
}

void ALUKABSS_Circle(void) {};

void ALUKABSS_SetUpWaterPlaneClip(void) {};

void ALUKABSS_Query(void) {};

void ALUKABSS_Message(void) {};

void ALUKABSS_Init(void) {};

void ALUKABSS_CleanUp(void) {};

void ALUKABSS_DamageEffect(void) {};

void ALUKABSS_IdleEntry(void) {};

void ALUKABSS_Idle(void) {};

void ALUKABSS_PursueEntry(void) {};

void ALUKABSS_Pursue(void) {};

void ALUKABSS_CombatEntry(void) {};

void ALUKABSS_Combat(void) {};

void ALUKABSS_AttackEntry(void) {};

void ALUKABSS_Attack(void) {};

void ALUKABSS_LandInWaterEntry(void) {};

void ALUKABSS_LandInWater(void) {};

void ALUKABSS_ProjectileEntry(void) {};

void ALUKABSS_Projectile(void) {};

void ALUKABSS_DeadEntry(void) {};

void ALUKABSS_Dead(void) {};

void ALUKABSS_DoNothingEntry(void) {};

void ALUKABSS_DoNothing(void) {};

#endif
