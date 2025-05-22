#include "common.h"
#include "Game/HASM.h"
#include "Game/GAMELOOP.h"
#include "Game/MATH3D.h"
#include "Game/DRAW.h"
#include "Game/STREAM.h"
#include "Game/COLLIDE.h"

static LightGroup default_lightgroup = {
    {{{63461, 64216, 3274}, {3363, 64057, 1809}, {2579, 2683, 1709}}, {0, 0, 0}},
    {{{2056, 2056, 2056}, {2056, 2056, 2056}, {2056, 2056, 2056}}, {0, 0, 0}}
};

static int D_800D0908[2] = {0x10001000, 4096};

static int D_800D0910[5] = {2048, 2048, 2048, 0, 0};

long (*light_sourceAndVertexFunc)();

void *light_light;

Terrain *light_terrain;

LightInfo *light_lightInfo;

static inline int LIGHT3D_FixedDivision(long a, long b)
{
    long r;

    r = (a << 12) / (a - b);

    return r;
}

static inline void LIGHT3D_FogCalc(long a, Level *level)
{
    int r;

    r = (a * level->fogFar) / (a - level->fogFar);

    if (level->holdFogNear == level->fogNear)
    {
        level->holdFogNear = r;
    }

    level->fogNear = r;
}

static inline int LIGHT3D_FixedMultiplication(long a, long b)
{
    long r;

    r = a * b;

    return r >> 12;
}

static inline int LIGHT3D_FixedMultiplication2(short a, short b)
{
    long r;

    r = a * b;

    return r >> 12;
}

static inline int LIGHT3D_FixedMultiplication3(short a, long b)
{
    long r;

    r = a * b;
    return r >> 12;
}

static inline int LIGHT3D_FixedNormalization(long a, long b, long x, long y)
{
    long r;

    r = LIGHT3D_FixedMultiplication2(a, b);

    if (r < x)
    {
        r = x;
    }

    if (y < r)
    {
        r = y;
    }

    return r;
}

void LIGHT_GetLightMatrix(struct _Instance *instance, struct Level *level, struct MATRIX *lightM, struct MATRIX *colorM)
{
    MATRIX *lightGroup;
    struct LightList *lightList;
    int lightGrp;
    MATRIX *tlightGroup;
    struct LightList *tlightList;
    int tlightGrp;
    MATRIX *start;
    MATRIX *end;
    int i;
    int j;
    long ratio;

    lightGrp = instance->lightGroup;
    lightGroup = NULL;

    if (gameTrackerX.gameData.asmData.MorphType != 0)
    {
        if (instance != gameTrackerX.playerInstance || level->razielSpectralLightGroup == NULL)
        {
            lightList = (struct LightList *)level->spectrallightList;

            if (lightList != NULL && lightList->numLightGroups != 0)
            {
                lightGrp = instance->spectralLightGroup;
            }
            else
            {
                lightList = level->lightList;
            }
        }
        else
        {
            lightGroup = (MATRIX *)level->razielSpectralLightGroup;
        }
    }
    else
    {
        if (instance == gameTrackerX.playerInstance && level->razielLightGroup != NULL)
        {
            lightGroup = (MATRIX *)level->razielLightGroup;
        }
        else
        {
            lightList = level->lightList;
        }
    }

    if (lightGroup == NULL)
    {
        if (lightList->numLightGroups == 0 || lightGrp >= lightList->numLightGroups)
        {
            lightGroup = (MATRIX *)&default_lightgroup;
        }
        else
        {
            lightGroup = (MATRIX *)&lightList->lightGroupList[lightGrp];
        }
    }

    if (gameTrackerX.gameData.asmData.MorphTime != 1000)
    {
        tlightGrp = instance->lightGroup;

        tlightGroup = NULL;

        if (gameTrackerX.gameData.asmData.MorphType == 0)
        {
            if (instance != gameTrackerX.playerInstance || level->razielSpectralLightGroup == NULL)
            {
                tlightList = level->spectrallightList;

                if (tlightList != NULL && tlightList->numLightGroups != 0)
                {
                    tlightGrp = instance->spectralLightGroup;
                }
                else
                {
                    tlightList = level->lightList;
                }
            }
            else
            {
                tlightGroup = (MATRIX *)level->razielSpectralLightGroup;
            }
        }
        else
        {
            if (instance == gameTrackerX.playerInstance && level->razielLightGroup != NULL)
            {
                tlightGroup = (MATRIX *)level->razielLightGroup;
            }
            else
            {
                tlightList = level->lightList;
            }
        }
        if (tlightGroup == NULL)
        {
            if (tlightList->numLightGroups == 0 || tlightGrp >= tlightList->numLightGroups)
            {
                tlightGroup = (MATRIX *)&default_lightgroup;
            }
            else
            {
                tlightGroup = (MATRIX *)&tlightList->lightGroupList[tlightGrp];
            }
        }

        lightM->m[0][0] = lightGroup[0].m[0][0];
        lightM->m[0][1] = lightGroup[0].m[0][1];
        lightM->m[0][2] = lightGroup[0].m[0][2];
        lightM->m[1][0] = lightGroup[0].m[1][0];
        lightM->m[1][1] = lightGroup[0].m[1][1];
        lightM->m[1][2] = lightGroup[0].m[1][2];
        lightM->m[2][0] = lightGroup[0].m[2][0];
        lightM->m[2][1] = lightGroup[0].m[2][1];
        lightM->m[2][2] = lightGroup[0].m[2][2];


        start = tlightGroup + 1;
        end = lightGroup + 1;


        ratio = ((gameTrackerX.gameData.asmData.MorphTime * 4096) / 1000);
        ratio = 0x1000 - ratio;

        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 3; j++)
            {
                colorM->m[i][j] = start->m[i][j] + LIGHT3D_FixedMultiplication3((end->m[i][j] - start->m[i][j]), ratio);
            }
        }
    }
    else
    {
        lightM->m[0][0] = lightGroup->m[0][0];
        lightM->m[0][1] = lightGroup->m[0][1];
        lightM->m[0][2] = lightGroup->m[0][2];
        lightM->m[1][0] = lightGroup->m[1][0];
        lightM->m[1][1] = lightGroup->m[1][1];
        lightM->m[1][2] = lightGroup->m[1][2];
        lightM->m[2][0] = lightGroup->m[2][0];
        lightM->m[2][1] = lightGroup->m[2][1];
        lightM->m[2][2] = lightGroup->m[2][2];
        colorM->m[0][0] = lightGroup[1].m[0][0];
        colorM->m[0][1] = lightGroup[1].m[0][1];
        colorM->m[0][2] = lightGroup[1].m[0][2];
        colorM->m[1][0] = lightGroup[1].m[1][0];
        colorM->m[1][1] = lightGroup[1].m[1][1];
        colorM->m[1][2] = lightGroup[1].m[1][2];
        colorM->m[2][0] = lightGroup[1].m[2][0];
        colorM->m[2][1] = lightGroup[1].m[2][1];
        colorM->m[2][2] = lightGroup[1].m[2][2];
    }
}

// Matches 100% on decomp.me but differs on this project
#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/Game/LIGHT3D", LIGHT_PresetInstanceLight);
#else
void LIGHT_PresetInstanceLight(Instance *instance, short attenuate, MATRIX *lm)
{
    MATRIX cm;
    long scale;
    long scaleRGB[3];
    int i;
    int j;
    Level *level;
    CDLight *extraLight = (CDLight *)instance->extraLight;
    short tempRGB[3] = {16, 16, 16};
    short *todRGB;

    level = STREAM_GetLevelWithID(instance->currentStreamUnitID);

    LIGHT_GetLightMatrix(instance, level, lm, &cm);

    if ((instance->flags & 0x200000))
    {
        scale = 2048;
    }
    else
    {
        scale = 4096;
    }

    if (attenuate != 4096)
    {
        scale = LIGHT3D_FixedMultiplication(scale, attenuate);
    }

    if ((instance->extraLight != NULL) && (!(instance->flags & 0x200000)))
    {
        scale = LIGHT3D_FixedMultiplication(4096 - instance->extraLightScale, scale);

        scaleRGB[0] = scale + ((instance->extraLightScale * extraLight->r) >> 6);
        scaleRGB[1] = scale + ((instance->extraLightScale * extraLight->g) >> 6);
        scaleRGB[2] = scale + ((instance->extraLightScale * extraLight->b) >> 6);
    }
    else
    {
        scaleRGB[0] = scale;
        scaleRGB[1] = scale;
        scaleRGB[2] = scale;
    }

    if (level != NULL)
    {
        todRGB = &level->TODRedScale;
    }
    else
    {
        todRGB = tempRGB;
    }

    for (i = 0; i < 3; i++)
    {
        scale = LIGHT3D_FixedMultiplication(scaleRGB[i], todRGB[i]);

        for (j = 0; j < 3; j++)
        {
            cm.m[i][j] = LIGHT3D_FixedNormalization(cm.m[i][j], scale, -32768, 32767);
        }
    }

    SetColorMatrix(&cm);
}
#endif

void LIGHT_GetAmbient(struct _ColorType *color, struct _Instance *instance)
{
    int lightval;

    if ((instance->object->oflags2 & 0x800))
    {
        lightval = 0;
    }
    else
    {
        lightval = 48;
    }

    color->b = lightval;
    color->g = lightval;
    color->r = lightval;
}

INCLUDE_ASM("asm/nonmatchings/Game/LIGHT3D", LIGHT_CalcLightValue);

void LIGHT_SetAmbientInstance(Instance *instance, Level *level)
{
    (void)level;
    SetBackColor(((ColorType *)&instance->light_color)->r, ((ColorType *)&instance->light_color)->g, ((ColorType *)&instance->light_color)->b);
}

// Matches 100% on decomp.me but differs on this project
#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/Game/LIGHT3D", LIGHT_SetMatrixForLightGroupInstance);
#else
void LIGHT_SetMatrixForLightGroupInstance(Instance *instance, Level *level)
{
    MATRIX *lgt;
    MATRIX lgt_cat;
    MATRIX lm;
    MATRIX cm;
    VECTOR half = {2048, 2048, 2048};
    LightList *lightList;
    int lightGrp;
    typedef struct
    {
        long m[5];
    } cmm;

    lightGrp = instance->lightGroup;

    if (instance->matrix != NULL)
    {
        if ((gameTrackerX.gameData.asmData.MorphType != 0) && (lightList = level->spectrallightList, lightList != NULL) && (lightList->numLightGroups != 0))
        {
            lightGrp = instance->spectralLightGroup;

            if (lightList->numLightGroups < lightGrp)
            {
                instance->spectralLightGroup = 0;

                lightGrp = 0;
            }
        }
        else
        {
            lightList = level->lightList;

            if ((lightList != NULL) && (lightList->numLightGroups < lightGrp))
            {
                instance->lightGroup = 0;

                lightGrp = 0;
            }
        }

        LIGHT_SetAmbientInstance(instance, level);

        if (lightList->numLightGroups == 0)
        {
            lgt = (MATRIX *)&default_lightgroup;
        }
        else
        {
            lgt = &lightList->lightGroupList[lightGrp].lightMatrix;
        }

        if (instance->lightMatrix != 0)
        {
            lgt_cat = instance->matrix[instance->lightMatrix];
        }
        else if ((instance->flags & 0x1))
        {
            lgt_cat = *instance->matrix;
        }
        else
        {
            RotMatrix((SVECTOR *)&instance->rotation, &lgt_cat);
        }

        if (instance->extraLight != NULL)
        {
            lm.m[0][0] = lgt->m[0][0];
            lm.m[0][1] = lgt->m[0][1];
            lm.m[0][2] = lgt->m[0][2];

            lm.m[1][0] = lgt->m[1][0];
            lm.m[1][1] = lgt->m[1][1];
            lm.m[1][2] = lgt->m[1][2];

            lm.m[2][0] = (instance->extraLightDir.x * instance->extraLightScale) >> 12;
            lm.m[2][1] = (instance->extraLightDir.y * instance->extraLightScale) >> 12;
            lm.m[2][2] = (instance->extraLightDir.z * instance->extraLightScale) >> 12;

            cm.m[0][0] = lgt[1].m[0][0];
            cm.m[1][0] = lgt[1].m[1][0];
            cm.m[2][0] = lgt[1].m[2][0];

            cm.m[0][1] = lgt[1].m[0][1];
            cm.m[1][1] = lgt[1].m[1][1];
            cm.m[2][1] = lgt[1].m[2][1];

            cm.m[0][2] = ((CDLight *)instance->extraLight)->r << 4;
            cm.m[1][2] = ((CDLight *)instance->extraLight)->g << 4;
            cm.m[2][2] = ((CDLight *)instance->extraLight)->b << 4;

            MulMatrix0(&lm, &lgt_cat, &lgt_cat);

            SetLightMatrix(&lgt_cat);

            if ((instance->flags & 0x200000))
            {
                ScaleMatrix(&cm, &half);
            }

            SetColorMatrix(&cm);
        }
        else
        {
            MulMatrix0(lgt, &lgt_cat, &lgt_cat);

            SetLightMatrix(&lgt_cat);

            if ((instance->flags & 0x200000))
            {
                *(cmm *)&cm = *(cmm *)&lgt[1];

                ScaleMatrix(&cm, &half);

                SetColorMatrix(&cm);
            }
            else
            {
                SetColorMatrix(&lgt[1]);
            }
        }
    }
}
#endif

void LIGHT_DrawShadow(MATRIX *wcTransform, struct _Instance *instance, struct _PrimPool *primPool, unsigned long **ot)
{
    SVECTOR face_orient; // $sp+0x10
    MATRIX rot; // $sp+0x18
    MATRIX scTransform; // $sp+0x38
    struct _Vector scale; // $sp+0x58
    struct _Instance *playerInstance; // $s0

    playerInstance = gameTrackerX.playerInstance;

    if (instance->position.z - 1280 >= instance->shadowPosition.z)
    {
        return;
    }

    face_orient.vx = -MATH3D_FastAtan2(instance->wNormal.y, instance->wNormal.z);
    face_orient.vy = MATH3D_FastAtan2(instance->wNormal.x, MATH3D_FastSqrt0(0x1000000 - instance->wNormal.x * instance->wNormal.x));
    face_orient.vz = instance->rotation.z;

    RotMatrix(&face_orient, &rot);

    if (playerInstance != NULL)
    {
        rot.t[0] = instance->shadowPosition.x;
        rot.t[1] = instance->shadowPosition.y;
        rot.t[2] = instance->shadowPosition.z;
    }
    else
    {
        rot.t[0] = instance->shadowPosition.x;
        rot.t[1] = instance->shadowPosition.y;
        rot.t[2] = instance->shadowPosition.z;
    }

    gte_SetRotMatrix(&wcTransform->m[0][0]);

    gte_ldclmv(&rot.m[0][0]);
    gte_nrtir12();
    gte_stclmv(&scTransform);

    gte_ldclmv(&rot.m[0][1]);
    gte_nrtir12();
    gte_stclmv(&scTransform.m[0][1]);

    gte_ldclmv(&rot.m[0][2]);
    gte_nrtir12();
    gte_stclmv(&scTransform.m[0][2]);

    gte_SetTransMatrix(wcTransform);
    gte_ldlv0(&rot.t);
    gte_nrtv0tr();
    gte_stlvnl(&scTransform.t);

    scale.z = (((instance->object->modelList[instance->currentModel]->maxRad) << 12) / 480 * (4096 - ((instance->position.z - instance->shadowPosition.z) << 12) / 1280)) >> 12;
    scale.y = scale.z;
    scale.x = scale.z;
    ScaleMatrix(&scTransform, (VECTOR *)&scale);
    SetRotMatrix(&scTransform);
    SetTransMatrix(&scTransform);
    primPool->nextPrim = DRAW_DrawShadow(primPool, 0, ot, instance->fadeValue);
}

void LIGHT_CalcLightValue(TFace *tface, Instance *instance, Terrain *terrain);
void LIGHT_CalcShadowPositions(struct GameTracker *gameTracker)
{
    struct _InstanceList *instanceList;
    struct _Instance *instance;
    struct _PCollideInfo pcollideInfo;
    struct _Position newPos;
    struct _Position oldPos;
    struct Level *level;

    instanceList = gameTracker->instanceList;

    for (instance = instanceList->first; instance; instance = instance->next)
    {
        if ((instance->flags2 & 0x40) == 0)
        {
            if (!(instance->flags2 & 0x40) && instance->tfaceLevel != NULL)
            {
                LIGHT_CalcLightValue(instance->tface, instance, ((struct Level *)instance->tfaceLevel)->terrain);
            }
        }
        else if ((instance->flags & 0xA00) == 0x200 && !(instance->flags2 & 0x4000000))
        {
            if ((instance->flags & 0x18000000) != 0x8000000)
            {
                if ((instance->flags & 0x10000000))
                {
                    newPos.x = (short)instance->matrix[1].t[0];
                    oldPos.x = newPos.x;

                    newPos.y = (short)instance->matrix[1].t[1];
                    oldPos.y = newPos.y;

                    newPos.z = (short)instance->matrix[1].t[2];
                    oldPos.z = newPos.z;
                }
                else
                {
                    newPos = instance->position;
                    oldPos = instance->position;
                }

                pcollideInfo.collideType = 55;
                pcollideInfo.newPoint = (SVECTOR *)&newPos;
                pcollideInfo.oldPoint = (SVECTOR *)&oldPos;
                pcollideInfo.instance = instance;

                newPos.z -= 1280;
                oldPos.z += 256;

                instance->flags |= 0x40u;

                level = STREAM_GetLevelWithID(instance->currentStreamUnitID);

                if (level)
                {
                    COLLIDE_PointAndWorld(&pcollideInfo, level);
                }
                else
                {
                    pcollideInfo.type = 0;
                }
                instance->flags &= ~0x40;
                if (pcollideInfo.type == 3)
                {
                    LIGHT_CalcLightValue((struct _TFace *)pcollideInfo.prim, instance, (struct _Terrain *)pcollideInfo.inst->node.prev);
                }
                else if (pcollideInfo.type != 5)
                {
                    LIGHT_CalcLightValue(NULL, instance, (struct _Terrain *)NULL);
                }

                if (pcollideInfo.type)
                {
                    if (pcollideInfo.type != 1)
                    {
                        if (pcollideInfo.type == 3
                            && (((struct _TFace *)pcollideInfo.prim)->textoff != 0xFFFF)
                            && (((struct TextureFT3 *)((char *)((struct _Terrain *)(pcollideInfo.inst->node.prev))->StartTextureList + ((struct _TFace *)pcollideInfo.prim)->textoff))->attr & 0x4000))
                        {
                            instance->flags |= 0x200000;
                        }
                        else
                        {
                            instance->flags &= ~0x200000;
                        }
                        instance->wNormal.x = pcollideInfo.wNormal.vx;
                        instance->wNormal.y = pcollideInfo.wNormal.vy;
                        instance->wNormal.z = pcollideInfo.wNormal.vz;
                    }
                    else
                    {
                        instance->wNormal.x = 0;
                        instance->wNormal.y = 0;
                        instance->wNormal.z = 4096;
                    }
                }
                instance->shadowPosition = newPos;
            }
            else
            {
                if (instance->waterFace != NULL)
                {
                    LIGHT_CalcLightValue(instance->waterFace, instance, instance->waterFaceTerrain);
                }
                else if (instance->tfaceLevel != NULL)
                {
                    LIGHT_CalcLightValue(instance->tface, instance, ((struct Level *)instance->tfaceLevel)->terrain);
                }
            }
        }
        else if (!(instance->flags2 & 0x40) && instance->tfaceLevel != NULL)
        {
            LIGHT_CalcLightValue(instance->tface, instance, ((struct Level *)instance->tfaceLevel)->terrain);
        }
        instance->flags &= ~0x8000000;
    }
}

void LIGHT_Restore()
{
}

void LIGHT_CalcDQPTable(Level *level)
{
    long dqa;
    long limit;

    if (level->fogFar != level->fogNear)
    {
        dqa = -((level->fogFar * level->fogNear) / (level->fogFar - level->fogNear));

        limit = 40958;

        if (dqa > limit)
        {
            dqa = limit;

            LIGHT3D_FogCalc(limit, level);
        }

        if (dqa < -limit)
        {
            dqa = -limit;

            LIGHT3D_FogCalc(-limit, level);
        }

        depthQFogStart = (4096 * -dqa) / LIGHT3D_FixedDivision(level->fogFar, level->fogNear);

        if ((level->backColorR != 0) || (level->backColorG != 0))
        {
            depthQBlendStart = depthQFogStart;
        }
        else
        {
            depthQBlendStart = 65535;
        }

        level->depthQFogStart = depthQFogStart;
        level->depthQBlendStart = depthQBlendStart;
    }
}
