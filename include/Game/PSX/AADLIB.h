#ifndef _AADLIB_H_
#define _AADLIB_H_

#include "common.h"

#define MASTER_VOLUME_SQUARED(volume, left, right) \
    { \
    unsigned long masterVolumeSquared; \
    \
    masterVolumeSquared = (volume + 1) * (volume + 1) - 1; \
	left = (left * masterVolumeSquared) >> 14; \
	right = (right * masterVolumeSquared) >> 14; \
    }

#define PAN_VOLUME(pan, left, right) \
{ \
    if ((pan) >= 65) \
    { \
        (left) = ((unsigned long)((right) * ((((0x80 - (pan)) * (0x80 - (pan))) - 1)))  ) >> 12; \
    }  \
    else if ((pan) < 63)  \
    {\
        (right) = (((left) * ((((pan) + 1) * ((pan) + 1)) + 1))  ) >> 12; \
    }\
}

AadMemoryStruct *aadMem;

int aadIsSfxLoaded(unsigned int toneID);
int aadFreeDynamicSfx(int handle);
int aadLoadDynamicSfx(char *fileName, long directoryID, long flags);
void aadStartMusicMasterVolFade(int targetVolume, int volumeStep, void (*fadeCompleteCallback)());
void aadStopAllSlots();
int aadFreeDynamicSoundBank(int dynamicBankIndex);
void aadProcessLoadQueue();
int aadGetNumLoadsQueued();
void (*aadInstallEndSequenceCallback(void (*callbackProc)(), long data))();
int aadAssignDynamicSequence(int bank, int sequenceNumber, int slotNumber);
void aadStartSlot(int slotNumber);
void aadEnableSlot(int slotNumber);
void aadDisableSlot(int slotNumber);
int aadGetSlotStatus(int slotNumber);
void aadStopSlot(int slotNumber);
void aadInitReverb();
void aadShutdownReverb();
void aadCancelPauseSound();
void aadResumeSound();
void aadPauseSound();
void aadSetSfxMasterVolume(int volume);
void aadSetMusicMasterVolume(int volume);
void aadSetUserVariable(int variableNumber, int value);
void aadShutdown();
unsigned long aadGetMemorySize(AadInitAttr *attributes);
int aadInit(AadInitAttr *attributes, unsigned char *memoryPtr);
int aadLoadDynamicSoundBank(char *sndFileName, char *smpFileName, int dynamicBankIndex, int loadOption, void (*retProc)());
unsigned long aadGetSramBlockAddr(int handle);
void aadRelocateSfxMemory(void *oldAddress, int offset);
void aadRelocateMusicMemoryEnd(void *oldAddress, int offset);
void aadRelocateMusicMemoryBegin();
void aadInitVolume();
void aadStartMasterVolumeFade(int targetVolume, int volumeStep, void (*fadeCompleteCallback)());
void aadSetNoUpdateMode(int noUpdate);
void aadAllNotesOff(int slotNumber);
void aadInitSequenceSlot(AadSequenceSlot *slot);
void aadSetSlotTempo(int slotNumber, AadTempo *tempo);
AadTempo *aadGetTempoFromDynamicSequence(int bank, int sequenceNumber, AadTempo *tempo);
int aadWaitForSramTransferComplete();
int aadGetReverbDepth();
void aadLoadDynamicSfxReturn2(void *loadedDataPtr, long loadedDataSize, short status, void *data1, void *data2);
void aadWaveFree(int handle);
void aadMuteChannels(AadSequenceSlot *slot, unsigned long channelList);
void aadUnMuteChannels(AadSequenceSlot *slot, unsigned long channelList);
void aadResumeSlot(int slotNumber);
void aadPauseSlot(int slotNumber);
int aadGetNumDynamicSequences(int bank);
int aadCheckSramFragmented();
void HackCallback();
void aadLoadSingleDynSfx(AadDynamicSfxLoadInfo *info);
unsigned long aadGetReverbSize();
void aadLoadDynamicSfxReturn(void *loadedDataPtr, void *data, void *data2);
void aadFreeSingleDynSfx(int sfxID);
void aadProcessSramDefrag();

#endif
