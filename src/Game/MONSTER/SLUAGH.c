#include "Game/MONSTER/SLUAGH.h"
#include "Game/STATE.h"
#include "Game/GAMELOOP.h"
#include "Game/INSTANCE.h"
#include "Game/SAVEINFO.h"
#include "Game/FX.h"
#include "Game/MONSTER/SOUL.h"
#include "Game/MONSTER/MONAPI.h"
#include "Game/MONSTER/MONLIB.h"
#include "Game/MONSTER/MONMSG.h"
#include "Game/MONSTER/MONSTER.h"
#include "Game/PSX/MAIN.h"

MonsterStateChoice SLUAGH_StateChoiceTable[] = {
    {6, {SLUAGH_AttackEntry, SLUAGH_Attack}},
    {0x10, {SLUAGH_DeathEntry, SLUAGH_Death}},
    {0xA, {SOUL_SoulSuckEntry, SOUL_SoulSuck}},
    {-1, {NULL, NULL}},
};

MonsterStateChoice SOUL_StateChoiceTable[] = {
    {0, {SOUL_BirthEntry, SOUL_Birth}},
    {0x13, {SOUL_FleeEntry, SOUL_Flee}},
    {2, {SOUL_IdleEntry, SOUL_Idle}},
    {5, {SOUL_WanderEntry, SOUL_Wander}},
    {0xA, {SOUL_SoulSuckEntry, SOUL_SoulSuck}},
    {0x17, {SOUL_ReanimateEntry, SOUL_Reanimate}},
    {-1, {NULL, NULL}},
};

void SLUAGH_Init(Instance *instance);
void SLUAGH_DamageEffect(Instance *instance, evFXHitData *data);
uintptr_t SLUAGH_Query(Instance *instance, unsigned long query);

MonsterFunctionTable SLUAGH_FunctionTable = {
    SLUAGH_Init,
    NULL,
    SLUAGH_DamageEffect,
    SLUAGH_Query,
    NULL,
    SLUAGH_StateChoiceTable,
    monVersion,
    NULL,
};

uintptr_t SLUAGH_Query(Instance *instance, unsigned long query)
{
    MonsterVars *mv;
    MonsterAttributes *ma;

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;

    switch (query)
    {
    case 1:
        if (mv->auxFlags & 1)
        {
            return ma->whatAmI | 4;
        }
        return ma->whatAmI;
    case 0:
        if (!(mv->auxFlags & 1))
        {
            // TODO: Investigate this line later.
            return (((unsigned char *)&mv->mvFlags)[1] & 1) << 0x1D;
        }
        return 0x04000000;
    default:
        return MonsterQuery(instance, query);
    }
}

void SLUAGH_DamageEffect(Instance *instance, evFXHitData *data)
{
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    if (data != NULL)
    {
        if (!(mv->auxFlags & 0x1))
        {
            SVector accel = {.x = 0, .y = 0, .z = -2};

            FX_Blood(&data->location, &data->velocity, &accel, data->amount, 0x308000, sizeof(SVector));

            FX_MakeHitFX(&data->location);
        }
    }
    else
    {
        FXGlowEffect *fx;

        fx = (FXGlowEffect *)mv->effect;

        if (fx != NULL)
        {
            long color;

            color = FX_GetHealthColor(mv->hitPoints / 4096);

            if (fx->currentColor != color)
            {
                ColorType current;
                ColorType target;

                current = *(ColorType *)&fx->currentColor;
                target = *(ColorType *)&color;

                if (current.r > target.r)
                {
                    current.r -= 4;
                }
                else if (current.r < target.r)
                {
                    current.r += 4;
                }

                if (current.g > target.g)
                {
                    current.g -= 4;
                }
                else if (current.g < target.g)
                {
                    current.g += 4;
                }

                if (current.b > target.b)
                {
                    current.b -= 4;
                }
                else if (current.b < target.b)
                {
                    current.b += 4;
                }

                fx->currentColor = *(long *)&current;
            }
        }

        if (((mv->auxFlags & 0x1)) && (mv->hitPoints != 0))
        {
            mv->mvFlags &= ~0x200000;
            mv->auxFlags &= ~0x1;

            mv->behaviorState = mv->triggeredBehavior;

            MON_StartSpecialFade(instance, 0, 20);
        }
    }
}

void SLUAGH_Init(Instance *instance)
{

    MonsterVars *mv;
    MonsterAttributes *ma;
    short hitpoints;
    long color;

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    hitpoints = mv->hitPoints;

    color = FX_GetHealthColor(hitpoints / 4096);
    mv->effect = FX_DoInstanceOneSegmentGlow(instance, ma->headSegment, &color, 1, 0x4B0, 0x68, 0x70);

    MON_DefaultInit(instance);
    mv->soulJuice = 8192;
    mv->auxFlags &= ~1;
    mv->mvFlags |= 0x02002000;
}

void SLUAGH_DeathEntry(Instance *instance)
{

    MonsterVars *mv;
    MonsterIR *enemy;

    mv = (MonsterVars *)instance->extraData;
    FX_StopGlowEffect(mv->effect, 0);

    enemy = mv->enemy;
    mv->effect = NULL;

    if (enemy != NULL)
    {
        if (MON_SetUpKnockBack(instance, enemy->instance, (evMonsterHitData *)mv->messageData) != 0)
        {
            MON_PlayAnim(instance, MONSTER_ANIM_HIT1, 1);
        }
        else
        {
            MON_PlayAnim(instance, MONSTER_ANIM_HIT2, 1);
        }
    }

    mv->behaviorState = MONSTER_STATE_STUNNED;

    if (!(mv->auxFlags & 1))
    {
        MON_BirthMana(instance);
        mv->auxFlags |= 1;
    }

    mv->mvFlags |= 0x200000;
    MON_StartSpecialFade(instance, 0x800, 0x14);
}

void SLUAGH_Death(Instance *instance)
{

    if (instance->flags2 & 0x10)
    {
        MON_PlayAnim(instance, MONSTER_ANIM_GENERALDEATH, 2);
    }

    if (instance->flags2 & 2)
    {

        MonsterVars *mv;
        mv = (MonsterVars *)instance->extraData;

        if (mv->enemy != NULL)
        {
            MON_SwitchState(instance, MONSTER_STATE_FLEE);
        }
        else
        {
            MON_SwitchState(instance, MONSTER_STATE_IDLE);
        }
    }

    MON_DefaultQueueHandler(instance);
}

void SLUAGH_AttackEntry(Instance *instance)
{

    MonsterVars *mv;
    mv = (MonsterVars *)instance->extraData;

    if (mv->enemy->mirFlags & 8)
    {
        MON_PlayAnim(instance, MONSTER_ANIM_SOULSUCK, 1);
        mv->auxFlags |= 4;
        return;
    }

    MON_AttackEntry(instance);
}

void SLUAGH_Attack(Instance *instance)
{

    MonsterVars *mv;
    Message *message;

    mv = (MonsterVars *)instance->extraData;

    if (mv->auxFlags & 4)
    {

        if (instance->flags2 & 0x10)
        {
            MON_SwitchState(instance, MONSTER_STATE_COMBAT);
        }

        while (message = DeMessageQueue(&mv->messageQueue), message != NULL)
        {
            if (message->ID == 0x01000009)
            {
                if (((evMonsterSoulSuckData *)message->Data)->sender != gameTrackerX.playerInstance)
                {

                    if (mv->effect == NULL)
                    {

                        short hitpoints;
                        MonsterAttributes *ma;
                        long color;

                        hitpoints = mv->hitPoints;
                        ma = (MonsterAttributes *)instance->data;
                        color = FX_GetHealthColor(hitpoints / 4096);
                        mv->effect = FX_DoInstanceOneSegmentGlow(instance, ma->headSegment, &color, 1, 0x4B0, 0x68, 0x70);
                    }
                    MON_SwitchState(instance, MONSTER_STATE_IDLE);
                }
                continue;
            }
            MON_DefaultMessageHandler(instance, message);
        }

        if (instance->currentMainState != MONSTER_STATE_ATTACK || mv->enemy == NULL)
        {
            mv->auxFlags &= ~4;
            return;
        }
        else
        {
            INSTANCE_Post(mv->enemy->instance, 0x01000009, SetMonsterSoulSuckData(instance, instance->position.x, instance->position.y, instance->position.z));
            return;
        }
    }
    MON_Attack(instance);
}
