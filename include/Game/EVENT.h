#ifndef _EVENT_H_
#define _EVENT_H_

#include "common.h"

void EVENT_Init();
void EVENT_AddInstanceToInstanceList(Instance *instance);
void EVENT_RemoveInstanceFromInstanceList(Instance *instance);
void EVENT_SaveEventsFromLevel(long levelID, Level *level);
void EVENT_RemoveStreamToInstanceList(StreamUnit *stream);
void EVENT_UpdateResetSignalArrayAndWaterMovement(Level *oldLevel, Level *newLevel, long sizeOfLevel);
void EVENT_LoadEventsForLevel(long levelID, Level *level);
void EVENT_AddSignalToReset(MultiSignal *mSignal);
void EVENT_AddStreamToInstanceList(StreamUnit *stream);
void EVENT_PrintVars();
void EVENT_ProcessMovingWater();
void EVENT_ProcessTimers();
void HINT_ResetHint();
void HINT_StopHint();
void EVENT_DoProcess();
void EVENT_ProcessEvents(EventPointers *eventPointers, Level *level);
void EVENT_BSPProcess(StreamUnit *streamUnit);
void EVENT_ResetAllOneTimeVariables();
void EVENT_ProcessHints();
long HINT_GetCurrentHint();
void HINT_StartHint(short hintNumber);
void HINT_KillSpecificHint(short hintNumber);
void EVENT_RelocateInstanceList(Level *oldLevel, Level *newLevel, long sizeOfLevel);
void EVENT_UpdatePuzzlePointers(EventPointers *events, long offset);

#endif
