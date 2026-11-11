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

unsigned char *draw_stack;

long modelDQP;

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

unsigned long *DRAW_DrawShadow(PrimPool *primPool, Model *model, unsigned long **ot, long fadeValue)
{
    POLY_G3_SEMITRANS *sg3;
    SVECTOR zero_vertex;
    long clip;
    long n;
    long p;
    long otz;
    unsigned long color;

    (void)model;

    sg3 = (POLY_G3_SEMITRANS *)primPool->nextPrim;

    zero_vertex.vx = zero_vertex.vy = zero_vertex.vz = 0;

    gte_ldv0(&zero_vertex);
    gte_nrtps();

    color = 0x606060;

    gte_stdp(&p);

    if ((p < fadeValue) && (fadeValue <= 4096))
    {
        p = fadeValue;
    }

    gte_SetRGBcd(&color);
    gte_lddp(p);
    gte_ndpcs();
    gte_strgb(&color);

    color |= 0x32000000;

    for (n = 0; n < 10; n++)
    {
        gte_ldv1(&shadow_vertices[n]);
        gte_ldv2(&shadow_vertices[n + 1]);
        gte_nrtpt();

        if ((unsigned long *)sg3 > (primPool->lastPrim - 12))
        {
            return (unsigned long *)sg3;
        }

        gte_nclip();
        gte_stopz(&clip);

        if (clip < 0)
        {
            gte_navsz3();
            gte_stotz(&otz);

            if ((otz >= 64) && (otz < 3072))
            {
                gte_stsxy3(&sg3->x0, &sg3->x1, &sg3->x2);

                *(int *)&sg3->r0 = color;
                *(int *)&sg3->r1 = 0;
                *(int *)&sg3->r2 = 0;

                // setDrawTPage(sg3, 1, 1, 64);
                sg3->dr_tpage = _get_mode(1, 1, 64);

                // addPrim(ot[otz], sg3);
                sg3->tag = getaddr(&ot[otz]) | 0x7000000;
                ot[otz] = (uintptr_t *)((intptr_t)sg3 & 0xFFFFFF);

                sg3++;

                primPool->numPrims++;
            }
        }
    }

    return (unsigned long *)sg3;
}

void DRAW_FlatQuad(CVECTOR *color, short x0, short y0, short x1, short y1, short x2, short y2, short x3, short y3, PrimPool *primPool, unsigned long **ot)
{
    unsigned long *prim;

    prim = primPool->nextPrim;

    if ((char *)prim < ((char *)primPool->lastPrim - (sizeof(POLY_F4) * 2)))
    {
        long temp; // not from decls.h

        temp = *(long *)color;

        setXY4((POLY_F4 *)prim, x0, y0, x1, y1, x2, y2, x3, y3);
        // setRGB0((POLY_F4*)prim, color->r, color->g, color->b);
        *(long *)&((POLY_F4 *)prim)->r0 = temp;
        // setPolyF4((POLY_F4*)prim);
        ((POLY_F4 *)prim)->code = 0x28;

        // addPrim(ot, prim);
        *(int *)prim = getaddr(ot) | 0x5000000;
        *(int *)ot = (intptr_t)prim & 0xFFFFFF;

        primPool->nextPrim = (uintptr_t *)((char *)primPool->nextPrim + sizeof(POLY_F4));

        primPool->numPrims++;
    }
}

void DRAW_TranslucentQuad(short x0, short y0, short x1, short y1, short x2, short y2, short x3, short y3, short r, short g, short b, short abr, PrimPool *primPool, unsigned long **ot)
{
    unsigned long *prim;

    prim = primPool->nextPrim;

    if (prim < (primPool->lastPrim - 12))
    {
        // setDrawTPage((POLY_F4_SEMITRANS*)prim, 0, 1, 8);
        ((POLY_F4_SEMITRANS *)prim)->dr_tpage = _get_mode(0, 1, 8);

        setAbr((POLY_F4_SEMITRANS *)prim, abr);

        ((POLY_F4_SEMITRANS *)prim)->code = 0x2a;

        setRGB0((POLY_F4_SEMITRANS *)prim, r, g, b);
        setXY4((POLY_F4_SEMITRANS *)prim, x0, y0, x1, y1, x2, y2, x3, y3);

        // addPrim(ot, prim);
        *(int *)prim = getaddr(ot) | 0x6000000;
        *(int *)ot = (intptr_t)prim & 0xFFFFFF;

        primPool->nextPrim += sizeof(POLY_F4_SEMITRANS) / sizeof(unsigned long);

        primPool->numPrims++;
    }
}

void DRAW_DrawButton(ButtonTexture *button, short x, short y, unsigned long **ot)
{
    PrimPool *primPool;
    unsigned long *prim;
    short w;
    short h;
    short offsetx;
    short offsety;

    primPool = gameTrackerX.primPool;

    prim = primPool->nextPrim;

    if (primPool->nextPrim <= (primPool->lastPrim - 12))
    {
        w = button->textureW << button->xshift;
        h = button->textureH;

        offsetx = (button->vramBlock->x & 0x3F) << button->xshift;
        offsety = (char)button->vramBlock->y;

        SetPolyFT4((POLY_FT4 *)prim);

        // setShadeTex(((POLY_FT4*)prim), 1);
        ((POLY_FT4 *)prim)->code |= 0x1;

        ((POLY_FT4 *)prim)->tpage = button->tpage;

        ((POLY_FT4 *)prim)->clut = button->clut;

        ((POLY_FT4 *)prim)->x0 = x;
        ((POLY_FT4 *)prim)->y0 = y;

        ((POLY_FT4 *)prim)->u0 = offsetx;
        ((POLY_FT4 *)prim)->v0 = offsety;

        ((POLY_FT4 *)prim)->x1 = (x + w) - 1;
        ((POLY_FT4 *)prim)->y1 = y;

        ((POLY_FT4 *)prim)->u1 = (offsetx + w) - 1;
        ((POLY_FT4 *)prim)->v1 = offsety;

        ((POLY_FT4 *)prim)->x2 = x;
        ((POLY_FT4 *)prim)->y2 = (y + h) - 1;

        ((POLY_FT4 *)prim)->u2 = offsetx;
        ((POLY_FT4 *)prim)->v2 = (offsety + h) - 1;

        ((POLY_FT4 *)prim)->x3 = (x + w) - 1;
        ((POLY_FT4 *)prim)->y3 = (y + h) - 1;

        ((POLY_FT4 *)prim)->u3 = (offsetx + w) - 1;
        ((POLY_FT4 *)prim)->v3 = (offsety + h) - 1;

        // addPrim(ot, prim);
        *(int *)prim = getaddr(ot) | 0x9000000;
        *(int *)ot = (intptr_t)prim & 0xFFFFFF;

        prim += sizeof(POLY_FT4) / sizeof(unsigned long);

        primPool->nextPrim = prim;

        primPool->numPrims++;
    }
}

void DRAW_LoadButton(long *addr, ButtonTexture *button)
{
    RECT vramRect;
    long *paletteAddr;
    short paletteW;
    short paletteH;

    addr = &addr[2];

    paletteAddr = NULL;

    paletteH = paletteW = 0;

    button->xshift = 0;

    // TODO: find the struct that addr gets parsed to
    if (((long *)addr)[-1] == 8)
    {
        button->xshift = 2;

        paletteW = 16;
        paletteH = 1;

        paletteAddr = &addr[3];
        addr = &addr[11];
    }

    button->textureW = ((unsigned short *)addr)[4];
    button->textureH = ((unsigned short *)addr)[5];

    vramRect.w = (paletteW < button->textureW) ? button->textureW : paletteW;
    vramRect.h = button->textureH + paletteH;

    button->vramBlock = VRAM_CheckVramSlot(&vramRect.x, &vramRect.y, vramRect.w, vramRect.h, 4, 0);

    button->tpage = getTPage(2 - button->xshift, 0, vramRect.x, vramRect.y);

    button->vramBlock->udata.button = button;

    vramRect.w = button->textureW;
    vramRect.h = button->textureH;

    LoadImage(&vramRect, (unsigned long *)&addr[3]);

    if (paletteAddr != NULL)
    {
        vramRect.y += button->textureH;

        vramRect.w = paletteW;
        vramRect.h = paletteH;

        LoadImage(&vramRect, (unsigned long *)paletteAddr);

        button->clut = getClut(vramRect.x, vramRect.y);
    }

    DrawSync(0);
}

void DRAW_FreeButton(ButtonTexture *button)
{
    VRAM_ClearVramBlock(button->vramBlock);
}

void DRAW_RingLine(PrimPool *primPool, unsigned long **ot, long color)
{
    unsigned long *prim;
    long flag;
    long clip;
    long z0;
    long z1;
    long z2;
    long otz;
    long p;

    prim = primPool->nextPrim;

    if (prim <= (primPool->lastPrim - 12))
    {
        gte_stflg(&flag);

        if (!(flag & 0x60000))
        {
            gte_nclip();
            gte_stopz(&p);
            gte_stsz3(&z0, &z1, &z2);

            if (z0 < z1)
            {
                otz = z2;

                if (z0 < z2)
                {
                    otz = z0;
                }
            }
            else
            {
                otz = z2;

                if (z1 < z2)
                {
                    otz = z1;
                }
            }

            otz >>= 2;

            if ((unsigned long)otz < 3072)
            {
                if (otz > 2)
                {
                    otz -= 2;
                }

                gte_stdp(&clip);

                *(int *)&((BLK_FILL *)prim)->r0 = color | 0x40000000;

                gte_stsxy01(&((BLK_FILL *)prim)->x0, &((BLK_FILL *)prim)->w);

                *(int *)prim = getaddr(&ot[otz]) | 0x3000000;
                ot[otz] = (unsigned long *)((intptr_t)prim & 0xFFFFFF);

                prim += 4;
            }
        }

        primPool->nextPrim = prim;
    }
}

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

unsigned long *DRAW_DrawRingPoints(Model *model, VertexPool *vertexPool, MATRIX *pcTransform, PrimPool *primPool, unsigned long **ot, long color, int ring_type)
{
    MFace *mface;          
    MFace *endMFace;      
    PVertex *pvertexList;  
    SVector *pvertex[3];   
    SVector newVertex[3];  
    long outcode;        
    int first;            
    int second;           
    SVector vel;           
    SVector acc;           

    mface    = model->faceList;
    endMFace = &mface[model->numFaces];

    pvertexList = vertexPool->vertex;

    if (ring_type == 2)
    {
        vel.x = vel.y = vel.z = 0;

        acc.x = acc.y = 0;
        acc.z = -1;
    }

    SetRotMatrix(pcTransform);
    SetTransMatrix(pcTransform);

    for (; mface < endMFace; mface++)
    {
        if (!(mface->flags & 0x10))
        {
            pvertex[0] = (SVector *)&pvertexList[mface->face.v0];
            pvertex[1] = (SVector *)&pvertexList[mface->face.v1];
            pvertex[2] = (SVector *)&pvertexList[mface->face.v2];

            outcode = pvertex[0]->z >= 0;

            if (pvertex[1]->z >= 0)
            {
                outcode |= 0x2;
            }

            if (pvertex[2]->z >= 0)
            {
                outcode |= 0x4;
            }

            switch (outcode)
            {
            case 1:
                fDRAW_SPLIT_INTPL_XYZ(&newVertex[1], pvertex[0], pvertex[1]);
                fDRAW_SPLIT_INTPL_XYZ(&newVertex[2], pvertex[0], pvertex[2]);

                first  = 1;
                second = 2;
                break;
            case 2:
                fDRAW_SPLIT_INTPL_XYZ(&newVertex[0], pvertex[1], pvertex[0]);
                fDRAW_SPLIT_INTPL_XYZ(&newVertex[2], pvertex[1], pvertex[2]);

                first  = 2;
                second = 0;
                break;
            case 3:
                fDRAW_SPLIT_INTPL_XYZ(&newVertex[1], pvertex[1], pvertex[2]);
                fDRAW_SPLIT_INTPL_XYZ(&newVertex[0], pvertex[0], pvertex[2]);

                first  = 1;
                second = 0;
                break;
            case 4:
                fDRAW_SPLIT_INTPL_XYZ(&newVertex[0], pvertex[2], pvertex[0]);
                fDRAW_SPLIT_INTPL_XYZ(&newVertex[1], pvertex[2], pvertex[1]);

                first  = 0;
                second = 1;
                break;
            case 5:
                fDRAW_SPLIT_INTPL_XYZ(&newVertex[0], pvertex[0], pvertex[1]);
                fDRAW_SPLIT_INTPL_XYZ(&newVertex[2], pvertex[2], pvertex[1]);

                first  = 0;
                second = 2;
                break;
            case 6:
                fDRAW_SPLIT_INTPL_XYZ(&newVertex[2], pvertex[2], pvertex[0]);
                fDRAW_SPLIT_INTPL_XYZ(&newVertex[1], pvertex[1], pvertex[0]);

                first  = 2;
                second = 1;
                break;
            default:
                continue;
            }

            if (ring_type == 2)
            {
                gte_ldv0(&newVertex[first]);
                gte_nrtv0tr();

                DRAW_RingPoint(primPool, ot, color, &vel, &acc);
            }
            else
            {
                gte_ldv3(&newVertex[first], &newVertex[second], &newVertex[first]);
                gte_nrtpt();

                DRAW_RingLine(primPool, ot, color);
            }
        }
    }

    return primPool->nextPrim;
}

void DRAW_GlowQuad(PrimPool *primPool, unsigned long **ot, long otz, long color, Vector *v0, Vector *v1, Vector *v2, Vector *v3)
{
    unsigned long *prim;
    POLY_NG4 *sg4;
    int *temp; // not from decls.h

    prim = primPool->nextPrim;

    if (prim <= (primPool->lastPrim - 12))
    {
        sg4 = (POLY_NG4 *)prim;

        primPool->numPrims++;

        prim += 10;

        if ((color & 0x1000000))
        {
            // setDrawTPage(sg4, 1, 1, 64);
            sg4->drawTPage1 = _get_mode(1, 1, 64);

            color &= 0xFFFFFF;
        }
        else
        {
            // setDrawTPage(sg4, 1, 1, 32);
            sg4->drawTPage1 = _get_mode(1, 1, 32);
        }

        gte_lddp(4096 - (short)v0->z);
        gte_ldcv(&color);
        gte_ngpf(1);

        sg4->p1.x0 = v0->x;
        sg4->p1.y0 = v0->y;

        gte_stcv(&sg4->p1.r0);
        gte_lddp(4096 - (short)v1->z);
        gte_ldcv(&color);
        gte_ngpf(1);

        sg4->p1.x1 = v1->x;
        sg4->p1.y1 = v1->y;

        gte_stcv(&sg4->p1.r1);
        gte_lddp(4096 - (short)v2->z);
        gte_ldcv(&color);
        gte_ngpf(1);

        sg4->p1.x2 = v2->x;
        sg4->p1.y2 = v2->y;

        gte_stcv(&sg4->p1.r2);
        gte_lddp(4096 - (short)v3->z);
        gte_ldcv(&color);
        gte_ngpf(1);

        sg4->p1.x3 = v3->x;
        sg4->p1.y3 = v3->y;

        gte_stcv(&sg4->p1.r3);

        // addPrim(ot[otz], sg4);

        temp = (int *)((otz * 4) + (uintptr_t)ot);

        *(int *)sg4 = getaddr(&ot[otz]) | 0x9000000;
        *(int *)temp = (intptr_t)sg4 & 0xFFFFFF;

        sg4->p1.code = 0x3A;

        primPool->nextPrim = prim;
    }
}

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

long *DRAW_Zclip_subdiv(POLY_GT3 *texture, unsigned long **ot, int ndiv)
{
    SP_SUBDIV_STRUCT *sp; 
    PrimPool *primPool;   
    int clip;             

    sp = (SP_SUBDIV_STRUCT *)getScratchAddr(128);

    if (ndiv != 0)
    {
        sp = (SP_SUBDIV_STRUCT *)getScratchAddr(181);
    }

    gte_strgb3(&sp->color0, &sp->color1, &sp->color2);

    gte_stv0(&sp->vertex0);
    gte_stv1(&sp->vertex1);
    gte_stv2(&sp->vertex2);

    primPool = gameTrackerX.primPool;

    primPool->nextPrim = (unsigned long *)texture;

    *(long *)&sp->face_v1.vx = *(long *)&sp->vertex1.x;
    *(long *)&sp->face_v0.vx = *(long *)&sp->vertex0.x;
    *(long *)&sp->face_v0.vz = *(long *)&sp->vertex0.z;
    *(long *)&sp->face_v1.vz = *(long *)&sp->vertex1.z;
    *(long *)&sp->face_v2.vx = *(long *)&sp->vertex2.x;
    *(long *)&sp->face_v2.vz = *(long *)&sp->vertex2.z;

    sp->face_v01.vx = (sp->face_v0.vx + sp->face_v1.vx) / 2;
    sp->face_v01.vy = (sp->face_v0.vy + sp->face_v1.vy) / 2;
    sp->face_v01.vz = (sp->face_v0.vz + sp->face_v1.vz) / 2;

    sp->face_v12.vx = (sp->face_v1.vx + sp->face_v2.vx) / 2;
    sp->face_v12.vy = (sp->face_v1.vy + sp->face_v2.vy) / 2;
    sp->face_v12.vz = (sp->face_v1.vz + sp->face_v2.vz) / 2;

    sp->face_v20.vx = (sp->face_v2.vx + sp->face_v0.vx) / 2;
    sp->face_v20.vy = (sp->face_v2.vy + sp->face_v0.vy) / 2;
    sp->face_v20.vz = (sp->face_v2.vz + sp->face_v0.vz) / 2;

    *(long *)&sp->face_uv0 = *(long *)&texture->u0;
    *(long *)&sp->face_uv1 = *(long *)&texture->u1;
    *(long *)&sp->face_uv2 = *(long *)&texture->u2;

    sp->face_uv01.u = (sp->face_uv0.u + sp->face_uv1.u) >> 1;
    sp->face_uv01.v = (sp->face_uv0.v + sp->face_uv1.v) >> 1;
    
    sp->face_uv12.u = (sp->face_uv1.u + sp->face_uv2.u) >> 1;

    sp->face_uv01.pad = sp->face_uv12.pad = sp->face_uv1.pad;

    sp->face_uv12.v = (sp->face_uv1.v + sp->face_uv2.v) >> 1;

    sp->face_uv20.u = (sp->face_uv2.u + sp->face_uv0.u) >> 1;
    sp->face_uv20.v = (sp->face_uv2.v + sp->face_uv0.v) >> 1;

    sp->face_uv20.pad = sp->face_uv0.pad;

    sp->color01.r = (sp->color0.r + sp->color1.r) >> 1;
    sp->color01.g = (sp->color0.g + sp->color1.g) >> 1;
    sp->color01.b = (sp->color0.b + sp->color1.b) >> 1;

    sp->color12.r = (sp->color1.r + sp->color2.r) >> 1;
    sp->color12.g = (sp->color1.g + sp->color2.g) >> 1;
    sp->color12.b = (sp->color1.b + sp->color2.b) >> 1;

    sp->color20.r = (sp->color2.r + sp->color0.r) >> 1;
    sp->color20.g = (sp->color2.g + sp->color0.g) >> 1;
    sp->color20.b = (sp->color2.b + sp->color0.b) >> 1;

    sp->color01.code = sp->color20.code = sp->color0.code;

    clip =  DRAW_DisplayTFace_zclipped_C(&sp->face_v0,  &sp->face_v01, &sp->face_v20, &sp->face_uv0,  &sp->face_uv01, &sp->face_uv20, *(long *)&sp->color0,  *(long *)&sp->color01, *(long *)&sp->color20, primPool, ot, ndiv);
    clip |= DRAW_DisplayTFace_zclipped_C(&sp->face_v20, &sp->face_v12, &sp->face_v2,  &sp->face_uv20, &sp->face_uv12, &sp->face_uv2,  *(long *)&sp->color20, *(long *)&sp->color12, *(long *)&sp->color2,  primPool, ot, ndiv);

    sp->face_uv01.pad = sp->face_uv0.pad;
    sp->face_uv20.pad = sp->face_uv1.pad;

    clip |= DRAW_DisplayTFace_zclipped_C(&sp->face_v01, &sp->face_v12, &sp->face_v20, &sp->face_uv01, &sp->face_uv12, &sp->face_uv20, *(long *)&sp->color01, *(long *)&sp->color12, *(long *)&sp->color20, primPool, ot, ndiv);
    clip |= DRAW_DisplayTFace_zclipped_C(&sp->face_v01, &sp->face_v1,  &sp->face_v12, &sp->face_uv01, &sp->face_uv1,  &sp->face_uv12, *(long *)&sp->color01, *(long *)&sp->color1,  *(long *)&sp->color12, primPool, ot, ndiv);

    if ((clip & 0x5) != 0x4)
    {
        return (long *)primPool->nextPrim;
    }

    return NULL;
}

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
