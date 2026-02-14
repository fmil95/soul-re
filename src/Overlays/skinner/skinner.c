#include "common.h"

// this conditional is for the objdiff report
#ifndef SKIP_ASM

INCLUDE_ASM("asm/nonmatchings/Overlays/skinner/skinner", MON_GetRandomGroundPoint);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinner/skinner", MONSENSE_DetectPlayer);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinner/skinner", FX_StartInstanceBurrow);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinner/skinner", FX_StopInstanceBurrow);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinner/skinner", SKINNER_BurrowInEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinner/skinner", SKINNER_BurrowIn);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinner/skinner", SKINNER_GetBurrowDest);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinner/skinner", SKINNER_CalcBurrowingMove);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinner/skinner", SKINNER_PupateEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinner/skinner", SKINNER_Pupate);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinner/skinner", SKINNER_PursueEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinner/skinner", SKINNER_Pursue);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinner/skinner", SKINNER_HideEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinner/skinner", SKINNER_Hide);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinner/skinner", SKINNER_SurpriseAttackEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinner/skinner", SKINNER_SurpriseAttack);

INCLUDE_RODATA("asm/nonmatchings/Overlays/skinner/skinner", D_88000000);

#else

void MON_GetRandomGroundPoint(void) { };

void MONSENSE_DetectPlayer(void) { };

void FX_StartInstanceBurrow(void) { };

void FX_StopInstanceBurrow(void) { };

void SKINNER_BurrowInEntry(void) { };

void SKINNER_BurrowIn(void) { };

void SKINNER_GetBurrowDest(void) { };

void SKINNER_CalcBurrowingMove(void) { };

void SKINNER_PupateEntry(void) { };

void SKINNER_Pupate(void) { };

void SKINNER_PursueEntry(void) { };

void SKINNER_Pursue(void) { };

void SKINNER_HideEntry(void) { };

void SKINNER_Hide(void) { };

void SKINNER_SurpriseAttackEntry(void) { };

void SKINNER_SurpriseAttack(void) { };

#endif
