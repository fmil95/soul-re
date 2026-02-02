#include "Game/LOAD3D.h"
#include "Game/STRMLOAD.h"
#include "Game/DEBUG.h"
#include "Game/PSX/SUPPORT.h"
#include "Game/FONT.h"
#include "Game/TIMER.h"
#include "Game/MEMPACK.h"
#include "Game/VOICEXA.h"
#include "Game/GAMELOOP.h"
#include "Game/VRAM.h"
#include "Game/PSX/MAIN.h"

static LoadQueueEntry LoadQueue[40];

static int loadFromHead = 0;

static int gCurDir = 0;

static LoadQueueEntry *loadFree;

static LoadQueueEntry *loadHead;

static LoadQueueEntry *loadTail;

static int numLoads;

void STREAM_NextLoadFromHead()
{
    loadFromHead = 1;
}

void STREAM_NextLoadAsNormal()
{
    loadFromHead = 0;
}

void STREAM_InitLoader(char *bigFileName, char *voiceFileName)
{
    int i;

    LOAD_InitCdLoader(bigFileName, voiceFileName);

    loadFree = (LoadQueueEntry *)&LoadQueue->next;

    loadHead = NULL;

    loadTail = NULL;

    numLoads = 0;

    for (i = 38; i >= 0; i--)
    {
        LoadQueue[i].next = &LoadQueue[i + 1];
    }

    LoadQueue[39].next = NULL;
}

void STREAM_RemoveQueueHead()
{
    LoadQueueEntry *entry;

    entry = loadHead;

    loadHead = entry->next;

    if (loadHead == NULL)
    {
        loadTail = NULL;
    }

    entry->next = loadFree;

    numLoads--;

    loadFree = entry;
}

void STREAM_RemoveQueueEntry(LoadQueueEntry *entry, LoadQueueEntry *prev)
{
    if (loadTail == entry)
    {
        loadTail = prev;
    }

    if (prev == NULL)
    {
        loadHead = entry->next;
    }
    else
    {
        prev->next = entry->next;
    }

    entry->next = loadFree;

    loadFree = entry;

    numLoads--;
}

LoadQueueEntry *STREAM_AddQueueEntryToTail()
{
    LoadQueueEntry *entry;

    entry = loadFree;

    if (entry == NULL)
    {
        DEBUG_FatalError("CD ERROR: too many queue entries\n");
    }

    loadFree = entry->next;

    entry->next = NULL;

    if (loadTail != NULL)
    {
        loadTail->next = entry;
    }
    else
    {
        loadHead = entry;
    }

    loadTail = entry;

    numLoads++;

    return entry;
}

LoadQueueEntry *STREAM_AddQueueEntryToHead()
{
    LoadQueueEntry *entry;

    entry = loadFree;

    if (entry == NULL)
    {
        DEBUG_FatalError("CD ERROR: too many queue entries\n");
    }

    loadFree = entry->next;

    if ((loadHead == NULL) || (loadHead->status == 1) || (loadHead->status == 5) || (loadHead->status == 10) || (loadHead->status == 8))
    {
        entry->next = loadHead;

        loadHead = entry;
    }
    else
    {
        entry->next = loadHead->next;

        loadHead->next = entry;
    }

    if (loadTail == NULL)
    {
        loadTail = entry;
    }

    numLoads++;

    return entry;
}

int STREAM_IsCdBusy(long *numberInQueue)
{
    if (numberInQueue != NULL)
    {
        *numberInQueue = numLoads;
    }

    return numLoads;
}

int STREAM_PollLoadQueue()
{
    LOAD_ProcessReadQueue();

    if (loadHead != NULL)
    {
        LoadQueueEntry *queueEntry;

        queueEntry = loadHead;

        if (gameTrackerX.debugFlags < 0)
        {
            FONT_Print("%s status %d\n", &queueEntry->loadEntry.fileName, queueEntry->status);
        }

        if ((!(gameTrackerX.gameFlags & 0x800)) || ((queueEntry->status != 1) && (queueEntry->status != 5) && (queueEntry->status != 8) && (queueEntry->status != 10)))
        {
            switch (queueEntry->status)
            {
            case 2:
                if (LOAD_IsFileLoading() == 0)
                {
                    queueEntry->endLoadTime = TIMER_GetTimeMS() - queueEntry->endLoadTime;

                    if (queueEntry->relocateBinary != 0)
                    {
                        long size;

                        size = LOAD_RelocBinaryData(queueEntry->loadEntry.loadAddr, queueEntry->loadEntry.loadSize);

                        if (queueEntry->mempackUsed != 0)
                        {
                            MEMPACK_Return((char *)queueEntry->loadEntry.loadAddr, size);
                        }

                        queueEntry->relocateBinary = 0;
                    }

                    if (queueEntry->loadEntry.retFunc != NULL)
                    {
                        queueEntry->status = 7;
                    }
                    else
                    {
                        queueEntry->status = 4;

                        if (queueEntry->mempackUsed != 0)
                        {
                            MEMPACK_SetMemoryDoneStreamed((char *)queueEntry->loadEntry.loadAddr);
                        }

                        STREAM_RemoveQueueHead();
                    }
                }

                break;
            case 7:
                queueEntry->status = 4;

                STREAM_RemoveQueueHead();

                if (queueEntry->mempackUsed != 0)
                {
                    MEMPACK_SetMemoryDoneStreamed((char *)queueEntry->loadEntry.loadAddr);
                }

                STREAM_NextLoadFromHead();

                if (queueEntry->loadEntry.retFunc != NULL)
                {
                    void (*retFunc)(void *, void *, void *); // not from decls.h

                    retFunc = queueEntry->loadEntry.retFunc;

                    retFunc(queueEntry->loadEntry.loadAddr, queueEntry->loadEntry.retData, queueEntry->loadEntry.retData2);
                }

                STREAM_NextLoadAsNormal();
                break;
            case 1:
                queueEntry->endLoadTime = TIMER_GetTimeMS();

                LOAD_NonBlockingReadFile(&queueEntry->loadEntry);

                if (LOAD_ChangeDirectoryFlag() != 0)
                {
                    LoadQueueEntry *newQueue;

                    newQueue = STREAM_AddQueueEntryToHead();

                    sprintf(&newQueue->loadEntry.fileName[0], "(%d)", queueEntry->loadEntry.dirHash);

                    newQueue->loadEntry.dirHash = queueEntry->loadEntry.dirHash;
                    newQueue->loadEntry.fileHash = 0;

                    newQueue->status = 10;
                }
                else
                {
                    queueEntry->status = 2;

                    if (queueEntry->mempackUsed != 0)
                    {
                        MEMPACK_SetMemoryBeingStreamed((char *)queueEntry->loadEntry.loadAddr);
                    }

                    if (queueEntry->loadEntry.retPointer != NULL)
                    {
                        *queueEntry->loadEntry.retPointer = queueEntry->loadEntry.loadAddr;
                    }
                }

                break;
            case 5:
                queueEntry->loadEntry.loadAddr = LOAD_InitBuffers();

                queueEntry->endLoadTime = TIMER_GetTimeMS();

                LOAD_CD_ReadPartOfFile(&queueEntry->loadEntry);

                if (LOAD_ChangeDirectoryFlag() != 0)
                {
                    LoadQueueEntry *newQueue;

                    newQueue = STREAM_AddQueueEntryToHead();

                    sprintf(&newQueue->loadEntry.fileName[0], "(%d)", queueEntry->loadEntry.dirHash);

                    newQueue->loadEntry.dirHash = queueEntry->loadEntry.dirHash;
                    newQueue->loadEntry.fileHash = 0;

                    newQueue->status = 10;

                    LOAD_CleanUpBuffers();
                }
                else
                {
                    queueEntry->status = 6;

                    queueEntry->loadEntry.posInFile = 0;
                }

                break;
            case 6:
                if (LOAD_IsFileLoading() == 0)
                {
                    queueEntry->endLoadTime = TIMER_GetTimeMS() - queueEntry->endLoadTime;

                    queueEntry->status = 4;

                    STREAM_RemoveQueueHead();

                    LOAD_CleanUpBuffers();

                    if (queueEntry->loadEntry.retFunc == VRAM_TransferBufferToVram)
                    {
                        VRAM_LoadReturn(queueEntry->loadEntry.loadAddr, queueEntry->loadEntry.retData, queueEntry->loadEntry.retData2);
                    }
                }

                break;
            case 8:
                queueEntry->status = 9;

                VOICEXA_Play(queueEntry->loadEntry.fileHash, 0);
                break;
            case 9:
                if (VOICEXA_IsPlaying() == 0)
                {
                    LOAD_InitCdStreamMode();

                    STREAM_RemoveQueueHead();
                }

                break;
            case 10:
                queueEntry->endLoadTime = TIMER_GetTimeMS();

                if (LOAD_ChangeDirectoryByID(queueEntry->loadEntry.dirHash) == 0)
                {
                    DEBUG_FatalError("Could not read directory hash %d\n", queueEntry->loadEntry.dirHash);
                }

                queueEntry->status = 11;
                break;
            case 11:
                if (LOAD_IsFileLoading() == 0)
                {
                    queueEntry->endLoadTime = TIMER_GetTimeMS() - queueEntry->endLoadTime;

                    STREAM_RemoveQueueHead();
                }

                break;
            case 3:
            case 4:
                break;
            }
        }
    }

    return numLoads;
}

LoadQueueEntry *STREAM_SetUpQueueEntry(char *fileName, void *retFunc, void *retData, void *retData2, void **retPointer, int fromhead)
{
    LoadQueueEntry *currentEntry;

    currentEntry = (fromhead != 0) ? STREAM_AddQueueEntryToHead() : STREAM_AddQueueEntryToTail();

    strcpy(currentEntry->loadEntry.fileName, fileName);

    currentEntry->loadEntry.fileHash = LOAD_HashName(fileName);
    currentEntry->loadEntry.dirHash = LOAD_GetSearchDirectory();

    currentEntry->loadEntry.posInFile = 0;

    currentEntry->loadEntry.checksumType = 1;

    if (LOAD_GetSearchDirectory() != 0)
    {
        currentEntry->loadEntry.dirHash = LOAD_GetSearchDirectory();

        LOAD_SetSearchDirectory(0);
    }
    else
    {
        currentEntry->loadEntry.dirHash = gCurDir;
    }

    currentEntry->loadEntry.retFunc = retFunc;
    currentEntry->loadEntry.retData = retData;
    currentEntry->loadEntry.retData2 = retData2;
    currentEntry->loadEntry.retPointer = retPointer;

    if (retPointer != NULL)
    {
        *(long *)retPointer = 0xFAFBFCFD;
    }

    return currentEntry;
}

void STREAM_QueueNonblockingLoads(char *fileName, unsigned char memType, void *retFunc, void *retData, void *retData2, void **retPointer, long relocateBinary)
{
    LoadQueueEntry *currentEntry;
    int fromhead;

    fromhead = loadFromHead;

    loadFromHead = 0;

    currentEntry = STREAM_SetUpQueueEntry(fileName, retFunc, retData, retData2, retPointer, fromhead);

    currentEntry->loadEntry.loadAddr = NULL;

    currentEntry->mempackUsed = 1;

    currentEntry->loadEntry.memType = memType;

    currentEntry->relocateBinary = relocateBinary;

    if (memType == 0)
    {
        currentEntry->status = 5;
    }
    else
    {
        currentEntry->status = 1;
    }
}

void LOAD_LoadToAddress(char *fileName, void *loadAddr, long relocateBinary)
{
    LoadQueueEntry *currentEntry;

    currentEntry = STREAM_SetUpQueueEntry(fileName, NULL, NULL, NULL, NULL, 0);

    currentEntry->loadEntry.loadAddr = loadAddr;

    currentEntry->status = 1;

    currentEntry->relocateBinary = relocateBinary;

    currentEntry->mempackUsed = 0;

    while (STREAM_PollLoadQueue() != 0)
        ;
}

void LOAD_NonBlockingBinaryLoad(char *fileName, void *retFunc, void *retData, void *retData2, void **retPointer, int memType)
{
    STREAM_QueueNonblockingLoads(fileName, memType, retFunc, retData, retData2, retPointer, 1);
}

void LOAD_NonBlockingFileLoad(char *fileName, void *retFunc, void *retData, void *retData2, void **retPointer, int memType)
{
    STREAM_QueueNonblockingLoads(fileName, memType, retFunc, retData, retData2, retPointer, 0);
}

void LOAD_NonBlockingBufferedLoad(char *fileName, void *retFunc, void *retData, void *retData2)
{
    STREAM_QueueNonblockingLoads(fileName, 0, retFunc, retData, retData2, NULL, 0);
}

int LOAD_IsXAInQueue()
{
    LoadQueueEntry *entry;

    entry = loadHead;

    while (entry != NULL)
    {
        if ((entry->status == 8) || (entry->status == 9))
        {
            return 1;
        }

        entry = entry->next;
    }

    return 0;
}

void LOAD_PlayXA(int number)
{
    LoadQueueEntry *currentEntry;

    currentEntry = STREAM_AddQueueEntryToTail();

    currentEntry->status = 8;

    currentEntry->loadEntry.fileHash = number;

    memcpy(currentEntry->loadEntry.fileName, "voice", sizeof("voice"));
}

long *LOAD_ReadFile(char *fileName, unsigned char memType)
{
    void *loadAddr;

    STREAM_QueueNonblockingLoads(fileName, memType, NULL, NULL, NULL, &loadAddr, 0);

    while (STREAM_PollLoadQueue() != 0)
        ;

    return (long *)loadAddr;
}

void LOAD_ChangeDirectory(char *name)
{
    LoadQueueEntry *currentEntry;

    currentEntry = STREAM_AddQueueEntryToTail();

    gCurDir = LOAD_HashUnit(name);

    currentEntry->loadEntry.dirHash = gCurDir;
    currentEntry->loadEntry.fileHash = 0;

    currentEntry->status = 10;

    sprintf(&currentEntry->loadEntry.fileName[0], "dir %s", name);
}

void LOAD_AbortDirectoryChange(char *name)
{
    LoadQueueEntry *entry;
    LoadQueueEntry *prev;
    long hash;

    if (loadHead != NULL)
    {
        hash = LOAD_HashUnit(name);

        prev = loadHead;

        entry = prev->next;

        while (entry != NULL)
        {
            if ((entry->status == 10) && (entry->loadEntry.dirHash == hash))
            {
                STREAM_RemoveQueueEntry(entry, prev);
                break;
            }

            prev = entry;

            entry = prev->next;
        }
    }
}

void LOAD_AbortFileLoad(char *fileName, void *retFunc)
{
    LoadQueueEntry *entry;
    LoadQueueEntry *prev;
    long hash;
    typedef void (*ret)(long *, void *, void *); // not from decls.h
    ret returnFunction;                          // not from decls.h

    if (loadHead != NULL)
    {
        prev = NULL;

        hash = LOAD_HashName(fileName);

        entry = loadHead;

        while (entry != NULL)
        {
            if (entry->loadEntry.fileHash == hash)
            {
                if (prev == NULL)
                {
                    LOAD_StopLoad();
                }

                if (entry->status == 6)
                {
                    LOAD_CleanUpBuffers();
                }

                returnFunction = (ret)retFunc;

                returnFunction(entry->loadEntry.loadAddr, entry->loadEntry.retData, entry->loadEntry.retData2);

                STREAM_RemoveQueueEntry(entry, prev);
                break;
            }
            else
            {
                prev = entry;

                entry = entry->next;
            }
        }
    }
}
