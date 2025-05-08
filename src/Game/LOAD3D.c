#include "Game/LOAD3D.h"
#include "Game/HASM.h"
#include "Game/MEMPACK.h"
#include "Game/RESOLVE.h"
#include "Game/DEBUG.h"

STATIC LoadStatus loadStatus;

long crap1;

char HashExtensions[7][4];

void LOAD_InitCd()
{
    CdInit();

    ResetCallback();

    CdSetDebug(0);
}

void LOAD_CdSeekCallback(unsigned char intr, unsigned char *result)
{
    (void)intr;
    (void)result;

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

void LOAD_UpdateCheckSum(long bytes)
{
    if (loadStatus.currentQueueFile.checksumType == 1)
    {
        for (; bytes != 0; bytes -= sizeof(int))
        {
            loadStatus.checksum += *loadStatus.checkAddr++;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/Game/LOAD3D", LOAD_DoCDReading);

INCLUDE_ASM("asm/nonmatchings/Game/LOAD3D", LOAD_DoCDBufferedReading);

INCLUDE_ASM("asm/nonmatchings/Game/LOAD3D", LOAD_SetupFileToDoCDReading);

INCLUDE_ASM("asm/nonmatchings/Game/LOAD3D", LOAD_SetupFileToDoBufferedCDReading);

INCLUDE_ASM("asm/nonmatchings/Game/LOAD3D", LOAD_ProcessReadQueue);

INCLUDE_ASM("asm/nonmatchings/Game/LOAD3D", LOAD_ReadFileFromCD);

void LOAD_CdReadFromBigFile(long fileOffset, unsigned long *loadAddr, long bytes, long chksumLevel, long checksum)
{
    loadStatus.currentQueueFile.readSize = bytes;
    loadStatus.currentQueueFile.readCurSize = 0;

    loadStatus.currentQueueFile.readStartDest = loadAddr;
    loadStatus.currentQueueFile.readStartPos = fileOffset;

    loadStatus.currentQueueFile.readStatus = 1;

    loadStatus.currentQueueFile.checksumType = chksumLevel;
    loadStatus.currentQueueFile.checksum = checksum;
}

BigFileDir *LOAD_ReadDirectory(BigFileDirEntry *dirEntry)
{
    BigFileDir *dir;
    long sizeOfDir;

    sizeOfDir = (dirEntry->numFiles * sizeof(BigFileEntry)) + sizeof(long);

    dir = (BigFileDir *)MEMPACK_Malloc(sizeOfDir, 44);

    LOAD_CdReadFromBigFile(dirEntry->subDirOffset, (uintptr_t *)dir, sizeOfDir, 0, 0);

    return dir;
}

void LOAD_InitCdLoader(char *bigFileName, char *voiceFileName)
{
    CdlFILE fp;
    long i;

    (void)voiceFileName;

    loadStatus.state = 0;

    for (i = 0; i < 10; i++)
    {
        if (CdSearchFile(&fp, bigFileName) != NULL)
        {
            break;
        }

        CdReset(0);
    }

    if (i != 10)
    {
        char *ptr;

        LOAD_InitCdStreamMode();

        loadStatus.bigFile.bigfileBaseOffset = CdPosToInt(&fp.pos);

        loadStatus.cdWaitTime = 0;

        loadStatus.currentQueueFile.readStatus = 0;

        loadStatus.bigFile.currentDir = NULL;
        loadStatus.bigFile.currentDirID = 0;

        loadStatus.bigFile.cachedDir = NULL;
        loadStatus.bigFile.cachedDirID = 0;

        loadStatus.bigFile.searchDirID = 0;

        LOAD_CdReadFromBigFile(0, (uintptr_t *)&loadStatus.bigFile.numSubDirs, sizeof(loadStatus.bigFile.numSubDirs), 0, 0);

        do
        {
            LOAD_ProcessReadQueue();
        } while (LOAD_IsFileLoading() != 0);

        CdControlF(CdlPause, NULL);

        i = (loadStatus.bigFile.numSubDirs * 8) + 4;

        ptr = MEMPACK_Malloc(i, 8);

        CdSync(0, NULL);

        LOAD_CdReadFromBigFile(0, (uintptr_t *)ptr, i, 0, 0);

        loadStatus.bigFile.subDirList = (BigFileDirEntry *)&ptr[4];

        do
        {
            LOAD_ProcessReadQueue();
        } while (LOAD_IsFileLoading() != 0);

        loadStatus.bigFile.rootDir = LOAD_ReadDirectory(loadStatus.bigFile.subDirList);

        do
        {
            LOAD_ProcessReadQueue();
        } while (LOAD_IsFileLoading() != 0);
    }
}

extern char D_800D095C[];
int LOAD_SetupFileInfo(NonBlockLoadEntry *loadEntry)
{
    BigFileEntry *fileInfo;

    fileInfo = LOAD_GetBigFileEntryByHash(loadEntry->fileHash);

    if (fileInfo == NULL)
    {
        if (loadEntry->dirHash == loadStatus.bigFile.currentDirID)
        {
            // DEBUG_FatalError("CD ERROR: File %s does not exist\n", loadEntry->fileName);
            DEBUG_FatalError(D_800D095C, loadEntry->fileName);
        }

        return 0;
    }

    loadEntry->filePos = fileInfo->filePos;

    loadEntry->loadSize = fileInfo->fileLen;

    loadEntry->checksum = fileInfo->checkSumFull;

    return 1;
}

void LOAD_NonBlockingReadFile(NonBlockLoadEntry *loadEntry)
{
    if (LOAD_SetupFileInfo(loadEntry) != 0)
    {
        if (loadEntry->loadAddr == NULL)
        {
            loadEntry->loadAddr = (long *)MEMPACK_Malloc(loadEntry->loadSize, loadEntry->memType);
        }

        LOAD_CdReadFromBigFile(loadEntry->filePos, (uintptr_t *)loadEntry->loadAddr, loadEntry->loadSize, loadEntry->checksumType, loadEntry->checksum);

        loadStatus.changeDir = 0;
    }
    else
    {
        loadStatus.changeDir = 1;
    }
}

void LOAD_CD_ReadPartOfFile(NonBlockLoadEntry *loadEntry)
{
    ReadQueueEntry *currentQueueReq;

    if (LOAD_SetupFileInfo(loadEntry) != 0)
    {
        currentQueueReq = &loadStatus.currentQueueFile;

        currentQueueReq->readSize = loadEntry->loadSize;

        currentQueueReq->readCurSize = 0;

        currentQueueReq->readStartDest = loadEntry->loadAddr;
        currentQueueReq->readCurDest = loadEntry->loadAddr;

        currentQueueReq->readStartPos = loadEntry->filePos;

        currentQueueReq->readStatus = 5;

        currentQueueReq->checksumType = 0;

        currentQueueReq->checksum = loadEntry->checksum;

        currentQueueReq->retFunc = loadEntry->retFunc;

        currentQueueReq->retData = loadEntry->retData;
        currentQueueReq->retData2 = loadEntry->retData2;

        loadStatus.changeDir = 0;
    }
    else
    {
        loadStatus.changeDir = 1;
    }
}

long LOAD_HashName(char *string)
{
    long sum;
    long xor;
    long length;
    long ext;
    char c;
    long strl;
    long endPos;
    long i;
    char *pos;

    endPos = 0;

    sum = 0;

    xor = 0;

    length = 0;

    ext = 0;

    ext = 0;

    endPos = 0;

    strl = strlen(string) - 1;

    pos = (char *)strchr(string, '.');

    if (pos != NULL)
    {
        pos++;

        for (i = 0; i < 7; i++)
        {
            if (strcmpi(pos, HashExtensions[i]) == 0)
            {
                ext = i;
                break;
            }
        }

        if (i < 7)
        {
            strl -= 4;
        }
    }

    for (; strl >= endPos; strl--)
    {
        c = string[strl];

        if (c != '\\')
        {
            if ((c >= 'a') && (c <= 'z'))
            {
                c &= ~' ';
            }

            c -= '\x1A';

            sum += c;

            xor ^= c * length;

            length++;
        }
    }

    return (length << 27) | (sum << 15) | (xor << 3) | ext;
}

long LOAD_HashUnit(char *name)
{
    int val;
    int last;
    int hash;
    int num;
    int flag;
    char *c;

    last = 0;

    hash = 0;

    num = 0;

    flag = 0;

    for (c = name; *c != 0; c++)
    {
        val = *c;

        if ((val >= '0') && (val <= '9'))
        {
            num *= 10;

            num += val - '0';
        }
        else
        {
            if ((val >= 'A') && (val <= 'Z'))
            {
                val -= 'A';
            }
            else
            {
                val -= 'a';
            }

            hash <<= 2;

            hash += (flag != 0) ? (val - last) << 5 : val - last;

            flag ^= 0x1;

            last = val;
        }
    }

    hash += num;

    return (short)hash;
}

BigFileEntry *LOAD_GetBigFileEntryByHash(long hash)
{
    int i;
    BigFileEntry *entry;

    if ((loadStatus.bigFile.currentDir != NULL) && (loadStatus.currentDirLoading == 0))
    {
        entry = &loadStatus.bigFile.currentDir->fileList[0];

        for (i = loadStatus.bigFile.currentDir->numFiles; i != 0; i--, entry++)
        {
            if (entry->fileHash == hash)
            {
                return entry;
            }
        }
    }

    entry = &loadStatus.bigFile.rootDir->fileList[0];

    for (i = loadStatus.bigFile.rootDir->numFiles; i != 0; i--, entry++)
    {
        if (entry->fileHash == hash)
        {
            return entry;
        }
    }

    return NULL;
}

BigFileEntry *LOAD_GetBigFileEntry(char *fileName)
{
    return LOAD_GetBigFileEntryByHash(LOAD_HashName(fileName));
}

long LOAD_DoesFileExist(char *fileName)
{
    BigFileEntry *entry;
    int temp; // not from decls.h

    entry = LOAD_GetBigFileEntry(fileName);

    temp = 0;

    if (entry != NULL)
    {
        temp = entry->fileLen != 0;
    }

    return temp;
}

void LOAD_LoadTIM(intptr_t *addr, long x_pos, long y_pos, long clut_x, long clut_y)
{
    RECT rect;
    intptr_t *clutAddr;

    clutAddr = NULL;

    addr += 2;

    if (addr[-1] == 8)
    {
        clutAddr = &addr[3];

        addr += 11;
    }

    rect.x = x_pos;
    rect.y = y_pos;

    rect.w = ((unsigned short *)addr)[4];
    rect.h = ((unsigned short *)addr)[5];

    LoadImage(&rect, (uintptr_t *)&addr[3]);

    if (clutAddr != NULL)
    {
        rect.x = clut_x;
        rect.y = clut_y;

        rect.w = 16;
        rect.h = 1;

        DrawSync(0);

        LoadImage(&rect, (uintptr_t *)clutAddr);
    }
}

void LOAD_LoadTIM2(intptr_t *addr, long x_pos, long y_pos, long width, long height)
{
    RECT rect;

    (void)width;
    (void)height;

    rect.x = x_pos;
    rect.y = y_pos;

    addr += 2;

    rect.w = ((unsigned short *)addr)[4];
    rect.h = ((unsigned short *)addr)[5];

    LoadImage(&rect, &((uintptr_t *)addr)[3]);

    DrawSync(0);
}

long LOAD_RelocBinaryData(intptr_t *data, long fileSize)
{
    intptr_t *lastMoveDest;
    long tableSize;
    RedirectList redirectListX;
    RedirectList *redirectList;

    fileSize = (fileSize + 3) >> 2;

    redirectList = &redirectListX;

    redirectList->data = &data[1];

    redirectList->numPointers = data[0];

    tableSize = ((redirectList->numPointers + 512) / 512) * 512;

    RESOLVE_Pointers(redirectList, &data[tableSize], data);

    lastMoveDest = &data[fileSize - tableSize];

    while (data < lastMoveDest)
    {
        data[0] = data[tableSize];

        data++;
    }

    return tableSize * 4;
}

void LOAD_CleanUpBuffers()
{
    if (loadStatus.buffer1 != NULL)
    {
        MEMPACK_Free(loadStatus.buffer1);

        loadStatus.buffer1 = NULL;
    }

    if (loadStatus.buffer2 != NULL)
    {
        MEMPACK_Free(loadStatus.buffer2);

        loadStatus.buffer2 = NULL;
    }
}

void *LOAD_InitBuffers()
{
    loadStatus.buffer1 = MEMPACK_Malloc(2048, 35);
    loadStatus.buffer2 = MEMPACK_Malloc(2048, 35);

    return loadStatus.buffer1;
}

void LOAD_InitCdStreamMode()
{
    unsigned char cdMode;

    cdMode = CdlModeSize1 | CdlModeSpeed;

    CdReadyCallback(LOAD_CdReadReady);
    CdSyncCallback(LOAD_CdSeekCallback);

    CdControl(CdlSetmode, &cdMode, NULL);
}

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
