#include "Game/PSX/AADLIB.h"
#include "Game/PSX/AADSEQEV.h"
#include "Game/PSX/AADSFX.h"

static unsigned short aadHblanksPerUpdate[4] = {262, 131, 312, 156};

static unsigned long aadUpdateRate[4] = {0x411AAAAB, 0x208D5555, 0x4E200000, 0x27100000};

AadMemoryStruct *aadMem = NULL;

static int gDefragRequest = 0;

static char D_800D1178[] = "0123456789";

static char D_800D1184[] = "\\kain2\\area\\%s\\bin\\%s.snf";
static char D_800D11A0[] = "\\kain2\\area\\%s\\bin\\%s.smf";
static char D_800D11BC[] = "\\kain2\\sfx\\object\\%s\\%s.snf";
static char D_800D11D8[] = "\\kain2\\sfx\\object\\%s\\%s.smf";

static unsigned long aadReverbModeSize[10] = {0x00000000, 0x000026C0, 0x00001F40, 0x00004840, 0x00006FE0, 0x00000ADE0, 0x0000F6C0, 0x00018040, 0x00018040, 0x00003C00};

static unsigned long __hblankEvent;

static unsigned char *smfDataPtr;

static unsigned long smfBytesLeft;

static AadDynamicSfxLoadInfo *smfInfo;

unsigned long aadGetMemorySize(AadInitAttr *attributes)
{
    return (1488 * attributes->numSlots) + 7304;
}

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadInit);
/* TODO: need to import aadGp into the project
int aadInit(AadInitAttr *attributes, unsigned char *memoryPtr)
{
    AadSequenceSlot* slot;
    unsigned long size;
    int slotNumber;
    int i;

    aadGp = GetGp();

    EnterCriticalSection();

    size = aadGetMemorySize(attributes);

    aadMem = (AadMemoryStruct*)memoryPtr;

    if (aadMem == NULL)
    {
        return 0x1009;
    }
    else
    {
        memset(memoryPtr, 0, size);

        if ((attributes->nonBlockLoadProc == NULL) || (attributes->nonBlockBufferedLoadProc == NULL) || (attributes->memoryMallocProc == NULL))
        {
            return 0x1008;
        }

        aadMem->nonBlockLoadProc = attributes->nonBlockLoadProc;
        aadMem->nonBlockBufferedLoadProc = attributes->nonBlockBufferedLoadProc;

        aadMem->memoryMallocProc = attributes->memoryMallocProc;
        aadMem->memoryFreeProc = attributes->memoryFreeProc;

        memset(aadMem->sfxToneMasterList, 255, sizeof(aadMem->sfxToneMasterList));
        memset(aadMem->sfxWaveMasterList, 255, sizeof(aadMem->sfxWaveMasterList));

        aadMem->nextSramDescIndex = 1;

        aadMem->sramDescriptorTbl->waveID = 32768;

        aadMem->sramDescriptorTbl->address = 514;

        aadMem->firstSramBlockDescIndex = 0;

        aadMem->sramDescriptorTbl->size = 37336;

        aadMem->sramDescriptorTbl->prevIndex = 255;
        aadMem->sramDescriptorTbl->nextIndex = 255;

        aadPurgeLoadQueue();

        SpuInit();

        SpuSetCommonMasterVolume(0, 0);

        for (slotNumber = 0; slotNumber < attributes->numSlots; slotNumber++)
        {
            slot = aadMem->sequenceSlots[slotNumber] = &((AadSequenceSlot*)&aadMem[1])[slotNumber]; // TODO: doesn't look very natural, reverify

            slot->thisSlotNumber = slotNumber;

            slot->sequenceNumberAssigned = 255;

            slot->slotID = slotNumber * 16;
            slot->slotVolume = 127;
            slot->slotPan = 63;

            slot->masterVolPtr = &aadMem->musicMasterVol;

            for (i = 0; i < 16; i++)
            {
                slot->currentProgram[i] = 255;

                slot->volume[i] = 127;

                slot->panPosition[i] = 63;

                slot->pitchWheel[i] = 8192;
            }

            slot->selectedSlotPtr = slot;
            slot->selectedSlotNum = slotNumber;
        }

        aadMem->sfxSlot.handleCounter = 12345;

        aadMem->sfxSlot.sfxVolume = 127;

        aadMem->numSlots = attributes->numSlots & 0xFF;

        aadMem->updateMode = attributes->updateMode & 0xFF;

        aadMem->sfxMasterVol = 127;
        aadMem->musicMasterVol = 127;

        aadMem->endSequenceCallback = NULL;
        aadMem->controller11Callback = NULL;

        for (i = 0; i < 24; i++)
        {
            aadMem->synthVoice[i].voiceMask = 1 << i;
            aadMem->synthVoice[i].voiceID = 255;
            aadMem->synthVoice[i].voiceNum = i;
        }

        aadMem->voiceKeyOffRequest = 0;
        aadMem->voiceKeyOnRequest = 0;
        aadMem->voiceReverbRequest = 0;

        if (aadMem->updateMode < 4)
        {
            aadInstallUpdateFunc(aadSlotUpdateWrapper, aadHblanksPerUpdate[aadMem->updateMode]);
        }

        aadMem->flags = 0;

        ExitCriticalSection();
    }

    return 0;
}*/

void aadInstallUpdateFunc(long (*updateFuncPtr)(), int hblanksPerUpdate)
{
    EnterCriticalSection();

    __hblankEvent = OpenEvent(0xF2000001, 2, 4096, updateFuncPtr);

    EnableEvent(__hblankEvent);

    SetRCnt(0xF2000001, hblanksPerUpdate, 4096);

    StartRCnt(0xF2000001);

    ExitCriticalSection();
}

void aadInitVolume()
{
    aadMem->masterVolume = 0;

    SpuSetCommonCDMix(0);
    SpuSetCommonMasterVolume(0, 0);
}

void aadSetMasterVolume(int volume)
{
    aadMem->masterVolume = (short)volume;

    SpuSetCommonMasterVolume(volume, volume);
}

void aadStartMasterVolumeFade(int targetVolume, int volumeStep, void (*fadeCompleteCallback)())
{
    aadMem->masterVolFader.volumeStep = volumeStep;

    aadMem->masterVolFader.targetVolume = targetVolume;

    aadMem->masterVolFader.fadeCompleteCallback = fadeCompleteCallback;
}

void aadSetSfxMasterVolume(int volume)
{
    aadMem->sfxMasterVol = volume & 0xFF;
}

void aadSetMusicMasterVolume(int volume)
{
    int slotNumber;

    aadMem->musicMasterVol = volume;

    for (slotNumber = 0; slotNumber < aadMem->numSlots; slotNumber++)
    {
        aadUpdateSlotVolPan(aadMem->sequenceSlots[slotNumber]);
    }
}

void aadStartMusicMasterVolFade(int targetVolume, int volumeStep, void (*fadeCompleteCallback)())
{
    aadMem->musicMasterVolFader.volumeStep = volumeStep;

    aadMem->musicMasterVolFader.targetVolume = targetVolume;

    aadMem->musicMasterVolFader.fadeCompleteCallback = fadeCompleteCallback;
}

void aadShutdown()
{
    EnterCriticalSection();

    StopRCnt(0xF2000001);

    DisableEvent(__hblankEvent);

    CloseEvent(__hblankEvent);

    aadMem->flags |= 0x2;

    ExitCriticalSection();

    SpuSetKey(0, 0xFFFFFF);

    SpuClearReverbWorkArea(aadGetReverbMode());

    SpuQuit();

    aadMem = NULL;
}

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadSlotUpdateWrapper);
/* TODO: need to import aadGp into the project
long aadSlotUpdateWrapper()
{
    unsigned long curGp;

    curGp = GetGp();

    SetGp(aadGp);

    aadSlotUpdate();

    SetGp(curGp);

    return 0;
}*/

void aadSlotUpdate()
{
    AadSequenceSlot *slot;
    AadSeqEvent *seqEventPtr;
    int slotNumber;
    int i;
    int fadeComplete;
    int track;
    int newVol;
    int slotDone;

    if (aadMem == NULL)
    {
        return;
    }

    if (!(aadMem->flags & 0x2))
    {
        unsigned long vmask;

        SpuGetAllKeysStatus(aadMem->voiceStatus);

        i = 0;

        vmask = 1;

        for (; i < 24; i++)
        {
            if (aadMem->voiceStatus[i] == 3)
            {
                aadMem->voiceKeyOffRequest |= vmask;
            }
            else if (((aadMem->voiceKeyOffRequest & vmask)) && ((aadMem->voiceStatus[i] == 0) || (aadMem->voiceStatus[i] == 2)))
            {
                aadMem->voiceKeyOffRequest &= ~vmask;
            }

            vmask <<= 1;
        }

        if (!(aadMem->flags & 0x4))
        {
            for (slotNumber = 0; slotNumber < aadMem->numSlots; slotNumber++)
            {
                slot = aadMem->sequenceSlots[slotNumber];

                if (((slot->status & 0x1)) && (!(slot->slotFlags & 0x1)))
                {
                    slot->tempo.currentTick += slot->tempo.ticksPerUpdate;
                    slot->tempo.currentError += slot->tempo.errorPerUpdate;

                    if (slot->tempo.currentError >= slot->tempo.tickTimeFixed)
                    {
                        slot->tempo.currentError -= slot->tempo.tickTimeFixed;

                        slot->tempo.currentTick++;
                    }

                    for (slotDone = 0; slotDone == 0; )
                    {
                        for (track = 0; track < 16; track++)
                        {
                            if (slot->sequencePosition[track] == NULL)
                            {
                                break;
                            }

                            while (slot->eventsInQueue[track] < 3)
                            {
                                if (aadQueueNextEvent(slot, track) != 0)
                                {
                                    break;
                                }
                            }
                        }

                        slotDone = 1;

                        for (track = 0; track < 16; track++)
                        {
                            if (slot->sequencePosition[track] == NULL)
                            {
                                break;
                            }

                            while (slot->eventsInQueue[track] != 0)
                            {
                                seqEventPtr = &slot->eventQueue[slot->eventOut[track]][track];

                                if (slot->tempo.currentTick < (seqEventPtr->deltaTime + slot->lastEventExecutedTime[track]))
                                {
                                    break;
                                }

                                slot->lastEventExecutedTime[track] += seqEventPtr->deltaTime;

                                slot->eventsInQueue[track]--;
                                slot->eventOut[track]++;

                                if (slot->eventOut[track] == 4)
                                {
                                    slot->eventOut[track] = 0;
                                }

                                aadExecuteEvent(seqEventPtr, slot);

                                slotDone = 0;
                            }
                        }
                    }
                }
            }
        }

        while (aadMem->sfxSlot.commandsInQueue != 0)
        {
            newVol = 32;

            aadExecuteSfxCommand(&aadMem->sfxSlot.commandQueue[aadMem->sfxSlot.commandOut]);

            aadMem->sfxSlot.commandsInQueue--;

            if (++aadMem->sfxSlot.commandOut == newVol)
            {
                aadMem->sfxSlot.commandOut = 0;
            }
        }

        aadMem->voiceKeyOffRequest &= ~aadMem->voiceKeyOnRequest;

        if (aadMem->voiceKeyOffRequest != 0)
        {
            SpuSetKey(0, aadMem->voiceKeyOffRequest);
        }

        SpuSetReverbVoice(1, aadMem->voiceReverbRequest);
        SpuSetReverbVoice(0, ~aadMem->voiceReverbRequest);

        if (aadMem->voiceKeyOnRequest != 0)
        {
            SpuSetKey(1, aadMem->voiceKeyOnRequest);

            aadMem->voiceKeyOnRequest = 0;
        }
    }


    if (aadMem->masterVolFader.volumeStep != 0)
    {
        fadeComplete = 0;

        newVol = aadMem->masterVolume;

        newVol += aadMem->masterVolFader.volumeStep;

        if (aadMem->masterVolFader.volumeStep < 0)
        {
            fadeComplete = newVol < aadMem->masterVolFader.targetVolume;
        }
        else if (newVol > aadMem->masterVolFader.targetVolume)
        {
            fadeComplete = 1;
        }

        if (fadeComplete != 0)
        {
            newVol = aadMem->masterVolFader.targetVolume;

            aadMem->masterVolFader.volumeStep = 0;

            if (aadMem->masterVolFader.fadeCompleteCallback != NULL)
            {
                aadMem->masterVolFader.fadeCompleteCallback();
            }
        }

        aadSetMasterVolume(newVol);
    }

    if ((aadMem->musicMasterVolFader.volumeStep != 0) && (!(aadMem->updateCounter & 0x1)))
    {
        fadeComplete = 0;

        newVol = aadMem->musicMasterVol;

        newVol += aadMem->musicMasterVolFader.volumeStep;

        if (aadMem->musicMasterVolFader.volumeStep < 0)
        {
            fadeComplete = newVol <= aadMem->musicMasterVolFader.targetVolume;
        }
        else if (newVol >= aadMem->musicMasterVolFader.targetVolume)
        {
            fadeComplete = 1;
        }

        if (fadeComplete != 0)
        {
            newVol = aadMem->musicMasterVolFader.targetVolume;

            aadMem->musicMasterVolFader.volumeStep = 0;

            if (aadMem->musicMasterVolFader.fadeCompleteCallback != NULL)
            {
                aadMem->musicMasterVolFader.fadeCompleteCallback();
            }
        }

        aadSetMusicMasterVolume(newVol);
    }

    aadMem->updateCounter++;
}

unsigned long aadCreateFourCharID(char a, char b, char c, char d)
{
    return d | (c << 8) | (b << 16) | (a << 24);
}

void aadLoadDynamicSoundBankReturn(void *loadedDataPtr, void *data, void *data2);
int aadLoadDynamicSoundBank(char *sndFileName, char *smpFileName, int dynamicBankIndex, int loadOption, void (*retProc)())
{
    struct AadDynamicBankLoadInfo *info = &aadMem->dynamicBankLoadInfo;
    int i;
    for (i = 0; i < 2; i++)
    {
        if (aadMem->dynamicBankStatus[i] == 1)
        {
            return 0x1006;
        }
    }

    if (dynamicBankIndex >= 2)
    {
        return 0x1005;
    }

    if (aadMem->dynamicBankStatus[dynamicBankIndex] == 2 && aadMem->dynamicSoundBankData[dynamicBankIndex] != NULL)
    {
        aadFreeDynamicSoundBank(dynamicBankIndex);
    }

    aadMem->dynamicBankStatus[dynamicBankIndex] = 1;

    strncpy(info->sndFileName, sndFileName, sizeof(info->sndFileName) - 1);
    strncpy(info->smpFileName, smpFileName, sizeof(info->smpFileName) - 1);
    info->dynamicBankIndex = dynamicBankIndex;
    info->loadOption = loadOption;
    info->userCallbackProc = retProc;
    info->flags = 0;

    aadMem->nonBlockLoadProc(sndFileName, (void *)&aadLoadDynamicSoundBankReturn, info, NULL, (void **)&aadMem->dynamicSoundBankData[dynamicBankIndex], 4);

    return 0;
}

int aadOpenDynamicSoundBank(unsigned char *soundBank, int dynamicBankIndex);
void aadLoadDynamicSoundBankReturn2(void *loadedDataPtr, long loadedDataSize, short status, void *data1, void *data2);
void aadLoadDynamicSoundBankReturn(void *loadedDataPtr, void *data, void *data2)
{
    int dynamicBankIndex;
    int error;

    (void)data2;

    dynamicBankIndex = ((AadDynamicBankLoadInfo *)data)->dynamicBankIndex;

    error = aadOpenDynamicSoundBank((unsigned char *)loadedDataPtr, dynamicBankIndex);

    if (error != 0)
    {
        aadMem->dynamicBankStatus[dynamicBankIndex] = (error | 0x80) & 0xFF;

        if (aadMem->dynamicSoundBankData[dynamicBankIndex] != NULL)
        {
            aadMem->memoryFreeProc((char *)aadMem->dynamicSoundBankData[dynamicBankIndex]);

            aadMem->dynamicSoundBankData[dynamicBankIndex] = NULL;
        }

        if (((AadDynamicBankLoadInfo *)data)->userCallbackProc != NULL)
        {
            ((AadDynamicBankLoadInfo *)data)->userCallbackProc(dynamicBankIndex & 0xFFFF, error);
        }
    }
    else
    {
        aadMem->nonBlockBufferedLoadProc(((AadDynamicBankLoadInfo *)data)->smpFileName, (void *)aadLoadDynamicSoundBankReturn2, data, NULL);
    }
}

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadLoadDynamicSoundBankReturn2);

int aadFreeDynamicSoundBank(int dynamicBankIndex)
{
    if (dynamicBankIndex >= 2)
    {
        return 0x1005;
    }

    if (aadMem->dynamicBankStatus[dynamicBankIndex] != 2)
    {
        return 0x1007;
    }

    if (aadMem->dynamicSoundBankData[dynamicBankIndex] == NULL)
    {
        return 0x1007;
    }

    aadMem->dynamicBankStatus[dynamicBankIndex] = 0;

    aadMem->memoryFreeProc(aadMem->dynamicSoundBankData[dynamicBankIndex]);

    aadMem->dynamicSoundBankData[dynamicBankIndex] = NULL;

    return 0;
}

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadOpenDynamicSoundBank);

int aadLoadDynamicSfx(char *fileName, long directoryID, long flags)
{
    AadDynamicLoadRequest *loadReq;

    if (aadMem->numLoadReqsQueued < 16)
    {
        loadReq = &aadMem->loadRequestQueue[aadMem->nextLoadReqIn];

        loadReq->type = 0;

        loadReq->handle = (aadMem->nextFileHandle++ & 0x3FFF) | 0x4000;

        loadReq->directoryID = directoryID;

        loadReq->flags = flags;

        strncpy(loadReq->fileName, fileName, sizeof(loadReq->fileName) - 1);

        aadMem->nextLoadReqIn = (aadMem->nextLoadReqIn + 1) & 0xF;

        aadMem->numLoadReqsQueued++;

        return loadReq->handle;
    }

    return 0;
}

int aadFreeDynamicSfx(int handle)
{
    AadDynamicLoadRequest *loadReq;
    int i;

    for (i = aadMem->nextLoadReqOut; i != aadMem->nextLoadReqIn; i = (i + 1) & 0xF)
    {
        loadReq = &aadMem->loadRequestQueue[i];

        if ((loadReq->type == 0) && (loadReq->handle == handle))
        {
            loadReq->type = 2;

            return 0;
        }
    }

    if (aadMem->numLoadReqsQueued >= 16)
    {
        return 0x100F;
    }
    else
    {
        loadReq = &aadMem->loadRequestQueue[(aadMem->nextLoadReqOut - 1) & 0xF];

        aadMem->nextLoadReqOut = (aadMem->nextLoadReqOut - 1) & 0xF;

        loadReq->type = 1;

        loadReq->handle = handle;

        aadMem->numLoadReqsQueued++;
    }

    return 0;
}

void aadRelocateMusicMemoryBegin()
{
    aadMem->flags |= 0x2;
}

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadRelocateMusicMemoryEnd);

void aadRelocateSfxMemory(void *oldAddress, int offset)
{
    AadDynSfxSnfFileHdr *snfFile;

    snfFile = aadMem->firstDynSfxFile;

    if ((char *)oldAddress == (char *)snfFile)
    {
        snfFile = (AadDynSfxSnfFileHdr *)((char *)snfFile + offset);

        aadMem->firstDynSfxFile = snfFile;
    }

    for (; snfFile != NULL; snfFile = snfFile->nextDynSfxFile)
    {
        if ((char *)oldAddress == (char *)snfFile->prevDynSfxFile)
        {
            snfFile->prevDynSfxFile = (AadDynSfxSnfFileHdr *)((char *)oldAddress + offset);
        }

        if ((char *)oldAddress == (char *)snfFile->nextDynSfxFile)
        {
            snfFile->nextDynSfxFile = (AadDynSfxSnfFileHdr *)((char *)oldAddress + offset);
        }
    }
}

int aadGetNumLoadsQueued()
{
    return aadMem->numLoadReqsQueued;
}

void aadPurgeLoadQueue()
{
    aadMem->nextLoadReqIn = 0;
    aadMem->nextLoadReqOut = 0;

    aadMem->numLoadReqsQueued = 0;
}

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadProcessLoadQueue);

void aadLoadDynamicSfxAbort(AadDynamicSfxLoadInfo *info, int error)
{
    (void)error;

    if (info->snfFile != NULL)
    {
        if ((info->flags & 0x2))
        {
            if (info->snfFile->prevDynSfxFile != NULL)
            {
                info->snfFile->prevDynSfxFile->nextDynSfxFile = NULL;
            }
            else
            {
                aadMem->firstDynSfxFile = NULL;
            }
        }

        aadMem->memoryFreeProc(info->snfFile);
    }

    info->flags = 0;
}

void aadLoadDynamicSfxDone(AadDynamicSfxLoadInfo *info)
{
    info->flags = 0;
}

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadLoadDynamicSfxReturn);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadWaveMalloc);

unsigned long aadGetSramBlockAddr(int handle)
{
    AadNewSramBlockDesc *sramDesc;

    if (handle < 128)
    {
        sramDesc = &aadMem->sramDescriptorTbl[handle];

        return sramDesc->address << 3;
    }

    return 0;
}

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadWaveFree);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadFreeSingleDynSfx);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", setSramFullAlarm);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadLoadSingleDynSfx);

void HackCallback()
{
    SpuSetTransferCallback(NULL);

    aadLoadDynamicSfxReturn2(smfDataPtr, smfBytesLeft, 0, smfInfo, NULL);
}

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadLoadDynamicSfxReturn2);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadCheckSramFragmented);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadProcessSramDefrag);

int aadIsSfxLoaded(unsigned int toneID)
{
    toneID = aadMem->sfxToneMasterList[toneID];

    if (toneID < 254)
    {
        return 1;
    }
    else if (toneID == 254)
    {
        return -1;
    }

    return 0;
}

void aadInitSequenceSlot(AadSequenceSlot *slot)
{
    AadSequenceHdr *seqHdr;
    // unsigned long trackOffset; // unused
    int i;
    int bank;

    bank = slot->sequenceAssignedDynamicBank;

    slot->slotFlags &= 0x1;

    slot->status = 0;

    slot->selectedDynamicBank = bank;

    seqHdr = (AadSequenceHdr *)aadMem->dynamicSequenceAddressTbl[bank][slot->sequenceNumberAssigned];

    for (i = 0; i < 16; i++)
    {
        if (i < seqHdr->numTracks)
        {
            slot->sequencePosition[i] = (unsigned char *)seqHdr + ((int *)seqHdr)[i + 4];
        }
        else
        {
            slot->sequencePosition[i] = NULL;
        }

        slot->trackFlags[i] = 0;

        slot->loopCurrentNestLevel[i] = 0;

        slot->eventsInQueue[i] = 0;

        slot->eventIn[i] = 0;
        slot->eventOut[i] = 0;

        slot->trackFlags[i] |= 0x20;
    }

    for (i = 0; i < 16; i++)
    {
        slot->currentDynamicBank[i] = slot->sequenceAssignedDynamicBank;
        slot->currentProgram[i] = 255;

        slot->volume[i] = 127;

        slot->panPosition[i] = 63;

        slot->pitchWheel[i] = 8192;
    }

    slot->selectedSlotPtr = slot;

    slot->delayedMuteMode = 0;
    slot->delayedMuteCmds = 0;
    slot->delayedUnMuteCmds = 0;

    slot->selectedSlotNum = slot->thisSlotNumber;
}

int aadWaitForSramTransferComplete()
{
    int n;

    for (n = 100000; n != 0; )
    {
        if (SpuIsTransferCompleted(0) != 0)
        {
            return 1;
        }

        n--;
    }

    return 0;
}

void aadInitReverb()
{
    SpuSetReverbModeType(aadGetReverbMode());
    SpuSetReverbVoice(0, 0xFFFFFF);

    if (aadWaitForSramTransferComplete() != 0)
    {
        SpuClearReverbWorkArea(aadGetReverbMode());
    }

    SpuSetReverbModeDepth(aadGetReverbDepth(), aadGetReverbDepth());
    SpuSetReverb(1);
}

void aadShutdownReverb()
{
    if (aadWaitForSramTransferComplete() != 0)
    {
        SpuClearReverbWorkArea(aadGetReverbMode());
    }
}

int aadGetReverbMode()
{
    return 3;
}

unsigned long aadGetReverbSize()
{
    return aadReverbModeSize[aadGetReverbMode()] + 64;
}

int aadGetReverbDepth()
{
    return 10000;
}

int aadGetNumDynamicSequences(int bank)
{
    if (aadMem->dynamicBankStatus[bank] == 2)
    {
        return aadMem->dynamicSoundBankHdr[bank]->numSequences;
    }

    return 0;
}

int aadAssignDynamicSequence(int bank, int sequenceNumber, int slotNumber)
{
    AadTempo tempo;
    AadSequenceSlot *slot;
    int i;

    if (aadMem->dynamicBankStatus[bank] != 2)
    {
        return 0x1007;
    }

    slot = aadMem->sequenceSlots[slotNumber];

    slot->sequenceNumberAssigned = sequenceNumber;

    slot->sequenceAssignedDynamicBank = bank;

    aadInitSequenceSlot(slot);

    aadAllNotesOff(slotNumber);

    if (slot->tempo.ticksPerUpdate == 0)
    {
        aadSetSlotTempo(slotNumber, aadGetTempoFromDynamicSequence(bank, sequenceNumber, &tempo));
    }

    slot->channelMute = 0;

    slot->enableSustainUpdate = 0;

    slot->ignoreTranspose = 0;

    for (i = 0; i < 16; i++)
    {
        slot->transpose[i] = 0;
    }

    return 0;
}

AadTempo *aadGetTempoFromDynamicSequence(int bank, int sequenceNumber, AadTempo *tempo)
{
    AadSequenceHdr *seqHdr;

    if (aadMem->dynamicBankStatus[bank] == 2)
    {
        seqHdr = (AadSequenceHdr *)aadMem->dynamicSequenceAddressTbl[bank][sequenceNumber];

        tempo->quarterNoteTime = seqHdr->quarterNoteTime;

        tempo->ppqn = seqHdr->ppqn;
    }

    return tempo;
}

void aadSetSlotTempo(int slotNumber, AadTempo *tempo)
{
    AadSequenceSlot *slot;
    unsigned long tickTime;
    // unsigned long tickTimeRemainder; // unused

    slot = aadMem->sequenceSlots[slotNumber];

    tickTime = ((tempo->quarterNoteTime / tempo->ppqn) << 16) + (((tempo->quarterNoteTime % tempo->ppqn) << 16) / tempo->ppqn);

    slot->tempo.tickTimeFixed = tickTime;

    slot->tempo.ticksPerUpdate = aadUpdateRate[aadMem->updateMode & 0x3] / tickTime;
    slot->tempo.errorPerUpdate = aadUpdateRate[aadMem->updateMode & 0x3] % slot->tempo.tickTimeFixed;

    slot->tempo.quarterNoteTime = tempo->quarterNoteTime;

    slot->tempo.ppqn = tempo->ppqn;
}

void aadStartSlot(int slotNumber)
{
    AadSequenceSlot *slot;

    if (slotNumber < aadMem->numSlots)
    {
        slot = aadMem->sequenceSlots[slotNumber];

        if ((!(slot->status & 0x1)) && (slot->sequenceNumberAssigned != 255))
        {
            aadInitSequenceSlot(slot);

            slot->status |= 0x1;
        }
    }
}

void aadStopSlot(int slotNumber)
{
    AadSequenceSlot *slot;

    if (slotNumber < aadMem->numSlots)
    {
        slot = aadMem->sequenceSlots[slotNumber];

        if (slot->sequenceNumberAssigned != 255)
        {
            slot->status &= ~0x1;

            aadInitSequenceSlot(slot);

            aadAllNotesOff(slotNumber);
        }
    }
}

void aadStopAllSlots()
{
    AadSequenceSlot *slot;
    int slotNumber;

    for (slotNumber = 0; slotNumber < aadMem->numSlots; slotNumber++)
    {
        slot = aadMem->sequenceSlots[slotNumber];

        if ((slot->status & 0x1))
        {
            aadStopSlot(slotNumber);
        }

        slot->sequenceNumberAssigned = 255;
    }
}

void aadDisableSlot(int slotNumber)
{
    if (slotNumber < aadMem->numSlots)
    {
        aadMem->sequenceSlots[slotNumber]->slotFlags |= 0x1;

        aadAllNotesOff(slotNumber);
    }
}

void aadEnableSlot(int slotNumber)
{
    if (slotNumber < aadMem->numSlots)
    {
        aadMem->sequenceSlots[slotNumber]->slotFlags &= ~0x1;
    }
}

void aadPauseSlot(int slotNumber)
{
    if (slotNumber < aadMem->numSlots)
    {
        aadMem->sequenceSlots[slotNumber]->status &= ~0x1;

        aadAllNotesOff(slotNumber);
    }
}

void aadResumeSlot(int slotNumber)
{
    AadSequenceSlot *slot;
    int track;

    if (slotNumber < aadMem->numSlots)
    {
        slot = aadMem->sequenceSlots[slotNumber];

        if (slot->sequenceNumberAssigned != 255)
        {
            for (track = 0; track < 16; track++)
            {
                slot->trackFlags[track] |= 0x20;
            }

            slot->status |= 0x1;
        }
    }
}

int aadGetSlotStatus(int slotNumber)
{
    return aadMem->sequenceSlots[slotNumber]->status;
}

void aadAllNotesOff(int slotNumber)
{
    AadSynthVoice *voice;
    unsigned long vmask;
    int i;
    AadSequenceSlot *slot;

    vmask = 0;

    slot = aadMem->sequenceSlots[slotNumber];

    for (i = 0; i < 24; i++)
    {
        voice = &aadMem->synthVoice[i];

        if ((voice->voiceID & 0xF0) == slot->slotID)
        {
            voice->voiceID = 255;

            vmask |= voice->voiceMask;

            voice->flags |= 0x2;
        }
    }

    if (vmask != 0)
    {
        aadMem->voiceKeyOffRequest |= vmask;
        aadMem->voiceKeyOnRequest &= ~vmask;
    }
}

void aadMuteChannels(AadSequenceSlot *slot, unsigned long channelList)
{
    AadSynthVoice *voice;
    unsigned long vmask;
    unsigned long delayedMute;
    int channel;
    int i;

    delayedMute = slot->delayedMuteMode & channelList;

    if (delayedMute != 0)
    {
        slot->delayedMuteCmds |= delayedMute;
        channelList &= ~delayedMute;
    }

    vmask = 0;

    slot->channelMute |= channelList;

    for (channel = 0; channel < 16; channel++)
    {
        if ((channelList & (1 << channel)))
        {
            for (i = 0; i < 24; i++)
            {
                voice = &aadMem->synthVoice[i];

                if (voice->voiceID == (slot->slotID | channel))
                {
                    voice->voiceID = 255;
                    vmask |= voice->voiceMask;
                }
            }
        }
    }

    if (vmask != 0)
    {
        aadMem->voiceKeyOffRequest |= vmask;
        aadMem->voiceKeyOnRequest &= ~vmask;
    }
}

void aadUnMuteChannels(AadSequenceSlot *slot, unsigned long channelList)
{
    unsigned long delayedUnMute;

    delayedUnMute = slot->delayedMuteMode & channelList;

    if (delayedUnMute != 0)
    {
        slot->delayedUnMuteCmds |= delayedUnMute;

        channelList &= ~delayedUnMute;
    }

    slot->channelMute &= ~channelList;
}

void (*aadInstallEndSequenceCallback(void (*callbackProc)(), long data))()
{
    void (*previousCallbackProc)();

    previousCallbackProc = (void *)aadMem->endSequenceCallback;

    aadMem->endSequenceCallback = callbackProc;
    aadMem->endSequenceCallbackData = data;

    return previousCallbackProc;
}

void aadSetUserVariable(int variableNumber, int value)
{
    aadMem->userVariables[variableNumber] = value;
}

void aadSetNoUpdateMode(int noUpdate)
{
    if (noUpdate != 0)
    {
        aadMem->flags |= 0x2;
    }
    else
    {
        aadMem->flags &= ~0x2;
    }
}

void aadPauseSound()
{
    int i;

    if (!(aadMem->flags & 0x8))
    {
        aadMem->flags |= 0xC;

        for (i = 0; i < 24; i++)
        {
            aadMem->synthVoice[i].flags &= ~0x2;

            SpuGetVoicePitch(i, &aadMem->voicePitchSave[i]);

            SpuSetVoicePitch(i, 0);
        }
    }
}

void aadCancelPauseSound()
{
    aadMem->flags &= ~0xC;
}

void aadResumeSound()
{
    int i;

    if ((aadMem->flags & 0x8))
    {
        aadMem->flags &= ~0xC;

        for (i = 0; i < 24; i++)
        {
            if (!(aadMem->synthVoice[i].flags & 0x2))
            {
                SpuSetVoicePitch(i, aadMem->voicePitchSave[i]);
            }
        }
    }
}
