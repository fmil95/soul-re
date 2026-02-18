#include "common.h"
#include "Game/MATH3D.h"
#include "Game/G2/ANMCTRLR.h"
#include "Game/MONSTER/MONLIB.h"
#include "Game/MONSTER/MONSTER.h"

// this conditional is for the objdiff report
#ifndef SKIP_ASM

G2Bool ALUKA_ControllersEnabled(Instance *instance)
{
    return G2Anim_IsControllerActive(&instance->anim, 1, 0x26) != 0;
}


void ALUKA_SetPitch(Instance *instance, int pitch)
{
    G2SVector3 extraRot;
    AlukaAttributes *attributes; // not from debug symbols

    attributes = (AlukaAttributes *)((MonsterVars *)instance->extraData)->extraVars;

    if (!ALUKA_ControllersEnabled(instance))
    {
        return;
    }

    extraRot.x = pitch;
    extraRot.z = 0;
    extraRot.y = 0;
    G2Anim_SetController_Vector(&instance->anim, 1, 0xE, &extraRot);
    attributes->swimfast_pitch = pitch;
}

void ALUKA_EnableControllers(Instance *instance)
{
    G2SVector3 translate;
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (ALUKA_ControllersEnabled(instance))
    {
        return;
    }

    G2Anim_EnableController(&instance->anim, 1, 0x26);
    G2Anim_EnableController(&instance->anim, 1, 0xE);
    G2Anim_EnableController(&instance->anim, 0x13, 0xE);
    G2Anim_EnableController(&instance->anim, 0x1A, 0xE);
    G2Anim_EnableController(&instance->anim, 0x20, 0xE);
    G2Anim_EnableController(&instance->anim, 5, 0xE);
    G2Anim_EnableController(&instance->anim, 0xC, 0xE);
    G2Anim_EnableController(&instance->anim, 4, 0xE);
    G2Anim_EnableController(&instance->anim, 0xB, 0xE);

    translate.y = 0;
    translate.x = 0;

    if (mv->age != 0)
    {
        translate.z = -0x1C2;
        instance->position.z -= translate.z;
    }
    else
    {
        translate.z = -0x190;
        instance->position.z -= translate.z;
    }

    G2Anim_SetController_Vector(&instance->anim, 1, 0x26, &translate);
}

void ALUKA_DisableControllers(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (!ALUKA_ControllersEnabled(instance))
    {
        return;
    }

    G2Anim_DisableController(&instance->anim, 1, 0x26);
    G2Anim_DisableController(&instance->anim, 1, 0xE);
    G2Anim_DisableController(&instance->anim, 0x13, 0xE);
    G2Anim_DisableController(&instance->anim, 0x1A, 0xE);
    G2Anim_DisableController(&instance->anim, 0x20, 0xE);
    G2Anim_DisableController(&instance->anim, 5, 0xE);
    G2Anim_DisableController(&instance->anim, 0xC, 0xE);
    G2Anim_DisableController(&instance->anim, 4, 0xE);
    G2Anim_DisableController(&instance->anim, 0xB, 0xE);

    if (mv->age != 0)
    {
        instance->position.z = instance->position.z - 0x1C2;
    }
    else
    {
        instance->position.z = instance->position.z - 0x190;
    }
}

void ALUKA_SetSwimBodyTwist(Instance *instance, int pitch, int yaw)
{

    G2SVector3 extraRot;

    if (!ALUKA_ControllersEnabled(instance))
    {
        return;
    }

    extraRot.z = 0;
    extraRot.x = 0;
    extraRot.y = -pitch & 0xFFF;

    G2Anim_SetController_Vector(&instance->anim, 5, 0xE, &extraRot);
    G2Anim_SetController_Vector(&instance->anim, 0xC, 0xE, &extraRot);
    G2Anim_SetController_Vector(&instance->anim, 4, 0xE, &extraRot);
    G2Anim_SetController_Vector(&instance->anim, 0xB, 0xE, &extraRot);

    extraRot.y = pitch & 0xFFF;
    extraRot.x = yaw & 0xFFF;

    G2Anim_SetController_Vector(&instance->anim, 0x13, 0xE, &extraRot);

    extraRot.x = -yaw & 0xFFF;

    if (pitch > 0)
    {
        extraRot.y = -(pitch * 2) & 0xFFF;
        G2Anim_SetController_Vector(&instance->anim, 0x1A, 0xE, &extraRot);
        extraRot.y = -pitch & 0xFFF;
        G2Anim_SetController_Vector(&instance->anim, 0x20, 0xE, &extraRot);
    }
    else
    {
        extraRot.y = -pitch & 0xFFF;
        G2Anim_SetController_Vector(&instance->anim, 0x1A, 0xE, &extraRot);
        extraRot.y = -(pitch * 2) & 0xFFF;
        G2Anim_SetController_Vector(&instance->anim, 0x20, 0xE, &extraRot);
    }

}

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_NotDaylight);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_CapDepth);

int ALUKA_AngleTooWide(Position *first, Position *second, int cosmult, int cosshift)
{

    long lenFirst; // not from debug symbols
    long lenSecond; // not from debug symbols
    long dot; // not from debug symbols

    lenFirst = MATH3D_LengthXYZ(first->x, first->y, first->z);
    lenSecond = MATH3D_LengthXYZ(second->x, second->y, second->z);
    dot = (first->x * second->x) + (first->y * second->y) + (first->z * second->z);

    return ((lenFirst * lenSecond * cosmult) >> cosshift) >= dot;
}

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_VectorFromPitchYaw);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_FacingVector);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_SimpleLineCheck);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_TerrainInPath);

int ALUKA_ApplyIncr(int start, int min, int max, int delta, int time)
{
    int result; // not from debug symbols

    result = start + ((delta * time) / 4096);

    if (result < min && delta < 0)
    {
        result = min;
    }
    else if (result > max && delta > 0)
    {
        result = max;
    }

    return result;
}


void ALUKA_ApplyForwardAccel(Instance *instance, int forward_accel, int time)
{
    MonsterVars *mv; // not from debug symbols
    AlukaAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    attrs = (AlukaAttributes *)mv->extraVars;

    mv->speed = ALUKA_ApplyIncr(mv->speed, 0, attrs->circle_dist, forward_accel, time);
}

void ALUKA_ApplyAngularAccel(Instance *instance, int yaw_accel, int pitch_accel, int time)
{
    MonsterVars *mv; // not from debug symbols
    AlukaAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    attrs = (AlukaAttributes *)mv->extraVars;

    attrs->swimfast_speed = ALUKA_ApplyIncr(attrs->swimfast_speed, -attrs->pitch_offset_speed, attrs->pitch_offset_speed, yaw_accel, time);
    attrs->swimattack_speed = ALUKA_ApplyIncr(attrs->swimattack_speed, -attrs->yaw_accel, attrs->yaw_accel, pitch_accel, time);
}

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

void ALUKA_ResetSwim(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (ALUKA_ControllersEnabled(instance) == G2FALSE)
    {
        return;
    }

    mv->auxFlags &= ~0x20000000;
    ALUKA_DisableControllers(instance);
    MON_PlayCombatIdle(instance, 2);
    mv->mvFlags &= ~0x800;
    instance->zVel = 0;
    instance->maxXVel = 0x190;
    instance->maxYVel = 0x190;
    instance->maxZVel = 0x190;

}

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

void ALUKA_Notice(Instance *instance)
{
    if (!(((MonsterVars *)instance->extraData)->mvFlags & 0x400))
    {
        ALUKA_ResetSwim(instance);
        MON_Notice(instance);
    }
    else
    {
        MON_SwitchState(instance, MONSTER_STATE_PURSUE);
    }
}

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_EmbraceEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_Embrace);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_GeneralDeathEntry);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_GeneralDeath);

#else

G2Bool ALUKA_ControllersEnabled(Instance *instance)
{
    return G2Anim_IsControllerActive(&instance->anim, 1, 0x26) != 0;
}

void ALUKA_SetPitch(Instance *instance, int pitch)
{
    G2SVector3 extraRot;
    AlukaAttributes *attributes; // not from debug symbols

    attributes = (AlukaAttributes *)((MonsterVars *)instance->extraData)->extraVars;

    if (!ALUKA_ControllersEnabled(instance))
    {
        return;
    }

    extraRot.x = pitch;
    extraRot.z = 0;
    extraRot.y = 0;
    G2Anim_SetController_Vector(&instance->anim, 1, 0xE, &extraRot);
    attributes->swimfast_pitch = pitch;
}

void ALUKA_EnableControllers(Instance *instance)
{
    G2SVector3 translate;
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (ALUKA_ControllersEnabled(instance))
    {
        return;
    }

    G2Anim_EnableController(&instance->anim, 1, 0x26);
    G2Anim_EnableController(&instance->anim, 1, 0xE);
    G2Anim_EnableController(&instance->anim, 0x13, 0xE);
    G2Anim_EnableController(&instance->anim, 0x1A, 0xE);
    G2Anim_EnableController(&instance->anim, 0x20, 0xE);
    G2Anim_EnableController(&instance->anim, 5, 0xE);
    G2Anim_EnableController(&instance->anim, 0xC, 0xE);
    G2Anim_EnableController(&instance->anim, 4, 0xE);
    G2Anim_EnableController(&instance->anim, 0xB, 0xE);

    translate.y = 0;
    translate.x = 0;

    if (mv->age != 0)
    {
        translate.z = -0x1C2;
        instance->position.z -= translate.z;
    }
    else
    {
        translate.z = -0x190;
        instance->position.z -= translate.z;
    }

    G2Anim_SetController_Vector(&instance->anim, 1, 0x26, &translate);
}

void ALUKA_DisableControllers(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (!ALUKA_ControllersEnabled(instance))
    {
        return;
    }

    G2Anim_DisableController(&instance->anim, 1, 0x26);
    G2Anim_DisableController(&instance->anim, 1, 0xE);
    G2Anim_DisableController(&instance->anim, 0x13, 0xE);
    G2Anim_DisableController(&instance->anim, 0x1A, 0xE);
    G2Anim_DisableController(&instance->anim, 0x20, 0xE);
    G2Anim_DisableController(&instance->anim, 5, 0xE);
    G2Anim_DisableController(&instance->anim, 0xC, 0xE);
    G2Anim_DisableController(&instance->anim, 4, 0xE);
    G2Anim_DisableController(&instance->anim, 0xB, 0xE);

    if (mv->age != 0)
    {
        instance->position.z = instance->position.z - 0x1C2;
    }
    else
    {
        instance->position.z = instance->position.z - 0x190;
    }
}

void ALUKA_SetSwimBodyTwist(Instance *instance, int pitch, int yaw)
{

    G2SVector3 extraRot;

    if (!ALUKA_ControllersEnabled(instance))
    {
        return;
    }

    extraRot.z = 0;
    extraRot.x = 0;
    extraRot.y = -pitch & 0xFFF;

    G2Anim_SetController_Vector(&instance->anim, 5, 0xE, &extraRot);
    G2Anim_SetController_Vector(&instance->anim, 0xC, 0xE, &extraRot);
    G2Anim_SetController_Vector(&instance->anim, 4, 0xE, &extraRot);
    G2Anim_SetController_Vector(&instance->anim, 0xB, 0xE, &extraRot);

    extraRot.y = pitch & 0xFFF;
    extraRot.x = yaw & 0xFFF;

    G2Anim_SetController_Vector(&instance->anim, 0x13, 0xE, &extraRot);

    extraRot.x = -yaw & 0xFFF;

    if (pitch > 0)
    {
        extraRot.y = -(pitch * 2) & 0xFFF;
        G2Anim_SetController_Vector(&instance->anim, 0x1A, 0xE, &extraRot);
        extraRot.y = -pitch & 0xFFF;
        G2Anim_SetController_Vector(&instance->anim, 0x20, 0xE, &extraRot);
    }
    else
    {
        extraRot.y = -pitch & 0xFFF;
        G2Anim_SetController_Vector(&instance->anim, 0x1A, 0xE, &extraRot);
        extraRot.y = -(pitch * 2) & 0xFFF;
        G2Anim_SetController_Vector(&instance->anim, 0x20, 0xE, &extraRot);
    }

}

void ALUKA_NotDaylight(void) {};

void ALUKA_CapDepth(void) {};

int ALUKA_AngleTooWide(Position *first, Position *second, int cosmult, int cosshift)
{

    long lenFirst; // not from debug symbols
    long lenSecond; // not from debug symbols
    long dot; // not from debug symbols

    lenFirst = MATH3D_LengthXYZ(first->x, first->y, first->z);
    lenSecond = MATH3D_LengthXYZ(second->x, second->y, second->z);
    dot = (first->x * second->x) + (first->y * second->y) + (first->z * second->z);

    return ((lenFirst * lenSecond * cosmult) >> cosshift) >= dot;
}

void ALUKA_VectorFromPitchYaw(void) {};

void ALUKA_FacingVector(void) {};

void ALUKA_SimpleLineCheck(void) {};

void ALUKA_TerrainInPath(void) {};

int ALUKA_ApplyIncr(int start, int min, int max, int delta, int time)
{
    int result; // not from debug symbols

    result = start + ((delta * time) / 4096);

    if (result < min && delta < 0)
    {
        result = min;
    }
    else if (result > max && delta > 0)
    {
        result = max;
    }

    return result;
}


void ALUKA_ApplyForwardAccel(Instance *instance, int forward_accel, int time)
{
    MonsterVars *mv; // not from debug symbols
    AlukaAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    attrs = (AlukaAttributes *)mv->extraVars;

    mv->speed = ALUKA_ApplyIncr(mv->speed, 0, attrs->circle_dist, forward_accel, time);
}

void ALUKA_ApplyAngularAccel(Instance *instance, int yaw_accel, int pitch_accel, int time)
{
    MonsterVars *mv; // not from debug symbols
    AlukaAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    attrs = (AlukaAttributes *)mv->extraVars;

    attrs->swimfast_speed = ALUKA_ApplyIncr(attrs->swimfast_speed, -attrs->pitch_offset_speed, attrs->pitch_offset_speed, yaw_accel, time);
    attrs->swimattack_speed = ALUKA_ApplyIncr(attrs->swimattack_speed, -attrs->yaw_accel, attrs->yaw_accel, pitch_accel, time);
}

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

void ALUKA_ResetSwim(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (ALUKA_ControllersEnabled(instance) == G2FALSE)
    {
        return;
    }

    mv->auxFlags &= ~0x20000000;
    ALUKA_DisableControllers(instance);
    MON_PlayCombatIdle(instance, 2);
    mv->mvFlags &= ~0x800;
    instance->zVel = 0;
    instance->maxXVel = 0x190;
    instance->maxYVel = 0x190;
    instance->maxZVel = 0x190;

}

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

void ALUKA_Notice(Instance *instance)
{
    if (!(((MonsterVars *)instance->extraData)->mvFlags & 0x400))
    {
        ALUKA_ResetSwim(instance);
        MON_Notice(instance);
    }
    else
    {
        MON_SwitchState(instance, MONSTER_STATE_PURSUE);
    }
}

void ALUKA_EmbraceEntry(void) {};

void ALUKA_Embrace(void) {};

void ALUKA_GeneralDeathEntry(void) {};

void ALUKA_GeneralDeath(void) {};

#endif
