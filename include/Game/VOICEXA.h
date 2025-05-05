#ifndef _VOICEXA_H_
#define _VOICEXA_H_

#include "common.h"

int VOICEXA_FinalStatus(XAVoiceTracker *vt);
void voiceCmdPlay(XAVoiceTracker *vt, short voiceIndex);
void voiceCmdStop(XAVoiceTracker *vt, short cmdParam);
void voiceCmdPause(XAVoiceTracker *vt, short cmdParam);
void voiceCmdResume(XAVoiceTracker *vt, short cmdParam);
void voiceCmdNull(XAVoiceTracker *vt, short cmdParam);
void VOICEXA_Play(int voiceIndex, int queueRequests);
int VOICEXA_IsPlaying();
void VOICEXA_Tick();
void VOICEXA_Resume();
void VOICEXA_Pause();
void VOICEXA_Init();

#endif
