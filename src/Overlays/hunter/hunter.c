#include "Overlays/hunter/hunter.h"

// this conditional is for the objdiff report
#ifndef SKIP_ASM

INCLUDE_ASM("asm/nonmatchings/Overlays/hunter/hunter", FX_MakeHitFlame);

INCLUDE_ASM("asm/nonmatchings/Overlays/hunter/hunter", HUNTER_InitFlamethrow);

INCLUDE_ASM("asm/nonmatchings/Overlays/hunter/hunter", HUNTER_Flamethrow);

INCLUDE_ASM("asm/nonmatchings/Overlays/hunter/hunter", HUNTER_Init);

INCLUDE_ASM("asm/nonmatchings/Overlays/hunter/hunter", HUNTER_CleanUp);

INCLUDE_ASM("asm/nonmatchings/Overlays/hunter/hunter", HUNTER_ProjectileEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/hunter/hunter", HUNTER_Projectile);

INCLUDE_RODATA("asm/nonmatchings/Overlays/hunter/hunter", D_88000000);

#else

void FX_MakeHitFlame(void) { };

void HUNTER_InitFlamethrow(void) { };

void HUNTER_Flamethrow(void) { };

void HUNTER_Init(void) { };

void HUNTER_CleanUp(void) { };

void HUNTER_ProjectileEntry(void) { };

void HUNTER_Projectile(void) { };

#endif
