#include "Game/REAVER.h"
#include "Game/GAMELOOP.h"
#include "Game/MATH3D.h"
#include "Game/PHYSOBS.h"
#include "Game/STATE.h"
#include "Game/GAMEPAD.h"
#include "Game/MEMPACK.h"
#include "Game/COLLIDE.h"
#include "Game/FX.h"
#include "Game/INSTANCE.h"
#include "Game/STREAM.h"
#include "Game/PSX/MAIN.h"

static short FireReaverFlag;

void SoulReaverInit(Instance *instance, GameTracker *gameTracker)
{
    ReaverData *data;

    (void)gameTracker;
    if ((instance->flags & 0x20000))
    {
        data = (ReaverData *)instance->extraData;

        MEMPACK_Free((char *)data);
    }
    else
    {
        data = (ReaverData *)MEMPACK_Malloc(sizeof(ReaverData), 30);

        instance->extraData = (void *)data;

        FireReaverFlag = 0;

        data->CurrentReaver = 2;

        data->ReaverOn = 1;
        data->ReaverChargeTime = 0;
        data->ReaverShockAmount = 0;
        data->ReaverPickedUp = 0;
        data->ReaverSize = 4096;
        data->ReaverDeg = 0;
        data->ReaverScale = 4096;
        data->ReaverTargetScale = 4096;

        COLLIDE_SegmentCollisionOff(instance, 0);
    }

    FX_ReaverBladeInit();
}

void SoulReaverCollide(Instance *instance, GameTracker *gameTracker)
{
    CollideInfo *collideInfo;
    HSphere *S;
    Instance *target;
    ReaverData *reaverData;
    long type;
    Instance *inst;

    (void)gameTracker;
    collideInfo = (CollideInfo *)instance->collideInfo;

    if ((collideInfo->type0 == 1) && (collideInfo->type1 == 1))
    {
        S = (HSphere *)collideInfo->prim0;

        if (S->id == 9)
        {
            S = (HSphere *)collideInfo->prim1;

            if (S->id == 8)
            {
                type = 0;

                target = (Instance *)collideInfo->inst1;

                COLLIDE_SegmentCollisionOff(instance, 0);

                reaverData = (ReaverData *)instance->extraData;

                switch (reaverData->CurrentReaver)
                {
                case 1:
                case 2:
                    type = 4096;
                    break;
                case 6:
                    type = 32;
                    break;
                }

                if (instance->LinkParent != NULL)
                {
                    COLLIDE_SegmentCollisionOff(instance, 0);

                    INSTANCE_Post(gameTrackerX.playerInstance, 0x100001F, SetMonsterHitData(instance->LinkParent, target, type, 0, 0));
                    INSTANCE_Post(target, 0x400000, SetFXHitData(instance, (unsigned char)collideInfo->segment, 50, type));
                }
            }
        }
    }

    if ((unsigned)collideInfo->type1 != 3)
    {
        inst = (Instance *)collideInfo->inst1;

        inst->flags |= 0x4;
    }
    else
    {
        COLLIDE_SetBSPTreeFlag(collideInfo, 0x800);
    }
}

void SoulReaverProcess(Instance *instance, GameTracker *gameTracker)
{
    ReaverData *data;

    (void)gameTracker;
    data = (ReaverData *)instance->extraData;

    if ((unsigned)data->ReaverPickedUp != 0)
    {
        instance->currentStreamUnitID = gameTrackerX.playerInstance->currentStreamUnitID;
    }

    _SoulReaverAnimate(instance);
}

void CollideReaverProjectile(Instance *instance, GameTracker *gameTracker)
{
    CollideInfo *collideInfo;
    Instance *target;
    long type;
    long reavType;
    Level *level;
    Instance *inst;

    collideInfo = instance->collideInfo;

    target = collideInfo->inst1;

    type = *(int *)((intptr_t)instance->extraData + 4) + -2; // extraData needs parsing to the correct struct

    reavType = 0;

    if ((collideInfo->type0 == 1) && (collideInfo->type1 == 1))
    {
        switch (type)
        {
        case 1:
        case 2:
            reavType = 4096;
            break;
        case 6:
            level = STREAM_GetLevelWithID(instance->currentStreamUnitID);

            reavType = 32;

            if ((level != NULL) && (instance->position.z < level->waterZLevel))
            {
                return;
            }

            break;
        }

        if (reavType != 0)
        {
            INSTANCE_Post(target, 0x1000021, SetMonsterHitData(instance, NULL, reavType, 0, 0));
            INSTANCE_Post(target, 0x400000, SetFXHitData(instance, 1, 50, reavType));
        }
    }

    if ((unsigned char)collideInfo->type1 != 3)
    {
        inst = collideInfo->inst1;

        inst->flags |= 0x4;
    }
    else
    {
        COLLIDE_SetBSPTreeFlag(collideInfo, 2048);
    }

    CollidePhysicalObject(instance, gameTracker);
}

unsigned long SoulReaverQuery(Instance *instance, unsigned long query)
{
    ReaverData *reaverData;
    unsigned long retval;

    switch (query)
    {
    case 1:
        return 0x20000;
    case 4:
        return 0x1000;
    case 40:
        reaverData = (ReaverData *)instance->extraData;

        if ((unsigned)reaverData->ReaverOn != 0)
        {
            if (reaverData->ReaverTargetScale != 0)
            {
                retval = 3;
            }
            else
            {
                retval = 1;
            }

            return retval;
        }
    default:
        retval = 0;
    }

    return retval;
}

void SoulReaverImbue(Instance *instance, int number)
{
    long color;
    ReaverTuneData *tuneData;

    tuneData = (ReaverTuneData *)instance->data;

    color = ((long *)tuneData)[number - 1];

    FX_DoBlastRing(instance, (SVector *)&instance->position, instance->matrix, 0, 320, 0, 240, 0, 0, -65536, 0, 0, 160, 320, color, 0, 0, 20, 1);
}

void SoulReaverCharge(Instance *instance, ReaverData *data)
{
    long color;
    long shock;
    ReaverTuneData *tuneData;

    tuneData = (ReaverTuneData *)instance->data;

    if (data->ReaverChargeTime != 0)
    {
        data->ReaverChargeTime -= gameTrackerX.timeMult;

        if (data->ReaverShockAmount <= 245759)
        {
            data->ReaverShockAmount += gameTrackerX.timeMult;

            shock = (data->ReaverShockAmount / 4096) + 50;
        }
        else
        {
            shock = 110;
        }

        GAMEPAD_Shock1(shock, 20480);

        if (data->ReaverChargeTime <= 0)
        {
            data->ReaverChargeTime += 61440;

            color = ((long *)tuneData)[data->CurrentReaver - 1];

            FX_DoBlastRing(instance, (SVector *)&instance->position, instance->matrix, 0, 360, 0, 0, 0, 0, 0xFFFF0000, 0, 320, 272, 224, color, 0, 0, -1, 1);
        }
    }
}

void StopSoulReaverCharge(ReaverData *data, Instance *instance)
{
    data->ReaverChargeTime = 0;
    data->ReaverShockAmount = 0;

    GAMEPAD_Shock1(0, 0);

    FX_EndInstanceEffects(instance);
}

void SoulReaverPost(Instance *instance, unsigned long message, unsigned long data)
{
    ReaverData *reaverData;

    reaverData = (ReaverData *)instance->extraData;

    switch (message)
    {
    case 0x800002:
        if (instance->LinkParent == NULL)
        {
            INSTANCE_LinkToParent(instance, (Instance *)data, 41);
        }

        reaverData->ReaverOn = 1;
        reaverData->ReaverPickedUp = 1;

        instance->flags &= ~0x800;
        break;
    case 0x800100:
        reaverData->ReaverOn = 1;

        if (reaverData->ReaverTargetScale == 0)
        {
            reaverData->ReaverTargetScale = 4096;
        }

        instance->flags &= ~0x800;
        break;
    case 0x800101:
        reaverData->ReaverOn = 0;

        StopSoulReaverCharge(reaverData, instance);

        instance->flags |= 0x800;
        break;
    case 0x800103:
        reaverData->CurrentReaver = data;

        if ((short)data == 6)
        {
            FireReaverFlag = 1;
        }
        else
        {
            FireReaverFlag = 0;
        }

        SoulReaverImbue(instance, data);
        break;
    case 0x800104:
        reaverData->ReaverChargeTime = 61440;

        INSTANCE_Broadcast(instance, 32, 0x800028, SetObjectAbsorbData(instance, 0, 60));
        break;
    case 0x800107:
        reaverData->ReaverTargetScale = data;
        break;
    case 0x800010:
    {
        SVECTOR oldVector;
        SVECTOR startPos;

        StopSoulReaverCharge(reaverData, instance);

        oldVector.vx = 0;
        oldVector.vy = 0;
        oldVector.vz = 400;

        ApplyMatrixSV(instance->matrix, &oldVector, &startPos);

        startPos.vx += instance->position.x;
        startPos.vy += instance->position.y;
        startPos.vz += instance->position.z;

        {
            ReaverData *reaverData;
            evObjectBirthProjectileData *pData;

            reaverData = (ReaverData *)instance->extraData;

            pData = PHYSOB_BirthProjectile(instance->LinkParent, 0, reaverData->CurrentReaver + 2);

            if (pData->birthInstance != NULL)
            {
                pData->birthInstance->collideFunc = CollideReaverProjectile;

                COPY_SVEC(Position, &pData->birthInstance->position, SVector, (SVector *)&startPos);

                INSTANCE_Post(pData->birthInstance, 0x800010, data);

                COPY_SVEC(Position, &pData->birthInstance->position, SVector, (SVector *)&startPos);
            }
        }
        break;
    }
    case 0x200002:
        COLLIDE_SegmentCollisionOn(instance, 0);
        break;
    case 0x200003:
        COLLIDE_SegmentCollisionOff(instance, 0);
        break;
    case 0x800105:
        StopSoulReaverCharge(reaverData, instance);
        break;
    case 0x800108:
        reaverData->ReaverTargetScale = 4096;

        instance->flags &= ~0x800;
        break;
    case 0x800109:
        StopSoulReaverCharge(reaverData, instance);

        reaverData->ReaverTargetScale = 0;
        break;
    }
}

unsigned long REAVER_GetGlowColor(Instance *instance)
{
    ReaverTuneData *tuneData;
    ReaverData *data;

    data = (ReaverData *)instance->extraData;

    tuneData = (ReaverTuneData *)instance->data;

    return ((unsigned long *)tuneData)[data->CurrentReaver - 1];
}

void _SoulReaverAnimate(Instance *instance)
{
    ReaverData *data;
    ReaverTuneData *tuneData;

    data = (ReaverData *)instance->extraData;

    tuneData = (ReaverTuneData *)instance->data;

    FX_SetReaverInstance(instance);

    data->ReaverDeg += 256 + (rand() & 31);

    data->ReaverDeg &= 0xFFF;

    if ((unsigned char)data->ReaverOn == 1)
    {
        if (data->ReaverSize < 4096)
        {
            data->ReaverSize += 256;
        }
    }
    else if (data->ReaverSize > 0)
    {
        data->ReaverSize -= 256;
    }

    if (data->CurrentReaver > 0)
    {
        unsigned long *temp; // not from decls.h

        temp = &tuneData->spectralInnerColor;

        data->ReaverBladeColor = temp[data->CurrentReaver - 1];

        temp = &tuneData->spectralInnerGlowColor;

        data->ReaverBladeGlowColor = temp[data->CurrentReaver - 1];

        temp = &tuneData->spectralGlowColor;

        data->ReaverGlowColor = temp[data->CurrentReaver - 1];
    }

    if (abs_diff(data->ReaverTargetScale, data->ReaverScale) <= 128)
    {
        data->ReaverScale = data->ReaverTargetScale;
    }
    else
    {
        if (data->ReaverScale > data->ReaverTargetScale)
        {
            data->ReaverScale -= 128;
        }

        if (data->ReaverScale < data->ReaverTargetScale)
        {
            data->ReaverScale += 128;
        }
    }

    if (((unsigned char)data->ReaverOn == 1) && (data->ReaverScale > 0))
    {
        instance->flags &= ~0x800;
    }
    else
    {
        instance->flags |= 0x800;
    }

    SoulReaverCharge(instance, data);
}

int SoulReaverFire()
{
    return FireReaverFlag;
}
