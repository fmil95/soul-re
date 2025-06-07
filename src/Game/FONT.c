#include "Game/FONT.h"
#include "Game/MEMPACK.h"
#include "Game/LOAD3D.h"
#include "Game/VRAM.h"

unsigned char fontTransTable[128];

static char fp_str[512];

STATIC font_color_t the_font_color_table[5];

BlockVramEntry *FONT_vramBlock;

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

extern char D_800D0674[];
void FONT_Init()
{
    unsigned long *timAddr;
    short x;
    short y;

    FONT_vramBlock = VRAM_CheckVramSlot(&x, &y, 16, 128, 3, -1);

    if (FONT_vramBlock != NULL)
    {
        // timAddr = LOAD_ReadFile("\\kain2\\game\\font.tim", 5);
        timAddr = (unsigned long *)LOAD_ReadFile(D_800D0674, 5);

        LOAD_LoadTIM((long *)timAddr, x, y, x, y + 126);

        MEMPACK_Free((char *)timAddr);

        fontTracker.sprite_sort_push = 0;

        fontTracker.font_tpage = getTPage(0, 0, x, y);
        fontTracker.font_clut = getClut(x, y + 126);

        fontTracker.font_vramX = x;
        fontTracker.font_vramY = y;

        fontTracker.font_vramU = (x & 0x3F) * 4;
        fontTracker.font_vramV = y & 0xFF;

        FONT_MakeSpecialFogClut(x, y + 127);
    }

    fontTracker.font_xpos = 10;
    fontTracker.font_ypos = 16;

    fontTracker.font_buffIndex = 0;

    fontTracker.sprite_sort_push = 0;

    fontTracker.color_global = 0;
    fontTracker.color_local = 0;
}

void FONT_ReloadFont()
{
    unsigned long *timAddr;

    timAddr = (unsigned long *)LOAD_ReadFile(D_800D0674, 5);
    // timAddr = (unsigned long*)LOAD_ReadFile("\\kain2\\game\\font.tim", 5);

    LOAD_LoadTIM((long *)timAddr, fontTracker.font_vramX, fontTracker.font_vramY, fontTracker.font_vramX, fontTracker.font_vramY + 126);

    MEMPACK_Free((char *)timAddr);

    FONT_MakeSpecialFogClut(fontTracker.font_vramX, fontTracker.font_vramY + 127);
}

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

void FONT_Print(const char *fmt, ...)
{
    char *hold;
    va_list ap;

    va_start(ap, fmt);

    vsprintf(fp_str, fmt, ap);

    for (hold = fp_str; *hold != '\0'; hold++)
    {
        if ((*hold >= 'A') && (*hold <= 'Z'))
        {
            *hold += 'a' - 'A';
        }
    }

    FONT_VaReallyPrint(fp_str, ap);

    va_end(ap);
}

void FONT_Print2(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);

    vsprintf(fp_str, fmt, ap);

    FONT_VaReallyPrint(fp_str, ap);

    va_end(ap);
}

int FONT_GetStringWidth(char *str)
{
    int w;
    int len;
    int i;

    len = strlen(str);

    w = 0;

    for (i = 0; i < len; i++)
    {
        w += FONT_CharSpacing(str[i], 8);
    }

    return w;
}

void FONT_Flush()
{
    long i;
    FontChar *fontChar;

    fontTracker.font_xpos = 10;
    fontTracker.font_ypos = 16;

    if (fontTracker.font_buffIndex == 0)
    {
        return;
    }

    fontChar = fontTracker.font_buffer;

    for (i = fontTracker.font_buffIndex; i != 0; i--, fontChar++)
    {
        if ((fontChar->c != ' ') && (fontChar->c != '@'))
        {
            FONT_DrawChar(fontChar);
        }
    }

    fontTracker.font_buffIndex = 0;
}

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

void FONT_SetColorIndex(int color)
{
    fontTracker.color_global = color;
}

void FONT_SetColorIndexCol(int color, int r, int g, int b)
{
    font_color_t *fcol;

    fcol = &the_font_color_table[color];

    fcol->r = r;
    fcol->g = g;
    fcol->b = b;
}
