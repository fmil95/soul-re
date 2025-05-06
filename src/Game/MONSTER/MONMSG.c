#include "Game/MONSTER/MONMSG.h"
#include "Game/STATE.h"
#include "Game/MONSTER/MONAPI.h"
#include "Game/MONSTER/MONLIB.h"
#include "Game/INSTANCE.h"
#include "Game/GAMELOOP.h"
#include "Game/MONSTER/MONSTER.h"
#include "Game/MONSTER/MONTABLE.h"
#include "Game/MONSTER/MONSENSE.h"
#include "Game/MATH3D.h"

int MON_GroundMoveQueueHandler(Instance *instance)
{
    MonsterVars *mv;
    Message *message;
    int ret;

    ret = 0;

    mv = (MonsterVars *)instance->extraData;

    while ((message = DeMessageQueue(&mv->messageQueue)) != NULL)
    {
        if (message->ID == 0x4010080)
        {
            ret = 0x4010080;
        }
        else
        {
            MON_IdleMessageHandler(instance, message);
        }
    }

    return ret;
}

void MON_PupateMessageHandler(Instance *instance, Message *message)
{
    MonsterVars *mv;
    long id;
    typedef int (*fn)(); // not from decls.h
    fn temp; // not from decls.h

    mv = (MonsterVars *)instance->extraData;

    id = message->ID;

    switch (id)
    {
    case 0x1000000:
        mv->mvFlags &= ~0x80;
        break;
    case 0x1000011:
    {
        evMonsterAlarmData *data;

        data = (evMonsterAlarmData *)message->Data;

        if (MATH3D_LengthXYZ(data->position.x - instance->position.x, data->position.y - instance->position.y, data->position.z - instance->position.z) < mv->subAttr->senses->alarmRadius)
        {
            mv->mvFlags &= ~0x80;

            if ((mv->enemy == NULL) && ((data->sender != NULL) && ((INSTANCE_Query(data->sender, 1) & 0xB))))
            {
                MONSENSE_SetEnemy(instance, data->sender);
            }
        }

        break;
    }
    case 0x80005:
        if ((mv->subAttr->fireVuln != 0) && (!(mv->mvFlags & 0x400)))
        {
            evFXHitData data;

            MON_SetFXHitData(instance, &data, 0x20, 1);

            mv->damageType = 0x20;

            temp = MONTABLE_GetDamageEffectFunc(instance);

            temp(instance, &data);

            MON_SwitchState(instance, MONSTER_STATE_GENERALDEATH);
            break;
        }
    case 0x80006:
        if (mv->subAttr->sunVuln != 0)
        {
            MON_SwitchState(instance, MONSTER_STATE_STUNNED);

            MON_MonsterGlow(instance, 0xFFFFFF, 70, 10, 20);

            mv->damageType = 0x40;
        }

        break;
    case 0x80004:
        if (mv->subAttr->waterVuln != 0)
        {
            MON_SwitchState(instance, MONSTER_STATE_STUNNED);

            mv->damageType = 0x10;
        }

        break;
    case 0x80001:
    case 0x80002:
    case 0x80003:
        break;
    }
}

void MON_IdleMessageHandler(Instance *instance, Message *message)
{
    MonsterVars *mv;
    long id;
    char temp; // not from decls.h

    temp = 0;

    id = message->ID;

    mv = (MonsterVars *)instance->extraData;

    switch (id)
    {
    case 0x1000002:
    case 0x100000A:
    case 0x1000023:
        if (message != NULL) // garbage code for reordering
        {
            temp = -temp;
        }

        if (message->Data == 0)
        {
            MON_SwitchState(instance, MONSTER_STATE_PARRY);
        }

        MON_DefaultMessageHandler(instance, message);
        break;
    case 0x100000E:
        if (!(mv->mvFlags & 0x4))
        {
            MonsterIR *mir;

            mir = (MonsterIR *)message->Data;

            if ((INSTANCE_Query(mir->instance, 1) & 0x1))
            {
                MON_ChangeBehavior(instance, mv->triggeredBehavior);
            }

            MON_SwitchState(instance, MONSTER_STATE_SURPRISED);
        }

        break;
    case 0x1000012:
        if (!(mv->mvFlags & 0x4))
        {
            MonsterIR *mir;

            mir = (MonsterIR *)message->Data;

            if ((INSTANCE_Query(mir->instance, 1) & 0x1))
            {
                MON_ChangeBehavior(instance, mv->triggeredBehavior);
            }

            MON_SwitchState(instance, MONSTER_STATE_NOTICE);
        }

        break;
    default:
        MON_DefaultMessageHandler(instance, message);
    }
}

void MON_DefaultMessageHandler(Instance *instance, Message *message)
{
    MonsterAttributes *ma;
    MonsterVars *mv;
    long id;
    void (*MONTABLE_DamageEffectFunc)(); // not from decls.h
    int temp3;                           // not from decls.h
    short temp2;                         // not from decls.h
    evMonsterImpaleData *temp;           // not from decls.h

    id = message->ID;

    ma = (MonsterAttributes *)instance->data;

    mv = (MonsterVars *)instance->extraData;

    switch (id)
    {
    case 0x100001F:
    case 0x1000021:
    {
        evMonsterHitData *data;
        int fatal;
        int damage;

        if ((mv->mvFlags & 0x200040))
        {
            break;
        }

        fatal = 0;

        data = (evMonsterHitData *)message->Data;

        mv->damageType = (short)data->power;

        damage = 0;

        switch (mv->damageType)
        {
        case 0x20:
            if ((id == 0x1000021) && ((INSTANCE_Query(instance, 0) & 0x10000000)))
            {
                fatal = 1;
            }
            else
            {
                MonsterCombatAttributes *combat;

                combat = mv->subAttr->combatAttributes;

                mv->mvFlags |= 0x800110;

                mv->damageTimer = MON_GetTime(instance) + combat->damageTime;
                mv->stunTimer = MON_GetTime(instance) + combat->stunTime;

                damage = 24576;
            }

            break;
        default:
            if ((instance->flags2 & 0x8000000))
            {
                damage = 8192;
            }
            else
            {
                damage = 12288;
            }
        }

        if (fatal != 0)
        {
            MON_SwitchState(instance, MONSTER_STATE_GENERALDEATH);
        }

        if (damage == 0)
        {
            mv->mvFlags |= 0x40;
            break;
        }
        else
        {
            data->power = damage;
        }
    }
    case 0x1000000:
        if ((!(mv->mvFlags & 0x200040)) && (instance->currentMainState != MONSTER_STATE_GRABBED))
        {
            int state;
            evMonsterHitData *data;

            data = (evMonsterHitData *)message->Data;

            if (data->power >= 12288)
            {
                instance->constrictAngle = 0;
            }

            mv->mvFlags |= 0x40;

            if (INSTANCE_Query(data->sender, 1) == 32)
            {
                MONSENSE_SetEnemy(instance, gameTrackerX.playerInstance);
            }
            else
            {
                MONSENSE_SetEnemy(instance, data->sender);
            }

            if (MON_TakeDamage(instance, data->power, 256) == 0)
            {
                state = 8;
            }
            else
            {
                state = 9;

                if ((mv->mvFlags & 0x2000))
                {
                    state = 16;
                }
            }

            if (state != -1)
            {
                MON_SwitchState(instance, state);
            }

            mv->damageType = 0x100;

            mv->messageData = data;
        }

        break;
    case 0x1000023:
        if (message->Data != 0)
        {
            if (message->Data == 0x1000)
            {
                MONSENSE_SetEnemy(instance, gameTrackerX.playerInstance);

                mv->damageType = 0x400;

                instance->flags2 |= 0x1000;

                MON_SwitchState(instance, MONSTER_STATE_GENERALDEATH);
            }
            else
            {
                MON_SwitchState(instance, MONSTER_STATE_STUNNED);
            }
        }

        break;
    case 0x1000009:
    {
        evMonsterSoulSuckData *data;

        data = (evMonsterSoulSuckData *)message->Data;

        if (MATH3D_LengthXYZ(instance->position.x - data->Destination.x, instance->position.y - data->Destination.y, instance->position.z - data->Destination.z) < 7000)
        {
            MON_SwitchState(instance, MONSTER_STATE_GRABBED);
        }

        break;
    }
    case 0x1000014:
        if (message->Data != 0)
        {
            if (mv->soulJuice < 8192)
            {
                mv->soulJuice = 0;

                mv->damageType = 0;

                MON_SwitchState(instance, MONSTER_STATE_GENERALDEATH);
            }
            else
            {
                MON_SwitchState(instance, MONSTER_STATE_EMBRACE);
            }
        }

        break;
    case 0x1000002:
        if (message->Data != 0)
        {
            MONSENSE_SetEnemy(instance, gameTrackerX.playerInstance);

            MON_SwitchState(instance, MONSTER_STATE_GRABBED);

            mv->mvFlags &= ~0x2;
        }
        else if (mv->enemy != NULL)
        {
            mv->enemy->mirConditions |= 0x40;
        }

        break;
    case 0x100000B:
        MON_SwitchState(instance, MONSTER_STATE_FALL);
        break;
    case 0x100000C:
        if ((!(mv->mvFlags & 0x1)) || (instance->currentMainState != MONSTER_STATE_THROWN))
        {
            int vuln;

            switch (message->Data)
            {
            case 0x10:
                vuln = mv->subAttr->waterVuln;
                break;
            case 0x20:
                vuln = 0;

                if (!(mv->mvFlags & 0x400))
                {
                    vuln = mv->subAttr->fireVuln;
                }

                break;
            case 0x40:
                vuln = mv->subAttr->sunVuln;
                break;
            default:
                vuln = 0;
            }

            if (vuln != 0)
            {
                mv->damageType = message->Data;

                INSTANCE_Post(instance, 0x400000, SetFXHitData(NULL, 0, 0, message->Data));
            }

            if (vuln == 1)
            {
                MON_SwitchState(instance, MONSTER_STATE_ENVIRONMENTDAMAGE);
            }
            else if ((vuln != 0) && (vuln < 4))
            {
                mv->mvFlags |= 0x200000;

                MON_SwitchState(instance, MONSTER_STATE_GENERALDEATH);
            }
        }

        break;
    case 0x1000019:
        MONSENSE_SetEnemy(instance, gameTrackerX.playerInstance);

        mv->held = (Instance *)message->Data;

        MON_SwitchState(instance, MONSTER_STATE_MISSILEHIT);
        break;
    case 0x100000A:
        temp = (evMonsterImpaleData *)message->Data;

        if (temp != NULL)
        {
            mv->held = (Instance *)temp->weapon;

            temp2 = temp->position.x + ((temp->distance * rsin(temp->direction.z)) / 4096);

            instance->position.y = temp->position.y - ((temp->distance * rcos(temp->direction.z)) / 4096);
            instance->position.x = temp2;
            instance->rotation.z = (temp->direction.z + 2048) & 0xFFF;

            MON_SwitchState(instance, MONSTER_STATE_IMPALEDEATH);
        }
        else if (mv->enemy != NULL)
        {
            mv->enemy->mirConditions |= 0x80;
        }

        break;
    case 0x100001C:
        mv->terrainImpaleID = message->Data;

        MON_MoveInstanceToImpalePoint(instance);

        MON_SwitchState(instance, MONSTER_STATE_TERRAINIMPALEDEATH);
        break;
    case 0x400000:
        MONTABLE_DamageEffectFunc = MONTABLE_GetDamageEffectFunc(instance);

        MONTABLE_DamageEffectFunc(instance, message->Data);
        break;
    case 0x1000011:
    {
        evMonsterAlarmData *data;
        Instance *enemy;

        data = (evMonsterAlarmData *)message->Data;

        enemy = data->sender;

        if ((enemy != NULL) && ((INSTANCE_Query(enemy, 1) & 0xB)) && (MONSENSE_FindIR(mv, enemy) == NULL) && (MATH3D_LengthXYZ(instance->position.x - data->position.x, instance->position.y - data->position.y, instance->position.z - data->position.z) < mv->subAttr->senses->alarmRadius))
        {
            MonsterIR *mir;

            mir = MONSENSE_SetEnemy(instance, enemy);

            if (mir != NULL)
            {
                INSTANCE_Post(instance, 0x1000012, (intptr_t)mir);
            }
        }

        break;
    }
    case 0x100000E:
    case 0x1000012:
        if (!(mv->mvFlags & 0x4))
        {
            MonsterIR *mir;

            mir = (MonsterIR *)message->Data;

            if ((INSTANCE_Query(mir->instance, 1) & 0x1))
            {
                mv->behaviorState = mv->triggeredBehavior;
            }
        }

        break;
    case 0x1000016:
        if ((signed char)mv->subAttr->combatAttributes->hitPoints != 0)
        {
            int hp;

            hp = mv->hitPoints + message->Data;

            if (hp > 24576)
            {
                hp = 24576;
            }

            mv->hitPoints = hp;
        }

        break;
    case 0x100001A:
        if (message->Data != 0)
        {
            if (!(mv->mvFlags & 0x20000000))
            {
                mv->mvFlags |= 0x20000000;

                mv->hitPoints *= 2;
            }
        }
        else if ((mv->mvFlags & 0x20000000))
        {
            mv->mvFlags &= ~0x20000000;

            mv->hitPoints = (mv->hitPoints + 1) / 2;
        }

        break;
    case 0x80005:
        if (mv->subAttr->fireVuln != 0)
        {
            evFXHitData data;

            MON_SetFXHitData(instance, &data, 0x20, 1);

            mv->damageType = 0x20;

            MONTABLE_DamageEffectFunc = MONTABLE_GetDamageEffectFunc(instance);

            MONTABLE_DamageEffectFunc(instance, (intptr_t)&data);

            MON_SwitchState(instance, MONSTER_STATE_GENERALDEATH);
            break;
        }
    case 0x80006:
        if (!(ma->whatAmI & 0x10008))
        {
            MON_SwitchState(instance, MONSTER_STATE_GENERALDEATH);

            MON_MonsterGlow(instance, 0xFFFFFF, 70, 10, 20);

            mv->damageType = 0x40;
        }
        else
        {
            INSTANCE_Post(instance, 0x1000011, SetMonsterAlarmData(gameTrackerX.playerInstance, &gameTrackerX.playerInstance->position, 2));
        }

        break;
    case 0x80002:
        if (instance->petrifyValue == 0)
        {
            MON_SwitchState(instance, MONSTER_STATE_PETRIFIED);

            instance->petrifyValue = 4096;
        }

        break;
    case 0x80004:
        if ((mv->subAttr->waterVuln != 0) && (!(ma->whatAmI & 0x8)))
        {
            MON_SwitchState(instance, MONSTER_STATE_STUNNED);

            MON_MonsterGlow(instance, 0xFF0000, 70, 10, 20);

            mv->damageType = 0x10;
        }
        else
        {
            INSTANCE_Post(instance, 0x1000011, SetMonsterAlarmData(gameTrackerX.playerInstance, &gameTrackerX.playerInstance->position, 2));
        }

        break;
    case 0x80003:
        if (mv->subAttr->soundVuln != 0)
        {
            evFXHitData data;

            MON_SetFXHitData(instance, &data, 0x200, 16);

            mv->damageType = 0x200;

            mv->hitPoints = 0;

            mv->mvFlags |= 0x800000;

            MONTABLE_DamageEffectFunc = MONTABLE_GetDamageEffectFunc(instance);

            MONTABLE_DamageEffectFunc(instance, (intptr_t)&data);

            MON_SwitchState(instance, MONSTER_STATE_STUNNED);
        }
        else
        {
            INSTANCE_Post(instance, 0x1000011, SetMonsterAlarmData(gameTrackerX.playerInstance, &gameTrackerX.playerInstance->position, 2));
        }

        break;
    case 0x80001:
        if ((instance->zVel == 0) && (!(mv->mvFlags & 0x200000)))
        {
            temp3 = MATH3D_AngleFromPosToPos(&gameTrackerX.playerInstance->position, &instance->position);

            instance->position.z += 120;
            instance->oldPos.z += 120;

            MON_LaunchMonster(instance, temp3, 100, 70);
        }

        break;
    case 0x40009:
        mv->damageType = 0x2000;

        MON_SwitchState(instance, MONSTER_STATE_STUNNED);
        break;
    case 0x4000D:
    {
        evPositionData *posData;

        posData = (evPositionData *)message->Data;

        mv->auxFlags |= 0x10000000;

        mv->destination.z = posData->z;
        break;
    }
    case 0x4000C:
    case 0x40016:
    {
        evPositionData *posData;

        posData = (evPositionData *)message->Data;

        mv->destination.x = posData->x;
        mv->destination.y = posData->y;
        mv->destination.z = posData->z;

        mv->mvFlags |= 0x40000;

        MON_SwitchState(instance, MONSTER_STATE_WANDER);
        break;
    }
    case 0x4000F:
    {
        evPositionData *posData;

        posData = (evPositionData *)message->Data;

        mv->mvFlags |= 0x80000000;

        mv->lookAtPosData.x = posData->x;
        mv->lookAtPosData.y = posData->y;
        mv->lookAtPosData.z = posData->z;
        break;
    }
    case 0x40010:
        mv->mvFlags &= ~0x80000000;
        break;
    case 0x40017:
    {
        static int deaths[] = {
            0x20, 0x40, 0x10,
            0x200, 0x80, 0x400,
            0x100, 0x400, 0x100};

        mv->damageType = deaths[message->Data];

        MON_SwitchState(instance, MONSTER_STATE_GENERALDEATH);
        break;
    }
    case 0x2000002:
    {
        evMonsterHitData *data;

        data = (evMonsterHitData *)message->Data;

        INSTANCE_Post(data->sender, 0x1000000, SetMonsterHitData(instance, NULL, data->power, mv->attackType->knockBackDistance, (signed char)mv->attackType->knockBackDuration));
        break;
    }
    case 0x40003:
    {
        evActionPlayHostAnimationData *data;
        int i;

        data = (evActionPlayHostAnimationData *)message->Data;

        for (i = mv->subAttr->numSections - 1; i >= 0; i--)
        {
            G2EmulationInstanceToInstanceSwitchAnimation(instance, data->host, i, data->newAnim, data->newFrame, data->frames, data->mode);
        }

        break;
    }
    case 0x8000008:
    {
        evAnimationInstanceSwitchData *data;

        data = (evAnimationInstanceSwitchData *)message->Data;

        MON_PlayAnimID(instance, data->anim, data->mode);
        break;
    }
    case 0x1000007:
    case 0x1000008:
    case 0x4010080:
        break;
    }
}

void MON_PupateQueueHandler(Instance *instance)
{
    MonsterVars *mv;
    Message *message;

    mv = (MonsterVars *)instance->extraData;

    while ((message = DeMessageQueue(&mv->messageQueue)) != NULL)
    {
        MON_PupateMessageHandler(instance, message);
    }
}

void MON_IdleQueueHandler(Instance *instance)
{
    MonsterVars *mv;
    Message *message;

    mv = (MonsterVars *)instance->extraData;

    while ((message = DeMessageQueue(&mv->messageQueue)) != NULL)
    {
        MON_IdleMessageHandler(instance, message);
    }
}

void MON_DefaultQueueHandler(Instance *instance)
{
    MonsterVars *mv;
    Message *message;

    mv = (MonsterVars *)instance->extraData;

    while ((message = DeMessageQueue(&mv->messageQueue)) != NULL)
    {
        MON_DefaultMessageHandler(instance, message);
    }
}
