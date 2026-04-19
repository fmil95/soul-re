#include "Overlays/alukabss/alukabss.h"
#include "Game/G2/ANMCTRLR.h"
#include "Game/MONSTER/MISSILE.h"
#include "Game/MONSTER/MONAPI.h"
#include "Game/MONSTER/MONLIB.h"
#include "Game/MONSTER/MONMSG.h"
#include "Game/MONSTER/MONSTER.h"
#include "Game/PLAN/PLANAPI.h"
#include "Game/RAZIEL/RAZIEL.h"
#include "Game/FX.h"
#include "Game/GAMELOOP.h"
#include "Game/MATH3D.h"
#include "Game/MEMPACK.h"
#include "Game/OBTABLE.h"
#include "Game/PHYSICS.h"
#include "Game/SAVEINFO.h"
#include "Game/SOUND.h"
#include "Game/STATE.h"
#include "Game/STREAM.h"


MonsterStateChoice ALUKABSS_StateChoiceTable[] = {
    {MONSTER_STATE_PURSUE, {ALUKABSS_PursueEntry, ALUKABSS_Pursue}},
    {MONSTER_STATE_COMBAT, {ALUKABSS_CombatEntry, ALUKABSS_Combat}},
    {MONSTER_STATE_IDLE, {ALUKABSS_IdleEntry, ALUKABSS_Idle}},
    {MONSTER_STATE_LANDINWATER, {ALUKABSS_LandInWaterEntry, ALUKABSS_LandInWater}},
    {MONSTER_STATE_PROJECTILE, {ALUKABSS_ProjectileEntry, ALUKABSS_Projectile}},
    {MONSTER_STATE_ATTACK, {ALUKABSS_AttackEntry, ALUKABSS_Attack}},
    {MONSTER_STATE_DEAD, {ALUKABSS_DeadEntry, ALUKABSS_Dead}},
    {MONSTER_STATE_HIT, {ALUKABSS_DoNothingEntry, ALUKABSS_DoNothing}},
    {MONSTER_STATE_MISSILEHIT, {ALUKABSS_DoNothingEntry, ALUKABSS_DoNothing}},
    {MONSTER_STATE_STUNNED, {ALUKABSS_DoNothingEntry, ALUKABSS_DoNothing}},
    {-1, {NULL, NULL}},
};

burntTuneType alukabssBurntTune = {300, 2}; // no canon name in symbols

const MonsterFunctionTable ALUKABSS_FunctionTable = {
    ALUKABSS_Init,
    ALUKABSS_CleanUp,
    ALUKABSS_DamageEffect,
    ALUKABSS_Query,
    ALUKABSS_Message,
    ALUKABSS_StateChoiceTable,
    monVersion,
    "Jul 14 1999"
};

// TODO: Delete once matched
int ALUKABSS_RotateToFace(Instance *, GameTracker *, Position *);
int ALUKABSS_ShouldAttack(Instance *instance);

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

void ALUKABSS_Circle(Instance *instance, GameTracker *gameTracker, int change_anim)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukabssAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    attrs = (AlukabssAttributes *)ma->tunData;

    instance->rotation.z = instance->rotation.z + mv->speed;
    mv->speed += (attrs->circle_accel * gameTracker->timeMult) / 4096;

    if (mv->speed >= attrs->fast_circle_speed && change_anim)
    {
        MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, MONSTER_ANIM_WALK, 2);
    }

    if (mv->speed > attrs->max_circle_speed)
    {
        mv->speed = attrs->max_circle_speed;
    }
}

void ALUKABSS_SetUpWaterPlaneClip(Instance *instance)
{

    PCollideInfo CInfo;
    SVECTOR Old;
    SVECTOR New;
    long *temp; // not from debug symbols

    instance->waterFace = NULL;
    instance->waterFaceTerrain = NULL;

    if (instance->matrix != NULL)
    {
        CInfo.oldPoint = &Old;
        CInfo.newPoint = &New;

        temp = instance->matrix[36].t;

        Old.vx = temp[0];
        Old.vy = temp[1];
        Old.vz = temp[2];

        temp = instance->matrix[5].t;

        New.vx = temp[0];
        New.vy = temp[1];
        New.vz = temp[2];

        gameTrackerX.gameFlags |= 0x8000;
        PHYSICS_CheckLineInWorld(instance, &CInfo);
        gameTrackerX.gameFlags &= ~0x8000;
    }
}

uintptr_t ALUKABSS_Query(Instance *instance, unsigned long query)
{
    if (query != 0)
    {
        return MonsterQuery(instance, query);
    }

    return 0;
}

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_Message);

void ALUKABSS_Init(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    AlukabssVars *vars; // not from debug symbols

    MON_DefaultInit(instance);
    mv = (MonsterVars *)instance->extraData;

    if (mv != NULL)
    {
        vars = (AlukabssVars *)MEMPACK_Malloc(sizeof(AlukabssVars), MEMORY_TYPE_ALUKABSSDATA);

        if (vars == NULL)
        {
            mv->extraVars = NULL;
        }
        else
        {
            mv->extraVars = vars;
            vars->upper_facing = instance->rotation.z;
            vars->time_since_spit = MON_GetTime(instance);
            vars->raz_time_at_marker = vars->time_since_spit;
            vars->laugh_timer = 0;
        }
    }

    G2Anim_AttachControllerToSeg(&instance->anim, 8, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_AttachControllerToSeg(&instance->anim, 6, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_EnableController(&instance->anim, 8, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_EnableController(&instance->anim, 6, G2ANIM_CTRLRTYPE_ADD_LOCALROT);

    instance->xVel = 0;
    instance->yVel = 0;
    instance->zVel = 0;

    instance->xAccl = 0;
    instance->yAccl = 0;
    instance->zAccl = 0;

    mv->mvFlags |= 0x800;

    MON_SwitchState(instance, MONSTER_STATE_IDLE);
    RAZIEL_SetInteractiveMusic(SOUND_MODIFIER_BOSS_LOADED, 1);
}

void ALUKABSS_CleanUp(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols

    RAZIEL_SetInteractiveMusic(SOUND_MODIFIER_BOSS_LOADED, 0);

    mv = (MonsterVars *)instance->extraData;

    if (mv != NULL && mv->extraVars != NULL)
    {
        MEMPACK_Free(mv->extraVars);
    }

    G2Anim_DetachControllerFromSeg(&instance->anim, 8, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DetachControllerFromSeg(&instance->anim, 6, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    MON_CleanUp(instance);
}

void ALUKABSS_DamageEffect(Instance *instance, evFXHitData *data)
{

    SVector localloc;
    MonsterAttributes *attrs; // not from debug symbols
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    attrs = (MonsterAttributes *)instance->data;
    localloc = data->location;

    if (data == NULL)
    {
        if (mv->mvFlags & 0x400000)
        {
            if (objectAccess[10].object != NULL)
            {
                Model *model; // not from debug symbols
                model = ((Object *)objectAccess[10].object)->modelList[0];

                FX_MakeSpark(instance, model, attrs->leftShoulderSegment);
                FX_MakeSpark(instance, model, attrs->rightShoulderSegment);
                FX_MakeSpark(instance, model, attrs->waistSegment);
                FX_MakeSpark(instance, model, attrs->leftKneeSegment);
                FX_MakeSpark(instance, model, attrs->rightKneeSegment);
            }

            MONSTER_VertexBurnt(instance, &alukabssBurntTune);
        }
    }
    else
    {
        if (data->type == 0x20) // TODO: See if there's any enum for this
        {
            if (data->amount != 0)
            {
                MONSTER_StartVertexBurnt(instance, &data->location, &alukabssBurntTune);
            }
            else
            {
                MONSTER_StartVertexBurnt(instance, (SVector *)&instance->position, &alukabssBurntTune);
            }
        }
        else if (data->type == 0x10)
        {
            MONSTER_StartVertexBurnt(instance, (SVector *)&instance->position, &alukabssBurntTune);
        }
        else
        {
            SOUND_Play3dSound(&instance->position, 21, 650, 80, 15500);
            FX_MakeHitFX(&localloc);
        }
    }
}

void ALUKABSS_IdleEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (mv->extraVars != NULL)
    {
        MON_IdleEntry(instance);
        MON_PlayAnimIfNotPlaying(instance, MONSTER_ANIM_STANCE_HEALTHY, 2);
        mv->mvFlags |= 0x800;
    }
}

void ALUKABSS_Idle(Instance *instance)
{

    MonsterIR *enemy; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukabssVars *vars; // not from debug symbols
    AlukabssAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (AlukabssVars *)mv->extraVars;
    attrs = (AlukabssAttributes *)ma->tunData;
    enemy = mv->enemy;

    if (vars == NULL)
    {
        return;
    }

    instance->xVel = 0;
    instance->yVel = 0;
    instance->zVel = 0;

    COPY_SVEC(Position, &instance->position, Position, &instance->intro->position);

    if (mv->mvFlags & 4)
    {
        vars->raz_time_at_marker = vars->time_since_spit = MON_GetTime(instance);
        MON_Idle(instance);
    }
    else
    {
        if (enemy != NULL)
        {
            if (ALUKABSS_RotateToFace(instance, &gameTrackerX, &enemy->instance->position) != 0 && ALUKABSS_RazTimeAtMarker(instance) > (attrs->time_since_attack * 33))
            {
                MON_SwitchState(instance, MONSTER_STATE_COMBAT);
                vars->combat_state = MONSTER_STATE_PURSUE;
                MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 0, 1);
            }
            else if (mv->auxFlags & 1 && ALUKABSS_TimeSinceSpit(instance) > (attrs->time_since_spit * 33))
            {
                MON_SwitchState(instance, MONSTER_STATE_PROJECTILE);
            }
        }
        else
        {
            ALUKABSS_RotateToFace(instance, &gameTrackerX, 0);
        }
        MON_DefaultQueueHandler(instance);
    }
    ALUKABSS_SetUpWaterPlaneClip(instance);
}

void ALUKABSS_PursueEntry(Instance *instance)
{
    (void)instance;
}

void ALUKABSS_Pursue(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (mv->extraVars != NULL)
    {

        instance->xVel = 0;
        instance->yVel = 0;
        instance->zVel = 0;

        COPY_SVEC(Position, &instance->position, Position, &instance->intro->position);

        if (mv->mvFlags & 4)
        {
            MON_GroundMoveQueueHandler(instance);
        }
        else
        {
            MON_SwitchState(instance, mv->previousMainState);
            MON_DefaultQueueHandler(instance);
        }

        ALUKABSS_SetUpWaterPlaneClip(instance);
    }
}

void ALUKABSS_CombatEntry(Instance *instance)
{
    (void)instance;
}

void ALUKABSS_Combat(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukabssVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (AlukabssVars *)mv->extraVars;

    if (vars == NULL)
    {
        return;
    }

    switch (vars->combat_state)
    {
    case 1:
        if (instance->flags2 & 0x10)
        {
            vars->combat_state = 2;
            MON_PlayAnimFromList(instance, ma->auxAnimList, MONSTER_ANIM_HIT2, 2);
            ALUKABSS_InitCircle(instance);
        }
        break;
    case 2:
        if (ALUKABSS_ShouldAttack(instance))
        {
            vars->combat_state = 3;
            MON_SwitchState(instance, MONSTER_STATE_ATTACK);
        }
        else
        {
            ALUKABSS_Circle(instance, &gameTrackerX, 1);
        }
        break;
    case 3:
        vars->combat_state = 4;
        MON_PlayAnimFromList(instance, ma->auxAnimList, MONSTER_ANIM_RUN, 1);
        break;
    case 4:
        if (instance->flags2 & 0x10)
        {
            vars->raz_time_at_marker = vars->time_since_spit = MON_GetTime(instance);
            vars->combat_state = 0;
            MON_PlayAnim(instance, MONSTER_ANIM_STANCE_HEALTHY, 2);
            MON_SwitchState(instance, MONSTER_STATE_IDLE);
        }
        break;
    }

    instance->xVel = 0;
    instance->yVel = 0;
    instance->zVel = 0;

    COPY_SVEC(Position, &instance->position, Position, &instance->intro->position);

    MON_DefaultQueueHandler(instance);
    ALUKABSS_SetUpWaterPlaneClip(instance);
}

void ALUKABSS_AttackEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;

    if (mv->mvFlags & 4)
    {
        MON_SwitchStateDoEntry(instance, MONSTER_STATE_IDLE);
    }

    mv->attackType = &ma->attackAttributesList[(int)mv->subAttr->combatAttributes->attackList[0]];
    mv->mode = 0x200000;

    MON_PlayAnimFromList(instance, mv->attackType->animList, 0, 1);
}

void ALUKABSS_Attack(Instance *instance)
{

    MonsterAttackAttributes *attack; // not from debug symbols
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    attack = mv->attackType;

    if (mv->mvFlags & 4)
    {
        MON_SwitchState(instance, MONSTER_STATE_IDLE);
    }
    else
    {
        if (MON_AnimPlayingFromList(instance, attack->animList, attack->sphereOnAnim) && G2EmulationInstanceQueryPassedFrame(instance, 0, attack->sphereOnFrame))
        {
            MON_TurnOnWeaponSphere(instance, attack->sphereSegment);
        }
        else if (MON_AnimPlayingFromList(instance, attack->animList, attack->sphereOffAnim) && G2EmulationInstanceQueryPassedFrame(instance, 0, attack->sphereOffFrame))
        {
            MON_TurnOffWeaponSpheres(instance);
        }

        if (instance->flags2 & 0x10)
        {
            MON_SwitchState(instance, MONSTER_STATE_COMBAT);
        }
    }

    MON_DefaultQueueHandler(instance);
    ALUKABSS_SetUpWaterPlaneClip(instance);
}

void ALUKABSS_LandInWaterEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    mv->mvFlags |= 0x800;
}

void ALUKABSS_LandInWater(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (mv == NULL || mv->extraVars == NULL)
    {
        return;
    }

    if (mv->previousMainState == MONSTER_STATE_FALL)
    {
        MON_SwitchState(instance, MONSTER_STATE_IDLE);
    }
    else
    {
        MON_SwitchState(instance, mv->previousMainState);
    }

    instance->xVel = 0;
    instance->yVel = 0;
    instance->zVel = 0;

    COPY_SVEC(Position, &instance->position, Position, &instance->intro->position);

    MON_DefaultQueueHandler(instance);
    ALUKABSS_SetUpWaterPlaneClip(instance);
}

void ALUKABSS_ProjectileEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    MonsterCombatAttributes *combat; // not from debug symbols
    MonsterMissile *missile; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    combat = mv->subAttr->combatAttributes;
    missile = &ma->missileList[(int)combat->missileAttack];

    if (mv != NULL && mv->extraVars != NULL)
    {
        MON_PlayAnimFromList(instance, missile->animList, 0, 1);
    }
}

void ALUKABSS_Projectile(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    MonsterMissile *missile; // not from debug symbols
    AlukabssVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    missile = &ma->missileList[(int)mv->subAttr->combatAttributes->missileAttack];

    if (mv != NULL)
    {
        vars = (AlukabssVars *)mv->extraVars;

        if (vars != NULL)
        {
            if (mv->enemy == NULL || instance->flags2 & 0x10)
            {
                MON_SwitchState(instance, MONSTER_STATE_IDLE);
                vars->time_since_spit = MON_GetTime(instance);
            }
            else if (MON_AnimPlayingFromList(instance, missile->animList, missile->anim) && G2EmulationInstanceQueryPassedFrame(instance, 0, missile->frame))
            {
                MISSILE_FireAtInstance(instance, missile, mv->enemy->instance);
            }

            ALUKABSS_RotateToFace(instance, &gameTrackerX, NULL);

            instance->xVel = 0;
            instance->yVel = 0;
            instance->zVel = 0;

            COPY_SVEC(Position, &instance->position, Position, &instance->intro->position);

            MON_DefaultQueueHandler(instance);
            ALUKABSS_SetUpWaterPlaneClip(instance);
        }
    }
}

void ALUKABSS_DeadEntry(Instance *instance)
{
    ((MonsterVars *)instance->extraData)->soulJuice = 0;
    MON_DeadEntry(instance);
}

void ALUKABSS_Dead(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (mv->causeOfDeath == MONSTER_CAUSEOFDEATH_FIRE && instance->perVertexColor == NULL)
    {
        Model *model; // not from debug symbols
        CVECTOR *ptr; // not from debug symbols
        int i; // not from debug symbols

        model = instance->object->modelList[instance->currentModel];
        ptr = (CVECTOR *)MEMPACK_Malloc(sizeof(CVECTOR) * model->numVertices, MEMORY_TYPE_INSTVTXCOLOR);
        instance->perVertexColor = ptr;

        for (i = model->numVertices; i != 0; i--, ptr++)
        {
            *(long *)ptr = 0x101010;
        }
    }

    if (mv->mvFlags & 0x400000 && mv->effectTimer < MON_GetTime(instance))
    {
        mv->mvFlags &= ~0x400000;
        FX_StopAllGlowEffects(instance, 64);
    }

    while (DeMessageQueue(&mv->messageQueue) != NULL);
}

void ALUKABSS_DoNothingEntry(Instance *instance)
{
    MON_SwitchStateDoEntry(instance, ((MonsterVars *)instance->extraData)->previousMainState);
}

void ALUKABSS_DoNothing(Instance *instance)
{
    (void)instance;
}

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


int ALUKABSS_RotateToFace(Instance *, GameTracker *, Position *) {};

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

int ALUKABSS_ShouldAttack(Instance *instance) {};

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

void ALUKABSS_Circle(Instance *instance, GameTracker *gameTracker, int change_anim)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukabssAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    attrs = (AlukabssAttributes *)ma->tunData;

    instance->rotation.z = instance->rotation.z + mv->speed;
    mv->speed += (attrs->circle_accel * gameTracker->timeMult) / 4096;

    if (mv->speed >= attrs->fast_circle_speed && change_anim)
    {
        MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, MONSTER_ANIM_WALK, 2);
    }

    if (mv->speed > attrs->max_circle_speed)
    {
        mv->speed = attrs->max_circle_speed;
    }
}

void ALUKABSS_SetUpWaterPlaneClip(Instance *instance)
{

    PCollideInfo CInfo;
    SVECTOR Old;
    SVECTOR New;
    long *temp; // not from debug symbols

    instance->waterFace = NULL;
    instance->waterFaceTerrain = NULL;

    if (instance->matrix != NULL)
    {
        CInfo.oldPoint = &Old;
        CInfo.newPoint = &New;

        temp = instance->matrix[36].t;

        Old.vx = temp[0];
        Old.vy = temp[1];
        Old.vz = temp[2];

        temp = instance->matrix[5].t;

        New.vx = temp[0];
        New.vy = temp[1];
        New.vz = temp[2];

        gameTrackerX.gameFlags |= 0x8000;
        PHYSICS_CheckLineInWorld(instance, &CInfo);
        gameTrackerX.gameFlags &= ~0x8000;
    }
}

uintptr_t ALUKABSS_Query(Instance *instance, unsigned long query)
{
    if (query != 0)
    {
        return MonsterQuery(instance, query);
    }

    return 0;
}

void ALUKABSS_Message(Instance *instance, unsigned long message, unsigned long data) {};

void ALUKABSS_Init(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    AlukabssVars *vars; // not from debug symbols

    MON_DefaultInit(instance);
    mv = (MonsterVars *)instance->extraData;

    if (mv != NULL)
    {
        vars = (AlukabssVars *)MEMPACK_Malloc(sizeof(AlukabssVars), MEMORY_TYPE_ALUKABSSDATA);

        if (vars == NULL)
        {
            mv->extraVars = NULL;
        }
        else
        {
            mv->extraVars = vars;
            vars->upper_facing = instance->rotation.z;
            vars->time_since_spit = MON_GetTime(instance);
            vars->raz_time_at_marker = vars->time_since_spit;
            vars->laugh_timer = 0;
        }
    }

    G2Anim_AttachControllerToSeg(&instance->anim, 8, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_AttachControllerToSeg(&instance->anim, 6, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_EnableController(&instance->anim, 8, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_EnableController(&instance->anim, 6, G2ANIM_CTRLRTYPE_ADD_LOCALROT);

    instance->xVel = 0;
    instance->yVel = 0;
    instance->zVel = 0;

    instance->xAccl = 0;
    instance->yAccl = 0;
    instance->zAccl = 0;

    mv->mvFlags |= 0x800;

    MON_SwitchState(instance, MONSTER_STATE_IDLE);
    RAZIEL_SetInteractiveMusic(SOUND_MODIFIER_BOSS_LOADED, 1);
}

void ALUKABSS_CleanUp(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols

    RAZIEL_SetInteractiveMusic(SOUND_MODIFIER_BOSS_LOADED, 0);

    mv = (MonsterVars *)instance->extraData;

    if (mv != NULL && mv->extraVars != NULL)
    {
        MEMPACK_Free(mv->extraVars);
    }

    G2Anim_DetachControllerFromSeg(&instance->anim, 8, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DetachControllerFromSeg(&instance->anim, 6, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    MON_CleanUp(instance);
}

void ALUKABSS_DamageEffect(Instance *instance, evFXHitData *data)
{

    SVector localloc;
    MonsterAttributes *attrs; // not from debug symbols
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    attrs = (MonsterAttributes *)instance->data;
    localloc = data->location;

    if (data == NULL)
    {
        if (mv->mvFlags & 0x400000)
        {
            if (objectAccess[10].object != NULL)
            {
                Model *model; // not from debug symbols
                model = ((Object *)objectAccess[10].object)->modelList[0];

                FX_MakeSpark(instance, model, attrs->leftShoulderSegment);
                FX_MakeSpark(instance, model, attrs->rightShoulderSegment);
                FX_MakeSpark(instance, model, attrs->waistSegment);
                FX_MakeSpark(instance, model, attrs->leftKneeSegment);
                FX_MakeSpark(instance, model, attrs->rightKneeSegment);
            }

            MONSTER_VertexBurnt(instance, &alukabssBurntTune);
        }
    }
    else
    {
        if (data->type == 0x20)
        {
            if (data->amount != 0)
            {
                MONSTER_StartVertexBurnt(instance, &data->location, &alukabssBurntTune);
            }
            else
            {
                MONSTER_StartVertexBurnt(instance, (SVector *)&instance->position, &alukabssBurntTune);
            }
        }
        else if (data->type == 0x10)
        {
            MONSTER_StartVertexBurnt(instance, (SVector *)&instance->position, &alukabssBurntTune);
        }
        else
        {
            SOUND_Play3dSound(&instance->position, 21, 650, 80, 15500);
            FX_MakeHitFX(&localloc);
        }
    }
}

void ALUKABSS_IdleEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (mv->extraVars != NULL)
    {
        MON_IdleEntry(instance);
        MON_PlayAnimIfNotPlaying(instance, MONSTER_ANIM_STANCE_HEALTHY, 2);
        mv->mvFlags |= 0x800;
    }
}

void ALUKABSS_Idle(Instance *instance)
{

    MonsterIR *enemy; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukabssVars *vars; // not from debug symbols
    AlukabssAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (AlukabssVars *)mv->extraVars;
    attrs = (AlukabssAttributes *)ma->tunData;
    enemy = mv->enemy;

    if (vars == NULL)
    {
        return;
    }

    instance->xVel = 0;
    instance->yVel = 0;
    instance->zVel = 0;

    COPY_SVEC(Position, &instance->position, Position, &instance->intro->position);

    if (mv->mvFlags & 4)
    {
        vars->raz_time_at_marker = vars->time_since_spit = MON_GetTime(instance);
        MON_Idle(instance);
    }
    else
    {
        if (enemy != NULL)
        {
            if (ALUKABSS_RotateToFace(instance, &gameTrackerX, &enemy->instance->position) != 0 && ALUKABSS_RazTimeAtMarker(instance) > (attrs->time_since_attack * 33))
            {
                MON_SwitchState(instance, MONSTER_STATE_COMBAT);
                vars->combat_state = MONSTER_STATE_PURSUE;
                MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 0, 1);
            }
            else if (mv->auxFlags & 1 && ALUKABSS_TimeSinceSpit(instance) > (attrs->time_since_spit * 33))
            {
                MON_SwitchState(instance, MONSTER_STATE_PROJECTILE);
            }
        }
        else
        {
            ALUKABSS_RotateToFace(instance, &gameTrackerX, 0);
        }
        MON_DefaultQueueHandler(instance);
    }
    ALUKABSS_SetUpWaterPlaneClip(instance);
}

void ALUKABSS_PursueEntry(Instance *instance) {};

void ALUKABSS_Pursue(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    if (mv->extraVars != NULL)
    {

        instance->xVel = 0;
        instance->yVel = 0;
        instance->zVel = 0;

        COPY_SVEC(Position, &instance->position, Position, &instance->intro->position);

        if (mv->mvFlags & 4)
        {
            MON_GroundMoveQueueHandler(instance);
        }
        else
        {
            MON_SwitchState(instance, mv->previousMainState);
            MON_DefaultQueueHandler(instance);
        }

        ALUKABSS_SetUpWaterPlaneClip(instance);
    }
}

void ALUKABSS_CombatEntry(Instance *instance) {};

void ALUKABSS_Combat(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukabssVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (AlukabssVars *)mv->extraVars;

    if (vars == NULL)
    {
        return;
    }

    switch (vars->combat_state)
    {
    case 1:
        if (instance->flags2 & 0x10)
        {
            vars->combat_state = 2;
            MON_PlayAnimFromList(instance, ma->auxAnimList, MONSTER_ANIM_HIT2, 2);
            ALUKABSS_InitCircle(instance);
        }
        break;
    case 2:
        if (ALUKABSS_ShouldAttack(instance))
        {
            vars->combat_state = 3;
            MON_SwitchState(instance, MONSTER_STATE_ATTACK);
        }
        else
        {
            ALUKABSS_Circle(instance, &gameTrackerX, 1);
        }
        break;
    case 3:
        vars->combat_state = 4;
        MON_PlayAnimFromList(instance, ma->auxAnimList, MONSTER_ANIM_RUN, 1);
        break;
    case 4:
        if (instance->flags2 & 0x10)
        {
            vars->raz_time_at_marker = vars->time_since_spit = MON_GetTime(instance);
            vars->combat_state = 0;
            MON_PlayAnim(instance, MONSTER_ANIM_STANCE_HEALTHY, 2);
            MON_SwitchState(instance, MONSTER_STATE_IDLE);
        }
        break;
    }

    instance->xVel = 0;
    instance->yVel = 0;
    instance->zVel = 0;

    COPY_SVEC(Position, &instance->position, Position, &instance->intro->position);

    MON_DefaultQueueHandler(instance);
    ALUKABSS_SetUpWaterPlaneClip(instance);
}

void ALUKABSS_AttackEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;

    if (mv->mvFlags & 4)
    {
        MON_SwitchStateDoEntry(instance, MONSTER_STATE_IDLE);
    }

    mv->attackType = &ma->attackAttributesList[(int)mv->subAttr->combatAttributes->attackList[0]];
    mv->mode = 0x200000;

    MON_PlayAnimFromList(instance, mv->attackType->animList, 0, 1);
}

void ALUKABSS_Attack(Instance *instance)
{

    MonsterAttackAttributes *attack; // not from debug symbols
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    attack = mv->attackType;

    if (mv->mvFlags & 4)
    {
        MON_SwitchState(instance, MONSTER_STATE_IDLE);
    }
    else
    {
        if (MON_AnimPlayingFromList(instance, attack->animList, attack->sphereOnAnim) && G2EmulationInstanceQueryPassedFrame(instance, 0, attack->sphereOnFrame))
        {
            MON_TurnOnWeaponSphere(instance, attack->sphereSegment);
        }
        else if (MON_AnimPlayingFromList(instance, attack->animList, attack->sphereOffAnim) && G2EmulationInstanceQueryPassedFrame(instance, 0, attack->sphereOffFrame))
        {
            MON_TurnOffWeaponSpheres(instance);
        }

        if (instance->flags2 & 0x10)
        {
            MON_SwitchState(instance, MONSTER_STATE_COMBAT);
        }
    }

    MON_DefaultQueueHandler(instance);
    ALUKABSS_SetUpWaterPlaneClip(instance);
}

void ALUKABSS_LandInWaterEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    mv->mvFlags |= 0x800;
}

void ALUKABSS_LandInWater(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (mv == NULL || mv->extraVars == NULL)
    {
        return;
    }

    if (mv->previousMainState == MONSTER_STATE_FALL)
    {
        MON_SwitchState(instance, MONSTER_STATE_IDLE);
    }
    else
    {
        MON_SwitchState(instance, mv->previousMainState);
    }

    instance->xVel = 0;
    instance->yVel = 0;
    instance->zVel = 0;

    COPY_SVEC(Position, &instance->position, Position, &instance->intro->position);

    MON_DefaultQueueHandler(instance);
    ALUKABSS_SetUpWaterPlaneClip(instance);
}

void ALUKABSS_ProjectileEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    MonsterCombatAttributes *combat; // not from debug symbols
    MonsterMissile *missile; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    combat = mv->subAttr->combatAttributes;
    missile = &ma->missileList[(int)combat->missileAttack];

    if (mv != NULL && mv->extraVars != NULL)
    {
        MON_PlayAnimFromList(instance, missile->animList, 0, 1);
    }
}

void ALUKABSS_Projectile(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    MonsterMissile *missile; // not from debug symbols
    AlukabssVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    missile = &ma->missileList[(int)mv->subAttr->combatAttributes->missileAttack];

    if (mv != NULL)
    {
        vars = (AlukabssVars *)mv->extraVars;

        if (vars != NULL)
        {
            if (mv->enemy == NULL || instance->flags2 & 0x10)
            {
                MON_SwitchState(instance, MONSTER_STATE_IDLE);
                vars->time_since_spit = MON_GetTime(instance);
            }
            else if (MON_AnimPlayingFromList(instance, missile->animList, missile->anim) && G2EmulationInstanceQueryPassedFrame(instance, 0, missile->frame))
            {
                MISSILE_FireAtInstance(instance, missile, mv->enemy->instance);
            }

            ALUKABSS_RotateToFace(instance, &gameTrackerX, NULL);

            instance->xVel = 0;
            instance->yVel = 0;
            instance->zVel = 0;

            COPY_SVEC(Position, &instance->position, Position, &instance->intro->position);

            MON_DefaultQueueHandler(instance);
            ALUKABSS_SetUpWaterPlaneClip(instance);
        }
    }
}

void ALUKABSS_DeadEntry(Instance *instance)
{
    ((MonsterVars *)instance->extraData)->soulJuice = 0;
    MON_DeadEntry(instance);
}

void ALUKABSS_Dead(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (mv->causeOfDeath == MONSTER_CAUSEOFDEATH_FIRE && instance->perVertexColor == NULL)
    {
        Model *model; // not from debug symbols
        CVECTOR *ptr; // not from debug symbols
        int i; // not from debug symbols

        model = instance->object->modelList[instance->currentModel];
        ptr = (CVECTOR *)MEMPACK_Malloc(sizeof(CVECTOR) * model->numVertices, MEMORY_TYPE_INSTVTXCOLOR);
        instance->perVertexColor = ptr;

        for (i = model->numVertices; i != 0; i--, ptr++)
        {
            *(long *)ptr = 0x101010;
        }
    }

    if (mv->mvFlags & 0x400000 && mv->effectTimer < MON_GetTime(instance))
    {
        mv->mvFlags &= ~0x400000;
        FX_StopAllGlowEffects(instance, 64);
    }

    while (DeMessageQueue(&mv->messageQueue) != NULL);
}

void ALUKABSS_DoNothingEntry(Instance *instance)
{
    MON_SwitchStateDoEntry(instance, ((MonsterVars *)instance->extraData)->previousMainState);
}

void ALUKABSS_DoNothing(Instance *instance) {};

#endif
