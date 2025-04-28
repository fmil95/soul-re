#ifndef _DRAW_H_
#define _DRAW_H_

#include "common.h"

void DRAW_CreateAGlowingCircle(Vector *f1, long z, PrimPool *primPool, unsigned long **ot, long otz, long color, long w, long h, long angle);
void DRAW_TranslucentQuad(short x0, short y0, short x1, short y1, int x2, int y2, int x3, int y3, int r, int g, int b, int abr, PrimPool *primPool, unsigned long **ot);
unsigned long *DRAW_DrawRingPoints(Model *model, VertexPool *vertexPool, MATRIX *pcTransform, PrimPool *primPool, unsigned long **ot, long color, int ring_type);
void DRAW_InitShadow();
void DRAW_FlatQuad(CVECTOR *color, short x0, short y0, short x1, int y1, int x2, int y2, int x3, int y3, PrimPool *primPool, unsigned long **ot);
void DRAW_LoadingMessage();
void DRAW_LoadButton(long *addr, ButtonTexture *button);
void DRAW_FreeButton(ButtonTexture *button);
void DRAW_DrawButton(ButtonTexture *button, short x, short y, unsigned long **ot);
unsigned long *DRAW_DrawShadow(PrimPool *primPool, Model *model, unsigned long **ot, long fadeValue);

#endif
