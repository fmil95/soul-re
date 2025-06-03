#ifndef _AADSQCMD_H_
#define _AADSQCMD_H_

#include "common.h"

void aadSubstituteVariables(AadSeqEvent *event, AadSequenceSlot *slot);
void aadGotoSequenceLabel(AadSequenceSlot *slot, int track, int labelNumber);

#endif
