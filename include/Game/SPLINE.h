#ifndef _SPLINE_H_
#define _SPLINE_H_

#include "common.h"

unsigned long SplineGetOffsetNext(Spline *spline, SplineDef *def, long fracOffset);
unsigned long SplineGetData(Spline *spline, SplineDef *def, void *p);
unsigned short SplineGetFrameNumber(Spline *spline, SplineDef *def);
G2Quat *SplineGetFirstRot(RSpline *rspline, SplineDef *def);
SVector *SplineGetFirstPoint(Spline *spline, SplineDef *def);
SVector *SplineGetNearestPoint(Spline *spline, SVector *point, SplineDef *def);
short SplineMultiIsWhere(MultiSpline *multi);
SVector *SplineGetOffsetPreviousPoint(Spline *spline, SplineDef *def, long offset);
SVector *SplineGetOffsetNextPoint(Spline *spline, SplineDef *def, long offset);
unsigned long SplineGetQuatData(Spline *spline, SplineDef *def, void *p);
unsigned long SplineGetOffsetPrev(Spline *spline, SplineDef *def, long fracOffset);
SVector *SplineGetNextPoint(Spline *spline, SplineDef *def);
SVector *SplineGetPreviousPoint(Spline *spline, SplineDef *def);
short SplineSetDef2FrameNumber(Spline *spline, SplineDef *def, unsigned short frame_number);

#endif
