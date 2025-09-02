#include "Game/FX.h"
#include "Game/MATH3D.h"
#include "Game/MEMPACK.h"
#include "Game/DRAW.h"
#include "Game/OBTABLE.h"
#include "Game/LIST.h"
#include "Game/GAMELOOP.h"
#include "Game/STREAM.h"
#include "Game/LIGHT3D.h"
#include "Game/INSTANCE.h"
#include "Game/SOUND.h"
#include "Game/CAMERA.h"
#include "Game/COLLIDE.h"
#include "Game/PIPE3D.h"
#include "Game/REAVER.h"
#include "Game/GAMEPAD.h"
#include "Game/GLYPH.h"
#include "Game/PHYSOBS.h"

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

STATIC short windx;

STATIC short windy;

STATIC short wind_deg;

STATIC short wind_speed;

STATIC FXRibbon *FX_ConstrictRibbon;

STATIC short FX_ConstrictStage;

STATIC Instance *FX_ConstrictInstance;

long FX_ColorArray[6];

static inline long FX_GetColor(ObjectEffect *effect, int i)
{
    return FX_ColorArray[effect->modifierList[i]];
}

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

struct _FX_PRIM *FX_BuildSingleFaceWithModel(struct _Model *model, struct _MFace *mface, struct SVECTOR *center, struct SVECTOR *vel, struct SVECTOR *accl, struct _FXTracker *fxTracker, void (*fxSetup)(), void (*fxProcess)(), short timeToLive)
{
    return _FX_BuildSingleFaceWithModel(model, mface, center, vel, accl, fxTracker, (void *)fxSetup, fxProcess, 0, timeToLive);
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

void FX_UpdateTexturePointers(FX_PRIM *fxPrim, Object *oldObject, long sizeOfObject, long offset)
{
    for (; fxPrim != NULL; fxPrim = (FX_PRIM *)fxPrim->node.next)
    {
        if (((fxPrim->flags & 0x1)) && (fxPrim->texture >= (TextureMT3 *)oldObject) && (((char *)oldObject + sizeOfObject) >= (char *)fxPrim->texture))
        {
            fxPrim->texture = fxPrim->texture != NULL ? (TextureMT3 *)((char *)fxPrim->texture + offset) : NULL;
        }
    }
}

void FX_RelocateFXPointers(struct Object *oldObject, struct Object *newObject, long sizeOfObject)
{
    struct _FXTracker *fxTracker; // $s0
    struct _FXGeneralEffect *currentEffect; // $a0
    long offset; // $s1
    struct _FXParticle *particle; // not from SYMDUMP

    fxTracker = gFXT;

    offset = (intptr_t)newObject - (intptr_t)oldObject;

    FX_UpdateTexturePointers((struct _FX_PRIM *)fxTracker->usedPrimList.next, oldObject, (int)sizeOfObject, (int)offset);
    FX_UpdateTexturePointers((struct _FX_PRIM *)fxTracker->usedPrimListSprite.next, oldObject, (int)sizeOfObject, (int)offset);

    currentEffect = FX_GeneralEffectTracker;

    while (currentEffect != NULL)
    {
        if (currentEffect->effectType == 1)
        {
            particle = (struct _FXParticle *)currentEffect;
            if (particle->texture != NULL && IN_BOUNDS(particle->texture, oldObject, (intptr_t)oldObject + sizeOfObject))
            {
                particle->texture = (struct TextureMT3 *)OFFSET_DATA(particle->texture, offset);
            }
        }

        currentEffect = (struct _FXGeneralEffect *)currentEffect->next;
    }
}

void FX_ProcessList(FXTracker *fxTracker)
{
    FX_PRIM *fxPrim;
    FX_PRIM *nextFXPrim;
    FX_MATRIX *fxMatrix;
    FX_MATRIX *nextFXMatrix;

    FX_TimeCount += gameTrackerX.timeMult;

    FX_Frames = FX_TimeCount / 4096;

    FX_TimeCount -= FX_Frames << 12;

    for (fxMatrix = (FX_MATRIX *)fxTracker->usedMatrixList.next; fxMatrix != NULL; fxMatrix = (FX_MATRIX *)fxMatrix->node.next)
    {
        if ((fxMatrix->flags & 0x4))
        {
            fxMatrix->flags |= 0x1;
        }
        else
        {
            fxMatrix->flags &= ~0x1;
        }

        fxMatrix->flags &= ~0x2;
    }

    for (fxPrim = (FX_PRIM *)fxTracker->usedPrimList.next; fxPrim != NULL; fxPrim = nextFXPrim)
    {
        nextFXPrim = (FX_PRIM *)fxPrim->node.next;

        if (fxPrim->matrix != NULL)
        {
            fxPrim->matrix->flags |= 0x1;
        }

        if (fxPrim->process != NULL)
        {
            fxPrim->process(fxPrim, fxTracker);
        }
    }

    for (fxPrim = (FX_PRIM *)fxTracker->usedPrimListSprite.next; fxPrim != NULL; fxPrim = nextFXPrim)
    {
        nextFXPrim = (FX_PRIM *)fxPrim->node.next;

        if (fxPrim->process != NULL)
        {
            fxPrim->process(fxPrim, fxTracker);
        }
    }

    for (fxMatrix = (FX_MATRIX *)fxTracker->usedMatrixList.next; fxMatrix != NULL; fxMatrix = nextFXMatrix)
    {
        nextFXMatrix = (FX_MATRIX *)fxMatrix->node.next;

        if (!(fxMatrix->flags & 0x1))
        {
            LIST_DeleteFunc((NodeType *)fxMatrix);

            LIST_InsertFunc(&fxTracker->freeMatrixList, (NodeType *)fxMatrix);
        }
    }

    {
        FXGeneralEffect *currentEffect;
        FXGeneralEffect *nextEffect;

        for (currentEffect = FX_GeneralEffectTracker; currentEffect != NULL; currentEffect = nextEffect)
        {
            nextEffect = (FXGeneralEffect *)currentEffect->next;

            if (currentEffect->continue_process != NULL)
            {
                currentEffect->continue_process(currentEffect, fxTracker);
            }
        }

        if (FX_ConstrictStage == 1)
        {
            FX_ConstrictStage = 0;
        }

        if (snow_amount != 0)
        {
            FX_ContinueSnow(fxTracker);
        }

        if (rain_amount != 0)
        {
            FX_ContinueRain(fxTracker);
        }

        FX_UpdateWind(fxTracker);
    }
}

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

void FX_SimpleQuadSetup(FX_PRIM *fxPrim, void (*fxProcess)(), FX_MATRIX *fxMatrix, Instance *instance, MFace *mface, MVertex *vertexList, SVECTOR *center, SVECTOR *vel, SVECTOR *accl, FXTracker *fxTracker, short timeToLive)
{
    MVertex *vertex1;
    MVertex *vertex2;
    MVertex *vertex3;

    (void)instance;
    (void)fxTracker;

    vertex1 = (MVertex *)&vertexList[1].vertex;
    vertex2 = (MVertex *)&vertexList[3].vertex;
    vertex3 = (MVertex *)&vertexList[2].vertex;

    MATH3D_SetUnityMatrix(&fxMatrix->lwTransform);

    COPY_SVEC(Position, &fxPrim->position, Position, (Position *)center);

    COPY_SVEC(Position, &fxPrim->v0, Position, (Position *)&vertexList->vertex);
    COPY_SVEC(Position, &fxPrim->v1, Position, (Position *)&vertex1->vertex);
    COPY_SVEC(Position, &fxPrim->v2, Position, (Position *)&vertex2->vertex);
    COPY_SVEC(Position, &fxPrim->v3, Position, (Position *)&vertex3->vertex);

    fxPrim->flags |= 0x8;

    if ((mface->flags & 0x2))
    {
        fxPrim->flags |= 0x9;

        fxPrim->texture = (TextureMT3 *)mface->color;

        fxPrim->color = (fxPrim->texture->color & 0x3FFFFFF) | 0x2C000000;
    }
    else
    {
        fxPrim->flags &= ~0x1;

        fxPrim->color = (mface->color & 0x3FFFFFF) | 0x28000000;
    }

    if (fxProcess != NULL)
    {
        fxPrim->process = fxProcess;
    }
    else
    {
        fxPrim->process = FX_StandardProcess;
    }

    fxPrim->matrix = fxMatrix;

    if (vel != NULL)
    {
        fxPrim->duo.phys.xVel = vel->vx;
        fxPrim->duo.phys.yVel = vel->vy;
        fxPrim->duo.phys.zVel = vel->vz;
    }
    else
    {
        fxPrim->duo.phys.xVel = 0;
        fxPrim->duo.phys.yVel = 0;
        fxPrim->duo.phys.zVel = 0;
    }

    if (accl != NULL)
    {
        fxPrim->duo.phys.xAccl = accl->vx;
        fxPrim->duo.phys.yAccl = accl->vy;
        fxPrim->duo.phys.zAccl = accl->vz;
    }
    else
    {
        fxPrim->duo.phys.xAccl = 0;
        fxPrim->duo.phys.yAccl = 0;
        fxPrim->duo.phys.zAccl = 0;
    }

    fxPrim->timeToLive = timeToLive;

    fxPrim->flags |= 0x4000;
}

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

void FX_WaterBubbleProcess(FX_PRIM *fxPrim, FXTracker *fxTracker)
{
    FX_PRIM *temp;

    if (fxPrim->duo.phys.zVel < fxPrim->work1)
    {
        fxPrim->duo.phys.zVel += fxPrim->duo.phys.zAccl;
    }

    if (fxPrim->work0 < fxPrim->duo.phys.xAccl)
    {
        fxPrim->position.x += fxPrim->duo.phys.xVel;
        fxPrim->position.y += fxPrim->duo.phys.yVel;
    }

    fxPrim->position.z += fxPrim->duo.phys.zVel;

    fxPrim->work0++;

    fxPrim->v0.y = fxPrim->v2.y = fxPrim->work3 - (fxPrim->work0 * fxPrim->work2);

    if (fxPrim->v0.y >= fxPrim->duo.phys.yAccl)
    {
        if (fxPrim->position.z > fxPrim->timeToLive)
        {
            Object *waterfx;
            Model *wxring;

            waterfx = objectAccess[3].object;

            if (waterfx != NULL)
            {
                if (fxPrim->v1.y < fxPrim->v0.y)
                {
                    wxring = *waterfx->modelList;

                    temp = FX_GetPrim(gFXT);

                    if (temp != NULL)
                    {
                        temp->position = fxPrim->position;

                        temp->v0.x = temp->v2.x = temp->position.x - 8;
                        temp->v1.x = temp->v3.x = temp->position.x + 8;

                        temp->v0.y = temp->v1.y = temp->position.y - 8;
                        temp->v2.y = temp->v3.y = temp->position.y + 8;

                        temp->v0.z = temp->v1.z = temp->v2.z = temp->v3.z = temp->position.z;

                        temp->texture = (TextureMT3 *)wxring->faceList->color;

                        temp->process = FX_WaterRingProcess;

                        temp->timeToLive = 16;

                        temp->flags |= 0x50009;

                        temp->color = 0x2EFFFFFF;
                        temp->startColor = 0xFFFFFF;
                        temp->endColor = 0;

                        temp->fadeValue[0] = 0;
                        temp->fadeStep = 256;

                        LIST_InsertFunc(&fxTracker->usedPrimList, (NodeType *)temp);
                    }
                }
            }

            FX_Die(fxPrim, fxTracker);
        }
    }
    else
    {
        FX_Die(fxPrim, fxTracker);
    }
}

void FX_Sprite_Insert(NodeType *list, FX_PRIM *fxPrim)
{
    LIST_InsertFunc(list, &fxPrim->node);

    if (FX_LastUsedPrim == NULL)
    {
        FX_LastUsedPrim = fxPrim;
    }
}

TextureMT3 *FX_GetTextureObject(Object *object, int modelnum, int texnum)
{
    Model *model;
    TextureMT3 *texture;

    object->oflags2 |= 0x20000000;

    model = object->modelList[modelnum];

    texture = (TextureMT3 *)&((char *)(((FX_PRIM *)model->faceList)->process))[(texnum * 16) + 16];

    return texture;
}

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

void FX_MakeWarpArrow(int x, int y, int xsize, int ysize, int fade)
{
    Object *particle;
    long color;
    POLY_GT4 *poly;

    particle = (Object *)objectAccess[10].object;

    if (particle != NULL)
    {
        if (fade < 0)
        {
            fade = 0;
        }

        color = fade / 32;

        color = color | (color << 8) | (color << 16);

        poly = FX_SetupPolyGT4(x + xsize, y, x, y + ysize, 3, FX_GetTextureObject(particle, 0, 9), color, color, color, color);

        if (poly != NULL)
        {
            poly->code |= 0x2;
        }
    }
}

void FX_MakeMannaIcon(int x, int y, int xsize, int ysize)
{
    // Object *manna; // unused
    TextureMT3 *texture;
    POLY_GT4 *poly;
    int n;
    int newx;
    int newy;
    int sizex;
    int sizey;
    unsigned long color;

    if (objectAccess[20].object != NULL)
    {
        texture = FX_GetTextureObject((Object *)objectAccess[20].object, 0, 7);

        poly = FX_SetupPolyGT4(x, y, x + xsize, y + ysize, 3, texture, 0x2962828, 0x2962828, 0x2962828, 0x2962828);

        if (poly != NULL)
        {
            poly->tpage |= 0x20;
        }

        if (objectAccess[10].object != NULL)
        {
            texture = FX_GetTextureObject((Object *)objectAccess[10].object, 0, 2);

            for (n = 0; n < 5; n++)
            {
                switch (n)
                {
                default:
                case 0:
                    newx = x + 33;
                    newy = y - 1;

                    sizex = 14;
                    sizey = 12;

                    color = 0x24B3232;
                    break;
                case 1:
                    newx = x + 3;
                    newy = y + 5;

                    sizex = 14;
                    sizey = 12;

                    color = 0x2704B4B;
                    break;
                case 2:
                    newx = x + 13;
                    newy = y + 4;

                    sizex = 19;
                    sizey = 16;

                    color = 0x2966464;
                    break;
                case 3:
                    newx = x + 3;
                    newy = y + 16;

                    sizex = 19;
                    sizey = 16;

                    color = 0x24B3232;
                    break;
                case 4:
                    newx = x + 32;
                    newy = y + 13;

                    sizex = 19;
                    sizey = 16;

                    color = 0x2966464;
                    break;
                }

                FX_SetupPolyGT4(newx, newy, newx + sizex, newy + sizey, 3, texture, color, color, color, color);
            }
        }
    }
}

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
    (void)position;
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

void FX_MakeWaterTrail(struct _Instance *instance, int depth)
{
    struct Object *waterfx;
    struct _Model *wxtrail;
    struct _SVector position;
    int zvel;

    (void)depth;

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

            FX_BuildSingleFaceWithModel(wxtrail, wxtrail->faceList, (SVECTOR *)&position, NULL, NULL, gFXT, (void *)&FX_SimpleQuadSetup, &FX_WaterTrailProcess, 8);
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

FXRibbon *FX_StartRibbon(Instance *instance, short startSegment, short endSegment, short type, short ribbonLifeTime, short faceLifeTime, short startFadeValue, long startColor, long endColor)
{
    MATRIX *swTransform;
    FXRibbon *ribbon;
    int i;
    int number;

    number = (endSegment - startSegment) + 1;

    if (number < 2)
    {
        return NULL;
    }

    ribbon = (FXRibbon *)MEMPACK_Malloc(sizeof(FXRibbon), 13);

    if (ribbon == NULL)
    {
        return NULL;
    }

    ribbon->continue_process = FX_ContinueRibbon;

    ribbon->effectType = 0;

    ribbon->endIndex = 0;

    if (type == 1)
    {
        ribbon->numberVerts = number * 2;
    }
    else
    {
        ribbon->numberVerts = 4;
    }

    ribbon->vertexPool = (SVECTOR *)MEMPACK_Malloc(ribbon->numberVerts * 8, 13);

    if (ribbon->vertexPool == NULL)
    {
        MEMPACK_Free((char *)ribbon);

        return NULL;
    }

    ribbon->faceLifeTime = faceLifeTime;

    ribbon->startSegment = startSegment;
    ribbon->endSegment = endSegment;

    ribbon->instance = instance;

    ribbon->type = type;

    ribbon->lifeTime = ribbonLifeTime;

    ribbon->startColor = startColor;
    ribbon->endColor = endColor;

    ribbon->startFadeValue = startFadeValue;

    ribbon->colorStepValue = 4096 / faceLifeTime;

    ribbon->fadeStep = (4096 - startFadeValue) / ribbon->faceLifeTime;

    if (ribbon->type == 2)
    {
        ribbon->fadeStep = (ribbon->fadeStep * 6) / 8;
    }

    swTransform = instance->matrix;

    if (swTransform != NULL)
    {
        if (ribbon->type == 1)
        {
            swTransform = &swTransform[startSegment];

            for (i = 0; i < number; i++)
            {
                ribbon->vertexPool[i].vx = swTransform->t[0];
                ribbon->vertexPool[i].vy = swTransform->t[1];
                ribbon->vertexPool[i].vz = swTransform->t[2];

                ribbon->endIndex++;

                swTransform++;
            }
        }
        else
        {
            swTransform = &instance->matrix[startSegment];

            ribbon->vertexPool->vx = swTransform->t[0];
            ribbon->vertexPool->vy = swTransform->t[1];
            ribbon->vertexPool->vz = swTransform->t[2];

            swTransform = &instance->matrix[endSegment];

            ribbon->vertexPool[1].vx = swTransform->t[0];
            ribbon->vertexPool[1].vy = swTransform->t[1];
            ribbon->vertexPool[1].vz = swTransform->t[2];

            ribbon->endIndex = 2;
        }
    }
    else
    {
        ribbon->endIndex = -1;
    }

    FX_InsertGeneralEffect(ribbon);

    return ribbon;
}

void FX_RibbonProcess(FX_PRIM *fxPrim, FXTracker *fxTracker)
{
    int d;
    // long fade; // unused

    if (fxPrim->timeToLive > 0)
    {
        fxPrim->timeToLive--;
    }

    if (fxPrim->timeToLive == 0)
    {
        FX_Die(fxPrim, fxTracker);
        return;
    }

    for (d = 0; d < 4; d++)
    {
        fxPrim->fadeValue[d] += fxPrim->fadeStep;

        if (fxPrim->fadeValue[d] > 4096)
        {
            fxPrim->fadeValue[d] = 4096;
        }
    }

    if (fxPrim->startColor != fxPrim->endColor)
    {
        fxPrim->colorFadeValue += fxPrim->colorFadeStep;

        if (fxPrim->colorFadeValue > 4096)
        {
            fxPrim->colorFadeValue = 4096;
        }

        LoadAverageCol((unsigned char *)&fxPrim->startColor, (unsigned char *)&fxPrim->endColor, 4096 - fxPrim->colorFadeValue, fxPrim->colorFadeValue, (unsigned char *)&fxPrim->color);
    }
}

void FX_ConstrictProcess(FX_PRIM *fxPrim, FXTracker *fxTracker)
{
    if (FX_ConstrictStage == 1)
    {

        gameTrackerX.streamFlags |= 4;
        fxPrim->startColor = 0xFFFFFF;
        fxPrim->timeToLive = 0x28;
        fxPrim->fadeStep = 0x66;
        fxPrim->fadeValue[2] = fxPrim->fadeValue[3] = 0;
        fxPrim->fadeValue[0] = fxPrim->fadeValue[1] = 0;
        fxPrim->work0 = fxPrim->work1 = 0x380;
        fxPrim->work1 = 0;

        if (fxPrim->work2 != 0)
        {

            fxPrim->fadeValue[0] = fxPrim->fadeValue[1] = 0x1000;
            fxPrim->fadeValue[2] = fxPrim->fadeValue[3] = 0;
            fxPrim->work2 = ratan2(fxPrim->v2.y - FX_ConstrictPosition.y, fxPrim->v2.x - FX_ConstrictPosition.x);
            fxPrim->work3 = ratan2(fxPrim->v3.y - FX_ConstrictPosition.y, fxPrim->v3.x - FX_ConstrictPosition.x);
            fxPrim->v0.z = (fxPrim->v2.z + ((fxPrim->v0.z - fxPrim->v2.z) /4));
            fxPrim->v1.z = (fxPrim->v3.z + ((fxPrim->v1.z - fxPrim->v3.z) * 3));

        }
        else
        {

            fxPrim->fadeValue[0] = fxPrim->fadeValue[1] = 0;
            fxPrim->fadeValue[2] = fxPrim->fadeValue[3] = 0x1000;
            fxPrim->work2 = ratan2(fxPrim->v0.y - FX_ConstrictPosition.y, fxPrim->v0.x - FX_ConstrictPosition.x);
            fxPrim->work3 = ratan2(fxPrim->v1.y - FX_ConstrictPosition.y, fxPrim->v1.x - FX_ConstrictPosition.x);
            fxPrim->v2.z = (fxPrim->v0.z + ((fxPrim->v2.z - fxPrim->v0.z) / 4));
            fxPrim->v3.z = (fxPrim->v1.z + ((fxPrim->v3.z - fxPrim->v1.z) * 3));

        }

        fxPrim->v3.x = fxPrim->v1.x = FX_ConstrictPosition.x;
        fxPrim->v3.y = fxPrim->v1.y = FX_ConstrictPosition.y;

    }
    else if (fxPrim->work0 > 0 || fxPrim->work1 > 0)
    {

        fxPrim->work0 -= 0x28;
        fxPrim->work2 -= 0x40;
        fxPrim->v2.x = fxPrim->v0.x = FX_ConstrictPosition.x + ((fxPrim->work0 * rcos(fxPrim->work2)) >> 0xC);
        fxPrim->v2.y = fxPrim->v0.y = FX_ConstrictPosition.y + ((fxPrim->work0 * rsin(fxPrim->work2)) >> 0xC);
        gameTrackerX.streamFlags |= 4;

    }
    FX_RibbonProcess(fxPrim, fxTracker);
}

void FX_StartConstrict(Instance *instance, SVector *constrict_point, short startSegment, short endSegment)
{
    if (FX_ConstrictRibbon == NULL)
    {
        if (constrict_point != NULL)
        {
            FX_ConstrictPosition.x = constrict_point->x;
            FX_ConstrictPosition.y = constrict_point->y;
            FX_ConstrictPosition.z = instance->position.z + 256;

            FX_ConstrictPositionPtr = (Position *)constrict_point;
        }

        FX_ConstrictRibbon = FX_StartRibbon(instance, startSegment, endSegment, 2, -1, 40, 0, 0xE84040, 0xE84040);

        FX_ConstrictStage = 0;
    }
}

void FX_EndConstrict(int ConstrictEnemyFlag, Instance *instance)
{
    if (ConstrictEnemyFlag != 0)
    {
        FX_ConstrictStage = 1;

        FX_ConstrictInstance = instance;

        if (instance != NULL)
        {
            COPY_SVEC(Position, &FX_ConstrictPosition, Position, &instance->position);
        }
        else
        {
            COPY_SVEC(Position, &FX_ConstrictPosition, Position, FX_ConstrictPositionPtr);
        }
    }

    FX_DeleteGeneralEffect((FXGeneralEffect *)FX_ConstrictRibbon);

    FX_ConstrictRibbon = NULL;
}

void FX_SubDividePrim(FX_PRIM *fxPrim1, FX_PRIM *fxPrim2)
{

    SVECTOR mp0;
    SVECTOR mp1;

    LoadAverageShort12((SVECTOR *)&fxPrim1->v0, (SVECTOR *)&fxPrim2->v2, 0x800, 0x800, &mp0);
    LoadAverageShort12((SVECTOR *)&fxPrim1->v1, (SVECTOR *)&fxPrim2->v3, 0x800, 0x800, &mp1);

    fxPrim1->v2 = *(Position *)&mp0;
    fxPrim1->v3 = *(Position *)&mp1;
    fxPrim2->v0 = *(Position *)&mp0;
    fxPrim2->v1 = *(Position *)&mp1;

}

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_ContinueRibbon);

void FX_StandardFXPrimProcess(FX_PRIM *fxPrim, FXTracker *fxTracker)
{
    long flags;

    if (fxPrim->timeToLive > 0)
    {
        fxPrim->timeToLive--;
    }

    if (fxPrim->timeToLive == 0)
    {
        FX_Die(fxPrim, fxTracker);
        return;
    }

    flags = fxPrim->flags;

    if ((flags & 0x40000))
    {
        long start;
        long end;

        start = fxPrim->startColor;
        end = fxPrim->endColor;

        fxPrim->fadeValue[0] += fxPrim->fadeStep;

        if (fxPrim->fadeValue[0] > 4096)
        {
            fxPrim->fadeValue[0] = 4096;
        }

        gte_lddp(4096 - fxPrim->fadeValue[0]);
        gte_ldcv(&start);
        gte_ngpf12();

        gte_lddp(fxPrim->fadeValue[0]);
        gte_ldcv(&end);
        gte_ngpl12();

        gte_stcv(&fxPrim->color);

        if ((flags & 0x1))
        {
            fxPrim->color = (fxPrim->color & 0x3FFFFFF) | 0x2C000000;
        }
    }

    if ((flags & 0x2000))
    {
        int current_scale;

        current_scale = fxPrim->v0.y - fxPrim->work3;

        if (current_scale <= 0)
        {
            FX_Die(fxPrim, fxTracker);
            return;
        }

        fxPrim->v0.y = current_scale;
    }

    if ((flags & 0x20))
    {
        MATRIX *swTransform;

        swTransform = &fxPrim->duo.flame.segment[fxPrim->duo.flame.parent->matrix];

        fxPrim->position.x = swTransform->t[0];
        fxPrim->position.y = swTransform->t[1];
        fxPrim->position.z = swTransform->t[2];
    }
    else if (!(flags & 0x2))
    {
        fxPrim->duo.phys.xVel += fxPrim->duo.phys.xAccl;
        fxPrim->duo.phys.yVel += fxPrim->duo.phys.yAccl;
        fxPrim->duo.phys.zVel += fxPrim->duo.phys.zAccl;

        if ((flags & 0x1000000))
        {
            fxPrim->v0.x += fxPrim->duo.phys.xVel;
            fxPrim->v1.x += fxPrim->duo.phys.xVel;

            fxPrim->v0.y += fxPrim->duo.phys.yVel;
            fxPrim->v1.y += fxPrim->duo.phys.yVel;

            fxPrim->v0.z += fxPrim->duo.phys.zVel;
            fxPrim->v1.z += fxPrim->duo.phys.zVel;
        }
        else
        {
            fxPrim->position.x += fxPrim->duo.phys.xVel;
            fxPrim->position.y += fxPrim->duo.phys.yVel;
            fxPrim->position.z += fxPrim->duo.phys.zVel;
        }

        if (((flags & 0x100)) && (fxPrim->position.z <= fxPrim->work0))
        {
            fxPrim->position.z = fxPrim->work0;

            fxPrim->flags |= 0x2;
        }
    }

    if ((!(flags & 0x8000000)) && (fxPrim->matrix != NULL) && (!(fxPrim->matrix->flags & 0x2)))
    {
        fxPrim->matrix->flags |= 0x2;

        if ((flags & 0x80))
        {
            Rotation rot;

            rot.x = ((char *)&fxPrim->work2)[1] * 4;
            rot.y = ((char *)&fxPrim->work3)[0] * 4;
            rot.z = ((char *)&fxPrim->work3)[1] * 4;

            RotMatrixX(rot.x, &fxPrim->matrix->lwTransform);
            RotMatrixY(rot.y, &fxPrim->matrix->lwTransform);
            RotMatrixZ(rot.z, &fxPrim->matrix->lwTransform);
        }
    }
}

void FX_AttachedParticlePrimProcess(FX_PRIM *fxPrim, FXTracker *fxTracker)
{
    MATRIX *swTransform;
    MATRIX *swTransformOld;
    Instance *instance;

    instance = (Instance *)fxPrim->matrix;

    swTransform = &instance->matrix[fxPrim->work0];
    swTransformOld = &instance->oldMatrix[fxPrim->work0];

    if ((instance->matrix != NULL) && (instance->oldMatrix != NULL))
    {
        fxPrim->position.x += swTransform->t[0] - swTransformOld->t[0];
        fxPrim->position.y += swTransform->t[1] - swTransformOld->t[1];
        fxPrim->position.z += swTransform->t[2] - swTransformOld->t[2];
    }

    FX_StandardFXPrimProcess(fxPrim, fxTracker);
}

void FX_FlamePrimProcess(FX_PRIM *fxPrim, FXTracker *fxTracker)
{

    Instance *instance;
    MATRIX *swTransform;
    MATRIX *swTransformOld;
    SVector movement;
    int total;

    instance = (Instance *)fxPrim->matrix;

    if (instance->matrix == NULL || instance->oldMatrix == NULL) { return; }

    swTransform = &instance->matrix[fxPrim->work0];
    swTransformOld = &instance->oldMatrix[fxPrim->work0];

    movement.x = ((swTransform->t[0] - swTransformOld->t[0]) * 0x6E) / 128;
    movement.y = ((swTransform->t[1] - swTransformOld->t[1]) * 0x6E) / 128;
    movement.z = ((swTransform->t[2] - swTransformOld->t[2]) * 0x6E) / 128;

    total = abs(movement.x) + abs(movement.y) + abs(movement.z);

    if (total != 0)
    {
        fxPrim->position.x += movement.x;
        fxPrim->position.y += movement.y;
        fxPrim->position.z += movement.z;
    }

    if (total > 40)
    {
        fxPrim->duo.phys.zAccl = 0;
        fxPrim->v0.y -= (total * 3) / 2;

        if (fxPrim->v0.y >> 0x10)
        {
            fxPrim->v0.y = 0;
        }
    }
    else
    {
        fxPrim->duo.phys.zAccl = 1;
    }
    FX_StandardFXPrimProcess(fxPrim, fxTracker);
}

void FX_DFacadeParticleSetup(FX_PRIM *fxPrim, SVECTOR *center, short halveWidth, short halveHeight, long color, SVECTOR *vel, SVECTOR *accl, FXTracker *fxTracker, int timeToLive)
{
    short temp; // not from decls.h

    (void)fxTracker;

    COPY_SVEC(Position, &fxPrim->position, Position, (Position *)center);

    fxPrim->flags |= 0x8;

    fxPrim->v0.x = halveWidth;
    fxPrim->v0.y = 4096;
    fxPrim->v0.z = halveHeight;

    fxPrim->color = (color & 0x3FFFFFF) | 0x20000000;

    fxPrim->process = FX_StandardFXPrimProcess;

    temp = timeToLive;

    if (vel != NULL)
    {
        fxPrim->duo.phys.xVel = vel->vx;
        fxPrim->duo.phys.yVel = vel->vy;
        fxPrim->duo.phys.zVel = vel->vz;
    }
    else
    {
        fxPrim->duo.phys.xVel = 0;
        fxPrim->duo.phys.yVel = 0;
        fxPrim->duo.phys.zVel = 0;
    }

    if (accl != NULL)
    {
        fxPrim->duo.phys.xAccl = accl->vx;
        fxPrim->duo.phys.yAccl = accl->vy;
        fxPrim->duo.phys.zAccl = accl->vz;
    }
    else
    {
        fxPrim->duo.phys.xAccl = 0;
        fxPrim->duo.phys.yAccl = 0;
        fxPrim->duo.phys.zAccl = 0;
    }

    fxPrim->timeToLive = temp;
}

FX_PRIM *FX_Dot(SVector *location, SVector *vel, SVector *accel, int scale_speed, long color, long size, int lifetime, int texture_num)
{
    FX_PRIM *fxPrim;

    fxPrim = FX_GetPrim(gFXT);

    if (fxPrim != NULL)
    {
        if (texture_num >= 0)
        {
            FX_MakeParticleTexFX(fxPrim, location, NULL, 0, texture_num, vel, accel, color, size, lifetime);
        }
        else
        {
            FX_DFacadeParticleSetup(fxPrim, (SVECTOR *)location, size, size, color, (SVECTOR *)vel, (SVECTOR *)accel, gFXT, (short)lifetime);

            if (color != 0)
            {
                fxPrim->flags |= 0xC0000;
            }

            fxPrim->startColor = color;
            fxPrim->endColor = 0;

            fxPrim->fadeValue[3] = 0;
            fxPrim->fadeValue[2] = 0;
            fxPrim->fadeValue[1] = 0;
            fxPrim->fadeValue[0] = 0;

            fxPrim->fadeStep = 4096 / lifetime;
        }

        if (scale_speed != 0)
        {
            fxPrim->v0.y = 4096;

            fxPrim->work3 = scale_speed;

            fxPrim->flags |= 0x2000;
        }

        FX_Sprite_Insert(&gFXT->usedPrimListSprite, fxPrim);
    }

    return fxPrim;
}

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

void FX_Blood_Impale(Instance *locinst, short locseg, Instance *instance, short segment)
{

    int i;
    SVector location;
    SVector accel;
    SVector vel;
    SVector input_vel;

    input_vel.x = instance->matrix[segment].t[0] - instance->oldMatrix[segment].t[0];
    input_vel.y = instance->matrix[segment].t[1] - instance->oldMatrix[segment].t[1];
    input_vel.z = instance->matrix[segment].t[2] - instance->oldMatrix[segment].t[2];

    location.x = locinst->matrix[locseg].t[0];
    location.y = locinst->matrix[locseg].t[1];
    location.z = locinst->matrix[locseg].t[2];

    accel.x = 0;
    accel.y = 0;
    accel.z = -2;


    for (i = 1; i < 0x40; i++)
    {
        vel.x = ((input_vel.x * i) / 64) + (rand() & 0xF) - 7;
        vel.y = ((input_vel.y * i) / 64) + (rand() & 0xF) - 7;
        vel.z = ((input_vel.z * i) / 64) + (rand() & 0xF) - 7;

        FX_Dot(&location, &vel, &accel, 0, 0x1800FF, 0x10, 0x16, 1);
    }

}

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

FXParticle *FX_GetTorchParticle(Instance *instance, short startSegment, int tex, int birthRadius, int num)
{
    FXParticle *currentParticle;

    currentParticle = FX_GetParticle(instance, startSegment);

    if (currentParticle != NULL)
    {
        currentParticle->type = 1;

        currentParticle->fxprim_process = FX_FlamePrimProcess;

        currentParticle->texture = FX_GetTextureObject(instance->object, 1, tex);

        currentParticle->startColor = 0x20040F0;

        currentParticle->primLifeTime = 16;

        currentParticle->acceleration.z = 1;

        currentParticle->lifeTime = -1;

        currentParticle->startFadeValue = 5;

        currentParticle->fadeStep = 5;

        currentParticle->size = 48;

        currentParticle->endColor = 0;

        currentParticle->scaleSpeed = 100;

        currentParticle->birthRadius = birthRadius;
        currentParticle->numberBirthParticles = num;

        currentParticle->flag_bits |= 0x1;

        FX_InsertGeneralEffect(currentParticle);
    }

    return currentParticle;
}

FXParticle *FX_TorchFlame(Instance *instance, short startSegment)
{

    Object *particle;
    FXParticle *currentParticle;

    particle = (Object *)objectAccess[0xA].object;

    if (particle == NULL) { return NULL; }

    currentParticle = FX_GetTorchParticle(instance, startSegment, 0, 0x10, -5);
    if (currentParticle != NULL)
    {
        currentParticle->direction.x = 0x30;
        currentParticle->direction.y = 0x20;
        currentParticle->direction.z = 0x38;
    }

    currentParticle = FX_GetTorchParticle(instance, startSegment, 1, 8, -1);
    if (currentParticle != NULL)
    {
        currentParticle->direction.x = 0x18;
        currentParticle->direction.y = 0x10;
        currentParticle->direction.z = 0x10;
    }

    currentParticle = FX_GetParticle(instance, startSegment);
    if (currentParticle != NULL)
    {
        currentParticle->size = 0xA;
        currentParticle->texture = FX_GetTextureObject(particle, 0, 1);
        currentParticle->birthRadius = 0x14;
        currentParticle->direction.x = 0x80;
        currentParticle->direction.y = 0x80;
        currentParticle->direction.z = 0x80;
        currentParticle->startColor = 0x020093E4;
        currentParticle->acceleration.x = 0;
        currentParticle->acceleration.y = 0;
        currentParticle->acceleration.z = 1;
        currentParticle->numberBirthParticles = 1;
        currentParticle->endColor = 0;
        currentParticle->lifeTime = -1;
        currentParticle->primLifeTime = 0xF;
        currentParticle->flag_bits |= 1;
        FX_InsertGeneralEffect(currentParticle);
    }

    return currentParticle;
}

int FX_GetMorphFadeVal()
{
    int fade;

    fade = (gameTrackerX.gameData.asmData.MorphTime * 4096) / 1000;

    if (gameTrackerX.gameData.asmData.MorphType == 1)
    {
        fade = 4096 - fade;
    }

    return fade;
}

void FX_ConvertCamPersToWorld(SVECTOR *campos, SVECTOR *worldpos)
{

    SetRotMatrix(theCamera.core.cwTransform2);
    SetTransMatrix(theCamera.core.cwTransform2);

    campos->vx -= 256;
    campos->vx = ((campos->vx * 320) / 512);
    campos->vy -= 120;

    campos->vx = ((campos->vx * campos->vz) / 320);
    campos->vy = ((campos->vy * campos->vz) / 320);

    gte_ldv0(campos);
    gte_nrtv0tr();
    gte_stsv(worldpos);
}

void FX_GetRandomScreenPt(SVECTOR *point)
{
    int temp; // not from decls.h

    temp = rand();

    point->vx = temp - ((temp / 512) * 512);

    point->vy = rand() % 240;

    point->vz = (rand() & 0xFFF) + 384;
}

void FX_ProcessSnow(FX_PRIM *fxPrim, FXTracker *fxTracker)
{

    SVECTOR position;

    if (fxPrim->work0 == 0x270F)
    {
        if (gameTrackerX.gameData.asmData.MorphType == 1 ||
            gameTrackerX.gameData.asmData.MorphTime != 0x3E8 ||
            snow_amount == 0)
        {
            FX_Die(fxPrim, fxTracker);
            return;
        }

        fxPrim->work0 = 0;
        FX_GetRandomScreenPt(&position);
        FX_ConvertCamPersToWorld(&position, (SVECTOR *)&fxPrim->position);

    }

    if (fxPrim->timeToLive > 0)
    {
        fxPrim->timeToLive--;
    }

    if (fxPrim->timeToLive == 0)
    {
        FX_Die(fxPrim, fxTracker);
        return;
    }

    fxPrim->position.x += (windx * ((rand() & 2047) + 2048)) / 4096;
    fxPrim->position.y += (windy * ((rand() & 2047) + 2048)) / 4096;
    fxPrim->position.z += fxPrim->duo.flame.segment;
}

void FX_ContinueSnow(FXTracker *fxTracker)
{
    FX_PRIM *fxPrim;
    SVECTOR position;
    SVECTOR vel;
    int temp; // not from decls.h

    if (gameTrackerX.gameData.asmData.MorphTime != 1000)
    {
        if (gameTrackerX.gameData.asmData.MorphType != 0)
        {
            goto label;
        }
    }
    else if (gameTrackerX.gameData.asmData.MorphType != 1)
    {
    label:
        if ((rand() & 0x3FF) <= snow_amount)
        {
            fxPrim = FX_GetPrim(gFXT);

            if (fxPrim != NULL)
            {
                SVECTOR campos;

                vel.vx = 0;
                vel.vy = 0;
                vel.vz = -13 - (rand() & 3);

                temp = rand();

                campos.vx = temp - ((temp / 512) * 512);
                campos.vy = 5;
                campos.vz = (rand() & 0xFFF) + 384;

                FX_ConvertCamPersToWorld(&campos, &position);

                FX_DFacadeParticleSetup(fxPrim, &position, 1, 1, 0xFFFFFF, &vel, NULL, fxTracker, 150);

                fxPrim->process = FX_ProcessSnow;

                fxPrim->work0 = 0;

                fxPrim->flags |= 0x800000;

                FX_Sprite_Insert(&fxTracker->usedPrimListSprite, fxPrim);
            }
        }
    }
}

void FX_UpdateWind(FXTracker *fxTracker)
{
    short change;

    (void)fxTracker;

    change = rand() % 4;

    if ((rand() & 0x1))
    {
        change = -change;
    }

    wind_speed += change;

    if (wind_speed > 40)
    {
        wind_speed = 40;
    }

    if (wind_speed < 15)
    {
        wind_speed = 15;
    }

    change = rand() % 8;

    if ((rand() & 0x1))
    {
        change = -change;
    }

    wind_deg += change;

    if (wind_deg > 1280)
    {
        wind_deg = 1280;
    }

    if (wind_deg < 768)
    {
        wind_deg = 768;
    }

    windx = (rcos(wind_deg) * wind_speed) / 4096;
    windy = (rsin(wind_deg) * wind_speed) / 4096;
}

void FX_ProcessRain(FX_PRIM *fxPrim, FXTracker *fxTracker)
{
    int zvel;

    zvel = fxPrim->duo.phys.zVel + theCamera.focusInstanceVelVec.z;

    if (fxPrim->timeToLive > 0)
    {
        fxPrim->timeToLive--;
    }

    fxPrim->v1.z += zvel;

    if ((fxPrim->work0 >= fxPrim->v1.z) || (fxPrim->timeToLive == 0))
    {
        FX_Die(fxPrim, fxTracker);
    }
    else
    {
        fxPrim->v0.x += fxPrim->duo.phys.xVel;
        fxPrim->v1.x += fxPrim->duo.phys.xVel;

        fxPrim->v0.y += fxPrim->duo.phys.yVel;
        fxPrim->v1.y += fxPrim->duo.phys.yVel;

        fxPrim->v0.z += zvel;
    }
}

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

void FX_MakeParticleTexFX(FX_PRIM *fxPrim, SVector *position, Object *object, int modelnum, int texnum, SVector *vel, SVector *accl, long color, int size, int life)
{

    if (object == NULL)
    {
        object = objectAccess[0xA].object;
    }

    if (object != NULL && fxPrim != NULL)
    {
        FX_DFacadeParticleSetup(fxPrim, (SVECTOR *)position, size, size, color, (SVECTOR *)vel, (SVECTOR *)accl, gFXT, (short)life);
        fxPrim->texture = FX_GetTextureObject(object, modelnum, texnum);
        fxPrim->fadeValue[3] = 0;
        fxPrim->fadeValue[2] = 0;
        fxPrim->fadeValue[1] = 0;
        fxPrim->fadeValue[0] = 0;
        fxPrim->color = color | 0x2E000000;
        fxPrim->startColor = color;
        fxPrim->endColor = 0;
        fxPrim->flags |= 0xC0001;
        fxPrim->fadeStep = 0x1000 / life;
    }
}

void FX_MakeHitFX(SVector *position)
{
    FX_PRIM *fxPrim;
    fxPrim = FX_GetPrim(gFXT);

    if (fxPrim != NULL)
    {
        FX_MakeParticleTexFX(fxPrim, position, 0, 0, 0, 0, 0, 0xFFFFFF, 0x60, 8);
        FX_Sprite_Insert(&gFXT->usedPrimListSprite, fxPrim);
        fxPrim->flags |= 0x8000;
    }
}

void FX_ContinueLightning(FXLightning *zap, FXTracker *fxTracker)
{
    (void)fxTracker;

    zap->deg += zap->deg_inc;

    if (zap->lifeTime > 0)
    {
        zap->lifeTime -= FX_Frames;

        if (zap->lifeTime <= 0)
        {
            FX_DeleteGeneralEffect((FXGeneralEffect *)zap);
        }
    }
}

void FX_SetReaverInstance(Instance *instance)
{
    FX_reaver_instance = instance;
}

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_SoulReaverBlade);

void FX_ReaverBladeInit()
{
}

void FX_SoulReaverWinding(Instance *instance, PrimPool *primPool, unsigned long **ot, MATRIX *wcTransform)
{
    MATRIX mat;
    MATRIX *swTransform;
    SVector start;
    SVector end;
    ReaverData *data;
    long color;
    long glow_color;
    short temp; // not from decls.h

    (void)primPool;

    data = (ReaverData *)instance->extraData;

    if (((unsigned char)data->ReaverPickedUp != 0) && ((unsigned char)data->ReaverOn != 0))
    {
        temp = -data->ReaverDeg;

        swTransform = gameTrackerX.playerInstance->matrix;

        CompMatrix(wcTransform, &swTransform[40], &mat);

        start.z = 0;
        start.y = 0;
        start.x = 0;

        end.y = 0;
        end.x = 0;
        end.z = -128;

        color = data->ReaverBladeColor;
        glow_color = data->ReaverBladeGlowColor;

        FX_Lightning(wcTransform, ot, &mat, temp, &start, &end, 30, 10, 16, 32, 0, color, glow_color);

        swTransform = gameTrackerX.playerInstance->matrix;

        CompMatrix(wcTransform, &swTransform[39], &mat);

        end.z = -96;

        FX_Lightning(wcTransform, ot, &mat, temp, &start, &end, 30, 10, 16, 32, 0, color, glow_color);

        swTransform = &instance->matrix[1];

        end.z = -(((data->ReaverSize * data->ReaverScale) / 4096) * 380) / 4096;

        if (data->CurrentReaver == 1)
        {
            color = 0xFCFFD3;
        }

        CompMatrix(wcTransform, swTransform, &mat);

        FX_Lightning(wcTransform, ot, &mat, temp, &start, &end, 0, 25, 4, 8, 0, color, glow_color);
    }
}

void FX_UpdateInstanceWaterSplit(Instance *instance)
{
    SVector normal;

    if ((instance->waterFace != NULL) && (instance->waterFaceTerrain != NULL))
    {
        if (!(instance->halvePlane.flags & 0x8))
        {
            COLLIDE_GetNormal(instance->waterFace->normal, (short *)instance->waterFaceTerrain->normalList, &normal);

            instance->halvePlane.flags = 0x2;

            FX_GetPlaneEquation(&normal, &instance->splitPoint, &instance->halvePlane);
        }

        FX_MakeWaterTrail(instance, instance->splitPoint.z);
    }
    else
    {
        instance->halvePlane.flags &= ~0x2;
    }

    instance->oldWaterFace = instance->waterFace;

    instance->waterFace = NULL;
    instance->waterFaceTerrain = NULL;
}

void FX_GetPlaneEquation(SVector *normal, SVector *poPlane, PlaneConstants *plane)
{
    plane->a = normal->x;
    plane->b = normal->y;
    plane->c = normal->z;

    plane->d = -(((plane->a * poPlane->x) + (plane->b * poPlane->y) + (plane->c * poPlane->z)) >> 12);
}

void FX_DoInstancePowerRing(Instance *instance, long atuTime, long *color, long numColors, int follow_halveplane)
{

    SVector normal;
    SVector point;
    FXHalvePlane *ring;

    ring = (FXHalvePlane *)MEMPACK_Malloc(sizeof(FXHalvePlane) + (numColors * 4), 13);

    if (ring == NULL) { return; }

    ring->effectType = 0x82;
    ring->continue_process = &FX_UpdateInstanceSplitRing;
    ring->diffTime = 0;
    ring->instance = instance;
    ring->type = follow_halveplane;
    ring->colorArray = NULL;
    ring->colorBlendLife = 0;
    ring->numColors = numColors;
    ring->lifeTime = (atuTime * 0x3E8) / 1200;

    if (numColors < 2)
    {
        ring->currentColor = color != NULL ? color[0] : 0xFF8010;
    }
    else
    {

        int i;
        ring->colorArray = (long *)(&ring[1]);

        for (i = 0; i < numColors; i++)
        {
            ring->colorArray[i] = color[i];
        }

        ring->colorBlendLife = ring->lifeTime / (numColors - 1);
        ring->currentColor = color[0];
    }

    normal.x = normal.y = 0;
    normal.z = 0x1000;

    point.x = instance->position.x;
    point.y = instance->position.y;
    point.z = instance->position.z;

    FX_GetPlaneEquation(&normal, &point, &ring->ringPlane);
    FX_InsertGeneralEffect(ring);

}

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_UpdatePowerRing);

void FX_UpdateInstanceSplitRing(FXHalvePlane *ring, FXTracker *fxTracker)
{

    (void)fxTracker;

    if (ring->lifeTime != 0)
    {
        if (ring->type == 0)
        {
            FX_UpdatePowerRing(ring);
        }
        else
        {
            ring->diffTime += gameTrackerX.lastLoopTime;
            if (ring->diffTime >= ring->lifeTime)
            {
                ring->diffTime = ring->lifeTime;
                ring->lifeTime = 0;
            }
        }
        if (ring->lifeTime == 0)
        {
            FX_DeleteGeneralEffect((FXGeneralEffect *)ring);
        }
    }
    else
    {
        FX_DeleteGeneralEffect((FXGeneralEffect *)ring);
    }
}

void FX_UpdateGlowEffect(FXGlowEffect *effect, FXTracker *fxTracker)
{
    (void)fxTracker;

    if (effect->lifeTime == 0)
    {
        FX_DeleteGeneralEffect((FXGeneralEffect *)effect);
    }
}

void FX_InsertGeneralEffect(void *ptr)
{
    ((FXGeneralEffect *)ptr)->next = FX_GeneralEffectTracker;

    FX_GeneralEffectTracker = ptr;
}

void FX_DeleteGeneralEffect(FXGeneralEffect *effect)
{
    FXGeneralEffect *currentEffect;
    FXGeneralEffect *previousEffect;

    if (effect == NULL)
    {
        return;
    }

    previousEffect = NULL;

    for (currentEffect = FX_GeneralEffectTracker; currentEffect != NULL; currentEffect = currentEffect->next)
    {
        if (currentEffect == effect)
        {
            if (previousEffect != NULL)
            {
                previousEffect->next = effect->next;
            }
            else
            {
                FX_GeneralEffectTracker = currentEffect->next;
            }

            break;
        }
        else
        {
            previousEffect = currentEffect;
        }
    }

    if (effect->effectType == 0)
    {
        MEMPACK_Free((char *)effect[1].continue_process);
    }

    MEMPACK_Free((char *)effect);
}

FXGlowEffect *FX_DoInstanceOneSegmentGlow(Instance *instance, long segment, long *color, long numColors, long atuColorCycleRate, long width, long height)
{
    FXGlowEffect *glowEffect;

    atuColorCycleRate *= 33;

    glowEffect = (FXGlowEffect *)MEMPACK_Malloc((numColors * 4) + sizeof(FXGlowEffect), 13);

    if (glowEffect != NULL)
    {
        glowEffect->effectType = 131;

        glowEffect->continue_process = FX_UpdateGlowEffect;

        glowEffect->colorArray = NULL;
        glowEffect->numColors = numColors;
        glowEffect->colorBlendCycle = atuColorCycleRate;

        glowEffect->width = width;
        glowEffect->height = height;

        glowEffect->instance = instance;

        glowEffect->segment = segment;

        glowEffect->lifeTime = -1;
        glowEffect->diffTime = 0;

        glowEffect->fadein_time = 0;
        glowEffect->fadeout_time = 0;

        glowEffect->SegmentInc = 1;
        glowEffect->numSegments = 1;

        if (numColors < 2)
        {
            if (color == NULL)
            {
                glowEffect->currentColor = 0xFF8010;
            }
            else
            {
                glowEffect->currentColor = color[0];
            }
        }
        else
        {
            int i;

            glowEffect->colorArray = (long *)&glowEffect[1]; // does not look quite valid 

            for (i = 0; i < numColors; i++)
            {
                glowEffect->colorArray[i] = color[i];
            }

            glowEffect->colorBlendCycle = atuColorCycleRate / (numColors - 1);
            glowEffect->currentColor = color[0];
        }

        glowEffect->next = FX_GeneralEffectTracker;

        FX_GeneralEffectTracker = (FXGeneralEffect *)glowEffect;
    }

    instance->flags2 |= 0x200;

    return glowEffect;
}

void FX_SetGlowFades(FXGlowEffect *glowEffect, int fadein, int fadeout)
{
    glowEffect->fadein_time = fadein * 33;
    glowEffect->fadeout_time = fadeout * 33;
}

FXGlowEffect *FX_DoInstanceTwoSegmentGlow(Instance *instance, long segment, long segmentEnd, long *color, long numColors, long atuColorCycleRate, long height)
{

    int inc;
    FXGlowEffect *glowEffect;

    inc = segmentEnd - segment;

    if (inc < 0)
    {
        segment = segmentEnd;
        inc = -inc;
    }

    glowEffect = FX_DoInstanceOneSegmentGlow(instance, segment, color, numColors, atuColorCycleRate, height, height);
    glowEffect->numSegments = 2;
    glowEffect->SegmentInc = inc;

    return glowEffect;
}

FXGlowEffect *FX_DoInstanceManySegmentGlow(Instance *instance, long segment, long numSegments, long *color, long numColors, long atuColorCycleRate, long height)
{
    FXGlowEffect *glowEffect;

    glowEffect = FX_DoInstanceOneSegmentGlow(instance, segment, color, numColors, atuColorCycleRate, height, height);

    glowEffect->numSegments = numSegments;

    return glowEffect;
}

FXGlowEffect *FX_DoInstanceOneSegmentGlowWithTime(Instance *instance, long segment, long *color, long numColors, long atuColorCycleRate, long width, long height, long ATULifeTime)
{
    FXGlowEffect *glowEffect;

    glowEffect = FX_DoInstanceOneSegmentGlow(instance, segment, color, numColors, atuColorCycleRate, width, height);

    glowEffect->lifeTime = ATULifeTime * 33;

    return glowEffect;
}

void FX_StopAllGlowEffects(Instance *instance, int fadeout_time)
{
    FXGlowEffect *currentEffect;
    FXGlowEffect *previousEffect;
    int temp; // not from decls.h

    temp = fadeout_time * 33;

    for (currentEffect = (FXGlowEffect *)FX_GeneralEffectTracker; currentEffect != NULL; currentEffect = previousEffect)
    {
        previousEffect = currentEffect->next;

        if ((currentEffect->effectType == 131) && (currentEffect->instance == instance))
        {
            if (temp != 0)
            {
                currentEffect->lifeTime = currentEffect->fadeout_time = temp;
            }
            else
            {
                FX_DeleteGeneralEffect((FXGeneralEffect *)currentEffect);
            }
        }
    }
}

void FX_StopGlowEffect(FXGlowEffect *glowEffect, int fadeout_time)
{
    FXGeneralEffect *currentEffect;
    FXGeneralEffect *previousEffect;

    if (glowEffect != NULL)
    {
        if (fadeout_time != 0)
        {
            glowEffect->fadeout_time = fadeout_time * 33;
            glowEffect->lifeTime = fadeout_time * 33;
        }
        else
        {
            for (currentEffect = FX_GeneralEffectTracker; currentEffect != NULL; currentEffect = previousEffect)
            {
                previousEffect = currentEffect->next;

                if ((currentEffect->effectType == 131) && ((FXGlowEffect *)currentEffect == glowEffect))
                {
                    FX_DeleteGeneralEffect(currentEffect);
                }
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_DrawLightning);

void FX_DrawAllGeneralEffects(MATRIX *wcTransform, VertexPool *vertexPool, PrimPool *primPool, unsigned long **ot)
{
    Instance *instance;
    FXGeneralEffect *currentEffect;

    for (currentEffect = FX_GeneralEffectTracker; currentEffect != NULL; currentEffect = currentEffect->next)
    {
        if ((currentEffect->effectType & 0x80))
        {
            instance = currentEffect->instance;

            if ((instance == NULL) || ((!(instance->flags & 0x800)) && (!(instance->flags2 & 0x4000000))))
            {
                if (currentEffect->effectType == 131)
                {
                    FXGlowEffect *currentGlow;

                    currentGlow = (FXGlowEffect *)currentEffect;

                    if (currentEffect->lifeTime > 0)
                    {
                        currentEffect->lifeTime -= gameTrackerX.lastLoopTime;

                        if (currentEffect->lifeTime < 0)
                        {
                            currentEffect->lifeTime = 0;
                        }
                    }

                    PIPE3D_DoGlow(instance, wcTransform, vertexPool, primPool, ot, currentGlow);
                }
                else if (currentEffect->effectType == 130)
                {
                    PIPE3D_HalvePlaneGetRingPoints(instance, wcTransform, vertexPool, primPool, ot, (FXHalvePlane *)currentEffect);
                }
                else if (currentEffect->effectType == 132)
                {
                    FX_DrawBlastring(wcTransform, (FXBlastringEffect *)currentEffect);
                }
                else if (currentEffect->effectType == 133)
                {
                    FX_LightHouse(wcTransform, ot, currentEffect->instance, ((FXLightBeam *)currentEffect)->startSeg, ((FXLightBeam *)currentEffect)->endSeg, 32, ((FXLightBeam *)currentEffect)->color);
                }
                else if (currentEffect->effectType == 134)
                {
                    FX_DrawFField(wcTransform, (FXForceFieldEffect *)currentEffect);
                }
                else if (currentEffect->effectType == 135)
                {
                    FX_DrawLightning((FXLightning *)currentEffect, wcTransform, ot);
                }
                else if (currentEffect->effectType == 136)
                {
                    FX_DrawFlash((FXFlash *)currentEffect);
                }
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_ContinueBlastRing);

FXBlastringEffect *FX_DoBlastRing(Instance *instance, SVector *position, MATRIX *mat, int segment, int radius, int endRadius, int colorChangeRadius, int size1, int size2, int vel, int accl, int height1, int height2, int height3, long startColor, long endColor, int pred_offset, int lifeTime, int sortInWorld)
{
    FXBlastringEffect *blast;

    blast = (FXBlastringEffect *)MEMPACK_Malloc(sizeof(FXBlastringEffect), 13);

    if (blast != NULL)
    {
        blast->continue_process = FX_ContinueBlastRing;

        blast->instance = instance;

        blast->effectType = 132;

        if (pred_offset != 0)
        {
            blast->type = 1;
        }
        else
        {
            blast->type = 0;
        }

        blast->predator_offset_orig = blast->predator_offset = pred_offset;

        blast->lifeTime = lifeTime;

        blast->position = *position;

        if (mat != NULL)
        {
            blast->matrix = *mat;
        }

        blast->segment = segment;

        blast->startRadius = blast->radius = radius * 4096;

        blast->size1 = size1;
        blast->size2 = size2;

        blast->endRadius = endRadius * 4096;
        blast->colorchange_radius = colorChangeRadius * 4096;

        blast->vel = vel;

        blast->accl = accl;

        blast->height1 = height1;
        blast->height2 = height2;
        blast->height3 = height3;

        blast->startColor = blast->color = startColor;
        blast->endColor = endColor;

        blast->sortInWorld = sortInWorld;

        blast->stay_in_place = 0;

        FX_InsertGeneralEffect(blast);
    }

    return blast;
}

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_DrawBlastring);

void FX_ContinueFlash(FXFlash *flash, FXTracker *fxTracker)
{

    (void)fxTracker;
    flash->currentTime += gameTrackerX.timeMult;

    if ((flash->currentTime / 16) >= flash->timeFromColor)
    {
        FX_DeleteGeneralEffect((FXGeneralEffect *)flash);
    }
}

void FX_DrawFlash(FXFlash *flash)
{

    unsigned long color;
    unsigned long black;

    int time;
    int div;
    int interp;
    int transtype;

    time = flash->currentTime / 16;
    color = flash->color;
    black = 0;
    transtype = 1;

    if (color & 0x01000000)
    {
        transtype = 2;
    }

    if (time < flash->timeToColor)
    {

        interp = (time * 4096) / flash->timeToColor;

        if (interp > 4096)
        {
            interp = 4096;
        }

        div = interp;
        LoadAverageCol((u_char *)&color, (u_char *)&black, div, 4096 - div, (u_char *)&color);

    }
    else if (flash->timeAtColor < time)
    {

        div = flash->timeFromColor - flash->timeAtColor;

        if (div != 0)
        {
            interp = ((time - flash->timeAtColor) * 4096) / div;
        }
        else
        {
            interp = 4096;
        }

        if (interp > 4096)
        {
            interp = 4096;
        }

        div = 4096 - interp;
        LoadAverageCol((u_char *)&color, (u_char *)&black, div, interp, (u_char *)&color);
    }

    FX_DrawScreenPoly(transtype, color, 5);
}


void FX_RelocateGeneric(Object *object, long offset)
{

    GenericFXObject *GFXO;

    GFXO = (GenericFXObject *)object->data;
    GFXO->ParticleList = (GenericParticleParams *)OFFSET_DATA(GFXO->ParticleList, offset);
    GFXO->RibbonList = (GenericRibbonParams *)OFFSET_DATA(GFXO->RibbonList, offset);
    GFXO->GlowList = (GenericGlowParams *)OFFSET_DATA(GFXO->GlowList, offset);
    GFXO->LightningList = (GenericLightningParams *)OFFSET_DATA(GFXO->LightningList, offset);
    GFXO->BlastList = (GenericBlastringParams *)OFFSET_DATA(GFXO->BlastList, offset);
    GFXO->FlashList = (GenericFlashParams *)OFFSET_DATA(GFXO->FlashList, offset);
    GFXO->ColorList = (long *)OFFSET_DATA(GFXO->ColorList, offset);
}

FXParticle *FX_StartGenericParticle(Instance *instance, int num, int segOverride, int lifeOverride, int InitFlag)
{
    FXParticle *currentParticle;
    GenericFXObject *GFXO;
    GenericParticleParams *GPP;
    Object *texture_obj;
    Object *particle;

    particle = objectAccess[10].object;

    texture_obj = NULL;

    if (particle == NULL)
    {
        return NULL;
    }

    GFXO = particle->data;

    GPP = &GFXO->ParticleList[num];

    if ((InitFlag != 0) && (GPP->StartOnInit == 0))
    {
        return NULL;
    }

    if (GPP->use_child != 0)
    {
        instance = instance->LinkChild;

        if (instance == NULL)
        {
            return NULL;
        }
    }

    if (GPP->texture != -1)
    {
        if (GPP->useInstanceModel != 0)
        {
            texture_obj = instance->object;
        }
        else
        {
            texture_obj = particle;
        }

        if (texture_obj == NULL)
        {
            return NULL;
        }
    }

    currentParticle = FX_GetParticle(instance, 0);

    if (currentParticle != NULL)
    {
        currentParticle->numberBirthParticles = GPP->numberBirthParticles;

        currentParticle->size = GPP->size;

        currentParticle->type = GPP->type;

        currentParticle->birthRadius = GPP->birthRadius;

        if (segOverride == 0)
        {
            currentParticle->startSegment = GPP->startSegment;
        }
        else
        {
            currentParticle->startSegment = segOverride;
        }

        currentParticle->endSegment = GPP->endSegment;

        currentParticle->direction = GPP->direction;

        currentParticle->acceleration.x = GPP->accx;
        currentParticle->acceleration.y = GPP->accy;
        currentParticle->acceleration.z = GPP->accz;

        currentParticle->startColor = *(long *)&GPP->startColor_red & 0xFFFFFF;
        currentParticle->endColor = *(long *)&GPP->endColor_red & 0xFFFFFF;

        if (lifeOverride != 0)
        {
            currentParticle->lifeTime = lifeOverride;
        }
        else
        {
            currentParticle->lifeTime = GPP->lifeTime;
        }

        currentParticle->primLifeTime = GPP->primLifeTime;

        currentParticle->startFadeValue = GPP->startFadeValue;
        currentParticle->startScale = GPP->startScale;

        currentParticle->scaleSpeed = GPP->scaleSpeed;

        currentParticle->offset.x = GPP->offset.x;
        currentParticle->offset.y = GPP->offset.y;
        currentParticle->offset.z = GPP->offset.z;

        currentParticle->z_undulate = GPP->z_undulation_mode;

        if ((GPP->fadeStep == -1) && (GPP->primLifeTime != 0))
        {
            currentParticle->fadeStep = (4096 - currentParticle->startFadeValue) / GPP->primLifeTime;
        }
        else
        {
            currentParticle->fadeStep = GPP->fadeStep;
        }

        if (GPP->texture != -1)
        {
            TextureMT3 *texture;

            texture = FX_GetTextureObject(texture_obj, GPP->model, GPP->texture);

            currentParticle->texture = texture;

            currentParticle->startColor |= texture->color & 0x3000000;
        }

        if (GPP->type == 1)
        {
            currentParticle->fxprim_process = FX_AttachedParticlePrimProcess;
        }

        if (GPP->spectral_colorize == 1)
        {
            currentParticle->flag_bits |= 0x1;
        }

        if (GPP->absolute_direction != 0)
        {
            currentParticle->flag_bits |= 0x2;
        }

        if (GPP->spectral_colorize == 2)
        {
            int tmp_blue;
            CVECTOR *ptr;

            if (SoulReaverFire() != 0)
            {
                ptr = (CVECTOR *)&currentParticle->startColor;

                tmp_blue = ptr->b;

                ptr->b = currentParticle->startColor;

                ptr = (CVECTOR *)&currentParticle->endColor;

                ((char *)&currentParticle->startColor)[0] = tmp_blue;

                tmp_blue = ptr->b;

                ptr->b = currentParticle->endColor;

                ((char *)&currentParticle->endColor)[0] = tmp_blue;
            }
        }

        FX_InsertGeneralEffect(currentParticle);
    }

    return currentParticle;
}

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_StartGenericRibbon);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_StartGenericGlow);

FXLightning *FX_CreateLightning(Instance *instance, int lifeTime, int deg, int deg_inc, int width, int small_width, int segs, int sine_size, int variation, unsigned long color, unsigned long glow_color)
{
    FXLightning *zap;

    zap = (FXLightning *)MEMPACK_Malloc(sizeof(FXLightning), 13);

    if (zap != NULL)
    {
        zap->continue_process = FX_ContinueLightning;

        zap->end_instance = zap->instance = instance;

        zap->effectType = 135;
        zap->type = 0;

        zap->lifeTime = lifeTime;

        zap->deg = deg;
        zap->deg_inc = deg_inc;

        zap->width = width;
        zap->small_width = small_width;

        zap->segs = segs;

        zap->sine_size = sine_size;

        zap->variation = variation;

        zap->color = color;
        zap->glow_color = glow_color;

        zap->start_offset.z = zap->start_offset.y = zap->start_offset.x = 0;
        zap->end_offset.z = zap->end_offset.y = zap->end_offset.x = 0;

        zap->matrixSeg = -1;

        FX_InsertGeneralEffect(zap);
    }

    return zap;
}

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_SetLightingPos);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_StartGenericLightning);

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_StartGenericBlastring);

FXFlash *FX_StartGenericFlash(Instance *instance, int num)
{
    GenericFXObject *GFXO;
    GenericFlashParams *GFP;
    Object *particle;
    FXFlash *flash;

    particle = (Object *)objectAccess[10].object;

    if (particle == NULL)
    {
        return NULL;
    }

    GFXO = (GenericFXObject *)particle->data;

    GFP = &GFXO->FlashList[num];

    flash = (FXFlash *)MEMPACK_Malloc(sizeof(FXFlash), 13);

    if (flash != NULL)
    {
        flash->continue_process = FX_ContinueFlash;

        flash->effectType = 136;

        flash->instance = instance;

        flash->type = 0;

        flash->lifeTime = -1;

        flash->color = GFP->color;

        flash->currentTime = 0;

        flash->timeToColor = GFP->timeToColor * 256;
        flash->timeAtColor = flash->timeToColor + (GFP->timeAtColor * 256);
        flash->timeFromColor = flash->timeAtColor + (GFP->timeFromColor * 256);

        FX_InsertGeneralEffect(flash);
    }

    return flash;
}

// Matches 100% on decomp.me but differs on this project
#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_GetHealthColor);
#else
long FX_GetHealthColor(int currentHealth)
{
    STATIC long HealthColors[8924 + 6];
    long color;

    if (currentHealth <= 0)
    {
        color = HealthColors[0];
    }
    else if ((currentHealth >= 0) && (currentHealth < 6))
    {
        color = HealthColors[currentHealth - 1];
    }
    else
    {
        color = HealthColors[5];
    }

    color &= 0xFFFCFCFC;

    return color;
}
#endif

void FX_Start_Snow(int percent)
{
    snow_amount = (percent * 0x300) / 100;
}

void FX_Start_Rain(int percent)
{
    rain_amount = (percent * 1024) / 100;
    if (rain_amount == 0)
    {
        current_rain_fade = 0;
    }
}

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

void FX_StartInstanceEffect(Instance *instance, ObjectEffect *effect, int InitFlag)
{
    long color;
    int temp, temp2, temp3; // not from decls.h

    color = 0x4080;

    switch (effect->effectNumber)
    {
    case 0:
        break;
    case 1:
        FX_TorchFlame(instance, effect->modifierList[0]);
        break;
    case 2:
        if ((effect->modifierList[1] == 1) || (effect->modifierList[1] == 2) || (effect->modifierList[1] == 3) || (effect->modifierList[1] == 4) || (effect->modifierList[1] == 5))
        {
            color = FX_GetColor(effect, 1);
        }

        FX_DoInstanceOneSegmentGlow(instance, effect->modifierList[0], &color, 1, 1024, 50, 100);
        break;
    case 3:
    {
        long numberOfSegments;

        numberOfSegments = effect->modifierList[1];

        if (numberOfSegments <= 0)
        {
            numberOfSegments = 1;
        }

        if ((effect->modifierList[2] == 1) || (effect->modifierList[2] == 2) || (effect->modifierList[2] == 3) || (effect->modifierList[2] == 4) || (effect->modifierList[2] == 5))
        {
            color = FX_GetColor(effect, 2);
        }

        FX_DoInstanceManySegmentGlow(instance, effect->modifierList[0], numberOfSegments, &color, 1, 1024, 65);
        break;
    }
    case 5:
        FX_StartGenericParticle(instance, effect->modifierList[0], effect->modifierList[1], effect->modifierList[2], InitFlag);
        break;
    case 6:
        FX_StartGenericRibbon(instance, effect->modifierList[0], effect->modifierList[1], effect->modifierList[2], InitFlag);
        break;
    case 7:
        FX_StartGenericGlow(instance, effect->modifierList[0], effect->modifierList[1], effect->modifierList[2], InitFlag);
        break;
    case 8:
        GlyphTrigger();
        break;
    case 10:
        FX_Start_Snow(effect->modifierList[0]);
        break;
    case 11:
        FX_Start_Rain(effect->modifierList[0]);
        break;
    case 12:
    {
        FXParticle *currentParticle;
        evObjectDraftData *draft;

        draft = (evObjectDraftData *)INSTANCE_Query(instance, 22);

        if (draft != NULL)
        {
            currentParticle = FX_StartGenericParticle(instance, effect->modifierList[0], effect->modifierList[1], effect->modifierList[2], InitFlag);

            if (currentParticle != NULL)
            {
                currentParticle->direction.x = draft->radius;
                currentParticle->direction.y = draft->radiusCoef;
                currentParticle->direction.z = draft->height;

                currentParticle->primLifeTime = draft->maxVelocity / 8;

                currentParticle->birthRadius = 0;

                currentParticle->fxprim_modify_process = FX_UpdraftPrimModify;
            }
        }

        break;
    }
    case 13:
        FX_StartLightbeam(instance, effect->modifierList[0], effect->modifierList[1], effect->modifierList[2]);
        break;
    case 14:
        FX_StartGenericLightning(instance, effect->modifierList[0], effect->modifierList[1], effect->modifierList[2]);
        break;
    case 15:
        if (InitFlag == 0)
        {
            FX_StartGenericBlastring(instance, effect->modifierList[0], effect->modifierList[1], effect->modifierList[2]);
        }

        break;
    case 16:
        if (InitFlag == 0)
        {
            FX_StartGenericFlash(instance, effect->modifierList[0]);
        }

        break;
    case 17:
    {
        FXSplinter *splinterData;
        GenericTune *tune;
        short shardFlags;

        splinterData = NULL;

        if (InitFlag == 0)
        {
            shardFlags = 0;

            tune = instance->object->data;

            if ((instance->object->oflags2 & 0x40000))
            {
                PhysObSplinter *splintDef;

                splintDef = PhysObGetSplinter(instance);

                if (splintDef != NULL)
                {
                    splinterData = splintDef->splinterData;
                }
            }
            else if (tune != NULL)
            {
                splinterData = tune->shatterData;

                if ((tune->flags & 0x2))
                {
                    shardFlags = 0x10;
                }
            }

            _FX_BuildSplinters(instance, NULL, NULL, NULL, splinterData, gFXT, NULL, NULL, shardFlags);
        }

        break;
    }
    case 18:
        FX_StartGenericBlastring(instance, effect->modifierList[0], effect->modifierList[1], effect->modifierList[2]);
        break;
    case 19:
        GAMEPAD_Shock0(effect->modifierList[0], effect->modifierList[1] * 4096);
        break;
    case 20:
    {
        int shock;
        int amt;

        shock = effect->modifierList[0];

        amt = effect->modifierList[1] * 4096;

        GAMEPAD_Shock1(shock, amt);
        break;
    }
    case 21:
        temp = MATH3D_DistanceBetweenPositions(&instance->position, &theCamera.core.position);

        temp2 = effect->modifierList[0];

        temp3 = ((8000 - temp) * temp2) / 8000;

        if (temp3 > 0)
        {
            temp3 += 50;

            if (effect->modifierList[0] < temp3)
            {
                temp3 = effect->modifierList[0];
            }

            GAMEPAD_Shock1(temp3, effect->modifierList[1] * 4096);
        }

        break;
    }
}

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

void FX_CalcSpiral(int degchange)
{
    int radius;
    int deg;
    int n;
    int pntx;
    int pnty;
    int px;
    int py;
    int mx;
    int my;
    int mod;
    int next_deg;
    int next_radius;
    int minx;
    int maxx;
    int miny;
    int maxy;

    minx = 32767;
    maxx = -32767;

    miny = 32767;
    maxy = -32767;

    next_radius = radius = 8192;

    next_deg = deg = 0;

    FX_GetSpiralPoint(8192, 0, &pntx, &pnty);

    Spiral_Array[64].vx = pntx;
    Spiral_Array[64].vy = pnty;

    for (n = 0; n < 64; n++)
    {
        if (Spiral_Number == 0)
        {
            mod = n % Spiral_Mod;

            if (mod == 0)
            {
                deg = next_deg;

                radius = next_radius;

                next_radius += 1088 * Spiral_Mod;

                next_deg += degchange * Spiral_Mod;
            }

            mod = (mod * 4096) / Spiral_Mod;

            FX_GetLinePoint(radius, next_radius, deg, next_deg, &pntx, &pnty, mod);
            FX_GetLinePoint(radius + 7168, next_radius + 7168, deg, next_deg, &px, &py, mod);
            FX_GetLinePoint(radius - 7168, next_radius - 7168, deg, next_deg, &mx, &my, mod);
        }
        else
        {
            radius += 1088;

            deg += degchange;

            FX_GetSpiralPoint(radius, deg, &pntx, &pnty);
            FX_GetSpiralPoint(radius + 7168, deg, &px, &py);
            FX_GetSpiralPoint(radius - 7168, deg, &mx, &my);
        }

        Spiral_Array[n].vx = pntx;
        Spiral_Array[n].vy = pnty;

        if (n == 63)
        {
            px = pntx;
            py = pnty;

            mx = pntx;
            my = pnty;
        }

        Spiral_OffsetP[n].vx = px;
        Spiral_OffsetP[n].vy = py;

        if (px < minx)
        {
            minx = px;
        }

        if (maxx < px)
        {
            maxx = px;
        }

        if (py < miny)
        {
            miny = py;
        }

        if (maxy < py)
        {
            maxy = py;
        }

        Spiral_OffsetM[n].vx = mx;
        Spiral_OffsetM[n].vy = my;
    }

    Spiral_Glow_Size = ((320 * ((maxx - minx) / 2)) / 512) + 2;

    Spiral_Glow_X = (minx + maxx) / 2;
    Spiral_Glow_Y = (miny + maxy) / 2;
}

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

long fx_get_startz(SVector *position)
{
    MATRIX tmpmat;

    gte_ldv0(position);
    gte_nrtv0tr();
    gte_stlvnl(&tmpmat.t[0]);

    SetTransMatrix(&tmpmat);

    return tmpmat.t[2] / 4;
}

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_DrawRing);

void fx_setTex(DVECTOR *x, unsigned char *uv, int tx, int offset)
{
    uv[0] = x->vx - tx;

    if (x->vy > 255)
    {
        uv[1] = 255;
    }
    else
    {
        uv[1] = x->vy;
    }

    x->vy += offset;
}

INCLUDE_ASM("asm/nonmatchings/Game/FX", FX_DrawRing2);

void FX_DrawFField(MATRIX *wcTransform, FXForceFieldEffect *field)
{
    Instance *instance;
    MATRIX tmpmat;
    SVector position;
    int size;
    short fade;
    long color;
    long black;
    int temp, temp2; // not from decls.h

    instance = field->instance;

    position.x = instance->position.x + field->offset.x;
    position.y = instance->position.y + field->offset.y;
    position.z = instance->position.z + field->offset.z;

    black = 0;

    temp = rcos(field->deg) * field->size_change;

    field->deg = (field->deg + field->deg_change) & 0xFFF;

    size = field->size_diff + (temp >> 12);

    if (field->start_fade != 0)
    {
        field->start_fade = fade = field->start_fade - 128;

        if (fade < 0)
        {
            field->start_fade = 0;
        }

        temp2 = (unsigned short)field->start_fade * 65536;
    }
    else
    {
        if (field->end_fade != 0)
        {
            field->end_fade = fade = field->end_fade - 512;

            if ((fade * 65536) <= 0)
            {
                FX_DeleteGeneralEffect((FXGeneralEffect *)field);
                return;
            }

            fade = 4096 - fade;
        }
        else
        {
            fade = 0;
        }

        temp2 = fade * 65536;
    }

    fade = temp2 >> 16;

    if (fade != 0)
    {
        LoadAverageCol((unsigned char *)&field->color, (unsigned char *)&black, 4096 - fade, fade, (unsigned char *)&color);
    }
    else
    {
        color = field->color;
    }

    color &= 0xFFFFFF;

    MATH3D_SetUnityMatrix(&tmpmat);

    RotMatrixZ(1024, &tmpmat);

    if (field->type == 1)
    {
        PIPE3D_AspectAdjustMatrix(&tmpmat);
    }
    else
    {
        RotMatrixX(theCamera.core.rotation.x, &tmpmat);
    }

    FX_DrawRing(wcTransform, &position, &tmpmat, field->size - size, field->size, field->size - size, 0, 0, 0, color, 1);
}

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

void FX_Draw_Glowing_Line(unsigned long **ot, long otz, DVECTOR *sxy0, DVECTOR *sxy1, DVECTOR *xy0, DVECTOR *xy1, long color, long color2)
{
    POLY_2G4T *poly;
    PrimPool *primPool;
    int negflag;

    negflag = 0;

    primPool = gameTrackerX.primPool;

    if ((otz >= 1) && (otz < 3072))
    {
        poly = (POLY_2G4T *)primPool->nextPrim;

        if ((primPool->nextPrim + 18) < primPool->lastPrim)
        {
            if ((color & 0x1000000))
            {
                color &= 0xFFFFFF;

                negflag = 1;
            }

            color |= 0x3A000000;

            *(unsigned int *)&poly->p1.r2 = color;
            *(unsigned int *)&poly->p2.r0 = color;

            *(unsigned int *)&poly->p1.r0 = 0x3A000000;
            *(unsigned int *)&poly->p1.r1 = 0;

            *(unsigned int *)&poly->p1.r3 = color2;
            *(unsigned int *)&poly->p2.r1 = color2;

            *(unsigned int *)&poly->p2.r2 = 0;
            *(unsigned int *)&poly->p2.r3 = 0;

            *(unsigned int *)&poly->p2.x0 = *(unsigned int *)&sxy0->vx;
            *(unsigned int *)&poly->p1.x2 = *(unsigned int *)&poly->p2.x0;

            *(unsigned short *)&poly->p1.x0 = sxy0->vx + xy0->vx;
            *(unsigned short *)&poly->p1.y0 = sxy0->vy - xy0->vy;

            *(unsigned short *)&poly->p2.x2 = sxy0->vx - xy0->vx;
            *(unsigned short *)&poly->p2.y2 = sxy0->vy + xy0->vy;

            *(unsigned int *)&poly->p2.x1 = *(unsigned int *)&sxy1->vx;
            *(unsigned int *)&poly->p1.x3 = *(unsigned int *)&poly->p2.x1;

            *(unsigned short *)&poly->p1.x1 = sxy1->vx + xy1->vx;
            *(unsigned short *)&poly->p1.y1 = sxy1->vy - xy1->vy;

            *(unsigned short *)&poly->p2.x3 = sxy1->vx - xy1->vx;
            *(unsigned short *)&poly->p2.y3 = sxy1->vy + xy1->vy;

            if (negflag != 0)
            {
                // setDrawTPage(poly, 1, 1, 64);

                poly->drawTPage1 = _get_mode(1, 1, 64);
            }
            else
            {
                // setDrawTPage(poly, 1, 1, 32);

                poly->drawTPage1 = _get_mode(1, 1, 32);
            }

            // addPrim(poly, ot[otz]);

            *(int *)poly = getaddr(&ot[otz]) | 0x11000000;
            *(int *)&ot[otz] = (intptr_t)poly & 0xFFFFFF;

            primPool->nextPrim = (unsigned long *)(poly + 1);
        }
    }
}

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
