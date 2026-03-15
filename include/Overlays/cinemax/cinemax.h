#ifndef _CINEMAX_H_
#define _CINEMAX_H_

#include "common.h"

int func_880000A4(char* strfile, unsigned short mask, int buffers);
void func_880000C4();
int func_880001C4(char* strfile, unsigned short mask, int buffers);
void func_88000720(char* buffer, CdlLOC* fp, void (*func)());
void func_88000BB8(CdlLOC* fp);
void func_88000F78(int mode); // DecDCTReset
int func_88001048(void (*func)()); // DecDCToutCallback

#endif
