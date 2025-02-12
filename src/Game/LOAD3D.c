#include "common.h"
#include "Game/HASM.h"

STATIC LoadStatus loadStatus;

long crap1;

void LOAD_InitCd()
{
    CdInit();

    ResetCallback();

    CdSetDebug(0);
}

void LOAD_CdSeekCallback(unsigned char intr, unsigned char *result)
{
    if (loadStatus.state == 1)
    {
        loadStatus.state = 2;

        crap1 = (GetRCnt(0xF2000000) & 0xFFFF) | (gameTimer << 16);
    }
}

void LOAD_CdDataReady()
{
    struct _ReadQueueEntry *currentQueueFile;
    long actualReadSize;
    int status;

    if (loadStatus.state == 3)
    {
        loadStatus.state = 4;
    }
    else if (loadStatus.state == 4)
    {
        currentQueueFile = &loadStatus.currentQueueFile;
        actualReadSize = loadStatus.bytesTransferred;
        currentQueueFile->readCurSize += actualReadSize;

        if (currentQueueFile->readStatus == 3)
        {
            currentQueueFile->readCurDest = (void *)((char *)currentQueueFile->readCurDest + actualReadSize);

            if (currentQueueFile->readCurSize == currentQueueFile->readSize)
            {
                loadStatus.state = 5;
            }
            else
            {
                loadStatus.state = 2;
            }
        }
        else
        {
            if (currentQueueFile->readStatus == 6)
            {
                if (currentQueueFile->readCurSize == currentQueueFile->readSize)
                {
                    status = 5;
                }
                else
                {
                    status = 2;
                }

                if (currentQueueFile->retFunc != NULL)
                {
                    typedef void (*retFunc)(void *, long, long, void *, void *);
                    ((retFunc)(currentQueueFile->retFunc))(currentQueueFile->readCurDest, actualReadSize, (status ^ 5) < 1, currentQueueFile->retData, currentQueueFile->retData2);
                }

                if (currentQueueFile->readCurDest == loadStatus.buffer1)
                {
                    currentQueueFile->readCurDest = loadStatus.buffer2;
                }
                else
                {
                    currentQueueFile->readCurDest = loadStatus.buffer1;
                }

                loadStatus.state = status;
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/Game/LOAD3D", LOAD_CdReadReady);

INCLUDE_ASM("asm/nonmatchings/Game/LOAD3D", LOAD_UpdateCheckSum);

INCLUDE_ASM("asm/nonmatchings/Game/LOAD3D", LOAD_DoCDReading);

INCLUDE_ASM("asm/nonmatchings/Game/LOAD3D", LOAD_DoCDBufferedReading);

INCLUDE_ASM("asm/nonmatchings/Game/LOAD3D", LOAD_SetupFileToDoCDReading);

INCLUDE_ASM("asm/nonmatchings/Game/LOAD3D", LOAD_SetupFileToDoBufferedCDReading);

INCLUDE_ASM("asm/nonmatchings/Game/LOAD3D", LOAD_ProcessReadQueue);

INCLUDE_ASM("asm/nonmatchings/Game/LOAD3D", LOAD_ReadFileFromCD);

INCLUDE_ASM("asm/nonmatchings/Game/LOAD3D", LOAD_CdReadFromBigFile);

INCLUDE_ASM("asm/nonmatchings/Game/LOAD3D", LOAD_ReadDirectory);

INCLUDE_ASM("asm/nonmatchings/Game/LOAD3D", LOAD_InitCdLoader);

INCLUDE_ASM("asm/nonmatchings/Game/LOAD3D", LOAD_SetupFileInfo);

INCLUDE_ASM("asm/nonmatchings/Game/LOAD3D", LOAD_NonBlockingReadFile);

INCLUDE_ASM("asm/nonmatchings/Game/LOAD3D", LOAD_CD_ReadPartOfFile);

INCLUDE_ASM("asm/nonmatchings/Game/LOAD3D", LOAD_HashName);

INCLUDE_ASM("asm/nonmatchings/Game/LOAD3D", LOAD_HashUnit);

INCLUDE_ASM("asm/nonmatchings/Game/LOAD3D", LOAD_GetBigFileEntryByHash);

INCLUDE_ASM("asm/nonmatchings/Game/LOAD3D", LOAD_GetBigFileEntry);

INCLUDE_ASM("asm/nonmatchings/Game/LOAD3D", LOAD_DoesFileExist);

INCLUDE_ASM("asm/nonmatchings/Game/LOAD3D", LOAD_LoadTIM);

INCLUDE_ASM("asm/nonmatchings/Game/LOAD3D", LOAD_LoadTIM2);

INCLUDE_ASM("asm/nonmatchings/Game/LOAD3D", LOAD_RelocBinaryData);

INCLUDE_ASM("asm/nonmatchings/Game/LOAD3D", LOAD_CleanUpBuffers);

INCLUDE_ASM("asm/nonmatchings/Game/LOAD3D", LOAD_InitBuffers);

INCLUDE_ASM("asm/nonmatchings/Game/LOAD3D", LOAD_InitCdStreamMode);

void LOAD_DumpCurrentDir()
{
    if (loadStatus.bigFile.currentDir != NULL)
    {
        MEMPACK_Free((char *)loadStatus.bigFile.currentDir);
        loadStatus.bigFile.currentDir = NULL;
        loadStatus.bigFile.currentDirID = 0;
    }

    if (loadStatus.bigFile.cachedDir != NULL)
    {
        MEMPACK_Free((char *)loadStatus.bigFile.cachedDir);
        loadStatus.bigFile.cachedDir = NULL;
        loadStatus.bigFile.cachedDirID = 0;
    }
}

int LOAD_ChangeDirectoryByID(int id)
{
    int i;
    struct _BigFileDir *dir;
    struct _BigFileDir *temp;  // not from decls.h

    if (id != 0)
    {
        if (loadStatus.bigFile.currentDirID == id)
        {
            return 1;
        }
        if (loadStatus.bigFile.cachedDirID == id)
        {
            temp = loadStatus.bigFile.cachedDir;
            loadStatus.bigFile.cachedDirID = loadStatus.bigFile.currentDirID;
            loadStatus.bigFile.currentDirID = id;
            loadStatus.bigFile.cachedDir = loadStatus.bigFile.currentDir;
            loadStatus.bigFile.currentDir = temp;
            return 1;
        }
        else
        {
            for (i = 0; i < loadStatus.bigFile.numSubDirs; i++)
            {
                if (id == loadStatus.bigFile.subDirList[i].streamUnitID)
                {
                    if (loadStatus.bigFile.cachedDir != NULL)
                    {
                        MEMPACK_Free((char *)loadStatus.bigFile.cachedDir);
                    }

                    loadStatus.currentDirLoading = 1;
                    loadStatus.bigFile.cachedDirID = loadStatus.bigFile.currentDirID;
                    loadStatus.bigFile.cachedDir = loadStatus.bigFile.currentDir;

                    dir = LOAD_ReadDirectory(&loadStatus.bigFile.subDirList[i]);
                    loadStatus.bigFile.currentDir = dir;
                    MEMPACK_SetMemoryBeingStreamed((char *)dir);
                    loadStatus.bigFile.currentDirID = id;
                    return 1;
                }
            }
        }
    }

    return 0;
}

void LOAD_SetSearchDirectory(long id)
{
    loadStatus.bigFile.searchDirID = id;
}

long LOAD_GetSearchDirectory()
{
    return loadStatus.bigFile.searchDirID;
}

int LOAD_ChangeDirectoryFlag()
{
    return loadStatus.changeDir;
}

void LOAD_UpdateBigFilePointers(BigFileDir *oldDir, BigFileDir *newDir)
{
    if (loadStatus.bigFile.currentDir == oldDir)
    {
        loadStatus.bigFile.currentDir = newDir;
    }

    if (loadStatus.bigFile.cachedDir == oldDir)
    {
        loadStatus.bigFile.cachedDir = newDir;
    }
}

int LOAD_IsFileLoading()
{
    return loadStatus.currentQueueFile.readStatus != 0;
}

void LOAD_StopLoad()
{
    loadStatus.state = 5;

    loadStatus.currentQueueFile.readStatus = 0;

    if (loadStatus.currentDirLoading != 0)
    {
        loadStatus.currentDirLoading = 0;
    }
}
