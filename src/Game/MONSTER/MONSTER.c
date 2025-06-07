#include "Game/MONSTER/MONSTER.h"
#include "Game/MONSTER/MONAPI.h"
#include "Game/MONSTER/MONLIB.h"
#include "Game/MONSTER/MONMSG.h"
#include "Game/MONSTER/MONSENSE.h"
#include "Game/MONSTER/MONTABLE.h"
#include "Game/PHYSICS.h"
#include "Game/MONSTER/MISSILE.h"
#include "Game/G2/ANIMG2.h"
#include "Game/MATH3D.h"
#include "Game/PLAN/PLANAPI.h"
#include "Game/FX.h"
#include "Game/STATE.h"
#include "Game/SOUND.h"
#include "Game/PLAN/ENMYPLAN.h"
#include "Game/CAMERA.h"
#include "Game/MEMPACK.h"
#include "Game/SAVEINFO.h"
#include "Game/GAMELOOP.h"
#include "Game/STREAM.h"
#include "Game/INSTANCE.h"
#include "Game/G2/ANMCTRLR.h"
#include "Game/G2/ANMG2ILF.h"
#include "Game/MONSTER/HUMAN.h"

static burntTuneType burntTest = {300, 2};

static int D_800D1B38[2] = {0, 65534}; // for use on MON_DamageEffect

static int D_800D1B40[2] = {0, 1}; // for use on MON_DamageEffect

static unsigned char D_800CC4D0[] = {15, 17, 16, 18}; // pupateObjects

void MON_DoCombatTimers(Instance *instance)
{
    MonsterVars *mv;
    unsigned long curtime;

    mv = (MonsterVars *)instance->extraData;

    curtime = MON_GetTime(instance);

    if ((mv->mvFlags & 0x10))
    {
        if (mv->damageTimer < curtime)
        {
            mv->mvFlags &= ~0x10;
            mv->mvFlags &= ~0x800000;
        }
    }
    else if ((!(mv->mvFlags & 0x2000)) && (mv->hitPoints < mv->maxHitPoints))
    {
        MonsterCombatAttributes *combat;

        combat = mv->subAttr->combatAttributes;

        mv->hitPoints += (combat->recovery * gameTrackerX.timeMult) / 4096;

        if (mv->hitPoints > mv->maxHitPoints)
        {
            mv->hitPoints = mv->maxHitPoints;
        }

        MONSTER_VertexBlood(instance, mv->mainColorVertex, MONSTER_CalcDamageIntensity(mv->hitPoints, mv->maxHitPoints));
    }

    if (((mv->mvFlags & 0x100)) && (mv->stunTimer < curtime))
    {
        mv->mvFlags &= ~0x100;

        do {} while (0); // garbage code for reordering
    }

    if ((curtime % 1000) < ((curtime - gameTrackerX.lastLoopTime) % 1000))
    {
        mv->chance = rand() % 100;
    }
}

void MON_ChangeHumanOpinion(Instance *instance)
{
    int good;

    good = INSTANCE_Query(instance, 1);

    if ((good & 0xC000))
    {
        int opinion;

        opinion = GlobalSave->humanOpinionOfRaziel;

        if (opinion < -40)
        {
            opinion -= 5;
        }
        else
        {
            opinion = -40;
        }

        if (opinion < -32767)
        {
            opinion = -32767;
        }
        else if (opinion > 32767)
        {
            opinion = 32767;
        }

        if ((opinion <= 0) && (GlobalSave->humanOpinionOfRaziel > 0))
        {
            HUMAN_GetAngry();
        }

        GlobalSave->humanOpinionOfRaziel = opinion;
    }
}

void MON_CutOut_Monster(Instance *instance, int fade_amount, int startseg, int endseg)
{
    SVector point;
    SVector normal;
    SVector p1;
    SVector p2;

    long color;
    int tmp;

    short _x0;
    short _x1;
    short _y0;
    short _y1;
    short _z0;
    short _z1;

    MATRIX *mat;

    if (instance->matrix != NULL)
    {

        mat = &instance->matrix[startseg];

        _x0 = mat->t[0];
        p1.x = _x0;

        _y0 = mat->t[1];
        p1.y = _y0;

        _z0 = mat->t[2];
        p1.z = _z0;

        mat = &instance->matrix[endseg];

        _x1 = mat->t[0];
        p2.x = _x1;

        _y1 = mat->t[1];
        p2.y = _y1;

        _z1 = mat->t[2];
        p2.z = mat->t[2];

        tmp = (_x0 - _x1) / 8;
        p1.x = _x0 + tmp;
        p2.x = _x1 - tmp;

        tmp = (_y0 - _y1) / 8;
        p1.y = _y0 + tmp;
        p2.y = _y1 - tmp;

        tmp = (_z0 - _z1) / 8;
        p1.z = _z0 + tmp;
        p2.z = _z1 - tmp;

        LoadAverageShort12((SVECTOR *)&p2, (SVECTOR *)&p1, fade_amount, 0x1000 - fade_amount, (SVECTOR *)&point);

        if (!(instance->halvePlane.flags & 8))
        {

            color = 0;
            SUB_SVEC(SVector, &normal, SVector, &p2, SVector, &p1);
            CAMERA_Normalize(&normal);

            color = 0x80FF;
            FX_DoInstancePowerRing(instance, 0x2EE0, &color, 0, 2);
            FX_DoInstancePowerRing(instance, 0x2EE0, &color, 0, 1);

            instance->halvePlane.flags = 8U;
            instance->halvePlane.a = normal.x;
            instance->halvePlane.b = normal.y;
            instance->halvePlane.c = normal.z;

        }
        instance->halvePlane.d = -((instance->halvePlane.a * point.x + instance->halvePlane.b * point.y + instance->halvePlane.c * point.z) >> 0xC);
    }
}

void MON_DeadEntry(Instance *instance)
{
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    MON_TurnOffAllSpheres(instance);

    if ((mv->enemy != NULL) && ((INSTANCE_Query(mv->enemy->instance, 1) & 0x1)))
    {
        MON_ChangeHumanOpinion(instance);
    }

    if (mv->previousMainState == -1)
    {
        MON_PlayAnim(instance, MONSTER_ANIM_GENERALDEATH, 1);
    }

    mv->mvFlags |= 0x200;

    instance->flags2 &= ~0x20000;
    instance->flags2 &= ~0x40;
    instance->flags2 &= ~0x80;

    mv->damageTimer = 0;

    if (mv->soulJuice != 0)
    {
        if (mv->soulID == 0)
        {
            MON_BirthSoul(instance, 1);
        }
    }
    else
    {
        mv->damageTimer = MON_GetTime(instance);
    }
}

INCLUDE_ASM("asm/nonmatchings/Game/MONSTER/MONSTER", MON_Dead);

void MON_MissileHitEntry(Instance *instance)
{
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    instance->xAccl = 0;
    instance->yAccl = 0;

    instance->xVel = 0;
    instance->yVel = 0;

    MON_PlayAnim(instance, MONSTER_ANIM_MISSILEIMPALEDEATH, 1);

    mv->mvFlags |= 0x200000;
    mv->mvFlags &= ~0x10;

    MON_TurnOffAllSpheres(instance);

    mv->causeOfDeath = 0;

    MON_DropAllObjects(instance);

    mv->heldID = mv->held->introUniqueID;

    INSTANCE_Post(mv->held, 0x800002, SetObjectData(0, 0, 0, instance, 3));

    SOUND_Play3dSound(&instance->position, 39, -100, 100, 16000);
}

void MON_MissileHit(Instance *instance)
{
    MonsterVars *mv;
    MonsterAttributes *ma;
    int thisFrame;
    int lastFrame;

    mv = (MonsterVars *)instance->extraData;

    ma = (MonsterAttributes *)instance->data;

    thisFrame = G2EmulationInstanceQueryFrame(instance, 0);
    lastFrame = G2EmulationInstanceQueryLastFrame(instance, 0);

    if ((lastFrame < ma->bloodImpaleFrame) && (thisFrame >= ma->bloodImpaleFrame))
    {
        FX_Blood_Impale(instance, ma->grabSegment, instance, ma->grabSegment);
    }
    else if ((lastFrame < ma->bloodConeFrame) && (thisFrame >= ma->bloodConeFrame))
    {
        FX_BloodCone(instance, ma->grabSegment, 80);
    }

    if ((instance->flags2 & 0x10))
    {
        MON_SwitchState(instance, MONSTER_STATE_DEAD);
    }

    while (DeMessageQueue(&mv->messageQueue) != NULL);
}

void MON_BirthEntry(Instance *instance)
{
    (void)instance;
}

void MON_Birth(Instance *instance)
{
    MON_SwitchState(instance, MONSTER_STATE_IDLE);
}

void MON_ParryEntry(Instance *instance)
{
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    MON_PlayAnim(instance, MONSTER_ANIM_JUMPRIGHT, 1);

    do
    {
    } while (0); // garbage code for reordering

    MON_TurnToPosition(instance, &mv->enemy->instance->position, 4096);
}

void MON_Parry(Instance *instance)
{
    if ((instance->flags2 & 0x10))
    {
        MON_SwitchState(instance, MONSTER_STATE_COMBAT);
    }

    MON_DefaultQueueHandler(instance);
}

void MON_LandOnFeetEntry(Instance *instance)
{
    int damage;
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    if ((signed char)mv->previousMainState == MONSTER_STATE_THROWN)
    {
        MON_PlayAnim(instance, MONSTER_ANIM_SPINLAND, 1);

        do {} while (0); // garbage code for reordering

        instance->xAccl = 0;
        instance->yAccl = 0;
        instance->zAccl = -0x10;

    }
    else
    {
        MON_PlayAnim(instance, MONSTER_ANIM_LANDONFEET, 1);
        instance->xAccl = 0;
        instance->yAccl = 0;
        instance->zAccl = 0;
    }

    if (instance->zVel < -0x65)
    {
        damage = 0x1000;
        if (instance->zVel < -0xF6)
        {
            damage = 0x3000;
            if (instance->zVel >= -0x15D)
            {
                damage = 0x2000;
            }
        }
        MON_TakeDamage(instance, damage, 0x40000);
    }
    instance->xVel = 0;
    instance->yVel = 0;
    instance->zVel = 0;
    instance->checkMask &= ~0x20;
}


void MON_LandOnFeet(Instance *instance)
{

    enum MonsterState state;
    MonsterVars *mv;

    mv = instance->extraData;
    MON_DefaultQueueHandler(instance);

    if ((instance->currentMainState == MONSTER_STATE_GENERALDEATH) && (mv->damageType == 0x20 || mv->damageType == 0x40))
    {
        MON_BurnInAir(instance, MONSTER_STATE_LANDONFEET);
    }

    if (instance->flags2 & 0x10)
    {

        MON_AnimPlaying(instance, MONSTER_ANIM_SPINLAND);

        if (!(instance->flags2 & 0x08000000))
        {
            if (mv->mvFlags & 0x2000 && mv->hitPoints == 0)
            {
                mv->damageType = 0x4000U;
                state = MONSTER_STATE_GENERALDEATH;
            }
            else
            {
                state = MONSTER_STATE_GENERALDEATH;
                if (!(mv->mvFlags & 0x400000))
                {
                    if (mv->hitPoints == 0 && mv->damageType != 0)
                    {
                        mv->mvFlags &= ~0x100;
                        state = MONSTER_STATE_STUNNED;
                    }
                    else if (mv->enemy != NULL)
                    {
                        state = MONSTER_STATE_COMBAT;
                    }
                    else
                    {
                        state = MONSTER_STATE_IDLE;
                    }
                }
            }
        }
        else if (mv->enemy != NULL)
        {
            state = MONSTER_STATE_COMBAT;
        }
        else
        {
            state = MONSTER_STATE_IDLE;
        }

        MON_SwitchState(instance, state);
    }

    if (mv->mvFlags & 0x400)
    {
        MON_SwitchState(instance, MONSTER_STATE_LANDINWATER);
    }
    else if (!(mv->mvFlags & 2))
    {
        MON_ApplyPhysics(instance);
    }

    if (instance->currentMainState != MONSTER_STATE_LANDONFEET)
    {
        instance->xAccl = 0;
        instance->yAccl = 0;
        instance->zAccl = 0;
        instance->xVel = 0;
        instance->yVel = 0;
        instance->zVel = 0;
    }
}

void MON_LandInWaterEntry(Instance *instance)
{
    instance->checkMask &= ~0x20;
}

void MON_LandInWater(Instance *instance)
{
    MON_SwitchState(instance, MONSTER_STATE_FALL);
}

void MON_BreakHoldEntry(Instance *instance)
{

    MonsterVars *mv;
    mv = (MonsterVars *)instance->extraData;

    MON_PlayAnim(instance, MONSTER_ANIM_BREAKHOLD, 1);
    mv->mode = 0x100000;

    if (instance->object->oflags & 0x200)
    {
        instance->flags2 |= 0x40;
    }
}

void MON_BreakHold(Instance *instance)
{

    if (instance->flags2 & 0x10)
    {
        MON_UnlinkFromRaziel(instance);
    }
    else
    {

        MonsterVars *mv;
        Message *message; // Not from decls.h

        mv = instance->extraData;
        instance->rotation.z = mv->enemy->instance->rotation.z + 0x800;

        while ((message = DeMessageQueue(&mv->messageQueue)) != NULL)
        {

            evMonsterHitData *messageData; // Not from decls.h

            switch (message->ID)
            {
            case 0x1000003:
                messageData = (evMonsterHitData *)message->Data;
                INSTANCE_UnlinkFromParent(instance);
                MON_LaunchMonster(instance, (messageData)->knockBackDistance, (short)messageData->power, 0x32);
                break;
            case 0x1000007:
                if (((evMonsterHitTerrainData *)message->Data)->bspFlags & 0x2000)
                {
                    MON_UnlinkFromRaziel(instance);
                }
                break;
            case 0x1000002:
            case 0x100000B:
                break;
            default:
                MON_DefaultMessageHandler(instance, message);
                break;
            }
        }

        if (instance->LinkParent == NULL)
        {
            if (instance->currentMainState == MONSTER_STATE_BREAKHOLD && mv->speed != 0)
            {
                MON_TurnOnBodySpheres(instance);
                MON_SwitchState(instance, MONSTER_STATE_FALL);
                instance->rotation.z = gameTrackerX.playerInstance->rotation.z + 0x800;
            }
            instance->rotation.x = 0;
            instance->rotation.y = 0;
        }
        else
        {
            mv->speed = 1;
        }
    }

    if (instance->LinkParent != NULL && instance->currentMainState != MONSTER_STATE_BREAKHOLD)
    {
        MON_UnlinkFromRaziel(instance);
    }
}

void MON_ImpactEntry(Instance *instance)
{

    evFXHitData data;
    MonsterVars *mv;
    MonsterCombatAttributes *combat;

    mv = (MonsterVars *)instance->extraData;
    combat = mv->subAttr->combatAttributes;

    do {} while (0); // garbage code for reordering
    MON_PlayAnim(instance, MONSTER_ANIM_IMPACT, 1);

    instance->xVel = 0;
    instance->yVel = 0;
    instance->zVel = 0;
    mv->mode = 0x100000;

    MON_TakeDamage(instance, 0x3000, 0x100);
    MON_SetFXHitData(instance, &data, 0x40000, 0);

    ((MONTABLE_DamageEffectFunc)MONTABLE_GetDamageEffectFunc(instance))(instance, (intptr_t)&data);

    if (mv->hitPoints == 0)
    {
        mv->damageTimer = MON_GetTime(instance) + combat->damageTime;
        mv->mvFlags |= 0x800010;
    }

    instance->checkMask &= ~0x20;
}

void MON_Impact(Instance *instance)
{

    MonsterVars *mv;
    mv = (MonsterVars *)instance->extraData;

    if (instance->flags2 & 0x10)
    {
        MON_SwitchState(instance, MONSTER_STATE_FALL);
    }

    MON_DefaultQueueHandler(instance);

    if (instance->currentMainState == MONSTER_STATE_GENERALDEATH && (mv->damageType == 0x20 || mv->damageType == 0x40))
    {
        MON_BurnInAir(instance, MONSTER_STATE_IMPACT);
    }
}

void MON_FallEntry(Instance *instance)
{

    MonsterVars *mv;
    mv = (MonsterVars *)instance->extraData;

    if ((signed char)mv->previousMainState == MONSTER_STATE_BREAKHOLD)
    {
        MON_PlayAnim(instance, MONSTER_ANIM_BREAKFALL, 1);
    }
    else
    {

        if ((signed char)mv->previousMainState == MONSTER_STATE_PURSUE ||
            (signed char)mv->previousMainState == MONSTER_STATE_FLEE ||
            (signed char)mv->previousMainState == MONSTER_STATE_COMBAT ||
            (signed char)mv->previousMainState == MONSTER_STATE_WANDER)
        {

            PhysicsSetVelFromRot(instance, &instance->rotation, 0x28);
            instance->zVel = 0x28;

        }

        MON_PlayAnim(instance, MONSTER_ANIM_FALL, 2);
    }

    instance->xAccl = 0;
    instance->yAccl = 0;
    instance->zAccl = -0x10;

    do {} while (0); // garbage code for reordering

    mv->mode = 0x100000;


    mv->generalTimer = MON_GetTime(instance) + 0x7D0;
}

void MON_Fall(Instance *instance)
{
    int x;
    int y;
    enum MonsterState state;
    Message *message;
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;
    state = MONSTER_STATE_FALL;

    if (mv->mvFlags & 2)
    {
        state = MONSTER_STATE_LANDONFEET;
    }
    else if (mv->mvFlags & 0x400)
    {
        state = MONSTER_STATE_LANDINWATER;
    }
    else
    {
        MON_ApplyPhysics(instance);
    }

    if (state != MONSTER_STATE_FALL)
    {
        MON_SwitchState(instance, state);
    }

    if (mv->generalTimer < MON_GetTime(instance))
    {
        if (instance->position.x == instance->oldPos.x &&
            instance->position.y == instance->oldPos.y &&
            instance->position.z == instance->oldPos.z)
        {
            x = (rand() & 0x7F) - 0x3F;
            y = (rand() & 0x7F) - 0x3F;;
            instance->position.x += x;
            instance->position.y += y;
        }
    }



    while ((message = DeMessageQueue(&mv->messageQueue)) != NULL)
    {
        if (message->ID != 0x01000007)
        {
            MON_DefaultMessageHandler(instance, message);
        }
    }

    if (instance->currentMainState == MONSTER_STATE_GENERALDEATH &&
        state == MONSTER_STATE_FALL &&
        (mv->damageType == 0x20 || mv->damageType == 0x40))
    {
        MON_BurnInAir(instance, MONSTER_STATE_FALL);
    }
}

void MON_ThrownEntry(Instance *instance)
{

    MonsterVars *mv;
    mv = (MonsterVars *)instance->extraData;

    do {} while (0); // garbage code for reordering

    instance->xAccl = 0;
    instance->yAccl = 0;
    instance->zAccl = -8;

    MON_PlayAnim(instance, MONSTER_ANIM_THROWN, 2);

    mv->mode = 0x100000;

    if (instance->object->oflags & 0x200)
    {
        instance->flags2 |= 0x40;
    }

    instance->checkMask |= 0x20;
    mv->mvFlags &= ~2;
}

void MON_Thrown(Instance *instance)
{

    Message *message; // Not from decls.h
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    if (mv->mvFlags & 2)
    {

        Intro *intro; // Not from decls.h
        intro = MON_TestForTerrainImpale(instance, STREAM_GetLevelWithID(instance->currentStreamUnitID)->terrain);

        if (intro != NULL)
        {
            INSTANCE_Post(instance, 0x0100001C, intro->UniqueID);
            MON_DefaultQueueHandler(instance);
            return;
        }

        instance->xAccl = 0;
        instance->yAccl = 0;
        instance->zAccl = 0;
        MON_SwitchState(instance, MONSTER_STATE_LANDONFEET);
        return;
    }

    MON_ApplyPhysics(instance);

    while ((message = DeMessageQueue(&mv->messageQueue)) != NULL)
    {
        switch (message->ID)
        {
        case 0x01000008:
            break;
        case 0x01000007:
            instance->rotation.z = ratan2(((evPhysicsGravityData *)message->Data)->y, ((evPhysicsGravityData *)message->Data)->x) + 0x400;
            MON_SwitchState(instance, MONSTER_STATE_IMPACT);
            break;
        default:
            MON_DefaultMessageHandler(instance, message);
        }
    }

    if (instance->currentMainState == MONSTER_STATE_GENERALDEATH && ((mv->damageType == 0x20) || mv->damageType == 0x40))
    {
        MON_BurnInAir(instance, MONSTER_STATE_THROWN);
    }
}

void MON_ImpaleDeathEntry(Instance *instance)
{

    MonsterVars *mv;
    mv = (MonsterVars *)instance->extraData;

    MON_PlayAnim(instance, MONSTER_ANIM_IMPALED, 1);

    mv->mvFlags |= 0x200000;
    mv->mvFlags &= ~0x10;

    MON_TurnOffAllSpheres(instance);
    mv->generalTimer = MON_GetTime(instance) + 0x7530;

    instance->xAccl = 0;
    instance->yAccl = 0;
    instance->xVel = 0;
    instance->yVel = 0;

    MON_DropAllObjects(instance);
}

void MON_ImpaleDeath(Instance *instance)
{

    Message *message;
    MonsterVars *mv;
    MonsterAttributes *ma;

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;

    if (MON_AnimPlaying(instance, MONSTER_ANIM_IMPALED) != 0)
    {

        int thisFrame;
        int lastFrame;
        thisFrame = G2EmulationInstanceQueryFrame(instance, 0);
        lastFrame = G2EmulationInstanceQueryLastFrame(instance, 0);

        if (lastFrame < ma->bloodImpaleFrame && thisFrame >= ma->bloodImpaleFrame)
        {
            FX_Blood_Impale(instance, ma->grabSegment, instance, ma->grabSegment);
        }
        else
        {
            if (lastFrame < ma->bloodConeFrame && thisFrame >= ma->bloodConeFrame)
            {
                FX_BloodCone(instance, ma->grabSegment, 0x50);
            }
        }
    }

    if (instance->flags2 & 0x10)
    {
        if (MON_AnimPlaying(instance, MONSTER_ANIM_IMPALED) == 0 || (signed char)mv->subAttr->animList[0xD] == (signed char)mv->subAttr->animList[0x16])
        {
            MON_SwitchState(instance, MONSTER_STATE_DEAD);
        }
        else
        {
            mv->generalTimer = MON_GetTime(instance) + 0x3E8;
        }
    }

    if (mv->generalTimer < MON_GetTime(instance))
    {
        mv->mvFlags &= ~0x200000;
        MON_TurnOnBodySpheres(instance);
        MON_SwitchState(instance, MONSTER_STATE_FALL);
    }

    while ((message = DeMessageQueue(&mv->messageQueue)) != NULL)
    {
        if (message->ID == 0x0100000A)
        {

            mv->generalTimer = MON_GetTime(instance) + 0x7530;
            mv->heldID = mv->held->introUniqueID;
            INSTANCE_Post(mv->held, 0x800002, SetObjectData(0, 0, 0, instance, 3));
            INSTANCE_Post(mv->held, 0x200003, 7);
            mv->causeOfDeath = 0;

            do {} while (0); // garbage code for reordering

            MON_PlayAnim(instance, MONSTER_ANIM_IMPALEDEATH, 1);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/Game/MONSTER/MONSTER", MON_TerrainImpaleDeathEntry);

void MON_TerrainImpaleDeath(Instance *instance)
{
    MonsterVars *mv;
    MonsterAttributes *ma;
    Intro *impaler;
    int temp; // not from decls.h

    mv = (MonsterVars *)instance->extraData;

    if (!(mv->mvFlags & 0x200))
    {
        MON_MoveInstanceToImpalePoint(instance);

        if ((instance->flags2 & 0x10))
        {
            if ((mv->enemy != NULL) && ((INSTANCE_Query(mv->enemy->instance, 1) & 0x1)))
            {
                MON_ChangeHumanOpinion(instance);
            }

            mv->mvFlags |= 0x200;

            instance->flags2 &= ~0x40;

            if ((mv->soulID == 0) && (mv->soulJuice != 0))
            {
                MON_BirthSoul(instance, 1);
            }
        }
    }
    else
    {
        if (mv->soulID != 0)
        {
            if (INSTANCE_Find(mv->soulID) == NULL)
            {
                MON_BirthSoul(instance, 1);
            }
        }
        else
        {
            ma = (MonsterAttributes *)instance->data;

            temp = MON_GetTime(instance) - mv->damageTimer;

            MON_CutOut_Monster(instance, temp, ma->headSegment, ma->rightFootSegment);

            if (temp >= 4096)
            {
                impaler = INSTANCE_FindIntro(instance->currentStreamUnitID, mv->terrainImpaleID);

                if (impaler != NULL)
                {
                    impaler->flags |= 0x8000;
                }

                MON_KillMonster(instance);
            }
        }

        if (((mv->mvFlags & 0x400000)) && (mv->effectTimer < MON_GetTime(instance)))
        {
            mv->mvFlags &= ~0x400000;

            FX_StopAllGlowEffects(instance, 64);
        }

        while (DeMessageQueue(&mv->messageQueue) != NULL);
    }
}

void MON_SurprisedEntry(Instance *instance)
{
    MonsterVars *mv;
    mv = (MonsterVars *)instance->extraData;

    do {} while (0); // garbage code for reordering

    MON_PlayAnim(instance, MONSTER_ANIM_SURPRISED, 1);
    mv->generalTimer = MON_GetTime(instance) + mv->subAttr->combatAttributes->surpriseTime;
}

void MON_Surprised(Instance *instance)
{

    MonsterVars *mv;
    mv = instance->extraData;

    if (instance->flags2 & 0x10)
    {
        MON_PlayCombatIdle(instance, 2);
    }

    if (mv->generalTimer < MON_GetTime(instance))
    {
        if (mv->behaviorState == MONSTER_STATE_HIT)
        {
            MON_SwitchState(instance, MONSTER_STATE_HIDE);
        }
        else
        {
            MON_SwitchState(instance, MONSTER_STATE_COMBAT);
        }
    }

    if (mv->enemy != NULL)
    {
        mv->lookAtPos = &mv->enemy->instance->position;
        MON_TurnToPosition(instance, &mv->enemy->instance->position, mv->subAttr->speedPivotTurn);
    }

    MON_DefaultQueueHandler(instance);
}

void MON_StunnedEntry(Instance *instance)
{
    MonsterVars *mv;
    MonsterCombatAttributes *combat;

    mv = (MonsterVars *)instance->extraData;

    combat = mv->subAttr->combatAttributes;

    if ((mv->mvFlags & 0x40))
    {
        mv->enemy->mirConditions |= 0x400;

        mv->mvFlags |= 0x10000;
        mv->mvFlags |= 0x800000;

        if (MON_SetUpKnockBack(instance, mv->enemy->instance, mv->messageData) != 0)
        {
            MON_PlayAnim(instance, MONSTER_ANIM_HIT1, 1);
        }
        else
        {
            MON_PlayAnim(instance, MONSTER_ANIM_HIT2, 1);
        }
    }
    else if (!(mv->mvFlags & 0x100))
    {
        if ((mv->damageType == 0x40) || (mv->damageType == 0x2000))
        {
            MON_PlayAnim(instance, MONSTER_ANIM_BLIND, 2);
        }
        else if (mv->damageType == 0x10)
        {
            MON_PlayAnim(instance, MONSTER_ANIM_AGONY, 2);
        }
        else if (mv->damageType == 0x200)
        {
            MON_PlayAnim(instance, MONSTER_ANIM_DEAF, 1);
        }
        else
        {
            MON_PlayAnim(instance, MONSTER_ANIM_STUNNED, 2);
        }
    }

    mv->mvFlags |= 0x10110;

    mv->damageTimer = MON_GetTime(instance) + combat->damageTime;

    if ((mv->damageType == 0x40) && (mv->subAttr->sunVuln != 0))
    {
        mv->stunTimer = MON_GetTime(instance) + (combat->stunTime * 2);
    }
    else
    {
        mv->stunTimer = MON_GetTime(instance) + combat->stunTime;
    }

    mv->mode = 0x8000;

    instance->checkMask |= 0x20;
}

void MON_Stunned(Instance *instance)
{
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    if ((instance->flags2 & 0x10))
    {
        if (!(mv->mvFlags & 0x100))
        {
            MON_SwitchState(instance, MONSTER_STATE_COMBAT);
        }
        else
        {
            MON_PlayAnim(instance, MONSTER_ANIM_STUNNED, 1);
        }
    }

    if (mv->generalTimer < MON_GetTime(instance))
    {
        instance->xVel = 0;
        instance->yVel = 0;
        instance->zVel = 0;
    }

    if ((!(mv->mvFlags & 0x100)) && (MON_AnimPlaying(instance, MONSTER_ANIM_STUNNED_RECOVERY) == 0))
    {
        MON_PlayAnim(instance, MONSTER_ANIM_STUNNED_RECOVERY, 1);
    }

    MON_DefaultQueueHandler(instance);

    PHYSICS_StopIfCloseToTarget(instance, 0, 0, 0);

    PhysicsMove(instance, &instance->position, gameTrackerX.timeMult);

    if (instance->currentMainState != MONSTER_STATE_STUNNED)
    {
        instance->checkMask &= ~0x20;
    }
}

void MON_GrabbedEntry(Instance *instance)
{

    MonsterVars *mv;
    mv = (MonsterVars *)instance->extraData;

    if ((signed char)mv->subAttr->animList[4] == (signed char)mv->subAttr->animList[0xE])
    {
        MON_PlayAnim(instance, MONSTER_ANIM_GRABBED, 2);
    }
    else
    {
        MON_PlayAnim(instance, MONSTER_ANIM_GRABBED, 1);
    }

    instance->flags2 &= ~0x40;
    mv->generalTimer = MON_GetTime(instance) + mv->subAttr->combatAttributes->grabTime;
    INSTANCE_LinkToParent(instance, gameTrackerX.playerInstance, 0x31);
    instance->rotation.z = (mv->enemy->instance->rotation.z + 0x800);
    mv->speed = 0;
    instance->checkMask |= 0x20;

}

void MON_Grabbed(Instance *instance)
{
    MonsterVars *mv;
    Message *message;
    int time;

    mv = (MonsterVars *)instance->extraData;

    if ((instance->flags2 & 0x10))
    {
        MON_PlayAnim(instance, MONSTER_ANIM_STRUGGLE, 2);
    }

    time = mv->generalTimer - MON_GetTime(instance);

    if ((time <= 0) && (INSTANCE_Query(mv->enemy->instance, 10) != 0x20000))
    {
        MON_SwitchState(instance, MONSTER_STATE_BREAKHOLD);
    }

    instance->rotation.z = mv->enemy->instance->rotation.z + 2048;

    while ((message = DeMessageQueue(&mv->messageQueue)) != NULL)
    {
        switch (message->ID)
        {
        case 0x1000003:
        {
            evMonsterThrownData *data;

            data = (evMonsterThrownData *)message->Data;

            INSTANCE_UnlinkFromParent(instance);

            MON_LaunchMonster(instance, data->direction.z, data->power, 50);
            break;
        }
        case 0x1000007:
        {
            evMonsterHitTerrainData *data;

            data = (evMonsterHitTerrainData *)message->Data;

            if ((data->bspFlags & 0x2000))
            {
                MON_UnlinkFromRaziel(instance);
            }

            break;
        }
        case 0x1000002:
        case 0x100000B:
            break;
        default:
            MON_DefaultMessageHandler(instance, message);
        }
    }

    if (instance->LinkParent == NULL)
    {
        if ((instance->currentMainState == MONSTER_STATE_GRABBED) && (mv->speed != 0))
        {
            MON_TurnOnBodySpheres(instance);

            MON_SwitchState(instance, MONSTER_STATE_FALL);

            instance->rotation.z = gameTrackerX.playerInstance->rotation.z + 2048;
        }

        instance->rotation.x = 0;
        instance->rotation.y = 0;
    }
    else
    {
        mv->speed = 1;
    }

    if ((instance->LinkParent != NULL) && ((instance->currentMainState != MONSTER_STATE_GRABBED) && (instance->currentMainState != MONSTER_STATE_BREAKHOLD)))
    {
        MON_UnlinkFromRaziel(instance);
    }
}

void MON_HitEntry(Instance *instance)
{

    MonsterIR *enemy;
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;
    enemy = mv->enemy;

    enemy->mirConditions |= 0x400;
    mv->mvFlags |= 0x10000;

    if (MON_SetUpKnockBack(instance, enemy->instance, (evMonsterHitData *)mv->messageData) != 0)
    {
        MON_PlayAnim(instance, MONSTER_ANIM_HIT1, 1);
    }
    else
    {
        MON_PlayAnim(instance, MONSTER_ANIM_HIT2, 1);
    }

    mv->mode = 0x8000;
    instance->checkMask |= 0x20;
    mv->generalTimer2 = MON_GetTime(instance) + 0x26AC;
}

void MON_Hit(Instance *instance)
{

    MonsterVars *mv;
    mv = (MonsterVars *)instance->extraData;

    if (mv->generalTimer < MON_GetTime(instance))
    {
        if (mv->mvFlags & 0x100)
        {
            MON_SwitchState(instance, MONSTER_STATE_STUNNED);
        }
        else
        {
            MON_SwitchState(instance, MONSTER_STATE_COMBAT);
        }

        instance->xVel = 0;
        instance->yVel = 0;
        instance->zVel = 0;

    }
    else if (instance->flags2 & 0x10)
    {
        MON_PlayCombatIdle(instance, 2);
    }

    MON_DefaultQueueHandler(instance);
    PHYSICS_StopIfCloseToTarget(instance, 0, 0, 0);
    PhysicsMove(instance, &instance->position, gameTrackerX.timeMult);

    if (instance->currentMainState != MONSTER_STATE_HIT)
    {
        instance->checkMask &= ~0x20;
    }
}

void MON_AttackEntry(Instance *instance)
{
    MonsterVars *mv;
    MonsterAttackAttributes *attack;

    mv = (MonsterVars *)instance->extraData;
    attack = mv->attackType;

    do {} while (0); // garbage code for reordering

    mv->mode = 0x200000;

    MON_PlayAnimFromList(instance, attack->animList, 0, 1);
    mv->generalTimer = MON_GetTime(instance) + ((signed char)attack->turnFrames * 0x21);
}


void MON_Attack(Instance *instance)
{

    MonsterAnim *anim;
    MonsterAttackAttributes *attack;
    MonsterVars *mv;
    MonsterIR *enemy;

    mv = (MonsterVars *)instance->extraData;
    attack = mv->attackType;
    enemy = mv->enemy;
    anim = MON_GetAnim(instance, attack->animList, (signed char)mv->attackState);

    if (anim->velocity != 0 && MON_TransNodeAnimation(instance) == 0)
    {
        mv->speed = anim->velocity;
        MON_MoveForward(instance);
    }

    if (MON_AnimPlayingFromList(instance, attack->animList, (signed char)attack->sphereOnAnim) != 0 &&
        G2EmulationInstanceQueryPassedFrame(instance, 0, (signed char)attack->sphereOnFrame) != 0)
    {
        MON_TurnOnWeaponSphere(instance, (signed char)attack->sphereSegment);
    }
    else if (MON_AnimPlayingFromList(instance, attack->animList, (signed char)attack->sphereOffAnim) != 0 &&
             G2EmulationInstanceQueryPassedFrame(instance, 0, (signed char)attack->sphereOffFrame) != 0 &&
             mv->mvFlags & 0x4000)
    {
        MON_TurnOffWeaponSpheres(instance);
        enemy->mirConditions |= 0x200;
    }

    if (instance->flags2 & 0x10)
    {
        instance->flags2 &= ~0x10;
        mv->attackState++;

        if ((signed char)mv->attackState < (signed char)attack->numAnims)
        {
            MON_PlayAnimFromList(instance, attack->animList, (signed char)mv->attackState, 1);
        }
        else
        {
            MON_SwitchState(instance, MONSTER_STATE_COMBAT);
        }
    }

    if (enemy != NULL && (MON_GetTime(instance) < (unsigned int)mv->generalTimer || !(INSTANCE_Query(enemy->instance, 0xA) & 0x02000000)))
    {
        MON_TurnToPosition(instance, &enemy->instance->position, mv->subAttr->speedPivotTurn);
    }

    MON_DefaultQueueHandler(instance);
}

void MON_CombatEntry(Instance *instance)
{
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    mv->mode = 0x2000000;

    mv->mvFlags &= ~0x40000;
    mv->mvFlags |= 0x10000;
}

void MON_Combat(Instance *instance)
{
    MonsterVars *mv;
    MonsterIR *enemy;

    mv = (MonsterVars *)instance->extraData;

    enemy = mv->enemy;

    if ((enemy == NULL) || ((mv->mvFlags & 0x4)))
    {
        MON_SwitchState(instance, MONSTER_STATE_IDLE);

        MON_IdleQueueHandler(instance);
    }
    else if (((mv->mvFlags & 0x10000000)) && ((enemy->mirFlags & 0x1000)))
    {
        MON_SwitchState(instance, MONSTER_STATE_EMBRACE);

        MON_IdleQueueHandler(instance);
    }
    else if ((MON_ValidPosition(instance) == 0) && (MON_GetTime(instance) >= mv->generalTimer2))
    {
        MON_SwitchState(instance, MONSTER_STATE_PURSUE);

        MON_IdleQueueHandler(instance);
    }
    else
    {
        MonsterCombatAttributes *combat;

        combat = mv->subAttr->combatAttributes;

        mv->lookAtPos = &enemy->instance->position;

        if (MON_ShouldIFlee(instance) != 0)
        {
            MON_SwitchState(instance, MONSTER_STATE_FLEE);
        }
        else if ((enemy->distance < combat->combatRange) || (mv->behaviorState == MONSTER_STATE_GRABBED))
        {
            int reason;

            if (((mv->mvFlags & 0x20000)) && ((instance->flags2 & 0x12)))
            {
                mv->mvFlags &= ~0x20000;
                mv->mvFlags &= ~0x40000000;
            }

            if (MON_ShouldIEvade(instance) != 0)
            {
                int anim;

                anim = MON_ChooseEvadeMove(instance);

                if (anim != MONSTER_ANIM_HIT1)
                {
                    MON_PlayAnim(instance, anim, 1);

                    mv->mvFlags |= 0x20000;
                    mv->mvFlags |= 0x40000000;
                }
            }
            else if (!(mv->mvFlags & 0x40000000))
            {
                if (mv->behaviorState == MONSTER_STATE_GRABBED)
                {
                    reason = MON_ShouldIFireAtTarget(instance, enemy);
                }
                else
                {
                    reason = MON_ShouldIAttack(instance, enemy, MON_ChooseAttack(instance, enemy));
                }

                if (reason == 1)
                {
                    MON_SwitchState(instance, MONSTER_STATE_ATTACK);
                }
                else if (reason == 2)
                {
                    MON_SwitchState(instance, MONSTER_STATE_PROJECTILE);
                }
                else if (!(mv->mvFlags & 0x20000))
                {
                    int anim;

                    anim = MON_ChooseCombatMove(instance, reason);

                    if (anim != MONSTER_ANIM_HIT1)
                    {
                        MON_PlayAnim(instance, anim, 1);

                        mv->mvFlags |= 0x20000;
                    }
                    else
                    {
                        MON_PlayCombatIdle(instance, 2);
                    }
                }
            }

            MON_TurnToPosition(instance, &enemy->instance->position, mv->subAttr->speedPivotTurn);
        }
        else
        {
            int state;

            state = MONSTER_STATE_PURSUE;

            if (mv->behaviorState == MONSTER_STATE_STUNNED)
            {
                state = MONSTER_STATE_WANDER;
            }
            else if (((mv->mvFlags & 0x2000000)) && (mv->ally == NULL))
            {
                state = MONSTER_STATE_FLEE;
            }

            MON_SwitchState(instance, state);
        }

        MON_IdleQueueHandler(instance);
    }
}

void MON_ProjectileEntry(Instance *instance)
{
    MonsterVars *mv;
    MonsterAttributes *ma;
    MonsterMissile *missileDef;

    mv = (MonsterVars *)instance->extraData;

    ma = (MonsterAttributes *)instance->data;

    missileDef = &ma->missileList[(int)mv->subAttr->combatAttributes->missileAttack];

    do
    {
    } while (0); // garbage code for reordering

    MON_PlayAnimFromList(instance, missileDef->animList, 0, 1);

    mv->attackState = 0;
}

void MON_Projectile(Instance *instance)
{
    MonsterVars *mv;
    MonsterAttributes *ma;
    MonsterMissile *missileDef;

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    missileDef = &ma->missileList[(signed char)mv->subAttr->combatAttributes->missileAttack];

    if (mv->enemy == NULL)
    {
        MON_SwitchState(instance, MONSTER_STATE_COMBAT);
    }
    else if (instance->flags2 & 0x10)
    {
        mv->attackState += 1;
        if ((signed char)mv->attackState < missileDef->numAnims)
        {
            MON_PlayAnimFromList(instance, missileDef->animList, (signed char)mv->attackState, 1);
        }
        else
        {
            MON_SwitchState(instance, MONSTER_STATE_COMBAT);
        }
    }
    else
    {
        if (MON_AnimPlayingFromList(instance, missileDef->animList, missileDef->anim) != 0 &&
            G2EmulationInstanceQueryPassedFrame(instance, 0, missileDef->frame) != 0)
        {
            MISSILE_FireAtInstance(instance, missileDef, mv->enemy->instance);
            if ((signed char)missileDef->reload != 0)
            {
                mv->mvFlags &= ~0x20;
            }
        }
        MON_TurnToPosition(instance, &mv->enemy->instance->position, mv->subAttr->speedPivotTurn);
    }

    MON_DefaultQueueHandler(instance);
}

void MON_IdleEntry(Instance *instance)
{
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    mv->mvFlags &= ~0x1000;
    mv->mvFlags &= ~0x40000;

    mv->mode = 1;

    if ((mv->mvFlags & 0x4))
    {
        MonsterAttributes *ma;

        ma = (MonsterAttributes *)instance->data;

        if (mv->enemy != NULL)
        {
            MON_PlayCombatIdle(instance, 2);
        }
        else
        {
            MON_PlayAnimID(instance, ma->idleList->anim, 2);
        }
    }
    else if (!(instance->anim.section->flags & 0x1))
    {
        G2Anim_SetLooping(&instance->anim);

        mv->mvFlags |= 0x4000000;
    }
}

void MON_Idle(Instance *instance)
{
    MonsterVars *mv;
    int combatIdle;

    mv = (MonsterVars *)instance->extraData;

    combatIdle = 0;

    if (!(mv->mvFlags & 0x4))
    {
        int state;
        MonsterIR *enemy;

        enemy = mv->enemy;

        state = -1;

        if (MON_ValidPosition(instance) == 0)
        {
            state = 1;
        }
        else if (enemy != NULL)
        {
            combatIdle = 1;

            if (MON_ShouldIFlee(instance) != 0)
            {
                state = 19;
            }
            else if ((enemy->mirFlags & 0x100))
            {
                state = 25;
            }
            else if (((mv->behaviorState == 7) && (enemy->distance > mv->guardRange)) || (MON_ValidUnit(instance, enemy->instance->currentStreamUnitID) == 0))
            {
                MON_TurnToPosition(instance, (Position *)&enemy->instance->position.x, mv->subAttr->speedPivotTurn);
            }
            else
            {
                state = 13;
            }

            mv->lookAtPos = &enemy->instance->position;
        }
        else if (((mv->behaviorState == 2) || (mv->behaviorState == 3)) && ((instance->flags2 & 0x2)))
        {
            state = 5;
        }

        if (state != -1)
        {
            MON_SwitchState(instance, state);
        }
    }
    else if ((mv->auxFlags & 0x10000000))
    {
        AngleMoveToward(&instance->rotation.z, mv->destination.z, (short)((mv->subAttr->speedPivotTurn * gameTrackerX.timeMult) / 4096));

        if (instance->rotation.z == mv->destination.z)
        {
            mv->auxFlags &= ~0x10000000;
        }
    }

    if ((!(mv->mvFlags & 0x4)) && ((!(mv->mvFlags & 0x4000000)) || ((instance->flags2 & 0x2))))
    {
        if (combatIdle != 0)
        {
            MON_PlayCombatIdle(instance, 2);
        }
        else
        {
            MON_PlayRandomIdle(instance, 2);
        }
    }

    MON_IdleQueueHandler(instance);
}

void MON_FleeEntry(Instance *instance)
{
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    mv->mode = 4;

    mv->mvFlags |= 0x1000;
    mv->mvFlags &= ~0x10000;
    mv->mvFlags &= ~0x20000;

    MON_PlayAnim(instance, MONSTER_ANIM_RUN, 2);
}

void MON_Flee(Instance *instance)
{
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    if (mv->enemy == NULL)
    {
        MON_SwitchState(instance, MONSTER_STATE_IDLE);

        mv->behaviorState = mv->initialBehavior;
        return;
    }

    if (((mv->mvFlags & 0x2000000)) && (mv->ally != NULL) && (mv->ally->distance < mv->enemy->distance))
    {
        if (!(INSTANCE_Query(instance, 1) & 0x4))
        {
            MON_SwitchState(instance, MONSTER_STATE_PURSUE);
            return;
        }
    }

    if (mv->enemy->distance > mv->subAttr->fleeRange)
    {
        MON_SwitchState(instance, MONSTER_STATE_WANDER);
        return;
    }

    if (MON_ValidPosition(instance) == 0)
    {
        MON_SwitchState(instance, MONSTER_STATE_PURSUE);
        return;
    }

    AngleMoveToward(&instance->rotation.z, MONSENSE_GetClosestFreeDirection(instance, MATH3D_AngleFromPosToPos(&mv->enemy->instance->position, &instance->position), 1000), mv->subAttr->speedFleeTurn);

    MON_DefaultQueueHandler(instance);
}

void MON_PursueEntry(Instance *instance)
{
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    mv->mvFlags |= 0x1000;
    mv->mvFlags &= ~0x10000;

    if ((mv->mvFlags & 0x4))
    {
        MON_PlayAnim(instance, MONSTER_ANIM_RUN, 2);
    }
    else
    {
        MON_GetPlanSlot(mv);

        MON_PlayCombatIdle(instance, 2);
    }

    mv->mode = 4;

    mv->mvFlags &= ~0x20000;
}

INCLUDE_ASM("asm/nonmatchings/Game/MONSTER/MONSTER", MON_Pursue);

void MON_WanderEntry(Instance *instance)
{
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    mv->mvFlags |= 0x11000;

    if ((mv->mvFlags & 0x4))
    {
        MON_PlayAnim(instance, MONSTER_ANIM_WALK, 2);
    }
    else
    {
        MON_GetPlanSlot(mv);

        if (MON_AnimPlaying(instance, MONSTER_ANIM_WALK) == 0)
        {
            MON_PlayRandomIdle(instance, 2);
        }
    }

    mv->mvFlags &= ~0x20000;

    do
    {
    } while (0); // garbage code for reordering

    mv->generalTimer = MON_GetTime(instance) + 1000;
}

void MON_Wander(Instance *instance)
{
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    if ((mv->mvFlags & 0x4))
    {
        MON_TurnToPosition(instance, &mv->destination, mv->subAttr->speedWalkTurn);

        if (MATH3D_LengthXYZ(instance->position.x - mv->destination.x, instance->position.y - mv->destination.y, instance->position.z - mv->destination.z) < 300)
        {
            MON_SwitchState(instance, MONSTER_STATE_IDLE);
        }

        MON_GroundMoveQueueHandler(instance);
    }
    else if (!(mv->mvFlags & 0x40000))
    {
        if (mv->generalTimer < MON_GetTime(instance))
        {
            MON_SwitchState(instance, MONSTER_STATE_IDLE);
        }
        else if (mv->behaviorState == 3)
        {
            if (mv->currentMarker != NULL)
            {
                int markerID;

                markerID = *mv->currentMarker;

                mv->currentMarker++;

                PLANAPI_FindNodePositionInUnit(STREAM_GetStreamUnitWithID(instance->currentStreamUnitID), &mv->destination, markerID, 0);

                if (*mv->currentMarker == 0)
                {
                    mv->currentMarker = mv->patrolMarkers;
                }

                mv->mvFlags |= 0x40000;
            }
        }
        else
        {
            Position *target;
            short range;

            range = mv->wanderRange;

            target = NULL;

            if (mv->leader != NULL)
            {
                range = 800;

                target = &mv->leader->instance->position;
            }
            else if (instance->intro != NULL)
            {
                target = &instance->intro->position;
            }

            MON_GetRandomDestinationInWorld(instance, target, range);
        }

        MON_GroundMoveQueueHandler(instance);
    }
    else
    {
        int planresult;
        int state;

        planresult = MON_DefaultPlanMovement(instance, 2, 100);

        state = -1;

        switch (planresult)
        {
        case 0:
        case 1:
        case 5:
        case 6:
            break;
        case 4:
            if (mv->behaviorState != 4)
            {
                if (!(rand() & 0x3))
                {
                    mv->generalTimer = MON_GetTime(instance) + 1000;

                    state = 5;
                }
                else
                {
                    state = 2;
                }
            }
            else
            {
                state = 20;
            }

            break;
        case 2:
        case 3:
            state = 2;
            break;
        }

        if (state != -1)
        {
            MON_SwitchState(instance, state);
        }
    }

    if (mv->enemy != NULL)
    {
        if (MON_ShouldIFlee(instance) != 0)
        {
            MON_SwitchState(instance, MONSTER_STATE_FLEE);
        }

        mv->lookAtPos = &mv->enemy->instance->position;
    }
}

void MON_HideEntry(Instance *instance)
{
    MON_PlayRandomIdle(instance, 1);
}

void MON_Hide(Instance *instance)
{
    MonsterVars *mv;
    MonsterIR *enemy;

    mv = (MonsterVars *)instance->extraData;

    enemy = mv->enemy;

    if ((!(mv->mvFlags & 0x4)) && (enemy != NULL))
    {
        switch (mv->behaviorState)
        {
        case 4:
            MON_ChangeBehavior(instance, mv->triggeredBehavior);
            break;
        case 9:
            MON_SwitchState(instance, MONSTER_STATE_FLEE);
            break;
        case 8:
            if ((!(mv->mvFlags & 0x40000000)) && (MON_ShouldIAmbushEnemy(instance) != 0))
            {
                if (mv->ambushJumpType == 0)
                {
                    MON_TurnToPosition(instance, &enemy->instance->position, 4096);

                    MON_SwitchState(instance, MONSTER_STATE_SURPRISEATTACK);
                }
                else
                {
                    if (mv->ambushJumpType == 1)
                    {
                        MON_PlayAnim(instance, MONSTER_ANIM_EVADELEFT, 1);
                    }
                    else
                    {
                        MON_PlayAnim(instance, MONSTER_ANIM_EVADERIGHT, 1);
                    }

                    mv->mvFlags |= 0x40000000;
                }
            }

            break;
        default:
            MON_SwitchState(instance, MONSTER_STATE_PURSUE);
        }
    }

    if ((instance->flags2 & 0x10))
    {
        if ((mv->mvFlags & 0x40000000))
        {
            MON_SwitchState(instance, MONSTER_STATE_COMBAT);
        }
        else
        {
            MON_PlayRandomIdle(instance, 1);
        }
    }

    MON_DefaultQueueHandler(instance);
}

void MON_SurpriseAttackEntry(Instance *instance)
{
    MonsterVars *mv;
    MonsterCombatAttributes *combatAttr;
    MonsterAttributes *ma;

    mv = (MonsterVars *)instance->extraData;

    combatAttr = mv->subAttr->combatAttributes;

    ma = (MonsterAttributes *)instance->data;

    do
    {
    } while (0); // garbage code for reordering

    mv->attackType = &ma->attackAttributesList[(int)combatAttr->ambushAttack];

    mv->attackState = 0;

    MON_PlayAnimFromList(instance, mv->attackType->animList, 0, 1);

    if (mv->behaviorState == 8)
    {
        if (mv->activeBehavior != -1)
        {
            mv->behaviorState = mv->activeBehavior;
        }
        else
        {
            mv->behaviorState = mv->initialBehavior;
        }
    }
}

void MON_SurpriseAttack(Instance *instance)
{
    MON_Attack(instance);
}

void MON_EnvironmentDamageEntry(Instance *instance)
{
    (void)instance;
}

void MON_EnvironmentDamage(Instance *instance)
{
    (void)instance;
}

void MON_MonsterGlow(Instance *instance, long color, int glowtime, int glowin, int glowfade)
{
    FXGlowEffect *glow;
    MonsterAttributes *ma;

    ma = (MonsterAttributes *)instance->data;

    if (glowtime > 0)
    {
        glowtime *= 33;
    }

    if (glowfade > 0)
    {
        glowfade *= 33;
    }

    if (glowin > 0)
    {
        glowin *= 33;
    }

    glow = FX_DoInstanceTwoSegmentGlow(instance, ma->waistSegment, ma->leftFootSegment, &color, 1, 1024, 160);

    glow->lifeTime = glowtime;

    glow->fadein_time = glowin;
    glow->fadeout_time = glowfade;

    glow = FX_DoInstanceTwoSegmentGlow(instance, ma->waistSegment, ma->rightFootSegment, &color, 1, 1024, 160);

    glow->lifeTime = glowtime;

    glow->fadein_time = glowin;
    glow->fadeout_time = glowfade;

    glow = FX_DoInstanceTwoSegmentGlow(instance, ma->leftShoulderSegment, ma->leftWeaponSegment, &color, 1, 1024, 128);

    glow->lifeTime = glowtime;

    glow->fadein_time = glowin;
    glow->fadeout_time = glowfade;

    glow = FX_DoInstanceTwoSegmentGlow(instance, ma->rightShoulderSegment, ma->rightWeaponSegment, &color, 1, 1024, 128);

    glow->lifeTime = glowtime;

    glow->fadein_time = glowin;
    glow->fadeout_time = glowfade;

    glow = FX_DoInstanceTwoSegmentGlow(instance, ma->headSegment, ma->waistSegment, &color, 1, 1024, 192);

    glow->lifeTime = glowtime;

    glow->fadein_time = glowin;
    glow->fadeout_time = glowfade;
}

void MON_GeneralDeathEntry(Instance *instance)
{
    MonsterVars *mv;
    MonsterAttributes *ma;
    char temp; // not from decls.h

    temp = 0;

    mv = (MonsterVars *)instance->extraData;

    ma = (MonsterAttributes *)instance->data;

    if (instance->LinkParent != NULL)
    {
        MON_UnlinkFromRaziel(instance);

        instance->xAccl = 0;
        instance->yAccl = 0;
        instance->zAccl = -16;
    }

    instance->flags2 |= 0x80;

    mv->mvFlags |= 0x202000;
    mv->mvFlags &= ~0x10;

    switch (mv->damageType)
    {
    case 16:
        mv->effect = (void *)SOUND_Play3dSound(&instance->position, 35, 600, 80, 10000);

        MON_PlayAnim(instance, MONSTER_ANIM_AGONY, 2);

        mv->causeOfDeath = 3;

        mv->generalTimer = MON_GetTime(instance) + 3000;
        mv->effectTimer = MON_GetTime(instance) + 12000;

        instance->xAccl = 0;
        instance->yAccl = 0;

        instance->xVel = 0;
        instance->yVel = 0;
        break;
    case 32:
        mv->causeOfDeath = 1;
    case 64:
        if ((mv != NULL) && (mv->mvFlags != 0)) // garbage code for reordering
        {
            temp = -temp;
        }

        if ((ma->whatAmI & 0x8))
        {
            MON_PlayAnim(instance, MONSTER_ANIM_FALLOVER, 1);
        }
        else
        {
            MON_PlayAnim(instance, MONSTER_ANIM_AGONY, 2);
        }

        if (mv->damageType == 64)
        {
            mv->causeOfDeath = 2;
        }

        mv->generalTimer = MON_GetTime(instance) + 2000;

        mv->mvFlags |= 0x400000;

        mv->effectTimer = MON_GetTime(instance) + 10000;

        MON_MonsterGlow(instance, 18784, -1, 0, 0);

        instance->xAccl = 0;
        instance->yAccl = 0;

        instance->xVel = 0;
        instance->yVel = 0;
        break;
    case 1024:
        MON_PlayAnim(instance, MONSTER_ANIM_FALLOVER, 1);

        mv->causeOfDeath = 6;

        mv->generalTimer = 0;

        instance->xAccl = 0;
        instance->yAccl = 0;

        instance->xVel = 0;
        instance->yVel = 0;
        break;
    default:
        if (mv->enemy != NULL)
        {
            if (MON_SetUpKnockBack(instance, mv->enemy->instance, (evMonsterHitData *)mv->messageData) != 0)
            {
                MON_PlayAnim(instance, MONSTER_ANIM_HIT1, 1);
            }
            else
            {
                MON_PlayAnim(instance, MONSTER_ANIM_HIT2, 1);
            }
        }
        else
        {
            MON_PlayAnim(instance, MONSTER_ANIM_FALLOVER, 1);
        }

        mv->generalTimer = 0;

        mv->causeOfDeath = 7;

        MON_TurnOffAllSpheres(instance);

        instance->xAccl = 0;
        instance->yAccl = 0;

        instance->xVel = 0;
        instance->yVel = 0;
    }

    MON_DropAllObjects(instance);
}

void MON_GeneralDeath(Instance *instance)
{
    MonsterVars *mv;
    Message *message;
    int dead;

    mv = (MonsterVars *)instance->extraData;

    dead = 0;

    if ((((instance->flags2 & 0x10)) && (MON_AnimPlaying(instance, MONSTER_ANIM_FALLOVER) != 0)) || (mv->causeOfDeath == 6))
    {
        dead = 1;
    }
    else if (((instance->flags2 & 0x12)) && (mv->generalTimer < MON_GetTime(instance)))
    {
        MON_PlayAnim(instance, MONSTER_ANIM_FALLOVER, 1);

        MON_TurnOffAllSpheres(instance);
    }

    if (dead != 0)
    {
        mv->mvFlags &= ~0x10;

        G2Anim_SetNoLooping(&instance->anim);

        MON_SwitchState(instance, MONSTER_STATE_DEAD);
    }

    if ((mv->mvFlags & 0x400))
    {
        if (MON_OnGround(instance) != 0)
        {
            mv->mvFlags |= 0x2;
        }
        else
        {
            mv->mvFlags &= ~0x2;
        }
    }

    if (!(mv->mvFlags & 0x2))
    {
        MON_ApplyPhysics(instance);
    }

    while ((message = DeMessageQueue(&mv->messageQueue)) != NULL)
    {
        if (message->ID == 0x100000B)
        {
            instance->xAccl = 0;
            instance->yAccl = 0;
            instance->zAccl = -16;
        }
    }
}

void MON_NoticeEntry(Instance *instance)
{
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    if (((long)mv->auxFlags < 0) && (mv->subAttr->allegiances->allies != 0))
    {
        MON_PlayAnim(instance, MONSTER_ANIM_ALARM, 1);

        INSTANCE_Broadcast(instance, mv->subAttr->allegiances->allies, 0x1000011, SetMonsterAlarmData(mv->enemy->instance, &mv->enemy->instance->position, 2));
    }
    else
    {
        MON_PlayAnim(instance, MONSTER_ANIM_NOTICE, 1);
    }
}

void MON_Notice(Instance *instance)
{
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    if ((instance->flags2 & 0x10))
    {
        if (mv->behaviorState == 8)
        {
            MON_SwitchState(instance, MONSTER_STATE_HIDE);
        }
        else
        {
            MON_SwitchState(instance, MONSTER_STATE_COMBAT);
        }
    }

    if (mv->enemy != NULL)
    {
        mv->lookAtPos = &mv->enemy->instance->position;

        MON_TurnToPosition(instance, &mv->enemy->instance->position, mv->subAttr->speedPivotTurn);
    }

    MON_DefaultQueueHandler(instance);
}

void MON_PupateEntry(Instance *instance)
{
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    instance->flags |= 0x800;
    instance->flags2 |= 0x20000000;

    mv->effectTimer = MON_GetTime(instance) + 2000 + ((rand() & 0xFFF));
}

INCLUDE_ASM("asm/nonmatchings/Game/MONSTER/MONSTER", MON_Pupate);

void MON_EmbraceEntry(Instance *instance)
{
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    MON_PlayAnim(instance, MONSTER_ANIM_SOULSUCK, 2);

    mv->generalTimer = MON_GetTime(instance) + mv->subAttr->combatAttributes->suckTime;
}

void MON_Embrace(Instance *instance)
{
    MonsterVars *mv;
    MonsterIR *enemy;
    Instance *ei;

    mv = (MonsterVars *)instance->extraData;

    enemy = mv->enemy;

    if (enemy != NULL)
    {
        if ((enemy->distance > mv->subAttr->combatAttributes->suckRange) || (!(enemy->mirFlags & 0x20)) || (!(enemy->mirFlags & 0x1000)) || ((mv->mvFlags & 0x4)) || (mv->generalTimer < MON_GetTime(instance)))
        {
            MON_SwitchState(instance, MONSTER_STATE_COMBAT);
        }
        else
        {
            ei = enemy->instance;

            MON_DoDrainEffects(instance, ei);

            INSTANCE_Post(ei, 0x40006, mv->subAttr->combatAttributes->suckPower << 8);
            INSTANCE_Post(instance, 0x1000016, mv->subAttr->combatAttributes->suckPower / 8);

            MON_TurnToPosition(instance, &ei->position, mv->subAttr->speedPivotTurn);
        }
    }
    else
    {
        MON_SwitchState(instance, MONSTER_STATE_IDLE);
    }

    MON_DefaultQueueHandler(instance);

    if ((enemy != NULL) && (instance->currentMainState != MONSTER_STATE_EMBRACE))
    {
        enemy->mirFlags &= ~0x1000;
    }
}

void MON_PetrifiedEntry(Instance *instance)
{
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    mv->generalTimer = MON_GetTime(instance) + 7000;

    mv->mvFlags |= 0x80;
}

void MON_Petrified(Instance *instance)
{
    MonsterVars *mv;
    Message *message;
    int time;

    mv = (MonsterVars *)instance->extraData;

    time = MON_GetTime(instance);

    if (mv->generalTimer < (unsigned int)time)
    {
        mv->mvFlags &= ~0x80;

        instance->petrifyValue = 0;

        MON_SwitchState(instance, MONSTER_STATE_IDLE);
    }
    else if ((mv->generalTimer - 1000) < (unsigned int)time)
    {
        instance->petrifyValue = (short)(4 * (mv->generalTimer - (unsigned int)time));
    }

    while ((message = DeMessageQueue(&mv->messageQueue)) != NULL)
    {
        if ((message->ID == 0x100001F) || ((message->ID == 0x1000023) && (message->Data == 0x1000)))
        {
            mv->damageType = 0x400;

            MON_SwitchState(instance, MONSTER_STATE_GENERALDEATH);
        }
    }
}

int MONSTER_CalcDamageIntensity(int hp, int maxHp)
{
    return ((maxHp - hp) * 256) / maxHp;
}

INCLUDE_ASM("asm/nonmatchings/Game/MONSTER/MONSTER", MONSTER_ProcessClosestVerts);

void ProcessBloodyMess(Instance *instance, int vertidx, int segidx, int dist, void *cb_data)
{

    long scl;
    struct CVECTOR *cv;
    MonsterVars *mv;

    mv = (MonsterVars *)instance->extraData;

    if (dist < ((bloodyMessType *)cb_data)->closestdist)
    {
        ((bloodyMessType *)cb_data)->closestvert = vertidx;
        ((bloodyMessType *)cb_data)->closestdist = dist;
        ((bloodyMessType *)cb_data)->closestseg = segidx;
    }

    if (dist < 0x64)
    {

        cv = &instance->perVertexColor[vertidx];

        scl = (((0x64 - dist) * 2) * (((bloodyMessType *)cb_data)->bloodIntensity)) / 100;
        scl = MIN(scl, 0x100);

        cv->r = ~(((0xFF - mv->subAttr->bruiseRed) * scl) >> 8);
        cv->g = ~(((0xFF - mv->subAttr->bruiseGreen) * scl) >> 8);
        cv->b = ~(((0xFF - mv->subAttr->bruiseBlue) * scl) >> 8);
        cv->cd = 1;

        ((bloodyMessType *)cb_data)->bloodiedAVert = 1;
    }
}

void MONSTER_InitVertexColors(Instance *instance, Model *model)
{
    int i;
    CVECTOR *ptr;

    instance->perVertexColor = (CVECTOR *)MEMPACK_Malloc(model->numVertices * 4, 33);

    for (ptr = instance->perVertexColor, i = model->numVertices; i != 0; i--, ptr++)
    {
        *(long *)ptr = 0xFFFFFF;
    }
}

int MONSTER_StartVertexBlood(Instance *instance, SVector *location, int amount)
{
    Model *model;
    bloodyMessType bmt;
    SVector localloc;
    MVertex *vertexList;
    Vector locVec;
    long flag;
    MATRIX *segMatrix;

    model = instance->object->modelList[instance->currentModel];

    if ((instance->perVertexColor != NULL) || (MONSTER_InitVertexColors(instance, model), instance->perVertexColor != NULL))
    {
        localloc = *location;

        bmt.closestvert = -1;
        bmt.closestdist = 65536;
        bmt.closestseg = -1;

        bmt.bloodiedAVert = 0;

        bmt.bloodIntensity = amount;

        MONSTER_ProcessClosestVerts(instance, &localloc, ProcessBloodyMess, &bmt.closestvert);

        if (bmt.closestdist >= 51)
        {
            vertexList = model->vertexList;

            segMatrix = &instance->matrix[bmt.closestseg];

            SetRotMatrix(segMatrix);
            SetTransMatrix(segMatrix);

            RotTrans((SVECTOR *)&vertexList[bmt.closestvert].vertex, (VECTOR *)&locVec, &flag);

            bmt.bloodiedAVert = 1;

            bmt.closestdist = 0;

            localloc.x = (short)locVec.x;
            localloc.y = (short)locVec.y;
            localloc.z = (short)locVec.z;

            FX_MakeHitFX(&localloc);

            MONSTER_ProcessClosestVerts(instance, &localloc, ProcessBloodyMess, &bmt);
        }
        else
        {
            FX_MakeHitFX(&localloc);
        }

        return bmt.closestvert;
    }

    return -1;
}

INCLUDE_ASM("asm/nonmatchings/Game/MONSTER/MONSTER", MONSTER_VertexBlood);

void ProcessBurntMess(Instance *instance, int vertidx, int segidx, int dist, void *cb_data)
{

    short scl;
    burntTuneType *burntTune;

    burntTune = ((burntMessType *)cb_data)->burntTune;

    if (dist < ((burntMessType *)cb_data)->closestdist)
    {
        ((burntMessType *)cb_data)->closestvert = vertidx;
        ((burntMessType *)cb_data)->closestdist = dist;
        ((burntMessType *)cb_data)->closestseg = segidx;
    }

    if (dist >= burntTune->burntDist || (scl = (((dist << 0xC) / burntTune->burntDist) << 0x10) >> 0x14, ((scl < 0xFF) == 0)))
    {
        scl = 0xFE;
    }

    instance->perVertexColor[vertidx].r = scl;
    instance->perVertexColor[vertidx].g = scl;
    instance->perVertexColor[vertidx].b = scl;
    instance->perVertexColor[vertidx].cd = 1;
}

int MONSTER_StartVertexBurnt(Instance *instance, SVector *location, burntTuneType *burntTune)
{
    Model *model;
    burntMessType bmt;
    SVector localloc;
    MVertex *vertexList;
    Vector locVec;
    long flag;
    MATRIX *segMatrix;

    model = instance->object->modelList[instance->currentModel];

    if ((instance->perVertexColor != NULL) || (MONSTER_InitVertexColors(instance, model), instance->perVertexColor != NULL))
    {
        localloc = *location;

        bmt.closestvert = -1;
        bmt.closestdist = 65536;
        bmt.closestseg = -1;

        bmt.burntTune = burntTune;

        MONSTER_ProcessClosestVerts(instance, &localloc, ProcessBurntMess, &bmt);

        if ((burntTune->burntDist >> 1) < bmt.closestdist)
        {
            vertexList = model->vertexList;

            segMatrix = &instance->matrix[bmt.closestseg];

            SetRotMatrix(segMatrix);
            SetTransMatrix(segMatrix);

            RotTrans((SVECTOR *)&vertexList[bmt.closestvert].vertex, (VECTOR *)&locVec, &flag);

            localloc.x = (short)locVec.x;
            localloc.y = (short)locVec.y;
            localloc.z = (short)locVec.z;

            bmt.closestdist = 0;

            FX_MakeHitFX(&localloc);

            MONSTER_ProcessClosestVerts(instance, &localloc, ProcessBurntMess, &bmt);
        }
        else
        {
            FX_MakeHitFX(&localloc);
        }

        return bmt.closestvert;
    }

    return -1;
}

void MONSTER_VertexBurnt(Instance *instance, burntTuneType *burntTune)
{
    int fade;
    Model *model;

    model = (instance->object->modelList[instance->currentModel]);
    fade = burntTune->fadeDelta;

    if (instance->perVertexColor != NULL)
    {
        int i;
        for (i = 0; i < model->numVertices; i++)
        {
            CVECTOR *c;
            c = &instance->perVertexColor[i];

            if (c->r >= fade)
            {
                c->r -= fade;
            }

            if (c->g >= fade)
            {
                c->g -= fade;
            }

            if (c->b >= fade)
            {
                c->b -= fade;
            }

            c->cd = 0;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/Game/MONSTER/MONSTER", MON_DamageEffect);

void MON_DefaultInit(Instance *instance)
{
    MonsterAttributes *ma;
    MonsterVars *mv;
    HModel *hModel;
    HPrim *hprim;
    int i;
    int state;
    Level *level;

    mv = (MonsterVars *)instance->extraData;

    ma = (MonsterAttributes *)instance->data;

    if ((mv->mvFlags & 0x1000000))
    {
        MONAPI_CheckGenerator(instance);
    }

    if (MON_OnGround(instance) != 0)
    {
        mv->mvFlags |= 0x2;
    }
    else
    {
        mv->mvFlags &= ~0x2;
    }

    if (instance->hModelList != NULL)
    {
        hModel = &instance->hModelList[instance->currentModel];

        hprim = (HPrim *)&hModel->hPrimList->hpFlags; // likely a bug from the game itself

        for (i = hModel->numHPrims; i != 0; i--, hprim++)
        {
            if (hprim->segment != 1)
            {
                hprim->withFlags &= 0xA5;
            }
        }
    }

    if (ma->neckSegment != 0)
    {
        G2Anim_AttachControllerToSeg(&instance->anim, ma->neckSegment, 14);

        G2Anim_DisableController(&instance->anim, ma->neckSegment, 14);
    }

    if ((ma->spineSegment != 0) && (ma->spineSegment != ma->neckSegment))
    {
        G2Anim_AttachControllerToSeg(&instance->anim, ma->spineSegment, 14);

        G2Anim_DisableController(&instance->anim, ma->spineSegment, 14);
    }

    if (!(instance->object->oflags & 0x80000))
    {
        level = STREAM_GetLevelWithID(theCamera.focusInstance->currentStreamUnitID);

        if (MATH3D_LengthXYZ(instance->position.x - theCamera.core.position.x, instance->position.y - theCamera.core.position.y, instance->position.z - theCamera.core.position.z) < level->fogNear)
        {
            instance->fadeValue = 4096;

            MON_StartSpecialFade(instance, 0, 20);
        }
    }

    state = 0;

    if (!(instance->flags & 0x2))
    {
        if (mv->behaviorState == 4)
        {
            state = 20;
        }
        else if (mv->behaviorState == 16)
        {
            state = 26;
        }
        else if (mv->behaviorState == 13)
        {
            state = 23;

            mv->damageType = 0;
        }
        else if ((mv->mvFlags & 0x2))
        {
            state = 2;
        }
        else
        {
            state = 4;
        }
    }

    MON_SwitchState(instance, state);
}

void MON_CleanUp(Instance *instance)
{
    MonsterVars *mv;
    MonsterAttributes *ma;

    mv = (MonsterVars *)instance->extraData;

    ma = (MonsterAttributes *)instance->data;

    MON_DropAllObjects(instance);

    if (instance->LinkParent != NULL)
    {
        MON_UnlinkFromRaziel(instance);
    }

    if ((mv->causeOfDeath == 3) && (mv->effect != NULL))
    {
        SndEndLoop((unsigned long)mv->effect);

        mv->effect = NULL;
    }

    if (mv->pathSlotID != -1)
    {
        ENMYPLAN_ReleasePlanningWorkspace(mv->pathSlotID);
    }

    if (ma->neckSegment != 0)
    {
        G2Anim_DetachControllerFromSeg(&instance->anim, ma->neckSegment, 14);
    }

    if ((ma->spineSegment != 0) && (ma->spineSegment != ma->neckSegment))
    {
        G2Anim_DetachControllerFromSeg(&instance->anim, ma->spineSegment, 14);
    }

    if ((INSTANCE_Query(instance, 1) & 0xC000))
    {
        GlobalSave->humanOpinionOfRaziel++;
    }
}
