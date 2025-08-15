#ifndef _SCRIPT_H_
#define _SCRIPT_H_

#include "common.h"

SplineDef *SCRIPT_GetPosSplineDef(Instance *instance, MultiSpline *multi, unsigned long isParent, unsigned long isClass);
int SCRIPT_GetSplineFrameNumber(Instance *instance, SplineDef *splineDef);
void ScriptKillInstance(Instance *instance, int effect);
void SCRIPT_InstanceSplineSet(Instance *instance, short frameNum, SplineDef *splineDef, SplineDef *rsplineDef, SplineDef *ssplineDef);
short SCRIPTCountFramesInSpline(Instance *instance);
SplineDef *SCRIPT_GetRotSplineDef(Instance *instance, MultiSpline *multi, unsigned long isParent, unsigned long isClass);
SplineDef *SCRIPT_GetScaleSplineDef(Instance *instance, MultiSpline *multi, unsigned long isParent, unsigned long isClass);
long SCRIPT_SplineProcess(Instance *instance, MultiSpline *multi, SplineDef *splineDef, SplineDef *rsplineDef, SplineDef *ssplineDef, int direction, int isClass);
MultiSpline *SCRIPT_GetMultiSpline(Instance *instance, unsigned long *isParent, unsigned long *isClass);
void SCRIPT_InstanceSplineInit(Instance *instance);
long SCRIPT_InstanceSplineProcess(Instance *instance, SplineDef *splineDef, SplineDef *rsplineDef, SplineDef *ssplineDef, int direction);

#endif
