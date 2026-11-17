#include "Overlays/hunter/hunter.h"
#include "Game/FX.h"
#include "Game/GAMELOOP.h"
#include "Game/MATH3D.h"
#include "Game/MEMPACK.h"
#include "Game/INSTANCE.h"
#include "Game/MONSTER/HUMAN.h"
#include "Game/MONSTER/MONLIB.h"
#include "Game/MONSTER/MONMSG.h"
#include "Game/MONSTER/MONSTER.h"
#include "Game/SAVEINFO.h"
#include "Game/STATE.h"

MonsterStateChoice HUNTER_StateChoiceTable[] = 
{
    {MONSTER_STATE_STUNNED, {HUMAN_StunnedEntry, HUMAN_Stunned}},
    {MONSTER_STATE_DEAD, {HUMAN_DeadEntry, HUMAN_Dead}},
    {MONSTER_STATE_EMBRACE, {HUMAN_EmbraceEntry, HUMAN_Embrace}},
    {MONSTER_STATE_IDLE, {HUMAN_IdleEntry, HUMAN_Idle}},
    {MONSTER_STATE_PROJECTILE, {HUNTER_ProjectileEntry, HUNTER_Projectile}},
    {-1, {NULL, NULL}}
};

const MonsterFunctionTable HUNTER_FunctionTable = 
{
    HUNTER_Init,
    HUNTER_CleanUp,
    NULL,
    HUMAN_Query,
    NULL,
    HUNTER_StateChoiceTable,
    monVersion,
    NULL
};

void FX_MakeHitFlame(SVector *startpos, int zpos, int angle, int dist, int size)
{

    SVector position;
    int cos; // not from debug symbols
    int sin; // not from debug symbols
    FX_PRIM *prim; // not from debug symbols

    prim = FX_GetPrim(gFXT);

    if (prim != NULL)
    {

        cos = rcos(angle - 1024);
        position.x = startpos->x + ((cos * dist) / 4096) + (rand() & 0xF);

        sin = rsin(angle - 1024);
        position.y = startpos->y + ((sin * dist) / 4096) + (rand() & 0xF);

        position.z = startpos->z + zpos + (rand() & 0xF);

        FX_MakeParticleTexFX(prim, &position, 0, 0, 0, 0, 0, 0x93E4, size, 2);
        FX_Sprite_Insert(&gFXT->usedPrimListSprite, prim);

    }
}

void HUNTER_InitFlamethrow(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    HunterVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    vars = (HunterVars *)mv->extraVars;

    if (vars != NULL)
    {
        vars->endIdx = 0;
        vars->startIdx = 0;
        vars->timer = 0;
        vars->rem_time = 0;
        mv->auxFlags &= ~4;
    }
}

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/Overlays/hunter/hunter", HUNTER_Flamethrow);
#else
int HUNTER_Flamethrow(Instance* instance, int damage, int newPoint, int segment) 
{
    Position mypos;              
    Position sp20; // not from debug symbols
    int flameAngle; // not from debug symbols
    int temp_v1; // not from debug symbols
    int idx; // not from debug symbols
    int temp_ret; // not from debug symbols
    int invAngle; // not from debug symbols
    int angle; // not from debug symbols
    int temp_v1_2; // not from debug symbols
    int rem_time; // not from debug symbols
    int flameDist; // not from debug symbols
    int var_s1; // not from debug symbols
    int var_s2; // not from debug symbols
    int var_v1_2; // not from debug symbols
    int flameSize; // not from debug symbols
    MonsterVars *mv = (MonsterVars*)instance->extraData; // not from debug symbols
    MonsterAttributes *ma = (MonsterAttributes*)instance->data; // not from debug symbols
    Instance *enemyInst; // not from debug symbols
    Instance *weaponInst; // not from debug symbols
    HunterVars *vars = (HunterVars*)mv->extraVars; // not from debug symbols
    MonsterIR *enemy = mv->enemy; // not from debug symbols
    char pad[8]; // not from debug symbols
    Position zero = { 0 };
    MATRIX *mat; // not from debug symbols
    int temp; // not from debug symbols
    int temp2; // not from debug symbols
    int temp3; // not from debug symbols

    weaponInst = INSTANCE_GetChildLinkedToSegment(instance, ma->leftWeaponSegment);
    
    if (vars != NULL && weaponInst != NULL)
    {
        mat = weaponInst->matrix;
        
        mypos.x = -mat[0].m[1][0];
        mypos.y = -mat[0].m[1][1];
        mypos.z = -mat[0].m[1][2];
        
        invAngle = MATH3D_AngleFromPosToPos(&zero, &mypos);

        mat = weaponInst->matrix;
        
        mypos.x = mat[segment].t[0];
        mypos.y = mat[segment].t[1];
        mypos.z = mat[segment].t[2];

        temp2 = 4096;
        
        temp3 = 4096 - invAngle;
       
        for (rem_time = vars->rem_time + gameTrackerX.timeMult; rem_time >= temp2; rem_time -= 4096) 
        {
            if (newPoint != 0) 
            {
                vars->angles[vars->endIdx] = temp3;
            } 
            else 
            {
                vars->angles[vars->endIdx] = -1;
            }
            
            vars->endIdx = (vars->endIdx + 1) & 0xF;
            
            if (vars->endIdx == vars->startIdx) 
            {
                if (vars->angles[vars->endIdx] == -1) 
                {
                    return 1;
                }
                vars->startIdx = (vars->startIdx + 1) & 0xF;
            }
        }
        
        vars->rem_time = rem_time;
    
        if (vars->startIdx != vars->endIdx) 
        {
            if (enemy != NULL) 
            {
                enemyInst = enemy->instance;
                    
                if (enemyInst->matrix != NULL)
                {
                    sp20.x = instance->position.x;
                    sp20.y = enemy->instance->position.y;
                    sp20.z = mypos.z;
                    
                    angle = MATH3D_AngleFromPosToPos(&instance->position, &enemyInst->position);

                    if (abs(mypos.z - enemyInst->matrix[1].t[2]) < 200) 
                    {
                        temp_ret = MATH3D_LengthXY(mypos.x - sp20.x, mypos.y - sp20.y);
                        
                        if (temp_ret < 1200) 
                        {

                            temp = ((temp_ret / 75) + 1);
                            temp_v1 = vars->angles[(vars->endIdx - temp) & 0xF];
    
                            if (temp_v1 != -1) 
                            {
                            
                                temp_v1_2 = (angle - temp_v1) & 0xFFF;
                                
                                if (temp_v1_2 <= 2048) 
                                {
                                    if (temp_v1_2 <= 80)
                                    {
                                        goto label;                                    
                                    }
                                }
                                else if (abs(temp_v1_2 - 4096) <= 80)
                                {
            label:
                                    damage *= 256;
                                    
                                    if (enemyInst == gameTrackerX.playerInstance) 
                                    {
                                        
                                        INSTANCE_Post(enemyInst, 0x01000000, SetMonsterHitData(instance, 0, damage, 0, 0));
                                    }
                                    else 
                                    {
                                        INSTANCE_Post(enemyInst, 0x0100000C, 0x20);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            
            var_s1 = 0;
            flameDist = 0;
            idx = vars->endIdx;
            var_s2 = 75;
            
            do 
            {
                idx = (idx - 1) & 0xF;
                flameAngle = vars->angles[idx];
                
                if (flameAngle != -1) 
                {
                    var_v1_2 = var_s1 / 8;
                    
                    if (var_v1_2 < 18) 
                    {
                        var_v1_2 = 18;
                    }
                    
                    while (flameDist < var_s2) 
                    {
                        flameSize = flameDist / 6;
                        if (flameSize < 37) 
                        {
                            flameSize = 37;
                        }
                        
                        FX_MakeHitFlame((SVector*)&mypos, 0, flameAngle, flameDist, flameSize);
                        flameDist += var_v1_2 / 2;
                    }
                }
                var_s2 += 75;
                var_s1 += 75;
            } while (idx != vars->startIdx);
            return 0;
        }
    }
    
    return 1;
}
#endif

void HUNTER_Init(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    HunterVars *vars; // not from debug symbols
    Instance *weaponInst; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;

    if (mv->age == 0)
    {
        weaponInst = HUMAN_CreateWeapon(instance, 13, ma->leftWeaponSegment);
    }
    else
    {
        weaponInst = HUMAN_CreateWeapon(instance, 14, ma->leftWeaponSegment);
    }

    if (weaponInst == NULL)
    {
        return;
    }

    HUMAN_Init(instance);
    mv->extraVars = NULL;

    if (mv->age != 0)
    {
        if (mv != NULL)
        {
            vars = (HunterVars *)MEMPACK_Malloc(sizeof(HunterVars), MEMORY_TYPE_HUNTERDATA);

            if (vars != NULL)
            {
                mv->extraVars = vars;
            }
        }
        mv->soulJuice = 24576;
    }
    else
    {
        mv->soulJuice = 20480;
    }
}

void HUNTER_CleanUp(Instance *instance)
{

    MonsterVars *mv; // not from debug symbols
    HunterVars *vars; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;

    if (mv != NULL)
    {
        vars = mv->extraVars;

        if (vars != NULL)
        {
            MEMPACK_Free((char *)vars);
        }
    }
    HUMAN_CleanUp(instance);
}

void HUNTER_ProjectileEntry(Instance *instance)
{
    MonsterVars *mv; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    MON_ProjectileEntry(instance);

    if (mv->age != 0)
    {
        mv->auxFlags &= ~1;
        mv->auxFlags &= ~2;
    }
}

void HUNTER_Projectile(Instance *instance)
{

    int mode; // not from debug symbols
    int dist; // not from debug symbols
    MonsterVars *mv; // not from debug symbols
    MonsterAttributes *ma; // not from debug symbols
    HunterVars *vars; // not from debug symbols
    HunterAttributes *attrs; // not from debug symbols
    MonsterMissile *missile; // not from debug symbols
    MonsterSubAttributes *subAttrs; // not from debug symbols

    mv = (MonsterVars *)instance->extraData;
    ma = (MonsterAttributes *)instance->data;
    subAttrs = mv->subAttr;
    vars = (HunterVars *)mv->extraVars;
    attrs = (HunterAttributes *)ma->tunData;
    missile = &ma->missileList[(int)subAttrs->combatAttributes->missileAttack];

    if (mv->age == 0)
    {
        MON_Projectile(instance);
        MON_DefaultQueueHandler(instance);
    }
    else if (vars != NULL && attrs != NULL)
    {
        if (!(mv->auxFlags & 1))
        {
            if (mv->enemy != NULL)
            {
                if (instance->flags2 & 0x10)
                {

                    mv->attackState++;

                    if (mv->attackState < missile->numAnims)
                    {
                        if (mv->attackState == missile->anim)
                        {
                            mode = 2;
                        }
                        else
                        {
                            mode = 1;
                        }
                        MON_PlayAnimFromList(instance, missile->animList, mv->attackState, mode);
                    }
                    else
                    {
                        MON_SwitchState(instance, MONSTER_STATE_COMBAT);
                    }
                }
                else if (MON_AnimPlayingFromList(instance, missile->animList, missile->anim) && G2EmulationInstanceQueryPassedFrame(instance, 0, missile->frame) != 0)
                {
                    mv->auxFlags |= 1;
                    HUNTER_InitFlamethrow(instance);
                    vars->timer = MON_GetTime(instance) + (attrs->timeToFire * 33);
                }
            }
            else
            {
                MON_SwitchState(instance, MONSTER_STATE_COMBAT);
            }
        }
        else if (mv->auxFlags & 2)
        {
            if (mv->enemy != NULL)
            {
                MON_TurnToPosition(instance, &mv->enemy->instance->position, subAttrs->speedPivotTurn);
            }

            if (HUNTER_Flamethrow(instance, missile->damage, 0, missile->segment) != 0)
            {

                mv->attackState++;

                if (mv->attackState < missile->numAnims)
                {
                    MON_PlayAnimFromList(instance, missile->animList, mv->attackState, 1);
                }
                else
                {
                    MON_SwitchState(instance, MONSTER_STATE_COMBAT);
                }
            }
        }
        else
        {

            HUNTER_Flamethrow(instance, missile->damage, 1, missile->segment);
            mv->auxFlags |= 2;

            if (mv->enemy != NULL)
            {

                dist = MATH3D_LengthXY(mv->enemy->instance->position.x - instance->position.x, mv->enemy->instance->position.y - instance->position.y);

                if ((unsigned long)vars->timer >= MON_GetTime(instance) || dist < 1200)
                {
                    mv->auxFlags &= ~2;
                }
                MON_TurnToPosition(instance, &mv->enemy->instance->position, mv->subAttr->speedPivotTurn);
            }
        }
        MON_DefaultQueueHandler(instance);
    }
}
