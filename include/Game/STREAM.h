#ifndef _STREAM_H_
#define _STREAM_H_

#include "common.h"

#define OFFSET_DATA(x, offset) (((x) == 0) ? 0 : ((uintptr_t)(x) + offset))
#define IN_BOUNDS(x, level, max) (((uintptr_t)(level) <= (uintptr_t)(x)) && ((uintptr_t)(x) <= (uintptr_t)(max)))

STracker StreamTracker;
WarpRoom WarpRoomArray[14];
extern long CurrentWarpNumber;
extern short RENDER_currentStreamUnitID;

Level *STREAM_GetLevelWithID(long id);
void STREAM_RelocateInstance(Instance *instance, SVector *offset);
void MORPH_SetupInstanceFlags(Instance *instance);
void STREAM_SetInstancePosition(Instance *instance, evPositionData *data);
StreamUnit *STREAM_GetStreamUnitWithID(long id);
ObjectTracker *STREAM_GetObjectTracker(char *name);
StreamUnit *FindStreamUnitFromLevel(Level *level);
int STREAM_IsSpecialMonster(char *name);
int STREAM_TryAndDumpNonResident(ObjectTracker *otr);
void STREAM_DumpObject(ObjectTracker *objectTracker);
void STREAM_FinishLoad(StreamUnit *streamUnit);
void STREAM_DumpUnit(StreamUnit *streamUnit, long doSave);
long WARPGATE_IsUnitWarpRoom(StreamUnit *streamUnit);
void WARPGATE_RemoveFromArray(StreamUnit *streamUnit);
void STREAM_ConnectStream(StreamUnit *streamUnit);
void STREAM_DumpAllLevels(long IDNoRemove, int DoSave);
void STREAM_PackVRAMObject(ObjectTracker *objectTracker);
int STREAM_IsMonster(char *name);
void WARPGATE_UpdateAddToArray(StreamUnit *streamUnit);
void MORPH_UpdateNormals(Level *BaseLevel);
void WARPGATE_FixUnit(StreamUnit *streamUnit);
void STREAM_AdjustMultiSpline(MultiSpline *multi, SVector *offset);
StreamUnit *STREAM_LoadLevel(char *baseAreaName, StreamUnitPortal *streamPortal, int loadnext);
void PreloadAllConnectedUnits(StreamUnit *streamUnit, SVector *offset);
void STREAM_OffsetInstancePosition(Instance *instance, SVector *offset, int streamSignalFlag);
long WARPGATE_GetWarpRoomIndex(char *name);
void MORPH_BringBackNormals(Level *BaseLevel);
void WARPGATE_DrawWarpGateRim(StreamUnit *streamUnit, long drawOn);
void WARPGATE_HideAllCloudCovers();
Instance *WARPGATE_UnHideCloudCoverInUnit(long streamUnitID);
void WARPGATE_RenderWarpUnit(unsigned long **mainOT, StreamUnitPortal *curStreamPortal, StreamUnit *mainStreamUnit, RECT *cliprect);
void WARPGATE_BlockWarpGateEntrance(StreamUnit *streamUnit, long collideOn);
long GetFogColor(StreamUnitPortal *portal, StreamUnit *mainStreamUnit, Level *mainLevel);
void DrawFogRectangle(RECT *cliprect, PrimPool *primPool, int otzpos, unsigned long **drawot, long color);
long StreamRenderLevel(StreamUnit *currentUnit, Level *mainLevel, unsigned long **drawot, long portalFogColor);
int AddClippedTri(SVECTOR *iv, RECT *cliprect, int *minz);
int WARPGATE_IsObjectOnWarpSide(Instance *instance);
int STREAM_IsMorphInProgress();
long STREAM_GetWaterZLevel(Level *level, Instance *instance);
void STREAM_LoadMainVram(GameTracker *gameTracker, char *baseAreaName, StreamUnit *streamUnit);
int InsertGlobalObject(char *name, GameTracker *gameTracker);
void WARPGATE_Init();
StreamUnit *STREAM_WhichUnitPointerIsIn(void *pointer);
void STREAM_AbortAreaLoad(char *baseAreaName);
void STREAM_NextLoadFromHead();
void STREAM_DumpLoadingObjects();
void STREAM_MORPH_Relocate();
void MORPH_UpdateTimeMult();
void MORPH_Continue();
void STREAM_MoveIntoNewStreamUnit();
int STREAM_GetClipRect(StreamUnitPortal *portal, RECT *rect);
void STREAM_RenderWarpGate(unsigned long **mainOT, StreamUnitPortal *curStreamPortal, StreamUnit *mainStreamUnit, RECT *cliprect);
void WARPGATE_IsItActive(StreamUnit *streamUnit);
void STREAM_RenderAdjacantUnit(unsigned long **curOT, StreamUnitPortal *curStreamPortal, StreamUnit *toStreamUnit, StreamUnit *mainStreamUnit, RECT *cliprect);
void STREAM_UpdateInstanceCollisionInfo(HModel *oldHModel, HModel *newHModel);
void RemoveIntroducedLights(Level *level);
void STREAM_UpdateObjectPointer(Object *oldObject, Object *newObject, long sizeOfObject);
int STREAM_TryAndDumpANonResidentObject();
void STREAM_DumpNonResidentObjects();
void WARPGATE_StartUsingWarpgate();
void WARPGATE_EndUsingWarpgate();
long WARPGATE_IncrementIndex();
long WARPGATE_DecrementIndex();
int WARPGATE_IsWarpgateUsable();
int WARPGATE_IsWarpgateSpectral();
int WARPGATE_IsWarpgateReady();
void MORPH_ToggleMorph();
void STREAM_UpdateLevelPointer(Level *oldLevel, Level *newLevel, long sizeOfLevel);
void STREAM_InitLoader(char *bigFileName, char *voiceFileName);
void STREAM_Init();
int STREAM_PollLoadQueue();
void RemoveAllObjects(GameTracker *gameTracker);

#endif
