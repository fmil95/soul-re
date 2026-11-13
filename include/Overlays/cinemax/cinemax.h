#ifndef _CINEMAX_H_
#define _CINEMAX_H_

#include "common.h"

typedef struct BufferInfo
{
	char* vlcbuf[2]; 
	int   vlcid;     
	char* imgbuf[2]; 
	int   imgid;     
	RECT  rect[3];   
	short unk_30;    
	short unk_32;    
	RECT  slice;     
	int   unk_3C;
} BufferInfo;

int func_88000068();
int func_880000A4(char *strfile, unsigned short mask, int buffers);
void func_880000C4();
void func_88000138();
int func_880001C4(char *strfile, unsigned short mask, int buffers);
void func_880006C0(char *buff1, char *buff2, char *buff3, char *buff4, BufferInfo *bufferInfo);
void func_88000720(char *buffer, CdlLOC *fp, void (*func)());
void func_88000794(BufferInfo *bufferInfo);
void func_880007C8(void);
char *func_880009B4(BufferInfo *buffer_info);
void func_88000B04(BufferInfo *bufferInfo, int mode);
void func_88000BB8(CdlLOC *fp);

#endif
