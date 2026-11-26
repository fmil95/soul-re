#include "Overlays/wallcr/wallcr.h"
#include "Game/DEBUG.h"
#include "Game/FX.h"
#include "Game/GAMELOOP.h"
#include "Game/G2/ANMCTRLR.h"
#include "Game/G2/ANMG2ILF.h"
#include "Game/INSTANCE.h"
#include "Game/MATH3D.h"
#include "Game/MEMPACK.h"
#include "Game/OBTABLE.h"
#include "Game/SAVEINFO.h"
#include "Game/SOUND.h"
#include "Game/STATE.h"
#include "Game/MONSTER/MONAPI.h"
#include "Game/MONSTER/MONLIB.h"
#include "Game/MONSTER/MONMSG.h"
#include "Game/MONSTER/MONSTER.h"
#include "Game/COLLIDE.h"
#include "Game/PHYSICS.h"
#include "Game/STREAM.h"
#include "Game/CAMERA.h"
#include "Game/PLAN/ENMYPLAN.h"
#include "Game/PLAN/PLANAPI.h"
#include "Game/MONSTER/MONSENSE.h"
#include "Game/RAZIEL/RAZIEL.h"
#include "Game/PSX/SUPPORT.h"
#include "Game/PSX/SUPPORT.h"

MonsterStateChoice WALLCR_StateChoiceTable[] = 
{
    {MONSTER_STATE_PURSUE, {WALLCR_PursueEntry, WALLCR_Pursue}},
    {MONSTER_STATE_IDLE, {WALLCR_IdleEntry, WALLCR_Idle}},
    {MONSTER_STATE_WANDER, {WALLCR_WanderEntry, WALLCR_Wander}},
    {MONSTER_STATE_SURPRISEATTACK, {WALLCR_SurpriseAttackEntry, WALLCR_SurpriseAttack}},
    {MONSTER_STATE_IMPACT, {WALLCR_ImpactEntry, WALLCR_Impact}},
    {MONSTER_STATE_GENERALDEATH, {WALLCR_GeneralDeathEntry, WALLCR_GeneralDeath}},
    {MONSTER_STATE_HIT, {WALLCR_HitEntry, WALLCR_Hit}},
    {MONSTER_STATE_MISSILEHIT, {WALLCR_MissileHitEntry, WALLCR_MissileHit}},
    {MONSTER_STATE_NOTICE, {WALLCR_NoticeEntry, WALLCR_Notice}},
    {MONSTER_STATE_SURPRISED, {WALLCR_SurprisedEntry, WALLCR_Surprised}},
    {MONSTER_STATE_STUNNED, {WALLCR_StunnedEntry, WALLCR_Stunned}},
    {MONSTER_STATE_THROWN, {WALLCR_ThrownEntry, WALLCR_Thrown}},
    {MONSTER_STATE_PUPATE, {WALLCR_PupateEntry, WALLCR_Pupate}},
    {-1, {NULL, NULL}},
};

const MonsterFunctionTable WALLCR_FunctionTable = 
{
    WALLCR_Init,
    WALLCR_CleanUp,
    NULL,
    WALLCR_Query,
    WALLCR_Message,
    WALLCR_StateChoiceTable,
    monVersion,
    NULL
};

void WALLCR_EnableControllers(Instance *instance)
{
    Position offset;

    if (G2Anim_IsControllerActive(&instance->anim, 1, 38) == G2FALSE)
    {
        G2Anim_EnableController(&instance->anim, 1, 38);

        offset.y = 0;
        offset.x = 0;
        offset.z = -436;

        G2Anim_SetController_Vector(&instance->anim, 1, 38, (G2SVector3 *)&offset);

        instance->position.z += 436;

        G2Anim_EnableController(&instance->anim, 0, 14);
        G2Anim_EnableController(&instance->anim, 18, 14);
    }
}

void WALLCR_DisableControllers(Instance *instance)
{
    if (G2Anim_IsControllerActive(&instance->anim, 1, 38) != G2FALSE)
    {
        G2Anim_DisableController(&instance->anim, 1, 38);

        instance->position.z -= 436;

        G2Anim_DisableController(&instance->anim, 0, 14);
        G2Anim_DisableController(&instance->anim, 18, 14);
    }
}

void WALLCR_SetFlex(Instance *instance, short flex)
{
    Position vec;

    vec.x = flex;
    vec.z = 0;
    vec.y = 0;

    G2Anim_SetController_Vector(&instance->anim, 18, 14, (G2SVector3 *)&vec);
}

void *WALLCR_CollideFace(Level *level, Position *start, Position *end, Position *hit, Normal *norm)
{
    PCollideInfo pcollideInfo;
    SVECTOR oldVector;
    SVECTOR newVector;
    void *result; // not from debug symbols
    short endZ; // not from debug symbols

    result = NULL;

    oldVector.vx = start->x;
    oldVector.vy = start->y;
    oldVector.vz = start->z;

    newVector.vx = end->x;
    newVector.vy = end->y;
    endZ         = end->z;

    pcollideInfo.oldPoint = &oldVector;
    pcollideInfo.newPoint = &newVector;
    pcollideInfo.inst = NULL;
    pcollideInfo.collideType = 1;
    pcollideInfo.instance = NULL;

    newVector.vz = endZ;

    COLLIDE_PointAndWorld(&pcollideInfo, level);

    if (pcollideInfo.type == 3)
    {
        result = pcollideInfo.prim;

        if (hit != NULL)
        {
            hit->x = newVector.vx;
            hit->y = newVector.vy;
            hit->z = newVector.vz;
        }

        if (norm != NULL)
        {
            norm->x = pcollideInfo.wNormal.vx;
            norm->y = pcollideInfo.wNormal.vy;
            norm->z = pcollideInfo.wNormal.vz;
        }
    }

    return result;
}

int WALLCR_NearFloor(Instance *instance)
{
    Level *level; // not from debug symbols
    Position start; // not from debug symbols
    Position end; // not from debug symbols

    level = STREAM_GetLevelWithID(instance->currentStreamUnitID);

    if (level != NULL)
    {
        COPY_SVEC(Position, &start, Position, &instance->position);
        COPY_SVEC(Position, &end, Position, &start);

        end.z -= 320;

        if (WALLCR_CollideFace(level, &start, &end, NULL, NULL) != NULL)
        {
            return 1;
        }
    }

    return 0;
}

int WALLCR_FindJumpOnPoint(Instance *instance, Position *point, Normal *norm)
{
    Position start; // not from debug symbols
    Position end; // not from debug symbols
    Position offset; // not from debug symbols
    Position center; // not from debug symbols
    Level *level; // not from debug symbols
    void *result; // not from debug symbols
    int i; // not from debug symbols

    result = NULL;

    level = STREAM_GetLevelWithID(instance->currentStreamUnitID);

    if (level != NULL)
    {
        i = 0;

        COPY_SVEC(Position, &center, Position, point);

        offset.x = 320;
        offset.y = 0;

        start.z = center.z;
        end.z = center.z;

        do
        {
            start.x = center.x - offset.x;
            start.y = center.y - offset.y;
            
            end.x = center.x + offset.x;
            end.y = center.y + offset.y;

            result = WALLCR_CollideFace(level, &start, &end, point, norm);

            if (result != NULL)
            {
                break;
            }

            i++;

            offset.y ^= offset.x;
            offset.x ^= offset.y;
            offset.y ^= offset.x;

            offset.x = -offset.x;
        } while (i < 4);
    }

    return (int)(intptr_t)result;
}

int WALLCR_AngleFromPosToPos(Position *from, Position *to, Rotation *rot)
{
    MATRIX mat;
    MATRIX xform;
    Position lto;

    mat.m[0][0] = 4096;
    mat.m[0][1] = 0;
    mat.m[0][2] = 0;
    mat.m[1][0] = 0;
    mat.m[1][1] = 4096;
    mat.m[1][2] = 0;
    mat.m[2][0] = 0;
    mat.m[2][1] = 0;
    mat.m[2][2] = 4096;

    RotMatrixZ(rot->z, &mat);
    RotMatrixY(rot->y, &mat);
    RotMatrixX(rot->x, &mat);

    TransposeMatrix(&mat, &xform);

    COPY_SVEC(Position, &lto, Position, to);
    lto.x -= from->x;
    lto.y -= from->y;
    lto.z -= from->z;

    ApplyMatrixSV(&xform, (SVECTOR *)&lto, (SVECTOR *)&lto);

    return (ratan2(lto.z, -lto.x) + 3072) & 0xFFF;
}

#ifndef NON_MATCHING
INCLUDE_RODATA("asm/nonmatchings/Overlays/wallcr/wallcr", D_88000020);

INCLUDE_RODATA("asm/nonmatchings/Overlays/wallcr/wallcr", D_88000038);

INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_FollowWall);
#else
int WALLCR_FollowWall(Instance *instance)
{
    SVECTOR testPos[3] = {{0, 0, 0, 0}, {0, 250, 250, 0}, {0, 250, -250, 0}};
    MATRIX mat;
    TFace *res[2];
    Position hitPos[2];
    Normal hitNorm[2];
    Rotation rot;
    Rotation temp;
    unsigned char order[4][2] = {{0, 1}, {0, 2}, {1, 2}, {2, 1}};
    int dist[2]; // not from debug symbols
    Position newVector;
    Normal tempn;
    SVECTOR oldVector;
    SVECTOR vec; // not from debug symbols
    Position zero;
    WallcrVars *vars; // not from debug symbols
    Level *level; // not from debug symbols
    int i; // not from debug symbols
    int n; // not from debug symbols
    int total; // not from debug symbols
    short flex; // not from debug symbols
    short roll; // not from debug symbols

    flex = 0;

    vars = (WallcrVars *)((MonsterVars *)instance->extraData)->extraVars;

    level = STREAM_GetLevelWithID(instance->currentStreamUnitID);

    memset(&zero, 0, sizeof(Position));

    mat.m[0][0] = 4096;
    mat.m[0][1] = 0;
    mat.m[0][2] = 0;
    mat.m[1][0] = 0;
    mat.m[1][1] = 4096;
    mat.m[1][2] = 0;
    mat.m[2][0] = 0;
    mat.m[2][1] = 0;
    mat.m[2][2] = 4096;

    RotMatrixY(vars->roll, &mat);
    RotMatrixZ(instance->rotation.z, &mat);
    RotMatrixY(instance->rotation.y, &mat);
    RotMatrixX(instance->rotation.x, &mat);

    for (i = 0; i < 3; i++)
    {
        ApplyMatrixSV(&mat, &testPos[i], &testPos[i]);

        testPos[i].vx += instance->position.x;
        testPos[i].vy += instance->position.y;
        testPos[i].vz += instance->position.z;
    }

    n = 0;

    for (i = 0; i < 4; i++)
    {
        res[n] = (TFace *)WALLCR_CollideFace(level, (Position *)&testPos[order[i][0]], (Position *)&testPos[order[i][1]], &hitPos[n], &hitNorm[n]);

        if (res[n] != NULL)
        {
            n++;

            if (n == 2)
            {
                if (res[1] != res[0])
                {
                    break;
                }

                n = 1;
            }
        }
    }

    switch (n)
    {
    case 0:
        return 0;
    case 1:
        flex = 0;

        tempn = hitNorm[0];
        break;
    case 2:
        newVector.x = instance->position.x - hitPos[0].x;
        newVector.y = instance->position.y - hitPos[0].y;
        newVector.z = instance->position.z - hitPos[0].z;

        dist[0] = (short)((newVector.x * hitNorm[0].x) >> 12) + (short)((newVector.y * hitNorm[0].y) >> 12) + (short)((newVector.z * hitNorm[0].z) >> 12);

        newVector.x = instance->position.x - hitPos[1].x;
        newVector.y = instance->position.y - hitPos[1].y;
        newVector.z = instance->position.z - hitPos[1].z;

        dist[1] = (short)((newVector.x * hitNorm[1].x) >> 12) + (short)((newVector.y * hitNorm[1].y) >> 12) + (short)((newVector.z * hitNorm[1].z) >> 12);

        if (dist[0] < 100)
        {
            dist[0] = 100;
        }
        else if (dist[0] > 200)
        {
            dist[0] = 1;
        }
        else
        {
            dist[0] = 201 - dist[0];
        }

        if (dist[1] < 100)
        {
            dist[1] = 100;
        }
        else if (dist[1] > 200)
        {
            dist[1] = 1;
        }
        else
        {
            dist[1] = 201 - dist[1];
        }

        total = dist[0] + dist[1];

        tempn.x = ((dist[0] * hitNorm[0].x) + (dist[1] * hitNorm[1].x)) / total;
        tempn.y = ((dist[0] * hitNorm[0].y) + (dist[1] * hitNorm[1].y)) / total;
        tempn.z = ((dist[0] * hitNorm[0].z) + (dist[1] * hitNorm[1].z)) / total;

        MATH3D_Normalize(&tempn);

        flex = MATH3D_AngleBetweenVectors((SVector *)&tempn, (dist[0] >= dist[1]) ? (SVector *)&hitNorm[0] : (SVector *)&hitNorm[1]);
        break;
    }

    newVector.x = instance->position.x - ((tempn.x * 75) >> 10);
    newVector.y = instance->position.y - ((tempn.y * 75) >> 10);
    newVector.z = instance->position.z - ((tempn.z * 75) >> 10);

    res[0] = (TFace *)WALLCR_CollideFace(level, &instance->position, &newVector, &newVector, NULL);

    if (res[0] == NULL)
    {
        return 0;
    }

    instance->position.x = newVector.x + ((tempn.x * 75) >> 11);
    instance->position.y = newVector.y + ((tempn.y * 75) >> 11);
    instance->position.z = newVector.z + ((tempn.z * 75) >> 11);

    MATH3D_RotationFromPosToPos(&zero, (Position *)&tempn, &rot);

    if (((instance->rotation.y == -1024) && (instance->rotation.z == -1024)) ? (rot.x < 1024) : (rot.x >= 1024))
    {
        oldVector.vx = 0;
        oldVector.vy = -4096;
        oldVector.vz = 0;

        ApplyMatrixSV(&mat, &oldVector, &vec);

        roll = vars->roll + (MATH3D_AngleFromPosToPos(&zero, (Position *)&vec) - rot.z);

        temp.z = 0;
        temp.x = 0;
        temp.y = roll;

        G2Anim_SetController_Vector(&instance->anim, 0, 14, (G2SVector3 *)&temp);

        vars->roll = roll;
    }

    WALLCR_SetFlex(instance, flex);

    instance->oldTFace = instance->tface;
    instance->tfaceLevel = level;
    instance->tface = res[0];

    instance->rotation.x = rot.x;
    instance->rotation.y = rot.y;
    instance->rotation.z = rot.z;

    MATH3D_ZYXtoXYZ(&instance->rotation);

    return 1;
}
#endif

int WALLCR_MoveForward(Instance *instance, Position *pos)
{
    Rotation offset;
    short current_roll;
    MonsterVars *mv; // not from debug symbols
    WallcrVars *vars; // not from debug symbols
    short angle; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (WallcrVars *)mv->extraVars;

    angle = WALLCR_AngleFromPosToPos(&instance->position, pos, &instance->rotation);

    current_roll = vars->roll;

    AngleMoveToward(&current_roll, angle, mv->subAttr->speedRunTurn);

    offset.z = 0;
    offset.x = 0;
    offset.y = current_roll;

    G2Anim_SetController_Vector(&instance->anim, 0, 14, (G2SVector3 *)&offset);

    vars->roll = current_roll;

    if (WALLCR_FollowWall(instance) == 0)
    {
        MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 6, 2);

        mv->auxFlags &= ~0x8;

        return 0;
    }

    if (!(mv->auxFlags & 0x8))
    {
        MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 8, 2);

        mv->auxFlags |= 0x8;
    }

    return 1;
}

void WALLCR_ApplySpin(Instance *instance, int timeMult)
{
    Rotation offset;
    WallcrVars *vars; // not from debug symbols

    vars = (WallcrVars *)((MonsterVars *)instance->extraData)->extraVars;

    instance->rotation.y += (vars->correct_roll_speed * timeMult) / 4096;
    instance->rotation.x += (vars->pitch_speed * timeMult) / 4096;

    vars->roll += (vars->roll_speed * timeMult) / 4096;

    if (G2Anim_IsControllerActive(&instance->anim, 1, 38) != G2FALSE)
    {
        offset.z = 0;
        offset.x = 0;
        offset.y = vars->roll;

        G2Anim_SetController_Vector(&instance->anim, 0, 14, (G2SVector3 *)&offset);
    }

    instance->rotation.z += (vars->yaw_speed * timeMult) / 4096;
}

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_AttachEntry);
#else
void WALLCR_AttachEntry(Instance *instance, Position *point, Normal *norm)
{
    Position zero;
    Rotation dest;
    MonsterVars *mv; // not from debug symbols
    WallcrVars *vars; // not from debug symbols
    int angle; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    memset(&zero, 0, sizeof(Position));

    if ((mv != NULL) && ((vars = (WallcrVars *)mv->extraVars) != NULL))
    {
        MATH3D_RotationFromPosToPos(&zero, (Position *)norm, &dest);

        if (instance->position.z < point->z)
        {
            WALLCR_EnableControllers(instance);

            angle = (dest.z - instance->rotation.z) & 0xFFF;

            if (angle < 2049)
            {
                vars->yaw_speed = angle / 13;
            }
            else
            {
                vars->yaw_speed = (angle - 4096) / 13;
            }

            vars->roll_speed = 0;
            vars->correct_roll_speed = 0;
            vars->roll = 0;

            angle = (dest.x - instance->rotation.x) & 0xFFF;

            if (angle < 2049)
            {
                vars->pitch_speed = angle / 13;
            }
            else
            {
                vars->pitch_speed = (angle - 4096) / 13;
            }

            vars->time = MON_GetTime(instance);

            instance->xVel = ((point->x + ((norm->x * 75) >> 11)) - instance->position.x) / 13;
            instance->zVel = 42;
            instance->zAccl = -3;
            instance->yVel = ((point->y + ((norm->y * 75) >> 11)) - instance->position.y) / 13;

            mv->mvFlags |= 0x800;
            mv->auxFlags |= 0x3;

            MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 0, 1);
        }
        else
        {
            printf("oh shit, too high\n");
        }
    }
}
#endif

void WALLCR_Attach(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols
    WallcrVars *vars; // not from debug symbols
    int time; // not from debug symbols
    int lastTime; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (WallcrVars *)mv->extraVars;

    if (vars != NULL)
    {
        if (MON_AnimPlayingFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 2) != 0)
        {
            if (instance->flags2 & 0x10)
            {
                mv->auxFlags &= ~0x1;
                mv->auxFlags &= ~0x8;
                mv->auxFlags |= 0x4;
            }
        }
        else
        {
            time = MON_GetTime(instance) - vars->time;
            lastTime = time - gameTrackerX.lastLoopTime;

            if (time >= 99)
            {
                WALLCR_ApplySpin(instance, gameTrackerX.timeMult);
            }

            if (time >= 165)
            {
                PhysicsMove(instance, &instance->position, gameTrackerX.timeMult);
            }

            if ((lastTime < 429) && (time >= 429))
            {
                MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 1, 1);
            }

            if (time >= 462)
            {
                if (WALLCR_FollowWall(instance) != 0)
                {
                    MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 2, 1);

                    instance->zAccl = 0;
                    instance->zVel = 0;
                    instance->yVel = 0;
                    instance->xVel = 0;

                    vars->roll = 0;
                    vars->pitch_speed = 0;
                    vars->roll_speed = 0;
                    vars->yaw_speed = 0;
                }

                if (time >= 594)
                {
                    WALLCR_DetachEntry(instance, NULL);
                }
            }
        }
    }
}

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_DetachEntry);
#else
void WALLCR_DetachEntry(Instance *instance, Position *position)
{
    MATRIX mat;
    SVECTOR flatPt;
    SVECTOR vel;
    MonsterVars *mv; // not from debug symbols
    WallcrVars *vars; // not from debug symbols
    int angle; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (WallcrVars *)mv->extraVars;

    if (vars != NULL)
    {
        if (position == NULL)
        {
            RotMatrix((SVECTOR *)&instance->rotation, &mat);

            flatPt.vx = 0;
            flatPt.vy = -20;
            flatPt.vz = 0;

            ApplyMatrixSV(&mat, &flatPt, &vel);

            instance->xVel = vel.vx;
            instance->yVel = vel.vy;
            instance->zVel = vel.vz + 42;
        }
        else
        {
            instance->xVel = (position->x - instance->position.x) / 15;
            instance->yVel = (position->y - instance->position.y) / 15;
            instance->zVel = ((position->z - instance->position.z) + 2236) / 15;
        }

        instance->zAccl = -16;

        vars->yaw_speed = 136;

        angle = -instance->rotation.x & 0xFFF;

        if (angle < 2049)
        {
            vars->pitch_speed = angle / 15;
        }
        else
        {
            vars->pitch_speed = (angle - 4096) / 15;
        }

        angle = -vars->roll & 0xFFF;

        if (angle < 2049)
        {
            vars->roll_speed = angle / 15;
        }
        else
        {
            vars->roll_speed = (angle - 4096) / 15;
        }

        angle = -instance->rotation.y & 0xFFF;

        if (angle > 2048)
        {
            angle -= 4096;
        }

        vars->correct_roll_speed = angle / 15;

        vars->time = MON_GetTime(instance);

        mv->auxFlags &= ~0x4;
        mv->auxFlags |= 0x1;
        mv->auxFlags &= ~0x2;

        if (mv->auxFlags & 0x10)
        {
            MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 11, 1);
        }
        else
        {
            MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 3, 1);
        }
    }
}
#endif

void WALLCR_Detach(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols
    WallcrVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (WallcrVars *)mv->extraVars;

    if (vars != NULL)
    {
        if (MON_AnimPlayingFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 5) != 0)
        {
            if (instance->flags2 & 0x10)
            {
                mv->auxFlags &= ~0x1;
            }
        }
        else
        {
            PhysicsMove(instance, &instance->position, gameTrackerX.timeMult);

            WALLCR_ApplySpin(instance, gameTrackerX.timeMult);

            if ((MON_GetTime(instance) - vars->time) >= 495)
            {
                WALLCR_DisableControllers(instance);

                vars->roll = 0;
                vars->pitch_speed = 0;
                vars->correct_roll_speed = 0;
                vars->roll_speed = 0;
                vars->yaw_speed = 0;

                instance->rotation.x = 0;
                instance->rotation.y = 0;
            }

            mv->mvFlags &= ~0x800;

            MON_OnGround(instance);

            if (mv->mvFlags & 0x2)
            {
                WALLCR_DisableControllers(instance);

                instance->rotation.x = 0;
                instance->rotation.y = 0;

                vars->roll = 0;

                instance->zAccl = 0;
                instance->zVel = 0;
                instance->yVel = 0;
                instance->xVel = 0;

                MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 5, 1);
            }
            else
            {
                mv->mvFlags |= 0x800;
            }
        }
    }
}

int WALLCR_SeekPath(Instance *instance, Position *pos, Position *targetPos, int targetType, int validNodes)
{
    MonsterVars *mv; // not from debug symbols
    int result; // not from debug symbols

    (void)targetType;

    mv = (MonsterVars *)instance->extraData;

    result = ENMYPLAN_MoveToTargetFinal(instance, pos, mv->pathSlotID, targetPos, validNodes);

    if (result == 3)
    {
        MON_GetPlanSlot(mv);
    }

    return result;
}

int WALLCR_ShouldJumpOff(Instance *instance)
{
    int type; // not from debug symbols

    type = ENMYPLAN_GetNodeTypeOfNextWaypoint(((MonsterVars *)instance->extraData)->pathSlotID);

    if ((type != 0x40) && (((type >> 3) & 0x3) == 1))
    {
        return 1;
    }

    return 0;
}

int WALLCR_ShouldJumpOn(Instance *instance)
{
    int type; // not from debug symbols

    type = ENMYPLAN_GetNodeTypeOfNextWaypoint(((MonsterVars *)instance->extraData)->pathSlotID);

    if ((type != 0x40) && (((type >> 3) & 0x3) == 0))
    {
        return 1;
    }

    return 0;
}

int WALLCR_ShouldSurpriseAttack(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols
    WallcrVars *vars; // not from debug symbols
    MonsterIR *enemy; // not from debug symbols
    WallcrAttributes *attrs; // not from debug symbols
    int time; // not from debug symbols
    int count; // not from debug symbols
    int chance; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (WallcrVars *)mv->extraVars;
    enemy = mv->enemy;
    attrs = (WallcrAttributes *)((MonsterAttributes *)instance->data)->tunData;

    time = vars->left_over_time + gameTrackerX.timeMult;

    count = time / 4096;

    vars->left_over_time = time - (count * 4096);

    chance = 32767 / (abs(attrs->surprise_attack_time) + 1);

    for (; count > 0; count--)
    {
        if ((rand() < chance) && (enemy != NULL))
        {
            if (PLANAPI_FindClosestNodePositionInUnit(STREAM_GetStreamUnitWithID(instance->currentStreamUnitID), &enemy->instance->position, &mv->destination, 0, 32767, 20, 0) != 0)
            {
                return 1;
            }

            break;
        }
    }

    return 0;
}

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/Overlays/wallcr/wallcr", WALLCR_MoveToDestination);
#else
int WALLCR_MoveToDestination(Instance *instance, Position *target)
{
    Position pos;
    Position point;
    Normal norm;
    MonsterVars *mv; // not from debug symbols
    int result; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    MON_DefaultQueueHandler(instance);

    if (mv->auxFlags & 0x1)
    {
        if (mv->auxFlags & 0x2)
        {
            WALLCR_Attach(instance);
        }
        else
        {
            WALLCR_Detach(instance);
        }

        return 1;
    }

    if (mv->auxFlags & 0x4)
    {
        result = WALLCR_SeekPath(instance, &pos, &mv->destination, 0, 0x181A1F);

        if (result != 0)
        {
            if (result == 3)
            {
                if (instance->currentMainState == MONSTER_STATE_SURPRISEATTACK)
                {
                    MON_SwitchState(instance, MONSTER_STATE_PURSUE);
                }
            }
            else if (WALLCR_ShouldJumpOn(instance) != 0)
            {
                if (ENMYPLAN_GetPosOfNextWaypoint(mv->pathSlotID, &pos) != 0)
                {
                    WALLCR_DetachEntry(instance, &pos);
                }
            }
            else
            {
                WALLCR_MoveForward(instance, &pos);
            }
        }

        return 1;
    }

    result = WALLCR_SeekPath(instance, &pos, &mv->destination, 0, 0x181A1F);

    if (target != NULL)
    {
        COPY_SVEC(Position, target, Position, &pos);
    }

    if (result != 0)
    {
        if (result != 3)
        {
            if (WALLCR_ShouldJumpOff(instance) != 0)
            {
                ENMYPLAN_GetPosOfNextWaypoint(mv->pathSlotID, &point);

                if ((MON_TurnToPosition(instance, &point, mv->subAttr->speedRunTurn) != 0) && (WALLCR_FindJumpOnPoint(instance, &point, &norm) != 0))
                {
                    WALLCR_AttachEntry(instance, &point, &norm);
                    return 1;
                }
            }
        }
        else if (instance->currentMainState == MONSTER_STATE_SURPRISEATTACK)
        {
            MON_SwitchState(instance, MONSTER_STATE_PURSUE);
            return 1;
        }
    }

    return 0;
}
#endif

void WALLCR_Message(Instance *instance, unsigned long message, unsigned long data)
{
    MonsterMessage(instance, message, data);
}

unsigned long WALLCR_Query(Instance *instance, unsigned long query)
{
    if (query == 13)
    {
        if (instance->matrix != NULL)
        {
            return (unsigned long)instance->matrix;
        }

        return 0;
    }

    return MonsterQuery(instance, query);
}

void WALLCR_Init(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols
    WallcrVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    MON_DefaultInit(instance);

    G2Anim_AttachControllerToSeg(&instance->anim, 1, 38);
    G2Anim_AttachControllerToSeg(&instance->anim, 0, 14);
    G2Anim_AttachControllerToSeg(&instance->anim, 18, 14);

    G2Anim_DisableController(&instance->anim, 1, 38);
    G2Anim_DisableController(&instance->anim, 0, 14);
    G2Anim_DisableController(&instance->anim, 18, 14);

    mv->auxFlags &= ~0x20;

    mv = (MonsterVars *)instance->extraData;

    if (mv != NULL)
    {
        vars = (WallcrVars *)MEMPACK_Malloc(sizeof(WallcrVars), MEMORY_TYPE_WALLCRDATA);

        if (vars == NULL)
        {
            mv->extraVars = NULL;
            return;
        }

        vars->wallcrFlags = 0;
        vars->left_over_time = 0;

        mv->extraVars = vars;

        if ((mv->initialBehavior != 16) && (WALLCR_NearFloor(instance) == 0) && (WALLCR_FollowWall(instance) != 0))
        {
            WALLCR_EnableControllers(instance);

            mv->auxFlags |= 0x4;
            mv->mvFlags |= 0x800;

            MON_SwitchState(instance, MONSTER_STATE_IDLE);
        }
    }
}

void WALLCR_CleanUp(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    G2Anim_DetachControllerFromSeg(&instance->anim, 1, 38);
    G2Anim_DetachControllerFromSeg(&instance->anim, 0, 14);
    G2Anim_DetachControllerFromSeg(&instance->anim, 18, 14);

    mv = (MonsterVars *)instance->extraData;

    if ((mv != NULL) && (mv->extraVars != NULL))
    {
        MEMPACK_Free((char *)mv->extraVars);
    }

    MON_CleanUp(instance);
}

void WALLCR_PursueEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols
    MonsterIR *enemy; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    enemy = mv->enemy;

    if ((mv->auxFlags & 0x4) && !(mv->mvFlags & 0x4))
    {
        MON_GetPlanSlot(mv);

        if (enemy != NULL)
        {
            COPY_SVEC(Position, &mv->destination, Position, &enemy->instance->position);
        }
        else
        {
            COPY_SVEC(Position, &mv->destination, Position, &instance->position);
        }

        mv->mode = 4;

        mv->mvFlags &= ~0x20000;
    }
    else
    {
        MON_PursueEntry(instance);
    }
}

void WALLCR_Pursue(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols
    MonsterIR *enemy; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    enemy = mv->enemy;

    if (mv->mvFlags & 0x4)
    {
        if (mv->auxFlags & 0x4)
        {
            MON_SwitchState(instance, MONSTER_STATE_IDLE);
            MON_DefaultQueueHandler(instance);
        }
        else
        {
            MON_Pursue(instance);
        }
    }
    else
    {
        mv->auxFlags &= ~0x20;

        if (WALLCR_ShouldSurpriseAttack(instance) != 0)
        {
            mv->auxFlags |= 0x20;
            mv->auxFlags &= ~0x10;

            MON_SwitchState(instance, MONSTER_STATE_SURPRISEATTACK);
            MON_DefaultQueueHandler(instance);
        }
        else if (WALLCR_MoveToDestination(instance, NULL) != 0)
        {
            if (enemy != NULL)
            {
                COPY_SVEC(Position, &mv->destination, Position, &enemy->instance->position);
            }
        }
        else if (instance->currentMainState == MONSTER_STATE_PURSUE)
        {
            MON_Pursue(instance);
        }
    }
}

void WALLCR_SurpriseAttackEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (mv->auxFlags & 0x20)
    {
        MON_GetPlanSlot(mv);
    }
    else
    {
        MON_SurpriseAttackEntry(instance);
    }
}

void WALLCR_SurpriseAttack(Instance *instance)
{
    Position pos;
    MonsterVars *mv; // not from debug symbols
    MonsterIR *enemy; // not from debug symbols
    Position *target; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    enemy = mv->enemy;

    if (mv->mvFlags & 0x4)
    {
        MON_SwitchState(instance, MONSTER_STATE_IDLE);
        MON_DefaultQueueHandler(instance);
    }
    else if (mv->auxFlags & 0x20)
    {
        if (WALLCR_MoveToDestination(instance, &pos) == 0)
        {
            if (MON_AnimPlaying(instance, MONSTER_ANIM_RUN) == 0)
            {
                MON_PlayAnim(instance, MONSTER_ANIM_RUN, 2);
            }

            MON_MoveToPosition(instance, &pos, mv->subAttr->speedRunTurn);

            mv->mvFlags |= 0x20000;
        }

        if (instance->currentMainState != MONSTER_STATE_FALL)
        {
            if (mv->auxFlags & 0x10)
            {
                if (!(mv->auxFlags & 0x1))
                {
                    MON_TurnOffWeaponSpheres(instance);

                    mv->auxFlags &= ~0x10;

                    MONSENSE_SetEnemy(instance, gameTrackerX.playerInstance);

                    MON_SwitchState(instance, MONSTER_STATE_PURSUE);
                }
            }
            else if (MATH3D_LengthXYZ(instance->position.x - mv->destination.x, instance->position.y - mv->destination.y, instance->position.z - mv->destination.z) < 300)
            {
                target = NULL;

                if ((enemy != NULL) && (instance->position.z > mv->destination.z))
                {
                    target = &enemy->instance->position;
                }

                mv->auxFlags |= 0x10;

                WALLCR_DetachEntry(instance, target);

                MON_TurnOnWeaponSpheres(instance);
            }
        }
    }
    else
    {
        MON_SurpriseAttack(instance);
    }
}

void WALLCR_IdleEntry(Instance *instance)
{
    if (((MonsterVars *)instance->extraData)->auxFlags & 0x4)
    {
        MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 6, 2);
    }
    else
    {
        MON_PlayRandomIdle(instance, 2);
        MON_IdleEntry(instance);
    }
}

int WALLCR_ShouldIAmbushPlayer(Instance *instance)
{
    Position pos;
    MonsterVars *mv; // not from debug symbols
    Instance *player; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    player = gameTrackerX.playerInstance;

    if ((mv->ambushMarker != 0) && (*(long *)&mv->ambushArc == 0x4000800))
    {
        if (PLANAPI_FindNodePositionInUnit(STREAM_GetStreamUnitWithID(instance->birthStreamUnitID), &pos, mv->ambushMarker, 4) != 0)
        {
            return MATH3D_LengthXYZ(player->position.x - pos.x, player->position.y - pos.y, player->position.z - pos.z) < mv->ambushRange;
        }
    }
    else if (instance->currentMainState != MONSTER_STATE_PUPATE)
    {
        return MATH3D_LengthXYZ(player->position.x - instance->position.x, player->position.y - instance->position.y, player->position.z - instance->position.z) < mv->ambushRange;
    }

    return 0;
}

void WALLCR_Idle(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols
    MonsterIR *enemy; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    enemy = mv->enemy;

    if (mv->mvFlags & 0x4)
    {
        if (mv->auxFlags & 0x4)
        {
            WALLCR_DetachEntry(instance, NULL);
        }

        if (mv->auxFlags & 0x1)
        {
            MON_DefaultQueueHandler(instance);

            WALLCR_Detach(instance);

            if (mv->auxFlags & 0x1)
            {
                return;
            }

            MON_IdleEntry(instance);
        }

        MON_Idle(instance);
    }
    else if (mv->auxFlags & 0x4)
    {
        MON_DefaultQueueHandler(instance);

        if (enemy != NULL)
        {
            MON_SwitchState(instance, MONSTER_STATE_PURSUE);
        }
        else if (WALLCR_ShouldIAmbushPlayer(instance) != 0)
        {
            mv->auxFlags |= 0x30;

            WALLCR_DetachEntry(instance, &gameTrackerX.playerInstance->position);

            MON_SwitchState(instance, MONSTER_STATE_SURPRISEATTACK);
        }
        else if (MON_AnimPlayingFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 7) == 0)
        {
            if (rand() < 109)
            {
                MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 7, 1);
            }
        }
        else if (instance->flags2 & 0x10)
        {
            MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 6, 2);
        }
    }
    else
    {
        MON_Idle(instance);
    }
}

void WALLCR_WanderEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (mv->mvFlags & 0x4)
    {
        MON_GetPlanSlot(mv);
    }
    else if (!(mv->auxFlags & 0x4))
    {
        MON_WanderEntry(instance);
    }
    else
    {
        WALLCR_DetachEntry(instance, NULL);
    }
}

void WALLCR_Wander(Instance *instance)
{
    Position pos;
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (mv->mvFlags & 0x4)
    {
        if (WALLCR_MoveToDestination(instance, &pos) == 0)
        {
            if (MON_AnimPlaying(instance, MONSTER_ANIM_RUN) == 0)
            {
                MON_PlayAnim(instance, MONSTER_ANIM_RUN, 2);
            }

            MON_MoveToPosition(instance, &pos, mv->subAttr->speedRunTurn);

            mv->mvFlags |= 0x20000;
        }

        if ((instance->currentMainState != MONSTER_STATE_FALL) && (MATH3D_LengthXYZ(instance->position.x - mv->destination.x, instance->position.y - mv->destination.y, instance->position.z - mv->destination.z) < 300))
        {
            MON_SwitchState(instance, MONSTER_STATE_IDLE);
        }
    }
    else if (mv->auxFlags & 0x1)
    {
        MON_DefaultQueueHandler(instance);
        WALLCR_Detach(instance);
    }
    else
    {
        MON_Wander(instance);
    }
}

void WALLCR_FallEntry(Instance *instance)
{
    if (((MonsterVars *)instance->extraData)->auxFlags & 0x4)
    {
        MON_SwitchStateDoEntry(instance, MONSTER_STATE_IDLE);
    }
    else
    {
        MON_FallEntry(instance);
    }
}

void WALLCR_Fall(Instance *instance)
{
    MON_Fall(instance);
}

void WALLCR_ImpactEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols
    WallcrVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (WallcrVars *)mv->extraVars;

    if (vars != NULL)
    {
        if ((WALLCR_FollowWall(instance) != 0) && (instance->tface->textoff != 0xFFFF)
        && (((TextureFT3 *)((char *)((Level *)instance->tfaceLevel)->terrain->StartTextureList + instance->tface->textoff))->attr & 0x800))
        {
            MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 2, 1);

            instance->zAccl = 0;
            instance->zVel = 0;
            instance->yVel = 0;
            instance->xVel = 0;

            vars->roll = 0;
            vars->pitch_speed = 0;
            vars->roll_speed = 0;
            vars->yaw_speed = 0;

            instance->checkMask &= ~0x20;

            mv->mvFlags |= 0x800;
            mv->auxFlags &= ~0x8;
            mv->mvFlags &= ~0x2;
        }
        else
        {
            MON_ImpactEntry(instance);
        }
    }
}

void WALLCR_Impact(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (mv->extraVars != NULL)
    {
        if (MON_AnimPlayingFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 2) != 0)
        {
            if (instance->flags2 & 0x10)
            {
                mv->auxFlags &= ~0x1;
                mv->auxFlags |= 0x4;

                MON_SwitchState(instance, MONSTER_STATE_IDLE);
            }

            MON_DefaultQueueHandler(instance);
        }
        else
        {
            MON_Impact(instance);
        }
    }
}

void WALLCR_GeneralDeathEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    WALLCR_DisableControllers(instance);

    mv->mvFlags &= ~0x800;
    mv->auxFlags &= ~0x4;

    instance->zAccl = -16;

    instance->rotation.y = 0;
    instance->rotation.x = 0;

    MON_GeneralDeathEntry(instance);
}

void WALLCR_GeneralDeath(Instance *instance)
{
    MON_GeneralDeath(instance);
}

void WALLCR_HitEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    WALLCR_DisableControllers(instance);

    mv->mvFlags &= ~0x800;
    mv->auxFlags &= ~0x4;

    instance->zAccl = -16;

    instance->rotation.y = 0;
    instance->rotation.x = 0;

    MON_HitEntry(instance);
}

void WALLCR_Hit(Instance *instance)
{
    MON_Hit(instance);
}

void WALLCR_MissileHitEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    WALLCR_DisableControllers(instance);

    mv->mvFlags &= ~0x800;
    mv->auxFlags &= ~0x4;

    instance->zAccl = -16;

    instance->rotation.y = 0;
    instance->rotation.x = 0;

    MON_MissileHitEntry(instance);
}

void WALLCR_MissileHit(Instance *instance)
{
    MON_MissileHit(instance);
}

void WALLCR_NoticeEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    WALLCR_DisableControllers(instance);

    mv->mvFlags &= ~0x800;
    mv->auxFlags &= ~0x4;

    instance->zAccl = -16;

    instance->rotation.y = 0;
    instance->rotation.x = 0;

    MON_NoticeEntry(instance);
}

void WALLCR_Notice(Instance *instance)
{
    MON_Notice(instance);
}

void WALLCR_SurprisedEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    WALLCR_DisableControllers(instance);

    mv->mvFlags &= ~0x800;
    mv->auxFlags &= ~0x4;

    instance->zAccl = -16;

    instance->rotation.y = 0;
    instance->rotation.x = 0;

    MON_SurprisedEntry(instance);
}

void WALLCR_Surprised(Instance *instance)
{
    MON_Surprised(instance);
}

void WALLCR_StunnedEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    WALLCR_DisableControllers(instance);

    mv->mvFlags &= ~0x800;
    mv->auxFlags &= ~0x4;

    instance->zAccl = -16;

    instance->rotation.y = 0;
    instance->rotation.x = 0;

    MON_StunnedEntry(instance);
}

void WALLCR_Stunned(Instance *instance)
{
    MON_Stunned(instance);
}

void WALLCR_ThrownEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    WALLCR_DisableControllers(instance);

    mv->mvFlags &= ~0x800;
    mv->auxFlags &= ~0x4;

    instance->zAccl = -16;

    instance->rotation.y = 0;
    instance->rotation.x = 0;

    MON_ThrownEntry(instance);
}

void WALLCR_Thrown(Instance *instance)
{
    MessageQueue temp;
    MonsterVars *mv; // not from debug symbols
    Message *message; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (!(mv->mvFlags & 0x2))
    {
        InitMessageQueue(&temp);

        while ((message = DeMessageQueue(&mv->messageQueue)) != NULL)
        {
            if ((message->ID != 0x100000E) && (message->ID != 0x1000012))
            {
                EnMessageQueueData(&temp, message->ID, message->Data);
            }
        }

        while ((message = DeMessageQueue(&temp)) != NULL)
        {
            EnMessageQueueData(&mv->messageQueue, message->ID, message->Data);
        }
    }

    MON_Thrown(instance);
}

void WALLCR_PupateEntry(Instance *instance)
{
    MON_PupateEntry(instance);
}

void WALLCR_Pupate(Instance *instance)
{
    MON_Pupate(instance);

    if (instance->currentMainState != MONSTER_STATE_PUPATE)
    {
        PhysicsSetVelFromRot(instance, &instance->rotation, 15);
    }
}
