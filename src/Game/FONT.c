#include "common.h"
#include "Game/FONT.h"

unsigned char fontTransTable[128];

static char fp_str[512];

STATIC font_color_t the_font_color_table[5];

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

void FONT_DrawChar(FontChar *fontChar)
{

    char c;
    long x;
    long y;

    c = fontChar->c;
    x = fontChar->x;
    y = fontChar->y;
    fontTracker.color_local = fontChar->color;

    FONT_DrawChar2D(c & 0xFF, x, y);
}

long FONT_Get2DImageIndex(unsigned char c)
{
    return fontTransTable[c];
}

INCLUDE_ASM("asm/nonmatchings/Game/FONT", drawChar2DPoly);

INCLUDE_ASM("asm/nonmatchings/Game/FONT", FONT_DrawChar2D);

INCLUDE_ASM("asm/nonmatchings/Game/FONT", FONT_CharSpacing);

void FONT_AddCharToBuffer(char c, long x, long y)
{
    FontChar *fontChar;

    fontChar = &fontTracker.font_buffer[fontTracker.font_buffIndex];

    if (fontTracker.font_buffIndex < 255)
    {
        if (c == '@')
        {
            fontChar->x = x & 0xFF;
            fontChar->y = y & 0xFF;
        }
        else
        {
            fontChar->x = x;
            fontChar->y = y;
        }

        fontChar->c = c;

        fontTracker.font_buffIndex++;

        fontChar->color = fontTracker.color_global;
    }
}

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

void FONT_SetColorIndexCol(int color, int r, int g, int b)
{
    font_color_t *fcol;

    fcol = &the_font_color_table[color];

    fcol->r = r;
    fcol->g = g;
    fcol->b = b;
}
