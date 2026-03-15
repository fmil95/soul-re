#include "Overlays/cinemax/cinemax.h"

// TODO: get rid of these defines by identifying the SDK functions properly via symbol_addrs.txt
#define DecDCTReset func_88000F78
#define DecDCToutCallback func_88001048 

// this conditional is for the objdiff report
#ifndef SKIP_ASM

INCLUDE_ASM("asm/nonmatchings/Overlays/cinemax/cinemax", func_88000068);

int func_880000A4(char* strfile, unsigned short mask, int buffers)
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

void func_880006C0(char* b1, char* b2, char* b3, char* b4, BufferInfo* bd)
{
	bd->unk_22 = 256;
	bd->unk_28 = 480;
    
	bd->unk_32 = -1;
    
	bd->buffer[0] = b1;
	bd->buffer[1] = b2;
	bd->buffer[2] = NULL;
    
	bd->buffer[3] = b3;
	bd->buffer[4] = b4;
	bd->buffer[5] = NULL;

	bd->unk_18 = 0;
	bd->unk_1A = 0;
	bd->unk_20 = 0;
	bd->unk_2A = 0;
	bd->unk_30 = 0;
	bd->unk_34 = 0;
	bd->unk_36 = 0;
	bd->unk_38 = 24;
	bd->unk_3C = 0;
}

void func_88000720(char* buffer, CdlLOC* fp, void (*func)())
{
	DecDCTReset(0);
    
	DecDCToutCallback(func);
    
	StSetRing((unsigned long*)buffer, (RING_SIZE * 2) + 2);
	StSetStream(1, 1, -1, NULL, NULL);
    
	func_88000BB8(fp);
}

INCLUDE_ASM("asm/nonmatchings/Overlays/cinemax/cinemax", func_88000794);

INCLUDE_ASM("asm/nonmatchings/Overlays/cinemax/cinemax", func_880007C8);

INCLUDE_ASM("asm/nonmatchings/Overlays/cinemax/cinemax", func_88000904);

INCLUDE_ASM("asm/nonmatchings/Overlays/cinemax/cinemax", func_880009B4);

INCLUDE_ASM("asm/nonmatchings/Overlays/cinemax/cinemax", func_88000B04);

void func_88000BB8(CdlLOC* fp)
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

INCLUDE_ASM("asm/nonmatchings/Overlays/cinemax/cinemax", func_88000C28);

INCLUDE_ASM("asm/nonmatchings/Overlays/cinemax/cinemax", func_88000F78);

INCLUDE_ASM("asm/nonmatchings/Overlays/cinemax/cinemax", func_88000FAC);

INCLUDE_ASM("asm/nonmatchings/Overlays/cinemax/cinemax", func_88001028);

INCLUDE_ASM("asm/nonmatchings/Overlays/cinemax/cinemax", func_88001048);

INCLUDE_ASM("asm/nonmatchings/Overlays/cinemax/cinemax", func_8800106C);

INCLUDE_ASM("asm/nonmatchings/Overlays/cinemax/cinemax", func_8800115C);

INCLUDE_ASM("asm/nonmatchings/Overlays/cinemax/cinemax", func_880011EC);

INCLUDE_ASM("asm/nonmatchings/Overlays/cinemax/cinemax", func_88001278);

INCLUDE_ASM("asm/nonmatchings/Overlays/cinemax/cinemax", func_8800130C);

INCLUDE_ASM("asm/nonmatchings/Overlays/cinemax/cinemax", func_880013A0);

INCLUDE_ASM("asm/nonmatchings/Overlays/cinemax/cinemax", func_880013B8);

#else

void func_88000068(void) { };

void func_880000A4(void) { };

void func_880000C4(void) { };

void func_88000138(void) { };

void func_880001C4(void) { };

void func_880006C0(void) { };

void func_88000720(void) { };

void func_88000794(void) { };

void func_880007C8(void) { };

void func_88000904(void) { };

void func_880009B4(void) { };

void func_88000B04(void) { };

void func_88000BB8(void) { };

void func_88000C28(void) { };

void func_88000F78(void) { };

void func_88000FAC(void) { };

void func_88001028(void) { };

void func_88001048(void) { };

void func_8800106C(void) { };

void func_8800115C(void) { };

void func_880011EC(void) { };

void func_88001278(void) { };

void func_8800130C(void) { };

void func_880013A0(void) { };

void func_880013B8(void) { };

#endif
