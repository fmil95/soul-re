#include "common.h"
#include "Game/G2/ANMCTRLR.h"

// this conditional is for the objdiff report
#ifndef SKIP_ASM

G2Bool ALUKA_ControllersEnabled(Instance *instance)
{
    return G2Anim_IsControllerActive(&instance->anim, 1, 0x26) != 0;
}


INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_SetPitch);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_EnableControllers);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_DisableControllers);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_SetSwimBodyTwist);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_NotDaylight);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_CapDepth);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_AngleTooWide);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_VectorFromPitchYaw);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_FacingVector);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_SimpleLineCheck);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_TerrainInPath);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_ApplyIncr);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_ApplyForwardAccel);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_ApplyAngularAccel);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_ApplyRots);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_MoveForward);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_FixPitch);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_ProportionalLimitsAndAccels);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_FacePoint);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_SetupSwimAnimWOTread);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_SetupSwimAnimWTread);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_SwimToDestination);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_GetRandomDestination);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_NearAluka);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_GetCircleDestination);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_SwimPlanMovement);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_ResetSwim);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_ShouldJumpIn);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_ShouldJumpOut);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_JumpToEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_SetJumpVels);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_SetHitGround);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_JumpTo);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_ChooseAttack);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_Init);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_CleanUp);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_Message);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_PursueEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_Pursue);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_AttackEntry);

INCLUDE_RODATA("asm/nonmatchings/Overlays/aluka/aluka", D_88000000);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_Attack);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_FleeEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_Flee);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_WanderEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_Wander);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_IdleEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_Idle);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_LandInWaterEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_LandInWater);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_HitEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_Hit);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_StunnedEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_Stunned);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_ProjectileEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_Projectile);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_SurpriseAttackEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_SurpriseAttack);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_SurprisedEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_Surprised);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_NoticeEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_Notice);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_EmbraceEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_Embrace);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_GeneralDeathEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_GeneralDeath);

#else

G2Bool ALUKA_ControllersEnabled(Instance *instance)
{
    return G2Anim_IsControllerActive(&instance->anim, 1, 0x26) != 0;
}

void ALUKA_SetPitch(void) {};

void ALUKA_EnableControllers(void) {};

void ALUKA_DisableControllers(void) {};

void ALUKA_SetSwimBodyTwist(void) {};

void ALUKA_NotDaylight(void) {};

void ALUKA_CapDepth(void) {};

void ALUKA_AngleTooWide(void) {};

void ALUKA_VectorFromPitchYaw(void) {};

void ALUKA_FacingVector(void) {};

void ALUKA_SimpleLineCheck(void) {};

void ALUKA_TerrainInPath(void) {};

void ALUKA_ApplyIncr(void) {};

void ALUKA_ApplyForwardAccel(void) {};

void ALUKA_ApplyAngularAccel(void) {};

void ALUKA_ApplyRots(void) {};

void ALUKA_MoveForward(void) {};

void ALUKA_FixPitch(void) {};

void ALUKA_ProportionalLimitsAndAccels(void) {};

void ALUKA_FacePoint(void) {};

void ALUKA_SetupSwimAnimWOTread(void) {};

void ALUKA_SetupSwimAnimWTread(void) {};

void ALUKA_SwimToDestination(void) {};

void ALUKA_GetRandomDestination(void) {};

void ALUKA_NearAluka(void) {};

void ALUKA_GetCircleDestination(void) {};

void ALUKA_SwimPlanMovement(void) {};

void ALUKA_ResetSwim(void) {};

void ALUKA_ShouldJumpIn(void) {};

void ALUKA_ShouldJumpOut(void) {};

void ALUKA_JumpToEntry(void) {};

void ALUKA_SetJumpVels(void) {};

void ALUKA_SetHitGround(void) {};

void ALUKA_JumpTo(void) {};

void ALUKA_ChooseAttack(void) {};

void ALUKA_Init(void) {};

void ALUKA_CleanUp(void) {};

void ALUKA_Message(void) {};

void ALUKA_PursueEntry(void) {};

void ALUKA_Pursue(void) {};

void ALUKA_AttackEntry(void) {};

void ALUKA_Attack(void) {};

void ALUKA_FleeEntry(void) {};

void ALUKA_Flee(void) {};

void ALUKA_WanderEntry(void) {};

void ALUKA_Wander(void) {};

void ALUKA_IdleEntry(void) {};

void ALUKA_Idle(void) {};

void ALUKA_LandInWaterEntry(void) {};

void ALUKA_LandInWater(void) {};

void ALUKA_HitEntry(void) {};

void ALUKA_Hit(void) {};

void ALUKA_StunnedEntry(void) {};

void ALUKA_Stunned(void) {};

void ALUKA_ProjectileEntry(void) {};

void ALUKA_Projectile(void) {};

void ALUKA_SurpriseAttackEntry(void) {};

void ALUKA_SurpriseAttack(void) {};

void ALUKA_SurprisedEntry(void) {};

void ALUKA_Surprised(void) {};

void ALUKA_NoticeEntry(void) {};

void ALUKA_Notice(void) {};

void ALUKA_EmbraceEntry(void) {};

void ALUKA_Embrace(void) {};

void ALUKA_GeneralDeathEntry(void) {};

void ALUKA_GeneralDeath(void) {};

#endif
