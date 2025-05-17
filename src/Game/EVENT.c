#include "common.h"
#include "Game/EVENT.h"
#include "Game/INSTANCE.h"
#include "Game/STREAM.h"
#include "Game/GAMELOOP.h"
#include "Game/LOCAL/LOCALSTR.h"
#include "Game/FONT.h"
#include "Game/MENU/MENU.h"
#include "Game/COLLIDE.h"
#include "Game/CINEMA/CINEPSX.h"
#include "Game/SAVEINFO.h"
#include "Game/SIGNAL.h"

STATIC long numActiveEventTimers;

STATIC EventTimer eventTimerArray[24];

STATIC WaterLevelProcess WaterLevelArray[5];

long WaterInUse;

long WaitingForVoiceNumber;

long WaitingToLoadSound;

long WaitingToLoadSFX;

short MoviePlayed;

short MovieToPlay;

HintSystemStruct gHintSystem;

long NumSignalsToReset;

struct SignalResetStruct ResetSignalArray[16];

STATIC struct Event *currentEventInstance;

STATIC short EventAbortLine;

STATIC struct Level *CurrentPuzzleLevel;

STATIC long EventCurrentEventIndex;

STATIC long CurrentEventLine;

STATIC short EventJustRecievedTimer;

STATIC ScriptPCode *currentActionScript;

STATIC short *EventAbortedPosition;

void EVENT_UpdateResetSignalArrayAndWaterMovement(struct Level *oldLevel, struct Level *newLevel, long sizeOfLevel)
{
    long offset;
    long i;
    struct WaterLevelProcess *curWater;

    offset = (intptr_t)newLevel - (intptr_t)oldLevel;

    if (NumSignalsToReset != 0)
    {
        for (i = 0; i < 16; i++)
        {
            if (ResetSignalArray[i].timeLeft > 0)
            {
                if (IN_BOUNDS(ResetSignalArray[i].mSignal, oldLevel, (intptr_t)oldLevel + sizeOfLevel))
                {
                    ResetSignalArray[i].mSignal = (struct _MultiSignal *)OFFSET_DATA(ResetSignalArray[i].mSignal, offset);
                }
            }
        }
    }

    if (WaterInUse != 0)
    {
        curWater = &WaterLevelArray[0];

        for (i = 5; i > 0; i--, curWater++)
        {
            if ((curWater->flags & 1))
            {
                if (IN_BOUNDS(curWater->bspTree, oldLevel, (intptr_t)oldLevel + sizeOfLevel))
                {
                    curWater->bspTree = (struct BSPTree *)OFFSET_DATA(curWater->bspTree, offset);
                }
            }
        }
    }
}

void EVENT_ResetAllOneTimeVariables()
{
    int i;

    if (NumSignalsToReset != 0)
    {
        for (i = 0; i < 16; i++)
        {
            if (ResetSignalArray[i].timeLeft > 0 && --ResetSignalArray[i].timeLeft == 0)
            {
                NumSignalsToReset--;

                ResetSignalArray[i].mSignal->flags &= ~0x1;
                ResetSignalArray[i].timeLeft = 0;
            }
        }
    }
}

void EVENT_AddSignalToReset(struct _MultiSignal *mSignal)
{
    int i;

    if (NumSignalsToReset < 16)
    {
        for (i = 0; i < 16; i++)
        {
            if (ResetSignalArray[i].timeLeft == 0)
            {
                ResetSignalArray[i].timeLeft = 1;

                ResetSignalArray[i].mSignal = mSignal;

                NumSignalsToReset++;

                return;
            }
        }
    }
}

void HINT_ResetHint()
{
    memset(&gHintSystem, 0, sizeof(HintSystemStruct));
}

void HINT_StartHint(short hintNumber)
{
    unsigned int h;  // changed type in regards to decls.h
    unsigned int temp;  // not from decls.h

    h = hintNumber + 55;

    gHintSystem.flags = 0x3;

    temp = gHintSystem.hintNumber = hintNumber;

    if (temp >= 54)
    {
        h = 108;
    }

    gHintSystem.stringNumber = h;

    gHintSystem.fadeTimer = 61440;

    gHintSystem.spawningUnitID = gameTrackerX.playerInstance->currentStreamUnitID;
}

void HINT_StopHint()
{
    if (!(gHintSystem.flags & 0x1))
    {
        return;
    }

    if (!(gHintSystem.flags & 0x2) && !(gHintSystem.flags & 0x4))
    {
        gHintSystem.flags |= 0x4;

        if (gHintSystem.flags & 0x2)
        {
            gHintSystem.flags &= ~0x2;
            gHintSystem.fadeTimer = (61440 - gHintSystem.fadeTimer);
        }
        else
        {
            gHintSystem.fadeTimer = 61440;
        }
    }
}

void HINT_KillSpecificHint(short hintNumber)
{
    if (gHintSystem.hintNumber == hintNumber)
    {
        HINT_StopHint();
    }
}

long HINT_GetCurrentHint()
{
    if ((gHintSystem.flags & 0x1))
    {
        return gHintSystem.hintNumber;
    }

    return -1;
}

void EVENT_ProcessTimers()
{
    int i;
    EventTimer *eventTimer;

    if (numActiveEventTimers != 0)
    {
        for (i = 0; i < 24; i++)
        {
            eventTimer = &eventTimerArray[i];

            if (eventTimer->flags & 1)
            {
                if ((unsigned long)eventTimer->time < gameTrackerX.timeMult)
                {
                    eventTimer->time = 0;
                }
                else
                {
                    eventTimer->time = (eventTimer->time - gameTrackerX.timeMult);
                }

                if (eventTimer->time <= 0)
                {
                    eventTimer->time = 0;
                    currentEventInstance = eventTimer->event;
                    eventTimer->actionScript->conditionBits &= ~0x1;
                    EVENT_RemoveTimer(eventTimer);
                    EventAbortLine = 0;
                    CurrentPuzzleLevel = eventTimer->level;
                    EventCurrentEventIndex = eventTimer->nextEventIndex;

                    if ((EVENT_DoAction(eventTimer->event, eventTimer->actionScript, eventTimer->scriptPos) != 0) && (EventCurrentEventIndex != -1))
                    {
                        EVENT_Process(eventTimer->event, EventCurrentEventIndex);
                    }
                }
            }
        }
    }
}

extern char D_800D1A08[];
extern char D_800D1A0C[];
void EVENT_ProcessHints()
{
    if ((gHintSystem.flags & 0x1))
    {
        char string[128];
        long y;

        // sprintf(string, "%s\n", localstr_get(gHintSystem.stringNumber));
        sprintf(string, D_800D1A08, localstr_get(gHintSystem.stringNumber));

        if ((gHintSystem.flags & 0x2))
        {
            y = ((gHintSystem.fadeTimer * 52) / 61440) + 200;
        }
        else
        {
            y = 200;

            if ((gHintSystem.flags & 0x4))
            {
                y = 252 - ((gHintSystem.fadeTimer * 52) / 61440);
            }
        }

        FONT_FontPrintCentered(string, y);
        // FONT_FontPrintCentered("$\n", y);
        FONT_FontPrintCentered(D_800D1A0C, y);

        DisplayHintBox(FONT_GetStringWidth(string), y);

        if (gHintSystem.fadeTimer != 0)
        {
            if ((unsigned long)gHintSystem.fadeTimer <= gameTrackerX.timeMult)
            {
                gHintSystem.fadeTimer = 0;

                if ((gHintSystem.flags & 0x2))
                {
                    gHintSystem.flags &= ~0x2;
                }
                else if ((gHintSystem.flags & 0x4))
                {
                    gHintSystem.flags = 0;

                    gHintSystem.hintNumber = -1;
                    gHintSystem.stringNumber = -1;
                }
            }
            else
            {
                gHintSystem.fadeTimer -= gameTrackerX.timeMult;
            }
        }

        if (gHintSystem.spawningUnitID != gameTrackerX.playerInstance->currentStreamUnitID)
        {
            HINT_StopHint();
        }

        if ((gameTrackerX.gameFlags & 0x10))
        {
            HINT_StopHint();
        }
    }
}

EventTimer *EVENT_GetNextTimer()
{
    int i;
    EventTimer *eventTimer;

    if (numActiveEventTimers < 24)
    {
        eventTimer = (EventTimer *)eventTimerArray;

        for (i = 24; i > 0; i--, eventTimer++)
        {
            if (!(eventTimer->flags & 0x1))
            {
                numActiveEventTimers++;

                eventTimer->flags |= 0x1;

                return eventTimer;
            }
        }
    }

    return NULL;
}

void EVENT_RemoveTimer(EventTimer *timer)
{
    if (timer != NULL)
    {
        numActiveEventTimers--;

        timer->flags &= ~0x1;
    }
}

void EVENT_InitTimers()
{
    numActiveEventTimers = 0;

    memset(eventTimerArray, 0, sizeof(EventTimer) * 24);
}

void EVENT_InitTerrainMovement()
{
    long i;

    for (i = 0; i < 5; i++)
    {
        memset(&WaterLevelArray[i], sizeof(WaterLevelProcess), 0); // last two function parameters are swapped, likely a mistake by the devs
    }

    WaterInUse = 0;
}

void EVENT_Init()
{
    EVENT_InitTerrainMovement();
    EVENT_InitTimers();

    HINT_ResetHint();

    WaitingForVoiceNumber = -1;

    WaitingToLoadSound = 614400;
    WaitingToLoadSFX = 614400;

    MoviePlayed = 0;

    MovieToPlay = -1;
}

WaterLevelProcess *EVENT_GetNextTerrainMove()
{
    int i;

    for (i = 0; i < 5; i++)
    {
        if (!(WaterLevelArray[i].flags & 0x1))
        {
            return &WaterLevelArray[i];
        }
    }

    return NULL;
}

void EVENT_ProcessMovingWater()
{
    int i;
    int inUse;
    int temp; // not from decls.h

    inUse = 0;

    if (WaterInUse != 0)
    {
        WaterLevelProcess *curWater;

        curWater = WaterLevelArray;

        for (i = 5; i > 0; i--, curWater++)
        {
            if ((curWater->flags & 0x1))
            {
                inUse = 1;

                curWater->curStep += gameTrackerX.timeMult;

                if (curWater->curStep > curWater->maxSteps)
                {
                    curWater->curStep = curWater->maxSteps;
                }

                temp = ((curWater->zValueTo - curWater->zValueFrom) * (curWater->curStep >> 12)) / (curWater->maxSteps >> 12);

                curWater->bspTree->globalOffset.z = curWater->oldGlobalOffset + temp;
                curWater->bspTree->localOffset.z = curWater->zValueFrom + temp;

                if ((curWater->flags & 0x2))
                {
                    curWater->streamUnit->level->waterZLevel = curWater->oldWaterZ + temp;

                    curWater->streamUnit->level->terrain->UnitChangeFlags |= 0x1;
                }

                if (curWater->curStep == curWater->maxSteps)
                {
                    curWater->flags = 0;
                }
            }
        }
    }

    if (inUse == 0)
    {
        WaterInUse = 0;
    }
}

void EVENT_DoProcess()
{
    EVENT_ProcessMovingWater();
    EVENT_ProcessTimers();
}

void EVENT_BSPProcess(StreamUnit *streamUnit)
{
    long curTree;
    Level *level;
    Terrain *terrain;

    level = streamUnit->level;

    terrain = level->terrain;

    for (curTree = 0; curTree < terrain->numBSPTrees; curTree++)
    {
        BSPTree *bspTree;

        bspTree = &terrain->BSPTreeArray[curTree];

        if (bspTree->ID >= 0)
        {
            Instance *instance;

            instance = bspTree->instanceSpline;

            if (instance != NULL)
            {
                short dx;
                short dy;
                short dz;

                if ((instance->matrix != NULL) && (instance->oldMatrix != NULL))
                {
                    dx = instance->matrix->t[0] - instance->oldMatrix->t[0];
                    dy = instance->matrix->t[1] - instance->oldMatrix->t[1];
                    dz = instance->matrix->t[2] - instance->oldMatrix->t[2];
                }
                else
                {
                    dx = instance->position.x - instance->oldPos.x;
                    dy = instance->position.y - instance->oldPos.y;
                    dz = instance->position.z - instance->oldPos.z;
                }

                if ((dx << 16) || (dy << 16) || (dz << 16))
                {
                    Instance *attachedInstance;
                    SVECTOR offset;

                    bspTree->globalOffset.x += dx;
                    bspTree->globalOffset.y += dy;
                    bspTree->globalOffset.z += dz;

                    bspTree->localOffset.x += dx;
                    bspTree->localOffset.y += dy;
                    bspTree->localOffset.z += dz;

                    terrain->UnitChangeFlags |= 0x2;

                    offset.vx = dx;
                    offset.vy = dy;
                    offset.vz = dz;

                    for (attachedInstance = gameTrackerX.instanceList->first; attachedInstance != NULL; attachedInstance = attachedInstance->next)
                    {
                        if ((instance->attachedID == 0) && (attachedInstance->tface != NULL) && (attachedInstance->currentStreamUnitID == streamUnit->StreamUnitID) && (attachedInstance->bspTree == curTree))
                        {
                            attachedInstance->position.x += dx;
                            attachedInstance->position.y += dy;
                            attachedInstance->position.z += dz;

                            COLLIDE_UpdateAllTransforms(attachedInstance, &offset);
                        }
                    }
                }
            }
        }
    }
}

void EVENT_Process(Event *eventInstance, long startIndex)
{
    long i;

    currentEventInstance = eventInstance;

    for (i = startIndex; i < eventInstance->numActions; i++)
    {
        EventCurrentEventIndex = i + 1;

        if (EventCurrentEventIndex == eventInstance->numActions)
        {
            EventCurrentEventIndex = -1;
        }

        if (eventInstance->conditionalList[i] != NULL)
        {
            CurrentEventLine = EventAbortLine = 0;

            if (EVENT_IsConditionTrue(eventInstance, eventInstance->conditionalList[i]) != 0)
            {
                while (eventInstance->actionList[i] == NULL)
                {
                    if (i >= eventInstance->numActions)
                    {
                        break;
                    }

                    i++;
                }

                if ((i < eventInstance->numActions) && (!(eventInstance->actionList[i]->conditionBits & 0x2)))
                {
                    EventAbortLine = 0;

                    EVENT_DoAction(eventInstance, eventInstance->actionList[i], eventInstance->actionList[i]->data);
                }
            }
        }
        else if (!(eventInstance->actionList[i]->conditionBits & 0x2))
        {
            EventAbortLine = 0;

            EVENT_DoAction(eventInstance, eventInstance->actionList[i], eventInstance->actionList[i]->data);
        }
    }
}

void EVENT_ProcessPuppetShow(Event *eventInstance, long startIndex)
{
    long i;
    long conditionIsStatisfied;

    i = startIndex;

    EventJustRecievedTimer = 0;

    currentEventInstance = eventInstance;

    conditionIsStatisfied = 1;

    if (!(eventInstance->actionList[startIndex]->conditionBits & 0x2))
    {
        EventCurrentEventIndex = startIndex + 1;

        if (EventCurrentEventIndex == eventInstance->numActions)
        {
            EventCurrentEventIndex = -1;
        }

        if (eventInstance->conditionalList[startIndex] == NULL)
        {
            if (!(eventInstance->actionList[startIndex]->conditionBits & 0x2))
            {
                EventAbortLine = 0;

                EVENT_DoAction(eventInstance, eventInstance->actionList[startIndex], eventInstance->actionList[i]->data);
            }
        }
        else
        {
            CurrentEventLine = EventAbortLine = 0;

            if (EVENT_IsConditionTrue(eventInstance, eventInstance->conditionalList[i]) != 0)
            {
                while ((eventInstance->actionList[i] == NULL) && (i < eventInstance->numActions))
                {
                    i++;
                }

                if (i < eventInstance->numActions)
                {
                    EventAbortLine = 0;

                    EVENT_DoAction(eventInstance, eventInstance->actionList[i], eventInstance->actionList[i]->data);
                }
            }
            else
            {
                conditionIsStatisfied = 0;
            }
        }

        if (((EventAbortLine == 0) || (EventJustRecievedTimer == 1)) && (conditionIsStatisfied == 1))
        {
            eventInstance->processingPuppetShow = i + 2;

            if (eventInstance->processingPuppetShow >= (eventInstance->numActions + 1))
            {
                eventInstance->processingPuppetShow = 0;
            }
        }
    }
}

void EVENT_ProcessEvents(EventPointers *eventPointers, Level *level)
{
    long i;

    CurrentPuzzleLevel = level;

    for (i = 0; i < eventPointers->numPuzzles; i++)
    {
        if (eventPointers->eventInstances[i]->eventNumber >= 0)
        {
            EVENT_Process(eventPointers->eventInstances[i], 0);
        }
        else if (eventPointers->eventInstances[i]->processingPuppetShow != 0)
        {
            EVENT_ProcessPuppetShow(eventPointers->eventInstances[i], eventPointers->eventInstances[i]->processingPuppetShow - 1);
        }
    }

    if (MovieToPlay != -1)
    {
        CINE_PlayIngame(MovieToPlay);

        MovieToPlay = -1;
        MoviePlayed = 1;
    }
}

long EVENT_DoAction(Event *eventInstance, ScriptPCode *actionScript, short *scriptData)
{
    long retValue;
    long operateOnStack;
    PCodeStack stack;

    stack.topOfStack = 0;

    retValue = 1;

    currentActionScript = actionScript;

    EventJustRecievedTimer = 0;

    EventAbortedPosition = scriptData;

    if ((!(actionScript->conditionBits & 0x1)) && (scriptData != NULL))
    {
        while (EventAbortLine == 0)
        {
            scriptData = EVENT_ParseOpcode(&stack, scriptData, &operateOnStack);

            if ((EventAbortLine != 0) && (EventJustRecievedTimer == 0))
            {
                EventTimer *timer;

                timer = EVENT_GetNextTimer();

                if (timer != NULL)
                {
                    retValue = 0;

                    timer->event = eventInstance;

                    timer->actionScript = actionScript;

                    timer->time = 0;

                    timer->scriptPos = EventAbortedPosition;

                    actionScript->conditionBits |= 0x1;

                    timer->level = CurrentPuzzleLevel;

                    timer->nextEventIndex = EventCurrentEventIndex;
                }
            }

            if ((operateOnStack != 0) && (EventAbortLine == 0) && (stack.topOfStack > 0))
            {
                stack.topOfStack--;

                EVENT_ExecuteActionCommand(&stack.stack[stack.topOfStack], NULL, &stack, scriptData);
            }

            if (scriptData == NULL)
            {
                break;
            }
        }
    }

    return retValue;
}

long EVENT_IsConditionTrue(Event *eventInstance, ScriptPCode *conditionScript)
{
    long retValue;
    long error;
    long operateOnStack;
    short flags;
    PCodeStack stack;
    short *scriptData;

    (void)eventInstance;

    stack.topOfStack = 0;

    scriptData = conditionScript->data;

    currentActionScript = NULL;

    retValue = 0;

    while ((scriptData != NULL) && (EventAbortLine == 0))
    {
        scriptData = EVENT_ParseOpcode(&stack, scriptData, &operateOnStack);

        if ((operateOnStack != 0) && (EventAbortLine == 0))
        {
            CurrentEventLine++;

            if (stack.topOfStack > 0)
            {
                retValue = EVENT_GetScalerValueFromOperand(&stack.stack[--stack.topOfStack], &error, &flags);

                if (error != 0)
                {
                    retValue = 0;
                    break;
                }
                else if (retValue == 0)
                {
                    break;
                }
            }
        }
    }

    if (EventAbortLine == 1)
    {
        retValue = 0;
    }

    return retValue;
}

long EVENT_WriteEventObject(StackType *stackEntry, long areaID, Event *event, long number)
{
    long retValue;

    retValue = 0;

    if (event != NULL)
    {
        stackEntry->id = 16;

        stackEntry->data.eventObject.event = event;

        stackEntry->data.eventObject.attribute = -1;
    }
    else
    {
        SavedBasic *savedEvent;

        savedEvent = SAVE_GetSavedEvent(areaID, number);

        if (savedEvent != NULL)
        {
            stackEntry->id = 21;

            stackEntry->data.savedEventObject.savedEvent = savedEvent;

            stackEntry->data.savedEventObject.areaID = areaID;

            stackEntry->data.savedEventObject.eventNumber = number;

            stackEntry->data.savedEventObject.attribute = -1;

            retValue = 1;
        }
        else
        {
            stackEntry->id = 21;

            stackEntry->data.savedEventObject.savedEvent = EVENT_CreateSaveEvent(areaID, number);

            stackEntry->data.savedEventObject.areaID = areaID;

            stackEntry->data.savedEventObject.eventNumber = number;

            stackEntry->data.savedEventObject.attribute = -1;

            retValue = 1;
        }
    }

    return retValue;
}

MultiSignal *EVENT_ResolveObjectSignal(StreamUnit *stream, long signalNumber)
{
    MultiSignal *retValue;
    Level *level;

    level = stream->level;

    retValue = NULL;

    if (signalNumber < 0)
    {
        switch (signalNumber)
        {
        case -1:
            retValue = level->startSignal;
            break;
        case -2:
            retValue = level->spectralSignal;
            break;
        case -3:
            retValue = level->materialSignal;
            break;
        case -4:
            retValue = level->startUnitLoadedSignal;
            break;
        case -5:
            retValue = level->startUnitMainSignal;
            break;
        }
    }
    else
    {
        retValue = SIGNAL_FindSignal(level, signalNumber);
    }

    return retValue;
}

Intro *EVENT_ResolveObjectIntro(EventInstanceObject *instanceObject)
{
    return INSTANCE_FindIntro(instanceObject->unitID, instanceObject->introUniqueID);
}

SFXMkr *EVENT_ResolveSFXMarker(StreamUnit *stream, EventInstanceObject *instanceObject)
{
    int i;
    Level *level;
    SFXMkr *result;

    level = stream->level;

    result = NULL;

    if (level->NumberOfSFXMarkers != 0)
    {
        for (i = 0; i < level->NumberOfSFXMarkers; i++)
        {
            if (instanceObject->introUniqueID == level->SFXMarkerList[i].uniqueID)
            {
                result = &level->SFXMarkerList[i];
                break;
            }
        }
    }

    return result;
}

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_AddGameObjectToStack);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_AddPlayerObjectToStack);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_AddObjectToStack);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_AddCharPointerToStack);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_AddShortPointerToStack);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_AddNumberToStack);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_ChangeOperandToNumber);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_ChangeOperandVector3d);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_Addvector3dToStack);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_ChangeOperandRotation3d);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_AddSubListObjectToStack);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_StackDuplicate);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_TransformTGroupAttribute);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_TransformConstrictAttribute);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_TransformInstanceAttribute);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_TransformSoundObjectAttribute);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_GetGameValue);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_TransformGameAttribute);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_TransformAreaAttribute);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_TransformEventAttribute);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_TransformSavedEventAttribute);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_TransformSubListObjectAttribute);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_TransformListObjectAttribute);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_TransformCameraObjectAttribute);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_TransformSplineAttribute);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_TransformIntroAttribute);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_ParseOperand2);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_DoVMObjectAction);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_GetVMObjectValue);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_DoGameAction);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_DoSignalAction);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_TransformSignalAttribute);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_TransformRotation3dAttribute);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_TransformVector3dAttribute);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_ModifyObjectToStackWithAttribute);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_DoInstanceAnimateTextureAction);

void EVENT_ResetAllSplineFlags(MultiSpline *spline)
{
    if (spline->positional != NULL)
    {
        spline->positional->flags &= ~0x7;
    }

    if (spline->rotational != NULL)
    {
        spline->rotational->flags &= ~0x7;
    }

    if (spline->scaling != NULL)
    {
        spline->scaling->flags &= ~0x7;
    }

    if (spline->color != NULL)
    {
        spline->color->flags &= ~0x7;
    }
}

void EVENT_SetSplineLoop(MultiSpline *spline)
{
    if (spline->positional != NULL)
    {
        spline->positional->flags |= 0x2;
    }

    if (spline->rotational != NULL)
    {
        spline->rotational->flags |= 0x2;
    }

    if (spline->scaling != NULL)
    {
        spline->scaling->flags |= 0x2;
    }

    if (spline->color != NULL)
    {
        spline->color->flags |= 0x2;
    }
}

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_DoSplineAction);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_DoAnimateAction);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_DoInstanceAction);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_GetTGroupValue);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_DoTGroupAction);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_DoCameraAction);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_DoObjectSoundAction);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_DoSoundMarkerAction);

long EVENT_GetSoundValue(SoundObject *soundObject)
{
    (void)soundObject;

    return 0;
}

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_DoAreaAction);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_DoIntroAction);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_DoStackOperationEquals);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_CompareVector3d);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_CompareRotationVectors);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_CompareListWithOperation);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_CompareSubListWithOperation);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_CompareOperandsWithOperation);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_DoStackMathOperation);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_ParseOpcode);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_GetIntroValue);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_DoSubListAction);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_GetInstanceValue);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_GetSplineFrameNumber);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_GetSplineValue);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_GetAnimateTextureValue);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_GetAnimateValue);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_TransformObjectOnStack);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_UpdatePuzzlePointers);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_SaveEventsFromLevel);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_LoadEventsForLevel);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_CreateSaveEvent);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_RemoveInstanceFromInstanceList);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_UpdatePuzzleWithInstance);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_AddInstanceToInstanceList);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_FindVMObject);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_FixPuzzlesForStream);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_AddStreamToInstanceList);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_RemoveStreamToInstanceList);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_RelocateInstanceList);

void EVENT_PrintVars()
{
}

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_ExecuteActionCommand);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_GetScalerValueFromOperand);

long EVENT_TransformOperand(struct StackType *stackObject, struct _PCodeStack *stack, long item, short *codeStream)
{
    long retValue;

    switch (stackObject->id)
    {
    case 2:
        retValue = EVENT_TransformInstanceAttribute(stack, stackObject, item, codeStream);
        break;
    case 24:
        retValue = EVENT_TransformConstrictAttribute(stack, stackObject, item);
        break;
    case 15:
        retValue = EVENT_TransformSplineAttribute(stack, stackObject, item, codeStream);
        break;
    case 4:
        retValue = EVENT_TransformIntroAttribute(stack, stackObject, item);
        break;
    case 9:
        retValue = EVENT_TransformVector3dAttribute(stack, stackObject, item, codeStream);
        break;
    case 14:
        retValue = EVENT_TransformRotation3dAttribute(stack, stackObject, item, codeStream);
        break;
    case 16:
        retValue = EVENT_TransformEventAttribute(stack, stackObject, item, codeStream);
        break;
    case 17:
        retValue = EVENT_TransformSignalAttribute(stack, stackObject, item);
        break;
    case 1:
        retValue = EVENT_TransformAreaAttribute(stack, stackObject, item, codeStream);
        break;
    case 6:
        EventAbortLine = 1;
        retValue = 1;
        break;
    case 3:
        retValue = EVENT_TransformGameAttribute(stack, stackObject, item, codeStream);
        break;
    case 18:
        retValue = EVENT_TransformListObjectAttribute(stack, stackObject, item);
        break;
    case 19:
        if (stackObject->data.subListObject.numberOfInstances == -1)
        {
            stackObject->data.subListObject.numberOfInstances = item;
            retValue = 1;
            break;
        }
        retValue = 0;
        break;
    case 27:
        retValue = EVENT_TransformSoundObjectAttribute(stack, &stackObject->data.soundObject, item, codeStream);
        break;
    case 20:
        if (stackObject->data.subListObject.numberOfAttributes == -1)
        {
            stackObject->data.subListObject.numberOfAttributes = item;
            retValue = 1;
            break;
        }
        retValue = 0;
        break;
    case 21:
        retValue = EVENT_TransformSavedEventAttribute(stack, stackObject, item, codeStream);
        break;
    case 22:
        retValue = EVENT_TransformSubListObjectAttribute(stack, stackObject, item);
        break;
    case 23:
        retValue = EVENT_TransformTGroupAttribute(stack, stackObject, item, codeStream);
        break;
    case 25:
        retValue = EVENT_TransformCameraObjectAttribute(stack, stackObject, item, codeStream);
        break;
    case 26:
        stackObject->data.subListObject.numberOfAttributes = item;
        retValue = 1;
        break;
    case 28:
    default:
        retValue = 0;
        break;
    }
    return retValue;
}
