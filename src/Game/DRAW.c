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
#include "Game/MENU/MENU.h"
#include "Game/FX.h"

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

void DRAW_RingPoint(PrimPool *primPool, unsigned long **ot, long color, SVector *vel, SVector *acc)
{
    SVector outpoint;
    int temp; // not from decls.h

    (void)primPool;
    (void)ot;

    if ((rand() & 0x3F) >= 24)
    {
        vel->x = (rand() & 0x1F) - 15;
        vel->y = (rand() & 0x1F) - 15;
        vel->z = rand() & 0xF;

        gte_stsv(&outpoint);

        temp = rand() & 0x3;

        outpoint.x--;

        outpoint.x += temp;

        temp = rand() & 0x3;

        outpoint.y--;

        outpoint.y += temp;

        temp = rand() & 0x3;

        outpoint.z--;

        outpoint.z += temp;

        FX_Dot(&outpoint, vel, acc, 0, color, 10, 7, 1);
    }
}

INCLUDE_ASM("asm/nonmatchings/Game/DRAW", DRAW_DrawRingPoints);

INCLUDE_ASM("asm/nonmatchings/Game/DRAW", DRAW_GlowQuad);

void DRAW_CreateAGlowingCircle(Vector *f1, long z, PrimPool *primPool, unsigned long **ot, long otz, long color, long w, long h, long angle)
{
    long hold;
    long x;
    long y;
    long diagScale;
    Vector width;
    Vector height;
    Vector diag1;
    Vector diag2;
    Vector points[9];
    int temp, temp2; // not from decls.h

    f1->z = 0;

    if ((z > 0) && (otz < 3072))
    {
        diagScale = 3072;

        temp = (320 * w) / z;
        temp2 = (320 * h) / z;

        height.x = 0;
        height.y = -temp2;
        height.z = 0;

        x = ((rcos(angle) * height.x) + (rsin(angle) * height.y)) >> 12;
        y = ((-rsin(angle) * height.x) + (rcos(angle) * height.y)) >> 12;

        height.x = (x << 9) / 240;
        height.y = y;

        width.x = -temp;
        width.y = 0;
        width.z = 0;

        x = ((rcos(angle) * width.x) + (rsin(angle) * width.y)) >> 12;
        y = ((-rsin(angle) * width.x) + (rcos(angle) * width.y)) >> 12;

        width.x = (x << 9) / 240;
        width.y = y;

        ADD_LVEC(Vector, &diag1, Vector, f1, Vector, &height);
        ADD_LVEC(Vector, &diag1, Vector, &diag1, Vector, &width);

        SUB_LVEC(Vector, &diag1, Vector, &diag1, Vector, f1);

        hold = diag1.x * diagScale;

        diag1.x = hold >> 12;

        hold = diag1.y * diagScale;

        diag1.y = hold >> 12;

        ADD_LVEC(Vector, &diag2, Vector, f1, Vector, &height);

        SUB_LVEC(Vector, &diag2, Vector, &diag2, Vector, &width);
        SUB_LVEC(Vector, &diag2, Vector, &diag2, Vector, f1);

        hold = diag2.x * diagScale;

        diag2.x = hold >> 12;

        hold = diag2.y * diagScale;

        diag2.y = hold >> 12;

        ADD_LVEC(Vector, &points[0], Vector, f1, Vector, &diag1);
        ADD_LVEC(Vector, &points[1], Vector, f1, Vector, &height);
        ADD_LVEC(Vector, &points[2], Vector, f1, Vector, &diag2);
        ADD_LVEC(Vector, &points[3], Vector, f1, Vector, &width);

        memcpy(&points[4], f1, sizeof(Vector));

        SUB_LVEC(Vector, &points[5], Vector, f1, Vector, &width);
        SUB_LVEC(Vector, &points[6], Vector, f1, Vector, &diag2);
        SUB_LVEC(Vector, &points[7], Vector, f1, Vector, &height);
        SUB_LVEC(Vector, &points[8], Vector, f1, Vector, &diag1);

        points[0].z = 4096;
        points[1].z = 4096;
        points[2].z = 4096;
        points[3].z = 4096;
        points[4].z = 0;
        points[5].z = 4096;
        points[6].z = 4096;
        points[7].z = 4096;
        points[8].z = 4096;

        DRAW_GlowQuad(primPool, ot, otz, color, &points[3], &points[0], &points[4], &points[1]);
        DRAW_GlowQuad(primPool, ot, otz, color, &points[3], &points[4], &points[6], &points[7]);
        DRAW_GlowQuad(primPool, ot, otz, color, &points[1], &points[2], &points[4], &points[5]);
        DRAW_GlowQuad(primPool, ot, otz, color, &points[7], &points[4], &points[8], &points[5]);
    }
}

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

unsigned long *DRAW_DrawGlowPoint(Instance *instance, long seg1, PrimPool *primPool, unsigned long **ot, long color, short width, short height)
{
    Position glowPoints1;
    Vector f1;
    long otz;
    long z;
    long angle;
    int temp; // not from decls.h

    angle = (gameTrackerX.currentTime & 0x7FF) * 2;

    temp = PIPE3D_Segment2ScreenPt(instance, theCamera.core.wcTransform, seg1, &glowPoints1);

    otz = temp + 20;

    z = otz * 4;

    if (z > 0)
    {
        otz = temp + 30;

        f1.x = glowPoints1.x;
        f1.y = glowPoints1.y;

        if (otz < 3072)
        {
            DRAW_CreateAGlowingCircle(&f1, z, primPool, ot, otz, color, width, height, angle);
        }
    }

    return primPool->nextPrim;
}

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
