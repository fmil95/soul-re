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
#include "Game/SOUND.h"
#include "Game/PSX/MAIN.h"
#include "Game/CAMERA.h"
#include "Game/LOAD3D.h"
#include "Game/GAMEPAD.h"
#include "Game/SCRIPT.h"
#include "Game/HASM.h"
#include "Game/VM.h"
#include "Game/FX.h"
#include "Game/VOICEXA.h"
#include "Game/STATE.h"
#include "Game/MATH3D.h"
#include "Game/PHYSOBS.h"
#include "Game/PSX/SUPPORT.h"

static short EventAbortLine = 0;

static short EventJustRecievedTimer = 0;

static long MoveCodeStreamExtra = 0;

static long CurrentEventLine = 0;

static long EventCurrentEventIndex = -1;

long WaterInUse = 0;

long NumSignalsToReset = 0;

STATIC long numActiveEventTimers;

STATIC EventTimer eventTimerArray[24];

STATIC WaterLevelProcess WaterLevelArray[5];

long WaitingForVoiceNumber;

long WaitingToLoadSound;

long WaitingToLoadSFX;

short MoviePlayed;

short MovieToPlay;

HintSystemStruct gHintSystem;

struct SignalResetStruct ResetSignalArray[16];

STATIC struct Event *currentEventInstance;

STATIC struct Level *CurrentPuzzleLevel;

STATIC ScriptPCode *currentActionScript;

STATIC short *EventAbortedPosition;

long eventListNumInstances[20];

Instance *eventListArray[20][10]; // order of indexes from decls.h has been reversed

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

void EVENT_ProcessHints()
{
    if ((gHintSystem.flags & 0x1))
    {
        char string[128];
        long y;

        sprintf(string, "%s\n", localstr_get(gHintSystem.stringNumber));

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
        FONT_FontPrintCentered("$\n", y);

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

void EVENT_AddGameObjectToStack(PCodeStack *stack)
{
    if (stack->topOfStack < 32)
    {
        StackType *stackEntry;

        stackEntry = &stack->stack[stack->topOfStack];

        stackEntry->id = 3;

        stackEntry->data.gameObject.attribute = -1;

        stack->topOfStack++;
    }
}

void EVENT_AddPlayerObjectToStack(PCodeStack *stack)
{
    if (stack->topOfStack < 32)
    {
        StackType *stackEntry;

        stackEntry = &stack->stack[stack->topOfStack];

        stackEntry->id = 2;

        stackEntry->data.instanceObject.instance = gameTrackerX.playerInstance;

        stackEntry->data.instanceObject.attribute = -1;

        stack->topOfStack++;
    }
}

void EVENT_AddObjectToStack(PCodeStack *stack, long item)
{
    EventBasicObject *basicObject;
    int topOfStack;

    topOfStack = stack->topOfStack;

    if (topOfStack < 32)
    {
        StackType *stackEntry;

        stackEntry = &stack->stack[topOfStack];

        basicObject = currentEventInstance->instanceList[item];

        switch ((short)(basicObject->id - 1))
        {
        case 5:
            if (((EventSignalObject *)basicObject)->signal != NULL)
            {
                stackEntry->id = 17;

                stackEntry->data.signalObject.msignal = ((EventSignalObject *)basicObject)->signal;

                stackEntry->data.signalObject.attribute = -1;
            }
            else
            {
                stackEntry->id = 6;

                stackEntry->data.Object.instanceNumber = item;

                stackEntry->data.Object.attribute = -1;
            }

            break;
        case 3:
            if (((EventTGroupObject *)basicObject)->bspTree != NULL)
            {
                stackEntry->id = 23;

                stackEntry->data.terrainGroup.bspTree = ((EventTGroupObject *)basicObject)->bspTree;

                stackEntry->data.terrainGroup.streamUnit = ((EventTGroupObject *)basicObject)->stream;

                stackEntry->data.terrainGroup.attribute = -1;
            }
            else
            {
                stackEntry->id = 6;

                stackEntry->data.Object.instanceNumber = item;

                stackEntry->data.Object.attribute = -1;
            }

            break;
        case 1:
            stackEntry->id = 18;

            stackEntry->data.listObject.eventInstance = (EventWildCardObject *)basicObject;

            stackEntry->data.listObject.numberOfAttributes = 0;

            stackEntry->data.listObject.lineID = CurrentEventLine;
            break;
        case 2:
            EVENT_WriteEventObject(stackEntry, ((EventEventObject *)basicObject)->unitID, ((EventEventObject *)basicObject)->event, ((EventEventObject *)basicObject)->eventNumber);
            break;
        case 4:
            stackEntry->id = 1;

            stackEntry->data.areaObject.streamUnit = (StreamUnit *)((EventAreaObject *)basicObject)->stream;

            stackEntry->data.areaObject.attribute = -1;

            stackEntry->data.areaObject.unitID = ((EventAreaObject *)basicObject)->unitID;
            break;
        case 0:
            if ((((EventInstanceObject *)basicObject)->flags & 0x1))
            {
                stackEntry->id = 27;

                stackEntry->data.soundObject.flags = 0x1;

                stackEntry->data.soundObject.data.sfxMarker = ((EventInstanceObject *)basicObject)->data.sfxMarker;

                stackEntry->data.soundObject.attribute = -1;

                stackEntry->data.soundObject.soundNumber = -1;

                stackEntry->data.soundObject.value = -1;

                stackEntry->data.soundObject.duration = -1;
            }
            else
            {
                if (((EventInstanceObject *)basicObject)->instance != NULL)
                {
                    stackEntry->id = 2;

                    stackEntry->data.instanceObject.instance = ((EventInstanceObject *)basicObject)->instance;

                    stackEntry->data.instanceObject.attribute = -1;
                    break;
                }

                if (((EventInstanceObject *)basicObject)->data.intro != NULL)
                {
                    stackEntry->id = 4;

                    stackEntry->data.introObject.intro = ((EventInstanceObject *)basicObject)->data.intro;

                    stackEntry->data.introObject.attribute = -1;
                    break;
                }

                stackEntry->id = 6;

                stackEntry->data.Object.instanceNumber = item;

                stackEntry->data.Object.attribute = -1;
            }

            break;
        case 6:
            if (((EventVMO *)basicObject)->vmObjectPtr != NULL)
            {
                stackEntry->id = 26;

                stackEntry->data.vmObject.vmObjectPtr = ((EventVMO *)basicObject)->vmObjectPtr;

                stackEntry->data.vmObject.level = STREAM_GetLevelWithID(((EventVMO *)basicObject)->unitID);

                stackEntry->data.vmObject.attribute = -1;
            }
            else
            {
                stackEntry->id = 6;

                stackEntry->data.Object.instanceNumber = item;

                stackEntry->data.Object.attribute = -1;
            }

            break;
        }

        topOfStack++;
    }

    stack->topOfStack = topOfStack;
}

void EVENT_AddCharPointerToStack(PCodeStack *stack, char *pointer)
{
    if (stack->topOfStack < 32)
    {
        StackType *stackEntry;

        stackEntry = &stack->stack[stack->topOfStack];

        stackEntry->id = 28;

        stackEntry->data.CharPointer.pointer = pointer;

        stack->topOfStack++;
    }
}

void EVENT_AddShortPointerToStack(PCodeStack *stack, short *pointer)
{
    if (stack->topOfStack < 32)
    {
        StackType *stackEntry;

        stackEntry = &stack->stack[stack->topOfStack];

        stackEntry->id = 10;

        stackEntry->data.ShortPointer.pointer = (char *)pointer;

        stack->topOfStack++;
    }
}

void EVENT_AddNumberToStack(PCodeStack *stack, long item, long flags)
{
    if (stack->topOfStack < 32)
    {
        StackType *stackEntry;

        stackEntry = &stack->stack[stack->topOfStack];

        stackEntry->id = 7;

        stackEntry->data.Number.currentNumber = item;

        stackEntry->data.Number.error = 0;

        stackEntry->data.Number.flags = flags;

        stack->topOfStack++;
    }
}

void EVENT_ChangeOperandToNumber(StackType *operand, long item, long flags)
{
    operand->id = 7;

    operand->data.Number.currentNumber = item;

    operand->data.Number.error = 0;

    operand->data.Number.flags = flags;
}

void EVENT_ChangeOperandVector3d(StackType *operand, short x, short y, short z, long streamUnitID)
{
    operand->id = 9;

    operand->data.vector3d.vx = x;
    operand->data.vector3d.vy = y;
    operand->data.vector3d.vz = z;

    operand->data.vector3d.errorx = 160;
    operand->data.vector3d.errory = 160;
    operand->data.vector3d.errorz = 160;

    operand->data.vector3d.streamUnitID = streamUnitID;
}

void EVENT_Addvector3dToStack(PCodeStack *stack, short x, short y, short z, long streamUnitID)
{
    if (stack->topOfStack < 32)
    {
        EVENT_ChangeOperandVector3d(&stack->stack[stack->topOfStack], x, y, z, streamUnitID);

        stack->topOfStack++;
    }
}

void EVENT_ChangeOperandRotation3d(StackType *operand, Rotation3d *rotation)
{
    operand->id = 14;

    operand->data.rotation3d.vx = rotation->vx;
    operand->data.rotation3d.vy = rotation->vy;
    operand->data.rotation3d.vz = rotation->vz;

    operand->data.rotation3d.errorx = rotation->errorx;
    operand->data.rotation3d.errory = rotation->errory;
    operand->data.rotation3d.errorz = rotation->errorz;

    operand->data.rotation3d.attribute = -1;
}

long EVENT_AddSubListObjectToStack(PCodeStack *stack, long listNumber)
{
    if (stack->topOfStack < 32)
    {
        StackType *stackEntry;

        stackEntry = &stack->stack[stack->topOfStack];

        stackEntry->id = 22;

        if (eventListNumInstances[listNumber] >= 1)
        {
            stackEntry->data.subListObject.instanceList = eventListArray[listNumber];

            stackEntry->data.subListObject.numberOfInstances = eventListNumInstances[listNumber];
        }
        else
        {
            stackEntry->data.subListObject.instanceList = NULL;

            stackEntry->data.subListObject.numberOfInstances = eventListNumInstances[listNumber];
        }

        stackEntry->data.subListObject.numberOfAttributes = 0;

        stack->topOfStack++;
    }

    return 0;
}

void EVENT_StackDuplicate(PCodeStack *stack)
{
    if ((stack->topOfStack < 32) && (stack->topOfStack != 0))
    {
        stack->stack[stack->topOfStack] = stack->stack[stack->topOfStack - 1];

        stack->topOfStack++;
    }
}

long EVENT_TransformTGroupAttribute(PCodeStack *stack, StackType *stackObject, long item, short *codeStream)
{
    long retValue;

    (void)stack;

    retValue = 0;

    if (stackObject->data.terrainGroup.attribute == -1)
    {
        stackObject->data.terrainGroup.attribute = item;

        retValue = 1;

        switch (item)
        {
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
        case 30:
        case 31:
        case 32:
        case 33:
        case 34:
        case 35:
        case 36:
        case 37:
        case 38:
        case 39:
        case 40:
        case 41:
        case 42:
        case 43:
            break;
        case 44:
            if (codeStream != NULL)
            {
                MoveCodeStreamExtra = 2;

                stackObject->data.terrainGroup.arg[0] = *++codeStream;

                stackObject->data.terrainGroup.arg[1] = *++codeStream;
            }

            retValue = 1;
            break;
        case 45:
            if (codeStream != NULL)
            {
                MoveCodeStreamExtra = 1;

                stackObject->data.terrainGroup.arg[0] = *++codeStream;
            }

            retValue = 1;
            break;
        case 46:
            if (codeStream != NULL)
            {
                MoveCodeStreamExtra = 2;

                stackObject->data.terrainGroup.arg[0] = *++codeStream;

                stackObject->data.terrainGroup.arg[1] = *++codeStream;
            }

            retValue = 1;
            break;
        case 47:
            if (codeStream != NULL)
            {
                MoveCodeStreamExtra = 1;

                stackObject->data.terrainGroup.arg[0] = *++codeStream;
            }

            retValue = 1;
            break;
        }
    }

    return retValue;
}

long EVENT_TransformConstrictAttribute(PCodeStack *stack, StackType *stackObject, long item)
{
    long retValue;
    Instance *instance;

    (void)stack;

    instance = stackObject->data.constrictInfo.instance;

    retValue = 0;

    switch (item)
    {
    case 58:
        EVENT_ChangeOperandToNumber(stackObject, instance->constrictAngle < -1, 0);

        retValue = 1;
        break;
    case 59:
        EVENT_ChangeOperandToNumber(stackObject, (instance->constrictAngle < 2) == 0, 0);

        retValue = 1;
        break;
    }

    return retValue;
}

long EVENT_TransformInstanceAttribute(PCodeStack *stack, StackType *stackObject, long item, short *codeStream)
{
    long retValue;
    long x;
    long y;
    long z;
    Instance *instance;

    (void)stack;

    instance = stackObject->data.instanceObject.instance;

    retValue = 0;

    switch (item)
    {
    case 5:
    {
        evPositionData *position;

        position = (evPositionData *)INSTANCE_Query(instance, 6);

        if (position == NULL)
        {
            x = instance->position.x;
            y = instance->position.y;
            z = instance->position.z;
        }
        else
        {
            x = position->x;
            y = position->y;
            z = position->z;
        }

        EVENT_ChangeOperandVector3d(stackObject, x, y, z, instance->currentStreamUnitID);

        retValue = 1;
        break;
    }
    case 9:
    {
        evPositionData *rotation;
        Rotation3d vector;

        rotation = (evPositionData *)INSTANCE_Query(instance, 7);

        if (rotation == NULL)
        {
            vector.vx = instance->rotation.x;
            vector.vy = instance->rotation.y;
            vector.vz = instance->rotation.z;
        }
        else
        {
            vector.vx = rotation->x;
            vector.vy = rotation->y;
            vector.vz = rotation->z;
        }

        vector.errorx = 512;
        vector.errorz = 512;
        vector.errory = 512;

        EVENT_ChangeOperandRotation3d(stackObject, &vector);

        retValue = 1;
        break;
    }
    case 12:
        stackObject->id = 15;

        stackObject->data.instanceSpline.instance = instance;

        stackObject->data.instanceSpline.spline = SCRIPT_GetMultiSpline(instance, (unsigned long *)&stackObject->data.instanceSpline.isParent, (unsigned long *)&stackObject->data.instanceSpline.isClass);

        stackObject->data.instanceSpline.splineFlags = 0;

        stackObject->data.instanceSpline.attribute = -1;

        retValue = 1;
        break;
    case 126:
    {
        int status;

        stackObject->id = 27;

        stackObject->data.soundObject.data.instance = instance;

        stackObject->data.soundObject.attribute = -1;

        if (codeStream != NULL)
        {
            MoveCodeStreamExtra = 1;

            stackObject->data.soundObject.soundNumber = *++codeStream;
        }

        status = SOUND_IsInstanceSoundLoaded(instance->object->soundData, stackObject->data.soundObject.soundNumber);

        if (status == 0)
        {
            WaitingToLoadSound -= gameTrackerX.timeMult;

            if (WaitingToLoadSound > 0)
            {
                EventAbortLine = 1;

                retValue = 1;
            }
            else
            {
                WaitingToLoadSound = 614400;
            }
        }
        else if (status != -1)
        {
            WaitingToLoadSound = 614400;
        }

        retValue = 1;
        break;
    }
    case 29:
        if (instance->object->animList != NULL)
        {
            retValue = 1;

            stackObject->id = 19;

            stackObject->data.instanceAnimate.instance = instance;

            stackObject->data.instanceAnimate.attribute = -1;
        }

        break;
    case 19:
        EVENT_ChangeOperandToNumber(stackObject, INSTANCE_Query(instance, 10), 3);

        retValue = 1;
        break;
    case 159:
        EVENT_ChangeOperandToNumber(stackObject, INSTANCE_Query(instance, 43), 0);

        retValue = 1;
        break;
    case 169:
        EVENT_ChangeOperandToNumber(stackObject, INSTANCE_Query(instance, 31), 0);

        retValue = 1;
        break;
    case 170:
        EVENT_ChangeOperandToNumber(stackObject, INSTANCE_Query(instance, 32), 0);

        retValue = 1;
        break;
    case 161:
    {
        Instance *tmpI;
        long value;

        tmpI = (Instance *)INSTANCE_Query(instance, 44);

        if (tmpI == NULL)
        {
            value = 0;
        }
        else
        {
            if ((INSTANCE_Query(tmpI, 1) & 0x20))
            {
                if ((INSTANCE_Query(tmpI, 4) & 0x3))
                {
                    return 1;
                }

                return 2;
            }

            value = 3;
        }

        EVENT_ChangeOperandToNumber(stackObject, value, 0);

        retValue = 1;
        break;
    }
    case 139:
    case 166:
        EVENT_ChangeOperandToNumber(stackObject, (INSTANCE_Query(instance, 0) >> 30) & 0x1, 0);

        retValue = 1;
        break;
    case 167:
        EVENT_ChangeOperandToNumber(stackObject, (INSTANCE_Query(instance, 0) >> 26) & 0x1, 0);

        retValue = 1;
        break;
    case 160:
    {
        unsigned long temp; // not from decls.h

        temp = INSTANCE_Query(instance, 1) & 0x4;

        EVENT_ChangeOperandToNumber(stackObject, temp > 0, 0);

        retValue = 1;
        break;
    }
    case 18:
        EVENT_ChangeOperandToNumber(stackObject, INSTANCE_Query(instance, 9), 1);

        retValue = 1;
        break;
    case 56:
        EVENT_ChangeOperandToNumber(stackObject, INSTANCE_Query(instance, 30), 0);

        retValue = 1;
        break;
    case 63:
        EVENT_ChangeOperandToNumber(stackObject, 1, 0);

        retValue = 1;
        break;
    case 33:
        stackObject->id = 20;

        stackObject->data.instanceAniTexture.instance = instance;

        retValue = 1;

        stackObject->data.instanceAniTexture.aniTextures = instance->object->modelList[instance->currentModel]->aniTextures;

        stackObject->data.instanceAniTexture.attribute = -1;
        break;
    case 43:
    {
        int temp; // not from decls.h
        StreamUnit *temp2; // not from decls.h

        temp = 0;

        if (instance->attachedID == 0)
        {
            temp2 = STREAM_GetStreamUnitWithID(instance->currentStreamUnitID);

            if ((temp2 != NULL) && (instance->bspTree < temp2->level->terrain->numBSPTrees))
            {
                temp = -((instance->currentStreamUnitID * 256) + temp2->level->terrain->BSPTreeArray[instance->bspTree].ID);
            }
        }
        else
        {
            temp = instance->attachedID;
        }

        EVENT_ChangeOperandToNumber(stackObject, temp, 0);

        retValue = 1;
        break;
    }
    case 146:
        EVENT_ChangeOperandToNumber(stackObject, 0, 0);

        retValue = 1;
        break;
    case 151:
        EVENT_ChangeOperandToNumber(stackObject, instance->currentStreamUnitID, 0);

        retValue = 1;
        break;
    case 153:
        EVENT_ChangeOperandToNumber(stackObject, instance->tface != NULL, 0);

        retValue = 1;
        break;
    case 57:
        retValue = 1;

        stackObject->id = 24;

        stackObject->data.instanceAnimate.instance = instance;

        stackObject->data.instanceAnimate.attribute = -1;
        break;
    case 60:
    {
        long value;

        value = 0;

        if ((instance->flags & 0x4))
        {
            value = 1;

            instance->flags &= ~0x4;
        }

        EVENT_ChangeOperandToNumber(stackObject, value, 0);

        retValue = 1;
        break;
    }
    case 121:
    {
        long value;

        value = 0;

        if ((instance->flags & 0x8))
        {
            value = 1;

            instance->flags &= ~0x8;
        }

        EVENT_ChangeOperandToNumber(stackObject, value, 0);

        retValue = 1;
        break;
    }
    case 122:
    {
        long value;

        value = 0;

        if ((instance->flags & 0x10))
        {
            value = 1;

            instance->flags &= ~0x10;
        }

        EVENT_ChangeOperandToNumber(stackObject, value, 0);

        retValue = 1;
        break;
    }
    case 106:
    {
        long value;

        value = 0;

        if (instance->flags < 0)
        {
            value = 1;

            instance->flags &= ~0x80000000;
        }

        EVENT_ChangeOperandToNumber(stackObject, value, 0);

        retValue = 1;
        break;
    }
    case 125:
    {
        long value;

        value = 0;

        if ((instance->flags2 & 0x10000))
        {
            value = 1;

            instance->flags2 &= ~0x10000;
        }

        EVENT_ChangeOperandToNumber(stackObject, value, 0);

        retValue = 1;
        break;
    }
    case 137:
        EVENT_ChangeOperandToNumber(stackObject, INSTANCE_Query(instance, 36), 3);

        retValue = 1;
        break;
    case 4:
    case 10:
    case 11:
    case 20:
    case 21:
    case 32:
    case 36:
    case 37:
    case 38:
    case 39:
    case 51:
    case 52:
    case 53:
    case 54:
    case 55:
    case 62:
    case 64:
    case 75:
    case 76:
    case 77:
    case 78:
    case 79:
    case 80:
    case 83:
    case 84:
    case 85:
    case 91:
    case 92:
    case 94:
    case 95:
    case 103:
    case 108:
    case 109:
    case 110:
    case 111:
    case 114:
    case 116:
    case 123:
    case 124:
    case 138:
    case 140:
    case 141:
    case 143:
    case 144:
    case 163:
    case 164:
    case 165:
        stackObject->data.instanceObject.attribute = item;

        retValue = 1;
    }

    return retValue;
}

long EVENT_TransformSoundObjectAttribute(PCodeStack *stack, SoundObject *soundObject, long item, short *codeStream)
{
    long retValue;

    (void)stack;

    retValue = 0;

    switch (item)
    {
    case 126:
    {
        int status;

        if (codeStream != NULL)
        {
            MoveCodeStreamExtra = 1;

            soundObject->soundNumber = *++codeStream;
        }

        status = SOUND_IsInstanceSoundLoaded(soundObject->data.sfxMarker->soundData, soundObject->soundNumber);

        if (status == 0)
        {
            WaitingToLoadSFX -= gameTrackerX.timeMult;

            if (WaitingToLoadSFX > 0)
            {
                EventAbortLine = 1;
            }
            else
            {
                WaitingToLoadSFX = 614400;
            }
        }
        else if (status != -1)
        {
            WaitingToLoadSFX = 614400;
        }

        retValue = 1;
        break;
    }
    case 127:
        soundObject->attribute = item;

        if (codeStream != NULL)
        {
            MoveCodeStreamExtra = 2;

            soundObject->value = *++codeStream;

            soundObject->duration = *++codeStream;
        }

        retValue = 1;
        break;
    case 13:
    case 14:
        soundObject->attribute = item;

        retValue = 1;
        break;
    case 128:
        soundObject->attribute = item;

        if (codeStream != NULL)
        {
            MoveCodeStreamExtra = 2;

            soundObject->value = *++codeStream;

            soundObject->duration = *++codeStream;
        }

        retValue = 1;
        break;
    }

    return retValue;
}

long EVENT_GetGameValue(GameObject *gameObject)
{
    long value;

    value = 0;

    switch (gameObject->attribute)
    {
    case -1:
        value = 1;
        break;
    case 2:
        value = GAMELOOP_GetTimeOfDay();

        switch (value)
        {
        case 600:
            value = 1;
            break;
        case 700:
            value = 2;
            break;
        case 1800:
            value = 3;
            break;
        case 1900:
            value = 4;
            break;
        }

        break;
    }

    return value;
}

long EVENT_TransformGameAttribute(PCodeStack *stack, StackType *stackObject, long item, short *codeStream)
{
    long value;
    long retValue;

    retValue = 0;

    switch (item)
    {
    case 1:
        EVENT_ChangeOperandToNumber(stackObject, (long)((gameTrackerX.currentTime * 30) / 1000), 0);

        retValue = 1;
        break;
    case 61:
        EVENT_ChangeOperandToNumber(stackObject, gameTrackerX.timeOfDay, 0);

        retValue = 1;
        break;
    case 27:
        value = gameTrackerX.controlCommand[0][0];

        if (((value & 0x80)) && ((value & 0x10)))
        {
            value &= ~0x90;
        }

        EVENT_ChangeOperandToNumber(stackObject, value, 1);

        retValue = 1;
        break;
    case 157:
        EVENT_ChangeOperandToNumber(stackObject, HINT_GetCurrentHint(), 0);

        retValue = 1;
        break;
    case 158:
        EVENT_ChangeOperandToNumber(stackObject, gameTrackerX.streamFlags & 0x100000, 0);

        retValue = 1;
        break;
    case 20:
        value = INSTANCE_Query(gameTrackerX.playerInstance, 11) >> 1;

        value &= 0x1;

        if (STREAM_IsMorphInProgress() != 0)
        {
            value = 0;
        }

        EVENT_ChangeOperandToNumber(stackObject, value, 0);

        retValue = 1;
        break;
    case 21:
        value = INSTANCE_Query(gameTrackerX.playerInstance, 11) & 0x1;

        if (STREAM_IsMorphInProgress() != 0)
        {
            value = 0;
        }

        EVENT_ChangeOperandToNumber(stackObject, value, 0);

        retValue = 1;
        break;
    case 49:
    {
        EventTimer *timer;
        short time;

        timer = EVENT_GetNextTimer();

        if ((timer != NULL) && (codeStream != NULL))
        {
            MoveCodeStreamExtra = 1;

            codeStream++;

            time = *codeStream++;

            EventAbortLine = 1;

            EventJustRecievedTimer = 1;

            timer->time = time << 12;

            timer->event = currentEventInstance;

            timer->actionScript = currentActionScript;

            timer->scriptPos = codeStream;

            currentActionScript->conditionBits |= 0x1;

            timer->level = CurrentPuzzleLevel;

            timer->nextEventIndex = EventCurrentEventIndex;
        }

        retValue = 1;
        break;
    }
    case 66:
        stackObject->id = 25;

        stackObject->data.cameraObject.camera = &theCamera;

        stackObject->data.cameraObject.attribute = -1;

        retValue = 1;
        break;
    case 145:
        stack->topOfStack--;

        EVENT_AddShortPointerToStack(stack, &gEndGameNow);

        retValue = 1;
        break;
    case 74:
        if (codeStream != NULL)
        {
            long wipeType;
            long wipeTime;

            MoveCodeStreamExtra = 2;

            wipeType = *++codeStream;
            wipeTime = *++codeStream;

            gameTrackerX.maxWipeTime = (*codeStream < 0) ? -*codeStream : *codeStream;

            gameTrackerX.wipeTime = wipeTime;
            gameTrackerX.wipeType = wipeType;

            if (wipeType == 11)
            {
                if (wipeTime < 0)
                {
                    gameTrackerX.streamFlags |= 0x2000000;
                }
                else
                {
                    gameTrackerX.streamFlags &= ~0x2000000;
                }
            }

            stack->topOfStack--;
        }

        retValue = 1;
        break;
    case 162:
        if (codeStream != NULL)
        {
            long motor0Time;
            long motor0Speed;
            long motor1Time;
            long motor1Speed;

            MoveCodeStreamExtra = 4;

            motor0Speed = *++codeStream;
            motor0Time = *++codeStream;

            codeStream++;

            do
            {
                motor1Time = codeStream[1];
                motor1Speed = codeStream[0];
            } while (0); // this loop isn't really necessary (it's garbage), however the two lines inside of it are

            stack->topOfStack--;

            GAMEPAD_Shock(motor0Speed, motor0Time << 12, motor1Speed, motor1Time << 12);
        }

        retValue = 1;
        break;
    case 82:
        INSTANCE_Broadcast(NULL, 42, 0x4000E, 1);

        stack->topOfStack--;

        retValue = 1;
        break;
    case 81:
        INSTANCE_Broadcast(NULL, 10, 0x4000E, 0);

        stack->topOfStack--;

        retValue = 1;
        break;
    case 90:
        if (codeStream != NULL)
        {
            short rand1;
            short rand2;

            MoveCodeStreamExtra = 2;

            rand1 = *++codeStream;
            rand2 = *++codeStream;

            EVENT_ChangeOperandToNumber(stackObject, (rand() % (rand2 - rand1)) + rand1, 0);
        }

        retValue = 1;
        break;
    case 120:
        EVENT_ChangeOperandToNumber(stackObject, LOAD_IsXAInQueue() == 0, 0);

        retValue = 1;
        break;
    case 134:
    {
        int number;

        number = ((unsigned long)gameTrackerX.streamFlags >> 23) & 0x1;

        EVENT_ChangeOperandToNumber(stackObject, number, 0);

        retValue = 1;
        break;
    }
    case 154:
        EVENT_ChangeOperandToNumber(stackObject, INSTANCE_Query(gameTrackerX.playerInstance, 41), 3);

        retValue = 1;
        break;
    case 155:
        EVENT_ChangeOperandToNumber(stackObject, INSTANCE_Query(gameTrackerX.playerInstance, 42), 3);

        retValue = 1;
        break;
    case 168:
        stack->topOfStack--;

        if (((gameTrackerX.debugFlags & 0x80000)) && (LOAD_IsXAInQueue() != 0))
        {
            EventAbortLine = 1;
        }

        retValue = 1;
        break;
    case 2:
    case 88:
    case 89:
    case 108:
    case 109:
    case 110:
    case 111:
    case 118:
    case 119:
    case 135:
    case 136:
    case 137:
    case 140:
    case 142:
    case 147:
    case 150:
    case 156:
        stackObject->data.gameObject.attribute = item;

        retValue = 1;
        break;
    }

    return retValue;
}

long EVENT_TransformAreaAttribute(PCodeStack *stack, StackType *stackObject, long item, short *codeStream)
{
    long retValue;
    long offset;
    StreamUnit *streamUnit;

    streamUnit = stackObject->data.areaObject.streamUnit;

    retValue = 0;

    if (streamUnit == NULL)
    {
        EventAbortLine = 1;

        retValue = 1;
    }
    else
    {
        switch (item)
        {
        case 3:
            MoveCodeStreamExtra = 1;

            offset = *++codeStream;

            if ((offset >= 0) && (offset <= 5))
            {
                stack->topOfStack--;

                EVENT_AddShortPointerToStack(stack, &streamUnit->eventVariables[offset]);

                retValue = 1;
            }

            break;
        case 65:
            stack->topOfStack--;

            EVENT_AddShortPointerToStack(stack, (short *)&streamUnit->level->waterZLevel);

            retValue = 1;
            break;
        case 113:
        case 112:
            stackObject->data.areaObject.attribute = item;

            retValue = 1;
            break;
        }
    }

    return retValue;
}

long EVENT_TransformEventAttribute(PCodeStack *stack, StackType *stackObject, long item, short *codeStream)
{
    long retValue;
    long offset;
    Event *event;

    event = stackObject->data.eventObject.event;

    retValue = 0;

    switch (item)
    {
    case 3:
        MoveCodeStreamExtra = 1;

        offset = *++codeStream;

        if ((unsigned long)offset < 6)
        {
            stack->topOfStack--;

            EVENT_AddShortPointerToStack(stack, &event->eventVariables[offset]);

            retValue = 1;
        }

        break;
    case 26:
        if (event->processingPuppetShow == 0)
        {
            event->processingPuppetShow = 1;
        }

        retValue = 1;
        break;
    }

    return retValue;
}

long EVENT_TransformSavedEventAttribute(PCodeStack *stack, StackType *stackObject, long item, short *codeStream)
{
    long retValue;
    long offset;
    SavedBasic *temp; // not from decls.h

    retValue = 0;

    switch (item)
    {
    case 3:
        MoveCodeStreamExtra = 1;

        offset = *++codeStream;

        if ((unsigned long)offset < 6)
        {
            temp = stackObject->data.savedEventObject.savedEvent;

            if (temp != NULL)
            {
                if (temp->savedID == 2)
                {
                    stack->topOfStack--;

                    EVENT_AddShortPointerToStack(stack, &((SavedEvent *)temp)->eventVariables[offset]);

                    retValue = 1;
                    break;
                }

                stack->topOfStack--;

                EVENT_AddCharPointerToStack(stack, &((SavedEventSmallVars *)temp)->eventVariables[offset]);

                retValue = 1;
                break;
            }

            EVENT_ChangeOperandToNumber(stackObject, 0, 0);

            retValue = 1;
        }

        break;
    }

    return retValue;
}

long EVENT_TransformSubListObjectAttribute(PCodeStack *stack, StackType *stackObject, long item)
{
    long retValue;
    int temp; // not from decls.h

    (void)stack;

    temp = stackObject->data.subListObject.numberOfAttributes;

    retValue = 0;

    if (temp < 5)
    {
        retValue = 1;

        stackObject->data.subListObject.attribute[temp] = item;

        stackObject->data.subListObject.numberOfAttributes = temp + 1;
    }

    return retValue;
}

long EVENT_TransformListObjectAttribute(PCodeStack *stack, StackType *stackObject, long item)
{
    long retValue;
    int temp; // not from decls.h

    (void)stack;

    temp = stackObject->data.listObject.numberOfAttributes;

    retValue = 0;

    if (temp < 5)
    {
        retValue = 1;

        stackObject->data.listObject.attribute[temp] = item;

        stackObject->data.listObject.numberOfAttributes = temp + 1;
    }

    return retValue;
}

long EVENT_TransformCameraObjectAttribute(PCodeStack *stack, StackType *stackObject, long item, short *codeStream)
{
    long retValue;
    Camera *camera;

    camera = stackObject->data.cameraObject.camera;

    retValue = 0;

    switch (item)
    {
    case 67:
    case 68:
    case 69:
    case 70:
    case 71:
    case 72:
    case 73:
        stackObject->data.cameraObject.attribute = item;

        retValue = 1;
        break;
    case 93:
    {
        short time;

        if (codeStream != NULL)
        {
            codeStream++;

            MoveCodeStreamExtra = 2;

            time = *codeStream++;

            CAMERA_SetShake(camera, time, (*codeStream << 12) / 100);

            stack->topOfStack--;
        }

        retValue = 1;
        break;
    }
    case 104:
        stackObject->data.cameraObject.attribute = item;

        retValue = 1;
        break;
    case 105:
        if (codeStream != NULL)
        {
            MoveCodeStreamExtra = 1;

            stackObject->data.cameraObject.frames = *++codeStream;
        }

        stackObject->data.cameraObject.attribute = item;

        retValue = 1;
        break;
    case 117:
        EVENT_ChangeOperandToNumber(stackObject, camera->data.Cinematic.cinema_done != 0, 0);

        retValue = 1;
        break;
    case 9:
    case 16:
        stackObject->data.cameraObject.attribute = item;

        retValue = 1;
        break;
    }

    return retValue;
}

long EVENT_TransformSplineAttribute(PCodeStack *stack, StackType *stackObject, long item, short *codeStream)
{
    long retValue;
    Instance *instance;

    (void)stack;

    retValue = 0;

    instance = stackObject->data.instanceSpline.instance;

    switch (item)
    {
    case 31:
        MoveCodeStreamExtra = 2;

        instance->clipBeg = *++codeStream;
        instance->clipEnd = *++codeStream;

        instance->splineFlags |= 0x2;

        if ((instance->clipBeg == -1) && (instance->clipEnd == -1))
        {
            instance->splineFlags &= ~0x2000002;
        }
        else
        {
            long maxKeyFrames;

            maxKeyFrames = SCRIPTCountFramesInSpline(instance);

            if (instance->clipEnd >= maxKeyFrames)
            {
                instance->clipEnd = maxKeyFrames;
            }

            if (instance->clipBeg < 0)
            {
                instance->clipBeg = 0;
            }
        }

        retValue = 1;
        break;
    case 34:
        stackObject->data.instanceSpline.splineFlags |= 0x4;

        retValue = 1;
        break;
    case 35:
        stackObject->data.instanceSpline.splineFlags |= 0x8;

        retValue = 1;
        break;
    default:
        stackObject->data.instanceSpline.attribute = item;

        retValue = 1;
    }

    return retValue;
}

long EVENT_TransformIntroAttribute(PCodeStack *stack, StackType *stackObject, long item)
{
    long retValue;
    long x;
    long y;
    long z;
    Intro *intro;

    (void)stack;

    retValue = 0;

    intro = stackObject->data.introObject.intro;

    switch (item)
    {
    case 5:
        if (!(intro->flags & 0x4000))
        {
            EventAbortLine = 1;
        }
        else
        {
            x = intro->position.x;
            y = intro->position.y;
            z = intro->position.z;

            EVENT_ChangeOperandVector3d(stackObject, x, y, z, STREAM_WhichUnitPointerIsIn(intro)->StreamUnitID);
        }

        retValue = 1;
        break;
    case 9:
    {
        Rotation3d vector;

        vector.vx = intro->rotation.x;
        vector.vy = intro->rotation.y;
        vector.vz = intro->rotation.z;

        vector.errorx = 512;
        vector.errorz = 512;
        vector.errory = 512;

        EVENT_ChangeOperandRotation3d(stackObject, &vector);

        retValue = 1;
        break;
    }
    case 63:
        EVENT_ChangeOperandToNumber(stackObject, 0, 0);

        retValue = 1;
        break;
    case 4:
    case 62:
    case 138:
    case 146:
    case 160:
    case 166:
        stackObject->data.introObject.attribute = item;

        retValue = 1;
        break;
    case 7:
    case 10:
    case 16:
    case 32:
    case 80:
    case 112:
    case 144:
    case 165:
        EventAbortLine = 1;

        retValue = 1;
        break;
    default:
        EventAbortLine = 1;

        retValue = 1;
    }

    return retValue;
}

long EVENT_ParseOperand2(StackType *operand2, long *error, long *trueValue)
{
    long number;

    number = -1;

    if (operand2 != NULL)
    {
        short flags;

        number = EVENT_GetScalerValueFromOperand(operand2, error, &flags);

        if ((*error == 0) && (number == 0))
        {
            *trueValue = 0;
        }
    }

    return number;
}

long EVENT_DoVMObjectAction(EventVmObject *vmobject, StackType *operand2)
{
    long result;
    long trueValue;
    long number;
    long error;

    result = 0;

    trueValue = 1;

    number = -1;

    error = 1;

    if (vmobject->attribute != -1)
    {
        number = EVENT_ParseOperand2(operand2, &error, &trueValue);

        switch (vmobject->attribute)
        {
        case 14:
            trueValue = trueValue == 0;
        case 13:
            if (trueValue != 0)
            {
                vmobject->vmObjectPtr->flags &= ~0x2;
            }
            else
            {
                vmobject->vmObjectPtr->flags |= 0x2;
            }

            result = 1;
            break;
        case 99:
            if (number != -1)
            {
                if ((number >= 0) && (number < vmobject->vmObjectPtr->numVMOffsetTables))
                {
                    VM_VMObjectSetTable(vmobject->level, vmobject->vmObjectPtr, number);

                    if (((vmobject->vmObjectPtr->flags & 0x2)) && (vmobject->level != NULL))
                    {
                        VM_ProcessVMObjectSetColor_S(vmobject->level, vmobject->vmObjectPtr);
                    }
                }
            }

            break;
        case 100:
            if (number != -1)
            {
                vmobject->vmObjectPtr->timer = number;

                if (((vmobject->vmObjectPtr->flags & 0x2)) && (vmobject->level != NULL))
                {
                    VM_ProcessVMObjectSetColor_S(vmobject->level, vmobject->vmObjectPtr);

                    result = 1;
                    break;
                }
            }

            result = 1;
            break;
        }
    }

    return result;
}

long EVENT_GetVMObjectValue(EventVmObject *vmobject)
{
    long value;
    long trueValue;

    value = 0;

    trueValue = 1;

    switch (vmobject->attribute)
    {
    case -1:
        value = 1;
        break;
    case 14:
        trueValue = 0;
    case 13:
        value = trueValue;

        if ((vmobject->vmObjectPtr->flags & 0x2))
        {
            value = value == 0;
        }

        break;
    case 99:
        value = vmobject->vmObjectPtr->currentIdx;
        break;
    case 100:
        value = vmobject->vmObjectPtr->timer % vmobject->vmObjectPtr->curVMOffsetTable->numVMOffsets;
        break;
    }

    return value;
}

long EVENT_DoGameAction(GameObject *gameObject, StackType *operand2)
{
    long error;
    long number;
    long trueValue;

    error = 1;

    trueValue = 1;

    if (gameObject->attribute != -1)
    {
        number = EVENT_ParseOperand2(operand2, &error, &trueValue);

        switch (gameObject->attribute)
        {
        case 88:
            if (error != 0)
            {
                FX_Start_Snow(100);
            }
            else
            {
                FX_Start_Snow(number);
            }

            break;
        case 89:
            if (error != 0)
            {
                FX_Start_Rain(100);
            }
            else
            {
                FX_Start_Rain(number);
            }

            break;
        case 118:
            if ((number >= 0) && ((gameTrackerX.debugFlags & 0x80000)))
            {
                if (WaitingForVoiceNumber != number)
                {
                    if (LOAD_IsXAInQueue() == 0)
                    {
                        LOAD_PlayXA(number);

                        EventAbortLine = 1;

                        WaitingForVoiceNumber = number;
                    }
                    else
                    {
                        EventAbortLine = 1;
                    }
                }
                else if (VOICEXA_IsPlaying() != 2)
                {
                    EventAbortLine = 1;
                }
                else
                {
                    WaitingForVoiceNumber = -1;
                }
            }

            break;
        case 119:
            if ((number > 0) && (number < 128))
            {
                SOUND_SetVoiceVolume(number);
            }

            break;
        case 142:
            HINT_StartHint(number);
            break;
        case 156:
            HINT_KillSpecificHint(number);
            break;
        case 147:
            HINT_StopHint();
            break;
        case 2:
            switch (number)
            {
            case 1:
                GAMELOOP_SetGameTime(600);
                break;
            case 2:
                GAMELOOP_SetGameTime(700);
                break;
            case 3:
                GAMELOOP_SetGameTime(1800);
                break;
            case 4:
                GAMELOOP_SetGameTime(1900);
                break;
            }

            break;
        case 135:
            SOUND_SetMusicModifier(number);
            break;
        case 136:
            SOUND_ResetMusicModifier(number);
            break;
        case 150:
            if (MoviePlayed == 0)
            {
                MovieToPlay = number;

                EventAbortLine = 1;
            }
            else
            {
                MoviePlayed = 0;

                MovieToPlay = -1;
            }

            break;
        }
    }

    return 1;
}

long EVENT_DoSignalAction(SignalObject *signalObject, StackType *operand2)
{
    long trueValue;
    long number;
    long error;
    long temp; // not from decls.h 

    number = -1;

    trueValue = 1;

    temp = 0;

    if (signalObject->attribute != number)
    {
        EVENT_ParseOperand2(operand2, &error, &trueValue);

        if ((signalObject->attribute == 26) && (trueValue != 0))
        {
            COLLIDE_HandleSignal(gameTrackerX.playerInstance, signalObject->msignal->signalList, signalObject->msignal->numSignals, 0);
        }
    }

    return temp;
}

long EVENT_TransformSignalAttribute(PCodeStack *stack, StackType *stackObject, long item)
{
    long retValue;
    MultiSignal *msignal;

    (void)stack;

    retValue = 0;

    msignal = stackObject->data.signalObject.msignal;

    switch (item)
    {
    case 26:
        stackObject->data.signalObject.attribute = item;

        retValue = 1;
        break;
    case 50:
        EVENT_ChangeOperandToNumber(stackObject, msignal->flags & 0x1, 0);

        retValue = 1;
        break;
    }

    return retValue;
}

long EVENT_TransformRotation3dAttribute(PCodeStack *stack, StackType *stackObject, long item, short *codeStream)
{
    long retValue;

    (void)stack;
    (void)codeStream;

    switch (item)
    {
    case 6:
    case 7:
    case 8:
    case 96:
    case 97:
    case 98:
        stackObject->data.rotation3d.attribute = item;

        retValue = 1;
        break;
    default:
        retValue = 0;
    }

    return retValue;
}

long EVENT_TransformVector3dAttribute(PCodeStack *stack, StackType *stackObject, long item, short *codeStream)
{
    long retValue;

    (void)stack;

    switch (item)
    {
    case 6:
        EVENT_ChangeOperandToNumber(stackObject, stackObject->data.vector3d.vx, 0);

        retValue = 1;
        break;
    case 7:
        EVENT_ChangeOperandToNumber(stackObject, stackObject->data.vector3d.vy, 0);

        retValue = 1;
        break;
    case 8:
        EVENT_ChangeOperandToNumber(stackObject, stackObject->data.vector3d.vz, 0);

        retValue = 1;
        break;
    case 23:
        stackObject->data.vector3d.errorx = *++codeStream;
        stackObject->data.vector3d.errory = -1;
        stackObject->data.vector3d.errorz = -1;

        MoveCodeStreamExtra = 1;

        retValue = 1;
        break;
    case 24:
        MoveCodeStreamExtra = 2;

        stackObject->data.vector3d.errorx = *++codeStream;
        stackObject->data.vector3d.errory = *++codeStream;
        stackObject->data.vector3d.errorz = -1;

        retValue = 1;
        break;
    case 25:
        MoveCodeStreamExtra = 3;

        stackObject->data.vector3d.errorx = *++codeStream;
        stackObject->data.vector3d.errory = *++codeStream;
        stackObject->data.vector3d.errorz = *++codeStream;

        retValue = 1;
        break;
    default:
        retValue = 0;
    }

    return retValue;
}

void EVENT_ModifyObjectToStackWithAttribute(PCodeStack *stack, long item, short *codeStream)
{
    if (stack->topOfStack != 0)
    {
        EVENT_TransformObjectOnStack(stack, item, codeStream);
    }
}

long EVENT_DoInstanceAnimateTextureAction(InstanceAnimateTexture *instanceAniTexture, StackType *operand2)
{
    long trueValue;
    long number;
    long result;
    long error;

    number = -1;

    result = 0;

    trueValue = 1;

    error = 1;

    if (instanceAniTexture->attribute == -1)
    {
        result = 1;
    }
    else
    {
        number = EVENT_ParseOperand2(operand2, &error, &trueValue);

        switch (instanceAniTexture->attribute)
        {
        case 14:
            trueValue ^= 1;
        case 13:
        case 41:
            result = 1;

            if (trueValue != 0)
            {
                instanceAniTexture->instance->flags &= ~0x80;
            }
            else
            {
                instanceAniTexture->instance->flags |= 0x80;
            }

            break;
        case 15:
        case 40:
            if (error == 0)
            {
                instanceAniTexture->instance->currentTextureAnimFrame = number;
            }

            break;
        }
    }

    return result;
}

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

long EVENT_DoSplineAction(InstanceSpline *instanceSpline, StackType *operand2)
{
    long trueValue;
    long number;
    long result;
    long error;

    number = -1;

    result = 0;

    trueValue = 1;

    if (instanceSpline->attribute == number)
    {
        result = 1;
    }
    else
    {
        Instance *instance;
        MultiSpline *spline;

        instance = instanceSpline->instance;

        spline = instanceSpline->spline;

        number = EVENT_ParseOperand2(operand2, &error, &trueValue);

        switch (instanceSpline->attribute)
        {
        case 14:
            trueValue ^= 1;
        case 13:
        case 41:
            if (trueValue != 0)
            {
                instance->flags |= 0x2000000;

                result = 1;
            }
            else
            {
                instance->flags &= ~0x2000000;

                result = 1;
            }

            break;
        case 15:
        case 40:
            if (number != -1)
            {
                SCRIPT_InstanceSplineSet(instance, number, NULL, NULL, NULL);

                instance->flags &= ~0x2000000;
            }

            break;
        case 17:
            if (number > 0)
            {
                instance->flags &= ~0x1000000;
            }
            else
            {
                instance->flags |= 0x1000000;
            }

            break;
        case 28:
        {
            long curKeyFrame;

            if (number < 0)
            {
                number = 0;
            }
            else
            {
                long maxKeyFrames;

                maxKeyFrames = SCRIPTCountFramesInSpline(instance);

                if (number >= maxKeyFrames)
                {
                    number = maxKeyFrames;
                }
            }

            curKeyFrame = EVENT_GetSplineFrameNumber(instanceSpline);

            if (curKeyFrame == number)
            {
                SCRIPT_InstanceSplineSet(instance, number, NULL, NULL, NULL);

                instance->flags &= ~0x2000000;
            }
            else
            {
                instance->splineFlags |= 0x1;

                instance->targetFrame = number;

                instance->flags |= 0x2000000;

                if (number < curKeyFrame)
                {
                    instanceSpline->splineFlags &= 0x4;

                    if (instanceSpline->splineFlags != 0)
                    {
                        EVENT_ResetAllSplineFlags(spline);

                        EVENT_SetSplineLoop(spline);

                        instance->splineFlags |= 0x20;

                        instance->flags &= ~0x1000000;
                    }
                    else
                    {
                        instance->flags |= 0x1000000;
                    }
                }
                else
                {
                    instanceSpline->splineFlags &= 0x8;

                    if (instanceSpline->splineFlags != 0)
                    {
                        EVENT_ResetAllSplineFlags(spline);

                        EVENT_SetSplineLoop(spline);

                        instance->splineFlags |= 0x20;

                        instance->flags |= 0x1000000;
                    }
                    else
                    {
                        instance->flags &= ~0x1000000;
                    }
                }
            }

            break;
        }
        case 16:
            EVENT_ResetAllSplineFlags(spline);

            instance->splineFlags &= ~0x70;

            switch (number)
            {
            case 1:
                if (spline->positional != NULL)
                {
                    spline->positional->flags |= 0x1;
                }

                if (spline->rotational != NULL)
                {
                    spline->rotational->flags |= 0x1;
                }

                if (spline->scaling != NULL)
                {
                    spline->scaling->flags |= 0x1;
                }

                if (spline->color != NULL)
                {
                    spline->color->flags |= 0x1;
                }

                instance->splineFlags |= 0x10;
                break;
            case 2:
                EVENT_SetSplineLoop(spline);

                instance->splineFlags |= 0x20;
                break;
            case 3:
            default:
                if (spline->positional != NULL)
                {
                    spline->positional->flags |= 0x4;
                }

                if (spline->rotational != NULL)
                {
                    spline->rotational->flags |= 0x4;
                }

                if (spline->scaling != NULL)
                {
                    spline->scaling->flags |= 0x4;
                }

                if (spline->color != NULL)
                {
                    spline->color->flags |= 0x4;
                }

                instance->splineFlags |= 0x40;
            }
        }
    }

    return result;
}

long EVENT_DoAnimateAction(InstanceAnimate *instanceAnimate, StackType *operand2)
{
    long trueValue;
    long number;
    long result;
    long error;
    Instance *instance;
    int temp, temp3; // not from decls.h
    Instance *temp2; // not from decls.h

    number = -1;

    result = 0;

    trueValue = 1;

    if (instanceAnimate->attribute != -1)
    {
        instance = instanceAnimate->instance;

        number = EVENT_ParseOperand2(operand2, &error, &trueValue);

        switch (instanceAnimate->attribute)
        {
        case 121:
            break;
        case 132:
            instance->aliasCommand.speed = number;
            break;
        case 30:
            result = 1;

            if (number != -1)
            {
                instance->aliasCommand.newanim = number;
            }

            break;
        case 15:
        case 40:
            if (number != -1)
            {
                instance->aliasCommand.newframe = number;
            }

            break;
        case 102:
            if (number != -1)
            {
                instance->aliasCommand.interpframes = number;
            }

            break;
        case 101:
            if (operand2 != NULL)
            {
                if (operand2->id == 2)
                {
                    instance->aliasCommand.hostInstance = operand2->data.instanceAnimate.instance;
                }
                else
                {
                    EventAbortLine = 1;
                }
            }

            break;
        case 14:
            trueValue ^= 1;
        case 13:
        case 41:
            if (trueValue != 0)
            {
                Instance *hostInstance;

                hostInstance = instance->aliasCommand.hostInstance;

                if (hostInstance == NULL)
                {
                    temp = SetAnimationInstanceSwitchData(instance, instance->aliasCommand.newanim, instance->aliasCommand.newframe, instance->aliasCommand.interpframes, 1);

                    temp2 = instance;

                    temp3 = 0x8000008;
                }
                else
                {
                    temp = SetActionPlayHostAnimationData(instance, instance->aliasCommand.hostInstance, instance->aliasCommand.newanim, instance->aliasCommand.newframe, instance->aliasCommand.interpframes, 1);

                    temp2 = instance;

                    temp3 = 0x40003;
                }

                INSTANCE_Post(temp2, temp3, temp);
            }
            else
            {
                INSTANCE_Post(instance, 0x8000010, 0);
            }

            result = 1;

            instance->aliasCommand = (EventAliasCommandStruct){0};
            break;
        case 42:
            if (instance->aliasCommand.hostInstance == NULL)
            {
                INSTANCE_Post(instance, 0x8000008, SetAnimationInstanceSwitchData(instance, instance->aliasCommand.newanim, instance->aliasCommand.newframe, instance->aliasCommand.interpframes, 2));
            }
            else
            {
                INSTANCE_Post(instance, 0x40003, SetActionPlayHostAnimationData(instance, instance->aliasCommand.hostInstance, instance->aliasCommand.newanim, instance->aliasCommand.newframe, instance->aliasCommand.interpframes, 2));
            }

            instance->aliasCommand = (EventAliasCommandStruct){0};

            result = 1;
            break;
        case 107:
            if (instance->aliasCommand.hostInstance == NULL)
            {
                INSTANCE_Post(instance, 0x8000008, SetAnimationInstanceSwitchData(instance, instance->aliasCommand.newanim, instance->aliasCommand.newframe, instance->aliasCommand.interpframes, 0));
            }
            else
            {
                INSTANCE_Post(instance, 0x40003, SetActionPlayHostAnimationData(instance, instance->aliasCommand.hostInstance, instance->aliasCommand.newanim, instance->aliasCommand.newframe, instance->aliasCommand.interpframes, 0));
            }

            if (instance->aliasCommand.speed > 0)
            {
                INSTANCE_Post(instance, 0x40020, instance->aliasCommand.speed);
            }

            instance->aliasCommand = (EventAliasCommandStruct){0};

            result = 1;
            break;
        }
    }

    return result;
}

long EVENT_DoInstanceAction(InstanceObject *instanceObject, StackType *operand2, short *codeStream)
{
    long trueValue;
    long number;
    long error;
    long result;
    Instance *instance;

    number = -1;

    trueValue = 1;

    instance = instanceObject->instance;

    result = 0;

    if (instanceObject->attribute != number)
    {
        number = EVENT_ParseOperand2(operand2, &error, &trueValue);

        switch (instanceObject->attribute)
        {
        case 38:
            trueValue ^= 1;
        case 39:
            if (trueValue != 0)
            {
                INSTANCE_Post(instance, 0x800021, 0);
            }
            else
            {
                INSTANCE_Post(instance, 0x800022, 0);
            }

            break;
        case 37:
            trueValue ^= 1;
        case 36:
            if (trueValue != 0)
            {
                if (!(INSTANCE_Query(instanceObject->instance, 5) & 0x1))
                {
                    INSTANCE_Post(instance, 0x800020, 0);
                }
            }
            else if ((INSTANCE_Query(instanceObject->instance, 5) & 0x1))
            {
                INSTANCE_Post(instance, 0x800020, 0);
            }

            break;
        case 20:
            trueValue ^= 1;
        case 21:
            if (trueValue != 0)
            {
                INSTANCE_Post(instance, 0x10002002, 0);

                result = 1;
            }
            else
            {
                INSTANCE_Post(instance, 0x10002001, 0);

                result = 1;
            }

            break;
        case 52:
            if (trueValue != 0)
            {
                instance->flags |= 0x800;
            }
            else
            {
                instance->flags &= ~0x800;
            }

            result = 1;
            break;
        case 53:
            if (trueValue != 0)
            {
                instance->flags2 |= 0x20000000;
                instance->flags |= 0x40000;
            }
            else
            {
                instance->flags2 &= ~0x20000000;
                instance->flags &= ~0x40000;
            }

            result = 1;
            break;
        case 11:
            trueValue ^= 1;
        case 10:
            if (trueValue != 0)
            {
                instance->flags |= 0x800;
                instance->flags2 |= 0x20000000;

                instance->flags |= 0x40000;

                if ((instance->object->oflags2 & 0x80000))
                {
                    instance->flags2 |= 0x10000000;
                }
            }
            else
            {
                instance->flags &= ~0x800;
                instance->flags2 &= ~0x20000000;

                instance->flags &= ~0x40000;

                if ((instance->object->oflags2 & 0x80000))
                {
                    instance->flags2 &= ~0x10000000;
                }
            }

            if ((INSTANCE_Query(instance, 1) & 0xE))
            {
                INSTANCE_Post(instance, 0x40013, trueValue);
            }

            result = 1;
            break;
        case 32:
            switch (number)
            {
            case 1:
                INSTANCE_Post(instance, 0x40000, 0);
                break;
            case 2:
                INSTANCE_Post(instance, 0x40012, 0);
                break;
            case 3:
                INSTANCE_Post(instance, 0x40014, 0);
                break;
            case 4:
                INSTANCE_Post(instance, 0x40005, 0xA000);
            }

            break;
        case 54:
        {
            int x;
            int y;

            x = 0;

            if ((unsigned long)number < 4)
            {
                y = 0;

                switch (number)
                {
                case 0:
                    y = 1;
                    break;
                case 1:
                    x = 1;
                    break;
                case 2:
                    x = 0;
                    y = -1;
                    break;
                case 3:
                    x = -1;
                    break;
                }

                INSTANCE_Post(instance, 0x800000, SetObjectData(x, y, 6, NULL, 0));
            }

            break;
        }
        case 55:
        {
            Object *object;
            int i;

            object = instance->object;

            if (object->numberOfEffects != 0)
            {
                if (number == -1)
                {
                    for (i = 0; i < object->numberOfEffects; i++)
                    {
                        FX_StartInstanceEffect(instance, &object->effectList[i], 0);
                    }

                    break;
                }
                else if (number < object->numberOfEffects)
                {
                    FX_StartInstanceEffect(instance, &object->effectList[number], 0);
                }
            }

            break;
        }
        case 51:
            if (number == -1)
            {
                number = 0;
            }

            INSTANCE_Post(instance, 0x1000017, number);
            break;
        case 64:
            if (number != 0)
            {
                number = 1;
            }

            INSTANCE_Post(instance, 0x100001A, number);
            break;
        case 138:
        {
            Intro *intro;

            intro = instance->intro;

            if (intro != NULL)
            {
                if (trueValue != 0)
                {
                    intro->flags |= 0x400;
                }
                else
                {
                    intro->flags &= ~0x400;
                }
            }

            break;
        }
        case 4:
            if (number >= 256)
            {
                INSTANCE_Post(instance, 0x40017, number - 256);
            }
            else if ((number == 1) || (number == 2) || (number == 3) || (number == 4) || (number == 5) || (number == 6) || (number == 7))
            {
                ScriptKillInstance(instance, number);
            }
            else if (number == 0)
            {
                SAVE_UndestroyInstance(instance);
            }

            break;
        case 76:
        {
            Vector3d *vector3d;

            if ((operand2 != NULL) && (operand2->id == 9))
            {
                vector3d = &operand2->data.vector3d;

                INSTANCE_Post(instance, 0x4000A, SetPositionData(vector3d->vx, vector3d->vy, vector3d->vz));

                instance->currentStreamUnitID = vector3d->streamUnitID;
            }

            break;
        }
        case 77:
            if ((operand2 != NULL) && (operand2->id == 14))
            {
                INSTANCE_Post(instance, 0x4000B, SetPositionData(operand2->data.rotation3d.vx, operand2->data.rotation3d.vy, operand2->data.rotation3d.vz));
            }

            break;
        case 80:
            if ((operand2 != NULL) && (operand2->id == 14))
            {
                INSTANCE_Post(instance, 0x4000D, SetPositionData(operand2->data.rotation3d.vx, operand2->data.rotation3d.vy, operand2->data.rotation3d.vz));
            }

            break;
        case 133:
            INSTANCE_Post(instance, 0x40021, number);
            break;
        case 123:
            if ((operand2 != NULL) && (operand2->id == 9))
            {
                INSTANCE_Post(instance, 0x40018, SetPositionData(operand2->data.vector3d.vx, operand2->data.vector3d.vy, operand2->data.vector3d.vz));
            }

            break;
        case 75:
            if ((operand2 != NULL) && (operand2->id == 9))
            {
                INSTANCE_Post(instance, 0x40016, SetPositionData(operand2->data.vector3d.vx, operand2->data.vector3d.vy, operand2->data.vector3d.vz));
            }

            break;
        case 114:
            if ((operand2 != NULL) && (operand2->id == 9))
            {
                INSTANCE_Post(instance, 0x4000C, SetPositionData(operand2->data.vector3d.vx, operand2->data.vector3d.vy, operand2->data.vector3d.vz));
            }

            break;
        case 79:
            trueValue = trueValue == 0;
        case 78:
            if (instance == gameTrackerX.playerInstance)
            {
                INSTANCE_Post(instance, 0x4000E, trueValue);

                if (trueValue != 0)
                {
                    gameTrackerX.gameFlags |= 0x90;
                }
                else
                {
                    gameTrackerX.gameFlags &= ~0x80;
                    gameTrackerX.gameFlags &= ~0x10;
                }

                if (trueValue != 0)
                {
                    EventTimer *timer;

                    timer = EVENT_GetNextTimer();

                    if (timer != NULL)
                    {
                        timer->time = 4096;

                        timer->event = currentEventInstance;

                        timer->scriptPos = codeStream;

                        timer->actionScript = currentActionScript;

                        currentActionScript->conditionBits |= 0x1;

                        timer->level = CurrentPuzzleLevel;

                        timer->nextEventIndex = EventCurrentEventIndex;

                        EventAbortLine = 1;

                        EventJustRecievedTimer = 1;
                    }
                }
            }
            else
            {
                INSTANCE_Post(instance, 0x4000E, trueValue);
            }

            break;
        case 141:
            if (number >= 0)
            {
                INSTANCE_Broadcast(NULL, 10, 0x1000011, SetMonsterAlarmData(instance, NULL, number));
            }

            break;
        case 83:
            if ((operand2 != NULL) && (operand2->id == 9))
            {
                INSTANCE_Post(instance, 0x4000F, SetPositionData(operand2->data.vector3d.vx, operand2->data.vector3d.vy, operand2->data.vector3d.vz));
            }

            break;
        case 84:
            if (trueValue != 0)
            {
                INSTANCE_Post(instance, 0x40010, 0);
            }

            break;
        case 85:
            INSTANCE_Post(instance, 0x800027, trueValue);
            break;
        case 91:
            if (number >= 0)
            {
                instance->lightGroup = number;
            }

            break;
        case 92:
            if (number >= 0)
            {
                instance->spectralLightGroup = number;
            }

            break;
        case 95:
            trueValue = 0;
        case 94:
            INSTANCE_Post(instance, 0x800029, trueValue);
            break;
        case 103:
            if (trueValue != 0)
            {
                instance->flags &= ~0x400000;
            }
            else
            {
                instance->flags |= 0x400000;
            }

            break;
        case 140:
            INSTANCE_Post(instance, 0x40022, number);
            break;
        case 108:
            number %= 360;

            if (number < 0)
            {
                number += 360;
            }

            number = (number << 12) / 360;

            INSTANCE_Post(instance, 0x4000005, rcos(number));
            break;
        case 109:
            INSTANCE_Post(instance, 0x4000006, 0);
            break;
        case 111:
            INSTANCE_Post(instance, 0x40015, 1 << number);
            break;
        case 110:
            INSTANCE_Post(instance, 0x40011, 0);
            break;
        case 116:
            INSTANCE_Post(instance, 0x40006, number << 12);
            break;
        case 124:
            number <<= 12;

            INSTANCE_Post(instance, 0x40019, number);
        case 163:
            INSTANCE_Post(instance, 0x1000022, number);
            break;
        case 164:
            INSTANCE_Post(instance, 0x1000022, 0);
            break;
        case 165:
            if (instance == gameTrackerX.playerInstance)
            {
                if (trueValue != 0)
                {
                    gameTrackerX.gameFlags |= 0x90;
                }
                else
                {
                    gameTrackerX.gameFlags &= ~0x80;
                    gameTrackerX.gameFlags &= ~0x10;
                }
            }
        }
    }

    return result;
}

long EVENT_GetTGroupValue(TGroupObject *terrainGroup)
{
    long value;
    long trueValue;
    BSPTree *bspTree;

    value = 0;

    trueValue = 1;

    bspTree = terrainGroup->bspTree;

    switch (terrainGroup->attribute)
    {
    case -1:
        value = -((terrainGroup->streamUnit->StreamUnitID << 8) + bspTree->ID);
        break;
    case 52:
        if ((bspTree->flags & 0x1))
        {
            value = 1;
        }
        else
        {
            value = 0;
        }

        break;
    case 53:
        if ((bspTree->flags & 0x2))
        {
            value = 1;
        }
        else
        {
            value = 0;
        }

        break;
    case 11:
        trueValue = 0;
    case 10:
        value = trueValue;

        if ((bspTree->flags & 0x3) != 0x3)
        {
            value ^= 1;
        }

        break;
    case 60:
        if ((bspTree->flags & 0x800))
        {
            value = 1;

            bspTree->flags &= ~0x800;
        }

        break;
    case 106:
        if ((bspTree->flags & 0x400))
        {
            value = 1;

            bspTree->flags &= ~0x400;
        }

        break;
    case 125:
        break;
    case 115:
        if ((bspTree->flags & 0x40))
        {
            value = 1;
        }
        else
        {
            value = 0;
        }

        break;
    case 149:
        if ((bspTree->flags & 0x20))
        {
            value = 1;
        }
        else
        {
            value = 0;
        }

        break;
    }

    return value;
}

long EVENT_DoTGroupAction(TGroupObject *terrainGroup, StackType *operand2)
{
    long trueValue;
    long number;
    long error;
    long result;

    number = -1;

    trueValue = 1;

    result = 0;

    if (terrainGroup->attribute != number)
    {
        BSPTree *bspTree;

        bspTree = terrainGroup->bspTree;

        EVENT_ParseOperand2(operand2, &error, &trueValue);

        switch (terrainGroup->attribute)
        {
        case 46:
        case 44:
        {
            WaterLevelProcess *curWater;

            curWater = EVENT_GetNextTerrainMove();

            if (curWater != NULL)
            {
                curWater->bspTree = bspTree;

                curWater->streamUnit = terrainGroup->streamUnit;

                curWater->oldWaterZ = terrainGroup->streamUnit->level->waterZLevel;
                curWater->oldGlobalOffset = bspTree->globalOffset.z;

                curWater->zValueFrom = bspTree->localOffset.z;
                curWater->zValueTo = terrainGroup->arg[0];

                curWater->maxSteps = terrainGroup->arg[1] << 12;

                curWater->curStep = 0;

                curWater->flags = 0x1;

                WaterInUse = 1;

                if (terrainGroup->attribute == 46)
                {
                    curWater->flags |= 0x2;
                }
            }

            break;
        }
        case 45:
        case 47:
        {
            long offsetz;

            if (bspTree->localOffset.z != terrainGroup->arg[0])
            {
                offsetz = terrainGroup->arg[0] - bspTree->localOffset.z;

                bspTree->globalOffset.z += offsetz;
                bspTree->localOffset.z = terrainGroup->arg[0];

                result = 1;

                if (terrainGroup->attribute == 47)
                {
                    terrainGroup->streamUnit->level->waterZLevel += offsetz;

                    terrainGroup->streamUnit->level->terrain->UnitChangeFlags |= 0x3;
                }
            }

            result = 1;
            break;
        }
        case 115:
            if (trueValue != 0)
            {
                bspTree->flags |= 0x40;
            }
            else
            {
                bspTree->flags &= ~0x40;
            }

            result = 1;
            break;
        case 149:
            if (trueValue != 0)
            {
                bspTree->flags |= 0x20;
            }
            else
            {
                bspTree->flags &= ~0x20;
            }

            result = 1;
            break;
        case 52:
            if (trueValue != 0)
            {
                bspTree->flags |= 0x1;
            }
            else
            {
                bspTree->flags &= ~0x1;
            }

            result = 1;
            break;
        case 53:
            if (trueValue != 0)
            {
                bspTree->flags |= 0x2;
            }
            else
            {
                bspTree->flags &= ~0x2;
            }

            result = 1;
            break;
        case 11:
            trueValue ^= 1;
        case 10:
            if (trueValue != 0)
            {
                bspTree->flags |= 0x3;
            }
            else
            {
                bspTree->flags &= ~0x3;
            }

            result = 1;
            break;
        }
    }

    return result;
}

long EVENT_DoCameraAction(CameraObject *cameraObject, StackType *operand2, short *codeStream)
{
    long trueValue;
    long number;
    long error;
    Camera *camera;

    (void)codeStream;

    trueValue = 1;

    error = 1;

    number = -1;

    camera = cameraObject->camera;

    if (cameraObject->attribute != number)
    {
        number = EVENT_ParseOperand2(operand2, &error, &trueValue);

        switch (cameraObject->attribute)
        {
        case 67:
            if (error == 0)
            {
                CAMERA_SetSmoothValue(camera, number);
            }

            break;
        case 68:
        case 69:
        {
            Intro *intro;

            intro = NULL;

            if (operand2 != NULL)
            {
                switch (operand2->id)
                {
                case 2:
                    intro = operand2->data.instanceObject.instance->intro;
                    break;
                case 4:
                    intro = operand2->data.introObject.intro;
                    break;
                }
            }

            if (intro != NULL)
            {
                if (cameraObject->attribute == 68)
                {
                    camera->Spline00 = intro->multiSpline;
                }
                else
                {
                    camera->Spline01 = intro->multiSpline;
                }
            }

            break;
        }
        case 9:
        case 70:
        {
            long angle;

            if (error == 0)
            {
                if (cameraObject->attribute == 70)
                {
                    angle = -number;

                    angle %= 360;

                    if (angle < 0)
                    {
                        angle += 360;
                    }

                    CAMERA_Adjust_tilt(cameraObject->camera, (angle * 4096) / 360);
                }
                else
                {
                    angle = -number;

                    angle %= 360;

                    if (angle < 0)
                    {
                        angle += 360;
                    }

                    CAMERA_Adjust_rotation(cameraObject->camera, (angle * 4096) / 360);
                }
            }

            break;
        }
        case 105:
        {
            long angle;

            if (error == 0)
            {
                angle = number % 360;

                if (angle < 0)
                {
                    angle += 360;
                }

                CAMERA_Adjust_roll((angle * 4096) / 360, cameraObject->frames);
            }

            break;
        }
        case 104:
        {
            long angle;

            if (error == 0)
            {
                angle = number % 360;

                if (angle < 0)
                {
                    angle += 360;
                }

                CAMERA_Adjust_roll((angle * 4096) / 360, 0);
            }

            break;
        }
        case 71:
            if (error == 0)
            {
                CAMERA_Adjust_distance(cameraObject->camera, number);
            }

            break;
        case 72:
            CAMERA_RestoreMode(camera);
            break;
        case 73:
            if ((operand2 != NULL) && (operand2->id == 2))
            {
                CAMERA_SetInstanceFocus(camera, operand2->data.instanceObject.instance);
            }

            break;
        case 16:
            if (error == 0)
            {
                CAMERA_SetMode(camera, number);
            }

            break;
        }
    }

    return 0;
}

long EVENT_DoObjectSoundAction(SoundObject *soundObject, StackType *operand2)
{
    long trueValue;
    long number;
    long error;
    long result;
    Instance *instance;

    trueValue = 1;

    result = 1;

    number = -1;

    instance = soundObject->data.instance;

    if (soundObject->attribute != number)
    {
        SoundInstance *soundInstance;

        soundInstance = &instance->soundInstanceTbl[soundObject->soundNumber];

        EVENT_ParseOperand2(operand2, &error, &trueValue);

        switch (soundObject->attribute)
        {
        case 14:
            trueValue = trueValue == 0;
        case 13:
            if (trueValue != 0)
            {
                SOUND_StartInstanceSound(soundInstance);
            }
            else
            {
                SOUND_StopInstanceSound(soundInstance);
            }

            break;
        case 127:
            SOUND_SetInstanceSoundVolume(soundInstance, soundObject->value, soundObject->duration);
            break;
        case 128:
            SOUND_SetInstanceSoundPitch(soundInstance, soundObject->value, soundObject->duration);
            break;
        case 129:
        case 130:
        case 131:
            break;
        default:
            result = 0;
        }
    }

    return result;
}

long EVENT_DoSoundMarkerAction(SoundObject *soundObject, StackType *operand2)
{
    long trueValue;
    long number;
    long error;
    long result;
    SFXMkr *sfxMarker;

    trueValue = 1;

    result = 1;

    number = -1;

    sfxMarker = soundObject->data.sfxMarker;

    if (soundObject->attribute != number)
    {
        SoundInstance *soundInstance;

        soundInstance = &sfxMarker->sfxTbl[soundObject->soundNumber];

        EVENT_ParseOperand2(operand2, &error, &trueValue);

        switch (soundObject->attribute)
        {
        case 14:
            trueValue = trueValue == 0;
        case 13:
            if (trueValue != 0)
            {
                SOUND_StartInstanceSound(soundInstance);
            }
            else
            {
                SOUND_StopInstanceSound(soundInstance);
            }

            break;
        case 127:
            SOUND_SetInstanceSoundVolume(soundInstance, soundObject->value, soundObject->duration);
            break;
        case 128:
            SOUND_SetInstanceSoundPitch(soundInstance, soundObject->value, soundObject->duration);
            break;
        case 129:
        case 130:
        case 131:
            break;
        default:
            result = 0;
        }
    }

    return result;
}

long EVENT_GetSoundValue(SoundObject *soundObject)
{
    (void)soundObject;

    return 0;
}

long EVENT_DoAreaAction(AreaObject *areaObject, StackType *operand2)
{
    long trueValue;
    long number;
    long error;
    StreamUnit *streamUnit;

    number = -1;

    trueValue = 1;

    streamUnit = areaObject->streamUnit;

    if (areaObject->attribute != number)
    {
        number = EVENT_ParseOperand2(operand2, &error, &trueValue);

        if (areaObject->attribute != 112)
        {
            if ((areaObject->attribute == 113) && (error == 0))
            {
                streamUnit->level->fogNear = streamUnit->TargetFogNear = number;
            }
        }
        else if (error == 0)
        {
            streamUnit->level->fogFar = streamUnit->TargetFogFar = number;
        }
    }

    return 0;
}

long EVENT_DoIntroAction(IntroObject *introObject, StackType *operand2)
{
    long trueValue;
    long number;
    long error;
    long result;
    StreamUnit *temp; // not from decls.h

    number = -1;

    result = 0;

    trueValue = 1;

    if (introObject->attribute != number)
    {
        Intro *intro;

        intro = introObject->intro;

        number = EVENT_ParseOperand2(operand2, &error, &trueValue);

        switch (introObject->attribute)
        {
        case 20:
            trueValue ^= 1;
            break;
        case 11:
            trueValue ^= 1;
        case 10:
            if (trueValue != 0)
            {
                intro->flags |= 0x80;
                intro->flags &= ~0x8;
            }
            else
            {
                intro->flags &= ~0x80;
            }

            result = 1;
            break;
        case 138:
            if (trueValue != 0)
            {
                intro->flags |= 0x400;
            }
            else
            {
                intro->flags &= ~0x400;
            }

            break;
        case 4:
            result = 0;

            if (number == 0)
            {
                SAVE_SetDeadDeadBit(intro->UniqueID, 0);
            }

            break;
        case 61:
            break;
        case 62:
        {
            int i;
            int j;
            int id;

            id = 0;

            for (i = 0; i < 16; i++)
            {
                temp = &StreamTracker.StreamList[i];

                if (temp->used == 2)
                {
                    Intro *intro1;

                    intro1 = temp->level->introList;

                    for (j = temp->level->numIntros; j != 0; j--, intro1++)
                    {
                        if (intro1 == intro)
                        {
                            id = temp->StreamUnitID;
                            break;
                        }
                    }
                }
            }

            if (INSTANCE_IntroduceInstance(intro, id) == NULL)
            {
                EventAbortLine = 1;
            }

            break;
        }
        }
    }

    return result;
}

void EVENT_DoStackOperationEquals(PCodeStack *stack, short *codeStream)
{
    StackType operand1;
    StackType operand2;

    stack->topOfStack--;

    operand2 = stack->stack[stack->topOfStack];

    stack->topOfStack--;

    operand1 = stack->stack[stack->topOfStack];

    EVENT_ExecuteActionCommand(&operand1, &operand2, stack, codeStream);
}

long EVENT_CompareVector3d(Vector3d *vector1, Vector3d *vector2)
{
    long retValue;

    retValue = 0;

    if (vector1->streamUnitID == vector2->streamUnitID)
    {
        if ((vector1->errory != -1) && (vector1->errorz != -1))
        {
            if ((vector2->vx >= (vector1->vx - vector1->errorx)) && (vector2->vx <= (vector1->vx + vector1->errorx)) && (vector2->vy >= (vector1->vy - vector1->errory)) && (vector2->vy <= (vector1->vy + vector1->errory)) && (vector2->vz >= (vector1->vz - vector1->errorz)))
            {
                long dist;

                dist = vector1->vz + vector1->errorz;

                retValue = dist >= vector2->vz;
            }
        }
        else if ((vector1->errory != -1) && (vector1->errory != vector1->errorz))
        {
            if ((POW2(vector2->vx - vector1->vx) + POW2(vector2->vy - vector1->vy)) < POW2(vector1->errorx))
            {
                long dist;

                dist = vector2->vz - vector1->vz;

                if (dist >= 0)
                {
                    if (dist < vector1->errory)
                    {
                        goto label;
                    }
                }
                else if ((vector1->vz - vector2->vz) < vector1->errory)
                {
                    retValue = 1;
                }
            }
        }
        else if ((POW2(vector2->vx - vector1->vx) + POW2(vector2->vy - vector1->vy) + POW2(vector2->vz - vector1->vz)) < POW2(vector1->errorx))
        {
        label:
            retValue = 1;
        }
    }

    return retValue;
}

long EVENT_CompareRotationVectors(Rotation3d *rot1, Rotation3d *rot2, long trueValue)
{
    MATRIX matrix1;
    MATRIX matrix2;
    long result;
    long c1;
    long attribute;
    SVector axis;
    long doSoft;

    result = trueValue == 0;

    attribute = rot1->attribute;

    doSoft = 0;

    if (attribute == -1)
    {
        attribute = rot2->attribute;
    }

    RotMatrixZYX((SVECTOR *)rot1, &matrix1);
    RotMatrixZYX((SVECTOR *)rot2, &matrix2);

    switch (attribute)
    {
    case -1:
        axis.x = 4096;
        axis.y = 0;
        axis.z = 0;

        if (((unsigned int)PHYSOBS_CheckObjectAxisAlignment(&matrix1, &matrix2, &axis) - 3968) < 257)
        {
            axis.x = 0;
            axis.y = 4096;
            axis.z = 0;

            if (((unsigned int)PHYSOBS_CheckObjectAxisAlignment(&matrix1, &matrix2, &axis) - 3968) < 257)
            {
                result = trueValue;
            }
        }

        break;
    case 96:
        doSoft = 1;
    case 6:
        axis.x = 4096;
        axis.y = 0;
        axis.z = 0;
        break;
    case 97:
        doSoft = 1;
    case 7:
        axis.x = 0;
        axis.y = 4096;
        axis.z = 0;
        break;
    case 98:
        doSoft = 1;
    case 8:
        axis.x = 0;
        axis.y = 0;
        axis.z = 4096;
        break;
    }

    if (attribute != -1)
    {
        c1 = PHYSOBS_CheckObjectAxisAlignment(&matrix1, &matrix2, &axis);

        if (doSoft != 0)
        {
            if (((unsigned long)(c1 - 3968) <= 256) || ((unsigned long)(c1 + 4224) <= 256))
            {
                result = trueValue;
            }
            else
            {
                result = trueValue == 0;
            }
        }
        else if ((unsigned long)(c1 - 3968) <= 256)
        {
            result = trueValue;
        }
        else
        {
            result = trueValue == 0;
        }
    }

    return result;
}

long EVENT_CompareListWithOperation(PCodeStack *stack, ListObject *listObject, StackType *operand2, long operation)
{
    long retValue;
    long d;
    StackType operand1;
    char objectName[16];
    char *temp;
    Instance *instance;
    long areaID;
    int temp2; // not from decls.h

    retValue = 0;

    if (CurrentEventLine < 20)
    {
        eventListNumInstances[CurrentEventLine] = 0;
    }

    strcpy((char *)&objectName, listObject->eventInstance->objectName);

    temp = (char *)strchr((char *)&objectName, 63);

    if (temp != NULL)
    {
        *temp = 0;
    }

    areaID = listObject->eventInstance->unitID;

    if (areaID == 63)
    {
        areaID = 0;
    }

    instance = NULL;

    while ((instance = INSTANCE_FindWithName(areaID, (char *)&objectName, instance)) != NULL)
    {
        operand1.id = 2;

        operand1.data.listObject.eventInstance = (EventWildCardObject *)instance;

        operand1.data.listObject.attribute[0] = -1;

        for (d = 0; d < listObject->numberOfAttributes; d++)
        {
            EVENT_TransformOperand(&operand1, stack, listObject->attribute[d], NULL);
        }

        if (EVENT_CompareOperandsWithOperation(stack, &operand1, operand2, operation) != 0)
        {
            if (CurrentEventLine < 20)
            {
                temp2 = eventListNumInstances[CurrentEventLine];

                if (temp2 < 10)
                {
                    eventListArray[CurrentEventLine][temp2] = instance;

                    eventListNumInstances[CurrentEventLine] = temp2 + 1;
                }
            }

            retValue = 1;
        }
        else if (operation == 11)
        {
            if (eventListNumInstances[CurrentEventLine] < 10)
            {
                eventListNumInstances[CurrentEventLine] = 0;
            }

            retValue = 0;
            break;
        }

        if (instance == NULL)
        {
            break;
        }
    }

    return retValue;
}

long EVENT_CompareSubListWithOperation(PCodeStack *stack, SubListObject *subListObject, StackType *operand2, long operation)
{
    long i;
    long retValue;
    long d;
    StackType operand1;
    int temp; // not from decls.h

    retValue = 0;

    if (CurrentEventLine < 20)
    {
        eventListNumInstances[CurrentEventLine] = 0;
    }

    for (i = 0; i < subListObject->numberOfInstances; i++)
    {
        operand1.id = 2;

        operand1.data.subListObject.instanceList = (Instance **)subListObject->instanceList[i];

        operand1.data.subListObject.numberOfInstances = -1;

        for (d = 0; d < subListObject->numberOfAttributes; d++)
        {
            EVENT_TransformOperand(&operand1, stack, subListObject->attribute[d], NULL);
        }

        if (EVENT_CompareOperandsWithOperation(stack, &operand1, operand2, operation) != 0)
        {
            if (CurrentEventLine < 20)
            {
                temp = eventListNumInstances[CurrentEventLine];

                if (temp < 10)
                {
                    eventListArray[CurrentEventLine][temp] = subListObject->instanceList[i];

                    eventListNumInstances[CurrentEventLine] = temp + 1;
                }
            }

            retValue = 1;
        }
    }

    return retValue;
}

long EVENT_CompareOperandsWithOperation(PCodeStack *stack, StackType *operand1, StackType *operand2, long operation)
{
    long number;
    long number1;
    long number2;
    long error;
    long trueValue;
    short flags1;
    short flags2;
    int temp; // not from decls.h

    number = 0;

    trueValue = 1;

    switch (operand1->id)
    {
    case 8:
        break;
    case 9:
        if (operand2->id != 9)
        {
            break;
        }

        switch (operation)
        {
        case 1:
            EVENT_Addvector3dToStack(stack, operand1->data.vector3d.vx + operand2->data.vector3d.vx, operand1->data.vector3d.vy + operand2->data.vector3d.vy, operand1->data.vector3d.vz + operand2->data.vector3d.vz, 0);

            number = -1;
            break;
        case 2:
            EVENT_Addvector3dToStack(stack, operand1->data.vector3d.vx - operand2->data.vector3d.vx, operand1->data.vector3d.vy - operand2->data.vector3d.vy, operand1->data.vector3d.vz - operand2->data.vector3d.vz, 0);

            number = -1;
            break;
        case 11:
            trueValue = 0;
        case 10:
            temp = EVENT_CompareVector3d(&operand1->data.vector3d, &operand2->data.vector3d);

            number = trueValue;

            if (temp == 0)
            {
                number ^= 1;
            }

            break;
        }

        break;
    case 14:
        if (operand2->id != 14)
        {
            break;
        }

        switch (operation)
        {
        case 11:
            trueValue = 0;
        case 10:
            number = EVENT_CompareRotationVectors(&operand1->data.rotation3d, &operand2->data.rotation3d, trueValue);
            break;
        }

        break;
    case 22:
        temp = EVENT_CompareSubListWithOperation(stack, &operand1->data.subListObject, operand2, operation);

        number = trueValue;

        if (temp == 0)
        {
            number ^= 1;
        }

        break;
    case 18:
        temp = EVENT_CompareListWithOperation(stack, &operand1->data.listObject, operand2, operation);

        number = trueValue;

        if (temp == 0)
        {
            number ^= 1;
        }

        break;
    default:
        number1 = EVENT_GetScalerValueFromOperand(operand1, &error, &flags1);
        number2 = EVENT_GetScalerValueFromOperand(operand2, &error, &flags2);

        switch (operation)
        {
        case 1:
            number = number1 + number2;
            break;
        case 2:
            number = number1 - number2;
            break;
        case 3:
            number = number1 * number2;
            break;
        case 4:
            number = 0;

            if (number2 != 0)
            {
                number = number1 / number2;
            }

            break;
        case 5:
            number = number1 % number2;

            if (number < 0)
            {
                number += number2;
            }

            break;
        case 9:
            trueValue = 0;
        case 6:
            number = trueValue;

            temp = number1 > number2;

            if (temp == 0)
            {
                number ^= 1;
            }

            break;
        case 8:
            trueValue = 0;
        case 7:
            number = trueValue;

            temp = number1 < number2;

            if (temp == 0)
            {
                number ^= 1;
            }

            break;
        case 11:
            trueValue = 0;
        case 10:
            if ((!(flags1 & 0x1)) && (!(flags2 & 0x1)))
            {
                number = trueValue;

                if (number1 != number2)
                {
                    number ^= 1;
                }
            }
            else
            {
                if ((flags1 & 0x2))
                {
                    number2 = 1 << number2;
                }

                number = trueValue;

                if ((number1 & number2) != number2)
                {
                    number ^= 1;
                }
            }

            break;
        case 12:
            number = (number1 & number2) != 0;
            break;
        }

        break;
    }

    return number;
}

void EVENT_DoStackMathOperation(PCodeStack *stack, long operation)
{
    StackType operand1;
    StackType operand2;

    stack->topOfStack--;

    operand2 = stack->stack[stack->topOfStack];

    stack->topOfStack--;

    operand1 = stack->stack[stack->topOfStack];

    if ((operand2.id == 18) || (operand2.id == 22))
    {
        StackType holdOperand;

        holdOperand = operand1;

        operand1 = operand2;
        operand2 = holdOperand;
    }

    EVENT_AddNumberToStack(stack, EVENT_CompareOperandsWithOperation(stack, &operand1, &operand2, operation), 0);
}

short *EVENT_ParseOpcode(PCodeStack *stack, short *codeStream, long *operateOnStack)
{
    short pcode;

    pcode = *codeStream++ - 1;

    MoveCodeStreamExtra = 0;

    *operateOnStack = 0;

    switch (pcode)
    {
    case 0:
        EVENT_AddObjectToStack(stack, *codeStream);

        codeStream++;
        break;
    case 11:
        EVENT_AddNumberToStack(stack, *codeStream, 0);

        codeStream++;
        break;
    case 1:
        EVENT_ModifyObjectToStackWithAttribute(stack, *codeStream, codeStream);

        codeStream++;
        break;
    case 3:
        EVENT_DoStackMathOperation(stack, 10);
        break;
    case 22:
        EVENT_DoStackMathOperation(stack, 11);
        break;
    case 4:
        EventAbortedPosition = codeStream;

        *operateOnStack = 1;
        break;
    case 31:
        EVENT_DoStackMathOperation(stack, 12);
        break;
    case 12:
        EVENT_DoStackMathOperation(stack, 1);
        break;
    case 13:
        EVENT_DoStackMathOperation(stack, 2);
        break;
    case 14:
        EVENT_DoStackMathOperation(stack, 3);
        break;
    case 15:
        EVENT_DoStackMathOperation(stack, 4);
        break;
    case 23:
        EVENT_DoStackMathOperation(stack, 5);
        break;
    case 16:
        EVENT_StackDuplicate(stack);

        EVENT_AddNumberToStack(stack, 1, 0);

        EVENT_DoStackMathOperation(stack, 1);
        EVENT_DoStackOperationEquals(stack, codeStream);
        break;
    case 17:
        EVENT_StackDuplicate(stack);

        EVENT_AddNumberToStack(stack, 1, 0);

        EVENT_DoStackMathOperation(stack, 2);
        EVENT_DoStackOperationEquals(stack, codeStream);
        break;
    case 2:
        EVENT_DoStackOperationEquals(stack, codeStream);
        break;
    case 18:
        EVENT_DoStackMathOperation(stack, 6);
        break;
    case 19:
        EVENT_DoStackMathOperation(stack, 8);
        break;
    case 20:
        EVENT_DoStackMathOperation(stack, 7);
        break;
    case 21:
        EVENT_DoStackMathOperation(stack, 9);
        break;
    case 6:
    case 8:
    case 9:
        codeStream = NULL;
        break;
    case 27:
        EVENT_AddGameObjectToStack(stack);
        break;
    case 28:
        EVENT_AddPlayerObjectToStack(stack);
        break;
    case 29:
        EVENT_AddSubListObjectToStack(stack, *codeStream);

        codeStream++;
        break;
    case 30:
        if (currentActionScript != NULL)
        {
            currentActionScript->conditionBits |= 0x2;
        }
    }

    return &codeStream[MoveCodeStreamExtra];
}

long EVENT_GetIntroValue(IntroObject *introObject)
{
    long value;
    long trueValue;

    value = 0;

    trueValue = 1;

    switch (introObject->attribute)
    {
    case -1:
        value = 1;
        break;
    case 20:
    case 21:
        value = -1;
        break;
    case 11:
        trueValue ^= 1;
    case 10:
        value = trueValue;

        if (!(introObject->intro->flags & 0x80))
        {
            value ^= 1;
        }

        break;
    case 146:
    case 166:
        value = trueValue;

        if (!(introObject->intro->flags & 0x8))
        {
            value ^= 1;
        }

        break;
    case 5:
    case 9:
    case 12:
    case 16:
    case 36:
    case 37:
    case 43:
    case 126:
    case 143:
    case 144:
    case 160:
        value = 0;
    }

    return value;
}

void EVENT_DoSubListAction(SubListObject *subListObject, StackType *operand2, PCodeStack *stack, short *codeStream)
{
    long i;
    long d;
    StackType operand1;

    for (i = 0; i < subListObject->numberOfInstances; i++)
    {
        operand1.id = 2;

        operand1.data.subListObject.instanceList = (Instance **)subListObject->instanceList[i];

        operand1.data.subListObject.numberOfInstances = -1;

        for (d = 0; d < subListObject->numberOfAttributes; d++)
        {
            EVENT_TransformOperand(&operand1, stack, subListObject->attribute[d], 0);
        }

        EVENT_ExecuteActionCommand(&operand1, operand2, stack, codeStream);
    }
}

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_GetInstanceValue);

long EVENT_GetSplineFrameNumber(InstanceSpline *instanceSpline)
{
    return SCRIPT_GetSplineFrameNumber(instanceSpline->instance, SCRIPT_GetPosSplineDef(instanceSpline->instance, instanceSpline->spline, instanceSpline->isParent, instanceSpline->isClass));
}

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

VMObject *EVENT_FindVMObject(StreamUnit *stream, char *vmoName)
{
    int i;
    VMObject *vmObject;
    Level *level;

    level = stream->level;

    vmObject = NULL;

    for (i = 0; i < level->numVMObjects; i++)
    {
        if (strcmpi(level->vmobjectList[i].name, vmoName) == 0)
        {
            vmObject = &level->vmobjectList[i];
            break;
        }
    }

    return vmObject;
}

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_FixPuzzlesForStream);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_AddStreamToInstanceList);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_RemoveStreamToInstanceList);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_RelocateInstanceList);

void EVENT_PrintVars()
{
}

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_ExecuteActionCommand);
/* TODO: Gives "small-data section too large" issue when compiling
void EVENT_ExecuteActionCommand(StackType *operand1, StackType *operand2, PCodeStack *stack, short *codeStream)
{
    long error;
    long value;
    short flags;

    switch (operand1->id)
    {
    case 27:
        if (operand1->data.soundObject.flags == 1)
        {
            EVENT_DoSoundMarkerAction(&operand1->data.soundObject, operand2);
        }
        else
        {
            EVENT_DoObjectSoundAction(&operand1->data.soundObject, operand2);
        }

        break;
    case 1:
        EVENT_DoAreaAction(&operand1->data.areaObject, operand2);
        break;
    case 4:
        EVENT_DoIntroAction(&operand1->data.introObject, operand2);
        break;
    case 10:
        value = EVENT_GetScalerValueFromOperand(operand2, &error, &flags);

        if (error == 0)
        {
            *operand1->data.ShortPointer.pointer = value;
        }

        break;
    case 28:
        value = EVENT_GetScalerValueFromOperand(operand2, &error, &flags);

        if (error == 0)
        {
            *operand1->data.CharPointer.pointer = value;
        }

        break;
    case 11:
        value = EVENT_GetScalerValueFromOperand(operand2, &error, &flags);

        if (error == 0)
        {
            *operand1->data.LongPointer.pointer = value;
        }

        break;
    case 15:
        EVENT_DoSplineAction(&operand1->data.instanceSpline, operand2);
        break;
    case 2:
        EVENT_DoInstanceAction(&operand1->data.instanceObject, operand2, codeStream);
        break;
    case 17:
        EVENT_DoSignalAction(&operand1->data.signalObject, operand2);
        break;
    case 19:
        EVENT_DoAnimateAction(&operand1->data.instanceAnimate, operand2);
        break;
    case 20:
        EVENT_DoInstanceAnimateTextureAction(&operand1->data.instanceAniTexture, operand2);
        break;
    case 23:
        EVENT_DoTGroupAction(&operand1->data.terrainGroup, operand2);
        break;
    case 25:
        EVENT_DoCameraAction(&operand1->data.cameraObject, operand2, codeStream);
        break;
    case 26:
        EVENT_DoVMObjectAction(&operand1->data.vmObject, operand2);
        break;
    case 3:
        EVENT_DoGameAction(&operand1->data.gameObject, operand2);
        break;
    case 22:
        EVENT_DoSubListAction(&operand1->data.subListObject, operand2, stack, codeStream);
        break;
    }
}*/

// Matches 100% on decomp.me but differs on this project
#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_GetScalerValueFromOperand);
#else
long EVENT_GetScalerValueFromOperand(StackType *operand, long *error, short *flags)
{
    long value;

    *error = 0;
    *flags = 0;

    value = 32767;

    switch (operand->id)
    {
    case 2:
        value = EVENT_GetInstanceValue(&operand->data.instanceObject);
        break;
    case 21:
        value = 0;
        break;
    case 27:
        if (operand->data.soundObject.flags == 1)
        {
            value = 0;
        }
        else
        {
            value = EVENT_GetSoundValue(&operand->data.soundObject);
        }

        break;
    case 10:
        value = *operand->data.ShortPointer.pointer;
        break;
    case 28:
        value = *operand->data.CharPointer.pointer;
        break;
    case 11:
        value = *operand->data.LongPointer.pointer;
        break;
    case 15:
        value = EVENT_GetSplineValue(&operand->data.instanceSpline);
        break;
    case 7:
        value = operand->data.Number.currentNumber;

        *flags = operand->data.Number.flags;
        break;
    case 4:
        value = EVENT_GetIntroValue(&operand->data.introObject);
        break;
    case 19:
        value = EVENT_GetAnimateValue(&operand->data.instanceAnimate);
        break;
    case 20:
        value = EVENT_GetAnimateTextureValue(&operand->data.instanceAniTexture);
        break;
    case 23:
        value = EVENT_GetTGroupValue(&operand->data.terrainGroup);
        break;
    case 24:
        value = operand->data.constrictInfo.instance->constrictAngle;

        if ((value < -1) || (value > 1))
        {
            value = 1;
        }
        else
        {
            value = 0;
        }

        break;
    case 26:
        value = EVENT_GetVMObjectValue(&operand->data.vmObject);
        break;
    case 3:
        value = EVENT_GetGameValue(&operand->data.gameObject);
        break;
    case 1:
        value = operand->data.areaObject.unitID;
        break;
    }

    return value;
}
#endif

long EVENT_TransformOperand(StackType *stackObject, PCodeStack *stack, long item, short *codeStream)
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
