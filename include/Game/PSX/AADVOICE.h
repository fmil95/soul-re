#ifndef _AADVOICE_H_
#define _AADVOICE_H_

#include "common.h"

void SpuSetVoiceADSR1ADSR2(int vNum, unsigned short adsr1, unsigned short adsr2);
AadSynthVoice *aadAllocateVoice(int priority);
void aadPlayTone(AadToneAtr *toneAtr, unsigned long waveStartAddr, AadProgramAtr *progAtr, int midiNote, int volume, int masterVolume, int masterPan, int slotVolume, int masterMasterVol, AadSynthVoice *voice, int pitchOffset);
void aadPlayTonePitchBend(AadToneAtr *toneAtr, unsigned long waveStartAddr, AadProgramAtr *progAtr, int midiNote, int volume, int masterVolume, int masterPan, int slotVolume, int masterMasterVol, AadSynthVoice *voice, int pitchOffset);

#endif
