#include "Overlays/kain/kain.h"
#include "Game/DEBUG.h"
#include "Game/FX.h"
#include "Game/GAMELOOP.h"
#include "Game/GAMEPAD.h"
#include "Game/INSTANCE.h"
#include "Game/MATH3D.h"
#include "Game/PSX/SUPPORT.h"
#include "Game/PLAN/PLANAPI.h"
#include "Game/MEMPACK.h"
#include "Game/MONSTER/MONAPI.h"
#include "Game/MONSTER/MONLIB.h"
#include "Game/MONSTER/MONMSG.h"
#include "Game/MONSTER/MONSTER.h"
#include "Game/RAZIEL/RAZIEL.h"
#include "Game/SOUND.h"
#include "Game/STATE.h"
#include "Game/STREAM.h"

int KAIN_Lightning(Instance *instance); // TODO: Delete once matched

// this conditional is for the objdiff report
#ifndef SKIP_ASM

INCLUDE_RODATA("asm/nonmatchings/Overlays/kain/kain", D_88000000);

INCLUDE_RODATA("asm/nonmatchings/Overlays/kain/kain", D_88000020);

Instance *KAIN_PickUpReaver(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    Instance *inst; // not from debug symbols
    Instance *target; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    target = NULL;

    for (inst = gameTrackerX.instanceList->first; inst != NULL; inst = inst->next)
    {
        if (strcmpi(inst->object->name, "matsrevr") == 0)
        {
            target = inst;
            break;
        }

    }

    if (target != NULL)
    {
        mv->auxFlags |= 0x10;
        INSTANCE_Post(target, 0x800002, SetObjectData(0, 0, 0, instance, 0xE));
        G2EmulationInstanceSetAnimation(target, 0, 1, 0, 0);
        G2EmulationInstanceSetMode(target, 0, 1);
        return target;
    }

    mv->auxFlags &= ~0x10;
    return NULL;

}

void KAIN_FindFarthestMarkerPosition(Instance *instance, Position *position, int start, int end)
{

    PlanMkr *marker; // not from swbug symbols
    int dist; // not from swbug symbols
    int i; // not from swbug symbols
    int maxDist; // not from swbug symbols
    Level *level; // not from swbug symbols

    level = STREAM_GetLevelWithID(instance->currentStreamUnitID);

    if (level != NULL)
    {
        for (i = level->NumberOfPlanMarkers, marker = level->PlanMarkerList, maxDist = 0; i > 0; i--, marker++)
        {
            if (marker->id >= start && end >= marker->id)
            {
                dist = MATH3D_LengthXY(marker->pos.x - gameTrackerX.playerInstance->position.x, marker->pos.y - gameTrackerX.playerInstance->position.y);
                if (maxDist < dist)
                {
                    maxDist = dist;
                    COPY_SVEC(Position, position, Position, &marker->pos);
                }
            }
        }
    }
}

void KAIN_TeleportEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    KainVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (KainVars *)mv->extraVars;

    if (vars != NULL)
    {

        Instance *linkedInst; // not from debug symbols

        instance->fadeValue = 0;

        for (linkedInst = instance->LinkChild; linkedInst != NULL; linkedInst = linkedInst->LinkSibling)
        {
            linkedInst->fadeValue = 0;
        }

        mv->auxFlags &= ~0x8000000;
        vars->teleportState = K_NORMAL;
    }
}

int KAIN_Teleport(Instance *instance)
{

    int temp; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    KainVars *vars; // not from debug symbols
    KainAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (KainVars *)mv->extraVars;
    attrs = (KainAttributes *)ma->tunData;

    if (vars == NULL || attrs == NULL)
    {
        return 1;
    }

    switch (vars->teleportState)
    {
    case K_NORMAL:

        if (!(mv->auxFlags & 0x10))
        {
            MON_PlayAnimFromList(instance, ma->auxAnimList, MONSTER_ANIM_RUN, 1);
        }

        MON_StartSpecialFade(instance, 0x1000, attrs->timeToFadeOut);

        temp = MON_GetTime(instance) + 33;
        vars->timer = temp + (attrs->timeToFadeOut * 33);

        vars->teleportState = K_PHASING_OUT;
        SOUND_Play3dSound(&instance->position, 406, 0, 90, 32000);

        return 0;

    case K_PHASING_OUT:

        if (MON_GetTime(instance) >= (unsigned long)vars->timer)
        {
            if (mv->auxFlags & 0x100)
            {

                Instance *childInst; // not from debug symbols

                childInst = instance->LinkChild;

                while (childInst != NULL)
                {
                    Instance *tempInst; // not from debug symbols

                    tempInst = childInst->LinkSibling;
                    INSTANCE_KillInstance(childInst);
                    childInst = tempInst;
                }

                INSTANCE_KillInstance(instance);
                mv->auxFlags &= ~0x100;

            }
            else if (!(mv->auxFlags & 0x800))
            {
                vars->timer = MON_GetTime(instance) + (attrs->timeFadePause * 33);
                vars->teleportState = K_TELEPORT_PAUSE;
            }
            else
            {
                vars->teleportState = K_TELEPORT_HOLD;
            }
        }
        return 0;

    case K_TELEPORT_PAUSE:

        if (MON_GetTime(instance) >= (unsigned int)vars->timer)
        {

            int temp; // not from debug symbols

            instance->flags2 |= 0x20000000;
            MON_StartSpecialFade(instance, 0, attrs->timeToFadeIn);
            temp = MON_GetTime(instance) + 33;
            vars->timer = temp + (attrs->timeToFadeIn * 33);
            vars->teleportState = K_MOVE_DONE;

            COPY_SVEC(Position, &instance->position, Position, &vars->teleportTarget);
            COPY_SVEC(Position, &instance->oldPos, Position, &vars->teleportTarget);

            MON_TurnToPosition(instance, &gameTrackerX.playerInstance->position, 4096);
        }

        return 0;

    case K_TELEPORT_HOLD:

        if (mv->auxFlags & 0x1000)
        {
            int temp; // not from debug symbols

            MON_StartSpecialFade(instance, 0, attrs->timeToFadeIn);
            temp = MON_GetTime(instance) + 33;
            vars->timer = temp + (attrs->timeToFadeIn * 33);
            vars->teleportState = K_MOVE_DONE;
        }

        return 0;

    case K_MOVE_DONE:

        if (!(mv->auxFlags & 0x10))
        {
            MON_PlayAnimFromList(instance, ma->auxAnimList, MONSTER_ANIM_WALK, 2);
        }

        instance->flags2 &= ~0x20000000;
        vars->teleportState = K_PHASING_IN;
        SOUND_Play3dSound(&instance->position, 405, 0, 90, 32000);

    case K_PHASING_IN:

        if (MON_GetTime(instance) >= (unsigned long)vars->timer)
        {
            vars->teleportState = K_NO_TELEPORT;
            return 1;
        }

        return 0;

    case K_NO_TELEPORT:
        return 1;
    default:
        return 0;
    }
}

void KAIN_EndEffects(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    KainVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (KainVars *)mv->extraVars;

    if (vars != NULL)
    {
        vars->field = NULL;
        vars->zap[2] = NULL;
        vars->zap[1] = NULL;
        vars->zap[0] = NULL;
        FX_EndInstanceEffects(instance);
    }
}

void KAIN_LightningEntry(Instance *instance)
{

    MATRIX *mat; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    KainVars *vars; // not from debug symbols
    KainAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (KainVars *)mv->extraVars;
    attrs = (KainAttributes *)ma->tunData;

    if (vars != NULL && attrs != NULL)
    {
        if (mv->auxFlags & 2)
        {
            vars->numBeams = 3;
            vars->zap[2] = 0;
            vars->zap[1] = 0;
            vars->zap[0] = 0;
        }
        else
        {
            vars->numBeams = 1;
            vars->zap[2] = 0;
            vars->zap[1] = 0;
            vars->zap[0] = 0;

            if (!(mv->auxFlags & 1))
            {
                mat = (MATRIX *)INSTANCE_Query(vars->zapTarget, queryLookatMatrix);
                vars->beamTarget.x = mat->t[0];
                vars->beamTarget.y = mat->t[1];
                vars->beamTarget.z = mat->t[2];
            }
        }

        vars->beamTimer = MON_GetTime(instance);
        vars->changeBeam = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_Lightning);

void KAIN_FFieldOffset(Instance *instance, int segment, Position *position)
{
    MATRIX *mat; // not from debug symbols

    mat = &instance->matrix[segment];
    position->x = mat->t[0] - instance->position.x;
    position->y = mat->t[1] - instance->position.y;
    position->z = mat->t[2] - instance->position.z;
}

void KAIN_ChargeEntry(Instance *instance, int segment)
{

    Position offset;
    long color; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    KainVars *vars; // not from debug symbols
    KainAttributes *attrs; // not from debug symbols

    color = 0xBF1F1F;
    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (KainVars *)mv->extraVars;
    attrs = (KainAttributes *)ma->tunData;

    if (vars != NULL && attrs != NULL)
    {
        KAIN_FFieldOffset(instance, segment, &offset);
        vars->field = FX_StartFField(instance, 0x80, &offset, 0, 0x400, 0, color);
    }
}

void KAIN_ChargeUp(Instance *instance, int time, int segment)
{

    int timeLeft; // not from debug symbols
    int timeToBolt; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    KainVars *vars; // not from debug symbols
    KainAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (KainVars *)mv->extraVars;
    attrs = (KainAttributes *)ma->tunData;

    if (vars != NULL && attrs != NULL && vars->field != NULL)
    {

        timeLeft = MAX(vars->timer - time, 0);
        KAIN_FFieldOffset(instance, segment, (Position *)&vars->field->offset);

        if (mv->auxFlags & 0x10)
        {
            timeToBolt = attrs->timeToBolt[vars->numHits] * 33;
        }
        else
        {
            timeToBolt = attrs->timeToBolt[vars->numHits + attrs->maxHits] * 33;
        }

        vars->field->size_change = ((timeLeft * 960) / timeToBolt) + 64;
        vars->field->size = (timeLeft * 128) / timeToBolt;
    }
}

void KAIN_ChargeDown(Instance *instance, int time, int segment)
{
    (void)time;
    (void)segment;
    KAIN_EndEffects(instance);
}

int CheckHit(Instance *instance)
{
    if (instance->currentMainState == MONSTER_STATE_COMBAT || instance->currentMainState == MONSTER_STATE_ATTACK)
    {
        return 1;
    }
    return 0;
}

void SwitchToHit(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = instance->extraData;
    mv->mvFlags |= 0x40;
    MON_SwitchStateDoEntry(instance, MONSTER_STATE_HIT);
}

void KAIN_DamageEffect(Instance *instance, evFXHitData *data)
{

    SVector localloc;
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (data != NULL)
    {
        if ((mv->auxFlags & 0x50) == 0x50 || (mv->auxFlags & 0x14) == 4)
        {
            MON_DamageEffect(instance, data);
        }
        else
        {
            localloc = data->location;
            FX_MakeHitFX(&localloc);
            SOUND_Play3dSound(&instance->position, 21, 650, 80, 15500);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_Message);

uintptr_t KAIN_Query(Instance *instance, unsigned long query)
{

    uintptr_t ret; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    KainVars *vars; // not from debug symbols
    KainAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (KainVars *)mv->extraVars;
    attrs = (KainAttributes *)ma->tunData;

    if (vars == NULL || attrs == NULL)
    {
        return MonsterQuery(instance, query);
    }

    switch (query)
    {                                 /* irregular */
    case queryHitState:
        ret = 0x1000000;
        break;
    case querySpecialInfo:

        ret = 0;

        if (mv->auxFlags & 0x20)
        {
            ret = 1;
        }

        if (mv->auxFlags & 8)
        {
            ret |= 2;
        }

        break;
    default:
        ret = MonsterQuery(instance, query);
        break;
    }

    return ret;
}

void KAIN_Init(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    KainVars *vars; // not from debug symbols

    MON_DefaultInit(instance);
    mv = instance->extraData;

    if (mv != NULL)
    {
        vars = (KainVars *)MEMPACK_Malloc(sizeof(KainVars), MEMORY_TYPE_KAINDATA);

        if (vars == NULL)
        {
            mv->extraVars = NULL;
        }
        else
        {
            mv->extraVars = vars;
            vars->numHits = 0;
            vars->tier = 0;
            vars->teleportState = K_NO_TELEPORT;
        }
    }

    KAIN_PickUpReaver(instance);
    RAZIEL_SetInteractiveMusic(15, 1);
    DEBUG_DoAreaProtection();
}

void KAIN_CleanUp(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    KainVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    RAZIEL_SetInteractiveMusic(0xF, 0);

    if (mv != NULL)
    {

        vars = (KainVars *)mv->extraVars;

        if (vars != NULL)
        {
            if (SndIsPlaying(vars->soundHandle))
            {
                SndEndLoop(vars->soundHandle);
            }

            MEMPACK_Free((char *)vars);
        }
    }

    MON_CleanUp(instance);
}

void KAIN_IdleEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (mv->extraVars == NULL)
    {
        return;
    }

    if (mv->auxFlags & 0x6100)
    {
        KAIN_EndEffects(instance);
        KAIN_TeleportEntry(instance);

        if (mv->auxFlags & 0x4000)
        {
            MON_PlayAnimIfNotPlaying(instance, MONSTER_ANIM_STANCE_HEALTHY, 2);
            mv->auxFlags &= ~0x4000;
            mv->auxFlags |= 0x800;
        }
        else if (mv->auxFlags & 0x2000)
        {
            mv->auxFlags &= ~0x2000;
            mv->auxFlags |= 0x800;
        }
    }
    else
    {
        if (mv->mvFlags & 4)
        {
            MON_IdleEntry(instance);
        }
        else
        {
            MON_PlayAnimIfNotPlaying(instance, MONSTER_ANIM_STANCE_HEALTHY, 2);
            KAIN_EndEffects(instance);
        }
    }
}

void KAIN_Idle(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    KainVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (KainVars *)mv->extraVars;

    if (vars != NULL)
    {
        if ((mv->auxFlags & 0x800 && (vars->teleportState != K_TELEPORT_HOLD || mv->auxFlags & 0x1000)) || mv->auxFlags & 0x100)
        {
            if (KAIN_Teleport(instance) != 0)
            {
                mv->auxFlags &= ~0x800;
                mv->auxFlags &= ~0x1000;
            }

            MON_DefaultQueueHandler(instance);
            return;
        }

        if (mv->mvFlags & 4)
        {
            MON_Idle(instance);
            return;
        }

        if (!(mv->auxFlags & 0x828) && instance->currentStreamUnitID == gameTrackerX.playerInstance->currentStreamUnitID)
        {
            MON_SwitchState(instance, MONSTER_STATE_PURSUE);
        }

        MON_DefaultQueueHandler(instance);
    }
}

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_CombatEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_Combat);

void KAIN_AttackEntry(Instance *instance)
{

    int attackIndex; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    KainVars *vars; // not from debug symbols
    KainAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (KainVars *)mv->extraVars;
    attrs = (KainAttributes *)ma->tunData;

    if (vars == NULL || attrs == NULL)
    {
        return;
    }

    if (mv->auxFlags & 0x200)
    {
        mv->attackType = &ma->attackAttributesList[(int)mv->subAttr->combatAttributes->attackList[4]];
        mv->auxFlags |= 2;
    }
    else
    {

        vars->zapTarget = gameTrackerX.playerInstance;
        attackIndex = mv->auxFlags & 0x10 ? 0 : 2;

        if (MATH3D_LengthXY(instance->position.x - gameTrackerX.playerInstance->position.x, instance->position.y - gameTrackerX.playerInstance->position.y) < attrs->handHitDist)
        {
            mv->attackType = &ma->attackAttributesList[(int)mv->subAttr->combatAttributes->attackList[attackIndex]];
            mv->auxFlags |= 2;
        }
        else
        {
            mv->attackType = &ma->attackAttributesList[(int)mv->subAttr->combatAttributes->attackList[attackIndex + 1]];
            mv->auxFlags &= ~2;

            if (MATH3D_LengthXY(gameTrackerX.playerInstance->oldPos.x - gameTrackerX.playerInstance->position.x, gameTrackerX.playerInstance->oldPos.y - gameTrackerX.playerInstance->position.y) > 10)
            {
                mv->auxFlags |= 1;
            }
            else
            {
                mv->auxFlags &= ~1;
            }
        }
    }

    vars->timer = MON_GetTime(instance) + (mv->attackType->sphereOnFrame * 33);
    MON_PlayAnimFromList(instance, mv->attackType->animList, MONSTER_ANIM_HIT1, 1);
    mv->attackState = K_NORMAL;
}

void KAIN_Attack(Instance *instance)
{

    int motorSpeed; // not from debug symbols
    int time; // not from debug symbols
    int motorTime; // not from debug symbols
    int attackState; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    KainVars *vars; // not from debug symbols
    KainAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (KainVars *)mv->extraVars;
    attrs = (KainAttributes *)ma->tunData;

    if (vars == NULL || attrs == NULL)
    {
        return;
    }

    MON_TurnToPosition(instance, &vars->zapTarget->position, 4096);
    time = MON_GetTime(instance);
    attackState = mv->attackState;

    switch (attackState)
    {
    case 0:
        KAIN_ChargeDown(instance, vars->timer - (attrs->attackLength * 33), mv->attackType->sphereSegment);

        if (time >= vars->timer)
        {
            GAMEPAD_Shock0(1, 8192);
            KAIN_LightningEntry(instance);
            vars->soundHandle = SOUND_Play3dSound(&instance->position, 397, 0, 100, 32762);
            vars->timer = MON_GetTime(instance) + (attrs->attackLength * 33);
            mv->attackState++;
        }

        if (instance->flags2 & 0x10 && mv->attackType->numAnims > 1)
        {
            MON_PlayAnimFromList(instance, mv->attackType->animList, MONSTER_ANIM_HIT2, 2);
        }
        break;
    case 1:
        if (mv->auxFlags & 0x200)
        {
            KAIN_Lightning(instance);

            if (mv->auxFlags & 0x400)
            {
                KAIN_EndEffects(instance);
                mv->auxFlags &= ~0x200;
                mv->auxFlags &= ~0x400;
                MON_SwitchState(instance, 2);
            }
        }
        else
        {
            if (KAIN_Lightning(instance) != 0)
            {
                motorTime = ((vars->timer - time) * 4096) / 33;
                INSTANCE_Post(vars->zapTarget, 0x40025, motorTime / 30);
                mv->attackState++;
                motorSpeed = mv->auxFlags & 2 ? 128 : 196;
                GAMEPAD_Shock1(motorSpeed, motorTime);
            }

            if (time >= vars->timer)
            {
                MON_SwitchState(instance, 1);
            }
        }
        if (instance->flags2 & 0x10 && mv->attackType->numAnims > 1)
        {
            MON_PlayAnimFromList(instance, mv->attackType->animList, MONSTER_ANIM_HIT2, 2);
        }
        break;
    case 2:
        INSTANCE_Post(vars->zapTarget, 0x40003, SetActionPlayHostAnimationData(vars->zapTarget, instance, 20, 0, 0, attackState));
        INSTANCE_Post(vars->zapTarget, 0x1000000, SetMonsterHitData(instance, 0, mv->attackType->damage * 512, 0, 0));
        mv->attackState++;
    case 3:
        KAIN_Lightning(instance);

        if (instance->flags2 & 0x10 && mv->attackType->numAnims > 1)
        {
            MON_PlayAnimFromList(instance, mv->attackType->animList, MONSTER_ANIM_HIT2, 2);
        }

        if (time >= vars->timer)
        {
            MON_SwitchState(instance, MONSTER_STATE_PURSUE);
        }
        break;
    }

    if (instance->currentMainState != MONSTER_STATE_ATTACK && SndIsPlaying(vars->soundHandle))
    {
        SndEndLoop(vars->soundHandle);
    }

    MON_DefaultQueueHandler(instance);
}

void KAIN_PursueEntry(Instance *instance)
{

    enum MonsterAnim anim; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;

    if (mv->extraVars != NULL)
    {
        if (mv->mvFlags & 4)
        {
            MON_PursueEntry(instance);
            return;
        }

        if (mv->previousMainState != MONSTER_STATE_PURSUE)
        {
            KAIN_EndEffects(instance);
            anim = mv->auxFlags & 0x10 ? MONSTER_ANIM_HIT2 : MONSTER_ANIM_WALK;
            MON_PlayAnimFromList(instance, ma->auxAnimList, anim, 2);
            KAIN_TeleportEntry(instance);
        }
    }
}

void KAIN_Pursue(Instance *instance)
{

    Position pos;
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    KainVars *vars; // not from debug symbols
    KainAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (KainVars *)mv->extraVars;
    ma = (MonsterAttributes *)instance->data;
    attrs = (KainAttributes *)ma->tunData;

    if (vars == NULL || attrs == NULL)
    {
        return;
    }

    if (mv->mvFlags & 4)
    {
        MON_Pursue(instance);
        return;
    }

    if (vars->teleportState == K_TELEPORT_PAUSE && MON_GetTime(instance) >= (unsigned long)vars->timer)
    {
        if (mv->auxFlags & 0x10)
        {
            PLANAPI_FindNodePositionInUnit(STREAM_GetStreamUnitWithID(instance->currentStreamUnitID), &pos, 1, 4);

            if (MATH3D_LengthXY(pos.x - gameTrackerX.playerInstance->position.x, pos.y - gameTrackerX.playerInstance->position.y) < attrs->outsideOfRoom)
            {
                KAIN_FindFarthestMarkerPosition(instance, &vars->teleportTarget, 1, 4);
            }
            else
            {
                COPY_SVEC(Position, &vars->teleportTarget, Position, &pos);
            }
        }
        else
        {
            KAIN_FindFarthestMarkerPosition(instance, &vars->teleportTarget, ((unsigned char)vars->tier * 4) | 1, ((unsigned char)vars->tier * 4) + 4);
        }
    }

    if (KAIN_Teleport(instance) != 0)
    {
        if (instance->currentStreamUnitID == gameTrackerX.playerInstance->currentStreamUnitID)
        {
            MON_SwitchState(instance, MONSTER_STATE_COMBAT);
        }
        else
        {
            MON_SwitchState(instance, MONSTER_STATE_IDLE);
        }
    }

    MON_DefaultQueueHandler(instance);
}

void KAIN_HitEntry(Instance *instance)
{

    int mode; // not from debug symbols
    int animType; // not from debug symbols
    int hits; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    KainVars *vars; // not from debug symbols
    KainAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (KainVars *)mv->extraVars;
    attrs = (KainAttributes *)ma->tunData;

    if (vars != NULL && attrs != NULL)
    {

        GAMEPAD_Shock0(1, 8192);

        animType = 0;
        mode = 2;
        hits = MIN(vars->numHits, attrs->maxHits);

        if ((mv->auxFlags & 0x50) == 0x50)
        {
            animType = hits + 4;
            mode = 1;
            vars->numHits++;
        }
        else if ((mv->auxFlags & 0x14) == 4)
        {
            mode = 1;
            vars->numHits++;
            animType = hits + 7;

            if (vars->numHits < attrs->maxHits)
            {
                mv->auxFlags |= 8;
            }
            else
            {
                mv->auxFlags |= 0x20;
            }
        }
        else
        {
            vars->timer = MON_GetTime(instance) + 1320;
            SOUND_Play3dSound(&instance->position, 403, 0, 100, 15500);
            SOUND_Play3dSound(&instance->position, 21, 650, 70, 15500);
        }

        if (!MON_AnimPlayingFromList(instance, ma->auxAnimList, animType))
        {
            MON_PlayAnimFromList(instance, ma->auxAnimList, animType, mode);
            mv->mode = 0x8000;
            KAIN_EndEffects(instance);
        }

        if (SndIsPlaying(vars->soundHandle))
        {
            SndEndLoop(vars->soundHandle);
        }
    }
}

void KAIN_Hit(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    KainVars *vars; // not from debug symbols
    KainAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (KainVars *)mv->extraVars;
    attrs = (KainAttributes *)ma->tunData;

    if (vars != NULL && attrs != NULL)
    {
        if (!MON_AnimPlayingFromList(instance, ma->auxAnimList, 0))
        {
            if (instance->flags2 & 0x10)
            {
                goto label;
            }
        }
        else if (MON_GetTime(instance) >= (unsigned long)vars->timer)
        {
        label:
            if ((mv->auxFlags & 0x50) == 0x50 && vars->numHits >= attrs->maxHits)
            {
                mv->auxFlags |= 0x20;
                MON_SwitchState(instance, MONSTER_STATE_IDLE);
            }
            else if ((mv->auxFlags & 0x14) == 4)
            {
                mv->auxFlags |= 0x4000;
                MON_SwitchState(instance, MONSTER_STATE_IDLE);
            }
            else
            {
                MON_SwitchState(instance, MONSTER_STATE_PURSUE);
            }

            mv->auxFlags &= ~0x40;
            mv->auxFlags &= ~4;
        }

        MON_DefaultQueueHandler(instance);
    }
}

void KAIN_DoNothingEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    KainVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (KainVars *)mv->extraVars;

    if (vars != NULL)
    {
        if (SndIsPlaying(vars->soundHandle))
        {
            SndEndLoop(vars->soundHandle);
        }

        MON_SwitchState(instance, mv->previousMainState);
    }
}

void KAIN_DoNothing() {};

#else

Instance *KAIN_PickUpReaver(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    Instance *inst; // not from debug symbols
    Instance *target; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    target = NULL;

    for (inst = gameTrackerX.instanceList->first; inst != NULL; inst = inst->next)
    {
        if (strcmpi(inst->object->name, "matsrevr") == 0)
        {
            target = inst;
            break;
        }

    }

    if (target != NULL)
    {
        mv->auxFlags |= 0x10;
        INSTANCE_Post(target, 0x800002, SetObjectData(0, 0, 0, instance, 0xE));
        G2EmulationInstanceSetAnimation(target, 0, 1, 0, 0);
        G2EmulationInstanceSetMode(target, 0, 1);
        return target;
    }

    mv->auxFlags &= ~0x10;
    return NULL;

}

void KAIN_FindFarthestMarkerPosition(Instance *instance, Position *position, int start, int end)
{

    PlanMkr *marker; // not from swbug symbols
    int dist; // not from swbug symbols
    int i; // not from swbug symbols
    int maxDist; // not from swbug symbols
    Level *level; // not from swbug symbols

    level = STREAM_GetLevelWithID(instance->currentStreamUnitID);

    if (level != NULL)
    {
        for (i = level->NumberOfPlanMarkers, marker = level->PlanMarkerList, maxDist = 0; i > 0; i--, marker++)
        {
            if (marker->id >= start && end >= marker->id)
            {
                dist = MATH3D_LengthXY(marker->pos.x - gameTrackerX.playerInstance->position.x, marker->pos.y - gameTrackerX.playerInstance->position.y);
                if (maxDist < dist)
                {
                    maxDist = dist;
                    COPY_SVEC(Position, position, Position, &marker->pos);
                }
            }
        }
    }
}

void KAIN_TeleportEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    KainVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (KainVars *)mv->extraVars;

    if (vars != NULL)
    {

        Instance *linkedInst; // not from debug symbols

        instance->fadeValue = 0;

        for (linkedInst = instance->LinkChild; linkedInst != NULL; linkedInst = linkedInst->LinkSibling)
        {
            linkedInst->fadeValue = 0;
        }

        mv->auxFlags &= ~0x8000000;
        vars->teleportState = K_NORMAL;
    }
}

int KAIN_Teleport(Instance *instance)
{

    int temp; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    KainVars *vars; // not from debug symbols
    KainAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (KainVars *)mv->extraVars;
    attrs = (KainAttributes *)ma->tunData;

    if (vars == NULL || attrs == NULL)
    {
        return 1;
    }

    switch (vars->teleportState)
    {
    case K_NORMAL:

        if (!(mv->auxFlags & 0x10))
        {
            MON_PlayAnimFromList(instance, ma->auxAnimList, MONSTER_ANIM_RUN, 1);
        }

        MON_StartSpecialFade(instance, 0x1000, attrs->timeToFadeOut);

        temp = MON_GetTime(instance) + 33;
        vars->timer = temp + (attrs->timeToFadeOut * 33);

        vars->teleportState = K_PHASING_OUT;
        SOUND_Play3dSound(&instance->position, 406, 0, 90, 32000);

        return 0;

    case K_PHASING_OUT:

        if (MON_GetTime(instance) >= (unsigned long)vars->timer)
        {
            if (mv->auxFlags & 0x100)
            {

                Instance *childInst; // not from debug symbols

                childInst = instance->LinkChild;

                while (childInst != NULL)
                {
                    Instance *tempInst; // not from debug symbols

                    tempInst = childInst->LinkSibling;
                    INSTANCE_KillInstance(childInst);
                    childInst = tempInst;
                }

                INSTANCE_KillInstance(instance);
                mv->auxFlags &= ~0x100;

            }
            else if (!(mv->auxFlags & 0x800))
            {
                vars->timer = MON_GetTime(instance) + (attrs->timeFadePause * 33);
                vars->teleportState = K_TELEPORT_PAUSE;
            }
            else
            {
                vars->teleportState = K_TELEPORT_HOLD;
            }
        }
        return 0;

    case K_TELEPORT_PAUSE:

        if (MON_GetTime(instance) >= (unsigned int)vars->timer)
        {

            int temp; // not from debug symbols

            instance->flags2 |= 0x20000000;
            MON_StartSpecialFade(instance, 0, attrs->timeToFadeIn);
            temp = MON_GetTime(instance) + 33;
            vars->timer = temp + (attrs->timeToFadeIn * 33);
            vars->teleportState = K_MOVE_DONE;

            COPY_SVEC(Position, &instance->position, Position, &vars->teleportTarget);
            COPY_SVEC(Position, &instance->oldPos, Position, &vars->teleportTarget);

            MON_TurnToPosition(instance, &gameTrackerX.playerInstance->position, 4096);
        }

        return 0;

    case K_TELEPORT_HOLD:

        if (mv->auxFlags & 0x1000)
        {
            int temp; // not from debug symbols

            MON_StartSpecialFade(instance, 0, attrs->timeToFadeIn);
            temp = MON_GetTime(instance) + 33;
            vars->timer = temp + (attrs->timeToFadeIn * 33);
            vars->teleportState = K_MOVE_DONE;
        }

        return 0;

    case K_MOVE_DONE:

        if (!(mv->auxFlags & 0x10))
        {
            MON_PlayAnimFromList(instance, ma->auxAnimList, MONSTER_ANIM_WALK, 2);
        }

        instance->flags2 &= ~0x20000000;
        vars->teleportState = K_PHASING_IN;
        SOUND_Play3dSound(&instance->position, 405, 0, 90, 32000);

    case K_PHASING_IN:

        if (MON_GetTime(instance) >= (unsigned long)vars->timer)
        {
            vars->teleportState = K_NO_TELEPORT;
            return 1;
        }

        return 0;

    case K_NO_TELEPORT:
        return 1;
    default:
        return 0;
    }
}

void KAIN_EndEffects(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    KainVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (KainVars *)mv->extraVars;

    if (vars != NULL)
    {
        vars->field = NULL;
        vars->zap[2] = NULL;
        vars->zap[1] = NULL;
        vars->zap[0] = NULL;
        FX_EndInstanceEffects(instance);
    }
}

void KAIN_LightningEntry(Instance *instance)
{

    MATRIX *mat; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    KainVars *vars; // not from debug symbols
    KainAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (KainVars *)mv->extraVars;
    attrs = (KainAttributes *)ma->tunData;

    if (vars != NULL && attrs != NULL)
    {
        if (mv->auxFlags & 2)
        {
            vars->numBeams = 3;
            vars->zap[2] = 0;
            vars->zap[1] = 0;
            vars->zap[0] = 0;
        }
        else
        {
            vars->numBeams = 1;
            vars->zap[2] = 0;
            vars->zap[1] = 0;
            vars->zap[0] = 0;

            if (!(mv->auxFlags & 1))
            {
                mat = (MATRIX *)INSTANCE_Query(vars->zapTarget, queryLookatMatrix);
                vars->beamTarget.x = mat->t[0];
                vars->beamTarget.y = mat->t[1];
                vars->beamTarget.z = mat->t[2];
            }
        }

        vars->beamTimer = MON_GetTime(instance);
        vars->changeBeam = 0;
    }
}

int KAIN_Lightning(Instance *instance) {};

void KAIN_FFieldOffset(Instance *instance, int segment, Position *position)
{
    MATRIX *mat; // not from debug symbols

    mat = &instance->matrix[segment];
    position->x = mat->t[0] - instance->position.x;
    position->y = mat->t[1] - instance->position.y;
    position->z = mat->t[2] - instance->position.z;
}

void KAIN_ChargeEntry(Instance *instance, int segment)
{

    Position offset;
    long color; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    KainVars *vars; // not from debug symbols
    KainAttributes *attrs; // not from debug symbols

    color = 0xBF1F1F;
    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (KainVars *)mv->extraVars;
    attrs = (KainAttributes *)ma->tunData;

    if (vars != NULL && attrs != NULL)
    {
        KAIN_FFieldOffset(instance, segment, &offset);
        vars->field = FX_StartFField(instance, 0x80, &offset, 0, 0x400, 0, color);
    }
}

void KAIN_ChargeUp(Instance *instance, int time, int segment)
{

    int timeLeft; // not from debug symbols
    int timeToBolt; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    KainVars *vars; // not from debug symbols
    KainAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (KainVars *)mv->extraVars;
    attrs = (KainAttributes *)ma->tunData;

    if (vars != NULL && attrs != NULL && vars->field != NULL)
    {

        timeLeft = MAX(vars->timer - time, 0);
        KAIN_FFieldOffset(instance, segment, (Position *)&vars->field->offset);

        if (mv->auxFlags & 0x10)
        {
            timeToBolt = attrs->timeToBolt[vars->numHits] * 33;
        }
        else
        {
            timeToBolt = attrs->timeToBolt[vars->numHits + attrs->maxHits] * 33;
        }

        vars->field->size_change = ((timeLeft * 960) / timeToBolt) + 64;
        vars->field->size = (timeLeft * 128) / timeToBolt;
    }
}

void KAIN_ChargeDown(Instance *instance, int time, int segment)
{
    (void)time;
    (void)segment;
    KAIN_EndEffects(instance);
}

int CheckHit(Instance *instance)
{
    if (instance->currentMainState == MONSTER_STATE_COMBAT || instance->currentMainState == MONSTER_STATE_ATTACK)
    {
        return 1;
    }
    return 0;
}

void SwitchToHit(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = instance->extraData;
    mv->mvFlags |= 0x40;
    MON_SwitchStateDoEntry(instance, MONSTER_STATE_HIT);
}

void KAIN_DamageEffect(Instance *instance, evFXHitData *data)
{

    SVector localloc;
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (data != NULL)
    {
        if ((mv->auxFlags & 0x50) == 0x50 || (mv->auxFlags & 0x14) == 4)
        {
            MON_DamageEffect(instance, data);
        }
        else
        {
            localloc = data->location;
            FX_MakeHitFX(&localloc);
            SOUND_Play3dSound(&instance->position, 21, 650, 80, 15500);
        }
    }
}

void KAIN_Message(void) {};

uintptr_t KAIN_Query(Instance *instance, unsigned long query)
{

    uintptr_t ret; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    KainVars *vars; // not from debug symbols
    KainAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (KainVars *)mv->extraVars;
    attrs = (KainAttributes *)ma->tunData;

    if (vars == NULL || attrs == NULL)
    {
        return MonsterQuery(instance, query);
    }

    switch (query)
    {                                 /* irregular */
    case queryHitState:
        ret = 0x1000000;
        break;
    case querySpecialInfo:

        ret = 0;

        if (mv->auxFlags & 0x20)
        {
            ret = 1;
        }

        if (mv->auxFlags & 8)
        {
            ret |= 2;
        }

        break;
    default:
        ret = MonsterQuery(instance, query);
        break;
    }

    return ret;
}

void KAIN_Init(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    KainVars *vars; // not from debug symbols

    MON_DefaultInit(instance);
    mv = instance->extraData;

    if (mv != NULL)
    {
        vars = (KainVars *)MEMPACK_Malloc(sizeof(KainVars), MEMORY_TYPE_KAINDATA);

        if (vars == NULL)
        {
            mv->extraVars = NULL;
        }
        else
        {
            mv->extraVars = vars;
            vars->numHits = 0;
            vars->tier = 0;
            vars->teleportState = K_NO_TELEPORT;
        }
    }

    KAIN_PickUpReaver(instance);
    RAZIEL_SetInteractiveMusic(15, 1);
    DEBUG_DoAreaProtection();
}

void KAIN_CleanUp(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    KainVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    RAZIEL_SetInteractiveMusic(0xF, 0);

    if (mv != NULL)
    {

        vars = (KainVars *)mv->extraVars;

        if (vars != NULL)
        {
            if (SndIsPlaying(vars->soundHandle))
            {
                SndEndLoop(vars->soundHandle);
            }

            MEMPACK_Free((char *)vars);
        }
    }

    MON_CleanUp(instance);
}

void KAIN_IdleEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (mv->extraVars == NULL)
    {
        return;
    }

    if (mv->auxFlags & 0x6100)
    {
        KAIN_EndEffects(instance);
        KAIN_TeleportEntry(instance);

        if (mv->auxFlags & 0x4000)
        {
            MON_PlayAnimIfNotPlaying(instance, MONSTER_ANIM_STANCE_HEALTHY, 2);
            mv->auxFlags &= ~0x4000;
            mv->auxFlags |= 0x800;
        }
        else if (mv->auxFlags & 0x2000)
        {
            mv->auxFlags &= ~0x2000;
            mv->auxFlags |= 0x800;
        }
    }
    else
    {
        if (mv->mvFlags & 4)
        {
            MON_IdleEntry(instance);
        }
        else
        {
            MON_PlayAnimIfNotPlaying(instance, MONSTER_ANIM_STANCE_HEALTHY, 2);
            KAIN_EndEffects(instance);
        }
    }
}

void KAIN_Idle(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    KainVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (KainVars *)mv->extraVars;

    if (vars != NULL)
    {
        if ((mv->auxFlags & 0x800 && (vars->teleportState != K_TELEPORT_HOLD || mv->auxFlags & 0x1000)) || mv->auxFlags & 0x100)
        {
            if (KAIN_Teleport(instance) != 0)
            {
                mv->auxFlags &= ~0x800;
                mv->auxFlags &= ~0x1000;
            }

            MON_DefaultQueueHandler(instance);
            return;
        }

        if (mv->mvFlags & 4)
        {
            MON_Idle(instance);
            return;
        }

        if (!(mv->auxFlags & 0x828) && instance->currentStreamUnitID == gameTrackerX.playerInstance->currentStreamUnitID)
        {
            MON_SwitchState(instance, MONSTER_STATE_PURSUE);
        }

        MON_DefaultQueueHandler(instance);
    }
}

void KAIN_CombatEntry(void) {};

void KAIN_Combat(void) {};

void KAIN_AttackEntry(Instance *instance)
{

    int attackIndex; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    KainVars *vars; // not from debug symbols
    KainAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (KainVars *)mv->extraVars;
    attrs = (KainAttributes *)ma->tunData;

    if (vars == NULL || attrs == NULL)
    {
        return;
    }

    if (mv->auxFlags & 0x200)
    {
        mv->attackType = &ma->attackAttributesList[(int)mv->subAttr->combatAttributes->attackList[4]];
        mv->auxFlags |= 2;
    }
    else
    {

        vars->zapTarget = gameTrackerX.playerInstance;
        attackIndex = mv->auxFlags & 0x10 ? 0 : 2;

        if (MATH3D_LengthXY(instance->position.x - gameTrackerX.playerInstance->position.x, instance->position.y - gameTrackerX.playerInstance->position.y) < attrs->handHitDist)
        {
            mv->attackType = &ma->attackAttributesList[(int)mv->subAttr->combatAttributes->attackList[attackIndex]];
            mv->auxFlags |= 2;
        }
        else
        {
            mv->attackType = &ma->attackAttributesList[(int)mv->subAttr->combatAttributes->attackList[attackIndex + 1]];
            mv->auxFlags &= ~2;

            if (MATH3D_LengthXY(gameTrackerX.playerInstance->oldPos.x - gameTrackerX.playerInstance->position.x, gameTrackerX.playerInstance->oldPos.y - gameTrackerX.playerInstance->position.y) > 10)
            {
                mv->auxFlags |= 1;
            }
            else
            {
                mv->auxFlags &= ~1;
            }
        }
    }

    vars->timer = MON_GetTime(instance) + (mv->attackType->sphereOnFrame * 33);
    MON_PlayAnimFromList(instance, mv->attackType->animList, MONSTER_ANIM_HIT1, 1);
    mv->attackState = K_NORMAL;
}

void KAIN_Attack(Instance *instance)
{

    int motorSpeed; // not from debug symbols
    int time; // not from debug symbols
    int motorTime; // not from debug symbols
    int attackState; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    KainVars *vars; // not from debug symbols
    KainAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (KainVars *)mv->extraVars;
    attrs = (KainAttributes *)ma->tunData;

    if (vars == NULL || attrs == NULL)
    {
        return;
    }

    MON_TurnToPosition(instance, &vars->zapTarget->position, 4096);
    time = MON_GetTime(instance);
    attackState = mv->attackState;

    switch (attackState)
    {
    case 0:
        KAIN_ChargeDown(instance, vars->timer - (attrs->attackLength * 33), mv->attackType->sphereSegment);

        if (time >= vars->timer)
        {
            GAMEPAD_Shock0(1, 8192);
            KAIN_LightningEntry(instance);
            vars->soundHandle = SOUND_Play3dSound(&instance->position, 397, 0, 100, 32762);
            vars->timer = MON_GetTime(instance) + (attrs->attackLength * 33);
            mv->attackState++;
        }

        if (instance->flags2 & 0x10 && mv->attackType->numAnims > 1)
        {
            MON_PlayAnimFromList(instance, mv->attackType->animList, MONSTER_ANIM_HIT2, 2);
        }
        break;
    case 1:
        if (mv->auxFlags & 0x200)
        {
            KAIN_Lightning(instance);

            if (mv->auxFlags & 0x400)
            {
                KAIN_EndEffects(instance);
                mv->auxFlags &= ~0x200;
                mv->auxFlags &= ~0x400;
                MON_SwitchState(instance, 2);
            }
        }
        else
        {
            if (KAIN_Lightning(instance) != 0)
            {
                motorTime = ((vars->timer - time) * 4096) / 33;
                INSTANCE_Post(vars->zapTarget, 0x40025, motorTime / 30);
                mv->attackState++;
                motorSpeed = mv->auxFlags & 2 ? 128 : 196;
                GAMEPAD_Shock1(motorSpeed, motorTime);
            }

            if (time >= vars->timer)
            {
                MON_SwitchState(instance, 1);
            }
        }
        if (instance->flags2 & 0x10 && mv->attackType->numAnims > 1)
        {
            MON_PlayAnimFromList(instance, mv->attackType->animList, MONSTER_ANIM_HIT2, 2);
        }
        break;
    case 2:
        INSTANCE_Post(vars->zapTarget, 0x40003, SetActionPlayHostAnimationData(vars->zapTarget, instance, 20, 0, 0, attackState));
        INSTANCE_Post(vars->zapTarget, 0x1000000, SetMonsterHitData(instance, 0, mv->attackType->damage * 512, 0, 0));
        mv->attackState++;
    case 3:
        KAIN_Lightning(instance);

        if (instance->flags2 & 0x10 && mv->attackType->numAnims > 1)
        {
            MON_PlayAnimFromList(instance, mv->attackType->animList, MONSTER_ANIM_HIT2, 2);
        }

        if (time >= vars->timer)
        {
            MON_SwitchState(instance, MONSTER_STATE_PURSUE);
        }
        break;
    }

    if (instance->currentMainState != MONSTER_STATE_ATTACK && SndIsPlaying(vars->soundHandle))
    {
        SndEndLoop(vars->soundHandle);
    }

    MON_DefaultQueueHandler(instance);
}

void KAIN_PursueEntry(Instance *instance)
{

    enum MonsterAnim anim; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;

    if (mv->extraVars != NULL)
    {
        if (mv->mvFlags & 4)
        {
            MON_PursueEntry(instance);
            return;
        }

        if (mv->previousMainState != MONSTER_STATE_PURSUE)
        {
            KAIN_EndEffects(instance);
            anim = mv->auxFlags & 0x10 ? MONSTER_ANIM_HIT2 : MONSTER_ANIM_WALK;
            MON_PlayAnimFromList(instance, ma->auxAnimList, anim, 2);
            KAIN_TeleportEntry(instance);
        }
    }
}

void KAIN_Pursue(Instance *instance)
{

    Position pos;
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    KainVars *vars; // not from debug symbols
    KainAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (KainVars *)mv->extraVars;
    ma = (MonsterAttributes *)instance->data;
    attrs = (KainAttributes *)ma->tunData;

    if (vars == NULL || attrs == NULL)
    {
        return;
    }

    if (mv->mvFlags & 4)
    {
        MON_Pursue(instance);
        return;
    }

    if (vars->teleportState == K_TELEPORT_PAUSE && MON_GetTime(instance) >= (unsigned long)vars->timer)
    {
        if (mv->auxFlags & 0x10)
        {
            PLANAPI_FindNodePositionInUnit(STREAM_GetStreamUnitWithID(instance->currentStreamUnitID), &pos, 1, 4);

            if (MATH3D_LengthXY(pos.x - gameTrackerX.playerInstance->position.x, pos.y - gameTrackerX.playerInstance->position.y) < attrs->outsideOfRoom)
            {
                KAIN_FindFarthestMarkerPosition(instance, &vars->teleportTarget, 1, 4);
            }
            else
            {
                COPY_SVEC(Position, &vars->teleportTarget, Position, &pos);
            }
        }
        else
        {
            KAIN_FindFarthestMarkerPosition(instance, &vars->teleportTarget, ((unsigned char)vars->tier * 4) | 1, ((unsigned char)vars->tier * 4) + 4);
        }
    }

    if (KAIN_Teleport(instance) != 0)
    {
        if (instance->currentStreamUnitID == gameTrackerX.playerInstance->currentStreamUnitID)
        {
            MON_SwitchState(instance, MONSTER_STATE_COMBAT);
        }
        else
        {
            MON_SwitchState(instance, MONSTER_STATE_IDLE);
        }
    }

    MON_DefaultQueueHandler(instance);
}

void KAIN_HitEntry(Instance *instance)
{

    int mode; // not from debug symbols
    int animType; // not from debug symbols
    int hits; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    KainVars *vars; // not from debug symbols
    KainAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (KainVars *)mv->extraVars;
    attrs = (KainAttributes *)ma->tunData;

    if (vars != NULL && attrs != NULL)
    {

        GAMEPAD_Shock0(1, 8192);

        animType = 0;
        mode = 2;
        hits = MIN(vars->numHits, attrs->maxHits);

        if ((mv->auxFlags & 0x50) == 0x50)
        {
            animType = hits + 4;
            mode = 1;
            vars->numHits++;
        }
        else if ((mv->auxFlags & 0x14) == 4)
        {
            mode = 1;
            vars->numHits++;
            animType = hits + 7;

            if (vars->numHits < attrs->maxHits)
            {
                mv->auxFlags |= 8;
            }
            else
            {
                mv->auxFlags |= 0x20;
            }
        }
        else
        {
            vars->timer = MON_GetTime(instance) + 1320;
            SOUND_Play3dSound(&instance->position, 403, 0, 100, 15500);
            SOUND_Play3dSound(&instance->position, 21, 650, 70, 15500);
        }

        if (!MON_AnimPlayingFromList(instance, ma->auxAnimList, animType))
        {
            MON_PlayAnimFromList(instance, ma->auxAnimList, animType, mode);
            mv->mode = 0x8000;
            KAIN_EndEffects(instance);
        }

        if (SndIsPlaying(vars->soundHandle))
        {
            SndEndLoop(vars->soundHandle);
        }
    }
}

void KAIN_Hit(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    KainVars *vars; // not from debug symbols
    KainAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (KainVars *)mv->extraVars;
    attrs = (KainAttributes *)ma->tunData;

    if (vars != NULL && attrs != NULL)
    {
        if (!MON_AnimPlayingFromList(instance, ma->auxAnimList, 0))
        {
            if (instance->flags2 & 0x10)
            {
                goto label;
            }
        }
        else if (MON_GetTime(instance) >= (unsigned long)vars->timer)
        {
        label:
            if ((mv->auxFlags & 0x50) == 0x50 && vars->numHits >= attrs->maxHits)
            {
                mv->auxFlags |= 0x20;
                MON_SwitchState(instance, MONSTER_STATE_IDLE);
            }
            else if ((mv->auxFlags & 0x14) == 4)
            {
                mv->auxFlags |= 0x4000;
                MON_SwitchState(instance, MONSTER_STATE_IDLE);
            }
            else
            {
                MON_SwitchState(instance, MONSTER_STATE_PURSUE);
            }

            mv->auxFlags &= ~0x40;
            mv->auxFlags &= ~4;
        }

        MON_DefaultQueueHandler(instance);
    }
}

void KAIN_DoNothingEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    KainVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (KainVars *)mv->extraVars;

    if (vars != NULL)
    {
        if (SndIsPlaying(vars->soundHandle))
        {
            SndEndLoop(vars->soundHandle);
        }

        MON_SwitchState(instance, mv->previousMainState);
    }
}

void KAIN_DoNothing(void) {};

#endif
