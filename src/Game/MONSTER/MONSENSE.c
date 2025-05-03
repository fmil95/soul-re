#include "Game/MONSTER/MONSENSE.h"
#include "Game/MONSTER/MONAPI.h"
#include "Game/MATH3D.h"
#include "Game/PHYSOBS.h"
#include "Game/MONSTER/MONLIB.h"
#include "Game/PHYSICS.h"
#include "Game/GAMELOOP.h"
#include "Game/INSTANCE.h"

STATIC int gNumMonsters;

STATIC int gNumSpectralMonsters;

STATIC int gNumMaterialMonsters;

STATIC MonsterVars *monsterSenseArray[40];

STATIC int lastSenseFrame;

STATIC char monsterSensed[40];

STATIC RadarInfo radarDir[8];

STATIC unsigned char radarDirIndex[8];

MonsterIR *MONSENSE_FindIR(MonsterVars *mv, Instance *instance)
{
    MonsterIR *mir;

    for (mir = mv->monsterIRList; mir != NULL; mir = mir->next)
    {
        if (mir->instance == instance)
        {
            break;
        }
    }

    return mir;
}

INCLUDE_ASM("asm/nonmatchings/Game/MONSTER/MONSENSE", MONSENSE_See);

int MONSENSE_Hear(Instance *instance, evCollideInstanceStatsData *data)
{
    MonsterVars *mv;
    MonsterSenses *senses;
    long mode;

    mv = (MonsterVars *)instance->extraData;

    senses = mv->subAttr->senses;

    mode = INSTANCE_Query(data->instance, 10);

    if (((!(mode & 0x10B0143)) || ((mode & 0x200000))) && (data->distance < (unsigned)senses->soundRadius))
    {
        return MATH3D_ConeDetect(&data->relativePosition, senses->soundArc, senses->soundElevation);
    }

    return 0;
}

int MONSENSE_Smell(Instance *instance, evCollideInstanceStatsData *data)
{
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    return data->distance < mv->subAttr->senses->scentRadius;
}

INCLUDE_ASM("asm/nonmatchings/Game/MONSTER/MONSENSE", MONSENSE_FirstSense);

void MONSENSE_SetupMIR(Instance *instance, evCollideInstanceStatsData *data, int flags)
{
    MonsterIR *mir;
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    mir = MONSENSE_FindIR(mv, data->instance);

    if (mir == NULL)
    {
        if (flags != 0)
        {
            mir = MONSENSE_FirstSense(instance, data->instance);
        }
    }
    else if (flags != 0)
    {
        mir->forgetTimer = MON_GetTime(instance) + (mv->subAttr->forgetTime * 1000);
    }

    if (mir != NULL)
    {
        long mode;

        mir->mirFlags |= flags;

        mir->distance = data->distance;

        COPY_SVEC(SVector, &mir->relativePosition, SVector, &data->relativePosition);

        if ((mv->subAttr->combatAttributes != NULL) && (mv->subAttr->combatAttributes->combatRange > mir->distance))
        {
            mir->mirConditions |= 0x8;
        }
        else
        {
            mir->mirConditions &= ~0x8;
        }

        if (((short)((MATH3D_AngleFromPosToPos(&mir->instance->position, &instance->position) - mir->instance->rotation.z) + 1024) & 0xFFF) > 2048)
        {
            mir->mirConditions |= 0x2;
        }
        else
        {
            mir->mirConditions &= ~0x2;
        }

        mode = INSTANCE_Query(mir->instance, 10);

        if ((mode & 0x208000) == 0x208000)
        {
            mode &= ~0x200000;
        }

        if ((mode & 0x4))
        {
            mir->mirConditions |= 0x1;
        }
        else
        {
            mir->mirConditions &= ~0x1;
        }

        if ((mir->mirFlags & 0x400))
        {
            if (!(mode & 0x200000))
            {
                mir->mirFlags &= ~0x400;
                mir->mirConditions &= ~0x820;

                if (((mir->mirFlags & 0x1)) && (!(mir->mirConditions & 0x400)))
                {
                    mir->mirConditions |= 0x4;
                }
            }
        }
        else
        {
            if ((mode & 0x200000))
            {
                int run;
                int distance;

                run = mode & 0x4;

                if (mv->subAttr->combatAttributes != NULL)
                {
                    if (run != 0)
                    {
                        distance = mv->subAttr->combatAttributes->enemyRunAttackRange;
                    }
                    else
                    {
                        distance = mv->subAttr->combatAttributes->enemyAttackRange;
                    }

                    mir->mirConditions &= ~0xE4;
                    mir->mirFlags |= 0x400;

                    if ((mir->distance < distance) && (((short)((MATH3D_AngleFromPosToPos(&mir->instance->position, &instance->position) - mir->instance->rotation.z) + 512) & 0xFFF) < 2048))
                    {
                        if (run != 0)
                        {
                            mir->mirConditions |= 0x20;
                        }

                        mir->mirConditions |= 0x800;
                    }
                }
            }
        }

        if ((mir->mirFlags & 0x800))
        {
            if (mode != 1)
            {
                mir->mirFlags &= ~0x800;
                mir->mirConditions &= ~0x10;
            }
            else
            {
                do
                {
                    if ((!(mir->mirConditions & 0x10)) && (mir->idleTime < MON_GetTime(instance)))
                    {
                        mir->mirConditions |= 0x10;
                    }
                } while (0); // this loop isn't really necessary (it's garbage), however the if statement inside of it is
            }
        }
        else if (mode == 1)
        {
            mir->mirFlags |= 0x800;

            mir->idleTime = MON_GetTime(instance) + 4950;
        }

        if ((mir->mirFlags & 0x200))
        {
            if ((mir->distance > (mv->subAttr->combatAttributes->allyRange + 300)) || ((INSTANCE_Query(mir->instance, 0) & 0x44000000)))
            {
                mir->mirFlags &= ~0x200;
            }
        }
        else if (((mir->mirFlags & 0x2)) && (mir->distance < mv->subAttr->combatAttributes->allyRange))
        {
            mir->mirFlags |= 0x200;
        }
    }
}

void MONSENSE_SenseInstance(Instance *instance, evCollideInstanceStatsData *data)
{
    MonsterVars *mv;
    long flags;
    unsigned long whatAmI;

    flags = 0;

    mv = (MonsterVars *)instance->extraData;

    whatAmI = INSTANCE_Query(data->instance, 1);

    if (!(mv->mvFlags & 0x200))
    {
        if ((whatAmI & 0x20))
        {
            PhysObProperties *prop;

            prop = (PhysObProperties *)data->instance->data;

            if ((prop->family == 3) && ((((PhysObInteractProperties *)prop)->engageXYDistance >> 1) > data->xyDistance) && (((PhysObInteractProperties *)prop)->engageZMaxDelta > data->zDelta) && (((PhysObInteractProperties *)prop)->engageZMinDelta < data->zDelta))
            {
                if ((((PhysObInteractProperties *)prop)->conditions & 0x40))
                {
                    INSTANCE_Post(instance, 0x100000C, 0x20);
                }

                if ((((PhysObInteractProperties *)prop)->conditions & 0x8))
                {
                    INSTANCE_Post(instance, 0x100000C, 0x40);
                }

                if ((((PhysObInteractProperties *)prop)->conditions & 0x10))
                {
                    INSTANCE_Post(instance, 0x100000C, 0x10);
                }
            }
        }
        else if (mv->alertCount != 0)
        {
            if (MONSENSE_Smell(instance, data) != 0)
            {
                flags = 0x80;
            }

            if (MONSENSE_See(instance, data) != 0)
            {
                flags |= 0x20;
            }

            if (MONSENSE_Hear(instance, data) != 0)
            {
                flags |= 0x40;
            }

            if (((whatAmI & mv->subAttr->allegiances->allies)) && (data->distance < mv->subAttr->senses->allyRadius))
            {
                flags |= 0x2;
            }

            MONSENSE_SetupMIR(instance, data, flags);
        }
    }
}

void MONSENSE_StartMonsterIRList(Instance *instance)
{
    MonsterVars *mv;
    MonsterIR *mir;
    MonsterIR *newlist;
    int enemyOnList;
    char temp; // not from decls.h

    temp = 0;

    mv = (MonsterVars *)instance->extraData;

    newlist = NULL;

    enemyOnList = 0;

    if (mv->alertCount != 0)
    {
        MonsterIR *next;

        for (mir = mv->monsterIRList; mir != NULL; mir = next)
        {
            next = mir->next;

            if ((mir->handle != mir->instance->instanceID) || ((!(mir->mirFlags & 0xE0)) && (mir->forgetTimer < MON_GetTime(instance))))
            {
                if (mv != NULL) // garbage code for reodering
                {
                    temp = -temp;
                }

                mir->next = mv->freeIRs;

                mv->freeIRs = mir;
            }
            else
            {
                mir->next = newlist;

                newlist = mir;

                newlist->mirFlags &= ~0xE0;

                if (mv->enemy == newlist)
                {
                    enemyOnList = 1;
                }
            }
        }

        if (enemyOnList == 0)
        {
            mv->enemy = NULL;
        }

        mv->monsterIRList = newlist;
    }
}

MonsterIR *MONSENSE_GetMonsterIR(MonsterVars *mv)
{
    MonsterIR *mir;

    if (mv->freeIRs != NULL)
    {
        mir = mv->freeIRs;

        mv->freeIRs = mir->next;
    }
    else
    {
        mir = NULL;
    }

    return mir;
}

void MONSENSE_InitIRList(MonsterVars *mv, MonsterIR *list, int num)
{
    int i;

    i = num;

    mv->freeIRs = list;

    while (--i != 0)
    {
        list->next = list + 1;

        list = list->next;
    }

    list->next = NULL;

    mv->monsterIRList = NULL;
}

INCLUDE_ASM("asm/nonmatchings/Game/MONSTER/MONSENSE", MONSENSE_SetEnemy);

INCLUDE_ASM("asm/nonmatchings/Game/MONSTER/MONSENSE", MONSENSE_ProcessIRList);

void MONSENSE_SetupSenses(Instance *instance)
{
    MonsterAllegiances *allegiances;
    MonsterVars *mv;
    int num;

    mv = (MonsterVars *)instance->extraData;

    allegiances = mv->subAttr->allegiances;

    MONSENSE_InitIRList(mv, (MonsterIR *)&mv[1], 5); // double-check

    instance->flags2 |= 0x400;

    if (allegiances != NULL)
    {
        instance->checkMask = allegiances->enemies | allegiances->allies | allegiances->food | allegiances->gods;
    }

    instance->maxCheckDistance = 12000;

    instance->checkMask |= 0xB;

    mv->enemy = NULL;

    mv->leader = NULL;

    monsterSenseArray[gNumMonsters] = mv;

    mv->senseIndex = gNumMonsters;

    if (gNumMonsters == 0)
    {
        lastSenseFrame = -1;
    }

    gNumMonsters++;

    if (((((MonsterAttributes *)instance->data)->whatAmI) & 0x1804))
    {
        num = ++gNumSpectralMonsters;
    }
    else
    {
        num = ++gNumMaterialMonsters;
    }

    if ((num > 6) && ((instance->parent == NULL) && (instance->currentStreamUnitID != instance->birthStreamUnitID)))
    {
        instance->flags |= 0x20;
        instance->flags2 |= 0x20000;
    }
}

void MONSENSE_RemoveSenses(Instance *instance)
{
    MonsterVars *mv;
    int i;

    mv = (MonsterVars *)instance->extraData;

    gNumMonsters--;

    if ((((MonsterAttributes *)instance->data)->whatAmI & 0x1804))
    {
        gNumSpectralMonsters--;
    }
    else
    {
        gNumMaterialMonsters--;
    }

    for (i = mv->senseIndex; i < gNumMonsters; i++)
    {
        monsterSenseArray[i] = monsterSenseArray[i + 1];

        monsterSenseArray[i]->senseIndex = i;
    }
}

void MONSENSE_Radar(Instance *instance)
{
    MonsterVars *mv;
    SVECTOR newPos;
    SVECTOR oldPos;
    PCollideInfo pCollideInfo;
    int index;

    mv = (MonsterVars *)instance->extraData;

    index = mv->radarCounter[radarDirIndex];

    newPos.vx = instance->position.x + radarDir[index].x;
    newPos.vy = instance->position.y + radarDir[index].y;
    newPos.vz = instance->position.z + 100;

    oldPos.vx = instance->position.x;
    oldPos.vy = instance->position.y;
    oldPos.vz = instance->position.z + 100;

    pCollideInfo.newPoint = &newPos;
    pCollideInfo.oldPoint = &oldPos;

    pCollideInfo.collideType = 0x13F;

    PHYSICS_CheckLineInWorldMask(instance, &pCollideInfo);

    if (pCollideInfo.type == 0)
    {
        oldPos.vx = instance->position.x + radarDir[index].x;
        oldPos.vy = instance->position.y + radarDir[index].y;
        oldPos.vz = instance->position.z - 256;

        newPos.vx = instance->position.x;
        newPos.vy = instance->position.y;
        newPos.vz = instance->position.z - 256;

        PHYSICS_CheckLineInWorld(instance, &pCollideInfo);

        if (pCollideInfo.type == 0)
        {
            mv->radarMap |= 0x1 << index;

            mv->radarDistance[index] = 1280;

            mv->radarCounter++;

            if (mv->radarCounter >= 8)
            {
                mv->radarCounter = 0;
            }

            return;
        }
    }

    mv->radarMap &= ~(0x1 << index);

    if ((pCollideInfo.type == 3) && ((((Level *)pCollideInfo.inst)->terrain->BSPTreeArray[pCollideInfo.segment].flags & 0x2)))
    {
        mv->radarDistance[index] = 0;
    }
    else
    {
        mv->radarDistance[index] = MATH3D_LengthXY(newPos.vx - instance->position.x, newPos.vy - instance->position.y);
    }

    mv->radarCounter++;

    if (mv->radarCounter >= 8)
    {
        mv->radarCounter = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/Game/MONSTER/MONSENSE", MONSENSE_GetClosestFreeDirection);

int MONSENSE_GetDistanceInDirection(Instance *instance, short angle)
{
    MonsterVars *mv;
    int bit;

    bit = (angle + 256) & 0xFFF;

    mv = (MonsterVars *)instance->extraData;

    return mv->radarDistance[bit / 512];
}

extern int D_800D1B2C;
void MONSENSE_DoSenses(Instance *instance)
{
    MonsterVars *mv;
    // static int doneThisFrame;

    mv = (MonsterVars *)instance->extraData;

    MONSENSE_ProcessIRList(instance);

    if (lastSenseFrame < (long)gameTrackerX.frameCount)
    {
        if (lastSenseFrame < (long)(gameTrackerX.frameCount - 1))
        {
            memset(&monsterSensed, 0, sizeof(monsterSensed));

            lastSenseFrame = gameTrackerX.frameCount - 1;
        }

        // doneThisFrame = 0;
        D_800D1B2C = 0;
    }

    // if ((doneThisFrame == 0) && (monsterSensed[mv->senseIndex] == 0))
    if ((D_800D1B2C == 0) && (monsterSensed[(int)mv->senseIndex] == 0))
    {
        lastSenseFrame = gameTrackerX.frameCount;

        monsterSensed[(int)mv->senseIndex] = 1;

        // doneThisFrame = 1;
        D_800D1B2C = 1;

        mv->alertCount = 1;

        if (!(mv->auxFlags & 0x20000000))
        {
            MONSENSE_Radar(instance);
        }
    }
    else
    {
        mv->alertCount = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/Game/MONSTER/MONSENSE", MONSENSE_AdjustRadarFromObjects);
