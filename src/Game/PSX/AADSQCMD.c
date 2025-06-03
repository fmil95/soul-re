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

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdGetSequenceAssigned);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdGetTempo);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdGetSlotStatus);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdGetChannelMute);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdGetChannelVolume);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdGetChannelPan);

void metaCmdGetChannelTranspose()
{
}

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdGetChannelProgram);

void metaCmdGetChannelBasePriority()
{
}

void metaCmdGetChannelBendRange()
{
}

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdGetSlotVolume);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdGetSlotPan);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdSetVariable);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdCopyVariable);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdAddVariable);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdSubtractVariable);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdSetVariableBits);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdClearVariableBits);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", aadGotoSequencePosition);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", aadGotoSequenceLabel);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdLoopStart);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdLoopEnd);

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

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdBranchIfVarEqual);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdBranchIfVarNotEqual);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdBranchIfVarLess);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSQCMD", metaCmdBranchIfVarGreater);

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
