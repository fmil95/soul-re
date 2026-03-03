#include "common.h"
#include "Game/COLLIDE.h"
#include "Game/DEBUG.h"
#include "Game/GAMELOOP.h"
#include "Game/INSTANCE.h"
#include "Game/MATH3D.h"
#include "Game/MEMPACK.h"
#include "Game/G2/ANMCTRLR.h"
#include "Game/G2/ANMG2ILF.h"
#include "Game/MONSTER/MISSILE.h"
#include "Game/MONSTER/MONLIB.h"
#include "Game/MONSTER/MONMSG.h"
#include "Game/MONSTER/MONSTER.h"
#include "Game/PHYSICS.h"
#include "Game/PLAN/ENMYPLAN.h"
#include "Game/STATE.h"
#include "Game/STREAM.h"
#include "Overlays/aluka/aluka.h"

// TODO: These can be deleted once these functions are matched
void ALUKA_SwimToDestination(Instance *instance);
void ALUKA_GetRandomDestination(Instance *instance, Position *destination, Position *start, int range);
int ALUKA_CapDepth(Instance *instance, Level *level);

// this conditional is for the objdiff report
#ifndef SKIP_ASM

G2Bool ALUKA_ControllersEnabled(Instance *instance)
{
    return G2Anim_IsControllerActive(&instance->anim, 1, G2ANIM_CTRLRTYPE_ADD_LOCALTRANS) != G2FALSE;
}


void ALUKA_SetPitch(Instance *instance, int pitch)
{
    G2SVector3 extraRot;
    AlukaAttributes *attributes; // not from debug symbols

    attributes = (AlukaAttributes *)((MonsterVars *)instance->extraData)->extraVars;

    if (ALUKA_ControllersEnabled(instance) == G2FALSE)
    {
        return;
    }

    extraRot.x = pitch;
    extraRot.z = 0;
    extraRot.y = 0;
    G2Anim_SetController_Vector(&instance->anim, 1, G2ANIM_CTRLRTYPE_ADD_LOCALROT, &extraRot);
    attributes->swimfast_pitch = pitch;
}

void ALUKA_EnableControllers(Instance *instance)
{
    G2SVector3 translate;
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (ALUKA_ControllersEnabled(instance) != G2FALSE)
    {
        return;
    }

    G2Anim_EnableController(&instance->anim, 1, G2ANIM_CTRLRTYPE_ADD_LOCALTRANS);
    G2Anim_EnableController(&instance->anim, 1, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_EnableController(&instance->anim, 19, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_EnableController(&instance->anim, 26, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_EnableController(&instance->anim, 32, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_EnableController(&instance->anim, 5, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_EnableController(&instance->anim, 12, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_EnableController(&instance->anim, 4, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_EnableController(&instance->anim, 11, G2ANIM_CTRLRTYPE_ADD_LOCALROT);

    translate.y = 0;
    translate.x = 0;

    if (mv->age != 0)
    {
        translate.z = -450;
        instance->position.z -= translate.z;
    }
    else
    {
        translate.z = -400;
        instance->position.z -= translate.z;
    }

    G2Anim_SetController_Vector(&instance->anim, 1, G2ANIM_CTRLRTYPE_ADD_LOCALTRANS, &translate);
}

void ALUKA_DisableControllers(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (ALUKA_ControllersEnabled(instance) == G2FALSE)
    {
        return;
    }

    G2Anim_DisableController(&instance->anim, 1, G2ANIM_CTRLRTYPE_ADD_LOCALTRANS);
    G2Anim_DisableController(&instance->anim, 1, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DisableController(&instance->anim, 19, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DisableController(&instance->anim, 26, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DisableController(&instance->anim, 32, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DisableController(&instance->anim, 5, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DisableController(&instance->anim, 12, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DisableController(&instance->anim, 4, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DisableController(&instance->anim, 11, G2ANIM_CTRLRTYPE_ADD_LOCALROT);

    if (mv->age != 0)
    {
        instance->position.z -= 450;
    }
    else
    {
        instance->position.z -= 400;
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

    G2Anim_SetController_Vector(&instance->anim, 5, G2ANIM_CTRLRTYPE_ADD_LOCALROT, &extraRot);
    G2Anim_SetController_Vector(&instance->anim, 12, G2ANIM_CTRLRTYPE_ADD_LOCALROT, &extraRot);
    G2Anim_SetController_Vector(&instance->anim, 4, G2ANIM_CTRLRTYPE_ADD_LOCALROT, &extraRot);
    G2Anim_SetController_Vector(&instance->anim, 11, G2ANIM_CTRLRTYPE_ADD_LOCALROT, &extraRot);

    extraRot.y = pitch & 0xFFF;
    extraRot.x = yaw & 0xFFF;

    G2Anim_SetController_Vector(&instance->anim, 19, G2ANIM_CTRLRTYPE_ADD_LOCALROT, &extraRot);

    extraRot.x = -yaw & 0xFFF;

    if (pitch > 0)
    {
        extraRot.y = -(pitch * 2) & 0xFFF;
        G2Anim_SetController_Vector(&instance->anim, 26, G2ANIM_CTRLRTYPE_ADD_LOCALROT, &extraRot);
        extraRot.y = -pitch & 0xFFF;
        G2Anim_SetController_Vector(&instance->anim, 32, G2ANIM_CTRLRTYPE_ADD_LOCALROT, &extraRot);
    }
    else
    {
        extraRot.y = -pitch & 0xFFF;
        G2Anim_SetController_Vector(&instance->anim, 26, G2ANIM_CTRLRTYPE_ADD_LOCALROT, &extraRot);
        extraRot.y = -(pitch * 2) & 0xFFF;
        G2Anim_SetController_Vector(&instance->anim, 32, G2ANIM_CTRLRTYPE_ADD_LOCALROT, &extraRot);
    }

}

int ALUKA_NotDaylight(Level *level)
{

    if (level->unitFlags & 2 && GAMELOOP_GetTimeOfDay() == 700)
    {
        return 0;
    }

    return 1;

}

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

void ALUKA_VectorFromPitchYaw(Position *vector, int pitch, int yaw, int dist)
{

    MATRIX mat;
    SVECTOR oldVector;
    SVECTOR newVector;

    oldVector.vx = 0;
    oldVector.vy = -dist;
    oldVector.vz = 0;

    mat.m[0][0] = 4096;
    mat.m[0][1] = 0;
    mat.m[0][2] = 0;
    mat.m[1][0] = 0;
    mat.m[1][1] = 4096;
    mat.m[1][2] = 0;
    mat.m[2][0] = 0;
    mat.m[2][1] = 0;
    mat.m[2][2] = 4096;

    RotMatrixX(pitch, &mat);
    RotMatrixZ(yaw, &mat);
    ApplyMatrixSV(&mat, &oldVector, &newVector);

    COPY_SVEC(Position, (Position *)vector, Position, (Position *)&newVector);
}


void ALUKA_FacingVector(Instance *instance, Position *vector, int dist)
{
    MonsterVars *mv; // not from debug symbols
    AlukaAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    attrs = (AlukaAttributes *)mv->extraVars;

    ALUKA_VectorFromPitchYaw(vector, (attrs->swimfast_pitch - attrs->min_swim_depth) & 0xFFF, instance->rotation.z, dist);
}

int ALUKA_SimpleLineCheck(Position *hit, Level *level, Position *start, Position *end)
{

    PCollideInfo info;
    SVECTOR oldPt;
    SVECTOR newPt;

    oldPt.vx = start->x;
    oldPt.vy = start->y;
    oldPt.vz = start->z;

    newPt.vx = end->x;
    newPt.vy = end->y;
    newPt.vz = end->z;

    info.oldPoint = &oldPt;
    info.newPoint = &newPt;

    info.collideType = 3;
    info.instance = NULL;
    info.inst = NULL;

    COLLIDE_PointAndWorld(&info, level);

    if (info.type != 0)
    {
        if (hit != NULL)
        {
            hit->x = newPt.vx;
            hit->y = newPt.vy;
            hit->z = newPt.vz;
        }
        return 1;
    }
    return 0;
}


int ALUKA_TerrainInPath(Instance *instance)
{
    Position vector; // not from debug symbols
    Level *level; // not from debug symbols

    level = STREAM_GetLevelWithID(instance->currentStreamUnitID);

    ALUKA_FacingVector(instance, &vector, 400);
    ADD_SVEC(Position, &vector, Position, &vector, Position, &instance->position);

    return ALUKA_SimpleLineCheck(0, level, &instance->position, &vector);
}

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

void ALUKA_ApplyRots(Instance *instance, int time)
{

    int newDepth; // not from debug symbols
    int newPitch; // not from debug symbols
    int newRot; // not from debug symbols
    int delta; // not from debug symbols
    int pitch; // not from debug symbols
    int swimPitch; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    AlukaVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (AlukaVars *)mv->extraVars;

    newRot = ALUKA_ApplyIncr(instance->rotation.z, -32767, 32767, vars->yaw_speed, time);
    delta = vars->pitch_offset;
    swimPitch = vars->pitch & 0xFFF;

    if (swimPitch <= 2048)
    {
        delta = swimPitch - vars->pitch_offset;
    }
    else
    {
        delta = swimPitch - 4096 - vars->pitch_offset;
    }

    newDepth = ALUKA_ApplyIncr(delta, -682, 682, vars->pitch_speed, time);
    newPitch = ALUKA_ApplyIncr(vars->pitch_offset, 0, 1024, vars->pitch_offset_speed, time);
    pitch = (newDepth + newPitch) & 0xFFF;
    instance->rotation.z = newRot & 0xFFF;
    ALUKA_SetPitch(instance, pitch);
    vars->pitch_offset = newPitch;
}

void ALUKA_MoveForward(Instance *instance, int time, int depth)
{
    Position vector; // not from debug symbols
    int dist; // not from debug symbols

    dist = ((MonsterVars *)instance->extraData)->speed * time;
    ALUKA_FacingVector(instance, &vector, dist / 4096);

    ADD_SVEC(Position, &instance->position, Position, &instance->position, Position, &vector);

    if (depth < instance->position.z)
    {
        instance->position.z = depth;
    }
}

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_FixPitch);

void ALUKA_ProportionalLimitsAndAccels(Instance *instance, int angley, int anglep)
{

    int absYaw; // not from debug symbols
    int absPitch; // not from debug symbols

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukaVars *vars; // not from debug symbols
    AlukaAttributes *attrs; // not from debug symbols


    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (AlukaVars *)mv->extraVars;
    attrs = (AlukaAttributes *)ma->tunData;

    absYaw = abs(angley);
    absPitch = abs(anglep);

    vars->use_yaw_speed_limit = MIN(abs(angley) >> 3, vars->yaw_speed_limit);
    vars->use_pitch_speed_limit = MIN(abs(anglep) >> 3, vars->pitch_speed_limit);

    vars->use_yaw_accel = MIN(MAX(abs(absYaw) >> 1, 1), attrs->yaw_accel);
    vars->use_pitch_accel = MIN(MAX(abs(absPitch) >> 1, 1), attrs->pitch_accel);

}

int ALUKA_FacePoint(Instance *instance, Position *point, int rate)
{
    int newPitch;

    int targetYaw; // not from debug symbols
    int targetPitch; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    AlukaAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    attrs = (AlukaAttributes *)mv->extraVars;

    targetYaw = MATH3D_AngleFromPosToPos(&instance->position, point);
    targetPitch = (MATH3D_ElevationFromPosToPos(&instance->position, point) + attrs->min_swim_depth) & 0xFFF;

    AngleMoveToward(&instance->rotation.z, targetYaw, (rate * gameTrackerX.timeMult) / 4096);

    newPitch = attrs->swimfast_pitch;
    AngleMoveToward((short *)&newPitch, targetPitch, (rate * gameTrackerX.timeMult) / 4096);
    ALUKA_SetPitch(instance, newPitch);

    if (instance->rotation.z == targetYaw && attrs->swimfast_pitch == targetPitch)
    {
        return 1;
    }

    return 0;
}

void ALUKA_SetupSwimAnimWOTread(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukaVars *vars; // not from debug symbols
    AlukaAttributes *alukaAttrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;

    vars = (AlukaVars *)mv->extraVars;
    alukaAttrs = (AlukaAttributes *)ma->tunData;

    if (vars->swim_anim == ALUKA_ANIM_SWIMATTACK || vars->swim_anim == ALUKA_ANIM_SWIMSPIT)
    {
        return;
    }

    if (mv->speed < alukaAttrs->swimfast_speed)
    {
        if (vars->swim_anim != ALUKA_ANIM_SWIMSLOW)
        {
            MON_PlayAnimFromList(instance, ma->auxAnimList, ALUKA_ANIM_SWIMSLOW, 2);
            vars->swim_anim = ALUKA_ANIM_SWIMSLOW;

            if (vars->pitch_offset < 1024)
            {
                vars->pitch_offset_speed = alukaAttrs->pitch_offset_speed;
            }
            else
            {
                vars->pitch_offset_speed = 0;
            }
        }
    }
    else if (vars->swim_anim != ALUKA_ANIM_SWIMFAST)
    {

        MON_PlayAnimFromList(instance, ma->auxAnimList, ALUKA_ANIM_SWIMFAST, 2);
        vars->swim_anim = ALUKA_ANIM_SWIMFAST;

        if (vars->pitch_offset < 1024)
        {
            vars->pitch_offset_speed = alukaAttrs->pitch_offset_speed;
        }
        else
        {
            vars->pitch_offset_speed = 0;
        }
    }
}

void ALUKA_SetupSwimAnimWTread(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukaVars *vars; // not from debug symbols
    AlukaAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;

    vars = (AlukaVars *)mv->extraVars;
    attrs = (AlukaAttributes *)ma->tunData;

    if (vars->swim_anim == ALUKA_ANIM_SWIMATTACK || vars->swim_anim == ALUKA_ANIM_SWIMSPIT)
    {
        return;
    }

    if (mv->speed < attrs->swimslow_speed)
    {
        if (vars->swim_anim != ALUKA_ANIM_TREAD)
        {
            MON_PlayAnimFromList(instance, ma->auxAnimList, ALUKA_ANIM_TREAD, 2);
            vars->swim_anim = ALUKA_ANIM_TREAD;
            if (vars->pitch_offset > 0)
            {
                vars->pitch_offset_speed = -attrs->pitch_offset_speed;
            }
            else
            {
                vars->pitch_offset_speed = 0;
            }
        }
    }
    else
    {
        ALUKA_SetupSwimAnimWOTread(instance);
    }
}


INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_SwimToDestination);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_GetRandomDestination);

int ALUKA_NearAluka(Instance *instance)
{

    int dist; // not from debug symbols
    int oldDist; // not from debug symbols
    Instance *inst; // not from debug symbols

    for (inst = gameTrackerX.instanceList->first; inst != NULL; inst = inst->next)
    {

        if (inst == instance || INSTANCE_Query(inst, queryWhatAmI) != 130)
        {
            continue;
        }

        dist = MATH3D_LengthXYZ(instance->position.x - inst->position.x, instance->position.y - inst->position.y, instance->position.z - inst->position.z);
        oldDist = MATH3D_LengthXYZ(instance->oldPos.x - inst->oldPos.x, instance->oldPos.y - inst->oldPos.y, instance->oldPos.z - inst->oldPos.z);

        if (dist < 640 && dist < oldDist)
        {
            return 1;
        }
    }

    return 0;
}


INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_GetCircleDestination);

int ALUKA_SwimPlanMovement(Instance *instance, Position *target, Position *step, Level *level)
{

    int dist; // not from debug symbols
    int depth; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukaAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    attrs = (AlukaAttributes *)ma->tunData;

    dist = MATH3D_LengthXYZ(instance->position.x - target->x, instance->position.y - target->y, instance->position.z - target->z);
    depth = ALUKA_CapDepth(instance, level);

    COPY_SVEC(Position, step, Position, target);

    if (attrs->circle_start_dist < dist || (mv->age != 0 && depth < target->z && ALUKA_NotDaylight(level)))
    {
        switch (ENMYPLAN_MoveToTargetFinal(instance, step, mv->pathSlotID, target, 0x1E00001F))
        {
        case 0:
            if (attrs->circle_start_dist < dist)
            {
                return 2;
            }
            return 0;
        case 3:
            MON_GetPlanSlot(mv);
            if (attrs->circle_start_dist >= dist)
            {
                return 0;
            }
            return 3;
        default:
            return 1;
        }
    }

    return 0;
}


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
    instance->maxXVel = 400;
    instance->maxYVel = 400;
    instance->maxZVel = 400;

}

int ALUKA_ShouldJumpIn(Instance *instance, Instance *enemy, Level *level)
{

    int nodeType; // not from debug symbols

    (void)enemy;
    (void)level;

    nodeType = ENMYPLAN_GetNodeTypeOfNextWaypoint(((MonsterVars *)instance->extraData)->pathSlotID);

    if (nodeType != 64 && ((nodeType >> 3) & 3) == 3)
    {
        return 1;
    }

    return 0;
}


int ALUKA_ShouldJumpOut(Instance *instance, Instance *enemy, Level *level)
{

    int nodeType; // not from debug symbols
    MonsterVars *mv; // not from debug symbols

    (void)enemy;

    mv = (MonsterVars *)instance->extraData;

    if (mv->age == 0 || !ALUKA_NotDaylight(level))
    {
        return 0;
    }

    nodeType = ENMYPLAN_GetNodeTypeOfNextWaypoint(mv->pathSlotID);

    if (nodeType != 64 && ((nodeType >> 3) & 3) == 0)
    {
        return 1;
    }

    return 0;
}

void ALUKA_JumpToEntry(Instance *instance, Position *target)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukaVars *vars; // not from debug symbols
    AlukaAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (AlukaVars *)mv->extraVars;
    attrs = (AlukaAttributes *)ma->tunData;

    mv->auxFlags |= 1;

    if (mv->auxFlags & 2)
    {
        MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, ALUKA_ANIM_JUMPIN, 0);
    }
    else
    {
        vars->forward_speed_limit = 0;
        vars->yaw_speed_limit = attrs->swimslow_yaw;
        vars->pitch_speed_limit = attrs->slow_pitch;
        mv->auxFlags &= ~0x10;
    }

    COPY_SVEC(Position, &vars->target, Position, target);

    mv->auxFlags &= ~4;
    mv->mvFlags |= 0x800;

    instance->xAccl = 0;
    instance->yAccl = 0;
    instance->zAccl = -16;

    instance->maxXVel = 65535;
    instance->maxYVel = 65535;
    instance->maxZVel = 65535;

    mv->mode = 0x100000;
}

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_SetJumpVels);

void ALUKA_SetHitGround(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    mv->auxFlags = mv->auxFlags & ~1;
    mv->mvFlags = mv->mvFlags & ~0x800;

    instance->zVel = 0;
    instance->maxXVel = 400;
    instance->maxYVel = 400;
    instance->maxZVel = 400;

    MON_SwitchState(instance, MONSTER_STATE_LANDONFEET);
}

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_JumpTo);

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_ChooseAttack);

void ALUKA_Init(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    AlukaVars *vars; // not from debug symbols

    MON_DefaultInit(instance);

    mv = (MonsterVars *)instance->extraData;

    if (mv != NULL)
    {

        vars = (AlukaVars *)MEMPACK_Malloc(sizeof(AlukaVars), 27);

        if (vars == NULL)
        {
            mv->extraVars = NULL;
        }
        else
        {
            mv->extraVars = vars;
            vars->pitch = 0;
        }
    }

    instance->rotation.x = 0;

    G2Anim_AttachControllerToSeg(&instance->anim, 1, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_AttachControllerToSeg(&instance->anim, 1, G2ANIM_CTRLRTYPE_ADD_LOCALTRANS);
    G2Anim_AttachControllerToSeg(&instance->anim, 19, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_AttachControllerToSeg(&instance->anim, 26, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_AttachControllerToSeg(&instance->anim, 32, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_AttachControllerToSeg(&instance->anim, 5, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_AttachControllerToSeg(&instance->anim, 12, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_AttachControllerToSeg(&instance->anim, 4, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_AttachControllerToSeg(&instance->anim, 11, G2ANIM_CTRLRTYPE_ADD_LOCALROT);

    G2Anim_DisableController(&instance->anim, 1, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DisableController(&instance->anim, 1, G2ANIM_CTRLRTYPE_ADD_LOCALTRANS);
    G2Anim_DisableController(&instance->anim, 19, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DisableController(&instance->anim, 26, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DisableController(&instance->anim, 32, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DisableController(&instance->anim, 5, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DisableController(&instance->anim, 12, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DisableController(&instance->anim, 4, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DisableController(&instance->anim, 11, G2ANIM_CTRLRTYPE_ADD_LOCALROT);

    DEBUG_DoAreaProtection();
}

void ALUKA_CleanUp(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    G2Anim_DetachControllerFromSeg(&instance->anim, 1, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DetachControllerFromSeg(&instance->anim, 1, G2ANIM_CTRLRTYPE_ADD_LOCALTRANS);
    G2Anim_DetachControllerFromSeg(&instance->anim, 19, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DetachControllerFromSeg(&instance->anim, 26, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DetachControllerFromSeg(&instance->anim, 32, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DetachControllerFromSeg(&instance->anim, 5, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DetachControllerFromSeg(&instance->anim, 12, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DetachControllerFromSeg(&instance->anim, 4, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DetachControllerFromSeg(&instance->anim, 11, G2ANIM_CTRLRTYPE_ADD_LOCALROT);

    mv = (MonsterVars *)instance->extraData;

    if (mv != NULL && mv->extraVars != NULL)
    {
        MEMPACK_Free(mv->extraVars);
    }

    MON_CleanUp(instance);
}

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_Message);

void ALUKA_PursueEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukaVars *vars; // not from debug symbols
    AlukaAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (AlukaVars *)mv->extraVars;
    attrs = (AlukaAttributes *)ma->tunData;

    if (vars == NULL)
    {
        return;
    }

    if (!(mv->mvFlags & 0x400))
    {
        MON_PursueEntry(instance);
        return;
    }

    if (ALUKA_ControllersEnabled(instance) == G2FALSE)
    {
        mv->speed = 0;
        vars->pitch_offset = 1024;
        vars->yaw_speed = 0;
        vars->pitch_speed = 0;
        vars->pitch_offset_speed = 0;
        vars->swim_anim = ALUKA_ANIM_NO_ANIM;
        ALUKA_EnableControllers(instance);
    }

    MON_GetPlanSlot(mv);
    vars->last_pm = 0;
    vars->special_time = MON_GetTime(instance) + (attrs->too_long_since_attack * 990);
    mv->mode = 4;
}

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_Pursue);

void ALUKA_AttackEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    AlukaVars *vars; // not from debug symbols
    AlukaAttributes *attrs; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    MonsterCombatAttributes *combat; // not from debug symbols
    int attack; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (AlukaVars *)mv->extraVars;
    attrs = (AlukaAttributes *)ma->tunData;

    if (vars == NULL)
    {
        return;
    }

    if (!(mv->mvFlags & 0x400))
    {
        MON_AttackEntry(instance);
        return;
    }

    combat = mv->subAttr->combatAttributes;

    /*
     The pointer manipulation here is because of the fact that there is no field
     at offset 0x2B. It's likely that the intent was to access 0x2C, which is
     the `attackList` field.
    */
    attack = combat->numAttacks + 0x2B;
    mv->attackType = &ma->attackAttributesList[*((signed char *)combat + attack)];

    mv->attackState = 0;

    vars->forward_speed_limit = attrs->swimattack_speed;
    vars->yaw_speed_limit = attrs->swimattack_yaw;
    vars->pitch_speed_limit = attrs->swimattack_pitch;
}

INCLUDE_RODATA("asm/nonmatchings/Overlays/aluka/aluka", D_88000000);

void ALUKA_Attack(Instance *instance)
{

    int destinationInRange; // not from debug symbols
    MonsterIR *enemy; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukaVars *vars; // not from debug symbols
    AlukaAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    attrs = (AlukaAttributes *)ma->tunData;
    vars = (AlukaVars *)mv->extraVars;
    enemy = mv->enemy;

    if (vars == NULL)
    {
        return;
    }

    if (!(mv->mvFlags & 0x400))
    {
        ALUKA_ResetSwim(instance);
        MON_Attack(instance);
        return;
    }

    if (mv->mvFlags & 4)
    {
        MON_SwitchState(instance, MONSTER_STATE_IDLE);
        return;
    }

    destinationInRange = 0;
    mv->auxFlags |= 0x20000000;

    if (enemy != NULL)
    {
        COPY_SVEC(Position, &mv->destination, Position, &enemy->instance->position);
        mv->destination.z += attrs->attack_pos_bump;
        destinationInRange = ALUKA_CapDepth(instance, STREAM_GetLevelWithID(instance->currentStreamUnitID)) > mv->destination.z;
    }

    if (destinationInRange)
    {
        switch (mv->attackState)
        {
        case 0:
            if (MATH3D_LengthXYZ(mv->destination.x - instance->position.x, mv->destination.y - instance->position.y, mv->destination.z - instance->position.z) < attrs->attack_dist)
            {
                mv->attackState++;
            }
            break;
        case 1:
            MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, ALUKA_ANIM_SWIMATTACK, 1);
            vars->swim_anim = ALUKA_ANIM_SWIMATTACK;
            mv->attackState++;
            vars->special_time = MON_GetTime(instance) + (mv->attackType->sphereOnFrame * 33);
            break;
        case 2:
            if (MON_GetTime(instance) >= (unsigned long)vars->special_time)
            {
                MON_TurnOnWeaponSpheres(instance);
                mv->attackState++;
                vars->special_time = MON_GetTime(instance) + (mv->attackType->sphereOffFrame * 33);
            }
            break;
        case 3:
            if (MON_GetTime(instance) >= (unsigned long)vars->special_time)
            {
                MON_TurnOffWeaponSpheres(instance);
                mv->attackState++;
            }
            break;
        case 4:
            if (instance->flags2 & 0x10)
            {

                vars->swim_anim = ALUKA_ANIM_NO_ANIM;
                mv->attackState = 0;

                if (mv->mvFlags & 0x10000000 && mv->enemy->mirFlags & 0x1000)
                {
                    MON_SwitchState(instance, MONSTER_STATE_EMBRACE);
                }
                else
                {
                    MON_SwitchState(instance, MONSTER_STATE_PURSUE);
                }
            }
            break;
        }
    }
    else
    {
        MON_TurnOffWeaponSpheres(instance);
        MON_SwitchState(instance, MONSTER_STATE_IDLE);
    }

    ALUKA_SetupSwimAnimWOTread(instance);
    ALUKA_SwimToDestination(instance);
}

void ALUKA_FleeEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukaVars *vars; // not from debug symbols
    AlukaAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;

    vars = (AlukaVars *)mv->extraVars;
    attrs = (AlukaAttributes *)ma->tunData;

    if (!(mv->mvFlags & 0x400))
    {
        MON_FleeEntry(instance);
        return;
    }

    vars->forward_speed_limit = attrs->swimfast_speed;
    vars->yaw_speed_limit = attrs->swimfast_yaw;
    vars->pitch_speed_limit = attrs->swimfast_pitch;
    mv->mode = 4;
}

void ALUKA_Flee(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (!(mv->mvFlags & 0x400))
    {
        ALUKA_ResetSwim(instance);
        MON_Flee(instance);
        return;
    }

    mv->auxFlags |= 0x20000000;

    ALUKA_SetupSwimAnimWOTread(instance);
    ALUKA_SwimToDestination(instance);
    MON_SwitchState(instance, MONSTER_STATE_IDLE);
}

void ALUKA_WanderEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukaVars *vars; // not from debug symbols
    AlukaAttributes *attrs; // not from debug symbols
    int range; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;

    vars = (AlukaVars *)mv->extraVars;
    attrs = (AlukaAttributes *)ma->tunData;

    if (vars == NULL || attrs == NULL)
    {
        return;
    }

    if (!(mv->mvFlags & 0x400))
    {
        MON_WanderEntry(instance);
        return;
    }


    if (mv->wanderRange != 0)
    {
        range = mv->wanderRange;
    }
    else
    {
        range = attrs->wander_range;
    }

    ALUKA_GetRandomDestination(instance, &mv->destination, &instance->intro->position, range);

    vars->forward_speed_limit = attrs->swimslow_speed;
    vars->yaw_speed_limit = attrs->swimslow_yaw;
    vars->pitch_speed_limit = attrs->slow_pitch;

}

void ALUKA_Wander(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukaAttributes *attrs; // not from debug symbols
    int range; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    attrs = (AlukaAttributes *)ma->tunData;

    if (attrs == NULL)
    {
        return;
    }

    if (!(mv->mvFlags & 0x400))
    {
        ALUKA_ResetSwim(instance);
        MON_Wander(instance);
        return;
    }

    if (mv->mvFlags & 4)
    {
        MON_SwitchState(instance, MONSTER_STATE_IDLE);
        return;
    }

    mv->auxFlags |= 0x20000000;

    if (mv->enemy != NULL)
    {
        MON_SwitchState(instance, MONSTER_STATE_PURSUE);
    }
    else if (rand() < 163)
    {
        MON_SwitchState(instance, MONSTER_STATE_IDLE);
    }

    if (mv->wanderRange != 0)
    {
        range = mv->wanderRange;
    }
    else
    {
        range = attrs->wander_range;
    }

    if (MATH3D_LengthXYZ(instance->position.x - mv->destination.x, instance->position.y - mv->destination.y, instance->position.z - mv->destination.z) < 640)
    {
        ALUKA_GetRandomDestination(instance, &mv->destination, &instance->intro->position, range);
    }

    ALUKA_SetupSwimAnimWOTread(instance);
    ALUKA_SwimToDestination(instance);
}


void ALUKA_IdleEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukaVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (AlukaVars *)mv->extraVars;

    if (vars == NULL)
    {
        return;
    }

    if (!(mv->mvFlags & 0x400))
    {
        MON_IdleEntry(instance);
        return;
    }

    vars->forward_speed_limit = 0;
    vars->yaw_speed_limit = 0;
    vars->pitch_speed_limit = 0;

    ma = (MonsterAttributes *)instance->data;
    MON_PlayAnimFromList(instance, ma->auxAnimList, ALUKA_ANIM_SWIMIDLE, 2);
    vars->swim_anim = ALUKA_ANIM_SWIMIDLE;
    mv->mode = 1;

}

void ALUKA_Idle(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (!(mv->mvFlags & 0x400))
    {
        ALUKA_ResetSwim(instance);
        MON_Idle(instance);
        return;
    }

    if (!(mv->mvFlags & 4))
    {
        mv->auxFlags |= 0x20000000;
        if (mv->enemy != NULL)
        {
            MON_SwitchState(instance, MONSTER_STATE_PURSUE);
        }
        else if (instance->flags2 & 0x12 && rand() < 10922)
        {
            MON_SwitchState(instance, MONSTER_STATE_WANDER);
        }
    }

    ALUKA_SwimToDestination(instance);
}

INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_LandInWaterEntry);

void ALUKA_LandInWater(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    AlukaVars *vars; // not from debug symbols
    Level *level; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (AlukaVars *)mv->extraVars;
    level = STREAM_GetLevelWithID(instance->currentStreamUnitID);

    if (vars == NULL)
    {
        return;
    }

    vars->pitch_offset_speed = 103;
    PhysicsMove(instance, &instance->position, gameTrackerX.timeMult);
    ALUKA_ApplyRots(instance, gameTrackerX.timeMult);

    if (ALUKA_CapDepth(instance, level) >= instance->position.z && vars->pitch_offset == 1024)
    {
        instance->zVel = 0;
        vars->pitch_offset_speed = 0;
        vars->pitch_speed = 0;
        MON_SwitchState(instance, MONSTER_STATE_PURSUE);
        vars->swim_anim = ALUKA_ANIM_NO_ANIM;
    }

    MON_DefaultQueueHandler(instance);
}

void ALUKA_HitEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    MonsterIR *enemy; // not from debug symbols
    AlukaVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (AlukaVars *)mv->extraVars;

    if (vars == NULL)
    {
        return;
    }

    if (!(mv->mvFlags & 0x400))
    {
        MON_HitEntry(instance);
        return;
    }

    enemy = mv->enemy;
    enemy->mirConditions |= 0x400;
    enemy->mirFlags &= ~0x1000;
    mv->mvFlags |= 0x10000;

    if (MON_SetUpKnockBack(instance, enemy->instance, (evMonsterHitData *)mv->messageData))
    {
        MON_PlayAnimFromList(instance, ma->auxAnimList, ALUKA_ANIM_SWIMHIT, 1);
    }

    vars->swim_anim = ALUKA_ANIM_NO_ANIM;
    mv->mode = 0x8000;
}

void ALUKA_Hit(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (mv->extraVars == NULL)
    {
        return;
    }

    if (!(mv->mvFlags & 0x400))
    {
        ALUKA_ResetSwim(instance);
        MON_Hit(instance);
        return;
    }

    mv->auxFlags |= 0x20000000;

    if (mv->generalTimer < MON_GetTime(instance))
    {
        if (mv->mvFlags & 0x100)
        {
            MON_SwitchState(instance, MONSTER_STATE_STUNNED);
        }
        else
        {
            MON_SwitchState(instance, MONSTER_STATE_PURSUE);
        }

        instance->xVel = 0;
        instance->yVel = 0;
        instance->zVel = 0;

    }

    MON_DefaultQueueHandler(instance);
    PHYSICS_StopIfCloseToTarget(instance, 0, 0, 0);
    PhysicsMove(instance, &instance->position, gameTrackerX.timeMult);
}

void ALUKA_StunnedEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukaVars *vars; // not from debug symbols
    MonsterCombatAttributes *combat; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (AlukaVars *)mv->extraVars;
    combat = mv->subAttr->combatAttributes;

    if (vars == NULL)
    {
        return;
    }

    if (!(mv->mvFlags & 0x400))
    {
        MON_StunnedEntry(instance);
        return;
    }

    if (mv->damageType == 64)
    {
        MON_PlayAnimFromList(instance, ma->auxAnimList, ALUKA_ANIM_SWIMAGONY, 2);
        vars->swim_anim = ALUKA_ANIM_SWIMAGONY;
    }
    else
    {
        MON_PlayAnimFromList(instance, ma->auxAnimList, ALUKA_ANIM_SWIMSTUN, 2);
        vars->swim_anim = ALUKA_ANIM_SWIMSTUN;
    }

    mv->mvFlags |= 0x100;
    mv->damageTimer = MON_GetTime(instance) + combat->damageTime;
    mv->stunTimer = MON_GetTime(instance) + combat->stunTime;
    mv->mode = 0x8000;

    vars->forward_speed_limit = 0;
    vars->yaw_speed_limit = 0;
    vars->pitch_speed_limit = 0;
}

void ALUKA_Stunned(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukaVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (AlukaVars *)mv->extraVars;

    if (vars == NULL)
    {
        return;
    }

    if (!(mv->mvFlags & 0x400))
    {
        ALUKA_ResetSwim(instance);
        MON_Stunned(instance);
        return;
    }

    mv->auxFlags |= 0x20000000;

    if (vars->swim_anim == ALUKA_ANIM_SWIMRECOVER)
    {
        if (!(mv->mvFlags & 0x100) && instance->flags2 & 0x10)
        {
            MON_SwitchState(instance, MONSTER_STATE_PURSUE);
        }
    }
    else if (vars->swim_anim == ALUKA_ANIM_SWIMSTUN)
    {
        if (!(mv->mvFlags & 0x100))
        {
            MON_PlayAnimFromList(instance, ma->auxAnimList, ALUKA_ANIM_SWIMRECOVER, 1);
            vars->swim_anim = ALUKA_ANIM_SWIMRECOVER;
        }
    }
    else
    {
        MON_SwitchState(instance, MONSTER_STATE_PURSUE);
    }

    ALUKA_SwimToDestination(instance);
}

void ALUKA_ProjectileEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukaVars *vars; // not from debug symbols
    AlukaAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (AlukaVars *)mv->extraVars;
    attrs = (AlukaAttributes *)ma->tunData;

    if (vars == NULL)
    {
        return;
    }

    if (!(mv->mvFlags & 0x400))
    {
        MON_ProjectileEntry(instance);
        return;
    }

    mv->attackState = 0;
    vars->forward_speed_limit = 0;
    vars->yaw_speed_limit = attrs->swimspit_yaw;
    vars->pitch_speed_limit = attrs->swimspit_pitch;
}

void ALUKA_Projectile(Instance *instance)
{

    MonsterIR *enemy; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    AlukaVars *vars; // not from debug symbols
    AlukaAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (AlukaVars *)mv->extraVars;
    attrs = (AlukaAttributes *)ma->tunData;

    if (vars == NULL)
    {
        return;
    }

    if (!(mv->mvFlags & 0x400))
    {
        ALUKA_ResetSwim(instance);
        MON_Projectile(instance);
        return;
    }

    if (mv->mvFlags & 4)
    {
        MON_SwitchState(instance, MONSTER_STATE_IDLE);
        return;
    }

    enemy = mv->enemy;
    mv->auxFlags |= 0x20000000;

    if (enemy != NULL)
    {

        Instance *enemyInst; // not from debug symbols
        Position *enemyPos; // not from debug symbols

        enemyInst = enemy->instance;
        enemyPos = &enemyInst->position;

        COPY_SVEC(Position, &mv->destination, Position, enemyPos);

        mv->destination.z += attrs->attack_pos_bump;

        switch (mv->attackState)
        {
        case 0:
            if (vars->pitch_offset <= 0)
            {

                Position delta; // not from debug symbols
                Position facing; // not from debug symbols

                delta.x = enemyPos->x - instance->position.x;
                delta.y = enemyPos->y - instance->position.y;
                delta.z = 0;

                ALUKA_FacingVector(instance, &facing, 4096);
                facing.z = 0;
                if (!ALUKA_AngleTooWide(&facing, &delta, 1008, 10))
                {
                    MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, ALUKA_ANIM_SWIMSPIT, 1);
                    vars->swim_anim = ALUKA_ANIM_SWIMSPIT;
                    mv->attackState++;
                    vars->special_time = MON_GetTime(instance) + ((unsigned char)attrs->spit_attack_frame * 33);
                }
            }
            break;
        case 1:
            if (MON_GetTime(instance) >= (unsigned long)vars->special_time)
            {
                MISSILE_FireAtInstance(instance, &ma->missileList[(int)mv->subAttr->combatAttributes->missileAttack], mv->enemy->instance);
                mv->attackState++;
            }
            break;
        case 2:
            if (instance->flags2 & 0x10)
            {
                vars->swim_anim = ALUKA_ANIM_NO_ANIM;
                MON_SwitchState(instance, MONSTER_STATE_PURSUE);
            }
            break;
        }
    }
    else
    {
        MON_SwitchState(instance, MONSTER_STATE_IDLE);
    }

    ALUKA_SetupSwimAnimWTread(instance);
    ALUKA_SwimToDestination(instance);
}

void ALUKA_SurpriseAttackEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    AlukaVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (AlukaVars *)mv->extraVars;

    if (vars == NULL)
    {
        return;
    }

    if (!(mv->mvFlags & 0x400))
    {
        MON_SurpriseAttackEntry(instance);
        return;
    }

    instance->zAccl = 0;
    instance->yAccl = 0;
    instance->xAccl = 0;

    instance->zVel = 0;
    instance->yVel = 0;
    instance->xVel = 0;

    vars->pitch_offset_speed = 0;
    vars->pitch_speed = 0;
    vars->yaw_speed = 0;
    mv->speed = 0;
    vars->pitch_offset = 1024;

    ALUKA_EnableControllers(instance);
    mv->mvFlags |= 0x800;
    ALUKA_AttackEntry(instance);

}

void ALUKA_SurpriseAttack(Instance *instance)
{
    if (!(((MonsterVars *)instance->extraData)->mvFlags & 0x400))
    {
        ALUKA_ResetSwim(instance);
        MON_SurpriseAttack(instance);
        return;
    }
    ALUKA_Attack(instance);
}

void ALUKA_SurprisedEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (mv->extraVars == NULL)
    {
        return;
    }

    if (!(mv->mvFlags & 0x400))
    {
        MON_SurprisedEntry(instance);
    }
    else
    {
        MON_SwitchStateDoEntry(instance, MONSTER_STATE_PURSUE);
    }
}

void ALUKA_Surprised(Instance *instance)
{
    if (!(((MonsterVars *)instance->extraData)->mvFlags & 0x400))
    {
        ALUKA_ResetSwim(instance);
        MON_Surprised(instance);
    }
}

void ALUKA_NoticeEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (mv->extraVars == NULL)
    {
        return;
    }

    if (!(mv->mvFlags & 0x400))
    {
        MON_NoticeEntry(instance);
        return;
    }

    MON_SwitchStateDoEntry(instance, MONSTER_STATE_PURSUE);
}

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

void ALUKA_EmbraceEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukaVars *vars; // not from debug symbols
    AlukaAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;

    vars = (AlukaVars *)mv->extraVars;
    attrs = (AlukaAttributes *)ma->tunData;

    if (vars == NULL || attrs == NULL)
    {
        return;
    }

    if (!(mv->mvFlags & 0x400))
    {
        MON_EmbraceEntry(instance);
        return;
    }

    vars->forward_speed_limit = 0;
    vars->yaw_speed_limit = attrs->swimslow_yaw;
    vars->pitch_speed_limit = attrs->slow_pitch;

}

void ALUKA_Embrace(Instance *instance)
{

    MonsterVars *mv; // not from dedbug symbols
    MonsterAttributes *ma; // not from dedbug symbols
    AlukaVars *vars; // not from dedbug symbols
    Instance *enemyInst; // not from dedbug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (AlukaVars *)mv->extraVars;

    if (vars == NULL)
    {
        return;
    }

    if (!(mv->mvFlags & 0x400))
    {
        ALUKA_ResetSwim(instance);
        MON_Embrace(instance);
        return;
    }

    if (mv->mvFlags & 4)
    {
        MON_SwitchState(instance, MONSTER_STATE_IDLE);
        return;
    }

    mv->auxFlags |= 0x20000000;

    if (vars->swim_anim == ALUKA_ANIM_SWIMSOULSUCK)
    {
        if (mv->enemy != NULL && mv->subAttr->combatAttributes->suckRange >= mv->enemy->distance && mv->enemy->mirFlags & 0x1000)
        {
            enemyInst = mv->enemy->instance;
            MON_DoDrainEffects(instance, enemyInst);
            INSTANCE_Post(enemyInst, 0x40006, mv->subAttr->combatAttributes->suckPower * 256);
            MON_TurnToPosition(instance, &enemyInst->position, mv->subAttr->speedPivotTurn);
        }
        else
        {
            MON_SwitchState(instance, MONSTER_STATE_PURSUE);
        }

        MON_DefaultQueueHandler(instance);
        return;
    }

    if (mv->speed > 0)
    {
        ALUKA_SetupSwimAnimWOTread(instance);
        ALUKA_SwimToDestination(instance);
        return;
    }

    MON_PlayAnimFromList(instance, ma->auxAnimList, ALUKA_ANIM_SWIMSOULSUCK, 2);
    vars->swim_anim = ALUKA_ANIM_SWIMSOULSUCK;
}

void ALUKA_GeneralDeathEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukaVars *vars; // not from debug symbols
    AlukaAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;

    vars = (AlukaVars *)mv->extraVars;
    attrs = (AlukaAttributes *)ma->tunData;

    if (vars == NULL || attrs == NULL)
    {
        return;
    }

    if (!(mv->mvFlags & 0x400))
    {
        MON_GeneralDeathEntry(instance);
        return;
    }

    if (instance->LinkParent != NULL)
    {
        MON_UnlinkFromRaziel(instance);
    }

    instance->xAccl = 0;
    instance->yAccl = 0;

    mv->mvFlags |= 0x202000;
    mv->mvFlags &= ~0x10;

    switch (mv->damageType)
    {
    case 32:
        mv->causeOfDeath = MONSTER_CAUSEOFDEATH_FIRE;
    case 64:
        MON_PlayAnimFromList(instance, ma->auxAnimList, ALUKA_ANIM_SWIMAGONY, 2);
        if (mv->damageType == 64)
        {
            mv->causeOfDeath = MONSTER_CAUSEOFDEATH_SUN;
        }
        mv->generalTimer = MON_GetTime(instance) + 3000;
        mv->mvFlags |= 0x400000;
        mv->effectTimer = MON_GetTime(instance) + 10000;
        MON_MonsterGlow(instance, 18784, -1, 0, 0);
        break;
    case 512:
        MON_PlayAnimFromList(instance, ma->auxAnimList, ALUKA_ANIM_SWIMAGONY, 2);
        mv->causeOfDeath = MONSTER_CAUSEOFDEATH_SOUND;
        mv->generalTimer = MON_GetTime(instance) + 1000;
        break;
    case 1024:
        mv->causeOfDeath = MONSTER_CAUSEOFDEATH_STONE;
        mv->generalTimer = 0;
        break;
    default:
        mv->causeOfDeath = MONSTER_CAUSEOFDEATH_DAMAGE;
        MON_PlayAnimFromList(instance, ma->auxAnimList, ALUKA_ANIM_SWIMDEATH, 1);
        break;
    }

    MON_TurnOffAllSpheres(instance);
    MON_DropAllObjects(instance);
}

void ALUKA_GeneralDeath(Instance *instance)
{

    int setDeadNoAnim; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;

    if (mv->extraVars == NULL)
    {
        return;
    }

    if (!(mv->mvFlags & 0x400))
    {
        ALUKA_ResetSwim(instance);
        MON_GeneralDeath(instance);
        return;
    }

    mv->auxFlags |= 0x20000000;
    setDeadNoAnim = 0;

    if (instance->flags2 & 2 && mv->generalTimer < MON_GetTime(instance))
    {
        if (mv->causeOfDeath == MONSTER_CAUSEOFDEATH_FIRE || mv->causeOfDeath == MONSTER_CAUSEOFDEATH_SUN)
        {
            MON_PlayAnimFromList(instance, ma->auxAnimList, ALUKA_ANIM_SWIMDEATH, 1);
        }
        else
        {
            setDeadNoAnim = 1;
        }
    }

    if (instance->flags2 & 0x10)
    {
        setDeadNoAnim = 1;
    }

    if (setDeadNoAnim || mv->causeOfDeath == MONSTER_CAUSEOFDEATH_STONE)
    {
        mv->mvFlags &= ~0x10;
        G2Anim_SetNoLooping(&instance->anim);
        MON_SwitchState(instance, MONSTER_STATE_DEAD);
    }

    while (DeMessageQueue(&mv->messageQueue) != NULL);
}


#else

G2Bool ALUKA_ControllersEnabled(Instance *instance)
{
    return G2Anim_IsControllerActive(&instance->anim, 1, G2ANIM_CTRLRTYPE_ADD_LOCALTRANS) != G2FALSE;
}

void ALUKA_SetPitch(Instance *instance, int pitch)
{
    G2SVector3 extraRot;
    AlukaAttributes *attributes; // not from debug symbols

    attributes = (AlukaAttributes *)((MonsterVars *)instance->extraData)->extraVars;

    if (ALUKA_ControllersEnabled(instance) == G2FALSE)
    {
        return;
    }

    extraRot.x = pitch;
    extraRot.z = 0;
    extraRot.y = 0;
    G2Anim_SetController_Vector(&instance->anim, 1, G2ANIM_CTRLRTYPE_ADD_LOCALROT, &extraRot);
    attributes->swimfast_pitch = pitch;
}

void ALUKA_EnableControllers(Instance *instance)
{
    G2SVector3 translate;
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (ALUKA_ControllersEnabled(instance) != G2FALSE)
    {
        return;
    }

    G2Anim_EnableController(&instance->anim, 1, G2ANIM_CTRLRTYPE_ADD_LOCALTRANS);
    G2Anim_EnableController(&instance->anim, 1, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_EnableController(&instance->anim, 19, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_EnableController(&instance->anim, 26, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_EnableController(&instance->anim, 32, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_EnableController(&instance->anim, 5, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_EnableController(&instance->anim, 12, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_EnableController(&instance->anim, 4, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_EnableController(&instance->anim, 11, G2ANIM_CTRLRTYPE_ADD_LOCALROT);

    translate.y = 0;
    translate.x = 0;

    if (mv->age != 0)
    {
        translate.z = -450;
        instance->position.z -= translate.z;
    }
    else
    {
        translate.z = -400;
        instance->position.z -= translate.z;
    }

    G2Anim_SetController_Vector(&instance->anim, 1, G2ANIM_CTRLRTYPE_ADD_LOCALTRANS, &translate);
}

void ALUKA_DisableControllers(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (ALUKA_ControllersEnabled(instance) == G2FALSE)
    {
        return;
    }

    G2Anim_DisableController(&instance->anim, 1, G2ANIM_CTRLRTYPE_ADD_LOCALTRANS);
    G2Anim_DisableController(&instance->anim, 1, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DisableController(&instance->anim, 19, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DisableController(&instance->anim, 26, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DisableController(&instance->anim, 32, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DisableController(&instance->anim, 5, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DisableController(&instance->anim, 12, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DisableController(&instance->anim, 4, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DisableController(&instance->anim, 11, G2ANIM_CTRLRTYPE_ADD_LOCALROT);

    if (mv->age != 0)
    {
        instance->position.z -= 450;
    }
    else
    {
        instance->position.z -= 400;
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

    G2Anim_SetController_Vector(&instance->anim, 5, G2ANIM_CTRLRTYPE_ADD_LOCALROT, &extraRot);
    G2Anim_SetController_Vector(&instance->anim, 12, G2ANIM_CTRLRTYPE_ADD_LOCALROT, &extraRot);
    G2Anim_SetController_Vector(&instance->anim, 4, G2ANIM_CTRLRTYPE_ADD_LOCALROT, &extraRot);
    G2Anim_SetController_Vector(&instance->anim, 11, G2ANIM_CTRLRTYPE_ADD_LOCALROT, &extraRot);

    extraRot.y = pitch & 0xFFF;
    extraRot.x = yaw & 0xFFF;

    G2Anim_SetController_Vector(&instance->anim, 19, G2ANIM_CTRLRTYPE_ADD_LOCALROT, &extraRot);

    extraRot.x = -yaw & 0xFFF;

    if (pitch > 0)
    {
        extraRot.y = -(pitch * 2) & 0xFFF;
        G2Anim_SetController_Vector(&instance->anim, 26, G2ANIM_CTRLRTYPE_ADD_LOCALROT, &extraRot);
        extraRot.y = -pitch & 0xFFF;
        G2Anim_SetController_Vector(&instance->anim, 32, G2ANIM_CTRLRTYPE_ADD_LOCALROT, &extraRot);
    }
    else
    {
        extraRot.y = -pitch & 0xFFF;
        G2Anim_SetController_Vector(&instance->anim, 26, G2ANIM_CTRLRTYPE_ADD_LOCALROT, &extraRot);
        extraRot.y = -(pitch * 2) & 0xFFF;
        G2Anim_SetController_Vector(&instance->anim, 32, G2ANIM_CTRLRTYPE_ADD_LOCALROT, &extraRot);
    }

}

int ALUKA_NotDaylight(Level *level)
{

    if (level->unitFlags & 2 && GAMELOOP_GetTimeOfDay() == 700)
    {
        return 0;
    }

    return 1;

}

int ALUKA_CapDepth(Instance *instance, Level *level) {};

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

void ALUKA_VectorFromPitchYaw(Position *vector, int pitch, int yaw, int dist)
{

    MATRIX mat;
    SVECTOR oldVector;
    SVECTOR newVector;

    oldVector.vx = 0;
    oldVector.vy = -dist;
    oldVector.vz = 0;

    mat.m[0][0] = 4096;
    mat.m[0][1] = 0;
    mat.m[0][2] = 0;
    mat.m[1][0] = 0;
    mat.m[1][1] = 4096;
    mat.m[1][2] = 0;
    mat.m[2][0] = 0;
    mat.m[2][1] = 0;
    mat.m[2][2] = 4096;

    RotMatrixX(pitch, &mat);
    RotMatrixZ(yaw, &mat);
    ApplyMatrixSV(&mat, &oldVector, &newVector);

    COPY_SVEC(Position, (Position *)vector, Position, (Position *)&newVector);
}


void ALUKA_FacingVector(Instance *instance, Position *vector, int dist)
{
    MonsterVars *mv; // not from debug symbols
    AlukaAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    attrs = (AlukaAttributes *)mv->extraVars;

    ALUKA_VectorFromPitchYaw(vector, (attrs->swimfast_pitch - attrs->min_swim_depth) & 0xFFF, instance->rotation.z, dist);
}

int ALUKA_SimpleLineCheck(Position *hit, Level *level, Position *start, Position *end)
{

    PCollideInfo info;
    SVECTOR oldPt;
    SVECTOR newPt;

    oldPt.vx = start->x;
    oldPt.vy = start->y;
    oldPt.vz = start->z;

    newPt.vx = end->x;
    newPt.vy = end->y;
    newPt.vz = end->z;

    info.oldPoint = &oldPt;
    info.newPoint = &newPt;

    info.collideType = 3;
    info.instance = NULL;
    info.inst = NULL;

    COLLIDE_PointAndWorld(&info, level);

    if (info.type != 0)
    {
        if (hit != NULL)
        {
            hit->x = newPt.vx;
            hit->y = newPt.vy;
            hit->z = newPt.vz;
        }
        return 1;
    }
    return 0;
}


int ALUKA_TerrainInPath(Instance *instance)
{
    Position vector; // not from debug symbols
    Level *level; // not from debug symbols

    level = STREAM_GetLevelWithID(instance->currentStreamUnitID);

    ALUKA_FacingVector(instance, &vector, 400);
    ADD_SVEC(Position, &vector, Position, &vector, Position, &instance->position);

    return ALUKA_SimpleLineCheck(0, level, &instance->position, &vector);
}

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

void ALUKA_ApplyRots(Instance *instance, int time)
{

    int newDepth; // not from debug symbols
    int newPitch; // not from debug symbols
    int newRot; // not from debug symbols
    int delta; // not from debug symbols
    int pitch; // not from debug symbols
    int swimPitch; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    AlukaVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (AlukaVars *)mv->extraVars;

    newRot = ALUKA_ApplyIncr(instance->rotation.z, -32767, 32767, vars->yaw_speed, time);
    delta = vars->pitch_offset;
    swimPitch = vars->pitch & 0xFFF;

    if (swimPitch <= 2048)
    {
        delta = swimPitch - vars->pitch_offset;
    }
    else
    {
        delta = swimPitch - 4096 - vars->pitch_offset;
    }

    newDepth = ALUKA_ApplyIncr(delta, -682, 682, vars->pitch_speed, time);
    newPitch = ALUKA_ApplyIncr(vars->pitch_offset, 0, 1024, vars->pitch_offset_speed, time);
    pitch = (newDepth + newPitch) & 0xFFF;
    instance->rotation.z = newRot & 0xFFF;
    ALUKA_SetPitch(instance, pitch);
    vars->pitch_offset = newPitch;
}

void ALUKA_MoveForward(Instance *instance, int time, int depth)
{
    Position vector; // not from debug symbols
    int dist; // not from debug symbols

    dist = ((MonsterVars *)instance->extraData)->speed * time;
    ALUKA_FacingVector(instance, &vector, dist / 4096);

    ADD_SVEC(Position, &instance->position, Position, &instance->position, Position, &vector);

    if (depth < instance->position.z)
    {
        instance->position.z = depth;
    }
}

void ALUKA_FixPitch(void) {};

void ALUKA_ProportionalLimitsAndAccels(Instance *instance, int angley, int anglep)
{

    int absYaw; // not from debug symbols
    int absPitch; // not from debug symbols

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukaVars *vars; // not from debug symbols
    AlukaAttributes *attrs; // not from debug symbols


    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (AlukaVars *)mv->extraVars;
    attrs = (AlukaAttributes *)ma->tunData;

    absYaw = abs(angley);
    absPitch = abs(anglep);

    vars->use_yaw_speed_limit = MIN(abs(angley) >> 3, vars->yaw_speed_limit);
    vars->use_pitch_speed_limit = MIN(abs(anglep) >> 3, vars->pitch_speed_limit);

    vars->use_yaw_accel = MIN(MAX(abs(absYaw) >> 1, 1), attrs->yaw_accel);
    vars->use_pitch_accel = MIN(MAX(abs(absPitch) >> 1, 1), attrs->pitch_accel);

}

int ALUKA_FacePoint(Instance *instance, Position *point, int rate)
{
    int newPitch;

    int targetYaw; // not from debug symbols
    int targetPitch; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    AlukaAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    attrs = (AlukaAttributes *)mv->extraVars;

    targetYaw = MATH3D_AngleFromPosToPos(&instance->position, point);
    targetPitch = (MATH3D_ElevationFromPosToPos(&instance->position, point) + attrs->min_swim_depth) & 0xFFF;

    AngleMoveToward(&instance->rotation.z, targetYaw, (rate * gameTrackerX.timeMult) / 4096);

    newPitch = attrs->swimfast_pitch;
    AngleMoveToward((short *)&newPitch, targetPitch, (rate * gameTrackerX.timeMult) / 4096);
    ALUKA_SetPitch(instance, newPitch);

    if (instance->rotation.z == targetYaw && attrs->swimfast_pitch == targetPitch)
    {
        return 1;
    }

    return 0;
}

void ALUKA_SetupSwimAnimWOTread(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukaVars *vars; // not from debug symbols
    AlukaAttributes *alukaAttrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;

    vars = (AlukaVars *)mv->extraVars;
    alukaAttrs = (AlukaAttributes *)ma->tunData;

    if (vars->swim_anim == ALUKA_ANIM_SWIMATTACK || vars->swim_anim == ALUKA_ANIM_SWIMSPIT)
    {
        return;
    }

    if (mv->speed < alukaAttrs->swimfast_speed)
    {
        if (vars->swim_anim != ALUKA_ANIM_SWIMSLOW)
        {
            MON_PlayAnimFromList(instance, ma->auxAnimList, ALUKA_ANIM_SWIMSLOW, 2);
            vars->swim_anim = ALUKA_ANIM_SWIMSLOW;

            if (vars->pitch_offset < 1024)
            {
                vars->pitch_offset_speed = alukaAttrs->pitch_offset_speed;
            }
            else
            {
                vars->pitch_offset_speed = 0;
            }
        }
    }
    else if (vars->swim_anim != ALUKA_ANIM_SWIMFAST)
    {

        MON_PlayAnimFromList(instance, ma->auxAnimList, ALUKA_ANIM_SWIMFAST, 2);
        vars->swim_anim = ALUKA_ANIM_SWIMFAST;

        if (vars->pitch_offset < 1024)
        {
            vars->pitch_offset_speed = alukaAttrs->pitch_offset_speed;
        }
        else
        {
            vars->pitch_offset_speed = 0;
        }
    }
}

void ALUKA_SetupSwimAnimWTread(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukaVars *vars; // not from debug symbols
    AlukaAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;

    vars = (AlukaVars *)mv->extraVars;
    attrs = (AlukaAttributes *)ma->tunData;

    if (vars->swim_anim == ALUKA_ANIM_SWIMATTACK || vars->swim_anim == ALUKA_ANIM_SWIMSPIT)
    {
        return;
    }

    if (mv->speed < attrs->swimslow_speed)
    {
        if (vars->swim_anim != ALUKA_ANIM_TREAD)
        {
            MON_PlayAnimFromList(instance, ma->auxAnimList, ALUKA_ANIM_TREAD, 2);
            vars->swim_anim = ALUKA_ANIM_TREAD;
            if (vars->pitch_offset > 0)
            {
                vars->pitch_offset_speed = -attrs->pitch_offset_speed;
            }
            else
            {
                vars->pitch_offset_speed = 0;
            }
        }
    }
    else
    {
        ALUKA_SetupSwimAnimWOTread(instance);
    }
}


void ALUKA_SwimToDestination(Instance *) {};

void ALUKA_GetRandomDestination(Instance *instance, Position *destination, Position *start, int range) {};

int ALUKA_NearAluka(Instance *instance)
{

    int dist; // not from debug symbols
    int oldDist; // not from debug symbols
    Instance *inst; // not from debug symbols

    for (inst = gameTrackerX.instanceList->first; inst != NULL; inst = inst->next)
    {

        if (inst == instance || INSTANCE_Query(inst, queryWhatAmI) != 130)
        {
            continue;
        }

        dist = MATH3D_LengthXYZ(instance->position.x - inst->position.x, instance->position.y - inst->position.y, instance->position.z - inst->position.z);
        oldDist = MATH3D_LengthXYZ(instance->oldPos.x - inst->oldPos.x, instance->oldPos.y - inst->oldPos.y, instance->oldPos.z - inst->oldPos.z);

        if (dist < 640 && dist < oldDist)
        {
            return 1;
        }
    }

    return 0;
}


void ALUKA_GetCircleDestination(void) {};

int ALUKA_SwimPlanMovement(Instance *instance, Position *target, Position *step, Level *level)
{

    int dist; // not from debug symbols
    int depth; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukaAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    attrs = (AlukaAttributes *)ma->tunData;

    dist = MATH3D_LengthXYZ(instance->position.x - target->x, instance->position.y - target->y, instance->position.z - target->z);
    depth = ALUKA_CapDepth(instance, level);

    COPY_SVEC(Position, step, Position, target);

    if (attrs->circle_start_dist < dist || (mv->age != 0 && depth < target->z && ALUKA_NotDaylight(level)))
    {
        switch (ENMYPLAN_MoveToTargetFinal(instance, step, mv->pathSlotID, target, 0x1E00001F))
        {
        case 0:
            if (attrs->circle_start_dist < dist)
            {
                return 2;
            }
            return 0;
        case 3:
            MON_GetPlanSlot(mv);
            if (attrs->circle_start_dist >= dist)
            {
                return 0;
            }
            return 3;
        default:
            return 1;
        }
    }

    return 0;
}


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
    instance->maxXVel = 400;
    instance->maxYVel = 400;
    instance->maxZVel = 400;

}

int ALUKA_ShouldJumpIn(Instance *instance, Instance *enemy, Level *level)
{
    int nodeType; // not from debug symbols

    (void)enemy;
    (void)level;

    nodeType = ENMYPLAN_GetNodeTypeOfNextWaypoint(((MonsterVars *)instance->extraData)->pathSlotID);

    if (nodeType != 64 && ((nodeType >> 3) & 3) == 3)
    {
        return 1;
    }

    return 0;
}


int ALUKA_ShouldJumpOut(Instance *instance, Instance *enemy, Level *level)
{

    int nodeType; // not from debug symbols
    MonsterVars *mv; // not from debug symbols

    (void)enemy;

    mv = (MonsterVars *)instance->extraData;

    if (mv->age == 0 || !ALUKA_NotDaylight(level))
    {
        return 0;
    }

    nodeType = ENMYPLAN_GetNodeTypeOfNextWaypoint(mv->pathSlotID);

    if (nodeType != 64 && ((nodeType >> 3) & 3) == 0)
    {
        return 1;
    }

    return 0;
}

void ALUKA_JumpToEntry(Instance *instance, Position *target)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukaVars *vars; // not from debug symbols
    AlukaAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (AlukaVars *)mv->extraVars;
    attrs = (AlukaAttributes *)ma->tunData;

    mv->auxFlags |= 1;

    if (mv->auxFlags & 2)
    {
        MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, ALUKA_ANIM_JUMPIN, 0);
    }
    else
    {
        vars->forward_speed_limit = 0;
        vars->yaw_speed_limit = attrs->swimslow_yaw;
        vars->pitch_speed_limit = attrs->slow_pitch;
        mv->auxFlags &= ~0x10;
    }

    COPY_SVEC(Position, &vars->target, Position, target);

    mv->auxFlags &= ~4;
    mv->mvFlags |= 0x800;

    instance->xAccl = 0;
    instance->yAccl = 0;
    instance->zAccl = -16;

    instance->maxXVel = 65535;
    instance->maxYVel = 65535;
    instance->maxZVel = 65535;

    mv->mode = 0x100000;
}

void ALUKA_SetJumpVels(void) {};

void ALUKA_SetHitGround(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    mv->auxFlags = mv->auxFlags & ~1;
    mv->mvFlags = mv->mvFlags & ~0x800;

    instance->zVel = 0;
    instance->maxXVel = 400;
    instance->maxYVel = 400;
    instance->maxZVel = 400;

    MON_SwitchState(instance, MONSTER_STATE_LANDONFEET);
}

void ALUKA_JumpTo(void) {};

void ALUKA_ChooseAttack(void) {};


void ALUKA_Init(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    AlukaVars *vars; // not from debug symbols

    MON_DefaultInit(instance);

    mv = (MonsterVars *)instance->extraData;

    if (mv != NULL)
    {

        vars = (AlukaVars *)MEMPACK_Malloc(sizeof(AlukaVars), 27);

        if (vars == NULL)
        {
            mv->extraVars = NULL;
        }
        else
        {
            mv->extraVars = vars;
            vars->pitch = 0;
        }
    }

    instance->rotation.x = 0;

    G2Anim_AttachControllerToSeg(&instance->anim, 1, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_AttachControllerToSeg(&instance->anim, 1, G2ANIM_CTRLRTYPE_ADD_LOCALTRANS);
    G2Anim_AttachControllerToSeg(&instance->anim, 19, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_AttachControllerToSeg(&instance->anim, 26, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_AttachControllerToSeg(&instance->anim, 32, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_AttachControllerToSeg(&instance->anim, 5, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_AttachControllerToSeg(&instance->anim, 12, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_AttachControllerToSeg(&instance->anim, 4, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_AttachControllerToSeg(&instance->anim, 11, G2ANIM_CTRLRTYPE_ADD_LOCALROT);

    G2Anim_DisableController(&instance->anim, 1, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DisableController(&instance->anim, 1, G2ANIM_CTRLRTYPE_ADD_LOCALTRANS);
    G2Anim_DisableController(&instance->anim, 19, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DisableController(&instance->anim, 26, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DisableController(&instance->anim, 32, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DisableController(&instance->anim, 5, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DisableController(&instance->anim, 12, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DisableController(&instance->anim, 4, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DisableController(&instance->anim, 11, G2ANIM_CTRLRTYPE_ADD_LOCALROT);

    DEBUG_DoAreaProtection();
}

void ALUKA_CleanUp(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    G2Anim_DetachControllerFromSeg(&instance->anim, 1, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DetachControllerFromSeg(&instance->anim, 1, G2ANIM_CTRLRTYPE_ADD_LOCALTRANS);
    G2Anim_DetachControllerFromSeg(&instance->anim, 19, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DetachControllerFromSeg(&instance->anim, 26, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DetachControllerFromSeg(&instance->anim, 32, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DetachControllerFromSeg(&instance->anim, 5, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DetachControllerFromSeg(&instance->anim, 12, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DetachControllerFromSeg(&instance->anim, 4, G2ANIM_CTRLRTYPE_ADD_LOCALROT);
    G2Anim_DetachControllerFromSeg(&instance->anim, 11, G2ANIM_CTRLRTYPE_ADD_LOCALROT);

    mv = (MonsterVars *)instance->extraData;

    if (mv != NULL && mv->extraVars != NULL)
    {
        MEMPACK_Free(mv->extraVars);
    }

    MON_CleanUp(instance);
}

void ALUKA_Message(void) {};

void ALUKA_PursueEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukaVars *vars; // not from debug symbols
    AlukaAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (AlukaVars *)mv->extraVars;
    attrs = (AlukaAttributes *)ma->tunData;

    if (vars == NULL)
    {
        return;
    }

    if (!(mv->mvFlags & 0x400))
    {
        MON_PursueEntry(instance);
        return;
    }

    if (ALUKA_ControllersEnabled(instance) == G2FALSE)
    {
        mv->speed = 0;
        vars->pitch_offset = 1024;
        vars->yaw_speed = 0;
        vars->pitch_speed = 0;
        vars->pitch_offset_speed = 0;
        vars->swim_anim = ALUKA_ANIM_NO_ANIM;
        ALUKA_EnableControllers(instance);
    }

    MON_GetPlanSlot(mv);
    vars->last_pm = 0;
    vars->special_time = MON_GetTime(instance) + (attrs->too_long_since_attack * 990);
    mv->mode = 4;
}

void ALUKA_Pursue(void) {};

void ALUKA_AttackEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    AlukaVars *vars; // not from debug symbols
    AlukaAttributes *attrs; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    MonsterCombatAttributes *combat; // not from debug symbols
    int attack; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (AlukaVars *)mv->extraVars;
    attrs = (AlukaAttributes *)ma->tunData;

    if (vars == NULL)
    {
        return;
    }

    if (!(mv->mvFlags & 0x400))
    {
        MON_AttackEntry(instance);
        return;
    }


    combat = mv->subAttr->combatAttributes;

    /*
     The pointer manipulation here is because of the fact that there is no field
     at offset 0x2B. It's likely that the intent was to access 0x2C, which is
     the `attackList` field.
    */
    attack = combat->numAttacks + 0x2B;
    mv->attackType = &ma->attackAttributesList[*((signed char *)combat + attack)];

    mv->attackState = 0;

    vars->forward_speed_limit = attrs->swimattack_speed;
    vars->yaw_speed_limit = attrs->swimattack_yaw;
    vars->pitch_speed_limit = attrs->swimattack_pitch;
}

void ALUKA_Attack(Instance *instance)
{

    int destinationInRange; // not from debug symbols
    MonsterIR *enemy; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukaVars *vars; // not from debug symbols
    AlukaAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    attrs = (AlukaAttributes *)ma->tunData;
    vars = (AlukaVars *)mv->extraVars;
    enemy = mv->enemy;

    if (vars == NULL)
    {
        return;
    }

    if (!(mv->mvFlags & 0x400))
    {
        ALUKA_ResetSwim(instance);
        MON_Attack(instance);
        return;
    }

    if (mv->mvFlags & 4)
    {
        MON_SwitchState(instance, MONSTER_STATE_IDLE);
        return;
    }

    destinationInRange = 0;
    mv->auxFlags |= 0x20000000;

    if (enemy != NULL)
    {
        COPY_SVEC(Position, &mv->destination, Position, &enemy->instance->position);
        mv->destination.z += attrs->attack_pos_bump;
        destinationInRange = ALUKA_CapDepth(instance, STREAM_GetLevelWithID(instance->currentStreamUnitID)) > mv->destination.z;
    }

    if (destinationInRange)
    {
        switch (mv->attackState)
        {
        case 0:
            if (MATH3D_LengthXYZ(mv->destination.x - instance->position.x, mv->destination.y - instance->position.y, mv->destination.z - instance->position.z) < attrs->attack_dist)
            {
                mv->attackState++;
            }
            break;
        case 1:
            MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, ALUKA_ANIM_SWIMATTACK, 1);
            vars->swim_anim = ALUKA_ANIM_SWIMATTACK;
            mv->attackState++;
            vars->special_time = MON_GetTime(instance) + (mv->attackType->sphereOnFrame * 33);
            break;
        case 2:
            if (MON_GetTime(instance) >= (unsigned long)vars->special_time)
            {
                MON_TurnOnWeaponSpheres(instance);
                mv->attackState++;
                vars->special_time = MON_GetTime(instance) + (mv->attackType->sphereOffFrame * 33);
            }
            break;
        case 3:
            if (MON_GetTime(instance) >= (unsigned long)vars->special_time)
            {
                MON_TurnOffWeaponSpheres(instance);
                mv->attackState++;
            }
            break;
        case 4:
            if (instance->flags2 & 0x10)
            {

                vars->swim_anim = ALUKA_ANIM_NO_ANIM;
                mv->attackState = 0;

                if (mv->mvFlags & 0x10000000 && mv->enemy->mirFlags & 0x1000)
                {
                    MON_SwitchState(instance, MONSTER_STATE_EMBRACE);
                }
                else
                {
                    MON_SwitchState(instance, MONSTER_STATE_PURSUE);
                }
            }
            break;
        }
    }
    else
    {
        MON_TurnOffWeaponSpheres(instance);
        MON_SwitchState(instance, MONSTER_STATE_IDLE);
    }

    ALUKA_SetupSwimAnimWOTread(instance);
    ALUKA_SwimToDestination(instance);
}

void ALUKA_FleeEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukaVars *vars; // not from debug symbols
    AlukaAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;

    vars = (AlukaVars *)mv->extraVars;
    attrs = (AlukaAttributes *)ma->tunData;

    if (!(mv->mvFlags & 0x400))
    {
        MON_FleeEntry(instance);
        return;
    }

    vars->forward_speed_limit = attrs->swimfast_speed;
    vars->yaw_speed_limit = attrs->swimfast_yaw;
    vars->pitch_speed_limit = attrs->swimfast_pitch;
    mv->mode = 4;
}

void ALUKA_Flee(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (!(mv->mvFlags & 0x400))
    {
        ALUKA_ResetSwim(instance);
        MON_Flee(instance);
        return;
    }

    mv->auxFlags |= 0x20000000;

    ALUKA_SetupSwimAnimWOTread(instance);
    ALUKA_SwimToDestination(instance);
    MON_SwitchState(instance, MONSTER_STATE_IDLE);
}

void ALUKA_WanderEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukaVars *vars; // not from debug symbols
    AlukaAttributes *attrs; // not from debug symbols
    int range; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;

    vars = (AlukaVars *)mv->extraVars;
    attrs = (AlukaAttributes *)ma->tunData;

    if (vars == NULL || attrs == NULL)
    {
        return;
    }

    if (!(mv->mvFlags & 0x400))
    {
        MON_WanderEntry(instance);
        return;
    }


    if (mv->wanderRange != 0)
    {
        range = mv->wanderRange;
    }
    else
    {
        range = attrs->wander_range;
    }

    ALUKA_GetRandomDestination(instance, &mv->destination, &instance->intro->position, range);

    vars->forward_speed_limit = attrs->swimslow_speed;
    vars->yaw_speed_limit = attrs->swimslow_yaw;
    vars->pitch_speed_limit = attrs->slow_pitch;

}

void ALUKA_Wander(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukaAttributes *attrs; // not from debug symbols
    int range; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    attrs = (AlukaAttributes *)ma->tunData;

    if (attrs == NULL)
    {
        return;
    }

    if (!(mv->mvFlags & 0x400))
    {
        ALUKA_ResetSwim(instance);
        MON_Wander(instance);
        return;
    }

    if (mv->mvFlags & 4)
    {
        MON_SwitchState(instance, MONSTER_STATE_IDLE);
        return;
    }

    mv->auxFlags |= 0x20000000;

    if (mv->enemy != NULL)
    {
        MON_SwitchState(instance, MONSTER_STATE_PURSUE);
    }
    else if (rand() < 163)
    {
        MON_SwitchState(instance, MONSTER_STATE_IDLE);
    }

    if (mv->wanderRange != 0)
    {
        range = mv->wanderRange;
    }
    else
    {
        range = attrs->wander_range;
    }

    if (MATH3D_LengthXYZ(instance->position.x - mv->destination.x, instance->position.y - mv->destination.y, instance->position.z - mv->destination.z) < 640)
    {
        ALUKA_GetRandomDestination(instance, &mv->destination, &instance->intro->position, range);
    }

    ALUKA_SetupSwimAnimWOTread(instance);
    ALUKA_SwimToDestination(instance);
}

void ALUKA_IdleEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukaVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (AlukaVars *)mv->extraVars;

    if (vars == NULL)
    {
        return;
    }

    if (!(mv->mvFlags & 0x400))
    {
        MON_IdleEntry(instance);
        return;
    }

    vars->forward_speed_limit = 0;
    vars->yaw_speed_limit = 0;
    vars->pitch_speed_limit = 0;

    ma = (MonsterAttributes *)instance->data;
    MON_PlayAnimFromList(instance, ma->auxAnimList, ALUKA_ANIM_SWIMIDLE, 2);
    vars->swim_anim = ALUKA_ANIM_SWIMIDLE;
    mv->mode = 1;

}

void ALUKA_Idle(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (!(mv->mvFlags & 0x400))
    {
        ALUKA_ResetSwim(instance);
        MON_Idle(instance);
        return;
    }

    if (!(mv->mvFlags & 4))
    {
        mv->auxFlags |= 0x20000000;
        if (mv->enemy != NULL)
        {
            MON_SwitchState(instance, MONSTER_STATE_PURSUE);
        }
        else if (instance->flags2 & 0x12 && rand() < 10922)
        {
            MON_SwitchState(instance, MONSTER_STATE_WANDER);
        }
    }

    ALUKA_SwimToDestination(instance);
}

void ALUKA_LandInWaterEntry(void) {};

void ALUKA_LandInWater(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    AlukaVars *vars; // not from debug symbols
    Level *level; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (AlukaVars *)mv->extraVars;
    level = STREAM_GetLevelWithID(instance->currentStreamUnitID);

    if (vars == NULL)
    {
        return;
    }

    vars->pitch_offset_speed = 103;
    PhysicsMove(instance, &instance->position, gameTrackerX.timeMult);
    ALUKA_ApplyRots(instance, gameTrackerX.timeMult);

    if (ALUKA_CapDepth(instance, level) >= instance->position.z && vars->pitch_offset == 1024)
    {
        instance->zVel = 0;
        vars->pitch_offset_speed = 0;
        vars->pitch_speed = 0;
        MON_SwitchState(instance, MONSTER_STATE_PURSUE);
        vars->swim_anim = ALUKA_ANIM_NO_ANIM;
    }

    MON_DefaultQueueHandler(instance);
}

void ALUKA_HitEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    MonsterIR *enemy; // not from debug symbols
    AlukaVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (AlukaVars *)mv->extraVars;

    if (vars == NULL)
    {
        return;
    }

    if (!(mv->mvFlags & 0x400))
    {
        MON_HitEntry(instance);
        return;
    }

    enemy = mv->enemy;
    enemy->mirConditions |= 0x400;
    enemy->mirFlags &= ~0x1000;
    mv->mvFlags |= 0x10000;

    if (MON_SetUpKnockBack(instance, enemy->instance, (evMonsterHitData *)mv->messageData))
    {
        MON_PlayAnimFromList(instance, ma->auxAnimList, ALUKA_ANIM_SWIMHIT, 1);
    }

    vars->swim_anim = ALUKA_ANIM_NO_ANIM;
    mv->mode = 0x8000;
}

void ALUKA_Hit(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (mv->extraVars == NULL)
    {
        return;
    }

    if (!(mv->mvFlags & 0x400))
    {
        ALUKA_ResetSwim(instance);
        MON_Hit(instance);
        return;
    }

    mv->auxFlags |= 0x20000000;

    if (mv->generalTimer < MON_GetTime(instance))
    {
        if (mv->mvFlags & 0x100)
        {
            MON_SwitchState(instance, MONSTER_STATE_STUNNED);
        }
        else
        {
            MON_SwitchState(instance, MONSTER_STATE_PURSUE);
        }

        instance->xVel = 0;
        instance->yVel = 0;
        instance->zVel = 0;

    }

    MON_DefaultQueueHandler(instance);
    PHYSICS_StopIfCloseToTarget(instance, 0, 0, 0);
    PhysicsMove(instance, &instance->position, gameTrackerX.timeMult);
}

void ALUKA_StunnedEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukaVars *vars; // not from debug symbols
    MonsterCombatAttributes *combat; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (AlukaVars *)mv->extraVars;
    combat = mv->subAttr->combatAttributes;

    if (vars == NULL)
    {
        return;
    }

    if (!(mv->mvFlags & 0x400))
    {
        MON_StunnedEntry(instance);
        return;
    }

    if (mv->damageType == 64)
    {
        MON_PlayAnimFromList(instance, ma->auxAnimList, ALUKA_ANIM_SWIMAGONY, 2);
        vars->swim_anim = ALUKA_ANIM_SWIMAGONY;
    }
    else
    {
        MON_PlayAnimFromList(instance, ma->auxAnimList, ALUKA_ANIM_SWIMSTUN, 2);
        vars->swim_anim = ALUKA_ANIM_SWIMSTUN;
    }

    mv->mvFlags |= 0x100;
    mv->damageTimer = MON_GetTime(instance) + combat->damageTime;
    mv->stunTimer = MON_GetTime(instance) + combat->stunTime;
    mv->mode = 0x8000;

    vars->forward_speed_limit = 0;
    vars->yaw_speed_limit = 0;
    vars->pitch_speed_limit = 0;
}

void ALUKA_Stunned(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukaVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (AlukaVars *)mv->extraVars;

    if (vars == NULL)
    {
        return;
    }

    if (!(mv->mvFlags & 0x400))
    {
        ALUKA_ResetSwim(instance);
        MON_Stunned(instance);
        return;
    }

    mv->auxFlags |= 0x20000000;

    if (vars->swim_anim == ALUKA_ANIM_SWIMRECOVER)
    {
        if (!(mv->mvFlags & 0x100) && instance->flags2 & 0x10)
        {
            MON_SwitchState(instance, MONSTER_STATE_PURSUE);
        }
    }
    else if (vars->swim_anim == ALUKA_ANIM_SWIMSTUN)
    {
        if (!(mv->mvFlags & 0x100))
        {
            MON_PlayAnimFromList(instance, ma->auxAnimList, ALUKA_ANIM_SWIMRECOVER, 1);
            vars->swim_anim = ALUKA_ANIM_SWIMRECOVER;
        }
    }
    else
    {
        MON_SwitchState(instance, MONSTER_STATE_PURSUE);
    }

    ALUKA_SwimToDestination(instance);
}

void ALUKA_ProjectileEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukaVars *vars; // not from debug symbols
    AlukaAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (AlukaVars *)mv->extraVars;
    attrs = (AlukaAttributes *)ma->tunData;

    if (vars == NULL)
    {
        return;
    }

    if (!(mv->mvFlags & 0x400))
    {
        MON_ProjectileEntry(instance);
        return;
    }

    mv->attackState = 0;
    vars->forward_speed_limit = 0;
    vars->yaw_speed_limit = attrs->swimspit_yaw;
    vars->pitch_speed_limit = attrs->swimspit_pitch;
}


void ALUKA_Projectile(Instance *instance)
{

    MonsterIR *enemy; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    AlukaVars *vars; // not from debug symbols
    AlukaAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (AlukaVars *)mv->extraVars;
    attrs = (AlukaAttributes *)ma->tunData;

    if (vars == NULL)
    {
        return;
    }

    if (!(mv->mvFlags & 0x400))
    {
        ALUKA_ResetSwim(instance);
        MON_Projectile(instance);
        return;
    }

    if (mv->mvFlags & 4)
    {
        MON_SwitchState(instance, MONSTER_STATE_IDLE);
        return;
    }

    enemy = mv->enemy;
    mv->auxFlags |= 0x20000000;

    if (enemy != NULL)
    {

        Instance *enemyInst; // not from debug symbols
        Position *enemyPos; // not from debug symbols

        enemyInst = enemy->instance;
        enemyPos = &enemyInst->position;

        COPY_SVEC(Position, &mv->destination, Position, enemyPos);

        mv->destination.z += attrs->attack_pos_bump;

        switch (mv->attackState)
        {
        case 0:
            if (vars->pitch_offset <= 0)
            {

                Position delta; // not from debug symbols
                Position facing; // not from debug symbols

                delta.x = enemyPos->x - instance->position.x;
                delta.y = enemyPos->y - instance->position.y;
                delta.z = 0;

                ALUKA_FacingVector(instance, &facing, 4096);
                facing.z = 0;
                if (!ALUKA_AngleTooWide(&facing, &delta, 1008, 10))
                {
                    MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, ALUKA_ANIM_SWIMSPIT, 1);
                    vars->swim_anim = ALUKA_ANIM_SWIMSPIT;
                    mv->attackState++;
                    vars->special_time = MON_GetTime(instance) + ((unsigned char)attrs->spit_attack_frame * 33);
                }
            }
            break;
        case 1:
            if (MON_GetTime(instance) >= (unsigned long)vars->special_time)
            {
                MISSILE_FireAtInstance(instance, &ma->missileList[(int)mv->subAttr->combatAttributes->missileAttack], mv->enemy->instance);
                mv->attackState++;
            }
            break;
        case 2:
            if (instance->flags2 & 0x10)
            {
                vars->swim_anim = ALUKA_ANIM_NO_ANIM;
                MON_SwitchState(instance, MONSTER_STATE_PURSUE);
            }
            break;
        }
    }
    else
    {
        MON_SwitchState(instance, MONSTER_STATE_IDLE);
    }

    ALUKA_SetupSwimAnimWTread(instance);
    ALUKA_SwimToDestination(instance);
}

void ALUKA_SurpriseAttackEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    AlukaVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (AlukaVars *)mv->extraVars;

    if (vars == NULL)
    {
        return;
    }

    if (!(mv->mvFlags & 0x400))
    {
        MON_SurpriseAttackEntry(instance);
        return;
    }

    instance->zAccl = 0;
    instance->yAccl = 0;
    instance->xAccl = 0;

    instance->zVel = 0;
    instance->yVel = 0;
    instance->xVel = 0;

    vars->pitch_offset_speed = 0;
    vars->pitch_speed = 0;
    vars->yaw_speed = 0;
    mv->speed = 0;
    vars->pitch_offset = 1024;

    ALUKA_EnableControllers(instance);
    mv->mvFlags |= 0x800;
    ALUKA_AttackEntry(instance);

}

void ALUKA_SurpriseAttack(Instance *instance)
{
    if (!(((MonsterVars *)instance->extraData)->mvFlags & 0x400))
    {
        ALUKA_ResetSwim(instance);
        MON_SurpriseAttack(instance);
        return;
    }
    ALUKA_Attack(instance);
}

void ALUKA_SurprisedEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (mv->extraVars == NULL)
    {
        return;
    }

    if (!(mv->mvFlags & 0x400))
    {
        MON_SurprisedEntry(instance);
    }
    else
    {
        MON_SwitchStateDoEntry(instance, MONSTER_STATE_PURSUE);
    }
}

void ALUKA_Surprised(Instance *instance)
{
    if (!(((MonsterVars *)instance->extraData)->mvFlags & 0x400))
    {
        ALUKA_ResetSwim(instance);
        MON_Surprised(instance);
    }
}

void ALUKA_NoticeEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (mv->extraVars == NULL)
    {
        return;
    }

    if (!(mv->mvFlags & 0x400))
    {
        MON_NoticeEntry(instance);
        return;
    }

    MON_SwitchStateDoEntry(instance, MONSTER_STATE_PURSUE);
}

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

void ALUKA_EmbraceEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukaVars *vars; // not from debug symbols
    AlukaAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;

    vars = (AlukaVars *)mv->extraVars;
    attrs = (AlukaAttributes *)ma->tunData;

    if (vars == NULL || attrs == NULL)
    {
        return;
    }

    if (!(mv->mvFlags & 0x400))
    {
        MON_EmbraceEntry(instance);
        return;
    }

    vars->forward_speed_limit = 0;
    vars->yaw_speed_limit = attrs->swimslow_yaw;
    vars->pitch_speed_limit = attrs->slow_pitch;

}

void ALUKA_Embrace(Instance *instance)
{

    MonsterVars *mv; // not from dedbug symbols
    MonsterAttributes *ma; // not from dedbug symbols
    AlukaVars *vars; // not from dedbug symbols
    Instance *enemyInst; // not from dedbug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (AlukaVars *)mv->extraVars;

    if (vars == NULL)
    {
        return;
    }

    if (!(mv->mvFlags & 0x400))
    {
        ALUKA_ResetSwim(instance);
        MON_Embrace(instance);
        return;
    }

    if (mv->mvFlags & 4)
    {
        MON_SwitchState(instance, MONSTER_STATE_IDLE);
        return;
    }

    mv->auxFlags |= 0x20000000;

    if (vars->swim_anim == ALUKA_ANIM_SWIMSOULSUCK)
    {
        if (mv->enemy != NULL && mv->subAttr->combatAttributes->suckRange >= mv->enemy->distance && mv->enemy->mirFlags & 0x1000)
        {
            enemyInst = mv->enemy->instance;
            MON_DoDrainEffects(instance, enemyInst);
            INSTANCE_Post(enemyInst, 0x40006, mv->subAttr->combatAttributes->suckPower * 256);
            MON_TurnToPosition(instance, &enemyInst->position, mv->subAttr->speedPivotTurn);
        }
        else
        {
            MON_SwitchState(instance, MONSTER_STATE_PURSUE);
        }

        MON_DefaultQueueHandler(instance);
        return;
    }

    if (mv->speed > 0)
    {
        ALUKA_SetupSwimAnimWOTread(instance);
        ALUKA_SwimToDestination(instance);
        return;
    }

    MON_PlayAnimFromList(instance, ma->auxAnimList, ALUKA_ANIM_SWIMSOULSUCK, 2);
    vars->swim_anim = ALUKA_ANIM_SWIMSOULSUCK;
}

void ALUKA_GeneralDeathEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    AlukaVars *vars; // not from debug symbols
    AlukaAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;

    vars = (AlukaVars *)mv->extraVars;
    attrs = (AlukaAttributes *)ma->tunData;

    if (vars == NULL || attrs == NULL)
    {
        return;
    }

    if (!(mv->mvFlags & 0x400))
    {
        MON_GeneralDeathEntry(instance);
        return;
    }

    if (instance->LinkParent != NULL)
    {
        MON_UnlinkFromRaziel(instance);
    }

    instance->xAccl = 0;
    instance->yAccl = 0;

    mv->mvFlags |= 0x202000;
    mv->mvFlags &= ~0x10;

    switch (mv->damageType)
    {
    case 32:
        mv->causeOfDeath = MONSTER_CAUSEOFDEATH_FIRE;
    case 64:
        MON_PlayAnimFromList(instance, ma->auxAnimList, ALUKA_ANIM_SWIMAGONY, 2);
        if (mv->damageType == 64)
        {
            mv->causeOfDeath = MONSTER_CAUSEOFDEATH_SUN;
        }
        mv->generalTimer = MON_GetTime(instance) + 3000;
        mv->mvFlags |= 0x400000;
        mv->effectTimer = MON_GetTime(instance) + 10000;
        MON_MonsterGlow(instance, 19784, -1, 0, 0);
        break;
    case 512:
        MON_PlayAnimFromList(instance, ma->auxAnimList, ALUKA_ANIM_SWIMAGONY, 2);
        mv->causeOfDeath = MONSTER_CAUSEOFDEATH_SOUND;
        mv->generalTimer = MON_GetTime(instance) + 1000;
        break;
    case 1024:
        mv->causeOfDeath = MONSTER_CAUSEOFDEATH_STONE;
        mv->generalTimer = 0;
        break;
    default:
        mv->causeOfDeath = MONSTER_CAUSEOFDEATH_DAMAGE;
        MON_PlayAnimFromList(instance, ma->auxAnimList, ALUKA_ANIM_SWIMDEATH, 1);
        break;
    }

    MON_TurnOffAllSpheres(instance);
    MON_DropAllObjects(instance);
}

void ALUKA_GeneralDeath(Instance *instance)
{

    int setDeadNoAnim; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;

    if (mv->extraVars == NULL)
    {
        return;
    }

    if (!(mv->mvFlags & 0x400))
    {
        ALUKA_ResetSwim(instance);
        MON_GeneralDeath(instance);
        return;
    }

    mv->auxFlags |= 0x20000000;
    setDeadNoAnim = 0;

    if (instance->flags2 & 2 && mv->generalTimer < MON_GetTime(instance))
    {
        if (mv->causeOfDeath == MONSTER_CAUSEOFDEATH_FIRE || mv->causeOfDeath == MONSTER_CAUSEOFDEATH_SUN)
        {
            MON_PlayAnimFromList(instance, ma->auxAnimList, ALUKA_ANIM_SWIMDEATH, 1);
        }
        else
        {
            setDeadNoAnim = 1;
        }
    }

    if (instance->flags2 & 0x10)
    {
        setDeadNoAnim = 1;
    }

    if (setDeadNoAnim || mv->causeOfDeath == MONSTER_CAUSEOFDEATH_STONE)
    {
        mv->mvFlags &= ~0x10;
        G2Anim_SetNoLooping(&instance->anim);
        MON_SwitchState(instance, MONSTER_STATE_DEAD);
    }

    while (DeMessageQueue(&mv->messageQueue) != NULL);
}


#endif
