#include "Game/MONSTER/VWRAITH.h"
#include "Game/MONSTER/SLUAGH.h"
#include "Game/MONSTER/SOUL.h"
#include "Game/FX.h"
#include "Game/MONSTER/MONSTER.h"
#include "Game/MONSTER/MONAPI.h"
#include "Game/MONSTER/MONLIB.h"
#include "Game/MONSTER/MONMSG.h"
#include "Game/SAVEINFO.h"

MonsterStateChoice VWRAITH_StateChoiceTable[] = {
    {MONSTER_STATE_ATTACK, {SLUAGH_AttackEntry, SLUAGH_Attack}},
    {MONSTER_STATE_COMBAT, {VWRAITH_CombatEntry, VWRAITH_Combat}},
    {MONSTER_STATE_PURSUE, {VWRAITH_PursueEntry, VWRAITH_Pursue}},
    {MONSTER_STATE_GENERALDEATH, {SLUAGH_DeathEntry, SLUAGH_Death}},
    {MONSTER_STATE_GRABBED, {SOUL_SoulSuckEntry, SOUL_SoulSuck}},
    {MONSTER_STATE_EMBRACE, {VWRAITH_EmbraceEntry, VWRAITH_Embrace}},
    {-1, {NULL, NULL}}
};

MonsterFunctionTable VWRAITH_FunctionTable = {
    VWRAITH_Init,
    SOUL_CleanUp,
    SLUAGH_DamageEffect,
    SLUAGH_Query,
    NULL,
    VWRAITH_StateChoiceTable,
    monVersion,
    NULL
};

void VWRAITH_MoveVertical(Instance *instance, long targetZ, int velocity)
{
    if (instance->position.z < targetZ)
    {
        instance->position.z += velocity;

        if (targetZ < instance->position.z)
        {
            instance->position.z = targetZ;
        }
    }
    else if (targetZ < instance->position.z)
    {
        instance->position.z -= velocity;

        if (instance->position.z < targetZ)
        {
            instance->position.z = targetZ;
        }
    }
}

void VWRAITH_Init(Instance *instance)
{

    long color;
    int hitpoints;
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;
    hitpoints = mv->hitPoints;

    color = FX_GetHealthColor(hitpoints / 4096);
    mv->effect = FX_DoInstanceTwoSegmentGlow(instance, 0x15, 0x17, &color, 1, 0x4B0, 0x9C);
    MON_DefaultInit(instance);

    mv->soulJuice = 0x3000;
    mv->auxFlags &= ~1;
    mv->mvFlags |= 0x10002800;
}

int VWRAITH_ShouldISwoop(Instance *instance)
{

    MonsterVars *mv;
    MonsterAttributes *ma;

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;

    if (!(mv->mvFlags & 4) && mv->enemy != NULL)
    {
        Instance *ei;
        ei = mv->enemy->instance;
        if (instance->position.z >= ei->position.z)
        {
            if (((MonsterCombatAttributes *)ma->tunData)->surpriseRange < mv->enemy->distance)
            {
                return MON_ShouldIAttackInstance(instance, ei);
            }
        }
    }
    return 0;
}

void VWRAITH_PursueEntry(Instance *instance)
{

    MonsterVars *mv;
    MonsterAttributes *ma;
    MonsterAnim *mAnim;

    mv = (MonsterVars *)instance->extraData;

    if (VWRAITH_ShouldISwoop(instance) != 0)
    {
        mv->auxFlags |= 2;
    }

    if (!(mv->auxFlags & 2))
    {
        MON_PursueEntry(instance);
        return;
    }

    ma = (MonsterAttributes *)instance->data;
    mAnim = (MonsterAnim *)ma->tunData;

    MON_PlayAnimFromList(instance, ma->auxAnimList, (signed char)mAnim->index[2], 1);

    mv->mode = 4;
    mv->speed = 0;
}

void VWRAITH_Pursue(Instance *instance)
{
    MonsterAttributes *ma;
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    ma = (MonsterAttributes *)instance->data;

    if (!(mv->auxFlags & 0x2))
    {
        if (VWRAITH_ShouldISwoop(instance) != 0)
        {
            MON_SwitchState(instance, MONSTER_STATE_PURSUE);
            return;
        }

        if (mv->enemy != NULL)
        {
            VWRAITH_MoveVertical(instance, mv->enemy->instance->position.z, ((Dummy5 *)ma->tunData)->unk6);
        }

        MON_Pursue(instance);
        return;
    }

    if (mv->enemy != NULL)
    {
        // MonsterAttributes *ma; // unused
        MonsterAttackAttributes *attack;

        attack = &ma->attackAttributesList[(signed char)((Dummy5 *)ma->tunData)->unk10];

        MON_TurnToPosition(instance, &mv->enemy->instance->position, mv->subAttr->speedRunTurn);

        if (mv->enemy->distance < ((Dummy5 *)ma->tunData)->unkA)
        {
            mv->attackType = attack;
            mv->attackState = 0;

            mv->speed = 0;

            mv->auxFlags &= ~0x2;

            MON_SwitchState(instance, MONSTER_STATE_ATTACK);
        }
        else if ((instance->flags2 & 0x10))
        {
            mv->speed = MON_GetAnim(instance, ma->auxAnimList, (signed char)((Dummy5 *)ma->tunData)->unkF)->velocity;

            do
            {
            } while (0); // garbage code for reordering

            MON_PlayAnimFromList(instance, ma->auxAnimList, (signed char)((Dummy5 *)ma->tunData)->unkF, 2);
        }

        VWRAITH_MoveVertical(instance, mv->enemy->instance->position.z + ((Dummy5 *)ma->tunData)->unk2, ((Dummy5 *)ma->tunData)->unk4);

        if (mv->speed != 0)
        {
            MON_MoveForward(instance);
        }
    }
    else
    {
        mv->auxFlags &= ~0x2;
    }

    MON_DefaultQueueHandler(instance);
}

void VWRAITH_VerticalMove(Instance *instance)
{

    short targetZ;
    G2LVector3 *velocity;
    MonsterVars *mv;
    MonsterAttributes *ma;
    MonsterIR *enemy;

    mv = (MonsterVars *)instance->extraData;
    enemy = mv->enemy;

    if (enemy == NULL)
    {
        return;
    }

    targetZ = enemy->instance->position.z;
    ma = (MonsterAttributes *)instance->data;
    velocity = (G2LVector3 *)ma->tunData;

    VWRAITH_MoveVertical(instance, targetZ, (short)velocity->z);

    if ((instance->currentMainState == MONSTER_STATE_ATTACK) && (instance->position.z != targetZ))
    {
        MON_SwitchState(instance, MONSTER_STATE_COMBAT);
    }
}

void VWRAITH_CombatEntry(Instance *instance)
{
    MON_CombatEntry(instance);
}

void VWRAITH_Combat(Instance *instance)
{
    MON_Combat(instance);

    VWRAITH_VerticalMove(instance);
}

void VWRAITH_EmbraceEntry(Instance *instance)
{
    MON_EmbraceEntry(instance);
}

void VWRAITH_Embrace(Instance *instance)
{
    MON_Embrace(instance);

    VWRAITH_VerticalMove(instance);
}
