#include "Overlays/kain/kain.h"

// this conditional is for the objdiff report
#ifndef SKIP_ASM

INCLUDE_RODATA("asm/nonmatchings/Overlays/kain/kain", D_88000000);

INCLUDE_RODATA("asm/nonmatchings/Overlays/kain/kain", D_88000020);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_PickUpReaver);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_FindFarthestMarkerPosition);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_TeleportEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_Teleport);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_EndEffects);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_LightningEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_Lightning);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_FFieldOffset);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_ChargeEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_ChargeUp);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_ChargeDown);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", CheckHit);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", SwitchToHit);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_DamageEffect);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_Message);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_Query);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_Init);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_CleanUp);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_IdleEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_Idle);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_CombatEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_Combat);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_AttackEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_Attack);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_PursueEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_Pursue);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_HitEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_Hit);

INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_DoNothingEntry);

void KAIN_DoNothing() { };

#else

void KAIN_PickUpReaver(void) { };

void KAIN_FindFarthestMarkerPosition(void) { };

void KAIN_TeleportEntry(void) { };           

void KAIN_Teleport(void) { }; 

void KAIN_EndEffects(void) { };  

void KAIN_LightningEntry(void) { };    

void KAIN_Lightning(void) { };      

void KAIN_FFieldOffset(void) { };    

void KAIN_ChargeEntry(void) { };    

void KAIN_ChargeUp(void) { };      

void KAIN_ChargeDown(void) { };  

void CheckHit(void) { };   

void SwitchToHit(void) { };   

void KAIN_DamageEffect(void) { };     

void KAIN_Message(void) { };    

void KAIN_Query(void) { };   

void KAIN_Init(void) { };  

void KAIN_CleanUp(void) { };  

void KAIN_IdleEntry(void) { };   

void KAIN_Idle(void) { };       

void KAIN_CombatEntry(void) { };   

void KAIN_Combat(void) { };  

void KAIN_AttackEntry(void) { };    
          
void KAIN_Attack(void) { };     

void KAIN_PursueEntry(void) { };   

void KAIN_Pursue(void) { };   

void KAIN_HitEntry(void) { };  

void KAIN_Hit(void) { };     

void KAIN_DoNothingEntry(void) { };  

void KAIN_DoNothing(void) { };       

#endif
