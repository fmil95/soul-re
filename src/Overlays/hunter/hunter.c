#include "Overlays/hunter/hunter.h"
#include "Game/MEMPACK.h"
#include "Game/MONSTER/HUMAN.h"

// this conditional is for the objdiff report
#ifndef SKIP_ASM

INCLUDE_ASM("asm/nonmatchings/Overlays/hunter/hunter", FX_MakeHitFlame);

INCLUDE_ASM("asm/nonmatchings/Overlays/hunter/hunter", HUNTER_InitFlamethrow);

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

INCLUDE_ASM("asm/nonmatchings/Overlays/hunter/hunter", HUNTER_CleanUp);

INCLUDE_ASM("asm/nonmatchings/Overlays/hunter/hunter", HUNTER_ProjectileEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/hunter/hunter", HUNTER_Projectile);

INCLUDE_RODATA("asm/nonmatchings/Overlays/hunter/hunter", D_88000000);

#else

void FX_MakeHitFlame(void) {};

void HUNTER_InitFlamethrow(void) {};

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

void HUNTER_CleanUp(void) {};

void HUNTER_ProjectileEntry(void) {};

void HUNTER_Projectile(void) {};

#endif
