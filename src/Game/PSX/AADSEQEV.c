#include "Game/PSX/AADLIB.h"
#include "Game/PSX/AADSEQEV.h"
#include "Game/PSX/AADSQCMD.h"

STATIC void (*midiEventFunction[8])();

STATIC void (*midiControlFunction[16])();

STATIC void (*midiMetaEventFunction[78])();

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSEQEV", aadQueueNextEvent);

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

INCLUDE_ASM("asm/nonmatchings/Game/PSX/AADSEQEV", midiNoteOn);

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
