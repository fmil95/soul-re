#ifndef _SAVEINFO_H_
#define _SAVEINFO_H_

#include "common.h"

void SAVE_DebugSaveGame();
void SAVE_LoadSaveGame();
void SAVE_LoadSaveGame();
void SAVE_DeleteInstance(Instance *instance);
void SAVE_MarkDeadDead(Instance *instance);
long SAVE_IsIntroDeadDead(Intro *intro);
long SAVE_HasSavedIntro(Intro *intro, long currentStreamID);
void SAVE_DoInstanceDeadDead(Instance *instance);
SavedIntroSmall *SAVE_GetSavedSmallIntro(Instance *instance);
SavedIntroSpline *SAVE_GetIntroSpline(Instance *instance);
void SAVE_Instance(Instance *instance, Level *level);
SavedLevel *SAVE_CreatedSavedLevel(long areaID, Level *level);
void SAVE_IntroForStreamID(StreamUnit *streamUnit);
void SAVE_UpdateLevelWithSave(StreamUnit *streamUnit);
void SAVE_Init(GameTracker *gt);
void SAVE_SaveGame();
long SAVE_SizeOfFreeSpace();
void SAVE_RestoreGame();
void SAVE_ClearMemory(GameTracker *gameTracker);
void *SAVE_GetSavedBlock(long saveType, long extraSize);
void SAVE_RestoreGlobalSavePointer();
void SAVE_RestoreGlobalSaveTracker();
void SAVE_SaveEverythingInMemory();
void SAVE_UpdateGlobalSaveTracker();
void SAVE_SetDeadDeadBit(int uniqueID, long set);
long SAVE_IsUniqueIDDeadDead(long uniqueID);
void SAVE_DeleteBlock(SavedBasic *savedBlock);
long SAVE_SaveableInstance(Instance *instance);
SavedIntro *SAVE_UpdateSavedIntro(Instance *instance, Level *level, SavedIntro *savedIntro, evControlSaveDataData *extraData);
SavedIntroWithIntro *SAVE_UpdateSavedIntroWithIntro(Instance *instance, Level *level, SavedIntroWithIntro *savedIntro, evControlSaveDataData *extraData);
SavedLevel *SAVE_HasSavedLevel(long areaID);
SavedBasic *SAVE_GetSavedEvent(long areaID, long eventNumber);
long SAVE_PurgeAMemoryBlock();
void SAVE_IntroduceBufferIntros();
void SAVE_UndestroyInstance(Instance *instance);
void SAVE_DeleteSavedEvent(long areaID, long eventNumber);

extern GlobalSaveTracker *GlobalSave;
extern char monVersion[];

#endif
