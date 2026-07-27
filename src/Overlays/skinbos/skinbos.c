#include "Overlays/skinbos/skinbos.h"
#include "Game/COLLIDE.h"
#include "Game/DEBUG.h"
#include "Game/FX.h"
#include "Game/GAMELOOP.h"
#include "Game/GAMEPAD.h"
#include "Game/MATH3D.h"
#include "Game/MEMPACK.h"
#include "Game/PLAN/ENMYPLAN.h"
#include "Game/PLAN/PLANAPI.h"
#include "Game/RAZIEL/RAZIEL.h"
#include "Game/SOUND.h"
#include "Game/STATE.h"
#include "Game/STREAM.h"
#include "Game/MONSTER/MONAPI.h"
#include "Game/MONSTER/MONLIB.h"
#include "Game/MONSTER/MONMSG.h"
#include "Game/MONSTER/MONSTER.h"
#include "Game/PSX/SUPPORT.h"

// TODO: double-check that SKINBOS_CheckInsideMasher and SKINBOS_ShouldEscapeJail aren't swapped

int SKINBOS_HandleOneShotAnims(Instance *instance); // TODO: Delete once matched

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

void SKINBOS_DoPhaseFade(Instance *instance, int limit)
{

    int chance; // not from debug symbols
    int lowerBound; // not from debug symbols
    int upperBound; // not from debug symbols
    int phaseLevel; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    SkinbosVars *vars; // not from debug symbols
    SkinbosAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (SkinbosVars *)mv->extraVars;
    attrs = (SkinbosAttributes *)ma->tunData;
    phaseLevel = vars->phase_level;

    switch (vars->num_hits)
    {
    case 0:
        upperBound = 15291;
        lowerBound = 0;
        break;
    case 1:
        upperBound = ((attrs->low_backstep + 7) * 32767) / 30;
        lowerBound = (attrs->low_backstep * 32767) / 30;
        break;
    case 2:
        upperBound = ((attrs->mid_backstep + 7) * 32767) / 60;
        lowerBound = (attrs->mid_backstep * 32767) / 60;
        break;
    default:
        upperBound = ((attrs->high_backstep + 7) * 32767) / 90;
        lowerBound = (attrs->high_backstep * 32767) / 90;
        break;
    }

    if (limit == 0)
    {
        upperBound = upperBound ^ lowerBound;
        lowerBound = upperBound ^ lowerBound;
        upperBound = upperBound ^ lowerBound;
    }

    chance = rand();

    if (chance < upperBound)
    {
        phaseLevel++;
    }
    else
    {

        chance = 32767 - chance;

        if (chance < lowerBound)
        {
            phaseLevel--;

            if (phaseLevel >= 0)
            {
                vars->phase_level = phaseLevel;
            }
        }
    }

    if (phaseLevel > 7)
    {
        phaseLevel = 7;
    }
    else if (phaseLevel < 0)
    {
        phaseLevel = 0;
    }

    vars->phase_level = phaseLevel;

    if ((gameTrackerX.gameData.asmData.MorphTime > 500 && gameTrackerX.gameData.asmData.MorphType == 1) || (gameTrackerX.gameData.asmData.MorphTime < 500 && gameTrackerX.gameData.asmData.MorphType == 0))
    {
        mv->targetFade = ((attrs->max_fadeout * phaseLevel) / 7) + (4096 - attrs->max_fadeout);

        if (phaseLevel == 7)
        {
            instance->flags &= ~0x1000;
        }
        else
        {
            instance->flags |= 0x1000;
        }
    }
    else
    {
        mv->targetFade = (attrs->max_fadeout * phaseLevel) / 7;

        if (phaseLevel == 0)
        {
            instance->flags &= ~0x1000;
        }
        else
        {
            instance->flags |= 0x1000;
        }
    }

    if (phaseLevel == limit)
    {
        vars->anim_state = 0;

        if (limit == 7)
        {
            FX_StartPassthruFX(instance, &vars->normal, &vars->point);
            vars->phase_hit_timer = MON_GetTime(instance) + (attrs->min_phase_time * 33);
        }
        else
        {
            FX_EndPassthruFX(instance);
            SKINBOS_CheckInsideMasher(instance);
        }
    }
}

INCLUDE_RODATA("asm/nonmatchings/Overlays/skinbos/skinbos", D_88000000);

INCLUDE_RODATA("asm/nonmatchings/Overlays/skinbos/skinbos", D_88000020);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_HandleOneShotAnims);

void SKINBOS_DoPhasingOutInit(Instance *instance, SVector *normal, SVector *point)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    SkinbosVars *vars; // not from debug symbols
    SkinbosAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (SkinbosVars *)mv->extraVars;
    attrs = (SkinbosAttributes *)ma->tunData;

    if (vars->num_hits >= attrs->max_allowed_damage && mv->auxFlags & 8 && vars->anim_state != 12)
    {
        MON_PlayAnim(instance, 1, 1);
        vars->anim_state = 12;
        mv->mvFlags &= ~0x20000;
        vars->last_hit_timer = MON_GetTime(instance) + 6930;
        MON_GetPlanSlot(mv);
    }

    if (vars->phase_level == 0 && vars->anim_state == 0)
    {

        MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 9, 1);

        if (normal != NULL && point != NULL)
        {
            COPY_SVEC(SVector, &vars->normal, SVector, normal);
            COPY_SVEC(SVector, &vars->point, SVector, point);
        }

        vars->anim_state = 10;
        mv->mvFlags &= ~0x20000;
    }
}

void SKINBOS_CheckPhaseIn(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    SkinbosVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (SkinbosVars *)mv->extraVars;

    if (!(mv->auxFlags & 4) && vars->phase_level == 7 && MON_GetTime(instance) >= (unsigned long)vars->phase_hit_timer)
    {
        MON_PlayCombatIdle(instance, 2);
        vars->anim_state = 11;
        mv->mvFlags &= ~0x20000;
    }

    mv->auxFlags &= ~4;
}

int SKINBOS_ShouldEscapeJail(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (mv->auxFlags & 8 && !SKINBOS_CheckPointInsideMasher(instance, &gameTrackerX.playerInstance->position, 0))
    {
        return 1;
    }

    return 0;
}

void SKINBOS_ProcessGateHitBlood(Instance *instance, int vertidx, int segidx, int dist, void *cb_data)
{

    int range; // not from debug symbols
    int scl; // not from debug symbols
    SkinbosVars *vars; // not from debug symbols
    CVECTOR *cv; // not from debug symbols
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (SkinbosVars *)mv->extraVars;

    if (vars != NULL)
    {

        range = vars->num_hits * 200;

        if (dist < ((bloodyMessType *)cb_data)->closestdist)
        {
            ((bloodyMessType *)cb_data)->closestvert = vertidx;
            ((bloodyMessType *)cb_data)->closestdist = dist;
            ((bloodyMessType *)cb_data)->closestseg = segidx;
        }

        if (dist < range)
        {

            cv = &instance->perVertexColor[vertidx];

            scl = (2 * (range - dist) * ((bloodyMessType *)cb_data)->bloodIntensity) / range;
            scl = MIN(scl, 256);

            cv->r = ~(((255 - mv->subAttr->bruiseRed) * scl) >> 8);
            cv->g = ~(((255 - mv->subAttr->bruiseGreen) * scl) >> 8);
            cv->b = ~(((255 - mv->subAttr->bruiseBlue) * scl) >> 8);
            cv->cd = 1;

            ((bloodyMessType *)cb_data)->bloodiedAVert = 1;
        }
    }
}

int SKINBOS_StartVertexBlood(Instance *instance)
{

    bloodyMessType bmt; // not from debug symbols
    Position pos;
    MonsterVars *mv; // not from debug symbols
    Model *model; // not from debug symbols


    mv = (MonsterVars *)instance->extraData;
    model = instance->object->modelList[instance->currentModel];

    if (mv->extraVars == NULL)
    {
        return -1;
    }

    if (instance->perVertexColor == NULL)
    {
        MONSTER_InitVertexColors(instance, model);

        if (instance->perVertexColor == NULL)
        {
            return -1;
        }
    }

    pos.x = instance->position.x;
    pos.y = instance->position.y;
    pos.z = instance->position.z + 640;

    bmt.closestvert = -1;
    bmt.closestdist = 65536;
    bmt.closestseg = -1;
    bmt.bloodiedAVert = 0;
    bmt.bloodIntensity = 256;

    MONSTER_ProcessClosestVerts(instance, (SVector *)&pos, SKINBOS_ProcessGateHitBlood, &bmt);

    return bmt.closestvert;
}

const char D_88000060[] = "skgate__"; // TODO: Remove once rest of rodata is brought in

void SKINBOS_Collide(Instance *instance, GameTracker *gameTracker)
{
    int isTargetAnimState; // not from debug symbols
    int didCollide; // not from debug symbols
    int faceFlags; // not from debug symbols
    CollideInfo *collideInfo; // not from debug symbols
    SkinbosVars *vars; // not from debug symbols
    HSphere *hsphere; // not from debug symbols
    MonsterVars *mv; // not from debug symbols

    (void)gameTracker;

    mv = (MonsterVars *)instance->extraData;
    vars = (SkinbosVars *)mv->extraVars;
    collideInfo = instance->collideInfo;
    hsphere = (HSphere *)collideInfo->prim0;
    isTargetAnimState = vars->anim_state > 5 && vars->anim_state < 10;
    didCollide = 0;

    if (vars == NULL)
    {
        MonsterCollide(instance, &gameTrackerX);
    }

    if (collideInfo->type1 == 1)
    {
        collideInfo->offset.x = 0;
        collideInfo->offset.y = 0;
        collideInfo->offset.z = 0;
    }

    if ((vars->phase_level > 0 || isTargetAnimState) && hsphere->id != 9)
    {
        if (instance->data == NULL)
        {
            return;
        }

        switch (collideInfo->type1)
        {
        case 4:
            break;
        case 1:
        {
            evMonsterHitObjectData *data; // not from debug symbols

            data = (evMonsterHitObjectData *)CIRC_Alloc(sizeof(evMonsterHitObjectData));
            data->instance = (Instance *)collideInfo->inst1;
            data->hitType = 1;
            didCollide = 1;
            break;
        }
        case 2:
        {
            evMonsterHitObjectData *data; // not from debug symbols

            data = (evMonsterHitObjectData *)CIRC_Alloc(sizeof(evMonsterHitObjectData));
            data->instance = (Instance *)collideInfo->inst1;
            data->hitType = 2;
            didCollide = 1;
            break;
        }
        case 5:
        {
            evMonsterHitObjectData *data; // not from debug symbols

            data = (evMonsterHitObjectData *)CIRC_Alloc(sizeof(evMonsterHitObjectData));
            data->instance = (Instance *)collideInfo->inst1;
            data->hitType = 5;

            if (strcmpi(data->instance->object->name, D_88000060) != 0)
            {
                didCollide = 1;
            }
            else
            {
                mv->auxFlags |= 4;
            }
            break;
        }
        case 3:
        {
            Level *level; // not from debug symbols
            Terrain *terrain; // not from debug symbols
            TFace *tface; // not from debug symbols
            evMonsterHitTerrainData *data; // not from debug symbols

            data = (evMonsterHitTerrainData *)CIRC_Alloc(sizeof(evMonsterHitTerrainData));
            tface = (TFace *)collideInfo->prim1;

            if (tface->textoff != 0xFFFF)
            {
                level = (Level *)collideInfo->level;
                terrain = level->terrain;
                faceFlags = ((TextureFT3 *)((char *)terrain->StartTextureList + tface->textoff))->attr;
            }
            else
            {
                faceFlags = 0;
            }

            data->faceFlags = faceFlags;
            COLLIDE_FindCollisionFaceNormal(collideInfo, &data->normal);
            data->tface = (TFace *)collideInfo->prim1;

            if (!(data->faceFlags & 0x1000))
            {
                if (!isTargetAnimState)
                {
                    didCollide = 1;
                    if (COLLIDE_FindCollisionFaceNormal(collideInfo, &data->normal))
                    {
                        collideInfo->offset.x += data->normal.x >> 10;
                        collideInfo->offset.y += data->normal.y >> 10;
                    }
                }
            }
            else
            {
                mv->auxFlags |= 4;
            }
            break;
        }
        }

        if (didCollide)
        {
            mv->mvFlags |= 8;
            instance->position.x += collideInfo->offset.x;
            instance->position.y += collideInfo->offset.y;
            COLLIDE_UpdateAllTransforms(instance, (SVECTOR *)&collideInfo->offset);
        }
    }
    else
    {
        MonsterCollide(instance, &gameTrackerX);
    }
}

void SKINBOS_DamageEffect(Instance *instance, evFXHitData *data)
{
    SVector localloc;

    if (data != NULL)
    {
        localloc = data->location;
        FX_MakeHitFX(&localloc);
        SOUND_Play3dSound(&instance->position, 21, 650, 80, 15500);
    }
}

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_Message);

uintptr_t SKINBOS_Query(Instance *instance, unsigned long query)
{

    uintptr_t ret; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    SkinbosVars *vars; // not from debug symbols
    SkinbosAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (SkinbosVars *)mv->extraVars;
    attrs = (SkinbosAttributes *)ma->tunData;

    if (vars == NULL)
    {
        return MonsterQuery(instance, query);
    }

    switch (query)
    {
    case 0:
        ret = 0;
        break;
    case 30:
        ret = 0;

        if (mv->auxFlags & 8 && vars->num_hits >= attrs->max_allowed_damage)
        {
            ret = vars->phase_level == 0;
        }

        if (vars->gate_drop_pos == 3)
        {
            ret |= 2;
        }

        if (mv->auxFlags & 0x20)
        {
            ret = ret | 8;
        }

        break;
    default:
        ret = MonsterQuery(instance, query);
        break;
    }

    return ret;
}

void SKINBOS_Init(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    SkinbosVars *vars; // not from debug symbols

    MON_DefaultInit(instance);
    mv = (MonsterVars *)instance->extraData;
    instance->collideFunc = SKINBOS_Collide;

    if (mv != NULL)
    {
        vars = (SkinbosVars *)MEMPACK_Malloc(sizeof(SkinbosVars), MEMORY_TYPE_SKINBOSDATA);

        if (vars == NULL)
        {
            mv->extraVars = NULL;
        }
        else
        {
            mv->extraVars = vars;
            vars->num_hits = 0;
            vars->phase_level = 0;
            vars->anim_state = 0;
            vars->last_hit_timer = 0;
        }
    }

    mv->auxFlags |= 0x20000000;
    gameTrackerX.gameFlags |= 0x80000000;
    RAZIEL_SetInteractiveMusic(SOUND_MODIFIER_BOSS_LOADED, 1);
    DEBUG_DoAreaProtection();
}

void SKINBOS_CleanUp(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    RAZIEL_SetInteractiveMusic(SOUND_MODIFIER_BOSS_LOADED, 0);
    gameTrackerX.gameFlags &= ~0x80000000;

    if (mv != NULL && mv->extraVars != NULL)
    {
        MEMPACK_Free(mv->extraVars);
    }

    MON_CleanUp(instance);
}

void SKINBOS_IdleEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    SkinbosVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (SkinbosVars *)mv->extraVars;

    if (vars != NULL)
    {

        mv->mvFlags &= ~0x1000;
        mv->mvFlags &= ~0x40000;
        mv->mode = 1;

        if (mv->mvFlags & 4)
        {
            MON_PlayAnimID(instance, (signed char)((MonsterAttributes *)instance->data)->idleList[0].anim, 2);
            vars->anim_state = 0;
            return;
        }

        if (vars->anim_state != 10 && vars->anim_state != 11)
        {
            vars->anim_state = 0;
        }

        MON_PlayCombatIdle(instance, 2);
    }
}

void SKINBOS_Idle(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    SkinbosVars *vars; // not from debug symbols
    SkinbosAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (SkinbosVars *)mv->extraVars;
    attrs = (SkinbosAttributes *)ma->tunData;

    if (vars == NULL || attrs == NULL)
    {
        return;
    }

    if (mv->mvFlags & 4)
    {
        vars->phase_level = 0;

        if (gameTrackerX.gameData.asmData.MorphType == 0)
        {
            mv->targetFade = 0;
        }
        else
        {
            mv->targetFade = 0x1000;
        }

        vars->anim_state = 0;
        MON_Idle(instance);
        return;
    }

    if (mv->auxFlags & 0x40)
    {
        if (instance->fadeValue == 0x1000)
        {
            if (gameTrackerX.gameData.asmData.MorphTime < 1000 && gameTrackerX.gameData.asmData.MorphType == 0)
            {
                mv->targetFade = ((attrs->max_fadeout * vars->phase_level) / 7) + (0x1000 - attrs->max_fadeout);
            }
            else
            {
                mv->targetFade = (attrs->max_fadeout * vars->phase_level) / 7;
            }

            mv->auxFlags &= ~0x40;
        }
        else
        {
            if (gameTrackerX.gameData.asmData.MorphTime == 1000 && gameTrackerX.gameData.asmData.MorphType == 1)
            {
                mv->targetFade = ((attrs->max_fadeout * vars->phase_level) / 7) + (0x1000 - attrs->max_fadeout);
                mv->auxFlags &= ~0x40;
            }
            else if (gameTrackerX.gameData.asmData.MorphTime == 1000 && gameTrackerX.gameData.asmData.MorphType == 0)
            {
                mv->targetFade = (attrs->max_fadeout * vars->phase_level) / 7;
                mv->auxFlags &= ~0x40;
            }
        }
    }

    if (SKINBOS_HandleOneShotAnims(instance) != 0 || mv->auxFlags & 0x40)
    {
        MON_DefaultQueueHandler(instance);
        return;
    }

    MON_Idle(instance);
}

void SKINBOS_FindRandomNodeInUnit(Instance *instance)
{

    int i; // not from debug symbols
    int counter; // not from debug symbols
    PlanMkr *planMkr; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    Level *level; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    level = STREAM_GetLevelWithID(instance->currentStreamUnitID);

    if (level == NULL)
    {
        return;
    }

    counter = 0;

    for (i = level->NumberOfPlanMarkers, planMkr = level->PlanMarkerList; i > 0; i--, planMkr++)
    {
        if (!((planMkr->id & 0x8000) | (planMkr->id & 0x4000) | (planMkr->id & 0x2000) | (planMkr->id & 0x1000)))
        {
            counter++;
        }
    }

    counter = rand() % counter;

    for (i = level->NumberOfPlanMarkers, planMkr = level->PlanMarkerList; i > 0; i--, planMkr++)
    {
        if (!((planMkr->id & 0x8000) | (planMkr->id & 0x4000) | (planMkr->id & 0x2000) | (planMkr->id & 0x1000)))
        {
            counter--;

            if (counter <= 0)
            {
                COPY_SVEC(Position, &mv->destination, Position, &planMkr->pos);
                break;
            }
        }
    }
}

void SKINBOS_WanderEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    MON_WanderEntry(instance);

    if (!(mv->mvFlags & 4))
    {
        SKINBOS_FindRandomNodeInUnit(instance);
        mv->mvFlags |= 0x40000;
    }
}

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_Wander);

void SKINBOS_PursueEntry(Instance *instance)
{
    MON_PursueEntry(instance);
}


void SKINBOS_Pursue(Instance *instance)
{

    Position pos;
    int anim; // not from debug symbols
    int range; // not from debug symbols
    int result; // not from debug symbols
    MonsterIR *enemy; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    SkinbosVars *vars; // not from debug symbols
    MonsterCombatAttributes *combat; // not from debug symbols
    SkinbosAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (SkinbosVars *)mv->extraVars;
    enemy = mv->enemy;
    attrs = (SkinbosAttributes *)ma->tunData;
    combat = mv->subAttr->combatAttributes;

    if (vars == NULL)
    {
        return;
    }

    if (mv->mvFlags & 4)
    {
        vars->phase_level = 0;

        if (gameTrackerX.gameData.asmData.MorphType == 0)
        {
            mv->targetFade = 0;
        }
        else
        {
            mv->targetFade = 4096;
        }

        vars->anim_state = 0;
        MON_Pursue(instance);
        return;
    }

    if (SKINBOS_HandleOneShotAnims(instance) == 0)
    {
        if (SKINBOS_ShouldEscapeJail(instance) && !SKINBOS_CheckPointInsideMasher(instance, &gameTrackerX.playerInstance->position, 1))
        {
            MON_SwitchState(instance, MONSTER_STATE_FLEE);
        }
        else if (mv->auxFlags & 1)
        {
            switch (vars->gate_drop_pos)
            {

            case 1:
                MON_PlayAnim(instance, MONSTER_ANIM_STANCE_HEALTHY, 1);
                vars->stop_timer = MON_GetTime(instance) + 1980;
                vars->anim_state = 3;
                mv->mvFlags &= ~0x20000;
                break;
            case 2:
                break;
            case 3:
                MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 6, 1);
                vars->anim_state = 6;
                mv->mvFlags &= ~0x20000;

                if (vars->num_hits < attrs->max_allowed_damage)
                {
                    vars->num_hits++;
                }

                SKINBOS_StartVertexBlood(instance);
                break;
            }

            mv->auxFlags &= ~1;

        }
        else if (enemy == NULL)
        {
            MON_SwitchState(instance, MONSTER_STATE_IDLE);
        }
        else
        {

            mv->destination = enemy->instance->position;
            mv->lookAtPos = &mv->destination;
            range = combat->combatRange;

            if (MATH3D_LengthXY(instance->position.x - mv->destination.x, instance->position.y - mv->destination.y) < range && abs(instance->position.z - mv->destination.z) < 90 && enemy->mirFlags & 0x20)
            {
                MON_SwitchState(instance, MONSTER_STATE_COMBAT);
            }
            else
            {

                if (enemy->distance < range + MON_GetAnim(instance, mv->subAttr->animList, MONSTER_ANIM_WALK)->distance)
                {
                    anim = MONSTER_ANIM_WALK;
                }
                else
                {
                    anim = MONSTER_ANIM_RUN;
                }

                result = ENMYPLAN_MoveToTargetFinal(instance, &pos, (signed char)mv->pathSlotID, &mv->destination, 0x1F);

                if (result == MOVE_TO_WAYPOINT || result == MOVE_TO_TARGET)
                {

                    result = SKINBOS_Turn(instance, &pos, 1365);

                    switch (result)
                    {
                    case 1:
                        MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 1, 1);
                        vars->anim_state = result;
                        mv->mvFlags &= ~0x20000;
                        break;
                    case 2:
                        MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 0, 1);
                        vars->anim_state = result;
                        mv->mvFlags &= ~0x20000;
                        break;
                    case 0:
                    default:

                        MON_TurnToPosition(instance, &pos, mv->subAttr->speedPivotTurn);
                        result = SKINBOS_Turn(instance, &pos, 0);

                        switch (result)
                        {
                        case 1:
                            MON_PlayAnimFromListIfNotPlaying(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 3, 2);
                            vars->anim_state = 0;
                            mv->mvFlags &= ~0x20000;
                            break;
                        case 2:
                            MON_PlayAnimFromListIfNotPlaying(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 2, 2);
                            vars->anim_state = 0;
                            mv->mvFlags &= ~0x20000;
                            break;
                        case 0:
                            if (!(mv->mvFlags & 0x20000))
                            {
                                mv->mvFlags |= 0x20000;
                                MON_PlayAnimIfNotPlaying(instance, anim, 2);
                                vars->anim_state = 0;
                            }
                            break;
                        }
                        break;
                    }
                }
                else
                {
                    if (result == MOVE_INVALID)
                    {
                        MON_GetPlanSlot(mv);
                    }

                    MON_PlayCombatIdle(instance, 2);
                    mv->mvFlags &= ~0x20000;
                }
            }
        }
    }

    SKINBOS_CheckPhaseIn(instance);
    MON_DefaultQueueHandler(instance);
}

void SKINBOS_HitEntry(Instance *instance)
{

    MonsterIR *enemy; // not from debug symbols
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    enemy = mv->enemy;

    if (mv->extraVars == NULL)
    {
        return;
    }

    if (enemy != NULL)
    {
        enemy->mirConditions |= 0x400;
        enemy->mirFlags &= ~0x1000;

        if ((((instance->rotation.z - MATH3D_AngleFromPosToPos(&instance->position, &enemy->instance->position)) + 1024) & 0xFFF) < 2048)
        {
            MON_TurnToPosition(instance, &enemy->instance->position, 4096);
            MON_PlayAnim(instance, MONSTER_ANIM_HIT1, 1);
        }
        else
        {
            MON_PlayAnim(instance, MONSTER_ANIM_HIT2, 1);
        }
    }
    else
    {
        MON_PlayAnim(instance, MONSTER_ANIM_HIT1, 1);
    }

    mv->mode = 0x8000;
}

void SKINBOS_Hit(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    SkinbosVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (SkinbosVars *)mv->extraVars;

    if (vars != NULL)
    {
        if (vars->anim_state == 10 || vars->anim_state == 11)
        {
            SKINBOS_HandleOneShotAnims(instance);
        }
        else
        {
            vars->anim_state = 0;
        }

        if (instance->flags2 & 0x10)
        {
            MON_PlayCombatIdle(instance, 2);
            MON_SwitchState(instance, MONSTER_STATE_COMBAT);
        }

        MON_DefaultQueueHandler(instance);
    }
}

void SKINBOS_CombatEntry(Instance *instance)
{
    MON_CombatEntry(instance);
}

void SKINBOS_Combat(Instance *instance)
{

    int turnResult; // not from debug symbols
    int reason; // not from debug symbols
    MonsterIR *enemy; // not from debug symbols
    MonsterCombatAttributes *combat; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    SkinbosVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (SkinbosVars *)mv->extraVars;
    enemy = mv->enemy;

    if (vars == NULL)
    {
        return;
    }

    if (mv->mvFlags & 4)
    {
        MON_SwitchState(instance, MONSTER_STATE_IDLE);
    }

    if (SKINBOS_HandleOneShotAnims(instance) == 0)
    {
        if (SKINBOS_ShouldEscapeJail(instance) && !SKINBOS_CheckPointInsideMasher(instance, &gameTrackerX.playerInstance->position, 1))
        {
            MON_SwitchState(instance, MONSTER_STATE_FLEE);
        }
        else if (enemy == NULL)
        {
            MON_SwitchState(instance, MONSTER_STATE_IDLE);
        }
        else
        {

            combat = mv->subAttr->combatAttributes;
            mv->lookAtPos = &enemy->instance->position;

            if (enemy->distance < combat->combatRange && enemy->mirFlags & 0x20)
            {

                reason = MON_ShouldIAttack(instance, enemy, MON_ChooseAttack(instance, enemy));

                if (reason == MONSTER_ATTACKRESULT_TOOCLOSE || reason == MONSTER_ATTACKRESULT_TOOFAR)
                {
                    mv->mvFlags |= 0x20000;
                    MON_PlayAnimIfNotPlaying(instance, 2, 2);
                    vars->anim_state = 0;
                }
                else
                {

                    turnResult = SKINBOS_Turn(instance, &enemy->instance->position, 5);
                    MON_TurnToPosition(instance, &enemy->instance->position, mv->subAttr->speedPivotTurn);
                    switch (turnResult)
                    {
                    case 1:
                        MON_PlayAnimFromListIfNotPlaying(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 3, 2);
                        vars->anim_state = 0;
                        mv->mvFlags &= ~0x20000;
                        break;
                    case 2:
                        MON_PlayAnimFromListIfNotPlaying(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 2, 2);
                        vars->anim_state = 0;
                        mv->mvFlags &= ~0x20000;
                        break;
                    case 0:
                        if (vars->phase_level != 0 || reason != MONSTER_ATTACKRESULT_SUCCESS)
                        {
                            MON_PlayCombatIdle(instance, 2);
                            vars->anim_state = 0;
                            mv->mvFlags &= ~0x20000;
                            break;
                        }

                        MON_SwitchState(instance, MONSTER_STATE_ATTACK);
                    }
                }
            }
            else
            {
                MON_SwitchState(instance, MONSTER_STATE_PURSUE);
            }
        }
    }

    SKINBOS_CheckPhaseIn(instance);
    MON_DefaultQueueHandler(instance);
}

void SKINBOS_LandOnFeetEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    MON_SwitchStateDoEntry(instance, (signed char)mv->previousMainState);
}

void SKINBOS_LandOnFeet(Instance *instance)
{
    (void)instance;
}

void SKINBOS_AttackEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    SkinbosVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (SkinbosVars *)mv->extraVars;

    if (vars == NULL)
    {
        return;
    }

    if (gameTrackerX.gameData.asmData.MorphTime == 1000)
    {
        if (gameTrackerX.gameData.asmData.MorphType == 1)
        {
            vars->anim_state = 10;
        }
        else
        {
            vars->anim_state = 0;
        }

        MON_AttackEntry(instance);
        mv->mvFlags &= ~0x20000;
    }
    else
    {
        MON_SwitchStateDoEntry(instance, MONSTER_STATE_COMBAT);
    }
}

void SKINBOS_Attack(Instance *instance)
{

    MonsterAttackAttributes *attack; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    SkinbosVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (SkinbosVars *)mv->extraVars;

    if (vars != NULL)
    {

        SKINBOS_HandleOneShotAnims(instance);
        MON_Attack(instance);

        if (mv->mvFlags & 4)
        {
            MON_SwitchState(instance, MONSTER_STATE_IDLE);
        }

        attack = mv->attackType;

        if ((signed char)attack->sphereSegment == 8 && G2EmulationInstanceQueryPassedFrame(instance, 0, (signed char)attack->sphereOnFrame))
        {
            GAMEPAD_Shock1(96, 40960);
        }

        mv->auxFlags &= ~4;

        if (instance->currentMainState != MONSTER_STATE_ATTACK && vars->phase_level == 7)
        {
            MON_PlayCombatIdle(instance, 2);
            vars->anim_state = 11;
            mv->mvFlags &= ~0x20000;
        }
    }
}

void SKINBOS_DeadEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    mv->soulJuice = 0;
    MON_DeadEntry(instance);
}

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

void SKINBOS_DoPhaseFade(Instance *instance, int limit)
{

    int chance; // not from debug symbols
    int lowerBound; // not from debug symbols
    int upperBound; // not from debug symbols
    int phaseLevel; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    SkinbosVars *vars; // not from debug symbols
    SkinbosAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (SkinbosVars *)mv->extraVars;
    attrs = (SkinbosAttributes *)ma->tunData;
    phaseLevel = vars->phase_level;

    switch (vars->num_hits)
    {
    case 0:
        upperBound = 15291;
        lowerBound = 0;
        break;
    case 1:
        upperBound = ((attrs->low_backstep + 7) * 32767) / 30;
        lowerBound = (attrs->low_backstep * 32767) / 30;
        break;
    case 2:
        upperBound = ((attrs->mid_backstep + 7) * 32767) / 60;
        lowerBound = (attrs->mid_backstep * 32767) / 60;
        break;
    default:
        upperBound = ((attrs->high_backstep + 7) * 32767) / 90;
        lowerBound = (attrs->high_backstep * 32767) / 90;
        break;
    }

    if (limit == 0)
    {
        upperBound = upperBound ^ lowerBound;
        lowerBound = upperBound ^ lowerBound;
        upperBound = upperBound ^ lowerBound;
    }

    chance = rand();

    if (chance < upperBound)
    {
        phaseLevel++;
    }
    else
    {

        chance = 32767 - chance;

        if (chance < lowerBound)
        {
            phaseLevel--;

            if (phaseLevel >= 0)
            {
                vars->phase_level = phaseLevel;
            }
        }
    }

    if (phaseLevel > 7)
    {
        phaseLevel = 7;
    }
    else if (phaseLevel < 0)
    {
        phaseLevel = 0;
    }

    vars->phase_level = phaseLevel;

    if ((gameTrackerX.gameData.asmData.MorphTime > 500 && gameTrackerX.gameData.asmData.MorphType == 1) || (gameTrackerX.gameData.asmData.MorphTime < 500 && gameTrackerX.gameData.asmData.MorphType == 0))
    {
        mv->targetFade = ((attrs->max_fadeout * phaseLevel) / 7) + (4096 - attrs->max_fadeout);

        if (phaseLevel == 7)
        {
            instance->flags &= ~0x1000;
        }
        else
        {
            instance->flags |= 0x1000;
        }
    }
    else
    {
        mv->targetFade = (attrs->max_fadeout * phaseLevel) / 7;

        if (phaseLevel == 0)
        {
            instance->flags &= ~0x1000;
        }
        else
        {
            instance->flags |= 0x1000;
        }
    }

    if (phaseLevel == limit)
    {
        vars->anim_state = 0;

        if (limit == 7)
        {
            FX_StartPassthruFX(instance, &vars->normal, &vars->point);
            vars->phase_hit_timer = MON_GetTime(instance) + (attrs->min_phase_time * 33);
        }
        else
        {
            FX_EndPassthruFX(instance);
            SKINBOS_CheckInsideMasher(instance);
        }
    }
}

int SKINBOS_HandleOneShotAnims(Instance *instance) {};

void SKINBOS_DoPhasingOutInit(Instance *instance, SVector *normal, SVector *point)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    SkinbosVars *vars; // not from debug symbols
    SkinbosAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (SkinbosVars *)mv->extraVars;
    attrs = (SkinbosAttributes *)ma->tunData;

    if (vars->num_hits >= attrs->max_allowed_damage && mv->auxFlags & 8 && vars->anim_state != 12)
    {
        MON_PlayAnim(instance, 1, 1);
        vars->anim_state = 12;
        mv->mvFlags &= ~0x20000;
        vars->last_hit_timer = MON_GetTime(instance) + 6930;
        MON_GetPlanSlot(mv);
    }

    if (vars->phase_level == 0 && vars->anim_state == 0)
    {

        MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 9, 1);

        if (normal != NULL && point != NULL)
        {
            COPY_SVEC(SVector, &vars->normal, SVector, normal);
            COPY_SVEC(SVector, &vars->point, SVector, point);
        }

        vars->anim_state = 10;
        mv->mvFlags &= ~0x20000;
    }
}

void SKINBOS_CheckPhaseIn(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    SkinbosVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (SkinbosVars *)mv->extraVars;

    if (!(mv->auxFlags & 4) && vars->phase_level == 7 && MON_GetTime(instance) >= (unsigned long)vars->phase_hit_timer)
    {
        MON_PlayCombatIdle(instance, 2);
        vars->anim_state = 11;
        mv->mvFlags &= ~0x20000;
    }

    mv->auxFlags &= ~4;
}

int SKINBOS_ShouldEscapeJail(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (mv->auxFlags & 8 && !SKINBOS_CheckPointInsideMasher(instance, &gameTrackerX.playerInstance->position, 0))
    {
        return 1;
    }

    return 0;
}

void SKINBOS_ProcessGateHitBlood(Instance *instance, int vertidx, int segidx, int dist, void *cb_data)
{

    int range; // not from debug symbols
    int scl; // not from debug symbols
    SkinbosVars *vars; // not from debug symbols
    CVECTOR *cv; // not from debug symbols
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (SkinbosVars *)mv->extraVars;

    if (vars != NULL)
    {

        range = vars->num_hits * 200;

        if (dist < ((bloodyMessType *)cb_data)->closestdist)
        {
            ((bloodyMessType *)cb_data)->closestvert = vertidx;
            ((bloodyMessType *)cb_data)->closestdist = dist;
            ((bloodyMessType *)cb_data)->closestseg = segidx;
        }

        if (dist < range)
        {

            cv = &instance->perVertexColor[vertidx];

            scl = (2 * (range - dist) * ((bloodyMessType *)cb_data)->bloodIntensity) / range;
            scl = MIN(scl, 256);

            cv->r = ~(((255 - mv->subAttr->bruiseRed) * scl) >> 8);
            cv->g = ~(((255 - mv->subAttr->bruiseGreen) * scl) >> 8);
            cv->b = ~(((255 - mv->subAttr->bruiseBlue) * scl) >> 8);
            cv->cd = 1;

            ((bloodyMessType *)cb_data)->bloodiedAVert = 1;
        }
    }
}

int SKINBOS_StartVertexBlood(Instance *instance)
{

    bloodyMessType bmt; // not from debug symbols
    Position pos;
    MonsterVars *mv; // not from debug symbols
    Model *model; // not from debug symbols


    mv = (MonsterVars *)instance->extraData;
    model = instance->object->modelList[instance->currentModel];

    if (mv->extraVars == NULL)
    {
        return -1;
    }

    if (instance->perVertexColor == NULL)
    {
        MONSTER_InitVertexColors(instance, model);

        if (instance->perVertexColor == NULL)
        {
            return -1;
        }
    }

    pos.x = instance->position.x;
    pos.y = instance->position.y;
    pos.z = instance->position.z + 640;

    bmt.closestvert = -1;
    bmt.closestdist = 65536;
    bmt.closestseg = -1;
    bmt.bloodiedAVert = 0;
    bmt.bloodIntensity = 256;

    MONSTER_ProcessClosestVerts(instance, (SVector *)&pos, SKINBOS_ProcessGateHitBlood, &bmt);

    return bmt.closestvert;
}

const char D_88000060[] = "skgate__"; // TODO: Remove once rest of rodata is brought in

void SKINBOS_Collide(Instance *instance, GameTracker *gameTracker)
{
    int isTargetAnimState; // not from debug symbols
    int didCollide; // not from debug symbols
    int faceFlags; // not from debug symbols
    CollideInfo *collideInfo; // not from debug symbols
    SkinbosVars *vars; // not from debug symbols
    HSphere *hsphere; // not from debug symbols
    MonsterVars *mv; // not from debug symbols

    (void)gameTracker;

    mv = (MonsterVars *)instance->extraData;
    vars = (SkinbosVars *)mv->extraVars;
    collideInfo = instance->collideInfo;
    hsphere = (HSphere *)collideInfo->prim0;
    isTargetAnimState = vars->anim_state > 5 && vars->anim_state < 10;
    didCollide = 0;

    if (vars == NULL)
    {
        MonsterCollide(instance, &gameTrackerX);
    }

    if (collideInfo->type1 == 1)
    {
        collideInfo->offset.x = 0;
        collideInfo->offset.y = 0;
        collideInfo->offset.z = 0;
    }

    if ((vars->phase_level > 0 || isTargetAnimState) && hsphere->id != 9)
    {
        if (instance->data == NULL)
        {
            return;
        }

        switch (collideInfo->type1)
        {
        case 4:
            break;
        case 1:
        {
            evMonsterHitObjectData *data; // not from debug symbols

            data = (evMonsterHitObjectData *)CIRC_Alloc(sizeof(evMonsterHitObjectData));
            data->instance = (Instance *)collideInfo->inst1;
            data->hitType = 1;
            didCollide = 1;
            break;
        }
        case 2:
        {
            evMonsterHitObjectData *data; // not from debug symbols

            data = (evMonsterHitObjectData *)CIRC_Alloc(sizeof(evMonsterHitObjectData));
            data->instance = (Instance *)collideInfo->inst1;
            data->hitType = 2;
            didCollide = 1;
            break;
        }
        case 5:
        {
            evMonsterHitObjectData *data; // not from debug symbols

            data = (evMonsterHitObjectData *)CIRC_Alloc(sizeof(evMonsterHitObjectData));
            data->instance = (Instance *)collideInfo->inst1;
            data->hitType = 5;

            if (strcmpi(data->instance->object->name, D_88000060) != 0)
            {
                didCollide = 1;
            }
            else
            {
                mv->auxFlags |= 4;
            }
            break;
        }
        case 3:
        {
            Level *level; // not from debug symbols
            Terrain *terrain; // not from debug symbols
            TFace *tface; // not from debug symbols
            evMonsterHitTerrainData *data; // not from debug symbols

            data = (evMonsterHitTerrainData *)CIRC_Alloc(sizeof(evMonsterHitTerrainData));
            tface = (TFace *)collideInfo->prim1;

            if (tface->textoff != 0xFFFF)
            {
                level = (Level *)collideInfo->level;
                terrain = level->terrain;
                faceFlags = ((TextureFT3 *)((char *)terrain->StartTextureList + tface->textoff))->attr;
            }
            else
            {
                faceFlags = 0;
            }

            data->faceFlags = faceFlags;
            COLLIDE_FindCollisionFaceNormal(collideInfo, &data->normal);
            data->tface = (TFace *)collideInfo->prim1;

            if (!(data->faceFlags & 0x1000))
            {
                if (!isTargetAnimState)
                {
                    didCollide = 1;
                    if (COLLIDE_FindCollisionFaceNormal(collideInfo, &data->normal))
                    {
                        collideInfo->offset.x += data->normal.x >> 10;
                        collideInfo->offset.y += data->normal.y >> 10;
                    }
                }
            }
            else
            {
                mv->auxFlags |= 4;
            }
            break;
        }
        }

        if (didCollide)
        {
            mv->mvFlags |= 8;
            instance->position.x += collideInfo->offset.x;
            instance->position.y += collideInfo->offset.y;
            COLLIDE_UpdateAllTransforms(instance, (SVECTOR *)&collideInfo->offset);
        }
    }
    else
    {
        MonsterCollide(instance, &gameTrackerX);
    }
}

void SKINBOS_DamageEffect(Instance *instance, evFXHitData *data)
{
    SVector localloc;

    if (data != NULL)
    {
        localloc = data->location;
        FX_MakeHitFX(&localloc);
        SOUND_Play3dSound(&instance->position, 21, 650, 80, 15500);
    }
}

void SKINBOS_Message(void) {};

uintptr_t SKINBOS_Query(Instance *instance, unsigned long query)
{

    uintptr_t ret; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    SkinbosVars *vars; // not from debug symbols
    SkinbosAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (SkinbosVars *)mv->extraVars;
    attrs = (SkinbosAttributes *)ma->tunData;

    if (vars == NULL)
    {
        return MonsterQuery(instance, query);
    }

    switch (query)
    {
    case 0:
        ret = 0;
        break;
    case 30:
        ret = 0;

        if (mv->auxFlags & 8 && vars->num_hits >= attrs->max_allowed_damage)
        {
            ret = vars->phase_level == 0;
        }

        if (vars->gate_drop_pos == 3)
        {
            ret |= 2;
        }

        if (mv->auxFlags & 0x20)
        {
            ret = ret | 8;
        }

        break;
    default:
        ret = MonsterQuery(instance, query);
        break;
    }

    return ret;
}

void SKINBOS_Init(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    SkinbosVars *vars; // not from debug symbols

    MON_DefaultInit(instance);
    mv = (MonsterVars *)instance->extraData;
    instance->collideFunc = SKINBOS_Collide;

    if (mv != NULL)
    {
        vars = (SkinbosVars *)MEMPACK_Malloc(sizeof(SkinbosVars), MEMORY_TYPE_SKINBOSDATA);

        if (vars == NULL)
        {
            mv->extraVars = NULL;
        }
        else
        {
            mv->extraVars = vars;
            vars->num_hits = 0;
            vars->phase_level = 0;
            vars->anim_state = 0;
            vars->last_hit_timer = 0;
        }
    }

    mv->auxFlags |= 0x20000000;
    gameTrackerX.gameFlags |= 0x80000000;
    RAZIEL_SetInteractiveMusic(SOUND_MODIFIER_BOSS_LOADED, 1);
    DEBUG_DoAreaProtection();
}

void SKINBOS_CleanUp(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    RAZIEL_SetInteractiveMusic(SOUND_MODIFIER_BOSS_LOADED, 0);
    gameTrackerX.gameFlags &= ~0x80000000;

    if (mv != NULL && mv->extraVars != NULL)
    {
        MEMPACK_Free(mv->extraVars);
    }

    MON_CleanUp(instance);
}

void SKINBOS_IdleEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    SkinbosVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (SkinbosVars *)mv->extraVars;

    if (vars != NULL)
    {

        mv->mvFlags &= ~0x1000;
        mv->mvFlags &= ~0x40000;
        mv->mode = 1;

        if (mv->mvFlags & 4)
        {
            MON_PlayAnimID(instance, (signed char)((MonsterAttributes *)instance->data)->idleList[0].anim, 2);
            vars->anim_state = 0;
            return;
        }

        if (vars->anim_state != 10 && vars->anim_state != 11)
        {
            vars->anim_state = 0;
        }

        MON_PlayCombatIdle(instance, 2);
    }
}

void SKINBOS_Idle(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    SkinbosVars *vars; // not from debug symbols
    SkinbosAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (SkinbosVars *)mv->extraVars;
    attrs = (SkinbosAttributes *)ma->tunData;

    if (vars == NULL || attrs == NULL)
    {
        return;
    }

    if (mv->mvFlags & 4)
    {
        vars->phase_level = 0;

        if (gameTrackerX.gameData.asmData.MorphType == 0)
        {
            mv->targetFade = 0;
        }
        else
        {
            mv->targetFade = 0x1000;
        }

        vars->anim_state = 0;
        MON_Idle(instance);
        return;
    }

    if (mv->auxFlags & 0x40)
    {
        if (instance->fadeValue == 0x1000)
        {
            if (gameTrackerX.gameData.asmData.MorphTime < 1000 && gameTrackerX.gameData.asmData.MorphType == 0)
            {
                mv->targetFade = ((attrs->max_fadeout * vars->phase_level) / 7) + (0x1000 - attrs->max_fadeout);
            }
            else
            {
                mv->targetFade = (attrs->max_fadeout * vars->phase_level) / 7;
            }

            mv->auxFlags &= ~0x40;
        }
        else
        {
            if (gameTrackerX.gameData.asmData.MorphTime == 1000 && gameTrackerX.gameData.asmData.MorphType == 1)
            {
                mv->targetFade = ((attrs->max_fadeout * vars->phase_level) / 7) + (0x1000 - attrs->max_fadeout);
                mv->auxFlags &= ~0x40;
            }
            else if (gameTrackerX.gameData.asmData.MorphTime == 1000 && gameTrackerX.gameData.asmData.MorphType == 0)
            {
                mv->targetFade = (attrs->max_fadeout * vars->phase_level) / 7;
                mv->auxFlags &= ~0x40;
            }
        }
    }

    if (SKINBOS_HandleOneShotAnims(instance) != 0 || mv->auxFlags & 0x40)
    {
        MON_DefaultQueueHandler(instance);
        return;
    }

    MON_Idle(instance);
}

void SKINBOS_FindRandomNodeInUnit(Instance *instance)
{

    int i; // not from debug symbols
    int counter; // not from debug symbols
    PlanMkr *planMkr; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    Level *level; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    level = STREAM_GetLevelWithID(instance->currentStreamUnitID);

    if (level == NULL)
    {
        return;
    }

    counter = 0;

    for (i = level->NumberOfPlanMarkers, planMkr = level->PlanMarkerList; i > 0; i--, planMkr++)
    {
        if (!((planMkr->id & 0x8000) | (planMkr->id & 0x4000) | (planMkr->id & 0x2000) | (planMkr->id & 0x1000)))
        {
            counter++;
        }
    }

    counter = rand() % counter;

    for (i = level->NumberOfPlanMarkers, planMkr = level->PlanMarkerList; i > 0; i--, planMkr++)
    {
        if (!((planMkr->id & 0x8000) | (planMkr->id & 0x4000) | (planMkr->id & 0x2000) | (planMkr->id & 0x1000)))
        {
            counter--;

            if (counter <= 0)
            {
                COPY_SVEC(Position, &mv->destination, Position, &planMkr->pos);
                break;
            }
        }
    }
}

void SKINBOS_WanderEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    MON_WanderEntry(instance);

    if (!(mv->mvFlags & 4))
    {
        SKINBOS_FindRandomNodeInUnit(instance);
        mv->mvFlags |= 0x40000;
    }
}

void SKINBOS_Wander(void) {};

void SKINBOS_PursueEntry(Instance *instance)
{
    MON_PursueEntry(instance);
}


void SKINBOS_Pursue(Instance *instance)
{

    Position pos;
    int anim; // not from debug symbols
    int range; // not from debug symbols
    int result; // not from debug symbols
    MonsterIR *enemy; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    SkinbosVars *vars; // not from debug symbols
    MonsterCombatAttributes *combat; // not from debug symbols
    SkinbosAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (SkinbosVars *)mv->extraVars;
    enemy = mv->enemy;
    attrs = (SkinbosAttributes *)ma->tunData;
    combat = mv->subAttr->combatAttributes;

    if (vars == NULL)
    {
        return;
    }

    if (mv->mvFlags & 4)
    {
        vars->phase_level = 0;

        if (gameTrackerX.gameData.asmData.MorphType == 0)
        {
            mv->targetFade = 0;
        }
        else
        {
            mv->targetFade = 4096;
        }

        vars->anim_state = 0;
        MON_Pursue(instance);
        return;
    }

    if (SKINBOS_HandleOneShotAnims(instance) == 0)
    {
        if (SKINBOS_ShouldEscapeJail(instance) && !SKINBOS_CheckPointInsideMasher(instance, &gameTrackerX.playerInstance->position, 1))
        {
            MON_SwitchState(instance, MONSTER_STATE_FLEE);
        }
        else if (mv->auxFlags & 1)
        {
            switch (vars->gate_drop_pos)
            {

            case 1:
                MON_PlayAnim(instance, MONSTER_ANIM_STANCE_HEALTHY, 1);
                vars->stop_timer = MON_GetTime(instance) + 1980;
                vars->anim_state = 3;
                mv->mvFlags &= ~0x20000;
                break;
            case 2:
                break;
            case 3:
                MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 6, 1);
                vars->anim_state = 6;
                mv->mvFlags &= ~0x20000;

                if (vars->num_hits < attrs->max_allowed_damage)
                {
                    vars->num_hits++;
                }

                SKINBOS_StartVertexBlood(instance);
                break;
            }

            mv->auxFlags &= ~1;

        }
        else if (enemy == NULL)
        {
            MON_SwitchState(instance, MONSTER_STATE_IDLE);
        }
        else
        {

            mv->destination = enemy->instance->position;
            mv->lookAtPos = &mv->destination;
            range = combat->combatRange;

            if (MATH3D_LengthXY(instance->position.x - mv->destination.x, instance->position.y - mv->destination.y) < range && abs(instance->position.z - mv->destination.z) < 90 && enemy->mirFlags & 0x20)
            {
                MON_SwitchState(instance, MONSTER_STATE_COMBAT);
            }
            else
            {

                if (enemy->distance < range + MON_GetAnim(instance, mv->subAttr->animList, MONSTER_ANIM_WALK)->distance)
                {
                    anim = MONSTER_ANIM_WALK;
                }
                else
                {
                    anim = MONSTER_ANIM_RUN;
                }

                result = ENMYPLAN_MoveToTargetFinal(instance, &pos, (signed char)mv->pathSlotID, &mv->destination, 0x1F);

                if (result == MOVE_TO_WAYPOINT || result == MOVE_TO_TARGET)
                {

                    result = SKINBOS_Turn(instance, &pos, 1365);

                    switch (result)
                    {
                    case 1:
                        MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 1, 1);
                        vars->anim_state = result;
                        mv->mvFlags &= ~0x20000;
                        break;
                    case 2:
                        MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 0, 1);
                        vars->anim_state = result;
                        mv->mvFlags &= ~0x20000;
                        break;
                    case 0:
                    default:

                        MON_TurnToPosition(instance, &pos, mv->subAttr->speedPivotTurn);
                        result = SKINBOS_Turn(instance, &pos, 0);

                        switch (result)
                        {
                        case 1:
                            MON_PlayAnimFromListIfNotPlaying(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 3, 2);
                            vars->anim_state = 0;
                            mv->mvFlags &= ~0x20000;
                            break;
                        case 2:
                            MON_PlayAnimFromListIfNotPlaying(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 2, 2);
                            vars->anim_state = 0;
                            mv->mvFlags &= ~0x20000;
                            break;
                        case 0:
                            if (!(mv->mvFlags & 0x20000))
                            {
                                mv->mvFlags |= 0x20000;
                                MON_PlayAnimIfNotPlaying(instance, anim, 2);
                                vars->anim_state = 0;
                            }
                            break;
                        }
                        break;
                    }
                }
                else
                {
                    if (result == MOVE_INVALID)
                    {
                        MON_GetPlanSlot(mv);
                    }

                    MON_PlayCombatIdle(instance, 2);
                    mv->mvFlags &= ~0x20000;
                }
            }
        }
    }

    SKINBOS_CheckPhaseIn(instance);
    MON_DefaultQueueHandler(instance);
}

void SKINBOS_HitEntry(Instance *instance)
{

    MonsterIR *enemy; // not from debug symbols
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    enemy = mv->enemy;

    if (mv->extraVars == NULL)
    {
        return;
    }

    if (enemy != NULL)
    {
        enemy->mirConditions |= 0x400;
        enemy->mirFlags &= ~0x1000;

        if ((((instance->rotation.z - MATH3D_AngleFromPosToPos(&instance->position, &enemy->instance->position)) + 1024) & 0xFFF) < 2048)
        {
            MON_TurnToPosition(instance, &enemy->instance->position, 4096);
            MON_PlayAnim(instance, MONSTER_ANIM_HIT1, 1);
        }
        else
        {
            MON_PlayAnim(instance, MONSTER_ANIM_HIT2, 1);
        }
    }
    else
    {
        MON_PlayAnim(instance, MONSTER_ANIM_HIT1, 1);
    }

    mv->mode = 0x8000;
}

void SKINBOS_Hit(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    SkinbosVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (SkinbosVars *)mv->extraVars;

    if (vars != NULL)
    {
        if (vars->anim_state == 10 || vars->anim_state == 11)
        {
            SKINBOS_HandleOneShotAnims(instance);
        }
        else
        {
            vars->anim_state = 0;
        }

        if (instance->flags2 & 0x10)
        {
            MON_PlayCombatIdle(instance, 2);
            MON_SwitchState(instance, MONSTER_STATE_COMBAT);
        }

        MON_DefaultQueueHandler(instance);
    }
}

void SKINBOS_CombatEntry(Instance *instance)
{
    MON_CombatEntry(instance);
}

void SKINBOS_Combat(Instance *instance)
{

    int turnResult; // not from debug symbols
    int reason; // not from debug symbols
    MonsterIR *enemy; // not from debug symbols
    MonsterCombatAttributes *combat; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    SkinbosVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (SkinbosVars *)mv->extraVars;
    enemy = mv->enemy;

    if (vars == NULL)
    {
        return;
    }

    if (mv->mvFlags & 4)
    {
        MON_SwitchState(instance, MONSTER_STATE_IDLE);
    }

    if (SKINBOS_HandleOneShotAnims(instance) == 0)
    {
        if (SKINBOS_ShouldEscapeJail(instance) && !SKINBOS_CheckPointInsideMasher(instance, &gameTrackerX.playerInstance->position, 1))
        {
            MON_SwitchState(instance, MONSTER_STATE_FLEE);
        }
        else if (enemy == NULL)
        {
            MON_SwitchState(instance, MONSTER_STATE_IDLE);
        }
        else
        {

            combat = mv->subAttr->combatAttributes;
            mv->lookAtPos = &enemy->instance->position;

            if (enemy->distance < combat->combatRange && enemy->mirFlags & 0x20)
            {

                reason = MON_ShouldIAttack(instance, enemy, MON_ChooseAttack(instance, enemy));

                if (reason == MONSTER_ATTACKRESULT_TOOCLOSE || reason == MONSTER_ATTACKRESULT_TOOFAR)
                {
                    mv->mvFlags |= 0x20000;
                    MON_PlayAnimIfNotPlaying(instance, 2, 2);
                    vars->anim_state = 0;
                }
                else
                {

                    turnResult = SKINBOS_Turn(instance, &enemy->instance->position, 5);
                    MON_TurnToPosition(instance, &enemy->instance->position, mv->subAttr->speedPivotTurn);
                    switch (turnResult)
                    {
                    case 1:
                        MON_PlayAnimFromListIfNotPlaying(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 3, 2);
                        vars->anim_state = 0;
                        mv->mvFlags &= ~0x20000;
                        break;
                    case 2:
                        MON_PlayAnimFromListIfNotPlaying(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 2, 2);
                        vars->anim_state = 0;
                        mv->mvFlags &= ~0x20000;
                        break;
                    case 0:
                        if (vars->phase_level != 0 || reason != MONSTER_ATTACKRESULT_SUCCESS)
                        {
                            MON_PlayCombatIdle(instance, 2);
                            vars->anim_state = 0;
                            mv->mvFlags &= ~0x20000;
                            break;
                        }

                        MON_SwitchState(instance, MONSTER_STATE_ATTACK);
                    }
                }
            }
            else
            {
                MON_SwitchState(instance, MONSTER_STATE_PURSUE);
            }
        }
    }

    SKINBOS_CheckPhaseIn(instance);
    MON_DefaultQueueHandler(instance);
}

void SKINBOS_LandOnFeetEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    MON_SwitchStateDoEntry(instance, (signed char)mv->previousMainState);
}

void SKINBOS_LandOnFeet(Instance *instance)
{
    (void)instance;
}

void SKINBOS_AttackEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    SkinbosVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (SkinbosVars *)mv->extraVars;

    if (vars == NULL)
    {
        return;
    }

    if (gameTrackerX.gameData.asmData.MorphTime == 1000)
    {
        if (gameTrackerX.gameData.asmData.MorphType == 1)
        {
            vars->anim_state = 10;
        }
        else
        {
            vars->anim_state = 0;
        }

        MON_AttackEntry(instance);
        mv->mvFlags &= ~0x20000;
    }
    else
    {
        MON_SwitchStateDoEntry(instance, MONSTER_STATE_COMBAT);
    }
}

void SKINBOS_Attack(Instance *instance)
{

    MonsterAttackAttributes *attack; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    SkinbosVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (SkinbosVars *)mv->extraVars;

    if (vars != NULL)
    {

        SKINBOS_HandleOneShotAnims(instance);
        MON_Attack(instance);

        if (mv->mvFlags & 4)
        {
            MON_SwitchState(instance, MONSTER_STATE_IDLE);
        }

        attack = mv->attackType;

        if ((signed char)attack->sphereSegment == 8 && G2EmulationInstanceQueryPassedFrame(instance, 0, (signed char)attack->sphereOnFrame))
        {
            GAMEPAD_Shock1(96, 40960);
        }

        mv->auxFlags &= ~4;

        if (instance->currentMainState != MONSTER_STATE_ATTACK && vars->phase_level == 7)
        {
            MON_PlayCombatIdle(instance, 2);
            vars->anim_state = 11;
            mv->mvFlags &= ~0x20000;
        }
    }
}

void SKINBOS_DeadEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    mv->soulJuice = 0;
    MON_DeadEntry(instance);
}

void SKINBOS_Dead(void) {};

void SKINBOS_FleeEntry(void) {};

void SKINBOS_Flee(void) {};

void SKINBOS_StunnedEntry(void) {};

#endif
