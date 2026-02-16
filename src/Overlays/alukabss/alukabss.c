#include "common.h"

// this conditional is for the objdiff report
#ifndef SKIP_ASM

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_SetTwist);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_RotateToFace);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_RazTimeAtMarker);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_TimeSinceSpit);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_ShouldAttack);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_InitCircle);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_Circle);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_SetUpWaterPlaneClip);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_Query);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_Message);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_Init);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_CleanUp);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_DamageEffect);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_IdleEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_Idle);

void ALUKABSS_PursueEntry() { };

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_Pursue);

void ALUKABSS_CombatEntry() { };

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_Combat);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_AttackEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_Attack);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_LandInWaterEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_LandInWater);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_ProjectileEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_Projectile);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_DeadEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_Dead);

INCLUDE_ASM("asm/nonmatchings/Overlays/alukabss/alukabss", ALUKABSS_DoNothingEntry);

void ALUKABSS_DoNothing() { };

INCLUDE_RODATA("asm/nonmatchings/Overlays/alukabss/alukabss", D_88000000);

INCLUDE_RODATA("asm/nonmatchings/Overlays/alukabss/alukabss", D_88000020);

#else 

void ALUKABSS_SetTwist(void) { };     

void ALUKABSS_RotateToFace(void) { }; 

void ALUKABSS_RazTimeAtMarker(void) { };  

void ALUKABSS_TimeSinceSpit(void) { };   

void ALUKABSS_ShouldAttack(void) { };    

void ALUKABSS_InitCircle(void) { };     

void ALUKABSS_Circle(void) { };         

void ALUKABSS_SetUpWaterPlaneClip(void) { };

void ALUKABSS_Query(void) { };       

void ALUKABSS_Message(void) { };    

void ALUKABSS_Init(void) { };       

void ALUKABSS_CleanUp(void) { };           

void ALUKABSS_DamageEffect(void) { };    

void ALUKABSS_IdleEntry(void) { };       

void ALUKABSS_Idle(void) { };            

void ALUKABSS_PursueEntry(void) { };      

void ALUKABSS_Pursue(void) { };           

void ALUKABSS_CombatEntry(void) { };     

void ALUKABSS_Combat(void) { };      

void ALUKABSS_AttackEntry(void) { };    
    
void ALUKABSS_Attack(void) { };             

void ALUKABSS_LandInWaterEntry(void) { };   

void ALUKABSS_LandInWater(void) { };        

void ALUKABSS_ProjectileEntry(void) { };    

void ALUKABSS_Projectile(void) { };         

void ALUKABSS_DeadEntry(void) { };       

void ALUKABSS_Dead(void) { };          

void ALUKABSS_DoNothingEntry(void) { };   
  
void ALUKABSS_DoNothing(void) { };          

#endif
