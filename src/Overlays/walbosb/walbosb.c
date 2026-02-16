#include "common.h"

// this conditional is for the objdiff report
#ifndef SKIP_ASM

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_WalbossMessage);

INCLUDE_RODATA("asm/nonmatchings/Overlays/walbosb/walbosb", D_88000000);

INCLUDE_RODATA("asm/nonmatchings/Overlays/walbosb/walbosb", D_88000020);

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_AutofaceMarker);

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_SetAutofacePos);

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_ResetSetAutofacePos);

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_Collide);

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_Query);

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_Message);

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_Init);

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_CleanUp);

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_IdleEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_HandleFade);

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_Idle);

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_CombatEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_Combat);

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_AttackEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_Attack);

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_HitEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_Hit);

INCLUDE_ASM("asm/nonmatchings/Overlays/walbosb/walbosb", WALBOSB_DoNothingEntry);

void WALBOSB_DoNothing() { };

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

void WALBOSB_WalbossMessage(void) { };     

void WALBOSB_AutofaceMarker(void) { };    

void WALBOSB_SetAutofacePos(void) { };    

void WALBOSB_ResetSetAutofacePos(void) { };

void WALBOSB_Collide(void) { };    

void WALBOSB_Query(void) { };   

void WALBOSB_Message(void) { };   

void WALBOSB_Init(void) { };      

void WALBOSB_CleanUp(void) { };     

void WALBOSB_IdleEntry(void) { };    

void WALBOSB_HandleFade(void) { };   

void WALBOSB_Idle(void) { };         

void WALBOSB_CombatEntry(void) { };    

void WALBOSB_Combat(void) { };        

void WALBOSB_AttackEntry(void) { };      

void WALBOSB_Attack(void) { };       

void WALBOSB_HitEntry(void) { };         

void WALBOSB_Hit(void) { };             

void WALBOSB_DoNothingEntry(void) { };     

void WALBOSB_DoNothing(void) { };          

void WALBOSB_GeneralDeathEntry(void) { };  

void WALBOSB_GeneralDeath(void) { };    

void WALBOSB_DeadEntry(void) { };     

void WALBOSB_Dead(void) { };               

#endif
