#include "Game/MONSTER/HUMAN.h"
#include "Game/GAMELOOP.h"
#include "Game/MONSTER/MONSENSE.h"
#include "Game/MONSTER/MONTABLE.h"
#include "Game/MONSTER/MONLIB.h"
#include "Game/MONSTER/MONMSG.h"
#include "Game/MONSTER/MONSTER.h"
#include "Game/GAMEPAD.h"
#include "Game/SAVEINFO.h"
#include "Game/PSX/SUPPORT.h"
#include "Game/MONSTER/MONAPI.h"
#include "Game/OBTABLE.h"
#include "Game/INSTANCE.h"
#include "Game/STATE.h"
#include "Game/SOUND.h"
#include "Game/PSX/MAIN.h"

MonsterStateChoice HUMAN_StateChoiceTable[] = {
    {9, {HUMAN_StunnedEntry, HUMAN_Stunned}},
    {0x17, {HUMAN_DeadEntry, HUMAN_Dead}},
    {0x1B, {HUMAN_EmbraceEntry, HUMAN_Embrace}},
    {2, {HUMAN_IdleEntry, HUMAN_Idle}},
    {0x13, {MON_FleeEntry, HUMAN_Flee}},
    {-1, {NULL, NULL}},
};

MonsterFunctionTable HUMAN_FunctionTable = {
    HUMAN_Init,
    HUMAN_CleanUp,
    NULL,
    HUMAN_Query,
    NULL,
    HUMAN_StateChoiceTable,
    monVersion,
    NULL,
};

typedef void (*MONTABLE_InitFunc)(Instance *);

void HUMAN_WaitForWeapon(Instance *instance, GameTracker *gameTracker)
{
    (void)gameTracker;
    ((MONTABLE_InitFunc)MONTABLE_GetInitFunc(instance))(instance);

    if (instance->LinkChild != NULL)
    {
        instance->processFunc = MonsterProcess;

        instance->flags &= ~0x800;

        instance->flags2 &= ~0x20000000;
        instance->flags2 &= ~0x80;
    }
}

Instance *HUMAN_CreateWeapon(Instance *instance, int weaponid, int segment)
{
    Object *weapon;
    Instance *iweapon;

    weapon = (Object *)objectAccess[weaponid].object;

    if (weapon != NULL)
    {
        iweapon = INSTANCE_BirthObject(instance, weapon, 0);

        if (iweapon != NULL)
        {
            INSTANCE_Post(iweapon, 0x800002, SetObjectData(0, 0, 0, instance, segment));

            iweapon->flags2 |= 0x20000;

            return iweapon;
        }
    }

    instance->processFunc = HUMAN_WaitForWeapon;

    instance->flags |= 0x800;

    instance->flags2 |= 0x20000080;

    return NULL;
}

void HUMAN_Init(Instance *instance)
{
    MonsterVars *mv;
    MonsterAttributes *ma;

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;

    if (!(ma->whatAmI & 0x2000))
    {
        int opinion;
        MonsterAllegiances *allegiances;

        (void)opinion;

        allegiances = mv->subAttr->allegiances;
        if (GlobalSave->humanOpinionOfRaziel > 0)
        {
            allegiances->enemies &= ~0x1;
            allegiances->gods |= 0x1;
            allegiances->allies |= 0x1;
        }
        else
        {
            allegiances->gods &= ~0x1;
            allegiances->allies &= ~0x1;
            allegiances->enemies |= 0x1;
        }
    }
    if (strcmpi(instance->object->name, "vlgrb___") == 0)
    {
        mv->auxFlags |= 0x20;
    }
    MON_DefaultInit(instance);
    mv->soulJuice = 16384;
    mv->mvFlags |= 0x2000;
}

/*Unused*/ static char D_800D1AC8[] = "";
/*Unused*/ static char D_800D1ACC[] = "Switching possession to %s\n";

void HUMAN_CleanUp(Instance *instance)
{
    MON_CleanUp(instance);
}

uintptr_t HUMAN_Query(Instance *instance, unsigned long query)
{
    MonsterVars *mv;
    MonsterAttributes *ma;
    unsigned long ret;

    ma = (MonsterAttributes *)instance->data;

    mv = (MonsterVars *)instance->extraData;

    if (ma == NULL)
    {
        return 0;
    }

    switch (query)
    {
    case 0:
        if ((mv->mvFlags & 0x200))
        {
            ret = 0x40000000;
            break;
        }

        ret = 0x12000000;

        if (instance->currentMainState != MONSTER_STATE_PETRIFIED)
        {
            ret = ((mv->mvFlags & 0x100) != 0) << 29;

            if (!(mv->mvFlags & 0x200000))
            {
                if ((mv->hitPoints <= 4096) || ((mv->auxFlags & 0x3)))
                {
                    ret |= 0x08000000;
                }
            }
        }

        break;
    case 37:
        if (!(ma->whatAmI & 0x8000))
        {
            ret = mv->subAttr->allegiances->enemies & 0x1;
        }
        else
        {
            ret = 0;
        }

        break;
    default:
        ret = MonsterQuery(instance, query);
    }

    return ret;
}

void HUMAN_DeadEntry(Instance *instance)
{
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    mv->soulJuice /= 4;

    if (mv->soulJuice > 4096)
    {
        mv->soulJuice = 4096;
    }

    MON_DeadEntry(instance);

    mv->damageTimer = MON_GetTime(instance);

    MON_BirthMana(instance);
}

void HUMAN_Dead(Instance *instance)
{
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    instance->fadeValue = (MON_GetTime(instance) - mv->damageTimer);

    if (mv->causeOfDeath == 6)
    {
        MON_Dead(instance);
        return;
    }

    if (instance->fadeValue >= 4096)
    {
        MON_KillMonster(instance);
    }

    if (((mv->mvFlags & 0x400000)) && (mv->effectTimer < MON_GetTime(instance)))
    {
        mv->mvFlags &= ~0x400000;
    }

    if (!(mv->mvFlags & 0x2))
    {
        MON_ApplyPhysics(instance);
    }

    while (DeMessageQueue(&mv->messageQueue) != NULL)
        ;
}

void HUMAN_StunnedEntry(Instance *instance)
{
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    if ((mv->auxFlags & 0x10))
    {
        mv->generalTimer = MON_GetTime(instance) + 60000;

        MON_PlayAnim(instance, MONSTER_ANIM_SOULSUCK, 1);
    }
    else
    {
        MON_StunnedEntry(instance);
    }
}

void HUMAN_Stunned(Instance *instance)
{
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    if ((mv->auxFlags & 0x10))
    {
        if ((instance->flags2 & 0x10))
        {
            MON_PlayAnim(instance, MONSTER_ANIM_STUNNED, 2);
        }

        if (mv->generalTimer < MON_GetTime(instance))
        {
            mv->soulJuice = 16384;

            mv->auxFlags &= ~0x10;
        }

        MON_DefaultQueueHandler(instance);
    }
    else
    {
        MON_Stunned(instance);
    }
}

void HUMAN_EmbraceEntry(Instance *instance)
{
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    MON_PlayAnim(instance, MONSTER_ANIM_EMBRACE, 1);

    MON_TurnOffBodySpheres(instance);

    mv->generalTimer = mv->soulJuice / 4096;
}

void HUMAN_Embrace(Instance *instance)
{
    MonsterVars *mv;
    Message *message;
    int letgo;
    int juice;

    letgo = 0;

    mv = (MonsterVars *)instance->extraData;

    MON_TurnToPosition(instance, &gameTrackerX.playerInstance->position, 4096);

    while ((message = DeMessageQueue(&mv->messageQueue)) != NULL)
    {
        if ((message != NULL) && (message->ID == 0x1000014))
        {
            letgo = 1;
        }
        else
        {
            MON_DefaultMessageHandler(instance, message);
        }
    }

    juice = (mv->generalTimer * gameTrackerX.timeMult * 33) / 5000;

    INSTANCE_Post(gameTrackerX.playerInstance, 0x1000016, juice);

    do
    {
    } while (0); // garbage code for reordering

    if (mv->soulJuice < juice)
    {
        mv->soulJuice = 0;
    }
    else
    {
        mv->soulJuice -= juice;
    }

    GAMEPAD_Shock1(128 - ((mv->soulJuice << 7) / (mv->generalTimer << 12)), 61440);

    if (mv->soulJuice == 0)
    {
        mv->damageType = 0;

        MON_SwitchState(instance, MONSTER_STATE_GENERALDEATH);

        INSTANCE_Post(gameTrackerX.playerInstance, 0x1000006, (intptr_t)instance);

        mv->soulJuice = 0;

        SOUND_Play3dSound(&instance->position, 8, -450, 80, 3500);
    }
    else if (letgo != 0)
    {
        mv->auxFlags |= 0x10;

        MON_SwitchState(instance, MONSTER_STATE_STUNNED);

        MON_TurnOnBodySpheres(instance);
    }
    else if (instance->currentMainState != MONSTER_STATE_EMBRACE)
    {
        INSTANCE_Post(gameTrackerX.playerInstance, 0x1000006, (intptr_t)instance);

        MON_TurnOnBodySpheres(instance);
    }
}

void HUMAN_IdleEntry(Instance *instance)
{
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    MON_IdleEntry(instance);

    mv->auxFlags &= ~0x2;
    mv->auxFlags &= ~0x4;
    mv->auxFlags &= ~0x1;
}

void HUMAN_Idle(Instance *instance)
{
    MonsterVars *mv;
    MonsterAttributes *ma;
    MonsterIR *ally;
    MonsterIR *enemy;

    mv = (MonsterVars *)instance->extraData;

    ma = (MonsterAttributes *)instance->data;

    ally = mv->ally;

    enemy = mv->enemy;

    if ((!(mv->mvFlags & 0x4)) && (ally != NULL) && ((ally->mirFlags & 0x4)))
    {
        if ((mv->auxFlags & 0x2))
        {
            MON_TurnToPosition(instance, &ally->instance->position, mv->subAttr->speedPivotTurn);

            if ((instance->flags2 & 0x2))
            {
                mv->auxFlags = (mv->auxFlags & ~0x2) | 0x1;
            }

            MON_DefaultQueueHandler(instance);
            return;
        }

        if ((mv->auxFlags & 0x1))
        {
            if ((ally->distance >= 2000) || ((enemy != NULL) && (enemy->distance < 2000)))
            {
                mv->auxFlags = (mv->auxFlags & ~0x1) | 0x4;

                MON_PlayAnimFromList(instance, ma->auxAnimList, 1, 1);
            }

            MON_DefaultQueueHandler(instance);
            return;
        }

        if ((mv->auxFlags & 0x4))
        {
            if ((instance->flags2 & 0x10))
            {
                mv->auxFlags &= ~0x4;

                MON_PlayRandomIdle(instance, 2);
            }

            MON_DefaultQueueHandler(instance);
            return;
        }

        if ((ally->distance < 2000) && ((enemy == NULL) || (enemy->distance >= 2000)))
        {
            mv->auxFlags |= 0x2;

            MON_PlayAnimFromList(instance, ma->auxAnimList, 0, 2);

            MON_DefaultQueueHandler(instance);
            return;
        }
    }

    MON_Idle(instance);
}

void HUMAN_Flee(Instance *instance)
{
    MonsterVars *mv;
    MonsterIR *enemy;

    mv = (MonsterVars *)instance->extraData;

    enemy = mv->enemy;

    if ((enemy != NULL) && (enemy->distance < 640))
    {
        if (!(mv->auxFlags & 0x8))
        {
            MonsterAttributes *ma;

            ma = (MonsterAttributes *)instance->data;

            MON_PlayAnimFromList(instance, ma->auxAnimList, 2, 2);

            mv->auxFlags |= 0x8;

            if ((mv->auxFlags & 0x20))
            {
                SOUND_Play3dSound(&instance->position, 459, 0, 88, 3500);
            }
            else
            {
                SOUND_Play3dSound(&instance->position, 458, -100, 92, 3500);
            }
        }

        MON_TurnToPosition(instance, &enemy->instance->position, mv->subAttr->speedPivotTurn);

        MON_DefaultQueueHandler(instance);
    }
    else if ((mv->auxFlags & 0x8))
    {
        MON_SwitchState(instance, MONSTER_STATE_FLEE);
    }
    else
    {
        MON_Flee(instance);
    }

    if (((mv->auxFlags & 0x8)) && ((mv->mvFlags & 0x1)))
    {
        mv->auxFlags &= ~0x8;
    }
}

void HUMAN_GetAngry()
{
    Instance *instance;

    instance = gameTrackerX.instanceList->first;

    while (instance != NULL)
    {
        if ((INSTANCE_Query(instance, 1) & 0xC000))
        {
            MonsterVars *mv;
            MonsterAllegiances *allegiances;
            MonsterIR *mir;

            mv = (MonsterVars *)instance->extraData;

            allegiances = mv->subAttr->allegiances;

            do
            {
            } while (0); // garbage code for reordering

            allegiances->gods &= ~0x1;

            allegiances->allies &= ~0x1;

            allegiances->enemies |= 0x1;

            mir = MONSENSE_SetEnemy(instance, gameTrackerX.playerInstance);

            if (mir != NULL)
            {
                mir->mirFlags &= ~0x6;
            }
        }

        instance = instance->next;
    }
}

int HUMAN_TypeOfHuman(Instance *instance)
{
    int type;
    MonsterVars *mv;

    type = INSTANCE_Query(instance, 1);

    mv = (MonsterVars *)instance->extraData;

    if ((type & 0x4000))
    {
        return 1;
    }

    if ((type & 0x2000))
    {
        return 4;
    }

    if (!(type & 0x8000))
    {
        return 0;
    }

    if ((mv->auxFlags & 0x20))
    {
        return 3;
    }

    return 2;
}
