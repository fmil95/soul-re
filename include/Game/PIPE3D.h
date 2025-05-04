#ifndef _PIPE3D_H_
#define _PIPE3D_H_

#include "common.h"

void PIPE3D_InvertTransform(MATRIX *target, MATRIX *source);
void PIPE3D_NormalizeMatrix(MATRIX *target, MATRIX *source);
void PIPE3D_AnimateTextures(AniTex *aniTextures, long req_frame);
void PIPE3D_InstanceTransformAndDraw(Instance *instance, CameraCore *cameraCore, VertexPool *vertexPool, PrimPool *primPool, unsigned long **ot, Mirror *mirror);
void PIPE3D_HalvePlaneInstanceTransformAndDraw(Instance *instance, MATRIX *wcTransform, VertexPool *vertexPool, PrimPool *primPool, unsigned long **ot, Mirror *mirror);
void PIPE3D_TransformFromZAxis(MATRIX *transform, SVector *normal);
long PIPE3D_TransformAnimatedSplitInstanceVertices(MVertex *vertexList, PVertex *poolVertex, Model *model, MATRIX *wcTransform, MATRIX *matrixPool, Mirror *mirror, MATRIX *lm, CVECTOR *vertexColor, CVECTOR *vertexSrcCol);
void PIPE3D_AnimateTerrainTextures(DrMoveAniTex *aniTextures, long req_frame, PrimPool *primPool, unsigned long **drawot);
void PIPE3D_InstanceListTransformAndDraw(StreamUnit *unit, GameTracker *gameTracker, unsigned long **ot, CameraCore *cameraCore);
void PIPE3D_CalculateWCTransform(CameraCore *cameraCore);
long PIPE3D_Segment2ScreenPt(Instance *instance, MATRIX *wcTransform, int segIndex, Position *pos);

#endif
