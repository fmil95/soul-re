#include "Game/PSX/AADLIB.h"
#include "Game/PSX/AADSQCMD.h"
#include "Game/PSX/AADSEQEV.h"

void aadSubstituteVariables(AadSeqEvent *event, AadSequenceSlot *slot)
{
    unsigned char trackFlags;

    slot = (AadSequenceSlot *)((uintptr_t)slot + event->track);

    trackFlags = slot->trackFlags[0];

    if (((trackFlags & 0x7)) && ((event->statusByte != 65) && (event->statusByte != 66) && (event->statusByte != 67)))
    {
        if ((trackFlags & 0x1))
        {
            trackFlags &= ~0x1;

            event->dataByte[0] = aadMem->userVariables[(unsigned char)event->dataByte[0]];
        }

        if ((trackFlags & 0x2))
        {
            trackFlags &= ~0x2;

            event->dataByte[1] = aadMem->userVariables[(unsigned char)event->dataByte[1]];
        }

        if ((trackFlags & 0x4))
        {
            trackFlags &= ~0x4;

            event->dataByte[2] = aadMem->userVariables[(unsigned char)event->dataByte[2]];
        }

        slot->trackFlags[0] = trackFlags;
    }
}

void metaCmdSelectChannel(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int channelNumber;

    channelNumber = event->dataByte[0];

    if (channelNumber < 16)
    {
        slot->selectedChannel = channelNumber;
    }
}

void metaCmdSelectSlot(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int slotNumber;

    slotNumber = event->dataByte[0];

    if (slotNumber < aadMem->numSlots)
    {
        slot->selectedSlotPtr = aadMem->sequenceSlots[slotNumber];

        slot->selectedSlotNum = slotNumber;
    }
    else if (slotNumber == 127)
    {
        slot->selectedSlotPtr = slot;

        slot->selectedSlotNum = slot->thisSlotNumber;
    }
}

void metaCmdAssignSequence(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int sequenceNumber;
    int bank;

    sequenceNumber = event->dataByte[0];

    bank = slot->selectedDynamicBank;

    if ((aadMem->dynamicBankStatus[bank] == 2) && (sequenceNumber < aadGetNumDynamicSequences(bank)))
    {
        aadAssignDynamicSequence(bank, sequenceNumber, slot->selectedSlotNum);
    }
}

void metaCmdUsePrimaryTempo()
{
}

void metaCmdUseSecondaryTempo()
{
}

void metaCmdSetTempo(AadSeqEvent *event, AadSequenceSlot *slot)
{
    AadTempo tempo;

    tempo.quarterNoteTime = event->dataByte[0] << 16;

    tempo.quarterNoteTime |= event->dataByte[1] << 8;
    tempo.quarterNoteTime |= event->dataByte[2];

    tempo.ppqn = slot->selectedSlotPtr->tempo.ppqn;

    aadSetSlotTempo(slot->selectedSlotNum, &tempo);
}

void metaCmdChangeTempo(AadSeqEvent *event, AadSequenceSlot *slot)
{
    AadTempo tempo;
    AadSequenceSlot *selectedSlot;

    selectedSlot = slot->selectedSlotPtr;

    tempo.quarterNoteTime = selectedSlot->tempo.quarterNoteTime * 100;

    tempo.quarterNoteTime /= event->dataByte[0];

    tempo.ppqn = selectedSlot->tempo.ppqn;

    aadSetSlotTempo(slot->selectedSlotNum, &tempo);
}

void metaCmdSetTempoFromSequence(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int sequenceNumber;
    AadTempo tempo;
    int bank;

    sequenceNumber = event->dataByte[0];

    bank = slot->selectedDynamicBank;

    if ((aadMem->dynamicBankStatus[bank] == 2) && (sequenceNumber < aadGetNumDynamicSequences(bank)))
    {
        aadGetTempoFromDynamicSequence(bank, sequenceNumber, &tempo);

        aadSetSlotTempo(slot->selectedSlotNum, &tempo);
    }
}

void metaCmdStartSlot(AadSeqEvent *event, AadSequenceSlot *slot)
{
    (void)event;

    aadStartSlot(slot->selectedSlotNum);
}

void metaCmdStopSlot(AadSeqEvent *event, AadSequenceSlot *slot)
{
    (void)event;

    aadStopSlot(slot->selectedSlotNum);
}

void metaCmdPauseSlot(AadSeqEvent *event, AadSequenceSlot *slot)
{
    (void)event;

    aadPauseSlot(slot->selectedSlotNum);
}

void metaCmdResumeSlot(AadSeqEvent *event, AadSequenceSlot *slot)
{
    (void)event;

    aadResumeSlot(slot->selectedSlotNum);
}

void metaCmdSetSlotBendRange()
{
}

void metaCmdSetChannelBendRange()
{
}

void metaCmdSetSlotVolume(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int volume;

    volume = event->dataByte[0];

    slot->selectedSlotPtr->slotVolume = volume;

    aadUpdateSlotVolPan(slot->selectedSlotPtr);
}

void metaCmdSetSlotPan(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int pan;

    pan = event->dataByte[0];

    slot->selectedSlotPtr->slotPan = pan;

    aadUpdateSlotVolPan(slot->selectedSlotPtr);
}

void metaCmdSetChannelVolume(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int volume;

    volume = event->dataByte[0];

    slot->selectedSlotPtr->volume[slot->selectedChannel] = volume;

    aadUpdateChannelVolPan(slot->selectedSlotPtr, slot->selectedChannel);
}

void metaCmdSetChannelPan(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int pan;

    pan = event->dataByte[0];

    slot->selectedSlotPtr->panPosition[slot->selectedChannel] = pan;

    aadUpdateChannelVolPan(slot->selectedSlotPtr, slot->selectedChannel);
}

void metaCmdEnableSustainUpdate(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int channel;

    (void)event;

    channel = slot->selectedChannel;

    slot->selectedSlotPtr->enableSustainUpdate |= 1 << channel;
}

void metaCmdDisableSustainUpdate(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int channel;

    (void)event;

    channel = slot->selectedChannel;

    slot->selectedSlotPtr->enableSustainUpdate &= ~(1 << channel);
}

void metaCmdMuteChannel(AadSeqEvent *event, AadSequenceSlot *slot)
{
    (void)event;

    aadMuteChannels(slot->selectedSlotPtr, 1 << slot->selectedChannel);
}

void metaCmdUnMuteChannel(AadSeqEvent *event, AadSequenceSlot *slot)
{
    (void)event;

    aadUnMuteChannels(slot->selectedSlotPtr, 1 << slot->selectedChannel);
}

void metaCmdMuteChannelList(AadSeqEvent *event, AadSequenceSlot *slot)
{
    aadMuteChannels(slot->selectedSlotPtr, (event->dataByte[1] << 8) | event->dataByte[0]);
}

void metaCmdUnMuteChannelList(AadSeqEvent *event, AadSequenceSlot *slot)
{
    aadUnMuteChannels(slot->selectedSlotPtr, (event->dataByte[1] << 8) | event->dataByte[0]);
}

void metaCmdSetChannelMute(AadSeqEvent *event, AadSequenceSlot *slot)
{
    unsigned long muteChannelMask;

    muteChannelMask = (event->dataByte[1] << 8) | event->dataByte[0];

    aadUnMuteChannels(slot->selectedSlotPtr, ~muteChannelMask);
    aadMuteChannels(slot->selectedSlotPtr, muteChannelMask);
}

void metaCmdDelayMute(AadSeqEvent *event, AadSequenceSlot *slot)
{
    unsigned long channelMask;

    channelMask = (event->dataByte[1] << 8) | event->dataByte[0];

    slot->selectedSlotPtr->delayedMuteMode |= channelMask;
}

void metaCmdUpdateMute(AadSeqEvent *event, AadSequenceSlot *slot)
{
    unsigned long channelMask;
    unsigned long mask;

    channelMask = (event->dataByte[1] << 8) | event->dataByte[0];

    slot->selectedSlotPtr->delayedMuteMode &= ~channelMask;

    mask = slot->selectedSlotPtr->delayedMuteCmds & channelMask;

    if (mask != 0)
    {
        aadMuteChannels(slot->selectedSlotPtr, mask);
    }

    mask = slot->selectedSlotPtr->delayedUnMuteCmds & channelMask;

    if (mask != 0)
    {
        aadUnMuteChannels(slot->selectedSlotPtr, mask);
    }
}

void metaCmdChannelVolumeFade()
{
}

void metaCmdChannelPanFade()
{
}

void metaCmdSlotVolumeFade()
{
}

void metaCmdSlotPanFade()
{
}

void metaCmdSetChannelProgram(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int program;

    program = event->dataByte[0];

    slot->selectedSlotPtr->currentProgram[slot->selectedChannel] = program;
}

void metaCmdSetChannelBasePriority()
{
}

void metaCmdSetChannelTranspose(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int channel;
    int transpose;

    channel = slot->selectedChannel;

    transpose = event->dataByte[0];

    slot->selectedSlotPtr->transpose[channel] = transpose;
}

void metaCmdIgnoreChannelTranspose(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int channel;

    (void)event;

    channel = slot->selectedChannel;

    slot->selectedSlotPtr->ignoreTranspose |= 1 << channel;
}

void metaCmdRespectChannelTranspose(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int channel;

    (void)event;

    channel = slot->selectedChannel;

    slot->selectedSlotPtr->ignoreTranspose &= ~(1 << channel);
}

void metaCmdSetChannelPitchMap()
{
}

void metaCmdIgnoreChannelPitchMap()
{
}

void metaCmdRespectChannelPitchMap()
{
}

void metaCmdGetSequenceAssigned(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int variableNum;

    variableNum = event->dataByte[0];

    if (variableNum < 128)
    {
        aadMem->userVariables[variableNum] = slot->selectedSlotPtr->sequenceNumberAssigned;
    }
}

void metaCmdGetTempo(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int variableNum1;
    int variableNum2;
    int variableNum3;
    unsigned long quarterNoteTime;

    variableNum1 = event->dataByte[0];
    variableNum2 = event->dataByte[1];
    variableNum3 = event->dataByte[2];

    if ((variableNum1 < 128) && (variableNum2 < 128) && (variableNum3 < 128))
    {
        quarterNoteTime = slot->selectedSlotPtr->tempo.quarterNoteTime;

        aadMem->userVariables[variableNum1] = quarterNoteTime;
        aadMem->userVariables[variableNum2] = quarterNoteTime >> 8;
        aadMem->userVariables[variableNum3] = quarterNoteTime >> 16;
    }
}

void metaCmdGetSlotStatus(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int variableNum;

    variableNum = event->dataByte[0];

    if (variableNum < 128)
    {
        aadMem->userVariables[variableNum] = slot->selectedSlotPtr->status;
    }
}

void metaCmdGetChannelMute(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int variableNum1;
    int variableNum2;

    variableNum1 = event->dataByte[0];
    variableNum2 = event->dataByte[1];

    if ((variableNum1 < 128) && (variableNum2 < 128))
    {
        aadMem->userVariables[variableNum1] = slot->selectedSlotPtr->channelMute;
        aadMem->userVariables[variableNum2] = slot->selectedSlotPtr->channelMute >> 8;
    }
}

void metaCmdGetChannelVolume(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int variableNum;
    int channel;

    variableNum = event->dataByte[0];

    channel = slot->selectedChannel;

    if (variableNum < 128)
    {
        aadMem->userVariables[variableNum] = slot->selectedSlotPtr->volume[channel];
    }
}

void metaCmdGetChannelPan(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int variableNum;
    int channel;

    variableNum = event->dataByte[0];

    channel = slot->selectedChannel;

    if (variableNum < 128)
    {
        aadMem->userVariables[variableNum] = slot->selectedSlotPtr->panPosition[channel];
    }
}

void metaCmdGetChannelTranspose()
{
}

void metaCmdGetChannelProgram(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int variableNum;
    int channel;

    variableNum = event->dataByte[0];

    channel = slot->selectedChannel;

    if (variableNum < 128)
    {
        aadMem->userVariables[variableNum] = slot->selectedSlotPtr->currentProgram[channel];
    }
}

void metaCmdGetChannelBasePriority()
{
}

void metaCmdGetChannelBendRange()
{
}

void metaCmdGetSlotVolume(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int variableNum;

    variableNum = event->dataByte[0];

    if (variableNum < 128)
    {
        aadMem->userVariables[variableNum] = slot->selectedSlotPtr->slotVolume;
    }
}

void metaCmdGetSlotPan(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int variableNum;

    variableNum = event->dataByte[0];

    if (variableNum < 128)
    {
        aadMem->userVariables[variableNum] = slot->selectedSlotPtr->slotPan;
    }
}

void metaCmdSetVariable(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int value;
    int destVariable;

    (void)slot;

    value = event->dataByte[0];

    destVariable = event->dataByte[1];

    if (destVariable < 128)
    {
        aadMem->userVariables[destVariable] = value;
    }
}

void metaCmdCopyVariable(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int srcVariable;
    int destVariable;

    (void)slot;

    srcVariable = event->dataByte[0];
    destVariable = event->dataByte[1];

    if ((srcVariable < 128) && (destVariable < 128))
    {
        aadMem->userVariables[destVariable] = aadMem->userVariables[srcVariable];
    }
}

void metaCmdAddVariable(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int value;
    int destVariable;

    (void)slot;

    value = event->dataByte[0];

    destVariable = event->dataByte[1];

    if (destVariable < 128)
    {
        aadMem->userVariables[destVariable] += value;
    }
}

void metaCmdSubtractVariable(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int value;
    int destVariable;

    (void)slot;

    value = event->dataByte[0];

    destVariable = event->dataByte[1];

    if (destVariable < 128)
    {
        aadMem->userVariables[destVariable] -= value;
    }
}

void metaCmdSetVariableBits(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int value;
    int destVariable;

    (void)slot;

    value = event->dataByte[0];

    destVariable = event->dataByte[1];

    if (destVariable < 128)
    {
        aadMem->userVariables[destVariable] |= value;
    }
}

void metaCmdClearVariableBits(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int value;
    int destVariable;

    (void)slot;

    value = event->dataByte[0];

    destVariable = event->dataByte[1];

    if (destVariable < 128)
    {
        aadMem->userVariables[destVariable] &= ~value;
    }
}

void aadGotoSequencePosition(AadSequenceSlot *slot, int track, unsigned char *newPosition)
{
    slot->sequencePosition[track] = newPosition;

    while (slot->eventsInQueue[track] != 0)
    {
        slot->eventsInQueue[track]--;
        slot->eventOut[track]++;

        if (slot->eventOut[track] == 4)
        {
            slot->eventOut[track] = 0;
        }
    }

    slot->trackFlags[track] &= ~0x18;
}

void aadGotoSequenceLabel(AadSequenceSlot *slot, int track, int labelNumber)
{
    AadSequenceHdr *seqHdr;
    unsigned long trackOffset;
    int bank;

    bank = slot->sequenceAssignedDynamicBank;

    seqHdr = (AadSequenceHdr *)aadMem->dynamicSequenceAddressTbl[bank][slot->sequenceNumberAssigned];

    trackOffset = ((unsigned long *)&seqHdr[1])[track];

    slot->sequencePosition[track] = (unsigned char *)seqHdr + trackOffset + aadMem->dynamicSequenceLabelOffsetTbl[bank][labelNumber];

    while (slot->eventsInQueue[track] != 0)
    {
        slot->eventsInQueue[track]--;
        slot->eventOut[track]++;

        if (slot->eventOut[track] == 4)
        {
            slot->eventOut[track] = 0;
        }
    }

    slot->trackFlags[track] &= ~0x18;
}

void metaCmdLoopStart(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int nestLevel;
    int track;

    track = event->track;

    nestLevel = slot->loopCurrentNestLevel[track];

    if (nestLevel < 4)
    {
        slot->loopSequencePosition[nestLevel][track] = slot->sequencePosition[track];

        slot->loopCounter[nestLevel][track] = event->dataByte[0];

        slot->loopCurrentNestLevel[track]++;
    }

    slot->trackFlags[track] &= ~0x10;
}

void metaCmdLoopEnd(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int prevNestLevel;
    int track;

    track = event->track;

    if (slot->loopCurrentNestLevel[track] != 0)
    {
        prevNestLevel = slot->loopCurrentNestLevel[track] - 1;

        if (slot->loopCounter[prevNestLevel][track] == 0)
        {
            aadGotoSequencePosition(slot, track, slot->loopSequencePosition[prevNestLevel][track]);
        }
        else if (--slot->loopCounter[prevNestLevel][track] != 0)
        {
            aadGotoSequencePosition(slot, track, slot->loopSequencePosition[prevNestLevel][track]);
        }
        else
        {
            slot->loopCurrentNestLevel[track] = prevNestLevel;
        }
    }
}

void metaCmdLoopBreak()
{
}

void metaCmdDefineLabel()
{
}

void metaCmdGotoLabel(AadSeqEvent *event, AadSequenceSlot *slot)
{
    aadGotoSequenceLabel(slot, event->track, event->dataByte[0]);
}

void metaCmdSetSequencePosition(AadSeqEvent *event, AadSequenceSlot *slot)
{
    aadGotoSequenceLabel(slot->selectedSlotPtr, event->track, event->dataByte[0]);
}

void metaCmdBranchIfVarEqual(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int variableNum;
    int value;
    int labelNum;

    variableNum = event->dataByte[0];

    value = event->dataByte[1];

    labelNum = event->dataByte[2];

    if (aadMem->userVariables[variableNum] == value)
    {
        aadGotoSequenceLabel(slot, event->track, labelNum);
    }
}

void metaCmdBranchIfVarNotEqual(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int variableNum;
    int value;
    int labelNum;

    variableNum = event->dataByte[0];

    value = event->dataByte[1];

    labelNum = event->dataByte[2];

    if (aadMem->userVariables[variableNum] != value)
    {
        aadGotoSequenceLabel(slot, event->track, labelNum);
    }
}

void metaCmdBranchIfVarLess(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int variableNum;
    int value;
    int labelNum;

    variableNum = event->dataByte[0];

    value = event->dataByte[1];

    labelNum = event->dataByte[2];

    if (aadMem->userVariables[variableNum] < value)
    {
        aadGotoSequenceLabel(slot, event->track, labelNum);
    }
}

void metaCmdBranchIfVarGreater(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int variableNum;
    int value;
    int labelNum;

    variableNum = event->dataByte[0];

    value = event->dataByte[1];

    labelNum = event->dataByte[2];

    if (aadMem->userVariables[variableNum] > value)
    {
        aadGotoSequenceLabel(slot, event->track, labelNum);
    }
}

void metaCmdBranchIfVarLessOrEqual(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int variableNum;
    int value;
    int labelNum;

    variableNum = event->dataByte[0];

    value = event->dataByte[1];

    labelNum = event->dataByte[2];

    if (aadMem->userVariables[variableNum] <= value)
    {
        aadGotoSequenceLabel(slot, event->track, labelNum);
    }
}

void metaCmdBranchIfVarGreaterOrEqual(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int variableNum;
    int value;
    int labelNum;

    variableNum = event->dataByte[0];

    value = event->dataByte[1];

    labelNum = event->dataByte[2];

    if (aadMem->userVariables[variableNum] >= value)
    {
        aadGotoSequenceLabel(slot, event->track, labelNum);
    }
}

void metaCmdBranchIfVarBitsSet(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int variableNum;
    int mask;
    int labelNum;

    variableNum = event->dataByte[0];

    mask = event->dataByte[1];

    labelNum = event->dataByte[2];

    if ((aadMem->userVariables[variableNum] & mask))
    {
        aadGotoSequenceLabel(slot, event->track, labelNum);
    }
}

void metaCmdBranchIfVarBitsClear(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int variableNum;
    int mask;
    int labelNum;

    variableNum = event->dataByte[0];

    mask = event->dataByte[1];

    labelNum = event->dataByte[2];

    if (!(aadMem->userVariables[variableNum] & mask))
    {
        aadGotoSequenceLabel(slot, event->track, labelNum);
    }
}

void metaCmdSubstituteVariableParam1(AadSeqEvent *event, AadSequenceSlot *slot)
{
    slot->trackFlags[event->track] |= 0x1;
}

void metaCmdSubstituteVariableParam2(AadSeqEvent *event, AadSequenceSlot *slot)
{
    slot->trackFlags[event->track] |= 0x2;
}

void metaCmdSubstituteVariableParam3(AadSeqEvent *event, AadSequenceSlot *slot)
{
    slot->trackFlags[event->track] |= 0x4;
}

void metaCmdEndSequence(AadSeqEvent *event, AadSequenceSlot *slot)
{
    (void)event;

    aadInitSequenceSlot(slot);

    aadAllNotesOff(slot->thisSlotNumber);

    if (aadMem->endSequenceCallback != NULL)
    {
        aadMem->endSequenceCallback(aadMem->endSequenceCallbackData, slot->thisSlotNumber, 0);
    }
}

void metaCmdPlaySoundEffect()
{
}

void metaCmdStopSoundEffect()
{
}

void metaCmdSetSoundEffectVolumePan()
{
}
