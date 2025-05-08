#include "Game/LOAD3D.h"
#include "Game/HASM.h"
#include "Game/MEMPACK.h"
#include "Game/RESOLVE.h"

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

INCLUDE_ASM("asm/nonmatchings/Game/LOAD3D", LOAD_CdReadFromBigFile);

INCLUDE_ASM("asm/nonmatchings/Game/LOAD3D", LOAD_ReadDirectory);

INCLUDE_ASM("asm/nonmatchings/Game/LOAD3D", LOAD_InitCdLoader);

INCLUDE_ASM("asm/nonmatchings/Game/LOAD3D", LOAD_SetupFileInfo);

INCLUDE_ASM("asm/nonmatchings/Game/LOAD3D", LOAD_NonBlockingReadFile);

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

    ext = ext = 0;

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

BigFileDir *LOAD_ReadDirectory(BigFileDirEntry *dirEntry);
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
