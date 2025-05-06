#ifndef _HASM_H_
#define _HASM_H_

#include "common.h"

void G2Quat_ToMatrix_S(G2Quat *quat, G2Matrix *matrix);
short MATH3D_racos_S(long a);
long COLLIDE_IntersectLineAndPlane_S(SVector *planePoint, Position *oldPos, Position *position, SVector *normal, long z);
void COLLIDE_NearestPointOnPlane_S(SVector *planePoint, SVector *normal, long z, Position *position);
int CheckVolatile(void *);
void G2Quat_FromEuler_S(G2Quat *quatInfo, G2EulerAngles *preQuat);
void COLLIDE_NearestPointOnLine_S(SVector *linePoint, SVECTOR *oldPoint, SVECTOR *newPoint, Position *position);
void G2Quat_FromMatrix_S(G2Quat *quat, G2Matrix *matrix);
unsigned long *DRAW_SplitModel_S(Model *model, Instance *instance, VertexPool *textexPool, MATRIX *transform, PrimPool *primPool, unsigned long **ot);
long PIPE3D_TransformAnimatedInstanceVertices_S(VertexPool *vertexPool, PVertex *poolVertex, Model *model, MATRIX *wcTransform, MATRIX *matrixPool, MATRIX *lm, CVECTOR *vertexColor, CVECTOR *perVertexColor);
int *DRAW_AnimatedModel_S(Model *model, PVertex *poolVertex, PrimPool *primPool, unsigned long **ot, CVECTOR *vertexColor);
unsigned long *DRAW_DisplayPolytopeList_S(PolytopeList *polytopeList, Terrain *terrain, Camera *camera, PrimPool *primPool, unsigned long **drawot, Position *globalOffset);
void BSP_MarkVisibleLeaves_S(BSPTree *bsp, Camera *camera, PolytopeList *polytopeList);
void VM_ProcessVMObjectList_S(Level *level, Camera *camera);
long TimerTick();
unsigned long GetGp();
void SetGp(unsigned long);

long depthQFogStart;
long depthQBlendStart;
long depthQBackColor;
long depthQFogFar;

extern void *overlayAddress;
extern long modelFadeValue;
extern long draw_belowSplit;
extern volatile int gameTimer;
extern short forceDrawInFront;
extern struct SVECTOR *face_v0;
extern struct SVECTOR *face_v1;
extern struct SVECTOR *face_v2;
extern struct SVECTOR *face_v01;
extern struct SVECTOR *face_v12;
extern struct SVECTOR *face_v20;
extern struct UVTYPE *face_uv0;
extern struct UVTYPE *face_uv1;
extern struct UVTYPE *face_uv2;
extern struct UVTYPE *face_uv01;
extern struct UVTYPE *face_uv12;
extern struct UVTYPE *face_uv20;

#endif
