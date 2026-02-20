#include "Overlays/wallcr/wallcr.h"

/* double-check that WALLCR_ShouldJumpOff and WALLCR_ShouldJumpOn aren't swapped */

// this conditional is for the objdiff report
#ifndef SKIP_ASM

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_EnableControllers);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_DisableControllers);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_SetFlex);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_CollideFace);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_NearFloor);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_FindJumpOnPoint);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_AngleFromPosToPos);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_FollowWall);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_MoveForward);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_ApplySpin);

INCLUDE_RODATA("asm/nonmatchings/Overlays/wallcr/wallcr", D_88000000);

INCLUDE_RODATA("asm/nonmatchings/Overlays/wallcr/wallcr", D_88000020);

INCLUDE_RODATA("asm/nonmatchings/Overlays/wallcr/wallcr", D_88000038);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_AttachEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_Attach);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_DetachEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_Detach);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_SeekPath);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_ShouldJumpOff);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_ShouldJumpOn);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_ShouldSurpriseAttack);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_MoveToDestination);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_Message);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_Query);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_Init);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_CleanUp);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_PursueEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_Pursue);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_SurpriseAttackEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_SurpriseAttack);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_IdleEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_ShouldIAmbushPlayer);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_Idle);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_WanderEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_Wander);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_FallEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_Fall);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_ImpactEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_Impact);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_GeneralDeathEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_GeneralDeath);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_HitEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_Hit);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_MissileHitEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_MissileHit);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_NoticeEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_Notice);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_SurprisedEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_Surprised);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_StunnedEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_Stunned);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_ThrownEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_Thrown);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_PupateEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_Pupate);

#else

void WALLCR_EnableControllers(void) { };

void WALLCR_DisableControllers(void) { };

void WALLCR_SetFlex(void) { };

void WALLCR_CollideFace(void) { };

void WALLCR_NearFloor(void) { };

void WALLCR_FindJumpOnPoint(void) { };

void WALLCR_AngleFromPosToPos(void) { };

void WALLCR_FollowWall(void) { };

void WALLCR_MoveForward(void) { };

void WALLCR_ApplySpin(void) { };

void WALLCR_AttachEntry(void) { };

void WALLCR_Attach(void) { };

void WALLCR_DetachEntry(void) { };

void WALLCR_Detach(void) { };

void WALLCR_SeekPath(void) { };

void WALLCR_ShouldJumpOff(void) { };

void WALLCR_ShouldJumpOn(void) { };

void WALLCR_ShouldSurpriseAttack(void) { };

void WALLCR_MoveToDestination(void) { };

void WALLCR_Message(void) { };

void WALLCR_Query(void) { };

void WALLCR_Init(void) { };

void WALLCR_CleanUp(void) { };

void WALLCR_PursueEntry(void) { };

void WALLCR_Pursue(void) { };

void WALLCR_SurpriseAttackEntry(void) { };

void WALLCR_SurpriseAttack(void) { };

void WALLCR_IdleEntry(void) { };

void WALLCR_ShouldIAmbushPlayer(void) { };

void WALLCR_Idle(void) { };

void WALLCR_WanderEntry(void) { };

void WALLCR_Wander(void) { };

void WALLCR_FallEntry(void) { };

void WALLCR_Fall(void) { };

void WALLCR_ImpactEntry(void) { };

void WALLCR_Impact(void) { };

void WALLCR_GeneralDeathEntry(void) { };

void WALLCR_GeneralDeath(void) { };

void WALLCR_HitEntry(void) { };

void WALLCR_Hit(void) { };

void WALLCR_MissileHitEntry(void) { };

void WALLCR_MissileHit(void) { };

void WALLCR_NoticeEntry(void) { };

void WALLCR_Notice(void) { };

void WALLCR_SurprisedEntry(void) { };

void WALLCR_Surprised(void) { };

void WALLCR_StunnedEntry(void) { };

void WALLCR_Stunned(void) { };

void WALLCR_ThrownEntry(void) { };

void WALLCR_Thrown(void) { };

void WALLCR_PupateEntry(void) { };

void WALLCR_Pupate(void) { };

#endif
