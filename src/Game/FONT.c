#include "common.h"
#include "Game/FONT.h"

static char fp_str[512];

INCLUDE_ASM("asm/nonmatchings/Game/FONT", FONT_MakeSpecialFogClut);

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

INCLUDE_ASM("asm/nonmatchings/Game/FONT", FONT_SetCursor);

INCLUDE_ASM("asm/nonmatchings/Game/FONT", FONT_VaReallyPrint);

void FONT_FontPrintCentered(char *text, long y)
{
    FONT_SetCursor((512 / 2) - (FONT_GetStringWidth(text) >> 1), y);

    FONT_Print2(text);
}

INCLUDE_ASM("asm/nonmatchings/Game/FONT", FONT_SetColorIndex);

INCLUDE_ASM("asm/nonmatchings/Game/FONT", FONT_SetColorIndexCol);
