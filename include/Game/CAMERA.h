#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "common.h"

void CAMERA_SetLookFocusAndDistance(Camera *camera, VECTOR *focuspoint, int distance);
void Decouple_AngleMoveToward(short *current_ptr, short destination, short step);
void CriticalDampAngle(long dampMode, short *currentVal, short target, short *vel, short *accl, int smooth);
short CAMERA_AngleDifference(short angle0, short angle1);
void CriticalDampValue(long dampMode, short *sourceVal, short targetVal, short *vel, short *accl, int smooth);
void CAMERA_SetMode(Camera *camera, long mode);
void CAMERA_RestoreMode(Camera *camera);
void CAMERA_CalcVVClipInfo(Camera *camera);
long CAMERA_CalcTilt(Normal *normal, short zRot);
unsigned long CAMERA_QueryMode(Camera *camera);
void CAMERA_HandleTransitions(Camera *camera);
void CAMERA_LookProcess(Camera *camera);
void CAMERA_GenericCameraProcess(Camera *camera);
void CAMERA_CinematicProcess(Camera *camera);
void CAMERA_SplineProcess(Camera *camera);
void CAMERA_ShakeCamera(Camera *camera);
void CAMERA_SplineGetNearestPoint2(Camera *camera, Spline *spline, SVector *point, int *currkey, SVector *ret_dpoint);
void CAMERA_CalcRotation(Rotation *rotation, Position *target, Position *position);
void CAMERA_SetFocus(Camera *camera, Position *targetfocusPoint);
void CAMERA_SplineGetNearestPoint(Spline *spline, SVector *point, int *currkey, SVector *ret_dpoint);
void CriticalDampPosition(long dampMode, Position *position, Position *targetPos, SVector *vel, SVector *accl, int smooth);
long CAMERA_DoCameraCollision2(Camera *camera, Position *targetCamPos, int simpleflag);
void CAMERA_CalcFollowPosition(Camera *camera, Rotation *rotation);
void CAMERA_CalculateLead(Camera *camera);
void CAMERA_Normalize(SVector *svector);
void CAMERA_CalcPosition(Position *position, Position *base, Rotation *rotation, short distance);
SVector *SplineGetNextPointDC(Spline *spline, SplineDef *def);
void CAMERA_SetMaxVel(Camera *camera);
void CAMERA_UpdateFocusRoll(Camera *camera);
long CAMERA_ACNoForcedMovement(Camera *camera, CameraCollisionInfo *colInfo);
long CAMERA_ACForcedMovement(Camera *camera, CameraCollisionInfo *colInfo);
void CAMERA_Restore(Camera *camera, long restore);
void CAMERA_ChangeToUnderWater(Camera *camera, Instance *instance);
int WARPGATE_IsWarpgateActive();
void CAMERA_Adjust_rotation(Camera *camera, long rotation);
void AngleMoveToward(short *current_ptr, short destination, short step);
void CAMERA_Adjust(Camera *camera, long adjust);
void CAMERA_ChangeTo(Camera *camera, CameraKey *cameraKey);
void CAMERA_Lock(Camera *camera, long lock);
void CAMERA_Unlock(Camera *camera, long unlock);
void CAMERA_SetSmoothValue(Camera *camera, long smooth);
void CAMERA_SetTimer(Camera *camera, long time);
void CAMERA_Save(Camera *camera, long save);
void CAMERA_SetValue(Camera *camera, long index, long value);
void CAMERA_SetShake(Camera *camera, long shake, long scale);
void CAMERA_Adjust_tilt(Camera *camera, long tilt);
void CAMERA_Adjust_distance(Camera *camera, long dist);
void CAMERA_Relocate(Camera *camera, SVector *offset, int streamSignalFlag);
long CAMERA_LengthSVector(SVector *sv);
short CAMERA_GetLineAngle(Camera *camera, CameraCollisionInfo *colInfo, SVector *linept, int line);
int CAMERA_FocusInstanceMoved(Camera *camera);
void CAMERA_EndLook(Camera *camera);
void CAMERA_ForceEndLookaroundMode(Camera *camera);
void CAMERA_ChangeToOutOfWater(Camera *camera, Instance *instance);
void CAMERA_EndSwimThrowMode(Camera *camera);
void CAMERA_SetLookRot(Camera *camera, int x_rotation, int z_rotation);
void CAMERA_StartSwimThrowMode(Camera *camera);
void CAMERA_Initialize(Camera *camera);
void CAMERA_SetInstanceFocus(Camera *camera, Instance *instance);
void CAMERA_CalcFocusOffset(SVector *offset, Camera *camera);
void CAMERA_FollowPlayerTilt(Camera *camera, Instance *focusInstance);
void CAMERA_UpdateFocusRotationX(Camera *camera, Instance *focusInstance);
void CAMERA_FollowGoBehindPlayer(Camera *camera);
void CAMERA_FollowGoBehindPlayerWithTimer(Camera *camera);
void CAMERA_CombatCamDist(Camera *camera);
void CAMERA_UpdateFocusDistance(Camera *camera);
short CAMERA_update_z_damped(Camera *camera, short current, short target);
void CAMERA_UpdateFocusTilt(Camera *camera);
void CAMERA_UpdateFocusRotate(Camera *camera);
void CAMERA_Process(Camera *camera);
void CAMERA_SetViewVolume(Camera *camera);
void CAMERA_Control(Camera *camera, Instance *playerInstance);
void CAMERA_Adjust_roll(long roll_degrees, int frames);

extern int CameraCenterDelay;
Camera theCamera;

#endif
