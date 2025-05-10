#ifndef _AADSFX_H_
#define _AADSFX_H_

#include "common.h"

unsigned long aadSetSfxVolPanPitch(unsigned long handle, int volume, int pan, int pitch);
unsigned long aadPlaySfx(unsigned int toneID, int volume, int pan, int pitchOffset);
unsigned long aadStopSfx(unsigned long handle);
int aadIsSfxTypePlayingOrRequested(unsigned int sfxToneID);
int aadIsSfxPlayingOrRequested(unsigned long handle);
int aadIsSfxPlaying(unsigned long handle);
void aadStopAllSfx();
void aadPutSfxCommand(int statusByte, int dataByte0, int dataByte1, unsigned long ulongParam, short shortParam);
unsigned long createSfxHandle(unsigned int toneID);
int aadIsSfxTypePlaying(unsigned int toneID);
void aadExecuteSfxCommand(AadSfxCommand *sfxCmd);
void sfxCmdPlayTone(AadSfxCommand *sfxCmd);
void sfxCmdStopTone(AadSfxCommand *sfxCmd);
void sfxCmdSetToneVolumeAndPan(AadSfxCommand *sfxCmd);
void sfxCmdSetToneVolPanPitch(AadSfxCommand *sfxCmd);
void sfxCmdStopAllTones(AadSfxCommand *sfxCmd);
void sfxCmdLockVoice(AadSfxCommand *sfxCmd);
void sfxCmdSetVoiceAttr(AadSfxCommand *sfxCmd);
void sfxCmdSetVoiceKeyOn(AadSfxCommand *sfxCmd);
void sfxCmdSetVoiceKeyOff(AadSfxCommand *sfxCmd);

#endif
