#include "Overlays/roninbss/roninbss.h"
#include "Game/FX.h"
#include "Game/GAMELOOP.h"
#include "Game/MATH3D.h"
#include "Game/MONSTER/MONLIB.h"
#include "Game/MONSTER/MONSTER.h"
#include "Game/PLAN/ENMYPLAN.h"

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
        instance->rotation.z -= 0x400;
        didTurn = MON_TurnToPosition(instance, &vars->current_constrict_pos, attrs->constrict_turn_rate);
        instance->rotation.z += 0x400;
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
        instance->rotation.z -= 0x400;
        MON_TurnToPosition(instance, &vars->current_constrict_pos, attrs->constrict_turn_rate);
        instance->rotation.z += 0x400;
        instance->rotation.z &= 0xFFF;

        SUB_SVEC(Position, &vec, Position, &instance->position, Position, &vars->current_constrict_pos);

        angle = MATH3D_AngleFromPosToPos(&zero, &vec);
        diff = AngleDiff(vars->old_constrict_angle, angle);

        vars->old_constrict_angle = angle;
        vars->total_constrict_angle = vars->total_constrict_angle + diff;

        if (vars->total_constrict_angle <= -0x1000)
        {
            instance->rotation.z -= 0x400;
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

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_StopSoulSuck);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_FadeMove);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_ChooseAttack);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_Collide);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_Message);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_Query);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_Init);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_CleanUp);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_DamageEffect);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_FindClosestMarkerInUnit);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_GetNextMarkerInSeries);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_FindValve);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_IdleEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_Idle);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_WanderEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_Wander);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_AttackEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_Attack);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_CombatEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_Combat);

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
        instance->rotation.z -= 0x400;
        didTurn = MON_TurnToPosition(instance, &vars->current_constrict_pos, attrs->constrict_turn_rate);
        instance->rotation.z += 0x400;
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
        instance->rotation.z -= 0x400;
        MON_TurnToPosition(instance, &vars->current_constrict_pos, attrs->constrict_turn_rate);
        instance->rotation.z += 0x400;
        instance->rotation.z &= 0xFFF;

        SUB_SVEC(Position, &vec, Position, &instance->position, Position, &vars->current_constrict_pos);

        angle = MATH3D_AngleFromPosToPos(&zero, &vec);
        diff = AngleDiff(vars->old_constrict_angle, angle);

        vars->old_constrict_angle = angle;
        vars->total_constrict_angle = vars->total_constrict_angle + diff;

        if (vars->total_constrict_angle <= -0x1000)
        {
            instance->rotation.z -= 0x400;
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

void RONINBSS_StopSoulSuck(void) {};

void RONINBSS_FadeMove(void) {};

void RONINBSS_ChooseAttack(void) {};

void RONINBSS_Collide(void) {};

void RONINBSS_Message(void) {};

void RONINBSS_Query(void) {};

void RONINBSS_Init(void) {};

void RONINBSS_CleanUp(void) {};

void RONINBSS_DamageEffect(void) {};

void RONINBSS_FindClosestMarkerInUnit(void) {};

void RONINBSS_GetNextMarkerInSeries(void) {};

void RONINBSS_FindValve(void) {};

void RONINBSS_IdleEntry(void) {};

void RONINBSS_Idle(void) {};

void RONINBSS_WanderEntry(void) {};

void RONINBSS_Wander(void) {};

void RONINBSS_AttackEntry(void) {};

void RONINBSS_Attack(void) {};

void RONINBSS_CombatEntry(void) {};

void RONINBSS_Combat(void) {};

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
