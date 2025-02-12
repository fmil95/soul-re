#include "Game/PSX/AADVOICE.h"
#include "Game/PSX/AADLIB.h"

unsigned short aadPitchTable[85];

unsigned long aadStepsPerCent[85];

short aadStepsPerSemitone[85];

AadSynthVoice *aadAllocateVoice(int priority)
{
    int i;
    int lowestPriSus;
    int lowestPriRel;
    AadSynthVoice *lowestPriSusVoice;
    AadSynthVoice *lowestPriRelVoice;
    AadSynthVoice *voice;

    lowestPriRel = 0x7FFF;
    lowestPriSus = 0x7FFF;

    lowestPriRelVoice = NULL;
    lowestPriSusVoice = NULL;

    for (i = 0; i < 24; i++)
    {
        voice = &aadMem->synthVoice[i];

        if (!(voice->flags & 0x1))
        {
            if (aadMem->voiceStatus[i] == 0)
            {
                aadMem->voiceStatus[i] = 1;

                voice->flags |= 0x2;

                return voice;
            }
            else
            {
                if (aadMem->voiceStatus[i] == 2)
                {
                    if (voice->priority < lowestPriRel)
                    {
                        lowestPriRel = voice->priority;

                        lowestPriRelVoice = voice;
                    }
                }
                else if (voice->priority < lowestPriSus)
                {
                    lowestPriSus = voice->priority;

                    lowestPriSusVoice = voice;
                }
            }
        }
    }

    if (priority >= lowestPriRel)
    {
        lowestPriRelVoice->flags |= 0x2;

        return lowestPriRelVoice;
    }

    if (priority >= lowestPriSus)
    {
        lowestPriSusVoice->flags |= 0x2;

        return lowestPriSusVoice;
    }

    return NULL;
}

void SpuSetVoiceADSR1ADSR2(int vNum, unsigned short adsr1, unsigned short adsr2)
{
    unsigned short sl;
    unsigned short dr;
    unsigned short ar;
    unsigned short arm;
    unsigned short rr;
    unsigned short rrm;
    unsigned short sr;
    unsigned short srm;

    sl = adsr1 & 0xF;
    dr = (adsr1 >> 4) & 0xF;
    ar = (adsr1 >> 8) & 0x7F;

    arm = 1;

    if ((adsr1 & 0x8000))
    {
        arm = 5;
    }

    rr = adsr2 & 0x1F;

    rrm = 3;

    if ((adsr2 & 0x20))
    {
        rrm = 7;
    }

    sr = (adsr2 >> 6) & 0x7F;

    if ((adsr2 & 0x4000))
    {
        if ((adsr2 & 0x8000))
        {
            srm = 7;
        }
        else
        {
            srm = 3;
        }
    }
    else if ((adsr2 & 0x8000))
    {
        srm = 5;
    }
    else
    {
        srm = 1;
    }

    SpuSetVoiceADSRAttr(vNum, ar, dr, sr, rr, sl, arm, srm, rrm);
}

void aadPlayTone(AadToneAtr *toneAtr, unsigned long waveStartAddr, AadProgramAtr *progAtr, int midiNote, int volume, int masterVolume, int masterPan, int slotVolume, int masterMasterVol, AadSynthVoice *voice, int pitchOffset)
{
    AadVolume voiceVol; // stack offset -32
    int pitch; // $a1
    int finePitch; // $a1
    int pitchIndex; // $a3

    voiceVol.right = ((volume + 1) * (volume + 1)) - 1;
    voiceVol.left = ((volume + 1) * (volume + 1)) - 1;

    if (!(aadMem->flags & 0x1))
    {
        if (masterPan >= 0x41)
        {
            voiceVol.left = (unsigned int)((short)((((volume + 1) * (volume + 1)) - 1)) * ((((0x80 - masterPan) * (0x80 - masterPan))) - 1)) >> 12;
        }
        else if (masterPan < 0x3F)
        {
            voiceVol.right = ((short)((((volume + 1) * (volume + 1)) - 1)) * ((((masterPan + 1) * (masterPan + 1))) + 1)) >> 12;
        }
    }

    MASTER_VOLUME_SQUARED(toneAtr->volume, voiceVol.left, voiceVol.right);

    if (!(aadMem->flags & 0x1))
    {
        if (toneAtr->panPosition >= 65)
        {
            voiceVol.left = (unsigned int)(voiceVol.right * (((0x80 - toneAtr->panPosition) * (0x80 - toneAtr->panPosition)) - 1)) >> 12;
        }
        else if (toneAtr->panPosition < 63)
        {
            voiceVol.right = (voiceVol.left * (((toneAtr->panPosition + 1) * (toneAtr->panPosition + 1)) + 1)) >> 12;
        }
    }

    MASTER_VOLUME_SQUARED(masterVolume, voiceVol.left, voiceVol.right);
    MASTER_VOLUME_SQUARED(progAtr->volume, voiceVol.left, voiceVol.right);
    MASTER_VOLUME_SQUARED(slotVolume, voiceVol.left, voiceVol.right);
    MASTER_VOLUME_SQUARED(masterMasterVol, voiceVol.left, voiceVol.right);

    SpuSetVoiceVolume(voice->voiceNum, voiceVol.left, voiceVol.right);

    pitchIndex = midiNote  + 60 - toneAtr->centerNote;

    if (toneAtr->centerFine & 0x80)
    {
        pitch = 0x100 - toneAtr->centerFine;
        finePitch = aadPitchTable[pitchIndex];
        finePitch -= ((aadStepsPerCent[pitchIndex] * (pitch * 100)) >> 23);
    }
    else
    {
        pitch = toneAtr->centerFine;
        finePitch = aadPitchTable[pitchIndex];
        finePitch += ((aadStepsPerCent[pitchIndex] * (pitch * 100)) >> 23);
    }

    finePitch += pitchOffset;
    SpuSetVoicePitch(voice->voiceNum, finePitch & 0xFFFF);
    SpuSetVoiceStartAddr(voice->voiceNum, waveStartAddr);
    SpuSetVoiceADSR1ADSR2(voice->voiceNum, toneAtr->adsr1, toneAtr->adsr2);

    if (toneAtr->mode == 4)
    {
        aadMem->voiceReverbRequest |= voice->voiceMask;
    }
    else
    {
        aadMem->voiceReverbRequest = aadMem->voiceReverbRequest & ~voice->voiceMask;
    }

    aadMem->voiceKeyOnRequest |= voice->voiceMask;
}

void aadPlayTonePitchBend(struct AadToneAtr *toneAtr, unsigned long waveStartAddr, struct AadProgramAtr *progAtr, int midiNote, int volume, int masterVolume, int masterPan, int slotVolume, int masterMasterVol, struct AadSynthVoice *voice, int pitchOffset)
{
    //struct AadVolume voiceVol; // stack offset -32
    //int pitch; // $a1
    //int finePitch; // $a1
    //int pitchIndex; // $a3

    struct AadVolume voiceVol;         // stack offset -32
    unsigned int pitch;                         // $a1
    unsigned int finePitch;                     // $a1
    unsigned int pitchIndex;                    // $a3
    unsigned long tmp;                 // $v0 line 10
    unsigned long pitchOffset2;
    int temp_lo;
    //unsigned long tmp;                 // $v0 line 12

    voiceVol.right = ((volume + 1) * (volume + 1)) - 1;
    voiceVol.left = ((volume + 1) * (volume + 1)) - 1;

    if (!(aadMem->flags & 0x1))
    {
        if (masterPan >= 0x41)
        {
            voiceVol.left = (unsigned int)((short)((((volume + 1) * (volume + 1)) - 1)) * ((((0x80 - masterPan) * (0x80 - masterPan))) - 1)) >> 12;
        }
        else if (masterPan < 0x3F)
        {
            voiceVol.right = ((short)((((volume + 1) * (volume + 1)) - 1)) * ((((masterPan + 1) * (masterPan + 1))) + 1)) >> 12;
        }
    }

    {
        unsigned long masterVolumeSquared; // $v1 line 11

        masterVolumeSquared = (toneAtr->volume + 1) * (toneAtr->volume + 1) - 1;
        voiceVol.left = (voiceVol.left * masterVolumeSquared) >> 14;
        voiceVol.right = (voiceVol.right * masterVolumeSquared) >> 14;
    }

    if (!(aadMem->flags & 0x1))
    {
        if (toneAtr->panPosition >= 65)
        {
            voiceVol.left = (unsigned int)(voiceVol.right * (((0x80 - toneAtr->panPosition) * (0x80 - toneAtr->panPosition)) - 1)) >> 12;
        }
        else if (toneAtr->panPosition < 63)
        {
            voiceVol.right = (voiceVol.left * (((toneAtr->panPosition + 1) * (toneAtr->panPosition + 1)) + 1)) >> 12;
        }
    }

    {
        unsigned long masterVolumeSquared; // $v1 line 14

        masterVolumeSquared = (masterVolume + 1) * (masterVolume + 1) - 1;
        voiceVol.left = (voiceVol.left * masterVolumeSquared) >> 14;
        voiceVol.right = (voiceVol.right * masterVolumeSquared) >> 14;
    }

    {
        unsigned long masterVolumeSquared; // $a1 line 14

        masterVolumeSquared = (progAtr->volume + 1) * (progAtr->volume + 1) - 1;
        voiceVol.left = (voiceVol.left * masterVolumeSquared) >> 14;
        voiceVol.right = (voiceVol.right * masterVolumeSquared) >> 14;
    }

    {
        unsigned long masterVolumeSquared; // $v1 line 14

        masterVolumeSquared = (slotVolume + 1) * (slotVolume + 1) - 1;
        voiceVol.left = (voiceVol.left * masterVolumeSquared) >> 14;
        voiceVol.right = (voiceVol.right * masterVolumeSquared) >> 14;
    }

    {
        unsigned long masterVolumeSquared; // $v1 line 14

        masterVolumeSquared = (masterMasterVol + 1) * (masterMasterVol + 1) - 1;
        voiceVol.left = (voiceVol.left * masterVolumeSquared) >> 14;
        voiceVol.right = (voiceVol.right * masterVolumeSquared) >> 14;
    }

    SpuSetVoiceVolume(voice->voiceNum, voiceVol.left, voiceVol.right);

    pitchOffset -= 0x2000;
    temp_lo = 0x2000 / toneAtr->pitchBendMax;
    pitchIndex = midiNote  + 60 - toneAtr->centerNote;
    pitchIndex += pitchOffset / temp_lo;
    pitchOffset2 = aadStepsPerSemitone[pitchIndex] * (pitchOffset % temp_lo) / temp_lo;

    if (toneAtr->centerFine & 0x80)
    {
        pitch = 0x100 - toneAtr->centerFine;
        finePitch = aadPitchTable[pitchIndex];
        finePitch -= ((aadStepsPerCent[pitchIndex] * (pitch * 100)) >> 23);
    }
    else
    {
        pitch = toneAtr->centerFine;
        finePitch = aadPitchTable[pitchIndex];
        finePitch += ((aadStepsPerCent[pitchIndex] * (pitch * 100)) >> 23);
    }

    finePitch += pitchOffset2;
    SpuSetVoicePitch(voice->voiceNum, finePitch & 0xFFFF);
    SpuSetVoiceStartAddr(voice->voiceNum, waveStartAddr);
    SpuSetVoiceADSR1ADSR2(voice->voiceNum, toneAtr->adsr1, toneAtr->adsr2);

    if (toneAtr->mode == 4)
    {
        aadMem->voiceReverbRequest |= voice->voiceMask;
    }
    else
    {
        aadMem->voiceReverbRequest = aadMem->voiceReverbRequest & ~voice->voiceMask;
    }

    aadMem->voiceKeyOnRequest |= voice->voiceMask;
}
