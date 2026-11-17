#include "Overlays/kain/kain.h"
#include "Game/DEBUG.h"
#include "Game/FX.h"
#include "Game/GAMELOOP.h"
#include "Game/GAMEPAD.h"
#include "Game/INSTANCE.h"
#include "Game/MATH3D.h"
#include "Game/PSX/SUPPORT.h"
#include "Game/PLAN/PLANAPI.h"
#include "Game/MEMPACK.h"
#include "Game/MONSTER/MONAPI.h"
#include "Game/MONSTER/MONLIB.h"
#include "Game/MONSTER/MONMSG.h"
#include "Game/MONSTER/MONSTER.h"
#include "Game/RAZIEL/RAZIEL.h"
#include "Game/SAVEINFO.h"
#include "Game/SOUND.h"
#include "Game/STATE.h"
#include "Game/STREAM.h"
#include "Game/OBTABLE.h"
#include "Game/HASM.h"

int KAIN_Lightning(Instance *instance); // TODO: Delete once matched

MonsterStateChoice KAIN_StateChoiceTable[] = 
{ 
    {MONSTER_STATE_PURSUE, {KAIN_PursueEntry, KAIN_Pursue}},
    {MONSTER_STATE_COMBAT, {KAIN_CombatEntry, KAIN_Combat}},
    {MONSTER_STATE_IDLE, {KAIN_IdleEntry, KAIN_Idle}},
    {MONSTER_STATE_ATTACK, {KAIN_AttackEntry, KAIN_Attack}},
    {MONSTER_STATE_HIT, {KAIN_HitEntry, KAIN_Hit}},
    {MONSTER_STATE_NOTICE, {KAIN_DoNothingEntry, KAIN_DoNothing}},
    {MONSTER_STATE_SURPRISED, {KAIN_DoNothingEntry, KAIN_DoNothing}},
    {-1, {NULL, NULL}},
};

const MonsterFunctionTable KAIN_FunctionTable = 
{
    KAIN_Init,
    KAIN_CleanUp,
    KAIN_DamageEffect,
    KAIN_Query,
    KAIN_Message,
    KAIN_StateChoiceTable,
    monVersion,
    "Jul 14 1999"
};

Instance *KAIN_PickUpReaver(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    Instance *inst; // not from debug symbols
    Instance *target; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    target = NULL;

    for (inst = gameTrackerX.instanceList->first; inst != NULL; inst = inst->next)
    {
        if (strcmpi(inst->object->name, "matsrevr") == 0)
        {
            target = inst;
            break;
        }

    }

    if (target != NULL)
    {
        mv->auxFlags |= 0x10;
        INSTANCE_Post(target, 0x800002, SetObjectData(0, 0, 0, instance, 0xE));
        G2EmulationInstanceSetAnimation(target, 0, 1, 0, 0);
        G2EmulationInstanceSetMode(target, 0, 1);
        return target;
    }

    mv->auxFlags &= ~0x10;
    return NULL;

}

void KAIN_FindFarthestMarkerPosition(Instance *instance, Position *position, int start, int end)
{

    PlanMkr *marker; // not from swbug symbols
    int dist; // not from swbug symbols
    int i; // not from swbug symbols
    int maxDist; // not from swbug symbols
    Level *level; // not from swbug symbols

    level = STREAM_GetLevelWithID(instance->currentStreamUnitID);

    if (level != NULL)
    {
        for (i = level->NumberOfPlanMarkers, marker = level->PlanMarkerList, maxDist = 0; i > 0; i--, marker++)
        {
            if (marker->id >= start && end >= marker->id)
            {
                dist = MATH3D_LengthXY(marker->pos.x - gameTrackerX.playerInstance->position.x, marker->pos.y - gameTrackerX.playerInstance->position.y);
                if (maxDist < dist)
                {
                    maxDist = dist;
                    COPY_SVEC(Position, position, Position, &marker->pos);
                }
            }
        }
    }
}

void KAIN_TeleportEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    KainVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (KainVars *)mv->extraVars;

    if (vars != NULL)
    {

        Instance *linkedInst; // not from debug symbols

        instance->fadeValue = 0;

        for (linkedInst = instance->LinkChild; linkedInst != NULL; linkedInst = linkedInst->LinkSibling)
        {
            linkedInst->fadeValue = 0;
        }

        mv->auxFlags &= ~0x8000000;
        vars->teleportState = K_NORMAL;
    }
}

int KAIN_Teleport(Instance *instance)
{

    int temp; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    KainVars *vars; // not from debug symbols
    KainAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (KainVars *)mv->extraVars;
    attrs = (KainAttributes *)ma->tunData;

    if (vars == NULL || attrs == NULL)
    {
        return 1;
    }

    switch (vars->teleportState)
    {
    case K_NORMAL:

        if (!(mv->auxFlags & 0x10))
        {
            MON_PlayAnimFromList(instance, ma->auxAnimList, MONSTER_ANIM_RUN, 1);
        }

        MON_StartSpecialFade(instance, 0x1000, attrs->timeToFadeOut);

        temp = MON_GetTime(instance) + 33;
        vars->timer = temp + (attrs->timeToFadeOut * 33);

        vars->teleportState = K_PHASING_OUT;
        SOUND_Play3dSound(&instance->position, 406, 0, 90, 32000);

        return 0;

    case K_PHASING_OUT:

        if (MON_GetTime(instance) >= (unsigned long)vars->timer)
        {
            if (mv->auxFlags & 0x100)
            {

                Instance *childInst; // not from debug symbols

                childInst = instance->LinkChild;

                while (childInst != NULL)
                {
                    Instance *tempInst; // not from debug symbols

                    tempInst = childInst->LinkSibling;
                    INSTANCE_KillInstance(childInst);
                    childInst = tempInst;
                }

                INSTANCE_KillInstance(instance);
                mv->auxFlags &= ~0x100;

            }
            else if (!(mv->auxFlags & 0x800))
            {
                vars->timer = MON_GetTime(instance) + (attrs->timeFadePause * 33);
                vars->teleportState = K_TELEPORT_PAUSE;
            }
            else
            {
                vars->teleportState = K_TELEPORT_HOLD;
            }
        }
        return 0;

    case K_TELEPORT_PAUSE:

        if (MON_GetTime(instance) >= (unsigned int)vars->timer)
        {

            int temp; // not from debug symbols

            instance->flags2 |= 0x20000000;
            MON_StartSpecialFade(instance, 0, attrs->timeToFadeIn);
            temp = MON_GetTime(instance) + 33;
            vars->timer = temp + (attrs->timeToFadeIn * 33);
            vars->teleportState = K_MOVE_DONE;

            COPY_SVEC(Position, &instance->position, Position, &vars->teleportTarget);
            COPY_SVEC(Position, &instance->oldPos, Position, &vars->teleportTarget);

            MON_TurnToPosition(instance, &gameTrackerX.playerInstance->position, 4096);
        }

        return 0;

    case K_TELEPORT_HOLD:

        if (mv->auxFlags & 0x1000)
        {
            int temp; // not from debug symbols

            MON_StartSpecialFade(instance, 0, attrs->timeToFadeIn);
            temp = MON_GetTime(instance) + 33;
            vars->timer = temp + (attrs->timeToFadeIn * 33);
            vars->teleportState = K_MOVE_DONE;
        }

        return 0;

    case K_MOVE_DONE:

        if (!(mv->auxFlags & 0x10))
        {
            MON_PlayAnimFromList(instance, ma->auxAnimList, MONSTER_ANIM_WALK, 2);
        }

        instance->flags2 &= ~0x20000000;
        vars->teleportState = K_PHASING_IN;
        SOUND_Play3dSound(&instance->position, 405, 0, 90, 32000);

    case K_PHASING_IN:

        if (MON_GetTime(instance) >= (unsigned long)vars->timer)
        {
            vars->teleportState = K_NO_TELEPORT;
            return 1;
        }

        return 0;

    case K_NO_TELEPORT:
        return 1;
    default:
        return 0;
    }
}

void KAIN_EndEffects(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    KainVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (KainVars *)mv->extraVars;

    if (vars != NULL)
    {
        vars->field = NULL;
        vars->zap[2] = NULL;
        vars->zap[1] = NULL;
        vars->zap[0] = NULL;
        FX_EndInstanceEffects(instance);
    }
}

void KAIN_LightningEntry(Instance *instance)
{

    MATRIX *mat; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    KainVars *vars; // not from debug symbols
    KainAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (KainVars *)mv->extraVars;
    attrs = (KainAttributes *)ma->tunData;

    if (vars != NULL && attrs != NULL)
    {
        if (mv->auxFlags & 2)
        {
            vars->numBeams = 3;
            vars->zap[2] = 0;
            vars->zap[1] = 0;
            vars->zap[0] = 0;
        }
        else
        {
            vars->numBeams = 1;
            vars->zap[2] = 0;
            vars->zap[1] = 0;
            vars->zap[0] = 0;

            if (!(mv->auxFlags & 1))
            {
                mat = (MATRIX *)INSTANCE_Query(vars->zapTarget, queryLookatMatrix);
                vars->beamTarget.x = mat->t[0];
                vars->beamTarget.y = mat->t[1];
                vars->beamTarget.z = mat->t[2];
            }
        }

        vars->beamTimer = MON_GetTime(instance);
        vars->changeBeam = 0;
    }
}

int KAIN_Lightning(Instance *instance)
{
    
    SVECTOR start;
    SVECTOR offset[3];
    SVECTOR nearest;
    int temp_lo; // not from debug symbols
    int time; // not from debug symbols
    int var_s0; // not from debug symbols
    int effectIndex; // not from debug symbols
    int i; // not from debug symbols
    int var_s7; // not from debug symbols
    Model *model; // not from debug symbols
    GenericLightningParams *lightningParams; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    KainVars *vars; // not from debug symbols
    KainAttributes *attrs; // not from debug symbols
    Instance *zapTarget; // not from debug symbols
    Object *object; // not from debug symbols
    MATRIX* temp_v1_2; // not from debug symbols
    MATRIX *mat; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (KainVars *)mv->extraVars;
    attrs = (KainAttributes *)ma->tunData;
    var_s7 = 0;
    
    if (vars == NULL || attrs == NULL)
    {
        return 0;
    }

    if ((unsigned char)vars->numBeams != 0)
    {
    
    time = MON_GetTime(instance);
    
    if (time >= vars->beamTimer)
    {

        effectIndex = mv->auxFlags & 0x10 ? 0 : 2;
        
        if (mv->auxFlags & 2)
        {
            
            zapTarget = vars->zapTarget;
            model = zapTarget->object->modelList[zapTarget->currentModel];
            vars->beamTimer = time + 165;
            
            if (model->numSegments > 1) 
            {
                vars->hitSegment[(unsigned char)vars->changeBeam] = (rand() % (model->numSegments - 1)) + 1;
            } 
            else 
            {
                vars->hitSegment[(unsigned char)vars->changeBeam] = 0;
            }
        } 
        else 
        {
            
            var_s0 = attrs->attackLength; // TODO: fake match, var_s0 reused as a temporary
            vars->beamTimer = time + (var_s0 * 33);
            vars->hitSegment[(unsigned char)vars->changeBeam] = 1;
            effectIndex++;
        }
        
        object = OBTABLE_FindObject("particle");
        
        if (object == NULL) 
        {
            return 0;
        }

        lightningParams = &((GenericFXObject *)object->data)->LightningList[instance->object->effectList[effectIndex].modifierList[0]];
        
        if (vars->zap[(unsigned char)vars->changeBeam] != NULL) 
        {
            FX_DeleteGeneralEffect((FXGeneralEffect *)vars->zap[(unsigned char)vars->changeBeam]);
            vars->zap[(unsigned char)vars->changeBeam] = NULL;
        }
        
        vars->zap[(unsigned char)vars->changeBeam] = FX_CreateLightning(instance, -1, lightningParams->deg, lightningParams->deg_inc, lightningParams->width, lightningParams->small_width, lightningParams->segs, lightningParams->sine_size, lightningParams->variation, lightningParams->color, lightningParams->glow_color);
        vars->changeBeam++;
        
        if ((unsigned char)vars->changeBeam >= (unsigned char)vars->numBeams) 
        {
            vars->changeBeam = 0;
        }
    }

    
    i = mv->auxFlags & 2 ? 165 : attrs->timeToHit * 33;
    var_s0 = ((vars->timer - (attrs->attackLength * 33)) + i) - time;

    if (var_s0 < 0) 
    {
        var_s0 = 0;
    } 
    else 
    {
        var_s0 = MIN(i, var_s0);
    }
    
    temp_v1_2 = &instance->matrix[(signed char)mv->attackType->sphereSegment];
    start.vx = temp_v1_2->t[0];
    start.vy = temp_v1_2->t[1];
    start.vz = temp_v1_2->t[2];
    
    if (mv->auxFlags & 3) 
    {
        for (i = 0; i < (unsigned char)vars->numBeams; i++) 
        {

            mat = &vars->zapTarget->matrix[(unsigned char)vars->hitSegment[i]];

            offset[i].vx = (var_s0 * (start.vx - mat->t[0])) / (attrs->timeToHit * 0x21);
            offset[i].vy = (var_s0 * (start.vy - mat->t[1])) / (attrs->timeToHit * 0x21);
            offset[i].vz = (var_s0 * (start.vz - mat->t[2])) / (attrs->timeToHit * 0x21);
            
            offset[i].vx += mat->t[0];
            offset[i].vy += mat->t[1];
            offset[i].vz += mat->t[2];
            
        }
        
        
        if (var_s0 <= 0)
        {
            var_s7 = 1;
        }
        
    }
    else 
    {

        int timeToHit; // not from debug symbols
        
        offset[0].vx = vars->beamTarget.x - start.vx;
        offset[0].vy = vars->beamTarget.y - start.vy;
        offset[0].vz = vars->beamTarget.z - start.vz;

        MATH3D_Normalize((Normal *)&offset);
        
        timeToHit = attrs->timeToHit * 33;
        temp_lo = (attrs->maxBoltDist * (timeToHit - var_s0)) / timeToHit;
        
        offset[0].vx = start.vx + ((temp_lo * offset[0].vx) / 4096);
        offset[0].vy = start.vy + ((temp_lo * offset[0].vy) / 4096);
        offset[0].vz = start.vz + ((temp_lo * offset[0].vz) / 4096);
        
        COLLIDE_NearestPointOnLine_S((SVector *)&nearest, &start, &offset[0], &gameTrackerX.playerInstance->position);
        
        mat = (MATRIX *)INSTANCE_Query(gameTrackerX.playerInstance, queryLookatMatrix);
        start.vx = mat->t[0];
        start.vy = mat->t[1];
        start.vz = mat->t[2];
        
        if (MATH3D_LengthXYZ(nearest.vx - start.vx, nearest.vy - start.vy, nearest.vz - start.vz) < 300)
        {
            mv->auxFlags |= 1;
            vars->timer += time - ((vars->timer - (attrs->attackLength * 33)) + (attrs->timeToHit * 33));
            FX_SetLightingPos(vars->zap[0], instance, (signed char)mv->attackType->sphereSegment, 0, 0, 0, (Position *)&start, -1);
            var_s7 = 1;
        }
    }

    for (i = 0; i < (unsigned char)vars->numBeams; i++)
    {
        if (vars->zap[i] != NULL)
        {
            FX_SetLightingPos(vars->zap[i], instance, (signed char)mv->attackType->sphereSegment, 0, 0, 0, (Position *)&offset[i], -1);
        }
    }
    
    }

    return var_s7;
}

void KAIN_FFieldOffset(Instance *instance, int segment, Position *position)
{
    MATRIX *mat; // not from debug symbols

    mat = &instance->matrix[segment];
    position->x = mat->t[0] - instance->position.x;
    position->y = mat->t[1] - instance->position.y;
    position->z = mat->t[2] - instance->position.z;
}

void KAIN_ChargeEntry(Instance *instance, int segment)
{

    Position offset;
    long color; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    KainVars *vars; // not from debug symbols
    KainAttributes *attrs; // not from debug symbols

    color = 0xBF1F1F;
    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (KainVars *)mv->extraVars;
    attrs = (KainAttributes *)ma->tunData;

    if (vars != NULL && attrs != NULL)
    {
        KAIN_FFieldOffset(instance, segment, &offset);
        vars->field = FX_StartFField(instance, 0x80, &offset, 0, 0x400, 0, color);
    }
}

void KAIN_ChargeUp(Instance *instance, int time, int segment)
{

    int timeLeft; // not from debug symbols
    int timeToBolt; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    KainVars *vars; // not from debug symbols
    KainAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (KainVars *)mv->extraVars;
    attrs = (KainAttributes *)ma->tunData;

    if (vars != NULL && attrs != NULL && vars->field != NULL)
    {

        timeLeft = MAX(vars->timer - time, 0);
        KAIN_FFieldOffset(instance, segment, (Position *)&vars->field->offset);

        if (mv->auxFlags & 0x10)
        {
            timeToBolt = attrs->timeToBolt[vars->numHits] * 33;
        }
        else
        {
            timeToBolt = attrs->timeToBolt[vars->numHits + attrs->maxHits] * 33;
        }

        vars->field->size_change = ((timeLeft * 960) / timeToBolt) + 64;
        vars->field->size = (timeLeft * 128) / timeToBolt;
    }
}

void KAIN_ChargeDown(Instance *instance, int time, int segment)
{
    (void)time;
    (void)segment;
    KAIN_EndEffects(instance);
}

int CheckHit(Instance *instance)
{
    if (instance->currentMainState == MONSTER_STATE_COMBAT || instance->currentMainState == MONSTER_STATE_ATTACK)
    {
        return 1;
    }
    return 0;
}

void SwitchToHit(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    mv->mvFlags |= 0x40;
    MON_SwitchStateDoEntry(instance, MONSTER_STATE_HIT);
}

void KAIN_DamageEffect(Instance *instance, evFXHitData *data)
{

    SVector localloc;
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (data != NULL)
    {
        if ((mv->auxFlags & 0x50) == 0x50 || (mv->auxFlags & 0x14) == 4)
        {
            MON_DamageEffect(instance, data);
        }
        else
        {
            localloc = data->location;
            FX_MakeHitFX(&localloc);
            SOUND_Play3dSound(&instance->position, 21, 650, 80, 15500);
        }
    }
}

void KAIN_Message(Instance *instance, unsigned long message, unsigned long data)
{
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    KainVars *vars; // not from debug symbols
    KainAttributes *attrs; // not from debug symbols
    Instance *child; // not from debug symbols
    Instance *target; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (KainVars *)mv->extraVars;
    attrs = (KainAttributes *)ma->tunData;

    if ((vars == NULL) || (attrs == NULL))
    {
        MonsterMessage(instance, message, data);
    }

    switch (message)
    {
    case 0x1000020:
        KAIN_EndEffects(instance);

        if (SndIsPlaying(vars->soundHandle) != 0)
        {
            SndEndLoop(vars->soundHandle);
        }

        MON_SwitchStateDoEntry(instance, 2);
        break;
    case 0x1000023:
        if (data != 0x1000)
        {
            break;
        }
    case 0x100001F:
        if (CheckHit(instance) != 0)
        {
            if (mv->auxFlags & 0x10)
            {
                mv->auxFlags &= ~0x4;
            }
            else
            {
                mv->auxFlags |= 0x4;
            }

            mv->auxFlags &= ~0x40;

            SwitchToHit(instance);
        }

        break;
    case 0x1000000:
        if (((evMonsterHitData *)data)->power >= 0x3000)
        {
            instance->constrictAngle = 0;
        }

        if (CheckHit(instance) != 0)
        {
            mv->auxFlags &= ~0x4;

            if (!(mv->auxFlags & 0x10) || (INSTANCE_Query(((evMonsterHitData *)data)->sender, 1) == 0x20))
            {
                mv->auxFlags &= ~0x40;
            }
            else
            {
                mv->auxFlags |= 0x40;
            }

            SwitchToHit(instance);
        }

        break;
    case 0x80001:
    case 0x80002:
    case 0x80003:
    case 0x80004:
    case 0x80005:
    case 0x80006:
    case 0x1000019:
    case 0x1000021:
        if (CheckHit(instance) != 0)
        {
            mv->auxFlags &= ~0x4;
            mv->auxFlags &= ~0x40;

            SwitchToHit(instance);
        }

        break;
    case 0x400000:
        if ((CheckHit(instance) != 0) && (mv->auxFlags & 0x44))
        {
            MonsterMessage(instance, 0x400000, data);
        }

        break;
    case 0x1000017:
        switch (data)
        {
        case 0:
            mv->auxFlags |= 0x100;

            MON_SwitchStateDoEntry(instance, 2);
            break;
        case 1:
            if (mv->auxFlags & 0x8)
            {
                vars->tier++;
            }

            mv->auxFlags &= ~0x8;
            break;
        case 7:
            vars->tier = 0;
            vars->numHits = 0;

            mv->auxFlags &= ~0x20;
            break;
        case 2:
            child = instance->LinkChild;

            if (child != NULL)
            {
                INSTANCE_Post(child, 0x800008, 0);
                INSTANCE_Post(child, 0x800002, SetObjectData(0, 0, 0, instance, ma->rightWeaponSegment));

                G2EmulationInstanceSetAnimation(child, 0, 0, 0, 0);
                G2EmulationInstanceSetMode(child, 0, 1);
            }

            break;
        case 3:
            mv->auxFlags |= 0x200;

            vars->zapTarget = gameTrackerX.playerInstance;

            MON_SwitchStateDoEntry(instance, 6);
            break;
        case 5:
            mv->auxFlags |= 0x1000;
            break;
        case 6:
            mv->auxFlags |= 0x2000;

            MON_SwitchStateDoEntry(instance, 2);
            break;
        case 4:
            mv->auxFlags |= 0x400;
            break;
        }

        break;
    case 0x1000022:
        if (data != 0)
        {
            target = INSTANCE_Find(data);

            if (target != NULL)
            {
                vars->zapTarget = target;

                mv->auxFlags |= 0x200;

                MON_SwitchStateDoEntry(instance, 6);
            }
        }
        else
        {
            mv->auxFlags |= 0x400;
        }

        break;
    case 0x40009:
        break;
    default:
        MonsterMessage(instance, message, data);
        break;
    }
}

uintptr_t KAIN_Query(Instance *instance, unsigned long query)
{

    uintptr_t ret; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    KainVars *vars; // not from debug symbols
    KainAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (KainVars *)mv->extraVars;
    attrs = (KainAttributes *)ma->tunData;

    if (vars == NULL || attrs == NULL)
    {
        return MonsterQuery(instance, query);
    }

    switch (query)
    {
    case queryHitState:
        ret = 0x1000000;
        break;
    case querySpecialInfo:

        ret = 0;

        if (mv->auxFlags & 0x20)
        {
            ret = 1;
        }

        if (mv->auxFlags & 8)
        {
            ret |= 2;
        }

        break;
    default:
        ret = MonsterQuery(instance, query);
        break;
    }

    return ret;
}

void KAIN_Init(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    KainVars *vars; // not from debug symbols

    MON_DefaultInit(instance);
    mv = (MonsterVars *)instance->extraData;

    if (mv != NULL)
    {
        vars = (KainVars *)MEMPACK_Malloc(sizeof(KainVars), MEMORY_TYPE_KAINDATA);

        if (vars == NULL)
        {
            mv->extraVars = NULL;
        }
        else
        {
            mv->extraVars = vars;
            vars->numHits = 0;
            vars->tier = 0;
            vars->teleportState = K_NO_TELEPORT;
        }
    }

    KAIN_PickUpReaver(instance);
    RAZIEL_SetInteractiveMusic(SOUND_MODIFIER_BOSS_LOADED, 1);
    DEBUG_DoAreaProtection();
}

void KAIN_CleanUp(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    KainVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    RAZIEL_SetInteractiveMusic(SOUND_MODIFIER_BOSS_LOADED, 0);

    if (mv != NULL)
    {

        vars = (KainVars *)mv->extraVars;

        if (vars != NULL)
        {
            if (SndIsPlaying(vars->soundHandle))
            {
                SndEndLoop(vars->soundHandle);
            }

            MEMPACK_Free((char *)vars);
        }
    }

    MON_CleanUp(instance);
}

void KAIN_IdleEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (mv->extraVars == NULL)
    {
        return;
    }

    if (mv->auxFlags & 0x6100)
    {
        KAIN_EndEffects(instance);
        KAIN_TeleportEntry(instance);

        if (mv->auxFlags & 0x4000)
        {
            MON_PlayAnimIfNotPlaying(instance, MONSTER_ANIM_STANCE_HEALTHY, 2);
            mv->auxFlags &= ~0x4000;
            mv->auxFlags |= 0x800;
        }
        else if (mv->auxFlags & 0x2000)
        {
            mv->auxFlags &= ~0x2000;
            mv->auxFlags |= 0x800;
        }
    }
    else
    {
        if (mv->mvFlags & 4)
        {
            MON_IdleEntry(instance);
        }
        else
        {
            MON_PlayAnimIfNotPlaying(instance, MONSTER_ANIM_STANCE_HEALTHY, 2);
            KAIN_EndEffects(instance);
        }
    }
}

void KAIN_Idle(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    KainVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (KainVars *)mv->extraVars;

    if (vars != NULL)
    {
        if ((mv->auxFlags & 0x800 && (vars->teleportState != K_TELEPORT_HOLD || mv->auxFlags & 0x1000)) || mv->auxFlags & 0x100)
        {
            if (KAIN_Teleport(instance) != 0)
            {
                mv->auxFlags &= ~0x800;
                mv->auxFlags &= ~0x1000;
            }

            MON_DefaultQueueHandler(instance);
            return;
        }

        if (mv->mvFlags & 4)
        {
            MON_Idle(instance);
            return;
        }

        if (!(mv->auxFlags & 0x828) && instance->currentStreamUnitID == gameTrackerX.playerInstance->currentStreamUnitID)
        {
            MON_SwitchState(instance, MONSTER_STATE_PURSUE);
        }

        MON_DefaultQueueHandler(instance);
    }
}

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/Overlays/kain/kain", KAIN_CombatEntry);
#else
void KAIN_CombatEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    KainVars *vars; // not from debug symbols
    KainAttributes *attrs; // not from debug symbols
    int index; // not from debug symbols
    int timeToBolt; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (KainVars *)mv->extraVars;
    ma = (MonsterAttributes *)instance->data;
    attrs = (KainAttributes *)ma->tunData;

    if ((vars != NULL) && (attrs != NULL))
    {
        if (mv->mvFlags & 4)
        {
            MON_SwitchStateDoEntry(instance, 2);
            return;
        }

        index = (mv->auxFlags & 0x10) ? vars->numHits : vars->numHits + attrs->maxHits;

        timeToBolt = attrs->timeToBolt[index] * 33;

        vars->timer = MON_GetTime(instance) + timeToBolt;

        if (mv->auxFlags & 0x10)
        {
            mv->attackType = &ma->attackAttributesList[(signed char)mv->subAttr->combatAttributes->attackList[0]];
        }
        else
        {
            mv->attackType = &ma->attackAttributesList[(signed char)mv->subAttr->combatAttributes->attackList[2]];
        }

        KAIN_ChargeEntry(instance, mv->attackType->sphereSegment);

        vars->soundHandle = SOUND_Play3dSound(&instance->position, 398, -200, 30, 32000);
    }
}
#endif

void KAIN_Combat(Instance *instance)
{
    
    int timeToBolt; // not from debug symbols
    int pitch; // not from debug symbols
    int maxVolume; // not from debug symbols
    int time; // not from debug symbols
    int timeLeft; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    KainVars *vars; // not from debug symbols
    KainAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (KainVars *)mv->extraVars;
    attrs = (KainAttributes *)ma->tunData;
    
    if (vars == NULL || attrs == NULL)
    {
        return;
    }
    
    if (mv->mvFlags & 4) 
    {
        MON_SwitchState(instance, 2);
    } 
    else 
    {

        time = MON_GetTime(instance);
        KAIN_ChargeUp(instance, time, mv->attackType->sphereSegment);
        timeLeft = vars->timer - MON_GetTime(instance);

        if (timeLeft < 0)
        {
            timeLeft = 0;
        }
        
        if (mv->auxFlags & 0x10) {
            timeToBolt = attrs->timeToBolt[vars->numHits] * 33;
        } 
        else 
        {
            timeToBolt = attrs->timeToBolt[vars->numHits + attrs->maxHits] * 33;
        }
        
        pitch = 100 - ((timeLeft * 300) / timeToBolt);
        maxVolume = 100 - ((timeLeft * 70) / timeToBolt);
        
        if (SndIsPlaying(vars->soundHandle)) 
        {
            SOUND_Update3dSound(&instance->position, vars->soundHandle, pitch, maxVolume, 32000);
        }
        else if (vars->field != NULL) 
        {
            vars->soundHandle = SOUND_Play3dSound(&instance->position, 398, pitch, maxVolume, 32000);
        }
        
        if (time >= vars->timer && MON_TurnToPosition(instance, &gameTrackerX.playerInstance->position, mv->subAttr->speedPivotTurn) != 0) 
        {
            MON_SwitchState(instance, 6);
        }
    }
    
    MON_DefaultQueueHandler(instance);
    
    if (instance->currentMainState != 0xD && SndIsPlaying(vars->soundHandle)) 
    {
        SndEndLoop(vars->soundHandle);
    }
}

void KAIN_AttackEntry(Instance *instance)
{

    int attackIndex; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    KainVars *vars; // not from debug symbols
    KainAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (KainVars *)mv->extraVars;
    attrs = (KainAttributes *)ma->tunData;

    if (vars == NULL || attrs == NULL)
    {
        return;
    }

    if (mv->auxFlags & 0x200)
    {
        mv->attackType = &ma->attackAttributesList[(int)mv->subAttr->combatAttributes->attackList[4]];
        mv->auxFlags |= 2;
    }
    else
    {

        vars->zapTarget = gameTrackerX.playerInstance;
        attackIndex = mv->auxFlags & 0x10 ? 0 : 2;

        if (MATH3D_LengthXY(instance->position.x - gameTrackerX.playerInstance->position.x, instance->position.y - gameTrackerX.playerInstance->position.y) < attrs->handHitDist)
        {
            mv->attackType = &ma->attackAttributesList[(int)mv->subAttr->combatAttributes->attackList[attackIndex]];
            mv->auxFlags |= 2;
        }
        else
        {
            mv->attackType = &ma->attackAttributesList[(int)mv->subAttr->combatAttributes->attackList[attackIndex + 1]];
            mv->auxFlags &= ~2;

            if (MATH3D_LengthXY(gameTrackerX.playerInstance->oldPos.x - gameTrackerX.playerInstance->position.x, gameTrackerX.playerInstance->oldPos.y - gameTrackerX.playerInstance->position.y) > 10)
            {
                mv->auxFlags |= 1;
            }
            else
            {
                mv->auxFlags &= ~1;
            }
        }
    }

    vars->timer = MON_GetTime(instance) + (mv->attackType->sphereOnFrame * 33);
    MON_PlayAnimFromList(instance, mv->attackType->animList, MONSTER_ANIM_HIT1, 1);
    mv->attackState = K_NORMAL;
}

void KAIN_Attack(Instance *instance)
{

    int motorSpeed; // not from debug symbols
    int time; // not from debug symbols
    int motorTime; // not from debug symbols
    int attackState; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    KainVars *vars; // not from debug symbols
    KainAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (KainVars *)mv->extraVars;
    attrs = (KainAttributes *)ma->tunData;

    if (vars == NULL || attrs == NULL)
    {
        return;
    }

    MON_TurnToPosition(instance, &vars->zapTarget->position, 4096);
    time = MON_GetTime(instance);
    attackState = mv->attackState;

    switch (attackState)
    {
    case 0:
        KAIN_ChargeDown(instance, vars->timer - (attrs->attackLength * 33), mv->attackType->sphereSegment);

        if (time >= vars->timer)
        {
            GAMEPAD_Shock0(1, 8192);
            KAIN_LightningEntry(instance);
            vars->soundHandle = SOUND_Play3dSound(&instance->position, 397, 0, 100, 32762);
            vars->timer = MON_GetTime(instance) + (attrs->attackLength * 33);
            mv->attackState++;
        }

        if (instance->flags2 & 0x10 && mv->attackType->numAnims > 1)
        {
            MON_PlayAnimFromList(instance, mv->attackType->animList, MONSTER_ANIM_HIT2, 2);
        }
        break;
    case 1:
        if (mv->auxFlags & 0x200)
        {
            KAIN_Lightning(instance);

            if (mv->auxFlags & 0x400)
            {
                KAIN_EndEffects(instance);
                mv->auxFlags &= ~0x200;
                mv->auxFlags &= ~0x400;
                MON_SwitchState(instance, 2);
            }
        }
        else
        {
            if (KAIN_Lightning(instance) != 0)
            {
                motorTime = ((vars->timer - time) * 4096) / 33;
                INSTANCE_Post(vars->zapTarget, 0x40025, motorTime / 30);
                mv->attackState++;
                motorSpeed = mv->auxFlags & 2 ? 128 : 196;
                GAMEPAD_Shock1(motorSpeed, motorTime);
            }

            if (time >= vars->timer)
            {
                MON_SwitchState(instance, 1);
            }
        }
        if (instance->flags2 & 0x10 && mv->attackType->numAnims > 1)
        {
            MON_PlayAnimFromList(instance, mv->attackType->animList, MONSTER_ANIM_HIT2, 2);
        }
        break;
    case 2:
        INSTANCE_Post(vars->zapTarget, 0x40003, SetActionPlayHostAnimationData(vars->zapTarget, instance, 20, 0, 0, attackState));
        INSTANCE_Post(vars->zapTarget, 0x1000000, SetMonsterHitData(instance, 0, mv->attackType->damage * 512, 0, 0));
        mv->attackState++;
    case 3:
        KAIN_Lightning(instance);

        if (instance->flags2 & 0x10 && mv->attackType->numAnims > 1)
        {
            MON_PlayAnimFromList(instance, mv->attackType->animList, MONSTER_ANIM_HIT2, 2);
        }

        if (time >= vars->timer)
        {
            MON_SwitchState(instance, MONSTER_STATE_PURSUE);
        }
        break;
    }

    if (instance->currentMainState != MONSTER_STATE_ATTACK && SndIsPlaying(vars->soundHandle))
    {
        SndEndLoop(vars->soundHandle);
    }

    MON_DefaultQueueHandler(instance);
}

void KAIN_PursueEntry(Instance *instance)
{

    enum MonsterAnim anim; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;

    if (mv->extraVars != NULL)
    {
        if (mv->mvFlags & 4)
        {
            MON_PursueEntry(instance);
            return;
        }

        if (mv->previousMainState != MONSTER_STATE_PURSUE)
        {
            KAIN_EndEffects(instance);
            anim = mv->auxFlags & 0x10 ? MONSTER_ANIM_HIT2 : MONSTER_ANIM_WALK;
            MON_PlayAnimFromList(instance, ma->auxAnimList, anim, 2);
            KAIN_TeleportEntry(instance);
        }
    }
}

void KAIN_Pursue(Instance *instance)
{

    Position pos;
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    KainVars *vars; // not from debug symbols
    KainAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (KainVars *)mv->extraVars;
    ma = (MonsterAttributes *)instance->data;
    attrs = (KainAttributes *)ma->tunData;

    if (vars == NULL || attrs == NULL)
    {
        return;
    }

    if (mv->mvFlags & 4)
    {
        MON_Pursue(instance);
        return;
    }

    if (vars->teleportState == K_TELEPORT_PAUSE && MON_GetTime(instance) >= (unsigned long)vars->timer)
    {
        if (mv->auxFlags & 0x10)
        {
            PLANAPI_FindNodePositionInUnit(STREAM_GetStreamUnitWithID(instance->currentStreamUnitID), &pos, 1, 4);

            if (MATH3D_LengthXY(pos.x - gameTrackerX.playerInstance->position.x, pos.y - gameTrackerX.playerInstance->position.y) < attrs->outsideOfRoom)
            {
                KAIN_FindFarthestMarkerPosition(instance, &vars->teleportTarget, 1, 4);
            }
            else
            {
                COPY_SVEC(Position, &vars->teleportTarget, Position, &pos);
            }
        }
        else
        {
            KAIN_FindFarthestMarkerPosition(instance, &vars->teleportTarget, ((unsigned char)vars->tier * 4) | 1, ((unsigned char)vars->tier * 4) + 4);
        }
    }

    if (KAIN_Teleport(instance) != 0)
    {
        if (instance->currentStreamUnitID == gameTrackerX.playerInstance->currentStreamUnitID)
        {
            MON_SwitchState(instance, MONSTER_STATE_COMBAT);
        }
        else
        {
            MON_SwitchState(instance, MONSTER_STATE_IDLE);
        }
    }

    MON_DefaultQueueHandler(instance);
}

void KAIN_HitEntry(Instance *instance)
{

    int mode; // not from debug symbols
    int animType; // not from debug symbols
    int hits; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    KainVars *vars; // not from debug symbols
    KainAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (KainVars *)mv->extraVars;
    attrs = (KainAttributes *)ma->tunData;

    if (vars != NULL && attrs != NULL)
    {

        GAMEPAD_Shock0(1, 8192);

        animType = 0;
        mode = 2;
        hits = MIN(vars->numHits, attrs->maxHits);

        if ((mv->auxFlags & 0x50) == 0x50)
        {
            animType = hits + 4;
            mode = 1;
            vars->numHits++;
        }
        else if ((mv->auxFlags & 0x14) == 4)
        {
            mode = 1;
            vars->numHits++;
            animType = hits + 7;

            if (vars->numHits < attrs->maxHits)
            {
                mv->auxFlags |= 8;
            }
            else
            {
                mv->auxFlags |= 0x20;
            }
        }
        else
        {
            vars->timer = MON_GetTime(instance) + 1320;
            SOUND_Play3dSound(&instance->position, 403, 0, 100, 15500);
            SOUND_Play3dSound(&instance->position, 21, 650, 70, 15500);
        }

        if (!MON_AnimPlayingFromList(instance, ma->auxAnimList, animType))
        {
            MON_PlayAnimFromList(instance, ma->auxAnimList, animType, mode);
            mv->mode = 0x8000;
            KAIN_EndEffects(instance);
        }

        if (SndIsPlaying(vars->soundHandle))
        {
            SndEndLoop(vars->soundHandle);
        }
    }
}

void KAIN_Hit(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    KainVars *vars; // not from debug symbols
    KainAttributes *attrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    vars = (KainVars *)mv->extraVars;
    attrs = (KainAttributes *)ma->tunData;

    if (vars != NULL && attrs != NULL)
    {
        if (!MON_AnimPlayingFromList(instance, ma->auxAnimList, 0))
        {
            if (instance->flags2 & 0x10)
            {
                goto label;
            }
        }
        else if (MON_GetTime(instance) >= (unsigned long)vars->timer)
        {
        label:
            if ((mv->auxFlags & 0x50) == 0x50 && vars->numHits >= attrs->maxHits)
            {
                mv->auxFlags |= 0x20;
                MON_SwitchState(instance, MONSTER_STATE_IDLE);
            }
            else if ((mv->auxFlags & 0x14) == 4)
            {
                mv->auxFlags |= 0x4000;
                MON_SwitchState(instance, MONSTER_STATE_IDLE);
            }
            else
            {
                MON_SwitchState(instance, MONSTER_STATE_PURSUE);
            }

            mv->auxFlags &= ~0x40;
            mv->auxFlags &= ~4;
        }

        MON_DefaultQueueHandler(instance);
    }
}

void KAIN_DoNothingEntry(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    KainVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (KainVars *)mv->extraVars;

    if (vars != NULL)
    {
        if (SndIsPlaying(vars->soundHandle))
        {
            SndEndLoop(vars->soundHandle);
        }

        MON_SwitchState(instance, mv->previousMainState);
    }
}

void KAIN_DoNothing(Instance *instance)
{
    (void)instance;
};
