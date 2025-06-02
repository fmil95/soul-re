#ifndef _AADSEQEV_H_
#define _AADSEQEV_H_

#include "common.h"

void aadUpdateSlotVolPan(AadSequenceSlot *slot);
int aadGetReverbMode();
void aadUpdateChannelVolPan(AadSequenceSlot *slot, int channel);
int aadQueueNextEvent(AadSequenceSlot *slot, int track);
void aadExecuteEvent(AadSeqEvent *event, AadSequenceSlot *slot);
void aadUpdateChannelPitchBend(AadSequenceSlot *slot, int channel);

#endif
