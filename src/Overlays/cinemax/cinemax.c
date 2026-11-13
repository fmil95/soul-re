#include "Overlays/cinemax/cinemax.h"
#include "Game/CINEMA/CINEPSX.h"
#include "Game/MEMPACK.h"

unsigned int D_880125F8;
int D_880125FC;
int D_88012600;
int D_88012604;
int D_88012608;

extern BufferInfo D_880125B8;
extern int D_8801260C;

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

void func_88000138()
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

// Damn near match, but has $s7 and $s8 registers swapped (99.94%!)
#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/Overlays/cinemax/cinemax", func_880001C4);
#else
int func_880001C4(char* strfile, unsigned short mask, int buffers) 
{
    CdlFILE sp18;
    CdlLOC sp30;
    SpuCommonAttr sp38;
    SpuCommonAttr sp60;
    DISPENV sp88;
    char* spA4;
    char* spA8;
    char* spAC;
    char* spB0;
    char* spB4;
    short temp_a0;
    short temp_v0;
    int var_s2;
    int var_s5;
    unsigned short temp_a1;
    unsigned short temp_s0;
    unsigned short var_fp;
    unsigned short var_s7;
    unsigned int temp_lo;

    var_s5 = 0;
    var_fp = 0;
    var_s7 = 0;
    
    D_880125F8 = 1;
    D_8801260C = buffers;

    if (buffers < 2) 
    {
        D_8801260C = 2;
    }
    
    if (D_8801260C >= 4)
    {
        D_8801260C = 3;
    }
    
    D_88012604 = 0;
    D_880125FC = 0;
    D_88012600 = 0;
    
    VSyncCallback(NULL);
    
    SetDispMask(0);
    
    func_880000C4();
    
    if (CdSearchFile(&sp18, strfile) == NULL) 
    {
        printf("file not found\n");
        
        return 0;
    }
    
    D_88012608 = (CdPosToInt(&sp18.pos) + (sp18.size / 2048)) - 5;
    
    SpuGetCommonAttr(&sp38);
    SpuGetCommonAttr(&sp60);
    
    sp38.ext.volume.left  = 16383;
    sp38.ext.volume.right = 16383;
    
    sp38.mvol.left  = 16383;
    sp38.mvol.right = 16383;
    
    sp38.cd.volume.left  = 16383;
    sp38.cd.volume.right = 16383;
    
    sp38.mvolmode.left  = 0;
    sp38.mvolmode.right = 0;
    
    sp38.ext.reverb = 0;
    sp38.ext.mix    = 1;
    
    sp38.cd.reverb = 0;
    sp38.cd.mix    = 1;
    
    sp38.mask |= 0x3FCF;
    sp60.mask  = sp38.mask & ~0xC;
    
    SpuSetCommonAttr(&sp38);
    
    spA4 = MEMPACK_Malloc(0x18000, 9);
    spA8 = MEMPACK_Malloc(0x10000, 9);
    spAC = MEMPACK_Malloc(0x10000, 9);
    spB0 = MEMPACK_Malloc(0x2D00,  9);
    spB4 = MEMPACK_Malloc(0x2D00,  9);
    
    func_880006C0(spA8, spAC, spB0, spB4, &D_880125B8);
    
    while (CdControlB(CdlSetloc, &sp18.pos.minute, NULL) == 0);
    while (CdControlB(CdlSeekL, NULL, NULL) == 0);
    
    CdSync(0, NULL);
    
    VSyncCallback(func_88000138);
    
    func_88000720(spA4, &sp18.pos, func_880007C8);
    
    while (func_88000904(&D_880125B8) == -1) 
    {
        sp30 = sp18.pos;
        
        func_88000BB8(&sp30);
    }
    do 
    {
        bufferstruct_t* bs;

        bs = (bufferstruct_t*)&D_880125B8;
        
        DecDCTin((unsigned long*)((intptr_t*)&bs->anchor1)[bs->index1], 3);
        
        temp_lo = D_880125B8.slice.w * D_880125B8.slice.h;
        
        DecDCTout((unsigned long*)((intptr_t*)&bs->anchor2)[bs->index2], (int)(temp_lo + (temp_lo >> 31)) >> 1);
        
        while ((func_88000904(&D_880125B8) == -1) && (D_88012604 != 1)) 
        {
            if (StGetBackloc(&sp30) <= 0) 
            {
                sp30 = sp18.pos;
            }
            
            func_88000BB8(&sp30);
        }
        
        func_88000B04(&D_880125B8, 0);
        
        if (func_88000068() != 0) 
        {
            D_88012604 = 1;
        }
        
        temp_s0 = CINE_Pad(0);
        temp_a1 = CINE_Pad(1);
        
        temp_v0 = var_s7 & ~temp_s0;
        temp_a0 = var_fp & ~temp_a1;
        
        if (temp_v0 & mask)
        {
            var_s5 = temp_v0 & mask;
        }
        
        var_s7 = temp_s0;
        
        if (temp_a0 & mask) 
        {
            var_s5 = temp_a0 & mask;
        }
        
        var_fp = temp_a1;
        
        if (var_s5 != 0) 
        {
            D_88012604 = 1;
        }
    } while (D_88012604 != 1);
    
    StSetMask(1, 0, 0);
    
    VSyncCallback(NULL);
    DecDCToutCallback(NULL);
    
    func_880000C4();
    
    DrawSync(0);
    
    SetDefDispEnv(&sp88, D_880125B8.rect[0].x, D_880125B8.rect[0].y, 320, 240);
    PutDispEnv(&sp88);
    
    for (var_s2 = 5; var_s2 >= 0; --var_s2) 
    {
        sp38.cd.volume.right = sp38.cd.volume.left = (var_s2 * 16383) / 6;
        
        SpuSetCommonAttr(&sp38);
        
        VSync(0);
    } 
    
    CdControlB(CdlPause, NULL, NULL);
    
    StClearRing();
    StUnSetRing();
    
    SpuSetCommonAttr(&sp60);
    
    MEMPACK_Free(spA4);
    MEMPACK_Free(spA8);
    MEMPACK_Free(spAC);
    MEMPACK_Free(spB0);
    MEMPACK_Free(spB4);
    
    return D_88012604 != 0;
}
#endif 

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

void func_88000794(BufferInfo *bufferInfo)
{
    if (++bufferInfo->unk_30 >= D_8801260C)
    {
        bufferInfo->unk_30 = 0;
    }
}

void func_880007C8()
{
    RECT snap;
    int id;

    if (CINE_CDIntrQuery() != 0)
    {
        StCdInterrupt();
    }

    id = D_880125B8.imgid;

    snap = D_880125B8.slice;

    D_880125B8.imgid = (D_880125B8.imgid != 0) ? 0 : 1;

    D_880125B8.slice.x += D_880125B8.slice.w;

    if (D_880125B8.slice.x < (D_880125B8.rect[D_880125B8.unk_30].x + D_880125B8.rect[D_880125B8.unk_30].w))
    {
        DecDCTout((unsigned long *)D_880125B8.imgbuf[D_880125B8.imgid], (D_880125B8.slice.w * D_880125B8.slice.h) / 2);
    }
    else
    {
        D_880125B8.unk_3C = 1;

        D_880125B8.unk_32 = D_880125B8.unk_30;

        func_88000794(&D_880125B8);

        D_880125B8.slice.x = D_880125B8.rect[D_880125B8.unk_30].x;
        D_880125B8.slice.y = D_880125B8.rect[D_880125B8.unk_30].y;
    }

    LoadImage(&snap, (unsigned long *)D_880125B8.imgbuf[id]);
}

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

char *func_880009B4(BufferInfo *bufferInfo) 
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
    
    bufferInfo->rect[0].w = bufferInfo->rect[1].w = bufferInfo->rect[2].w = (int)(mult + (mult >> 31)) >> 1; 
    bufferInfo->rect[0].h = bufferInfo->rect[1].h = bufferInfo->rect[2].h = D_88012600;
    
    bufferInfo->slice.h = D_88012600;
    
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

void func_88000138()
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

int func_880001C4(char* strfile, unsigned short mask, int buffers) 
{
    CdlFILE sp18;
    CdlLOC sp30;
    SpuCommonAttr sp38;
    SpuCommonAttr sp60;
    DISPENV sp88;
    char* spA4;
    char* spA8;
    char* spAC;
    char* spB0;
    char* spB4;
    short temp_a0;
    short temp_v0;
    int var_s2;
    int var_s5;
    unsigned short temp_a1;
    unsigned short temp_s0;
    unsigned short var_fp;
    unsigned short var_s7;
    unsigned int temp_lo;

    var_s5 = 0;
    var_fp = 0;
    var_s7 = 0;
    
    D_880125F8 = 1;
    D_8801260C = buffers;

    if (buffers < 2) 
    {
        D_8801260C = 2;
    }
    
    if (D_8801260C >= 4)
    {
        D_8801260C = 3;
    }
    
    D_88012604 = 0;
    D_880125FC = 0;
    D_88012600 = 0;
    
    VSyncCallback(NULL);
    
    SetDispMask(0);
    
    func_880000C4();
    
    if (CdSearchFile(&sp18, strfile) == NULL) 
    {
        printf("file not found\n");
        
        return 0;
    }
    
    D_88012608 = (CdPosToInt(&sp18.pos) + (sp18.size / 2048)) - 5;
    
    SpuGetCommonAttr(&sp38);
    SpuGetCommonAttr(&sp60);
    
    sp38.ext.volume.left  = 16383;
    sp38.ext.volume.right = 16383;
    
    sp38.mvol.left  = 16383;
    sp38.mvol.right = 16383;
    
    sp38.cd.volume.left  = 16383;
    sp38.cd.volume.right = 16383;
    
    sp38.mvolmode.left  = 0;
    sp38.mvolmode.right = 0;
    
    sp38.ext.reverb = 0;
    sp38.ext.mix    = 1;
    
    sp38.cd.reverb = 0;
    sp38.cd.mix    = 1;
    
    sp38.mask |= 0x3FCF;
    sp60.mask  = sp38.mask & ~0xC;
    
    SpuSetCommonAttr(&sp38);
    
    spA4 = MEMPACK_Malloc(0x18000, 9);
    spA8 = MEMPACK_Malloc(0x10000, 9);
    spAC = MEMPACK_Malloc(0x10000, 9);
    spB0 = MEMPACK_Malloc(0x2D00,  9);
    spB4 = MEMPACK_Malloc(0x2D00,  9);
    
    func_880006C0(spA8, spAC, spB0, spB4, &D_880125B8);
    
    while (CdControlB(CdlSetloc, &sp18.pos.minute, NULL) == 0);
    while (CdControlB(CdlSeekL, NULL, NULL) == 0);
    
    CdSync(0, NULL);
    
    VSyncCallback(func_88000138);
    
    func_88000720(spA4, &sp18.pos, func_880007C8);
    
    while (func_88000904(&D_880125B8) == -1) 
    {
        sp30 = sp18.pos;
        
        func_88000BB8(&sp30);
    }
    do 
    {
        bufferstruct_t* bs;

        bs = (bufferstruct_t*)&D_880125B8;
        
        DecDCTin((unsigned long*)((intptr_t*)&bs->anchor1)[bs->index1], 3);
        
        temp_lo = D_880125B8.slice.w * D_880125B8.slice.h;
        
        DecDCTout((unsigned long*)((intptr_t*)&bs->anchor2)[bs->index2], (int)(temp_lo + (temp_lo >> 31)) >> 1);
        
        while ((func_88000904(&D_880125B8) == -1) && (D_88012604 != 1)) 
        {
            if (StGetBackloc(&sp30) <= 0) 
            {
                sp30 = sp18.pos;
            }
            
            func_88000BB8(&sp30);
        }
        
        func_88000B04(&D_880125B8, 0);
        
        if (func_88000068() != 0) 
        {
            D_88012604 = 1;
        }
        
        temp_s0 = CINE_Pad(0);
        temp_a1 = CINE_Pad(1);
        
        temp_v0 = var_s7 & ~temp_s0;
        temp_a0 = var_fp & ~temp_a1;
        
        if (temp_v0 & mask)
        {
            var_s5 = temp_v0 & mask;
        }
        
        var_s7 = temp_s0;
        
        if (temp_a0 & mask) 
        {
            var_s5 = temp_a0 & mask;
        }
        
        var_fp = temp_a1;
        
        if (var_s5 != 0) 
        {
            D_88012604 = 1;
        }
    } while (D_88012604 != 1);
    
    StSetMask(1, 0, 0);
    
    VSyncCallback(NULL);
    DecDCToutCallback(NULL);
    
    func_880000C4();
    
    DrawSync(0);
    
    SetDefDispEnv(&sp88, D_880125B8.rect[0].x, D_880125B8.rect[0].y, 320, 240);
    PutDispEnv(&sp88);
    
    for (var_s2 = 5; var_s2 >= 0; --var_s2) 
    {
        sp38.cd.volume.right = sp38.cd.volume.left = (var_s2 * 16383) / 6;
        
        SpuSetCommonAttr(&sp38);
        
        VSync(0);
    } 
    
    CdControlB(CdlPause, NULL, NULL);
    
    StClearRing();
    StUnSetRing();
    
    SpuSetCommonAttr(&sp60);
    
    MEMPACK_Free(spA4);
    MEMPACK_Free(spA8);
    MEMPACK_Free(spAC);
    MEMPACK_Free(spB0);
    MEMPACK_Free(spB4);
    
    return D_88012604 != 0;
}

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

void func_88000794(BufferInfo *bufferInfo)
{
    if (++bufferInfo->unk_30 >= D_8801260C)
    {
        bufferInfo->unk_30 = 0;
    }
}

void func_880007C8()
{
    RECT snap;
    int id;

    if (CINE_CDIntrQuery() != 0)
    {
        StCdInterrupt();
    }

    id = D_880125B8.imgid;

    snap = D_880125B8.slice;

    D_880125B8.imgid = (D_880125B8.imgid != 0) ? 0 : 1;

    D_880125B8.slice.x += D_880125B8.slice.w;

    if (D_880125B8.slice.x < (D_880125B8.rect[D_880125B8.unk_30].x + D_880125B8.rect[D_880125B8.unk_30].w))
    {
        DecDCTout((unsigned long *)D_880125B8.imgbuf[D_880125B8.imgid], (D_880125B8.slice.w * D_880125B8.slice.h) / 2);
    }
    else
    {
        D_880125B8.unk_3C = 1;
        
        D_880125B8.unk_32 = D_880125B8.unk_30;

        func_88000794(&D_880125B8);

        D_880125B8.slice.x = D_880125B8.rect[D_880125B8.unk_30].x;
        D_880125B8.slice.y = D_880125B8.rect[D_880125B8.unk_30].y;
    }

    LoadImage(&snap, (unsigned long *)D_880125B8.imgbuf[id]);
}

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

char *func_880009B4(BufferInfo *bufferInfo) 
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
    
    bufferInfo->rect[0].w = bufferInfo->rect[1].w = bufferInfo->rect[2].w = (int)(mult + (mult >> 31)) >> 1; 
    bufferInfo->rect[0].h = bufferInfo->rect[1].h = bufferInfo->rect[2].h = D_88012600;
    
    bufferInfo->slice.h = D_88012600;
    
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
