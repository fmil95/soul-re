#include "Game/VRAM.h"
#include "Game/MEMPACK.h"
#include "Game/GAMELOOP.h"
#include "Game/FONT.h"
#include "Game/HASM.h"

BlockVramEntry *usedVramBlocks;

BlockVramEntry *openVramBlocks;

BlockVramEntry vramBlockList[90];

long numOfBlocksUsed;

/* Unused */ long VRAM_NeedToUpdateMorph = 0;

static inline long VRAM_GetDispPage()
{
    return (gameTrackerX.gameData.asmData.dispPage ^ 1) << 8;
}

void VRAM_PrintVramBlock()
{
}

void VRAM_PrintInfo()
{
    BlockVramEntry *vblock;

    vblock = usedVramBlocks;

    while (vblock != NULL)
    {
        vblock = vblock->next;
    }

    vblock = openVramBlocks;

    while (vblock != NULL)
    {
        vblock = vblock->next;
    }
}

void VRAM_InitVramBlockCache()
{
    int i;

    openVramBlocks = NULL;
    usedVramBlocks = NULL;

    numOfBlocksUsed = 0;

    for (i = 89; i >= 0; i--)
    {
        vramBlockList[i].flags = 0;
    }

    VRAM_InsertFreeVram(512, 240 + 16, 512, 240 + 16, 1);

    VRAM_InitMorphPalettes();
}

void VRAM_EnableTerrainArea()
{
    VRAM_InsertFreeVram(512, 0, 512, 240 + 16, 0);
}

void VRAM_DisableTerrainArea()
{
    VRAM_DeleteFreeVram(512, 0, 512, 240 + 16);
}

int VRAM_ConcatanateMemory(BlockVramEntry *curBlock)
{
    BlockVramEntry *nextBlock;

    while (curBlock != NULL)
    {
        nextBlock = curBlock->next;

        while (nextBlock != NULL)
        {
            if ((curBlock->x == nextBlock->x) && (curBlock->w == nextBlock->w))
            {
                if ((curBlock->y >> 8) == (nextBlock->y >> 8))
                {
                    if ((curBlock->y + curBlock->h) == nextBlock->y)
                    {
                        curBlock->h += nextBlock->h;

                        VRAM_DeleteFreeBlock(nextBlock);

                        nextBlock->flags = 0;
                        return 1;
                    }

                    if ((nextBlock->y + nextBlock->h) == curBlock->y)
                    {
                        nextBlock->h += curBlock->h;

                        VRAM_DeleteFreeBlock(curBlock);

                        curBlock->flags = 0;
                        return 1;
                    }
                }
            }

            if ((curBlock->y == nextBlock->y) && (curBlock->h == nextBlock->h))
            {
                if (((curBlock->x + curBlock->w) == nextBlock->x) && ((!(curBlock->x & 0x3F)) || ((curBlock->w + nextBlock->w) < 65)))
                {
                    curBlock->w += nextBlock->w;

                    VRAM_DeleteFreeBlock(nextBlock);

                    nextBlock->flags = 0;
                    return 1;
                }

                if (((nextBlock->x + nextBlock->w) == curBlock->x) && ((!(nextBlock->x & 0x3F)) || ((curBlock->w + nextBlock->w) < 65)))
                {
                    nextBlock->w += curBlock->w;

                    VRAM_DeleteFreeBlock(curBlock);

                    curBlock->flags = 0;
                    return 1;
                }
            }

            nextBlock = nextBlock->next;
        }

        curBlock = curBlock->next;
    }

    return 0;
}

void VRAM_GarbageCollect()
{
    while (VRAM_ConcatanateMemory(openVramBlocks) == 1);
}

int VRAM_InsertFreeBlock(BlockVramEntry *block)
{
    BlockVramEntry *next;
    BlockVramEntry *prev;

    prev = NULL;

    if (block == NULL)
    {
        return 0;
    }

    next = openVramBlocks;

    while (next != NULL)
    {
        if (next->area >= block->area)
        {
            break;
        }

        prev = next;
        next = prev->next;
    }

    if (prev == NULL)
    {
        block->next = openVramBlocks;

        openVramBlocks = block;
    }
    else
    {
        block->next = next;
        prev->next = block;
    }

    VRAM_GarbageCollect();

    return 1;
}

void VRAM_DeleteFreeBlock(BlockVramEntry *block)
{
    BlockVramEntry *next;
    BlockVramEntry *prev;

    next = openVramBlocks;
    prev = NULL;

    if (block != NULL)
    {
        while (block != next)
        {
            if (next != NULL)
            {
                prev = next;
                next = prev->next;

                if (block != next)
                {
                    continue;
                }
            }

            if (block != next)
            {
                return;
            }
        }

        if (prev == NULL)
        {
            openVramBlocks = block->next;
        }
        else
        {
            prev->next = block->next;
        }
    }
}

void VRAM_InsertUsedBlock(BlockVramEntry *block)
{
    if (block != NULL)
    {
        if (usedVramBlocks == NULL)
        {
            usedVramBlocks = block;
        }
        else
        {
            block->next = usedVramBlocks;

            usedVramBlocks = block;
        }
    }
}

void VRAM_DeleteUsedBlock(BlockVramEntry *block)
{
    BlockVramEntry *next;
    BlockVramEntry *prev;

    next = usedVramBlocks;
    prev = NULL;

    while (block != next)
    {
        if (next != NULL)
        {
            prev = next;
            next = prev->next;

            if (block != next)
            {
                continue;
            }
        }

        if (block != next)
        {
            return;
        }
    }

    if (prev == NULL)
    {
        usedVramBlocks = block->next;
    }
    else
    {
        prev->next = block->next;
    }
}

BlockVramEntry *VRAM_GetOpenBlock()
{
    int i;

    for (i = 0; i < 90; i++)
    {
        if (vramBlockList[i].flags == 0)
        {
            return &vramBlockList[i];
        }
    }

    return NULL;
}

int VRAM_DeleteFreeVram(short x, short y, short w, short h)
{
    BlockVramEntry *prev;
    BlockVramEntry *vblock;
    BlockVramEntry *nextVBlock;
    BlockVramEntry *blockLists[2] = {
        openVramBlocks, usedVramBlocks
    };
    int i;
    int delCount;

    delCount = 0;

    for (i = 0; i < 2; i++)
    {
        vblock = blockLists[i];

        prev = NULL;

        while (vblock != NULL)
        {
            nextVBlock = vblock->next;

            if ((vblock->x >= x) && ((x + w) >= (vblock->x + vblock->w)) && (vblock->y >= y)
            && ((y + h) >= (vblock->y + vblock->h)))
            {
                vblock->flags = 0;

                if (prev == NULL)
                {
                    if (i == 0)
                    {
                        openVramBlocks = nextVBlock;
                    }
                    else
                    {
                        usedVramBlocks = nextVBlock;
                    }
                }
                else
                {
                    prev->next = nextVBlock;
                }

                delCount++;
            }
            else
            {
                prev = vblock;
            }

            vblock = nextVBlock;
        }
    }

    return delCount;
}

int VRAM_InsertFreeVram(short x, short y, short w, short h, short flags)
{
    BlockVramEntry *useBlock;

    if (((x & 0x3F)) && (w > (64 - ((x & 0x3F)))))
    {
        useBlock = VRAM_GetOpenBlock();

        useBlock->next = NULL;
        useBlock->flags = flags;
        useBlock->time = 0;
        useBlock->ID = 0;

        useBlock->x = x;
        useBlock->y = y;
        useBlock->w = 64;

        useBlock->w -= ((x & 0x3F));

        useBlock->h = h;

        useBlock->area = useBlock->w * h;

        VRAM_InsertFreeBlock(useBlock);

        useBlock = VRAM_GetOpenBlock();

        useBlock->next = NULL;
        useBlock->flags = flags;
        useBlock->time = 0;
        useBlock->ID = 0;

        useBlock->x = x + 64;

        useBlock->x -= ((x & 0x3F));

        useBlock->y = y;

        useBlock->w = w - 64;

        useBlock->w += ((x & 0x3F));

        useBlock->h = h;

        useBlock->area = useBlock->w * useBlock->h;

        VRAM_InsertFreeBlock(useBlock);
    }
    else
    {
        useBlock = VRAM_GetOpenBlock();

        useBlock->next = NULL;
        useBlock->flags = flags;
        useBlock->time = 0;
        useBlock->ID = 0;

        useBlock->x = x;
        useBlock->y = y;
        useBlock->w = w;
        useBlock->h = h;

        useBlock->area = w * h;

        VRAM_InsertFreeBlock(useBlock);
    }

    return 1;
}

BlockVramEntry *VRAM_CheckVramSlot(short *x, short *y, short w, short h, short type, short startY)
{
    struct _BlockVramEntry *vblock; // $s1
    short hldx; // $s4
    short hldy; // $s5
    short hldw; // $s0
    short hldh; // $fp
    long fits; // $t1
    long offset; // $s0
    struct _BlockVramEntry *vblockright; // $s3
    long offsetright; // $s2
    // int xval; // unused

    vblock = openVramBlocks;
    fits = 0;
    vblockright = NULL;
    offsetright = 0;

    while (vblock != NULL)
    {
        if ((w <= vblock->w) && (((h > vblock->h) == 0)) && ((startY == -1) || ((((vblock->y < startY) == 0)) && (vblock->y < (startY + 0x100)))))
        {
            int newx; // $v0 line 36
            newx = 0x40 - (vblock->x & 0x3F);
            if ((vblock->x & 0x3F) == 0)
            {
                fits = 0;
                break;
            }
            else
            {
                if (newx >= w)
                {
                    if ((vblock->x & 0xF) == 0)
                    {
                        fits = 0;
                        break;
                    }
                    else
                    {
                        offset = 0x10 - (vblock->x & 0xF);
                        if (((0x40 - ((vblock->x + offset) & 0x3F)) >= w) && ((vblock->w - offset) >= w))
                        {
                            VRAM_InsertFreeVram(vblock->x, vblock->y, offset, vblock->h, vblock->flags);
                            fits = 0;
                            vblock->x += offset;
                            vblock->w -= offset;
                            break;
                        }
                    }
                }
                else
                {
                    offset = newx;
                    if (((0x40 - ((vblock->x + offset) & 0x3F)) >= w) && ((vblock->w - offset) >= w))
                    {
                        fits = 1;
                        vblockright = vblock;
                        offsetright = offset;
                    }
                }
            }
        }

        vblock = vblock->next;
    }

    if (vblock == NULL && (vblockright != NULL) && (fits == 1))
    {
        vblock = vblockright;
        VRAM_InsertFreeVram(vblock->x, vblock->y, offsetright, vblock->h, vblock->flags);
        fits = 0;
        vblock->x += offsetright;
        vblock->w -= offsetright;
    }

    if (vblock != NULL && fits == 0)
    {
        hldx = vblock->x;
        hldy = vblock->y;
        hldw = vblock->w;
        hldh = vblock->h;
        VRAM_DeleteFreeBlock(vblock);
        vblock->next = 0;
        vblock->flags = 1;
        vblock->type = type;
        vblock->w = w;
        vblock->h = h;
        VRAM_InsertUsedBlock(vblock);
        *x = vblock->x;
        *y = vblock->y;
        if (hldw == w)
        {
            if (hldh != h)
            {
                VRAM_InsertFreeVram(hldx, hldy + h, w, hldh - h, 1);
            }
        }
        else
        {
            if (hldh == h)
            {
                VRAM_InsertFreeVram(hldx + w, hldy, hldw - w, hldh, 1);
            }
            else
            {
                if (abs(((hldw - w) * h) - (hldw * (hldh - h))) > abs(((hldw - w) * hldh) - (w * (hldh - h))))
                {
                    VRAM_InsertFreeVram(hldx + w, hldy, (hldw - w), h, 1);
                    VRAM_InsertFreeVram(hldx, hldy + h, hldw, hldh - h, 1);
                }
                else
                {
                    VRAM_InsertFreeVram(hldx + w, hldy, (hldw - w), hldh, 1);
                    VRAM_InsertFreeVram(hldx, hldy + h, w, hldh - h, 1);
                }
            }
        }
    }

    return vblock;
}

void VRAM_ClearVramBlock(BlockVramEntry *block)
{
    if (block != NULL)
    {
        VRAM_DeleteUsedBlock(block);

        VRAM_InsertFreeBlock(block);
    }
}

INCLUDE_ASM("asm/nonmatchings/Game/VRAM", AdjustVramCoordsObject);

BlockVramEntry *VRAM_InsertionSort(BlockVramEntry *rootNode, BlockVramEntry *newBlock)
{
    BlockVramEntry *prev;
    BlockVramEntry *next;

    prev = NULL;
    next = rootNode;

    while (next != NULL)
    {
        if (newBlock->area >= next->area)
        {
            break;
        }

        prev = next;
        next = next->next;
    }

    if (prev == NULL)
    {
        newBlock->next = rootNode;

        rootNode = newBlock;
    }
    else
    {
        newBlock->next = next;

        prev->next = newBlock;
    }

    return rootNode;
}

void VRAM_RearrangeVramsLayer(long whichLayer)
{
    BlockVramEntry *vblock;
    BlockVramEntry *sortedBlocks;
    BlockVramEntry *curBlock;
    BlockVramEntry *savedVramBlocksPTR[48];
    BlockVramEntry savedVramBlocks[48];
    RECT vramLoc;
    long numBlocks;
    long d;
    short x;
    short y;
    short dispYPos;
    int startY;

    numBlocks = 0;

    if ((gameTrackerX.gameFlags & 0x8000000))
    {
        GAMELOOP_Set_Pause_Redraw();
    }

    for (vblock = usedVramBlocks; vblock != NULL; vblock = vblock->next)
    {
        if ((whichLayer == 0) && (vblock->y < 256))
        {
            savedVramBlocksPTR[numBlocks] = vblock;
            savedVramBlocks[numBlocks] = *vblock;

            numBlocks++;
        }

        if ((whichLayer != 0) && (vblock->y >= 256))
        {
            savedVramBlocksPTR[numBlocks] = vblock;
            savedVramBlocks[numBlocks] = *vblock;

            numBlocks++;
        }
    }

    if (numBlocks > 0)
    {
        if (whichLayer == 0)
        {
            vramLoc.x = 512;
            vramLoc.y = 0;
            vramLoc.w = 512;
            vramLoc.h = 256;

            startY = vramLoc.y;
        }
        else
        {
            vramLoc.x = 512;
            vramLoc.y = 256;
            vramLoc.w = 512;
            vramLoc.h = 256;

            startY = vramLoc.y;
        }

        do
        {
            while (CheckVolatile(gameTrackerX.drawTimerReturn) != 0);
        } while (CheckVolatile(gameTrackerX.reqDisp) != 0);

        sortedBlocks = NULL;

        dispYPos = VRAM_GetDispPage();

        MoveImage(&vramLoc, 0, dispYPos);

        DrawSync(0);

        for (d = 0; d < numBlocks; d++)
        {
            VRAM_ClearVramBlock(savedVramBlocksPTR[d]);

            savedVramBlocks[d].next = NULL;
            savedVramBlocks[d].area = savedVramBlocks[d].w * savedVramBlocks[d].h;

            sortedBlocks = VRAM_InsertionSort(sortedBlocks, &savedVramBlocks[d]);
        }

        for (curBlock = sortedBlocks; curBlock != NULL; curBlock = curBlock->next)
        {
            vramLoc.x = curBlock->x;
            vramLoc.y = curBlock->y;
            vramLoc.w = curBlock->w;
            vramLoc.h = curBlock->h;

            vramLoc.x -= 512;

            if (whichLayer == 0)
            {
                vramLoc.y += dispYPos;
            }
            else
            {
                vramLoc.y = (vramLoc.y - 256) + dispYPos;
            }

            x = curBlock->x;
            y = curBlock->y;

            vblock = VRAM_CheckVramSlot(&x, &y, vramLoc.w, vramLoc.h, curBlock->type, (short)startY);

            if (vblock == NULL)
            {
                VRAM_PrintInfo();
                VRAM_PrintVramBlock(curBlock);
            }

            MoveImage(&vramLoc, x, y);

            DrawSync(0);

            switch (curBlock->type)
            {
            case 1:
                break;
            case 2:
                AdjustVramCoordsObject(curBlock->x, curBlock->y, x, y, ((curBlock->udata).streamObject)->object);

                curBlock->udata.streamObject->vramBlock = vblock;

                vblock->udata = curBlock->udata;
                break;
            case 3:
                fontTracker.font_vramX = x;
                fontTracker.font_vramY = y;

                fontTracker.font_tpage = getTPage(0, 0, x, y);

                fontTracker.font_vramU = (x & 63) << 2;
                fontTracker.font_vramV = y & 255;

                fontTracker.font_clut = getClut(x, y + 126);

                SpecialFogClut = getClut(x, y + 127);
                break;
            }
        }
    }

    DrawSync(0);
}

void VRAM_TransferBufferToVram(void *dataPtr, long dataSize, short status, void *data1, void *data2)
{
    VramBuffer *vramControl;
    RECT rect;
    long *nextOTag;
    long *drawTimerReturn;
    short temp, temp2, temp3; // not from decls.h

    nextOTag = (long *)BreakDraw();

    DrawSync(0);

    drawTimerReturn = gameTrackerX.drawTimerReturn;

    gameTrackerX.drawTimerReturn = NULL;

    vramControl = (VramBuffer *)data1;

    if (vramControl != NULL)
    {
        if (!(vramControl->flags & 0x1))
        {
            vramControl->flags |= 0x1;

            dataPtr = (char *)dataPtr + 36; // sizeof?
            dataSize -= 36;
        }

        if (vramControl->lengthOfLeftOverData != 0)
        {
            temp = (vramControl->w * 2) - vramControl->lengthOfLeftOverData;

            if (dataSize < temp)
            {
                memcpy((char *)vramControl->lineOverFlow + vramControl->lengthOfLeftOverData, dataPtr, dataSize);

                vramControl->lengthOfLeftOverData += (short)dataSize;

                dataSize = 0;
            }
            else
            {
                memcpy((char *)vramControl->lineOverFlow + vramControl->lengthOfLeftOverData, dataPtr, temp);

                dataPtr = (char *)dataPtr + temp;
                dataSize -= temp;

                vramControl->lengthOfLeftOverData += temp;

                rect.x = vramControl->x;
                rect.y = vramControl->y + vramControl->yOffset;
                rect.w = vramControl->w;
                rect.h = 1;

                vramControl->yOffset++;

                LoadImage(&rect, (u_long *)vramControl->lineOverFlow);

                vramControl->lengthOfLeftOverData = 0;
            }
        }

        if (dataSize > 0)
        {
            temp2 = (short)(dataSize / (vramControl->w * 2));
            temp3 = vramControl->w * (temp2 << 1);

            rect.x = vramControl->x;
            rect.y = vramControl->y + vramControl->yOffset;
            rect.w = vramControl->w;
            rect.h = temp2;

            LoadImage(&rect, (u_long *)dataPtr);

            vramControl->yOffset += temp2;

            dataSize -= temp3;
            dataPtr = (char *)dataPtr + temp3;

            if (dataSize > 0)
            {
                memcpy((char *)vramControl->lineOverFlow + vramControl->lengthOfLeftOverData, dataPtr, dataSize);

                vramControl->lengthOfLeftOverData += (short)dataSize;
            }
        }

        if ((status == 1) && (data2 != NULL) && (((ObjectTracker *)data2)->objectStatus == 4))
        {
            ((ObjectTracker *)data2)->objectStatus = 2;
        }
    }

    DrawSync(0);

    gameTrackerX.drawTimerReturn = drawTimerReturn;

    if ((nextOTag != 0) && ((intptr_t)nextOTag != -1))
    {
        DrawOTag((u_long *)nextOTag);
    }
}

void VRAM_LoadReturn(void *dataPtr, void *data1, void *data2)
{
    (void)dataPtr;
    (void)data2;
    MEMPACK_Free((char *)data1);
}

long VRAM_GetObjectVramSpace(VramSize *vramSize, ObjectTracker *objectTracker)
{
    RECT rect;
    long result;
    BlockVramEntry *lastVramBlockUsed;

    result = 1;

    rect.x = vramSize->x + 512;
    rect.y = vramSize->y;
    rect.w = vramSize->w;
    rect.h = vramSize->h;

    lastVramBlockUsed = VRAM_CheckVramSlot(&rect.x, &rect.y, rect.w, rect.h, 2, 256);

    if (lastVramBlockUsed == NULL)
    {
        VRAM_RearrangeVramsLayer(result);

        lastVramBlockUsed = VRAM_CheckVramSlot(&rect.x, &rect.y, rect.w, rect.h, 2, 256);

        if (lastVramBlockUsed == NULL)
        {
            result = 0;

            VRAM_PrintInfo();
        }
    }

    objectTracker->vramBlock = lastVramBlockUsed;

    if (lastVramBlockUsed != NULL)
    {
        lastVramBlockUsed->udata.streamObject = objectTracker;
    }

    return result;
}

void VRAM_InitMorphPalettes()
{
}

void VRAM_UpdateMorphPalettes()
{
}

void MORPH_ChangeAreaPalettes(long time)
{
    (void)time;
}
