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
#include "Game/SOUND.h"
#include "Game/PSX/MAIN.h"
#include "Game/CAMERA.h"
#include "Game/LOAD3D.h"
#include "Game/GAMEPAD.h"
#include "Game/SCRIPT.h"

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

long eventListNumInstances[20];

Instance *eventListArray[20][10]; // order of indexes from decls.h has been reversed

STATIC long MoveCodeStreamExtra;

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

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_TransformGameAttribute);

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
