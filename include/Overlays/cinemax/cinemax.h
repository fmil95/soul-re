#ifndef _CINEMAX_H_
#define _CINEMAX_H_

#include "common.h"

#define MAX_BUFFER_COUNT (3*2)

typedef struct BufferInfo
{
	char*          buffer[MAX_BUFFER_COUNT];
	unsigned short unk_18;
	unsigned short unk_1A;
	unsigned short unk_1C;
	unsigned short unk_1E;
	unsigned short unk_20;
	unsigned short unk_22;
	unsigned short unk_24;
	unsigned short unk_26;
	unsigned short unk_28;
	unsigned short unk_2A;
	unsigned short unk_2C;
	unsigned short unk_2E;
	unsigned short unk_30;
	short          unk_32;
	unsigned short unk_34;
	unsigned short unk_36;
	unsigned short unk_38;
	unsigned short unk_3A;
	int            unk_3C;
} BufferInfo;

int func_88000068();
int func_880000A4(char* strfile, unsigned short mask, int buffers);
void func_880000C4();
int func_880001C4(char* strfile, unsigned short mask, int buffers);
void func_880006C0(char* buff1, char* buff2, char* buff3, char* buff4, BufferInfo* bufferInfo);
void func_88000720(char* buffer, CdlLOC* fp, void (*func)());
void func_88000BB8(CdlLOC* fp);
void func_88000F78(int mode); // DecDCTReset
int func_88001048(void (*func)()); // DecDCToutCallback

#endif
