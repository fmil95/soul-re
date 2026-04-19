#ifndef _ALUKABSS_H_
#define _ALUKABSS_H_

#include "common.h"

// size: 0x10
typedef struct _alukabssAttributes
{
    // offset: 0x0000
    short spit_wedge;
    // offset: 0x0002
    short init_circle_speed;
    // offset: 0x0004
    short fast_circle_speed;
    // offset: 0x0006
    short max_circle_speed;
    // offset: 0x0008
    short circle_accel;
    // offset: 0x000A
    short raz_dist_from_marker;
    // offset: 0x000C
    short time_since_attack;
    // offset: 0x000E
    short time_since_spit;
} AlukabssAttributes;

// size: 0x1C
typedef struct _AlukabssVars
{
    // offset: 0x0000
    short upper_facing;
    // offset: 0x0002
    short combat_state;
    // offset: 0x0004
    int raz_time_at_marker;
    // offset: 0x0008
    int time_since_spit;
    // offset: 0x000C
    short markerx;
    // offset: 0x000E
    short markery;
    // offset: 0x0010
    int accumulated_angle;
    // offset: 0x0014
    int laugh_timer;
    // offset: 0x0018
    short last_angle;
    // offset: 0x001A
    short junk;
} AlukabssVars;

void ALUKABSS_Init(Instance *instance);
void ALUKABSS_CleanUp(Instance *instance);
void ALUKABSS_DamageEffect(Instance *instance, evFXHitData *data);
uintptr_t ALUKABSS_Query(Instance *instance, unsigned long query);
void ALUKABSS_Message(Instance *instance, unsigned long message, unsigned long data);
void ALUKABSS_PursueEntry(Instance *instance);
void ALUKABSS_Pursue(Instance *instance);
void ALUKABSS_CombatEntry(Instance *instance);
void ALUKABSS_Combat(Instance *instance);
void ALUKABSS_IdleEntry(Instance *instance);
void ALUKABSS_Idle(Instance *instance);
void ALUKABSS_LandInWaterEntry(Instance *instance);
void ALUKABSS_LandInWater(Instance *instance);
void ALUKABSS_ProjectileEntry(Instance *instance);
void ALUKABSS_Projectile(Instance *instance);
void ALUKABSS_AttackEntry(Instance *instance);
void ALUKABSS_Attack(Instance *instance);
void ALUKABSS_DeadEntry(Instance *instance);
void ALUKABSS_Dead(Instance *instance);
void ALUKABSS_DoNothingEntry(Instance *instance);
void ALUKABSS_DoNothing(Instance *instance);

#endif
