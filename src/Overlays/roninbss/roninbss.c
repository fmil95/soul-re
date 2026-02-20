#include "Overlays/roninbss/roninbss.h"

// this conditional is for the objdiff report
#ifndef SKIP_ASM

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_StartBand);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_StopBand);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_InitConstrict);

INCLUDE_RODATA("asm/nonmatchings/Overlays/roninbss/roninbss", D_88000000);

INCLUDE_RODATA("asm/nonmatchings/Overlays/roninbss/roninbss", D_88000020);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_Constrict);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_StopSoulSuck);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_FadeMove);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_ChooseAttack);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_Collide);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_Message);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_Query);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_Init);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_CleanUp);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_DamageEffect);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_FindClosestMarkerInUnit);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_GetNextMarkerInSeries);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_FindValve);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_IdleEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_Idle);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_WanderEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_Wander);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_AttackEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_Attack);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_CombatEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_Combat);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_HitEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_Hit);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_FallEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_Fall);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_PursueEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_Pursue);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_DoNothingEntry);

void RONINBSS_DoNothing() { };

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_DeadEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/roninbss/roninbss", RONINBSS_Dead);

#else

void RONINBSS_StartBand(void) { };         

void RONINBSS_StopBand(void) { };        

void RONINBSS_InitConstrict(void) { };      

void RONINBSS_Constrict(void) { };          

void RONINBSS_StopSoulSuck(void) { };     

void RONINBSS_FadeMove(void) { };      

void RONINBSS_ChooseAttack(void) { };   

void RONINBSS_Collide(void) { };    

void RONINBSS_Message(void) { };    

void RONINBSS_Query(void) { };     

void RONINBSS_Init(void) { };     

void RONINBSS_CleanUp(void) { };   

void RONINBSS_DamageEffect(void) { };   

void RONINBSS_FindClosestMarkerInUnit(void) { };

void RONINBSS_GetNextMarkerInSeries(void) { };  

void RONINBSS_FindValve(void) { };   

void RONINBSS_IdleEntry(void) { };  

void RONINBSS_Idle(void) { };    

void RONINBSS_WanderEntry(void) { };  

void RONINBSS_Wander(void) { };      

void RONINBSS_AttackEntry(void) { };   

void RONINBSS_Attack(void) { };       

void RONINBSS_CombatEntry(void) { };  

void RONINBSS_Combat(void) { };    

void RONINBSS_HitEntry(void) { };   

void RONINBSS_Hit(void) { };       

void RONINBSS_FallEntry(void) { };     

void RONINBSS_Fall(void) { };         

void RONINBSS_PursueEntry(void) { };     

void RONINBSS_Pursue(void) { };       

void RONINBSS_DoNothingEntry(void) { };   

void RONINBSS_DoNothing(void) { };      

void RONINBSS_DeadEntry(void) { };     
         
void RONINBSS_Dead(void) { };                   

#endif
