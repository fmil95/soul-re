#ifndef _EVENT_H_
#define _EVENT_H_

#include "common.h"

// Further debugging symbols were provided by the remasters' PDB file for the following structs
typedef struct EventVMO
{
    short id;
    short spad;
    long unitID;
    char vmObjectName[12];
    struct _VMObject *vmObjectPtr;
} EventVMO;

typedef struct EventSignalObject
{
    short id;
    short signalNumber;
    long unitID;
    void *signal;
} EventSignalObject;

typedef struct EventAreaObject
{
    short id;
    short spad;
    long unitID;
    struct _StreamUnit *stream;
} EventAreaObject;

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
void EVENT_RemoveTimer(EventTimer *timer);
long EVENT_DoAction(Event *eventInstance, ScriptPCode *actionScript, short *scriptData);
void EVENT_Process(Event *eventInstance, long startIndex);
long EVENT_TransformInstanceAttribute(PCodeStack *stack, StackType *stackObject, long item, short *codeStream);
long EVENT_TransformConstrictAttribute(PCodeStack *stack, StackType *stackObject, long item);
long EVENT_TransformSplineAttribute(PCodeStack *stack, StackType *stackObject, long item, short *codeStream);
long EVENT_TransformIntroAttribute(PCodeStack *stack, StackType *stackObject, long item);
long EVENT_TransformVector3dAttribute(PCodeStack *stack, StackType *stackObject, long item, short *codeStream);
long EVENT_TransformRotation3dAttribute(PCodeStack *stack, StackType *stackObject, long item, short *codeStream);
long EVENT_TransformEventAttribute(PCodeStack *stack, StackType *stackObject, long item, short *codeStream);
long EVENT_TransformSignalAttribute(PCodeStack *stack, StackType *stackObject, long item);
long EVENT_TransformAreaAttribute(PCodeStack *stack, StackType *stackObject, long item, short *codeStream);
long EVENT_TransformGameAttribute(PCodeStack *stack, StackType *stackObject, long item, short *codeStream);
long EVENT_TransformListObjectAttribute(PCodeStack *stack, StackType *stackObject, long item);
long EVENT_TransformSoundObjectAttribute(PCodeStack *stack, SoundObject *soundObject, long item, short *codeStream);
long EVENT_TransformSavedEventAttribute(PCodeStack *stack, StackType *stackObject, long item, short *codeStream);
long EVENT_TransformSubListObjectAttribute(PCodeStack *stack, StackType *stackObject, long item);
long EVENT_TransformTGroupAttribute(PCodeStack *stack, StackType *stackObject, long item, short *codeStream);
long EVENT_TransformCameraObjectAttribute(PCodeStack *stack, StackType *stackObject, long item, short *codeStream);
long EVENT_IsConditionTrue(Event *eventInstance, ScriptPCode *conditionScript);
short *EVENT_ParseOpcode(PCodeStack *stack, short *codeStream, long *operateOnStack);
void EVENT_ExecuteActionCommand(StackType *operand1, StackType *operand2, PCodeStack *stack, short *codeStream);
long EVENT_GetScalerValueFromOperand(StackType *operand, long *error, short *flags);
SavedBasic *EVENT_CreateSaveEvent(long levelID, long eventNumber);
long EVENT_TransformObjectOnStack(PCodeStack *stack, long item, short *codeStream);
long EVENT_GetSplineFrameNumber(InstanceSpline *instanceSpline);

#endif
