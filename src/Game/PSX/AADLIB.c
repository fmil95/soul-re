#include "Game/PSX/AADLIB.h"
#include "Game/PSX/AADSEQEV.h"

AadMemoryStruct *aadMem;

STATIC unsigned long __hblankEvent;

unsigned long aadGetMemorySize(AadInitAttr *attributes)
{
    return (1488 * attributes->numSlots) + 7304;
}

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadInit);

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

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadSlotUpdate);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadCreateFourCharID);

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

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadFreeDynamicSoundBank);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadOpenDynamicSoundBank);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadLoadDynamicSfx);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadFreeDynamicSfx);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadRelocateMusicMemoryBegin);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadRelocateMusicMemoryEnd);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadRelocateSfxMemory);

int aadGetNumLoadsQueued()
{
    return aadMem->numLoadReqsQueued;
}

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadPurgeLoadQueue);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadProcessLoadQueue);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadLoadDynamicSfxAbort);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadLoadDynamicSfxDone);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadLoadDynamicSfxReturn);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadWaveMalloc);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadGetSramBlockAddr);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadWaveFree);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadFreeSingleDynSfx);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", setSramFullAlarm);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadLoadSingleDynSfx);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", HackCallback);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadLoadDynamicSfxReturn2);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadCheckSramFragmented);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadProcessSramDefrag);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadIsSfxLoaded);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadInitSequenceSlot);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadWaitForSramTransferComplete);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadInitReverb);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadShutdownReverb);

int aadGetReverbMode()
{
    return 3;
}

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadGetReverbSize);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadGetReverbDepth);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadGetNumDynamicSequences);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadAssignDynamicSequence);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadGetTempoFromDynamicSequence);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadSetSlotTempo);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadStartSlot);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadStopSlot);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadStopAllSlots);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadDisableSlot);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadEnableSlot);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadPauseSlot);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadResumeSlot);

int aadGetSlotStatus(int slotNumber)
{
    return aadMem->sequenceSlots[slotNumber]->status;
}

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADLIB", aadAllNotesOff);

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
