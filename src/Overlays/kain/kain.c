#include "Overlays/kain/kain.h"
#include "Game/FX.h"
#include "Game/GAMELOOP.h"
#include "Game/INSTANCE.h"
#include "Game/MATH3D.h"
#include "Game/PSX/SUPPORT.h"
#include "Game/MONSTER/MONLIB.h"
#include "Game/SOUND.h"
#include "Game/STATE.h"
#include "Game/STREAM.h"

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

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_FFieldOffset);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_ChargeEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_ChargeUp);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_ChargeDown);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", CheckHit);

void SwitchToHit(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = instance->extraData;
    mv->mvFlags |= 0x40;
    MON_SwitchStateDoEntry(instance, MONSTER_STATE_HIT);
}

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_DamageEffect);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_Message);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_Query);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_Init);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_CleanUp);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_IdleEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_Idle);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_CombatEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_Combat);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_AttackEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_Attack);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_PursueEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_Pursue);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_HitEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_Hit);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_DoNothingEntry);

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

void KAIN_Lightning(void) {};

void KAIN_FFieldOffset(void) {};

void KAIN_ChargeEntry(void) {};

void KAIN_ChargeUp(void) {};

void KAIN_ChargeDown(void) {};

void CheckHit(void) {};

void SwitchToHit(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = instance->extraData;
    mv->mvFlags |= 0x40;
    MON_SwitchStateDoEntry(instance, MONSTER_STATE_HIT);
}

void KAIN_DamageEffect(void) {};

void KAIN_Message(void) {};

void KAIN_Query(void) {};

void KAIN_Init(void) {};

void KAIN_CleanUp(void) {};

void KAIN_IdleEntry(void) {};

void KAIN_Idle(void) {};

void KAIN_CombatEntry(void) {};

void KAIN_Combat(void) {};

void KAIN_AttackEntry(void) {};

void KAIN_Attack(void) {};

void KAIN_PursueEntry(void) {};

void KAIN_Pursue(void) {};

void KAIN_HitEntry(void) {};

void KAIN_Hit(void) {};

void KAIN_DoNothingEntry(void) {};

void KAIN_DoNothing(void) {};

#endif
