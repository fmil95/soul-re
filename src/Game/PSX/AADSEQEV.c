#include "Game/PSX/AADLIB.h"
#include "Game/PSX/AADSEQEV.h"

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSEQEV", aadQueueNextEvent);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSEQEV", aadExecuteEvent);

void midiNoteOff()
{
}

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSEQEV", midiNoteOn);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSEQEV", aadUpdateChannelVolPan);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSEQEV", aadUpdateSlotVolPan);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSEQEV", aadUpdateChannelPitchBend);

void midiPolyphonicAftertouch()
{
}

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSEQEV", midiControlChange);

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
