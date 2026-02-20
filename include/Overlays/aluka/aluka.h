#ifndef _ALUKA_H_
#define _ALUKA_H_

#include "common.h"

// size: 0x38
typedef struct _AlukaAttributes {
    // offset: 0x0000
    short too_long_since_attack;
    // offset: 0x0002
    short swimslow_speed;
    // offset: 0x0004
    short swimfast_speed;
    // offset: 0x0006
    short swimattack_speed;
    // offset: 0x0008
    short min_swim_depth;
    // offset: 0x000A
    short circle_start_dist;
    // offset: 0x000C
    short circle_dist;
    // offset: 0x000E
    short wander_range;
    // offset: 0x0010
    short twist_mult;
    // offset: 0x0012
    short attack_dist;
    // offset: 0x0014
    short attack_pos_bump;
    // offset: 0x0016
    short pitch_offset_speed;
    // offset: 0x0018
    short yaw_accel;
    // offset: 0x001A
    short pitch_accel;
    // offset: 0x001C
    short forward_accel;
    // offset: 0x001E
    short swimslow_yaw;
    // offset: 0x0020
    short swimfast_yaw;
    // offset: 0x0022
    short swimattack_yaw;
    // offset: 0x0024
    short swimspit_yaw;
    // offset: 0x0026
    short slow_pitch;
    // offset: 0x0028
    short swimfast_pitch;
    // offset: 0x002A
    short swimattack_pitch;
    // offset: 0x002C
    short swimspit_pitch;
    // offset: 0x002E
    short homing_dist;
    // offset: 0x0030
    char attack_on_frame;
    // offset: 0x0031
    char attack_off_frame;
    // offset: 0x0032
    char spit_attack_frame;
    // offset: 0x0033
    char start_jump_frame;
    // offset: 0x0034
    short daylight_depth;
    // offset: 0x0036
    short swim_smell_distance;
} AlukaAttributes;

#endif
