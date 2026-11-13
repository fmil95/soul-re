#ifndef _CINEPSX_H_
#define _CINEPSX_H_

#include "common.h"

int CINE_CDIntrQuery();
unsigned short CINE_Pad(int pad);
void CINE_Play(char *strfile, unsigned short mask, int buffers);
int CINE_Load();
int CINE_Loaded();
void CINE_Unload();
void CINE_PlayIngame(int number);

#endif
