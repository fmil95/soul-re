#include "Game/MENU/MENUFACE.h"
#include "Game/LOAD3D.h"
#include "Game/MEMPACK.h"
#include "Game/DRAW.h"
#include "Game/GAMELOOP.h"

static ButtonTexture *FaceButtons;

STATIC int hack_initialized;

STATIC menuface_t MenuFaces[8];

char *NextTimAddr(char *addr, int w, int h, bdepth bpp)
{
    long addtl;

    if (bpp == TIM_4BIT)
    {
        addtl = ((w * h) >> 1) + 44;
    }
    else if (bpp == TIM_8BIT)
    {
        addtl = (w * h) + 524;
    }
    else
    {
        addtl = (w * h) << 1;
    }

    addtl += 20;

    return &addr[addtl];
}

extern char D_800D2088[];
void menuface_initialize()
{
    char *addr;
    char *buttonAddr;
    int i;
    int j;

    if (hack_initialized == 0)
    {
        // buttonAddr = addr = (char*)LOAD_ReadFile("\\kain2\\game\\psx\\frontend\\faces.tim", 11);
        buttonAddr = addr = (char *)LOAD_ReadFile(D_800D2088, 11);

        if (buttonAddr != NULL)
        {
            FaceButtons = (ButtonTexture *)MEMPACK_Malloc(sizeof(ButtonTexture) * 56, 45);

            if (FaceButtons == NULL)
            {
                MEMPACK_Free(addr);
            }
            else
            {
                for (i = 0; i < 8; i++)
                {
                    MenuFaces[i].curFrame = -1;

                    MenuFaces[i].transitionDir = 0;

                    MenuFaces[i].loaded = 0;

                    MenuFaces[i].delay = 0;

                    for (j = 0; j < 7; j++)
                    {
                        DRAW_LoadButton((long *)buttonAddr, &FaceButtons[(i * 7) + j]);

                        buttonAddr = NextTimAddr(buttonAddr, MenuFaces[i].w, MenuFaces[i].h, TIM_4BIT);

                        MenuFaces[i].loaded |= 1 << j;
                    }
                }

                MEMPACK_Free(addr);

                hack_initialized = 1;
            }
        }
    }
}

void menuface_terminate()
{
    int i;
    int j;

    if (hack_initialized != 0)
    {
        for (i = 0; i < 8; i++)
        {
            for (j = 0; j < 7; j++)
            {
                if (((MenuFaces[i].loaded >> j) & 0x1))
                {
                    DRAW_FreeButton(&FaceButtons[(i * 7) + j]);
                }
            }
        }

        MEMPACK_Free((char *)FaceButtons);

        hack_initialized = 0;
    }
}

void MENUFACE_ChangeStateRandomly(int index)
{
    menuface_t *face;
    menuface_t *lastFace;
    menuface_t *temp; // not from decls.h

    (void)index;

    if (hack_initialized != 0)
    {
        lastFace = &MenuFaces[8];

        for (face = &MenuFaces[0], temp = &lastFace[-8]; face < lastFace; face++, temp++)
        {
            if (temp->delay == 0)
            {
                if (temp->transitionDir == 0)
                {
                    int temp2; // not from decls.h

                    temp2 = rand();

                    if (temp2 == ((temp2 / 500) * 500))
                    {
                        if (temp->curFrame == -1)
                        {
                            temp->transitionDir = 1;
                        }
                        else
                        {
                            temp->transitionDir = -1;
                        }
                    }
                }
                else
                {
                    temp->curFrame += temp->transitionDir;

                    if (temp->curFrame == ((temp->frames * 8) - temp->frames - 1) || (temp->curFrame == -1))
                    {
                        temp->transitionDir = 0;

                        temp->delay = 200;
                    }
                }
            }
            else
            {
                temp->delay--;
            }
        }
    }
}

// Matches 100% on decomp.me but differs on this project
#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/Game/MENU/MENUFACE", MENUFACE_RefreshFaces);
#else
void MENUFACE_RefreshFaces()
{
    int i;
    menuface_t *face;

    if (hack_initialized == 0)
    {
        return;
    }

    for (i = 0, face = MenuFaces; i < 8; i++, face++)
    {
        if (face->curFrame >= face->frames)
        {
            DRAW_DrawButton(&FaceButtons[(i * 7) + (face->curFrame / face->frames)], face->x, face->y, &gameTrackerX.dispOT[1]);
        }
    }
}
#endif
