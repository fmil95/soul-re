#include "Overlays/hunter/hunter.h"
#include "Game/FX.h"
#include "Game/GAMELOOP.h"
#include "Game/MATH3D.h"
#include "Game/MEMPACK.h"
#include "Game/MONSTER/HUMAN.h"
#include "Game/MONSTER/MONLIB.h"
#include "Game/MONSTER/MONMSG.h"
#include "Game/MONSTER/MONSTER.h"
#include "Game/SAVEINFO.h"
#include "Game/STATE.h"

MonsterStateChoice HUNTER_StateChoiceTable[] = {
    {MONSTER_STATE_STUNNED, {HUMAN_StunnedEntry, HUMAN_Stunned}},
    {MONSTER_STATE_DEAD, {HUMAN_DeadEntry, HUMAN_Dead}},
    {MONSTER_STATE_EMBRACE, {HUMAN_EmbraceEntry, HUMAN_Embrace}},
    {MONSTER_STATE_IDLE, {HUMAN_IdleEntry, HUMAN_Idle}},
    {MONSTER_STATE_PROJECTILE, {HUNTER_ProjectileEntry, HUNTER_Projectile}},
    {-1, {NULL, NULL}}
};

const MonsterFunctionTable HUNTER_FunctionTable = {
    HUNTER_Init,
    HUNTER_CleanUp,
    NULL,
    HUMAN_Query,
    NULL,
    HUNTER_StateChoiceTable,
    monVersion,
    NULL
};

// this conditional is for the objdiff report
#ifndef SKIP_ASM

void FX_MakeHitFlame(SVector *startpos, int zpos, int angle, int dist, int size, int q)
{

    SVector position;
    int cos; // not from debug symbols
    int sin; // not from debug symbols
    FX_PRIM *prim; // not from debug symbols

    (void)q;
    prim = FX_GetPrim(gFXT);

    if (prim != NULL)
    {

        cos = rcos(angle - 1024);
        position.x = startpos->x + ((cos * dist) / 4096) + (rand() & 0xF);

        sin = rsin(angle - 1024);
        position.y = startpos->y + ((sin * dist) / 4096) + (rand() & 0xF);

        position.z = startpos->z + zpos + (rand() & 0xF);

        FX_MakeParticleTexFX(prim, &position, 0, 0, 0, 0, 0, 0x93E4, size, 2);
        FX_Sprite_Insert(&gFXT->usedPrimListSprite, prim);

    }
}

void HUNTER_InitFlamethrow(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    HunterVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (HunterVars *)mv->extraVars;

    if (vars != NULL)
    {
        vars->endIdx = 0;
        vars->startIdx = 0;
        vars->timer = 0;
        vars->rem_time = 0;
        mv->auxFlags &= ~4;
    }
}

INCLUDE_ASM("asm/nonmatchings/Overlays/hunter/hunter", HUNTER_Flamethrow);

void HUNTER_Init(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    HunterVars *vars; // not from debug symbols
    Instance *weaponInst; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;

    if (mv->age == 0)
    {
        weaponInst = HUMAN_CreateWeapon(instance, 13, ma->leftWeaponSegment);
    }
    else
    {
        weaponInst = HUMAN_CreateWeapon(instance, 14, ma->leftWeaponSegment);
    }

    if (weaponInst == NULL)
    {
        return;
    }

    HUMAN_Init(instance);
    mv->extraVars = NULL;

    if (mv->age != 0)
    {
        if (mv != NULL)
        {
            vars = (HunterVars *)MEMPACK_Malloc(sizeof(HunterVars), MEMORY_TYPE_HUNTERDATA);

            if (vars != NULL)
            {
                mv->extraVars = vars;
            }
        }
        mv->soulJuice = 24576;
    }
    else
    {
        mv->soulJuice = 20480;
    }
}

void HUNTER_CleanUp(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    HunterVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (mv != NULL)
    {
        vars = mv->extraVars;

        if (vars != NULL)
        {
            MEMPACK_Free((char *)vars);
        }
    }
    HUMAN_CleanUp(instance);
}

void HUNTER_ProjectileEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    MON_ProjectileEntry(instance);

    if (mv->age != 0)
    {
        mv->auxFlags &= ~1;
        mv->auxFlags &= ~2;
    }
}

void HUNTER_Projectile(Instance *instance)
{

    int mode; // not from debug symbols
    int dist; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    HunterVars *vars; // not from debug symbols
    HunterAttributes *attrs; // not from debug symbols
    MonsterMissile *missile; // not from debug symbols
    MonsterSubAttributes *subAttrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    subAttrs = mv->subAttr;
    vars = (HunterVars *)mv->extraVars;
    attrs = (HunterAttributes *)ma->tunData;
    missile = &ma->missileList[(int)subAttrs->combatAttributes->missileAttack];

    if (mv->age == 0)
    {
        MON_Projectile(instance);
        MON_DefaultQueueHandler(instance);
    }
    else if (vars != NULL && attrs != NULL)
    {
        if (!(mv->auxFlags & 1))
        {
            if (mv->enemy != NULL)
            {
                if (instance->flags2 & 0x10)
                {

                    mv->attackState++;

                    if (mv->attackState < missile->numAnims)
                    {
                        if (mv->attackState == missile->anim)
                        {
                            mode = 2;
                        }
                        else
                        {
                            mode = 1;
                        }
                        MON_PlayAnimFromList(instance, missile->animList, mv->attackState, mode);
                    }
                    else
                    {
                        MON_SwitchState(instance, MONSTER_STATE_COMBAT);
                    }
                }
                else if (MON_AnimPlayingFromList(instance, missile->animList, missile->anim) && G2EmulationInstanceQueryPassedFrame(instance, 0, missile->frame) != 0)
                {
                    mv->auxFlags |= 1;
                    HUNTER_InitFlamethrow(instance);
                    vars->timer = MON_GetTime(instance) + (attrs->timeToFire * 33);
                }
            }
            else
            {
                MON_SwitchState(instance, MONSTER_STATE_COMBAT);
            }
        }
        else if (mv->auxFlags & 2)
        {
            if (mv->enemy != NULL)
            {
                MON_TurnToPosition(instance, &mv->enemy->instance->position, subAttrs->speedPivotTurn);
            }

            if (HUNTER_Flamethrow(instance, missile->damage, 0, missile->segment) != 0)
            {

                mv->attackState++;

                if (mv->attackState < missile->numAnims)
                {
                    MON_PlayAnimFromList(instance, missile->animList, mv->attackState, 1);
                }
                else
                {
                    MON_SwitchState(instance, MONSTER_STATE_COMBAT);
                }
            }
        }
        else
        {

            HUNTER_Flamethrow(instance, missile->damage, 1, missile->segment);
            mv->auxFlags |= 2;

            if (mv->enemy != NULL)
            {

                dist = MATH3D_LengthXY(mv->enemy->instance->position.x - instance->position.x, mv->enemy->instance->position.y - instance->position.y);

                if ((unsigned long)vars->timer >= MON_GetTime(instance) || dist < 1200)
                {
                    mv->auxFlags &= ~2;
                }
                MON_TurnToPosition(instance, &mv->enemy->instance->position, mv->subAttr->speedPivotTurn);
            }
        }
        MON_DefaultQueueHandler(instance);
    }
}

#else

void FX_MakeHitFlame(SVector *startpos, int zpos, int angle, int dist, int size, int q)
{

    SVector position;

    int cos; // not from debug symbols
    int sin; // not from debug symbols
    FX_PRIM *prim; // not from debug symbols

    prim = FX_GetPrim(gFXT);

    if (prim != NULL)
    {

        cos = rcos(angle - 1024);
        position.x = startpos->x + ((cos * dist) / 4096) + (rand() & 0xF);

        sin = rsin(angle - 1024);
        position.y = startpos->y + ((sin * dist) / 4096) + (rand() & 0xF);

        position.z = startpos->z + zpos + (rand() & 0xF);

        FX_MakeParticleTexFX(prim, &position, 0, 0, 0, 0, 0, 0x93E4, size, 2);
        FX_Sprite_Insert(&gFXT->usedPrimListSprite, prim);

    }
}

void HUNTER_InitFlamethrow(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    HunterVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (HunterVars *)mv->extraVars;

    if (vars != NULL)
    {
        vars->endIdx = 0;
        vars->startIdx = 0;
        vars->timer = 0;
        vars->rem_time = 0;
        mv->auxFlags &= ~4;
    }
}

int HUNTER_Flamethrow(Instance *instance, int damage, int newPoint, int segment) {};

void HUNTER_Init(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    HunterVars *vars; // not from debug symbols
    Instance *weaponInst; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;

    if (mv->age == 0)
    {
        weaponInst = HUMAN_CreateWeapon(instance, 13, ma->leftWeaponSegment);
    }
    else
    {
        weaponInst = HUMAN_CreateWeapon(instance, 14, ma->leftWeaponSegment);
    }

    if (weaponInst == NULL)
    {
        return;
    }

    HUMAN_Init(instance);
    mv->extraVars = NULL;

    if (mv->age != 0)
    {
        if (mv != NULL)
        {
            vars = (HunterVars *)MEMPACK_Malloc(sizeof(HunterVars), MEMORY_TYPE_HUNTERDATA);

            if (vars != NULL)
            {
                mv->extraVars = vars;
            }
        }
        mv->soulJuice = 24576;
    }
    else
    {
        mv->soulJuice = 20480;
    }
}

void HUNTER_CleanUp(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    HunterVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (mv != NULL)
    {
        vars = mv->extraVars;

        if (vars != NULL)
        {
            MEMPACK_Free((char *)vars);
        }
    }
    HUMAN_CleanUp(instance);
}

void HUNTER_ProjectileEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    MON_ProjectileEntry(instance);

    if (mv->age != 0)
    {
        mv->auxFlags &= ~1;
        mv->auxFlags &= ~2;
    }
}

void HUNTER_Projectile(Instance *instance)
{

    int mode; // not from debug symbols
    int dist; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    HunterVars *vars; // not from debug symbols
    HunterAttributes *attrs; // not from debug symbols
    MonsterMissile *missile; // not from debug symbols
    MonsterSubAttributes *subAttrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    subAttrs = mv->subAttr;
    vars = (HunterVars *)mv->extraVars;
    attrs = (HunterAttributes *)ma->tunData;
    missile = &ma->missileList[(int)subAttrs->combatAttributes->missileAttack];

    if (mv->age == 0)
    {
        MON_Projectile(instance);
        MON_DefaultQueueHandler(instance);
    }
    else if (vars != NULL && attrs != NULL)
    {
        if (!(mv->auxFlags & 1))
        {
            if (mv->enemy != NULL)
            {
                if (instance->flags2 & 0x10)
                {

                    mv->attackState++;

                    if (mv->attackState < missile->numAnims)
                    {
                        if (mv->attackState == missile->anim)
                        {
                            mode = 2;
                        }
                        else
                        {
                            mode = 1;
                        }
                        MON_PlayAnimFromList(instance, missile->animList, mv->attackState, mode);
                    }
                    else
                    {
                        MON_SwitchState(instance, MONSTER_STATE_COMBAT);
                    }
                }
                else if (MON_AnimPlayingFromList(instance, missile->animList, missile->anim) && G2EmulationInstanceQueryPassedFrame(instance, 0, missile->frame) != 0)
                {
                    mv->auxFlags |= 1;
                    HUNTER_InitFlamethrow(instance);
                    vars->timer = MON_GetTime(instance) + (attrs->timeToFire * 33);
                }
            }
            else
            {
                MON_SwitchState(instance, MONSTER_STATE_COMBAT);
            }
        }
        else if (mv->auxFlags & 2)
        {
            if (mv->enemy != NULL)
            {
                MON_TurnToPosition(instance, &mv->enemy->instance->position, subAttrs->speedPivotTurn);
            }

            if (HUNTER_Flamethrow(instance, missile->damage, 0, missile->segment) != 0)
            {

                mv->attackState++;

                if (mv->attackState < missile->numAnims)
                {
                    MON_PlayAnimFromList(instance, missile->animList, mv->attackState, 1);
                }
                else
                {
                    MON_SwitchState(instance, MONSTER_STATE_COMBAT);
                }
            }
        }
        else
        {

            HUNTER_Flamethrow(instance, missile->damage, 1, missile->segment);
            mv->auxFlags |= 2;

            if (mv->enemy != NULL)
            {

                dist = MATH3D_LengthXY(mv->enemy->instance->position.x - instance->position.x, mv->enemy->instance->position.y - instance->position.y);

                if ((unsigned long)vars->timer >= MON_GetTime(instance) || dist < 1200)
                {
                    mv->auxFlags &= ~2;
                }
                MON_TurnToPosition(instance, &mv->enemy->instance->position, mv->subAttr->speedPivotTurn);
            }
        }
        MON_DefaultQueueHandler(instance);
    }
}

#endif
