#ifndef _SKINBOS_H_
#define _SKINBOS_H_

#include "common.h"

// size: 0x20
typedef struct _skinbosAttributes
{
    // offset: 0x0000
    short hit_distance;
    // offset: 0x0002
    short stop_distance;
    // offset: 0x0004
    short forward_distance;
    // offset: 0x0006
    short max_allowed_damage;
    // offset: 0x0008
    short max_fadeout;
    // offset: 0x000A
    short min_phase_time;
    // offset: 0x000C
    short low_backstep;
    // offset: 0x000E
    short mid_backstep;
    // offset: 0x0010
    short high_backstep;
    // offset: 0x0012
    char gate_zero_center_marker;
    // offset: 0x0013
    char gate_zero_side_one_marker;
    // offset: 0x0014
    char gate_zero_side_two_marker;
    // offset: 0x0015
    char gate_zero_forward_marker;
    // offset: 0x0016
    char gate_one_center_marker;
    // offset: 0x0017
    char gate_one_side_one_marker;
    // offset: 0x0018
    char gate_one_side_two_marker;
    // offset: 0x0019
    char gate_one_forward_marker;
    // offset: 0x001A
    char masher_center_marker;
    // offset: 0x001B
    char masher_radius_marker_inner;
    // offset: 0x001C
    short masher_radius_marker_outer;
    // offset: 0x001E
    short junk;
} SkinbosAttributes;

// size: 0x30
typedef struct _SkinbosVars
{
    // offset: 0x0000
    int num_hits;
    // offset: 0x0004
    int phase_level;
    // offset: 0x0008
    int phase_hit_timer;
    // offset: 0x000C
    int fade_timer;
    // offset: 0x0010
    int last_hit_timer;
    // offset: 0x0014
    int stop_timer;
    // offset: 0x0018
    short fade_state;
    // offset: 0x001A
    short anim_state;
    // offset: 0x001C
    short target_angle;
    // offset: 0x001E
    short gate_drop_pos;
    // offset: 0x0020
    SVector normal;
    // offset: 0x0028
    SVector point;
} SkinbosVars;

void SKINBOS_Init(Instance *instance);
void SKINBOS_CleanUp(Instance *instance);
void SKINBOS_DamageEffect(Instance *instance, evFXHitData *data);
uintptr_t SKINBOS_Query(Instance *instance, unsigned long query);
void SKINBOS_Message(Instance *instance, unsigned long message, unsigned long data);
void SKINBOS_IdleEntry(Instance *instance);
void SKINBOS_Idle(Instance *instance);
void SKINBOS_WanderEntry(Instance *instance);
void SKINBOS_Wander(Instance *instance);
void SKINBOS_PursueEntry(Instance *instance);
void SKINBOS_Pursue(Instance *instance);
void SKINBOS_HitEntry(Instance *instance);
void SKINBOS_Hit(Instance *instance);
void SKINBOS_CombatEntry(Instance *instance);
void SKINBOS_Combat(Instance *instance);
void SKINBOS_LandOnFeetEntry(Instance *instance);
void SKINBOS_LandOnFeet(Instance *instance);
void SKINBOS_AttackEntry(Instance *instance);
void SKINBOS_Attack(Instance *instance);
void SKINBOS_DeadEntry(Instance *instance);
void SKINBOS_Dead(Instance *instance);
void SKINBOS_FleeEntry(Instance *instance);
void SKINBOS_Flee(Instance *instance);
void SKINBOS_StunnedEntry(Instance *instance);
void SKINBOS_Stunned(Instance *instance);

#endif
