#include "Overlays/walbosb/walbosb.h"
#include "Game/DEBUG.h"
#include "Game/GAMELOOP.h"
#include "Game/G2/ANMCTRLR.h"
#include "Game/INSTANCE.h"
#include "Game/MATH3D.h"
#include "Game/MEMPACK.h"
#include "Game/OBTABLE.h"
#include "Game/SOUND.h"
#include "Game/MONSTER/MONAPI.h"
#include "Game/MONSTER/MONLIB.h"
#include "Game/MONSTER/MONMSG.h"
#include "Game/MONSTER/MONSTER.h"

// TODO: Delete once matched
int WALBOSB_ChooseAttack(Instance *instance, MonsterIR *enemy);
int WALBOSB_ShouldIAttack(Instance *instance, MonsterIR *enemy, int attack);
int WALBOSB_TurnToPosition(Instance *instance, Position *target, int speed);

// this conditional is for the objdiff report
#ifndef SKIP_ASM

void WALBOSB_SetYaw(Instance *instance, int yaw)
{
    short z; // not from debug symbols

    z = (instance->intro->rotation.z << 16) >> 16; // Garbage shifts to force a signed load
    instance->rotation.z = (z + yaw) & 0xFFF;
}

void WALBOSB_SetPitch(Instance *instance, int pitch)
{

    G2SVector3 extraRot;
    MonsterVars *mv; // not from debug symbols
    WalbosbVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (WalbosbVars *)mv->extraVars;

    if (vars != NULL)
    {
        vars->pitch = pitch;
        extraRot.x = pitch;
        extraRot.y = 0;
        extraRot.z = 0;
        G2Anim_SetController_Vector(&instance->anim, 0, G2ANIM_CTRLRTYPE_ADD_LOCALROT, &extraRot);
    }
}

void WALBOSB_InterpYaw(Instance *instance, int yaw, int speed)
{
    int z; // not from debug symbols

    z = (instance->intro->rotation.z << 16) >> 16; // Garbage shifts to force signed load

    AngleMoveToward(&instance->rotation.z, (z + yaw) & 0xFFF, (gameTrackerX.timeMult * speed * 16) >> 16);
    instance->rotation.z &= 0xFFF;
}

void WALBOSB_InterpPitch(Instance *instance, int pitch, int speed)
{

    G2SVector3 extraRot;
    int newPitch; // not from debug symbols
    WalbosbVars *vars; // not from debug symbols
    WalbosbAttributes *attrs; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols

    newPitch = pitch;
    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (WalbosbVars *)mv->extraVars;
    attrs = (WalbosbAttributes *)ma->tunData;

    if (attrs != NULL && vars != NULL)
    {

        if (newPitch < -attrs->maxPitchAngle)
        {
            newPitch = -attrs->maxPitchAngle & 0xFFF;
        }
        else if (attrs->maxPitchAngle < newPitch)
        {
            newPitch = attrs->maxPitchAngle;
        }

        AngleMoveToward(&vars->pitch, newPitch, (gameTrackerX.timeMult * speed * 16) >> 16);

        extraRot.x = vars->pitch;
        extraRot.y = 0;
        extraRot.z = 0;

        G2Anim_SetController_Vector(&instance->anim, 0, G2ANIM_CTRLRTYPE_ADD_LOCALROT, &extraRot);
    }
}

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_TurnToPosition);

void WALBOSB_ElevateToPosition(Instance *instance, Position *target, int speed, int behind)
{

    int elevation; // not from debug symbols

    elevation = MATH3D_ElevationFromPosToPos(&instance->position, target);

    if (behind != 0)
    {
        elevation -= 1024;
    }
    else
    {
        elevation = 1024 - elevation;
    }

    WALBOSB_InterpPitch(instance, elevation + 128, speed);
}


INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_OtherAttackingLegs);

void WALBOSB_ResetAbortedAttacks(Instance *instance)
{

    Instance *inst; // not from debug symbols

    (void)instance;

    for (inst = gameTrackerX.instanceList->first; inst != NULL; inst = inst->next)
    {
        if (INSTANCE_Query(inst, queryWhatAmI) == 0x410002)
        {

            MonsterVars *mv; // not from debug symbols
            WalbosbVars *vars; // not from debug symbols

            mv = (MonsterVars *)inst->extraData;
            vars = (WalbosbVars *)mv->extraVars;

            if (vars != NULL)
            {
                vars->abortedAttacks = 0;
            }
        }
    }
}

int WALBOSB_AbortedAttacks(Instance *instance)
{

    Instance *inst; // not from debug symbols
    int numAborted; // not from debug symbols

    (void)instance;

    numAborted = 0;

    for (inst = gameTrackerX.instanceList->first; inst != NULL; inst = inst->next)
    {
        if (INSTANCE_Query(inst, queryWhatAmI) == 0x410002)
        {

            MonsterVars *mv; // not from debug symbols
            WalbosbVars *vars; // not from debug symbols

            mv = (MonsterVars *)inst->extraData;
            vars = (WalbosbVars *)mv->extraVars;

            if (vars != NULL)
            {
                numAborted += vars->abortedAttacks;
            }
        }
    }

    return numAborted;
}

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_ShouldIAttack);

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_ChooseAttack);

void WALBOSB_WalbossMessage(int message)
{
    Instance *inst; // not from debug symbols

    for (inst = gameTrackerX.instanceList->first; inst != NULL; inst = inst->next)
    {
        if (INSTANCE_Query(inst, queryWhatAmI) == 0x10102)
        {
            INSTANCE_Post(inst, 0x01000017, message);
            break;
        }
    }
}

INCLUDE_RODATA("asm/nonmatchings/Overlays/walbosb/walbosb", D_88000000);

INCLUDE_RODATA("asm/nonmatchings/Overlays/walbosb/walbosb", D_88000020);

void WALBOSB_AutofaceMarker(Instance *instance)
{

    Instance *inst; // not from debug symbols
    Object *object; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    WalbosbVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (WalbosbVars *)mv->extraVars;

    if (vars != NULL)
    {
        object = OBTABLE_FindObject("autoface");

        if (object != NULL)
        {
            inst = INSTANCE_BirthObject(instance, object, 0);

            if (inst != NULL)
            {
                inst->flags |= 0x800;
                inst->flags2 &= ~0x20;
                vars->autofaceInst = inst;
            }
        }
    }
}

void WALBOSB_SetAutofacePos(Instance *instance)
{

    HModel *model; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    WalbosbVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (WalbosbVars *)mv->extraVars;
    model = &instance->hModelList[instance->currentModel];

    if (vars != NULL && vars->autofaceInst != NULL)
    {

        int i; // not from debug symbols
        int dist; // not from debug symbols
        int minDist; // not from debug symbols
        HPrim *hPrim; // not from debug symbols

        minDist = 999999;

        for (i = model->numHPrims, hPrim = &model->hPrimList[0]; i != 0; i--, hPrim++)
        {
            if (hPrim->type == 1)
            {

                HSphere *hSphere; // not from debug symbols
                hSphere = hPrim->data.hsphere;

                if (hSphere->id == 8)
                {

                    Position pos;

                    MON_SphereWorldPos(&instance->matrix[hPrim->segment], hSphere, &pos);
                    dist = MATH3D_LengthXYZ(pos.x - gameTrackerX.playerInstance->position.x, pos.y - gameTrackerX.playerInstance->position.y, pos.z - gameTrackerX.playerInstance->position.z);

                    if (dist < minDist)
                    {
                        minDist = dist;
                        COPY_SVEC(Position, &vars->autofaceInst->position, Position, &pos);
                    }
                }
            }
        }

        vars->autofaceInst->flags2 |= 0x20;
    }
}

void WALBOSB_ResetSetAutofacePos(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    WalbosbVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (WalbosbVars *)mv->extraVars;

    if (vars != NULL && vars->autofaceInst != NULL)
    {
        vars->autofaceInst->flags2 &= ~0x20;
        COPY_SVEC(Position, &vars->autofaceInst->position, Position, &instance->position);
    }
}


void WALBOSB_Collide(Instance *instance, GameTracker *gameTracker)
{

    CollideInfo *collideInfo; // not from debug symbols
    HSphere *hSphere; // not from debug symbols

    collideInfo = (CollideInfo *)instance->collideInfo;
    hSphere = (HSphere *)collideInfo->prim0;

    if (collideInfo->type0 != 1 || hSphere->id != 9)
    {
        MonsterCollide(instance, gameTracker);
    }
    else
    {

        Instance *collideInst; // not from debug symbols
        collideInst = (Instance *)collideInfo->inst1;

        if (collideInst == NULL)
        {
            MonsterCollide(instance, gameTracker);
        }
        else
        {

            uintptr_t queryResult; // not from debug symbols
            queryResult = INSTANCE_Query(collideInst, queryWhatAmI);

            if (queryResult != 0x10102 && queryResult != 0x410002)
            {
                MonsterCollide(instance, gameTracker);
            }
        }
    }
}

uintptr_t WALBOSB_Query(Instance *instance, unsigned long query)
{
    if (query != 0)
    {
        return MonsterQuery(instance, query);
    }

    return 0;
}

void WALBOSB_Message(Instance *instance, unsigned long message, unsigned long data)
{

    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    switch (message)
    {
    case 0x400000:
        if (data != 0)
        {
            SOUND_Play3dSound(&instance->position, 21, 650, 80, 15500);
        }
        MonsterMessage(instance, message, data);
        break;
    case 0x100001F:
    case 0x1000021:
        ((evMonsterHitData *)data)->power = 4096;
        MonsterMessage(instance, message, data);
        break;
    case 0x1000020:
        if (!(mv->mvFlags & 4))
        {
            if (instance->currentMainState != MONSTER_STATE_DEAD && instance->currentMainState != MONSTER_STATE_GENERALDEATH)
            {
                MON_SwitchStateDoEntry(instance, MONSTER_STATE_IDLE);
                break;
            }
        }
        break;
    case 0x1000017:
        switch (data)
        {
        case 0:
            if (instance->currentMainState != MONSTER_STATE_GENERALDEATH && instance->currentMainState != MONSTER_STATE_DEAD)
            {
                mv->auxFlags |= 4;
                MON_SwitchStateDoEntry(instance, MONSTER_STATE_DEAD);
            }
            break;
        case 1:
            if (instance->currentMainState != MONSTER_STATE_GENERALDEATH && instance->currentMainState != MONSTER_STATE_DEAD)
            {
                mv->auxFlags |= 1;
                MON_SwitchStateDoEntry(instance, MONSTER_STATE_HIT);
            }
            break;
        case 2:
            if (instance->currentMainState != MONSTER_STATE_GENERALDEATH && instance->currentMainState != MONSTER_STATE_DEAD)
            {
                mv->auxFlags &= ~1;
                MON_SwitchStateDoEntry(instance, MONSTER_STATE_COMBAT);
            }
            break;
        case 3:
            if (!(mv->auxFlags & 2))
            {
                mv->auxFlags |= 2;
                MON_SwitchStateDoEntry(instance, MONSTER_STATE_IDLE);
            }
            else
            {
                mv->auxFlags &= ~2;
            }
            break;
        case 4:
            mv->auxFlags &= ~2;
            break;
        default:
            break;
        }
        break;
    case 0x40009:
    case 0x80001:
    case 0x80002:
    case 0x80003:
    case 0x80004:
    case 0x80005:
    case 0x80006:
        break;
    default:
        MonsterMessage(instance, message, data);
    }
}

void WALBOSB_Init(Instance *instance)
{

    WalbosbVars *vars; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols

    MON_DefaultInit(instance);

    mv = (MonsterVars *)instance->extraData;

    if (mv != NULL)
    {
        vars = (WalbosbVars *)MEMPACK_Malloc(sizeof(WalbosbVars), MEMORY_TYPE_WALBOSBDATA);

        if (vars == NULL)
        {
            mv->extraVars = NULL;
        }
        else
        {
            mv->extraVars = vars;
            vars->pitch = 0;
            vars->tauntState = 0;
            vars->abortedAttacks = 0;
            WALBOSB_AutofaceMarker(instance);
            WALBOSB_SetAutofacePos(instance);
        }
    }

    G2Anim_AttachControllerToSeg(&instance->anim, 0, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_EnableController(&instance->anim, 0, G2ANIM_CTRLRTYPE_ADD_LOCALROT);

    instance->xVel = 0;
    instance->yVel = 0;
    instance->zVel = 0;

    mv->mvFlags |= 0x800;

    MON_SwitchState(instance, MONSTER_STATE_IDLE);

    ma = (MonsterAttributes *)instance->data;
    MON_PlayAnimFromList(instance, ma->auxAnimList, 0, 1);

    mv->auxFlags |= 0x20000000;
    mv->mvFlags |= 0x2000;
    instance->collideFunc = WALBOSB_Collide;
    mv->soulJuice = 0;
    mv->targetFade = 0;

    DEBUG_DoAreaProtection();
}

void WALBOSB_CleanUp(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    WalbosbVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    G2Anim_DetachControllerFromSeg(&instance->anim, 0, G2ANIM_CTRLRTYPE_ADD_LOCALROT);

    if (mv != NULL)
    {
        vars = (WalbosbVars *)mv->extraVars;

        if (vars != NULL)
        {
            if (vars->autofaceInst != NULL)
            {
                INSTANCE_KillInstance(vars->autofaceInst);
            }

            MEMPACK_Free((char *)vars);
        }
    }

    MON_CleanUp(instance);
}


void WALBOSB_IdleEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    WalbosbVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (WalbosbVars *)mv->extraVars;

    if (vars != NULL)
    {
        if (!(mv->mvFlags & 4))
        {
            if (mv->auxFlags & 2)
            {
                mv->attackState = 0;
                MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 0, 1);
            }
            else
            {
                MON_PlayRandomIdle(instance, 2);
            }
        }
        else
        {
            MON_IdleEntry(instance);
        }
    }
}

int WALBOSB_HandleFade(Instance *instance)
{

    int fade; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    WalbosbVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (WalbosbVars *)mv->extraVars;

    switch (vars->tauntState)
    {
    case 0:
        if (gameTrackerX.gameData.asmData.MorphTime == 1000)
        {
            if (gameTrackerX.gameData.asmData.MorphType == 1)
            {
                vars->tauntTimer = MON_GetTime(instance) + 1980;
                vars->tauntState++;
                break;
            }
            return 0;
        }
        break;
    case 1:
        if (MON_GetTime(instance) >= (unsigned long)vars->tauntTimer)
        {
            vars->tauntState++;
        }
        break;
    case 2:
        if (mv->targetFade == 4096)
        {
            vars->tauntTimer = MON_GetTime(instance) + 1980;
            vars->tauntState++;
            break;
        }

        fade = ((MON_GetTime(instance) - vars->tauntTimer) * 4096) / 990;

        if (fade >= 4096)
        {
            mv->targetFade = 4096;
        }
        else
        {
            mv->targetFade = fade;
        }

        break;
    case 3:
        if (MON_GetTime(instance) >= (unsigned long)vars->tauntTimer)
        {
            vars->tauntTimer = MON_GetTime(instance);
            vars->tauntState++;
        }
        break;
    case 4:
        if (mv->targetFade == 0)
        {
            vars->tauntTimer = MON_GetTime(instance) + 9900;
            vars->tauntState = 5;
        }
        else
        {
            fade = ((990 - ((int)MON_GetTime(instance) - vars->tauntTimer)) * 4096) / 990;
            if (fade <= 0)
            {
                mv->targetFade = 0;
            }
            else
            {
                mv->targetFade = fade;
            }
        }
        break;
    case 5:
        if (MON_GetTime(instance) >= (unsigned long)vars->tauntTimer || (gameTrackerX.gameData.asmData.MorphTime == 1000 && gameTrackerX.gameData.asmData.MorphType == 0))
        {
            vars->tauntState = 0;
        }
        break;
    default:
        break;
    }

    return 1;
}

void WALBOSB_Idle(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterIR *enemy; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    enemy = mv->enemy;

    if (mv->extraVars != NULL)
    {
        if (!(mv->mvFlags & 4))
        {

            WALBOSB_InterpYaw(instance, 0, mv->subAttr->speedPivotTurn);
            WALBOSB_InterpPitch(instance, 0, mv->subAttr->speedPivotTurn);

            if (WALBOSB_HandleFade(instance) == 0)
            {
                if (mv->auxFlags & 2)
                {
                    if (instance->flags2 & 0x10)
                    {
                        instance->flags2 &= ~0x10;
                        mv->attackState++;

                        if ((signed char)mv->attackState >= 7)
                        {
                            mv->attackState = 0;
                        }

                        MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, (signed char)mv->attackState, 1);
                    }
                }
                else if (enemy != NULL && enemy->distance < mv->subAttr->combatAttributes->combatRange)
                {
                    MON_SwitchState(instance, MONSTER_STATE_COMBAT);
                }
            }

            MON_IdleQueueHandler(instance);
        }
        else
        {

            WALBOSB_SetYaw(instance, 0);
            WALBOSB_SetPitch(instance, 0);
            MON_Idle(instance);
        }
    }
}

void WALBOSB_CombatEntry(Instance *instance)
{
    MON_CombatEntry(instance);
    MON_PlayCombatIdle(instance, 2);
    WALBOSB_ResetSetAutofacePos(instance);
}

void WALBOSB_Combat(Instance *instance)
{

    MonsterIR *enemy; // not from debug symbols
    int turnResult; // not from debug symbols
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    enemy = mv->enemy;

    if (mv->mvFlags & 4 || enemy == NULL)
    {
        MON_SwitchState(instance, MONSTER_STATE_IDLE);
    }
    else
    {
        turnResult = WALBOSB_TurnToPosition(instance, &enemy->instance->position, mv->subAttr->speedPivotTurn) & 1;
        WALBOSB_InterpPitch(instance, 0, mv->subAttr->speedPivotTurn);

        if (enemy->distance < mv->subAttr->combatAttributes->combatRange)
        {
            if (WALBOSB_ShouldIAttack(instance, enemy, WALBOSB_ChooseAttack(instance, enemy)) == 1 && turnResult != 0)
            {
                MON_SwitchState(instance, MONSTER_STATE_ATTACK);
            }
        }
        else
        {
            MON_SwitchState(instance, MONSTER_STATE_IDLE);
        }
    }

    MON_IdleQueueHandler(instance);
}

void WALBOSB_AttackEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    WalbosbAttributes *attrs; // not from debug symbols
    MonsterAttackAttributes *attack; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    attrs = (WalbosbAttributes *)ma->tunData;
    attack = mv->attackType;

    if (ma != NULL)
    {

        mv->mode = 0x200000;

        switch ((signed char)mv->attackType->numAnims)
        {
        case 2:
            mv->generalTimer = MON_GetTime(instance) + (attrs->lengthOfTense * 33);
            break;
        case 4:
            mv->generalTimer = MON_GetTime(instance) + (attrs->lengthOfStrikeTrack * 33);
            SOUND_Play3dSound(&instance->position, 377, -200, 90, 15500);
            break;

        }

        MON_PlayAnimFromList(instance, attack->animList, 0, 2);
    }
}

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_Attack);

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_HitEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_Hit);

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_DoNothingEntry);

void WALBOSB_DoNothing() {};

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_GeneralDeathEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_GeneralDeath);

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_DeadEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_Dead);

#else 

void WALBOSB_SetYaw(Instance *instance, int yaw)
{
    short z; // not from debug symbols

    z = (instance->intro->rotation.z << 16) >> 16; // Garbage shifts to force a signed load
    instance->rotation.z = (z + yaw) & 0xFFF;
}

void WALBOSB_SetPitch(Instance *instance, int pitch)
{

    G2SVector3 extraRot;
    MonsterVars *mv; // not from debug symbols
    WalbosbVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (WalbosbVars *)mv->extraVars;

    if (vars != NULL)
    {
        vars->pitch = pitch;
        extraRot.x = pitch;
        extraRot.y = 0;
        extraRot.z = 0;
        G2Anim_SetController_Vector(&instance->anim, 0, G2ANIM_CTRLRTYPE_ADD_LOCALROT, &extraRot);
    }
}

void WALBOSB_InterpYaw(Instance *instance, int yaw, int speed)
{
    int z; // not from debug symbols

    z = (instance->intro->rotation.z << 16) >> 16; // Garbage shifts to force signed load

    AngleMoveToward(&instance->rotation.z, (z + yaw) & 0xFFF, (gameTrackerX.timeMult * speed * 16) >> 16);
    instance->rotation.z &= 0xFFF;
}


void WALBOSB_InterpPitch(Instance *instance, int pitch, int speed)
{

    G2SVector3 extraRot;
    int newPitch; // not from debug symbols
    WalbosbVars *vars; // not from debug symbols
    WalbosbAttributes *attrs; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols

    newPitch = pitch;
    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (WalbosbVars *)mv->extraVars;
    attrs = (WalbosbAttributes *)ma->tunData;

    if (attrs != NULL && vars != NULL)
    {

        if (newPitch < -attrs->maxPitchAngle)
        {
            newPitch = -attrs->maxPitchAngle & 0xFFF;
        }
        else if (attrs->maxPitchAngle < newPitch)
        {
            newPitch = attrs->maxPitchAngle;
        }

        AngleMoveToward(&vars->pitch, newPitch, (gameTrackerX.timeMult * speed * 16) >> 16);

        extraRot.x = vars->pitch;
        extraRot.y = 0;
        extraRot.z = 0;

        G2Anim_SetController_Vector(&instance->anim, 0, G2ANIM_CTRLRTYPE_ADD_LOCALROT, &extraRot);
    }
}

int WALBOSB_TurnToPosition(Instance *instance, Position *target, int speed) {}

void WALBOSB_ElevateToPosition(Instance *instance, Position *target, int speed, int behind)
{

    int elevation; // not from debug symbols

    elevation = MATH3D_ElevationFromPosToPos(&instance->position, target);

    if (behind != 0)
    {
        elevation -= 1024;
    }
    else
    {
        elevation = 1024 - elevation;
    }

    WALBOSB_InterpPitch(instance, elevation + 128, speed);
}


int WALBOSB_OtherAttackingLegs(Instance *) {}

void WALBOSB_ResetAbortedAttacks(Instance *instance)
{

    Instance *inst; // not from debug symbols

    (void)instance;

    for (inst = gameTrackerX.instanceList->first; inst != NULL; inst = inst->next)
    {
        if (INSTANCE_Query(inst, queryWhatAmI) == 0x410002)
        {

            MonsterVars *mv; // not from debug symbols
            WalbosbVars *vars; // not from debug symbols

            mv = (MonsterVars *)inst->extraData;
            vars = (WalbosbVars *)mv->extraVars;

            if (vars != NULL)
            {
                vars->abortedAttacks = 0;
            }
        }
    }
}

int WALBOSB_AbortedAttacks(Instance *instance)
{

    Instance *inst; // not from debug symbols
    int numAborted; // not from debug symbols

    (void)instance;

    numAborted = 0;

    for (inst = gameTrackerX.instanceList->first; inst != NULL; inst = inst->next)
    {
        if (INSTANCE_Query(inst, queryWhatAmI) == 0x410002)
        {

            MonsterVars *mv; // not from debug symbols
            WalbosbVars *vars; // not from debug symbols

            mv = (MonsterVars *)inst->extraData;
            vars = (WalbosbVars *)mv->extraVars;

            if (vars != NULL)
            {
                numAborted += vars->abortedAttacks;
            }
        }
    }

    return numAborted;
}

int WALBOSB_ShouldIAttack(Instance *instance, MonsterIR *enemy, int attack) {}

int WALBOSB_ChooseAttack(Instance *instance, MonsterIR *enemy) {}

void WALBOSB_WalbossMessage(int message)
{
    Instance *inst; // not from debug symbols

    for (inst = gameTrackerX.instanceList->first; inst != NULL; inst = inst->next)
    {
        if (INSTANCE_Query(inst, queryWhatAmI) == 0x10102)
        {
            INSTANCE_Post(inst, 0x01000017, message);
            break;
        }
    }
}

void WALBOSB_AutofaceMarker(Instance *instance)
{

    Instance *inst; // not from debug symbols
    Object *object; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    WalbosbVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (WalbosbVars *)mv->extraVars;

    if (vars != NULL)
    {
        object = OBTABLE_FindObject("autoface");

        if (object != NULL)
        {
            inst = INSTANCE_BirthObject(instance, object, 0);

            if (inst != NULL)
            {
                inst->flags |= 0x800;
                inst->flags2 &= ~0x20;
                vars->autofaceInst = inst;
            }
        }
    }
}

void WALBOSB_SetAutofacePos(Instance *instance)
{

    HModel *model; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    WalbosbVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (WalbosbVars *)mv->extraVars;
    model = &instance->hModelList[instance->currentModel];

    if (vars != NULL && vars->autofaceInst != NULL)
    {

        int i; // not from debug symbols
        int dist; // not from debug symbols
        int minDist; // not from debug symbols
        HPrim *hPrim; // not from debug symbols

        minDist = 999999;

        for (i = model->numHPrims, hPrim = &model->hPrimList[0]; i != 0; i--, hPrim++)
        {
            if (hPrim->type == 1)
            {

                HSphere *hSphere; // not from debug symbols
                hSphere = hPrim->data.hsphere;

                if (hSphere->id == 8)
                {

                    Position pos;

                    MON_SphereWorldPos(&instance->matrix[hPrim->segment], hSphere, &pos);
                    dist = MATH3D_LengthXYZ(pos.x - gameTrackerX.playerInstance->position.x, pos.y - gameTrackerX.playerInstance->position.y, pos.z - gameTrackerX.playerInstance->position.z);

                    if (dist < minDist)
                    {
                        minDist = dist;
                        COPY_SVEC(Position, &vars->autofaceInst->position, Position, &pos);
                    }
                }
            }
        }

        vars->autofaceInst->flags2 |= 0x20;
    }
}

void WALBOSB_ResetSetAutofacePos(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    WalbosbVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (WalbosbVars *)mv->extraVars;

    if (vars != NULL && vars->autofaceInst != NULL)
    {
        vars->autofaceInst->flags2 &= ~0x20;
        COPY_SVEC(Position, &vars->autofaceInst->position, Position, &instance->position);
    }
}


void WALBOSB_Collide(Instance *instance, GameTracker *gameTracker)
{

    CollideInfo *collideInfo; // not from debug symbols
    HSphere *hSphere; // not from debug symbols

    collideInfo = (CollideInfo *)instance->collideInfo;
    hSphere = (HSphere *)collideInfo->prim0;

    if (collideInfo->type0 != 1 || hSphere->id != 9)
    {
        MonsterCollide(instance, gameTracker);
    }
    else
    {

        Instance *collideInst; // not from debug symbols
        collideInst = (Instance *)collideInfo->inst1;

        if (collideInst == NULL)
        {
            MonsterCollide(instance, gameTracker);
        }
        else
        {

            uintptr_t queryResult; // not from debug symbols
            queryResult = INSTANCE_Query(collideInst, queryWhatAmI);

            if (queryResult != 0x10102 && queryResult != 0x410002)
            {
                MonsterCollide(instance, gameTracker);
            }
        }
    }
}

uintptr_t WALBOSB_Query(Instance *instance, unsigned long query)
{
    if (query != 0)
    {
        return MonsterQuery(instance, query);
    }

    return 0;
}

void WALBOSB_Message(Instance *instance, unsigned long message, unsigned long data)
{

    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    switch (message)
    {
    case 0x400000:
        if (data != 0)
        {
            SOUND_Play3dSound(&instance->position, 21, 650, 80, 15500);
        }
        MonsterMessage(instance, message, data);
        break;
    case 0x100001F:
    case 0x1000021:
        ((evMonsterHitData *)data)->power = 4096;
        MonsterMessage(instance, message, data);
        break;
    case 0x1000020:
        if (!(mv->mvFlags & 4))
        {
            if (instance->currentMainState != MONSTER_STATE_DEAD && instance->currentMainState != MONSTER_STATE_GENERALDEATH)
            {
                MON_SwitchStateDoEntry(instance, MONSTER_STATE_IDLE);
                break;
            }
        }
        break;
    case 0x1000017:
        switch (data)
        {
        case 0:
            if (instance->currentMainState != MONSTER_STATE_GENERALDEATH && instance->currentMainState != MONSTER_STATE_DEAD)
            {
                mv->auxFlags |= 4;
                MON_SwitchStateDoEntry(instance, MONSTER_STATE_DEAD);
            }
            break;
        case 1:
            if (instance->currentMainState != MONSTER_STATE_GENERALDEATH && instance->currentMainState != MONSTER_STATE_DEAD)
            {
                mv->auxFlags |= 1;
                MON_SwitchStateDoEntry(instance, MONSTER_STATE_HIT);
            }
            break;
        case 2:
            if (instance->currentMainState != MONSTER_STATE_GENERALDEATH && instance->currentMainState != MONSTER_STATE_DEAD)
            {
                mv->auxFlags &= ~1;
                MON_SwitchStateDoEntry(instance, MONSTER_STATE_COMBAT);
            }
            break;
        case 3:
            if (!(mv->auxFlags & 2))
            {
                mv->auxFlags |= 2;
                MON_SwitchStateDoEntry(instance, MONSTER_STATE_IDLE);
            }
            else
            {
                mv->auxFlags &= ~2;
            }
            break;
        case 4:
            mv->auxFlags &= ~2;
            break;
        default:
            break;
        }
        break;
    case 0x40009:
    case 0x80001:
    case 0x80002:
    case 0x80003:
    case 0x80004:
    case 0x80005:
    case 0x80006:
        break;
    default:
        MonsterMessage(instance, message, data);
    }
}

void WALBOSB_Init(Instance *instance)
{

    WalbosbVars *vars; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols

    MON_DefaultInit(instance);

    mv = (MonsterVars *)instance->extraData;

    if (mv != NULL)
    {
        vars = (WalbosbVars *)MEMPACK_Malloc(sizeof(WalbosbVars), MEMORY_TYPE_WALBOSBDATA);

        if (vars == NULL)
        {
            mv->extraVars = NULL;
        }
        else
        {
            mv->extraVars = vars;
            vars->pitch = 0;
            vars->tauntState = 0;
            vars->abortedAttacks = 0;
            WALBOSB_AutofaceMarker(instance);
            WALBOSB_SetAutofacePos(instance);
        }
    }

    G2Anim_AttachControllerToSeg(&instance->anim, 0, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_EnableController(&instance->anim, 0, G2ANIM_CTRLRTYPE_ADD_LOCALROT);

    instance->xVel = 0;
    instance->yVel = 0;
    instance->zVel = 0;

    mv->mvFlags |= 0x800;

    MON_SwitchState(instance, MONSTER_STATE_IDLE);

    ma = (MonsterAttributes *)instance->data;
    MON_PlayAnimFromList(instance, ma->auxAnimList, 0, 1);

    mv->auxFlags |= 0x20000000;
    mv->mvFlags |= 0x2000;
    instance->collideFunc = WALBOSB_Collide;
    mv->soulJuice = 0;
    mv->targetFade = 0;

    DEBUG_DoAreaProtection();
}

void WALBOSB_CleanUp(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    WalbosbVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    G2Anim_DetachControllerFromSeg(&instance->anim, 0, G2ANIM_CTRLRTYPE_ADD_LOCALROT);

    if (mv != NULL)
    {
        vars = (WalbosbVars *)mv->extraVars;

        if (vars != NULL)
        {
            if (vars->autofaceInst != NULL)
            {
                INSTANCE_KillInstance(vars->autofaceInst);
            }

            MEMPACK_Free((char *)vars);
        }
    }

    MON_CleanUp(instance);
}


void WALBOSB_IdleEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    WalbosbVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (WalbosbVars *)mv->extraVars;

    if (vars != NULL)
    {
        if (!(mv->mvFlags & 4))
        {
            if (mv->auxFlags & 2)
            {
                mv->attackState = 0;
                MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 0, 1);
            }
            else
            {
                MON_PlayRandomIdle(instance, 2);
            }
        }
        else
        {
            MON_IdleEntry(instance);
        }
    }
}

int WALBOSB_HandleFade(Instance *instance)
{

    int fade; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    WalbosbVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (WalbosbVars *)mv->extraVars;

    switch (vars->tauntState)
    {
    case 0:
        if (gameTrackerX.gameData.asmData.MorphTime == 1000)
        {
            if (gameTrackerX.gameData.asmData.MorphType == 1)
            {
                vars->tauntTimer = MON_GetTime(instance) + 1980;
                vars->tauntState++;
                break;
            }
            return 0;
        }
        break;
    case 1:
        if (MON_GetTime(instance) >= (unsigned long)vars->tauntTimer)
        {
            vars->tauntState++;
        }
        break;
    case 2:
        if (mv->targetFade == 4096)
        {
            vars->tauntTimer = MON_GetTime(instance) + 1980;
            vars->tauntState++;
            break;
        }

        fade = ((MON_GetTime(instance) - vars->tauntTimer) * 4096) / 990;

        if (fade >= 4096)
        {
            mv->targetFade = 4096;
        }
        else
        {
            mv->targetFade = fade;
        }

        break;
    case 3:
        if (MON_GetTime(instance) >= (unsigned long)vars->tauntTimer)
        {
            vars->tauntTimer = MON_GetTime(instance);
            vars->tauntState++;
        }
        break;
    case 4:
        if (mv->targetFade == 0)
        {
            vars->tauntTimer = MON_GetTime(instance) + 9900;
            vars->tauntState = 5;
        }
        else
        {
            fade = ((990 - ((int)MON_GetTime(instance) - vars->tauntTimer)) * 4096) / 990;
            if (fade <= 0)
            {
                mv->targetFade = 0;
            }
            else
            {
                mv->targetFade = fade;
            }
        }
        break;
    case 5:
        if (MON_GetTime(instance) >= (unsigned long)vars->tauntTimer || (gameTrackerX.gameData.asmData.MorphTime == 1000 && gameTrackerX.gameData.asmData.MorphType == 0))
        {
            vars->tauntState = 0;
        }
        break;
    default:
        break;
    }

    return 1;
}

void WALBOSB_Idle(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterIR *enemy; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    enemy = mv->enemy;

    if (mv->extraVars != NULL)
    {
        if (!(mv->mvFlags & 4))
        {

            WALBOSB_InterpYaw(instance, 0, mv->subAttr->speedPivotTurn);
            WALBOSB_InterpPitch(instance, 0, mv->subAttr->speedPivotTurn);

            if (WALBOSB_HandleFade(instance) == 0)
            {
                if (mv->auxFlags & 2)
                {
                    if (instance->flags2 & 0x10)
                    {
                        instance->flags2 &= ~0x10;
                        mv->attackState++;

                        if ((signed char)mv->attackState >= 7)
                        {
                            mv->attackState = 0;
                        }

                        MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, (signed char)mv->attackState, 1);
                    }
                }
                else if (enemy != NULL && enemy->distance < mv->subAttr->combatAttributes->combatRange)
                {
                    MON_SwitchState(instance, MONSTER_STATE_COMBAT);
                }
            }

            MON_IdleQueueHandler(instance);
        }
        else
        {

            WALBOSB_SetYaw(instance, 0);
            WALBOSB_SetPitch(instance, 0);
            MON_Idle(instance);
        }
    }
}

void WALBOSB_CombatEntry(Instance *instance)
{
    MON_CombatEntry(instance);
    MON_PlayCombatIdle(instance, 2);
    WALBOSB_ResetSetAutofacePos(instance);
}

void WALBOSB_Combat(Instance *instance)
{

    MonsterIR *enemy; // not from debug symbols
    int turnResult; // not from debug symbols
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    enemy = mv->enemy;

    if (mv->mvFlags & 4 || enemy == NULL)
    {
        MON_SwitchState(instance, MONSTER_STATE_IDLE);
    }
    else
    {
        turnResult = WALBOSB_TurnToPosition(instance, &enemy->instance->position, mv->subAttr->speedPivotTurn) & 1;
        WALBOSB_InterpPitch(instance, 0, mv->subAttr->speedPivotTurn);

        if (enemy->distance < mv->subAttr->combatAttributes->combatRange)
        {
            if (WALBOSB_ShouldIAttack(instance, enemy, WALBOSB_ChooseAttack(instance, enemy)) == 1 && turnResult != 0)
            {
                MON_SwitchState(instance, MONSTER_STATE_ATTACK);
            }
        }
        else
        {
            MON_SwitchState(instance, MONSTER_STATE_IDLE);
        }
    }

    MON_IdleQueueHandler(instance);
}

void WALBOSB_AttackEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    WalbosbAttributes *attrs; // not from debug symbols
    MonsterAttackAttributes *attack; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    attrs = (WalbosbAttributes *)ma->tunData;
    attack = mv->attackType;

    if (ma != NULL)
    {

        mv->mode = 0x200000;

        switch ((signed char)mv->attackType->numAnims)
        {
        case 2:
            mv->generalTimer = MON_GetTime(instance) + (attrs->lengthOfTense * 33);
            break;
        case 4:
            mv->generalTimer = MON_GetTime(instance) + (attrs->lengthOfStrikeTrack * 33);
            SOUND_Play3dSound(&instance->position, 377, -200, 90, 15500);
            break;

        }

        MON_PlayAnimFromList(instance, attack->animList, 0, 2);
    }
}

void WALBOSB_Attack(void) {};

void WALBOSB_HitEntry(void) {};

void WALBOSB_Hit(void) {};

void WALBOSB_DoNothingEntry(void) {};

void WALBOSB_DoNothing(void) {};

void WALBOSB_GeneralDeathEntry(void) {};

void WALBOSB_GeneralDeath(void) {};

void WALBOSB_DeadEntry(void) {};

void WALBOSB_Dead(void) {};

#endif
