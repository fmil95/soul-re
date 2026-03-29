#ifndef _RONINBSS_H_
#define _RONINBSS_H_

#include "common.h"

// size: 0x26
typedef struct _roninbssAttributes
{
    // offset: 0x0000
    short resume_chase_range;
    // offset: 0x0002
    short stomp_min_range;
    // offset: 0x0004
    short stomp_max_range;
    // offset: 0x0006
    short allowed_stop_constrict_time;
    // offset: 0x0008
    short constrict_turn_rate;
    // offset: 0x000A
    short soul_suck_range;
    // offset: 0x000C
    short valve_constrict_dist;
    // offset: 0x000E
    short stomp_shake_frame;
    // offset: 0x0010
    short raz_stumble_time;
    // offset: 0x0012
    short time_between_armstomps;
    // offset: 0x0014
    short raz_hit_stumble_time;
    // offset: 0x0016
    short constrict_start_seg;
    // offset: 0x0018
    short constrict_end_seg;
    // offset: 0x001A
    short stand_marker;
    // offset: 0x001C
    short initialDrain;
    // offset: 0x001E
    short constrictDrain;
    // offset: 0x0020
    short armstompPauseTime;
    // offset: 0x0022
    short junk;
    // offset: 0x0024
    char valve_name[0x2];
} RoninbssAttributes;

// size: 0x30
typedef struct _RoninbssVars
{
    // offset: 0x0000
    int markerStreamUnitID;
    // offset: 0x0004
    int constrict_time;
    // offset: 0x0008
    int stumble_time;
    // offset: 0x000C
    short anim_state;
    // offset: 0x000E
    short current_marker_id;
    // offset: 0x0010
    short old_constrict_angle;
    // offset: 0x0012
    short total_constrict_angle;
    // offset: 0x0014
    Position current_constrict_pos;
    // offset 0x001A
    short faded_state;
    // offset 0x001C
    Position last_rb_pos;
    // offset 0x0022
    short to_what_plane;
    // offset 0x0024
    int faded_time;
    // offset 0x0028
    int hit_timer;
    // offset 0x002C
    int armstomp_time;
} RoninbssVars;

#endif
