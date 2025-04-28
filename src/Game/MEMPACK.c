#include "common.h"
#include "Game/PSX/AADLIB.h"
#include "Game/MEMPACK.h"
#include "Game/LOAD3D.h"
#include "Game/HASM.h"
#include "Game/STREAM.h"
#include "Game/GAMELOOP.h"
#include "Game/DEBUG.h"
#include "Game/SIGNAL.h"

static NewMemTracker newMemTracker;

void MEMPACK_Init()
{
    newMemTracker.rootNode = (MemHeader *)overlayAddress;

    newMemTracker.totalMemory = (0x80000000 + (0x200000 - (0x100000 / 256))) - (intptr_t)overlayAddress;

    newMemTracker.rootNode->magicNumber = 0xBADE;

    newMemTracker.rootNode->memStatus = 0;
    newMemTracker.rootNode->memType = 0;
    newMemTracker.rootNode->memSize = newMemTracker.totalMemory;

    newMemTracker.lastMemoryAddress = (char *)newMemTracker.rootNode + newMemTracker.totalMemory;

    newMemTracker.currentMemoryUsed = 0;

    newMemTracker.doingGarbageCollection = 0;
}

MemHeader *MEMPACK_GetSmallestBlockTopBottom(long allocSize)
{
    MemHeader *address;
    MemHeader *bestAddress;

    address = newMemTracker.rootNode;

    bestAddress = NULL;

    while ((char *)address != newMemTracker.lastMemoryAddress)
    {
        if ((address->memStatus == 0) && (address->memSize >= (unsigned long)allocSize) && (bestAddress == NULL))
        {
            bestAddress = address;
            break;
        }

        address = (MemHeader *)((char *)address + address->memSize);
    }

    return bestAddress;
}

MemHeader *MEMPACK_GetSmallestBlockBottomTop(long allocSize)
{
    MemHeader *address;
    MemHeader *bestAddress;

    address = newMemTracker.rootNode;

    bestAddress = NULL;

    while ((char *)address != newMemTracker.lastMemoryAddress)
    {
        if ((address->memStatus == 0) && (address->memSize >= (unsigned long)allocSize) && ((bestAddress == NULL) || ((char *)address > (char *)bestAddress)))
        {
            bestAddress = address;
        }

        address = (MemHeader *)((char *)address + address->memSize);
    }

    return bestAddress;
}

long MEMPACK_RelocatableType(long memType)
{
    if ((memType == 1) || (memType == 2) || (memType == 44) || (memType == 47) || (memType == 4))
    {
        return 1;
    }

    return 0;
}

char *MEMPACK_Malloc(unsigned long allocSize, unsigned char memType)
{
    MemHeader *bestAddress;
    long relocatableMemory;
    int curMem;
    MemHeader *address;
    long topOffset;

    relocatableMemory = MEMPACK_RelocatableType(memType);

    allocSize = ((allocSize + 11) / 4) << 2;

    do
    {
        if ((newMemTracker.doingGarbageCollection == 0) && (relocatableMemory != 0))
        {
            MEMPACK_DoGarbageCollection();
        }

        if (relocatableMemory != 0)
        {
            bestAddress = MEMPACK_GetSmallestBlockTopBottom(allocSize);
        }
        else if (relocatableMemory == 0)
        {
            bestAddress = MEMPACK_GetSmallestBlockBottomTop(allocSize);
        }

        if (bestAddress == NULL)
        {
            curMem = newMemTracker.currentMemoryUsed;

            STREAM_TryAndDumpANonResidentObject();

            if ((unsigned int)curMem == newMemTracker.currentMemoryUsed)
            {
                if (memType == 16)
                {
                    return NULL;
                }

                MEMPACK_ReportMemory2();

                DEBUG_FatalError("Trying to fit memory size %d Type = %d\nAvailable memory : used = %d, free = %d\n", allocSize, memType, newMemTracker.currentMemoryUsed, newMemTracker.totalMemory - newMemTracker.currentMemoryUsed);
                break;
            }
        }
        else
        {
            break;
        }
    } while ((unsigned int)curMem != newMemTracker.currentMemoryUsed);

    topOffset = bestAddress->memSize;

    if ((topOffset - allocSize) <= 7)
    {
        allocSize = bestAddress->memSize;
    }

    if (allocSize != bestAddress->memSize)
    {
        if (relocatableMemory != 0)
        {
            address = (MemHeader *)((char *)bestAddress + allocSize);

            address->magicNumber = 0xBADE;

            address->memStatus = 0;
            address->memType = 0;
            address->memSize = bestAddress->memSize - allocSize;

            bestAddress->magicNumber = 0xBADE;

            bestAddress->memStatus = 1;
            bestAddress->memType = memType;
            bestAddress->memSize = allocSize;

            newMemTracker.currentMemoryUsed += allocSize;
        }
        else
        {
            address = (MemHeader *)((char *)bestAddress + (topOffset - allocSize));

            address->magicNumber = 0xBADE;

            address->memStatus = 1;
            address->memType = memType;
            address->memSize = allocSize;

            newMemTracker.currentMemoryUsed += allocSize;

            bestAddress->magicNumber = 0xBADE;

            bestAddress->memStatus = 0;
            bestAddress->memType = 0;
            bestAddress->memSize = topOffset - allocSize;

            bestAddress = address;
        }
    }
    else
    {
        bestAddress->magicNumber = 0xBADE;

        bestAddress->memStatus = 1;
        bestAddress->memType = memType;
        bestAddress->memSize = allocSize;

        newMemTracker.currentMemoryUsed += allocSize;
    }

    return (char *)(bestAddress + 1);
}

void MEMORY_MergeAddresses(MemHeader *firstAddress, MemHeader *secondAddress)
{
    if ((firstAddress->memStatus == 0) && (secondAddress->memStatus == 0))
    {
        firstAddress->memSize += secondAddress->memSize;

        secondAddress->magicNumber = 0;
        secondAddress->memStatus = 1;
    }
}

void MEMPACK_Return(char *address, long takeBackSize)
{
    MemHeader *memAddress;
    MemHeader *nextAddress;

    takeBackSize = (takeBackSize >> 2) << 2;

    if (takeBackSize >= (long)sizeof(MemHeader))
    {
        memAddress = (MemHeader *)(address - 8);

        memAddress->memSize -= takeBackSize;

        newMemTracker.currentMemoryUsed -= takeBackSize;

        memAddress = (MemHeader *)((char *)memAddress + memAddress->memSize);

        memAddress->magicNumber = 0xBADE;

        memAddress->memStatus = 0;
        memAddress->memType = 0;
        memAddress->memSize = takeBackSize;

        nextAddress = (MemHeader *)((char *)memAddress + takeBackSize);

        if ((char *)nextAddress != newMemTracker.lastMemoryAddress)
        {
            MEMORY_MergeAddresses(memAddress, nextAddress);
        }
    }
}

void MEMPACK_Free(char *address)
{
    MemHeader *memAddress;
    MemHeader *secondAddress;

    memAddress = (MemHeader *)(address - 8);

    memAddress->memStatus = 0;
    memAddress->memType = 0;

    newMemTracker.currentMemoryUsed -= memAddress->memSize;

    secondAddress = (MemHeader *)((char *)memAddress + memAddress->memSize);

    if ((char *)secondAddress != newMemTracker.lastMemoryAddress)
    {
        MEMORY_MergeAddresses(memAddress, secondAddress);
    }

    secondAddress = memAddress;

    memAddress = newMemTracker.rootNode;

    while ((char *)memAddress != newMemTracker.lastMemoryAddress)
    {
        if (((char *)memAddress + memAddress->memSize) == (char *)secondAddress)
        {
            MEMORY_MergeAddresses(memAddress, (MemHeader *)((char *)memAddress + memAddress->memSize));
            break;
        }

        memAddress = (MemHeader *)((char *)memAddress + memAddress->memSize);
    }
}

void MEMPACK_FreeByType(unsigned char memType)
{
    MemHeader *address;
    int freed;

    do
    {
        freed = 0;

        address = newMemTracker.rootNode;

        while ((char *)address != newMemTracker.lastMemoryAddress)
        {
            if ((address->memStatus == 1) && (address->memType == memType))
            {
                address++;

                freed = 1;

                MEMPACK_Free((char *)address);
                break;
            }

            address = (MemHeader *)((char *)address + address->memSize);
        }
    } while (freed == 1);
}

unsigned long MEMPACK_Size(char *address)
{
    return ((int *)address)[-1] - sizeof(MemHeader);
}

unsigned long MEMPACK_ReportFreeMemory()
{
    return newMemTracker.totalMemory - newMemTracker.currentMemoryUsed;
}

void MEMPACK_ReportMemory2()
{
    MemHeader *address;

    address = newMemTracker.rootNode;

    while ((char *)address != newMemTracker.lastMemoryAddress)
    {
        address = (MemHeader *)((char *)address + address->memSize);
    }
}

void MEMPACK_ReportMemory()
{
    MemHeader *address;
    long i;
    long firstTime;

    address = newMemTracker.rootNode;

    while ((char *)address != newMemTracker.lastMemoryAddress)
    {
        address = (MemHeader *)((char *)address + address->memSize);
    }

    for (i = 0; i < 49; i++)
    {
        firstTime = 1;

        address = newMemTracker.rootNode;

        while ((char *)address != newMemTracker.lastMemoryAddress)
        {
            if ((address->memStatus != 0) && (address->memType == i) && (firstTime != 0))
            {
                firstTime = 0;
            }

            address = (MemHeader *)((char *)address + address->memSize);
        }
    }
}

void MEMPACK_SetMemoryBeingStreamed(char *address)
{
    address[-6] = 2;
}

void MEMPACK_SetMemoryDoneStreamed(char *address)
{
    address[-6] = 1;
}

long MEMPACK_MemoryValidFunc(char *address)
{
    if ((address != (char *)0xFAFBFCFD) && (address != NULL))
    {
        return address[-6] == 1;
    }

    return 0;
}

char *MEMPACK_GarbageCollectMalloc(unsigned long *allocSize, unsigned char memType, unsigned long *freeSize)
{
    MemHeader *bestAddress;

    *allocSize = ((*allocSize + 11) / 4) << 2;

    bestAddress = MEMPACK_GetSmallestBlockTopBottom(*allocSize);

    if (bestAddress == NULL)
    {
        STREAM_DumpNonResidentObjects();

        bestAddress = MEMPACK_GetSmallestBlockTopBottom(*allocSize);

        if (bestAddress == NULL)
        {
            if (memType != 16)
            {
                MEMPACK_ReportMemory();

                DEBUG_FatalError("Trying to fit memory size %d Type = %d\nAvalible memory : used = %d, free = %d\n", *allocSize, memType, newMemTracker.currentMemoryUsed, newMemTracker.totalMemory - newMemTracker.currentMemoryUsed);
            }
            else
            {
                return NULL;
            }
        }
    }

    if ((bestAddress->memSize - *allocSize) < 8)
    {
        *allocSize = bestAddress->memSize;
    }

    if (*allocSize != bestAddress->memSize)
    {
        *freeSize = bestAddress->memSize - *allocSize;

        bestAddress->magicNumber = 0xBADE;

        bestAddress->memStatus = 1;
        bestAddress->memType = memType;
        bestAddress->memSize = *allocSize;

        newMemTracker.currentMemoryUsed += *allocSize;
    }
    else
    {
        bestAddress->magicNumber = 0xBADE;

        bestAddress->memStatus = 1;
        bestAddress->memType = memType;
        bestAddress->memSize = *allocSize;

        newMemTracker.currentMemoryUsed += *allocSize;

        *freeSize = 0;
    }

    return (char *)(bestAddress + 1);
}

void MEMPACK_GarbageSplitMemoryNow(unsigned long allocSize, MemHeader *bestAddress, long memType, unsigned long freeSize)
{
    (void)memType;

    if (freeSize != 0)
    {
        MemHeader *address;

        address = (MemHeader *)((char *)bestAddress + allocSize);

        address->magicNumber = 0xBADE;

        address->memStatus = 0;
        address->memType = 0;
        address->memSize = freeSize;
    }
}

void MEMPACK_GarbageCollectFree(MemHeader *memAddress)
{
    MemHeader *secondAddress;

    memAddress->memStatus = 0;
    memAddress->memType = 0;

    newMemTracker.currentMemoryUsed -= memAddress->memSize;

    secondAddress = (MemHeader *)((char *)memAddress + memAddress->memSize);

    if ((char *)secondAddress != newMemTracker.lastMemoryAddress)
    {
        MEMORY_MergeAddresses(memAddress, secondAddress);
    }

    secondAddress = memAddress;

    memAddress = newMemTracker.rootNode;

    while ((char *)memAddress != newMemTracker.lastMemoryAddress)
    {
        if (((char *)memAddress + memAddress->memSize) == (char *)secondAddress)
        {
            MEMORY_MergeAddresses(memAddress, (MemHeader *)((char *)memAddress + memAddress->memSize));
            break;
        }

        memAddress = (MemHeader *)((char *)memAddress + memAddress->memSize);
    }
}

void MEMPACK_DoGarbageCollection()
{
    MemHeader *relocateAddress;
    long foundOpening;
    long done;
    long addressSize;
    long addressMemType;
    long holdSize;
    long freeSize;
    char *oldAddress;
    char *newAddress;

    done = 0;

    freeSize = 0;

    newMemTracker.doingGarbageCollection = 1;

    while (done == 0)
    {
        relocateAddress = newMemTracker.rootNode;

        foundOpening = 0;

        while ((char *)relocateAddress != newMemTracker.lastMemoryAddress)
        {
            if (relocateAddress->memStatus != 0)
            {
                if ((MEMPACK_RelocatableType(relocateAddress->memType) != 0) && (foundOpening == 1) && (relocateAddress->memStatus != 2))
                {
                    foundOpening = 2;
                    break;
                }
            }
            else
            {
                foundOpening = 1;
            }

            relocateAddress = (MemHeader *)((char *)relocateAddress + relocateAddress->memSize);
        }

        if (foundOpening == 2)
        {
            addressMemType = relocateAddress->memType;

            addressSize = relocateAddress->memSize - sizeof(MemHeader);

            MEMPACK_GarbageCollectFree(relocateAddress);

            holdSize = addressSize;

            newAddress = MEMPACK_GarbageCollectMalloc((unsigned long *)&holdSize, addressMemType, (unsigned long *)&freeSize);

            oldAddress = (char *)(relocateAddress + 1);

            if (newAddress != NULL)
            {
                if (addressMemType == 2)
                {
                    RemoveIntroducedLights((Level *)oldAddress);
                }
                else if (addressMemType == 4)
                {
                    aadRelocateMusicMemoryBegin();
                }

                memcpy(newAddress, oldAddress, addressSize);

                if (addressMemType == 2)
                {
                    MEMPACK_RelocateAreaType((MemHeader *)(newAddress - 8), newAddress - oldAddress, (Level *)oldAddress);
                }
                else if (addressMemType == 1)
                {
                    MEMPACK_RelocateObjectType((MemHeader *)(newAddress - 8), newAddress - oldAddress, (Object *)oldAddress);
                }
                else if (addressMemType == 14)
                {
                    STREAM_UpdateInstanceCollisionInfo((HModel *)oldAddress, (HModel *)newAddress);
                }
                else if (addressMemType == 44)
                {
                    MEMPACK_RelocateCDMemory((MemHeader *)(newAddress - 8), newAddress - oldAddress, (BigFileDir *)oldAddress);
                }
                else if (addressMemType == 4)
                {
                    aadRelocateMusicMemoryEnd(oldAddress, newAddress - oldAddress);
                }
                else if (addressMemType == 47)
                {
                    aadRelocateSfxMemory(oldAddress, newAddress - oldAddress);
                }

                MEMPACK_GarbageSplitMemoryNow(holdSize, (MemHeader *)(newAddress - 8), addressMemType, freeSize);
            }
        }
        else
        {
            done = 1;
        }
    }

    newMemTracker.doingGarbageCollection = 0;
}

void MEMPACK_RelocateAreaType(MemHeader *newAddress, long offset, Level *oldLevel)
{
    Level *level;
    MultiSignal *msignal;
    long sizeOfLevel;
    long i;
    long d;

    level = (Level *)(newAddress + 1);

    sizeOfLevel = newAddress->memSize - sizeof(MemHeader);

    ((Level *)(newAddress + 1))->terrain = (Terrain *)OFFSET_DATA(((Level *)(newAddress + 1))->terrain, offset);

    level->lightList = (LightList *)OFFSET_DATA(level->lightList, offset);

    level->razielLightGroup = (LightGroup *)OFFSET_DATA(level->razielLightGroup, offset);
    level->razielSpectralLightGroup = (LightGroup *)OFFSET_DATA(level->razielSpectralLightGroup, offset);

    level->vmobjectList = (VMObject *)OFFSET_DATA(level->vmobjectList, offset);
    level->spotLightList = (SpotLight *)OFFSET_DATA(level->spotLightList, offset);
    level->pointLightList = (PointLight *)OFFSET_DATA(level->pointLightList, offset);

    level->spotSpecturalLightList = (SpotLight *)OFFSET_DATA(level->spotSpecturalLightList, offset);
    level->pointSpecturalLightList = (PointLight *)OFFSET_DATA(level->pointSpecturalLightList, offset);

    level->bgObjectList = (BGObject *)OFFSET_DATA(level->bgObjectList, offset);
    level->cameraList = (void *)OFFSET_DATA(level->cameraList, offset);
    level->vGroupList = (VGroup *)OFFSET_DATA(level->vGroupList, offset);

    level->startSignal = (MultiSignal *)OFFSET_DATA(level->startSignal, offset);

    level->introList = (Intro *)OFFSET_DATA(level->introList, offset);
    level->bgAniList = (DrMoveAniTex *)OFFSET_DATA(level->bgAniList, offset);
    level->hotSpotList = (HotSpot *)OFFSET_DATA(level->hotSpotList, offset);
    level->objectNameList = (void *)OFFSET_DATA(level->objectNameList, offset);

    level->worldName = (char *)OFFSET_DATA(level->worldName, offset);
    level->enemyUnitsNames = (char **)OFFSET_DATA(level->enemyUnitsNames, offset);

    level->timesSignalList = (long **)OFFSET_DATA(level->timesSignalList, offset);

    level->spectralSignal = (MultiSignal *)OFFSET_DATA(level->spectralSignal, offset);
    level->materialSignal = (MultiSignal *)OFFSET_DATA(level->materialSignal, offset);

    level->startUnitLoadedSignal = (MultiSignal *)OFFSET_DATA(level->startUnitLoadedSignal, offset);
    level->startUnitMainSignal = (MultiSignal *)OFFSET_DATA(level->startUnitMainSignal, offset);

    level->startGoingIntoWaterSignal = (MultiSignal *)OFFSET_DATA(level->startGoingIntoWaterSignal, offset);
    level->startGoingOutOfWaterSignal = (MultiSignal *)OFFSET_DATA(level->startGoingOutOfWaterSignal, offset);

    level->SignalListStart = (MultiSignal *)OFFSET_DATA(level->SignalListStart, offset);
    level->SignalListEnd = (MultiSignal *)OFFSET_DATA(level->SignalListEnd, offset);

    level->PuzzleInstances = (EventPointers *)OFFSET_DATA(level->PuzzleInstances, offset);

    level->PlanMarkerList = (PlanMkr *)OFFSET_DATA(level->PlanMarkerList, offset);
    level->SFXMarkerList = (SFXMkr *)OFFSET_DATA(level->SFXMarkerList, offset);

    for (i = 0; i < level->NumberOfSFXMarkers; i++)
    {
        level->SFXMarkerList[i].soundData = (unsigned char *)OFFSET_DATA(level->SFXMarkerList[i].soundData, offset);
    }

    level->dynamicMusicName = (char *)OFFSET_DATA(level->dynamicMusicName, offset);
    level->spectrallightList = (LightList *)OFFSET_DATA(level->spectrallightList, offset);

    if (level->terrain != NULL)
    {
        Terrain *terrain;

        terrain = level->terrain;

        terrain->introList = (Intro *)OFFSET_DATA(terrain->introList, offset);
        terrain->vertexList = (TVertex *)OFFSET_DATA(terrain->vertexList, offset);
        terrain->faceList = (TFace *)OFFSET_DATA(terrain->faceList, offset);
        terrain->normalList = (Normal *)OFFSET_DATA(terrain->normalList, offset);
        terrain->aniList = (DrMoveAniTex *)OFFSET_DATA(terrain->aniList, offset);
        terrain->StreamUnits = (void *)OFFSET_DATA(terrain->StreamUnits, offset);
        terrain->StartTextureList = (TextureFT3 *)OFFSET_DATA(terrain->StartTextureList, offset);
        terrain->EndTextureList = (TextureFT3 *)OFFSET_DATA(terrain->EndTextureList, offset);
        terrain->MorphDiffList = (MorphVertex *)OFFSET_DATA(terrain->MorphDiffList, offset);
        terrain->MorphColorList = (MorphColor *)OFFSET_DATA(terrain->MorphColorList, offset);
        terrain->BSPTreeArray = (BSPTree *)OFFSET_DATA(terrain->BSPTreeArray, offset);
        terrain->morphNormalIdx = (short *)OFFSET_DATA(terrain->morphNormalIdx, offset);
        terrain->signals = (MultiSignal *)OFFSET_DATA(terrain->signals, offset);

        for (i = 0; i < terrain->numIntros; i++)
        {
            Intro *intro;

            intro = &terrain->introList[i];

            intro->data = (void *)OFFSET_DATA(intro->data, offset);
            intro->multiSpline = (MultiSpline *)OFFSET_DATA(intro->multiSpline, offset);

            if (intro->multiSpline != NULL)
            {
                MultiSpline *multiSpline;

                multiSpline = intro->multiSpline;

                multiSpline->positional = (Spline *)OFFSET_DATA(multiSpline->positional, offset);
                multiSpline->rotational = (RSpline *)OFFSET_DATA(multiSpline->rotational, offset);
                multiSpline->scaling = (Spline *)OFFSET_DATA(multiSpline->scaling, offset);
                multiSpline->color = (Spline *)OFFSET_DATA(multiSpline->color, offset);

                if (multiSpline->positional != NULL)
                {
                    multiSpline->positional->key = (SplineKey *)OFFSET_DATA(multiSpline->positional->key, offset);
                }

                if (multiSpline->rotational != NULL)
                {
                    multiSpline->rotational->key = (SplineRotKey *)OFFSET_DATA(multiSpline->rotational->key, offset);
                }

                if (multiSpline->scaling != NULL)
                {
                    multiSpline->scaling->key = (SplineKey *)OFFSET_DATA(multiSpline->scaling->key, offset);
                }

                if (multiSpline->color != NULL)
                {
                    multiSpline->color->key = (SplineKey *)OFFSET_DATA(multiSpline->color->key, offset);
                }
            }

            intro->dsignal = (void *)OFFSET_DATA(intro->dsignal, offset);
        }

        if ((terrain->aniList != NULL) && (terrain->aniList->numAniTextues > 0))
        {
            DrMoveAniTexDestInfo **dest;

            dest = &terrain->aniList->aniTexInfo;

            for (d = 0; d < terrain->aniList->numAniTextues; d++)
            {
                dest[d] = (DrMoveAniTexDestInfo *)OFFSET_DATA(dest[d], offset);
            }
        }

        if (level->lightList != NULL)
        {
            level->lightList->lightGroupList = (LightGroup *)OFFSET_DATA(level->lightList->lightGroupList, offset);
        }

        if (level->spectrallightList != NULL)
        {
            level->spectrallightList->lightGroupList = (LightGroup *)OFFSET_DATA(level->spectrallightList->lightGroupList, offset);
        }

        for (i = 0; i < level->numVMObjects; i++)
        {
            VMObject *vmo;

            vmo = &level->vmobjectList[i];

            vmo->vmoffsetTableList = (VMOffsetTable **)OFFSET_DATA(vmo->vmoffsetTableList, offset);

            if (vmo->curVMOffsetTable == vmo->vmoffsetTableList[vmo->currentIdx])
            {
                vmo->curVMOffsetTable = (VMOffsetTable *)OFFSET_DATA(vmo->curVMOffsetTable, offset);
            }

            for (d = 0; d < vmo->numVMOffsetTables; d++)
            {
                vmo->vmoffsetTableList[d] = (VMOffsetTable *)OFFSET_DATA(vmo->vmoffsetTableList[d], offset);
            }

            vmo->vmvertexList = (void *)OFFSET_DATA(vmo->vmvertexList, offset);
            vmo->vminterpolatedList = (VMInterpolated *)OFFSET_DATA(vmo->vminterpolatedList, offset);
            vmo->name = (char *)OFFSET_DATA(vmo->name, offset);
        }

        {
            BSPTree *bsp;
            BSPNode *node;
            BSPLeaf *leaf;

            bsp = terrain->BSPTreeArray;

            for (d = 0; d < terrain->numBSPTrees; d++, bsp++)
            {
                bsp->bspRoot = (BSPNode *)OFFSET_DATA(bsp->bspRoot, offset);
                bsp->startLeaves = (BSPLeaf *)OFFSET_DATA(bsp->startLeaves, offset);
                bsp->endLeaves = (BSPLeaf *)OFFSET_DATA(bsp->endLeaves, offset);

                for (node = bsp->bspRoot; (BSPLeaf *)node < bsp->startLeaves; node++)
                {
                    node->front = (void *)OFFSET_DATA(node->front, offset);
                    node->back = (void *)OFFSET_DATA(node->back, offset);
                }

                for (leaf = bsp->startLeaves; leaf < bsp->endLeaves; leaf++)
                {
                    leaf->faceList = (TFace *)OFFSET_DATA(leaf->faceList, offset);
                }
            }
        }
    }

    for (msignal = level->SignalListStart; msignal < level->SignalListEnd; )
    {
        msignal = SIGNAL_RelocateSignal(msignal, offset);
    }

    EVENT_UpdatePuzzlePointers(level->PuzzleInstances, offset);
    STREAM_UpdateLevelPointer(oldLevel, level, sizeOfLevel);
    EVENT_RelocateInstanceList(oldLevel, level, sizeOfLevel);
}

void MEMPACK_RelocateG2AnimKeylistType(G2AnimKeylist **pKeylist, int offset, char *start, char *end)
{
    int j;
    G2AnimKeylist *keylist;

    if (((char *)*pKeylist >= start) && ((char *)*pKeylist < end))
    {
        keylist = *pKeylist = (G2AnimKeylist *)OFFSET_DATA(*pKeylist, offset);

        if ((keylist->sectionCount != 255) || (keylist->s0TailTime != 224) || (keylist->s1TailTime != 172) || (keylist->s2TailTime != 15))
        {
            keylist->fxList = (G2AnimFxHeader *)OFFSET_DATA(keylist->fxList, offset);

            for (j = 0; j < keylist->sectionCount; j++)
            {
                keylist->sectionData[j] = (unsigned short *)OFFSET_DATA(keylist->sectionData[j], offset);
            }
        }
    }
}

void MEMPACK_RelocateObjectType(MemHeader *newAddress, long offset, Object *oldObject)
{
    Instance *instance;
    Object *object;
    int i;
    int j;
    int d;
    int sizeOfObject;
    Model *model;

    object = (Object *)(newAddress + 1);

    sizeOfObject = newAddress->memSize - sizeof(MemHeader);

    object->modelList = (Model **)OFFSET_DATA(object->modelList, offset);
    object->animList = (G2AnimKeylist **)OFFSET_DATA(object->animList, offset);

    object->soundData = (unsigned char *)OFFSET_DATA(object->soundData, offset);
    object->data = (void *)OFFSET_DATA(object->data, offset);

    object->script = (char *)OFFSET_DATA(object->script, offset);
    object->name = (char *)OFFSET_DATA(object->name, offset);

    object->effectList = (ObjectEffect *)OFFSET_DATA(object->effectList, offset);

    if ((((Object *)(newAddress + 1))->oflags & 0x8000000))
    {
        object->relocList = (unsigned long *)OFFSET_DATA(object->relocList, offset);
        object->relocModule = (void *)OFFSET_DATA(object->relocModule, offset);
    }

    for (i = 0; i < object->numModels; i++)
    {
        object->modelList[i] = (Model *)OFFSET_DATA(object->modelList[i], offset);

        model = object->modelList[i];

        model->vertexList = (MVertex *)OFFSET_DATA(model->vertexList, offset);
        model->normalList = (SVectorNoPad *)OFFSET_DATA(model->normalList, offset);
        model->faceList = (MFace *)OFFSET_DATA(model->faceList, offset);
        model->segmentList = (Segment *)OFFSET_DATA(model->segmentList, offset);

        model->aniTextures = (AniTex *)OFFSET_DATA(model->aniTextures, offset);

        model->multiSpline = (MultiSpline *)OFFSET_DATA(model->multiSpline, offset);

        model->startTextures = (TextureMT3 *)OFFSET_DATA(model->startTextures, offset);
        model->endTextures = (TextureMT3 *)OFFSET_DATA(model->endTextures, offset);

        for (d = 0; d < model->numFaces; d++)
        {
            MFace *mface;

            mface = &model->faceList[d];

            if ((mface->flags & 0x2))
            {
                mface->color = (long)OFFSET_DATA(mface->color, offset);
            }
        }

        for (d = 0; d < model->numSegments; d++)
        {
            Segment *segment;
            HInfo *hInfo;

            segment = &model->segmentList[d];

            segment->hInfo = (HInfo *)OFFSET_DATA(segment->hInfo, offset);

            if (segment->hInfo != NULL)
            {
                hInfo = segment->hInfo;

                hInfo->hfaceList = (HFace *)OFFSET_DATA(hInfo->hfaceList, offset);
                hInfo->hsphereList = (HSphere *)OFFSET_DATA(hInfo->hsphereList, offset);
                hInfo->hboxList = (HBox *)OFFSET_DATA(hInfo->hboxList, offset);
            }
        }

        if (model->aniTextures != NULL)
        {
            AniTexInfo *aniTexInfo;

            aniTexInfo = &model->aniTextures->aniTexInfo;

            for (d = 0; d < model->aniTextures->numAniTextues; d++, aniTexInfo++)
            {
                aniTexInfo->texture = (TextureMT3 *)OFFSET_DATA(aniTexInfo->texture, offset);
            }
        }

        {
            MultiSpline *multiSpline;

            if (model->multiSpline != NULL)
            {
                multiSpline = model->multiSpline;

                multiSpline->positional = (Spline *)OFFSET_DATA(multiSpline->positional, offset);
                multiSpline->rotational = (RSpline *)OFFSET_DATA(multiSpline->rotational, offset);

                multiSpline->scaling = (Spline *)OFFSET_DATA(multiSpline->scaling, offset);

                multiSpline->color = (Spline *)OFFSET_DATA(multiSpline->color, offset);

                if (multiSpline->positional != NULL)
                {
                    multiSpline->positional->key = (SplineKey *)OFFSET_DATA(multiSpline->positional->key, offset);
                }

                if (multiSpline->rotational != NULL)
                {
                    multiSpline->rotational->key = (SplineRotKey *)OFFSET_DATA(multiSpline->rotational->key, offset);
                }

                if (multiSpline->scaling != NULL)
                {
                    multiSpline->scaling->key = (SplineKey *)OFFSET_DATA(multiSpline->scaling->key, offset);
                }

                if (multiSpline->color != NULL)
                {
                    multiSpline->color->key = (SplineKey *)OFFSET_DATA(multiSpline->color->key, offset);
                }
            }
        }
    }

    for (i = 0; i < object->numAnims; i++)
    {
        MEMPACK_RelocateG2AnimKeylistType(&object->animList[i], offset, (char *)oldObject, (char *)oldObject + sizeOfObject);
    }

    if (object->animList != NULL)
    {
        for (instance = gameTrackerX.instanceList->first; instance != NULL; instance = instance->next)
        {
            if (instance->object == oldObject)
            {
                instance->anim.modelData = (Model *)OFFSET_DATA(instance->anim.modelData, offset);
            }

            for (j = 0; j < instance->anim.sectionCount; j++)
            {
                if (((uintptr_t)instance->anim.section[j].keylist >= (uintptr_t)oldObject) && ((uintptr_t)((char *)oldObject + sizeOfObject) >= (uintptr_t)instance->anim.section[j].keylist))
                {
                    instance->anim.section[j].keylist = (G2AnimKeylist *)OFFSET_DATA(instance->anim.section[j].keylist, offset);
                }
            }
        }
    }

    STREAM_UpdateObjectPointer(oldObject, object, sizeOfObject);
}

void MEMPACK_RelocateCDMemory(MemHeader *newAddress, long offset, BigFileDir *oldDir)
{
    BigFileDir *newDir;

    (void)offset;

    newDir = (BigFileDir *)(newAddress + 1);

    LOAD_UpdateBigFilePointers(oldDir, newDir);
}

static char unused1[] = "Verify failed: %s :%s, line %d\n";
static char unused2[] = "Assert failed: %s :%s, line %d\n";
static char unused3[] = "%s\n";
