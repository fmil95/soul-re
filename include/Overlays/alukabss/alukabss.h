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

#endif
