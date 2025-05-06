#include "common.h"
#include "Game/FONT.h"

static char fp_str[512];

void FONT_MakeSpecialFogClut(int x, int y)
{

    int n;
    short cl[16];
    RECT myrect;

    for (n = 0; n < 16; n++)
    {
        cl[n] = 0x4210;
    }

    cl[0] = 0;

    myrect.x = x;
    myrect.y = y;
    myrect.w = 0x10;
    myrect.h = 1;

    SpecialFogClut = getClut(x, y);
    DrawSync(0);
    LoadImage(&myrect, (u_long *)cl);
    DrawSync(0);
}

INCLUDE_ASM("asm/nonmatchings/Game/FONT", FONT_Init);

INCLUDE_ASM("asm/nonmatchings/Game/FONT", FONT_ReloadFont);

INCLUDE_ASM("asm/nonmatchings/Game/FONT", FONT_DrawChar);

INCLUDE_ASM("asm/nonmatchings/Game/FONT", FONT_Get2DImageIndex);

INCLUDE_ASM("asm/nonmatchings/Game/FONT", drawChar2DPoly);

INCLUDE_ASM("asm/nonmatchings/Game/FONT", FONT_DrawChar2D);

INCLUDE_ASM("asm/nonmatchings/Game/FONT", FONT_CharSpacing);

INCLUDE_ASM("asm/nonmatchings/Game/FONT", FONT_AddCharToBuffer);

INCLUDE_ASM("asm/nonmatchings/Game/FONT", FONT_Print);

INCLUDE_ASM("asm/nonmatchings/Game/FONT", FONT_Print2);

INCLUDE_ASM("asm/nonmatchings/Game/FONT", FONT_GetStringWidth);

INCLUDE_ASM("asm/nonmatchings/Game/FONT", FONT_Flush);

void FONT_SetCursor(short x, short y)
{
    fontTracker.font_xpos = x;
    fontTracker.font_ypos = y;
}

INCLUDE_ASM("asm/nonmatchings/Game/FONT", FONT_VaReallyPrint);

void FONT_FontPrintCentered(char *text, long y)
{
    FONT_SetCursor((512 / 2) - (FONT_GetStringWidth(text) >> 1), y);

    FONT_Print2(text);
}

INCLUDE_ASM("asm/nonmatchings/Game/FONT", FONT_SetColorIndex);

INCLUDE_ASM("asm/nonmatchings/Game/FONT", FONT_SetColorIndexCol);
