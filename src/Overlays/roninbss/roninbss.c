#include "Overlays/roninbss/roninbss.h"
#include "Game/FX.h"
#include "Game/GAMELOOP.h"
#include "Game/INSTANCE.h"
#include "Game/MATH3D.h"
#include "Game/MEMPACK.h"
#include "Game/OBTABLE.h"
#include "Game/MONSTER/MONAPI.h"
#include "Game/MONSTER/MONLIB.h"
#include "Game/MONSTER/MONMSG.h"
#include "Game/MONSTER/MONSTER.h"
#include "Game/PLAN/ENMYPLAN.h"
#include "Game/PLAN/PLANAPI.h"
#include "Game/RAZIEL/RAZIEL.h"
#include "Game/SOUND.h"
#include "Game/STATE.h"
#include "Game/STREAM.h"

burntTuneType roninbssBurntTune = {300, 2}; // no canon name in symbols

void RONINBSS_DamageEffect(Instance *instance, evFXHitData *data);

// this conditional is for the objdiff report
#ifndef SKIP_ASM

INCLUDE_RODATA("asm/nonmatchings/Overlays/roninbss/roninbss", D_88000000);

INCLUDE_RODATA("asm/nonmatchings/Overlays/roninbss/roninbss", D_88000020);

void RONINBSS_StartBand(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    RoninbssVars *vars; // not from debug symbols
    RoninbssAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (RoninbssVars *)mv->extraVars;
    attrs = (RoninbssAttributes *)ma->tunData;

    if (!(mv->auxFlags & 0x20))
    {
        FX_StartConstrict(instance, (SVector *)&vars->current_constrict_pos, attrs->constrict_start_seg, attrs->constrict_end_seg);
        mv->auxFlags |= 0x20;
    }
}

void RONINBSS_StopBand(Instance *instance, int flash)
{

    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (mv->auxFlags & 0x20)
    {
        FX_EndConstrict(flash, NULL);
        mv->auxFlags &= ~0x20;
    }
}

void RONINBSS_InitConstrict(Instance *instance, Position *target)
{

    MonsterVars *mv; // not from debug symbols
    mv = (MonsterVars *)instance->extraData;

    if (mv != NULL)
    {

        RoninbssVars *vars; // not from debug symbols
        vars = (RoninbssVars *)mv->extraVars;

        if (vars != NULL)
        {
            COPY_SVEC(Position, &vars->current_constrict_pos, Position, target);
            vars->anim_state = 0;
            COPY_SVEC(Position, &vars->last_rb_pos, Position, &instance->position);
        }
    }
}

int RONINBSS_Constrict(Instance *instance)
{

    Position vec;
    Position zero;

    int diff; // not from debug symbols
    int angle; // not from debug symbols
    int rc; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    RoninbssVars *vars; // not from debug symbols
    RoninbssAttributes *attrs; // not from debug symbols
    int didTurn; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (RoninbssVars *)mv->extraVars;
    attrs = (RoninbssAttributes *)ma->tunData;

    memset(&zero, 0, sizeof(Position));
    rc = 0;

    switch (vars->anim_state)
    {
    case 0:
        mv->mvFlags &= ~0x20000;
        MON_PlayAnim(instance, MONSTER_ANIM_STANCE_HEALTHY, 2);

        SUB_SVEC(Position, &vec, Position, &instance->position, Position, &vars->current_constrict_pos);

        vars->old_constrict_angle = MATH3D_AngleFromPosToPos(&zero, &vec);
        vars->total_constrict_angle = 0;
        vars->constrict_time = MON_GetTime(instance) + (attrs->allowed_stop_constrict_time * 33);
        vars->anim_state++;
        break;
    case 1:
        instance->rotation.z -= 1024;
        didTurn = MON_TurnToPosition(instance, &vars->current_constrict_pos, attrs->constrict_turn_rate);
        instance->rotation.z += 1024;
        instance->rotation.z &= 0xFFF;

        if (didTurn)
        {
            MON_PlayAnim(instance, MONSTER_ANIM_RUN, 2);
            mv->mvFlags |= 0x20000;
            vars->anim_state++;
            RONINBSS_StartBand(instance);
        }

        break;
    case 2:
        instance->rotation.z -= 1024;
        MON_TurnToPosition(instance, &vars->current_constrict_pos, attrs->constrict_turn_rate);
        instance->rotation.z += 1024;
        instance->rotation.z &= 0xFFF;

        SUB_SVEC(Position, &vec, Position, &instance->position, Position, &vars->current_constrict_pos);

        angle = MATH3D_AngleFromPosToPos(&zero, &vec);
        diff = AngleDiff(vars->old_constrict_angle, angle);

        vars->old_constrict_angle = angle;
        vars->total_constrict_angle = vars->total_constrict_angle + diff;

        if (vars->total_constrict_angle <= -0x1000)
        {
            instance->rotation.z -= 1024;
            MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 0, 1);
            RONINBSS_StopBand(instance, 1);
            rc = 1;
            vars->anim_state++;
        }
        else if (mv->auxFlags & 0x2000 || diff >= 0)
        {
            if (MON_GetTime(instance) >= (unsigned long)vars->constrict_time)
            {
                vars->anim_state = 5;
                RONINBSS_StopBand(instance, 0);
                rc = 2;
            }
        }
        else if (ENMYPLAN_PathClear(&vars->last_rb_pos, &instance->position))
        {
            vars->constrict_time += (gameTrackerX.timeMult * 33) / 4096;
        }
        else
        {
            vars->anim_state = 5;
            RONINBSS_StopBand(instance, 0);
            rc = 2;
        }
        break;
    case 3:
        if (instance->flags2 & 0x10)
        {
            mv->mvFlags &= ~0x20000;
            vars->anim_state++;
        }
        break;
    case 4:
    case 5:
        rc = 2;
        break;
    }

    mv->auxFlags &= ~0x2000;
    COPY_SVEC(Position, &vars->last_rb_pos, Position, &instance->position);

    return rc;
}

void RONINBSS_StopSoulSuck(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    RONINBSS_StopBand(instance, 0);
    mv->auxFlags &= ~1;
    mv->auxFlags &= ~2;
}

void RONINBSS_FadeMove(Instance *instance, int to_what_plane)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma;// not from debug symbols
    RoninbssAttributes *attrs;// not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    attrs = (RoninbssAttributes *)ma->tunData;

    if (attrs == NULL)
    {
        return;
    }

    switch (to_what_plane)
    {
    case 0:
        mv->targetFade = 4096;
        break;
    case 1:
        mv->targetFade = 0;
        break;
    }

    mv->auxFlags &= ~0x1000;

    if (!(mv->auxFlags & 4))
    {
        switch (to_what_plane)
        {
        case 0:
            PLANAPI_FindNodePositionInUnit(STREAM_GetStreamUnitWithID(instance->currentStreamUnitID), &instance->position, attrs->stand_marker, 5);
            MON_OnGround(instance);
            MON_PlayAnim(instance, MONSTER_ANIM_STANCE_HEALTHY, 2);
            break;
        case 1:
            COPY_SVEC(Position, &instance->position, Position, &instance->intro->position);
            MON_PlayAnimFromList(instance, ma->auxAnimList, 4, 1);
            break;
        }

        COPY_SVEC(Rotation, &instance->rotation, Rotation, &instance->intro->rotation);
    }
}

MonsterAttackAttributes *RONINBSS_ChooseAttack(Instance *instance, MonsterIR *enemy)
{

    int dist; // not from debug symbols
    int shortestDist; // not from debug symbols
    int i; // not from debug symbols
    char *attackIndex; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    MonsterCombatAttributes *combatAttrs; // not from debug symbols
    MonsterAttackAttributes *chosenAttack; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;

    chosenAttack = NULL;
    combatAttrs = mv->subAttr->combatAttributes;
    shortestDist = 99999;

    if (mv->mvFlags & 4 || enemy->mirFlags & 8)
    {
        dist = 0;
    }
    else
    {
        dist = enemy->distance;
    }

    for (i = combatAttrs->numAttacks, attackIndex = combatAttrs->attackList; i != 0; i--, attackIndex++)
    {

        MonsterAttackAttributes *attack; // not from debug symbols
        int effectiveRange; // not from debug symbols
        int attackDist; // not from debug symbols

        attack = &ma->attackAttributesList[(int)*attackIndex];
        effectiveRange = attack->attackRange * mv->subAttr->scale;
        attackDist = (effectiveRange / 4096) - dist;

        if (abs(attackDist) < abs(shortestDist))
        {
            chosenAttack = attack;
            shortestDist = attackDist;
        }
    }

    mv->attackType = chosenAttack;
    mv->attackState = 0;
    return chosenAttack;
}

void RONINBSS_Collide(Instance *instance, GameTracker *gameTracker)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    RoninbssAttributes *attrs; // not from debug symbols
    CollideInfo *collideInfo; // not from debug symbols
    HSphere *prim0; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    attrs = (RoninbssAttributes *)ma->tunData;
    collideInfo = (CollideInfo *)instance->collideInfo;
    prim0 = (HSphere *)collideInfo->prim0;

    if (attrs != NULL)
    {
        if (prim0->id == 9)
        {
            if ((unsigned char)collideInfo->type1 == 1 && ((HSphere *)collideInfo->prim1)->id == 8)
            {

                Instance *collideInst; // not from debug symbols
                collideInst = collideInfo->inst1;

                if (INSTANCE_Query(collideInst, queryWhatAmI) == 1)
                {
                    INSTANCE_Post(gameTrackerX.playerInstance, 0x40005, (attrs->raz_hit_stumble_time * 4096) / 30);
                    RONINBSS_InitConstrict(instance, &collideInst->position);
                    mv->auxFlags |= 1;
                }
            }
        }
        else
        {
            if ((unsigned char)collideInfo->type1 == 2 || (unsigned char)collideInfo->type1 == 3 || (unsigned char)collideInfo->type1 == 5)
            {
                mv->auxFlags |= 0x2000;
            }
        }
    }
    MonsterCollide(instance, gameTracker);
}

void RONINBSS_Message(Instance *instance, unsigned long message, unsigned long data)
{

    int time; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    RoninbssVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (mv == NULL)
    {
        return;
    }

    vars = (RoninbssVars *)mv->extraVars;

    if (vars == NULL)
    {
        return;
    }

    switch (message)
    {
    case 0x100008:
        ADD_SVEC(Position, &vars->current_constrict_pos, Position, &vars->current_constrict_pos, Position, (Position *)data);
        ADD_SVEC(Position, &vars->last_rb_pos, Position, &vars->last_rb_pos, Position, (Position *)data);
        MonsterMessage(instance, message, data);
        break;
    case 0x1000023:
        if (data != 0x1000)
        {
            break;
        }

        time = MON_GetTime(instance);

        if (vars->hit_timer < time)
        {
            vars->hit_timer = time + 4950;
            MonsterMessage(instance, 0x0100001F, SetMonsterHitData(gameTrackerX.playerInstance, instance, 4096, 0, 0));
        }
        break;
    case 0x1000000:
    case 0x100001F:
    case 0x1000021:
        time = MON_GetTime(instance);

        if (vars->hit_timer < time)
        {
            ((evMonsterHitData *)data)->power = 4096;
            vars->hit_timer = time + 4950;
            MonsterMessage(instance, message, data);
        }
        break;
    case 0x1000020:
        RONINBSS_StopSoulSuck(instance);
        vars->to_what_plane = data;
        vars->faded_state = 0;
        mv->auxFlags |= 0x1100;
        MON_SwitchStateDoEntry(instance, MONSTER_STATE_IDLE);
        break;
    case 0x100007:
        mv->auxFlags = ((MonsterSaveInfo *)((evControlSaveDataData *)data)->data)->mvFlags & 0x44;

        if (mv->auxFlags & 4)
        {
            mv->validUnits[0] = 0;
            STREAM_NoMonsters();
        }
        break;
    case 0x1000017:
        switch (data)
        {
        case 1:
            mv->auxFlags |= 8;
            break;
        case 2:
            mv->auxFlags &= ~0x8;
            mv->auxFlags |= 0x10;
            vars->anim_state = 0;
            break;
        case 3:
            mv->auxFlags |= 0x40;
            mv->auxFlags &= ~0x80;
            MON_SwitchStateDoEntry(instance, MONSTER_STATE_PURSUE);
            break;
        case 0:
            RONINBSS_StopSoulSuck(instance);
            mv->damageType = 0x20;
            MON_SwitchStateDoEntry(instance, MONSTER_STATE_GENERALDEATH);
            RONINBSS_DamageEffect(instance, (evFXHitData *)SetFXHitData(0, 0, 0, 0x20));
            break;
        case 4:
            mv->validUnits[0] = 0;
            mv->auxFlags |= 4;
            STREAM_NoMonsters();
            break;
        case 5:
            mv->auxFlags |= 0x200;
            break;
        }
    case 0x80001:
    case 0x80002:
    case 0x80003:
    case 0x80004:
    case 0x80005:
    case 0x80006:
    case 0x40009:
        break;
    default:
        MonsterMessage(instance, message, data);
    }
}

uintptr_t RONINBSS_Query(Instance *instance, unsigned long query)
{

    uintptr_t ret; // not from debug symbols
    RoninbssSaveInfo *saveInfo; // not from debug symbols
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    switch (query)
    {
    case queryHitState:
        ret = 0x01000000;

        if (!(mv->auxFlags & 4))
        {
            ret = 0x40000000;
        }
        break;
    case queryLookatMatrix:
        ret = (uintptr_t)&instance->matrix[2];
        break;
    case querySaveData:
        if (mv == NULL)
        {
            ret = 0;
            break;
        }

        saveInfo = (RoninbssSaveInfo *)CIRC_Alloc(sizeof(RoninbssSaveInfo));
        saveInfo->forRoninbss = mv->auxFlags & 4;
        ret = SetControlSaveDataData(sizeof(RoninbssSaveInfo), saveInfo);
        break;
    case querySpecialInfo:
        if (mv->auxFlags & 4)
        {
            ret = 1;
        }
        else
        {
            ret = 0;
        }
        break;
    default:
        ret = MonsterQuery(instance, query);
        break;
    }

    return ret;
}

void RONINBSS_Init(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    GameTrackerASMData *data; // not from debug symbols

    MON_DefaultInit(instance);
    mv = (MonsterVars *)instance->extraData;
    instance->collideFunc = &RONINBSS_Collide;

    if (mv != NULL)
    {
        RoninbssVars *vars; // not from debug symbols

        vars = (RoninbssVars *)MEMPACK_Malloc(sizeof(RoninbssVars), MEMORY_TYPE_RONINBSSDATA);

        if (vars == NULL)
        {
            mv->extraVars = NULL;
        }
        else
        {
            mv->extraVars = vars;
            vars->stumble_time = 0;
            vars->hit_timer = 0;
        }
    }

    mv->validUnits[0] = instance->currentStreamUnitID;
    mv->validUnits[1] = 0;

    data = &gameTrackerX.gameData.asmData;

    if ((data->MorphTime == 1000 && data->MorphType == 0) || (data->MorphType == 1 && data->MorphTime != 1000))
    {
        RONINBSS_FadeMove(instance, 1);
    }
    else
    {
        RONINBSS_FadeMove(instance, 0);
    }

    mv->auxFlags |= 0x20000000;
    MON_SwitchState(instance, MONSTER_STATE_IDLE);
    instance->flags2 |= 4;
    RAZIEL_SetInteractiveMusic(SOUND_MODIFIER_BOSS_LOADED, 1);
}

void RONINBSS_CleanUp(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    RAZIEL_SetInteractiveMusic(SOUND_MODIFIER_BOSS_LOADED, 0);
    STREAM_YesMonsters();

    if (mv != NULL && mv->extraVars != NULL)
    {
        MEMPACK_Free(mv->extraVars);
    }

    MON_CleanUp(instance);
}

void RONINBSS_DamageEffect(Instance *instance, evFXHitData *data)
{

    SVector localloc;
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;

    if (data == NULL)
    {
        if (mv->mvFlags & 0x400000)
        {
            if (objectAccess[10].object != NULL)
            {
                Model *model; // not from debug symbols

                model = ((Object *)objectAccess[10].object)->modelList[0];
                FX_MakeSpark(instance, model, ma->leftShoulderSegment);
                FX_MakeSpark(instance, model, ma->rightShoulderSegment);
                FX_MakeSpark(instance, model, ma->waistSegment);
                FX_MakeSpark(instance, model, ma->leftKneeSegment);
                FX_MakeSpark(instance, model, ma->rightKneeSegment);
            }
            MONSTER_VertexBurnt(instance, &roninbssBurntTune);
        }
        return;
    }

    localloc = data->location;

    if (data->type == 0x20)
    {
        if (data->amount != 0)
        {
            MONSTER_StartVertexBurnt(instance, &data->location, &roninbssBurntTune);
        }
        else
        {
            MONSTER_StartVertexBurnt(instance, (SVector *)&instance->position, &roninbssBurntTune);
        }
    }
    else if (data->type == 0x10)
    {
        MONSTER_StartVertexBurnt(instance, (SVector *)&instance->position, &roninbssBurntTune);
    }
    else
    {
        SOUND_Play3dSound(&instance->position, 21, 650, 80, 15500);
        FX_MakeHitFX(&localloc);
    }
}

int RONINBSS_FindClosestMarkerInUnit(Instance *instance, StreamUnit *su)
{

    PlanMkr *planMkr; // not from debug symbols
    int dist; // not from debug symbols
    int closest; // not from debug symbols
    int i; // not from debug symbols
    int rc; // not from debug symbols
    Level *level; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    RoninbssVars *vars; // not from debug symbols

    closest = 0x7FFFFFFF;
    rc = 0;
    mv = (MonsterVars *)instance->extraData;
    vars = (RoninbssVars *)mv->extraVars;

    if (su != NULL)
    {

        level = su->level;
        planMkr = level->PlanMarkerList;
        vars->current_marker_id = -1;
        i = level->NumberOfPlanMarkers;

        for (; i > 0; i--, planMkr++)
        {
            if (!((planMkr->id & 0x8000) | (planMkr->id & 0x4000) | (planMkr->id & 0x2000) | (planMkr->id & 0x1000)))
            {
                closest = MATH3D_LengthXY(planMkr->pos.x - instance->position.x, planMkr->pos.y - instance->position.y);
                vars->current_marker_id = planMkr->id;

                rc = 1;
                i++;
                planMkr++;
                break;
            }

        }

        for (; i > 0; i--, planMkr++)
        {
            if (!((planMkr->id & 0x8000) | (planMkr->id & 0x4000) | (planMkr->id & 0x2000) | (planMkr->id & 0x1000)))
            {
                dist = MATH3D_LengthXY(planMkr->pos.x - instance->position.x, planMkr->pos.y - instance->position.y);

                if (dist < closest)
                {
                    closest = dist;
                    vars->current_marker_id = planMkr->id;
                }
            }
        }
    }

    return rc;
}

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_GetNextMarkerInSeries);

Instance *RONINBSS_FindValve(Instance *instance)
{

    MonsterAttributes *ma; // not from debug symbols
    RoninbssAttributes *attrs; // not from debug symbols

    ma = (MonsterAttributes *)instance->data;
    attrs = (RoninbssAttributes *)ma->tunData;

    if (attrs != NULL)
    {

        Instance *inst; // not from debug symbols

        for (inst = gameTrackerX.instanceList->first; inst != NULL; inst = inst->next)
        {
            if (((int *)inst->object->name)[0] == ((int *)attrs->valve_name)[0] && ((int *)inst->object->name)[1] == ((int *)attrs->valve_name)[1])
            {
                return inst;
            }
        }
    }

    return NULL;
}

void RONINBSS_IdleEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (mv->extraVars != NULL)
    {

        if (mv->mvFlags & 4)
        {
            MON_IdleEntry(instance);
            return;
        }

        mv->mvFlags &= ~0x20000;

        if (mv->auxFlags & 4)
        {
            MON_PlayAnimIfNotPlaying(instance, MONSTER_ANIM_STANCE_HEALTHY, 2);
            return;
        }

        if (gameTrackerX.gameData.asmData.MorphType == 1 && (gameTrackerX.gameData.asmData.MorphTime == 1000 || mv->auxFlags & 0x100))
        {
            MON_PlayAnimIfNotPlaying(instance, MONSTER_ANIM_STANCE_HEALTHY, 2);
        }
        else
        {
            MON_PlayAnimFromListIfNotPlaying(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 4, 1);
        }
    }
}

void RONINBSS_Idle(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    RoninbssVars *vars; // not from debug symbols
    RoninbssAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (RoninbssVars *)mv->extraVars;
    attrs = (RoninbssAttributes *)ma->tunData;

    if (vars == NULL || attrs == NULL)
    {
        return;
    }

    if (mv->mvFlags & 4)
    {
        MON_Idle(instance);
        return;
    }

    if (mv->auxFlags & 0x100)
    {
        if (instance->fadeValue == 4096 && mv->auxFlags & 0x1000)
        {
            RONINBSS_FadeMove(instance, vars->to_what_plane);
        }

        if (gameTrackerX.gameData.asmData.MorphTime == 1000)
        {
            mv->auxFlags &= ~0x100;
        }
    }
    else if (mv->auxFlags & 4 || (gameTrackerX.gameData.asmData.MorphTime == 1000 && gameTrackerX.gameData.asmData.MorphType == 1 && mv->auxFlags & 0x200))
    {
        MON_SwitchState(instance, MONSTER_STATE_WANDER);
    }
    else
    {
        if (mv->auxFlags & 8)
        {
            MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 5, 1);
            mv->auxFlags &= ~8;
        }
        else if (mv->auxFlags & 0x10)
        {
            switch (vars->anim_state)
            {
            case 0:
                MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 6, 1);
                vars->anim_state++;
                break;
            case 1:
                if (instance->flags2 & 0x10)
                {
                    MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 7, 1);
                    vars->anim_state++;
                }
                break;
            case 2:
                if (instance->flags2 & 0x10)
                {
                    mv->validUnits[0] = 0;
                    mv->auxFlags |= 4;
                }
                break;
            }
        }
    }

    MON_DefaultQueueHandler(instance);
}

void RONINBSS_WanderEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (mv->extraVars != NULL)
    {
        if (mv->mvFlags & 4)
        {
            MON_WanderEntry(instance);
        }
        else
        {
            RONINBSS_FindClosestMarkerInUnit(instance, STREAM_GetStreamUnitWithID(instance->currentStreamUnitID));
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_Wander);

void RONINBSS_AttackEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    MON_AttackEntry(instance);

    if (mv->auxFlags & 1)
    {
        mv->auxFlags &= ~1;
    }
}

void RONINBSS_Attack(Instance *instance)
{
    MON_Attack(instance);
}

void RONINBSS_CombatEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols
    RoninbssVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (RoninbssVars *)mv->extraVars;

    if (vars != NULL)
    {
        MON_CombatEntry(instance);
        mv->attackState = 0;
        COPY_SVEC(Position, &vars->last_rb_pos, Position, &instance->position);
    }
}

void RONINBSS_Combat(Instance *instance)
{

    int delta; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterIR *enemy; // not from debug symbols
    RoninbssVars *vars; // not from debug symbols
    RoninbssAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    enemy = mv->enemy;
    vars = (RoninbssVars *)mv->extraVars;
    attrs = (RoninbssAttributes *)((MonsterAttributes *)instance->data)->tunData;

    if (vars == NULL)
    {
        return;
    }

    if (mv->auxFlags & 1)
    {
        switch (RONINBSS_Constrict(instance))
        {
        case 1:
            if (MATH3D_LengthXY(vars->current_constrict_pos.x - gameTrackerX.playerInstance->position.x, vars->current_constrict_pos.y - gameTrackerX.playerInstance->position.y) < 320)
            {
                INSTANCE_Post(gameTrackerX.playerInstance, 0x1000000, SetMonsterHitData(instance, 0, attrs->initialDrain * 256, 0, 0));
            }
            break;
        case 2:
        case 3:
            switch (mv->attackState)
            {
            case 0:
                mv->mvFlags &= ~0x20000;
                MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 1, 1);
                mv->attackState++;
                break;
            case 1:
                if (enemy != NULL)
                {
                    MON_TurnToPosition(instance, &enemy->instance->position, mv->subAttr->speedRunTurn);
                }

                if (instance->flags2 & 0x10)
                {
                    mv->attackState++;
                    MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 2, 2);
                }
                break;
            case 2:
                if (enemy == NULL)
                {
                    mv->auxFlags &= ~1;
                    break;
                }

                delta = MATH3D_LengthXYZ(gameTrackerX.playerInstance->position.x - instance->position.x, gameTrackerX.playerInstance->position.y - instance->position.y, gameTrackerX.playerInstance->position.z - instance->position.z);

                if (!(enemy->mirFlags & 0x20) || attrs->soul_suck_range < delta)
                {
                    mv->auxFlags &= ~1;
                    break;
                }

                MON_TurnToPosition(instance, &gameTrackerX.playerInstance->position, 0x1000);
                INSTANCE_Post(gameTrackerX.playerInstance, 0x40006, attrs->constrictDrain * 256);
                MON_DoDrainEffects(instance, gameTrackerX.playerInstance);
                break;
            }
        }
    }
    else
    {
        if (enemy == NULL || mv->mvFlags & 4)
        {
            MON_SwitchState(instance, MONSTER_STATE_IDLE);
        }
        else
        {

            MonsterCombatAttributes *combat; // not from debug symbols

            combat = mv->subAttr->combatAttributes;
            mv->lookAtPos = &enemy->instance->position;

            if (enemy->distance < combat->combatRange)
            {
                if (ENMYPLAN_PathClear(&vars->last_rb_pos, &instance->position))
                {

                    int reason; // not from debug symbols

                    reason = MON_ShouldIAttack(instance, enemy, RONINBSS_ChooseAttack(instance, enemy));

                    if (reason == MONSTER_ATTACKRESULT_TOOCLOSE || reason == MONSTER_ATTACKRESULT_TOOFAR)
                    {
                        mv->mvFlags |= 0x20000;
                        MON_PlayAnimIfNotPlaying(instance, MONSTER_ANIM_WALK, 2);
                    }
                    else if (reason == MONSTER_ATTACKRESULT_SUCCESS)
                    {
                        MON_SwitchState(instance, MONSTER_STATE_ATTACK);
                    }
                    else
                    {
                        mv->mvFlags &= ~0x20000;
                        MON_PlayCombatIdle(instance, 2);
                    }
                }
                else
                {
                    mv->mvFlags &= ~0x20000;
                    MON_PlayCombatIdle(instance, 2);
                }

                MON_TurnToPosition(instance, &enemy->instance->position, mv->subAttr->speedPivotTurn);
            }
            else
            {
                MON_SwitchState(instance, MONSTER_STATE_PURSUE);
            }
        }
    }

    MON_DefaultQueueHandler(instance);
    COPY_SVEC(Position, &vars->last_rb_pos, Position, &instance->position);
}

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_HitEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_Hit);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_FallEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_Fall);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_PursueEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_Pursue);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_DoNothingEntry);

void RONINBSS_DoNothing(Instance *instance)
{
    (void)instance;
};

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_DeadEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_Dead);

#else

void RONINBSS_StartBand(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    RoninbssVars *vars; // not from debug symbols
    RoninbssAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (RoninbssVars *)mv->extraVars;
    attrs = (RoninbssAttributes *)ma->tunData;

    if (!(mv->auxFlags & 0x20))
    {
        FX_StartConstrict(instance, (SVector *)&vars->current_constrict_pos, attrs->constrict_start_seg, attrs->constrict_end_seg);
        mv->auxFlags |= 0x20;
    }
}

void RONINBSS_StopBand(Instance *instance, int flash)
{

    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (mv->auxFlags & 0x20)
    {
        FX_EndConstrict(flash, NULL);
        mv->auxFlags &= ~0x20;
    }
}

void RONINBSS_InitConstrict(Instance *instance, Position *target)
{

    MonsterVars *mv; // not from debug symbols
    mv = (MonsterVars *)instance->extraData;

    if (mv != NULL)
    {

        RoninbssVars *vars; // not from debug symbols
        vars = (RoninbssVars *)mv->extraVars;

        if (vars != NULL)
        {
            COPY_SVEC(Position, &vars->current_constrict_pos, Position, target);
            vars->anim_state = 0;
            COPY_SVEC(Position, &vars->last_rb_pos, Position, &instance->position);
        }
    }
}

int RONINBSS_Constrict(Instance *instance)
{

    Position vec;
    Position zero;

    int diff; // not from debug symbols
    int angle; // not from debug symbols
    int rc; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    RoninbssVars *vars; // not from debug symbols
    RoninbssAttributes *attrs; // not from debug symbols
    int didTurn; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (RoninbssVars *)mv->extraVars;
    attrs = (RoninbssAttributes *)ma->tunData;

    memset(&zero, 0, sizeof(Position));
    rc = 0;

    switch (vars->anim_state)
    {
    case 0:
        mv->mvFlags &= ~0x20000;
        MON_PlayAnim(instance, MONSTER_ANIM_STANCE_HEALTHY, 2);

        SUB_SVEC(Position, &vec, Position, &instance->position, Position, &vars->current_constrict_pos);

        vars->old_constrict_angle = MATH3D_AngleFromPosToPos(&zero, &vec);
        vars->total_constrict_angle = 0;
        vars->constrict_time = MON_GetTime(instance) + (attrs->allowed_stop_constrict_time * 33);
        vars->anim_state++;
        break;
    case 1:
        instance->rotation.z -= 1024;
        didTurn = MON_TurnToPosition(instance, &vars->current_constrict_pos, attrs->constrict_turn_rate);
        instance->rotation.z += 1024;
        instance->rotation.z &= 0xFFF;

        if (didTurn)
        {
            MON_PlayAnim(instance, MONSTER_ANIM_RUN, 2);
            mv->mvFlags |= 0x20000;
            vars->anim_state++;
            RONINBSS_StartBand(instance);
        }

        break;
    case 2:
        instance->rotation.z -= 1024;
        MON_TurnToPosition(instance, &vars->current_constrict_pos, attrs->constrict_turn_rate);
        instance->rotation.z += 1024;
        instance->rotation.z &= 0xFFF;

        SUB_SVEC(Position, &vec, Position, &instance->position, Position, &vars->current_constrict_pos);

        angle = MATH3D_AngleFromPosToPos(&zero, &vec);
        diff = AngleDiff(vars->old_constrict_angle, angle);

        vars->old_constrict_angle = angle;
        vars->total_constrict_angle = vars->total_constrict_angle + diff;

        if (vars->total_constrict_angle <= -0x1000)
        {
            instance->rotation.z -= 1024;
            MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 0, 1);
            RONINBSS_StopBand(instance, 1);
            rc = 1;
            vars->anim_state++;
        }
        else if (mv->auxFlags & 0x2000 || diff >= 0)
        {
            if (MON_GetTime(instance) >= (unsigned long)vars->constrict_time)
            {
                vars->anim_state = 5;
                RONINBSS_StopBand(instance, 0);
                rc = 2;
            }
        }
        else if (ENMYPLAN_PathClear(&vars->last_rb_pos, &instance->position))
        {
            vars->constrict_time += (gameTrackerX.timeMult * 33) / 4096;
        }
        else
        {
            vars->anim_state = 5;
            RONINBSS_StopBand(instance, 0);
            rc = 2;
        }
        break;
    case 3:
        if (instance->flags2 & 0x10)
        {
            mv->mvFlags &= ~0x20000;
            vars->anim_state++;
        }
        break;
    case 4:
    case 5:
        rc = 2;
        break;
    }

    mv->auxFlags &= ~0x2000;
    COPY_SVEC(Position, &vars->last_rb_pos, Position, &instance->position);

    return rc;
}

void RONINBSS_StopSoulSuck(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    RONINBSS_StopBand(instance, 0);
    mv->auxFlags &= ~1;
    mv->auxFlags &= ~2;
}

void RONINBSS_FadeMove(Instance *instance, int to_what_plane)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma;// not from debug symbols
    RoninbssAttributes *attrs;// not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    attrs = (RoninbssAttributes *)ma->tunData;

    if (attrs == NULL)
    {
        return;
    }

    switch (to_what_plane)
    {
    case 0:
        mv->targetFade = 4096;
        break;
    case 1:
        mv->targetFade = 0;
        break;
    }

    mv->auxFlags &= ~0x1000;

    if (!(mv->auxFlags & 4))
    {
        switch (to_what_plane)
        {
        case 0:
            PLANAPI_FindNodePositionInUnit(STREAM_GetStreamUnitWithID(instance->currentStreamUnitID), &instance->position, attrs->stand_marker, 5);
            MON_OnGround(instance);
            MON_PlayAnim(instance, MONSTER_ANIM_STANCE_HEALTHY, 2);
            break;
        case 1:
            COPY_SVEC(Position, &instance->position, Position, &instance->intro->position);
            MON_PlayAnimFromList(instance, ma->auxAnimList, 4, 1);
            break;
        }

        COPY_SVEC(Rotation, &instance->rotation, Rotation, &instance->intro->rotation);
    }
}

MonsterAttackAttributes *RONINBSS_ChooseAttack(Instance *instance, MonsterIR *enemy)
{

    int dist; // not from debug symbols
    int shortestDist; // not from debug symbols
    int i; // not from debug symbols
    char *attackIndex; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    MonsterCombatAttributes *combatAttrs; // not from debug symbols
    MonsterAttackAttributes *chosenAttack; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;

    chosenAttack = NULL;
    combatAttrs = mv->subAttr->combatAttributes;
    shortestDist = 99999;

    if (mv->mvFlags & 4 || enemy->mirFlags & 8)
    {
        dist = 0;
    }
    else
    {
        dist = enemy->distance;
    }

    for (i = combatAttrs->numAttacks, attackIndex = combatAttrs->attackList; i != 0; i--, attackIndex++)
    {

        MonsterAttackAttributes *attack; // not from debug symbols
        int effectiveRange; // not from debug symbols
        int attackDist; // not from debug symbols

        attack = &ma->attackAttributesList[(int)*attackIndex];
        effectiveRange = attack->attackRange * mv->subAttr->scale;
        attackDist = (effectiveRange / 4096) - dist;

        if (abs(attackDist) < abs(shortestDist))
        {
            chosenAttack = attack;
            shortestDist = attackDist;
        }
    }

    mv->attackType = chosenAttack;
    mv->attackState = 0;
    return chosenAttack;
}

void RONINBSS_Collide(Instance *instance, GameTracker *gameTracker)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    RoninbssAttributes *attrs; // not from debug symbols
    CollideInfo *collideInfo; // not from debug symbols
    HSphere *prim0; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    attrs = (RoninbssAttributes *)ma->tunData;
    collideInfo = (CollideInfo *)instance->collideInfo;
    prim0 = (HSphere *)collideInfo->prim0;

    if (attrs != NULL)
    {
        if (prim0->id == 9)
        {
            if ((unsigned char)collideInfo->type1 == 1 && ((HSphere *)collideInfo->prim1)->id == 8)
            {

                Instance *collideInst; // not from debug symbols
                collideInst = collideInfo->inst1;

                if (INSTANCE_Query(collideInst, queryWhatAmI) == 1)
                {
                    INSTANCE_Post(gameTrackerX.playerInstance, 0x40005, (attrs->raz_hit_stumble_time * 4096) / 30);
                    RONINBSS_InitConstrict(instance, &collideInst->position);
                    mv->auxFlags |= 1;
                }
            }
        }
        else
        {
            if ((unsigned char)collideInfo->type1 == 2 || (unsigned char)collideInfo->type1 == 3 || (unsigned char)collideInfo->type1 == 5)
            {
                mv->auxFlags |= 0x2000;
            }
        }
    }
    MonsterCollide(instance, gameTracker);
}

void RONINBSS_Message(Instance *instance, unsigned long message, unsigned long data)
{

    int time; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    RoninbssVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (mv == NULL)
    {
        return;
    }

    vars = (RoninbssVars *)mv->extraVars;

    if (vars == NULL)
    {
        return;
    }

    switch (message)
    {
    case 0x100008:
        ADD_SVEC(Position, &vars->current_constrict_pos, Position, &vars->current_constrict_pos, Position, (Position *)data);
        ADD_SVEC(Position, &vars->last_rb_pos, Position, &vars->last_rb_pos, Position, (Position *)data);
        MonsterMessage(instance, message, data);
        break;
    case 0x1000023:
        if (data != 0x1000)
        {
            break;
        }

        time = MON_GetTime(instance);

        if (vars->hit_timer < time)
        {
            vars->hit_timer = time + 4950;
            MonsterMessage(instance, 0x0100001F, SetMonsterHitData(gameTrackerX.playerInstance, instance, 4096, 0, 0));
        }
        break;
    case 0x1000000:
    case 0x100001F:
    case 0x1000021:
        time = MON_GetTime(instance);

        if (vars->hit_timer < time)
        {
            ((evMonsterHitData *)data)->power = 4096;
            vars->hit_timer = time + 4950;
            MonsterMessage(instance, message, data);
        }
        break;
    case 0x1000020:
        RONINBSS_StopSoulSuck(instance);
        vars->to_what_plane = data;
        vars->faded_state = 0;
        mv->auxFlags |= 0x1100;
        MON_SwitchStateDoEntry(instance, MONSTER_STATE_IDLE);
        break;
    case 0x100007:
        mv->auxFlags = ((MonsterSaveInfo *)((evControlSaveDataData *)data)->data)->mvFlags & 0x44;

        if (mv->auxFlags & 4)
        {
            mv->validUnits[0] = 0;
            STREAM_NoMonsters();
        }
        break;
    case 0x1000017:
        switch (data)
        {
        case 1:
            mv->auxFlags |= 8;
            break;
        case 2:
            mv->auxFlags &= ~0x8;
            mv->auxFlags |= 0x10;
            vars->anim_state = 0;
            break;
        case 3:
            mv->auxFlags |= 0x40;
            mv->auxFlags &= ~0x80;
            MON_SwitchStateDoEntry(instance, MONSTER_STATE_PURSUE);
            break;
        case 0:
            RONINBSS_StopSoulSuck(instance);
            mv->damageType = 0x20;
            MON_SwitchStateDoEntry(instance, MONSTER_STATE_GENERALDEATH);
            RONINBSS_DamageEffect(instance, (evFXHitData *)SetFXHitData(0, 0, 0, 0x20));
            break;
        case 4:
            mv->validUnits[0] = 0;
            mv->auxFlags |= 4;
            STREAM_NoMonsters();
            break;
        case 5:
            mv->auxFlags |= 0x200;
            break;
        }
    case 0x80001:
    case 0x80002:
    case 0x80003:
    case 0x80004:
    case 0x80005:
    case 0x80006:
    case 0x40009:
        break;
    default:
        MonsterMessage(instance, message, data);
    }
}

uintptr_t RONINBSS_Query(Instance *instance, unsigned long query)
{

    uintptr_t ret; // not from debug symbols
    RoninbssSaveInfo *saveInfo; // not from debug symbols
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    switch (query)
    {
    case queryHitState:
        ret = 0x01000000;

        if (!(mv->auxFlags & 4))
        {
            ret = 0x40000000;
        }
        break;
    case queryLookatMatrix:
        ret = (uintptr_t)&instance->matrix[2];
        break;
    case querySaveData:
        if (mv == NULL)
        {
            ret = 0;
            break;
        }

        saveInfo = (RoninbssSaveInfo *)CIRC_Alloc(sizeof(RoninbssSaveInfo));
        saveInfo->forRoninbss = mv->auxFlags & 4;
        ret = SetControlSaveDataData(sizeof(RoninbssSaveInfo), saveInfo);
        break;
    case querySpecialInfo:
        if (mv->auxFlags & 4)
        {
            ret = 1;
        }
        else
        {
            ret = 0;
        }
        break;
    default:
        ret = MonsterQuery(instance, query);
        break;
    }

    return ret;
}

void RONINBSS_Init(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    GameTrackerASMData *data; // not from debug symbols

    MON_DefaultInit(instance);
    mv = (MonsterVars *)instance->extraData;
    instance->collideFunc = &RONINBSS_Collide;

    if (mv != NULL)
    {
        RoninbssVars *vars; // not from debug symbols

        vars = (RoninbssVars *)MEMPACK_Malloc(sizeof(RoninbssVars), MEMORY_TYPE_RONINBSSDATA);

        if (vars == NULL)
        {
            mv->extraVars = NULL;
        }
        else
        {
            mv->extraVars = vars;
            vars->stumble_time = 0;
            vars->hit_timer = 0;
        }
    }

    mv->validUnits[0] = instance->currentStreamUnitID;
    mv->validUnits[1] = 0;

    data = &gameTrackerX.gameData.asmData;

    if ((data->MorphTime == 1000 && data->MorphType == 0) || (data->MorphType == 1 && data->MorphTime != 1000))
    {
        RONINBSS_FadeMove(instance, 1);
    }
    else
    {
        RONINBSS_FadeMove(instance, 0);
    }

    mv->auxFlags |= 0x20000000;
    MON_SwitchState(instance, MONSTER_STATE_IDLE);
    instance->flags2 |= 4;
    RAZIEL_SetInteractiveMusic(SOUND_MODIFIER_BOSS_LOADED, 1);
}

void RONINBSS_CleanUp(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    RAZIEL_SetInteractiveMusic(SOUND_MODIFIER_BOSS_LOADED, 0);
    STREAM_YesMonsters();

    if (mv != NULL && mv->extraVars != NULL)
    {
        MEMPACK_Free(mv->extraVars);
    }

    MON_CleanUp(instance);
}

void RONINBSS_DamageEffect(Instance *instance, evFXHitData *data)
{

    SVector localloc;
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;

    if (data == NULL)
    {
        if (mv->mvFlags & 0x400000)
        {
            if (objectAccess[10].object != NULL)
            {
                Model *model; // not from debug symbols

                model = ((Object *)objectAccess[10].object)->modelList[0];
                FX_MakeSpark(instance, model, ma->leftShoulderSegment);
                FX_MakeSpark(instance, model, ma->rightShoulderSegment);
                FX_MakeSpark(instance, model, ma->waistSegment);
                FX_MakeSpark(instance, model, ma->leftKneeSegment);
                FX_MakeSpark(instance, model, ma->rightKneeSegment);
            }
            MONSTER_VertexBurnt(instance, &roninbssBurntTune);
        }
        return;
    }

    localloc = data->location;

    if (data->type == 0x20)
    {
        if (data->amount != 0)
        {
            MONSTER_StartVertexBurnt(instance, &data->location, &roninbssBurntTune);
        }
        else
        {
            MONSTER_StartVertexBurnt(instance, (SVector *)&instance->position, &roninbssBurntTune);
        }
    }
    else if (data->type == 0x10)
    {
        MONSTER_StartVertexBurnt(instance, (SVector *)&instance->position, &roninbssBurntTune);
    }
    else
    {
        SOUND_Play3dSound(&instance->position, 21, 650, 80, 15500);
        FX_MakeHitFX(&localloc);
    }
}

int RONINBSS_FindClosestMarkerInUnit(Instance *instance, StreamUnit *su)
{

    PlanMkr *planMkr; // not from debug symbols
    int dist; // not from debug symbols
    int closest; // not from debug symbols
    int i; // not from debug symbols
    int rc; // not from debug symbols
    Level *level; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    RoninbssVars *vars; // not from debug symbols

    closest = 0x7FFFFFFF;
    rc = 0;
    mv = (MonsterVars *)instance->extraData;
    vars = (RoninbssVars *)mv->extraVars;

    if (su != NULL)
    {

        level = su->level;
        planMkr = level->PlanMarkerList;
        vars->current_marker_id = -1;
        i = level->NumberOfPlanMarkers;

        for (; i > 0; i--, planMkr++)
        {
            if (!((planMkr->id & 0x8000) | (planMkr->id & 0x4000) | (planMkr->id & 0x2000) | (planMkr->id & 0x1000)))
            {
                closest = MATH3D_LengthXY(planMkr->pos.x - instance->position.x, planMkr->pos.y - instance->position.y);
                vars->current_marker_id = planMkr->id;

                rc = 1;
                i++;
                planMkr++;
                break;
            }

        }

        for (; i > 0; i--, planMkr++)
        {
            if (!((planMkr->id & 0x8000) | (planMkr->id & 0x4000) | (planMkr->id & 0x2000) | (planMkr->id & 0x1000)))
            {
                dist = MATH3D_LengthXY(planMkr->pos.x - instance->position.x, planMkr->pos.y - instance->position.y);

                if (dist < closest)
                {
                    closest = dist;
                    vars->current_marker_id = planMkr->id;
                }
            }
        }
    }

    return rc;
}

void RONINBSS_GetNextMarkerInSeries(void) {};

Instance *RONINBSS_FindValve(Instance *instance)
{

    MonsterAttributes *ma; // not from debug symbols
    RoninbssAttributes *attrs; // not from debug symbols

    ma = (MonsterAttributes *)instance->data;
    attrs = (RoninbssAttributes *)ma->tunData;

    if (attrs != NULL)
    {

        Instance *inst; // not from debug symbols

        for (inst = gameTrackerX.instanceList->first; inst != NULL; inst = inst->next)
        {
            if (((int *)inst->object->name)[0] == ((int *)attrs->valve_name)[0] && ((int *)inst->object->name)[1] == ((int *)attrs->valve_name)[1])
            {
                return inst;
            }
        }
    }

    return NULL;
}

void RONINBSS_IdleEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (mv->extraVars != NULL)
    {

        if (mv->mvFlags & 4)
        {
            MON_IdleEntry(instance);
            return;
        }

        mv->mvFlags &= ~0x20000;

        if (mv->auxFlags & 4)
        {
            MON_PlayAnimIfNotPlaying(instance, MONSTER_ANIM_STANCE_HEALTHY, 2);
            return;
        }

        if (gameTrackerX.gameData.asmData.MorphType == 1 && (gameTrackerX.gameData.asmData.MorphTime == 1000 || mv->auxFlags & 0x100))
        {
            MON_PlayAnimIfNotPlaying(instance, MONSTER_ANIM_STANCE_HEALTHY, 2);
        }
        else
        {
            MON_PlayAnimFromListIfNotPlaying(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 4, 1);
        }
    }
}

void RONINBSS_Idle(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    RoninbssVars *vars; // not from debug symbols
    RoninbssAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (RoninbssVars *)mv->extraVars;
    attrs = (RoninbssAttributes *)ma->tunData;

    if (vars == NULL || attrs == NULL)
    {
        return;
    }

    if (mv->mvFlags & 4)
    {
        MON_Idle(instance);
        return;
    }

    if (mv->auxFlags & 0x100)
    {
        if (instance->fadeValue == 4096 && mv->auxFlags & 0x1000)
        {
            RONINBSS_FadeMove(instance, vars->to_what_plane);
        }

        if (gameTrackerX.gameData.asmData.MorphTime == 1000)
        {
            mv->auxFlags &= ~0x100;
        }
    }
    else if (mv->auxFlags & 4 || (gameTrackerX.gameData.asmData.MorphTime == 1000 && gameTrackerX.gameData.asmData.MorphType == 1 && mv->auxFlags & 0x200))
    {
        MON_SwitchState(instance, MONSTER_STATE_WANDER);
    }
    else
    {
        if (mv->auxFlags & 8)
        {
            MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 5, 1);
            mv->auxFlags &= ~8;
        }
        else if (mv->auxFlags & 0x10)
        {
            switch (vars->anim_state)
            {
            case 0:
                MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 6, 1);
                vars->anim_state++;
                break;
            case 1:
                if (instance->flags2 & 0x10)
                {
                    MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 7, 1);
                    vars->anim_state++;
                }
                break;
            case 2:
                if (instance->flags2 & 0x10)
                {
                    mv->validUnits[0] = 0;
                    mv->auxFlags |= 4;
                }
                break;
            }
        }
    }

    MON_DefaultQueueHandler(instance);
}

void RONINBSS_WanderEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (mv->extraVars != NULL)
    {
        if (mv->mvFlags & 4)
        {
            MON_WanderEntry(instance);
        }
        else
        {
            RONINBSS_FindClosestMarkerInUnit(instance, STREAM_GetStreamUnitWithID(instance->currentStreamUnitID));
        }
    }
}

void RONINBSS_Wander(void) {};

void RONINBSS_AttackEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    MON_AttackEntry(instance);

    if (mv->auxFlags & 1)
    {
        mv->auxFlags &= ~1;
    }
}

void RONINBSS_Attack(Instance *instance)
{
    MON_Attack(instance);
}

void RONINBSS_CombatEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols
    RoninbssVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (RoninbssVars *)mv->extraVars;

    if (vars != NULL)
    {
        MON_CombatEntry(instance);
        mv->attackState = 0;
        COPY_SVEC(Position, &vars->last_rb_pos, Position, &instance->position);
    }
}

void RONINBSS_Combat(Instance *instance)
{

    int delta; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterIR *enemy; // not from debug symbols
    RoninbssVars *vars; // not from debug symbols
    RoninbssAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    enemy = mv->enemy;
    vars = (RoninbssVars *)mv->extraVars;
    attrs = (RoninbssAttributes *)((MonsterAttributes *)instance->data)->tunData;

    if (vars == NULL)
    {
        return;
    }

    if (mv->auxFlags & 1)
    {
        switch (RONINBSS_Constrict(instance))
        {
        case 1:
            if (MATH3D_LengthXY(vars->current_constrict_pos.x - gameTrackerX.playerInstance->position.x, vars->current_constrict_pos.y - gameTrackerX.playerInstance->position.y) < 320)
            {
                INSTANCE_Post(gameTrackerX.playerInstance, 0x1000000, SetMonsterHitData(instance, 0, attrs->initialDrain * 256, 0, 0));
            }
            break;
        case 2:
        case 3:
            switch (mv->attackState)
            {
            case 0:
                mv->mvFlags &= ~0x20000;
                MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 1, 1);
                mv->attackState++;
                break;
            case 1:
                if (enemy != NULL)
                {
                    MON_TurnToPosition(instance, &enemy->instance->position, mv->subAttr->speedRunTurn);
                }

                if (instance->flags2 & 0x10)
                {
                    mv->attackState++;
                    MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 2, 2);
                }
                break;
            case 2:
                if (enemy == NULL)
                {
                    mv->auxFlags &= ~1;
                    break;
                }

                delta = MATH3D_LengthXYZ(gameTrackerX.playerInstance->position.x - instance->position.x, gameTrackerX.playerInstance->position.y - instance->position.y, gameTrackerX.playerInstance->position.z - instance->position.z);

                if (!(enemy->mirFlags & 0x20) || attrs->soul_suck_range < delta)
                {
                    mv->auxFlags &= ~1;
                    break;
                }

                MON_TurnToPosition(instance, &gameTrackerX.playerInstance->position, 0x1000);
                INSTANCE_Post(gameTrackerX.playerInstance, 0x40006, attrs->constrictDrain * 256);
                MON_DoDrainEffects(instance, gameTrackerX.playerInstance);
                break;
            }
        }
    }
    else
    {
        if (enemy == NULL || mv->mvFlags & 4)
        {
            MON_SwitchState(instance, MONSTER_STATE_IDLE);
        }
        else
        {

            MonsterCombatAttributes *combat; // not from debug symbols

            combat = mv->subAttr->combatAttributes;
            mv->lookAtPos = &enemy->instance->position;

            if (enemy->distance < combat->combatRange)
            {
                if (ENMYPLAN_PathClear(&vars->last_rb_pos, &instance->position))
                {

                    int reason; // not from debug symbols

                    reason = MON_ShouldIAttack(instance, enemy, RONINBSS_ChooseAttack(instance, enemy));

                    if (reason == MONSTER_ATTACKRESULT_TOOCLOSE || reason == MONSTER_ATTACKRESULT_TOOFAR)
                    {
                        mv->mvFlags |= 0x20000;
                        MON_PlayAnimIfNotPlaying(instance, MONSTER_ANIM_WALK, 2);
                    }
                    else if (reason == MONSTER_ATTACKRESULT_SUCCESS)
                    {
                        MON_SwitchState(instance, MONSTER_STATE_ATTACK);
                    }
                    else
                    {
                        mv->mvFlags &= ~0x20000;
                        MON_PlayCombatIdle(instance, 2);
                    }
                }
                else
                {
                    mv->mvFlags &= ~0x20000;
                    MON_PlayCombatIdle(instance, 2);
                }

                MON_TurnToPosition(instance, &enemy->instance->position, mv->subAttr->speedPivotTurn);
            }
            else
            {
                MON_SwitchState(instance, MONSTER_STATE_PURSUE);
            }
        }
    }

    MON_DefaultQueueHandler(instance);
    COPY_SVEC(Position, &vars->last_rb_pos, Position, &instance->position);
}

void RONINBSS_HitEntry(void) {};

void RONINBSS_Hit(void) {};

void RONINBSS_FallEntry(void) {};

void RONINBSS_Fall(void) {};

void RONINBSS_PursueEntry(void) {};

void RONINBSS_Pursue(void) {};

void RONINBSS_DoNothingEntry(void) {};

void RONINBSS_DoNothing(Instance *instance) {};

void RONINBSS_DeadEntry(void) {};

void RONINBSS_Dead(void) {};

#endif
