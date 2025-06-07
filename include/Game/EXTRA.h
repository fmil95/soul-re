#ifndef _EXTRA_H_
#define _EXTRA_H_

/* The purpose of this header is to include all of the structs from the game that can be found on the PDB file that was accidentally released with the remasters.
   These structs aren't present on the SYM file, but they are likely to have been used on the original codebase. */

enum EventObjectID
{
    EVENT_ID_NONE = 0,
    EVENT_ID_INSTANCE = 1,
    EVENT_ID_WILDCARD = 2,
    EVENT_ID_EVENT = 3,
    EVENT_ID_TGROUP = 4,
    EVENT_ID_AREA = 5,
    EVENT_ID_SIGNAL = 6,
    EVENT_ID_VMO_OBJECT = 7,
    EVENT_ID_MAX_TYPES = 8
};

#endif
