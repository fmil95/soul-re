#include "Game/PSX/AADLIB.h"
#include "Game/PSX/AADSEQEV.h"
#include "Game/PSX/AADSQCMD.h"
#include "Game/PSX/AADVOICE.h"

static char midiDataByteCount[8] = {2, 2, 2, 2, 1, 1, 2, 2};

static void (*midiEventFunction[8])() =
{
    midiNoteOff,
    midiNoteOn,
    midiPolyphonicAftertouch,
    midiControlChange,
    midiProgramChange,
    midiChannelAftertouch,
    midiPitchWheelControl,
    midiMetaEvent
};

static void (*midiControlFunction[16])() =
{
    midiControlBankSelect,
    midiControlDummy,
    midiControlDummy,
    midiControlDummy,
    midiControlDummy,
    midiControlDummy,
    midiControlDummy,
    midiControlVolume,
    midiControlDummy,
    midiControlDummy,
    midiControlPan,
    midiControlCallback,
    midiControlDummy,
    midiControlDummy,
    midiControlDummy,
    midiControlDummy
};

static void (*midiMetaEventFunction[78])() =
{
    metaCmdSelectChannel,
    metaCmdSelectSlot,
    metaCmdAssignSequence,
    metaCmdUsePrimaryTempo,
    metaCmdUseSecondaryTempo,
    metaCmdSetTempo,
    metaCmdSetTempoFromSequence,
    metaCmdChangeTempo,
    metaCmdStartSlot,
    metaCmdStopSlot,
    metaCmdPauseSlot,
    metaCmdResumeSlot,
    metaCmdSetSlotBendRange,
    metaCmdSetChannelBendRange,
    metaCmdSetSlotVolume,
    metaCmdSetSlotPan,
    metaCmdSetChannelVolume,
    metaCmdSetChannelPan,
    metaCmdMuteChannel,
    metaCmdUnMuteChannel,
    metaCmdMuteChannelList,
    metaCmdUnMuteChannelList,
    metaCmdChannelVolumeFade,
    metaCmdChannelPanFade,
    metaCmdSlotVolumeFade,
    metaCmdSlotPanFade,
    metaCmdSetChannelProgram,
    metaCmdSetChannelBasePriority,
    metaCmdSetChannelTranspose,
    metaCmdIgnoreChannelTranspose,
    metaCmdRespectChannelTranspose,
    metaCmdSetChannelPitchMap,
    metaCmdIgnoreChannelPitchMap,
    metaCmdRespectChannelPitchMap,
    metaCmdGetSequenceAssigned,
    metaCmdGetTempo,
    metaCmdGetSlotStatus,
    metaCmdGetSlotVolume,
    metaCmdGetSlotPan,
    metaCmdGetChannelVolume,
    metaCmdGetChannelPan,
    metaCmdGetChannelMute,
    metaCmdGetChannelBendRange,
    metaCmdGetChannelTranspose,
    metaCmdGetChannelProgram,
    metaCmdGetChannelBasePriority,
    metaCmdLoopStart,
    metaCmdLoopEnd,
    metaCmdLoopBreak,
    metaCmdDefineLabel,
    metaCmdGotoLabel,
    metaCmdSetVariable,
    metaCmdCopyVariable,
    metaCmdAddVariable,
    metaCmdSubtractVariable,
    metaCmdSetVariableBits,
    metaCmdClearVariableBits,
    metaCmdBranchIfVarEqual,
    metaCmdBranchIfVarNotEqual,
    metaCmdBranchIfVarLess,
    metaCmdBranchIfVarGreater,
    metaCmdBranchIfVarLessOrEqual,
    metaCmdBranchIfVarGreaterOrEqual,
    metaCmdBranchIfVarBitsSet,
    metaCmdBranchIfVarBitsClear,
    metaCmdSubstituteVariableParam1,
    metaCmdSubstituteVariableParam2,
    metaCmdSubstituteVariableParam3,
    metaCmdEndSequence,
    metaCmdPlaySoundEffect,
    metaCmdStopSoundEffect,
    metaCmdSetSoundEffectVolumePan,
    metaCmdSetSequencePosition,
    metaCmdEnableSustainUpdate,
    metaCmdDisableSustainUpdate,
    metaCmdSetChannelMute,
    metaCmdDelayMute,
    metaCmdUpdateMute
};

int aadQueueNextEvent(AadSequenceSlot *slot, int track)
{
    AadSeqEvent seqEvent;
    unsigned char *seqData;
    unsigned long deltaTime;
    int c;
    int n;
    int i;

    if ((slot->trackFlags[track] & 0x18))
    {
        return -1;
    }

    if ((slot->trackFlags[track] & 0x20))
    {
        slot->lastEventExecutedTime[track] = slot->tempo.currentTick;

        slot->trackFlags[track] &= ~0x20;
    }

    seqData = slot->sequencePosition[track];

    deltaTime = *seqData++;

    if ((deltaTime & 0x80))
    {
        c = *seqData;

        deltaTime &= 0x7F | (c & 0x7F);

        do
        {
            c = *seqData++;

            deltaTime = (deltaTime * 128) | (c & 0x7F);
        } while ((c & 0x80));
    }

    seqEvent.track = track;

    seqEvent.deltaTime = deltaTime;

    c = *seqData;

    if (c == 255)
    {
        seqData++;

        seqEvent.statusByte = *seqData++;

        n = *seqData++;

        if (seqEvent.statusByte == 68)
        {
            slot->trackFlags[track] |= 0x8;
        }
        else if (seqEvent.statusByte == 46)
        {
            slot->trackFlags[track] |= 0x10;
        }
    }
    else
    {
        if ((c & 0x80))
        {
            slot->runningStatus[track] = seqEvent.statusByte = *seqData++;
        }
        else
        {
            seqEvent.statusByte = slot->runningStatus[track];
        }

        n = midiDataByteCount[(seqEvent.statusByte >> 4) & 0x7];
    }

    i = 0;

    while (--n != -1)
    {
        seqEvent.dataByte[i++] = *seqData++;
    }

    slot->sequencePosition[track] = seqData;

    memcpy(&slot->eventQueue[slot->eventIn[track]][track], &seqEvent, sizeof(seqEvent));

    slot->eventsInQueue[track]++;
    slot->eventIn[track]++;

    if (slot->eventIn[track] == 4)
    {
        slot->eventIn[track] = 0;
    }

    return 0;
}

void aadExecuteEvent(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int eventType;

    if ((event->statusByte & 0x80))
    {
        eventType = (event->statusByte >> 4) & 0x7;

        midiEventFunction[eventType](event, slot);
    }
    else
    {
        aadSubstituteVariables(event, slot);

        eventType = event->statusByte & 0x7F;

        if (event->statusByte < 78)
        {
            midiMetaEventFunction[eventType](event, slot);
        }
    }
}

void midiNoteOff()
{
}

void midiNoteOn(AadSeqEvent *event, AadSequenceSlot *slot)
{
    AadProgramAtr *progAtr;
    AadToneAtr *toneAtrTbl;
    AadSynthVoice *voice;
    int channel;
    int midiNote;
    int transposedNote;
    int t;
    int dynBank;

    channel = event->statusByte & 0xF;

    if ((!((slot->channelMute >> channel) & 0x1)) && (slot->currentProgram[channel] != 255))
    {
        midiNote = event->dataByte[0];

        if (event->dataByte[1] == 0)
        {
            for (t = 0; t < 24; t++)
            {
                voice = &aadMem->synthVoice[t];

                if ((voice->voiceID == (slot->slotID | channel)) && (voice->note == midiNote) && ((aadMem->voiceStatus[t] != 0) && (aadMem->voiceStatus[t] != 2)))
                {
                    aadMem->voiceKeyOffRequest |= voice->voiceMask;
                    aadMem->voiceKeyOnRequest &= ~voice->voiceMask;

                    voice->voiceID = 255;
                }
            }
        }
        else
        {
            if (((slot->ignoreTranspose >> channel) & 0x1))
            {
                transposedNote = midiNote;
            }
            else
            {
                transposedNote = (midiNote + slot->transpose[channel]) & 0xFF;
            }

            dynBank = slot->currentDynamicBank[channel];

            if (aadMem->dynamicBankStatus[dynBank] == 2)
            {
                progAtr = aadMem->dynamicProgramAtr[dynBank] + slot->currentProgram[channel];

                toneAtrTbl = aadMem->dynamicToneAtr[dynBank];

                for (t = progAtr->firstTone; t < (progAtr->firstTone + progAtr->numTones); t++)
                {
                    if ((midiNote >= (&toneAtrTbl[t])->minNote) && ((&toneAtrTbl[t])->maxNote >= midiNote))
                    {
                        voice = aadAllocateVoice((&toneAtrTbl[t])->priority);

                        if (voice != NULL)
                        {
                            unsigned long waveStartAddr;

                            waveStartAddr = aadMem->dynamicWaveAddr[dynBank][(&toneAtrTbl[t])->waveIndex];

                            if (((&toneAtrTbl[t])->pitchBendMax != 0) && (slot->pitchWheel[channel] != 8192))
                            {
                                aadPlayTonePitchBend(&toneAtrTbl[t], waveStartAddr, progAtr, transposedNote, event->dataByte[1], slot->volume[channel], slot->panPosition[channel], slot->slotVolume, slot->masterVolPtr[0], voice, slot->pitchWheel[channel]);

                                voice->handle = 0;
                            }
                            else
                            {
                                aadPlayTone(&toneAtrTbl[t], waveStartAddr, progAtr, transposedNote, event->dataByte[1], slot->volume[channel], slot->panPosition[channel], slot->slotVolume, slot->masterVolPtr[0], voice, 0);

                                voice->handle = 0;
                            }

                            voice->voiceID = slot->slotID | channel;

                            voice->priority = (&toneAtrTbl[t])->priority;

                            voice->note = midiNote;

                            voice->program = slot->currentProgram[channel];

                            voice->volume = event->dataByte[1];
                            voice->updateVol = slot->volume[channel];

                            voice->pan = slot->panPosition[channel];

                            voice->progAtr = progAtr;
                            voice->toneAtr = &toneAtrTbl[t];
                        }
                    }
                }
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSEQEV", aadUpdateChannelVolPan);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSEQEV", aadUpdateSlotVolPan);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSEQEV", aadUpdateChannelPitchBend);

void midiPolyphonicAftertouch()
{
}

void midiControlChange(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int controlNumber;

    controlNumber = event->dataByte[0] & 0xF;

    midiControlFunction[controlNumber](event, slot);
}

void midiProgramChange(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int channel;

    channel = event->statusByte & 0xF;

    slot->currentProgram[channel] = event->dataByte[0];
}

void midiChannelAftertouch()
{
}

void midiPitchWheelControl(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int channel;

    channel = event->statusByte & 0xF;

    slot->pitchWheel[channel] = event->dataByte[0] | (event->dataByte[1] << 7);

    aadUpdateChannelPitchBend(slot, channel);
}

void midiMetaEvent()
{
}

void midiControlBankSelect()
{
}

void midiControlVolume(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int channel;

    channel = event->statusByte & 0xF;

    slot->volume[channel] = event->dataByte[1];

    if (((slot->enableSustainUpdate >> channel) & 0x1))
    {
        aadUpdateChannelVolPan(slot, channel);
    }
}

void midiControlPan(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int channel;

    channel = event->statusByte & 0xF;

    slot->panPosition[channel] = event->dataByte[1];

    if (((slot->enableSustainUpdate >> channel) & 0x1))
    {
        aadUpdateChannelVolPan(slot, channel);
    }
}

void midiControlCallback(AadSeqEvent *event, AadSequenceSlot *slot)
{
    if (aadMem->controller11Callback != NULL)
    {
        aadMem->controller11Callback(aadMem->controller11CallbackData, slot->thisSlotNumber, event->statusByte & 0xF, event->dataByte[1]);
    }
}

void midiControlDummy()
{
}
