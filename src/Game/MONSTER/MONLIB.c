#include "Game/MONSTER/MONLIB.h"
#include "Game/PLAN/PLANAPI.h"
#include "Game/PLAN/ENMYPLAN.h"
#include "Game/MONSTER/MONSTER.h"
#include "Game/MONSTER/MONSENSE.h"
#include "Game/MONSTER/MONAPI.h"
#include "Game/MONSTER/MONMSG.h"
#include "Game/PHYSOBS.h"
#include "Game/INSTANCE.h"
#include "Game/COLLIDE.h"
#include "Game/MONSTER/MONTABLE.h"
#include "Game/PLAN/ENMYPLAN.h"
#include "Game/STATE.h"
#include "Game/MATH3D.h"
#include "Game/PHYSICS.h"
#include "Game/FX.h"
#include "Game/G2/ANIMG2.h"
#include "Game/CAMERA.h"
#include "Game/GAMELOOP.h"
#include "Game/SOUND.h"
#include "Game/STREAM.h"
#include "Game/G2/ANIMG2.h"
#include "Game/OBTABLE.h"
#include "Game/SAVEINFO.h"
#include "Game/G2/ANMG2ILF.h"
#include "Game/G2/ANMCTRLR.h"

void MON_TurnOffWeaponSpheres(Instance *instance)
{
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    if (instance->LinkChild != NULL)
    {
        Instance *weapon;

        for (weapon = instance->LinkChild; weapon != NULL; weapon = weapon->LinkSibling)
        {
            TurnOffCollisionPhysOb(weapon, 3);
        }
    }

    if ((mv->mvFlags & 0x4000))
    {
        int i;
        HPrim *hprim;
        HModel *hmodel;

        hmodel = &instance->hModelList[instance->currentModel];

        hprim = hmodel->hPrimList;

        for (i = hmodel->numHPrims; i != 0; i--, hprim++)
        {
            if ((hprim->type == 1) && (hprim->data.hsphere->id == 9))
            {
                hprim->hpFlags &= ~0x1;
            }
        }

        mv->mvFlags &= ~0x4000;
    }
}

void MON_TurnOnWeaponSpheres(Instance *instance)
{
    MonsterVars *mv;
    int i;
    HPrim *hprim;
    HModel *hmodel;

    mv = (MonsterVars *)instance->extraData;

    if ((!(mv->mvFlags & 0x4000)) && (instance->hModelList != NULL))
    {
        hmodel = &instance->hModelList[instance->currentModel];

        hprim = hmodel->hPrimList;

        for (i = hmodel->numHPrims; i != 0; i--, hprim++)
        {
            if ((hprim->type == 1) && (hprim->data.hsphere->id == 9))
            {
                hprim->hpFlags |= 0x1;
                break;
            }
        }

        mv->mvFlags |= 0x4000;
    }
}

void MON_TurnOnWeaponSphere(Instance *instance, int segment)
{
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    if (instance->LinkChild != NULL)
    {
        Instance *weapon;

        weapon = INSTANCE_GetChildLinkedToSegment(instance, segment);

        if (weapon != NULL)
        {
            TurnOnCollisionPhysOb(weapon, 3);

            mv->mvFlags |= 0x4000;
        }
    }
    else if ((!(mv->mvFlags & 0x4000)) && (instance->hModelList != NULL))
    {
        int i;
        HPrim *hprim;
        HModel *hmodel;

        hmodel = &instance->hModelList[instance->currentModel];

        hprim = hmodel->hPrimList;

        for (i = hmodel->numHPrims; i != 0; i--, hprim++)
        {
            if ((hprim->segment == segment) && (hprim->type == 1) && (hprim->data.hsphere->id == 9))
            {
                hprim->hpFlags |= 0x1;
            }
        }

        mv->mvFlags |= 0x4000;
    }
}

void MON_TurnOffBodySpheres(Instance *instance)
{
    MonsterVars *mv;
    int i;
    HPrim *hprim;
    HModel *hmodel;

    mv = (MonsterVars *)instance->extraData;

    if ((mv->mvFlags & 0x8000))
    {
        hmodel = &instance->hModelList[instance->currentModel];

        hprim = hmodel->hPrimList;

        for (i = hmodel->numHPrims; i != 0; i--, hprim++)
        {
            if ((hprim->type == 1) && (hprim->data.hsphere->id == 8))
            {
                hprim->hpFlags &= ~0x1;
            }
        }

        mv->mvFlags &= ~0x8000;
    }
}

void MON_TurnOnBodySpheres(Instance *instance)
{
    MonsterVars *mv;
    int i;
    HPrim *hprim;
    HModel *hmodel;

    mv = (MonsterVars *)instance->extraData;

    if ((!(mv->mvFlags & 0x8000)) && (instance->hModelList != NULL))
    {
        hmodel = &instance->hModelList[instance->currentModel];

        hprim = hmodel->hPrimList;

        for (i = hmodel->numHPrims; i != 0; i--, hprim++)
        {
            if ((hprim->type == 1) && (hprim->data.hsphere->id == 8))
            {
                hprim->hpFlags |= 0x1;
            }
        }

        mv->mvFlags |= 0x8000;
    }
}

void MON_TurnOffAllSpheres(Instance *instance)
{
    MON_TurnOffWeaponSpheres(instance);

    MON_TurnOffBodySpheres(instance);
}

void MON_TurnOnAllSpheres(Instance *instance)
{
    MON_TurnOnWeaponSpheres(instance);

    MON_TurnOnBodySpheres(instance);
}

void MON_SwitchState(Instance *instance, enum MonsterState state)
{
    MonsterVars *mv;
    int temp; // not from decls.h

    mv = (MonsterVars *)instance->extraData;

    if ((mv->mvFlags & 0x4000))
    {
        MON_TurnOffWeaponSpheres(instance);
    }

    if (!(mv->mvFlags & 0x1))
    {
        mv->previousMainState = (char)instance->currentMainState;
    }

    if ((state == MONSTER_STATE_GENERALDEATH) || (state == MONSTER_STATE_DEAD) || (state == MONSTER_STATE_IMPALEDEATH) || (state == MONSTER_STATE_GRABBED))
    {
        PurgeMessageQueue(&mv->messageQueue);
    }
    else if (state == MONSTER_STATE_MISSILEHIT)
    {
        PurgeMessageQueue(&mv->messageQueue);
    }

    instance->currentMainState = state;

    mv->mvFlags |= 0x1;
    mv->mvFlags &= ~0x1000;
    mv->mvFlags &= ~0x20000;
    mv->mvFlags &= ~0x40000000;
    mv->mvFlags &= ~0x4000000;

    temp = -1;

    if ((char)mv->pathSlotID != temp)
    {
        ENMYPLAN_ReleasePlanningWorkspace((char)mv->pathSlotID);

        mv->pathSlotID = temp;
    }
}

void MON_SwitchStateDoEntry(Instance *instance, enum MonsterState state)
{
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    MON_SwitchState(instance, state);

    if (mv != NULL)
    {
        (((MonsterState *)MONTABLE_GetStateFuncs(instance, instance->currentMainState))->entryFunction)(instance);

        mv->mvFlags &= ~0x1;
    }
}

int MON_TransNodeAnimation(Instance *instance)
{
    return G2Anim_SegmentHasActiveChannels(&instance->anim, 0, 0x700);
}

MonsterAnim *MON_GetAnim(Instance *instance, char *animList, int index)
{
    int whichAnim;
    MonsterAttributes *temp; // not from decls.h

    whichAnim = animList[index];

    if (whichAnim == -1)
    {
        return NULL;
    }

    temp = (MonsterAttributes *)instance->data;

    return &temp->animList[whichAnim];
}

void MON_PlayAnimID(Instance *instance, int index, int mode)
{
    MonsterVars *mv;
    MonsterAnim *manim;
    MonsterAttributes *ma;
    int anim;
    int anim0;
    int i;
    int interpFrames;
    int alphaTable;

    ma = (MonsterAttributes *)instance->data;

    mv = (MonsterVars *)instance->extraData;

    manim = &ma->animList[index];

    anim0 = *manim->index;

    if ((anim0 < 0) || (anim0 >= instance->object->numAnims))
    {
        anim0 = 0;
    }

    G2Anim_SetCallback(&instance->anim, (void *)INSTANCE_DefaultAnimCallback, instance);

    if ((mv->anim != NULL) && (mv->anim->interpOut != 0))
    {
        interpFrames = mv->anim->interpOut;

        alphaTable = mv->anim->alphaTableOut;
    }
    else
    {
        interpFrames = manim->interpFrames;

        alphaTable = manim->alphaTable;
    }

    for (i = 0; i < mv->subAttr->numSections; i++)
    {
        anim = manim->index[i];

        if (anim == -1)
        {
            anim = anim0;
        }

        G2EmulationInstanceSwitchAnimationAlpha(instance, i, anim, manim->startFrame, interpFrames, mode, alphaTable);

        G2EmulationInstanceSetAnimSpeed(instance, i, manim->playSpeed);
    }

    mv->anim = manim;

    mv->mvFlags |= 0x4000000;

    (instance->anim.section + manim->controllingSection)->callback = MON_AnimCallback;
    (instance->anim.section + manim->controllingSection)->callbackData = instance;
}

void MON_PlayAnimFromList(Instance *instance, char *animList, int animtype, int mode)
{
    MON_PlayAnimID(instance, animList[animtype], mode);
}

int MON_AnimIDPlaying(Instance *instance, int index)
{
    MonsterAttributes *ma;
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    ma = (MonsterAttributes *)instance->data;

    return mv->anim == &ma->animList[index];
}

void MON_PlayAnimIDIfNotPlaying(Instance *instance, int index, int mode)
{
    if (MON_AnimIDPlaying(instance, index) == 0)
    {
        MON_PlayAnimID(instance, index, mode);
    }
}

int MON_AnimPlayingFromList(Instance *instance, char *animList, int animtype)
{
    return MON_AnimIDPlaying(instance, animList[animtype]);
}

void MON_PlayAnimFromListIfNotPlaying(Instance *instance, char *animList, int animtype, int mode)
{
    int index;

    index = animList[animtype];

    if (MON_AnimIDPlaying(instance, index) == 0)
    {
        MON_PlayAnimID(instance, index, mode);
    }
}

void MON_PlayAnim(Instance *instance, enum MonsterAnim animtype, int mode)
{
    MON_PlayAnimFromList(instance, ((MonsterVars *)instance->extraData)->subAttr->animList, animtype, mode);
}

int MON_AnimPlaying(Instance *instance, enum MonsterAnim animtype)
{
    return MON_AnimPlayingFromList(instance, ((MonsterVars *)instance->extraData)->subAttr->animList, animtype);
}

void MON_PlayAnimIfNotPlaying(Instance *instance, enum MonsterAnim animtype, int mode)
{
    MON_PlayAnimFromListIfNotPlaying(instance, ((MonsterVars *)instance->extraData)->subAttr->animList, animtype, mode);
}

long MON_AnimCallback(G2Anim *anim, int sectionID, G2AnimCallbackMsg message, long messageDataA, long messageDataB, void *data)
{
    Instance *instance;
    MonsterVars *mv;

    instance = (Instance *)data;

    mv = (MonsterVars *)instance->extraData;

    switch (message)
    {
    case 1:
        instance->flags2 |= 0x10;

        mv->mvFlags &= ~0x4000000;
        break;
    case 2:
        instance->flags2 |= 0x2;
        break;
    case 4:
        break;
    default:
        INSTANCE_DefaultAnimCallback(anim, sectionID, message, messageDataA, messageDataB, instance);
    }

    return messageDataA;
}

void MON_AnimInit(Instance *instance)
{
    MonsterVars *mv;
    G2AnimSection *animSection;
    int i;
    int startSection;
    int numSections;
    int sectionEnd;

    mv = (MonsterVars *)instance->extraData;

    numSections = mv->subAttr->numSections;

    mv->anim = NULL;

    G2EmulationInstanceSetTotalSections(instance, numSections);

    startSection = 0;

    for (i = 0; i < numSections; i++)
    {
        sectionEnd = mv->subAttr->sectionEnd[i];

        animSection = &instance->anim.section[i];

        if (sectionEnd == 0)
        {
            G2EmulationInstanceSetStartAndEndSegment(instance, i, startSection, (short)instance->object->modelList[instance->currentModel]->numSegments - 1);
        }
        else
        {
            G2EmulationInstanceSetStartAndEndSegment(instance, i, startSection, sectionEnd);

            startSection = mv->subAttr->sectionEnd[i] + 1;
        }

        G2AnimSection_SetInterpInfo(animSection, &mv->interpInfo[i]);

        G2EmulationInstanceSetAnimation(instance, i, 0, 0, 0);
        G2EmulationInstanceSetMode(instance, i, 2);
    }

    G2EmulationInstanceInitSection(instance, 0, MON_AnimCallback, instance);

    MON_PlayRandomIdle(instance, 2);
}

short MON_FacingOffset(Instance *instance, Instance *target)
{
    return MATH3D_AngleFromPosToPos(&target->position, &instance->position) - target->rotation.z;
}

int MON_CheckConditions(Instance *instance, MonsterIR *mir, char *probArray)
{
    MonsterVars *mv;
    char *probability;
    int chance;
    int i;
    int prob;
    int nprob;

    mv = (MonsterVars *)instance->extraData;

    chance = (signed char)mv->chance;

    probability = probArray;

    prob = 100;
    nprob = 100;

    for (i = 1; i < 4096; i *= 2)
    {
        if ((mir->mirConditions & i))
        {
            int conditionProb;

            conditionProb = (signed char)*probability;

            if (conditionProb >= 0)
            {
                prob *= 100 - conditionProb;
                prob /= 100;
            }
            else
            {
                nprob *= conditionProb + 100;
                nprob /= 100;
            }
        }

        probability++;
    }

    prob = ((100 - prob) * nprob) / 100;

    return prob > chance;
}

int MON_ShouldIAttackInstance(Instance *instance, Instance *ei)
{
    if (ei == gameTrackerX.playerInstance)
    {
        Instance *enemyAttackee;
        long mode;
        MonsterVars *mv;

        enemyAttackee = (Instance *)INSTANCE_Query(ei, 34);

        mode = INSTANCE_Query(ei, 10);

        mv = (MonsterVars *)instance->extraData;

        if ((mode & 0x20010000)) return 0;
        {
            if (((enemyAttackee != NULL) && (enemyAttackee != instance)) && ((mode & 0x2000000)))
            {
                return 0;
            }

            if (INSTANCE_Query(ei, 46) != 0)
            {
                return 0;
            }

            if ((mv->behaviorState == MONSTER_STATE_IMPALEDEATH) && ((instance->intro != NULL) && (instance->matrix != NULL)) && (((MATH3D_LengthXYZ(ei->position.x - instance->intro->position.x, ei->position.y - instance->intro->position.y, ei->position.z - instance->intro->position.z) > (mv->guardRange + 640)) != 0)))
            {
                return 0;
            }
        }
    }

    return 1;
}

int MON_ShouldIAttack(Instance *instance, MonsterIR *enemy, MonsterAttackAttributes *attack)
{

    MonsterVars *mv;
    int rv;
    Instance *ei;

    ei = enemy->instance;
    mv = (MonsterVars *)instance->extraData;

    if (MON_ShouldIAttackInstance(instance, ei) == 0)
    {
        return 0;
    }

    if (attack == NULL)
    {
        rv = 0;
    }
    else if (ei == gameTrackerX.playerInstance && MON_CheckConditions(instance, enemy, attack->attackProbability) == 0)
    {
        rv = 0;
    }
    else
    {

        long distance;
        distance = attack->attackRange - enemy->distance;

        if (enemy->mirFlags & 0x08)
        {
            rv = abs(distance) < 0x3E8 ? 1 : 5;
        }
        else if (distance < -0x96)
        {
            rv = 5;
        }
        else if (distance > 0x96)
        {
            rv = 4;
        }
        else if (enemy->relativePosition.y > 0 || abs(enemy->relativePosition.x) > -enemy->relativePosition.y)
        {
            rv = 6;
        }
        else if ((enemy->mirFlags & 0x20) ==0)
        {
            rv = 3;
        }
        else if (!(mv->ally == NULL || (INSTANCE_Query(mv->ally->instance, 0xA) & 0x200000) == 0))
        {
            rv = 7;
        }
        else if (mv->auxFlags & 0x20000000 || MONSENSE_GetDistanceInDirection(instance, MATH3D_AngleFromPosToPos(&instance->position, &ei->position)) > enemy->distance)
        {
            rv = 1;
            enemy->mirConditions = 0;
        }
        else
        {
            rv = 0;
        }
    }

    return rv;
}

MonsterAttackAttributes *MON_ChooseAttack(Instance *instance, MonsterIR *enemy)
{

    long distance;
    long zdiff;
    long smallest;
    int i;
    char *attackIndex;
    MonsterCombatAttributes *combat;
    MonsterVars *mv;
    MonsterAttackAttributes *bestAttack;
    MonsterAttributes *ma;

    bestAttack = NULL;
    zdiff = 0x100;
    smallest = 0x1869F;

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    combat = mv->subAttr->combatAttributes;

    if (mv->mvFlags & 4 || enemy->mirFlags & 8)
    {
        distance = 0;
    }
    else
    {
        distance = enemy->distance;
        if (enemy->instance->matrix != NULL)
        {
            if (enemy->instance == gameTrackerX.playerInstance)
            {
                zdiff = enemy->instance->matrix[14].t[2] - instance->position.z;
            }
            else
            {
                zdiff = enemy->instance->matrix[1].t[2] - instance->position.z;
            }
        }
    }

    i = (signed char)combat->numAttacks;
    attackIndex = combat->attackList;

    if (i != 0)
    {
        for (; i != 0; i--, attackIndex++)
        {

            long delta;
            MonsterAttackAttributes *attack;

            attack = &ma->attackAttributesList[(signed char)*attackIndex];
            delta = zdiff - attack->attackHeight;

            if (abs(delta) < 100 || enemy->mirFlags & 8)
            {

                int effectiveRange; // not from decls.h
                effectiveRange = attack->attackRange * mv->subAttr->scale;
                delta = (effectiveRange / 4096) - distance;

                if (abs(delta) < abs(smallest))
                {
                    bestAttack = attack;
                    smallest = delta;
                }
            }
        }
    }

    mv->attackType = bestAttack;
    mv->attackState = 0;
    return bestAttack;
}

int MON_ShouldIEvade(Instance *instance)
{

    int rv;
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;
    rv = 0;

    if (!(mv->mvFlags & 0x40000000))
    {

        MonsterIR *enemy;
        enemy = mv->enemy;

        if ((unsigned short)(MON_FacingOffset(instance, enemy->instance) + 0x2A9U) < 0x553U)
        {
            if (MON_CheckConditions(instance, enemy, mv->subAttr->combatAttributes->evadeProbability) != 0)
            {
                mv->enemy->mirConditions = 0;
                rv = 1;
            }
        }
    }

    return rv;
}

int MON_ChooseLeftOrRight(struct _Instance *instance, struct _MonsterIR *enemy)
{

    short offset;
    int leftDist;
    int rightDist;

    offset = MON_FacingOffset(instance, enemy->instance);

    if (offset >= -0x40)
    {
        if (offset >= 0x41)
        {
            return 1;
        }

        leftDist = MONSENSE_GetDistanceInDirection(instance, instance->rotation.z + 0x400);
        rightDist = MONSENSE_GetDistanceInDirection(instance, instance->rotation.z - 0x400);

        if (leftDist != rightDist)
        {
            if (leftDist < rightDist)
            {
                return 1;
            }
            return -1;
        }
        return 0;
    }
    return -1;
}

int MON_ChooseEvadeMove(Instance *instance)
{

    int anim;        // v0
    int leftOrRight; // v1

    leftOrRight = MON_ChooseLeftOrRight(instance, ((MonsterVars *)instance->extraData)->enemy);

    if (leftOrRight == 0)
    {
        if (rand() & 1)
        {
            leftOrRight = 1;
        }
        else
        {
            leftOrRight = -1;
        }
    }

    anim = MONSTER_ANIM_EVADERIGHT;
    if (leftOrRight < 0)
    {
        anim = MONSTER_ANIM_EVADELEFT;
    }

    return anim;
}

int MON_ChooseCombatMove(Instance *instance, int reason)
{
    struct _MonsterVars *mv;
    struct _MonsterSubAttributes *subAttr;
    int anim;
    int data;
    struct _MonsterIR *enemy;
    int infront;
    struct _MonsterAnim *manim;
    int dir;
    short zrot;
    struct _Position temp;
    struct SVECTOR New;
    struct VECTOR OutTrans;

    mv = (MonsterVars *)instance->extraData;
    enemy = mv->enemy;
    subAttr = mv->subAttr;

    infront = (unsigned)enemy->relativePosition.y >> 0x1F;

    if ((mv->ally != NULL) && (mv->ally->distance < 0x2BC))
    {
        anim = (mv->ally->relativePosition.x > 0) ? MONSTER_ANIM_JUMPRIGHT : MONSTER_ANIM_JUMPLEFT;
    }
    else if (reason == 4)
    {
        goto label;
    }
    else
    {
        anim = MONSTER_ANIM_JUMPFORWARD;
        if (reason != 5)
        {
            if (abs(enemy->relativePosition.y) < subAttr->combatAttributes->preferredCombatRange)
            {
                if (infront != 0)
                {
                label:
                    anim = MONSTER_ANIM_JUMPBACK;
                }
            }
            else
            {
                zrot = MON_FacingOffset(instance, enemy->instance);
                if (zrot > -682 && zrot < 682)
                {
                    data = MON_ChooseLeftOrRight(instance, enemy);

                    if (data < 0)
                    {
                        anim = MONSTER_ANIM_JUMPLEFT;
                    }
                    else if (data > 0)
                    {
                        anim = MONSTER_ANIM_JUMPRIGHT;
                    }
                    else if (!(rand() & 3))
                    {
                        anim = rand() & 1 ? MONSTER_ANIM_JUMPRIGHT : MONSTER_ANIM_JUMPLEFT;
                    }
                    else
                    {
                        anim = (signed char)mv->lastSideMove;
                    }
                }
                else
                {
                    anim = MONSTER_ANIM_HIT1;
                }

                if (anim != MONSTER_STATE_BIRTH)
                {
                    data = anim;
                }
            }
        }
    }

    if (anim != MONSTER_ANIM_HIT1)
    {
        zrot = instance->rotation.z;

        switch (anim)
        {
        case MONSTER_ANIM_JUMPLEFT:
            zrot += 0x400;
            break;
        case MONSTER_ANIM_JUMPRIGHT:
            zrot -= 0x400;
            break;
        case MONSTER_ANIM_JUMPBACK:
            zrot += 0x800;
            break;
        }

        MONSENSE_AdjustRadarFromObjects(instance);
        dir = (short)((MONSENSE_GetClosestFreeDirection(instance, zrot, 0x2EE) - (unsigned short)instance->rotation.z) + 0x200) & 0xFFF;
        anim = MONSTER_ANIM_JUMPFORWARD;
        if (dir >= 0x400)
        {
            anim = MONSTER_ANIM_JUMPLEFT;
            if (dir >= 0x800)
            {
                anim = MONSTER_ANIM_JUMPRIGHT;
                if (dir < 0xC00)
                {
                    anim = MONSTER_ANIM_JUMPBACK;
                }
            }
        }

        manim = &((MonsterAttributes *)instance->data)->animList[(signed char)subAttr->animList[anim]];

        if (mv->behaviorState == MONSTER_STATE_IMPALEDEATH && instance->intro != NULL)
        {
            if (instance->matrix != NULL)
            {

                New.vx = 0;
                New.vz = 0;

                if (anim == MONSTER_ANIM_JUMPLEFT || anim == MONSTER_ANIM_JUMPBACK)
                {
                    New.vy = manim->distance;
                }
                else
                {
                    New.vy = -manim->distance;
                }

                ApplyMatrix(instance->matrix, &New, &OutTrans);
                temp.x = instance->position.x + OutTrans.vx;
                temp.y = instance->position.y + OutTrans.vy;
                temp.z = instance->position.z + OutTrans.vz;
                if (MATH3D_LengthXYZ(temp.x - instance->intro->position.x, temp.y - instance->intro->position.y, temp.z - instance->intro->position.z) > (mv->guardRange + 0x140))
                {
                    zrot = (MATH3D_AngleFromPosToPos(&instance->position, &instance->intro->position) - instance->rotation.z) & 0xFFF;
                    if (zrot < 0x400)
                    {
                        anim = MONSTER_ANIM_JUMPFORWARD;
                    }
                    else if (zrot < 0x800)
                    {
                        anim = MONSTER_ANIM_JUMPLEFT;
                    }
                    else if (zrot < 0xC00)
                    {
                        anim = MONSTER_ANIM_JUMPBACK;
                    }
                    else
                    {
                        anim = MONSTER_ANIM_JUMPRIGHT;
                    }
                    manim = &((MonsterAttributes *)instance->data)->animList[(signed char)subAttr->animList[anim]];
                }
            }
        }

        switch (anim)
        {
        case MONSTER_ANIM_JUMPLEFT:
            data = SetPhysicsDropOffData(manim->distance, 0, 0xC8, 0x800, 0x190);
            break;
        case MONSTER_ANIM_JUMPRIGHT:
            data = SetPhysicsDropOffData(-manim->distance, 0, 0xC8, 0x800, 0x190);
            break;
        case MONSTER_ANIM_JUMPBACK:
            data = SetPhysicsDropOffData(0, manim->distance, 0xC8, 0x800, 0x190);
            break;
        default:
            data = SetPhysicsDropOffData(0, -manim->distance, 0xC8, 0x800, 0x190);
            break;
        }

        if (PhysicsCheckDropOff(instance, data, 4) != 0)
        {
            anim = MONSTER_ANIM_HIT1;
        }
        else if (anim == MONSTER_ANIM_JUMPLEFT || anim == MONSTER_ANIM_JUMPRIGHT)
        {
            mv->lastSideMove = anim;
        }
    }
    return anim;
}

void MON_PlayRandomIdle(Instance *instance, int mode)
{
    MonsterVars *mv;
    MonsterAttributes *ma;
    MonsterBehavior *behavior;
    MonsterIdle *idle;
    int chance;
    int i;
    char *idleIndex;

    mv = (MonsterVars *)instance->extraData;

    ma = (MonsterAttributes *)instance->data;

    idle = NULL;

    chance = rand() % 100;

    if ((mv->mvFlags & 0x4))
    {
        behavior = ma->behaviorList;

        idle = &ma->idleList[(int)behavior->idleList[0]];
    }
    else
    {
        if (mv->subAttr->behaviorList != NULL)
        {
            i = mv->subAttr->behaviorList[mv->behaviorState];
        }
        else
        {
            i = -1;
        }

        if (i == -1)
        {
            behavior = ma->behaviorList;
        }
        else
        {
            behavior = &ma->behaviorList[i];
        }

        if (mv->behaviorState == 0)
        {
            idle = &ma->idleList[(int)((MonsterBehavior *)((uintptr_t)behavior + mv->ambient))->idleList[0]];
        }
        else
        {
            for (i = behavior->numIdles, idleIndex = &behavior->idleList[0]; i != 0; i--, idleIndex++)
            {
                if (*idleIndex < 0)
                {
                    break;
                }

                idle = &ma->idleList[(int)*idleIndex];

                if (chance < idle->probability)
                {
                    break;
                }

                chance -= idle->probability;
            }
        }
    }

    if (idle != NULL)
    {
        MON_PlayAnimIDIfNotPlaying(instance, idle->anim, mode);

        mv->alertness = behavior->alertness;
    }
}

void MON_PlayCombatIdle(Instance *instance, int mode)
{
    int anim;

    if ((((MonsterVars *)instance->extraData)->mvFlags & 0x10))
    {
        anim = MONSTER_ANIM_STANCE_VULNERABLE;
    }
    else
    {
        anim = MONSTER_ANIM_STANCE_HEALTHY;
    }

    MON_PlayAnimIfNotPlaying(instance, (enum MonsterAnim)anim, mode);
}

void MON_GetRandomPoint(Position *out, Position *in, short r)
{

    int mult;
    int ang;

    mult = rand() % r;
    ang = rand();

    out->x = in->x + (mult * rcos(ang) >> 0xC);
    out->y = in->y + (mult * rsin(ang) >> 0xC);
    out->z = in->z;
}

int MON_GetRandomDestinationInWorld(Instance *instance, Position *in, short r)
{

    int result;
    result = 0;

    if (instance->matrix != NULL)
    {

        evPhysicsLOSData data;
        int avoidEnemy;
        MonsterVars *mv;

        mv = instance->extraData;
        avoidEnemy = 0;

        if (mv->behaviorState == MONSTER_STATE_STUNNED || mv->behaviorState == MONSTER_STATE_GRABBED)
        {
            avoidEnemy = mv->enemy != NULL;
        }

        if (in == NULL)
        {
            in = &instance->position;
        }
        else if (avoidEnemy != 0 && MATH3D_LengthXYZ(in->x - mv->enemy->instance->position.x, in->y - mv->enemy->instance->position.y, in->z - mv->enemy->instance->position.z) < mv->subAttr->fleeRange)
        {
            in = &instance->position;
        }

        data.origin.x = instance->matrix[1].t[0];
        data.origin.y = instance->matrix[1].t[1];
        data.origin.z = instance->matrix[1].t[2];

        MON_GetRandomPoint(&data.destination, in, r);

        data.destination.z += data.origin.z - instance->position.z;
        result = MON_CheckPointSuitability(instance, &data.origin, &data.destination);

        if (result != 0)
        {

            if (avoidEnemy != 0 &&
                MATH3D_LengthXYZ(data.destination.x - mv->enemy->instance->position.x, data.destination.y - mv->enemy->instance->position.y, data.destination.z - mv->enemy->instance->position.z) < mv->subAttr->fleeRange)
            {
                result = 0;
            }

            if (result != 0)
            {
                mv->mvFlags |= 0x40000;
                mv->destination = data.destination;
            }
        }
        return result;
    }
    return result;
}

void MON_MoveForward(Instance *instance)
{
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    PhysicsSetVelFromRot(instance, &instance->rotation, mv->speed);

    instance->xAccl = 0;
    instance->yAccl = 0;
    instance->zAccl = 0;

    PhysicsMove(instance, &instance->position, gameTrackerX.timeMult);
}

int MON_TurnToPosition(Instance *instance, Position *position, short turnspeed)
{
    short temp; // not from decls.h

    temp = MATH3D_AngleFromPosToPos(&instance->position, position);

    AngleMoveToward(&instance->rotation.z, temp, (short)((turnspeed * gameTrackerX.timeMult) / 4096));

    return instance->rotation.z == temp;
}

void MON_MoveToPosition(Instance *instance, Position *position, short turnSpeed)
{
    MON_TurnToPosition(instance, position, turnSpeed);

    if (MON_TransNodeAnimation(instance) == 0)
    {
        MON_MoveForward(instance);
    }
}

int MON_OnGround(Instance *instance)
{
    MonsterVars *mv;
    evPhysicsGravityData data;
    int minUpper;
    long xyDist;

    mv = (MonsterVars *)instance->extraData;

    xyDist = MATH3D_LengthXY(instance->oldPos.x - instance->position.x, instance->oldPos.y - instance->position.y);

    data.UpperOffset = instance->oldPos.z - instance->position.z;
    data.LowerOffset = (short)xyDist;

    minUpper = 400;

    if ((mv->mvFlags & 0x2))
    {
        minUpper = 100;
    }

    if (data.UpperOffset < minUpper)
    {
        data.UpperOffset = minUpper;
    }

    if ((short)xyDist < 100)
    {
        data.LowerOffset = 100;
    }

    data.UpperOffset += mv->subAttr->upOnGroundOffset;
    data.LowerOffset += mv->subAttr->downOnGroundOffset;

    data.slipSlope = 3200;

    return PhysicsCheckGravity(instance, (intptr_t)&data.UpperOffset, (!(mv->mvFlags & 0x800)) << 2);
}

void MON_ApplyPhysics(Instance *instance)
{
    PhysicsMove(instance, &instance->position, gameTrackerX.timeMult);
}

void MON_ChangeBehavior(Instance *instance, int behavior)
{

    enum MonsterState state;
    MonsterVars *mv;

    if (behavior != -1)
    {
        mv = (MonsterVars *)instance->extraData;
        switch (behavior)
        {
        case 9:
            state = MONSTER_STATE_FLEE;
            break;
        case 2:
            state = MONSTER_STATE_WANDER;
            break;
        case 4:
        case 8:
            state = MONSTER_STATE_HIDE;
            break;
        case 11:
        default:
            state = MONSTER_STATE_PURSUE;
            break;
        }
        MON_SwitchState(instance, state);
        mv->behaviorState = behavior;
    }
}

void MON_CheckEnvironment(Instance *instance)
{
    MonsterVars *mv;
    MonsterAttributes *ma;
    int inwater;
    SVector Hack;
    Position *temp; // not from decls.h

    mv = (MonsterVars *)instance->extraData;

    ma = (MonsterAttributes *)instance->data;

    if (instance->currentMainState != MONSTER_STATE_TERRAINIMPALEDEATH)
    {
        Hack.z = 0;
        Hack.y = 0;
        Hack.x = 0;

        instance->tface = NULL;
        instance->tfaceLevel = NULL;

        if (!(ma->whatAmI & 0x4))
        {
            inwater = PhysicsCheckSwim(instance, SetPhysicsSwimData(0, &Hack, 256, 160, 64), 1);
        }
        else
        {
            inwater = 0x20;
        }

        inwater &= 0x20;

        if (inwater == 0)
        {
            if (!(mv->mvFlags & 0x400))
            {
                mv->mvFlags |= 0x400;
                mv->mvFlags &= ~0x400000;

                instance->maxXVel = 20;
                instance->maxYVel = 20;
                instance->maxZVel = 20;

                instance->xVel = 0;
                instance->yVel = 0;

                do
                {

                } while (0); // garbage code for reordering

                SOUND_Play3dSound(&instance->position, 1, -200, 120, 10000);

                instance->flags2 &= ~0x40;
            }

            if ((mv->avoidMask & 0x10))
            {
                INSTANCE_Post(instance, 0x100000C, 16);
            }
        }
        else
        {
            int moveback;
            int onground;

            moveback = 0;

            if ((mv->mvFlags & 0x400))
            {
                mv->mvFlags &= ~0x400;

                instance->maxXVel = 400;
                instance->maxYVel = 400;
                instance->maxZVel = 400;

                do
                {

                } while (0); // garbage code for reordering

                if ((instance->object->oflags & 0x200))
                {
                    instance->flags2 |= 0x40;
                }
            }

            if (instance->LinkParent == NULL)
            {
                onground = MON_OnGround(instance);

                if (onground != 0)
                {
                    if (!(mv->mvFlags & 0x2))
                    {
                        mv->mvFlags |= 0x2;
                    }

                    if ((mv->mvFlags & 0x1000))
                    {
                        int offset;
                        int result;
                        Position pos;

                        offset = mv->anim->velocity * 8;

                        if (offset < 100)
                        {
                            offset = 100;
                        }

                        pos.y = -offset;
                        pos.x = 0;
                        pos.z = offset / 2;

                        if (!(mv->mvFlags & 0x10000))
                        {
                            offset = mv->subAttr->fallDistance;
                        }

                        result = PhysicsCheckDropHeight(instance, SetPhysicsDropHeightData(&pos, offset, 96), 1);

                        if (!(result & mv->avoidMask))
                        {
                            if (result == 0)
                            {
                                INSTANCE_Post(instance, 0x4010080, 0);
                            }
                        }
                        else
                        {
                            INSTANCE_Post(instance, 0x4010080, 0);
                        }
                    }

                    if ((onground & 0x100000))
                    {
                        moveback = 1;
                    }
                }
                else if ((mv->mvFlags & 0x2))
                {
                    int result;
                    int offset;

                    mv->mvFlags &= ~0x2;

                    if (!(mv->mvFlags & 0x800))
                    {
                        offset = 320;

                        if (!(mv->mvFlags & 0x10000))
                        {
                            offset = mv->subAttr->fallDistance;

                            temp = &instance->position;
                        }
                        else
                        {
                            temp = &instance->position;
                        }

                        result = PhysicsCheckDropHeight(instance, SetPhysicsDropHeightData(temp, offset, 64), 1);

                        if (((result & mv->avoidMask)) || (result == 0))
                        {
                            moveback = 1;
                        }
                    }
                }

                if (moveback != 0)
                {
                    int result;

                    result = PhysicsCheckDropHeight(instance, SetPhysicsDropHeightData(&instance->oldPos, 100, 64), 1);

                    if ((result != 0) && (!(result & mv->avoidMask)))
                    {
                        instance->position = instance->oldPos;

                        mv->mvFlags |= 0x2;
                    }
                }

                if ((!(mv->mvFlags & 0x802)) && (mv->mode != 0x100000))
                {
                    INSTANCE_Post(instance, 0x100000B, 0);
                }
            }

            if (instance->tfaceLevel != NULL)
            {
                MON_CheckTerrainAndRespond(instance, &((Level *)(instance->tfaceLevel))->terrain->BSPTreeArray[instance->bspTree], instance->tface);
            }
        }

        if ((ma->whatAmI != 0x10082) && (!(ma->whatAmI & 0x4)))
        {
            FX_UpdateInstanceWaterSplit(instance);
        }
    }
}

void MON_CheckTerrainAndRespond(Instance *instance, BSPTree *bsp, TFace *tface)
{
    unsigned long results;

    results = MON_CheckTerrain(instance, bsp, tface);

    if ((results & 0x10))
    {
        INSTANCE_Post(instance, 0x100000C, 0x10);
    }

    if ((results & 0x20))
    {
        INSTANCE_Post(instance, 0x100000C, 0x20);
    }

    if ((results & 0x40))
    {
        INSTANCE_Post(instance, 0x100000C, 0x40);
    }
}

unsigned long MON_CheckTerrain(Instance *instance, BSPTree *bsp, TFace *tface)
{
    unsigned long rv;

    rv = 0;

    if (tface != NULL)
    {
        Level *level;

        level = STREAM_GetLevelWithID(instance->currentStreamUnitID);

        if ((!(bsp->flags & 0x8)) && (((level->unitFlags & 0x2)) || ((bsp->flags & 0x50))))
        {
            rv = 0x40;
        }

        if (((bsp->flags & 0x80)) || (instance->waterFace != NULL))
        {
            rv |= 0x10;
        }

        if ((bsp->flags & 0x20))
        {
            rv |= 0x20;
        }
    }

    return rv;
}

int MON_CheckPointSuitability(Instance *instance, Position *origin, Position *destination)
{
    int result;
    int rc;
    MonsterVars *mv;
    evPhysicsDropHeightData *data;

    (void)origin;

    rc = 0;
    mv = (MonsterVars *)instance->extraData;
    data = (evPhysicsDropHeightData *)SetPhysicsDropHeightData(destination, mv->subAttr->fallDistance, 0x40);
    result = PhysicsCheckDropHeight(instance, (intptr_t)data, 1);

    if (result == 1)
    {
        result = MON_CheckTerrain(instance, data->bsp, data->tface) | 1;
        destination->z = data->origin.z;
    }

    if (!(result & mv->avoidMask))
    {
        if (mv->mvFlags & 0x800 || result != 0)
        {
            rc = 1;
        }
    }

    return rc;
}

unsigned long MON_GetTime(Instance *instance)
{
    if ((instance->object->oflags & 0x80000))
    {
        return gameTrackerX.currentTime;
    }
    else if ((instance->flags2 & 0x8000000))
    {
        return gameTrackerX.currentSpectralTime;
    }

    return gameTrackerX.currentMaterialTime;
}

void MON_BirthSoul(Instance *instance, int link)
{

    MonsterVars *mv;
    mv = instance->extraData;

    if (mv->soulJuice != 0)
    {

        long soulID;
        soulID = mv->soulID;

        if (soulID == 0 || soulID >= 0x2000)
        {

            Object *soulObj;
            soulObj = (Object *)objectAccess[9].object;

            if (soulObj != NULL)
            {

                Instance *newInstance;
                newInstance = INSTANCE_BirthObject(instance, soulObj, 0);

                if (newInstance != NULL)
                {

                    if (mv->soulID >= 0x2000)
                    {

                        newInstance->flags2 |= 0x08000000;
                        MORPH_SetupInstanceFlags(newInstance);
                    }

                    if (link != 0)
                    {
                        mv->soulID = newInstance->introUniqueID;
                    }
                    else
                    {
                        newInstance->parent = NULL;
                    }

                    if (instance->matrix != NULL)
                    {

                        MATRIX *mat;
                        mat = instance->matrix + 1;

                        newInstance->position.x = mat->t[0];
                        newInstance->position.y = mat->t[1];
                        newInstance->position.z = mat->t[2];
                    }

                    ((MonsterVars *)newInstance->extraData)->soulJuice = mv->soulJuice;
                }
            }
        }
        else
        {

            Instance *monster;
            monster = INSTANCE_Find(soulID);

            if (monster != NULL)
            {

                instance->flags2 &= ~0x80;
                mv->soulID = monster->introUniqueID;
                ((MonsterVars *)monster->extraData)->soulID = instance->introUniqueID;
            }
            else
            {
                instance->flags2 |= 0x80;
            }
        }
    }
}

void MON_ProcessIntro(Instance *instance)
{
    MonsterVars *mv;
    long initialBehavior;
    long triggeredBehavior;
    long activeBehavior;
    long guardRange;
    long wanderRange;
    long ambushRange;
    long hitPoints;
    int spectral;
    long flags;
    MonsterAttributes *ma;

    initialBehavior = -1;
    triggeredBehavior = -1;
    activeBehavior = -1;

    guardRange = 0;
    wanderRange = 0;
    ambushRange = 0;

    spectral = 0;

    ma = (MonsterAttributes *)instance->data;

    mv = (MonsterVars *)instance->extraData;

    mv->age = ma->defaultAge;

    mv->ambushArc = 2048;
    mv->ambushElevation = 1024;

    hitPoints = -1;

    mv->auxFlags |= 0x80000000;

    flags = 0;

    if (instance->introData != NULL)
    {
        INICommand *command;

        command = (INICommand *)instance->introData;

        if (!(instance->flags & 0x2))
        {
            while (command->command != 0)
            {
                switch (command->command)
                {
                case 255:
                {
                    break;
                }
                case 1:
                {
                    initialBehavior = command->parameter[0];
                    break;
                }
                case 2:
                {
                    triggeredBehavior = command->parameter[0];
                    break;
                }
                case 3:
                {
                    spectral = 1;
                    break;
                }
                case 22:
                {
                    ambushRange = command->parameter[0];
                    break;
                }
                case 25:
                {
                    mv->ambushJumpType = (char)command->parameter[0];
                    break;
                }
                case 23:
                {
                    mv->ambushArc = (short)((command->parameter[0] << 12) / 360);
                    break;
                }
                case 24:
                {
                    mv->ambushElevation = (short)((command->parameter[0] << 12) / 360);
                    break;
                }
                case 4:
                {
                    wanderRange = command->parameter[0];
                    break;
                }
                case 5:
                {
                    guardRange = command->parameter[0];
                    break;
                }
                case 6:
                {
                    mv->age = (unsigned char)command->parameter[0];
                    break;
                }
                case 7:
                {
                    flags |= 0x100000;
                    break;
                }
                case 8:
                {
                    hitPoints = command->parameter[0];
                    break;
                }
                case 9:
                {
                    instance->flags2 |= 0x20000;

                    flags |= 0x1000000;

                    if (command->numParameters != 0)
                    {
                        mv->regenTime = (short)command->parameter[0];
                    }
                    else
                    {
                        mv->regenTime = 0;
                    }

                    break;
                }
                case 10:
                {
                    short *pmarker;
                    long *param;
                    int i;

                    i = command->numParameters;

                    pmarker = mv->patrolMarkers;

                    param = command->parameter;

                    for (; i != 0; i--)
                    {
                        *pmarker = (short)*param;

                        param++;

                        pmarker++;
                    }

                    *pmarker = 0;

                    mv->currentMarker = mv->patrolMarkers;
                    break;
                }
                case 12:
                {
                    mv->ambushMarker = (char)command->parameter[0];
                    break;
                }
                case 13:
                {
                    mv->fleeMarker = (short)command->parameter[0];
                    break;
                }
                case 14:
                {
                    int i;
                    short *unit;

                    unit = mv->validUnits;

                    for (i = 0; i < command->numParameters; i++, unit++)
                    {
                        *unit = (short)command->parameter[i];
                    }

                    *unit = 0;
                    break;
                }
                case 11:
                {
                    mv->ambient = (unsigned char)command->parameter[0];
                    break;
                }
                case 15:
                {
                    mv->soulID = command->parameter[0];
                    break;
                }
                case 16:
                {
                    mv->causeOfDeath = (unsigned char)command->parameter[0];

                    if (mv->soulID == 0)
                    {
                        mv->soulID = ~0x80000000;
                    }

                    break;
                }
                case 17:
                {
                    mv->auxFlags &= ~0x80000000;
                    break;
                }
                case 20:
                {
                    activeBehavior = command->parameter[0];
                    break;
                }
                }

                command = (INICommand *)&command->parameter[command->numParameters];
            }
        }
    }

    MON_SetDefaults(instance);

    mv->mvFlags ^= flags;

    if (initialBehavior != -1)
    {
        mv->initialBehavior = (char)initialBehavior;

        mv->behaviorState = (char)initialBehavior;
    }

    if (triggeredBehavior != -1)
    {
        mv->triggeredBehavior = (char)triggeredBehavior;
    }

    if (activeBehavior != -1)
    {
        mv->activeBehavior = (char)activeBehavior;
    }

    if (hitPoints != -1)
    {
        mv->hitPoints = (short)(hitPoints << 8);
    }

    if (spectral != 0)
    {
        if ((instance->flags2 & 0x8000000))
        {
            instance->flags2 &= ~0x8000000;
        }
        else
        {
            instance->flags2 |= 0x8000000;
        }
    }

    if (guardRange != 0)
    {
        mv->guardRange = (short)guardRange;
    }

    if (wanderRange != 0)
    {
        mv->wanderRange = (short)wanderRange;
    }

    if (ambushRange != 0)
    {
        mv->ambushRange = (short)ambushRange;
    }

    if ((mv->triggeredBehavior == 8) && (mv->initialBehavior != 4) && (mv->initialBehavior != 16))
    {
        mv->initialBehavior = 4;

        mv->behaviorState = 4;
    }
}

void MON_SetDefaults(Instance *instance)
{
    MonsterVars *mv;
    MonsterAttributes *ma;
    MonsterSubAttributes *subAttr;

    mv = (MonsterVars *)instance->extraData;

    ma = (MonsterAttributes *)instance->data;

    if (mv->age >= ma->numSubAttributes)
    {
        mv->age = ma->numSubAttributes - 1;
    }

    subAttr = ma->subAttributesList[mv->age];

    mv->subAttr = subAttr;

    instance->currentModel = subAttr->modelNum;

    instance->scale.x = instance->scale.y = instance->scale.z = subAttr->scale;

    mv->behaviorState = subAttr->defInitialBehavior;

    mv->initialBehavior = subAttr->defInitialBehavior;
    mv->triggeredBehavior = subAttr->defTriggeredBehavior;

    mv->wanderRange = subAttr->defWanderRange;
    mv->guardRange = subAttr->defGuardRange;
    mv->ambushRange = subAttr->defAmbushRange;

    mv->soulJuice = 4096;

    mv->alertness = 3;

    mv->lastSideMove = 8;

    if (subAttr->combatAttributes != NULL)
    {
        mv->hitPoints = subAttr->combatAttributes->hitPoints * 256;

        if (subAttr->combatAttributes->missileAttack != -1)
        {
            mv->mvFlags |= 0x20;
        }
    }

    if ((mv->age == 2) && ((ma->whatAmI & 0x2)))
    {
        mv->mvFlags |= 0x10000000;
    }

    if (subAttr->defSpectral != 0)
    {
        instance->flags2 |= 0x8000000;
    }

    mv->avoidMask = 0x100;

    if (subAttr->waterVuln != 0)
    {
        mv->avoidMask = 0x110;
    }

    if (subAttr->fireVuln != 0)
    {
        mv->avoidMask |= 0x20;
    }

    if (subAttr->sunVuln != 0)
    {
        mv->avoidMask |= 0x40;
    }

    if (subAttr->soundVuln != 0)
    {
        mv->avoidMask |= 0x200;
    }

    if (subAttr->impaleVuln != 0)
    {
        mv->avoidMask |= 0x80;
    }
}

void MON_GetPlanSlot(MonsterVars *mv)
{
    if (mv->pathSlotID != -1)
    {
        ENMYPLAN_ReleasePlanningWorkspace(mv->pathSlotID);
    }

    mv->pathSlotID = ENMYPLAN_GetInitializedPlanningWorkspaceFinal();
}

int MON_DefaultPlanMovement(Instance *instance, int anim, long distance)
{
    MonsterVars *mv;
    int rc;
    int felloff;
    long length;
    MonsterAnim *manim;
    long dist; // not from decls.h

    dist = distance;

    mv = (MonsterVars *)instance->extraData;

    manim = MON_GetAnim(instance, mv->subAttr->animList, anim);

    length = MATH3D_LengthXYZ(instance->position.x - mv->destination.x, instance->position.y - mv->destination.y, instance->position.z - mv->destination.z);

    if ((signed char)mv->pathSlotID == -1)
    {
        return 3;
    }

    if ((!(mv->mvFlags & 0x20000)) && (MON_AnimPlaying(instance, anim) != 0))
    {
        mv->mvFlags |= 0x20000;
    }

    felloff = MON_GroundMoveQueueHandler(instance);

    rc = 6;

    if (!(mv->mvFlags & 0x1))
    {
        if (felloff == 0)
        {
            rc = 4;

            if (length >= dist)
            {
                Position pos;
                int planresult;

                planresult = ENMYPLAN_MoveToTargetFinal(instance, &pos, (signed char)mv->pathSlotID, &mv->destination, 0x1F);

                if (planresult == 3)
                {
                    rc = 2;

                    goto label_2;
                }

                if (planresult == 0)
                {
                    MON_TurnToPosition(instance, &mv->destination, mv->subAttr->speedPivotTurn);

                    rc = 1;
                }
                else
                {
                    if (!(mv->mvFlags & 0x20000))
                    {
                        mv->mvFlags |= 0x20000;

                        goto label_1;
                    }

                    rc = 0;

                    if ((instance->flags2 & 0x2))
                    {
                    label_1:
                        MON_PlayAnimIfNotPlaying(instance, anim, 2);

                        rc = 0;
                    }
                }

                if ((mv->mvFlags & 0x20000))
                {
                    short turnSpeed;

                    switch (anim)
                    {
                    case 2:
                        turnSpeed = mv->subAttr->speedWalkTurn;
                        break;
                    case 3:
                        turnSpeed = mv->subAttr->speedRunTurn;
                        break;
                    default:
                        turnSpeed = mv->subAttr->speedPivotTurn;
                    }

                    if ((length * 3) < ((manim->velocity << 12) / turnSpeed))
                    {
                        turnSpeed *= 2;
                    }

                    MON_MoveToPosition(instance, &pos, turnSpeed);
                }
            }

            return rc;
        }

        rc = 3;

        if (!(mv->mvFlags & 0x20000))
        {
            if (MON_TurnToPosition(instance, &mv->destination, mv->subAttr->speedPivotTurn) != 0)
            {
                rc = 3;
            }
            else
            {
                rc = 0;
            }
        }

        return rc;
    }

label_2:
    return rc;
}

void MON_DropAllObjects(Instance *instance)
{
    Instance *child;
    Instance *next;

    child = instance->LinkChild;

    while (child != NULL)
    {
        next = child->LinkSibling;

        do
        {

        } while (0); // garbage code for reordering

        if (child->ParentLinkNode == 3)
        {
            INSTANCE_Post(child, 0x800008, 2);
        }
        else
        {
            INSTANCE_Post(child, 0x800008, 1);
        }

        child = next;
    }
}

void MON_EnableHeadMove(Instance *instance)
{
    MonsterAttributes *ma;

    ma = (MonsterAttributes *)instance->data;

    if ((ma->neckSegment != 0) && (G2Anim_IsControllerActive(&instance->anim, ma->neckSegment, 14) == G2FALSE))
    {
        G2Anim_SetControllerAngleOrder(&instance->anim, ma->neckSegment, 14, 1);

        G2Anim_EnableController(&instance->anim, ma->neckSegment, 14);

        if (ma->spineSegment != ma->neckSegment)
        {
            G2Anim_SetControllerAngleOrder(&instance->anim, ma->spineSegment, 14, 1);

            G2Anim_EnableController(&instance->anim, ma->spineSegment, 14);
        }
    }
}

void MON_DisableHeadMove(Instance *instance)
{
    MonsterAttributes *ma;

    ma = (MonsterAttributes *)instance->data;

    if ((ma->neckSegment != 0) && (G2Anim_IsControllerActive(&instance->anim, ma->neckSegment, 14) != G2FALSE))
    {
        G2Anim_DisableController(&instance->anim, ma->neckSegment, 14);

        if ((ma->spineSegment != 0) && (ma->spineSegment != ma->neckSegment))
        {
            G2Anim_DisableController(&instance->anim, ma->spineSegment, 14);
        }
    }
}

void MON_LookInDirection(Instance *instance, short tx, short tz)
{
    G2SVector3 Rot;
    MonsterAttributes *ma;

    ma = (MonsterAttributes *)instance->data;

    if (ma->neckSegment != 0)
    {
        if (ma->spineSegment != ma->neckSegment)
        {
            Rot.y = 0;
            Rot.x = (tx * 70) / 100;
            Rot.z = (tz * 70) / 100;

            G2Anim_SetController_Vector(&instance->anim, ma->neckSegment, 14, &Rot);

            if (ma->spineSegment != 0)
            {
                Rot.y = 0;
                Rot.x = (tx * 30) / 100;
                Rot.z = (tz * 30) / 100;

                G2Anim_SetController_Vector(&instance->anim, ma->spineSegment, 14, &Rot);
            }
        }
        else
        {
            Rot.x = tx;
            Rot.y = 0;
            Rot.z = tz;

            G2Anim_SetController_Vector(&instance->anim, ma->neckSegment, 14, &Rot);
        }
    }
}

void MON_LookAtPos(Instance *instance, Position *position)
{
    MonsterVars *mv;
    short z;
    char temp; // not from decls.h

    temp = 0;

    mv = (MonsterVars *)instance->extraData;

    z = AngleDiff(instance->rotation.z, MATH3D_AngleFromPosToPos(&instance->position, position));

    if (z >= 797)
    {
        z = 796;
    }
    else if (z < -796)
    {
        z = -796;
    }

    if (z < mv->lookAngleZ)
    {
        mv->lookAngleZ -= 273;

        if (mv->lookAngleZ < z)
        {
            mv->lookAngleZ = z;
        }
    }
    else if (mv->lookAngleZ < z)
    {
        mv->lookAngleZ += 273;

        if (z < mv->lookAngleZ)
        {
            mv->lookAngleZ = z;
        }
    }

    if (z != 0) // garbage code for reordering
    {
        temp = -temp;
    }

    mv->lookAngleX = 0;

    MON_LookInDirection(instance, 0, mv->lookAngleZ);
}

void MON_ProcessLookAt(Instance *instance)
{
    MonsterVars *mv;
    int temp; // not from decls.h

    mv = (MonsterVars *)instance->extraData;

    if (mv->mode != 0x80000)
    {
        temp = mv->mvFlags;

        if (temp < 0)
        {
            MON_EnableHeadMove(instance);

            MON_LookAtPos(instance, &mv->lookAtPosData);
        }
        else if (mv->lookAtPos != NULL)
        {
            MON_EnableHeadMove(instance);

            MON_LookAtPos(instance, mv->lookAtPos);

            mv->lookAtPos = NULL;
        }
        else if ((mv->lookAngleX != 0) || (mv->lookAngleZ != 0))
        {
            AngleMoveToward(&mv->lookAngleX, 0, 100);
            AngleMoveToward(&mv->lookAngleZ, 0, 100);

            MON_LookInDirection(instance, mv->lookAngleX, mv->lookAngleZ);
        }
        else
        {
            MON_DisableHeadMove(instance);
        }
    }
}

int MON_TakeDamage(Instance *instance, int damage, int type)
{

    MonsterVars *mv;
    MonsterCombatAttributes *mca;

    mv = instance->extraData;
    mca = mv->subAttr->combatAttributes;

    if (mca != NULL)
    {

        if ((signed char)mca->hitPoints != 0)
        {

            uintptr_t enemyInstance;
            enemyInstance = INSTANCE_Query(instance, 1);

            if (type != 0x40000 || enemyInstance & 8)
            {

                mv->hitPoints -= damage;
                mv->damageType = type;

                if (mv->hitPoints <= 0)
                {
                    mv->hitPoints = 0;
                    return 1;
                }
            }
        }
    }

    return 0;
}

void MON_SetUpSaveInfo(Instance *instance, MonsterSaveInfo *saveData)
{
    MonsterVars *mv;
    MonsterAttributes *ma;

    mv = (MonsterVars *)instance->extraData;

    ma = (MonsterAttributes *)instance->data;

    saveData->mvFlags = mv->mvFlags & ~0x440004;
    saveData->auxFlags = mv->auxFlags & ~0x8000000;

    saveData->age = mv->age;

    saveData->state = instance->currentMainState;

    saveData->behaviorState = mv->behaviorState;

    saveData->causeOfDeath = mv->causeOfDeath;

    if ((instance->currentMainState == MONSTER_STATE_DEAD) && (mv->causeOfDeath == 0))
    {
        saveData->soulJuice = mv->heldID;
    }
    else
    {
        saveData->soulJuice = mv->soulJuice;
    }

    saveData->soulID = mv->soulID;

    if (mv->anim != NULL)
    {
        saveData->animLooping = instance->anim.section->flags >> 1;

        saveData->anim = mv->anim - ma->animList;
    }
    else
    {
        saveData->anim = ma->numAnims;
    }
}

void MON_GetSaveInfo(Instance *instance, MonsterSaveInfo *saveData)
{
    MonsterVars *mv;
    MonsterAttributes *ma;

    mv = (MonsterVars *)instance->extraData;

    ma = (MonsterAttributes *)instance->data;

    do
    {

    } while (0); // garbage code for reordering

    mv->age = saveData->age;

    mv->subAttr = ma->subAttributesList[mv->age];

    instance->currentModel = mv->subAttr->modelNum;

    if (!(saveData->mvFlags & 0x8000))
    {
        MON_TurnOffBodySpheres(instance);
    }
    else
    {
        MON_TurnOnBodySpheres(instance);
    }

    mv->mvFlags = saveData->mvFlags | (mv->mvFlags & 0xC000);
    mv->auxFlags = saveData->auxFlags | (mv->auxFlags & 0x8000000);

    mv->behaviorState = saveData->behaviorState;

    mv->causeOfDeath = saveData->causeOfDeath;

    mv->soulID = saveData->soulID;

    mv->soulJuice = saveData->soulJuice;

    switch (saveData->state)
    {
    case 16:
        instance->flags2 &= ~0x40;

        mv->soulID = ~0x80000000;
    case 23:
        if (mv->causeOfDeath == 0)
        {
            mv->heldID = mv->soulJuice;

            mv->soulJuice = 4096;

            instance->flags2 |= 0x80;
        }

        instance->currentMainState = MONSTER_STATE_DEAD;
        break;
    case 10:
    case 11:
    case 12:
    case 14:
        instance->currentMainState = MONSTER_STATE_IDLE;
        break;
    case 6:
    case 8:
    case 9:
    case 21:
    case 28:
        instance->currentMainState = MONSTER_STATE_COMBAT;
        break;
    default:
        instance->currentMainState = saveData->state;
    }

    {
        int mode;

        mode = 1;

        instance->position = instance->oldPos;

        if (saveData->anim < ma->numAnims)
        {
            if (saveData->animLooping)
            {
                mode = 2;
            }

            if (instance->currentMainState == MONSTER_STATE_DEAD)
            {
                MON_PlayAnim(instance, MONSTER_ANIM_GENERALDEATH, 1);
            }
            else
            {
                MON_PlayAnimID(instance, saveData->anim, mode);
            }
        }
    }
}

void MON_KillMonster(Instance *instance)
{

    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;
    MON_DropAllObjects(instance);

    if (mv->causeOfDeath == 6)
    {

        MonsterAttributes *attr;
        FXSplinter *splintDef;

        attr = (MonsterAttributes *)instance->data;
        splintDef = attr->shatterList;

        _FX_BuildSplinters(instance, 0, 0, 0, splintDef, gFXT, NULL, NULL, 8);

        if (!(instance->flags2 & 0x1000))
        {
            SOUND_Play3dSound(&instance->position, 0x30, 0, 0x5F, 0x3E80);
        }
    }

    if (mv->mvFlags & 0x01000000 && (instance->intro == NULL || !(instance->intro->flags & 0x400)))
    {
        if (mv->regenTime != 0)
        {
            MONAPI_AddToGenerator(instance);
        }
        SAVE_DeleteInstance(instance);
    }
    else
    {
        SAVE_MarkDeadDead(instance);
    }

    if (instance->object->oflags2 & 4)
    {
        if (instance->flags2 & 0x1000)
        {
            SOUND_ProcessInstanceSounds(instance->object->soundData, instance->soundInstanceTbl, &instance->position, instance->object->oflags2 & 0x02000000, instance->flags2 & 0x08000000, 0, 0, &instance->flags2);
            SOUND_ProcessInstanceSounds(instance->object->soundData, instance->soundInstanceTbl, &instance->position, instance->object->oflags2 & 0x02000000, instance->flags2 & 0x08000000, 0, 0, &instance->flags2);
        }
    }
    instance->flags |= 0x20;
}

int MON_ShouldIAmbushEnemy(Instance *instance)
{

    MonsterVars *mv;
    MonsterIR *enemy;

    mv = instance->extraData;
    enemy = mv->enemy;


    if (instance->currentStreamUnitID == instance->birthStreamUnitID && (signed char)mv->ambushMarker != 0 && mv->ambushArc == 0x800 && mv->ambushElevation == 0x400)
    {

        Position pos;
        Instance *inst;

        if (enemy != NULL)
        {
            inst = enemy->instance;
        }
        else
        {
            inst = gameTrackerX.playerInstance;
        }

        if (PLANAPI_FindNodePositionInUnit(STREAM_GetStreamUnitWithID(instance->birthStreamUnitID), &pos, (signed char)mv->ambushMarker, 4) != 0 &&
            MATH3D_LengthXYZ(inst->position.x - pos.x, inst->position.y - pos.y, inst->position.z - pos.z) < mv->ambushRange)
        {
            return 1;
        }
    }

    if (enemy != NULL)
    {
        if (instance->currentMainState != MONSTER_STATE_PUPATE || enemy->mirFlags & 0x40)
        {
            if (mv->ambushArc == 0x800 && mv->ambushElevation == 0x400)
            {
                return enemy->distance < mv->ambushRange;
            }
            if (enemy->distance < mv->ambushRange)
            {
                return MATH3D_ConeDetect(&enemy->relativePosition, mv->ambushArc, mv->ambushElevation);
            }
        }
    }
    return 0;
}

int MON_ShouldIFireAtTarget(Instance *instance, MonsterIR *target)
{

    long distance;
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    if (mv->mvFlags & 0x20 && target->mirFlags & 0x20)
    {

        MonsterMissile *missileAttack;
        MonsterAttributes *ma = (MonsterAttributes *)instance->data;
        missileAttack = &ma->missileList[(signed char)mv->subAttr->combatAttributes->missileAttack];

        if ((signed char)mv->chance < (signed char)missileAttack->fireChance)
        {
            distance = target->distance;
            if (distance < (int)missileAttack->range)
            {

                MonsterIR *known;
                known = mv->monsterIRList;

                while (known != NULL)
                {
                    if (known->distance < distance && known->relativePosition.y > 0 && known->relativePosition.x < 0xC8)
                    {
                        return 0;
                    }
                    known = known->next;
                }
                return 2;
            }
        }
    }
    return 0;
}


int MON_ShouldIFlee(Instance *instance)
{
    MonsterVars *mv;
    MonsterIR *enemy;

    mv = (MonsterVars *)instance->extraData;

    enemy = mv->enemy;

    if ((enemy != NULL) && (!(enemy->mirFlags & 0x8)) && (enemy->distance < mv->subAttr->fleeRange))
    {
        if (MON_ValidPosition(instance) != 0)
        {
            if ((mv->behaviorState == 9) || (((mv->mvFlags & 0x2000000)) && ((mv->ally == NULL) || (!(mv->ally->mirFlags & 0x200))) && (mv->hitPoints < 8192)))
            {
                return 1;
            }

            if (((mv->mvFlags & 0x2000)) && (mv->hitPoints == 0))
            {
                return 1;
            }
        }
    }

    return 0;
}

void MON_RelocateCoords(Instance *instance, SVector *offset)
{
    MonsterVars *mv;
    int ox;
    int oy;
    int oz;

    mv = (MonsterVars *)instance->extraData;

    ox = offset->x;
    oy = offset->y;
    oz = offset->z;

    mv->destination.x += ox;
    mv->destination.y += oy;
    mv->destination.z += oz;

    mv->lastValidPos.x += ox;
    mv->lastValidPos.y += oy;
    mv->lastValidPos.z += oz;

    if (mv->pathSlotID != -1)
    {
        ENMYPLAN_RelocatePlanPositions(mv->pathSlotID, (Position *)offset);
    }
}

int MON_ValidUnit(Instance *instance, unsigned long unitId)
{
    MonsterVars *mv;
    short *unit;

    mv = (MonsterVars *)instance->extraData;

    unit = (short *)&mv->validUnits;

    if (*unit == 0)
    {
        return 1;
    }

    while (*unit != 0)
    {
        if ((unsigned short)unitId == (unsigned short)*unit++)
        {
            return 1;
        }
    }

    return 0;
}

int MON_ValidPosition(Instance *instance)
{
    MonsterVars *mv;

    if (MON_ValidUnit(instance, instance->currentStreamUnitID) != 0)
    {
        mv = (MonsterVars *)instance->extraData;

        mv->lastValidPos = instance->position;

        return 1;
    }

    return 0;
}

void MON_SphereWorldPos(MATRIX *mat, HSphere *sphere, Position *ret)
{
    ApplyMatrixSV(mat, (SVECTOR *)&sphere->position, (SVECTOR *)ret);
    ret->x += mat->t[0];
    ret->y += mat->t[1];
    ret->z += mat->t[2];
}

HPrim *MON_FindSphereForTerrain(Instance *instance)
{

    int maxRad;
    HPrim *usePrim;

    usePrim = NULL;
    maxRad = 0;

    if (instance->hModelList != NULL)
    {
        int i;
        HModel *hmodel;
        HPrim *currentP;

        hmodel = &instance->hModelList[instance->currentModel];
        currentP = hmodel->hPrimList;

        for (i = hmodel->numHPrims; i != 0; i--, currentP++)
        {
            if (currentP->withFlags & 0x2 && currentP->type == 1)
            {

                HSphere *tempS;
                tempS = currentP->data.hsphere;

                if (maxRad < tempS->radius)
                {
                    usePrim = currentP;
                    maxRad = tempS->radius;
                }
            }
        }
    }
    return usePrim;
}

Intro *MON_FindNearestImpalingIntro(int unitID, Position *position)
{

    int dist;
    int i;
    int min_dist;
    Level *level;
    Intro *res;
    Intro *current;

    level = STREAM_GetLevelWithID(unitID);
    res = NULL;

    if (level != NULL)
    {

        i = level->numIntros;

        if (i > 0)
        {

            min_dist = 0x7FFFFFFF;
            current = level->introList;

            for (; i > 0; i--, current++)
            {
                if (current->flags & 0x8000)
                {
                    res = current;
                    min_dist = MATH3D_LengthXYZ(res->position.x - position->x, res->position.y - position->y, res->position.z - position->z);
                    current++;
                    i--;
                    break;
                }
            }

            for (; i > 0; i--, current++)
            {
                if (current->flags & 0x8000)
                {
                    dist = MATH3D_LengthXYZ(current->position.x - position->x, current->position.y - position->y, current->position.z - position->z);
                    if (dist < min_dist)
                    {
                        res = current;
                        min_dist = dist;
                    }
                }
            }
        }
    }
    return res;
}

Intro *MON_TestForTerrainImpale(Instance *instance, Terrain *terrain)
{

    MonsterVars *mv;
    mv = (MonsterVars *)instance->extraData;

    if (mv->mode == 0x100000)
    {

        HPrim *prim;
        prim = MON_FindSphereForTerrain(instance);

        if (prim != NULL)
        {

            Position spherePos;
            Intro *current;
            HSphere *sphere;
            int i;
            int radius;

            sphere = prim->data.hsphere;
            radius = sphere->radius * 3;
            MON_SphereWorldPos(&instance->matrix[prim->segment], sphere, &spherePos);
            current = terrain->introList;

            for (i = terrain->numIntros; i != 0; i--, current++)
            {
                if (current->flags & 0x8000)
                {
                    if (MATH3D_LengthXYZ(spherePos.x - current->position.x, spherePos.y - current->position.y, spherePos.z - current->position.z) < radius)
                    {
                        return current;
                    }
                }
            }
        }
    }
    return 0;
}

void MON_MoveInstanceToImpalePoint(Instance *instance)
{

    Position offset;
    Intro *impaler;
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    if (instance->matrix == NULL) { return; }

    impaler = INSTANCE_FindIntro(instance->currentStreamUnitID, mv->terrainImpaleID);

    if (impaler == NULL) { return; }

    offset.x = impaler->position.x - instance->matrix[3].t[0];
    offset.y = impaler->position.y - instance->matrix[3].t[1];
    offset.z = impaler->position.z - instance->matrix[3].t[2];

    ADD_SVEC(Position, &instance->position, Position, &instance->position, Position, &offset);
    COPY_SVEC(Rotation, &instance->rotation, Rotation, &impaler->rotation);

    COLLIDE_UpdateAllTransforms(instance, (SVECTOR *)&offset);

}

INCLUDE_ASM("asm/nonmatchings/Game/MONSTER/MONLIB", MON_ReachableIntro);

int MON_SetVelocityTowardsImpalingObject(Instance *instance, int checkOrientation)
{
    Level *level;
    Intro *currentI;
    HPrim *usePrim;
    int i;
    Position spherePos;

    level = STREAM_GetLevelWithID(instance->currentStreamUnitID);

    if (!(((Dummy6 *)instance->data)->unknown & 0x8)) // TODO: find the suitable parsing here, could be instanceID
    {
        usePrim = MON_FindSphereForTerrain(instance);

        if (usePrim != NULL)
        {
            Instance *target;

            MON_SphereWorldPos(&instance->matrix[usePrim->segment], usePrim->data.hsphere, &spherePos);

            currentI = level->terrain->introList;

            for (i = level->terrain->numIntros; i != 0; i--, currentI++)
            {
                if (((currentI->flags & 0x8000)) && (MON_ReachableIntro(instance, &spherePos, &currentI->position, &currentI->rotation, checkOrientation) != 0))
                {
                    return 1;
                }
            }

            for (target = gameTrackerX.instanceList->first; target != NULL; target = target->next)
            {
                if ((INSTANCE_Query(target, 1) & 0x20))
                {
                    PhysObProperties *prop;

                    prop = target->data;

                    if ((prop->family == 3) && ((((PhysObInteractProperties *)prop)->conditions & 0x58)) && (MON_ReachableIntro(instance, &spherePos, &target->position, NULL, checkOrientation) != 0))
                    {
                        return 1;
                    }
                }
            }
        }
    }

    return 0;
}

void MON_TurnOffSphereCollisions(Instance *instance)
{

    int i;
    HModel *hmodel;

    hmodel = &instance->hModelList[instance->currentModel];
    i = hmodel->numHPrims;

    if (i != 0)
    {

        HPrim *hprim;
        hprim = hmodel->hPrimList;

        if (hprim->withFlags & 0x20)
        {
            for (; i != 0; i--, hprim++)
            {
                hprim->withFlags &= 0xDF;
            }
        }
    }
}

void MON_TurnOnSphereCollisions(Instance *instance)
{

    int i;
    HModel *hModel;

    hModel = &instance->hModelList[instance->currentModel];
    i = hModel->numHPrims;

    if (i != 0)
    {

        HPrim *hprim;
        hprim = hModel->hPrimList;

        if (!(hprim->withFlags & 0x20))
        {
            for (; i != 0; i--, hprim++)
            {
                hprim->withFlags |= 0x20;
            }
        }
    }
}

void MON_ProcessSpecialFade(Instance *instance)
{
    MonsterVars *mv;
    int currentFadeValue;
    int targetFadeValue;
    int fadeRate;
    Instance *child;
    int temp; // not from decls.h

    mv = (MonsterVars *)instance->extraData;

    if ((instance->object->oflags & 0x80000))
    {
        if ((instance->currentMainState != MONSTER_STATE_DEAD) && (instance->currentMainState != MONSTER_STATE_GENERALDEATH))
        {
            if ((*(int *)&gameTrackerX.gameData.asmData.MorphTime == 1000) || ((gameTrackerX.gameData.asmData.MorphType == 1) && (gameTrackerX.gameData.asmData.MorphTime != 1000))) // double-check
            {
                targetFadeValue = mv->targetFade;

                currentFadeValue = 4096 - targetFadeValue;
            }
            else
            {
                currentFadeValue = mv->targetFade;

                targetFadeValue = 4096 - currentFadeValue;
            }

            if (gameTrackerX.gameData.asmData.MorphTime < 333)
            {
                instance->fadeValue = currentFadeValue + (((4096 - currentFadeValue) * gameTrackerX.gameData.asmData.MorphTime) / 333);
            }
            else if (gameTrackerX.gameData.asmData.MorphTime > 666)
            {
                instance->fadeValue = (((targetFadeValue - 4096) * (gameTrackerX.gameData.asmData.MorphTime - 666)) / 333) + 4096;
            }
            else
            {
                instance->fadeValue = 4096;
            }

            if (instance->fadeValue < 0)
            {
                instance->fadeValue = 0;
            }
            else if (instance->fadeValue > 4096)
            {
                instance->fadeValue = 4096;
            }

            if (instance->fadeValue == 4096)
            {
                MON_TurnOffSphereCollisions(instance);

                instance->flags |= 0x800;
            }
            else
            {
                MON_TurnOnSphereCollisions(instance);

                instance->flags &= ~0x800;
            }
        }
    }
    else if ((mv->auxFlags & 0x8000000))
    {
        fadeRate = mv->fadeRate;

        targetFadeValue = mv->targetFade;

        temp = fadeRate * gameTrackerX.timeMult;

        currentFadeValue = instance->fadeValue + (temp / 4096);

        if (fadeRate > 0)
        {
            if (targetFadeValue < currentFadeValue)
            {
                currentFadeValue = targetFadeValue;

                mv->auxFlags &= ~0x8000000;
            }
        }
        else if (currentFadeValue < targetFadeValue)
        {
            currentFadeValue = targetFadeValue;

            mv->auxFlags &= ~0x8000000;
        }

        instance->fadeValue = currentFadeValue;
    }

    for (child = instance->LinkChild; child != NULL; child = child->LinkSibling)
    {
        child->fadeValue = instance->fadeValue;
    }
}

void MON_StartSpecialFade(Instance *instance, int fadeLevel, int fadeTime)
{
    MonsterVars *mv;
    int diff;

    mv = (MonsterVars *)instance->extraData;

    mv->targetFade = fadeLevel;

    mv->auxFlags |= 0x8000000;

    diff = fadeLevel - instance->fadeValue;

    if (fadeTime <= 0)
    {
        fadeTime = 1;
    }

    mv->fadeRate = diff / fadeTime;

    if (((diff / fadeTime) << 16) == 0)
    {
        if (diff < 0)
        {
            mv->fadeRate = -1;
        }
        else
        {
            mv->fadeRate = 1;
        }
    }
}

void MON_UnlinkFromRaziel(Instance *instance)
{
    MonsterVars *mv;
    evPositionData *data;
    Instance *enemy;

    mv = (MonsterVars *)instance->extraData;

    enemy = mv->enemy->instance;

    INSTANCE_UnlinkFromParent(instance);

    MON_SwitchState(instance, MONSTER_STATE_FALL);

    data = (evPositionData *)INSTANCE_Query(enemy, 7);

    if (data != NULL)
    {
        instance->rotation.x = data->x;
        instance->rotation.y = data->y;
        instance->rotation.z = data->z + 2048;
    }

    INSTANCE_Post(enemy, 0x1000006, (intptr_t)instance);
}

void MON_BurnInAir(Instance *instance, int currentState)
{

    MonsterVars *mv;
    mv = (MonsterVars *)instance->extraData;

    if (!(mv->mvFlags & 0x400000))
    {
        mv->mvFlags |= 0x400000;
        mv->effectTimer = MON_GetTime(instance) + 0x2710;
        MON_MonsterGlow(instance, 0x4960, -1, 0, 0);
        INSTANCE_Post(instance, 0x400000, SetFXHitData(NULL, 0, 0, 0x20));
    }

    instance->currentMainState = currentState;
    mv->mvFlags &= ~1;
    instance->checkMask &= ~0x20;
}

void MON_BirthMana(Instance *instance)
{
    MonsterVars *mv;

    if (instance->matrix != NULL)
    {
        mv = (MonsterVars *)instance->extraData;

        if (mv->chance < 25)
        {
            int type;
            MATRIX *mat;

            type = ((rand() % 100) < 80) == 0;

            mat = &instance->matrix[1];

            PHYSOB_BirthCollectible(instance, mat->t[0], mat->t[1], mat->t[2], type, 10);
        }
    }
}

void MON_SoulSucked(Instance *instance)
{
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    if ((mv->mvFlags & 0x200))
    {
        if (!(((MonsterAttributes *)instance->data)->whatAmI & 0x8))
        {
            mv->damageTimer = MON_GetTime(instance);
        }

        mv->soulID = 0;

        MON_BirthMana(instance);
    }
}

int MON_SetUpKnockBack(Instance *instance, Instance *enemy, evMonsterHitData *data)
{
    MonsterVars *mv;
    long d;
    long t;
    long a;
    long v;
    int front;
    short temp; // not from decls.h

    mv = (MonsterVars *)instance->extraData;

    d = data->knockBackDistance;
    t = data->knockBackDuration;

    a = PHYSICS_FindAFromDAndT(d, t);
    v = PHYSICS_FindVFromAAndD(a, d);

    temp = ((instance->rotation.z - MATH3D_AngleFromPosToPos(&instance->position, &enemy->position)) + 1024) & 0xFFF;

    if (temp <= 2047)
    {
        MON_TurnToPosition(instance, &enemy->position, 4096);

        PHYSICS_SetVAndAFromRot(instance, &instance->rotation, -v, a);

        front = 1;
    }
    else
    {
        PHYSICS_SetVAndAFromRot(instance, &instance->rotation, v, -a);

        front = 0;
    }

    mv->generalTimer = MON_GetTime(instance) + (t * 33);

    return front;
}

void MON_DoDrainEffects(Instance *instance, Instance *ei)
{
    MonsterAttributes *ma = (MonsterAttributes *)instance->data;
    MonsterVars *mv = (MonsterVars *)instance->extraData;
    MATRIX *mat;
    SVector location;
    SVector position;
    SVector vel;
    SVector accel = {0};

    if (ei == gameTrackerX.playerInstance)
    {
        mat = &ei->matrix[(rand() % (ei->object->modelList[ei->currentModel]->numSegments - 16)) + 14];
    }
    else
    {
        mat = &ei->matrix[rand() % ei->object->modelList[ei->currentModel]->numSegments];
    }

    location.x = mat->t[0];
    location.y = mat->t[1];
    location.z = mat->t[2];

    mat = &instance->matrix[ma->headSegment];

    vel.x = (mat->t[0] - location.x) / 48;
    vel.y = (mat->t[1] - location.y) / 48;
    vel.z = (mat->t[2] - location.z) / 48;

    position.x = mat->t[0];
    position.y = mat->t[1];
    position.z = mat->t[2];

    FX_Dot(&location, &vel, &accel, 0, 0xFF6060, 24, 20, 0);

    if (ei == gameTrackerX.playerInstance)
    {
        mat = &ei->matrix[14];

        location.x = mat->t[0];
        location.y = mat->t[1];
        location.z = mat->t[2];

        mv->effectTimer = (mv->effectTimer + ((gameTrackerX.timeMult * 80) / 4096)) & 0xFFF;

        FX_Lightning(theCamera.core.wcTransform, gameTrackerX.drawOT, NULL, mv->effectTimer, &position, &location, 0, 96, 24, 80, 1, 0xD06060, 0xFF6400);
    }
}

void MON_SetFXHitData(Instance *instance, evFXHitData *data, int type, int amount)
{
    data->type = type;

    data->amount = amount;

    if (instance->matrix != NULL)
    {
        MATRIX *mat;

        mat = &instance->matrix[((MonsterAttributes *)instance->data)->spineSegment];

        data->location.x = mat->t[0];
        data->location.y = mat->t[1];
        data->location.z = mat->t[2];
    }
    else
    {
        data->location.x = instance->position.x;
        data->location.y = instance->position.y;
        data->location.z = instance->position.z + 256;
    }

    data->velocity.x = 0;
    data->velocity.y = 0;
    data->velocity.z = 0;
}

void MON_LaunchMonster(Instance *instance, int zDirection, int power, int loft)
{
    instance->rotation.z = zDirection + 2048;
    instance->rotation.x = 0;
    instance->rotation.y = 0;

    instance->xAccl = 0;
    instance->yAccl = 0;
    instance->zAccl = -8;

    if (MON_SetVelocityTowardsImpalingObject(instance, 1) == 0)
    {
        PhysicsSetVelFromZRot(instance, zDirection, power);

        instance->zVel = loft;
    }

    MON_SwitchState(instance, MONSTER_STATE_THROWN);
}
