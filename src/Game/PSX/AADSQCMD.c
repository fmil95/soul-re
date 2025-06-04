#include "Game/PSX/AADLIB.h"
#include "Game/PSX/AADSQCMD.h"

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", aadSubstituteVariables);

void metaCmdSelectChannel(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int channelNumber;

    channelNumber = (unsigned char)event->dataByte[0];

    if (channelNumber < 16)
    {
        slot->selectedChannel = channelNumber;
    }
}

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdSelectSlot);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdAssignSequence);

void metaCmdUsePrimaryTempo()
{
}

void metaCmdUseSecondaryTempo()
{
}

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdSetTempo);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdChangeTempo);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdSetTempoFromSequence);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdStartSlot);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdStopSlot);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdPauseSlot);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdResumeSlot);

void metaCmdSetSlotBendRange()
{
}

void metaCmdSetChannelBendRange()
{
}

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdSetSlotVolume);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdSetSlotPan);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdSetChannelVolume);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdSetChannelPan);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdEnableSustainUpdate);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdDisableSustainUpdate);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdMuteChannel);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdUnMuteChannel);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdMuteChannelList);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdUnMuteChannelList);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdSetChannelMute);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdDelayMute);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdUpdateMute);

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

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdSetChannelProgram);

void metaCmdSetChannelBasePriority()
{
}

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdSetChannelTranspose);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdIgnoreChannelTranspose);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdRespectChannelTranspose);

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

    variableNum = (unsigned char)event->dataByte[0];

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

    variableNum1 = (unsigned char)event->dataByte[0];
    variableNum2 = (unsigned char)event->dataByte[1];
    variableNum3 = (unsigned char)event->dataByte[2];

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

    variableNum = (unsigned char)event->dataByte[0];

    if (variableNum < 128)
    {
        aadMem->userVariables[variableNum] = slot->selectedSlotPtr->status;
    }
}

void metaCmdGetChannelMute(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int variableNum1;
    int variableNum2;

    variableNum1 = (unsigned char)event->dataByte[0];
    variableNum2 = (unsigned char)event->dataByte[1];

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

    variableNum = (unsigned char)event->dataByte[0];

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

    variableNum = (unsigned char)event->dataByte[0];

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

    variableNum = (unsigned char)event->dataByte[0];

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

    variableNum = (unsigned char)event->dataByte[0];

    if (variableNum < 128)
    {
        aadMem->userVariables[variableNum] = slot->selectedSlotPtr->slotVolume;
    }
}

void metaCmdGetSlotPan(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int variableNum;

    variableNum = (unsigned char)event->dataByte[0];

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

    value = (unsigned char)event->dataByte[0];

    destVariable = (unsigned char)event->dataByte[1];

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

    srcVariable = (unsigned char)event->dataByte[0];
    destVariable = (unsigned char)event->dataByte[1];

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

    value = (unsigned char)event->dataByte[0];

    destVariable = (unsigned char)event->dataByte[1];

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

    value = (unsigned char)event->dataByte[0];

    destVariable = (unsigned char)event->dataByte[1];

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

    value = (unsigned char)event->dataByte[0];

    destVariable = (unsigned char)event->dataByte[1];

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

    value = (unsigned char)event->dataByte[0];

    destVariable = (unsigned char)event->dataByte[1];

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

        slot->loopCounter[nestLevel][track] = (unsigned char)event->dataByte[0];

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
    aadGotoSequenceLabel(slot, event->track, (unsigned char)event->dataByte[0]);
}

void metaCmdSetSequencePosition(AadSeqEvent *event, AadSequenceSlot *slot)
{
    aadGotoSequenceLabel(slot->selectedSlotPtr, event->track, (unsigned char)event->dataByte[0]);
}

void metaCmdBranchIfVarEqual(AadSeqEvent *event, AadSequenceSlot *slot)
{
    int variableNum;
    int value;
    int labelNum;

    variableNum = (unsigned char)event->dataByte[0];

    value = (unsigned char)event->dataByte[1];

    labelNum = (unsigned char)event->dataByte[2];

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

    variableNum = (unsigned char)event->dataByte[0];

    value = (unsigned char)event->dataByte[1];

    labelNum = (unsigned char)event->dataByte[2];

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

    variableNum = (unsigned char)event->dataByte[0];

    value = (unsigned char)event->dataByte[1];

    labelNum = (unsigned char)event->dataByte[2];

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

    variableNum = (unsigned char)event->dataByte[0];

    value = (unsigned char)event->dataByte[1];

    labelNum = (unsigned char)event->dataByte[2];

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

    variableNum = (unsigned char)event->dataByte[0];

    value = (unsigned char)event->dataByte[1];

    labelNum = (unsigned char)event->dataByte[2];

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

    variableNum = (unsigned char)event->dataByte[0];

    value = (unsigned char)event->dataByte[1];

    labelNum = (unsigned char)event->dataByte[2];

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

    variableNum = (unsigned char)event->dataByte[0];

    mask = (unsigned char)event->dataByte[1];

    labelNum = (unsigned char)event->dataByte[2];

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

    variableNum = (unsigned char)event->dataByte[0];

    mask = (unsigned char)event->dataByte[1];

    labelNum = (unsigned char)event->dataByte[2];

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
