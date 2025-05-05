#include "Game/PSX/AADSFX.h"
#include "Game/PSX/AADLIB.h"
#include "Game/PSX/AADVOICE.h"

STATIC void (*sfxCmdFunction[9])();

STATIC AadProgramAtr aadDummySfxProgram;

unsigned short aadPitchTable[85];

unsigned long aadStepsPerCent[85];

unsigned long aadPlaySfx(unsigned int toneID, int volume, int pan, int pitchOffset)
{
    unsigned long handle;

    handle = createSfxHandle(toneID);

    aadPutSfxCommand(0, volume, pan, handle, pitchOffset);

    return handle;
}

unsigned long aadStopSfx(unsigned long handle)
{
    aadPutSfxCommand(1, 0, 0, handle, 0);

    return handle;
}

void aadStopAllSfx()
{
    aadPutSfxCommand(4, 0, 0, 0, 0);
}

int aadIsSfxPlaying(unsigned long handle)
{
    AadSynthVoice *voice;
    int i;

    for (i = 0; i < 24; i++)
    {
        voice = &aadMem->synthVoice[i];

        if ((voice->voiceID == 208) && ((aadMem->voiceStatus[i] != 0) && (aadMem->voiceStatus[i] != 2)) && (voice->handle == handle))
        {
            return 1;
        }
    }

    return 0;
}

int aadIsSfxPlayingOrRequested(unsigned long handle)
{
    int commandOut;
    int i;
    AadSfxCommand *sfxCmd;

    EnterCriticalSection();

    commandOut = aadMem->sfxSlot.commandOut;

    for (i = aadMem->sfxSlot.commandsInQueue; i != 0; i--)
    {
        sfxCmd = &aadMem->sfxSlot.commandQueue[commandOut];

        if (sfxCmd->statusByte == 0)
        {
            if (sfxCmd->ulongParam == handle)
            {
                ExitCriticalSection();

                return 1;
            }
        }

        commandOut++;

        if (commandOut == 32)
        {
            commandOut = 0;
        }
    }

    ExitCriticalSection();

    return aadIsSfxPlaying(handle);
}

int aadIsSfxTypePlaying(unsigned int toneID)
{
    AadSynthVoice *voice;
    int i;

    for (i = 0; i < 24; i++)
    {
        voice = &aadMem->synthVoice[i];

        if ((voice->voiceID == 208) && ((unsigned short)voice->handle == (unsigned short)toneID) && ((aadMem->voiceStatus[i] != 0) && (aadMem->voiceStatus[i] != 2)))
        {
            return 1;
        }
    }

    return 0;
}

int aadIsSfxTypePlayingOrRequested(unsigned int sfxToneID)
{
    int commandOut;
    int i;
    AadSfxCommand *sfxCmd;

    EnterCriticalSection();

    commandOut = aadMem->sfxSlot.commandOut;

    for (i = aadMem->sfxSlot.commandsInQueue; i != 0; i--)
    {
        sfxCmd = &aadMem->sfxSlot.commandQueue[commandOut];

        if (sfxCmd->statusByte == 0)
        {
            if ((unsigned short)sfxCmd->ulongParam == sfxToneID)
            {
                ExitCriticalSection();

                return 1;
            }
        }

        commandOut++;

        if (commandOut == 32)
        {
            commandOut = 0;
        }
    }

    ExitCriticalSection();

    return aadIsSfxTypePlaying(sfxToneID);
}

unsigned long aadSetSfxVolPanPitch(unsigned long handle, int volume, int pan, int pitch)
{
    aadPutSfxCommand(3, volume & 0xFF, pan & 0xFF, handle, pitch);

    return handle;
}

unsigned long createSfxHandle(unsigned int toneID)
{
    aadMem->sfxSlot.handleCounter++;

    if (aadMem->sfxSlot.handleCounter == 0)
    {
        aadMem->sfxSlot.handleCounter += 1;
    }

    return (aadMem->sfxSlot.handleCounter << 16) | (unsigned short)toneID;
}

void aadPutSfxCommand(int statusByte, int dataByte0, int dataByte1, unsigned long ulongParam, short shortParam)
{
    AadSfxCommand *sfxCmd;

    if ((aadMem->sfxSlot.commandsInQueue < 30) || (statusByte = 4, (aadMem->sfxSlot.commandsInQueue < 31)))
    {
        EnterCriticalSection();

        sfxCmd = &aadMem->sfxSlot.commandQueue[aadMem->sfxSlot.commandIn];

        sfxCmd->statusByte = statusByte;

        sfxCmd->dataByte[0] = dataByte0;
        sfxCmd->dataByte[1] = dataByte1;

        sfxCmd->ulongParam = ulongParam;
        sfxCmd->shortParam = shortParam;

        if (aadMem->sfxSlot.commandIn++ == 31)
        {
            aadMem->sfxSlot.commandIn = 0;
        }

        aadMem->sfxSlot.commandsInQueue++;

        ExitCriticalSection();
    }
}

void aadExecuteSfxCommand(AadSfxCommand *sfxCmd)
{
    if (sfxCmd->statusByte < 9)
    {
        sfxCmdFunction[sfxCmd->statusByte](sfxCmd);
    }
}

void sfxCmdPlayTone(AadSfxCommand *sfxCmd)
{
    unsigned long handle;
    AadProgramAtr *progAtr;
    AadToneAtr *toneAtr;
    AadSynthVoice *voice;
    unsigned short midiNote;
    unsigned long waveAddr;
    AadLoadedSfxToneAttr *sfxToneAttr;
    AadLoadedSfxWaveAttr *sfxWaveAttr;
    int i;

    handle = sfxCmd->ulongParam;

    i = aadMem->sfxToneMasterList[handle & 0xFFFF];

    if (i < 254)
    {
        sfxToneAttr = &aadMem->sfxToneAttrTbl[i];

        i = aadMem->sfxWaveMasterList[sfxToneAttr->waveID];

        if (i < 254)
        {
            sfxWaveAttr = &aadMem->sfxWaveAttrTbl[i];

            waveAddr = aadGetSramBlockAddr(sfxWaveAttr->sramHandle);

            toneAtr = &sfxToneAttr->toneAttr;

            midiNote = toneAtr->minNote;

            progAtr = &aadDummySfxProgram;

            voice = aadAllocateVoice(sfxToneAttr->toneAttr.priority);

            if (voice != NULL)
            {
                aadPlayTone(toneAtr, waveAddr, progAtr, midiNote, 127, sfxCmd->dataByte[0], sfxCmd->dataByte[1], aadMem->sfxSlot.sfxVolume, aadMem->sfxMasterVol, voice, sfxCmd->shortParam);

                voice->handle = handle;

                voice->voiceID = 208;

                voice->priority = toneAtr->priority;

                voice->note = (unsigned char)midiNote;

                voice->program = toneAtr->parentProgram;

                voice->volume = 127;

                voice->updateVol = sfxCmd->dataByte[0];

                voice->pan = sfxCmd->dataByte[1];

                voice->progAtr = progAtr;
                voice->toneAtr = toneAtr;
            }
        }
    }
}

void sfxCmdStopTone(AadSfxCommand *sfxCmd)
{
    unsigned long handle;
    AadSynthVoice *voice;
    unsigned short i;
    unsigned long vmask;

    vmask = 0;

    handle = sfxCmd->ulongParam;

    for (i = 0; i < 24; i++)
    {
        voice = &aadMem->synthVoice[i];

        if ((voice->voiceID == 208) && (voice->handle == handle) && ((aadMem->voiceStatus[i] != 0) && (aadMem->voiceStatus[i] != 2)))
        {
            voice->voiceID = 0xFF;

            vmask |= voice->voiceMask;
        }
    }

    aadMem->voiceKeyOffRequest |= vmask;
    aadMem->voiceKeyOnRequest &= ~vmask;
}

void sfxCmdStopAllTones(AadSfxCommand *sfxCmd)
{
    AadSynthVoice *voice;
    unsigned long vmask;
    unsigned short i;

    (void)sfxCmd;

    vmask = 0;

    for (i = 0; i < 24; i++)
    {
        voice = &aadMem->synthVoice[i];

        if ((voice->voiceID == 208) && ((aadMem->voiceStatus[i] != 0) && (aadMem->voiceStatus[i] != 2)))
        {
            voice->voiceID = 0xFF;

            vmask |= voice->voiceMask;
        }
    }

    aadMem->voiceKeyOffRequest |= vmask;
    aadMem->voiceKeyOnRequest &= ~vmask;
}

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSFX", sfxCmdSetToneVolumeAndPan);
/* TODO: this function gives build error, further investigate
void sfxCmdSetToneVolPanPitch(AadSfxCommand *sfxCmd)
{
    unsigned long handle;
    AadSynthVoice *voice;
    AadVolume voiceVol;
    unsigned short pitch;
    unsigned short finePitch;
    unsigned short pitchIndex;
    short volumeSquaredLeft;
    short volumeSquaredRight;
    unsigned long volumeSquared;
    unsigned short i;

    handle = sfxCmd->ulongParam;

    for (i = 0; i < 24; i++)
    {
        voice = &aadMem->synthVoice[i];

        if (((voice->voiceID == 208) && (voice->handle == handle)) && ((aadMem->voiceStatus[i] != 0) && (aadMem->voiceStatus[i] != 2)))
        {
            voiceVol.left = voice->volume * voice->volume;
            voiceVol.right = voice->volume * voice->volume;

            if (!(aadMem->flags & 0x1))
            {
                PAN_VOLUME(sfxCmd->dataByte[1], voiceVol.left, voiceVol.right);
            }

            MASTER_VOLUME_SQUARED(voice->toneAtr->volume, voiceVol.left, voiceVol.right);

            if (!(aadMem->flags & 0x1))
            {
                PAN_VOLUME(voice->toneAtr->panPosition, voiceVol.left, voiceVol.right);
            }

            MASTER_VOLUME_SQUARED(sfxCmd->dataByte[0], voiceVol.left, voiceVol.right);

            voice->updateVol = sfxCmd->dataByte[0];

            MASTER_VOLUME_SQUARED(voice->progAtr->volume, voiceVol.left, voiceVol.right);
            MASTER_VOLUME_SQUARED(aadMem->sfxSlot.sfxVolume, voiceVol.left, voiceVol.right);
            MASTER_VOLUME_SQUARED(aadMem->sfxMasterVol, voiceVol.left, voiceVol.right);

            SpuSetVoiceVolume(i, voiceVol.left, voiceVol.right);
            break;
        }
    }
}*/

void sfxCmdSetToneVolPanPitch(AadSfxCommand *sfxCmd)
{
    unsigned long handle;
    AadSynthVoice *voice;
    unsigned short i;
    AadVolume newVoiceVol;
    unsigned short newPitch;
    unsigned short finePitch;
    unsigned short pitchIndex;

    handle = sfxCmd->ulongParam;

    for (i = 0; i < 24; i++)
    {
        voice = &aadMem->synthVoice[i];

        if (((voice->voiceID == 208) && (voice->handle == handle)) && ((aadMem->voiceStatus[i] != 0) && (aadMem->voiceStatus[i] != 2)))
        {
            newVoiceVol.left = voice->volume * voice->volume;
            newVoiceVol.right = voice->volume * voice->volume;

            if (!(aadMem->flags & 0x1))
            {
                PAN_VOLUME(sfxCmd->dataByte[1], newVoiceVol.left, newVoiceVol.right);
            }

            MASTER_VOLUME_SQUARED(voice->toneAtr->volume, newVoiceVol.left, newVoiceVol.right);

            if (!(aadMem->flags & 0x1))
            {
                PAN_VOLUME(voice->toneAtr->panPosition, newVoiceVol.left, newVoiceVol.right);
            }

            MASTER_VOLUME_SQUARED(sfxCmd->dataByte[0], newVoiceVol.left, newVoiceVol.right);

            voice->updateVol = sfxCmd->dataByte[0];

            MASTER_VOLUME_SQUARED(voice->progAtr->volume, newVoiceVol.left, newVoiceVol.right);
            MASTER_VOLUME_SQUARED(aadMem->sfxSlot.sfxVolume, newVoiceVol.left, newVoiceVol.right);
            MASTER_VOLUME_SQUARED(aadMem->sfxMasterVol, newVoiceVol.left, newVoiceVol.right);

            SpuSetVoiceVolume(i, newVoiceVol.left, newVoiceVol.right);

            pitchIndex = (voice->note + 60) - voice->toneAtr->centerNote;

            if ((voice->toneAtr->centerFine & 0x80))
            {
                newPitch = 256 - voice->toneAtr->centerFine;

                finePitch = aadPitchTable[pitchIndex];

                finePitch -= (aadStepsPerCent[pitchIndex] * (newPitch * 100)) >> 23;
            }
            else
            {
                newPitch = voice->toneAtr->centerFine;

                finePitch = aadPitchTable[pitchIndex];

                finePitch += (aadStepsPerCent[pitchIndex] * (newPitch * 100)) >> 23;
            }

            finePitch += sfxCmd->shortParam;

            SpuSetVoicePitch(i, finePitch);
            break;
        }
    }
}

void sfxCmdLockVoice(AadSfxCommand *sfxCmd)
{
    // void (*callbackProc)();
    AadSynthVoice *voice;
    typedef void *(*func)(unsigned long); // not from decls.h
    func callbackProc;                    // not from decls.h

    callbackProc = (func)sfxCmd->ulongParam;

    voice = aadAllocateVoice(255);

    if (voice != NULL)
    {
        voice->flags |= 0x1;

        callbackProc(voice->voiceMask);
    }
    else
    {
        callbackProc(0);
    }
}

void sfxCmdSetVoiceAttr(AadSfxCommand *sfxCmd)
{
    unsigned long v;
    unsigned long vmask;
    SpuVoiceAttr *voiceAttr;

    v = 0;

    voiceAttr = (SpuVoiceAttr *)sfxCmd->ulongParam;

    vmask = 1;

    for (; v < 24; v++)
    {
        if ((vmask & voiceAttr->voice))
        {
            break;
        }

        vmask *= 2;
    }

    SpuSetVoiceVolume(v, voiceAttr->volume.left, voiceAttr->volume.right);
    SpuSetVoicePitch(v, voiceAttr->pitch);
    SpuSetVoiceStartAddr(v, voiceAttr->addr);
    SpuSetVoiceADSR1ADSR2(v, voiceAttr->adsr1, voiceAttr->adsr2);
}

void sfxCmdSetVoiceKeyOn(AadSfxCommand *sfxCmd)
{
    aadMem->voiceKeyOnRequest |= sfxCmd->ulongParam;
}

void sfxCmdSetVoiceKeyOff(AadSfxCommand *sfxCmd)
{
    aadMem->voiceKeyOffRequest |= sfxCmd->ulongParam;
    aadMem->voiceKeyOnRequest &= ~sfxCmd->ulongParam;
}
