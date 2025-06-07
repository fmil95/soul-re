#include "Game/SPLINE.h"
#include "Game/GAMELOOP.h"
#include "Game/SAVEINFO.h"
#include "Game/SCRIPT.h"
#include "Game/MEMPACK.h"
#include "Game/MATH3D.h"
#include "Game/LIGHT3D.h"
#include "Game/PSX/SUPPORT.h"
#include "Game/G2/ANIMG2.h"
#include "Game/G2/INSTNCG2.h"
#include "Game/G2/QUATG2.h"
#include "Game/MONSTER/HUMAN.h"
#include "Game/CAMERA.h"
#include "Game/INSTANCE.h"
#include "Game/LIST.h"
#include "Game/EVENT.h"
#include "Game/FX.h"
#include "Game/OBTABLE.h"
#include "Game/SOUND.h"
#include "Game/STREAM.h"
#include "Game/STATE.h"
#include "Game/G2/ANMG2ILF.h"
#include "Game/FONT.h"
#include "Game/SIGNAL.h"

void INSTANCE_Deactivate(Instance *instance)
{
    Object *object;

    object = instance->object;

    if (!(gameTrackerX.streamFlags & 0x2000000))
    {
        instance->flags2 |= 0x1;

        if ((instance->flags2 & 0x20000000))
        {
            instance->flags |= 0x40000;
        }
        else
        {
            instance->flags &= ~0x40000;
            instance->flags2 |= 0x20000000;
        }

        if ((object->animList != NULL) && (!(object->oflags2 & 0x40000000)))
        {
            G2Anim_Free(&instance->anim);
        }

        instance->waterFace = NULL;
        instance->waterFaceTerrain = NULL;
    }
}

void INSTANCE_Reactivate(Instance *instance)
{
    Object *object;

    object = instance->object;

    instance->flags2 &= ~0x1;

    if ((instance->flags & 0x40000))
    {
        instance->flags &= ~0x40000;
        instance->flags2 |= 0x20000000;
    }
    else
    {
        instance->flags2 &= ~0x20000000;
    }

    if (object->animList != NULL)
    {
        if (!(object->oflags2 & 0x40000000))
        {
            G2Anim_Restore(&instance->anim);
        }
    }
}

void INSTANCE_ForceActive(Instance *instance)
{
    if ((instance->flags2 & 0x1))
    {
        INSTANCE_Reactivate(instance);
    }
}

void INSTANCE_DeactivatedProcess()
{
}

void INSTANCE_DeactivateFarInstances(GameTracker *gameTracker)
{
    InstanceList *instanceList;
    Instance *instance;
    SVECTOR *line;
    long distSq;
    int numInstances;
    int numToProcess;
    int cntInst;
    static int lastInst = 0;

    line = (SVECTOR *)getScratchAddr(0);

    instanceList = gameTracker->instanceList;
    numInstances = instanceList->numInstances;
    instance = instanceList->first;
    numToProcess = (numInstances >> 3) + 1;

    if (numInstances <= lastInst)
    {
        lastInst = 0;
    }

    cntInst = lastInst;
    lastInst = lastInst + numToProcess;

    if (numInstances <= lastInst)
    {
        lastInst = 0;
    }

    while (cntInst != 0)
    {
        instance = instance->next;
        cntInst--;
    }

    for (; instance != NULL; instance = instance->next, numToProcess--)
    {
        if (numToProcess == 0)
        {
            return;
        }

        if ((instance->flags2 & 0x80) || (instance->object->oflags & 0x10000) || (INSTANCE_Query(instance, 0x23) != 0) ||
            (INSTANCE_Query(instance, 0x2F) != 0) || (instance->LinkParent != NULL) || (instance->matrix == NULL))
        {
            if ((instance->flags2 & 1) != 0)
            {
                INSTANCE_Reactivate(instance);
            }
        }
        else
        {
            line->vx = instance->position.x - theCamera.core.position.x;
            line->vy = instance->position.y - theCamera.core.position.y;
            line->vz = instance->position.z - theCamera.core.position.z;
            distSq = line->vx * line->vx;
            cntInst = distSq + (line->vy * line->vy);
            cntInst += (line->vz * line->vz);

            if ((instance->flags & 0x200) != 0)
            {
                if (cntInst > (instance->object->vvRemoveDist * instance->object->vvRemoveDist))
                {
                    if ((instance->flags2 & 1) == 0)
                    {
                        INSTANCE_Deactivate(instance);
                    }
                }
                else if ((instance->flags2 & 1) != 0)
                {
                    INSTANCE_Reactivate(instance);
                }
            }
            else if ((instance->flags2 & 0x80000) == 0)
            {
                if (cntInst > (instance->object->removeDist * instance->object->removeDist))
                {
                    if ((instance->flags2 & 1) == 0)
                    {
                        INSTANCE_Deactivate(instance);
                    }
                }
                else if ((instance->flags2 & 1) != 0)
                {
                    INSTANCE_Reactivate(instance);
                }
            }
            else
            {
                if (cntInst > (gameTracker->defRemoveDist * gameTracker->defRemoveDist))
                {
                    if ((instance->flags2 & 1) == 0)
                    {
                        INSTANCE_Deactivate(instance);
                    }
                }
                else if ((instance->flags2 & 1) != 0)
                {
                    INSTANCE_Reactivate(instance);
                }
            }
        }
    }
}

void INSTANCE_InitInstanceList(InstanceList *list, InstancePool *pool)
{
    long i;

    pool->numFreeInstances = 60;

    for (i = 1; i < 59; i++)
    {
        pool->instance[i].next = &pool->instance[i + 1];
        pool->instance[i].prev = &pool->instance[i - 1];
    }

    pool->instance[0].next = &pool->instance[1];
    pool->instance[0].prev = NULL;

    pool->instance[59].prev = &pool->instance[58];
    pool->instance[59].next = NULL;

    pool->first_free = &pool->instance[0];

    list->pool = pool;

    list->numInstances = 0;

    list->first = NULL;

    for (i = 0; i < 32; i++)
    {
        list->group[i].next = NULL;
        list->group[i].prev = NULL;
    }

    pool->nextInstanceID = 1;
}

Instance *INSTANCE_NewInstance(InstanceList *list)
{
    Instance *temp;
    Instance *instance;

    if (list->pool->numFreeInstances != 0)
    {
        list->pool->numFreeInstances--;

        instance = list->pool->first_free;

        list->pool->first_free = instance->next;

        temp = list->first;

        list->first = instance;

        instance->next = temp;

        if (instance->next != NULL)
        {
            temp->prev = instance;
        }

        instance->prev = NULL;

        instance->instanceID = list->pool->nextInstanceID++;

        list->numInstances++;

        return instance;
    }

    return NULL;
}

long INSTANCE_InstanceGroupNumber(Instance *instance)
{
    long result;

    result = 0;

    if (((instance->object->oflags & 0x80)) && (!(instance->flags & 0x8000)))
    {
        result = 0x1;
    }

    if (((instance->object->oflags & 0x20)) && (!(instance->flags & 0x2000)))
    {
        result |= 0x2;
    }

    if (((instance->object->oflags & 0x40)) && (!(instance->flags & 0x4000)))
    {
        result |= 0x4;
    }

    if (((instance->object->oflags & 0x10)) && (!(instance->flags & 0x1000)))
    {
        result |= 0x10;
    }

    if ((instance->object->oflags & 0x1))
    {
        result |= 0x8;
    }
    else if (strcmp(instance->object->name, "raziel__") != 0)
    {
        result &= ~0x2;
        result &= ~0x1;
    }

    return result;
}

void INSTANCE_InsertInstanceGroup(InstanceList *list, Instance *instance)
{
    LIST_InsertFunc(&list->group[INSTANCE_InstanceGroupNumber(instance)], &instance->node);
}

void INSTANCE_ReallyRemoveInstance(InstanceList *list, Instance *instance, long reset)
{
    Instance *temp;
    int i;

    EVENT_RemoveInstanceFromInstanceList(instance);

    if ((instance->flags & 0x800000))
    {
        SAVE_DoInstanceDeadDead(instance);
    }

    if ((!(instance->flags & 0x2)) && (instance->intro != NULL))
    {
        instance->intro->flags &= ~reset;

        instance->intro->instance = NULL;
    }

    if (instance->prev != NULL)
    {
        instance->prev->next = instance->next;
    }
    else
    {
        list->first = instance->next;
    }

    if (instance->next != NULL)
    {
        instance->next->prev = instance->prev;
    }

    instance->instanceID = 0;

    list->numInstances--;

    list->pool->numFreeInstances++;

    temp = list->pool->first_free;

    list->pool->first_free = instance;

    instance->prev = NULL;
    instance->next = temp;

    if (temp != NULL)
    {
        temp->prev = instance;
    }

    LIST_DeleteFunc((NodeType *)instance);

    FX_EndInstanceEffects(instance);

    if ((instance->flags & 0x10000))
    {
        instance->flags |= 0x20000;

        OBTABLE_InstanceInit(instance);
    }

    if ((instance->object->animList != NULL) && (!(instance->object->oflags2 & 0x40000000)))
    {
        G2Anim_Free(&instance->anim);
    }

    if (instance->shadow0 != NULL)
    {
        ((LightInstance *)(instance->shadow0))->radius = 0;
    }

    if (instance->shadow1 != NULL)
    {
        ((LightInstance *)(instance->shadow1))->radius = 0;
    }

    if ((instance->object->oflags2 & 0x4))
    {
        SOUND_EndInstanceSounds(instance->object->soundData, instance->soundInstanceTbl);
    }

    if (instance->LinkParent != NULL)
    {
        INSTANCE_UnlinkFromParent(instance);
    }

    if (instance->LinkChild != NULL)
    {
        INSTANCE_UnlinkChildren(instance);
    }

    if (instance->hModelList != NULL)
    {
        MEMPACK_Free((char *)instance->hModelList);
    }

    if (instance->perVertexColor != NULL)
    {
        MEMPACK_Free((char *)instance->perVertexColor);

        instance->perVertexColor = NULL;
    }

    for (i = 0; i < 1; i++)
    {
        if (gameTrackerX.gameData.asmData.lightInstances[i].lightInstance == instance)
        {
            gameTrackerX.gameData.asmData.lightInstances[i].lightInstance = NULL;
            break;
        }
    }
}

void INSTANCE_CleanUpInstanceList(InstanceList *list, long reset)
{
    Instance *instance;
    Instance *next;

    instance = list->first;

    while (instance != NULL)
    {
        next = instance->next;

        if (!(instance->flags & 0x400))
        {
            if ((instance->flags & 0x20))
            {
                instance->flags |= 0x20;
            }
        }
        else if ((instance->flags & 0x20))
        {
            instance->flags &= ~0x20;
        }

        instance = next;
    }

    instance = list->first;

    while (instance != NULL)
    {
        next = instance->next;

        if ((instance->flags & 0x20))
        {
            INSTANCE_ReallyRemoveInstance(list, instance, reset);
        }

        instance = next;
    }
}

long INSTANCE_Introduced(Intro *intro, short streamUnitID)
{
    Instance *instance;
    Instance *next;
    long ret;

    instance = gameTrackerX.instanceList->first;

    ret = 0;

    while (instance != NULL)
    {
        next = instance->next;

        if (intro->UniqueID == instance->introUniqueID)
        {
            ret = 1;

            intro->flags |= 0x8;
            break;
        }

        instance = next;
    }

    if (ret == 0)
    {
        if (SAVE_HasSavedIntro(intro, streamUnitID) != 0)
        {
            intro->flags |= 0x8;

            ret = 1;
        }
        else if (SAVE_IsIntroDeadDead(intro) != 0)
        {
            intro->flags |= 0x8;

            ret = 1;
        }
    }

    return ret;
}

INICommand *INSTANCE_GetIntroCommand(INICommand *command, int cmd)
{
    if (command != NULL)
    {
        while (command->command != 0)
        {
            if (command->command != cmd)
            {
                command += command->numParameters + 1;
            }
            else
            {
                return command;
            }
        }
    }

    return 0;
}

INICommand *INSTANCE_FindIntroCommand(Instance *instance, int cmd)
{
    return INSTANCE_GetIntroCommand((INICommand *)instance->introData, cmd);
}

void INSTANCE_ProcessIntro(Instance *instance)
{
    INICommand *command;

    if (instance->introData != NULL)
    {
        command = (INICommand *)instance->introData;

        if (!(instance->flags & 0x2))
        {
            while (command->command != 0)
            {
                if (command->command == 18)
                {
                    instance->currentModel = command->parameter[0];
                }

                command += command->numParameters + 1;
            }
        }
    }
}

void INSTANCE_InitEffects(Instance *instance, Object *object)
{
    int i;

    if ((!(object->oflags2 & 0x100)) && (object->numberOfEffects != 0))
    {
        for (i = 0; i < object->numberOfEffects; i++)
        {
            FX_StartInstanceEffect(instance, &object->effectList[i], 1);
        }
    }
}

Instance *INSTANCE_IntroduceInstance(Intro *intro, short streamUnitID)
{
    Object *object;
    Instance *instance;
    Instance *attachInst;
    ObjectTracker *objectTracker;

    attachInst = NULL;

    if (INSTANCE_Introduced(intro, streamUnitID) == 0)
    {
        INICommand *index;
        long attachedUniqueID;

        index = INSTANCE_GetIntroCommand((INICommand *)intro->data, 26);

        attachedUniqueID = 0;

        if (index != NULL)
        {
            attachedUniqueID = index->parameter[0];
        }

        objectTracker = STREAM_GetObjectTracker(intro->name);

        if (objectTracker != NULL)
        {
            object = objectTracker->object;

            if ((objectTracker->objectStatus == 2) && ((attachedUniqueID == 0) || (attachInst = INSTANCE_Find(attachedUniqueID), (attachInst != NULL))) && ((!(object->oflags2 & 0x10000000)) || (OBTABLE_InitAnimPointers(objectTracker), (!(object->oflags2 & 0x10000000)))))
            {
                SavedIntroSmall *savedIntroSmall;
                MultiSpline *spline;

                instance = INSTANCE_NewInstance(gameTrackerX.instanceList);

                if (instance != NULL)
                {
                    intro->flags |= 0x8;

                    objectTracker->numInUse++;

                    INSTANCE_DefaultInit(instance, object, 0);

                    strcpy(instance->introName, intro->name);

                    instance->introUniqueID = intro->UniqueID;
                    instance->introNum = intro->intronum;

                    instance->birthStreamUnitID = streamUnitID;
                    instance->currentStreamUnitID = streamUnitID;

                    intro->instance = instance;

                    instance->intro = intro;
                    instance->introData = intro->data;

                    instance->position = intro->position;

                    if (gameTrackerX.gameData.asmData.MorphType == 1)
                    {
                        if ((intro->spectralPosition.x != 0) || (intro->spectralPosition.y != 0) || (intro->spectralPosition.z != 0))
                        {
                            instance->position.x = intro->position.x + intro->spectralPosition.x;
                            instance->position.y = intro->position.y + intro->spectralPosition.y;
                            instance->position.z = intro->position.z + intro->spectralPosition.z;
                        }
                    }

                    instance->initialPos = instance->position;
                    instance->oldPos = intro->position;

                    instance->attachedID = attachedUniqueID;

                    LIGHT_GetAmbient((ColorType *)&instance->light_color, instance);

                    instance->rotation = intro->rotation;

                    if ((instance->object->oflags & 0x100))
                    {
                        INSTANCE_BuildStaticShadow(instance);
                    }

                    instance->scale.x = 4096;
                    instance->scale.y = 4096;
                    instance->scale.z = 4096;

                    if ((intro->flags & 0x2000))
                    {
                        instance->flags |= 0x400;
                    }

                    if ((intro->flags & 0x10000))
                    {
                        instance->flags2 |= 0x20000;
                    }

                    if (attachInst != NULL)
                    {
                        INSTANCE_ForceActive(attachInst);

                        attachInst->flags2 |= 0x80;
                    }

                    if ((object->oflags2 & 0x80))
                    {
                        instance->flags |= 0x800;
                    }

                    if (((intro->flags & 0x800)) && (object->id == -1))
                    {
                        SCRIPT_InstanceSplineSet(instance, SCRIPTCountFramesInSpline(instance), NULL, NULL, NULL);

                        instance->flags = (instance->flags ^ 0x1000000) | 0x100000;
                    }

                    instance->lightGroup = (unsigned char)intro->rotation.pad; // double-check
                    instance->spectralLightGroup = (unsigned char)intro->specturalLightGroup;

                    INSTANCE_InsertInstanceGroup(gameTrackerX.instanceList, instance);

                    OBTABLE_GetInstanceCollideFunc(instance);
                    OBTABLE_GetInstanceProcessFunc(instance);
                    OBTABLE_GetInstanceQueryFunc(instance);
                    OBTABLE_GetInstanceMessageFunc(instance);
                    OBTABLE_GetInstanceAdditionalCollideFunc(instance);

                    if (!(intro->flags & 0x10))
                    {
                        OBTABLE_InstanceInit(instance);
                    }

                    MORPH_SetupInstanceFlags(instance);

                    if ((intro->flags & 0x80))
                    {
                        instance->flags |= 0x800;
                        instance->flags2 |= 0x20000000;

                        if ((object->oflags2 & 0x80000))
                        {
                            instance->flags2 |= 0x10000000;
                        }
                    }

                    if (SCRIPT_GetMultiSpline(instance, NULL, NULL) == NULL)
                    {
                        instance->flags = (instance->flags & ~0x2000000) | 0x100000;
                    }
                    else
                    {
                        SavedIntroSpline *savedIntroSpline;

                        spline = SCRIPT_GetMultiSpline(instance, NULL, NULL);

                        savedIntroSpline = SAVE_GetIntroSpline(instance);

                        if (savedIntroSpline != NULL)
                        {
                            SCRIPT_InstanceSplineSet(instance, savedIntroSpline->splineKeyFrame, NULL, NULL, NULL);

                            instance->oldPos = instance->position;

                            instance->splineFlags = savedIntroSpline->splineFlags;

                            instance->clipBeg = savedIntroSpline->splineClipBeg;
                            instance->clipEnd = savedIntroSpline->splineClipEnd;

                            if ((instance->splineFlags & 0x80))
                            {
                                instance->flags |= 0x1000000;
                            }

                            if ((instance->splineFlags & 0x100))
                            {
                                instance->flags |= 0x2000000;
                            }

                            if ((savedIntroSpline->splineFlags & 0x10))
                            {
                                if (spline->positional != NULL)
                                {
                                    spline->positional->flags |= 0x1;
                                }

                                if (spline->rotational != NULL)
                                {
                                    spline->rotational->flags |= 0x1;
                                }

                                if (spline->scaling != NULL)
                                {
                                    spline->scaling->flags |= 0x1;
                                }

                                if (spline->color != NULL)
                                {
                                    spline->color->flags |= 0x1;
                                }
                            }
                            else if ((instance->splineFlags & 0x20))
                            {
                                if (spline->positional != NULL)
                                {
                                    spline->positional->flags |= 0x2;
                                }

                                if (spline->rotational != NULL)
                                {
                                    spline->rotational->flags |= 0x2;
                                }

                                if (spline->scaling != NULL)
                                {
                                    spline->scaling->flags |= 0x2;
                                }

                                if (spline->color != NULL)
                                {
                                    spline->color->flags |= 0x2;
                                }
                            }
                            else if ((instance->splineFlags & 0x40))
                            {
                                if (spline->positional != NULL)
                                {
                                    spline->positional->flags |= 0x4;
                                }

                                if (spline->rotational != NULL)
                                {
                                    spline->rotational->flags |= 0x4;
                                }

                                if (spline->scaling != NULL)
                                {
                                    spline->scaling->flags |= 0x4;
                                }

                                if (spline->color != NULL)
                                {
                                    spline->color->flags |= 0x4;
                                }
                            }
                        }
                    }

                    EVENT_AddInstanceToInstanceList(instance);

                    INSTANCE_ProcessIntro(instance);

                    INSTANCE_InitEffects(instance, object);

                    savedIntroSmall = SAVE_GetSavedSmallIntro(instance);

                    if (savedIntroSmall != NULL)
                    {
                        INSTANCE_Post(instance, 0x100007, SetControlSaveDataData((savedIntroSmall->shiftedSaveSize << 2) - 4, savedIntroSmall + 1)); // double-check
                    }

                    return instance;
                }
            }
        }
    }

    return NULL;
}

void INSTANCE_AdditionalCollideFunctions(InstanceList *instanceList)
{
    Instance *instance;

    instance = instanceList->first;

    while (instance != NULL)
    {
        if ((gameTrackerX.gameMode != 6) || ((instance->object->oflags & 0x20000)))
        {
            if ((!(gameTrackerX.streamFlags & 0x100000)) || ((instance->object->oflags & 0x40000)))
            {
                if ((instance->additionalCollideFunc != NULL) && (!(instance->flags2 & 0x24000000)))
                {
                    if ((instance->object != NULL) && ((instance->object->oflags2 & 0x2000000)))
                    {
                        if ((instance->flags2 & 0x8000000))
                        {
                            gameTrackerX.timeMult = gameTrackerX.spectralTimeMult;
                        }
                        else
                        {
                            gameTrackerX.timeMult = gameTrackerX.materialTimeMult;
                        }
                    }
                    else
                    {
                        gameTrackerX.timeMult = gameTrackerX.globalTimeMult;
                    }

                    instance->additionalCollideFunc(instance, &gameTrackerX);
                }
            }
        }

        instance = instance->next;
    }

    gameTrackerX.timeMult = gameTrackerX.globalTimeMult;
}

long INSTANCE_GetSplineFrameNumber(Instance *instance, MultiSpline *spline)
{
    return SCRIPT_GetSplineFrameNumber(instance, SCRIPT_GetPosSplineDef(instance, spline, 0, 0));
}

void INSTANCE_ProcessFunctions(InstanceList *instanceList)
{
    if (!(gameTrackerX.debugFlags & 0x8000010))
    {
        Instance *instance;

        if ((gameTrackerX.gameMode != 6) && (!(gameTrackerX.streamFlags & 0x100000)))
        {
            gameTrackerX.streamFlags &= ~0x4;
        }

        for (instance = instanceList->first; instance != NULL; )
        {
            int hidden;
            int burning;

            if ((instance->object != NULL) && ((instance->object->oflags2 & 0x2000000)))
            {
                if ((instance->flags2 & 0x8000000))
                {
                    gameTrackerX.timeMult = gameTrackerX.spectralTimeMult;
                }
                else
                {
                    gameTrackerX.timeMult = gameTrackerX.materialTimeMult;
                }
            }
            else
            {
                gameTrackerX.timeMult = gameTrackerX.globalTimeMult;
            }

            burning = 0;

            if ((instance->object->oflags2 & 0x4))
            {
                hidden = instance->flags & 0x800;

                if ((INSTANCE_Query(instance, 2) & 0x20))
                {
                    burning = (unsigned int)burning < ((INSTANCE_Query(instance, 3) & 0x10000));
                }
                else if (((instance->object->oflags2 & 0x80000)) && ((((PhysObData *)instance->extraData)->Mode & 0x400000)))
                {
                    burning = 1;
                }

                SOUND_ProcessInstanceSounds(instance->object->soundData, instance->soundInstanceTbl, &instance->position, instance->object->oflags2 & 0x2000000, instance->flags2 & 0x8000000, hidden, burning, &instance->flags2);
            }

            if (((instance->intro == NULL) || (!(instance->intro->flags & 0x100))) && ((gameTrackerX.gameMode != 6) || ((instance->object->oflags & 0x20000))) && ((!(gameTrackerX.streamFlags & 0x100000)) || ((instance->object->oflags & 0x40000))) && (!(instance->flags2 & 0x10000000)))
            {
                instance->oldPos.x = instance->position.x;
                instance->oldPos.y = instance->position.y;
                instance->oldPos.z = instance->position.z;

                instance->oldRotation.x = instance->rotation.x;
                instance->oldRotation.y = instance->rotation.y;
                instance->oldRotation.z = instance->rotation.z;

                if (!(instance->flags & 0x100000))
                {
                    long prevFrame;
                    MultiSpline *multi;

                    multi = SCRIPT_GetMultiSpline(instance, NULL, NULL);

                    if ((multi != NULL) && ((instance->flags & 0x2000000)))
                    {
                        long endOfSpline;
                        short direction;

                        direction = ((instance->flags & 0x1000000)) ? -1 : 1;

                        if ((instance->object->oflags & 0x10000000))
                        {
                            instance->flags |= 0x400;

                            if (SplineMultiIsWhere(multi) != 0)
                            {
                                if (direction > 0)
                                {
                                    instance->intro->flags |= 0x800;
                                }
                                else
                                {
                                    instance->intro->flags &= ~0x800;
                                }
                            }
                        }

                        prevFrame = INSTANCE_GetSplineFrameNumber(instance, multi);

                        endOfSpline = SCRIPT_InstanceSplineProcess(instance, &multi->curPositional, &multi->curRotational, &multi->curScaling, direction);

                        if (instance->splineFlags != 0)
                        {
                            long frame;
                            long maxFrames;

                            frame = INSTANCE_GetSplineFrameNumber(instance, multi);

                            maxFrames = SCRIPTCountFramesInSpline(instance);

                            if ((instance->splineFlags & 0x1))
                            {
                                if ((gameTrackerX.debugFlags2 & 0x400000))
                                {
                                    printf("Spline %s%ld playto %d preveFram=%ld frame=%ld endOfSpline=%ld, maxFrames=%ld\n", instance->introName, instance->introNum, instance->targetFrame, prevFrame, frame, endOfSpline, maxFrames);
                                }

                                if (direction > 0)
                                {
                                    if (prevFrame > maxFrames)
                                    {
                                        prevFrame = maxFrames;
                                    }

                                    if ((((instance->targetFrame <= frame) && (instance->targetFrame >= prevFrame)) || ((prevFrame > frame) && (instance->targetFrame <= frame) && ((instance->targetFrame + maxFrames) >= prevFrame))))
                                    {
                                        instance->flags &= ~0x2000000;

                                        SCRIPT_InstanceSplineSet(instance, instance->targetFrame, NULL, NULL, NULL);

                                        endOfSpline = 0;

                                        instance->splineFlags &= ~0x1;
                                    }
                                }
                                else if ((instance->targetFrame >= frame) && ((instance->targetFrame <= prevFrame) || ((prevFrame < frame) && (instance->targetFrame <= (prevFrame + maxFrames)))))
                                {
                                    instance->flags &= ~0x2000000;

                                    SCRIPT_InstanceSplineSet(instance, instance->targetFrame, NULL, NULL, NULL);

                                    endOfSpline = 0;

                                    instance->splineFlags &= ~0x1;
                                }
                            }

                            if ((instance->splineFlags & 0x2))
                            {
                                if ((gameTrackerX.debugFlags2 & 0x400000))
                                {
                                    FONT_Print("Spline %s%d : clip(%d,%d) prevFrame=%d, frame=%d\n", instance->introName, instance->introNum, instance->clipBeg, instance->clipEnd, prevFrame, frame);
                                }

                                if (direction == 1)
                                {
                                    if (((instance->clipEnd >= prevFrame) && (instance->clipEnd <= frame)) || (instance->clipBeg > frame))
                                    {
                                        if (((multi->positional->flags & 0x4)) || ((multi->positional->flags & 0x2)))
                                        {
                                            SCRIPT_InstanceSplineSet(instance, instance->clipBeg, NULL, NULL, NULL);
                                        }
                                        else
                                        {
                                            SCRIPT_InstanceSplineSet(instance, instance->clipEnd, NULL, NULL, NULL);

                                            endOfSpline = 1;
                                        }
                                    }
                                }
                                else
                                {
                                    if (((instance->clipBeg <= prevFrame) && (instance->clipBeg >= frame)) || (instance->clipEnd < frame))
                                    {
                                        if (((multi->positional->flags & 0x4)) || ((multi->positional->flags & 0x2)))
                                        {
                                            SCRIPT_InstanceSplineSet(instance, instance->clipEnd, NULL, NULL, NULL);
                                        }
                                        else
                                        {
                                            SCRIPT_InstanceSplineSet(instance, instance->clipBeg, NULL, NULL, NULL);

                                            endOfSpline = 1;
                                        }
                                    }
                                }
                            }
                        }
                        else if ((gameTrackerX.debugFlags2 & 0x400000))
                        {
                            FONT_Print("Spline %s%d prevFrame=%d, frame=%d\n", instance->introName, instance->introNum, prevFrame, INSTANCE_GetSplineFrameNumber(instance, multi));
                        }

                        if (endOfSpline > 0)
                        {
                            if ((instance->object->oflags & 0x10000000))
                            {
                                instance->flags &= ~0x400;
                                instance->flags &= ~0x2000000;

                                instance->flags |= 0x100000;
                            }

                            if ((instance->object->oflags & 0x2000))
                            {
                                instance->flags |= 0x100000;

                                INSTANCE_KillInstance(instance);

                                instance = instance->next;
                                continue;
                            }
                            else
                            {
                                if ((instance->object->oflags & 0x1000))
                                {
                                    instance->flags &= ~0x2000000;

                                    if ((instance->object->oflags & 0x800000))
                                    {
                                        SCRIPT_InstanceSplineInit(instance);
                                    }
                                }
                                else if ((instance->object->oflags & 0x1000000))
                                {
                                    instance->flags &= ~0x2000000;
                                }

                                if (multi->positional != NULL)
                                {
                                    if ((!(multi->positional->flags & 0x4)) && (!(instance->object->oflags & 0x800000)))
                                    {
                                        instance->flags ^= 0x1000000;
                                    }
                                }
                                else if (multi->rotational != NULL)
                                {
                                    if ((!(multi->rotational->flags & 0x4)) && (!(instance->object->oflags & 0x800000)))
                                    {
                                        instance->flags ^= 0x1000000;
                                    }
                                }

                                if (((instance->object->oflags & 0x200000)) && (instance->introData != NULL))
                                {
                                    Signal *temp; // not from decls.h

                                    temp = instance->introData;

                                    if (temp->id != 0)
                                    {
                                        SIGNAL_HandleSignal(instance, (Signal *)(temp->id + 8), 0); // TODO: find the union from the Signal struct that would logically fit in here
                                    }
                                }

                            }
                        }
                    }
                }

                if (instance->processFunc != NULL)
                {
                    if ((instance->flags2 & 0x1))
                    {
                        INSTANCE_DeactivatedProcess(instance, &gameTrackerX);
                    }
                    else
                    {
                        instance->processFunc(instance, &gameTrackerX);
                    }
                }

                {
                    int killTest;

                    killTest = instance->position.z - instance->oldPos.z;

                    if (killTest > 32768)
                    {
                        INSTANCE_PlainDeath(instance);
                    }

                    if (killTest < -32768)
                    {
                        INSTANCE_PlainDeath(instance);
                    }

                    if (INSTANCE_Query(instance, 47) != 0)
                    {
                        gameTrackerX.streamFlags |= 0x4;
                    }
                }
            }

            instance = instance->next;
        }
    }

    gameTrackerX.timeMult = gameTrackerX.globalTimeMult;

    if (gameTrackerX.gameData.asmData.MorphTime != 1000)
    {
        gameTrackerX.streamFlags |= 0x4;
    }
}

Instance *INSTANCE_BirthObject(Instance *parent, Object *object, int modelNum)
{
    Instance *instance;
    int i;

    if (object == NULL)
    {
        return NULL;
    }

    instance = INSTANCE_NewInstance(gameTrackerX.instanceList);

    if (instance != NULL)
    {
        INSTANCE_DefaultInit(instance, object, modelNum);

        instance->position = parent->position;
        instance->initialPos = instance->position;
        instance->oldPos = parent->position;

        instance->rotation = parent->rotation;
        instance->scale = parent->scale;

        instance->lightGroup = parent->lightGroup;
        instance->spectralLightGroup = parent->spectralLightGroup;

        instance->currentStreamUnitID = parent->currentStreamUnitID;
        instance->birthStreamUnitID = parent->birthStreamUnitID;

        instance->introUniqueID = GlobalSave->CurrentBirthID++;

        strcpy(instance->introName, object->name);

        for (i = 0; i < (int)strlen(instance->introName); i++)
        {
            if (instance->introName[i] == '_')
            {
                instance->introName[i] = 0;
                break;
            }
        }

        instance->parent = parent;

        instance->intro = parent->intro;
        instance->introData = parent->introData;

        LIGHT_GetAmbient((ColorType *)&instance->light_color, instance);

        if ((instance->object->oflags & 0x100))
        {
            INSTANCE_BuildStaticShadow(instance);
        }

        if (SCRIPT_GetMultiSpline(instance, NULL, NULL) == NULL)
        {
            instance->flags |= 0x100000;
        }

        if ((parent->flags2 & 0x8000000))
        {
            instance->flags2 |= 0x8000000;
        }

        INSTANCE_InsertInstanceGroup(gameTrackerX.instanceList, instance);

        OBTABLE_GetInstanceCollideFunc(instance);
        OBTABLE_GetInstanceProcessFunc(instance);
        OBTABLE_GetInstanceQueryFunc(instance);
        OBTABLE_GetInstanceMessageFunc(instance);
        OBTABLE_GetInstanceAdditionalCollideFunc(instance);

        instance->flags |= 0x2;

        OBTABLE_InstanceInit(instance);

        EVENT_AddInstanceToInstanceList(instance);

        INSTANCE_InitEffects(instance, object);

        return instance;
    }

    return NULL;
}

void INSTANCE_BuildStaticShadow()
{
}

void INSTANCE_DefaultInit(Instance *instance, Object *object, int modelNum)
{
    int i;
    int j;
    int numHPrims;
    int numModels;
    HPrim *hprim;
    Model **pModel;
    HModel *hmodel;

    memset(&instance->flags, 0, sizeof(Instance) - 20);

    instance->object = object;

    instance->data = object->data;

    instance->currentModel = modelNum;

    instance->cachedTFace = -1;
    instance->cachedTFaceLevel = NULL;

    if ((object->animList != NULL) && (!(object->oflags2 & 0x40000000)))
    {
        G2Anim_Init(&instance->anim, object->modelList[modelNum]);

        G2Anim_SwitchToKeylist(&instance->anim, G2Instance_GetKeylist(instance, 0), 0);

        G2Anim_SetCallback(&instance->anim, (void *)INSTANCE_DefaultAnimCallback, instance);
    }

    if ((object->oflags & 0x4000000))
    {
        instance->flags2 |= 0x4;
    }

    if ((object->oflags & 0x200))
    {
        instance->flags2 |= 0x40;
    }

    if ((object->oflags & 0x8))
    {
        instance->flags2 |= 0x8000000;
    }

    if (!(object->oflags & 0x1000800))
    {
        instance->flags |= 0x2000000;
    }

    if ((object->oflags & 0x20000000))
    {
        instance->flags2 |= 0x20;
    }

    numModels = object->numModels;

    pModel = object->modelList;

    numHPrims = 0;

    for (i = numModels; i != 0; i--, pModel++)
    {
        Model *model;
        Segment *seg;

        model = *pModel;

        seg = model->segmentList;

        for (j = model->numSegments; j != 0; j--, seg++)
        {
            HInfo *hinfo;

            hinfo = seg->hInfo;

            if (hinfo != NULL)
            {
                numHPrims += hinfo->numHFaces + hinfo->numHSpheres + hinfo->numHBoxes;
            }
        }
    }

    if (numHPrims != 0)
    {
        instance->hModelList = (HModel *)MEMPACK_Malloc((instance->object->numModels + numHPrims) * sizeof(HModel), 14);

        pModel = object->modelList;

        hprim = (HPrim *)instance->hModelList + numModels;

        hmodel = instance->hModelList;

        for (i = numModels; i != 0; i--, pModel++, hmodel++)
        {
            Model *model;
            Segment *seg;

            model = pModel[0];

            hmodel->numHPrims = 0;

            hmodel->hPrimList = hprim;

            seg = model->segmentList;

            for (j = 0; j < model->numSegments; j++, seg++)
            {
                HInfo *hinfo;

                hinfo = seg->hInfo;

                if (hinfo != NULL)
                {
                    int k;
                    HFace *hface;
                    HSphere *hsphere;
                    HBox *hbox;

                    hface = hinfo->hfaceList;

                    hsphere = hinfo->hsphereList;

                    hbox = hinfo->hboxList;

                    hmodel->numHPrims += hinfo->numHFaces + hinfo->numHSpheres + hinfo->numHBoxes;

                    for (k = hinfo->numHFaces; k != 0; hprim++, hface++, k--)
                    {
                        hprim->hpFlags = 0x4D;
                        hprim->withFlags = 0x24;

                        hprim->type = 2;

                        hprim->segment = j;

                        hprim->data.hface = hface;
                    }

                    for (k = hinfo->numHSpheres; k != 0; hprim++, hsphere++, k--)
                    {
                        hprim->hpFlags = 0x2F;
                        hprim->withFlags = 0x76;

                        hprim->type = 1;

                        hprim->segment = j;

                        hprim->data.hsphere = hsphere;
                    }

                    for (k = hinfo->numHBoxes; k != 0; hprim++, hbox++, k--)
                    {
                        hprim->hpFlags = 0x1D;
                        hprim->withFlags = 0x24;

                        hprim->type = 5;

                        hprim->segment = j;

                        hprim->data.hbox = hbox;
                    }
                }
            }
        }
    }
    else
    {
        instance->hModelList = NULL;

        instance->flags2 |= 0x40000;
    }

    if (instance->maxCheckDistance == 0)
    {
        instance->maxCheckDistance = 12000;
    }
}

void INSTANCE_PlainDeath(Instance *instance)
{
    Instance *oldOn;

    instance->flags &= ~0x400;

    if ((instance->object->oflags2 & 0x4))
    {
        instance->flags2 |= 0x1000;

        SOUND_ProcessInstanceSounds(instance->object->soundData, instance->soundInstanceTbl, &instance->position, instance->object->oflags2 & 0x2000000, instance->flags2 & 0x8000000, 0, 0, &instance->flags2);
        SOUND_ProcessInstanceSounds(instance->object->soundData, instance->soundInstanceTbl, &instance->position, instance->object->oflags2 & 0x2000000, instance->flags2 & 0x8000000, 0, 0, &instance->flags2);
    }

    oldOn = INSTANCE_Find(instance->attachedID);

    if (oldOn != NULL)
    {
        oldOn->flags2 &= ~0x80;
    }

    SAVE_MarkDeadDead(instance);

    instance->flags |= 0x20;
}

void INSTANCE_KillInstance(Instance *instance)
{
    if (!(instance->flags & 0x20))
    {
        INSTANCE_PlainDeath(instance);
    }
}

uintptr_t INSTANCE_Query(Instance *Inst, int Query)
{
    uintptr_t(*Func)(Instance *, unsigned long);

    Func = Inst->queryFunc;

    if (Func == NULL)
    {
        return 0;
    }

    return Func(Inst, Query);
}

void INSTANCE_Post(Instance *Inst, int Message, intptr_t Data)
{
    void (*Func)(Instance *, unsigned long, intptr_t);

    Func = Inst->messageFunc;

    if (Func != NULL)
    {
        INSTANCE_ForceActive(Inst);

        Func(Inst, Message, Data);
    }
}

void INSTANCE_Broadcast(Instance *sender, long whatAmIMask, int Message, intptr_t Data)
{
    Instance *instance;
    int plane;

    instance = gameTrackerX.instanceList->first;

    plane = gameTrackerX.gameData.asmData.MorphType;

    while (instance != NULL)
    {
        if ((instance != sender) && ((INSTANCE_Query(instance, 1) & whatAmIMask)) && (INSTANCE_InPlane(instance, plane) != 0))
        {
            INSTANCE_Post(instance, Message, Data);
        }

        instance = instance->next;
    }
}

int INSTANCE_InPlane(Instance *instance, int plane)
{
    int ret;

    ret = 0;

    if (instance->object != NULL)
    {
        if (!(instance->object->oflags2 & 0x2000000))
        {
            ret = 1;
        }
        else if (!(instance->flags2 & 0x8000000))
        {
            if (plane == 0)
            {
                ret = 1;
            }
        }
        else if (plane != 0)
        {
            ret = 1;
        }
    }

    return ret;
}

long INSTANCE_FindWithID(long uniqueID)
{
    Instance *instance;
    Instance *next;
    long ret;

    instance = gameTrackerX.instanceList->first;

    ret = 0;

    while (instance != NULL)
    {
        next = instance->next;

        if (instance->introUniqueID == uniqueID)
        {
            ret = 1;
            break;
        }

        instance = next;
    }

    return ret;
}

Instance *INSTANCE_FindWithName(long areaID, char *instanceName, Instance *startInstance)
{
    Instance *instance;
    Instance *ret;
    Instance *next;

    ret = NULL;

    instance = gameTrackerX.instanceList->first;

    if (startInstance != NULL)
    {
        instance = startInstance->next;
    }

    if (areaID != 0)
    {
        while (instance != NULL)
        {
            next = instance->next;

            if ((instance->birthStreamUnitID != areaID) || (strcmpi(instance->introName, instanceName) != 0))
            {
                instance = next;
            }
            else
            {
                ret = instance;
                break;
            }
        }
    }
    else
    {
        while (instance != NULL)
        {
            next = instance->next;

            if (strcmpi(instance->introName, instanceName) != 0)
            {
                instance = next;
            }
            else
            {
                ret = instance;
                break;
            }
        }
    }

    return ret;
}

Intro *INSTANCE_FindIntro(long areaID, long introUniqueID)
{
    Intro *ret;
    long i;
    Level *level;

    ret = NULL;

    level = STREAM_GetLevelWithID(areaID);

    if (level != NULL)
    {
        for (i = 0; i < level->numIntros; i++)
        {
            if (introUniqueID == level->introList[i].UniqueID)
            {
                ret = &level->introList[i];
                break;
            }
        }
    }

    return ret;
}

Instance *INSTANCE_Find(long introUniqueID)
{
    Instance *instance;

    instance = gameTrackerX.instanceList->first;

    while (instance != NULL)
    {
        if (instance->introUniqueID == introUniqueID)
        {
            break;
        }

        instance = instance->next;
    }

    return instance;
}

INCLUDE_ASM("asm/nonmatchings/Game/INSTANCE", INSTANCE_IntroduceSavedInstance);

INCLUDE_ASM("asm/nonmatchings/Game/INSTANCE", INSTANCE_IntroduceSavedInstanceWithIntro);

void INSTANCE_SpatialRelationships(InstanceList *instanceList)
{
    Instance *instance;
    int checkMask;
    MATRIX *lookMatrix;
    Instance *checkee;
    MATRIX *mat;
    MATRIX invMatrix;
    evCollideInstanceStatsData data;

    for (instance = instanceList->first; instance != NULL; instance = instance->next)
    {
        if ((instance->flags2 & 0x10000401) == 0x400)
        {
            checkMask = instance->checkMask;

            lookMatrix = (MATRIX *)INSTANCE_Query(instance, 13);

            INSTANCE_Post(instance, 0x200000, 0);

            if (lookMatrix == NULL)
            {
                continue;
            }

            TransposeMatrix(lookMatrix, &invMatrix);

            for (checkee = instanceList->first; checkee != NULL; checkee = checkee->next)
            {
                if ((checkee != instance) && (!(checkee->flags2 & 0x10000000)) && (!(checkee->flags & 0x20)) && ((INSTANCE_Query(checkee, 1) & checkMask)))
                {
                    mat = (MATRIX *)INSTANCE_Query(checkee, 14);

                    if (mat == NULL)
                    {
                        mat = checkee->matrix;
                    }

                    if ((mat != NULL) && (INSTANCE_SetStatsData(instance, checkee, (Vector *)&mat->t[0], &data, &invMatrix) != 0))
                    {
                        INSTANCE_Post(instance, 0x200001, (intptr_t)&data);
                    }
                }
            }
        }
    }
}

int INSTANCE_SetStatsData(Instance *instance, Instance *checkee, Vector *checkPoint, evCollideInstanceStatsData *data, MATRIX *mat)
{
    long distance;
    SVECTOR *delta;

    distance = MATH3D_LengthXYZ(instance->position.x - checkPoint->x, instance->position.y - checkPoint->y, instance->position.z - checkPoint->z);

    if ((unsigned)distance < instance->maxCheckDistance)
    {
        delta = (SVECTOR *)getScratchAddr(0);

        delta->vx = checkPoint->x - instance->position.x;
        delta->vy = checkPoint->y - instance->position.y;
        delta->vz = checkPoint->z - instance->position.z;

        ApplyMatrixSV(mat, delta, (SVECTOR *)&data->relativePosition);

        data->instance = checkee;

        data->distance = distance;

        data->zDelta = delta->vz;

        data->xyDistance = MATH3D_LengthXY(delta->vx, delta->vy);

        return 1;
    }

    return 0;
}

void INSTANCE_LinkToParent(Instance *instance, Instance *parent, int node)
{
    instance->LinkSibling = parent->LinkChild;

    parent->LinkChild = instance;

    instance->LinkParent = parent;
    instance->ParentLinkNode = node;

    instance->scale.x = (instance->scale.x << 12) / parent->scale.x;
    instance->scale.y = (instance->scale.y << 12) / parent->scale.y;
    instance->scale.z = (instance->scale.z << 12) / parent->scale.z;

    INSTANCE_UpdateFamilyStreamUnitID(parent);

    INSTANCE_Post(parent, 0x100012, (intptr_t)instance);

    instance->flags2 |= 0x8;
}

void INSTANCE_UnlinkFromParent(Instance *instance)
{
    Instance *parent;
    G2EulerAngles ea;
    Instance *sibling;

    parent = instance->LinkParent;

    if (parent != NULL)
    {
        if (parent->LinkChild == instance)
        {
            parent->LinkChild = instance->LinkSibling;
        }
        else
        {
            sibling = parent->LinkChild;

            while (sibling->LinkSibling != instance)
            {
                sibling = sibling->LinkSibling;
            }

            sibling->LinkSibling = instance->LinkSibling;
        }

        instance->LinkSibling = NULL;
        instance->LinkParent = NULL;

        instance->scale.x = (instance->scale.x * parent->scale.x) / 4096;
        instance->scale.y = (instance->scale.y * parent->scale.y) / 4096;
        instance->scale.z = (instance->scale.z * parent->scale.z) / 4096;

        if (parent->matrix != NULL)
        {
            G2EulerAngles_FromMatrix(&ea, (G2Matrix *)(&parent->matrix[instance->ParentLinkNode]), 21);

            instance->rotation.x = ea.x;
            instance->rotation.y = ea.y;
            instance->rotation.z = ea.z;
        }

        INSTANCE_Post(parent, 0x100013, (intptr_t)instance);
    }
}

void INSTANCE_UnlinkChildren(Instance *instance)
{
    Instance *child;
    Instance *sibling;

    child = instance->LinkChild;

    while (child != NULL)
    {
        sibling = child->LinkSibling;

        INSTANCE_Post(instance, 0x100013, (intptr_t)child);

        child->LinkParent = NULL;
        child->LinkSibling = NULL;

        child = sibling;
    }

    instance->LinkChild = NULL;
}

void INSTANCE_UpdateFamilyStreamUnitID(Instance *instance)
{
    Instance *child;

    child = instance->LinkChild;

    while (child != NULL)
    {
        child->currentStreamUnitID = instance->currentStreamUnitID;

        INSTANCE_UpdateFamilyStreamUnitID(child);

        child = child->LinkSibling;
    }
}

void INSTANCE_ReallyRemoveAllChildren(Instance *instance)
{
    Instance *child;
    Instance *sibling;

    child = instance->LinkChild;

    while (child != NULL)
    {
        sibling = child->LinkSibling;

        INSTANCE_ReallyRemoveAllChildren(child);

        INSTANCE_ReallyRemoveInstance(gameTrackerX.instanceList, child, 0);

        child = sibling;
    }
}

Instance *INSTANCE_GetChildLinkedToSegment(Instance *instance, int segment)
{
    Instance *child;

    child = instance->LinkChild;

    while (child != NULL)
    {
        if (child->ParentLinkNode == segment)
        {
            break;
        }

        child = child->LinkSibling;
    }

    return child;
}

int INSTANCE_Linked(Instance *instance1, Instance *instance2)
{
    while (instance1->LinkParent != NULL)
    {
        instance1 = instance1->LinkParent;
    }

    while (instance2->LinkParent != NULL)
    {
        instance2 = instance2->LinkParent;
    }

    return instance1 == instance2;
}

int INSTANCE_GetFadeValue(Instance *instance)
{
    int fadeValue;

    fadeValue = instance->fadeValue;

    if (gameTrackerX.gameData.asmData.MorphTime != 1000)
    {
        fadeValue = 4096 - fadeValue;

        if (MEMPACK_MemoryValidFunc((char *)instance->object) != 0)
        {
            if (((instance->object->oflags2 & 0x2000000)) || ((instance->LinkParent != NULL) && ((instance->LinkParent->object->oflags2 & 0x2000000))))
            {
                if ((instance->flags2 & 0x8000000))
                {
                    if ((gameTrackerX.spectral_fadeValue * fadeValue) < 0)
                    {
                        fadeValue = ((gameTrackerX.spectral_fadeValue * fadeValue) + 4095) >> 12;
                    }
                    else
                    {
                        fadeValue = (gameTrackerX.spectral_fadeValue * fadeValue) >> 12;
                    }
                }
                else if ((gameTrackerX.material_fadeValue * fadeValue) < 0)
                {
                    fadeValue = ((gameTrackerX.material_fadeValue * fadeValue) + 4095) >> 12;
                }
                else
                {
                    fadeValue = (gameTrackerX.material_fadeValue * fadeValue) >> 12;
                }
            }
        }

        fadeValue = 4096 - fadeValue;
    }

    return fadeValue;
}

unsigned long INSTANCE_DefaultAnimCallback(G2Anim *anim, int sectionID, G2AnimCallbackMsg message, long messageDataA, long messageDataB, Instance *instance)
{
    AnimSoundData *soundData;
    int id;
    int vol;
    int temp; // not from decls.h

    (void)id;
    (void)anim;
    (void)sectionID;
    if (message == G2ANIM_MSG_PLAYEFFECT)
    {
        switch (messageDataA)
        {
        case 0:
            soundData = (AnimSoundData *)messageDataB;

            if (soundData != NULL)
            {
                vol = soundData->volume;

                if (soundData->volume >= 1000)
                {
                    temp = vol / 1000;

                    vol %= 1000;

                    if (temp != HUMAN_TypeOfHuman(instance))
                    {
                        return 0;
                    }
                }

                SOUND_Play3dSound(&instance->position, soundData->sfxToneID, soundData->pitch, vol, soundData->minVolDistance);
            }

            break;
        case 1:
            FX_StartInstanceEffect(instance, (ObjectEffect *)messageDataB, 0);
            break;
        default:
            return messageDataA;
        }
    }

    return messageDataA;
}
