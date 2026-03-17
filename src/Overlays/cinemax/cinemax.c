#include "Overlays/cinemax/cinemax.h"

unsigned int D_880125F8;
int D_880125FC;
int D_88012600;
int D_88012604;
int D_88012608;

// this conditional is for the objdiff report
#ifndef SKIP_ASM

int func_88000068()
{
    CdlLOC loc[2]; // should be just one element, but fuck knows why only two match

    CdControlB(CdlGetlocP, NULL, &loc[0].minute);

    return CdPosToInt((CdlLOC*)&loc[1].second) >= D_88012608;
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

    StSetRing((unsigned long*)buffer, (RING_SIZE * 2) + 2);
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

char *func_880009B4(BufferInfo *buffer_info) 
{
    int i;
	char* buffer;
	StHEADER* header; 
    unsigned int mult;
    int temp;

    for (i = 14000; i != 0; ) 
    {
        if (StGetNext((unsigned long**)&buffer, (unsigned long**)&header) == 0)
        {
            break;
        }

        if (--i == 0)
        {
            if (func_88000068() != 0)
            {
                D_88012604 = 1;
                
                return 0;
            }
            else
            {
                return 0;
            }
        }
    }

    if (header != NULL) 
    {
        if (header->frameCount < D_880125F8) 
        {
            D_88012604 = 1;
            
            return 0;
        }
    
        D_880125F8 = header->frameCount;
    }

	if (func_88000068() != 0)
	{
        D_88012604 = 1;
	}

    if ((D_880125FC != header->width) || (D_88012600 != header->height)) 
    {
        func_880000C4();
        
        D_880125FC = header->width;
        D_88012600 = header->height;
    }

    mult = D_880125FC * 3;
    
    temp = (int)(mult + (mult >> 31)) >> 1;
    
    buffer_info->unk_2C = temp; 
    buffer_info->unk_24 = temp;
    buffer_info->unk_1C = temp;
    
    buffer_info->unk_2E = D_88012600;
    buffer_info->unk_26 = D_88012600;
    buffer_info->unk_1E = D_88012600;
    buffer_info->unk_3A = D_88012600;
    
    return buffer;
}

INCLUDE_ASM("asm/nonmatchings/Overlays/cinemax/cinemax", func_88000B04);

void func_88000BB8(CdlLOC *fp)
{
    unsigned char param;

    param = CdlModeSpeed;

    do
    {
        while (CdControl(CdlSetloc, (unsigned char*)fp, NULL) == 0);
        while (CdControl(CdlSetmode, &param, NULL) == 0);

        VSync(3);
    } while (CdRead2(CdlModeStream | CdlModeSpeed | CdlModeRT | CdlModeSize1) == 0);
}

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

char *func_880009B4(BufferInfo *buffer_info) {};

void func_88000B04(void) {};

void func_88000BB8(CdlLOC *fp) {};

#endif
