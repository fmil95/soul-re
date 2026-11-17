#include "Overlays/walboss/walboss.h"
#include "Game/DEBUG.h"
#include "Game/FX.h"
#include "Game/GAMELOOP.h"
#include "Game/G2/ANMCTRLR.h"
#include "Game/G2/ANMG2ILF.h"
#include "Game/INSTANCE.h"
#include "Game/MATH3D.h"
#include "Game/MEMPACK.h"
#include "Game/OBTABLE.h"
#include "Game/SAVEINFO.h"
#include "Game/SOUND.h"
#include "Game/STATE.h"
#include "Game/MONSTER/MONAPI.h"
#include "Game/MONSTER/MONLIB.h"
#include "Game/MONSTER/MONMSG.h"
#include "Game/MONSTER/MONSTER.h"
#include "Game/COLLIDE.h"
#include "Game/PHYSICS.h"
#include "Game/STREAM.h"
#include "Game/CAMERA.h"
#include "Game/PLAN/ENMYPLAN.h"
#include "Game/PLAN/PLANAPI.h"
#include "Game/MONSTER/MONSENSE.h"
#include "Game/MONSTER/MBMISS.h"
#include "Game/RAZIEL/RAZIEL.h"
#include "Game/PSX/SUPPORT.h"

MonsterStateChoice WALBOSS_StateChoiceTable[] = 
{
    {MONSTER_STATE_IDLE, {WALBOSS_IdleEntry, WALBOSS_Idle}},
    {MONSTER_STATE_COMBAT, {WALBOSS_CombatEntry, WALBOSS_Combat}},
    {MONSTER_STATE_ATTACK, {WALBOSS_AttackEntry, WALBOSS_Attack}},
    {MONSTER_STATE_NOTICE, {WALBOSS_NoticeEntry, WALBOSS_Notice}},
    {MONSTER_STATE_HIT, {WALBOSS_HitEntry, WALBOSS_Hit}},
    {MONSTER_STATE_MISSILEHIT, {WALBOSS_HitEntry, WALBOSS_Hit}},
    {MONSTER_STATE_STUNNED, {WALBOSS_HitEntry, WALBOSS_Hit}},
    {MONSTER_STATE_PARRY, {WALBOSS_DoNothingEntry, WALBOSS_DoNothing}},
    {MONSTER_STATE_FALL, {WALBOSS_DoNothingEntry, WALBOSS_DoNothing}},
    {MONSTER_STATE_SURPRISED, {WALBOSS_DoNothingEntry, WALBOSS_DoNothing}},
    {MONSTER_STATE_DEAD, {WALBOSS_DeadEntry, WALBOSS_Dead}},
    {-1, {NULL, NULL}},
};

const MonsterFunctionTable WALBOSS_FunctionTable = 
{
    WALBOSS_Init,
    WALBOSS_CleanUp,
    WALBOSS_DamageEffect,
    WALBOSS_Query,
    WALBOSS_Message,
    WALBOSS_StateChoiceTable,
    monVersion,
    "Jul 14 1999"
};

void FX_StartInstanceBurrow(Instance *instance, Level *level, TFace *tface)
{

    SVector normal;
    SVector translation; // not from debug symbols
    Vertex *vertex; // not from debug symbols
    Position *offset; // not from debug symbols

    COLLIDE_GetNormal(tface->normal, (short *)level->terrain->normalList, &normal);

    instance->halvePlane.a = normal.x;
    instance->halvePlane.b = normal.y;
    instance->halvePlane.c = normal.z;

    vertex = &level->terrain->vertexList[tface->face.v0].vertex;
    offset = &level->terrain->BSPTreeArray[instance->bspTree].globalOffset;

    translation.x = vertex->x + offset->x;
    translation.y = vertex->y + offset->y;
    translation.z = vertex->z + offset->z;

    instance->halvePlane.d = -(((instance->halvePlane.a * translation.x) + (instance->halvePlane.b * translation.y) + (instance->halvePlane.c * translation.z)) >> 12);
    instance->halvePlane.flags = 1;
}

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_SetSegmentYaw);
#else
void WALBOSS_SetSegmentYaw(Instance *instance, int yaw)
{
    G2SVector3 extraRot;
    WalbossVars *vars; // not from debug symbols
    WalbossAttributes *attrs; // not from debug symbols

    vars = (WalbossVars *)((MonsterVars *)instance->extraData)->extraVars;
    attrs = (WalbossAttributes *)((MonsterAttributes *)instance->data)->tunData;

    if ((vars != NULL) && (attrs != NULL))
    {
        yaw &= 0xFFF;

        vars->lastDirection = yaw;

        extraRot.y = 0;
        extraRot.x = 0;

        if (yaw < 2049)
        {
            extraRot.z = ((yaw * 600) >> 10) & 0xFFF;
        }
        else
        {
            extraRot.z = (((yaw - 4096) * 600) >> 10) & 0xFFF;
        }

        G2Anim_SetController_Vector(&instance->anim, attrs->segmentForBodyTwist, 14, &extraRot);
    }
}
#endif

void WALBOSS_InterpSegmentYaw(Instance *instance, int yaw)
{
    int newAngle;
    MonsterVars *mv; // not from debug symbols
    WalbossVars *vars; // not from debug symbols
    WalbossAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (WalbossVars *)mv->extraVars;
    attrs = (WalbossAttributes *)((MonsterAttributes *)instance->data)->tunData;

    if ((vars != NULL) && (attrs != NULL))
    {
        yaw &= 0xFFF;

        if (yaw > 2048)
        {
            yaw -= 4096;
        }

        if (yaw > attrs->maxAngle)
        {
            yaw = attrs->maxAngle;
        }
        else if (yaw < -attrs->maxAngle)
        {
            yaw = -attrs->maxAngle;
        }

        newAngle = vars->lastDirection;

        AngleMoveToward((short *)&newAngle, yaw, (gameTrackerX.timeMult * mv->subAttr->speedPivotTurn) >> 12);

        newAngle &= 0xFFF;

        WALBOSS_SetSegmentYaw(instance, newAngle);
    }
}

void WALBOSS_TurnToPosition(Instance *instance, Position *target)
{
    WALBOSS_InterpSegmentYaw(instance, (MATH3D_AngleFromPosToPos(&instance->position, target) - instance->rotation.z) & 0xFFF);
}

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_ChooseAttack);
#else
int WALBOSS_ChooseAttack(Instance *instance, MonsterIR *enemy)
{
    int shortestDist; // not from debug symbols
    short attackIndex; // not from debug symbols
    short i; // not from debug symbols
    short numAttacks; // not from debug symbols
    int distance; // not from debug symbols
    int angle; // not from debug symbols
    int attackDist; // not from debug symbols
    char *attackListPtr; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    WalbossVars *vars; // not from debug symbols
    WalbossAttributes *attrs; // not from debug symbols
    MonsterCombatAttributes *combat; // not from debug symbols

    shortestDist = 99999;

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (WalbossVars *)mv->extraVars;
    attrs = (WalbossAttributes *)ma->tunData;
    combat = mv->subAttr->combatAttributes;

    attackIndex = -1;

    if ((attrs != NULL) && (vars != NULL))
    {
        distance = enemy->distance;

        if (((((vars->lastDirection & 0xFFF) > 2048) ? ((vars->lastDirection & 0xFFF) - 4096) : (vars->lastDirection & 0xFFF))) >= 0)
        {
            angle = ((vars->lastDirection & 0xFFF) > 2048) ? ((vars->lastDirection & 0xFFF) - 4096) : (vars->lastDirection & 0xFFF);
        }
        else
        {
            angle = -(((vars->lastDirection & 0xFFF) > 2048) ? ((vars->lastDirection & 0xFFF) - 4096) : (vars->lastDirection & 0xFFF));
        }

        i = 0;

        if (angle > attrs->angleForShortAttack)
        {
            i = attrs->closeAttackStart;

            numAttacks = combat->numAttacks;

            attackListPtr = &combat->attackList[i];
        }
        else
        {
            numAttacks = attrs->closeAttackStart;

            attackListPtr = combat->attackList;
        }

        for (; i < numAttacks; i++, attackListPtr++)
        {
            if (mv->hitPoints >= attrs->attackDeltas[i].validAtHitPoint)
            {
                attackDist = ((ma->attackAttributesList[(signed char)*attackListPtr].attackRange * mv->subAttr->scale) / 4096) - distance;

                if ((abs(attackDist) < abs(shortestDist)) || ((abs(attackDist) == abs(shortestDist)) && (rand() & 1)))
                {
                    attackIndex = i;

                    shortestDist = attackDist;
                }
            }
        }

        mv->attackState = 0;
    }

    return attackIndex;
}
#endif

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_ShouldIAttack);
#else
int WALBOSS_ShouldIAttack(Instance *instance, MonsterIR *enemy, int attack)
{
    MonsterAttributes *ma; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    WalbossAttributes *attrs; // not from debug symbols
    MonsterAttackAttributes *attackAttrs; // not from debug symbols
    Instance *attackee; // not from debug symbols
    long mode; // not from debug symbols
    int delta; // not from debug symbols
    int result; // not from debug symbols

    ma = (MonsterAttributes *)instance->data;
    mv = (MonsterVars *)instance->extraData;
    attrs = (WalbossAttributes *)ma->tunData;

    attackee = (Instance *)INSTANCE_Query(enemy->instance, queryPlayerAutoFaceInstance);
    mode = INSTANCE_Query(enemy->instance, queryMode);

    if (attrs == NULL)
    {
        result = 0;
    }
    else if ((attackee != NULL) && (attackee != instance) && (mode & 0x2000000))
    {
        result = 0;
    }
    else if ((attack == -1) || (MON_CheckConditions(instance, enemy, (attackAttrs = &ma->attackAttributesList[(signed char)mv->subAttr->combatAttributes->attackList[attack]])->attackProbability) == 0))
    {
        result = 0;
    }
    else
    {
        delta = enemy->distance - attackAttrs->attackRange;

        mv->attackType = attackAttrs;

        if (attrs->attackDeltas[attack].plusDelta < delta)
        {
            result = 5;
        }
        else if (delta < attrs->attackDeltas[attack].minusDelta)
        {
            result = 4;
        }
        else
        {
            result = 1;

            enemy->mirConditions = 0;
        }
    }

    return result;
}
#endif

void WALBOSS_MessageToLegs(int message)
{
    Instance *instance; // not from debug symbols

    for (instance = gameTrackerX.instanceList->first; instance != NULL; instance = instance->next)
    {
        if (INSTANCE_Query(instance, queryWhatAmI) == 0x410002)
        {
            INSTANCE_Post(instance, 0x1000017, message);
        }
    }
}

int WALBOSS_LivingLegs()
{
    Instance *instance; // not from debug symbols

    for (instance = gameTrackerX.instanceList->first; instance != NULL; instance = instance->next)
    {
        if ((INSTANCE_Query(instance, queryWhatAmI) == 0x410002) && (instance->currentMainState != MONSTER_STATE_DEAD))
        {
            return 1;
        }
    }

    return 0;
}

Instance *WALBOSS_FindEgg()
{
    Instance *instance; // not from debug symbols

    for (instance = gameTrackerX.instanceList->first; instance != NULL; instance = instance->next)
    {
        if ((INSTANCE_Query(instance, queryWhatAmI) & 0x20) && (strcmpi(instance->object->name, (char *)"walbose_") == 0))
        {
            return instance;
        }
    }

    return NULL;
}

Instance *WALBOSS_CreateEgg(Instance *parent)
{
    WalbossAttributes *attrs; // not from debug symbols
    Object *object; // not from debug symbols
    Instance *egg; // not from debug symbols
    Dummy *data; // not from debug symbols

    egg = NULL;

    attrs = (WalbossAttributes *)((MonsterAttributes *)parent->data)->tunData;

    object = OBTABLE_FindObject("walbose_");

    if (object != NULL)
    {
        egg = INSTANCE_BirthObject(parent, object, 0);

        if (egg != NULL)
        {
            data = (Dummy *)egg->extraData;

            data->unknown = MON_GetTime(egg);

            INSTANCE_Post(egg, 0x800029, 2);

            G2EmulationInstanceSetAnimation(egg, 0, 0, 0, 0);
            G2EmulationInstanceSetMode(egg, 0, 2);

            INSTANCE_LinkToParent(egg, parent, attrs->segmentToAttach);

            egg->processFunc = WCBEGG_Process;
            egg->collideFunc = WCBEGG_Collide;
            egg->messageFunc = WCBEGG_Message;
        }
    }

    return egg;
}

void WALBOSS_EjectEgg(Instance *parent)
{
    Instance *egg; // not from debug symbols

    egg = INSTANCE_GetChildLinkedToSegment(parent, ((WalbossAttributes *)((MonsterAttributes *)parent->data)->tunData)->segmentToAttach);

    if (egg != NULL)
    {
        G2EmulationInstanceSwitchAnimationAlpha(egg, 0, 1, 0, 0, 2, 0);

        INSTANCE_Post(egg, 0x800010, SetObjectThrowData(NULL, NULL, 0, 1, 60, -16, 40, 1024));
    }
}

void WALBOSS_BirthEggEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    WalbossVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (WalbossVars *)mv->extraVars;

    if (vars != NULL)
    {
        vars->animState = 0;

        mv->auxFlags &= ~0x1;
        mv->auxFlags |= 0x2;

        MON_PlayAnimFromList(instance, ma->auxAnimList, 0, 1);
    }
}

void WALBOSS_BirthEgg(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    WalbossVars *vars; // not from debug symbols
    WalbossAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (WalbossVars *)mv->extraVars;
    attrs = (WalbossAttributes *)ma->tunData;

    if ((vars != NULL) && (attrs != NULL))
    {
        switch (vars->animState)
        {
        case 0:
            if (instance->flags2 & 0x10)
            {
                MON_PlayAnimFromList(instance, ma->auxAnimList, 1, 1);

                vars->animState++;
            }

            break;
        case 1:
            if (instance->flags2 & 0x10)
            {
                vars->animState++;

                mv->auxFlags &= ~0x2;
            }
            else if (MON_AnimPlayingFromList(instance, ma->auxAnimList, 1) != 0)
            {
                if (G2EmulationInstanceQueryPassedFrame(instance, 0, attrs->frameToBirth) != 0)
                {
                    WALBOSS_CreateEgg(instance);
                }
                else if (G2EmulationInstanceQueryPassedFrame(instance, 0, attrs->frameToExpel) != 0)
                {
                    WALBOSS_EjectEgg(instance);
                }
            }

            break;
        }
    }
}

void WALBOSS_CorrectLookAT(Instance *instance, Position *lookAtPos)
{
    MATRIX myMat;
    Position headPos; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    WalbossVars *vars; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    MATRIX *mat; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (WalbossVars *)mv->extraVars;
    ma = (MonsterAttributes *)instance->data;

    if (vars != NULL)
    {
        mv->lookAtPos = &vars->correctedLookAt;

        myMat.m[0][0] = 4096;
        myMat.m[0][1] = 0;
        myMat.m[0][2] = 0;
        myMat.m[1][0] = 0;
        myMat.m[1][1] = 4096;
        myMat.m[1][2] = 0;
        myMat.m[2][0] = 0;
        myMat.m[2][1] = 0;
        myMat.m[2][2] = 4096;

        RotMatrixZ(-vars->lastDirection & 0xFFF, &myMat);

        mat = &instance->matrix[ma->headSegment];

        headPos.x = mat->t[0];
        headPos.y = mat->t[1];
        headPos.z = mat->t[2];

        SUB_SVEC(Position, &vars->correctedLookAt, Position, lookAtPos, Position, &headPos);

        ApplyMatrixSV(&myMat, (SVECTOR *)&vars->correctedLookAt, (SVECTOR *)&vars->correctedLookAt);

        ADD_SVEC(Position, &vars->correctedLookAt, Position, &vars->correctedLookAt, Position, &headPos);
    }
}

void WALBOSS_LegHurtEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;

    if (mv->extraVars != NULL)
    {
        mv->auxFlags &= ~0x10;
        mv->auxFlags |= 0x20;

        mv->generalTimer = MON_GetTime(instance) + 1485;

        MON_PlayAnimFromList(instance, ma->auxAnimList, 2, 2);

        SOUND_Play3dSound(&instance->position, 370, 0, 90, 15500);
    }
}

void WALBOSS_LegHurt(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if ((mv->extraVars != NULL) && (mv->generalTimer < MON_GetTime(instance)))
    {
        instance->flags2 &= ~0x10;

        mv->auxFlags &= ~0x20;

        WALBOSS_MessageToLegs(2);

        INSTANCE_Post(instance, 0x1000017, 1);
    }
}

void WALBOSS_HitByEggEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    WalbossVars *vars; // not from debug symbols
    WalbossAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (WalbossVars *)mv->extraVars;
    attrs = (WalbossAttributes *)ma->tunData;

    if (vars != NULL)
    {
        mv->auxFlags &= ~0x1;
        mv->auxFlags &= ~0x2;
        mv->auxFlags &= ~0x4;
        mv->auxFlags |= 0x8;

        if (--vars->numEggHits > 0)
        {
            mv->generalTimer = MON_GetTime(instance) + (attrs->timeForHitAgony * 33);

            if (vars->numEggHits >= 2)
            {
                MON_PlayAnimFromList(instance, ma->auxAnimList, 2, 2);
            }
            else
            {
                MON_PlayAnimFromList(instance, ma->auxAnimList, 3, 2);
            }

            SOUND_Play3dSound(&instance->position, 370, 0, 90, 15500);
        }
    }
}

void WALBOSS_HitByEgg(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if ((mv->extraVars != NULL) && (mv->generalTimer < MON_GetTime(instance)))
    {
        instance->flags2 &= ~0x10;

        mv->auxFlags &= ~0x8;

        WALBOSS_MessageToLegs(2);
    }
}

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_HandleThorn);
#else
void WALBOSS_HandleThorn(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols
    WalbossVars *vars; // not from debug symbols
    WalbossAttributes *attrs; // not from debug symbols
    Instance *player; // not from debug symbols
    Instance *thorn; // not from debug symbols
    Level *level; // not from debug symbols
    Level *playerLevel; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (WalbossVars *)mv->extraVars;
    attrs = (WalbossAttributes *)((MonsterAttributes *)instance->data)->tunData;
    thorn = vars->thornInst;
    player = gameTrackerX.playerInstance;

    if (!(mv->mvFlags & 0x4) && (gameTrackerX.gameData.asmData.MorphType != 0) && (gameTrackerX.gameData.asmData.MorphTime == 1000) && (thorn != NULL))
    {
        switch (vars->thornState)
        {
        case 0:
            level = STREAM_GetLevelWithID(instance->currentStreamUnitID);
            playerLevel = STREAM_GetLevelWithID(player->currentStreamUnitID);

            if ((player->tface != NULL) && (level == playerLevel) && (player->tface->textoff != 0xFFFF))
            {
                if (!(((TextureFT3 *)((char *)level->terrain->StartTextureList + player->tface->textoff))->attr & 0x100))
                {
                    goto prep;
                }

                if (MON_GetTime(instance) >= (unsigned long)vars->thornTimer)
                {
                    thorn->tface = player->tface;
                    thorn->tfaceLevel = player->tfaceLevel;
                    thorn->bspTree = player->bspTree;
                    thorn->currentStreamUnitID = player->currentStreamUnitID;

                    COPY_SVEC(Position, &vars->thornTarget, Position, &gameTrackerX.playerInstance->position);

                    vars->thornState++;

                    vars->thornTimer = MON_GetTime(instance) + (attrs->timeToThornPoke * 33);
                }
            }
            else
            {
            prep:
                vars->thornTimer = MON_GetTime(instance) + (attrs->timeToThornPrep * 33);
            }

            break;
        case 1:
            if (MON_GetTime(instance) >= (unsigned long)vars->thornTimer)
            {
                COPY_SVEC(Position, &thorn->position, Position, &vars->thornTarget);

                thorn->rotation.x = 0;
                thorn->rotation.y = 0;
                thorn->rotation.z = MATH3D_AngleFromPosToPos(&thorn->position, &instance->position);

                thorn->flags &= ~0x800;
                thorn->flags2 &= ~0x20000000;

                COLLIDE_SegmentCollisionOn(thorn, 1);

                if (thorn->tface != NULL)
                {
                    level = STREAM_GetLevelWithID(thorn->currentStreamUnitID);

                    if (level != NULL)
                    {
                        FX_StartInstanceBurrow(thorn, level, thorn->tface);
                    }
                }

                G2EmulationInstanceSetAnimation(thorn, 0, 0, 0, 0);
                G2EmulationInstanceSetMode(thorn, 0, 1);

                vars->thornState++;
            }

            break;
        case 2:
            if (thorn->flags & 0x800)
            {
                vars->thornState = 0;

                vars->thornTimer = MON_GetTime(instance) + (attrs->timeToThornPrep * 33);
            }

            break;
        }
    }
}
#endif

void WALBOSS_Collide(Instance *instance, GameTracker *gameTracker)
{
    CollideInfo *collideInfo; // not from debug symbols
    HSphere *hsphere; // not from debug symbols
    Instance *inst; // not from debug symbols

    collideInfo = (CollideInfo *)instance->collideInfo;
    hsphere = (HSphere *)collideInfo->prim0;

    if (((unsigned char)collideInfo->type0 == 1) && (hsphere->id == 9))
    {
        inst = (Instance *)collideInfo->inst1;

        if ((inst != NULL) && (INSTANCE_Query(inst, queryWhatAmI) == 0x410002))
        {
            return;
        }
    }

    collideInfo->offset.z = 0;
    collideInfo->offset.y = 0;
    collideInfo->offset.x = 0;

    MonsterCollide(instance, gameTracker);
}

void WALBOSS_Message(Instance *instance, unsigned long message, unsigned long data)
{
    MonsterVars *mv; // not from debug symbols
    WalbossVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (WalbossVars *)mv->extraVars;

    if (vars != NULL)
    {
        switch (message)
        {
        case 0x100001F:
        case 0x1000021:
            ((evMonsterHitData *)data)->power = 4096;

            MonsterMessage(instance, message, data);
            break;
        case 0x1000020:
            mv->auxFlags &= ~0x1;
            mv->auxFlags &= ~0x2;
            mv->auxFlags &= ~0x4;
            mv->auxFlags &= ~0x8;

            if (!(mv->mvFlags & 0x4) && (instance->currentMainState != MONSTER_STATE_DEAD) && (instance->currentMainState != MONSTER_STATE_GENERALDEATH))
            {
                MON_PlayCombatIdle(instance, 2);
                MON_SwitchStateDoEntry(instance, MONSTER_STATE_IDLE);
            }

            INSTANCE_Post(vars->thornInst, 0x1000017, 0);
            break;
        case 0x1000017:
            switch (data)
            {
            case 0:
                WALBOSS_MessageToLegs(0);
                MON_SwitchStateDoEntry(instance, MONSTER_STATE_DEAD);
                break;
            case 1:
                if (WALBOSS_FindEgg() == NULL)
                {
                    mv->auxFlags |= 0x1;
                    MON_SwitchStateDoEntry(instance, MONSTER_STATE_IDLE);
                }

                break;
            case 2:
                mv->auxFlags |= 0x4;

                WALBOSS_MessageToLegs(1);
                MON_SwitchStateDoEntry(instance, MONSTER_STATE_IDLE);
                break;
            case 3:
                if (!(mv->auxFlags & 0xF))
                {
                    mv->auxFlags |= 0x10;

                    WALBOSS_MessageToLegs(1);
                    MON_SwitchStateDoEntry(instance, MONSTER_STATE_IDLE);
                }

                break;
            }

            break;
        case 0x100007:
            MonsterMessage(instance, 0x100007, data);
            MON_PlayAnim(instance, MONSTER_ANIM_FALLOVER, 1);
            break;
        case 0x40009:
        case 0x80001:
        case 0x80002:
        case 0x80003:
        case 0x80004:
        case 0x80005:
        case 0x80006:
            break;
        default:
            MonsterMessage(instance, message, data);
            break;
        }
    }
}

unsigned long WALBOSS_Query(Instance *instance, unsigned long query)
{
    WalbossVars *vars; // not from debug symbols

    vars = (WalbossVars *)((MonsterVars *)instance->extraData)->extraVars;

    if (vars != NULL)
    {
        switch (query)
        {
        case 0:
            return 0;
        case 12:
            return (unsigned long)instance->matrix;
        case 30:
            return vars->numEggHits <= 0;
        }
    }

    return MonsterQuery(instance, query);
}

void WALBOSS_Init(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols
    WalbossVars *vars; // not from debug symbols
    WalbossAttributes *attrs; // not from debug symbols
    Object *object; // not from debug symbols

    attrs = (WalbossAttributes *)((MonsterAttributes *)instance->data)->tunData;

    MON_DefaultInit(instance);

    mv = (MonsterVars *)instance->extraData;

    if (mv != NULL)
    {
        vars = (WalbossVars *)MEMPACK_Malloc(sizeof(WalbossVars), 41);

        if (vars == NULL)
        {
            mv->extraVars = NULL;

            vars->thornInst = NULL;
        }
        else
        {
            mv->extraVars = vars;

            vars->tauntState = 0;
            vars->lastDirection = 0;

            if (attrs != NULL)
            {
                vars->numEggHits = attrs->maxEggHits;
            }

            vars->thornInst = NULL;

            object = OBTABLE_FindObject("walbosc_");

            if (object != NULL)
            {
                vars->thornInst = INSTANCE_BirthObject(instance, object, 0);

                if (vars->thornInst != NULL)
                {
                    vars->thornState = 2;

                    vars->thornInst->collideFunc = WALBOSC_Collide;

                    G2Anim_SetCallback(&vars->thornInst->anim, WALBOSC_AnimCallback, vars->thornInst);

                    vars->thornInst->flags |= 0x800;
                    vars->thornInst->flags2 |= 0x20000000;

                    SAVE_MarkDeadDead(vars->thornInst);
                }
            }
        }
    }

    instance->xVel = 0;
    instance->yVel = 0;
    instance->zVel = 0;

    if (attrs != NULL)
    {
        G2Anim_AttachControllerToSeg(&instance->anim, attrs->segmentForBodyTwist, 14);
        G2Anim_EnableController(&instance->anim, attrs->segmentForBodyTwist, 14);
    }

    WALBOSS_SetSegmentYaw(instance, 0);

    instance->collideFunc = WALBOSS_Collide;

    mv->mvFlags |= 0x800;

    MON_SwitchState(instance, MONSTER_STATE_IDLE);

    MON_PlayAnimFromList(instance, ((MonsterAttributes *)instance->data)->auxAnimList, 5, 1);

    mv->soulJuice = 0;
    mv->targetFade = 0;

    mv->auxFlags |= 0x20000000;

    RAZIEL_SetInteractiveMusic(15, 1);
}

void WALBOSS_CleanUp(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols
    WalbossAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    attrs = (WalbossAttributes *)((MonsterAttributes *)instance->data)->tunData;

    RAZIEL_SetInteractiveMusic(15, 0);

    if (attrs != NULL)
    {
        G2Anim_DetachControllerFromSeg(&instance->anim, attrs->segmentForBodyTwist, 14);
    }

    if ((mv != NULL) && (mv->extraVars != NULL))
    {
        MEMPACK_Free((char *)mv->extraVars);
    }

    MON_CleanUp(instance);
}

void WALBOSS_DamageEffect(Instance *instance, evFXHitData *data)
{
    SVector localloc;

    if (data != NULL)
    {
        localloc = data->location;

        SOUND_Play3dSound(&instance->position, 21, 650, 80, 15500);

        FX_MakeHitFX(&localloc);
    }
}

int WALBOSS_HandleFade(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols
    WalbossVars *vars; // not from debug symbols
    int fade; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (WalbossVars *)mv->extraVars;

    switch (vars->tauntState)
    {
    case 0:
        if (gameTrackerX.gameData.asmData.MorphTime != 1000)
        {
            return 1;
        }

        if (gameTrackerX.gameData.asmData.MorphType == 1)
        {
            vars->tauntTimer = MON_GetTime(instance) + 1980;
            vars->tauntState++;
        }
        else
        {
            return 0;
        }

        break;
    case 1:
        if (MON_GetTime(instance) >= (unsigned long)vars->tauntTimer)
        {
            vars->tauntState++;
        }

        break;
    case 2:
        if (mv->targetFade == 4096)
        {
            vars->tauntTimer = MON_GetTime(instance) + 1980;
            vars->tauntState++;
        }
        else
        {
            fade = ((MON_GetTime(instance) - vars->tauntTimer) << 12) / 990;

            if (fade >= 4096)
            {
                mv->targetFade = 4096;
            }
            else
            {
                mv->targetFade = fade;
            }
        }

        break;
    case 3:
        if (MON_GetTime(instance) >= (unsigned long)vars->tauntTimer)
        {
            vars->tauntTimer = MON_GetTime(instance);
            vars->tauntState++;
        }

        break;
    case 4:
        if (mv->targetFade == 0)
        {
            vars->tauntTimer = MON_GetTime(instance) + 9900;
            vars->tauntState = 5;
        }
        else
        {
            fade = ((990 - (int)(MON_GetTime(instance) - vars->tauntTimer)) << 12) / 990;

            if (fade <= 0)
            {
                mv->targetFade = 0;
            }
            else
            {
                mv->targetFade = fade;
            }
        }

        break;
    case 5:
        if ((MON_GetTime(instance) >= (unsigned long)vars->tauntTimer) || (*(long *)&gameTrackerX.gameData.asmData.MorphTime == 1000))
        {
            vars->tauntState = 0;
        }

        break;
    }

    return 1;
}

void WALBOSS_IdleEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (!(mv->mvFlags & 0x4))
    {
        if (mv->auxFlags & 0x4)
        {
            WALBOSS_HitByEggEntry(instance);
            return;
        }

        if (mv->auxFlags & 0x1)
        {
            WALBOSS_BirthEggEntry(instance);
            return;
        }

        if (mv->auxFlags & 0x10)
        {
            WALBOSS_LegHurtEntry(instance);
            return;
        }

        if (mv->auxFlags & 0x2A)
        {
            return;
        }
    }

    MON_IdleEntry(instance);
}

void WALBOSS_Idle(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols
    MonsterIR *enemy; // not from debug symbols
    int combat; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    enemy = mv->enemy;

    combat = 0;

    if (mv->extraVars != NULL)
    {
        if (!(mv->mvFlags & 0x4))
        {
            WALBOSS_InterpSegmentYaw(instance, 0);
            WALBOSS_HandleThorn(instance);

            if (WALBOSS_HandleFade(instance) == 0)
            {
                if (mv->auxFlags & 0x20)
                {
                    WALBOSS_LegHurt(instance);
                }
                else if (mv->auxFlags & 0x8)
                {
                    WALBOSS_HitByEgg(instance);
                }
                else if (mv->auxFlags & 0x2)
                {
                    WALBOSS_BirthEgg(instance);
                }
                else
                {
                    if (enemy != NULL)
                    {
                        if (enemy->mirFlags & 0x100)
                        {
                            MON_SwitchState(instance, MONSTER_STATE_NOTICE);
                        }
                        else if (enemy->distance < mv->subAttr->combatAttributes->combatRange)
                        {
                            WALBOSS_TurnToPosition(instance, &enemy->instance->position);

                            combat = 1;

                            MON_SwitchState(instance, MONSTER_STATE_COMBAT);
                        }

                        WALBOSS_CorrectLookAT(instance, &enemy->instance->position);
                    }

                    if (!(mv->mvFlags & 0x4000000) || (instance->flags2 & 0x2))
                    {
                        if (combat != 0)
                        {
                            MON_PlayCombatIdle(instance, 2);
                        }
                        else
                        {
                            MON_PlayRandomIdle(instance, 2);
                        }
                    }
                }
            }

            MON_IdleQueueHandler(instance);
        }
        else
        {
            WALBOSS_SetSegmentYaw(instance, 0);

            MON_Idle(instance);
        }
    }
}

void WALBOSS_NoticeEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (mv->extraVars != NULL)
    {
        if ((WALBOSS_HandleFade(instance) != 0) || (mv->auxFlags & 0x2))
        {
            MON_SwitchStateDoEntry(instance, MONSTER_STATE_IDLE);
        }
        else
        {
            MON_PlayAnim(instance, MONSTER_ANIM_NOTICE, 1);
        }
    }
}

void WALBOSS_Notice(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (instance->flags2 & 0x10)
    {
        MON_SwitchState(instance, MONSTER_STATE_COMBAT);
    }

    if (mv->enemy != NULL)
    {
        WALBOSS_CorrectLookAT(instance, &mv->enemy->instance->position);
    }

    MON_DefaultQueueHandler(instance);
}

void WALBOSS_CombatEntry(Instance *instance)
{
    MON_CombatEntry(instance);
}

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_Combat);
#else
void WALBOSS_Combat(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols
    MonsterIR *enemy; // not from debug symbols
    MonsterCombatAttributes *combat; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    enemy = mv->enemy;

    if (mv->extraVars != NULL)
    {
        if (!(mv->mvFlags & 0x4) && (enemy != NULL))
        {
            combat = mv->subAttr->combatAttributes;

            WALBOSS_TurnToPosition(instance, &enemy->instance->position);
            WALBOSS_CorrectLookAT(instance, &enemy->instance->position);

            if (enemy->distance < combat->combatRange)
            {
                if (WALBOSS_ShouldIAttack(instance, enemy, WALBOSS_ChooseAttack(instance, enemy)) == 1)
                {
                    MON_SwitchState(instance, MONSTER_STATE_ATTACK);
                }
                else
                {
                    MON_PlayCombatIdle(instance, 2);
                }
            }
            else
            {
                MON_SwitchState(instance, MONSTER_STATE_IDLE);
            }
        }
        else
        {
            MON_SwitchState(instance, MONSTER_STATE_IDLE);
        }

        MON_IdleQueueHandler(instance);
    }
}
#endif

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_AttackEntry);
#else
void WALBOSS_AttackEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols
    MonsterAttackAttributes *attack; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    attack = mv->attackType;

    mv->mode = 0x200000;

    MON_PlayAnimFromList(instance, attack->animList, 0, 1);
}
#endif

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/Overlays/walboss/walboss", WALBOSS_Attack);
#else
void WALBOSS_Attack(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols
    MonsterIR *enemy; // not from debug symbols
    MonsterAttackAttributes *attack; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    enemy = mv->enemy;
    attack = mv->attackType;

    if (mv->extraVars != NULL)
    {
        if (mv->mvFlags & 0x4)
        {
            MON_SwitchState(instance, MONSTER_STATE_IDLE);
        }
        else if (enemy != NULL)
        {
            WALBOSS_TurnToPosition(instance, &enemy->instance->position);
            WALBOSS_CorrectLookAT(instance, &enemy->instance->position);
        }
        else
        {
            WALBOSS_SetSegmentYaw(instance, 0);
        }

        if ((MON_AnimPlayingFromList(instance, attack->animList, attack->sphereOnAnim) != 0) && (G2EmulationInstanceQueryPassedFrame(instance, 0, attack->sphereOnFrame) != 0))
        {
            MON_TurnOnWeaponSphere(instance, attack->sphereSegment);
        }
        else if ((MON_AnimPlayingFromList(instance, attack->animList, attack->sphereOffAnim) != 0) && (G2EmulationInstanceQueryPassedFrame(instance, 0, attack->sphereOffFrame) != 0) && (mv->mvFlags & 0x4000))
        {
            MON_TurnOffWeaponSpheres(instance);

            if (enemy != NULL)
            {
                enemy->mirConditions |= 0x200;
            }
        }

        if (instance->flags2 & 0x10)
        {
            instance->flags2 &= ~0x10;

            if (++mv->attackState < attack->numAnims)
            {
                MON_PlayAnimFromList(instance, attack->animList, mv->attackState, 1);
            }
            else
            {
                MON_SwitchState(instance, MONSTER_STATE_COMBAT);
            }
        }

        MON_DefaultQueueHandler(instance);
    }
}
#endif

void WALBOSS_HitEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols
    MonsterIR *enemy; // not from debug symbols
    short angle; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    enemy = mv->enemy;

    if ((WALBOSS_HandleFade(instance) != 0) || (mv->auxFlags & 0x2A))
    {
        MON_SwitchStateDoEntry(instance, MONSTER_STATE_IDLE);
    }
    else if (WALBOSS_LivingLegs() != 0)
    {
        mv->enemy->mirConditions |= 0x400;

        angle = ((instance->rotation.z - MATH3D_AngleFromPosToPos(&instance->position, &enemy->instance->position)) + 1024) & 0xFFF;

        if (angle < 2048)
        {
            MON_PlayAnim(instance, MONSTER_ANIM_HIT1, 1);
        }
        else
        {
            MON_PlayAnim(instance, MONSTER_ANIM_HIT2, 1);
        }

        mv->mode = 0x8000;
    }
    else
    {
        INSTANCE_Post(instance, 0x1000017, 1);
    }
}

void WALBOSS_Hit(Instance *instance)
{
    if (!(((MonsterVars *)instance->extraData)->mvFlags & 0x4000000))
    {
        MON_SwitchState(instance, MONSTER_STATE_COMBAT);
    }

    MON_DefaultQueueHandler(instance);
}

void WALBOSS_DoNothingEntry(Instance *instance)
{
    MON_SwitchStateDoEntry(instance, ((MonsterVars *)instance->extraData)->previousMainState);
}

void WALBOSS_DoNothing()
{
}

void WALBOSS_DeadEntry(Instance *instance)
{
    ((MonsterVars *)instance->extraData)->soulJuice = 0;

    MON_DeadEntry(instance);
    MON_TurnOnBodySpheres(instance);
}

void WALBOSS_Dead(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    while (DeMessageQueue(&mv->messageQueue) != NULL);
}
