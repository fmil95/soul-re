#include "Game/MONSTER/MONAPI.h"
#include "Game/SAVEINFO.h"
#include "Game/GAMELOOP.h"
#include "Game/MONSTER/MONSTER.h"
#include "Game/STATE.h"
#include "Game/MEMPACK.h"
#include "Game/MONSTER/MONLIB.h"
#include "Game/MONSTER/MONTABLE.h"
#include "Game/MONSTER/MONSENSE.h"
#include "Game/G2/INSTNCG2.h"
#include "Game/COLLIDE.h"
#include "Game/GENERIC.h"
#include "Game/MATH3D.h"
#include "Game/MONSTER/MONTABLE.h"
#include "Game/MONSTER/MISSILE.h"
#include "Game/STREAM.h"
#include "Game/INSTANCE.h"
#include "Game/G2/ANIMG2.h"
#include "Game/G2/ANMG2ILF.h"
#include "Game/PSX/MAIN.h"

void MonsterProcess(Instance *instance, GameTracker *gameTracker)
{
    MonsterState *state;
    MonsterVars *mv;
    MonsterAttributes *attributes;

    (void)gameTracker;
    attributes = (MonsterAttributes *)instance->data;

    mv = (MonsterVars *)instance->extraData;

    if ((mv != NULL) && (attributes != NULL) && (!(mv->mvFlags & 0x80000)))
    {
        instance->waterFace = NULL;
        instance->waterFaceTerrain = NULL;

        MONSENSE_DoSenses(instance);

        MON_DoCombatTimers(instance);

        state = MONTABLE_GetStateFuncs(instance, instance->currentMainState);

        instance->flags2 &= ~0x10;
        instance->flags2 &= ~0x2;

        if (!(mv->mvFlags & 0x80))
        {
            G2EmulationInstancePlayAnimation(instance);
        }

        (state->stateFunction)(instance);

        if ((mv->mvFlags & 0x1))
        {
            state = MONTABLE_GetStateFuncs(instance, instance->currentMainState);

            ((state)->entryFunction)(instance);
        }

        mv->mvFlags &= ~0x41;

        if ((instance->flags & 0x200))
        {
            ((MONTABLE_DamageEffectFunc)MONTABLE_GetDamageEffectFunc(instance))(instance, 0);
        }

        MON_ProcessLookAt(instance);
        MON_ProcessSpecialFade(instance);
    }
}

void MonsterInit(Instance *instance, GameTracker *gameTracker)
{
    MonsterState *state;
    MonsterAttributes *attributes;
    MonsterVars *mv;
    typedef void (*MONTABLE_CleanUpFunc)(Instance *); // not from decls.h
    typedef void (*MONTABLE_InitFunc)(Instance *);    // not from decls.h

    (void)gameTracker;

    attributes = (MonsterAttributes *)instance->data;

    if ((instance->flags & 0x20000))
    {
        mv = (MonsterVars *)instance->extraData;

        ((MONTABLE_CleanUpFunc)MONTABLE_GetCleanUpFunc(instance))(instance);

        MONSENSE_RemoveSenses(instance);

        MEMPACK_Free((char *)mv);
    }
    else if ((attributes == NULL) || (attributes->magicnum != 0xACE00065))
    {
        instance->data = NULL;

        G2Anim_SwitchToKeylist(&instance->anim, G2Instance_GetKeylist(instance, 0), 0);
    }
    else
    {
        mv = (MonsterVars *)MEMPACK_Malloc(608, 23);

        instance->extraData = mv;

        if (instance->instanceID == 0)
        {
            if (instance->extraData == NULL)
            {
                return;
            }

            MEMPACK_Free((char *)mv);
        }
        else if (mv == NULL)
        {
            instance->data = NULL;

            G2Anim_SwitchToKeylist(&instance->anim, G2Instance_GetKeylist(instance, 0), 0);
        }
        else
        {
            memset(mv, 0, 608); // TODO: third parameter needs changing to sizeof a struct (and maybe some arithmetic operation?)

            InitMessageQueue(&mv->messageQueue);

            instance->currentMainState = -1;

            mv->pathSlotID = -1;

            mv->lastValidPos = instance->position;

            mv->terrainImpaleID = -1;

            mv->mode = 0;

            instance->flags |= 0x10000;
            instance->flags |= 0x40;

            if (!(attributes->whatAmI & 0x10000))
            {
                instance->flags2 |= 0x100;
            }

            instance->maxXVel = 400;
            instance->maxYVel = 400;
            instance->maxZVel = 400;

            instance->xVel = 0;
            instance->yVel = 0;
            instance->zVel = 0;

            MONTABLE_SetQueryFunc(instance);
            MONTABLE_SetMessageFunc(instance);

            MON_ProcessIntro(instance);

            mv->maxHitPoints = mv->hitPoints;

            MON_TurnOnAllSpheres(instance);
            MON_TurnOffWeaponSpheres(instance);

            MONSENSE_SetupSenses(instance);

            MON_AnimInit(instance);

            ((MONTABLE_InitFunc)MONTABLE_GetInitFunc(instance))(instance);

            if ((mv->mvFlags & 0x1))
            {
                state = MONTABLE_GetStateFuncs(instance, instance->currentMainState);

                mv->mvFlags &= ~0x1;

                state->entryFunction(instance);
            }
        }
    }
}

void SendHitObject(Instance *instance, Instance *hit, int type)
{
    MonsterVars *mv;
    int i;

    mv = (MonsterVars *)instance->extraData;

    i = mv->messageQueue.Head;

    do
    {
        if (mv->messageQueue.Queue[i].ID == 0x1000008)
        {
            evMonsterHitObjectData *data1;

            data1 = (evMonsterHitObjectData *)mv->messageQueue.Queue[i].Data;

            if (data1->instance == hit)
            {
                break;
            }
        }

        i++;

        if (i == 16)
        {
            i = 0;
        }
    } while (i != mv->messageQueue.Tail);

    if (i == mv->messageQueue.Tail)
    {
        evMonsterHitObjectData *data;

        data = (evMonsterHitObjectData *)CIRC_Alloc(sizeof(evMonsterHitObjectData));

        data->instance = hit;
        data->hitType = type;

        INSTANCE_Post(instance, 0x1000008, (intptr_t)data);
    }
}

void MonsterCollide(Instance *instance, GameTracker *gameTracker)
{
    CollideInfo *collideInfo;
    MonsterVars *mv;

    (void)gameTracker;

    collideInfo = instance->collideInfo;

    mv = (MonsterVars *)instance->extraData;

    if (instance->data == NULL)
    {
        return;
    }

    if ((unsigned char)collideInfo->type0 == 1)
    {
        HSphere *s0;

        s0 = collideInfo->prim0;

        if (s0->id == 9)
        {
            HSphere *s1;
            MonsterAttackAttributes *temp; // not from decls.h

            temp = mv->attackType;

            if ((temp != NULL) && ((unsigned char)collideInfo->type1 == (unsigned char)collideInfo->type0))
            {
                s1 = collideInfo->prim1;

                if (s1->id == 8)
                {
                    Instance *inst1;
                    int power;

                    power = temp->damage;

                    inst1 = collideInfo->inst1;

                    if ((mv->mvFlags & 0x20000000))
                    {
                        power *= 2;
                    }

                    MON_TurnOffWeaponSpheres(instance);

                    INSTANCE_Post(inst1, 0x400000, SetFXHitData(instance, (unsigned char)collideInfo->segment, (power << 8) / 128, 256));
                    INSTANCE_Post(inst1, 0x1000000, SetMonsterHitData(instance, NULL, power << 8, temp->knockBackDistance, temp->knockBackDuration));

                    if (inst1 == mv->enemy->instance)
                    {
                        if ((mv->mvFlags & 0x10000000))
                        {
                            mv->enemy->mirFlags |= 0x1000;
                        }

                        mv->enemy->mirConditions |= 0x100;
                    }
                }
            }
        }
        else
        {
            int move;

            move = 1;

            switch ((unsigned char)collideInfo->type1)
            {
            case 4:
                break;
            case 1:
            {
                HSphere *hsphere;

                hsphere = collideInfo->prim1;

                SendHitObject(instance, collideInfo->inst1, 1);

                if (hsphere->id != 9)
                {
                    if ((instance->position.x == instance->oldPos.x) && (instance->position.y == instance->oldPos.y) && (instance->position.z == instance->oldPos.z))
                    {
                        move = 0;
                    }
                }
                else
                {
                    move = 0;
                }

                break;
            }
            case 2:
                SendHitObject(instance, collideInfo->inst1, 2);
                break;
            case 5:
                if ((collideInfo->offset.z < -50) && (!(((MonsterAttributes *)instance->data)->whatAmI & 0x10004)))
                {
                    INSTANCE_Post(instance, 0x40017, 6);
                }

                SendHitObject(instance, collideInfo->inst1, 5);
                break;
            case 3:
            {
                BSPTree *bsp;

                bsp = collideInfo->inst1;

                if ((bsp->flags & 0xE0))
                {
                    MON_CheckTerrainAndRespond(instance, bsp, collideInfo->prim1);
                }

                if ((bsp->flags & 0x102))
                {
                    move = 0;
                    break;
                }

                {
                    evMonsterHitTerrainData *data;
                    Level *level;
                    Intro *impaler;
                    TFace *tface;

                    data = CIRC_Alloc(sizeof(evMonsterHitTerrainData));

                    tface = collideInfo->prim1;

                    level = collideInfo->level;

                    data->faceFlags = (tface->textoff != 0xFFFF) ? ((TextureFT3 *)((char *)level->terrain->StartTextureList + tface->textoff))->attr : 0;

                    data->tface = tface;

                    data->bspFlags = bsp->flags;

                    if ((!(INSTANCE_Query(instance, 1) & 0x4)) || (tface->textoff == 0xFFFF) || (!(((TextureFT3 *)((char *)level->terrain->StartTextureList + tface->textoff))->attr & 0x1000)))
                    {
                        impaler = MON_TestForTerrainImpale(instance, level->terrain);

                        if (impaler != NULL)
                        {
                            INSTANCE_Post(instance, 0x100001C, impaler->UniqueID);

                            collideInfo->offset.x = 0;
                            collideInfo->offset.y = 0;
                        }
                        else if (COLLIDE_FindCollisionFaceNormal(collideInfo, &data->normal) != 0)
                        {
                            collideInfo->offset.x += data->normal.x >> 10;
                            collideInfo->offset.y += data->normal.y >> 10;

                            INSTANCE_Post(instance, 0x1000007, (intptr_t)data);
                        }

                        {
                            CollideInfo parentCI;

                            mv->mvFlags |= 0x8;

                            if (instance->LinkParent != NULL)
                            {
                                parentCI = *collideInfo;

                                if (parentCI.inst0 == instance)
                                {
                                    parentCI.inst0 = instance->LinkParent;
                                }

                                if (parentCI.inst1 == instance)
                                {
                                    parentCI.inst1 = instance->LinkParent;
                                }

                                INSTANCE_Post(instance->LinkParent, 0x200004, SetCollideInfoData(&parentCI));
                            }
                        }
                    }
                    else
                    {
                        move = 0;
                    }
                }

                break;
            }
            }

            if (move != 0)
            {
                instance->position.x += collideInfo->offset.x;
                instance->position.y += collideInfo->offset.y;
                instance->position.z += collideInfo->offset.z;

                COLLIDE_UpdateAllTransforms(instance, (SVECTOR *)&collideInfo->offset);
            }
        }
    }
}

void MonsterAdditionalCollide(Instance *instance, GameTracker *gameTracker)
{
    (void)gameTracker;

    if (instance->data != NULL)
    {
        MON_CheckEnvironment(instance);
    }
}

uintptr_t MonsterQuery(Instance *instance, unsigned long query)
{
    MonsterVars *mv;
    MonsterAttributes *ma;
    uintptr_t ret;
    evShadowSegmentData *shadowData;
    MonsterSaveInfo *saveInfo;

    mv = (MonsterVars *)instance->extraData;

    ma = (MonsterAttributes *)instance->data;

    ret = 0;

    if (ma == NULL)
    {
        return ret;
    }

    switch (query)
    {
    case 38:
        shadowData = (evShadowSegmentData *)SetShadowSegmentData(2);

        shadowData->shadowSegments[0] = ma->leftFootSegment;
        shadowData->shadowSegments[1] = ma->rightFootSegment;

        ret = (intptr_t)shadowData;
        break;
    case 0:
        if ((mv->mvFlags & 0x200))
        {
            ret = 0x40000000;
            break;
        }

        if (instance->currentMainState == MONSTER_STATE_PETRIFIED)
        {
            ret = 0x12000000;
            break;
        }

        ret = 0;

        if (!(mv->mvFlags & 0x200000))
        {
            if (((mv->mvFlags & 0x10)) || (mv->enemy == NULL) || (instance->currentMainState == MONSTER_STATE_SURPRISED))
            {
                if (mv->subAttr->grabable != 0)
                {
                    ret |= 0x90000000;
                }
                else
                {
                    ret |= 0x10000000;
                }
            }
        }
        else
        {
            ret = 0x4000000;
        }

        if ((mv->mvFlags & 0x100))
        {
            ret |= 0x20000000;
        }

        break;
    case 1:
        ret = ma->whatAmI;
        break;
    case 2:
        ret = (intptr_t)mv->subAttr->physAbility;
        break;
    case 9:
        if ((mv->mvFlags & 0x400))
        {
            ret = 16;
        }
        else
        {
            ret = 32;
        }

        break;
    case 10:
        ret = mv->mode;
        break;
    case 13:
        if (instance->matrix != NULL)
        {
            ret = (intptr_t)&instance->matrix[1];
        }
        else
        {
            ret = 0;
        }

        break;
    case 12:
        if (instance->matrix != NULL)
        {
            ret = (intptr_t)&instance->matrix[ma->headSegment];
        }
        else
        {
            ret = 0;
        }

        break;
    case 15:
        ret = (intptr_t)ma->neckSegment;
        break;
    case 25:
        ret = (intptr_t)mv->soulJuice;
        break;
    case 24:
        saveInfo = (MonsterSaveInfo *)CIRC_Alloc(sizeof(MonsterSaveInfo));

        MON_SetUpSaveInfo(instance, saveInfo);

        ret = SetControlSaveDataData(sizeof(MonsterSaveInfo), saveInfo);
        break;
    case 33:
        ret = mv->mvFlags >> 29;

        ret &= 0x1;
        break;
    case 35:
        ret = (intptr_t)PeekMessageQueue(&mv->messageQueue);
        break;
    case 47:
        ret = 0;

        if ((instance->currentMainState == MONSTER_STATE_THROWN) || (instance->currentMainState == MONSTER_STATE_IMPACT) || (instance->currentMainState == MONSTER_STATE_FALL) || (instance->currentMainState == MONSTER_STATE_GRABBED))
        {
            ret = 1;
        }

        break;
    case 37:
        ret = 1;
        break;
    default:
        ret = GenericQuery(instance, query);
    }

    return ret;
}

void MonsterMessage(Instance *instance, unsigned long message, unsigned long data)
{
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    if (mv != NULL)
    {
        switch (message)
        {
        case 0x40026:
            PurgeMessageQueue(&mv->messageQueue);
            break;
        case 0x1000020:
            if (mv->enemy != NULL)
            {
                mv->enemy->mirFlags &= ~0x1000;
            }

            break;
        case 0x100007:
            MON_GetSaveInfo(instance, (MonsterSaveInfo *)((evControlSaveDataData *)data)->data);
            break;
        }

        if (!(instance->flags2 & 0x4000000))
        {
            switch (message)
            {
            case 0x1000024:
                mv->mvFlags |= 0x40;
                return;
            case 0x200000:
                MONSENSE_StartMonsterIRList(instance);
                return;
            case 0x200001:
                MONSENSE_SenseInstance(instance, (evCollideInstanceStatsData *)data);
                return;
            case 0x100008:
                MON_RelocateCoords(instance, (SVector *)data);
                return;
            case 0x4000A:
                STREAM_SetInstancePosition(instance, (evPositionData *)data);
                return;
            case 0x4000B:
                instance->rotation.x = ((evPositionData *)data)->x;
                instance->rotation.y = ((evPositionData *)data)->y;
                instance->rotation.z = ((evPositionData *)data)->z;
                return;
            case 0x4000E:
                if ((uintptr_t *)data != NULL)
                {
                    mv->mvFlags |= 0x4;
                }
                else
                {
                    mv->mvFlags &= ~0x4;
                }

                if ((instance->currentMainState == MONSTER_STATE_PURSUE) || (instance->currentMainState == MONSTER_STATE_WANDER) || (instance->currentMainState == MONSTER_STATE_PROJECTILE) || (instance->currentMainState == MONSTER_STATE_FLEE))
                {
                    MON_SwitchStateDoEntry(instance, MONSTER_STATE_IDLE);
                }

                return;
            case 0x40013:
                if ((uintptr_t *)data != NULL)
                {
                    mv->mvFlags |= 0x80000;
                }
                else
                {
                    mv->mvFlags &= ~0x80000;
                }

                return;
            }

            EnMessageQueueData(&mv->messageQueue, message, data);
        }
    }
}

void AnimDistanceAndVel(Object *object, MonsterAnim *mAnim)
{
    G2Anim anim;
    G2AnimSection *animSection;
    G2AnimKeylist *keylist;
    long total;

    keylist = object->animList[(int)*mAnim->index];

    G2Anim_Init(&anim, object->modelList[0]);

    animSection = &anim.section[0];

    animSection->firstSeg = 0;

    animSection->segCount = (unsigned char)object->modelList[0]->numSegments;

    animSection->callback = NULL;
    animSection->callbackData = NULL;

    G2AnimSection_SetInterpInfo(animSection, NULL);

    G2AnimSection_SwitchToKeylistAtTime(animSection, keylist, *mAnim->index, 0);

    if (G2Anim_SegmentHasActiveChannels(&anim, 0, 0x700) != G2FALSE)
    {
        G2SVector3 dist;

        G2Anim_GetRootMotionOverInterval(&anim, 0, G2AnimKeylist_GetDuration(keylist), &dist);

        total = (int)MATH3D_FastSqrt0((dist.x * dist.x) + (dist.y * dist.y) + (dist.z * dist.z));

        mAnim->velocity = ((mAnim->playSpeed * 100) * (int)total) / (G2AnimKeylist_GetDuration(keylist) << 12);

        mAnim->distance = (int)total;
    }

    G2Anim_Free(&anim);
}

void TranslateAnimList(Object *object, MonsterAnim *animList, int numAnims)
{
    int i;

    if (animList != NULL)
    {
        MonsterAnim *animPtr;

        animPtr = animList;

        for (i = 0; i < numAnims; i++, animPtr++)
        {
            int index;
            G2AnimKeylist *keylist;

            index = *animPtr->index;

            keylist = object->animList[index];

            if (index != -1)
            {
                AnimDistanceAndVel(object, animPtr);

                if (animPtr->startFrame >= (G2AnimKeylist_GetDuration(keylist) / 100))
                {
                    animPtr->startFrame = G2AnimKeylist_GetKeyframeCount(keylist) - 1;
                }
            }
        }
    }
}

void MonsterTranslateAnim(Object *object)
{
    MonsterAttributes *attributes;
    int i;
    void *relocModule;
    MonsterAttackAttributes *attackAttr;
    MonsterCombatAttributes *combatAttr;
    int largest;
    int j;
    MonsterAttackAttributes *attack;

    attributes = (MonsterAttributes *)object->data;

    if (!(object->oflags2 & 0x10000000))
    {
        if ((object->oflags & 0x8000000))
        {
            relocModule = object->relocModule;

            if ((relocModule == NULL) || ((intptr_t)((Dummy3 *)relocModule)->unknown != (intptr_t)monVersion)) // relocModule needs parsing to the correct struct
            {
                object->data = NULL;
                return;
            }
        }
        else
        {
            MONTABLE_SetupTablePointer(object);
        }

        if ((attributes != NULL) && (attributes->magicnum == 0xACE00065))
        {
            TranslateAnimList(object, attributes->animList, attributes->numAnims);

            for (i = 0; i < attributes->numAttackAttributes; i++)
            {
                attackAttr = &attributes->attackAttributesList[i];

                if ((attackAttr->sphereOnAnim >= attackAttr->numAnims) || (attackAttr->sphereOffAnim >= attackAttr->numAnims))
                {
                    object->data = NULL;
                }
            }

            for (i = 0; i < attributes->numCombatAttributes; i++)
            {
                combatAttr = attributes->combatAttributesList[i];

                if (combatAttr->combatRange == 0)
                {
                    largest = 0;

                    for (j = 0; j < combatAttr->numAttacks; j++)
                    {
                        attack = &attributes->attackAttributesList[(int)combatAttr->attackList[j]];

                        if (largest < attack->attackRange)
                        {
                            largest = attack->attackRange;
                        }
                    }

                    combatAttr->combatRange = largest + 200;
                }
            }
        }
    }
}

void MonsterRelocateTune(Object *object, long offset)
{
    MonsterAttributes *attributes;

    attributes = object->data;

    if ((attributes != NULL) && (attributes->magicnum == 0xACE00065))
    {
        int i;
        MonsterSubAttributes *subAttr;
        MonsterSubAttributes **pSubAttr;
        MonsterCombatAttributes **pCombatAttr;

        attributes->tunData = (void *)OFFSET_DATA(attributes->tunData, offset);
        attributes->auxAnimList = (char *)OFFSET_DATA(attributes->auxAnimList, offset);
        attributes->ambientAnimList = (char *)OFFSET_DATA(attributes->ambientAnimList, offset);
        attributes->subAttributesList = (MonsterSubAttributes **)OFFSET_DATA(attributes->subAttributesList, offset);
        attributes->combatAttributesList = (MonsterCombatAttributes **)OFFSET_DATA(attributes->combatAttributesList, offset);
        attributes->attackAttributesList = (MonsterAttackAttributes *)OFFSET_DATA(attributes->attackAttributesList, offset);
        attributes->missileList = (MonsterMissile *)OFFSET_DATA(attributes->missileList, offset);
        attributes->animList = (MonsterAnim *)OFFSET_DATA(attributes->animList, offset);
        attributes->idleList = (MonsterIdle *)OFFSET_DATA(attributes->idleList, offset);
        attributes->behaviorList = (MonsterBehavior *)OFFSET_DATA(attributes->behaviorList, offset);
        attributes->shatterList = (void *)OFFSET_DATA(attributes->shatterList, offset);

        pSubAttr = attributes->subAttributesList;

        for (i = 0; i < attributes->numSubAttributes; i++, pSubAttr++)
        {
            int j;
            MonsterSubAttributes **oldSubAttr;

            subAttr = *pSubAttr = (MonsterSubAttributes *)OFFSET_DATA(*pSubAttr, offset);

            oldSubAttr = attributes->subAttributesList;

            for (j = 0; j < i; j++, oldSubAttr++)
            {
                if (*oldSubAttr == subAttr)
                {
                    break;
                }
            }

            if (j == i)
            {
                subAttr->animList = (char *)OFFSET_DATA(subAttr->animList, offset);
                subAttr->senses = (MonsterSenses *)OFFSET_DATA(subAttr->senses, offset);
                subAttr->combatAttributes = (MonsterCombatAttributes *)OFFSET_DATA(subAttr->combatAttributes, offset);
                subAttr->allegiances = (MonsterAllegiances *)OFFSET_DATA(subAttr->allegiances, offset);
                subAttr->behaviorList = (char *)OFFSET_DATA(subAttr->behaviorList, offset);
            }
        }

        pCombatAttr = attributes->combatAttributesList;

        for (i = attributes->numCombatAttributes; i != 0; i--, pCombatAttr++)
        {
            *pCombatAttr = (MonsterCombatAttributes *)OFFSET_DATA(*pCombatAttr, offset);
        }
    }
}

void MonsterRelocateInstanceObject(Instance *instance, long offset)
{
    MonsterVars *mv;
    Dummy4 *temp;  // not from decls.h
    Dummy4 *temp2; // not from decls.h

    mv = (MonsterVars *)instance->extraData;

    if (mv != NULL)
    {
        mv->subAttr = (MonsterSubAttributes *)OFFSET_DATA(mv->subAttr, offset);
        mv->attackType = (MonsterAttackAttributes *)OFFSET_DATA(mv->attackType, offset);
        mv->anim = (MonsterAnim *)OFFSET_DATA(mv->anim, offset);

        if ((instance->object->oflags & 0x8000000))
        {
            temp = (Dummy4 *)instance->object->relocModule;

            if ((uintptr_t)temp < (uintptr_t)temp->unknown)
            {
                instance->queryFunc = (void *)OFFSET_DATA(instance->queryFunc, offset);
            }

            temp2 = (Dummy4 *)instance->object->relocModule;

            if ((uintptr_t)temp2 < (uintptr_t)temp2->unknown2)
            {
                instance->messageFunc = (void *)OFFSET_DATA(instance->messageFunc, offset);
            }

            if ((instance->collideFunc != MonsterCollide) && ((uintptr_t)instance->object->relocModule < (uintptr_t)instance->collideFunc))
            {
                instance->collideFunc = (void *)OFFSET_DATA(instance->collideFunc, offset);
            }
        }
    }
}

void MONAPI_TurnHead(Instance *instance, short *rotx, short *rotz, GameTracker *gameTracker)
{
    (void)gameTracker;

    if (*rotx < -768)
    {
        *rotx = -768;
    }
    else if (*rotx > 512)
    {
        *rotx = 512;
    }

    if (*rotz > 1024)
    {
        *rotz = 1024;
    }
    else if (*rotz < -1024)
    {
        *rotz = -1024;
    }

    MON_LookInDirection(instance, (unsigned short)*rotx * -1, *rotz);
}

void MONAPI_SetLookAround(Instance *instance)
{
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    MON_EnableHeadMove(instance);

    mv->mode = 0x80000;
}

void MONAPI_ResetLookAround(Instance *instance)
{
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    MON_DisableHeadMove(instance);

    mv->mode = 0x1;
}

long MONAPI_OkToLookAround(Instance *instance)
{
    return instance->currentMainState == MONSTER_STATE_IDLE;
}

void MONAPI_DeleteRegen(MONAPI_Regenerator *regen)
{
    GlobalSave->numRegens--;

    memcpy(regen, &regen[1], ((signed char)GlobalSave->numRegens - (regen - GlobalSave->regenEntries)) * sizeof(MONAPI_Regenerator));
}

void MONAPI_ProcessGenerator()
{
    MONAPI_Regenerator *regen;
    int i;

    regen = &GlobalSave->regenEntries[0];

    if ((GlobalSave->flags & 0x1))
    {
        return;
    }

    for (i = 0; i < (signed char)GlobalSave->numRegens;)
    {
        unsigned long time;

        if (gameTrackerX.gameData.asmData.MorphType != 0)
        {
            time = gameTrackerX.currentSpectralTime;
        }
        else
        {
            time = gameTrackerX.currentMaterialTime;
        }

        if (regen->regenTime < time)
        {
            Level *level;

            level = STREAM_GetLevelWithID(regen->streamUnitID);

            if (level != NULL)
            {
                Intro *intro;
                int j;
                long id;

                id = regen->introUniqueID;

                for (j = level->numIntros, intro = level->introList; j != 0; j--, intro++)
                {
                    if (intro->UniqueID == id)
                    {
                        if ((intro->flags & 0x400))
                        {
                            MONAPI_DeleteRegen(regen);
                        }
                        else if (INSTANCE_IntroduceInstance(intro, regen->streamUnitID) == NULL)
                        {
                            MONAPI_DeleteRegen(regen);
                        }

                        break;
                    }
                }
            }
            else
            {
                MONAPI_DeleteRegen(regen);
            }
        }
        else
        {
            i++;
            regen++;
        }
    }
}

void MONAPI_AddToGenerator(Instance *instance)
{
    MonsterVars *mv;
    MONAPI_Regenerator *regen;

    if (GlobalSave->numRegens < 9)
    {
        mv = (MonsterVars *)instance->extraData;

        regen = &GlobalSave->regenEntries[(int)GlobalSave->numRegens];

        GlobalSave->numRegens++;

        regen->regenTime = MON_GetTime(instance) + (mv->regenTime * 1000);

        regen->introUniqueID = instance->introUniqueID;
        regen->streamUnitID = instance->birthStreamUnitID;
    }
}

void MONAPI_CheckGenerator(Instance *instance)
{
    int i;
    MONAPI_Regenerator *regen;
    int id;

    regen = GlobalSave->regenEntries;

    id = instance->introUniqueID;

    for (i = 0; i < GlobalSave->numRegens; i++, regen++)
    {
        if (regen->introUniqueID == id)
        {
            MONAPI_DeleteRegen(regen);
            break;
        }
    }
}
