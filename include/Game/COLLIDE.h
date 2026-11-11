#ifndef _COLLIDE_H_
#define _COLLIDE_H_

#include "common.h"

TFace *COLLIDE_PointAndTerrainFunc(Terrain *terrain, PCollideInfo *pCollideInfo, int Flags, short *Backface_Flag, long ignoreAttr, long acceptAttr, LCollideInfo *lcolinfo);
int COLLIDE_PointInTriangle(SVector *v0, SVector *v1, SVector *v2, SVector *point, SVector *normal);
int COLLIDE_PointInTriangle2DPub(short *v0, short *v1, short *v2, short *point);
void COLLIDE_SetBSPTreeFlag(CollideInfo *collideInfo, short flag);
void COLLIDE_SegmentCollisionOff(Instance *instance, int segment);
void COLLIDE_SegmentCollisionOn(Instance *instance, int segment);
void COLLIDE_UpdateAllTransforms(Instance *instance, SVECTOR *offset);
void COLLIDE_PointAndWorld(PCollideInfo *pcollideInfo, Level *level);
long COLLIDE_GetNormal(short nNum, short *nrmlArray, SVector *nrml);
void COLLIDE_Instance1SpheresToInstance2(Instance *instance1, Instance *instance2, long sphereToSphere);
void COLLIDE_MoveAllTransforms(Instance *instance, Position *offset);
long COLLIDE_FindCollisionFaceNormal(CollideInfo *collideInfo, Normal *normal);
int COLLIDE_PointAndHFace(SVector *newPos, SVector *oldPos, HFace *hface, Model *model, SVector *hfNormal);
int COLLIDE_PointAndTfaceFunc(Terrain *terrain, BSPTree *bsp, SVector *orgNewPos, SVector *orgOldPos, TFace *tface, long ignoreAttr, long flags);
long COLLIDE_SAndT(SCollideInfo *scollideInfo, Level *level);
long COLLIDE_LineWithSignals(SVector *startPoint, SVector *endPoint, MultiSignal **signalList, long maxSignals, Level *level);
void COLLIDE_MakeNormal(Terrain *terrain, TFace *tface, SVector *normal);
long COLLIDE_SphereAndHFace(Sphere *sphere, Position *oldPos, HFaceInfo *hfaceInfo, SVector *intersect, long *edge);
void COLLIDE_PointAndInstanceTrivialReject(PCollideInfo *pcollideInfo, Instance *instance);
void COLLIDE_LineWithBoxFace(short startDist, long lineDist, short planeDist, SVector *start, Vector *line, HBox *hbox, long (*collideBoundFunc)(), SVector *normal);
StreamUnit *COLLIDE_CameraWithStreamSignals(Camera *camera);
void COLLIDE_InstanceList(InstanceList *instanceList);
void COLLIDE_InstanceListTerrain(InstanceList *instanceList);
void COLLIDE_InstanceListWithSignals(InstanceList *instanceList);

#endif
