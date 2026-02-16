#include "common.h"

// this conditional is for the objdiff report
#ifndef SKIP_ASM

INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_SetSegmentYaw);

INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_InterpSegmentYaw);

INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_TurnToPosition);

INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_ChooseAttack);

INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_ShouldIAttack);

INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_MessageToLegs);

INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_LivingLegs);

INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_FindEgg);

INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_CreateEgg);

INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_EjectEgg);

INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_BirthEggEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_BirthEgg);

INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_CorrectLookAT);

INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_LegHurtEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_LegHurt);

INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_HitByEggEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_HitByEgg);

INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_HandleThorn);

INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_Collide);

INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_Message);

INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_Query);

INCLUDE_RODATA("asm/nonmatchings/Overlays/walboss/walboss", D_88000000);

INCLUDE_RODATA("asm/nonmatchings/Overlays/walboss/walboss", D_88000020);

INCLUDE_RODATA("asm/nonmatchings/Overlays/walboss/walboss", D_8800002C);

INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_Init);

INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_CleanUp);

INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_DamageEffect);

INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_HandleFade);

INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_IdleEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_Idle);

INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_NoticeEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_Notice);

INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_CombatEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_Combat);

INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_AttackEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_Attack);

INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_HitEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_Hit);

INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_DoNothingEntry);

void WALBOSS_DoNothing() { };

INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_DeadEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_Dead);

INCLUDE_RODATA("asm/nonmatchings/Overlays/walboss/walboss", func_88000060);

#else 

void WALBOSS_SetSegmentYaw(void) { };   

void WALBOSS_InterpSegmentYaw(void) { };

void WALBOSS_TurnToPosition(void) { };  

void WALBOSS_ChooseAttack(void) { };   

void WALBOSS_ShouldIAttack(void) { };  

void WALBOSS_MessageToLegs(void) { };   

void WALBOSS_LivingLegs(void) { };   

void WALBOSS_FindEgg(void) { };   

void WALBOSS_CreateEgg(void) { };  

void WALBOSS_EjectEgg(void) { }; 

void WALBOSS_BirthEggEntry(void) { };  

void WALBOSS_BirthEgg(void) { };    

void WALBOSS_CorrectLookAT(void) { };   

void WALBOSS_LegHurtEntry(void) { };   

void WALBOSS_LegHurt(void) { };     

void WALBOSS_HitByEggEntry(void) { };  

void WALBOSS_HitByEgg(void) { };    

void WALBOSS_HandleThorn(void) { };  

void WALBOSS_Collide(void) { };   

void WALBOSS_Message(void) { };   

void WALBOSS_Query(void) { };   

void WALBOSS_Init(void) { };     

void WALBOSS_CleanUp(void) { };  

void WALBOSS_DamageEffect(void) { };  

void WALBOSS_HandleFade(void) { };    

void WALBOSS_IdleEntry(void) { };     

void WALBOSS_Idle(void) { };           

void WALBOSS_NoticeEntry(void) { };    

void WALBOSS_Notice(void) { };     

void WALBOSS_CombatEntry(void) { };   

void WALBOSS_Combat(void) { };     

void WALBOSS_AttackEntry(void) { }; 

void WALBOSS_Attack(void) { };  

void WALBOSS_HitEntry(void) { };     

void WALBOSS_Hit(void) { };         

void WALBOSS_DoNothingEntry(void) { };  

void WALBOSS_DoNothing(void) { };     

void WALBOSS_DeadEntry(void) { };  

void WALBOSS_Dead(void) { };        

#endif
