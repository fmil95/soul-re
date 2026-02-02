#include "Game/MONSTER/MBMISS.h"
#include "Game/GAMELOOP.h"
#include "Game/PHYSOBS.h"
#include "Game/STATE.h"
#include "Game/MONSTER/MONLIB.h"
#include "Game/PSX/SUPPORT.h"
#include "Game/OBTABLE.h"
#include "Game/MATH3D.h"
#include "Game/INSTANCE.h"
#include "Game/COLLIDE.h"
#include "Game/PSX/MAIN.h"

void WCBEGG_Message(Instance *instance, unsigned long message, unsigned long data)
{
    // PhysObData *pod; unused
    Dummy *temp; // not from decls.h

    if (message == 0x800002)
    {
        if (instance->processFunc != WCBEGG_Process)
        {
            return;
        }

        temp = (Dummy *)instance->extraData; // extraData needs parsing to the correct struct

        temp->unknown = MON_GetTime(instance);

        G2EmulationInstanceSwitchAnimationAlpha(instance, 0, 1, 0, 0, 2, 0);
    }

    PhysicalObjectPost(instance, message, data);
}

int WCBEGG_ShouldIgniteEgg(Instance *egg, WalbossAttributes *wa)
{
    InstanceList *instanceList;
    Instance *instance;

    instanceList = gameTrackerX.instanceList;

    if (!(INSTANCE_Query(egg, 3) & 0x10000))
    {
        instance = instanceList->first;

        while (instance != NULL)
        {
            if (((INSTANCE_Query(instance, 1) & 0x20)) && (MATH3D_LengthXYZ(instance->position.x - egg->position.x,
                                                                            instance->position.y - egg->position.y, instance->position.z - egg->position.z) < wa->eggIgniteDist))
            {
                if ((INSTANCE_Query(instance, 3) & 0x10000))
                {
                    return 1;
                }

                if (strcmpi(instance->object->name, "walfire_") == 0)
                {
                    return 1;
                }
            }

            instance = instance->next;
        }
    }

    return 0;
}

void WCBEGG_Process(Instance *instance, GameTracker *gameTracker)
{
    PhysObData *data;
    int time;
    int lastTime;
    int timeBetween;
    // int birthTime; unused
    Object *walboss;
    WalbossAttributes *wa;

    data = instance->extraData;

    time = MON_GetTime(instance);

    lastTime = time - gameTrackerX.lastLoopTime;

    if ((instance->collideFunc == WCBEGG_Collide) && (instance->LinkParent == gameTrackerX.playerInstance))
    {
        instance->collideFunc = WCBEGG_ExplodeCollide;
    }

    walboss = OBTABLE_FindObject("walboss_");

    if (walboss != NULL)
    {
        wa = (WalbossAttributes *)(intptr_t)((Dummy2 *)walboss->data)->unknown; // walboss->data needs parsing to the correct struct

        timeBetween = ((Dummy *)data)->unknown + (wa->timeToEggThrob * 33); // data needs parsing to the correct struct

        if ((lastTime < timeBetween) && (time >= timeBetween))
        {
            G2EmulationInstanceSwitchAnimationAlpha(instance, 0, 2, 0, 0, 2, 0);
        }

        timeBetween += wa->timeToEggExplode * 33;

        if ((lastTime < timeBetween) && (time >= timeBetween))
        {
            if ((INSTANCE_Query(instance, 3) & 0x10000))
            {
                G2EmulationInstanceSwitchAnimationAlpha(instance, 0, 4, 0, 0, 1, 0);

                instance->processFunc = WCBEGG_ExplodeProcess;
            }
            else
            {
                G2EmulationInstanceSwitchAnimationAlpha(instance, 0, 3, 0, 0, 1, 0);

                instance->processFunc = WCBEGG_SplitProcess;
            }

            ((Dummy *)data)->unknown = MON_GetTime(instance);
        }

        if (WCBEGG_ShouldIgniteEgg(instance, wa) != 0)
        {
            INSTANCE_Post(instance, 0x800029, 1);
        }

        ProcessPhysicalObject(instance, gameTracker);
        return;
    }

    INSTANCE_UnlinkFromParent(instance);

    INSTANCE_KillInstance(instance);
}

void WCBEGG_ExplodeProcess(Instance *instance, GameTracker *gameTracker)
{
    PhysObData *data;
    int currentTime;
    int time;
    Object *walboss;
    WalbossAttributes *wa;

    (void)time;

    data = instance->extraData;

    currentTime = MON_GetTime(instance);

    if ((instance->collideFunc == WCBEGG_Collide) && (instance->LinkParent == gameTrackerX.playerInstance))
    {
        instance->collideFunc = WCBEGG_ExplodeCollide;
    }

    walboss = OBTABLE_FindObject("walboss_");

    if (walboss != NULL)
    {
        wa = (WalbossAttributes *)(intptr_t)((Dummy2 *)walboss->data)->unknown; // walboss->data needs parsing to the correct struct

        if (currentTime >= (((Dummy *)data)->unknown + 330)) // data needs parsing to the correct struct
        {
            if (gameTrackerX.playerInstance == instance->LinkParent)
            {
                INSTANCE_Post(gameTrackerX.playerInstance, 0x40005, wa->razielStunTime * 33);

                INSTANCE_UnlinkFromParent(instance);

                data->Mode = (data->Mode | 0x1001) & ~0x4000;

                instance->processFunc = WCBEGG_CommonPostProcess;
            }
            else
            {
                instance->processFunc = WCBEGG_CommonPostProcess2;
            }

            INSTANCE_Post(instance, 0x800029, 0);
        }

        ProcessPhysicalObject(instance, gameTracker);
        return;
    }

    INSTANCE_UnlinkFromParent(instance);

    INSTANCE_KillInstance(instance);
}

void WCBEGG_SplitProcess(Instance *instance, GameTracker *gameTracker)
{
    PhysObData *data;
    int currentTime;
    int time;
    Object *walboss;
    WalbossAttributes *wa;

    data = (PhysObData *)instance->extraData;

    currentTime = MON_GetTime(instance);

    walboss = OBTABLE_FindObject("walboss_");

    if (walboss != NULL)
    {
        wa = (WalbossAttributes *)(intptr_t)((Dummy2 *)walboss->data)->unknown; // walboss->data needs parsing to the correct struct

        if (currentTime >= (((Dummy *)data)->unknown + 198)) // data needs parsing to the correct struct
        {
            if (gameTrackerX.playerInstance == instance->LinkParent)
            {
                time = wa->razielStunTime * 33;

                INSTANCE_Post(gameTrackerX.playerInstance, 0x40005, time);

                INSTANCE_UnlinkFromParent(instance);

                data->Mode = (data->Mode | 0x1001) & ~0x4000;

                instance->processFunc = WCBEGG_CommonPostProcess;
            }
            else
            {
                instance->processFunc = WCBEGG_CommonPostProcess2;
            }
        }

        ProcessPhysicalObject(instance, gameTracker);
        return;
    }

    INSTANCE_UnlinkFromParent(instance);

    INSTANCE_KillInstance(instance);
}

void WCBEGG_CommonPostProcess(Instance *instance, GameTracker *gameTracker)
{
    // PhysObData *data; unused
    Dummy *temp; // not from decls.h

    temp = (Dummy *)instance->extraData; // extraData needs parsing to the correct struct

    INSTANCE_Post(gameTrackerX.playerInstance, 0x40003, SetActionPlayHostAnimationData(gameTrackerX.playerInstance, instance, 6, 0, 0, 2));

    instance->processFunc = WCBEGG_CommonPostProcess2;

    temp->unknown = MON_GetTime(instance);

    ProcessPhysicalObject(instance, gameTracker);
}

void WCBEGG_CommonPostProcess2(Instance *instance, GameTracker *gameTracker)
{
    // PhysObData *data; unused
    int time;
    Dummy *temp; // not from decls.h

    temp = (Dummy *)instance->extraData; // extraData needs parsing to the correct struct

    time = 3960;

    if ((long)MON_GetTime(instance) >= (temp->unknown + time))
    {
        INSTANCE_UnlinkFromParent(instance);

        INSTANCE_KillInstance(instance);
    }
    else
    {
        ProcessPhysicalObject(instance, gameTracker);
    }
}

void WCBEGG_ExplodeCollide(Instance *instance, GameTracker *gameTracker)
{
    CollideInfo *collideInfo;
    HSphere *s1;
    Instance *inst1;
    PhysObData *data;
    HSphere *temp; // not from decls.h

    collideInfo = instance->collideInfo;

    data = (PhysObData *)instance->extraData;

    s1 = (HSphere *)((uintptr_t)(unsigned char)collideInfo->type1);

    temp = collideInfo->prim1;

    if (((uintptr_t)s1 == 1) && (temp->id == 8))
    {
        inst1 = collideInfo->inst1;

        if ((inst1 != NULL) && (INSTANCE_Query(inst1, 1) == 0x10102))
        {
            if (instance->LinkParent == NULL)
            {
                if ((data->Mode & 0x10000))
                {
                    INSTANCE_Post(inst1, 0x1000017, 2);

                    G2EmulationInstanceSwitchAnimationAlpha(instance, 0, 4, 0, 0, (uintptr_t)s1, 0);

                    instance->processFunc = WCBEGG_ExplodeProcess;
                }
                else
                {
                    G2EmulationInstanceSwitchAnimationAlpha(instance, 0, 3, 0, 0, (uintptr_t)s1, 0);

                    instance->processFunc = WCBEGG_SplitProcess;
                }

                data->Mode = (data->Mode | 0x1001) & ~0x4000;

                instance->zVel = 0;
                instance->yVel = 0;
                instance->xVel = 0;

                ((Dummy *)data)->unknown = MON_GetTime(instance); // data needs parsing to the correct struct
            }

            TurnOffCollisionPhysOb(instance, 7);
            return;
        }
    }

    CollidePhysicalObject(instance, gameTracker);

    data->zRotVel = 0;
    data->yRotVel = 0;
    data->xRotVel = 0;

    data->throwFlags &= ~0x1;
}

void WCBEGG_Collide(Instance *instance, GameTracker *gameTracker)
{
    CollideInfo *collideInfo;
    Instance *inst1;

    collideInfo = (CollideInfo *)instance->collideInfo;

    if (((unsigned char)collideInfo->type1 != 1) || (inst1 = (Instance *)collideInfo->inst1, strcmpi(inst1->object->name, "walboss_") != 0))
    {
        TurnOffCollisionPhysOb(instance, 7);

        instance->collideFunc = WCBEGG_ExplodeCollide;
    }

    CollidePhysicalObject(instance, gameTracker);
}

long WALBOSC_AnimCallback(G2Anim *anim, int sectionID, G2AnimCallbackMsg message, long messageDataA, long messageDataB, Instance *instance)
{
    if (message == G2ANIM_MSG_DONE)
    {
        COLLIDE_SegmentCollisionOff(instance, 1);

        instance->flags |= 0x800;

        instance->flags2 |= 0x20000000;

        instance->halvePlane.flags = 0;
    }
    else
    {
        INSTANCE_DefaultAnimCallback(anim, sectionID, message, messageDataA, messageDataB, instance);
    }

    return messageDataA;
}

void WALBOSC_Collide(Instance *instance, GameTracker *gameTracker)
{
    CollideInfo *collideInfo;
    HSphere *s1;
    Instance *inst1;

    (void)gameTracker;

    collideInfo = (CollideInfo *)instance->collideInfo;

    inst1 = collideInfo->inst1;

    s1 = collideInfo->prim1;

    if (s1->id == 8)
    {
        PhysObWeaponProperties *Prop;

        Prop = (PhysObWeaponProperties *)instance->data;

        COLLIDE_SegmentCollisionOff(instance, 1);

        INSTANCE_Post(inst1, 0x400000, SetFXHitData(instance, (unsigned char)collideInfo->segment, Prop->WeaponAttributes.Damage / 128, 256));
        INSTANCE_Post(inst1, 0x1000000, SetMonsterHitData(instance, NULL, Prop->WeaponAttributes.Damage, Prop->WeaponAttributes.knockBackDistance, Prop->WeaponAttributes.knockBackFrames));
    }
}

void WALBOSC_Message(Instance *instance, unsigned long message, unsigned long data)
{
    if ((message == 0x1000017) && (data == 0))
    {
        COLLIDE_SegmentCollisionOff(instance, 1);

        instance->flags2 |= 0x20000000;
    }
}
