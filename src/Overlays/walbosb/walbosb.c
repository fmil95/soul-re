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
#include "Game/MONSTER/MONSTER.h"

// this conditional is for the objdiff report
#ifndef SKIP_ASM

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

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_HandleFade);

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_Idle);

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_CombatEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_Combat);

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_AttackEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_Attack);

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_HitEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_Hit);

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_DoNothingEntry);

void WALBOSB_DoNothing() {};

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_GeneralDeathEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_GeneralDeath);

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_DeadEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_Dead);

INCLUDE_RODATA("asm/nonmatchings/Overlays/walbosb/walbosb", func_88000068);

INCLUDE_RODATA("asm/nonmatchings/Overlays/walbosb/walbosb", func_88000088);

INCLUDE_RODATA("asm/nonmatchings/Overlays/walbosb/walbosb", func_880000D8);

INCLUDE_RODATA("asm/nonmatchings/Overlays/walbosb/walbosb", func_88000140);

INCLUDE_RODATA("asm/nonmatchings/Overlays/walbosb/walbosb", func_88000204);

INCLUDE_RODATA("asm/nonmatchings/Overlays/walbosb/walbosb", func_88000324);

INCLUDE_RODATA("asm/nonmatchings/Overlays/walbosb/walbosb", func_88000390);

INCLUDE_RODATA("asm/nonmatchings/Overlays/walbosb/walbosb", func_88000488);

INCLUDE_RODATA("asm/nonmatchings/Overlays/walbosb/walbosb", func_88000508);

INCLUDE_RODATA("asm/nonmatchings/Overlays/walbosb/walbosb", func_880005A0);

INCLUDE_RODATA("asm/nonmatchings/Overlays/walbosb/walbosb", func_880006DC);

#else 

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

void WALBOSB_HandleFade(void) {};

void WALBOSB_Idle(void) {};

void WALBOSB_CombatEntry(void) {};

void WALBOSB_Combat(void) {};

void WALBOSB_AttackEntry(void) {};

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
