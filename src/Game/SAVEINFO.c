#include "Game/SAVEINFO.h"
#include "Game/MCARD/MEMCARD.h"
#include "Game/MEMPACK.h"
#include "Game/STRMLOAD.h"
#include "Game/DEBUG.h"
#include "Game/GAMEPAD.h"
#include "Game/SCRIPT.h"
#include "Game/GAMELOOP.h"
#include "Game/STREAM.h"
#include "Game/SOUND.h"
#include "Game/INSTANCE.h"
#include "Game/EVENT.h"
#include "Game/MATH3D.h"

GlobalSaveTracker *GlobalSave = NULL;

static long numbufferedIntros = 0;

static int the_header_size = 0;

long SaveArraySize[10] = {
    0,
    sizeof(SavedIntro),
    sizeof(SavedInfoTracker),
    sizeof(SavedLevel),
    sizeof(SavedDeadDeadBits),
    sizeof(SavedIntroSmall),
    sizeof(GlobalSaveTracker),
    sizeof(SavedIntroWithIntro),
    sizeof(SavedIntroSpline),
    sizeof(SavedEventSmallVars)
};

static SavedInfoTracker savedInfoTracker;

static SavedBasic *bufferSavedIntroArray[64];

long DoMainMenu;


void SAVE_GetInstanceRotation(Instance *instance, SmallRotation *vector)
{
    evPositionData *rotation;

    rotation = (evPositionData *)INSTANCE_Query(instance, 7);

    if (rotation == NULL)
    {
        vector->x = instance->rotation.x;
        vector->y = instance->rotation.y;
        vector->z = instance->rotation.z;
    }
    else
    {
        vector->x = rotation->x;
        vector->y = rotation->y;
        vector->z = rotation->z;
    }
}

void SAVE_ClearMemory(GameTracker *gameTracker)
{
    char *buffer;

    (void)gameTracker;

    buffer = savedInfoTracker.MemoryCardBuffer;

    savedInfoTracker.InfoStart = &buffer[the_header_size];
    savedInfoTracker.InfoEnd = &buffer[the_header_size];

    savedInfoTracker.EndOfMemory = &buffer[24576];

    memset(&savedInfoTracker.MemoryCardBuffer[the_header_size], 0, 24576 - the_header_size);

    numbufferedIntros = 0;

    memset(bufferSavedIntroArray, 0, sizeof(bufferSavedIntroArray));

    GlobalSave = (GlobalSaveTracker *)SAVE_GetSavedBlock(6, 0);

    GlobalSave->CurrentBirthID = 8192;

    GlobalSave->humanOpinionOfRaziel = 0;

    SAVE_GetSavedBlock(4, 0);
}

void SAVE_Init(GameTracker *gt)
{
    void *buffer;

    buffer = MEMPACK_Malloc(24576, 18);

    if (DoMainMenu != 0)
    {
        gt->memcard = &gMemcard;

        the_header_size = memcard_initialize(&gMemcard, gt, 3, buffer, 24576);
    }
    else
    {
        gt->memcard = NULL;
    }

    savedInfoTracker.MemoryCardBuffer = (char *)buffer;

    SAVE_ClearMemory(gt);
}

void *SAVE_GetSavedBlock(long saveType, long extraSize)
{
    SavedBasic *savedInfo;
    long sizeOfSave;
    long done;

    savedInfo = NULL;

    if (saveType >= 10)
    {
        DEBUG_FatalError("illegal save type %d\n", saveType);
    }

    sizeOfSave = SaveArraySize[saveType] + extraSize;

    sizeOfSave = ((sizeOfSave + 3) >> 2) << 2;

    done = 0;

    if (sizeOfSave >= 1021)
    {
        DEBUG_FatalError("save %d is too big! (type %d)\n", sizeOfSave, saveType);
    }

    do
    {
        if ((savedInfoTracker.InfoEnd + sizeOfSave) < savedInfoTracker.EndOfMemory)
        {
            savedInfo = (SavedBasic *)savedInfoTracker.InfoEnd;

            *savedInfoTracker.InfoEnd = (char)saveType;

            savedInfo->shiftedSaveSize = (unsigned char)(sizeOfSave >> 2);

            savedInfoTracker.InfoEnd += sizeOfSave;

            done = 1;
        }
        else if (SAVE_PurgeAMemoryBlock() == 0)
        {
            DEBUG_FatalError("ran out of saved memory. needed %d, used %d.\nincrease from %d\n", sizeOfSave, savedInfoTracker.EndOfMemory - savedInfoTracker.InfoEnd, 24576);

            done = 1;
        }
    } while (done == 0);

    return savedInfo;
}

long SAVE_PurgeAMemoryBlock()
{
    SavedBasic *curSave;
    long result;

    result = 0;

    for (curSave = (SavedBasic *)savedInfoTracker.InfoStart; (uintptr_t)curSave < (uintptr_t)savedInfoTracker.InfoEnd; curSave += curSave->shiftedSaveSize << 1)
    {
        if (((curSave->savedID == 1) && (((SavedIntro *)curSave)->flags2 & 0x100))
        || ((curSave->savedID == 7) && (((SavedIntroWithIntro *)curSave)->flags2 & 0x100)))
        {
            SAVE_DeleteBlock(curSave);

            result = 1;
            break;
        }
    }

    return result;
}

long SAVE_SaveableInstance(Instance *instance)
{
    long result;
    int temp, temp2; // not from decls.h

    result = 0;

    temp2 = instance->flags2;

    if ((!(temp2 & 0x20000)) && (instance->object != NULL))
    {
        if (((instance->object->oflags2 & 0x80000)) || (instance == gameTrackerX.playerInstance))
        {
            result = 1;
        }
        else if ((instance->object->oflags2 & 0x40000))
        {
            result = 1;

            if ((instance->object->oflags & result))
            {
                temp = (temp2 & 0x8) > 0;

                result = temp > 0;
            }
        }
        else if (!(instance->flags & 0x100000))
        {
            result = 3;
        }
    }

    if ((instance->object->oflags2 & 0x100000))
    {
        result = 0;
    }

    if ((result == 1) && (instance->currentStreamUnitID == instance->birthStreamUnitID) && (instance->introUniqueID < 8192))
    {
        result = 2;
    }

    return result;
}

SavedIntro *SAVE_UpdateSavedIntro(Instance *instance, Level *level, SavedIntro *savedIntro, evControlSaveDataData *extraData)
{
    Position *levelOffset;

    levelOffset = &level->terrain->BSPTreeArray->globalOffset;
    if (savedIntro)
    {
        memcpy(savedIntro->name, instance->introName, 8);
        savedIntro->savedID = 1;
        savedIntro->introUniqueID = (short)instance->introUniqueID;
        savedIntro->streamUnitID = (short)instance->currentStreamUnitID;
        savedIntro->birthUnitID = (short)instance->birthStreamUnitID;
        SUB_SVEC(Position, &savedIntro->position, Position, &instance->position, Position, levelOffset);
        SAVE_GetInstanceRotation(instance, &savedIntro->smallRotation);
        savedIntro->flags = instance->flags;
        savedIntro->flags2 = instance->flags2;
        savedIntro->specturalLightGroup = instance->spectralLightGroup;
        savedIntro->lightGroup = instance->lightGroup;
        savedIntro->attachedUniqueID = (short)instance->attachedID;
        if (extraData)
        {
            memcpy(savedIntro + 1, extraData->data, extraData->length);
        }
    }
    return savedIntro;
}

SavedIntroWithIntro *SAVE_UpdateSavedIntroWithIntro(Instance *instance, Level *level, SavedIntroWithIntro *savedIntro, evControlSaveDataData *extraData)
{
    Position *levelOffset;

    levelOffset = &level->terrain->BSPTreeArray->globalOffset;

    if ((savedIntro != NULL) && (instance->intro != NULL))
    {
        savedIntro->savedID = 7;

        savedIntro->introOffset = instance->intro - level->introList;

        savedIntro->introUniqueID = (short)instance->introUniqueID;
        savedIntro->birthUnitID = (short)instance->birthStreamUnitID;

        SUB_SVEC(Position, &savedIntro->position, Position, &instance->position, Position, levelOffset);

        SAVE_GetInstanceRotation(instance, &savedIntro->smallRotation);

        savedIntro->flags = instance->flags;
        savedIntro->flags2 = instance->flags2;

        savedIntro->specturalLightGroup = instance->spectralLightGroup;
        savedIntro->lightGroup = instance->lightGroup;

        savedIntro->attachedUniqueID = (unsigned short)instance->attachedID;

        if (extraData != NULL)
        {
            memcpy(&savedIntro[1], extraData->data, extraData->length); // TODO: first parameter unlikely, perhaps savedIntro was casted?
        }
    }

    return savedIntro;
}

SavedBasic *SAVE_GetSavedEvent(long areaID, long eventNumber)
{
    SavedBasic *curSave;

    for (curSave = (SavedBasic *)savedInfoTracker.InfoStart; (uintptr_t)curSave < (uintptr_t)savedInfoTracker.InfoEnd; curSave += curSave->shiftedSaveSize << 1)
    {
        if (((curSave->savedID == 2) && (((SavedEvent *)curSave)->areaID == areaID) && (((SavedEvent *)curSave)->eventNumber == eventNumber))
        || ((curSave->savedID == 9) && (((SavedEventSmallVars *)curSave)->areaID == areaID) && ((unsigned char)((SavedEventSmallVars *)curSave)->eventNumber == eventNumber)))
        {
            return curSave;
        }
    }

    return NULL;
}

void SAVE_DeleteSavedEvent(long areaID, long eventNumber)
{
    SavedBasic *savedEvent;

    savedEvent = SAVE_GetSavedEvent(areaID, eventNumber);

    if (savedEvent != NULL)
    {
        SAVE_DeleteBlock(savedEvent);
    }
}

SavedBasic *SAVE_GetSavedNextEvent(long areaID, SavedBasic *curSave)
{
    if (curSave == NULL)
    {
        curSave = (SavedBasic *)savedInfoTracker.InfoStart;
    }
    else
    {
        curSave = &curSave[curSave->shiftedSaveSize << 1];
    }

    for (; (uintptr_t)curSave < (uintptr_t)savedInfoTracker.InfoEnd; curSave += curSave->shiftedSaveSize << 1)
    {
        if (((curSave->savedID == 2) && (((SavedEvent *)curSave)->areaID == areaID))
        || ((curSave->savedID == 9) && (((SavedEventSmallVars *)curSave)->areaID == areaID)))
        {
            return curSave;
        }
    }

    return NULL;
}

void SAVE_BufferIntro(SavedBasic *savedIntro)
{
    long i;

    if (numbufferedIntros < 64)
    {
        for (i = 0; i < 64; i++)
        {
            if (bufferSavedIntroArray[i] == savedIntro)
            {
                break;
            }
        }

        if (i == 64)
        {
            numbufferedIntros++;

            for (i = 0; i < 64; i++)
            {
                if (bufferSavedIntroArray[i] == NULL)
                {
                    bufferSavedIntroArray[i] = savedIntro;
                    break;
                }
            }
        }
    }
}

void SAVE_IntroduceBufferIntros()
{
    long i;
    StreamUnit *streamUnit;

    for (i = 0; numbufferedIntros != 0; i++)
    {
        if (i >= 64)
        {
            break;
        }

        if (bufferSavedIntroArray[i] != NULL)
        {
            int deleted;

            if (bufferSavedIntroArray[i]->savedID == 1)
            {
                SavedIntro *temp; // not from decls.h

                temp = (SavedIntro *)bufferSavedIntroArray[i];

                streamUnit = STREAM_GetStreamUnitWithID(((SavedIntro *)bufferSavedIntroArray[i])->streamUnitID);

                if (streamUnit != NULL)
                {
                    if (INSTANCE_IntroduceSavedInstance(temp, streamUnit, &deleted) != NULL)
                    {
                        bufferSavedIntroArray[i] = NULL;

                        numbufferedIntros--;
                    }
                }
                else
                {
                    bufferSavedIntroArray[i] = NULL;

                    numbufferedIntros--;
                }
            }
            else
            {
                SavedIntroWithIntro *temp; // not from decls.h

                temp = (SavedIntroWithIntro *)bufferSavedIntroArray[i];

                streamUnit = STREAM_GetStreamUnitWithID(((SavedIntroWithIntro *)bufferSavedIntroArray[i])->birthUnitID);

                if (streamUnit != NULL)
                {
                    if (INSTANCE_IntroduceSavedInstanceWithIntro(temp, streamUnit, &deleted) != NULL)
                    {
                        bufferSavedIntroArray[i] = NULL;

                        numbufferedIntros--;
                    }
                }
                else
                {
                    bufferSavedIntroArray[i] = NULL;

                    numbufferedIntros--;
                }
            }
        }
    }
}

void SAVE_IntroForStreamID(StreamUnit *streamUnit)
{
    SavedBasic *saveIntro;
    long streamID;
    int deleted;

    streamID = streamUnit->StreamUnitID;

    for (saveIntro = (SavedBasic *)savedInfoTracker.InfoStart; (uintptr_t)saveIntro < (uintptr_t)savedInfoTracker.InfoEnd; )
    {
        deleted = 0;

        if ((saveIntro->savedID == 1) && (((SavedIntro *)saveIntro)->streamUnitID == streamID))
        {
            INSTANCE_IntroduceSavedInstance((SavedIntro *)saveIntro, streamUnit, &deleted);
        }
        else if ((saveIntro->savedID == 7) && (((SavedIntroWithIntro *)saveIntro)->birthUnitID == streamID))
        {
            INSTANCE_IntroduceSavedInstanceWithIntro((SavedIntroWithIntro *)saveIntro, streamUnit, &deleted);
        }

        if (deleted == 0)
        {
            saveIntro += saveIntro->shiftedSaveSize << 1;
        }
    }
}

long SAVE_HasSavedIntro(Intro *intro, long currentStreamID)
{
    SavedIntro *saveIntro;
    long result;

    (void)currentStreamID;

    result = 0;

    for (saveIntro = (SavedIntro *)savedInfoTracker.InfoStart; (uintptr_t)saveIntro < (uintptr_t)savedInfoTracker.InfoEnd; saveIntro = (SavedIntro *)((char *)saveIntro + (saveIntro->shiftedSaveSize << 2)))
    {
        if (((saveIntro->savedID == 1) && (saveIntro->introUniqueID == intro->UniqueID))
        || ((saveIntro->savedID == 7) && (((SavedIntroWithIntro *)saveIntro)->introUniqueID == intro->UniqueID)))
        {
            result = 1;
            break;
        }
    }

    return result;
}

SavedLevel *SAVE_HasSavedLevel(long areaID)
{
    SavedLevel *savedLevel;

    for (savedLevel = (SavedLevel *)savedInfoTracker.InfoStart; (uintptr_t)savedLevel < (uintptr_t)savedInfoTracker.InfoEnd; savedLevel = (SavedLevel *)((char *)savedLevel + (savedLevel->shiftedSaveSize << 2)))
    {
        if ((savedLevel->savedID == 3) && (savedLevel->areaID == areaID))
        {
            return savedLevel;
        }
    }

    return NULL;
}

void SAVE_UpdateLevelWithSave(StreamUnit *streamUnit)
{
    long Zoffset;
    ActualSavedLevel *savedLevel;
    Terrain *terrain;
    long i;
    BSPTree *bspTree;

    Zoffset = streamUnit->level->terrain->BSPTreeArray->globalOffset.z;

    savedLevel = (ActualSavedLevel *)SAVE_HasSavedLevel(streamUnit->StreamUnitID);

    if (savedLevel != NULL)
    {
        terrain = streamUnit->level->terrain;

        for (i = 0; i < savedLevel->numberBSPTreesSaved; i++)
        {
            bspTree = &terrain->BSPTreeArray[savedLevel->bspTreeArray[i].bspIndex];

            bspTree->localOffset = savedLevel->bspTreeArray[i].localOffset;

            bspTree->flags = (((unsigned short)bspTree->flags << 16) >> 24) << 8;

            bspTree->flags |= savedLevel->bspTreeArray[i].importantFlagsSaved;

            bspTree->globalOffset.x += bspTree->localOffset.x;
            bspTree->globalOffset.y += bspTree->localOffset.y;
            bspTree->globalOffset.z += bspTree->localOffset.z;
        }

        if ((savedLevel->waterZ != -32767) && (savedLevel->waterZ != 32767))
        {
            streamUnit->level->waterZLevel = savedLevel->waterZ + Zoffset;
        }
        else
        {
            streamUnit->level->waterZLevel = savedLevel->waterZ;
        }

        terrain->UnitChangeFlags |= 0x3;
    }
}

SavedLevel *SAVE_CreatedSavedLevel(long areaID, Level *level)
{
    SavedLevel *savedLevel;
    ActualSavedLevel *slevel;
    long doSave;
    long i;
    BSPTree *bspTree;
    long Zoffset;

    Zoffset = level->terrain->BSPTreeArray->globalOffset.z;

    savedLevel = NULL;

    doSave = 0;

    if (((level->terrain->UnitChangeFlags & 0x1)) || ((level->terrain->UnitChangeFlags & 0x2)))
    {
        doSave = 1;
    }

    if (doSave != 0)
    {
        long numBSPTrees;

        numBSPTrees = level->terrain->numBSPTrees - 2;

        if (numBSPTrees > 0)
        {
            savedLevel = SAVE_HasSavedLevel(areaID);

            if ((savedLevel != NULL) || (savedLevel = SAVE_GetSavedBlock(3, numBSPTrees * 8), savedLevel != NULL))
            {
                slevel = (ActualSavedLevel *)savedLevel;

                slevel->areaID = areaID;

                if ((level->waterZLevel != -32767) && (level->waterZLevel != 32767))
                {
                    slevel->waterZ = level->waterZLevel - Zoffset;
                }
                else
                {
                    slevel->waterZ = level->waterZLevel;
                }

                slevel->numberBSPTreesSaved = numBSPTrees;

                for (i = 0; i < level->terrain->numBSPTrees; i++)
                {
                    bspTree = &level->terrain->BSPTreeArray[i];

                    if ((unsigned short)(bspTree->ID + 1) > 1)
                    {
                        do
                        {
                            slevel->bspTreeArray->bspIndex = i;

                            memcpy(&slevel->bspTreeArray->localOffset, &bspTree->localOffset, sizeof(Position));

                            slevel->bspTreeArray->importantFlagsSaved = bspTree->flags;

                            slevel = (ActualSavedLevel *)&slevel->bspTreeArray;
                        } while (0); // this loop isn't really necessary (it's garbage), however the four lines inside of it are
                    }
                }
            }
        }
    }
    else
    {
        savedLevel = (SavedLevel *)SAVE_HasSavedLevel(areaID);

        if (savedLevel != NULL)
        {
            SAVE_DeleteBlock((SavedBasic *)savedLevel);
        }
    }

    return savedLevel;
}

void SAVE_DeleteBlock(SavedBasic *savedBlock)
{
    long size;
    char *nextBlock;
    int i;

    size = savedBlock->shiftedSaveSize << 2;

    nextBlock = (char *)savedBlock + size;

    if (numbufferedIntros != 0)
    {
        for (i = 0; i < 64; i++)
        {
            if (bufferSavedIntroArray[i] == savedBlock)
            {
                bufferSavedIntroArray[i] = NULL;
            }
            else if (bufferSavedIntroArray[i] > savedBlock)
            {
                bufferSavedIntroArray[i] = (SavedBasic *)((char *)bufferSavedIntroArray[i] - size);
            }
        }
    }

    memmove(savedBlock, nextBlock, savedInfoTracker.InfoEnd - nextBlock);

    savedInfoTracker.InfoEnd -= size;
}

void SAVE_Instance(Instance *instance, Level *level)
{
    SavedIntro *savedIntro;
    evControlSaveDataData *extraData;
    long extraSize;
    long saveType;
    SavedIntroSmall *savedSmallIntro;
    SavedIntroWithIntro *savedIntroWithIntro;
    SavedIntroSpline *savedIntroSpline;
    MultiSpline *multi;

    extraSize = 0;

    saveType = SAVE_SaveableInstance(instance);

    if (saveType != 0)
    {
        if ((instance->flags2 & 0x4))
        {
            SAVE_DeleteInstance(instance);

            extraData = (evControlSaveDataData *)INSTANCE_Query(instance, 24);

            if (extraData != NULL)
            {
                savedSmallIntro = (SavedIntroSmall *)SAVE_GetSavedBlock(5, extraData->length);

                if (savedSmallIntro != NULL)
                {
                    savedSmallIntro->introUniqueID = (short)instance->introUniqueID;

                    memcpy(&((evControlSaveDataData *)savedSmallIntro)->data, extraData->data, extraData->length);
                }
            }
        }
        else if (saveType == 1)
        {
            SAVE_DeleteInstance(instance);

            extraData = (evControlSaveDataData *)INSTANCE_Query(instance, 24);

            if (extraData != NULL)
            {
                extraSize = extraData->length;
            }

            savedIntro = (SavedIntro *)SAVE_GetSavedBlock(1, extraSize);

            if (savedIntro != NULL)
            {
                SAVE_UpdateSavedIntro(instance, level, savedIntro, extraData);
            }
        }
        else if (saveType == 2)
        {
            SAVE_DeleteInstance(instance);

            extraData = (evControlSaveDataData *)INSTANCE_Query(instance, 24);

            if (extraData != NULL)
            {
                extraSize = extraData->length;
            }

            savedIntroWithIntro = (SavedIntroWithIntro *)SAVE_GetSavedBlock(7, extraSize);

            if (savedIntroWithIntro != NULL)
            {
                SAVE_UpdateSavedIntroWithIntro(instance, level, savedIntroWithIntro, extraData);
            }
        }
        else if (saveType == 3)
        {
            SAVE_DeleteInstance(instance);

            savedIntroSpline = (SavedIntroSpline *)SAVE_GetSavedBlock(8, 0);

            if (savedIntroSpline != NULL)
            {
                multi = SCRIPT_GetMultiSpline(instance, NULL, NULL);

                if (multi != NULL)
                {
                    instance->splineFlags &= ~0x180;

                    if ((instance->flags & 0x1000000))
                    {
                        instance->splineFlags |= 0x80;
                    }

                    if ((instance->flags & 0x2000000))
                    {
                        instance->splineFlags |= 0x100;
                    }

                    savedIntroSpline->savedID = 8;

                    savedIntroSpline->introUniqueID = (short)instance->introUniqueID;

                    savedIntroSpline->splineFlags = instance->splineFlags;

                    savedIntroSpline->splineKeyFrame = (short)INSTANCE_GetSplineFrameNumber(instance, multi);

                    savedIntroSpline->splineClipBeg = instance->clipBeg;
                    savedIntroSpline->splineClipEnd = instance->clipEnd;
                }
            }
        }
    }
}

void SAVE_DeleteInstance(Instance *instance)
{
    SavedBasic *saveIntro;

    for (saveIntro = (SavedBasic *)savedInfoTracker.InfoStart; (uintptr_t)saveIntro < (uintptr_t)savedInfoTracker.InfoEnd; saveIntro += saveIntro->shiftedSaveSize << 1)
    {
        if (((saveIntro->savedID == 1) && (((SavedIntro *)saveIntro)->introUniqueID == instance->introUniqueID)) ||
        ((saveIntro->savedID == 7) && (((SavedIntroWithIntro *)saveIntro)->introUniqueID == instance->introUniqueID)) ||
        ((saveIntro->savedID == 5) && (((SavedIntroSmall *)saveIntro)->introUniqueID == instance->introUniqueID)) ||
        ((saveIntro->savedID == 8) && (((SavedIntroSpline *)saveIntro)->introUniqueID == instance->introUniqueID)))
        {
            SAVE_DeleteBlock(saveIntro);
            break;
        }
    }
}

void SAVE_SetDeadDeadBit(int uniqueID, long set)
{
    SavedIntro *saveIntro;
    SavedDeadDeadBits *deadDeadBits;
    int deadByte;
    int deadBit;

    deadDeadBits = NULL;

    if (uniqueID < 8192)
    {
        for (saveIntro = (SavedIntro *)savedInfoTracker.InfoStart; (uintptr_t)saveIntro < (uintptr_t)savedInfoTracker.InfoEnd; saveIntro = (SavedIntro *)((char *)saveIntro + (saveIntro->shiftedSaveSize << 2)))
        {
            if (saveIntro->savedID == 4)
            {
                deadDeadBits = (SavedDeadDeadBits *)saveIntro;
                break;
            }
        }

        if (deadDeadBits != NULL)
        {
            deadByte = uniqueID / 8;

            deadBit = 1 << (uniqueID & 0x7);

            if (deadByte < 832)
            {
                if (set == 1)
                {
                    deadDeadBits->deadDeadBits[deadByte] |= (char)deadBit;
                }
                else
                {
                    deadDeadBits->deadDeadBits[deadByte] &= (char)~deadBit;
                }
            }
        }
    }
}

void SAVE_RestoreGlobalSavePointer()
{
    SavedBasic *saveIntro;

    GlobalSave = NULL;

    for (saveIntro = (SavedBasic *)savedInfoTracker.InfoStart; (uintptr_t)saveIntro < (uintptr_t)savedInfoTracker.InfoEnd; saveIntro += saveIntro->shiftedSaveSize << 1)
    {
        if (saveIntro->savedID == 6)
        {
            GlobalSave = (GlobalSaveTracker *)saveIntro;
            break;
        }
    }
}

long SAVE_IsUniqueIDDeadDead(long uniqueID)
{
    SavedIntro *saveIntro;
    SavedDeadDeadBits *deadDeadBits;
    long result;
    int deadByte;
    int deadBit;

    deadDeadBits = NULL;

    result = 0;

    if (uniqueID < 8192)
    {
        for (saveIntro = (SavedIntro *)savedInfoTracker.InfoStart; (uintptr_t)saveIntro < (uintptr_t)savedInfoTracker.InfoEnd; saveIntro = (SavedIntro *)((char *)saveIntro + (saveIntro->shiftedSaveSize << 2)))
        {
            if (saveIntro->savedID == 4)
            {
                deadDeadBits = (SavedDeadDeadBits *)saveIntro;
                break;
            }
        }

        if (deadDeadBits != NULL)
        {
            deadByte = uniqueID / 8;

            deadBit = 1 << (uniqueID & 0x7);

            if (deadByte < 832)
            {
                result = !(((unsigned char)deadDeadBits->deadDeadBits[deadByte] & deadBit) ^ deadBit);
            }
        }
    }

    return result;
}

long SAVE_IsIntroDeadDead(Intro *intro)
{
    return SAVE_IsUniqueIDDeadDead(intro->UniqueID);
}

void SAVE_DoInstanceDeadDead(Instance *instance)
{
    SAVE_DeleteInstance(instance);

    SAVE_SetDeadDeadBit(instance->introUniqueID, 1);
}

void SAVE_MarkDeadDead(Instance *instance)
{
    instance->flags |= 0x800000;
}

void SAVE_UndestroyInstance(Instance *instance)
{
    SAVE_SetDeadDeadBit(instance->introUniqueID, 0);
}

SavedIntroSmall *SAVE_GetSavedSmallIntro(Instance *instance)
{
    SavedBasic *curSave;

    for (curSave = (SavedBasic *)savedInfoTracker.InfoStart; (uintptr_t)curSave < (uintptr_t)savedInfoTracker.InfoEnd; curSave += curSave->shiftedSaveSize << 1)
    {
        if ((curSave->savedID == 5) && (((SavedIntroSmall *)curSave)->introUniqueID == instance->introUniqueID))
        {
            return (SavedIntroSmall *)curSave;
        }
    }

    return NULL;
}

SavedIntroSpline *SAVE_GetIntroSpline(Instance *instance)
{
    SavedBasic *curSave;

    for (curSave = (SavedBasic *)savedInfoTracker.InfoStart; (uintptr_t)curSave < (uintptr_t)savedInfoTracker.InfoEnd; curSave += curSave->shiftedSaveSize << 1)
    {
        if ((curSave->savedID == 8) && (((SavedIntroSpline *)curSave)->introUniqueID == instance->introUniqueID))
        {
            return (SavedIntroSpline *)curSave;
        }
    }

    return NULL;
}

void SAVE_UpdateGlobalSaveTracker()
{
    GlobalSave->currentTime = (unsigned long)gameTrackerX.currentTime;

    memcpy(&GlobalSave->sound, &gameTrackerX.sound, sizeof(gSoundData));

    GlobalSave->saveVersion = 21793;

    if (GAMEPAD_DualShockEnabled() != 0)
    {
        GlobalSave->flags |= 0x2;
    }
    else
    {
        GlobalSave->flags &= ~0x2;
    }
}

void SAVE_RestoreGlobalSaveTracker()
{
    if (GlobalSave->saveVersion != 21793)
    {
        DEBUG_FatalError("error: old save game\n");
    }
    else
    {
        gameTrackerX.currentTime = GlobalSave->currentTime;

        memcpy(&gameTrackerX.sound, &GlobalSave->sound, sizeof(gSoundData));

        SOUND_SetSfxVolume(gameTrackerX.sound.gSfxVol);
        SOUND_SetMusicVolume(gameTrackerX.sound.gMusicVol);
        SOUND_SetVoiceVolume(gameTrackerX.sound.gVoiceVol);

        if ((GlobalSave->flags & 0x2))
        {
            GAMEPAD_EnableDualShock();
        }
        else
        {
            GAMEPAD_DisableDualShock();
        }
    }
}

void SAVE_SaveEverythingInMemory()
{
    Instance *instance;
    long i;
    Instance *next;
    Level *level;

    for (instance = gameTrackerX.instanceList->first, next = NULL; instance != NULL; instance = next, next = NULL)
    {
        next = instance->next;

        level = STREAM_GetLevelWithID(instance->currentStreamUnitID);

        if (level != NULL)
        {
            SAVE_Instance(instance, level);
        }
    }

    for (i = 0; i < 16; i++)
    {
        if (StreamTracker.StreamList[i].used == 2)
        {
            EVENT_SaveEventsFromLevel(StreamTracker.StreamList[i].StreamUnitID, StreamTracker.StreamList[i].level);

            SAVE_CreatedSavedLevel(StreamTracker.StreamList[i].StreamUnitID, StreamTracker.StreamList[i].level);
        }
    }
}

void SAVE_SaveGame()
{
    while (STREAM_PollLoadQueue() != 0)
    {
    }

    SAVE_SaveEverythingInMemory();

    SAVE_UpdateGlobalSaveTracker();

    GlobalSave->sizeUsedInBlock = *(unsigned short *)&savedInfoTracker.InfoEnd - *(unsigned short *)&savedInfoTracker.InfoStart;
}

void SAVE_RestoreGame()
{
    gameTrackerX.streamFlags |= 0x200000;

    SAVE_RestoreGlobalSavePointer();
    SAVE_RestoreGlobalSaveTracker();

    savedInfoTracker.InfoEnd = &savedInfoTracker.InfoStart[GlobalSave->sizeUsedInBlock];

    GAMELOOP_RequestLevelChange("under", 1, &gameTrackerX);
}

void SAVE_DebugSaveGame()
{
}

void SAVE_LoadSaveGame()
{
    gameTrackerX.streamFlags |= 0x200000;

    GAMELOOP_RequestLevelChange("under", 1, &gameTrackerX);

    gameTrackerX.gameMode = 0;
}

long SAVE_SizeOfFreeSpace()
{
    return savedInfoTracker.EndOfMemory - savedInfoTracker.InfoEnd;
}

char monVersion[] = "Jun 30 1999";
