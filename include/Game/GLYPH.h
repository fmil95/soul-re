#ifndef _GLYPH_H_
#define _GLYPH_H_

#include "common.h"

void _GlyphSelectProcess(Instance *instance, int data1, int data2);
void Glyph_Broadcast(Instance *sender, int glyphnum);
void _GlyphOffProcess(Instance *instance, int data1, int data2);
void ShrinkGlyphMenu(Instance *instance);
void Glyph_StartSpell(Instance *instance, int glyphnum);
void Glyph_DoSpell(Instance *instance, int glyphnum);
void HUD_Init();
void GlyphDrawMenu(Instance *instance);
void Glyph_DoFX(Instance *instance);
void MANNA_Pickup();
void HEALTHU_Pickup(Instance *instance);
void HUD_Setup_Chit_Count(int chits);
void HUD_Draw();
void GlyphProcess(Instance *instance, GameTracker *gameTracker);
int GlyphIsGlyphOpen(Instance *instance);
void GlyphTrigger();

extern int hud_warp_arrow_flash;

#endif
