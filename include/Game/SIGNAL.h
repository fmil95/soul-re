#ifndef _SIGNAL_H_
#define _SIGNAL_H_

#include "common.h"

void SIGNAL_HandleSignal(Instance *instance, Signal *signal, int dontForceDoSignal);
void COLLIDE_HandleSignal(Instance *instance, Signal *signal, long numSignals, int dontForceDoSignal);
long SIGNAL_IsStreamSignal(Signal *signal, long *isWarpGate);
void SIGNAL_InWater(Instance *instance);
void SIGNAL_OutOfWater(Instance *instance);
MultiSignal *SIGNAL_RelocateSignal(MultiSignal *multiSignal, long offset);

#endif
