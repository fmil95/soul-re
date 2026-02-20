#include "Overlays/skinbos/skinbos.h"

/* double-check that SKINBOS_CheckInsideMasher and SKINBOS_ShouldEscapeJail aren't swapped */

// this conditional is for the objdiff report
#ifndef SKIP_ASM

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_Turn);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_GateDrop);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_CheckPointInsideMasher);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_CheckInsideMasher);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_DoPhaseFade);

INCLUDE_RODATA("asm/nonmatchings/Overlays/skinbos/skinbos", D_88000000);

INCLUDE_RODATA("asm/nonmatchings/Overlays/skinbos/skinbos", D_88000020);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_HandleOneShotAnims);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_DoPhasingOutInit);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_CheckPhaseIn);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_ShouldEscapeJail);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_ProcessGateHitBlood);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_StartVertexBlood);

INCLUDE_RODATA("asm/nonmatchings/Overlays/skinbos/skinbos", D_88000060);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_Collide);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_DamageEffect);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_Message);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_Query);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_Init);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_CleanUp);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_IdleEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_Idle);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_FindRandomNodeInUnit);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_WanderEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_Wander);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_PursueEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_Pursue);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_HitEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_Hit);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_CombatEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_Combat);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_LandOnFeetEntry);

void SKINBOS_LandOnFeet(void) { };

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_AttackEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_Attack);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_DeadEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_Dead);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_FleeEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_Flee);

INCLUDE_ASM("asm/nonmatchings/Overlays/skinbos/skinbos", SKINBOS_StunnedEntry);

#else

void SKINBOS_Turn(void) { };  

void SKINBOS_GateDrop(void) { };      

void SKINBOS_CheckPointInsideMasher(void) { };  

void SKINBOS_CheckInsideMasher(void) { };       

void SKINBOS_DoPhaseFade(void) { };     

void SKINBOS_HandleOneShotAnims(void) { };    

void SKINBOS_DoPhasingOutInit(void) { };    

void SKINBOS_CheckPhaseIn(void) { };     

void SKINBOS_ShouldEscapeJail(void) { };     

void SKINBOS_ProcessGateHitBlood(void) { };     

void SKINBOS_StartVertexBlood(void) { };    

void SKINBOS_Collide(void) { };       

void SKINBOS_DamageEffect(void) { };      

void SKINBOS_Message(void) { };       

void SKINBOS_Query(void) { };            

void SKINBOS_Init(void) { };           

void SKINBOS_CleanUp(void) { };        

void SKINBOS_IdleEntry(void) { };      

void SKINBOS_Idle(void) { };          

void SKINBOS_FindRandomNodeInUnit(void) { };    

void SKINBOS_WanderEntry(void) { };          

void SKINBOS_Wander(void) { };           

void SKINBOS_PursueEntry(void) { };      

void SKINBOS_Pursue(void) { };     

void SKINBOS_HitEntry(void) { };    

void SKINBOS_Hit(void) { };        

void SKINBOS_CombatEntry(void) { };     

void SKINBOS_Combat(void) { };          

void SKINBOS_LandOnFeetEntry(void) { };   

void SKINBOS_LandOnFeet(void) { };     

void SKINBOS_AttackEntry(void) { };    

void SKINBOS_Attack(void) { };       

void SKINBOS_DeadEntry(void) { };     

void SKINBOS_Dead(void) { };          

void SKINBOS_FleeEntry(void) { };     

void SKINBOS_Flee(void) { };        

void SKINBOS_StunnedEntry(void) { };      

#endif
