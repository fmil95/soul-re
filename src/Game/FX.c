#include "common.h"
#include "Game/FX.h"
#include "Game/MATH3D.h"
#include "Game/MEMPACK.h"
#include "Game/DRAW.h"
#include "Game/OBTABLE.h"
#include "Game/LIST.h"
#include "Game/GAMELOOP.h"
#include "Game/STREAM.h"

STATIC FXGeneralEffect *FX_GeneralEffectTracker;

STATIC short Spiral_Number;

STATIC short Spiral_Degrees;

STATIC long Spiral_Current;

STATIC long Spiral_Max;

STATIC FX_PRIM *FX_LastUsedPrim;

STATIC DVECTOR Spiral_Array[65];

STATIC DVECTOR Spiral_OffsetP[64];

STATIC DVECTOR Spiral_OffsetM[64];

STATIC int Spiral_Glow_X;

STATIC int Spiral_Glow_Y;

STATIC int Spiral_Glow_Size;

STATIC int Spiral_Mod;

STATIC Instance *FX_reaver_instance;

STATIC Position FX_ConstrictPosition;

STATIC Position *FX_ConstrictPositionPtr;

STATIC short snow_amount;

STATIC short rain_amount;

STATIC short current_rain_fade;

STATIC short FX_Frames;

STATIC short FX_TimeCount;

void FX_Init(struct _FXTracker *fxTracker)
{
    struct _FX_MATRIX *fxMatrix;
    struct _FX_MATRIX *endFXMatrix;
    struct _FX_PRIM *fxPrim;
    struct _FX_PRIM *endFXPrim;

    fxMatrix = fxTracker->matrixPool;

    endFXMatrix = (struct _FX_MATRIX *)&fxTracker->usedMatrixList;

    fxTracker->usedMatrixList.next = 0;
    fxTracker->usedMatrixList.prev = 0;

    fxTracker->freeMatrixList.next = 0;
    fxTracker->freeMatrixList.prev = 0;

    fxTracker->usedPrimList.next = 0;
    fxTracker->usedPrimList.prev = 0;

    fxTracker->usedPrimListSprite.next = 0;
    fxTracker->usedPrimListSprite.prev = 0;

    fxTracker->freePrimList.next = 0;
    fxTracker->freePrimList.prev = 0;

    while (fxMatrix < endFXMatrix)
    {
        LIST_InsertFunc(&fxTracker->freeMatrixList, &fxMatrix->node);

        fxMatrix++;
    }

    fxPrim = fxTracker->primPool;

    endFXPrim = (struct _FX_PRIM *)&fxTracker->usedPrimList.prev;

    while (fxPrim < endFXPrim)
    {
        LIST_InsertFunc(&fxTracker->freePrimList, &fxPrim->node);

        fxPrim++;
    }

    FX_LastUsedPrim = NULL;

    FX_ConstrictPositionPtr = &FX_ConstrictPosition;

    FX_Spiral_Init();

    snow_amount = 0;

    rain_amount = 0;

    current_rain_fade = 0;

    FX_reaver_instance = NULL;

    FX_Frames = 1;

    FX_TimeCount = 0;
}

void FX_Die(FX_PRIM *fxPrim, FXTracker *fxTracker)
{
    if (FX_LastUsedPrim == fxPrim)
    {
        FX_LastUsedPrim = (FX_PRIM *)fxPrim->node.prev;

        if (FX_LastUsedPrim->node.prev == NULL)
        {
            FX_LastUsedPrim = NULL;
        }
    }

    fxPrim->flags |= 0x10;

    LIST_DeleteFunc(&fxPrim->node);

    LIST_InsertFunc(&fxTracker->freePrimList, &fxPrim->node);
}

FX_MATRIX *FX_GetMatrix(FXTracker *fxTracker)
{
    FX_MATRIX *fxMatrix;

    fxMatrix = (FX_MATRIX *)LIST_GetFunc(&fxTracker->freeMatrixList);

    if (fxMatrix != NULL)
    {
        fxMatrix->flags = 0x1;

        LIST_InsertFunc(&fxTracker->usedMatrixList, &fxMatrix->node);
    }

    return fxMatrix;
}

FX_PRIM *FX_GetPrim(FXTracker *fxTracker)
{
    FX_PRIM *fxPrim;

    fxPrim = (FX_PRIM *)LIST_GetFunc(&fxTracker->freePrimList);

    if (fxPrim == NULL)
    {
        if (FX_LastUsedPrim != NULL)
        {
            fxPrim = FX_LastUsedPrim;

            FX_LastUsedPrim = (FX_PRIM *)fxPrim->node.prev;

            FX_LastUsedPrim->node.next = NULL;

            if (FX_LastUsedPrim->node.prev == NULL)
            {
                FX_LastUsedPrim = NULL;
            }
        }

        if (fxPrim != NULL)
        {
            fxPrim->process = NULL;
            fxPrim->flags = 0;
            fxPrim->matrix = NULL;
        }
    }
    else
    {
        fxPrim->process = NULL;
        fxPrim->flags = 0;
        fxPrim->matrix = NULL;
    }

    return fxPrim;
}

FXParticle *FX_GetParticle(Instance *instance, short startSegment)
{
    FXParticle *particle;

    particle = (FXParticle *)MEMPACK_Malloc(sizeof(FXParticle), 13);

    if (particle != NULL)
    {
        particle->effectType = 1;

        particle->continue_process = FX_ContinueParticle;

        particle->type = 0;

        particle->instance = instance;

        particle->startSegment = startSegment;

        particle->texture = NULL;

        particle->fxprim_process = NULL;
        particle->fxprim_modify_process = NULL;

        particle->startScale = 4096;
        particle->scaleSpeed = 0;

        particle->offset.z = 0;
        particle->offset.y = 0;
        particle->offset.x = 0;

        particle->acceleration.z = 0;
        particle->acceleration.y = 0;
        particle->acceleration.x = 0;

        particle->flag_bits = 0;

        particle->z_undulate = 0;
    }

    return particle;
}

void FX_AniTexSetup(FX_PRIM *fxPrim, MFace *mface, Model *model, FXTracker *fxTracker)
{
    (void)model;
    (void)fxTracker;
    if ((mface->flags & 0x2))
    {
        fxPrim->flags |= 0x1;
        fxPrim->texture = (TextureMT3 *)mface->color;
        fxPrim->color = ((fxPrim->texture->color & 0x3FFFFFF)) | 0x24000000;
    }
    else
    {
        fxPrim->flags &= ~0x1;
        fxPrim->flags &= ~0x4;
        fxPrim->color = ((mface->color & 0x3FFFFFF)) | 0x20000000;
    }
}

void FX_StandardProcess(FX_PRIM *fxPrim, FXTracker *fxTracker)
{
    FX_StandardFXPrimProcess(fxPrim, fxTracker);
}

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_ShatterProcess);

void FX_DFacadeProcess(struct _FX_PRIM *fxPrim, struct _FXTracker *fxTracker)
{
    MATRIX *swTransform;
    struct _Rotation rot;
    if (fxPrim->timeToLive > 0)
    {
        fxPrim->timeToLive = fxPrim->timeToLive - 1;
    }
    if (fxPrim->timeToLive == 0)
    {
        FX_Die(fxPrim, fxTracker);
    }
    else
    {
        if (fxPrim->flags & 0x20)
        {
            swTransform = (MATRIX *)fxPrim->duo.flame.parent->matrix + ((int *)&fxPrim->duo.phys.zVel)[0];
            fxPrim->position.x = (short)swTransform->t[0];
            fxPrim->position.y = (short)swTransform->t[1];
            fxPrim->position.z = (short)swTransform->t[2];
        }
        else
        {
            if ((fxPrim->flags & 2) == 0)
            {
                fxPrim->duo.phys.xVel += fxPrim->duo.phys.xAccl;
                fxPrim->duo.phys.yVel += fxPrim->duo.phys.yAccl;
                fxPrim->duo.phys.zVel += fxPrim->duo.phys.zAccl;
                fxPrim->position.x += fxPrim->duo.phys.xVel;
                fxPrim->position.y += fxPrim->duo.phys.yVel;
                fxPrim->position.z += fxPrim->duo.phys.zVel;
                if (((fxPrim->flags & 0x100) != 0) && (fxPrim->work0 < fxPrim->position.z) == 0)
                {
                    fxPrim->position.z = fxPrim->work0;
                    fxPrim->flags |= 2;
                }
            }
        }
        if ((fxPrim->matrix->flags & 2) == 0)
        {
            fxPrim->matrix->flags |= 2;
            if ((fxPrim->flags & 0x80) != 0)
            {
                rot.x = ((char *)&fxPrim->work2)[1] * 4;
                rot.y = ((char *)&fxPrim->work3)[0] * 4;
                rot.z = ((char *)&fxPrim->work3)[1] * 4;
                RotMatrixX(rot.x, (MATRIX *)&fxPrim->matrix->lwTransform);
                RotMatrixY(rot.y, (MATRIX *)&fxPrim->matrix->lwTransform);
                RotMatrixZ(rot.z, (MATRIX *)&fxPrim->matrix->lwTransform);
            }
        }
    }
}

struct _FX_PRIM *_FX_BuildSingleFaceWithModel(struct _Model *model, struct _MFace *mface, SVECTOR *center, SVECTOR *vel, SVECTOR *accl, struct _FXTracker *fxTracker,
    void (*fxSetup)(struct _FX_PRIM *fxPrim, void *fxProcess, struct _FX_MATRIX *fxMatrix, int value, struct _MFace *mface, struct _MVertex *vertexList, SVECTOR *center, SVECTOR *vel, SVECTOR *accl, struct _FXTracker *fxTracker, short timetoLive),
    void *fxProcess, struct _FX_MATRIX *fxMatrix, short timeToLive)
{
    struct _FX_PRIM *fxPrim;
    struct _MVertex *vertexList;
    struct _FX_MATRIX *temp; // not from decls.h

    temp = fxMatrix;

    vertexList = model->vertexList;

    fxPrim = NULL;

    if ((temp != NULL) || (temp = FX_GetMatrix(fxTracker), temp != NULL))
    {
        fxPrim = FX_GetPrim(fxTracker);

        if (fxPrim != NULL)
        {
            if (fxSetup != NULL)
            {
                fxSetup(fxPrim, fxProcess, temp, 0, mface, vertexList, center, vel, accl, fxTracker, timeToLive);
            }

            LIST_InsertFunc(&fxTracker->usedPrimList, (struct NodeType *)fxPrim);
        }
    }

    return fxPrim;
}

// timeToLive parameter is a different data type than the one specified in the debugging symbols
// struct _FX_PRIM *FX_BuildSingleFaceWithModel(struct _Model *model, struct _MFace *mface, struct SVECTOR *center, struct SVECTOR *vel, struct SVECTOR *accl, struct _FXTracker *fxTracker, void (*fxSetup)(), void (*fxProcess)(), int timeToLive)
struct _FX_PRIM *FX_BuildSingleFaceWithModel(struct _Model *model, struct _MFace *mface, struct SVECTOR *center, struct SVECTOR *vel, struct SVECTOR *accl, struct _FXTracker *fxTracker, void (*fxSetup)(), void (*fxProcess)(), short timeToLive)
{
    return _FX_BuildSingleFaceWithModel(model, mface, center, vel, accl, fxTracker, fxSetup, fxProcess, 0, timeToLive);
}

void _FX_SetupLighting(struct _Instance *instance)
{
    MATRIX lm;

    LIGHT_PresetInstanceLight(instance, 2048, &lm);

    MulMatrix0(&lm, &instance->matrix[instance->lightMatrix], &lm);

    SetLightMatrix(&lm);
}

INCLUDE_ASM("asm/nonmatchings/Game/FX", _FX_DoLighting);

INCLUDE_ASM("asm/nonmatchings/Game/FX", _FX_BuildSegmentedSplinters);

INCLUDE_ASM("asm/nonmatchings/Game/FX", _FX_BuildNonSegmentedSplinters);

//shardflags is a different parameter than specified in debugging symbols
//void _FX_BuildSplinters(struct _Instance *instance, struct SVECTOR *center, struct SVECTOR *vel, struct SVECTOR *accl, struct FXSplinter *splintDef, struct _FXTracker *fxTracker, void (*fxSetup)(), void (*fxProcess)(), int shardFlags)
void _FX_BuildSplinters(struct _Instance *instance, struct SVECTOR *center, struct SVECTOR *vel, struct SVECTOR *accl, struct FXSplinter *splintDef, struct _FXTracker *fxTracker, void (*fxSetup)(), void (*fxProcess)(), short shardFlags)
{
    if (MEMPACK_MemoryValidFunc((char *)instance->object) != 0)
    {
        if (splintDef != NULL)
        {
            shardFlags |= splintDef->flags;

            if (splintDef->soundFx != 0)
            {
                SndPlay(splintDef->soundFx);
            }
        }

        if ((instance->object->modelList[instance->currentModel]->numSegments < 4) || ((shardFlags & 0x20)))
        {
            _FX_BuildNonSegmentedSplinters(instance, center, vel, accl, splintDef, fxTracker, fxSetup, fxProcess, shardFlags);
        }
        else
        {
            _FX_BuildSegmentedSplinters(instance, center, vel, accl, splintDef, fxTracker, fxSetup, fxProcess, shardFlags);
        }
    }
}

// shardFlags is a different type than specified in debugging symbols 
// void _FX_Build(struct _Instance *instance, struct SVECTOR *center, struct SVECTOR *vel, struct SVECTOR *accl, struct _FXTracker *fxTracker, void (*fxSetup)(), void (*fxProcess)(), int shardFlags)
void _FX_Build(struct _Instance *instance, struct SVECTOR *center, struct SVECTOR *vel, struct SVECTOR *accl, struct _FXTracker *fxTracker, void (*fxSetup)(), void (*fxProcess)(), short shardFlags)
{
    if (MEMPACK_MemoryValidFunc((char *)instance->object) != 0)
    {
        if (instance->object->modelList[instance->currentModel]->numSegments < 4)
        {
            _FX_BuildNonSegmentedSplinters(instance, center, vel, accl, NULL, fxTracker, fxSetup, fxProcess, shardFlags);
        }
        else
        {
            _FX_BuildSegmentedSplinters(instance, center, vel, accl, NULL, fxTracker, fxSetup, fxProcess, shardFlags);
        }
    }
}

void FX_Build(struct _Instance *instance, struct SVECTOR *center, struct SVECTOR *vel, struct SVECTOR *accl, struct _FXTracker *fxTracker, void (*fxSetup)(), void (*fxProcess)())
{
    _FX_Build(instance, center, vel, accl, fxTracker, fxSetup, fxProcess, 0);
}

void FX_UpdatePos(FX_PRIM *fxPrim, SVector *offset, int spriteflag)
{
    ADD_SVEC(Position, &fxPrim->position, Position, &fxPrim->position, SVector, offset);

    if ((spriteflag == 0) && ((fxPrim->flags & 0x10000)))
    {
        fxPrim->v0.x += offset->x;
        fxPrim->v0.y += offset->y;
        fxPrim->v0.z += offset->z;

        fxPrim->v1.x += offset->x;
        fxPrim->v1.y += offset->y;
        fxPrim->v1.z += offset->z;

        fxPrim->v2.x += offset->x;
        fxPrim->v2.y += offset->y;
        fxPrim->v2.z += offset->z;

        if ((fxPrim->flags & 0x8))
        {
            fxPrim->v3.x += offset->x;
            fxPrim->v3.y += offset->y;
            fxPrim->v3.z += offset->z;
        }
    }
}

void FX_WaterBubbleProcess(struct _FX_PRIM *fxPrim, struct _FXTracker *fxTracker);
void FX_Relocate(struct _SVector *offset)
{
    struct _FX_PRIM *fxPrim;
    struct _FXTracker *fxTracker;
    struct _FXGeneralEffect *currentEffect;
    int i;
    int end;
    struct _FXRibbon *currentRibbon;

    fxTracker = gFXT;

    fxPrim = (struct _FX_PRIM *)fxTracker->usedPrimList.next;

    while (fxPrim != NULL)
    {
        FX_UpdatePos(fxPrim, offset, 0);

        fxPrim = (struct _FX_PRIM *)fxPrim->node.next;
    }

    fxPrim = (struct _FX_PRIM *)fxTracker->usedPrimListSprite.next;

    while (fxPrim != NULL)
    {
        FX_UpdatePos(fxPrim, offset, 1);

        if (fxPrim->process == &FX_WaterBubbleProcess)
        {
            fxPrim->timeToLive += offset->z;
        }

        fxPrim = (struct _FX_PRIM *)fxPrim->node.next;
    }

    currentEffect = FX_GeneralEffectTracker;

    while (currentEffect != NULL)
    {
        if (currentEffect->effectType == 0)
        {
            end = ((short *)currentEffect)[8];

            currentRibbon = (struct _FXRibbon *)currentEffect;

            for (i = 0; i < end; i++)
            {
                currentRibbon->vertexPool[i].vx += offset->x;
                currentRibbon->vertexPool[i].vy += offset->y;
                currentRibbon->vertexPool[i].vz += offset->z;
            }
        }
        else if (currentEffect->effectType == 0x84)
        {
            ((struct _GenericLightningParams *)currentEffect)->end_offset.x += offset->x;
            ((struct _GenericLightningParams *)currentEffect)->end_offset.y += offset->y;
            ((struct _GenericLightningParams *)currentEffect)->end_offset.z += offset->z;
        }

        currentEffect = (struct _FXGeneralEffect *)currentEffect->next;
    }
}

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_UpdateTexturePointers);

void FX_RelocateFXPointers(struct Object *oldObject, struct Object *newObject, long sizeOfObject)
{
    struct _FXTracker *fxTracker; // $s0
    struct _FXGeneralEffect *currentEffect; // $a0
    long offset; // $s1
    struct _FXParticle *particle; // not from SYMDUMP

    fxTracker = gFXT;

    offset = (int)newObject - (int)oldObject;

    FX_UpdateTexturePointers((struct _FX_PRIM *)fxTracker->usedPrimList.next, oldObject, (int)sizeOfObject, (int)offset);
    FX_UpdateTexturePointers((struct _FX_PRIM *)fxTracker->usedPrimListSprite.next, oldObject, (int)sizeOfObject, (int)offset);

    currentEffect = FX_GeneralEffectTracker;

    while (currentEffect != NULL)
    {
        if (currentEffect->effectType == 1)
        {
            particle = (struct _FXParticle *)currentEffect;
            if (particle->texture != NULL && IN_BOUNDS(particle->texture, oldObject, (int)oldObject + sizeOfObject))
            {
                particle->texture = (struct TextureMT3 *)OFFSET_DATA(particle->texture, offset);
            }
        }

        currentEffect = (struct _FXGeneralEffect *)currentEffect->next;
    }
}

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_ProcessList);

void FX_DrawReaver(PrimPool *primPool, unsigned long **ot, MATRIX *wcTransform)
{
    if (FX_reaver_instance != NULL)
    {
        FX_SoulReaverWinding(FX_reaver_instance, primPool, ot, wcTransform);
        FX_SoulReaverBlade(FX_reaver_instance, ot);
    }

    FX_reaver_instance = NULL;
}

void FX_DrawList(FXTracker *fxTracker, GameTracker *gameTracker, unsigned long **ot, MATRIX *wcTransform)
{
    TextureMT3 *texture;
    PrimPool *primPool;
    long *prim;
    FX_PRIM *fxPrim;
    FX_PRIM *nextFXPrim;
    SVECTOR sv0;
    SVECTOR sv1;
    SVECTOR sv2;
    long otz;
    long sz0;
    long sz1;
    long sz2;
    char whitec[4];
    int sizex;
    int sizey;
    int matrix_wc;

    matrix_wc = 0;

    primPool = gameTracker->primPool;

    prim = (long *)primPool->nextPrim;

    whitec[0] = whitec[1] = whitec[2] = 31;

    for (fxPrim = (FX_PRIM *)fxTracker->usedPrimList.next; fxPrim != NULL; fxPrim = nextFXPrim)
    {
        long flags;

        if ((prim + 10) >= (long *)primPool->lastPrim)
        {
            break;
        }

        flags = fxPrim->flags;

        nextFXPrim = (FX_PRIM *)fxPrim->node.next;

        if (!(flags & 0x10))
        {
            if (!(flags & 0x10000))
            {
                fxPrim->matrix->lwTransform.t[0] = fxPrim->position.x;
                fxPrim->matrix->lwTransform.t[1] = fxPrim->position.y;
                fxPrim->matrix->lwTransform.t[2] = fxPrim->position.z;

                CompMatrix(wcTransform, &fxPrim->matrix->lwTransform, (MATRIX *)getScratchAddr(0));

                SetRotMatrix((MATRIX *)getScratchAddr(0));
                SetTransMatrix((MATRIX *)getScratchAddr(0));

                matrix_wc = 0;
            }
            else if (matrix_wc == 0)
            {
                gte_SetRotMatrix(wcTransform);
                gte_SetTransMatrix(wcTransform);

                matrix_wc = 1;
            }

            if (!(flags & 0x2000))
            {
                gte_ldv3(&fxPrim->v0, &fxPrim->v1, &fxPrim->v2);
            }
            else
            {
                sv0.vx = (fxPrim->v0.x * fxPrim->v0.y) >> 12;
                sv1.vx = (fxPrim->v1.x * fxPrim->v0.y) >> 12;
                sv2.vx = (fxPrim->v2.x * fxPrim->v0.y) >> 12;

                sv0.vz = (fxPrim->v0.z * fxPrim->v2.y) >> 12;
                sv1.vz = (fxPrim->v1.z * fxPrim->v2.y) >> 12;
                sv2.vz = (fxPrim->v2.z * fxPrim->v2.y) >> 12;

                sv0.vy = 0;
                sv1.vy = 0;
                sv2.vy = 0;

                gte_ldv3(&sv0, &sv1, &sv2);
            }

            gte_nrtpt();

            if ((flags & 0x1000000))
            {
                gte_stsz3(&sz0, &sz1, &sz2);

                otz = sz0 >> 2;
            }
            else if ((flags & 0x4000))
            {
                int temp; // not from decls.h

                gte_stsz3(&sz0, &sz1, &sz2);

                if (sz0 > sz1)
                {
                    temp = (sz2 < sz0) ? sz0 : sz2;
                }
                else
                {
                    temp = (sz2 < sz1) ? sz1 : sz2;
                }

                otz = temp >> 2;
            }
            else
            {
                gte_stszotz(&otz);
            }

            if ((flags & 0x8000))
            {
                otz -= 20;
            }

            if ((otz >= 1) && (otz < 3072))
            {
                if (!(flags & 0x1))
                {
                    if ((flags & 0x8))
                    {
                        if ((flags & 0x80000))
                        {
                            POLY_NG4 *ng4;
                            int n;
                            long *ptr;
                            int *temp; // not from decls.h

                            gte_stsxy3(&((POLY_NG4 *)prim)->p1.x0, &((POLY_NG4 *)prim)->p1.x1, &((POLY_NG4 *)prim)->p1.x2);

                            gte_ldv0(&fxPrim->v3);

                            gte_nrtps();

                            ng4 = (POLY_NG4 *)prim;

                            prim += 10;

                            // setDrawTPage(ng4, 1, 1, 32);

                            ng4->drawTPage1 = _get_mode(1, 1, 32);

                            gte_stsxy(&ng4->p1.x3);

                            ptr = (long *)&ng4->p1;

                            for (n = 0; n < 4; n++)
                            {
                                gte_lddp(4096 - fxPrim->fadeValue[n]);

                                gte_ldcv(&fxPrim->color);

                                gte_ngpf(1);

                                gte_stcv(ptr);

                                ptr += 2;
                            }

                            // addPrim(ot[otz], ng4);

                            temp = (int *)((otz * 4) + (uintptr_t)ot);

                            *(int *)ng4 = getaddr(temp) | 0x9000000;
                            *(int *)temp = (intptr_t)ng4 & 0xFFFFFF;

                            ng4->p1.code = 0x3A; // Poly G4 + semitrans flag
                        }
                        else
                        {
                            POLY_G4 *temp; // not from decls.h
                            int *temp2;    // not from decls.h

                            temp = (POLY_G4 *)prim;

                            gte_stsxy3(&temp->x0, &temp->x1, &temp->x2);

                            gte_ldv0(&fxPrim->v3);

                            gte_nrtps();

                            *(int *)&temp->r0 = fxPrim->color;
                            *(int *)&temp->r1 = fxPrim->color;
                            *(int *)&temp->r2 = fxPrim->color;
                            *(int *)&temp->r3 = fxPrim->color;

                            setPolyG4(temp);

                            gte_stsxy(&temp->x3);

                            // addPrim(ot[otz], temp);

                            temp2 = (int *)((otz * 4) + (uintptr_t)ot);

                            *(int *)temp = getaddr(temp2) | 0x8000000;
                            *(int *)temp2 = (intptr_t)temp & 0xFFFFFF;

                            prim += 9;
                        }
                    }
                    else if ((flags & 0x1000000))
                    {
                        if ((flags & 0x80000))
                        {
                            LINE_SG2 *temp; // not from decls.h
                            int *temp2;     // not from decls.h

                            temp = (LINE_SG2 *)prim;

                            *(int *)&temp->r0 = fxPrim->color;
                            *(int *)&temp->r1 = fxPrim->endColor;

                            // setDrawTPage(temp, 1, 1, 32);

                            temp->drawTPage1 = _get_mode(1, 1, 32);

                            gte_stsxy0(&temp->x1);
                            gte_stsxy1(&temp->x0);

                            temp->code = 0x52; // Line G2 + semitrans flag

                            // addPrim(ot[otz], temp);

                            temp2 = (int *)((otz * 4) + (uintptr_t)ot);

                            *(int *)temp = getaddr(temp2) | 0x5000000;
                            *(int *)temp2 = (intptr_t)temp & 0xFFFFFF;

                            prim += 6;
                        }
                    }
                    else
                    {
                        // usage of POLY F3 in this block is just an estimate, it could be another prim type
                        POLY_F3 *temp; // not from decls.h
                        int *temp2;    // not from decls.h

                        temp = (POLY_F3 *)prim;

                        *(int *)&temp->r0 = fxPrim->color;

                        gte_stsxy3(&temp->x0, &temp->x1, &temp->x2);

                        // addPrim(ot[otz], temp);

                        temp2 = (int *)((otz * 4) + (uintptr_t)ot);

                        *(int *)temp = getaddr(temp2) | 0x4000000;
                        *(int *)temp2 = (intptr_t)temp & 0xFFFFFF;

                        prim += 5;
                    }
                }
                else
                {
                    texture = fxPrim->texture;

                    if ((flags & 0x8))
                    {
                        POLY_FT4 *ft4;
                        unsigned short uMin;
                        unsigned short uMax;
                        unsigned short vMin;
                        unsigned short vMax;
                        int *temp; // not from decls.h

                        ft4 = (POLY_FT4 *)prim;

                        gte_stsxy3(&ft4->x0, &ft4->x1, &ft4->x2);

                        gte_ldv0(&fxPrim->v3);

                        gte_nrtps();

                        if (texture->u0 < texture->u1)
                        {
                            if (texture->u0 < texture->u2)
                            {
                                uMin = texture->u0;
                            }
                            else
                            {
                                uMin = texture->u2;
                            }
                        }
                        else if (texture->u1 < texture->u2)
                        {
                            uMin = texture->u1;
                        }
                        else
                        {
                            uMin = texture->u2;
                        }

                        if (texture->u0 > texture->u1)
                        {
                            if (texture->u0 > texture->u2)
                            {
                                uMax = texture->u0;
                            }
                            else
                            {
                                uMax = texture->u2;
                            }
                        }
                        else if (texture->u1 > texture->u2)
                        {
                            uMax = texture->u1;
                        }
                        else
                        {
                            uMax = texture->u2;
                        }

                        if (texture->v0 < texture->v1)
                        {
                            if (texture->v0 < texture->v2)
                            {
                                vMin = texture->v0 << 8;
                            }
                            else
                            {
                                vMin = texture->v2 << 8;
                            }
                        }
                        else if (texture->v1 < texture->v2)
                        {
                            vMin = texture->v1 << 8;
                        }
                        else
                        {
                            vMin = texture->v2 << 8;
                        }

                        if (texture->v0 > texture->v1)
                        {
                            if (texture->v0 > texture->v2)
                            {
                                vMax = texture->v0 << 8;
                            }
                            else
                            {
                                vMax = texture->v2 << 8;
                            }
                        }
                        else if (texture->v1 > texture->v2)
                        {
                            vMax = texture->v1 << 8;
                        }
                        else
                        {
                            vMax = texture->v2 << 8;
                        }

                        *(short *)&ft4->u0 = uMin | vMin;
                        *(short *)&ft4->u1 = uMax | vMin;
                        *(short *)&ft4->u2 = uMin | vMax;
                        *(short *)&ft4->u3 = uMax | vMax;

                        *(unsigned short *)&ft4->clut = texture->clut;

                        *(unsigned short *)&ft4->tpage = texture->tpage;

                        gte_stsxy(&((POLY_FT4 *)prim)->x3);

                        if ((flags & 0x1000))
                        {
                            char temp; // not from decls.h

                            ft4->u0 ^= ft4->u1;

                            temp = ft4->u0 ^ ft4->u1;

                            ft4->u1 ^= ft4->u0;
                            ft4->u0 ^= ft4->u1;
                            ft4->u2 ^= ft4->u3;

                            temp = ft4->u2 ^ ft4->u3;

                            (void)temp;

                            ft4->u3 ^= ft4->u2;
                            ft4->u2 ^= ft4->u3;
                        }

                        *(int *)&((POLY_FT4 *)prim)->r0 = fxPrim->color;

                        // addPrim(ot[otz], prim);

                        temp = (int *)((otz * 4) + (uintptr_t)ot);

                        *(int *)prim = getaddr(temp) | 0x9000000;
                        *(int *)temp = (intptr_t)prim & 0xFFFFFF;

                        prim += 10;
                    }
                    else
                    {
                        POLY_FT3 *ft3;
                        int *temp; // not from decls.h

                        ft3 = (POLY_FT3 *)prim;

                        gte_stsxy3(&ft3->x0, &ft3->x1, &ft3->x2);

                        *(int *)&ft3->u0 = *(int *)&texture->u0;
                        *(int *)&ft3->u1 = *(int *)&texture->u1;
                        *(int *)&ft3->u2 = *(int *)&texture->u2;

                        if ((flags & 0x1000))
                        {
                            short uMin;
                            short uMax;

                            if (texture->u0 < texture->u1)
                            {
                                if (texture->u0 < texture->u2)
                                {
                                    uMin = texture->u0;
                                }
                                else
                                {
                                    uMin = texture->u2;
                                }
                            }
                            else if (texture->u1 < texture->u2)
                            {
                                uMin = texture->u1;
                            }
                            else
                            {
                                uMin = texture->u2;
                            }

                            if (texture->u0 > texture->u1)
                            {
                                if (texture->u0 > texture->u2)
                                {
                                    uMax = texture->u0;
                                }
                                else
                                {
                                    uMax = texture->u2;
                                }
                            }
                            else if (texture->u1 > texture->u2)
                            {
                                uMax = texture->u1;
                            }
                            else
                            {
                                uMax = texture->u2;
                            }

                            ft3->u0 = (texture->u0 == uMin) ? uMax : uMin;
                            ft3->u1 = (texture->u1 == uMin) ? uMax : uMin;
                            ft3->u2 = (texture->u2 == uMin) ? uMax : uMin;
                        }

                        *(int *)&((POLY_FT3 *)prim)->r0 = fxPrim->color;

                        // addPrim(ot[otz], prim);

                        temp = (int *)((otz * 4) + (uintptr_t)ot);

                        *(int *)prim = getaddr(temp) | 0x7000000;
                        *(int *)temp = (intptr_t)prim & 0xFFFFFF;

                        prim += 8;
                    }
                }
            }
        }
    }

    SetRotMatrix(wcTransform);
    SetTransMatrix(wcTransform);

    for (fxPrim = (FX_PRIM *)fxTracker->usedPrimListSprite.next; fxPrim != NULL; fxPrim = nextFXPrim)
    {
        DVECTOR xy_pos;
        long flags;

        sizex = fxPrim->v0.x << 1;

        gte_ldv0(&fxPrim->position);

        gte_lddqb(0);

        gte_lddqa(sizex);

        gte_nrtps();

        nextFXPrim = (FX_PRIM *)fxPrim->node.next;

        flags = fxPrim->flags;

        if ((prim + 10) >= (long *)primPool->lastPrim)
        {
            break;
        }

        if (!(flags & 0x10))
        {
            gte_stsxy(&xy_pos);

            gte_stopz(&sizex);

            gte_stsz(&sz0);

            otz = sz0 >> 2;

            if ((flags & 0x2008000))
            {
                if (!(flags & 0x8000))
                {
                    if (((flags & 0x2000000)) && (otz > 50))
                    {
                        otz = 3071;
                    }
                }
                else
                {
                    otz -= 20;
                }
            }

            if (((flags & 0x800000)) && ((sz0 < 384) || (sz0 >= 4096)))
            {
                otz = 0;
            }

            if (((otz > 50) && (otz < 3072)) && (((flags & 0x4000000)) || ((xy_pos.vx >= 0) && (xy_pos.vy >= 0) && (xy_pos.vx < 512) && (xy_pos.vy < 240))))
            {
                int temp; // not from decls.h

                if (((flags & 0x2000)) && (fxPrim->v0.y != 4096))
                {
                    sizex = (sizex >> 12) * fxPrim->v0.y;

                    if (fxPrim->v0.x == fxPrim->v0.z)
                    {
                        sizey = sizex;
                    }
                    else
                    {
                        sizey = (((fxPrim->v0.z * 640) * fxPrim->v0.y) / sz0) << 4;
                    }

                    temp = 0xFFFF;
                }
                else if (fxPrim->v0.x == fxPrim->v0.z)
                {
                    sizey = sizex;
                }
                else
                {
                    sizey = ((fxPrim->v0.z << 16) * 640) / sz0;
                }

                temp = 0xFFFF;

                sizex = (((sizex / 320) << 9) + temp) >> 16;
                sizey = (sizey + temp) >> 16;

                if (!(flags & 0x1))
                {
                    if ((flags & 0x8))
                    {
                        if ((flags & 0x80000))
                        {
                            POLY_SG4 *sg4;
                            int n;
                            long *src;
                            long *dst;
                            long *ptr;
                            int *temp; // not from decls.h

                            sg4 = (POLY_SG4 *)prim;

                            prim += 16;

                            // setDrawTPage(sg4, 1, 1, 32);

                            sg4->drawTPage1 = _get_mode(1, 1, 32);

                            // setDrawTPage(sg4, 1, 1, 64);

                            sg4->drawTPage2 = _get_mode(1, 1, 64);

                            ptr = (long *)&sg4->p1;

                            src = (long *)&sg4->p1.x0;
                            dst = (long *)&sg4->p2.x0;

                            sg4->p1.x0 = sg4->p1.x2 = xy_pos.vx - (sizex / 2);
                            sg4->p1.y0 = sg4->p1.y1 = xy_pos.vy - (sizey / 2);
                            sg4->p1.x1 = sg4->p1.x3 = sg4->p1.x0 + sizex;
                            sg4->p1.y2 = sg4->p1.y3 = sg4->p1.y1 + sizey;

                            for (n = 0; n < 4; n++)
                            {
                                gte_lddp(4096 - fxPrim->fadeValue[0]);

                                gte_ldcv(&fxPrim->color);

                                gte_ngpf(1);

                                *dst = *src;

                                src += 2;

                                dst += 1;

                                gte_stcv(ptr);

                                ptr += 2;
                            }

                            gte_lddp(4096 - fxPrim->fadeValue[0]);

                            gte_ldcv(&whitec);

                            gte_ngpf(1);

                            // addPrim(ot[otz], sg4);

                            temp = (int *)((otz * 4) + (uintptr_t)ot);

                            *(int *)sg4 = getaddr(temp) | 0xF000000;
                            *(int *)temp = (intptr_t)sg4 & 0xFFFFFF;

                            gte_stcv(&sg4->p2.r0);

                            sg4->p1.code = 0x3A; // Poly G4 + semitrans flag
                            sg4->p2.code = 0x2A; // POLY_F4_SEMITRANS code
                        }
                        else
                        {
                            POLY_G4 *temp; // not from decls.h
                            int *temp2;    // not from decls.h

                            temp = (POLY_G4 *)prim;

                            temp->x0 = temp->x2 = xy_pos.vx - (sizex / 2);
                            temp->y0 = temp->y1 = xy_pos.vy - (sizey / 2);
                            temp->x1 = temp->x3 = temp->x0 + sizex;
                            temp->y2 = temp->y3 = temp->y1 + sizey;

                            *(int *)&temp->r0 = fxPrim->color;
                            *(int *)&temp->r1 = fxPrim->color;
                            *(int *)&temp->r2 = fxPrim->color;
                            *(int *)&temp->r3 = fxPrim->color;

                            setPolyG4(prim);

                            // addPrim(ot[otz], prim);

                            temp2 = (int *)((otz * 4) + (uintptr_t)ot);

                            *(int *)prim = getaddr(temp2) | 0x8000000;
                            *(int *)temp2 = (intptr_t)prim & 0xFFFFFF;

                            prim += 9;
                        }
                    }
                }
                else
                {
                    unsigned short uMin;
                    unsigned short uMax;
                    unsigned short vMin;
                    unsigned short vMax;
                    POLY_FT4 *ft4;
                    int *temp; // not from decls.h

                    texture = fxPrim->texture;

                    if ((flags & 0x8))
                    {
                        ft4 = (POLY_FT4 *)prim;

                        if (texture->u0 < texture->u1)
                        {
                            if (texture->u0 < texture->u2)
                            {
                                uMin = texture->u0;
                            }
                            else
                            {
                                uMin = texture->u2;
                            }
                        }
                        else if (texture->u1 < texture->u2)
                        {
                            uMin = texture->u1;
                        }
                        else
                        {
                            uMin = texture->u2;
                        }

                        if (texture->u0 > texture->u1)
                        {
                            if (texture->u0 > texture->u2)
                            {
                                uMax = texture->u0;
                            }
                            else
                            {
                                uMax = texture->u2;
                            }
                        }
                        else if (texture->u1 > texture->u2)
                        {
                            uMax = texture->u1;
                        }
                        else
                        {
                            uMax = texture->u2;
                        }

                        if (texture->v0 < texture->v1)
                        {
                            if (texture->v0 < texture->v2)
                            {
                                vMin = texture->v0 << 8;
                            }
                            else
                            {
                                vMin = texture->v2 << 8;
                            }
                        }
                        else if (texture->v1 < texture->v2)
                        {
                            vMin = texture->v1 << 8;
                        }
                        else
                        {
                            vMin = texture->v2 << 8;
                        }

                        if (texture->v0 > texture->v1)
                        {
                            if (texture->v0 > texture->v2)
                            {
                                vMax = texture->v0 << 8;
                            }
                            else
                            {
                                vMax = texture->v2 << 8;
                            }
                        }
                        else if (texture->v1 > texture->v2)
                        {
                            vMax = texture->v1 << 8;
                        }
                        else
                        {
                            vMax = texture->v2 << 8;
                        }

                        *(short *)&ft4->u0 = uMin | vMin;
                        *(short *)&ft4->u1 = uMax | vMin;
                        *(short *)&ft4->u2 = uMin | vMax;
                        *(short *)&ft4->u3 = uMax | vMax;

                        *(unsigned short *)&ft4->clut = texture->clut;

                        *(unsigned short *)&ft4->tpage = texture->tpage;

                        ft4->x0 = ft4->x2 = xy_pos.vx + (sizex / 2);
                        ft4->y0 = ft4->y1 = xy_pos.vy + (sizey / 2);
                        ft4->x1 = ft4->x3 = ft4->x0 - sizex;
                        ft4->y2 = ft4->y3 = ft4->y1 - sizey;

                        if ((flags & 0x1000))
                        {
                            char temp; // not from decls.h

                            ft4->u0 ^= ft4->u1;

                            temp = ft4->u0 ^ ft4->u1;

                            ft4->u1 ^= ft4->u0;
                            ft4->u0 ^= ft4->u1;
                            ft4->u2 ^= ft4->u3;

                            temp = ft4->u2 ^ ft4->u3;

                            (void)temp;

                            ft4->u3 ^= ft4->u2;
                            ft4->u2 ^= ft4->u3;
                        }

                        *(int *)&ft4->r0 = fxPrim->color;

                        // addPrim(ot[otz], prim);

                        temp = (int *)((otz * 4) + (uintptr_t)ot);

                        *(int *)prim = getaddr(temp) | 0x9000000;
                        *(int *)temp = (intptr_t)prim & 0xFFFFFF;

                        prim += 10;
                    }
                }
            }
            else if ((flags & 0x400000))
            {
                fxPrim->timeToLive = 1;
            }
            else if ((flags & 0x800000))
            {
                fxPrim->work0 = 9999;
            }
        }
    }

    primPool->nextPrim = (unsigned long *)prim;

    FX_DrawAllGeneralEffects(wcTransform, gameTrackerX.vertexPool, primPool, ot);
}

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_SimpleQuadSetup);

void FX_WaterRingProcess(FX_PRIM *fxPrim, FXTracker *fxTracker)
{
    fxPrim->v0.x -= 8;
    fxPrim->v0.y -= 8;
    fxPrim->v1.x += 8;
    fxPrim->v1.y -= 8;
    fxPrim->v2.x -= 8;
    fxPrim->v2.y += 8;
    fxPrim->v3.x += 8;
    fxPrim->v3.y += 8;

    FX_StandardFXPrimProcess(fxPrim, fxTracker);
}

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_WaterBubbleProcess);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_Sprite_Insert);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_GetTextureObject);

void FX_MakeWaterBubble(struct _SVector *position, struct _SVector *vel, struct _SVector *accl, long splashZ, struct __BubbleParams *BP)
{
    struct Object *waterfx; // $s3
    struct _FX_PRIM *fxPrim; // $s1

    waterfx = (struct Object *)objectAccess[3].object;

    if (waterfx != NULL)
    {
        fxPrim = FX_GetPrim(gFXT);

        if (fxPrim != NULL)
        {
            FX_DFacadeParticleSetup(fxPrim, (SVECTOR *)position, 12, 12, 0x2C000000, (SVECTOR *)vel, (SVECTOR *)accl, gFXT, (short)splashZ);

            fxPrim->texture = FX_GetTextureObject(waterfx, 2, rand() % (BP->UniqueBubbles - 1));
            fxPrim->flags |= 0x1;
            fxPrim->color = ((fxPrim->texture->color & 0x03FFFFFF) | 0x2C000000);
            fxPrim->process = &FX_WaterBubbleProcess;

            fxPrim->work0 = 0;
            fxPrim->work1 = (BP->MaxSpeed + (rand() % BP->MaxSpeedRange));
            fxPrim->work2 = (BP->ScaleRate + (rand() % BP->ScaleRateRange));
            fxPrim->work3 = (BP->StartScale + (rand() % BP->StartScaleRange));

            fxPrim->duo.phys.xAccl = BP->DisperseFrames;
            fxPrim->duo.phys.yAccl = BP->KillScale;

            fxPrim->v1.y = BP->MinSplashSize;

            FX_Sprite_Insert(&gFXT->usedPrimListSprite, fxPrim);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_DrawScreenPoly);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_SetupPolyGT4);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_MakeWarpArrow);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_MakeMannaIcon);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_MakeGlyphIcon);

void FX_SoulDustProcess(struct _FX_PRIM *fxPrim, struct _FXTracker *fxTracker)
{
    MATRIX *swTransform;
    struct _Position position;
    long color;
    long black;
    int fade;

    if ((fxPrim->work1 < 32) != 0)
    {
        FX_Die(fxPrim, fxTracker);
        return;
    }
    position = fxPrim->position;
    swTransform = fxPrim->duo.flame.segment + fxPrim->duo.flame.parent->matrix;
    fxPrim->position.x = (short)swTransform->t[0];
    fxPrim->position.y = (short)swTransform->t[1];
    fxPrim->position.z = (short)swTransform->t[2];
    fxPrim->v1.x += fxPrim->work2;
    fxPrim->position.x += (rcos(fxPrim->v1.x) * fxPrim->work1) / 4096;
    fxPrim->position.y += (rsin(fxPrim->v1.x) * fxPrim->work1) / 4096;
    fxPrim->position.z += (rcos(fxPrim->v1.y) * fxPrim->work0) / 4096;
    black = 0;
    color = 0x60FF60;
    fxPrim->work1 -= fxPrim->v2.x;
    fxPrim->v0.y -= 144;
    fxPrim->v1.y += 64;
    fade = fxPrim->v0.y;
    if ((fade < 0) != 0)
    {
        fade = 0;
    }
    LoadAverageCol((u_char *)&color, (u_char *)&black, 4096 - fade, fade, (u_char *)&fxPrim->color);
    fxPrim->color = ((fxPrim->color & 0xFFFFFF) | 0x2E000000);
}

void FX_MakeSoulDust(struct _Instance *instance, short segment)
{
    struct _FX_PRIM *fxPrim;
    SVECTOR location;
    struct Object *particle;

    if ((rand() & 0xFF) > 64)
    {
        return;
    }

    particle = (struct Object *)objectAccess[10].object;

    if (particle == NULL)
    {
        return;
    }

    fxPrim = FX_GetPrim(gFXT);

    if (fxPrim != NULL)
    {
        location.vx = 0;
        location.vy = 0;

        location.vz = (short)instance->matrix[1].t[2] + (rand() % 512) - 256;

        FX_DFacadeParticleSetup(fxPrim, &location, 25, 25, 0x2E000000, NULL, NULL, gFXT, 8);

        fxPrim->flags |= 0x2001;

        fxPrim->texture = FX_GetTextureObject(particle, 0, 0);

        fxPrim->v0.y = 4096;
        fxPrim->process = &FX_SoulDustProcess;
        fxPrim->color = 0x2E000000;
        fxPrim->v1.y = 0;
        fxPrim->duo.flame.parent = instance;
        fxPrim->duo.flame.segment = segment;

        fxPrim->work0 = (rand() % 320) - 160;

        fxPrim->work1 = (rand() & 63) + 320;

        fxPrim->v1.x = (rand() % 896) + 128;

        fxPrim->work2 = ((fxPrim->v1.x * 65536) >> 16) / 16;
        fxPrim->v2.x = fxPrim->work1 / 16;

        if ((rand() & 0x1))
        {
            fxPrim->v1.x = -fxPrim->v1.x;
            fxPrim->work2 = -fxPrim->work2;
        }

        fxPrim->v1.x = (instance->rotation.z - 1024) - fxPrim->v1.x;

        FX_SoulDustProcess(fxPrim, gFXT);

        FX_Sprite_Insert(&gFXT->usedPrimListSprite, fxPrim);
    }
}

void FX_WaterTrailProcess(FX_PRIM *fxPrim, FXTracker *fxTracker)
{
    FX_StandardProcess(fxPrim, fxTracker);

    fxPrim->v0.x = (fxPrim->v0.x * 7) >> 3;
    fxPrim->v0.y = (fxPrim->v0.y * 7) >> 3;
    fxPrim->v0.z = (fxPrim->v0.z * 7) >> 3;

    fxPrim->v1.x = (fxPrim->v1.x * 7) >> 3;
    fxPrim->v1.y = (fxPrim->v1.y * 7) >> 3;
    fxPrim->v1.z = (fxPrim->v1.z * 7) >> 3;

    fxPrim->v2.x = (fxPrim->v2.x * 7) >> 3;
    fxPrim->v2.y = (fxPrim->v2.y * 7) >> 3;
    fxPrim->v2.z = (fxPrim->v2.z * 7) >> 3;

    fxPrim->v3.x = (fxPrim->v3.x * 7) >> 3;
    fxPrim->v3.y = (fxPrim->v3.y * 7) >> 3;
    fxPrim->v3.z = (fxPrim->v3.z * 7) >> 3;
}

void FX_SimpleQuadSetup(struct _FX_PRIM *fxPrim, void (*fxProcess)(), struct _FX_MATRIX *fxMatrix, struct _Instance *instance, struct _MFace *mface, struct _MVertex *vertexList, struct SVECTOR *center, struct SVECTOR *vel, struct SVECTOR *accl, struct _FXTracker *fxTracker, int timeToLive);
void FX_MakeWaterTrail(struct _Instance *instance, int depth)
{
    struct Object *waterfx;
    struct _Model *wxtrail;
    struct _SVector position;
    int zvel;

    if ((instance->matrix != NULL) && (instance->oldMatrix != NULL) && (gameTrackerX.gameData.asmData.MorphType != 1) && (gameTrackerX.gameData.asmData.MorphTime == 1000))
    {
        position.x = (short)instance->matrix[1].t[0];
        position.y = (short)instance->matrix[1].t[1];
        position.z = instance->splitPoint.z;

        waterfx = (struct Object *)objectAccess[3].object;

        zvel = instance->matrix[1].t[2] - instance->oldMatrix[1].t[2];

        if (waterfx != NULL)
        {
            waterfx->oflags2 |= 0x20000000;

            wxtrail = waterfx->modelList[1];

            FX_BuildSingleFaceWithModel(wxtrail, wxtrail->faceList, (SVECTOR *)&position, NULL, NULL, gFXT, &FX_SimpleQuadSetup, &FX_WaterTrailProcess, 8);
        }

        if (abs(zvel) >= 21)
        {
            int n;
            int deg;
            struct _SVector vel;
            struct _SVector accel;
            struct _SVector startpos;

            accel.y = 0;
            accel.x = 0;
            accel.z = -2;

            startpos.z = position.z;

            for (n = 0; n < 8; n++)
            {
                int sinVal;
                int cosVal;
                int spd;

                deg = rand() & 0xFFF;

                cosVal = rcos(deg);

                sinVal = rsin(deg);

                spd = (rand() & 0x3) + 11;

                vel.x = (cosVal * spd) / 4096;
                vel.y = (sinVal * spd) / 4096;
                vel.z = 18;

                startpos.x = position.x + (cosVal / 128);
                startpos.y = position.y + (sinVal / 128);

                FX_Dot(&startpos, &vel, &accel, 0, 0x404040, 24, 20, 0);
            }

            INSTANCE_Post(gameTrackerX.playerInstance, 0x40024, 0);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_StartRibbon);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_RibbonProcess);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_ConstrictProcess);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_StartConstrict);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_EndConstrict);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_SubDividePrim);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_ContinueRibbon);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_StandardFXPrimProcess);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_AttachedParticlePrimProcess);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_FlamePrimProcess);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_DFacadeParticleSetup);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_Dot);

void FX_Blood(SVector *location, SVector *input_vel, SVector *accel, int amount, long color, long size)
{
    SVector vel;
    int i;

    for (i = amount; i != 0; i--)
    {
        vel.x = ((input_vel->x * i) / 128) + ((rand() & 0x7)) - 4;
        vel.y = ((input_vel->y * i) / 128) + ((rand() & 0x7)) - 4;
        vel.z = ((input_vel->z * i) / 128) + ((rand() & 0x7)) - 4;

        FX_Dot(location, &vel, accel, 0, color, size * 2, 16, 1);
    }
}

void FX_Blood2(SVector *location, SVector *input_vel, SVector *accel, int amount, long color, long dummyCrapShouldRemove)
{
    (void)dummyCrapShouldRemove;
    FX_Blood(location, input_vel, accel, amount, color, 4);
}

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_Blood_Impale);

FXParticle *FX_BloodCone(Instance *instance, short startSegment, long time)
{
    FXParticle *currentParticle;
    Object *particle;

    particle = (Object *)objectAccess[10].object;

    if (particle == NULL)
    {
        return NULL;
    }

    currentParticle = FX_GetParticle(instance, startSegment);

    if (currentParticle != NULL)
    {
        currentParticle->size = 14;

        currentParticle->texture = FX_GetTextureObject(particle, 0, 1);

        currentParticle->birthRadius = 20;

        currentParticle->direction.x = 256;
        currentParticle->direction.y = 256;
        currentParticle->direction.z = 256;

        currentParticle->acceleration.x = 0;
        currentParticle->acceleration.y = 0;
        currentParticle->acceleration.z = -2;

        currentParticle->numberBirthParticles = 10;

        currentParticle->startColor = 0x021800FF;
        currentParticle->endColor = 0;

        currentParticle->lifeTime = time;
        currentParticle->primLifeTime = 10;

        FX_InsertGeneralEffect(currentParticle);
    }

    return currentParticle;
}

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_GetTorchParticle);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_TorchFlame);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_GetMorphFadeVal);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_ConvertCamPersToWorld);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_GetRandomScreenPt);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_ProcessSnow);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_ContinueSnow);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_UpdateWind);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_ProcessRain);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_ContinueRain);

void FX_MakeSpark(Instance *instance, Model *model, int segment)
{
    FXParticle *currentParticle;
    Object *particle;

    (void)model;

    particle = (Object *)objectAccess[10].object;

    if (particle != NULL)
    {
        currentParticle = FX_GetParticle(instance, segment);

        if (currentParticle != NULL)
        {
            currentParticle->size = 48;

            currentParticle->birthRadius = 50;

            currentParticle->direction.x = 128;
            currentParticle->direction.y = 128;
            currentParticle->direction.z = 160;

            currentParticle->acceleration.x = 0;
            currentParticle->acceleration.y = 0;
            currentParticle->acceleration.z = 1;

            currentParticle->texture = FX_GetTextureObject(particle, 0, 0);

            currentParticle->numberBirthParticles = 2;

            currentParticle->lifeTime = 2;

            currentParticle->startColor = 0x02004960;
            currentParticle->endColor = 0;

            currentParticle->primLifeTime = 4;

            FX_InsertGeneralEffect(currentParticle);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_ContinueParticle);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_UpdraftPrimModify);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_MakeParticleTexFX);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_MakeHitFX);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_ContinueLightning);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_SetReaverInstance);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_SoulReaverBlade);

void FX_ReaverBladeInit()
{
}

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_SoulReaverWinding);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_UpdateInstanceWaterSplit);

void FX_GetPlaneEquation(SVector *normal, SVector *poPlane, PlaneConstants *plane)
{
    plane->a = normal->x;
    plane->b = normal->y;
    plane->c = normal->z;

    plane->d = -(((plane->a * poPlane->x) + (plane->b * poPlane->y) + (plane->c * poPlane->z)) >> 12);
}

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_DoInstancePowerRing);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_UpdatePowerRing);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_UpdateInstanceSplitRing);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_UpdateGlowEffect);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_InsertGeneralEffect);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_DeleteGeneralEffect);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_DoInstanceOneSegmentGlow);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_SetGlowFades);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_DoInstanceTwoSegmentGlow);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_DoInstanceManySegmentGlow);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_DoInstanceOneSegmentGlowWithTime);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_StopAllGlowEffects);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_StopGlowEffect);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_DrawLightning);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_DrawAllGeneralEffects);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_ContinueBlastRing);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_DoBlastRing);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_DrawBlastring);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_ContinueFlash);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_DrawFlash);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_RelocateGeneric);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_StartGenericParticle);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_StartGenericRibbon);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_StartGenericGlow);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_CreateLightning);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_SetLightingPos);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_StartGenericLightning);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_StartGenericBlastring);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_StartGenericFlash);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_GetHealthColor);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_Start_Snow);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_Start_Rain);

void FX_StartLightbeam(Instance *instance, int startSeg, int endSeg, int color_num)
{
    Object *particle;
    FXLightBeam *beam;
    GenericFXObject *GFXO;
    long color;

    particle = (Object *)objectAccess[10].object;

    if (particle != NULL)
    {
        GFXO = (GenericFXObject *)particle->data;

        color = GFXO->ColorList[color_num];

        beam = (FXLightBeam *)MEMPACK_Malloc(sizeof(FXLightBeam), 13);

        if (beam != NULL)
        {
            beam->effectType = 133;

            beam->instance = instance;

            beam->continue_process = NULL;

            beam->type = 0;

            beam->lifeTime = -1;

            beam->startSeg = startSeg;
            beam->endSeg = endSeg;

            beam->color = color;

            FX_InsertGeneralEffect(beam);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_StartInstanceEffect);

void FX_EndInstanceEffects(Instance *instance)
{
    FXGeneralEffect *currentEffect;
    FXGeneralEffect *nextEffect;

    currentEffect = FX_GeneralEffectTracker;

    while (currentEffect != NULL)
    {
        nextEffect = (FXGeneralEffect *)currentEffect->next;

        if (currentEffect->instance == instance)
        {
            FX_DeleteGeneralEffect(currentEffect);
        }

        currentEffect = nextEffect;
    }
}

void FX_EndInstanceParticleEffects(Instance *instance)
{
    FXGeneralEffect *currentEffect;
    FXGeneralEffect *nextEffect;

    currentEffect = FX_GeneralEffectTracker;

    while (currentEffect != NULL)
    {
        nextEffect = (FXGeneralEffect *)currentEffect->next;

        if ((currentEffect->instance == instance) && (currentEffect->effectType == 1))
        {
            FX_DeleteGeneralEffect(currentEffect);
        }

        currentEffect = nextEffect;
    }
}

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_GetSpiralPoint);

void FX_GetLinePoint(int radius, int next_radius, int deg, int next_deg, int *pntx, int *pnty, int part)
{
    int x1;
    int y1;
    int x2;
    int y2;

    FX_GetSpiralPoint(radius, deg, &x1, &y1);
    FX_GetSpiralPoint(next_radius, next_deg, &x2, &y2);

    *pntx = x1 + (((x2 - x1) * part) / 4096);
    *pnty = y1 + (((y2 - y1) * part) / 4096);
}

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_CalcSpiral);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_Spiral);

void FX_Health_Spiral(int number, int current_health, int max_health)
{
    int degchange;

    if (number != 0)
    {
        degchange = Spiral_Mod;

        if (degchange >= 2)
        {
            Spiral_Mod--;

            FX_CalcSpiral(128);
            return;
        }
    }
    else
    {
        degchange = Spiral_Mod;

        if (degchange < 6)
        {
            Spiral_Mod++;

            Spiral_Number = 0;

            FX_CalcSpiral(128);
            return;
        }
    }

    switch (number)
    {
    case 0:
    case 1:
    default:
        degchange = 128;
        break;
    case 2:
        degchange = 160;
        break;
    case 3:
        degchange = 192;
        break;
    case 4:
        degchange = 224;
        break;
    }

    if ((degchange != Spiral_Degrees) || (Spiral_Number != number))
    {
        Spiral_Number = number;

        if (Spiral_Degrees < degchange)
        {
            Spiral_Degrees += 4;

            if (degchange < Spiral_Degrees)
            {
                Spiral_Degrees = degchange;
            }
        }
        else
        {
            if (degchange < Spiral_Degrees)
            {
                Spiral_Degrees -= 4;

                if (Spiral_Degrees < degchange)
                {
                    Spiral_Degrees = degchange;
                }
            }
        }

        FX_CalcSpiral(Spiral_Degrees);
    }

    Spiral_Current = current_health;

    Spiral_Max = max_health;
}

void FX_Spiral_Init()
{
    Spiral_Max = 100000;

    Spiral_Current = 100000;

    Spiral_Number = 1;

    Spiral_Degrees = 128;

    FX_CalcSpiral(128);
}

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_DrawModel);

INCLUDE_ASM("asm/nonmatchings/Game/FX", fx_calc_points);

INCLUDE_ASM("asm/nonmatchings/Game/FX", fx_get_startz);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_DrawRing);

INCLUDE_ASM("asm/nonmatchings/Game/FX", fx_setTex);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_DrawRing2);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_DrawFField);

FXForceFieldEffect *FX_StartFField(Instance *instance, int size, Position *offset, int size_diff, int size_change, int deg_change, long color)
{
    FXForceFieldEffect *field;

    field = (FXForceFieldEffect *)MEMPACK_Malloc(sizeof(FXForceFieldEffect), 13);

    if (field != NULL)
    {
        field->effectType = 134;
        field->instance = instance;
        field->type = 0;
        field->lifeTime = -1;
        field->continue_process = NULL;
        field->size = size;

        COPY_SVEC(SVector, &field->offset, Position, offset);

        field->size_diff = size_diff;
        field->size_change = size_change;
        field->deg = 0;
        field->deg_change = deg_change;
        field->start_fade = 4096;
        field->end_fade = 0;
        field->color = color;

        FX_InsertGeneralEffect(field);
    }

    return field;
}

void FX_EndFField(Instance *instance)
{
    FXGeneralEffect *currentEffect;
    FXForceFieldEffect *forceField; // not from decls.h

    currentEffect = FX_GeneralEffectTracker;

    while (currentEffect != NULL)
    {
        if ((currentEffect->instance == instance) && (currentEffect->effectType == 134))
        {
            forceField = (FXForceFieldEffect *)currentEffect;

            if (forceField->start_fade != 0)
            {
                forceField->end_fade = 4096 - forceField->start_fade;
                forceField->start_fade = 0;

                if (forceField->end_fade == 0)
                {
                    forceField->end_fade = 1;
                }
            }
            else
            {
                forceField->end_fade = 4096;
            }
        }

        currentEffect = (FXGeneralEffect *)currentEffect->next;
    }
}

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_Draw_Glowing_Line);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_Lightning);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_LightHouse);

void FX_StartPassthruFX(Instance *instance, SVector *normal, SVector *point)
{
    long color;

    instance->halvePlane.a = normal->x;
    instance->halvePlane.b = normal->y;
    instance->halvePlane.c = normal->z;

    color = 0x20FF40;

    instance->halvePlane.d = -(((normal->x * point->x) + (normal->y * point->y) + (normal->z * point->z)) >> 12);

    FX_DoInstancePowerRing(instance, 8400, &color, 0, 2);
    FX_DoInstancePowerRing(instance, 8400, &color, 0, 1);
}

void FX_EndPassthruFX(Instance *instance)
{
    FX_EndInstanceEffects(instance);
}
