#include "Overlays/cinemax/cinemax.h"

int D_88012604;
int D_88012608;

// this conditional is for the objdiff report
#ifndef SKIP_ASM

int func_88000068()
{
    CdlLOC loc[2]; // should be just one element, but fuck knows why only two match

    CdControlB(CdlGetlocP, NULL, &loc[0].minute);

    return CdPosToInt((CdlLOC *)&loc[1].second) >= D_88012608;
}

int func_880000A4(char *strfile, unsigned short mask, int buffers)
{
    return func_880001C4(strfile, mask, buffers);
}

void func_880000C4()
{
    RECT rect;

    rect.x = 0;
    rect.y = 0;
    rect.w = 512;
    rect.h = 256;

    ClearImage(&rect, 0, 0, 0);

    rect.x = 0;
    rect.y = 256;
    rect.w = 512;
    rect.h = 256;

    ClearImage(&rect, 0, 0, 0);
}

INCLUDE_ASM("asm/nonmatchings/Overlays/cinemax/cinemax", func_88000138);

INCLUDE_RODATA("asm/nonmatchings/Overlays/cinemax/cinemax", D_88000000);

INCLUDE_ASM("asm/nonmatchings/Overlays/cinemax/cinemax", func_880001C4);

void func_880006C0(char *buff1, char *buff2, char *buff3, char *buff4, BufferInfo *bufferInfo)
{
    bufferInfo->unk_22 = 256;
    bufferInfo->unk_28 = 480;

    bufferInfo->unk_32 = -1;

    bufferInfo->buffer[0] = buff1;
    bufferInfo->buffer[1] = buff2;
    bufferInfo->buffer[2] = NULL;

    bufferInfo->buffer[3] = buff3;
    bufferInfo->buffer[4] = buff4;
    bufferInfo->buffer[5] = NULL;

    bufferInfo->unk_18 = 0;
    bufferInfo->unk_1A = 0;
    bufferInfo->unk_20 = 0;
    bufferInfo->unk_2A = 0;
    bufferInfo->unk_30 = 0;
    bufferInfo->unk_34 = 0;
    bufferInfo->unk_36 = 0;
    bufferInfo->unk_38 = 24;
    bufferInfo->unk_3C = 0;
}

void func_88000720(char *buffer, CdlLOC *fp, void (*func)())
{
    DecDCTReset(0);

    DecDCToutCallback(func);

    StSetRing((unsigned long *)buffer, (RING_SIZE * 2) + 2);
    StSetStream(1, 1, -1, NULL, NULL);

    func_88000BB8(fp);
}

INCLUDE_ASM("asm/nonmatchings/Overlays/cinemax/cinemax", func_88000794);

INCLUDE_ASM("asm/nonmatchings/Overlays/cinemax/cinemax", func_880007C8);

int func_88000904(BufferInfo *bufferInfo)
{
    int i;
    char *buffer;

	for (i = 150; i != 0; i--) 
    {
		buffer = func_880009B4(bufferInfo);

		if (buffer != NULL) 
        {
            break;
        }
        
        if (D_88012604 == 1) 
        {
            return 0;
        }
	}

    if (buffer != NULL)
    {
        bufferInfo->buffer[2] = (char*)(intptr_t)!bufferInfo->buffer[2];
        
        DecDCTvlc((unsigned long*)buffer, (unsigned long*)bufferInfo->buffer[(intptr_t)bufferInfo->buffer[2]]);
        
        StFreeRing((unsigned long*)buffer);
        
        return 0;
    }
    
    return -1;
}

INCLUDE_ASM("asm/nonmatchings/Overlays/cinemax/cinemax", func_880009B4);

INCLUDE_ASM("asm/nonmatchings/Overlays/cinemax/cinemax", func_88000B04);

void func_88000BB8(CdlLOC *fp)
{
    unsigned char param;

    param = CdlModeSpeed;

    do
    {
        while (CdControl(CdlSetloc, (unsigned char *)fp, NULL) == 0);
        while (CdControl(CdlSetmode, &param, NULL) == 0);

        VSync(3);
    } while (CdRead2(CdlModeStream | CdlModeSpeed | CdlModeRT | CdlModeSize1) == 0);
}

INCLUDE_ASM("asm/nonmatchings/Overlays/cinemax/cinemax", DecDCTvlc);

INCLUDE_ASM("asm/nonmatchings/Overlays/cinemax/cinemax", DecDCTReset);

INCLUDE_ASM("asm/nonmatchings/Overlays/cinemax/cinemax", DecDCTin);

INCLUDE_ASM("asm/nonmatchings/Overlays/cinemax/cinemax", DecDCTout);

INCLUDE_ASM("asm/nonmatchings/Overlays/cinemax/cinemax", DecDCToutCallback);

INCLUDE_ASM("asm/nonmatchings/Overlays/cinemax/cinemax", LIBPRESS_OBJ1);

INCLUDE_ASM("asm/nonmatchings/Overlays/cinemax/cinemax", LIBPRESS_OBJ2);

INCLUDE_ASM("asm/nonmatchings/Overlays/cinemax/cinemax", LIBPRESS_OBJ3);

INCLUDE_ASM("asm/nonmatchings/Overlays/cinemax/cinemax", LIBPRESS_OBJ4);

INCLUDE_ASM("asm/nonmatchings/Overlays/cinemax/cinemax", LIBPRESS_OBJ5);

INCLUDE_ASM("asm/nonmatchings/Overlays/cinemax/cinemax", LIBPRESS_OBJ6);

INCLUDE_ASM("asm/nonmatchings/Overlays/cinemax/cinemax", LIBPRESS_OBJ7);

#else

int func_88000068(void) {};

int func_880000A4(char *strfile, unsigned short mask, int buffers) {};

void func_880000C4(void) {};

void func_88000138(void) {};

int func_880001C4(char *strfile, unsigned short mask, int buffers) {};

void func_880006C0(char *buff1, char *buff2, char *buff3, char *buff4, BufferInfo *bufferInfo) {};

void func_88000720(char *buffer, CdlLOC *fp, void (*func)()) {};

void func_88000794(void) {};

void func_880007C8(void) {};

int func_88000904(BufferInfo *bufferInfo) {};

char *func_880009B4(BufferInfo *bufferInfo) {};

void func_88000B04(void) {};

void func_88000BB8(CdlLOC *fp) {};

int DecDCTvlc(u_long *bs, u_long *buf) {};

void DecDCTReset(int mode) {};

void DecDCTin(u_long *buf, int mode) {};

void DecDCTout(u_long *buf, int size) {};

int DecDCToutCallback(void (*func)()) {};

void LIBPRESS_OBJ1(void) {};

void LIBPRESS_OBJ2(void) {};

void LIBPRESS_OBJ3(void) {};

void LIBPRESS_OBJ4(void) {};

void LIBPRESS_OBJ5(void) {};

void LIBPRESS_OBJ6(void) {};

void LIBPRESS_OBJ7(void) {};

#endif
