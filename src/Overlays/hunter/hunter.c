#include "Overlays/hunter/hunter.h"
#include "Game/FX.h"
#include "Game/GAMELOOP.h"
#include "Game/MATH3D.h"
#include "Game/MEMPACK.h"
#include "Game/MONSTER/HUMAN.h"
#include "Game/MONSTER/MONLIB.h"
#include "Game/MONSTER/MONMSG.h"
#include "Game/MONSTER/MONSTER.h"
#include "Game/STATE.h"

// this conditional is for the objdiff report
#ifndef SKIP_ASM

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
            vars = (HunterVars *)MEMPACK_Malloc(sizeof(HunterVars), 36);

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

INCLUDE_ASM("asm/nonmatchings/Overlays/hunter/hunter", HUNTER_Projectile);

INCLUDE_RODATA("asm/nonmatchings/Overlays/hunter/hunter", D_88000000);

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

void HUNTER_Flamethrow(void) {};

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
            vars = (HunterVars *)MEMPACK_Malloc(sizeof(HunterVars), 36);

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

void HUNTER_Projectile(void) {};

#endif
