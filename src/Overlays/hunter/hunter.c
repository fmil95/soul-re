#include "Overlays/hunter/hunter.h"
#include "Game/MEMPACK.h"
#include "Game/MONSTER/HUMAN.h"
#include "Game/MONSTER/MONSTER.h"

// this conditional is for the objdiff report
#ifndef SKIP_ASM

INCLUDE_ASM("asm/nonmatchings/Overlays/hunter/hunter", FX_MakeHitFlame);

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

void FX_MakeHitFlame(void) {};

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
