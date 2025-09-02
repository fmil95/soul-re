#ifndef _DRAW_H_
#define _DRAW_H_

#include "common.h"

void DRAW_CreateAGlowingCircle(Vector *f1, long z, PrimPool *primPool, unsigned long **ot, long otz, long color, long w, long h, long angle);
void DRAW_TranslucentQuad(short x0, short y0, short x1, short y1, short x2, short y2, short x3, short y3, short r, short g, short b, short abr, PrimPool *primPool, unsigned long **ot);
unsigned long *DRAW_DrawRingPoints(Model *model, VertexPool *vertexPool, MATRIX *pcTransform, PrimPool *primPool, unsigned long **ot, long color, int ring_type);
void DRAW_InitShadow();
void DRAW_FlatQuad(CVECTOR *color, short x0, short y0, short x1, short y1, short x2, short y2, short x3, short y3, PrimPool *primPool, unsigned long **ot);
void DRAW_LoadingMessage();
void DRAW_LoadButton(long *addr, ButtonTexture *button);
void DRAW_FreeButton(ButtonTexture *button);
void DRAW_DrawButton(ButtonTexture *button, short x, short y, unsigned long **ot);
unsigned long *DRAW_DrawShadow(PrimPool *primPool, Model *model, unsigned long **ot, long fadeValue);
void DRAW_GlowQuad(PrimPool *primPool, unsigned long **ot, long otz, long color, Vector *v0, Vector *v1, Vector *v2, Vector *v3);

#endif
