#ifndef _FONT_H_
#define _FONT_H_

#include "common.h"

int FONT_GetStringWidth(char *str);
// void FONT_Print(char *fmt);
void FONT_Print(const char *fmt, ...);
void FONT_Print2(char *fmt);
void FONT_SetCursor(short x, short y);
void FONT_FontPrintCentered(char *text, long y);
void FONT_SetColorIndex(int color);
void FONT_SetColorIndexCol(int color, int r, int g, int b);
void FONT_Init();
void FONT_DrawChar2D(unsigned char c, long x, long y);
void FONT_ReloadFont();
long FONT_CharSpacing(char c, long fontXSize);

FontTracker fontTracker;

unsigned short SpecialFogClut;

void FONT_Flush();

#endif
