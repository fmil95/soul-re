#ifndef _AADSEQEV_H_
#define _AADSEQEV_H_

#include "common.h"

void aadUpdateSlotVolPan(AadSequenceSlot *slot);
int aadGetReverbMode();
void aadUpdateChannelVolPan(AadSequenceSlot *slot, int channel);
int aadQueueNextEvent(AadSequenceSlot *slot, int track);
void aadExecuteEvent(AadSeqEvent *event, AadSequenceSlot *slot);
void aadUpdateChannelPitchBend(AadSequenceSlot *slot, int channel);
void midiNoteOn(AadSeqEvent *event, AadSequenceSlot *slot);
void midiPolyphonicAftertouch();
void midiControlChange(AadSeqEvent *event, AadSequenceSlot *slot);
void midiChannelAftertouch();
void midiPitchWheelControl(AadSeqEvent *event, AadSequenceSlot *slot);
void midiMetaEvent();
void midiNoteOff();
void midiProgramChange(AadSeqEvent *event, AadSequenceSlot *slot);
void midiControlBankSelect();
void midiControlDummy();
void midiControlVolume(AadSeqEvent *event, AadSequenceSlot *slot);
void midiControlPan(AadSeqEvent *event, AadSequenceSlot *slot);
void midiControlCallback(AadSeqEvent *event, AadSequenceSlot *slot);

#endif
