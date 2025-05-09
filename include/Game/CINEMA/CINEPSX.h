#ifndef _CINEPSX_H_
#define _CINEPSX_H_

#include "common.h"

int CINE_Load();
void CINE_Play(char *strfile, unsigned short mask, int buffers);
void CINE_Unload();
int CINE_Loaded();

#endif
