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
#include "Game/MONSTER/MONAPI.h"
#include "Game/MONSTER/MONSENSE.h"
#include "Game/PHYSICS.h"
#include "Game/PLAN/ENMYPLAN.h"
#include "Game/PLAN/PLANAPI.h"
#include "Game/STATE.h"
#include "Game/STREAM.h"
#include "Game/SAVEINFO.h"
#include "Overlays/aluka/aluka.h"

// TODO: These can be deleted once these functions are matched
void ALUKA_SwimToDestination(Instance *instance); 
void ALUKA_FixPitch(Instance *instance, int depth);
void ALUKA_GetRandomDestination(Instance *instance, Position *destination, Position *start, int range);
int ALUKA_CapDepth(Instance *instance, Level *level);

MonsterStateChoice ALUKA_StateChoiceTable[] = 
{
    {MONSTER_STATE_ATTACK, {ALUKA_AttackEntry, ALUKA_Attack}},
    {MONSTER_STATE_PURSUE, {ALUKA_PursueEntry, ALUKA_Pursue}},
    {MONSTER_STATE_IDLE, {ALUKA_IdleEntry, ALUKA_Idle}},
    {MONSTER_STATE_WANDER, {ALUKA_WanderEntry, ALUKA_Wander}},
    {MONSTER_STATE_LANDINWATER, {ALUKA_LandInWaterEntry, ALUKA_LandInWater}},
    {MONSTER_STATE_FLEE, {ALUKA_FleeEntry, ALUKA_Flee}},
    {MONSTER_STATE_HIT, {ALUKA_HitEntry, ALUKA_Hit}},
    {MONSTER_STATE_PROJECTILE, {ALUKA_ProjectileEntry, ALUKA_Projectile}},
    {MONSTER_STATE_STUNNED, {ALUKA_StunnedEntry, ALUKA_Stunned}},
    {MONSTER_STATE_SURPRISEATTACK, {ALUKA_SurpriseAttackEntry, ALUKA_SurpriseAttack}},
    {MONSTER_STATE_EMBRACE, {ALUKA_EmbraceEntry, ALUKA_Embrace}},
    {MONSTER_STATE_NOTICE, {ALUKA_NoticeEntry, ALUKA_Notice}},
    {MONSTER_STATE_SURPRISED, {ALUKA_SurprisedEntry, ALUKA_Surprised}},
    {MONSTER_STATE_GENERALDEATH, {ALUKA_GeneralDeathEntry, ALUKA_GeneralDeath}},
    {-1, {NULL, NULL}},
};

const MonsterFunctionTable ALUKA_FunctionTable = 
{
    ALUKA_Init,
    ALUKA_CleanUp,
    NULL,
    NULL,
    ALUKA_Message,
    ALUKA_StateChoiceTable,
    monVersion,
    NULL
};

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

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_CapDepth);
#else
int ALUKA_CapDepth(Instance *instance, Level *level)
{
    AlukaAttributes *attrs; // not from debug symbols
    int depth; // not from debug symbols
    int time; // not from debug symbols
    int minutes; // not from debug symbols
    int duration; // not from debug symbols

    attrs = (AlukaAttributes *)((MonsterAttributes *)instance->data)->tunData;

    depth = attrs->min_swim_depth;

    if (level->unitFlags & 2)
    {
        time = gameTrackerX.timeOfDay;

        switch (GAMELOOP_GetTimeOfDayIdx(time))
        {
        case 1:
            minutes = ((time % 100) + ((time / 100) * 60)) - 360;
            duration = 60;

            depth = attrs->min_swim_depth + (((attrs->daylight_depth - attrs->min_swim_depth) * minutes) / duration);
            break;
        case 2:
            depth = attrs->daylight_depth;
            break;
        case 3:
            minutes = ((time % 100) + ((time / 100) * 60)) - 1080;
            duration = 60;

            depth = attrs->daylight_depth + (((attrs->min_swim_depth - attrs->daylight_depth) * minutes) / duration);
            break;
        }
    }

    depth = level->waterZLevel - depth;

    if (depth < -32767)
    {
        depth = -32767;
    }

    return depth;
}
#endif

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

void ALUKA_FixPitch(Instance *instance, int depth)
{
    AlukaAttributes *attrs; // not from debug symbols
    int pitch; // not from debug symbols
    int min; // not from debug symbols
    int diff; // not from debug symbols
    int fullCircle; // not from debug symbols

    attrs = (AlukaAttributes *)((MonsterVars *)instance->extraData)->extraVars;

    pitch = attrs->swimfast_pitch & 0xFFF;
    min = attrs->min_swim_depth;

    fullCircle = 4096;

    pitch = (pitch < 2049) ? pitch - min : pitch - (min + fullCircle);

    diff = instance->position.z - depth;

    if (pitch < diff)
    {
        attrs->swimfast_pitch = (attrs->min_swim_depth + diff) & 0xFFF;

        ALUKA_SetPitch(instance, attrs->swimfast_pitch);
    }
}

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


void ALUKA_SwimToDestination(Instance  *instance) {
    int angle; // not from debug symbols
    int elevation; // not from debug symbols
    int forwardAccel;
    int yawAccel;
    int pitchAccel;
    int depth;
    AlukaVars *vars;
    MonsterVars *mv;
    AlukaAttributes *attrs;

    mv = (MonsterVars *)instance->extraData;
    vars = (AlukaVars *)mv->extraVars;
    attrs = (AlukaAttributes *)((MonsterAttributes *)instance->data)->tunData;
    depth = ALUKA_CapDepth(instance, STREAM_GetLevelWithID(instance->currentStreamUnitID));
    angle = MATH3D_AngleFromPosToPos(&instance->position, &mv->destination);
    elevation = MATH3D_ElevationFromPosToPos(&instance->position, &mv->destination);

    angle = AngleDiff(angle, instance->rotation.z);

    elevation = AngleDiff(elevation, (((vars->pitch - vars->pitch_offset) & 0xFFF) > 2048) ? (((vars->pitch - vars->pitch_offset) & 0xFFF) | 0xF000) : ((vars->pitch - vars->pitch_offset) & 0xFFF));

    ALUKA_ProportionalLimitsAndAccels(instance, angle, elevation);
    
    if (angle > 0) {
        yawAccel = -vars->use_yaw_accel;
    } else {
        yawAccel = vars->use_yaw_accel;
    }
    
    if (ALUKA_TerrainInPath(instance)) {
        yawAccel *= 2;
        vars->use_yaw_speed_limit *= 2;
    }
    
    if (elevation > 0) {
        pitchAccel = -vars->use_pitch_accel;
    } else {
        pitchAccel = vars->use_pitch_accel;
    }
    
    if (mv->speed > vars->forward_speed_limit) {
        forwardAccel = -attrs->forward_accel;
    } else {
        forwardAccel = attrs->forward_accel;
    }
    
    ALUKA_ApplyForwardAccel(instance, forwardAccel, gameTrackerX.timeMult);
    ALUKA_MoveForward(instance, gameTrackerX.timeMult, depth);
    ALUKA_ApplyRots(instance, gameTrackerX.timeMult);
    ALUKA_FixPitch(instance, depth);
    
    ALUKA_SetSwimBodyTwist(instance, (vars->yaw_speed * attrs->twist_mult) / 4096, (vars->pitch_speed * attrs->twist_mult) / 4096);
    ALUKA_ApplyAngularAccel(instance, yawAccel, pitchAccel, gameTrackerX.timeMult);
    MON_DefaultQueueHandler(instance);
}

void ALUKA_GetRandomDestination(Instance* instance, Position* destination, Position* start, int range) 
{

    StreamUnit* streamUnit;
    int r1, r2, r3;
    int dx;
    Position temp;

    streamUnit = STREAM_GetStreamUnitWithID(instance->currentStreamUnitID);
    
    if (streamUnit == NULL) 
    {
        return;
    }

    if (PLANAPI_FindClosestNodePositionInUnit(streamUnit, start, destination, range / 2, range / 2, 0x1C, 1) != 0)
    {
        return;
    }

    r1 = rand() % range;
    r2 = rand();
    r3 = rand();

    temp.z = start->z - ((r1 * rsin(r3)) >> 0xC);
    dx = (r1 * rcos(r3)) >> 0xC;

    temp.x = start->x + ((dx * rcos(r2)) >> 0xC);
    temp.y = start->y + ((dx * rsin(r2)) >> 0xC);

    if (temp.z > streamUnit->level->waterZLevel)
    {
        destination->z -= ((short)temp.z - (short)streamUnit->level->waterZLevel) << 1;
    }
}

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


void ALUKA_GetCircleDestination(Instance* instance, Position* target, Level* level) // matches 100% on decomp.me
{
    
    Position sp10;
    Position sp18;
    Position sp20;
    int temp_a2;
    int depth;
    int temp_a1;
    int temp_a1_2;
    int var_a0;
    int var_a0_2;
    int var_t0;
    unsigned char priorOverride;
    MonsterVars* mv;
    AlukaAttributes* attrs;
    MonsterAttributes *ma;

    ma = (MonsterAttributes *)instance->data;
    mv = (MonsterVars *)instance->extraData;
    priorOverride = gameTrackerX.monster_collide_override;
    attrs = (AlukaAttributes *)ma->tunData;
    gameTrackerX.monster_collide_override = 1;
    
    if (rand() < 0xA3 || ALUKA_NearAluka(instance))
    {
        mv->auxFlags ^= 8;
    }
    
    temp_a2 = MATH3D_AngleFromPosToPos(target, &instance->position);
    var_t0 = mv->auxFlags & 8 ? -0x199 : 0x199;
    
    COPY_SVEC(Position, &sp18, Position, target);
    sp18.z += attrs->attack_pos_bump;
    
    ALUKA_VectorFromPitchYaw(&mv->destination, 0, temp_a2 + var_t0, attrs->circle_dist);

    ADD_SVEC(Position, &mv->destination, Position, &mv->destination, Position, &sp18);
    
    if (ALUKA_SimpleLineCheck(&sp10, level, &sp18, &mv->destination))
    {
        var_a0 = MATH3D_LengthXYZ(sp10.x - sp18.x, sp10.y - sp18.y, sp10.z - sp18.z);
        temp_a1 = var_a0 - 640;
        
        if (var_a0 == 0)
        {
            var_a0 = 1;
        }

        mv->destination.x = sp18.x + (((sp10.x - sp18.x) * temp_a1) / var_a0);
        mv->destination.y = sp18.y + (((sp10.y - sp18.y) * temp_a1) / var_a0);
        mv->destination.z = sp18.z + (((sp10.z - sp18.z) * temp_a1) / var_a0);
        
        if (ALUKA_SimpleLineCheck(&sp20, level, &instance->position, &mv->destination))
        {
            var_a0_2 = MATH3D_LengthXYZ(sp20.x - instance->position.x, sp20.y - instance->position.y, sp20.z - instance->position.z);
            temp_a1_2 = var_a0_2 - 320;
            
            if (var_a0_2 == 0) 
            {
                var_a0_2 = 1;
            }
            
            mv->destination.x = instance->position.x + (((sp20.x - instance->position.x) * temp_a1_2) / var_a0_2);
            mv->destination.y = instance->position.y + (((sp20.y - instance->position.y) * temp_a1_2) / var_a0_2);
            mv->destination.z = instance->position.z + (((sp20.z - instance->position.z) * temp_a1_2) / var_a0_2);
        }
    }
    
    depth = ALUKA_CapDepth(instance, level);
    
    if (depth < mv->destination.z)
    {
        mv->destination.z = depth;
    }

    gameTrackerX.monster_collide_override = priorOverride;
}

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
        case STILL_PLANNING:
            if (attrs->circle_start_dist < dist)
            {
                return MOVE_TO_TARGET;
            }
            return STILL_PLANNING;
        case MOVE_INVALID:
            MON_GetPlanSlot(mv);
            if (attrs->circle_start_dist >= dist)
            {
                return STILL_PLANNING;
            }
            return MOVE_INVALID;
        default:
            return MOVE_TO_WAYPOINT;
        }
    }

    return STILL_PLANNING;
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

    if (nodeType != 64 && ((nodeType >> 3) & 3) == WATER_NODE)
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

    if (nodeType != 64 && ((nodeType >> 3) & 3) == FLOOR_NODE)
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

void ALUKA_SetJumpVels(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols
    AlukaVars *vars; // not from debug symbols
    Position *target; // not from debug symbols
    Position *high; // not from debug symbols
    Position *low; // not from debug symbols
    int dx; // not from debug symbols
    int dy; // not from debug symbols
    int up; // not from debug symbols
    int down; // not from debug symbols
    int time; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (AlukaVars *)mv->extraVars;
    target = &vars->target;

    if (mv->auxFlags & 2)
    {
        high = &instance->position;
        low = target;
    }
    else
    {
        high = target;
        low = &instance->position;
    }

    dx = target->x - instance->position.x;
    dy = target->y - instance->position.y;

    up = MATH3D_FastSqrt(0x50000);
    down = MATH3D_FastSqrt(((high->z - low->z) + 640) << 17);

    time = (up + (down / 16)) / 4096;

    if (mv->auxFlags & 2)
    {
        instance->zVel = up / 256;
    }
    else
    {
        instance->zVel = down / 4096;
    }

    instance->xVel = (short)(dx / time);
    instance->yVel = (short)(dy / time);
}

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

void ALUKA_JumpTo(Instance *instance) 
{

    int newPitch;
    int angle; // not from debug symbols
    int onGround; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    AlukaVars *vars; // not from debug symbols
    AlukaAttributes *attrs; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (AlukaVars *)mv->extraVars;
    ma = (MonsterAttributes *)instance->data;
    attrs = (AlukaAttributes *)ma->tunData;
    
    if (!(mv->auxFlags & 4)) 
    {
        if (mv->auxFlags & 2) 
        {
            
            angle = MATH3D_AngleFromPosToPos(&instance->position, &vars->target);
            AngleMoveToward(&instance->rotation.z, angle, (mv->subAttr->speedRunTurn * gameTrackerX.timeMult) / 4096);
            
            if (instance->rotation.z == angle)
            {
                vars->special_time = MON_GetTime(instance) + ((unsigned char)attrs->start_jump_frame * 33);
                ALUKA_SetSwimBodyTwist(instance, 0, 0);
                MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, ALUKA_ANIM_JUMPIN, 1);
                ALUKA_SetJumpVels(instance);
                mv->auxFlags |= 4;
            }
        } 
        else if (vars->pitch_offset == 0 && mv->speed == 0)
        {
            
            ALUKA_SetSwimBodyTwist(instance, 0, 0);
            angle = MATH3D_AngleFromPosToPos(&instance->position, &vars->target);
            AngleMoveToward(&instance->rotation.z, angle, (mv->subAttr->speedRunTurn * gameTrackerX.timeMult) / 4096);
            
            newPitch = vars->pitch;
            AngleMoveToward((short *)&newPitch, 0, (attrs->slow_pitch * gameTrackerX.timeMult) / 4096);
            ALUKA_SetPitch(instance, newPitch);
            
            if (instance->rotation.z == angle && vars->pitch == 0) 
            {
                ALUKA_DisableControllers(instance);
                MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, ALUKA_ANIM_JUMPOUT, 1);
                ALUKA_SetJumpVels(instance);
                mv->auxFlags |= 4;
            }
        } 
        else 
        {
            ALUKA_SetupSwimAnimWTread(instance);
            ALUKA_SwimToDestination(instance);
        }
    } 
    else if (mv->auxFlags & 2) 
    {
        if (!(mv->mvFlags & 0x400)) 
        {
            if (instance->zVel < 0) 
            {
                
                mv->mvFlags &= ~0x800;

                onGround = MON_OnGround(instance);

                mv->mvFlags |= 0x800;

                if (onGround != 0)
                {
                    ALUKA_SetHitGround(instance);
                    return;
                }
            }
            if (MON_GetTime(instance) >= (unsigned long)vars->special_time) 
            {
                PhysicsMove(instance, &instance->position, gameTrackerX.timeMult);
            }
        } 
        else 
        {
            MON_SwitchState(instance, MONSTER_STATE_LANDINWATER);
        }
    } 
    else if (mv->auxFlags & 0x10)
    {
        
        mv->mvFlags &= ~0x800;

        onGround = MON_OnGround(instance);

        mv->mvFlags |= 0x800;

        if (onGround != 0)
        {
            ALUKA_SetHitGround(instance);
        } 
        else
        {
            
            PhysicsMove(instance, &instance->position, gameTrackerX.timeMult);
            
            if (mv->mvFlags & 0x400)
            {
                MON_SwitchState(instance, MONSTER_STATE_LANDINWATER);
            }
        }
    } 
    else 
    {
        
        PhysicsMove(instance, &instance->position, gameTrackerX.timeMult);
        
        if (!(mv->mvFlags & 0x400)) 
        {
            mv->auxFlags |= 0x10;
        } 
        else if (instance->zVel < 0)
        {
            MON_SwitchState(instance, MONSTER_STATE_LANDINWATER);
        }
    }
    
    MON_DefaultQueueHandler(instance);
}

int ALUKA_ChooseAttack(Instance *instance, MonsterIR *enemy) 
{

    Position toinstance;
    Position facing; // not from debug symbols
    Position target; // not from debug symbols
    Position* facingNormal;
    
    int depth;
    AlukaVars *vars;
    MonsterVars *mv;
    Level *level;
    Instance *enemyInst;
    AlukaAttributes* attrs;

    mv = (MonsterVars *)instance->extraData;
    vars = (AlukaVars *)mv->extraVars;
    attrs = (AlukaAttributes *)((MonsterAttributes *)instance->data)->tunData;
    level = STREAM_GetLevelWithID(instance->currentStreamUnitID);
    enemyInst = enemy->instance;
    
    if (mv->age == 0 && level->waterZLevel < enemyInst->position.z && ALUKA_NotDaylight(level))
    {
        if (MON_GetTime(instance) >= (unsigned long)vars->special_time || MON_ShouldIFireAtTarget(instance, enemy)) 
        {
            return SPIT_ATTACK;
        }
        return NO_ATTACK;
    }
    
    depth = ALUKA_CapDepth(instance, level);
    COPY_SVEC(Position, &target, Position, &enemyInst->position);
    target.z += attrs->attack_pos_bump;
    
    if (!(enemy->mirFlags & 0x20) || depth < target.z) 
    {
        return NO_ATTACK;
    }
        
    toinstance.x = target.x - instance->position.x;
    toinstance.y = target.y - instance->position.y;
    toinstance.z = target.z - instance->position.z;

    ALUKA_FacingVector(instance, &facing, 0x1000);
    
    if (ALUKA_AngleTooWide(&facing, &toinstance, 0x2D4, 0xA) || INSTANCE_Query(enemyInst, 1) != 1) 
    {
        return NO_ATTACK;
    }
    
    facingNormal = (Position *)INSTANCE_Query(enemyInst, 0x10);
    
    if (facingNormal == NULL) 
    {
        return NO_ATTACK;
    }
    
    toinstance.x = -toinstance.x;
    toinstance.y = -toinstance.y;
    toinstance.z = -toinstance.z;
    
    if (MON_GetTime(instance) >= (unsigned long)vars->special_time || !ALUKA_AngleTooWide(facingNormal, &toinstance, 0x2D4, 0xA))
    {
        return CHOMP_ATTACK;
    }
    
    return NO_ATTACK;
}

void ALUKA_Init(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    AlukaVars *vars; // not from debug symbols

    MON_DefaultInit(instance);

    mv = (MonsterVars *)instance->extraData;

    if (mv != NULL)
    {

        vars = (AlukaVars *)MEMPACK_Malloc(sizeof(AlukaVars), MEMORY_TYPE_ALUKADATA);

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

void ALUKA_Message(Instance *instance, unsigned long message, unsigned long data)
{
    MonsterVars *mv; // not from debug symbols
    AlukaAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    attrs = (AlukaAttributes *)((MonsterAttributes *)instance->data)->tunData;

    if ((mv->extraVars != NULL) && (attrs != NULL))
    {
        switch (message)
        {
        case 0x100007:
            MonsterMessage(instance, 0x100007, data);

            mv->mvFlags &= ~0x800;
            return;
        case 0x1000021:
            if ((mv->mvFlags & 0x400) && (mv->damageType != 0x20))
            {
                ((evMonsterHitData *)data)->power = 0x4000;

                message = 0x1000000;
            }

            break;
        case 0x1000000:
            if ((mv->mvFlags & 0x400) && (INSTANCE_Query(((evMonsterHitData *)data)->sender, 1) == 0x20))
            {
                ((evMonsterHitData *)data)->power *= 2;
            }

            break;
        case 0x100000C:
            if ((data == 0x40) && ((mv->mvFlags & 0x400) || (*(long *)&gameTrackerX.gameData.asmData.MorphTime != 1000)))
            {
                if (GAMELOOP_GetTimeOfDay() != 700)
                {
                    return;
                }

                if (instance->position.z <= ALUKA_CapDepth(instance, STREAM_GetLevelWithID(instance->currentStreamUnitID)))
                {
                    return;
                }
            }

            break;
        case 0x80001:
            if (!(mv->mvFlags & 0x400))
            {
                MonsterMessage(instance, 0x80001, data);
            }

            return;
        case 0x200001:
        {
            evCollideInstanceStatsData *ptr; // not from debug symbols

            ptr = (evCollideInstanceStatsData *)data;

            if (!(INSTANCE_Query(ptr->instance, 1) & 0x20) && (mv->mvFlags & 0x400)
            && ((signed char)mv->alertCount == 0) && (ptr->distance < (unsigned long)attrs->swim_smell_distance))
            {
                MONSENSE_SetupMIR(instance, ptr, 0x80);
            }
            else
            {
                MONSENSE_SenseInstance(instance, ptr);
            }
        }

            return;
        }
    }

    MonsterMessage(instance, message, data);
}

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

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_Pursue);
#else
void ALUKA_Pursue(Instance *instance) 
{
    
    Position target;
    int pm;
    Level *level;
    int attack;
    MonsterVars *mv;
    MonsterAttributes *ma;
    AlukaVars *vars;
    AlukaAttributes *attrs;
    MonsterIR *enemy;

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    enemy = mv->enemy;
    attrs = (AlukaAttributes *)ma->tunData;
    level = STREAM_GetLevelWithID(instance->currentStreamUnitID);
    vars = (AlukaVars *)mv->extraVars;
    
    if (vars == NULL)
    {
        return;
    }
        
    mv->auxFlags |= 0x20000000;
    
    if (mv->auxFlags & 1)
    {
        ALUKA_JumpTo(instance);
        return;
    }
    
    if (mv->mvFlags & 4) 
    {
        MON_SwitchState(instance, 2);
        return;
    }
    
    if (!(mv->mvFlags & 0x400)) 
    {
        
        ALUKA_ResetSwim(instance);
        
        if (!(mv->mvFlags & 4) && enemy != NULL)
        {
            
            ENMYPLAN_MoveToTargetFinal(instance, &mv->destination, (signed char)mv->pathSlotID, &enemy->instance->position, 0x1E00001F);
            
            if (ALUKA_ShouldJumpIn(instance, enemy->instance, level))
            {
                if (ENMYPLAN_GetPosOfNextWaypoint((signed char)mv->pathSlotID, &target))
                {
                    mv->auxFlags |= 2;
                    ALUKA_JumpToEntry(instance, &target);
                    mv->lookAtPos = NULL;
                }
            
                MON_DefaultQueueHandler(instance);
                return;
            }
        }
        
        MON_Pursue(instance);
        return;
    }
    
    if (enemy != NULL) 
    {
        
        pm = ALUKA_SwimPlanMovement(instance, &enemy->instance->position, &mv->destination, level);
        
        if (pm == 0) 
        {
            vars->forward_speed_limit = attrs->swimslow_speed;
            vars->yaw_speed_limit = attrs->swimslow_yaw;
            vars->pitch_speed_limit = attrs->slow_pitch;
        } 
        else 
        {
            if (vars->last_pm == 0) 
            {
                MON_GetPlanSlot(mv);
            }
            
            vars->forward_speed_limit = attrs->swimfast_speed;
            vars->yaw_speed_limit = attrs->swimfast_yaw;
            vars->pitch_speed_limit = attrs->swimfast_pitch;
        }
        
        vars->last_pm = pm;
        
        if (ALUKA_ShouldJumpOut(instance, enemy->instance, level)) 
        {
            if (ENMYPLAN_GetPosOfNextWaypoint((signed char)mv->pathSlotID, &target)) 
            {
                mv->auxFlags &= ~2;
                ALUKA_JumpToEntry(instance, &target);
                return;
            }
            
            ALUKA_SetupSwimAnimWOTread(instance);
            ALUKA_SwimToDestination(instance);
            return;
        }
        
        if (pm == 0) 
        {
            
            attack = ALUKA_ChooseAttack(instance, enemy);
            
            if (attack == 0) {
                ALUKA_GetCircleDestination(instance, &enemy->instance->position, level);
            } 
            else
            {

                COPY_SVEC(Position, &mv->destination, Position, &enemy->instance->position);
                mv->destination.z += attrs->attack_pos_bump;

                if (attack == 1)
                {
                    MON_SwitchState(instance, MONSTER_STATE_ATTACK);
                }
                else if ((unsigned long)(vars->special_time - 4950) <= MON_GetTime(instance))
                {
                    MON_SwitchState(instance, 28);
                }
            }
        }
        
        ALUKA_SetupSwimAnimWOTread(instance);
        ALUKA_SwimToDestination(instance);
        return;
    }
    
    MON_SwitchState(instance, 2);
    MON_DefaultQueueHandler(instance);
}
#endif

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

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/Overlays/aluka/aluka", ALUKA_LandInWaterEntry);
#else
void ALUKA_LandInWaterEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols
    AlukaVars *vars; // not from debug symbols
    Level *level; // not from debug symbols
    int zVel; // not from debug symbols
    short pitch; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (AlukaVars *)mv->extraVars;

    level = STREAM_GetLevelWithID(instance->currentStreamUnitID);

    if (vars != NULL)
    {
        mv->speed = 0;

        vars->yaw_speed = 0;
        vars->pitch_offset = 0;

        instance->yAccl = 0;
        instance->xAccl = 0;

        zVel = instance->position.z - ALUKA_CapDepth(instance, level);
        zVel = (zVel / 10) + 1;

        if (zVel < instance->zVel)
        {
            instance->zVel = zVel;
        }

        pitch = vars->pitch & 0xFFF;

        if (pitch < 2049)
        {
            vars->pitch_speed = pitch / 10;
        }
        else
        {
            vars->pitch_speed = (pitch - 4096) / 10;
        }

        ALUKA_EnableControllers(instance);

        MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 5, 1);

        vars->swim_anim = -1;

        mv->auxFlags &= ~1;
        mv->mvFlags |= 0x800;
    }
}
#endif

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
