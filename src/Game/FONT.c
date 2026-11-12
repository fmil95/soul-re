#include "Game/FONT.h"
#include "Game/MEMPACK.h"
#include "Game/LOAD3D.h"
#include "Game/VRAM.h"
#include "Game/GAMELOOP.h"

FontPos fontPos[] =
{
    { 0x00, 0x00, 0x0B, 0x0C },
    { 0x0B, 0x00, 0x08, 0x0C },
    { 0x14, 0x00, 0x08, 0x0C },
    { 0x1C, 0x00, 0x09, 0x0C },
    { 0x25, 0x00, 0x08, 0x0C },
    { 0x2D, 0x00, 0x08, 0x0C },
    { 0x35, 0x00, 0x0A, 0x0C },
    { 0x00, 0x0C, 0x09, 0x0C },
    { 0x09, 0x0C, 0x03, 0x0C },
    { 0x2A, 0x0C, 0x05, 0x0C },
    { 0x2F, 0x0D, 0x08, 0x0B },
    { 0x01, 0x6F, 0x07, 0x0C },
    { 0x00, 0x1A, 0x0A, 0x0C },
    { 0x0B, 0x1A, 0x08, 0x0C },
    { 0x13, 0x1A, 0x0A, 0x0C },
    { 0x1D, 0x1A, 0x07, 0x0C },
    { 0x2E, 0x1A, 0x08, 0x0C },
    { 0x00, 0x27, 0x09, 0x0C },
    { 0x0A, 0x27, 0x09, 0x0C },
    { 0x13, 0x27, 0x08, 0x0B },
    { 0x1B, 0x27, 0x09, 0x0B },
    { 0x25, 0x27, 0x0A, 0x0C },
    { 0x2F, 0x27, 0x08, 0x0C },
    { 0x01, 0x33, 0x09, 0x0D },
    { 0x0A, 0x33, 0x08, 0x0D },
    { 0x12, 0x33, 0x08, 0x0D },
    { 0x1B, 0x33, 0x07, 0x0D },
    { 0x23, 0x33, 0x08, 0x0D },
    { 0x2C, 0x33, 0x07, 0x0D },
    { 0x34, 0x33, 0x07, 0x0C },
    { 0x0C, 0x0C, 0x08, 0x0C },
    { 0x14, 0x0C, 0x07, 0x0C },
    { 0x21, 0x0C, 0x07, 0x0C },
    { 0x3D, 0x20, 0x03, 0x03 },
    { 0x3D, 0x33, 0x02, 0x0D },
    { 0x36, 0x19, 0x07, 0x0C },
    { 0x30, 0x05, 0x05, 0x07 },
    { 0x1B, 0x0C, 0x05, 0x0C },
    { 0x39, 0x27, 0x06, 0x0B },
    { 0x3D, 0x1A, 0x03, 0x0A },
    { 0x13, 0x1A, 0x05, 0x0C },
    { 0x18, 0x1A, 0x05, 0x0B },
    { 0x10, 0x64, 0x07, 0x0A },
    { 0x31, 0x66, 0x08, 0x07 },
    { 0x31, 0x25, 0x05, 0x0C },
    { 0x20, 0x40, 0x07, 0x0C },
    { 0x24, 0x19, 0x09, 0x05 },
    { 0x14, 0x46, 0x05, 0x04 },
    { 0x00, 0x40, 0x08, 0x0A },
    { 0x28, 0x40, 0x07, 0x0C },
    { 0x3D, 0x33, 0x02, 0xF3 },
    { 0x25, 0x1E, 0x08, 0x02 },
    { 0x12, 0x40, 0x07, 0x04 },
    { 0x0A, 0x40, 0x06, 0x05 },
    { 0x0A, 0x46, 0x06, 0x05 },
    { 0x24, 0x19, 0x08, 0x04 },
    { 0x1B, 0x47, 0x05, 0x04 },
    { 0x2F, 0x40, 0x0B, 0x0C },
    { 0x00, 0x4D, 0x0C, 0x0C },
    { 0x0C, 0x4D, 0x0C, 0x0C },
    { 0x18, 0x4D, 0x0C, 0x0C },
    { 0x24, 0x4D, 0x0C, 0x0C },
    { 0x30, 0x4D, 0x0A, 0x0C },
    { 0x30, 0x4B, 0x0A, 0xF4 },
    { 0x00, 0x59, 0x0C, 0x0B },
    { 0x00, 0x59, 0xF4, 0x0B },
    { 0x0D, 0x59, 0x0E, 0x0B },
    { 0x1C, 0x59, 0x0E, 0x0B },
    { 0x2B, 0x59, 0x0E, 0x0B },
    { 0x01, 0x64, 0x0E, 0x0B },
    { 0x1A, 0x64, 0x09, 0x0B },
    { 0x24, 0x64, 0x0C, 0x0B }
};

char charMap[][3] =
{
    { 0x00, 0xFF, 0xFF },
    { 0x01, 0xFF, 0xFF },
    { 0x02, 0xFF, 0xFF },
    { 0x03, 0xFF, 0xFF },
    { 0x04, 0xFF, 0xFF },
    { 0x05, 0xFF, 0xFF },
    { 0x06, 0xFF, 0xFF },
    { 0x07, 0xFF, 0xFF },
    { 0x08, 0xFF, 0xFF },
    { 0x09, 0xFF, 0xFF },
    { 0x0A, 0xFF, 0xFF },
    { 0x0B, 0xFF, 0xFF },
    { 0x0C, 0xFF, 0xFF },
    { 0x0D, 0xFF, 0xFF },
    { 0x0E, 0xFF, 0xFF },
    { 0x0F, 0xFF, 0xFF },
    { 0x39, 0xFF, 0xFF },
    { 0x10, 0xFF, 0xFF },
    { 0x11, 0xFF, 0xFF },
    { 0x12, 0xFF, 0xFF },
    { 0x13, 0xFF, 0xFF },
    { 0x14, 0xFF, 0xFF },
    { 0x15, 0xFF, 0xFF },
    { 0x16, 0xFF, 0xFF },
    { 0x17, 0xFF, 0xFF },
    { 0x18, 0xFF, 0xFF },
    { 0x0E, 0xFF, 0xFF },
    { 0x08, 0xFF, 0xFF },
    { 0x19, 0xFF, 0xFF },
    { 0x1A, 0xFF, 0xFF },
    { 0x1B, 0xFF, 0xFF },
    { 0x1C, 0xFF, 0xFF },
    { 0x1D, 0xFF, 0xFF },
    { 0x1E, 0xFF, 0xFF },
    { 0x1F, 0xFF, 0xFF },
    { 0x20, 0xFF, 0xFF },
    { 0x21, 0xFF, 0xFF },
    { 0x22, 0xFF, 0xFF },
    { 0x23, 0xFF, 0xFF },
    { 0x24, 0xFF, 0xFF },
    { 0x25, 0xFF, 0xFF },
    { 0x26, 0xFF, 0xFF },
    { 0x27, 0xFF, 0xFF },
    { 0x28, 0xFF, 0xFF },
    { 0x29, 0xFF, 0xFF },
    { 0x2A, 0xFF, 0xFF },
    { 0x2B, 0xFF, 0xFF },
    { 0x2C, 0xFF, 0xFF },
    { 0x2D, 0xFF, 0xFF },
    { 0x2E, 0xFF, 0xFF },
    { 0x2F, 0xFF, 0xFF },
    { 0xFF, 0xFF, 0x2F },
    { 0x02, 0x2F, 0xFF },
    { 0x13, 0xFF, 0x33 },
    { 0x04, 0xFF, 0x35 },
    { 0x00, 0xFF, 0x36 },
    { 0x00, 0xFF, 0x33 },
    { 0x00, 0xFF, 0x34 },
    { 0x30, 0xFF, 0xFF },
    { 0x04, 0xFF, 0x36 },
    { 0x04, 0xFF, 0x33 },
    { 0x04, 0xFF, 0x34 },
    { 0x08, 0xFF, 0x33 },
    { 0x08, 0xFF, 0x36 },
    { 0x08, 0xFF, 0x34 },
    { 0x0E, 0xFF, 0x36 },
    { 0x0E, 0xFF, 0x33 },
    { 0x0E, 0xFF, 0x34 },
    { 0x13, 0xFF, 0x36 },
    { 0x13, 0xFF, 0x34 },
    { 0x17, 0xFF, 0x33 },
    { 0x00, 0xFF, 0x35 },
    { 0x08, 0xFF, 0x35 },
    { 0x0E, 0xFF, 0x35 },
    { 0x13, 0xFF, 0x35 },
    { 0x0D, 0xFF, 0x37 },
    { 0x31, 0xFF, 0xFF },
    { 0x32, 0xFF, 0xFF },
    { 0x3A, 0xFF, 0xFF },
    { 0x3B, 0xFF, 0xFF },
    { 0x3C, 0xFF, 0xFF },
    { 0x3D, 0xFF, 0xFF },
    { 0x3E, 0xFF, 0xFF },
    { 0x3F, 0xFF, 0xFF },
    { 0x40, 0xFF, 0xFF },
    { 0x41, 0xFF, 0xFF },
    { 0x42, 0xFF, 0xFF },
    { 0x43, 0xFF, 0xFF },
    { 0x44, 0xFF, 0xFF },
    { 0x45, 0xFF, 0xFF },
    { 0x46, 0xFF, 0xFF },
    { 0x47, 0xFF, 0xFF }
};

static char fp_str[512];

static font_color_t the_font_color_table[5] = { { 0x00, 0x00, 0x00 }, { 0x40, 0x40, 0x40 }, { 0x40, 0x40, 0xff }, { 0x58, 0x58, 0x68 }, { 0xdc, 0xdc, 0x40 } };

BlockVramEntry *FONT_vramBlock;

unsigned short SpecialFogClut;

FontTracker fontTracker;

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

void FONT_Init()
{
    unsigned long *timAddr;
    short x;
    short y;

    FONT_vramBlock = VRAM_CheckVramSlot(&x, &y, 16, 128, 3, -1);

    if (FONT_vramBlock != NULL)
    {
        timAddr = (unsigned long*)LOAD_ReadFile("\\kain2\\game\\font.tim", 5);

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

    timAddr = (unsigned long*)LOAD_ReadFile("\\kain2\\game\\font.tim", 5);

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

unsigned char fontTransTable[128] = { 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24,
									  0x24, 0x24, 0x24, 0x24, 0x24, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x24,
									  0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x25, 0x28, 0x24,
									  0x24, 0x29, 0x24, 0x33, 0x2B, 0x2C, 0x2F, 0x2D, 0x32, 0x27, 0x24, 0x26,
									  0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x2A, 0x24,
									  0x55, 0x2E, 0x54, 0x30, 0x24, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A,
									  0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46,
									  0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x24, 0x4F, 0x52, 0x24,
									  0x24, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A,
									  0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
								      0x17, 0x18, 0x19, 0x50, 0x53, 0x51, 0x31, 0x24 };

long FONT_Get2DImageIndex(unsigned char c)
{
    return fontTransTable[c];
}

INCLUDE_ASM("asm/nonmatchings/Game/FONT", drawChar2DPoly);

// Mediocre match, the code should work just the same but needs refining (90.76%)
#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/Game/FONT", FONT_DrawChar2D);
#else
void FONT_DrawChar2D(unsigned char c, long x, long y)
{
    long w;
    long h;
    long w1;
    long w2;
    long w3;
    long xoff;
    long yoff;
    int i1;
    int i2;
    int i3;
    
    if (gameTrackerX.primPool->nextPrim <= (gameTrackerX.primPool->lastPrim - 12))
    {
        i1 = FONT_Get2DImageIndex(c);
        
        i2 = (signed char)charMap[i1][1];
        i3 = (signed char)charMap[i1][2];
        i1 = (signed char)charMap[i1][0];
        
        if (i1 >= 0)
        {
            w1 = abs((signed char)fontPos[i1].w);
        }
        else
        {
            w1 = 8;
        }
        
        if (i2 >= 0)
        {
            w2 = abs((signed char)fontPos[i2].w);
        }
        else
        {
            w2 = w1;
        }
        
        if (i3 >= 0)
        {
            w3 = abs((signed char)fontPos[i3].w);
        }
        else
        {
            w3 = w1;
        }
        
        if (w2 < w1)
        {
            if (w3 < w1)
            {
                w = w1;
            }
            else
            {
                w = w3;
            }
        }
        else if (w3 < w2)
        {
            w = w2;
        }
        else
        {
            w = w3;
        }
        
        if (i1 >= 0)
        {
            h = abs((signed char)fontPos[i1].h);
        }
        else
        {
            h = 12;
        }
        
        if (i1 >= 0)
        {
            xoff = x + ((w - w1) / 2);
            yoff = y;
            
            drawChar2DPoly(i1, xoff, yoff);
        }
        
        xoff = x + ((w - w2) / 2);
        yoff = y;
        
        if (c == 'A')
        {
            yoff = y + 2;
        }
        
        if (c == '\'')
        {
            y += 3;
        }
        
        if (i2 >= 0)
        {
            drawChar2DPoly(i2, xoff, yoff);
        }
        
        if (i3 >= 0)
        {
            xoff = x + ((w - w3) / 2);
            yoff = y + 1;
            
            yoff -= h;
            
            drawChar2DPoly(i3, xoff, yoff);
        }
    }
}
#endif

// Weak match, code should be equivalent though (87.47%) 
#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/Game/FONT", FONT_CharSpacing);
#else
long FONT_CharSpacing(char c, long fontXSize)
{
    long index;
    long w;
    long w1;
    long w2;
    long w3;
    char i1;
    char i2;
    char i3;
    
    if (c != ' ')
    {
        index = FONT_Get2DImageIndex(c);
        
        if (index != -1)
        {
            int holdw;
            int holdw2;
        
            i1 = (signed char)charMap[index][0];
            i2 = (signed char)charMap[index][1];
                
            w2 = i1;
                
            i3 = (signed char)charMap[index][2];
                
            holdw = abs((signed char)fontPos[(signed char)w2].w);
                
            if ((signed char)fontPos[(signed char)w2].x < 0)
            {
                holdw = (signed char)fontPos[(signed char)w2].h;
            }
            
            w1 = 8;
                
            if ((signed char)i1 >= 0)
            {
                w1 = holdw;
            }
                
            holdw2 = abs((signed char)fontPos[(signed char)i2].w);
            
            w2 = w1;
                
            if ((signed char)i2 >= 0)
            {
                w2 = holdw2;
            }
                
            holdw2 = abs((signed char)fontPos[(signed char)i3].w);
            
            w3 = w1;
                
            if ((signed char)i3 >= 0)
            {
                w3 = holdw2;
            }
                
            if (w2 < w1)
            {
                if (w3 < w1)
                {
                    w = w1;
                }
                else
                {
                    w = w3;
                }
            }
            else if (w3 < w2)
            {
                w = w2;
            }
            else
            {
                w = w3;
            }
                
            return w + 1;
        }
    }
    
    return fontXSize;
}
#endif

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

// Close match, only a few instructions reordered at the start (96.44%)
#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/Game/FONT", FONT_VaReallyPrint);
#else
void FONT_VaReallyPrint(char *fmt, void *ap)
{
    char *p;
    long *xpos;
    long *ypos;
    short tabWidth;  // not from decls.h
    long spaceWidth; // not from decls.h
    long fontXSize;  // not from decls.h

    (void*)ap;

    fontXSize = 8 * 65536;
    
    spaceWidth = 8;
    tabWidth   = 32;
    
    xpos = &fontTracker.font_xpos;
    ypos = &fontTracker.font_ypos;

    for (p = fmt; *p != '\0'; p++)
    {
        if (*p == '\n')
        {
            *xpos  = 10;
            *ypos += 12;
        } 
        else if (*p == '@') 
        {
            unsigned char w;
            unsigned char h;
            
            h = p[1] - 64;
            w = p[2] - 64;
            
            FONT_AddCharToBuffer('@', *xpos, *ypos);
            FONT_AddCharToBuffer('@', w,     h);
            
            p += 2;
        } 
        else if (*p == '$') 
        {
            *xpos = 10;
            *ypos = 16;
        } 
        else if (*p == '\r') 
        {
            *xpos = 10;
        }
        else if (*p == '\t')
        {
            *xpos += tabWidth;
        } 
        else if ((*p == ' ') || (*p == '_')) 
        {
            *xpos += spaceWidth;
        } 
        else 
        {
            FONT_AddCharToBuffer(*p, *xpos, *ypos);
            
            *xpos += FONT_CharSpacing(*p, fontXSize >> 16);
        }
    }
}
#endif

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
