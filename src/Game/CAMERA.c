#include "Game/CAMERA.h"
#include "Game/PLAYER.h"
#include "Game/GAMELOOP.h"
#include "Game/MATH3D.h"
#include "Game/STREAM.h"
#include "Game/COLLIDE.h"
#include "Game/LIGHT3D.h"
#include "Game/SPLINE.h"
#include "Game/GAMEPAD.h"
#include "Game/MEMPACK.h"
#include "Game/HASM.h"
#include "Game/INSTANCE.h"
#include "Game/PSX/MAIN.h"
#include "Game/RAZIEL/RAZIEL.h"

long camera_modeToIndex[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0};

static short shorten_count = 0;

static short shorten_flag = 0;

static short camera_still = 0;

static long roll_target = 0;

static long current_roll_amount = 0;

static int roll_inc = 0;

static MATRIX wcTransformX;

static MATRIX wcTransform2X;

static MATRIX cwTransform2X;

static short combat_cam_distance;

static short combat_cam_angle;

static short combat_cam_weight;

static short combat_cam_debounce;

static short CenterFlag;

static SVector camera_shakeOffset[16];

static SVector camera_plane;

static SVector left_point;

static SVector right_point;

static short hitline_rot;

static long ACE_amount;

static Rotation splinecam_helprot;

static short Camera_lookHeight;

static short Camera_lookDist;

static inline int GetSecondCheckFlag(Camera *camera)
{
    if ((camera->flags & 0x10000))
    {
        return 0;
    }

    if ((camera->real_focuspoint.z - camera->targetFocusPoint.z) >= 0)
    {
        if ((camera->real_focuspoint.z - camera->targetFocusPoint.z) < 5)
        {
            return 0;
        }
    }
    else if ((camera->targetFocusPoint.z - camera->real_focuspoint.z) < 5)
    {
        return 0;
    }

    return 1;
}

void CAMERA_CalculateViewVolumeNormals(Camera *camera)
{
    short projDistance;
    Normal n0;
    Normal n1;
    Normal n2;
    Normal n3;
    int x1;
    int x2;
    int y1;
    int y2;

    projDistance = camera->core.projDistance;

    x1 = (camera->core.leftX - 160) << 4;
    x2 = (camera->core.rightX - 160) << 4;

    y1 = (camera->core.topY - 120) << 4;
    y2 = (camera->core.bottomY - 120) << 4;

    n0.x = x1;
    n0.y = y1;
    n0.z = (projDistance << 16) >> 12;

    n1.x = x2;
    n1.y = y1;
    n1.z = (projDistance << 16) >> 12;

    n2.x = x1;
    n2.y = y2;
    n2.z = (projDistance << 16) >> 12;

    n3.x = x2;
    n3.y = y2;
    n3.z = (projDistance << 16) >> 12;

    camera->core.viewVolumeNormal[0].x = 0;
    camera->core.viewVolumeNormal[0].y = 0;
    camera->core.viewVolumeNormal[0].z = 4096;

    camera->core.viewVolumeNormal[1].x = (((n0.y * n1.z) - (n0.z * n1.y)) >> 12);
    camera->core.viewVolumeNormal[1].y = -(((n0.x * n1.z) - (n0.z * n1.x)) >> 12);
    camera->core.viewVolumeNormal[1].z = (((n0.x * n1.y) - (n0.y * n1.x)) >> 12);

    CAMERA_Normalize((SVector *)&camera->core.viewVolumeNormal[1]);

    camera->core.viewVolumeNormal[2].x = (((n2.y * n0.z) - (n2.z * n0.y)) >> 12);
    camera->core.viewVolumeNormal[2].y = -(((n2.x * n0.z) - (n2.z * n0.x)) >> 12);
    camera->core.viewVolumeNormal[2].z = (((n2.x * n0.y) - (n2.y * n0.x)) >> 12);

    CAMERA_Normalize((SVector *)&camera->core.viewVolumeNormal[2]);

    camera->core.viewVolumeNormal[3].x = (((n1.y * n3.z) - (n1.z * n3.y)) >> 12);
    camera->core.viewVolumeNormal[3].y = -(((n1.x * n3.z) - (n1.z * n3.x)) >> 12);
    camera->core.viewVolumeNormal[3].z = (((n1.x * n3.y) - (n1.y * n3.x)) >> 12);

    CAMERA_Normalize((SVector *)&camera->core.viewVolumeNormal[3]);

    camera->core.viewVolumeNormal[4].x = (((n3.y * n2.z) - (n3.z * n2.y)) >> 12);
    camera->core.viewVolumeNormal[4].y = -(((n3.x * n2.z) - (n3.z * n2.x)) >> 12);
    camera->core.viewVolumeNormal[4].z = (((n3.x * n2.y) - (n3.y * n2.x)) >> 12);

    CAMERA_Normalize((SVector *)&camera->core.viewVolumeNormal[4]);
}

void CAMERA_CalcVVClipInfo(Camera *camera)
{
    ApplyMatrixSV(camera->core.cwTransform2, (SVECTOR *)&camera->core.viewVolumeNormal[0], (SVECTOR *)&camera->core.vvNormalWorVecMat[0].m[0][0]);
    ApplyMatrixSV(camera->core.cwTransform2, (SVECTOR *)&camera->core.viewVolumeNormal[1], (SVECTOR *)&camera->core.vvNormalWorVecMat[0].m[1][0]);
    ApplyMatrixSV(camera->core.cwTransform2, (SVECTOR *)&camera->core.viewVolumeNormal[2], (SVECTOR *)&camera->core.vvNormalWorVecMat[0].m[2][0]);
    ApplyMatrixSV(camera->core.cwTransform2, (SVECTOR *)&camera->core.viewVolumeNormal[3], (SVECTOR *)&camera->core.vvNormalWorVecMat[1].m[0][0]);
    ApplyMatrixSV(camera->core.cwTransform2, (SVECTOR *)&camera->core.viewVolumeNormal[4], (SVECTOR *)&camera->core.vvNormalWorVecMat[1].m[1][0]);

    gte_SetRotMatrix(&camera->core.vvNormalWorVecMat[0]);
    gte_ldv0(&camera->core.position);
    gte_nmvmva(1, 0, 0, 3, 0);
    gte_stlvnl(&camera->core.vvPlaneConsts[0]);

    gte_SetRotMatrix(&camera->core.vvNormalWorVecMat[1]);
    gte_ldv0(&camera->core.position);
    gte_nmvmva(1, 0, 0, 3, 0);
    gte_stlvnl(&camera->core.vvPlaneConsts[3]);
}

void CAMERA_SetViewVolume(Camera *camera)
{
    CAMERA_CalculateViewVolumeNormals(camera);

    CAMERA_CalcVVClipInfo(camera);
}

void CAMERA_SetProjDistance(Camera *camera, long distance)
{
    Level *level;
    int i;

    SetGeomScreen(distance);

    camera->core.projDistance = distance;

    CAMERA_CalculateViewVolumeNormals(camera);

    for (i = 0; i < 16; i++)
    {
        if (StreamTracker.StreamList[i].used == 2)
        {
            level = StreamTracker.StreamList[i].level;

            SetFogNearFar(level->fogNear, level->fogFar, camera->core.projDistance);

            LIGHT_CalcDQPTable(level);
        }
    }
}

void CAMERA_CreateNewFocuspoint(Camera *camera)
{
    SVector sv;

    camera->focusRotation.x = camera->actual_x_rot;

    CAMERA_CalcPosition(&camera->targetPos, &camera->core.position, &camera->focusRotation, camera->focusDistance);

    SUB_SVEC(SVector, &sv, Position, &camera->core.position, Position, &camera->targetPos);

    ADD_SVEC(Position, &camera->focusPoint, Position, &camera->core.position, SVector, &sv);
}

void CAMERA_SaveMode(Camera *camera, long mode)
{
    long i;

    if (++camera->stack >= 3)
    {
        camera->stack = 2;

        for (i = 0; i < 2; i++)
        {
            camera->savedMode[i] = camera->savedMode[i + 1];
            camera->savedCinematic[i] = camera->savedCinematic[i + 1];
        }
    }

    camera->savedMode[camera->stack] = mode;

    if ((mode == 2) || (mode == 5) || (mode == 4))
    {
        camera->savedCinematic[camera->stack].position = camera->core.position;

        camera->savedCinematic[camera->stack].focusPoint = camera->focusPoint;

        camera->savedCinematic[camera->stack].targetPos = camera->targetPos;
        camera->savedCinematic[camera->stack].targetFocusPoint = camera->targetFocusPoint;

        camera->savedCinematic[camera->stack].focusRotation = camera->focusRotation;
        camera->savedCinematic[camera->stack].targetFocusRotation = camera->targetFocusRotation;

        camera->savedCinematic[camera->stack].focusPointVel = camera->focusPointVel;
        camera->savedCinematic[camera->stack].focusPointAccl = camera->focusPointAccl;

        camera->savedCinematic[camera->stack].maxVel = camera->maxVel;

        camera->savedCinematic[camera->stack].focusDistance = camera->focusDistance;
        camera->savedCinematic[camera->stack].targetFocusDistance = camera->targetFocusDistance;

        camera->savedCinematic[camera->stack].posSpline = camera->data.Cinematic.posSpline;
        camera->savedCinematic[camera->stack].targetSpline = camera->data.Cinematic.targetSpline;

        camera->savedCinematic[camera->stack].level = gameTrackerX.level;
    }
}

void CAMERA_RestoreMode(Camera *camera)
{
    long mode;
    short temp; // not from decls.h

    if (camera->stack >= 0)
    {
        mode = camera->savedMode[camera->stack];

        if (camera->mode == 5)
        {
            if (camera->smooth != 0)
            {
                camera->smooth = 8;
            }
            else
            {
                camera->flags |= 0x1000;
            }
        }

        switch (mode)
        {
        case 0:
        case 12:
        case 13:
        case 16:
            CAMERA_SetProjDistance(camera, 320);

            if (camera->mode == 5)
            {
                camera->focusInstance = gameTrackerX.playerInstance;

                camera->focusOffset.x = 0;
                camera->focusOffset.y = 0;
                camera->focusOffset.z = 352;

                CAMERA_Restore(camera, 7);
            }

            cameraMode = mode;

            if (mode == 12)
            {
                gameTrackerX.gameFlags &= ~0x40;
            }
            else
            {
                gameTrackerX.gameFlags |= 0x40;
            }

            camera->mode = (short)mode;

            camera->targetFocusDistance = (short)camera->focusDistanceList[camera_modeToIndex[(short)mode]][camera->presetIndex];

            camera->data.Follow.stopTimer = 0xE5A20000;

            camera->focusRotVel.z = 0;

            if (mode == 16)
            {
                camera->flags |= 0x2000;
            }
            else
            {
                camera->flags &= ~0x2000;
            }

            break;
        case 2:
        case 4:
        case 5:
            CAMERA_SetProjDistance(camera, 320);

            camera->core.position = camera->savedCinematic[camera->stack].position;

            camera->focusPoint = camera->savedCinematic[camera->stack].focusPoint;

            camera->targetPos = camera->savedCinematic[camera->stack].targetPos;

            camera->targetFocusPoint = camera->savedCinematic[camera->stack].targetFocusPoint;
            camera->targetFocusDistance = camera->savedCinematic[camera->stack].targetFocusDistance;
            camera->targetFocusRotation = camera->savedCinematic[camera->stack].targetFocusRotation;

            if ((camera->smooth == 0) && (camera->mode != 6))
            {
                camera->focusDistance = camera->savedCinematic[camera->stack].focusDistance;
                camera->focusRotation = camera->savedCinematic[camera->stack].focusRotation;
            }
            else
            {
                camera->always_rotate_flag = 1;
            }

            camera->focusPointVel = camera->savedCinematic[camera->stack].focusPointVel;
            camera->focusPointAccl = camera->savedCinematic[camera->stack].focusPointAccl;

            camera->maxVel = (short)camera->savedCinematic[camera->stack].maxVel;

            camera->data.Cinematic.posSpline = camera->savedCinematic[camera->stack].posSpline;
            camera->data.Cinematic.targetSpline = camera->savedCinematic[camera->stack].targetSpline;

            camera->mode = (short)mode;

            if ((INSTANCE_Query(camera->focusInstance, 9) & 0x50))
            {
                CAMERA_ChangeToUnderWater(camera, camera->focusInstance);
            }

            break;
        case 6:
            if ((camera->mode == 4) || (camera->mode == 5) || (camera->mode == 2))
            {
                temp = (short)camera->savedTargetFocusDistance[camera->targetStack];

                camera->focusDistance = temp;

                camera->targetFocusDistance = temp;

                if (camera->targetStack >= 0)
                {
                    camera->targetStack--;
                }

                camera->flags |= 0x800;
            }

            camera->lookTimer = 4;

            camera->mode = (short)mode;

            camera->targetFocusDistance = 2000;
            break;
        case 1:
        case 3:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 14:
        case 15:
        default:
            camera->mode = (short)mode;
        }

        camera->stack--;
    }
}

void CAMERA_Save(Camera *camera, long save)
{
    int i;

    if ((save & 0x1))
    {
        camera->targetStack++;

        if (camera->targetStack == 3)
        {
            camera->targetStack = 2;

            for (i = 0; i < 2; i++)
            {
                camera->savedTargetFocusDistance[i] = camera->savedTargetFocusDistance[i + 1];
            }
        }

        camera->savedTargetFocusDistance[camera->targetStack] = camera->targetFocusDistance;
    }

    if ((save & 0x2))
    {
        camera->savedfocusRotation.x = camera->targetFocusRotation.x;
    }

    if ((save & 0x4))
    {
        camera->savedfocusRotation.z = camera->focusRotation.z;
    }

    if ((save & 0x100))
    {
        CAMERA_SaveMode(camera, camera->mode);
    }
}

void CAMERA_Restore(Camera *camera, long restore)
{
    if ((restore & 0x7))
    {
        if ((restore & 0x1))
        {
            if (camera->targetStack >= 0)
            {
                camera->distanceState = 3;

                camera->signalFocusDistance = (short)camera->savedTargetFocusDistance[camera->targetStack];

                if (camera->targetStack >= 0)
                {
                    camera->targetStack -= 1;
                }
            }
        }

        if ((restore & 0x2))
        {
            camera->tiltState = 3;

            camera->signalRot.x = camera->savedfocusRotation.x;
        }

        if ((restore & 0x4))
        {
            camera->rotState = 3;

            camera->forced_movement = 0;

            camera->always_rotate_flag = 1;

            camera->signalRot.z = camera->savedfocusRotation.z;

            camera->teleportZRot = camera->signalRot.z;
        }
    }

    if ((restore & 0x100))
    {
        CAMERA_RestoreMode(camera);
    }
}

SVector *SplineGetNextPointDC(Spline *spline, SplineDef *def)
{
    static SVector point;

    if (SplineGetOffsetNext(spline, def, gameTrackerX.timeMult) != 0)
    {
        if (SplineGetData(spline, def, &point) != 0)
        {
            return &point;
        }
    }

    return NULL;
}

void CAMERA_SetMode(Camera *camera, long mode)
{
    int oldMode;
    SplineDef curPositional;
    SVector sv;

    oldMode = camera->mode;

    if ((oldMode != mode) || ((mode < 12) || (mode >= 14)))
    {
        switch (camera->mode)
        {
        case 6:
            CAMERA_EndLook(camera);

            CAMERA_SaveMode(camera, camera->mode);

            if (mode == 5)
            {
                CenterFlag = ~0;

                camera->focusRotation.z = camera->targetFocusRotation.z;

                CAMERA_Save(camera, 7);
            }

            break;
        case 2:
        case 4:
            CAMERA_SaveMode(camera, camera->mode);
            break;
        case 5:
            if (mode != 5)
            {
                CAMERA_SaveMode(camera, camera->mode);

                COPY_SVEC(Rotation, &camera->targetFocusRotation, Rotation, &camera->core.rotation);
                COPY_SVEC(Rotation, &camera->focusRotation, Rotation, &camera->core.rotation);

                camera->actual_x_rot = camera->core.rotation.x;

                SUB_SVEC(SVector, &sv, Position, &camera->focusPoint, Position, &camera->core.position);

                camera->focusDistance = (short)CAMERA_LengthSVector(&sv);

                roll_target = 0;

                current_roll_amount = 0;

                roll_inc = 0;
            }

            break;
        case 12:
        case 13:
            CAMERA_SaveMode(camera, camera->mode);

            if (mode == 5)
            {
                CAMERA_Save(camera, 7);
            }

            CAMERA_CreateNewFocuspoint(camera);
        default:
            break;
        }

        switch (mode)
        {
        case 2:
        case 4:
        case 5:
            CAMERA_SetProjDistance(camera, 320);

            camera->data.Cinematic.posSpline = camera->Spline00;
            camera->data.Cinematic.targetSpline = camera->Spline01;

            camera->data.Cinematic.cinema_done = 0;

            camera->Spline00 = NULL;

            camera->data.Cinematic.lastSplinePos.z = 0;
            camera->data.Cinematic.lastSplinePos.y = 0;
            camera->data.Cinematic.lastSplinePos.x = 0;

            camera->Spline01 = NULL;

            if (camera->data.Cinematic.posSpline != NULL)
            {
                *(int *)&camera->data.Cinematic.posSpline->curPositional.currkey = 0; // double-check
            }

            if (camera->data.Cinematic.targetSpline != NULL)
            {
                *(int *)&camera->data.Cinematic.targetSpline->curPositional.currkey = 0; // double-check
            }

            camera->mode = (short)mode;

            if (camera->data.Cinematic.posSpline != NULL)
            {
                SVector *camPos;
                SVector *targetPos;

                if ((mode == 4) || (mode == 2))
                {
                    Position pos;

                    splinecam_helprot.x = camera->focusRotation.x;
                    splinecam_helprot.z = (camera->focusRotation.z + 2048) & 0xFFF;

                    CAMERA_CalcPosition(&pos, &camera->targetFocusPoint, &splinecam_helprot, camera->targetFocusDistance);

                    camPos = SplineGetNearestPoint(camera->data.Cinematic.posSpline->positional, (SVector *)&pos, &curPositional);

                    CAMERA_CalcRotation(&splinecam_helprot, &camera->focusInstance->position, (Position *)&camPos);

                    camera->data.Cinematic.splinecam_helpkey = curPositional.currkey;

                    camPos = SplineGetNearestPoint(camera->data.Cinematic.posSpline->positional, (SVector *)&camera->core.position, &curPositional);

                    camera->data.Cinematic.splinecam_currkey = curPositional.currkey;
                }
                else
                {
                    camPos = SplineGetFirstPoint(camera->data.Cinematic.posSpline->positional, &camera->data.Cinematic.posSpline->curPositional);
                }

                if (camPos != NULL)
                {
                    COPY_SVEC(Position, &camera->targetPos, SVector, camPos);

                    if (mode == 5)
                    {
                        camera->posState = 3;
                    }

                    if (camera->data.Cinematic.targetSpline != NULL)
                    {
                        targetPos = SplineGetFirstPoint(camera->data.Cinematic.targetSpline->positional, &camera->data.Cinematic.targetSpline->curPositional);

                        if (targetPos != NULL)
                        {
                            COPY_SVEC(Position, &camera->targetFocusPoint, SVector, targetPos);
                        }
                    }
                }
            }

            camera->targetRotation = camera->focusRotation;

            CenterFlag = ~0;

            camera->tiltState = 0;

            camera->lead_angle = 0;

            camera->flags &= ~0x2000;
            break;
        case 12:
        case 13:
        case 16:
            CAMERA_SetProjDistance(camera, 320);

            if (mode == 16)
            {
                mode = 12;

                camera->flags |= 0x2000;
            }
            else
            {
                camera->flags &= ~0x2000;
            }

            cameraMode = mode;

            gameTrackerX.gameFlags &= ~0x40;

            camera->mode = (short)mode;

            camera->targetFocusDistance = (short)camera->focusDistanceList[camera_modeToIndex[(short)mode]][camera->presetIndex];

            camera->smooth = 8;

            camera->data.Follow.stopTimer = 0xE5A20000;

            camera->focusRotVel.z = 0;

            if ((oldMode == 5) && (camera->focusInstance != NULL))
            {
                CAMERA_SetFocus(camera, &camera->targetFocusPoint);
            }

            break;
        case 0:
        case 10:
        case 11:
            break;
        default:
            camera->mode = mode;
        }

        camera->collisionTargetFocusDistance = camera->targetFocusDistance;
        camera->collisionTargetFocusRotation = camera->targetFocusRotation;
    }
}

void CAMERA_Initialize(Camera *camera)
{
    long i;

    memset(camera, 0, sizeof(Camera));

    for (i = 0; i < 16; i++)
    {
        camera_shakeOffset[i].x = (rand() % 0x100) - 128;
        camera_shakeOffset[i].y = (rand() % 0x100) - 128;
        camera_shakeOffset[i].z = (rand() % 0x100) - 128;
    }

    camera->core.rotation.x = 4039;
    camera->targetRotation.x = 4039;
    camera->focusRotation.x = 4039;
    camera->targetFocusRotation.x = 4039;

    camera->focusDistanceList[0][0] = 1500;
    camera->focusDistanceList[0][1] = 2250;
    camera->focusDistanceList[0][2] = 3200;
    camera->focusDistanceList[1][0] = 1500;
    camera->focusDistanceList[1][1] = 2000;
    camera->focusDistanceList[1][2] = 2600;
    camera->focusDistanceList[2][0] = 1200;
    camera->focusDistanceList[2][1] = 1600;
    camera->focusDistanceList[2][2] = 2000;
    camera->focusDistance = camera->targetFocusDistance = camera->focusDistanceList[0][1];

    camera->tiltList[0][0] = 4039;
    camera->tiltList[0][1] = 4039;
    camera->tiltList[0][2] = 4039;
    camera->tiltList[1][0] = 4039;
    camera->tiltList[1][1] = 4039;
    camera->tiltList[1][2] = 4039;
    camera->tiltList[2][0] = 4039;
    camera->tiltList[2][1] = 4039;
    camera->tiltList[2][2] = 4039;

    camera->smallBaseSphere.radiusSquared = 78400;
    camera->focusSphere.radiusSquared = 78400;
    camera->posSphere.radiusSquared = 78400;
    camera->smallBaseSphere.radius = 280;
    camera->focusSphere.radius = 280;
    camera->posSphere.radius = 280;

    camera->core.projDistance = 320;
    camera->core.nearPlane = 50;
    camera->core.farPlane = 12000;
    camera->core.bottomY = 240;
    camera->core.wcTransform = &wcTransformX;
    camera->core.wcTransform2 = &wcTransform2X;
    camera->core.cwTransform2 = &cwTransform2X;
    camera->smooth = 16;
    camera->core.leftX = 0;
    camera->core.rightX = 320;
    camera->core.topY = 0;
    camera->maxVel = 200;
    camera->always_rotate_flag = 0;
    camera->follow_flag = 0;
    camera->real_focuspoint = camera->focusPoint;
    camera->focuspoint_fallz = camera->focusPoint.z;
    camera->Spline01 = NULL;
    camera->Spline00 = NULL;
    shorten_count = 0;
    shorten_flag = 0;
    camera->maxFocusDistance = 4096;
    camera->minFocusDistance = 512;
    camera->flags |= 0x800;

    if (camera->focusInstance != NULL)
    {
        CAMERA_EndLook(camera);
    }

    camera->presetIndex = 1;
    camera->mode = 0;

    CAMERA_SetMode(camera, playerCameraMode);

    camera->maxXYDist = 3000;
    camera->minXYDist = 0;
    camera->rotDirection = 1;

    camera->core.screenScale.z = 4096;
    camera->core.screenScale.y = 4096;
    camera->core.screenScale.x = 4096;

    camera->stack = -1;
    camera->targetStack = -1;

    camera->flags |= 0x8000;

    for (i = 0; i < 3; i++)
    {
        camera->savedMode[i] = 0;
    }

    camera->core.projDistance = 320;

    CAMERA_SetProjDistance(camera, 320);

    camera->data.Cinematic.cinema_done = 0;

    Camera_lookHeight = 512;
    Camera_lookDist = 650;
    CameraCenterDelay = 10;
    CenterFlag = -1;
    combat_cam_distance = 3000;
    roll_target = 0;
    current_roll_amount = 0;
    roll_inc = 0;
    combat_cam_angle = 0;
    combat_cam_weight = 4096;
    combat_cam_debounce = 0;
}

void CAMERA_SetInstanceFocus(Camera *camera, Instance *instance)
{
    camera->focusInstance = instance;

    camera->flags |= 0x800;

    camera->newFocusInstancePos = instance->position;

    if (instance->object != NULL)
    {
        if (instance == gameTrackerX.playerInstance)
        {
            camera->focusOffset.x = 0;
            camera->focusOffset.y = 0;
            camera->focusOffset.z = 352;
        }
        else
        {
            camera->focusOffset.x = 0;
            camera->focusOffset.y = 0;
            camera->focusOffset.z = 512;
        }
    }
}

void CAMERA_SetZRotation(Camera *camera, short zrot)
{
    camera->core.rotation.z = zrot;

    camera->focusRotation.z = zrot;

    camera->targetRotation.z = zrot;

    camera->targetFocusRotation.z = zrot;

    camera->rotationAccl.z = 0;
    camera->rotationVel.z = 0;

    camera->rotationAccl.z = 0;
    camera->rotationVel.z = 0;
}

long CAMERA_LengthSVector(SVector *sv)
{
    return MATH3D_FastSqrt0((sv->x * sv->x) + (sv->y * sv->y) + (sv->z * sv->z));
}

void CAMERA_SetValue(Camera *camera, long index, long value)
{
    long min;
    long max;
    long minTilt;
    long maxTilt;
    long *temp; // not from decls.h

    min = 0;
    max = 16000;

    minTilt = -4096;
    maxTilt = 4096;

    temp = &camera->minFocusDistance;

    temp[index] = value;

    if (camera->cineControl != 0)
    {
        gameTrackerX.gameFlags |= 0x80;
    }
    else
    {
        gameTrackerX.gameFlags &= ~0x80;
    }

    switch (camera->mode)
    {
    case 0:
        if (camera->focusDistanceList[0][0] < camera->focusDistanceList[0][1])
        {
            min = MIN(camera->focusDistanceList[0][0], camera->focusDistanceList[0][2]);
        }
        else
        {
            min = MIN(camera->focusDistanceList[0][1], camera->focusDistanceList[0][2]);
        }

        if (camera->focusDistanceList[0][0] > camera->focusDistanceList[0][1])
        {
            max = MAX(camera->focusDistanceList[0][0], camera->focusDistanceList[0][2]);
        }
        else
        {
            max = MAX(camera->focusDistanceList[0][1], camera->focusDistanceList[0][2]);
        }

        if (camera->tiltList[0][0] < camera->tiltList[0][1])
        {
            minTilt = MIN(camera->tiltList[0][0], camera->tiltList[0][2]);
        }
        else
        {
            minTilt = MIN(camera->tiltList[0][1], camera->tiltList[0][2]);
        }

        if (camera->tiltList[0][0] > camera->tiltList[0][1])
        {
            maxTilt = MAX(camera->tiltList[0][0], camera->tiltList[0][2]);
        }
        else
        {
            maxTilt = MAX(camera->tiltList[0][1], camera->tiltList[0][2]);
        }

        break;
    case 12:
        if (camera->focusDistanceList[1][0] < camera->focusDistanceList[1][1])
        {
            min = MIN(camera->focusDistanceList[1][0], camera->focusDistanceList[1][2]);
        }
        else
        {
            min = MIN(camera->focusDistanceList[1][1], camera->focusDistanceList[1][2]);
        }

        if (camera->focusDistanceList[1][0] > camera->focusDistanceList[1][1])
        {
            max = MAX(camera->focusDistanceList[1][0], camera->focusDistanceList[1][2]);
        }
        else
        {
            max = MAX(camera->focusDistanceList[1][1], camera->focusDistanceList[1][2]);
        }

        if (camera->tiltList[1][0] < camera->tiltList[1][1])
        {
            minTilt = MIN(camera->tiltList[1][0], camera->tiltList[1][2]);
        }
        else
        {
            minTilt = MIN(camera->tiltList[1][1], camera->tiltList[1][2]);
        }

        if (camera->tiltList[1][0] > camera->tiltList[1][1])
        {
            maxTilt = MAX(camera->tiltList[1][0], camera->tiltList[1][2]);
        }
        else
        {
            maxTilt = MAX(camera->tiltList[1][1], camera->tiltList[1][2]);
        }

        break;
    case 13:
        if (camera->focusDistanceList[2][0] < camera->focusDistanceList[2][1])
        {
            min = MIN(camera->focusDistanceList[2][0], camera->focusDistanceList[2][2]);
        }
        else
        {
            min = MIN(camera->focusDistanceList[2][1], camera->focusDistanceList[2][2]);
        }

        if (camera->focusDistanceList[2][0] > camera->focusDistanceList[2][1])
        {
            max = MAX(camera->focusDistanceList[2][0], camera->focusDistanceList[2][2]);
        }
        else
        {
            max = MAX(camera->focusDistanceList[2][1], camera->focusDistanceList[2][2]);
        }

        if (camera->tiltList[2][0] < camera->tiltList[2][1])
        {
            minTilt = MIN(camera->tiltList[2][0], camera->tiltList[2][2]);
        }
        else
        {
            minTilt = MIN(camera->tiltList[2][1], camera->tiltList[2][2]);
        }

        if (camera->tiltList[2][0] > camera->tiltList[2][1])
        {
            maxTilt = MAX(camera->tiltList[2][0], camera->tiltList[2][2]);
        }
        else
        {
            maxTilt = MAX(camera->tiltList[2][1], camera->tiltList[2][2]);
        }

        break;
    }

    if (camera->targetFocusDistance < min)
    {
        camera->targetFocusDistance = (short)min;
    }
    else if (camera->targetFocusDistance > max)
    {
        camera->targetFocusDistance = (short)max;
    }

    if (camera->targetFocusRotation.x < minTilt)
    {
        camera->targetFocusRotation.x = (short)minTilt;
    }
    else if (camera->targetFocusRotation.x > maxTilt)
    {
        camera->targetFocusRotation.x = (short)maxTilt;
    }
}

short CAMERA_AngleDifference(short angle0, short angle1)
{
    angle0 &= 0xFFF;
    angle1 &= 0xFFF;

    if ((angle1 - angle0) > 2048)
    {
        angle0 |= 0x1000;
    }
    else if ((angle0 - angle1) > 2048)
    {
        angle1 |= 0x1000;
    }

    return (MAX(angle0, angle1) - MIN(angle0, angle1));
}

short CAMERA_SignedAngleDifference(short angle0, short angle1)
{
    return AngleDiff(angle1, angle0);
}

unsigned long CAMERA_QueryMode(Camera *camera)
{
    unsigned long mode;

    mode = INSTANCE_Query(camera->focusInstance, 10);

    if (camera->focusInstance == gameTrackerX.playerInstance)
    {
        if ((mode & 0x2000000))
        {
            combat_cam_debounce = 1;
        }
        else if (combat_cam_debounce > 0)
        {
            combat_cam_debounce--;

            mode |= 0x2000000;
        }
    }
    else
    {
        mode &= ~0x2000000;
    }

    if (WARPGATE_IsWarpgateActive() != 0)
    {
        mode |= 0x80000000;
    }

    return mode;
}

void CAMERA_SetMaxVel(Camera *camera)
{
    long extraVel;
    long targetMaxVel;
    SVector cam_dist;
    static long maxVelAccl;
    static long maxVelVel;

    SUB_SVEC(SVector, &cam_dist, Position, &camera->focusPoint, Position, &camera->targetFocusPoint);

    extraVel = camera->focusDistance / 100;

    if (extraVel < 20)
    {
        extraVel = 20;
    }

    if (camera->forced_movement != 0)
    {
        extraVel += extraVel * 4;
    }

    targetMaxVel = ((short)CAMERA_LengthSVector(&cam_dist) + extraVel - camera->maxVel) >> 2;

    targetMaxVel -= maxVelVel;

    maxVelVel += targetMaxVel;

    maxVelAccl = targetMaxVel;
    (void)maxVelAccl;

    camera->maxVel += (short)maxVelVel;

    if (camera->maxVel <= 0)
    {
        camera->maxVel = 1;
    }
}

void CAMERA_SetTarget(Camera *camera, Position *pos)
{
    SVector sv;
    long len;

    CAMERA_CalcRotation(&camera->targetFocusRotation, pos, &camera->core.position);

    SUB_SVEC(SVector, &sv, Position, pos, Position, &camera->core.position);

    len = CAMERA_LengthSVector(&sv);

    camera->focusDistance = (short)len;

    camera->targetFocusDistance = (short)len;

    camera->collisionTargetFocusDistance = (short)len;
}

void CAMERA_CalcPosition(Position *position, Position *base, Rotation *rotation, short distance)
{
    SVECTOR sv;
    VECTOR v;
    MATRIX matrix;
    Vector vectorPos;

    distance = -distance;

    sv.vx = 0;
    sv.vy = distance;
    sv.vz = 0;

    MATH3D_SetUnityMatrix(&matrix);

    RotMatrixX(rotation->x, &matrix);
    RotMatrixY(rotation->y, &matrix);
    RotMatrixZ(rotation->z, &matrix);

    gte_SetRotMatrix(&matrix);
    gte_ldv0(&sv);
    gte_nrtv0();
    gte_stlvnl(&v);

    vectorPos.x = v.vx + base->x;
    vectorPos.y = v.vy + base->y;
    vectorPos.z = v.vz + base->z;

    COPY_SVEC(Position, position, Vector, &vectorPos);
}

void CAMERA_SetFocus(Camera *camera, Position *targetfocusPoint)
{
    Instance *focusInstance;
    Model *model;
    SVector temp1;
    SVector offset;
    SVector *segPosVector;
    Vector temp2;

    focusInstance = camera->focusInstance;

    if (((camera->flags & 0x10000)) || ((camera->instance_mode & 0x4000000)))
    {
        model = focusInstance->object->modelList[focusInstance->currentModel];

        segPosVector = (SVector *)&model->segmentList[1].px;

        COPY_SVEC(SVector, &temp1, SVector, segPosVector);

        ApplyMatrix(&focusInstance->matrix[1], (SVECTOR *)&temp1, (VECTOR *)&temp2);

        COPY_SVEC(Position, targetfocusPoint, Position, &focusInstance->position);

        if ((camera->flags & 0x10000))
        {
            ADD_SVEC(Position, targetfocusPoint, Position, targetfocusPoint, SVector, &temp1);

            if ((INSTANCE_Query(focusInstance, 9) & 0x40))
            {
                targetfocusPoint->z += 192;
            }
        }

        camera->real_focuspoint = *targetfocusPoint;

        camera->focuspoint_fallz = targetfocusPoint->z;
    }
    else
    {
        Instance *instance;
        SVector output;

        if (((camera->instance_mode & 0x2000000)) && ((camera->mode >= 12) && (camera->mode < 14)) && ((instance = (Instance *)INSTANCE_Query(focusInstance, 34)) != NULL))
        {
            LoadAverageShort12((SVECTOR *)&instance->position, (SVECTOR *)&focusInstance->position, 4096 - combat_cam_weight, combat_cam_weight, (SVECTOR *)&output);

            COPY_SVEC(Position, targetfocusPoint, SVector, &output);
        }
        else
        {
            COPY_SVEC(Position, targetfocusPoint, Position, &focusInstance->position);
        }

        CAMERA_CalcFocusOffset(&offset, camera);

        ADD_SVEC(Position, targetfocusPoint, Position, targetfocusPoint, SVector, &offset);

        camera->real_focuspoint = *targetfocusPoint;

        if ((!(camera->instance_mode & 0x2038)) || (((camera->instance_mode & 0x2000)) && (camera->focusInstanceVelVec.z > 70)))
        {
            camera->focuspoint_fallz = targetfocusPoint->z;
        }
    }
}

void CAMERA_Lock(Camera *camera, long lock)
{
    camera->lock |= lock;
}

void CAMERA_Unlock(Camera *camera, long unlock)
{
    camera->lock &= ~unlock;
}

void CAMERA_SetSmoothValue(Camera *camera, long smooth)
{
    camera->smooth = (short)smooth;

    if ((smooth == 0) && ((camera->mode == 12) || (camera->mode == 13) || (camera->mode == 16)))
    {
        camera->focusPoint = camera->targetFocusPoint;
    }
}

void CAMERA_SetTimer(Camera *camera, long time)
{
    (void)time;

    CAMERA_Save(camera, -1);
}

void CAMERA_Adjust_tilt(Camera *camera, long tilt)
{
    camera->tiltState = 3;

    camera->signalRot.x = tilt & 0xFFF;

    if (camera->smooth == 0)
    {
        camera->tilt = 0;

        camera->actual_x_rot = camera->tfaceTilt = camera->focusRotation.x = camera->targetRotation.x = camera->targetFocusRotation.x = camera->signalRot.x;
    }
}

void CAMERA_Adjust_distance(Camera *camera, long dist)
{
    camera->signalFocusDistance = (short)dist;

    camera->distanceState = 3;

    if (camera->maxFocusDistance < (short)dist)
    {
        camera->signalFocusDistance = (short)camera->maxFocusDistance;
    }
    else if ((short)dist < camera->minFocusDistance)
    {
        camera->signalFocusDistance = (short)camera->minFocusDistance;
    }

    if (camera->smooth == 0)
    {
        camera->collisionTargetFocusDistance = camera->targetFocusDistance = camera->focusDistance = camera->signalFocusDistance;
    }
}

void CAMERA_Adjust_rotation(Camera *camera, long rotation)
{
    if (CAMERA_AngleDifference((short)rotation, camera->targetFocusRotation.z))
    {
        camera->rotState = 3;
        camera->forced_movement = 0;
        camera->signalRot.z = rotation & 0xFFF;

        if (!camera->smooth)
        {
            camera->teleportZRot = camera->focusRotation.z = camera->targetFocusRotation.z = camera->signalRot.z;
            camera->collisionTargetFocusRotation.z = camera->signalRot.z;
        }

        camera->lastModTime = gameTrackerX.frameCount;
        camera->always_rotate_flag = 1;
    }
}

void CAMERA_Adjust_roll(long roll_degrees, int frames)
{
    int temp; // not from decls.h

    temp = roll_degrees & 0xFFF;

    if (frames == 0)
    {
        current_roll_amount = temp * 4096;

        roll_target = current_roll_amount;

        roll_inc = 0;
    }
    else
    {
        roll_inc = (CAMERA_SignedAngleDifference(temp, (current_roll_amount / 4096)) * 4096) / frames;

        roll_target = temp * 4096;
    }
}

void CAMERA_Adjust(Camera *camera, long adjust)
{
    SVector dv;
    CameraKey *temp; // not from decls.h

    temp = camera->cameraKey;

    if (temp != NULL)
    {
        if ((adjust & 0x1))
        {
            SUB_SVEC(SVector, &dv, SVector, (SVector *)temp, SVector, (SVector *)&temp->tx);

            CAMERA_Adjust_distance(camera, CAMERA_LengthSVector(&dv));
        }

        if ((adjust & 0x2))
        {
            CAMERA_Adjust_tilt(camera, temp->rx);
        }

        if ((adjust & 0x4))
        {
            CAMERA_Adjust_rotation(camera, temp->rz);
        }
    }
}

void CAMERA_ChangeTo(Camera *camera, CameraKey *cameraKey)
{
    camera->cameraKey = cameraKey;
}

void CAMERA_SetShake(Camera *camera, long shake, long scale)
{
    int shock;
    int duration;
    int temp, temp2; // not from decls.h

    camera->shakeFrame = 0;

    if (camera->shake == 0)
    {
        camera->shake = shake << 12;
        camera->shakeScale = (short)scale;
    }
    else
    {
        temp = camera->shake;

        if (temp < (shake << 12))
        {
            temp = shake << 12;
        }

        camera->shake = temp;

        temp2 = camera->shakeScale;

        if (temp2 < scale)
        {
            temp2 = scale;
        }

        camera->shakeScale = (short)temp2;
    }

    shock = (short)camera->shakeScale >> 1;

    if (shock >= 256)
    {
        shock = 255;
    }

    duration = camera->shake;

    if (duration < 16384)
    {
        duration = 16384;
    }

    GAMEPAD_Shock1(shock, duration);
}

void Decouple_AngleMoveToward(short *current_ptr, short destination, short step)
{
    if (gameTrackerX.timeMult != 4096)
    {
        step = (short)((step * gameTrackerX.timeMult) >> 12);
    }

    AngleMoveToward(current_ptr, destination, step);
}

INCLUDE_ASM("asm/nonmatchings/Game/CAMERA", CriticalDampValue);

INCLUDE_ASM("asm/nonmatchings/Game/CAMERA", CriticalDampPosition);

void CriticalDampAngle(long dampMode, short *currentVal, short target, short *vel, short *accl, int smooth)
{
    short current;
    short temp; // not from decls.h

    temp = smooth;

    target &= 0xFFF;

    current = *currentVal & 0xFFF;

    if ((target - current) >= 2048)
    {
        current += 4096;
    }
    else if ((current - target) > 2048)
    {
        target |= 0x1000;
    }

    CriticalDampValue(dampMode, &current, target, vel, accl, temp);

    current &= 0xFFF;

    *currentVal = current;
}

short CAMERA_CalcZRotation(Position *target, Position *position)
{
    SVector onPlane;
    SVector sv;

    SUB_SVEC(SVector, &onPlane, Position, position, Position, target);

    sv.x = onPlane.x;
    sv.y = onPlane.y;
    sv.z = 0;

    CAMERA_LengthSVector(&sv);

    return (ratan2(onPlane.y, onPlane.x) + 1024) & 0xFFF;
}

void CAMERA_CalcRotation(Rotation *rotation, Position *target, Position *position)
{
    SVector sv;
    SVector onPlane;

    SUB_SVEC(SVector, &sv, Position, position, Position, target);

    onPlane.x = sv.x;
    onPlane.y = sv.y;
    onPlane.z = 0;

    rotation->x = (short)-ratan2(sv.z, CAMERA_LengthSVector(&onPlane));
    rotation->y = 0;
    rotation->z = (short)ratan2(sv.y, sv.x) + 1024;
}

void CAMERA_CalcFSRotation(Camera *camera, Rotation *rotation, Position *target, Position *position)
{
    SVector sv;
    SVector onPlane;
    SVector sv2;

    (void)camera;

    SUB_SVEC(SVector, &sv, Position, position, Position, target);
    COPY_SVEC(SVector, &sv2, SVector, &sv);

    onPlane.x = sv2.x;
    onPlane.y = sv2.y;
    onPlane.z = 0;

    rotation->x = (short)-ratan2(sv2.z, CAMERA_LengthSVector(&onPlane));
    rotation->y = 0;
    rotation->z = (short)ratan2((short)sv2.y, sv2.x) + 1024;
}

void CAMERA_Relocate(Camera *camera, SVector *offset, int streamSignalFlag)
{
    if ((streamSignalFlag != 0) || (camera->mode != 5))
    {
        camera->core.position.x += offset->x;
        camera->core.position.y += offset->y;
        camera->core.position.z += offset->z;

        camera->focusPoint.x += offset->x;
        camera->focusPoint.y += offset->y;
        camera->focusPoint.z += offset->z;

        camera->targetPos.x += offset->x;
        camera->targetPos.y += offset->y;
        camera->targetPos.z += offset->z;

        camera->targetFocusPoint.x += offset->x;
        camera->targetFocusPoint.y += offset->y;
        camera->targetFocusPoint.z += offset->z;

        camera->newFocusInstancePos.x += offset->x;
        camera->newFocusInstancePos.y += offset->y;
        camera->newFocusInstancePos.z += offset->z;
    }
}

TFace *CAMERA_SphereToSphereWithLines(Camera *camera, CameraCollisionInfo *colInfo, int secondcheck_flag)
{
    long minLength;
    SVector sv;
    SVector startPt[5];
    SVector endPt[5];
    SVector startLine;
    Vector adjStartLine;
    SVector endLine;
    Vector adjEndLine;
    Vector CamLineNormalized;
    Rotation rotation;
    MATRIX matrix;
    TFace *result;
    long i;
    long init;
    Level *level;
    // Instance *focusInstance; unused
    Vector ACE_vect;
    LCollideInfo lcol;
    int ACE_force;
    int in_warpRoom;
    int flag;
    short backface_flag;
    PCollideInfo pCollideInfo;
    int n;
    Level *thislevel;

    minLength = 0;

    result = NULL;

    init = 1;

    ACE_force = 0;

    level = STREAM_GetLevelWithID(camera->focusInstance->currentStreamUnitID);

    if ((level != NULL) && (MEMPACK_MemoryValidFunc((char *)level) != 0))
    {
        colInfo->line = -1;

        colInfo->flags = 0;

        colInfo->numCollided = 0;

        startLine.x = 0;
        startLine.z = 0;

        endLine.x = 0;
        endLine.z = 0;

        CAMERA_CalcFSRotation(camera, &rotation, &colInfo->end->position, &colInfo->start->position);

        if ((camera->flags & 0x10000))
        {
            colInfo->start->position.x += ((colInfo->end->position.x - colInfo->start->position.x) >> 5);
            colInfo->start->position.y += ((colInfo->end->position.y - colInfo->start->position.y) >> 5);
            colInfo->start->position.z += ((colInfo->end->position.z - colInfo->start->position.z) >> 5);
        }

        MATH3D_SetUnityMatrix(&matrix);

        RotMatrixZ(rotation.z + 1024, &matrix);

        if ((!(camera->flags & 0x10000)) && (!(camera->instance_mode & 0x4000000)) && (camera->mode != 6))
        {
            startLine.y = 4096;

            ApplyMatrix(&matrix, (SVECTOR *)&startLine, (VECTOR *)&ACE_vect);

            ACE_amount = ((ACE_vect.x * camera->focusInstanceVelVec.x) + (ACE_vect.y * camera->focusInstanceVelVec.y) + (ACE_vect.z * camera->focusInstanceVelVec.z)) >> 12;

            if ((camera->always_rotate_flag != 0) || (camera->forced_movement != 0))
            {
                if (camera->forced_movement != 0)
                {
                    if (camera->rotDirection <= 0)
                    {
                        if (camera->rotDirection < 0)
                        {
                            ACE_force = -72;
                        }
                    }
                    else
                    {
                        ACE_force = 72;
                    }
                }
                else
                {
                    ACE_force = -72;

                    if ((CAMERA_SignedAngleDifference(camera->focusRotation.z, camera->targetFocusRotation.z) << 16) < 0)
                    {
                        ACE_force = 72;
                    }
                }

                if (ACE_amount > 0)
                {
                    if (ACE_force > 0)
                    {
                        ACE_force -= ACE_amount;

                        if (ACE_force < 0)
                        {
                            ACE_force = 0;
                        }
                    }
                }
                else if (ACE_force < 0)
                {
                    ACE_force -= ACE_amount;

                    if (ACE_force > 0)
                    {
                        ACE_force = 0;
                    }
                }
            }
        }
        else
        {
            ACE_amount = 0;
        }

        startLine.y = 32;
        endLine.y = 290;

        if (ACE_amount > 0)
        {
            startLine.y += (short)ACE_amount;
            endLine.y += (short)((ACE_amount * 5) + (ACE_amount / 2));
        }
        else
        {
            startLine.y -= (short)(ACE_amount * 2);
        }

        if (ACE_force > 0)
        {
            endLine.y += ACE_force * 5;
        }

        ApplyMatrix(&matrix, (SVECTOR *)&startLine, (VECTOR *)&adjStartLine);
        ApplyMatrix(&matrix, (SVECTOR *)&endLine, (VECTOR *)&adjEndLine);

        ADD_SVEC(SVector, &startPt[1], Position, &colInfo->start->position, Vector, &adjStartLine);
        ADD_SVEC(SVector, &endPt[1], Position, &colInfo->end->position, Vector, &adjEndLine);

        startLine.y = 32;
        endLine.y = 290;

        if (ACE_amount < 0)
        {
            startLine.y -= (short)ACE_amount;
            endLine.y -= (short)((ACE_amount * 5) + (ACE_amount / 2));
        }
        else
        {
            startLine.y += (short)(ACE_amount * 2);
        }

        if (ACE_force < 0)
        {
            endLine.y -= ACE_force * 5;
        }

        ApplyMatrix(&matrix, (SVECTOR *)&startLine, (VECTOR *)&adjStartLine);
        ApplyMatrix(&matrix, (SVECTOR *)&endLine, (VECTOR *)&adjEndLine);

        SUB_SVEC(SVector, &startPt[2], Position, &colInfo->start->position, Vector, &adjStartLine);
        SUB_SVEC(SVector, &endPt[2], Position, &colInfo->end->position, Vector, &adjEndLine);

        endLine.y = 180;
        startLine.y = 32;

        MATH3D_SetUnityMatrix(&matrix);

        RotMatrixX(rotation.x + 1024, &matrix);
        RotMatrixZ(rotation.z, &matrix);

        ApplyMatrix(&matrix, (SVECTOR *)&startLine, (VECTOR *)&adjStartLine);
        ApplyMatrix(&matrix, (SVECTOR *)&endLine, (VECTOR *)&adjEndLine);

        ADD_SVEC(SVector, &startPt[3], Vector, &adjStartLine, Position, &colInfo->start->position);
        ADD_SVEC(SVector, &endPt[3], Vector, &adjEndLine, Position, &colInfo->end->position);

        SUB_SVEC(SVector, &startPt[4], Position, &colInfo->start->position, Vector, &adjStartLine);
        SUB_SVEC(SVector, &endPt[4], Position, &colInfo->end->position, Vector, &adjEndLine);

        ADD_SVEC(SVector, &right_point, SVector, &startPt[1], SVector, &camera->focusInstanceVelVec);
        ADD_SVEC(SVector, &left_point, SVector, &startPt[2], SVector, &camera->focusInstanceVelVec);

        startLine.y = 4096;

        ApplyMatrix(&matrix, (SVECTOR *)&startLine, (VECTOR *)&adjStartLine);

        camera_plane.x = adjStartLine.x;
        camera_plane.y = adjStartLine.y;
        camera_plane.z = adjStartLine.z;

        startLine.y = 0;
        startLine.z = 4096;

        ApplyMatrix(&matrix, (SVECTOR *)&startLine, (VECTOR *)&CamLineNormalized);

        COPY_SVEC(SVector, &startPt[0], Position, &colInfo->start->position);
        COPY_SVEC(SVector, &endPt[0], Position, &colInfo->end->position);

        colInfo->lenCenterToExtend = (int)camera->targetFocusDistance;

        in_warpRoom = (unsigned int)STREAM_GetStreamUnitWithID(level->streamUnitID)->flags & 0x1;

        for (i = 0; i < 5; i++)
        {
            if ((colInfo->cldLines & (1 << i)))
            {
                if ((i == 1) || (i == 2))
                {
                    flag = 1;
                }
                else
                {
                    flag = 0;
                }

                backface_flag = 0;

                pCollideInfo.collideType = 1;

                pCollideInfo.newPoint = (SVECTOR *)&endPt[i];
                pCollideInfo.oldPoint = (SVECTOR *)&startPt[i];

                pCollideInfo.instance = NULL;

                colInfo->tfaceList[i] = COLLIDE_PointAndTerrainFunc(level->terrain, &pCollideInfo, flag, &backface_flag, 208, 32, &lcol);
                colInfo->tfaceTerrain[i] = level->terrain;

                if (colInfo->tfaceList[i] == NULL)
                {
                    StreamUnit *streamUnit = StreamTracker.StreamList;

                    for (n = 0; n < 16; n++, streamUnit++)
                    {
                        thislevel = streamUnit->level;

                        if ((streamUnit->used == 2) && (thislevel != level) && (MEMPACK_MemoryValidFunc((char *)thislevel) != 0) && ((in_warpRoom == 0) || (!(streamUnit->flags & 0x1))))
                        {
                            colInfo->tfaceList[i] = COLLIDE_PointAndTerrainFunc(thislevel->terrain, &pCollideInfo, flag, &backface_flag, 208, 32, &lcol);

                            if (colInfo->tfaceList[i] != NULL)
                            {
                                colInfo->tfaceTerrain[i] = thislevel->terrain;
                                break;
                            }
                        }
                    }
                }

                colInfo->bspTree[i] = lcol.curTree;

                if (colInfo->tfaceList[i] != NULL)
                {
                    if (secondcheck_flag != 0)
                    {
                        return colInfo->tfaceList[i];
                    }

                    colInfo->numCollided++;

                    SUB_SVEC(SVector, &sv, SVector, &startPt[i], SVector, &endPt[i]);

                    colInfo->lengthList[i] = (short)(((sv.x * CamLineNormalized.x) + (sv.y * CamLineNormalized.y) + (sv.z * CamLineNormalized.z)) >> 12);

                    if ((backface_flag == 0) || (colInfo->lengthList[i] >= 100))
                    {
                        colInfo->lengthList[i] -= 100;

                        if (colInfo->lengthList[i] < 220)
                        {
                            colInfo->lengthList[i] = 220;
                        }

                        if ((init != 0) || ((int)colInfo->lengthList[i] < minLength))
                        {
                            colInfo->line = i;

                            minLength = (int)colInfo->lengthList[i];

                            init = 0;

                            colInfo->lenCenterToExtend = minLength;

                            result = colInfo->tfaceList[i];
                        }

                        colInfo->flags |= (1 << i);
                    }
                }
                else
                {
                    SUB_SVEC(SVector, &sv, SVector, &startPt[i], SVector, &endPt[i]);

                    colInfo->lengthList[i] = (short)(((sv.x * CamLineNormalized.x) + (sv.y * CamLineNormalized.y) + (sv.z * CamLineNormalized.z)) >> 12);
                }
            }
        }

        if (colInfo->line == 2)
        {
            hitline_rot = CAMERA_CalcZRotation((Position *)&startPt[2], (Position *)&endPt[2]);
        }
        else if (colInfo->line == 1)
        {
            hitline_rot = CAMERA_CalcZRotation((Position *)&startPt[1], (Position *)&endPt[1]);
        }
        else if ((colInfo->flags & 0x4))
        {
            hitline_rot = CAMERA_CalcZRotation((Position *)&startPt[2], (Position *)&endPt[2]);
        }
        else if ((colInfo->flags & 0x2))
        {
            hitline_rot = CAMERA_CalcZRotation((Position *)&startPt[1], (Position *)&endPt[1]);
        }
    }

    return result;
}

long CAMERA_CalcTilt(Normal *normal, short zRot)
{
    MATRIX matrix;
    VECTOR newNormal;

    MATH3D_SetUnityMatrix(&matrix);

    RotMatrixZ(-zRot, &matrix);

    ApplyMatrix(&matrix, (SVECTOR *)normal, &newNormal);

    return -(short)ratan2(newNormal.vy, newNormal.vz);
}

void CAMERA_SetLookFocusAndBase(Instance *focusInstance, Position *focusPoint)
{
    Position lookFocus;

    lookFocus.x = focusInstance->position.x;
    lookFocus.y = focusInstance->position.y;
    lookFocus.z = focusInstance->position.z + 512;

    *focusPoint = lookFocus;
}

void CAMERA_SetLookFocusAndDistance(Camera *camera, VECTOR *focuspoint, int distance)
{
    camera->targetFocusPoint.x = (short)focuspoint->vx;
    camera->targetFocusPoint.y = (short)focuspoint->vy;
    camera->targetFocusPoint.z = (short)focuspoint->vz;

    Camera_lookDist = distance;
}

void CAMERA_LookProcess(Camera *camera)
{
    Instance *focusInstance;
    int smooth;
    int distance;
    long dampMode;

    camera->targetFocusPoint.x += camera->focusInstanceVelVec.x;
    camera->targetFocusPoint.y += camera->focusInstanceVelVec.y;
    camera->targetFocusPoint.z += camera->focusInstanceVelVec.z;

    focusInstance = camera->focusInstance;

    if (camera->focusDistance <= Camera_lookDist)
    {
        distance = Camera_lookDist;
    }
    else
    {
        distance = camera->focusDistance;
    }

    CAMERA_CalcPosition(&camera->targetPos, &camera->focusPoint, &camera->focusRotation, distance);

    camera->data.Follow.hit = CAMERA_DoCameraCollision2(camera, &camera->targetPos, 1);

    if (camera->data.Follow.hit != 0)
    {
        smooth = 512;

        dampMode = 5;

        distance = camera->collisionTargetFocusDistance;

        if (distance > Camera_lookDist)
        {
            distance = Camera_lookDist;
        }
    }
    else
    {
        distance = Camera_lookDist;

        smooth = 128;

        dampMode = 1;
    }

    if (((camera->instance_mode & 0x20000)) && (distance < 550))
    {
        distance = 550;
    }

    if ((camera->flags & 0x800))
    {
        camera->focusDistance = camera->targetFocusDistance;

        COPY_SVEC(Position, &camera->focusPoint, Position, &camera->targetFocusPoint);
    }
    else
    {
        CriticalDampValue(dampMode, &camera->focusDistance, distance, &camera->focusDistanceVel, &camera->focusDistanceAccl, smooth);
        CriticalDampPosition(0, &camera->focusPoint, &camera->targetFocusPoint, &camera->focusPointVel, &camera->focusPointAccl, camera->maxVel);
    }

    camera->targetFocusRotation.x = camera->lookRot.x;
    camera->targetFocusRotation.y = camera->lookRot.y;
    camera->targetFocusRotation.z = (camera->lookRot.z + focusInstance->rotation.z + 2048) & 0xFFF;

    CriticalDampAngle(1, &camera->focusRotation.z, camera->targetFocusRotation.z, &camera->focusRotVel.z, &camera->focusRotAccl.z, 64);
    CriticalDampAngle(1, &camera->focusRotation.x, camera->targetFocusRotation.x, &camera->focusRotVel.x, &camera->focusRotAccl.x, 64);

    CAMERA_CalcPosition(&camera->targetPos, &camera->focusPoint, &camera->focusRotation, camera->focusDistance);

    COPY_SVEC(Position, &camera->core.position, Position, &camera->targetPos);
    COPY_SVEC(Rotation, &camera->targetRotation, Rotation, &camera->focusRotation);
    COPY_SVEC(Rotation, &camera->core.rotation, Rotation, &camera->targetRotation);

    camera->distanceState = 0;

    camera->lagZ = camera->core.rotation.z;

    CAMERA_CalculateLead(camera);
}

void CAMERA_Normalize(SVector *svector)
{
    long len;

    len = CAMERA_LengthSVector(svector);

    if (len != 0)
    {
        svector->x = (short)((svector->x << 12) / len);
        svector->y = (short)((svector->y << 12) / len);
        svector->z = (short)((svector->z << 12) / len);
    }
}

void CAMERA_HandleTransitions(Camera *camera)
{
    if (camera->rotState == 3)
    {
        camera->collisionTargetFocusRotation.z = camera->targetFocusRotation.z = camera->signalRot.z;

        if ((camera->forced_movement != 1) || ((camera->lock & 0x4)))
        {
            if (CAMERA_AngleDifference(camera->targetFocusRotation.z, camera->focusRotation.z) < 4)
            {
                camera->rotState = 0;

                camera->focusRotVel.z = 0;
                camera->focusRotAccl.z = 0;
            }
        }
        else
        {
            camera->rotState = 0;

            camera->focusRotVel.z = 0;
            camera->focusRotAccl.z = 0;
        }
    }

    if (camera->tiltState == 3)
    {
        camera->targetFocusRotation.x = camera->signalRot.x;

        if ((camera->forced_movement != 3) || ((camera->lock & 0x2)))
        {
            if (CAMERA_AngleDifference(camera->focusRotation.x, camera->signalRot.x) < 4)
            {
                camera->tiltState = 0;

                camera->focusRotVel.x = 0;
                camera->focusRotAccl.x = 0;
            }
        }
        else
        {
            camera->tiltState = 0;

            camera->focusRotVel.x = 0;
            camera->focusRotAccl.x = 0;
        }
    }

    if (camera->distanceState == 3)
    {
        if ((camera->forced_movement != 2) || ((camera->lock & 0x1)))
        {
            if (ABS(camera->targetFocusDistance - camera->signalFocusDistance) < 4)
            {
                camera->distanceState = 0;

                camera->focusDistanceVel = 0;
                camera->focusDistanceAccl = 0;
            }
        }
        else
        {
            camera->distanceState = 0;

            camera->focusDistanceVel = 0;
            camera->focusDistanceAccl = 0;
        }

        camera->targetFocusDistance = camera->signalFocusDistance;
    }

    if ((camera->posState == 3) && ((camera->mode != 5) || ((camera->flags & 0x1000))))
    {
        camera->posState = 0;
    }
}

void CAMERA_CalcFocusOffset(SVector *offset, Camera *camera)
{
    Vector adjustedOffset;
    SVector temp;
    Instance *focusInstance;

    focusInstance = camera->focusInstance;

    temp.x = camera->focusOffset.x;
    temp.y = camera->focusOffset.y;
    temp.z = camera->focusOffset.z;

    if ((int)camera->instance_mode < 0)
    {
        temp.z += 256;
    }

    gte_SetRotMatrix(focusInstance->matrix);
    gte_ldv0(&temp);
    gte_nrtv0();
    gte_stlvnl(&adjustedOffset);

    COPY_SVEC(SVector, offset, Vector, &adjustedOffset);
}

void CAMERA_CalcFocusOffsetForSwim(SVector *offset, Camera *camera)
{
    Vector adjustedOffset;
    SVector temp;
    Instance *focusInstance;

    focusInstance = camera->focusInstance;

    temp.x = camera->focusOffset.x;
    temp.y = camera->focusOffset.y;
    temp.z = camera->focusOffset.z;

    ApplyMatrix(&focusInstance->matrix[1], (SVECTOR *)&temp, (VECTOR *)&adjustedOffset);

    COPY_SVEC(SVector, offset, Vector, &adjustedOffset);
}

short CAMERA_CalcIntersectAngle(SVector *linept, SVector *vertex0, SVector *vertex1, short *high, short *low)
{
    SVector point;
    short zrot;
    long camera_plane_d;

    camera_plane_d = ((camera_plane.x * linept->x) + (camera_plane.y * linept->y) + (camera_plane.z * linept->z)) >> 12;

    if (COLLIDE_IntersectLineAndPlane_S(&point, (Position *)vertex0, (Position *)vertex1, &camera_plane, camera_plane_d) != 0)
    {
        zrot = CAMERA_SignedAngleDifference(CAMERA_CalcZRotation((Position *)linept, (Position *)&point), hitline_rot);

        if (zrot < *low)
        {
            *low = zrot;
        }

        if (*high < zrot)
        {
            *high = zrot;
        }

        return zrot;
    }

    return -9999;
}

short CAMERA_GetLineAngle(Camera *camera, CameraCollisionInfo *colInfo, SVector *linept, int line)
{
    Terrain *terrain;
    SVector *vertex0;
    SVector *vertex1;
    SVector *vertex2;
    SVector new_linept;
    short high;
    short low;
    short temp; // not from decls.h

    (void)camera;

    high = -5000;
    low = 5000;

    terrain = colInfo->tfaceTerrain[line];

    temp = colInfo->bspTree[line];

    vertex0 = (SVector *)&terrain->vertexList[colInfo->tfaceList[line]->face.v0].vertex;
    vertex1 = (SVector *)&terrain->vertexList[colInfo->tfaceList[line]->face.v1].vertex;
    vertex2 = (SVector *)&terrain->vertexList[colInfo->tfaceList[line]->face.v2].vertex;

    new_linept.x = linept->x - terrain->BSPTreeArray[temp].globalOffset.x;
    new_linept.y = linept->y - terrain->BSPTreeArray[temp].globalOffset.y;
    new_linept.z = linept->z - terrain->BSPTreeArray[temp].globalOffset.z;

    CAMERA_CalcIntersectAngle(&new_linept, vertex0, vertex1, &high, &low);
    CAMERA_CalcIntersectAngle(&new_linept, vertex1, vertex2, &high, &low);
    CAMERA_CalcIntersectAngle(&new_linept, vertex2, vertex0, &high, &low);

    if (high == -5000)
    {
        high = 0;
    }

    if (low == 5000)
    {
        low = 0;
    }

    if (high > 1024)
    {
        high = 0;
    }

    if (low < -1024)
    {
        low = 0;
    }

    if (line == 2)
    {
        return high;
    }

    return low;
}

long CAMERA_ACForcedMovement(Camera *camera, CameraCollisionInfo *colInfo)
{
    // short dp; unused
    Normal normal;
    SVector sv;

    COLLIDE_GetNormal(colInfo->tfaceList[colInfo->line]->normal, (short *)colInfo->tfaceTerrain[colInfo->line]->normalList, (SVector *)&normal);

    SUB_SVEC(SVector, &sv, Position, &colInfo->start->position, Position, &colInfo->end->position);

    CAMERA_Normalize(&sv);

    if (((sv.x * normal.x) + (sv.y * normal.y) + (sv.z * normal.z)) != 0)
    {
        camera->collisionTargetFocusDistance = (short)colInfo->lenCenterToExtend;
    }
    else
    {
        camera->collisionTargetFocusDistance = (short)colInfo->lenCenterToExtend;
    }

    return 0;
}

void CAMERA_update_dist_debounced(Camera *camera, short dist)
{
    if ((!(camera->instance_mode & 0x2000000)) || (dist >= 600) || (combat_cam_weight >= 4040))
    {
        shorten_flag = 1;

        if (++shorten_count >= 3)
        {
            camera->collisionTargetFocusDistance = dist;
        }
    }
}

short CAMERA_dampgetline(short angle)
{
    static short target_angle = 0;
    static short angle_vel = 0;
    static short angle_accl = 0;

    CriticalDampAngle(1, &target_angle, angle, &angle_vel, &angle_accl, 1024);

    if (target_angle > 2048)
    {
        target_angle -= 4096;
    }

    if ((abs(target_angle) < 32) && (abs(angle) > 31))
    {
        target_angle = angle < 0 ? -32 : 32;
    }

    return target_angle;
}

long CAMERA_ACNoForcedMovement(Camera *camera, CameraCollisionInfo *colInfo)
{
    long hit;
    short playerSamePos;
    short angle;
    int temp, temp2; // not from decls.h
    short temp3;     // not from decls.h

    hit = 0;

    playerSamePos = CAMERA_FocusInstanceMoved(camera) == 0;

    if ((camera->always_rotate_flag == 0) && (camera->rotState != 3) && (colInfo->numCollided >= 2))
    {
        int n;
        int flag;
        short dist;

        flag = 0x1;

        if ((colInfo->line == 1) || (colInfo->line == 2))
        {
            dist = 32767;

            if ((colInfo->flags & 0x6) == 0x6)
            {
                angle = colInfo->lengthList[2];

                if ((angle >= 121) && (colInfo->lengthList[1] >= 121) && (ABS(angle - colInfo->lengthList[1]) < 600))
                {
                    CAMERA_update_dist_debounced(camera, colInfo->lenCenterToExtend);

                    CAMERA_dampgetline(0);

                    if ((camera->mode == 13) && (abs(ACE_amount) >= 11))
                    {
                        camera->collisionTargetFocusRotation.z = camera->targetFocusRotation.z;
                    }

                    return 1;
                }
            }

            for (n = 1; n < 5; n++)
            {
                if (n != colInfo->line)
                {
                    if (colInfo->lengthList[n] < 800)
                    {
                        flag = 0;
                        break;
                    }

                    if (colInfo->lengthList[n] < dist)
                    {
                        dist = colInfo->lengthList[n];
                    }
                }
            }

            if (flag != 0)
            {
                colInfo->numCollided = 1;

                colInfo->flags = 1 << colInfo->line;

                if (colInfo->lenCenterToExtend >= 801)
                {
                    CAMERA_update_dist_debounced(camera, colInfo->lenCenterToExtend);
                }
                else
                {
                    CAMERA_update_dist_debounced(camera, dist);
                }
            }
            else if (temp = colInfo->numCollided, colInfo->numCollided == 4)
            {
                if ((camera->collision_lastPush != 0) && (colInfo->lenCenterToExtend < 400))
                {
                    colInfo->numCollided = 1;

                    if (camera->collision_lastPush == 2)
                    {
                        colInfo->flags = 0x2;

                        colInfo->line = 1;
                    }
                    else
                    {
                        colInfo->line = 2;

                        colInfo->flags = temp;
                    }
                }
            }
        }
    }

    if ((colInfo->numCollided == 4) && (!(camera->lock & 0x1)))
    {
        hit = 1;

        camera->collisionTargetFocusDistance = colInfo->lenCenterToExtend;

        camera->collisionTargetFocusRotation.z = camera->targetFocusRotation.z;
    }
    else if (((colInfo->flags & 0x2)) && (!(camera->lock & 0x4)) && ((playerSamePos == 0) || (camera->collision_lastPush != 1)))
    {
        if (!(colInfo->flags & 0x4))
        {
            camera->collideRotControl = 1;

            camera->collision_lastPush = 2;

            if ((camera->mode != 13) || (ACE_amount >= -10))
            {
                temp3 = CAMERA_dampgetline(CAMERA_GetLineAngle(camera, colInfo, &right_point, 1));

                if (abs(ACE_amount) < 6)
                {
                    if (temp3 >= -63)
                    {
                        temp2 = (unsigned short)camera->focusRotation.z - 4;

                        camera->collisionTargetFocusRotation.z = temp3 + temp2;
                    }
                    else
                    {
                        camera->collisionTargetFocusRotation.z -= 64;
                    }

                    return 1;
                }
                else if ((temp3 << 16) <= 0)
                {
                    camera->collisionTargetFocusRotation.z = (temp3 - 4) + camera->focusRotation.z;
                }
            }
            else
            {
                camera->collisionTargetFocusRotation.z = camera->targetFocusRotation.z;

                CAMERA_update_dist_debounced(camera, colInfo->lenCenterToExtend);
            }
        }
        else
        {
            CAMERA_update_dist_debounced(camera, colInfo->lenCenterToExtend);
        }

        return 1;
    }
    else if (((colInfo->flags & 0x4)) && (!(camera->lock & 0x4)) && ((playerSamePos == 0) || (camera->collision_lastPush != 2)))
    {
        if (!(colInfo->flags & 0x2))
        {
            camera->collideRotControl = 1;

            camera->collision_lastPush = 1;

            if ((camera->mode == 13) && (ACE_amount >= 11))
            {
                camera->collisionTargetFocusRotation.z = camera->targetFocusRotation.z;

                CAMERA_update_dist_debounced(camera, colInfo->lenCenterToExtend);
                return 1;
            }
        }
        else
        {
            CAMERA_update_dist_debounced(camera, colInfo->lenCenterToExtend);
            return 1;
        }

        temp3 = CAMERA_dampgetline(CAMERA_GetLineAngle(camera, colInfo, &left_point, 2));

        if (abs(ACE_amount) < 6)
        {
            if (temp3 >= 64)
            {
                camera->collisionTargetFocusRotation.z += 64;
            }
            else
            {
                temp2 = (unsigned short)camera->focusRotation.z + 4;

                camera->collisionTargetFocusRotation.z = temp3 + temp2;
            }
        }
        else if (temp3 >= 0)
        {
            temp2 = (unsigned short)camera->focusRotation.z + 4;

            camera->collisionTargetFocusRotation.z = temp3 + temp2;
        }

        return 1;
    }
    else if ((((colInfo->flags & 0x8)) && (temp = colInfo->flags, (!(camera->lock & 0x2)))) || (((colInfo->flags & 0x10)) && (!(camera->lock & 0x2))))
    {
        CAMERA_update_dist_debounced(camera, colInfo->lenCenterToExtend);

        do
        {
        } while (0); // garbage code for reordering

        hit = 1;
    }

    CAMERA_dampgetline(0);

    return hit;
}

long CAMERA_AbsoluteCollision(Camera *camera, CameraCollisionInfo *colInfo)
{
    long hit;

    hit = 0;

    if ((gameTrackerX.debugFlags & 0x10000))
    {
        return hit;
    }

    camera->focusRotation.x &= 0xFFF;

    if ((((camera->flags & 0x10000)) || ((camera->instance_mode & 0x24000000)) || ((camera->flags & 0x2000)) || (camera->rotState != 0) || (camera->always_rotate_flag != 0)) && (!(camera->lock & 0x1)))
    {
        if ((camera->flags & 0x10000))
        {
            if ((colInfo->flags & 0x18))
            {
                camera->collisionTargetFocusDistance = (short)colInfo->lenCenterToExtend - 150;

                if (400 > camera->collisionTargetFocusDistance)
                {
                    camera->collisionTargetFocusDistance = 400;
                }
            }
            else
            {
                hit = CAMERA_ACNoForcedMovement(camera, colInfo);
            }
        }
        else
        {
            camera->collisionTargetFocusDistance = (short)colInfo->lenCenterToExtend;
        }
    }
    else if ((camera->forced_movement != 0) || (camera->last_forced_movement != 0))
    {
        hit = CAMERA_ACForcedMovement(camera, colInfo);
    }
    else
    {
        hit = CAMERA_ACNoForcedMovement(camera, colInfo);
    }

    return hit;
}

short CAMERA_update_z_damped(Camera *camera, short current, short target)
{
    static short upvel = 0;
    static short upaccl = 0;
    static short upmaxVel = 0;
    short current_tmp;

    current_tmp = current;

    if (current > target)
    {
        if (((camera->instance_mode & 0x1038)) && (camera->real_focuspoint.z >= camera->focuspoint_fallz))
        {
            upmaxVel = ABS(current - target) / 6;
        }
        else
        {
            upmaxVel = ABS(current - target) * 2;
        }

        CriticalDampValue(1, &current_tmp, target, &upvel, &upaccl, upmaxVel);
    }
    else if (ABS(current - target) >= 5)
    {
        if ((camera->instance_mode & 0x100))
        {
            upmaxVel = ABS(current - target) / 6;

            if (upmaxVel < 50)
            {
                upmaxVel = 50;
            }
        }
        else
        {
            upmaxVel = ABS(current - target) / 6;
        }

        if (ABS(current - target) < upmaxVel)
        {
            upmaxVel = ABS(current - target);
        }

        CriticalDampValue(1, &current_tmp, target, &upvel, &upaccl, upmaxVel);
    }
    else
    {
        upvel = 0;

        upaccl = 0;

        upmaxVel = 0;

        return current;
    }

    return current_tmp;
}

void CAMERA_CombatCamDist(Camera *camera)
{
    DVECTOR xy;         
    DVECTOR xy2;        
    SVector position;   
    long z;             
    Instance *instance; 
    short angle; // not from decls.h

    instance = (Instance*)INSTANCE_Query(camera->focusInstance, 34);
    
    if (instance == NULL) 
    {
        combat_cam_weight = 4096;
        combat_cam_distance = camera->targetFocusDistance;
        return;
    }
    
    SetRotMatrix(camera->core.wcTransform);
    SetTransMatrix(camera->core.wcTransform);
    
    position.x = camera->focusInstance->position.x;
    position.y = camera->focusInstance->position.y;
    position.z = camera->focusInstance->position.z;
    
    gte_ldv0(&position);
    gte_nrtps();
    gte_stsxy(&xy.vx);
    gte_stsz(&z);
    
    if (xy.vx < 256)
    {
        combat_cam_distance = -(((xy.vx - 256) * z) / 128);
    }
    else
    {
        combat_cam_distance = ((xy.vx - 256) * z) / 128;
    }
    
    if (combat_cam_distance > 3000) 
    {
        combat_cam_distance = 3000;
    }
    
    if (combat_cam_distance < camera->targetFocusDistance)
    {
        combat_cam_distance = camera->targetFocusDistance;
    }
    
    if (xy.vy > 240)
    {
        combat_cam_weight += 48;
        
        if (combat_cam_weight > 4096) 
        {
            combat_cam_weight = 4096;
        }
    }
    else if (xy.vy < 210) 
    {
        combat_cam_weight -= 48;
        
        if (combat_cam_weight < 2048) 
        {
            combat_cam_weight = 2048;
        }
    }
    
    position.x = instance->position.x;
    position.y = instance->position.y;
    position.z = instance->position.z;
    
    gte_ldv0(&position);
    gte_nrtps(); 
    gte_stsxy(&xy2.vx);
    
    if (ratan2(xy2.vy - xy.vy, xy2.vx - xy.vx) >= 0) 
    {
        angle = ratan2(xy2.vy - xy.vy, xy2.vx - xy.vx);
    } 
    else 
    {
        angle = -ratan2(xy2.vy - xy.vy, xy2.vx - xy.vx);
    }
    
    combat_cam_angle = angle;
    
    if (angle > 1024) 
    {
        combat_cam_angle = 2048 - angle;
    }
    
    combat_cam_angle = -170 - (combat_cam_angle / 8);
}

void CAMERA_GenericCameraProcess(Camera *camera)
{
    Instance *focusInstance;
    Position targetCamPos;

    focusInstance = camera->focusInstance;

    if (((camera->flags & 0x10000)) || ((camera->instance_mode & 0x4000000)))
    {
        camera->rotationVel.z = 64;
    }

    if (!(camera->instance_mode & 0x2000000))
    {
        combat_cam_weight = 2048;
    }

    CAMERA_SetMaxVel(camera);
    CAMERA_SetFocus(camera, &camera->targetFocusPoint);

    if (((camera->flags & 0x10000)) || ((camera->instance_mode & 0x4000000)))
    {
        CAMERA_FollowPlayerTilt(camera, focusInstance);
    }
    else
    {
        short angle;
        Rotation test_rot;

        CAMERA_UpdateFocusRotationX(camera, focusInstance);

        if ((camera->mode == 13) && (camera->instance_xyvel > 0) && (!(camera->instance_mode & 0x82000400)) && (camera->always_rotate_flag == 0) && (camera->rotState != 3) && (!(camera->instance_mode & 0x2000)) && (gameTrackerX.cheatMode != 1))
        {
            CAMERA_CalcRotation(&test_rot, &camera->targetFocusPoint, &camera->core.position);

            angle = CAMERA_SignedAngleDifference(test_rot.z, camera->focusRotation.z);

            if ((camera->instance_mode & 0x2))
            {
                angle >>= 1;
            }
            else
            {
                angle = (angle * 3) >> 2;
            }

            camera->targetFocusRotation.z = camera->focusRotation.z + angle;
        }
    }

    if ((int)camera->instance_mode < 0)
    {
        Instance *warpInstance;
        int tmp;

        warpInstance = RAZIEL_QueryEngagedInstance(14);

        if (warpInstance != NULL)
        {
            tmp = warpInstance->rotation.z;

            camera->tfaceTilt = 3988;

            camera->rotationVel.z = 64;

            camera->smooth = 8;

            camera->always_rotate_flag = 1;

            camera->forced_movement = 0;

            camera->collisionTargetFocusRotation.z = ((tmp & 0xFFF) <= 1024) || ((tmp & 0xFFF) >= 3072);

            camera->collisionTargetFocusRotation.z <<= 11;

            camera->targetFocusRotation.z = camera->collisionTargetFocusRotation.z;
        }
    }
    else
    {
        int mod;

        if (((camera->flags & 0x2000)) || ((camera->instance_mode & 0x20000000)))
        {
            if (!(camera->instance_mode & 0x2000000))
            {
                camera->rotationVel.z = 64;

                CAMERA_FollowGoBehindPlayer(camera);
            }
        }
        else if (((camera->flags & 0x10000)) || ((camera->instance_mode & 0x4000000)))
        {
            CAMERA_FollowGoBehindPlayer(camera);
        }
        else
        {
            if (((camera->instance_mode & 0x400)) && (!(camera->prev_instance_mode & 0x400)))
            {
                mod = camera->core.rotation.z % 1024;

                if (mod > 512)
                {
                    mod = 1024 - mod;
                }
                else
                {
                    mod = -mod;
                }

                if (ABS(mod) > 128)
                {
                    mod += camera->core.rotation.z;
                    mod &= 0xFFF;

                    camera->targetFocusRotation.z = mod;
                }
            }

            CAMERA_FollowGoBehindPlayerWithTimer(camera);
        }
    }

    if (!(camera->flags & 0x1800))
    {
        int dist;

        if ((camera->instance_mode & 0x2000000))
        {
            CAMERA_CombatCamDist(camera);

            dist = combat_cam_distance;
        }
        else
        {
            combat_cam_distance = dist = camera->targetFocusDistance;
        }

        CAMERA_CalcPosition(&targetCamPos, &camera->focusPoint, &camera->focusRotation, dist);

        camera->data.Follow.hit = CAMERA_DoCameraCollision2(camera, &targetCamPos, 0);
    }

    if ((int)camera->instance_mode < 0)
    {
        camera->collisionTargetFocusDistance = 2000;
    }

    CAMERA_UpdateFocusDistance(camera);
    CAMERA_UpdateFocusTilt(camera);
    CAMERA_UpdateFocusRotate(camera);

    if ((camera->flags & 0x1800))
    {
        COPY_SVEC(Position, &camera->focusPoint, Position, &camera->targetFocusPoint);

        camera->focusDistance = camera->targetFocusDistance;

        if ((camera->flags & 0x1000))
        {
            CAMERA_SetZRotation(camera, camera->teleportZRot);
        }
    }
    else
    {
        Position target;

        target = camera->targetFocusPoint;

        if (!(camera->flags & 0x10000))
        {
            target.z = camera->focusPoint.z;
        }

        CriticalDampPosition(1, &camera->focusPoint, &target, &camera->focusPointVel, &camera->focusPointAccl, camera->maxVel);

        if (!(camera->flags & 0x10000))
        {
            camera->focusPoint.z = CAMERA_update_z_damped(camera, camera->focusPoint.z, camera->targetFocusPoint.z);
        }
    }

    CAMERA_CalcFollowPosition(camera, &camera->focusRotation);
    CAMERA_CalculateLead(camera);

    CAMERA_UpdateFocusRoll(camera);
}

void CAMERA_CinematicProcess(Camera *camera)
{
    SVector *camPos;
    SVector *camTarget;
    MultiSpline *posSpline;
    MultiSpline *targetSpline;

    camPos = NULL;

    posSpline = camera->data.Cinematic.posSpline;

    targetSpline = camera->data.Cinematic.targetSpline;

    camTarget = NULL;

    if (camera->posState == 0)
    {
        if (targetSpline == NULL)
        {
            CAMERA_SetFocus(camera, &camera->targetFocusPoint);
        }

        if (posSpline != NULL)
        {
            camPos = SplineGetNextPointDC(posSpline->positional, &posSpline->curPositional);

            if (camPos != NULL)
            {
                if (camera->data.Cinematic.splinePointAhead == 0)
                {
                    COPY_SVEC(Position, &camera->core.position, SVector, camPos);
                }
                else
                {
                    COPY_SVEC(Position, &camera->core.position, SVector, &camera->data.Cinematic.lastSplinePos);
                }
            }

            if (camera->data.Cinematic.splinePointAhead != 0)
            {
                CAMERA_SetTarget(camera, (Position *)camPos);

                COPY_SVEC(SVector, &camera->data.Cinematic.lastSplinePos, SVector, camPos);
            }
            else if (targetSpline != NULL)
            {
                camTarget = SplineGetNextPointDC(targetSpline->positional, &targetSpline->curPositional);

                if (camTarget != NULL)
                {
                    COPY_SVEC(Position, &camera->targetFocusPoint, SVector, camTarget);

                    CAMERA_SetTarget(camera, (Position *)camTarget);
                }
            }
            else
            {
                CAMERA_SetMaxVel(camera);

                CriticalDampPosition(1, &camera->focusPoint, &camera->targetFocusPoint, &camera->focusPointVel, &camera->focusPointAccl, (short)(camera->maxVel * 2));
            }

            COPY_SVEC(Position, &camera->targetPos, Position, &camera->core.position);

            if (posSpline != NULL)
            {
                if ((camera->data.Cinematic.targetSpline != NULL) || (camera->data.Cinematic.splinePointAhead != 0))
                {
                    CAMERA_SetMaxVel(camera);

                    CriticalDampPosition(1, &camera->focusPoint, &camera->targetFocusPoint, &camera->focusPointVel, &camera->focusPointAccl, (short)(camera->maxVel * 2));
                }
            }
        }

        CAMERA_CalcRotation(&camera->focusRotation, &camera->focusPoint, &camera->core.position);

        if ((camPos == NULL) & (camTarget == NULL))
        {
            camera->data.Cinematic.cinema_done = 1;
        }
    }
    else
    {
        CriticalDampPosition(4, &camera->core.position, &camera->targetPos, &camera->positionVel, &camera->positionAccl, (short)(-camera->smooth));

        if (camera->data.Cinematic.targetSpline != NULL)
        {
            CriticalDampPosition(4, &camera->focusPoint, &camera->targetFocusPoint, &camera->focusPointVel, &camera->focusPointAccl, (short)(-camera->smooth));

            CAMERA_SetTarget(camera, &camera->focusPoint);

            if ((ABS(camera->core.position.x - camera->targetPos.x) < 9) &&
                (ABS(camera->core.position.y - camera->targetPos.y) < 9) &&
                (ABS(camera->core.position.z - camera->targetPos.z) < 9) &&
                (ABS(camera->focusPoint.x - camera->targetFocusPoint.x) < 9) &&
                (ABS(camera->focusPoint.y - camera->targetFocusPoint.y) < 9) &&
                (ABS(camera->focusPoint.z - camera->targetFocusPoint.z) < 9))
            {
                camera->posState = 0;
            }
        }
        else
        {
            CAMERA_SetFocus(camera, &camera->targetFocusPoint);

            CriticalDampPosition(4, &camera->focusPoint, &camera->targetFocusPoint, &camera->focusPointVel, &camera->focusPointAccl, (short)(-camera->smooth));

            CAMERA_SetTarget(camera, &camera->focusPoint);

            if ((ABS(camera->core.position.x - camera->targetPos.x) < 9) &&
                (ABS(camera->core.position.y - camera->targetPos.y) < 9) &&
                (ABS(camera->core.position.z - camera->targetPos.z) < 9))
            {
                camera->posState = 0;
            }
        }
    }

    CAMERA_CalcRotation(&camera->targetRotation, &camera->focusPoint, &camera->core.position);

    COPY_SVEC(Rotation, &camera->core.rotation, Rotation, &camera->targetRotation);

    camera->actual_x_rot = camera->core.rotation.x;

    camera->lagZ = camera->core.rotation.z;

    CAMERA_UpdateFocusRoll(camera);
}

int CAMERA_GetDistSq(SVector *point1, SVector *point2)
{
    Vector d;

    d.x = point1->x - point2->x;
    d.y = point1->y - point2->y;
    d.z = point1->z - point2->z;

    gte_ldlvl(&d);
    gte_nsqr0();
    gte_stlvnl(&d);

    return d.x + d.y + d.z;
}

void CAMERA_NearestPointOnLineVec(SVector *linePoint, SVector *start, SVector *line, Position *point)
{
    MATRIX nmat;
    Vector dpv;
    long t;

    nmat.m[0][0] = line->x;
    nmat.m[0][1] = line->y;
    nmat.m[0][2] = line->z;

    nmat.m[1][0] = start->x;
    nmat.m[1][1] = start->y;
    nmat.m[1][2] = start->z;

    nmat.m[2][0] = point->x;
    nmat.m[2][1] = point->y;
    nmat.m[2][2] = point->z;

    ApplyMatrix(&nmat, (SVECTOR *)line, (VECTOR *)&dpv);

    t = (dpv.y - dpv.z) * 4096;

    if (dpv.x != 0)
    {
        t = -t / dpv.x;
    }

    linePoint->x = start->x + ((line->x * t) >> 12);
    linePoint->y = start->y + ((line->y * t) >> 12);
    linePoint->z = start->z + ((line->z * t) >> 12);
}

int CAMERA_CheckPoint(int linePoint, int linept1, int linept2)
{
    if ((linept1 < (linePoint - 20)) && (linept2 < (linePoint - 20)))
    {
        return 1;
    }

    if (((linePoint + 20) < linept1) && ((linePoint + 20) < linept2))
    {
        return 1;
    }

    return 0;
}

int CAMERA_CheckIfPointOnLine(SVector *linePoint, SVector *linept1, SVector *linept2)
{
    if ((CAMERA_CheckPoint(linePoint->x, linept1->x, linept2->x) == 0) && (CAMERA_CheckPoint(linePoint->y, linept1->y, linept2->y) == 0))
    {
        return CAMERA_CheckPoint(linePoint->z, linept1->z, linept2->z) == 0;
    }
    return 0;
}

int CAMERA_FindLinePoint(Position *point, SVector *linept1, SVector *linept2, int targetdistsq, SVector *results)
{
    SVector outPoint;
    SVector line;
    int calc;
    int hits;

    hits = 0;

    SUB_SVEC(SVector, &line, SVector, linept2, SVector, linept1);

    CAMERA_NearestPointOnLineVec(&outPoint, linept1, &line, point);

    calc = targetdistsq - CAMERA_GetDistSq(&outPoint, (SVector *)point);

    if (calc > 0)
    {
        SVector linePoint;
        int n;

        calc = MATH3D_FastSqrt0(calc);

        CAMERA_Normalize(&line);

        for (n = 0; n < 2; n++)
        {
            linePoint.x = outPoint.x + ((line.x * calc) / 4096);
            linePoint.y = outPoint.y + ((line.y * calc) / 4096);
            linePoint.z = outPoint.z + ((line.z * calc) / 4096);

            if (CAMERA_CheckIfPointOnLine(&linePoint, linept1, linept2) != 0)
            {
                COPY_SVEC(SVector, &results[hits], SVector, &linePoint);

                hits++;
            }

            line.x = -line.x;
            line.y = -line.y;
            line.z = -line.z;
        }
    }

    return hits;
}

void CAMERA_SplineGetNearestPoint2(Camera *camera, Spline *spline, SVector *point, int *currkey, SVector *ret_dpoint)
{
    SplineKey *key;
    int n;
    int target_dist_sq;
    long dist;

    key = spline->key;

    COPY_SVEC(SVector, ret_dpoint, Position, &camera->core.position);

    dist = 0x7FFFFFFF;

    target_dist_sq = camera->targetFocusDistance;
    target_dist_sq *= target_dist_sq;

    for (n = 0; n < (spline->numkeys - 1); n++)
    {
        if ((n >= 0) && (n <= (spline->numkeys - 2)))
        {
            SVector point1;
            SVector point2;
            SVector results[2];
            int i;
            int hits;

            COPY_SVEC(SVector, &point1, vecS, &key[n].point);
            COPY_SVEC(SVector, &point2, vecS, &key[n + 1].point);

            hits = CAMERA_FindLinePoint(&camera->targetFocusPoint, &point1, &point2, target_dist_sq, results);

            for (i = 0; i < hits; i++)
            {
                int tmpdist;

                tmpdist = CAMERA_GetDistSq(point, &results[i]);

                if (tmpdist < dist)
                {
                    COPY_SVEC(SVector, ret_dpoint, SVector, &results[i]);

                    dist = tmpdist;

                    *currkey = n;
                }
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/Game/CAMERA", CAMERA_SplineGetNearestPoint);

void CAMERA_SplineHelpMove(Camera *camera)
{
    SVector camPos;
    Position pos;
    Rotation rotation;
    Instance *focusInstance;
    MultiSpline *posSpline;
    short temp; // not from decls.h

    posSpline = camera->data.Cinematic.posSpline;

    focusInstance = camera->focusInstance;

    if (CAMERA_AngleDifference(splinecam_helprot.z, camera->targetFocusRotation.z) < 400)
    {
        splinecam_helprot.z = camera->targetFocusRotation.z + 2048;

        camera->targetFocusRotation.z &= 0xFFF;
    }

    CAMERA_CalcPosition(&pos, &camera->targetFocusPoint, &splinecam_helprot, camera->targetFocusDistance);

    CAMERA_SplineGetNearestPoint2(camera, posSpline->positional, (SVector *)&pos, &camera->data.Cinematic.splinecam_helpkey, &camPos);

    CAMERA_CalcRotation(&rotation, &camera->focusPoint, (Position *)&camPos);

    splinecam_helprot.x = rotation.x;
    splinecam_helprot.z = rotation.z;

    if (CAMERA_AngleDifference(focusInstance->rotation.z, camera->lagZ + 2048) < 900)
    {
        rotation.z = (rotation.z + 2048) & 0xFFF;

        temp = CAMERA_SignedAngleDifference(rotation.z, camera->lagZ);

        if (abs(temp) < 900)
        {
            camera->lagZ += (temp * 1331) >> 11;
        }
    }
}

void CAMERA_SplineProcess(Camera *camera)
{
    SVector camPos;
    SVector sv;
    short targetFocusDistance;
    MultiSpline *posSpline;
    short smooth;
    short dist_smooth;
    static short hold_flag = 0;
    Rotation targetFocusRotation;

    posSpline = camera->data.Cinematic.posSpline;

    targetFocusRotation.x = camera->targetFocusRotation.x;
    targetFocusRotation.y = 0;
    targetFocusRotation.z = camera->targetFocusRotation.z;

    CAMERA_SetFocus(camera, &camera->targetFocusPoint);

    targetFocusDistance = camera->targetFocusDistance;

    if (posSpline != NULL)
    {
        CAMERA_CalcPosition(&camera->targetPos, &camera->targetFocusPoint, &targetFocusRotation, targetFocusDistance);

        if (camera->mode == 4)
        {
            CAMERA_SplineGetNearestPoint2(camera, posSpline->positional, (SVector *)&camera->targetPos, &camera->data.Cinematic.splinecam_currkey, &camPos);
        }
        else
        {
            CAMERA_SplineGetNearestPoint(posSpline->positional, (SVector *)&camera->targetPos, &camera->data.Cinematic.splinecam_currkey, &camPos);
        }

        camera->maxVel = 512;

        CriticalDampPosition(5, &camera->focusPoint, &camera->targetFocusPoint, &camera->focusPointVel, &camera->focusPointAccl, camera->maxVel);

        if (camera->forced_movement == 1)
        {
            hold_flag = 1;
        }
        else if (CAMERA_FocusInstanceMoved(camera) != 0)
        {
            if (hold_flag != 0)
            {
                camera->always_rotate_flag = 1;
            }

            hold_flag = 0;
        }

        if ((camera->forced_movement != 1) && (hold_flag == 0) && (camera->rotState != 3))
        {
            CAMERA_CalcRotation(&targetFocusRotation, &camera->focusPoint, (Position *)&camPos);

            SUB_SVEC(SVector, &sv, Position, &camera->focusPoint, SVector, &camPos);

            targetFocusDistance = (short)CAMERA_LengthSVector(&sv);

            camera->targetFocusRotation.z = targetFocusRotation.z;
        }

        smooth = 64;

        if (camera->always_rotate_flag != 0)
        {
            dist_smooth = 128;
        }
        else
        {
            smooth = 80;
            dist_smooth = 64;
        }

        camera->focusRotation.y = targetFocusRotation.y;

        camera->x_rot_change = camera->focusRotation.x;

        CriticalDampAngle(1, &camera->focusRotation.x, targetFocusRotation.x, &camera->focusRotVel.x, &camera->focusRotAccl.x, 64);

        camera->x_rot_change = CAMERA_SignedAngleDifference(camera->x_rot_change, camera->focusRotation.x);

        if (camera->rotState == 3)
        {
            targetFocusRotation.z = camera->targetFocusRotation.z;
        }

        CriticalDampAngle(5, &camera->focusRotation.z, targetFocusRotation.z, &camera->focusRotVel.z, &camera->focusRotAccl.z, smooth);

        if ((camera->forced_movement == 1) || (camera->always_rotate_flag != 0) || (hold_flag != 0))
        {
            CAMERA_CalcPosition(&camera->targetPos, &camera->focusPoint, &camera->focusRotation, camera->targetFocusDistance);

            camera->data.Follow.hit = CAMERA_DoCameraCollision2(camera, &camera->targetPos, 1);

            if (camera->data.Follow.hit != 0)
            {
                targetFocusDistance = camera->collisionTargetFocusDistance;
            }
        }

        CriticalDampValue(5, &camera->focusDistance, targetFocusDistance, &camera->focusDistanceVel, &camera->focusDistanceAccl, dist_smooth);

        if (CAMERA_AngleDifference(camera->focusRotation.z, targetFocusRotation.z) < 32)
        {
            camera->always_rotate_flag = 0;

            camera->rotState = 0;
        }

        CAMERA_CalcFollowPosition(camera, &camera->focusRotation);

        if ((camera->mode == 4) && (!(camera->flags & 0x10000)))
        {
            CAMERA_SplineHelpMove(camera);
        }

        CAMERA_CalculateLead(camera);
    }
}

void CAMERA_ShakeCamera(Camera *camera)
{
    if (camera->shake > 0)
    {
        camera->core.position.x += (camera_shakeOffset[(camera->shakeFrame & 0xF)].x * camera->shakeScale) >> 12;
        camera->core.position.y += (camera_shakeOffset[(camera->shakeFrame & 0xF)].y * camera->shakeScale) >> 12;
        camera->core.position.z += (camera_shakeOffset[(camera->shakeFrame & 0xF)].z * camera->shakeScale) >> 12;

        camera->shake -= gameTrackerX.timeMult;

        if (camera->shake < 0)
        {
            camera->shake = 0;
        }

        camera->shakeFrame++;
    }
}

void CAMERA_Process(Camera *camera)
{
    Instance *focusInstance;

    focusInstance = camera->focusInstance;

    camera->focuspoint_fallz -= camera->newFocusInstancePos.z - focusInstance->oldPos.z;

    camera->oldFocusInstancePos = camera->newFocusInstancePos;
    camera->newFocusInstancePos = focusInstance->position;

    camera->oldFocusInstanceRot = camera->newFocusInstanceRot;
    camera->newFocusInstanceRot = focusInstance->rotation;

    if ((camera->signalPos.x == camera->core.position.x) && (camera->signalPos.y == camera->core.position.y) && (camera->signalPos.z == camera->core.position.z))
    {
        camera_still = 1;
    }
    else
    {
        camera_still = 0;
    }

    camera->signalPos = camera->core.position;

    camera->prev_instance_mode = camera->instance_mode;

    camera->x_rot_change = 0;

    camera->instance_mode = CAMERA_QueryMode(camera);

    SUB_SVEC(SVector, &camera->focusInstanceVelVec, Position, &camera->newFocusInstancePos, Position, &camera->oldFocusInstancePos);

    camera->instance_prev_xyvel = camera->instance_xyvel;

    camera->instance_xyvel = MATH3D_FastSqrt0((camera->focusInstanceVelVec.x * camera->focusInstanceVelVec.x) + (camera->focusInstanceVelVec.y * camera->focusInstanceVelVec.y));

    if (shorten_flag == 0)
    {
        shorten_count = 0;
    }

    shorten_flag = 0;

    if (camera->cuckooTimer > 0)
    {
        camera->cuckooTimer--;
    }

    if (camera->mode != 6)
    {
        CAMERA_HandleTransitions(camera);
    }

    switch (camera->mode)
    {
    case 6:
        CAMERA_LookProcess(camera);
        break;
    case 0:
    case 12:
    case 13:
        CAMERA_GenericCameraProcess(camera);
        break;
    case 5:
        CAMERA_CinematicProcess(camera);
        break;
    case 2:
    case 4:
        CAMERA_SplineProcess(camera);
        break;
    case 8:
    case 15:
        return;
    }

    camera->flags &= ~0x800;
    camera->flags &= ~0x1000;

    CAMERA_ShakeCamera(camera);
}

void CAMERA_CenterCamera(Camera *camera)
{
    if ((camera->instance_mode & 0x2000000))
    {
        int tmp1 = CAMERA_AngleDifference(camera->focusRotation.z, (camera->focusInstance->rotation.z + 1024));
        int tmp2 = CAMERA_AngleDifference(camera->focusRotation.z, (camera->focusInstance->rotation.z - 1024));

        if (tmp1 < tmp2)
        {
            CenterFlag = camera->focusInstance->rotation.z + 1024;
        }
        else
        {
            CenterFlag = camera->focusInstance->rotation.z - 1024;
        }
    }
    else
    {
        CenterFlag = camera->focusInstance->rotation.z + 2048;
    }

    camera->focusRotAccl.z = 0;

    camera->focusRotVel.z = 0;

    CenterFlag &= 0xFFF;
}

void CAMERA_SetLookRot(Camera *camera, int x_rotation, int z_rotation)
{
    camera->lookRot.x = x_rotation;
    camera->lookRot.z = z_rotation;
}

void CAMERA_StartLookaroundMode(Camera *camera)
{
    Camera_lookHeight = 512;
    Camera_lookDist = 650;

    CAMERA_SaveMode(camera, camera->mode);

    camera->mode = 6;

    camera->savedFocusDistance = camera->targetFocusDistance;
    camera->savedFocusRotation = camera->targetFocusRotation;

    camera->targetFocusDistance = 650;

    camera->lookRot.x = 0;
    camera->lookRot.y = 0;
    camera->lookRot.z = 0;

    CAMERA_SetLookFocusAndBase(camera->focusInstance, &camera->targetFocusPoint);
}

static short CameraLookStickyFlag = 0;
void CAMERA_StartSwimThrowMode(Camera *camera)
{
    CameraLookStickyFlag = 0x1;

    CAMERA_StartLookaroundMode(camera);

    PLAYER_SetLookAround(camera->focusInstance);

    camera->lookTimer = 2;
}

void CAMERA_EndSwimThrowMode(Camera *camera)
{
    CameraLookStickyFlag = 0;

    CAMERA_EndLook(camera);
}

void CAMERA_ForceEndLookaroundMode(Camera *camera)
{
    CameraLookStickyFlag = 0;

    CAMERA_EndLook(camera);

    camera->flags &= ~0x20000;
}

void CAMERA_Control(Camera *camera, Instance *playerInstance)
{
    long *controlCommand;
    int lookmode;
    Instance *focusInstance;

    controlCommand = &gameTrackerX.controlCommand[0][0];

    focusInstance = camera->focusInstance;

    if ((gameTrackerX.streamFlags & 0x100000))
    {
        return;
    }

    lookmode = 1;

    if (CameraLookStickyFlag == 0)
    {
        lookmode = ((gameTrackerX.controlCommand[0][0] & 0xC00) == 0xC00);
    }

    camera->last_forced_movement = camera->forced_movement;

    if ((camera->instance_xyvel != 0) || (camera->forced_movement != 1) || ((camera->instance_mode & 0x2000000)))
    {
        camera->forced_movement = 0;
    }

    if ((camera->mode == 8) || ((int)camera->instance_mode < 0))
    {
        return;
    }

    if (CenterFlag != -1)
    {
        if (!(camera->instance_mode & 0x2000000))
        {
            camera->forced_movement = 1;
        }

        CriticalDampAngle(1, &camera->focusRotation.z, CenterFlag, &camera->focusRotVel.z, &camera->focusRotAccl.z, 144);

        camera->targetFocusRotation.z = camera->collisionTargetFocusRotation.z = camera->focusRotation.z;

        if (CAMERA_AngleDifference(camera->focusRotation.z, CenterFlag) < 8)
        {
            CenterFlag = -1;
        }
    }
    else
    {
        if ((gameTrackerX.cheatMode == 1) && ((controlCommand[0] & 0xF)))
        {
            return;
        }

        if ((!(camera->lock & 0x4)) && (!(camera->flags & 0x10000)) && ((camera->mode == 0) || (camera->mode == 12) || (camera->mode == 4) || (camera->mode == 13)) && (!(playerInstance->flags & 0x100)))
        {
            if (((controlCommand[0] & 0x400)) && (lookmode == 0))
            {
                if (++camera->leftTimer >= 3)
                {
                    camera->rotDirection = -1;

                    camera->focusRotation.z = (camera->focusRotation.z - ((gameTrackerX.timeMult * 32) >> 12)) & 0xFFF;

                    camera->lastModTime = gameTrackerX.frameCount;

                    camera->forced_movement = 1;

                    camera->data.Follow.stopTimer = 0xE5A20000;

                    camera->focusRotation.z &= 0xFFF;

                    camera->targetFocusRotation.z = camera->collisionTargetFocusRotation.z = camera->focusRotation.z;
                }
            }
            else
            {
                if (((camera->leftTimer >= 1) && (camera->leftTimer < 4)) && (lookmode == 0))
                {
                    CAMERA_CenterCamera(camera);
                }

                camera->leftTimer = 0;
            }

            if (((controlCommand[0] & 0x800)) && (lookmode == 0))
            {
                if (++camera->rightTimer >= 3)
                {
                    camera->rotDirection = 1;

                    camera->forced_movement = 1;

                    camera->focusRotation.z = (camera->focusRotation.z + ((gameTrackerX.timeMult * 32) >> 12)) & 0xFFF;

                    camera->lastModTime = gameTrackerX.frameCount;

                    camera->data.Follow.stopTimer = 0xE5A20000;

                    camera->focusRotation.z &= 0xFFF;

                    camera->targetFocusRotation.z = camera->collisionTargetFocusRotation.z = camera->focusRotation.z;
                }
            }
            else
            {
                if (((camera->rightTimer >= 1) && (camera->rightTimer < 4)) && (lookmode == 0))
                {
                    CAMERA_CenterCamera(camera);
                }

                camera->rightTimer = 0;
            }
        }
        else
        {
            camera->rightTimer = 0;
            camera->leftTimer = 0;
        }

        if ((gameTrackerX.debugFlags2 & 0x2000000))
        {
            if (!(camera->lock & 0x1))
            {
                if ((controlCommand[0] & 0x40000008) == 0x40000008)
                {
                    camera->targetFocusDistance -= 20;

                    if (camera->targetFocusDistance < 200)
                    {
                        camera->targetFocusDistance = 200;
                    }
                }

                if ((controlCommand[0] & 0x40000004) == 0x40000004)
                {
                    camera->targetFocusDistance += 20;

                    if (camera->targetFocusDistance > 4096)
                    {
                        camera->targetFocusDistance = 4096;
                    }
                }
            }

            if (!(camera->lock & 0x2))
            {
                if ((controlCommand[0] & 0x40000002) == 0x40000002)
                {
                    camera->extraXRot += 16;
                }

                if ((controlCommand[0] & 0x40000001) == 0x40000001)
                {
                    camera->extraXRot -= 16;
                }
            }
        }

        if ((lookmode != 0) && (!(playerInstance->flags & 0x100)))
        {
            if (((camera->flags & 0x20000)) && (camera->mode != 5))
            {
                if (++camera->lookTimer == 2)
                {
                    if ((PLAYER_OkToLookAround(focusInstance) != 0) && (!(camera->lock & 0x200)))
                    {
                        CAMERA_StartLookaroundMode(camera);

                        PLAYER_SetLookAround(focusInstance);
                    }
                    else
                    {
                        camera->lookTimer = 0;
                    }
                }
                else if ((camera->lookTimer >= 3) && (camera->mode == 6))
                {
                    PLAYER_TurnHead(focusInstance, &camera->lookRot.x, &camera->lookRot.z, &gameTrackerX);
                }
            }
        }
        else
        {
            camera->flags |= 0x20000;

            CAMERA_EndLook(camera);
        }

        camera->collideRotControl = 0;
    }
}

void CAMERA_EndLook(Camera *camera)
{
    Instance *focusInstance;

    focusInstance = camera->focusInstance;

    if ((camera->lookTimer >= 2) && (camera->mode == 6))
    {
        CAMERA_RestoreMode(camera);

        camera->forced_movement = 0;

        camera->smooth = 8;

        camera->targetFocusDistance = camera->savedFocusDistance;

        camera->targetFocusRotation = camera->savedFocusRotation;

        if ((camera->mode != 4) && (camera->mode != 2))
        {
            camera->targetFocusRotation.z = (focusInstance->rotation.z + 2048) & 0xFFF;

            CAMERA_CenterCamera(camera);
        }

        camera->always_rotate_flag = 1;

        camera->collisionTargetFocusRotation.z = camera->targetFocusRotation.z;

        if (!(camera->flags & 0x10000))
        {
            camera->actual_x_rot = camera->core.rotation.x;
        }

        PLAYER_ReSetLookAround(focusInstance);
    }

    camera->lookTimer = 0;
}

void CAMERA_ChangeToUnderWater(Camera *camera, Instance *instance)
{
    if (instance == camera->focusInstance)
    {
        if (camera->mode != 4)
        {
            if (camera->mode != 5)
            {
                if (camera->mode != 13)
                {
                    CAMERA_SetMode(camera, 13);
                }
                else
                {
                    CAMERA_CreateNewFocuspoint(camera);
                }

                camera->smooth = 8;

                camera->targetFocusDistance = 1600;

                camera->collisionTargetFocusDistance = 1600;

                camera->signalFocusDistance = 1600;

                camera->rotationVel.z = 32;

                camera->always_rotate_flag = 0;
            }
            else
            {
                CAMERA_SaveMode(camera, 13);
            }
        }

        camera->flags |= 0x10000;
    }
}

void CAMERA_ChangeToOutOfWater(Camera *camera, Instance *instance)
{
    if (instance == camera->focusInstance)
    {
        if (camera->mode != 5)
        {
            CAMERA_SetMode(camera, 13);

            camera->rotationVel.z = 0;

            camera->targetTilt = 0;
        }
        else
        {
            CAMERA_SaveMode(camera, 13);
        }

        camera->flags &= ~0x10000;
    }
}

void CAMERA_UpdateFocusDistance(Camera *camera)
{
    int smooth;
    long dampMode;

    if ((camera->forced_movement != 2) && (!(camera->lock & 0x1)))
    {
        if ((camera->targetFocusDistance < camera->collisionTargetFocusDistance) && (!(camera->instance_mode & 0x82000000)))
        {
            camera->collisionTargetFocusDistance = camera->targetFocusDistance;
        }

        dampMode = 6;

        if (camera->collisionTargetFocusDistance < camera->focusDistance)
        {
            if (camera->collisionTargetFocusDistance < camera->targetFocusDistance)
            {
                smooth = 512;

                dampMode = 5;
            }
            else
            {
                smooth = 128;

                dampMode = 1;
            }
        }
        else
        {
            smooth = 64;
        }

        CriticalDampValue(dampMode, &camera->focusDistance, camera->collisionTargetFocusDistance, &camera->focusDistanceVel, &camera->focusDistanceAccl, smooth);
    }
}

void CAMERA_UpdateFocusTilt(Camera *camera)
{
    if ((camera->flags & 0x1800))
    {
        camera->x_rot_change = 0;

        camera->focusRotation.x = camera->tfaceTilt;
    }
    else
    {
        camera->x_rot_change = camera->focusRotation.x;

        if (((camera->forced_movement != 3) && (!(camera->lock & 0x2))) || ((camera->flags & 0x10000)))
        {
            CriticalDampAngle(1, &camera->focusRotation.x, camera->tfaceTilt, &camera->focusRotVel.x, &camera->focusRotAccl.x, 32);
        }
        else
        {
            camera->focusRotation.x = camera->targetFocusRotation.x;
        }

        camera->x_rot_change = CAMERA_SignedAngleDifference(camera->x_rot_change, camera->focusRotation.x);
    }
}

void CAMERA_UpdateFocusRoll(Camera *camera)
{
    long tmp_inc;
    int temp; // not from decls.h

    if (roll_inc != 0)
    {
        if (gameTrackerX.timeMult != 4096)
        {
            temp = (roll_inc / 32) * gameTrackerX.timeMult;

            tmp_inc = temp / 128;
        }
        else
        {
            tmp_inc = roll_inc;
        }

        if (CAMERA_AngleDifference(current_roll_amount / 4096, roll_target / 4096) <= abs(tmp_inc / 4096))
        {
            current_roll_amount = roll_target;

            roll_inc = 0;
        }
        else
        {
            current_roll_amount += tmp_inc;
        }

        current_roll_amount &= 0xFFFFFF;
    }

    camera->core.rotation.y = current_roll_amount / 4096;
}

void CAMERA_UpdateFocusRotate(Camera *camera)
{
    int dampspeed;
    long dampmode;

    if (camera->forced_movement != 1)
    {
        if (!(camera->lock & 0x4))
        {
            dampmode = 1;

            if (camera->always_rotate_flag != 0)
            {
                dampspeed = -camera->smooth;
            }
            else if (!(camera->flags & 0x10000))
            {
                dampmode = 5;
                dampspeed = 160;
            }
            else
            {
                dampspeed = 128;
            }

            CriticalDampAngle(dampmode, &camera->focusRotation.z, camera->collisionTargetFocusRotation.z, &camera->focusRotVel.z, &camera->focusRotAccl.z, (short)dampspeed);

            if (CAMERA_AngleDifference(camera->targetFocusRotation.z, camera->focusRotation.z) < 4)
            {
                camera->always_rotate_flag = 0;
            }
        }
        else
        {
            camera->focusRotation.z = camera->targetFocusRotation.z;
        }
    }
    else if ((camera->lock & 0x4))
    {
        camera->focusRotation.z = camera->targetFocusRotation.z;
    }
}

void CAMERA_UpdateFocusRotationX(Camera *camera, Instance *focusInstance)
{
    short cameraPlayerRotX;
    short dist;
    short tfaceFlag;
    Normal normal;
    // int mult; unused
    int tmpsmooth;

    dist = camera->targetFocusDistance;

    tfaceFlag = 0;

    if ((focusInstance->tface != NULL) &&
        (((Level *)focusInstance->tfaceLevel)->terrain != NULL) &&
        (focusInstance->tface->textoff != 0xFFFF) &&
        ((((TextureFT3 *)((char *)((Level *)focusInstance->tfaceLevel)->terrain->StartTextureList + focusInstance->tface->textoff))->attr & 0x8000)) && /*TODO: FAKEMATCH?*/
        (dist < 2912))
    {
        COLLIDE_GetNormal(focusInstance->tface->normal, (short *)((Level *)focusInstance->tfaceLevel)->terrain->normalList, (SVector *)&normal);

        if (normal.z < 3950)
        {
            tfaceFlag = 1;

            camera->targetTilt = (CAMERA_CalcTilt(&normal, camera->focusRotation.z) * 9) / 16;

            if (camera->targetTilt < -256)
            {
                camera->targetTilt = -256;
            }
            else if (camera->targetTilt > 256)
            {
                camera->targetTilt = 256;
            }

            if ((2912 - dist) < 512)
            {
                camera->targetTilt = (camera->targetTilt * (2912 - dist)) / 512;
            }

            CriticalDampAngle(1, &camera->tilt, camera->targetTilt, &camera->tiltVel, &camera->tiltAccl, 8);
        }
    }

    if (tfaceFlag == 0)
    {
        tmpsmooth = 24;

        if (!(camera->instance_mode & 0x38))
        {
            camera->targetTilt = 0;
        }

        if (((camera->instance_mode & 0x2000)) && (camera->real_focuspoint.z < camera->focuspoint_fallz))
        {
            camera->targetTilt = -384;

            tmpsmooth = 12;
        }

        CriticalDampAngle(1, &camera->tilt, camera->targetTilt, &camera->tiltVel, &camera->tiltAccl, tmpsmooth);
    }

    cameraPlayerRotX = (((camera->targetFocusRotation.x + camera->tilt + camera->extraXRot) & 0xFFF));

    if ((cameraPlayerRotX > 768) && (cameraPlayerRotX < 2048))
    {
        cameraPlayerRotX = 768;
    }
    else if ((cameraPlayerRotX >= 2048) && (cameraPlayerRotX < 3328))
    {
        cameraPlayerRotX = -768;
    }

    cameraPlayerRotX &= 0xFFF;

    camera->tfaceTilt = cameraPlayerRotX;

    if ((camera->instance_mode & 0x2000000))
    {
        if (cameraPlayerRotX > 2048)
        {
            cameraPlayerRotX |= 0xF000;
        }

        if (combat_cam_angle < cameraPlayerRotX)
        {
            camera->tfaceTilt = combat_cam_angle + 0x1000;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/Game/CAMERA", CAMERA_FollowPlayerTilt);

void CAMERA_FollowGoBehindPlayerWithTimer(Camera *camera)
{
    Instance *focusInstance;

    focusInstance = camera->focusInstance;

    if (camera->data.Follow.hit != 0)
    {
        camera->data.Follow.stopTimer = 0xE5A20000;
    }

    if (CAMERA_FocusInstanceMoved(camera) != 0)
    {
        camera->data.Follow.stopTimer = -CameraCenterDelay * 122880;
    }
    else if (!(gameTrackerX.streamFlags & 0x100000))
    {
        camera->data.Follow.stopTimer += gameTrackerX.timeMult;
    }

    if (camera->data.Follow.stopTimer > 0)
    {
        Decouple_AngleMoveToward(&camera->targetFocusRotation.z, focusInstance->rotation.z + 2048, 32);
        Decouple_AngleMoveToward(&camera->collisionTargetFocusRotation.z, focusInstance->rotation.z + 2048, 32);

        CriticalDampAngle(1, &camera->focusRotation.z, camera->collisionTargetFocusRotation.z, &camera->focusRotVel.z, &camera->focusRotAccl.z, 32);

        camera->forced_movement = 1;
    }
}

void CAMERA_FollowGoBehindPlayer(Camera *camera)
{
    Instance *focusInstance;

    focusInstance = camera->focusInstance;

    Decouple_AngleMoveToward(&camera->targetFocusRotation.z, focusInstance->rotation.z + 2048, camera->rotationVel.z);
}

void CAMERA_CalculateLead(Camera *camera)
{
    short lead_target;
    short lead_smooth;

    if ((!(camera->flags & 0x10000)) && (camera->mode != 6) && (!(camera->instance_mode & 0x3002043)))
    {
        int speedxy;
        short angle;

        speedxy = camera->instance_xyvel;

        angle = CAMERA_SignedAngleDifference((camera->focusInstance->rotation.z + 2048), camera->core.rotation.z);

        if (speedxy >= 23)
        {
            camera->lead_timer++;
        }
        else if (camera->instance_prev_xyvel == 0)
        {
            camera->lead_timer = 0;
        }

        if (speedxy >= 23)
        {
            int calc;

            lead_target = 0;
            lead_smooth = 3;

            if ((abs(angle) > 400) && (abs(angle) < 1600))
            {
                if (camera->lead_timer > 35)
                {
                    calc = ((camera->lead_timer - 35) * 3) + 35;
                }
                else
                {
                    calc = camera->lead_timer;
                }

                if (angle > 0)
                {
                    if (calc < 80)
                    {
                        lead_target = calc;
                    }
                    else
                    {
                        lead_target = 80;
                    }
                }
                else
                {
                    if (calc < 80)
                    {
                        lead_target = -calc;
                    }
                    else
                    {
                        lead_target = -80;
                    }
                }

                if (CAMERA_AngleDifference(lead_target, camera->lead_angle) > 80)
                {
                    lead_smooth = 12;
                }
                else
                {
                    lead_smooth = 8;
                }
            }
        }
        else
        {
            lead_target = 0;
            lead_smooth = 3;
        }
    }
    else
    {
        camera->lead_timer = 0;

        lead_target = 0;
        lead_smooth = 3;
    }

    CriticalDampAngle(1, &camera->lead_angle, lead_target, &camera->lead_vel, &camera->lead_accl, lead_smooth);

    camera->core.rotation.z = (camera->core.rotation.z + camera->lead_angle) & 0xFFF;
}

void CAMERA_CalcFollowPosition(Camera *camera, Rotation *rotation)
{
    Instance *focusInstance;

    focusInstance = camera->focusInstance;

    CAMERA_CalcPosition(&camera->targetPos, &camera->focusPoint, rotation, camera->focusDistance);

    camera->core.position = camera->targetPos;

    COPY_SVEC(Rotation, &camera->core.rotation, Rotation, rotation);

    if (!(camera->flags & 0x10000))
    {
        short target_rotx;
        int hypotXY;
        int smooth;
        int diff;
        Vector dpv;
        int zdiff;

        camera->actual_x_rot -= camera->x_rot_change;

        dpv.x = camera->real_focuspoint.x - camera->targetPos.x;
        dpv.y = camera->real_focuspoint.y - camera->targetPos.y;
        dpv.z = 0;

        gte_ldlvl(&dpv);
        gte_nsqr0();
        gte_stlvnl(&dpv);

        hypotXY = MATH3D_FastSqrt0(dpv.x + dpv.y);

        diff = ratan2(camera->real_focuspoint.z - camera->targetPos.z, hypotXY);

        target_rotx = diff;

        if ((camera->instance_mode & 0x1038))
        {
            int velz;

            velz = camera->focusInstanceVelVec.z;

            if (focusInstance->shadowPosition.z != focusInstance->position.z)
            {
                if (velz < 0)
                {
                    if (velz < -260)
                    {
                        velz = -520 - velz;

                        if (velz > 0)
                        {
                            velz = 0;
                        }
                    }
                    {
                        int ground;
                        int pos;

                        pos = camera->real_focuspoint.z + (velz * 2);

                        ground = focusInstance->shadowPosition.z + 352;

                        if (pos < ground)
                        {
                            ground -= camera->targetPos.z;
                        }
                        else
                        {
                            ground = pos - camera->targetPos.z;
                        }

                        target_rotx = (short)ratan2(ground, hypotXY);

                        if (CAMERA_SignedAngleDifference(target_rotx, camera->actual_x_rot) > 0)
                        {
                            target_rotx = camera->actual_x_rot;
                        }
                    }
                }
                else
                {
                    target_rotx = camera->core.rotation.x;
                }
            }
        }
        else if (CAMERA_AngleDifference(target_rotx, camera->core.rotation.x) < 4)
        {
            target_rotx = camera->core.rotation.x;
        }

        if ((camera->flags & 0x1800))
        {
            camera->actual_x_rot = target_rotx;

            camera->x_rot_change = 0;
        }

        zdiff = CAMERA_SignedAngleDifference(target_rotx, camera->actual_x_rot);

        if ((camera->instance_mode & 0x2000))
        {
            if (zdiff >= 81)
            {
                smooth = (zdiff - 80) / 3;

                if (smooth >= 4)
                {
                    if (smooth >= 25)
                    {
                        smooth = 24;
                    }
                }
                else
                {
                    smooth = 4;
                }
            }
            else
            {
                smooth = 24;

                if (zdiff > 0)
                {
                    smooth = 4;
                }
            }
        }
        else
        {
            smooth = 24;
        }

        if (smooth != 0)
        {
            CriticalDampAngle(1, &camera->actual_x_rot, target_rotx, &camera->actual_vel_x, &camera->actual_acc_x, (short)smooth);
        }
        else
        {
            camera->actual_acc_x = 0;
            camera->actual_vel_x = 0;
        }

        camera->core.rotation.x = camera->actual_x_rot;
    }
    else
    {
        camera->actual_x_rot = camera->core.rotation.x;
    }

    camera->lagZ = camera->core.rotation.z;
}

void CAMERA_SetupColInfo(Camera *camera, CameraCollisionInfo *colInfo, Position *targetCamPos)
{
    static short toggle = 0;
    if (camera->mode == 6)
    {
        COPY_SVEC(Position, &camera->focusSphere.position, Position, &camera->targetFocusPoint);
    }
    else
    {
        COPY_SVEC(Position, &camera->focusSphere.position, Position, &camera->real_focuspoint);
    }

    COPY_SVEC(Position, &camera->posSphere.position, Position, targetCamPos);

    colInfo->start = &camera->focusSphere;
    colInfo->end = &camera->posSphere;

    if (camera->data.Follow.tface == NULL)
    {
        colInfo->cldLines = 6;

        if (toggle != 0)
        {
            colInfo->cldLines = 14;
        }
        else
        {
            colInfo->cldLines = 22;
        }
    }
    else
    {
        colInfo->cldLines = 30;
    }

    if (toggle != 0)
    {
        toggle = 0;
    }
    else
    {
        toggle = 1;
    }
}

void CAMERA_DoPanicCheck(Camera *camera, CameraCollisionInfo *tmpcolInfo, Rotation *rotation, short *best_z, short *max_dist)
{
    Position targetCamPos;

    CAMERA_CalcPosition(&targetCamPos, &camera->focusPoint, rotation, camera->targetFocusDistance);

    COPY_SVEC(Position, &camera->posSphere.position, Position, &targetCamPos);

    CAMERA_SphereToSphereWithLines(camera, tmpcolInfo, 0);

    if ((tmpcolInfo->numCollided == 0) || (*max_dist < tmpcolInfo->lenCenterToExtend))
    {
        *best_z = rotation->z;

        *max_dist = tmpcolInfo->lenCenterToExtend;
    }
}

static short panic_count = 0;
void CAMERA_Panic(Camera *camera, short min_dist)
{
    Position targetCamPos;
    Rotation rotation;
    int n;
    short best_z;
    short max_dist;
    CameraCollisionInfo tmpcolInfo;
    short free_count1;
    short free_count2;

    free_count1 = 0;
    free_count2 = 0;

    max_dist = min_dist;

    CAMERA_SetupColInfo(camera, &tmpcolInfo, &targetCamPos);

    n = 0;

    rotation = camera->focusRotation;

    best_z = rotation.z;

    while (1)
    {
        rotation.z = camera->focusRotation.z + n;

        CAMERA_DoPanicCheck(camera, &tmpcolInfo, &rotation, &best_z, &max_dist);

        if (tmpcolInfo.numCollided == 0)
        {
            free_count1++;

            if (2 < ((free_count1 * 65536) >> 16))
            {
                break;
            }
        }

        rotation.z = camera->focusRotation.z - n;

        CAMERA_DoPanicCheck(camera, &tmpcolInfo, &rotation, &best_z, &max_dist);

        if (tmpcolInfo.numCollided == 0)
        {
            free_count2++;

            if (2 < ((free_count2 * 65536) >> 16))
            {
                break;
            }
        }

        n += 128;

        if (n > 2047)
        {
            break;
        }
    }

    if (max_dist == min_dist)
    {
        panic_count = -32767;
    }
    else
    {
        camera->always_rotate_flag = 0x1;

        camera->rotState = 3;

        camera->smooth = -112;

        camera->targetFocusRotation.z = best_z;

        camera->collisionTargetFocusRotation.z = best_z;

        camera->signalRot.z = best_z;
    }
}

long CAMERA_DoCameraCollision2(Camera *camera, Position *targetCamPos, int simpleflag)
{
    int secondcheck_flag;
    long hit;
    CameraCollisionInfo colInfo;
    static int collisiontimeDown = 0;
    int speed;
    int angle1;
    int angle2;

    hit = 0;

    CAMERA_SetupColInfo(camera, &colInfo, targetCamPos);

    secondcheck_flag = GetSecondCheckFlag(camera);

    camera->data.Follow.tface = CAMERA_SphereToSphereWithLines(camera, &colInfo, secondcheck_flag);

    if (((camera->instance_mode & 0x2000000)) && (colInfo.numCollided > 0) && (colInfo.lenCenterToExtend < 600))
    {
        CenterFlag = -1;

        if ((colInfo.numCollided != 1) || (!(colInfo.flags & 0x6)))
        {
            if (combat_cam_weight < 4096)
            {
                combat_cam_weight += 144;

                if (combat_cam_weight >= 4096)
                {
                    combat_cam_weight = 4096;
                }

                if (combat_cam_weight < 3900)
                {
                    return 1;
                }
            }
        }
    }

    if ((!(camera->flags & 0x12000)) && (camera->instance_xyvel == 0) && (secondcheck_flag == 0) && (camera->always_rotate_flag == 0) && (camera->forced_movement == 0) && (colInfo.numCollided > 0) && ((colInfo.numCollided == 4) || (camera_still != 0)) && (colInfo.lenCenterToExtend < 400))
    {
        panic_count++;

        if ((((gameTrackerX.controlCommand[0][0] & 0x1)) && (panic_count > 10)) || ((!(gameTrackerX.controlCommand[0][0] & 0x1)) && (panic_count > 1)))
        {
            CAMERA_Panic(camera, (short)colInfo.lenCenterToExtend);
        }
    }
    else
    {
        panic_count = 0;
    }

    if ((camera->data.Follow.tface != NULL) && (secondcheck_flag != 0))
    {
        COPY_SVEC(Position, &camera->focusSphere.position, Position, &camera->targetFocusPoint);

        camera->data.Follow.tface = CAMERA_SphereToSphereWithLines(camera, &colInfo, 0);
    }

    if (simpleflag != 0)
    {
        if (camera->data.Follow.tface != NULL)
        {
            if ((camera->mode == 4) || (camera->mode == 2) || (camera->mode == 6))
            {
                camera->collisionTargetFocusDistance = colInfo.lengthList[colInfo.line];
            }

            return 1;
        }

        return 0;
    }

    if (collisiontimeDown > 0)
    {
        collisiontimeDown--;
    }

    if (camera->always_rotate_flag != 0)
    {
        colInfo.numCollided = 4;
    }

    if (((camera->flags & 0x10000)) || ((camera->instance_mode & 0x24000000)) || (((camera->flags & 0x2000)) && (!(camera->instance_mode & 0x2000000))) || (camera->always_rotate_flag != 0))
    {
        if (camera->data.Follow.tface != NULL)
        {
            hit = CAMERA_AbsoluteCollision(camera, &colInfo);

            collisiontimeDown = 30;
        }
        else
        {
            camera->collisionTargetFocusDistance = camera->targetFocusDistance;
        }

        if ((camera->flags & 0x10000))
        {
            if ((AngleDiff(camera->collisionTargetFocusRotation.z, camera->targetFocusRotation.z) << 16) >= 0)
            {
                speed = AngleDiff(camera->collisionTargetFocusRotation.z, camera->targetFocusRotation.z);
            }
            else
            {
                speed = -AngleDiff(camera->collisionTargetFocusRotation.z, camera->targetFocusRotation.z);
            }

            if (!(colInfo.flags & 0x6))
            {
                CAMERA_dampgetline(0);
            }

            if ((speed < 1024) || ((colInfo.flags & 0x6)))
            {
                speed /= 16;

                if (speed < 8)
                {
                    speed = 8;
                }

                if (speed > 32)
                {
                    speed = 32;
                }
            }
            else
            {
                speed = 64;
            }

            Decouple_AngleMoveToward(&camera->collisionTargetFocusRotation.z, camera->targetFocusRotation.z, speed);

            return hit;
        }
        else
        {
            Decouple_AngleMoveToward(&camera->collisionTargetFocusRotation.z, camera->targetFocusRotation.z, 64);

            return hit;
        }
    }

    if (camera->data.Follow.tface != NULL)
    {
        hit = CAMERA_AbsoluteCollision(camera, &colInfo);

        collisiontimeDown = 30;

        camera->targetFocusRotation.z = camera->collisionTargetFocusRotation.z;
    }
    else
    {
        CAMERA_dampgetline(0);

        if ((camera->mode == 13) && (camera->instance_xyvel > 0))
        {
            if (CAMERA_AngleDifference(camera->collisionTargetFocusRotation.z, camera->focusRotation.z) < 5)
            {
                camera->collisionTargetFocusRotation.z = camera->targetFocusRotation.z;

                if (collisiontimeDown == 0)
                {
                    camera->collision_lastPush = 0;
                }
            }
            else
            {
                angle1 = CAMERA_SignedAngleDifference(camera->collisionTargetFocusRotation.z, camera->focusRotation.z);
                angle2 = CAMERA_SignedAngleDifference(camera->targetFocusRotation.z, camera->focusRotation.z);

                if (((angle1 < 0) && (angle2 < 0)) || ((angle1 > 0) && (angle2 > 0)))
                {
                    if (abs(angle2) > abs(angle1))
                    {
                        camera->collisionTargetFocusRotation.z = camera->targetFocusRotation.z;
                    }
                }
            }
        }
        else if (collisiontimeDown == 0)
        {
            Decouple_AngleMoveToward(&camera->collisionTargetFocusRotation.z, camera->targetFocusRotation.z, 64);

            camera->collision_lastPush = 0;
        }

        if ((camera->instance_mode & 0x2000000))
        {
            collisiontimeDown = 0;

            camera->collisionTargetFocusDistance = combat_cam_distance;
        }
        else
        {
            camera->collisionTargetFocusDistance = camera->targetFocusDistance;
        }
    }

    return hit;
}

int CAMERA_FocusInstanceMoved(Camera *camera)
{
    return ((camera->newFocusInstancePos.x != camera->oldFocusInstancePos.x) || (camera->newFocusInstancePos.y != camera->oldFocusInstancePos.y) || (camera->newFocusInstancePos.z != camera->oldFocusInstancePos.z) || (camera->newFocusInstanceRot.x != camera->oldFocusInstanceRot.x) || (camera->newFocusInstanceRot.y != camera->oldFocusInstanceRot.y) || (camera->newFocusInstanceRot.z != camera->oldFocusInstanceRot.z));
}
