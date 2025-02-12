#include "common.h"
#include "Game/EVENT.h"
#include "Game/INSTANCE.h"
#include "Game/STREAM.h"

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

void EVENT_UpdateResetSignalArrayAndWaterMovement(struct Level *oldLevel, struct Level *newLevel, long sizeOfLevel)
{
    long offset;
    long i;
    struct WaterLevelProcess *curWater;

    offset = (int)newLevel - (int)oldLevel;

    if (NumSignalsToReset != 0)
    {
        for (i = 0; i < 16; i++)
        {
            if (ResetSignalArray[i].timeLeft > 0)
            {
                if (IN_BOUNDS(ResetSignalArray[i].mSignal, oldLevel, (int)oldLevel + sizeOfLevel))
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
                if (IN_BOUNDS(curWater->bspTree, oldLevel, (int)oldLevel + sizeOfLevel))
                {
                    curWater->bspTree = (struct BSPTree *)OFFSET_DATA(curWater->bspTree, offset);
                }
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_ResetAllOneTimeVariables);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_AddSignalToReset);

void HINT_ResetHint()
{
    memset(&gHintSystem, 0, sizeof(HintSystemStruct));
}

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", HINT_StartHint);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", HINT_StopHint);

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

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_ProcessTimers);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_ProcessHints);

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

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_ProcessMovingWater);

void EVENT_DoProcess()
{
    EVENT_ProcessMovingWater();
    EVENT_ProcessTimers();
}

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_BSPProcess);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_Process);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_ProcessPuppetShow);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_ProcessEvents);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_DoAction);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_IsConditionTrue);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_WriteEventObject);

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_ResolveObjectSignal);

Intro *EVENT_ResolveObjectIntro(EventInstanceObject *instanceObject)
{
    return INSTANCE_FindIntro(instanceObject->unitID, instanceObject->introUniqueID);
}

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_ResolveSFXMarker);

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

INCLUDE_ASM("asm/nonmatchings/Game/EVENT", EVENT_TransformOperand);
