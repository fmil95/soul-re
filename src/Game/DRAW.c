#include "common.h"
#include "Game/DRAW.h"
#include "Game/GAMELOOP.h"
#include "Game/FONT.h"
#include "Game/HASM.h"
#include "Game/MATH3D.h"
#include "Game/VRAM.h"
#include "Game/PIPE3D.h"
#include "Game/CAMERA.h"
#include "Game/PSX/MAIN.h"
#include "Game/LOCAL/LOCALSTR.h"
#include "Game//MENU/MENU.h"

SVECTOR shadow_vertices[11];

void fDRAW_SPLIT_INTPL_XYZ(SVector *newVertex, SVector *pvb, SVector *pvc)
{
    newVertex->x = pvb->x + ((pvb->z * (pvc->x - pvb->x)) / (pvb->z - pvc->z));
    newVertex->y = pvb->y + ((pvb->z * (pvc->y - pvb->y)) / (pvb->z - pvc->z));
    newVertex->z = 0;
}

void DRAW_InitShadow()
{
    int n;
    int inc;
    int deg;

    deg = 0;

    inc = 1677721;

    for (n = 0; n < 10; n++)
    {
        shadow_vertices[n].vx = rcos(deg / 4096) / 32;
        shadow_vertices[n].vy = rsin(deg / 4096) / 32;
        shadow_vertices[n].vz = 0;

        deg += inc;
    }

    shadow_vertices[10].vx = shadow_vertices[0].vx;
    shadow_vertices[10].vy = shadow_vertices[0].vy;
    shadow_vertices[10].vz = shadow_vertices[0].vz;
}

INCLUDE_ASM("asm/nonmatchings/Game/DRAW", DRAW_DrawShadow);

INCLUDE_ASM("asm/nonmatchings/Game/DRAW", DRAW_FlatQuad);

INCLUDE_ASM("asm/nonmatchings/Game/DRAW", DRAW_TranslucentQuad);

INCLUDE_ASM("asm/nonmatchings/Game/DRAW", DRAW_DrawButton);

INCLUDE_ASM("asm/nonmatchings/Game/DRAW", DRAW_LoadButton);

void DRAW_FreeButton(ButtonTexture *button)
{
    VRAM_ClearVramBlock(button->vramBlock);
}

INCLUDE_ASM("asm/nonmatchings/Game/DRAW", DRAW_RingLine);

INCLUDE_ASM("asm/nonmatchings/Game/DRAW", DRAW_RingPoint);

INCLUDE_ASM("asm/nonmatchings/Game/DRAW", DRAW_DrawRingPoints);

INCLUDE_ASM("asm/nonmatchings/Game/DRAW", DRAW_GlowQuad);

INCLUDE_ASM("asm/nonmatchings/Game/DRAW", DRAW_CreateAGlowingCircle);

unsigned long *DRAW_DrawGlowPoints2(Instance *instance, long seg1, long seg2, PrimPool *primPool, unsigned long **ot, long color, long height)
{

    Position glowPoints1;
    Position glowPoints2;
    Vector f1;
    Vector f2;
    Vector f3;

    long otz;
    long otz2;
    long angle;
    long length;
    long z;

    otz = PIPE3D_Segment2ScreenPt(instance, theCamera.core.wcTransform, seg1, &glowPoints1) + 0x14;
    otz2 = PIPE3D_Segment2ScreenPt(instance, theCamera.core.wcTransform, seg2, &glowPoints2) + 0x14;

    if (otz < otz2)
    {
        otz = otz2;
    }

    if (otz <= 0) { return primPool->nextPrim; }

    f1.x = glowPoints1.x;
    f1.y = glowPoints1.y;
    f2.x = glowPoints2.x;
    f2.y = glowPoints2.y;

    f3.x = glowPoints2.x - glowPoints1.x;
    f3.y = glowPoints2.y - glowPoints1.y;

    length = MATH3D_veclen2(f3.x, f3.y) + height;
    angle = ratan2(f3.y, -f3.x) & 0xFFF;

    f3.x = f2.x + f1.x;
    f3.y = f2.y + f1.y;
    f3.z = 0;

    f3.x = (f3.x * 2) / 4;
    f3.y = (f3.y * 2) / 4;
    z = ((otz + otz2) / 2) * 4;
    otz += 0xA;

    if (otz < 0xC00)
    {
        DRAW_CreateAGlowingCircle(&f3, z, primPool, ot, otz, color, length, height, angle);
    }

    return primPool->nextPrim;
}

INCLUDE_ASM("asm/nonmatchings/Game/DRAW", DRAW_DrawGlowPoint);

INCLUDE_ASM("asm/nonmatchings/Game/DRAW", DRAW_DisplayTFace_zclipped_C);

INCLUDE_ASM("asm/nonmatchings/Game/DRAW", DRAW_Zclip_subdiv);

void DRAW_LoadingMessage()
{
    unsigned long **drawot;

    while (CheckVolatile(gameTrackerX.drawTimerReturn) != 0) {}
    while (CheckVolatile(gameTrackerX.reqDisp) != 0) {}

    DrawSyncCallback(NULL);
    VSyncCallback(NULL);

    drawot = gameTrackerX.drawOT;

    PutDrawEnv(&draw[gameTrackerX.drawPage ^ 1]);
    FONT_FontPrintCentered(localstr_get(LOCALSTR_Hint52), 150);
    DisplayHintBox(FONT_GetStringWidth(localstr_get(LOCALSTR_Hint52)), 150);
    FONT_Flush();

    DrawOTag(drawot[3071]);
    DrawSync(0);
    ClearOTagR((u_long *)drawot, 3072);
    PutDrawEnv(&draw[gameTrackerX.drawPage]);
    VSyncCallback(VblTick);
    DrawSyncCallback(DrawCallback);
}
