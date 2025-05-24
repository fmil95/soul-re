#ifndef _SOUND_H_
#define _SOUND_H_

#include "common.h"

unsigned long SOUND_Play3dSound(Position *position, int sfxToneID, int pitch, int maxVolume, int minVolDist);
void SOUND_ProcessInstanceSounds(unsigned char *sfxFileData, SoundInstance *soundInstTbl, Position *position, int livesInOnePlace, int inSpectral, int hidden, int burning, long *triggerFlags);
void SOUND_EndInstanceSounds(unsigned char *sfxFileData, SoundInstance *soundInstTbl);
unsigned long SndPlayVolPan(unsigned int sample, unsigned short vol, unsigned short pan, short pitch);
void SOUND_HandleGlobalValueSignal(int name, long data);
void SOUND_MusicOff();
void SOUND_MusicOn();
void SOUND_SfxOff();
void SOUND_SfxOn();
void SOUND_ShutdownMusic();
void SOUND_SetMusicVolume(int newVolume);
void SOUND_SetSfxVolume(int newVolume);
void SOUND_SetVoiceVolume(int newVolume);
void SOUND_StopAllSound();
void SOUND_ResetAllSound();
void SndEndLoop(unsigned long handle);
void SOUND_PlaneShift(int newPlane);
void SOUND_SetMusicVariable(int variable, int value);
void SOUND_PutMusicCommand(int cmdType, int cmdData);
void SOUND_ProcessMusicLoad();
unsigned long SndUpdateVolPanPitch(unsigned long handle, unsigned short vol, unsigned short pan, short pitch);
SoundEffectChannel *SndGetSfxChannel(int channelNum);
void SndCloseSfxChannel(int channelNum);
void SOUND_MusicInit();
unsigned long SOUND_Update3dSound(Position *position, unsigned long handle, int pitch, int maxVolume, int minVolDist);
int isOkayToPlaySound(int flags, int spectralPlane, int hidden, int burning);
SoundEffectChannel *SndOpenSfxChannel(unsigned char *channelNum);
int SndIsPlaying(unsigned long handle);
int SndIsPlayingOrRequested(unsigned long handle);
void processPeriodicSound(Position *position, int livesInOnePlane, int inSpectral, int hidden, int burning, SoundInstance *soundInst, ObjectPeriodicSound *sound);
void processEventSound(Position *position, SoundInstance *soundInst, ObjectEventSound *sound);
void processOneShotSound(Position *position, int hidden, int burning, long *triggerFlags, SoundInstance *soundInst, ObjectOneShotSound *sound);
unsigned long SndPlay(unsigned int sample);
void SOUND_ResumeAllSound();
void SOUND_PauseAllSound();
int SndTypeIsPlayingOrRequested(unsigned int sfxToneID);
void SOUND_SetSfxVolume(int newVolume);
void SOUND_SetMusicVolume(int newVolume);
void SOUND_SetVoiceVolume(int newVolume);
void SOUND_SetMusicModifier(long modifier);
void SOUND_ResetMusicModifier(long modifier);
void SOUND_Init();
void SOUND_UpdateSound();
void SOUND_Free();
int SOUND_IsInstanceSoundLoaded(unsigned char *sfxFileData, long soundNumber);
void SOUND_StartInstanceSound(SoundInstance *soundInst);
void SOUND_StopInstanceSound(SoundInstance *soundInst);
void SOUND_SetInstanceSoundVolume(SoundInstance *soundInst, long volumeChangeAmt, long time);
void SOUND_SetInstanceSoundPitch(SoundInstance *soundInst, long pitchChangeAmt, long time);

#endif
