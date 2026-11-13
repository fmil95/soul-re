#include "Overlays/cinemax/cinemax.h"
#include "Game/CINEMA/CINEPSX.h"

unsigned int D_880125F8;
int D_880125FC;
int D_88012600;
int D_88012604;
int D_88012608;

extern BufferInfo D_880125B8;
extern int D_8801260C;

#define CINE_RECT(info, i) (((RECT *)&(info)->rect[0].x)[i])

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

void func_88000138(void)
{
    DISPENV disp;

    if (D_880125B8.unk_32 != -1)
    {
        SetDefDispEnv(&disp, D_880125B8.rect[D_880125B8.unk_32].x, D_880125B8.rect[D_880125B8.unk_32].y, 480, 240);

        disp.isrgb24 = 1;

        disp.disp.w = (disp.disp.w * 2) / 3;

        PutDispEnv(&disp);

        SetDispMask(1);
    }
}

INCLUDE_RODATA("asm/nonmatchings/Overlays/cinemax/cinemax", D_88000000);
INCLUDE_ASM("asm/nonmatchings/Overlays/cinemax/cinemax", func_880001C4);

void func_880006C0(char *buff1, char *buff2, char *buff3, char *buff4, BufferInfo *bufferInfo)
{
    bufferInfo->rect[1].y = 256;
    bufferInfo->rect[2].x = 480;

    bufferInfo->unk_32 = -1;

    bufferInfo->vlcbuf[0] = buff1;
    bufferInfo->vlcbuf[1] = buff2;

    bufferInfo->vlcid = 0;

    bufferInfo->imgbuf[0] = buff3;
    bufferInfo->imgbuf[1] = buff4;

    bufferInfo->imgid = 0;

    bufferInfo->rect[0].x = 0;
    bufferInfo->rect[0].y = 0;

    bufferInfo->rect[1].x = 0;
    bufferInfo->rect[2].y = 0;

    bufferInfo->unk_30 = 0;

    bufferInfo->slice.x = 0;
    bufferInfo->slice.y = 0;
    bufferInfo->slice.w = 24;

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
        bufferInfo->vlcid = !bufferInfo->vlcid;
        
        DecDCTvlc((unsigned long*)buffer, (unsigned long*)bufferInfo->vlcbuf[bufferInfo->vlcid]);
        
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
    
    buffer_info->rect[0].w = buffer_info->rect[1].w = buffer_info->rect[2].w = (int)(mult + (mult >> 31)) >> 1; 
    buffer_info->rect[0].h = buffer_info->rect[1].h = buffer_info->rect[2].h = D_88012600;
    
    buffer_info->slice.h = D_88012600;
    
    return buffer;
}

void func_88000B04(BufferInfo *bufferInfo, int mode)
{
    volatile int count;

    (void)mode;

    count = 0x800000;

    while (bufferInfo->unk_3C == 0)
    {
        if (--count == 0)
        {
            bufferInfo->unk_3C = 1;
            bufferInfo->unk_32 = bufferInfo->unk_30;

            func_88000794(bufferInfo);

            bufferInfo->slice.x = bufferInfo->rect[bufferInfo->unk_30].x;
            bufferInfo->slice.y = bufferInfo->rect[bufferInfo->unk_30].y;
        }
    }

    bufferInfo->unk_3C = 0;
}

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

void func_88000138(void)
{
    DISPENV disp;

    if (D_880125B8.unk_32 != -1)
    {
        SetDefDispEnv(&disp, D_880125B8.rect[D_880125B8.unk_32].x, D_880125B8.rect[D_880125B8.unk_32].y, 480, 240);

        disp.isrgb24 = 1;
        
        disp.disp.w = (disp.disp.w * 2) / 3;

        PutDispEnv(&disp);

        SetDispMask(1);
    }
}

int func_880001C4(char *strfile, unsigned short mask, int buffers) {};

void func_880006C0(char *buff1, char *buff2, char *buff3, char *buff4, BufferInfo *bufferInfo)
{
    bufferInfo->rect[1].y = 256;
    bufferInfo->rect[2].x = 480;

    bufferInfo->unk_32 = -1;

    bufferInfo->vlcbuf[0] = buff1;
    bufferInfo->vlcbuf[1] = buff2;

    bufferInfo->vlcid = 0;

    bufferInfo->imgbuf[0] = buff3;
    bufferInfo->imgbuf[1] = buff4;

    bufferInfo->imgid = 0;

    bufferInfo->rect[0].x = 0;
    bufferInfo->rect[0].y = 0;

    bufferInfo->rect[1].x = 0;
    bufferInfo->rect[2].y = 0;

    bufferInfo->unk_30 = 0;

    bufferInfo->slice.x = 0;
    bufferInfo->slice.y = 0;
    bufferInfo->slice.w = 24;

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

void func_88000794(BufferInfo *bufferInfo) {}

void func_880007C8(void) {}

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
        bufferInfo->vlcid = !bufferInfo->vlcid;
        
        DecDCTvlc((unsigned long*)buffer, (unsigned long*)bufferInfo->vlcbuf[bufferInfo->vlcid]);
        
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
    
    buffer_info->rect[0].w = buffer_info->rect[1].w = buffer_info->rect[2].w = (int)(mult + (mult >> 31)) >> 1; 
    buffer_info->rect[0].h = buffer_info->rect[1].h = buffer_info->rect[2].h = D_88012600;
    
    buffer_info->slice.h = D_88012600;
    
    return buffer;
}

void func_88000B04(BufferInfo *bufferInfo, int mode)
{
    volatile int count;

    (void)mode;

    count = 0x800000;

    while (bufferInfo->unk_3C == 0)
    {
        if (--count == 0)
        {
            bufferInfo->unk_3C = 1;
            bufferInfo->unk_32 = bufferInfo->unk_30;

            func_88000794(bufferInfo);

            bufferInfo->slice.x = bufferInfo->rect[bufferInfo->unk_30].x;
            bufferInfo->slice.y = bufferInfo->rect[bufferInfo->unk_30].y;
        }
    }

    bufferInfo->unk_3C = 0;
}

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

#endif
