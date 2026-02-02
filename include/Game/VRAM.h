#ifndef _VRAM_H_
#define _VRAM_H_

#include "common.h"

void VRAM_LoadReturn(void *dataPtr, void *data1, void *data2);
void VRAM_ClearVramBlock(BlockVramEntry *block);
BlockVramEntry *VRAM_CheckVramSlot(short *x, short *y, short w, short h, short type, short startY);
void VRAM_RearrangeVramsLayer(long whichLayer);
void VRAM_InitMorphPalettes();
void AdjustVramCoordsObject(int oldx, int oldy, int newx, int newy, Object *object);
void VRAM_TransferBufferToVram(void *dataPtr, long dataSize, short status, void *data1, void *data2);
long VRAM_GetObjectVramSpace(VramSize *vramSize, ObjectTracker *objectTracker);
void MORPH_ChangeAreaPalettes(long time);
int VRAM_InsertFreeVram(short x, short y, short w, short h, short flags);
int VRAM_DeleteFreeVram(short x, short y, short w, short h);
void VRAM_DeleteFreeBlock(BlockVramEntry *block);
void VRAM_UpdateMorphPalettes();
void VRAM_DisableTerrainArea();
void VRAM_InitVramBlockCache();
void VRAM_EnableTerrainArea();

extern long VRAM_NeedToUpdateMorph;

#endif
