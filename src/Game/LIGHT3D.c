#include "common.h"
#include "Game/HASM.h"

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

INCLUDE_ASM("asm/nonmatchings/Game/LIGHT3D", LIGHT_GetLightMatrix);

INCLUDE_ASM("asm/nonmatchings/Game/LIGHT3D", LIGHT_PresetInstanceLight);

INCLUDE_ASM("asm/nonmatchings/Game/LIGHT3D", LIGHT_GetAmbient);

INCLUDE_ASM("asm/nonmatchings/Game/LIGHT3D", LIGHT_CalcLightValue);

void LIGHT_SetAmbientInstance(Instance *instance, Level *level)
{
    (void)level;
    SetBackColor(((ColorType *)&instance->light_color)->r, ((ColorType *)&instance->light_color)->g, ((ColorType *)&instance->light_color)->b);
}

INCLUDE_ASM("asm/nonmatchings/Game/LIGHT3D", LIGHT_SetMatrixForLightGroupInstance);

INCLUDE_ASM("asm/nonmatchings/Game/LIGHT3D", LIGHT_DrawShadow);

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
