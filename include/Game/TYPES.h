#ifndef _TYPES_H_
#define _TYPES_H_

typedef enum _G2Bool_Enum
{
    G2FALSE = 0,
} G2Bool;

typedef enum DEBUG_LINE_TYPE
{
    DEBUG_LINE_TYPE_BIT = 0,
    DEBUG_LINE_TYPE_LONG = 1,
    DEBUG_LINE_TYPE_ACTION = 2,
    DEBUG_LINE_TYPE_ACTION_WITH_LINE = 3,
    DEBUG_LINE_TYPE_MENU = 4,
    DEBUG_LINE_TYPE_ENDLIST = 5,
    DEBUG_LINE_TYPE_FORMAT = 6,
    DEBUG_LINE_TYPE_INFO = 7,
} DEBUG_LINE_TYPE;

typedef enum option_ctrl_t
{
    option_ctrl_bogus = 0,
    option_ctrl_none = 1,
    option_ctrl_select = 2,
    option_ctrl_left = 3,
    option_ctrl_right = 4,
    option_ctrl_num_states = 5,
} option_ctrl_t;

typedef enum _G2AnimCallbackMsg_Enum
{
    G2ANIM_MSG_DONE = 1,
    G2ANIM_MSG_LOOPPOINT = 2,
    G2ANIM_MSG_SECTION_INTERPDONE = 3,
    G2ANIM_MSG_SEGCTRLR_INTERPDONE = 4,
    G2ANIM_MSG_SWALARMSET = 5,
    G2ANIM_MSG_PLAYEFFECT = 6,
} G2AnimCallbackMsg;

enum MATH3D_AXIS
{
    AXIS_X = 0,
    AXIS_Y = 1,
    AXIS_Z = 2,
    AXIS_NEG_X = 3,
    AXIS_NEG_Y = 4,
    AXIS_NEG_Z = 5,
};

enum MonsterAnim
{
    MONSTER_ANIM_HIT1 = 0,
    MONSTER_ANIM_HIT2 = 1,
    MONSTER_ANIM_WALK = 2,
    MONSTER_ANIM_RUN = 3,
    MONSTER_ANIM_GRABBED = 4,
    MONSTER_ANIM_STUNNED = 5,
    MONSTER_ANIM_JUMPFORWARD = 6,
    MONSTER_ANIM_JUMPBACK = 7,
    MONSTER_ANIM_JUMPLEFT = 8,
    MONSTER_ANIM_JUMPRIGHT = 9,
    MONSTER_ANIM_STANCE_HEALTHY = 10,
    MONSTER_ANIM_STANCE_VULNERABLE = 11,
    MONSTER_ANIM_STUNNED_RECOVERY = 12,
    MONSTER_ANIM_IMPALED = 13,
    MONSTER_ANIM_STRUGGLE = 14,
    MONSTER_ANIM_BREAKHOLD = 15,
    MONSTER_ANIM_THROWN = 16,
    MONSTER_ANIM_IMPACT = 17,
    MONSTER_ANIM_LANDONFEET = 18,
    MONSTER_ANIM_FALL = 19,
    MONSTER_ANIM_BREAKFALL = 20,
    MONSTER_ANIM_SPINLAND = 21,
    MONSTER_ANIM_IMPALEDEATH = 22,
    MONSTER_ANIM_GETUP = 23,
    MONSTER_ANIM_GENERALDEATH = 24,
    MONSTER_ANIM_MISSILEIMPALEDEATH = 25,
    MONSTER_ANIM_SURPRISED = 26,
    MONSTER_ANIM_AGONY = 27,
    MONSTER_ANIM_PUPATE = 28,
    MONSTER_ANIM_NOTICE = 29,
    MONSTER_ANIM_SOULSUCK = 30,
    MONSTER_ANIM_IMPALEHORZ = 31,
    MONSTER_ANIM_IMPALEVERT = 32,
    MONSTER_ANIM_SOULOUTGRND = 33,
    MONSTER_ANIM_SOULOUTHORZ = 34,
    MONSTER_ANIM_SOULOUTVERT = 35,
    MONSTER_ANIM_FALLOVER = 36,
    MONSTER_ANIM_EMBRACE = 37,
    MONSTER_ANIM_SHOVE = 38,
    MONSTER_ANIM_FLIPSWITCH = 39,
    MONSTER_ANIM_EVADELEFT = 40,
    MONSTER_ANIM_EVADERIGHT = 41,
    MONSTER_ANIM_ALARM = 42,
    MONSTER_ANIM_DEAF = 43,
    MONSTER_ANIM_BLIND = 44,
    MONSTER_ANIM_SUCKED = 45,
    MONSTER_NUM_ANIMS = 46,
};

enum MonsterState
{
    MONSTER_STATE_BIRTH = 0,
    MONSTER_STATE_PURSUE = 1,
    MONSTER_STATE_IDLE = 2,
    MONSTER_STATE_MISSILEHIT = 3,
    MONSTER_STATE_FALL = 4,
    MONSTER_STATE_WANDER = 5,
    MONSTER_STATE_ATTACK = 6,
    MONSTER_STATE_IMPALEDEATH = 7,
    MONSTER_STATE_HIT = 8,
    MONSTER_STATE_STUNNED = 9,
    MONSTER_STATE_GRABBED = 10,
    MONSTER_STATE_THROWN = 11,
    MONSTER_STATE_IMPACT = 12,
    MONSTER_STATE_COMBAT = 13,
    MONSTER_STATE_BREAKHOLD = 14,
    MONSTER_STATE_LANDONFEET = 15,
    MONSTER_STATE_GENERALDEATH = 16,
    MONSTER_STATE_ENVIRONMENTDAMAGE = 17,
    MONSTER_STATE_LANDINWATER = 18,
    MONSTER_STATE_FLEE = 19,
    MONSTER_STATE_HIDE = 20,
    MONSTER_STATE_SURPRISEATTACK = 21,
    MONSTER_STATE_PARRY = 22,
    MONSTER_STATE_DEAD = 23,
    MONSTER_STATE_SURPRISED = 24,
    MONSTER_STATE_NOTICE = 25,
    MONSTER_STATE_PUPATE = 26,
    MONSTER_STATE_EMBRACE = 27,
    MONSTER_STATE_PROJECTILE = 28,
    MONSTER_STATE_TERRAINIMPALEDEATH = 29,
    MONSTER_STATE_PETRIFIED = 30,
    MONSTER_NUM_STATES = 31,
};

typedef enum language_t
{
    language_english = 0,
    language_french = 1,
    language_german = 2,
    language_italian = 3,
    language_spanish = 4,
    language_num_states = 5,
    language_default = 0xFFFFFFFF,
} language_t;

typedef enum localstr_t
{
    LOCALSTR_BOGUS = 0,
    LOCALSTR_format_card = 1,
    LOCALSTR_accessing_card = 2,
    LOCALSTR_back = 3,
    LOCALSTR_cancel = 4,
    LOCALSTR_choose_load_file = 5,
    LOCALSTR_choose_save_file = 6,
    LOCALSTR_confirm = 7,
    LOCALSTR_continue = 8,
    LOCALSTR_controller = 9,
    LOCALSTR_create_file = 10,
    LOCALSTR_creating = 11,
    LOCALSTR_demos = 12,
    LOCALSTR_do_not_remove_card = 13,
    LOCALSTR_done = 14,
    LOCALSTR_exit_game = 15,
    LOCALSTR_formatting = 16,
    LOCALSTR_go_back = 17,
    LOCALSTR_insert_card = 18,
    LOCALSTR_insert_card_free = 19,
    LOCALSTR_invalid_card = 20,
    LOCALSTR_load_failed = 21,
    LOCALSTR_load_game = 22,
    LOCALSTR_loading = 23,
    LOCALSTR_music = 24,
    LOCALSTR_new_game = 25,
    LOCALSTR_no = 26,
    LOCALSTR_no_card = 27,
    LOCALSTR_card_full = 28,
    LOCALSTR_card_not_enough_blocks = 29,
    LOCALSTR_ok = 30,
    LOCALSTR_options = 31,
    LOCALSTR_overwrite_game = 32,
    LOCALSTR_paused = 33,
    LOCALSTR_press_start = 34,
    LOCALSTR_query_format_card = 35,
    LOCALSTR_query_overwrite = 36,
    LOCALSTR_query_quit = 37,
    LOCALSTR_quit_game = 38,
    LOCALSTR_resume_game = 39,
    LOCALSTR_save_failed = 40,
    LOCALSTR_save_game = 41,
    LOCALSTR_save_ok = 42,
    LOCALSTR_saving = 43,
    LOCALSTR_sound = 44,
    LOCALSTR_start_game = 45,
    LOCALSTR_start_new_game = 46,
    LOCALSTR_try_card_again = 47,
    LOCALSTR_unformatted_card = 48,
    LOCALSTR_vibration_off = 49,
    LOCALSTR_vibration_on = 50,
    LOCALSTR_voice = 51,
    LOCALSTR_warning_one = 52,
    LOCALSTR_warning_two = 53,
    LOCALSTR_yes = 54,
    LOCALSTR_Hint0 = 55,
    LOCALSTR_Hint1 = 56,
    LOCALSTR_Hint2 = 57,
    LOCALSTR_Hint3 = 58,
    LOCALSTR_Hint4 = 59,
    LOCALSTR_Hint5 = 60,
    LOCALSTR_Hint6 = 61,
    LOCALSTR_Hint7 = 62,
    LOCALSTR_Hint8 = 63,
    LOCALSTR_Hint9 = 64,
    LOCALSTR_Hint10 = 65,
    LOCALSTR_Hint11 = 66,
    LOCALSTR_Hint12 = 67,
    LOCALSTR_Hint13 = 68,
    LOCALSTR_Hint14 = 69,
    LOCALSTR_Hint15 = 70,
    LOCALSTR_Hint16 = 71,
    LOCALSTR_Hint17 = 72,
    LOCALSTR_Hint18 = 73,
    LOCALSTR_Hint19 = 74,
    LOCALSTR_Hint20 = 75,
    LOCALSTR_Hint21 = 76,
    LOCALSTR_Hint22 = 77,
    LOCALSTR_Hint23 = 78,
    LOCALSTR_Hint24 = 79,
    LOCALSTR_Hint25 = 80,
    LOCALSTR_Hint26 = 81,
    LOCALSTR_Hint27 = 82,
    LOCALSTR_Hint28 = 83,
    LOCALSTR_Hint29 = 84,
    LOCALSTR_Hint30 = 85,
    LOCALSTR_Hint31 = 86,
    LOCALSTR_Hint32 = 87,
    LOCALSTR_Hint33 = 88,
    LOCALSTR_Hint34 = 89,
    LOCALSTR_Hint35 = 90,
    LOCALSTR_Hint36 = 91,
    LOCALSTR_Hint37 = 92,
    LOCALSTR_Hint38 = 93,
    LOCALSTR_Hint39 = 94,
    LOCALSTR_Hint40 = 95,
    LOCALSTR_Hint41 = 96,
    LOCALSTR_Hint42 = 97,
    LOCALSTR_Hint43 = 98,
    LOCALSTR_Hint44 = 99,
    LOCALSTR_Hint45 = 100,
    LOCALSTR_Hint46 = 101,
    LOCALSTR_Hint47 = 102,
    LOCALSTR_Hint48 = 103,
    LOCALSTR_Hint49 = 104,
    LOCALSTR_Hint50 = 105,
    LOCALSTR_Hint51 = 106,
    LOCALSTR_Hint52 = 107,
    LOCALSTR_EndOfHintStrings = 108,
    LOCALSTR_no_controller = 109,
    LOCALSTR_NUM_ITEMS = 110,
} localstr_t;

typedef enum menu_ctrl_t
{
    menu_ctrl_none = 0,
    menu_ctrl_up = 1,
    menu_ctrl_down = 2,
    menu_ctrl_left = 3,
    menu_ctrl_right = 4,
    menu_ctrl_engage = 5,
    menu_ctrl_cancel = 6,
    menu_ctrl_start = 7,
    menu_ctrl_num_items = 8,
} menu_ctrl_t;

typedef enum menu_sound_t
{
    menu_sound_none = 0,
    menu_sound_default = 1,
    menu_sound_select = 2,
    menu_sound_adjust = 3,
    menu_sound_engage = 4,
    menu_sound_pop = 5,
} menu_sound_t;

typedef enum sfx_t
{
    sfx_sound = 0,
    sfx_music = 1,
    sfx_voice = 2,
    sfx_error = 3,
} sfx_t;

typedef enum bdepth
{
    TIM_4BIT = 0,
    TIM_8BIT = 1,
    TIM_16BIT = 2,
} bdepth;

typedef enum mcpsx_err_t
{
    mcpsx_err_none = 0,
    mcpsx_err_card_not_exist = 1,
    mcpsx_err_card_invalid = 2,
    mcpsx_err_new_card = 3,
    mcpsx_err_not_format = 4,
    mcpsx_err_file_not_exist = 5,
    mcpsx_err_already_exists = 6,
    mcpsx_err_block_full = 7,
    mcpsx_err_busy = 8,
    mcpsx_err_num_items = 9,
} mcpsx_err_t;

typedef enum mcpsx_func_t
{
    func_none = 0,
    func_exist = 1,
    func_accept = 2,
    func_read_file = 3,
    func_write_file = 4,
    func_create_file = 5,
    func_delete_file = 6,
    func_format = 7,
    func_unformat = 8,
    func_directory = 9,
    func_num_items = 10,
} mcpsx_func_t;

typedef enum mcpsx_sync_t
{
    sync_busy = 0,
    sync_done = 1,
    sync_func = 2,
    sync_num_items = 3,
    sync_idle = 0xFFFFFFFF,
} mcpsx_sync_t;

typedef enum mcpsx_mode_t
{
    mode_bogus = 0,
    mode_initialized = 1,
    mode_running = 2,
    mode_terminated = 3,
    mode_num_items = 4,
} mcpsx_mode_t;

typedef enum state_t
{
    fsmcard_card_not_exist = 0,
    fsmcard_card_invalid = 1,
    fsmcard_not_format = 2,
    fsmcard_new_card = 3,
    fsmcard_directory = 4,
    fsmcard_ready = 5,
    fsmcard_format = 6,
    fsmcard_unformat = 7,
    fsmcard_delete = 8,
    fsmcard_create = 9,
    fsmcard_write = 10,
    fsmcard_read = 11,
    fsmcard_error = 12,
    fsmcard_num_items = 13,
} fsmcard_t;

typedef enum mcard_status_t
{
    mcard_status_card_not_exist = 0,
    mcard_status_card_invalid = 1,
    mcard_status_not_format = 2,
    mcard_status_new_card = 3,
    mcard_status_ready = 4,
    mcard_status_accessing = 5,
    mcard_status_error = 6,
    mcard_status_num_items = 7,
} mcard_status_t;

typedef enum fsm_t
{
    fsm_card_not_exist = 0,
    fsm_card_invalid = 1,
    fsm_not_format = 2,
    fsm_new_card = 3,
    fsm_ready = 4,
    fsm_format = 5,
    fsm_start = 6,
    fsm_loading = 7,
    fsm_query_overwrite = 8,
    fsm_overwrite = 9,
    fsm_saving = 10,
    fsm_create = 11,
    fsm_save_complete = 12,
    fsm_num_items = 13,
} fsm_t;

// size: 0xC0
struct TCB
{
    // offset: 0x0000
    long status;
    // offset: 0x0004
    long mode;
    // offset: 0x0008 (160 bytes)
    unsigned long reg[40];
    // offset: 0x00A8 (24 bytes)
    long system[6];
};

// size: 0x3C
struct EXEC
{
    // offset: 0x0000
    unsigned long pc0;
    // offset: 0x0004
    unsigned long gp0;
    // offset: 0x0008
    unsigned long t_addr;
    // offset: 0x000C
    unsigned long t_size;
    // offset: 0x0010
    unsigned long d_addr;
    // offset: 0x0014
    unsigned long d_size;
    // offset: 0x0018
    unsigned long b_addr;
    // offset: 0x001C
    unsigned long b_size;
    // offset: 0x0020
    unsigned long s_addr;
    // offset: 0x0024
    unsigned long s_size;
    // offset: 0x0028
    unsigned long sp;
    // offset: 0x002C
    unsigned long fp;
    // offset: 0x0030
    unsigned long gp;
    // offset: 0x0034
    unsigned long ret;
    // offset: 0x0038
    unsigned long base;
};

// size: 0x28
struct DIRENTRY
{
    // offset: 0x0000 (20 bytes)
    char name[20];
    // offset: 0x0014
    long attr;
    // offset: 0x0018
    long size;
    // offset: 0x001C (4 bytes)
    struct DIRENTRY *next;
    // offset: 0x0020
    long head;
    // offset: 0x0024 (4 bytes)
    char system[4];
};

// size: 0x20
typedef struct MATRIX
{
    // offset: 0x0000 (18 bytes)
    short m[3][3];
    // offset: 0x0014 (12 bytes)
    long t[3];
} MATRIX, Transform;

// size: 0x10
typedef struct VECTOR
{
    // offset: 0x0000
    long vx;
    // offset: 0x0004
    long vy;
    // offset: 0x0008
    long vz;
    // offset: 0x000C
    long pad;
} VECTOR;

// size: 0x8
typedef struct SVECTOR
{
    // offset: 0x0000
    short vx;
    // offset: 0x0002
    short vy;
    // offset: 0x0004
    short vz;
    // offset: 0x0006
    short pad;
} SVECTOR;

// size: 0x4
typedef struct CVECTOR
{
    // offset: 0x0000
    unsigned char r;
    // offset: 0x0001
    unsigned char g;
    // offset: 0x0002
    unsigned char b;
    // offset: 0x0003
    unsigned char cd;
} CVECTOR;

// size: 0x4
typedef struct DVECTOR
{
    // offset: 0x0000
    short vx;
    // offset: 0x0002
    short vy;
} DVECTOR;

// size: 0x18
typedef struct RVECTOR
{
    // offset: 0x0000 (8 bytes)
    SVECTOR v;
    // offset: 0x0008 (2 bytes)
    unsigned char uv[2];
    // offset: 0x000A
    unsigned short pad;
    // offset: 0x000C (4 bytes)
    CVECTOR c;
    // offset: 0x0010 (4 bytes)
    DVECTOR sxy;
    // offset: 0x0014
    unsigned long sz;
} RVECTOR;

// size: 0x58
typedef struct CRVECTOR3
{
    // offset: 0x0000 (24 bytes)
    RVECTOR r01;
    // offset: 0x0018 (24 bytes)
    RVECTOR r12;
    // offset: 0x0030 (24 bytes)
    RVECTOR r20;
    // offset: 0x0048 (4 bytes)
    RVECTOR *r0;
    // offset: 0x004C (4 bytes)
    RVECTOR *r1;
    // offset: 0x0050 (4 bytes)
    RVECTOR *r2;
    // offset: 0x0054
    unsigned long *rtn;
} CRVECTOR3;

// size: 0x8C
typedef struct CRVECTOR4
{
    // offset: 0x0000 (24 bytes)
    RVECTOR r01;
    // offset: 0x0018 (24 bytes)
    RVECTOR r02;
    // offset: 0x0030 (24 bytes)
    RVECTOR r31;
    // offset: 0x0048 (24 bytes)
    RVECTOR r32;
    // offset: 0x0060 (24 bytes)
    RVECTOR rc;
    // offset: 0x0078 (4 bytes)
    RVECTOR *r0;
    // offset: 0x007C (4 bytes)
    RVECTOR *r1;
    // offset: 0x0080 (4 bytes)
    RVECTOR *r2;
    // offset: 0x0084 (4 bytes)
    RVECTOR *r3;
    // offset: 0x0088
    unsigned long *rtn;
} CRVECTOR4;

// size: 0x8
typedef struct RECT
{
    // offset: 0x0000
    short x;
    // offset: 0x0002
    short y;
    // offset: 0x0004
    short w;
    // offset: 0x0006
    short h;
} RECT;

// size: 0x40
typedef struct DR_ENV
{
    // offset: 0x0000
    unsigned long tag;
    // offset: 0x0004 (60 bytes)
    unsigned long code[15];
} DR_ENV;

// size: 0x8
typedef struct P_TAG
{
    // offset: 0x0000
    unsigned int addr : 24;
    // offset: 0x0003
    unsigned int len : 8;
    // offset: 0x0004
    unsigned char r0;
    // offset: 0x0005
    unsigned char g0;
    // offset: 0x0006
    unsigned char b0;
    // offset: 0x0007
    unsigned char code;
} P_TAG;

// size: 0x4
typedef struct CdlLOC
{
    // offset: 0x0000
    unsigned char minute;
    // offset: 0x0001
    unsigned char second;
    // offset: 0x0002
    unsigned char sector;
    // offset: 0x0003
    unsigned char track;
} CdlLOC;

// size: 0x8
typedef struct NodeType
{
    // offset: 0x0000 (4 bytes)
    struct NodeType *prev;
    // offset: 0x0004 (4 bytes)
    struct NodeType *next;
} NodeType;

// size: 0x6
typedef struct _Vertex
{
    // offset: 0x0000
    short x;
    // offset: 0x0002
    short y;
    // offset: 0x0004
    short z;
} Vertex;

// size: 0x8
typedef struct _MVertex
{
    // offset: 0x0000 (6 bytes)
    Vertex vertex;
    // offset: 0x0006
    unsigned short normal;
} MVertex;

// size: 0x6
typedef struct _SVectorNoPad
{
    // offset: 0x0000
    short x;
    // offset: 0x0002
    short y;
    // offset: 0x0004
    short z;
} SVectorNoPad;

// size: 0x6
typedef struct _Face
{
    // offset: 0x0000
    unsigned short v0;
    // offset: 0x0002
    unsigned short v1;
    // offset: 0x0004
    unsigned short v2;
} Face;

// size: 0xC
typedef struct _MFace
{
    // offset: 0x0000 (6 bytes)
    Face face;
    // offset: 0x0006
    unsigned char normal;
    // offset: 0x0007
    unsigned char flags;
    // offset: 0x0008
    long color;
} MFace;

// size: 0xC
typedef struct _HFace
{
    // offset: 0x0000
    short v0;
    // offset: 0x0002
    short v1;
    // offset: 0x0004
    short v2;
    // offset: 0x0006
    unsigned char attr;
    // offset: 0x0007
    char pad;
    // offset: 0x0008
    unsigned short normal;
    // offset: 0x000A
    unsigned short n0;
} HFace;

// size: 0x6
typedef struct _Position
{
    // offset: 0x0000
    short x;
    // offset: 0x0002
    short y;
    // offset: 0x0004
    short z;
} Position;

// size: 0x14
typedef struct _HSphere
{
    // offset: 0x0000
    long attr;
    // offset: 0x0004
    unsigned char id;
    // offset: 0x0005
    unsigned char rank;
    // offset: 0x0006
    short pad;
    // offset: 0x0008 (6 bytes)
    Position position;
    // offset: 0x000E
    unsigned short radius;
    // offset: 0x0010
    unsigned long radiusSquared;
} HSphere;

// size: 0x1C
typedef struct _HBox
{
    // offset: 0x0000
    short flags;
    // offset: 0x0002
    unsigned char id;
    // offset: 0x0003
    unsigned char rank;
    // offset: 0x0004
    short minX;
    // offset: 0x0006
    short minY;
    // offset: 0x0008
    short minZ;
    // offset: 0x000A
    short maxX;
    // offset: 0x000C
    short maxY;
    // offset: 0x000E
    short maxZ;
    // offset: 0x0010
    short refMinX;
    // offset: 0x0012
    short refMinY;
    // offset: 0x0014
    short refMinZ;
    // offset: 0x0016
    short refMaxX;
    // offset: 0x0018
    short refMaxY;
    // offset: 0x001A
    short refMaxZ;
} HBox;

// size: 0x18
typedef struct _HInfo
{
    // offset: 0x0000
    long numHFaces;
    // offset: 0x0004 (4 bytes)
    HFace *hfaceList;
    // offset: 0x0008
    long numHSpheres;
    // offset: 0x000C (4 bytes)
    HSphere *hsphereList;
    // offset: 0x0010
    long numHBoxes;
    // offset: 0x0014 (4 bytes)
    HBox *hboxList;
} HInfo;

// size: 0x18
typedef struct _Segment
{
    // offset: 0x0000
    long flags;
    // offset: 0x0004
    short firstTri;
    // offset: 0x0006
    short lastTri;
    // offset: 0x0008
    short firstVertex;
    // offset: 0x000A
    short lastVertex;
    // offset: 0x000C
    short px;
    // offset: 0x000E
    short py;
    // offset: 0x0010
    short pz;
    // offset: 0x0012
    short parent;
    // offset: 0x0014 (4 bytes)
    HInfo *hInfo;
} Segment;

// size: 0x10
typedef struct TextureMT3
{
    // offset: 0x0000
    unsigned char u0;
    // offset: 0x0001
    unsigned char v0;
    // offset: 0x0002
    unsigned short clut;
    // offset: 0x0004
    unsigned char u1;
    // offset: 0x0005
    unsigned char v1;
    // offset: 0x0006
    unsigned short tpage;
    // offset: 0x0008
    unsigned char u2;
    // offset: 0x0009
    unsigned char v2;
    // offset: 0x000A
    char pad1;
    // offset: 0x000B
    char sortPush;
    // offset: 0x000C
    long color;
} TextureMT3;

// size: 0xC
typedef struct AniTexInfo
{
    // offset: 0x0000 (4 bytes)
    TextureMT3 *texture;
    // offset: 0x0004
    long numFrames;
    // offset: 0x0008
    long speed;
} AniTexInfo;

// size: 0x10
typedef struct AniTex
{
    // offset: 0x0000
    long numAniTextues;
    // offset: 0x0004 (12 bytes)
    AniTexInfo aniTexInfo;
} AniTex;

// size: 0x6
typedef struct vecS
{
    // offset: 0x0000
    short x;
    // offset: 0x0002
    short y;
    // offset: 0x0004
    short z;
} vecS;

// size: 0xC
typedef struct vecL
{
    // offset: 0x0000
    long x;
    // offset: 0x0004
    long y;
    // offset: 0x0008
    long z;
} vecL;

// size: 0x20
typedef struct SplineKey
{
    // offset: 0x0000
    short count;
    // offset: 0x0002 (6 bytes)
    vecS point;
    // offset: 0x0008 (12 bytes)
    vecL dd;
    // offset: 0x0014 (12 bytes)
    vecL ds;
} SplineKey;

// size: 0x8
typedef struct Spline
{
    // offset: 0x0000 (4 bytes)
    SplineKey *key;
    // offset: 0x0004
    short numkeys;
    // offset: 0x0006
    unsigned char type;
    // offset: 0x0007
    unsigned char flags;
} Spline;

// size: 0x8
typedef struct _G2Quat_Type
{
    // offset: 0x0000
    short x;
    // offset: 0x0002
    short y;
    // offset: 0x0004
    short z;
    // offset: 0x0006
    short w;
} G2Quat;

// size: 0xA
typedef struct SplineRotKey
{
    // offset: 0x0000
    short count;
    // offset: 0x0002 (8 bytes)
    G2Quat q;
} SplineRotKey;

// size: 0x8
typedef struct RSpline
{
    // offset: 0x0000 (4 bytes)
    SplineRotKey *key;
    // offset: 0x0004
    short numkeys;
    // offset: 0x0006
    unsigned char type;
    // offset: 0x0007
    unsigned char flags;
} RSpline;

// size: 0x8
typedef struct SplineDef
{
    // offset: 0x0000
    short currkey;
    // offset: 0x0002
    unsigned short denomFlag;
    // offset: 0x0004
    long fracCurr;
} SplineDef;

// size: 0x50
typedef struct MultiSpline
{
    // offset: 0x0000 (4 bytes)
    Spline *positional;
    // offset: 0x0004 (4 bytes)
    RSpline *rotational;
    // offset: 0x0008 (4 bytes)
    Spline *scaling;
    // offset: 0x000C (4 bytes)
    Spline *color;
    // offset: 0x0010 (8 bytes)
    SplineDef curPositional;
    // offset: 0x0018 (8 bytes)
    SplineDef curRotational;
    // offset: 0x0020 (8 bytes)
    SplineDef curScaling;
    // offset: 0x0028 (8 bytes)
    SplineDef curColor;
    // offset: 0x0030 (32 bytes)
    MATRIX curRotMatrix;
} MultiSpline;

// size: 0x38
typedef struct _Model
{
    // offset: 0x0000
    long numVertices;
    // offset: 0x0004 (4 bytes)
    MVertex *vertexList;
    // offset: 0x0008
    long numNormals;
    // offset: 0x000C (4 bytes)
    SVectorNoPad *normalList;
    // offset: 0x0010
    long numFaces;
    // offset: 0x0014 (4 bytes)
    MFace *faceList;
    // offset: 0x0018
    long numSegments;
    // offset: 0x001C (4 bytes)
    Segment *segmentList;
    // offset: 0x0020 (4 bytes)
    AniTex *aniTextures;
    // offset: 0x0024
    short maxRad;
    // offset: 0x0026
    short pad;
    // offset: 0x0028
    long maxRadSq;
    // offset: 0x002C (4 bytes)
    MultiSpline *multiSpline;
    // offset: 0x0030 (4 bytes)
    TextureMT3 *startTextures;
    // offset: 0x0034 (4 bytes)
    TextureMT3 *endTextures;
} Model;

// size: 0x4
typedef struct _G2AnimFxHeader_Type
{
    // offset: 0x0000
    unsigned char sizeAndSection;
    // offset: 0x0001
    char type;
    // offset: 0x0002
    unsigned short keyframeID;
} G2AnimFxHeader;

// size: 0x14
typedef struct _G2AnimKeylist_Type
{
    // offset: 0x0000
    unsigned char sectionCount;
    // offset: 0x0001
    unsigned char s0TailTime;
    // offset: 0x0002
    unsigned char s1TailTime;
    // offset: 0x0003
    unsigned char s2TailTime;
    // offset: 0x0004
    unsigned short keyCount;
    // offset: 0x0006
    short timePerKey;
    // offset: 0x0008
    unsigned short pad00;
    // offset: 0x000A
    short pad01;
    // offset: 0x000C
    unsigned short pad10;
    // offset: 0x000E
    short pad11;
    // offset: 0x0010 (4 bytes)
    G2AnimFxHeader *fxList;
    // offset: 0x0014
    unsigned short *sectionData[0];
} G2AnimKeylist;

// size: 0x4
typedef struct ObjectEffect
{
    // offset: 0x0000
    unsigned char effectNumber;
    // offset: 0x0001 (3 bytes)
    unsigned char modifierList[3];
} ObjectEffect, AnimEffectData;

// size: 0x8
typedef struct VramSize
{
    // offset: 0x0000
    short x;
    // offset: 0x0002
    short y;
    // offset: 0x0004
    short w;
    // offset: 0x0006
    short h;
} VramSize;

// size: 0x4C
typedef struct Object
{
    // offset: 0x0000
    long oflags;
    // offset: 0x0004
    short id;
    // offset: 0x0006
    short sfxFileHandle;
    // offset: 0x0008
    short numModels;
    // offset: 0x000A
    short numAnims;
    // offset: 0x000C (4 bytes)
    Model **modelList;
    // offset: 0x0010 (4 bytes)
    G2AnimKeylist **animList;
    // offset: 0x0014
    short introDist;
    // offset: 0x0016
    short vvIntroDist;
    // offset: 0x0018
    short removeDist;
    // offset: 0x001A
    short vvRemoveDist;
    // offset: 0x001C
    void *data;
    // offset: 0x0020
    char *script;
    // offset: 0x0024
    char *name;
    // offset: 0x0028
    unsigned char *soundData;
    // offset: 0x002C
    long oflags2;
    // offset: 0x0030
    short sectionA;
    // offset: 0x0032
    short sectionB;
    // offset: 0x0034
    short sectionC;
    // offset: 0x0036
    short numberOfEffects;
    // offset: 0x0038 (4 bytes)
    ObjectEffect *effectList;
    // offset: 0x003C
    unsigned long *relocList;
    // offset: 0x0040
    void *relocModule;
    // offset: 0x0044 (8 bytes)
    VramSize vramSize;
} Object;

// size: 0x8
typedef struct _Rotation
{
    // offset: 0x0000
    short x;
    // offset: 0x0002
    short y;
    // offset: 0x0004
    short z;
    // offset: 0x0006
    short pad;
} Rotation;

// size: 0x4C
typedef struct Intro
{
    // offset: 0x0000 (16 bytes)
    char name[16];
    // offset: 0x0010
    long intronum;
    // offset: 0x0014
    long UniqueID;
    // offset: 0x0018 (8 bytes)
    Rotation rotation;
    // offset: 0x0020 (6 bytes)
    Position position;
    // offset: 0x0026
    short maxRad;
    // offset: 0x0028
    long maxRadSq;
    // offset: 0x002C
    long flags;
    // offset: 0x0030
    void *data;
    // offset: 0x0034 (4 bytes)
    struct _Instance *instance;
    // offset: 0x0038 (4 bytes)
    MultiSpline *multiSpline;
    // offset: 0x003C
    void *dsignal;
    // offset: 0x0040
    short specturalLightGroup;
    // offset: 0x0042
    short meshColor;
    // offset: 0x0044 (6 bytes)
    Position spectralPosition;
    // offset: 0x004A
    short spad;
} Intro;

// size: 0x3
typedef struct SoundInstance
{
    // offset: 0x0000
    unsigned char channel;
    // offset: 0x0001
    unsigned char state;
    // offset: 0x0002
    unsigned char delay;
} SoundInstance;

// size: 0x8
typedef struct _SVector
{
    // offset: 0x0000
    short x;
    // offset: 0x0002
    short y;
    // offset: 0x0004
    short z;
    // offset: 0x0006
    short pad;
} SVector;

// size: 0xC
typedef struct _TFace
{
    // offset: 0x0000 (6 bytes)
    Face face;
    // offset: 0x0006
    unsigned char attr;
    // offset: 0x0007
    char sortPush;
    // offset: 0x0008
    unsigned short normal;
    // offset: 0x000A
    unsigned short textoff;
} TFace;

// size: 0x8
typedef struct _Normal
{
    // offset: 0x0000
    short x;
    // offset: 0x0002
    short y;
    // offset: 0x0004
    short z;
    // offset: 0x0006
    short pad;
} Normal;

// size: 0x8
typedef struct _HPrim
{
    // offset: 0x0000
    unsigned char hpFlags;
    // offset: 0x0001
    unsigned char withFlags;
    // offset: 0x0002
    unsigned char type;
    // offset: 0x0003
    unsigned char segment;
    // offset: 0x0004 (4 bytes)
    union
    {
        // offset: 0x0000 (4 bytes)
        HSphere *hsphere;
        // offset: 0x0000 (4 bytes)
        HBox *hbox;
        // offset: 0x0000 (4 bytes)
        HFace *hface;
    } data;
} HPrim;

// size: 0x8
typedef struct _HModel
{
    // offset: 0x0000 (4 bytes)
    HPrim *hPrimList;
    // offset: 0x0004
    long numHPrims;
} HModel;

// size: 0xC
typedef struct _TVertex
{
    // offset: 0x0000 (6 bytes)
    Vertex vertex;
    // offset: 0x0006
    unsigned short rgb15;
    // offset: 0x0008
    unsigned char r0;
    // offset: 0x0009
    unsigned char g0;
    // offset: 0x000A
    unsigned char b0;
    // offset: 0x000B
    unsigned char code;
} TVertex;

// size: 0x8
typedef struct DrMoveAniTexSrcInfo
{
    // offset: 0x0000
    short pixSrcX;
    // offset: 0x0002
    short pixSrcY;
    // offset: 0x0004
    short clutSrcX;
    // offset: 0x0006
    short clutSrcY;
} DrMoveAniTexSrcInfo;

// size: 0x28
typedef struct DrMoveAniTexDestInfo
{
    // offset: 0x0000
    short pixDstX;
    // offset: 0x0002
    short pixDstY;
    // offset: 0x0004
    short pixW;
    // offset: 0x0006
    short pixH;
    // offset: 0x0008
    short clutDstX;
    // offset: 0x000A
    short clutDstY;
    // offset: 0x000C
    short clutW;
    // offset: 0x000E
    short clutH;
    // offset: 0x0010
    short pixCurrentX;
    // offset: 0x0012
    short pixCurrentY;
    // offset: 0x0014
    short clutCurrentX;
    // offset: 0x0016
    short clutCurrentY;
    // offset: 0x0018
    long numFrames;
    // offset: 0x001C
    long speed;
    // offset: 0x0020 (8 bytes)
    DrMoveAniTexSrcInfo frame;
} DrMoveAniTexDestInfo;

// size: 0x8
typedef struct DrMoveAniTex
{
    // offset: 0x0000
    long numAniTextues;
    // offset: 0x0004 (4 bytes)
    DrMoveAniTexDestInfo *aniTexInfo;
} DrMoveAniTex;

// size: 0xC
typedef struct TextureFT3
{
    // offset: 0x0000
    unsigned char u0;
    // offset: 0x0001
    unsigned char v0;
    // offset: 0x0002
    unsigned short clut;
    // offset: 0x0004
    unsigned char u1;
    // offset: 0x0005
    unsigned char v1;
    // offset: 0x0006
    unsigned short tpage;
    // offset: 0x0008
    unsigned char u2;
    // offset: 0x0009
    unsigned char v2;
    // offset: 0x000A
    unsigned short attr;
} TextureFT3;

// size: 0xE
typedef struct _MorphVertex
{
    // offset: 0x0000
    short x;
    // offset: 0x0002
    short y;
    // offset: 0x0004
    short z;
    // offset: 0x0006
    short vindex;
    // offset: 0x0008
    short hx;
    // offset: 0x000A
    short hy;
    // offset: 0x000C
    short hz;
} MorphVertex;

// size: 0x2
typedef struct _MorphColor
{
    // offset: 0x0000
    short morphColor15;
} MorphColor;

// size: 0x8
typedef struct _Sphere_noSq
{
    // offset: 0x0000 (6 bytes)
    Position position;
    // offset: 0x0006
    unsigned short radius;
} Sphere_noSq;

// size: 0x2C
typedef struct _BSPNode
{
    // offset: 0x0000 (8 bytes)
    Sphere_noSq sphere;
    // offset: 0x0008
    short a;
    // offset: 0x000A
    short b;
    // offset: 0x000C
    short c;
    // offset: 0x000E
    short flags;
    // offset: 0x0010
    long d;
    // offset: 0x0014
    void *front;
    // offset: 0x0018
    void *back;
    // offset: 0x001C (8 bytes)
    Sphere_noSq spectralSphere;
    // offset: 0x0024
    short front_spectral_error;
    // offset: 0x0026
    short back_spectral_error;
    // offset: 0x0028
    short front_material_error;
    // offset: 0x002A
    short back_material_error;
} BSPNode, SBSPNode;

// size: 0xC
typedef struct _BoundingBox
{
    // offset: 0x0000
    short minX;
    // offset: 0x0002
    short minY;
    // offset: 0x0004
    short minZ;
    // offset: 0x0006
    short maxX;
    // offset: 0x0008
    short maxY;
    // offset: 0x000A
    short maxZ;
} BoundingBox;

// size: 0x30
typedef struct _BSPLeaf
{
    // offset: 0x0000 (8 bytes)
    Sphere_noSq sphere;
    // offset: 0x0008 (4 bytes)
    TFace *faceList;
    // offset: 0x000C
    short numFaces;
    // offset: 0x000E
    short flags;
    // offset: 0x0010 (12 bytes)
    BoundingBox box;
    // offset: 0x001C (12 bytes)
    BoundingBox spectralBox;
    // offset: 0x0028 (8 bytes)
    Sphere_noSq spectralSphere;
} BSPLeaf;

// size: 0x24
typedef struct BSPTree
{
    // offset: 0x0000 (4 bytes)
    BSPNode *bspRoot;
    // offset: 0x0004 (4 bytes)
    BSPLeaf *startLeaves;
    // offset: 0x0008 (4 bytes)
    BSPLeaf *endLeaves;
    // offset: 0x000C (6 bytes)
    Position globalOffset;
    // offset: 0x0012
    short flags;
    // offset: 0x0014 (6 bytes)
    Position localOffset;
    // offset: 0x001A
    short ID;
    // offset: 0x001C
    long splineID;
    // offset: 0x0020 (4 bytes)
    struct _Instance *instanceSpline;
} BSPTree;

// size: 0x18
typedef struct _CameraKey
{
    // offset: 0x0000
    short x;
    // offset: 0x0002
    short y;
    // offset: 0x0004
    short z;
    // offset: 0x0006
    short id;
    // offset: 0x0008
    short rx;
    // offset: 0x000A
    short ry;
    // offset: 0x000C
    short rz;
    // offset: 0x000E
    short flags;
    // offset: 0x0010
    short tx;
    // offset: 0x0012
    short ty;
    // offset: 0x0014
    short tz;
    // offset: 0x0016
    short pad;
} CameraKey;

// size: 0x8
typedef struct _BVertex
{
    // offset: 0x0000
    short x;
    // offset: 0x0002
    short y;
    // offset: 0x0004
    short z;
    // offset: 0x0006
    short pad;
} BVertex;

// size: 0xC
typedef struct _BFace
{
    // offset: 0x0000
    short v0;
    // offset: 0x0002
    short v1;
    // offset: 0x0004
    short v2;
    // offset: 0x0006
    short pad;
    // offset: 0x0008 (4 bytes)
    TextureFT3 *texture;
} BFace;

// size: 0x18
typedef struct _BGObject
{
    // offset: 0x0000
    short flags;
    // offset: 0x0002
    short numFaces;
    // offset: 0x0004 (4 bytes)
    BVertex *vertexList;
    // offset: 0x0008 (4 bytes)
    BFace *faceList;
    // offset: 0x000C
    short x;
    // offset: 0x000E
    short y;
    // offset: 0x0010
    short z;
    // offset: 0x0012
    short radius;
    // offset: 0x0014
    long numVertices;
} BGObject;

/*struct MultiSignal {
};*/

// size: 0x1C
typedef struct Signal
{
    // offset: 0x0000
    long id;
    // offset: 0x0004 (24 bytes)
    union
    {
        // offset: 0x0000 (12 bytes)
        struct
        {
            // offset: 0x0000 (4 bytes)
            union
            {
                // offset: 0x0000
                long l;
                // offset: 0x0000 (4 bytes)
                short s[2];
                // offset: 0x0000 (4 bytes)
                char c[4];
            } size;
        } misc[3];
        // offset: 0x0000 (4 bytes)
        CameraKey *cameraKey;
        // offset: 0x0000 (16 bytes)
        struct
        {
            // offset: 0x0000
            short x;
            // offset: 0x0002
            short y;
            // offset: 0x0004
            short z;
            // offset: 0x0006
            short zrot;
            // offset: 0x0008 (8 bytes)
            SVector offset;
        } teleport;
        // offset: 0x0000 (8 bytes)
        struct
        {
            // offset: 0x0000
            long index;
            // offset: 0x0004
            long value;
        } cameraValue;
        // offset: 0x0000 (8 bytes)
        struct
        {
            // offset: 0x0000
            long index;
            // offset: 0x0004
            long value;
        } miscValue;
        // offset: 0x0000 (8 bytes)
        struct
        {
            // offset: 0x0000
            long index;
            // offset: 0x0004
            long value;
        } logicValue;
        // offset: 0x0000
        long timer;
        // offset: 0x0000
        long times;
        // offset: 0x0000
        long lightGroup;
        // offset: 0x0000
        long farPlane;
        // offset: 0x0000
        long cameraAdjust;
        // offset: 0x0000
        long cameraMode;
        // offset: 0x0000
        long cameraLock;
        // offset: 0x0000
        long cameraUnlock;
        // offset: 0x0000
        long cameraSmooth;
        // offset: 0x0000
        long cameraTimer;
        // offset: 0x0000 (8 bytes)
        struct
        {
            // offset: 0x0000
            long time;
            // offset: 0x0004
            long scale;
        } cameraShake;
        // offset: 0x0000
        long cameraSave;
        // offset: 0x0000
        long cameraRestore;
        // offset: 0x0000 (4 bytes)
        BGObject *bgObject;
        // offset: 0x0000 (8 bytes)
        long value[2];
        // offset: 0x0000 (12 bytes)
        struct
        {
            // offset: 0x0000
            long action;
            // offset: 0x0004 (8 bytes)
            long value[2];
        } sound;
        // offset: 0x0000
        void *intro;
        // offset: 0x0000
        long deathZ;
        // offset: 0x0000 (8 bytes)
        struct
        {
            // offset: 0x0000
            void *intro;
            // offset: 0x0004
            void *data;
        } dsignal;
        // offset: 0x0000
        void *mirror;
        // offset: 0x0000
        long fogNear;
        // offset: 0x0000
        long fogFar;
        // offset: 0x0000 (8 bytes)
        struct
        {
            // offset: 0x0000
            void *intro;
            // offset: 0x0004
            long frame;
        } gotoFrame;
        // offset: 0x0000 (8 bytes)
        struct
        {
            // offset: 0x0000
            void *intro;
            // offset: 0x0004
            long model;
        } changeModel;
        // offset: 0x0000
        void *vmObject;
        // offset: 0x0000 (8 bytes)
        struct
        {
            // offset: 0x0000
            unsigned char target;
            // offset: 0x0001
            unsigned char source1;
            // offset: 0x0002
            unsigned char source2;
            // offset: 0x0003
            unsigned char pad;
            // offset: 0x0004 (4 bytes)
            struct Signal *gotoLabel;
        } logic;
        // offset: 0x0000
        void *callSignal;
        // offset: 0x0000 (8 bytes)
        SVector offset;
        // offset: 0x0000 (4 bytes)
        struct Signal *gotoLabel;
        // offset: 0x0000 (4 bytes)
        struct Signal *gosubLabel;
        // offset: 0x0000 (8 bytes)
        struct
        {
            // offset: 0x0000
            long timer;
            // offset: 0x0004
            long action;
        } control;
        // offset: 0x0000 (12 bytes)
        struct
        {
            // offset: 0x0000
            short index;
            // offset: 0x0002
            short z;
            // offset: 0x0004 (4 bytes)
            struct MultiSignal *upSignal;
            // offset: 0x0008 (4 bytes)
            struct MultiSignal *downSignal;
        } zSignal;
        // offset: 0x0000 (24 bytes)
        struct
        {
            // offset: 0x0000
            long currentnum;
            // offset: 0x0004
            long streamID;
            // offset: 0x0008 (16 bytes)
            char toname[16];
        } StreamLevel;
        // offset: 0x0000 (12 bytes)
        struct
        {
            // offset: 0x0000
            char num;
            // offset: 0x0001
            char vel;
            // offset: 0x0002
            short misc;
            // offset: 0x0004
            char flags;
            // offset: 0x0005
            char timeToLive;
            // offset: 0x0006
            short dummy2;
            // offset: 0x0008 (4 bytes)
            Object *object;
        } shards;
        // offset: 0x0000 (8 bytes)
        struct
        {
            // offset: 0x0000
            long index;
            // offset: 0x0004
            void *intro;
        } cameraSpline;
        // offset: 0x0000 (4 bytes)
        struct
        {
            // offset: 0x0000
            short type;
            // offset: 0x0002
            short time;
        } screenWipe;
        // offset: 0x0000
        long voiceSitId;
        // offset: 0x0000 (8 bytes)
        struct
        {
            // offset: 0x0000
            long time;
            // offset: 0x0004
            void *intro;
        } introFX;
        // offset: 0x0000 (8 bytes)
        struct
        {
            // offset: 0x0000
            void *intro;
            // offset: 0x0004 (4 bytes)
            struct Signal *gotoLabel;
        } introActive;
        // offset: 0x0000 (18 bytes)
        struct
        {
            // offset: 0x0000 (9 bytes)
            char world[9];
            // offset: 0x0009 (9 bytes)
            char level[9];
        } levelChange;
        // offset: 0x0000
        long blendStart;
        // offset: 0x0000 (8 bytes)
        struct
        {
            // offset: 0x0000 (4 bytes)
            struct Signal *signal;
            // offset: 0x0004
            long times;
        } setTimes;
        // offset: 0x0000 (4 bytes)
        struct
        {
            // offset: 0x0000
            unsigned char r;
            // offset: 0x0001
            unsigned char g;
            // offset: 0x0002
            unsigned char b;
            // offset: 0x0003
            unsigned char pad;
        } color;
        // offset: 0x0000 (12 bytes)
        struct
        {
            // offset: 0x0000
            short x;
            // offset: 0x0002
            short y;
            // offset: 0x0004
            short time;
            // offset: 0x0006
            short index;
            // offset: 0x0008
            char *text;
        } print;
        // offset: 0x0000
        long slideAngle;
    } data;
} Signal;

// size: 0x388
typedef struct _MultiSignal
{
    // offset: 0x0000
    long numSignals;
    // offset: 0x0004
    short signalNum;
    // offset: 0x0006
    short flags;
    // offset: 0x0008 (896 bytes)
    Signal signalList[32];
} MultiSignal;

// size: 0x54
typedef struct _Terrain
{
    // offset: 0x0000
    short UnitChangeFlags;
    // offset: 0x0002
    short spad;
    // offset: 0x0004
    long lpad2;
    // offset: 0x0008
    long numIntros;
    // offset: 0x000C (4 bytes)
    Intro *introList;
    // offset: 0x0010
    long numVertices;
    // offset: 0x0014
    long numFaces;
    // offset: 0x0018
    long numNormals;
    // offset: 0x001C (4 bytes)
    TVertex *vertexList;
    // offset: 0x0020 (4 bytes)
    TFace *faceList;
    // offset: 0x0024 (4 bytes)
    Normal *normalList;
    // offset: 0x0028 (4 bytes)
    DrMoveAniTex *aniList;
    // offset: 0x002C
    long pad;
    // offset: 0x0030
    void *StreamUnits;
    // offset: 0x0034 (4 bytes)
    TextureFT3 *StartTextureList;
    // offset: 0x0038 (4 bytes)
    TextureFT3 *EndTextureList;
    // offset: 0x003C (4 bytes)
    MorphVertex *MorphDiffList;
    // offset: 0x0040 (4 bytes)
    MorphColor *MorphColorList;
    // offset: 0x0044
    long numBSPTrees;
    // offset: 0x0048 (4 bytes)
    BSPTree *BSPTreeArray;
    // offset: 0x004C
    short *morphNormalIdx;
    // offset: 0x0050 (4 bytes)
    MultiSignal *signals;
} Terrain;

// size: 0xC
typedef struct _PlaneConstants
{
    // offset: 0x0000
    short a;
    // offset: 0x0002
    short b;
    // offset: 0x0004
    short c;
    // offset: 0x0006
    short flags;
    // offset: 0x0008
    long d;
} PlaneConstants;

// size: 0x6
typedef struct _G2SVector3_Type
{
    // offset: 0x0000
    short x;
    // offset: 0x0002
    short y;
    // offset: 0x0004
    short z;
} G2SVector3;

// size: 0xC
typedef struct _G2LVector3_Type
{
    // offset: 0x0000
    long x;
    // offset: 0x0004
    long y;
    // offset: 0x0008
    long z;
} G2LVector3;

// size: 0x20
typedef struct _G2Matrix_Type
{
    // offset: 0x0000 (18 bytes)
    short rotScale[3][3];
    // offset: 0x0012
    short scaleFlag;
    // offset: 0x0014 (12 bytes)
    G2LVector3 trans;
} G2Matrix;

// size: 0x4
typedef struct _G2AnimChanStatus_Type
{
    // offset: 0x0000
    short keyData;
    // offset: 0x0002
    short index;
} G2AnimChanStatus;

// size: 0x24
typedef struct _G2AnimChanStatusBlock_Type
{
    // offset: 0x0000 (4 bytes)
    struct _G2AnimChanStatusBlock_Type *next;
    // offset: 0x0004 (32 bytes)
    G2AnimChanStatus chunks[8];
} G2AnimChanStatusBlock;

// size: 0x4
typedef struct _G2AnimAlphaTable_Type
{
    // offset: 0x0000
    unsigned short size;
    // offset: 0x0004
    long data[0];
} G2AnimAlphaTable;

// size: 0x28
typedef struct _G2AnimQuatInfo_Type
{
    // offset: 0x0000 (8 bytes)
    G2Quat srcQuat;
    // offset: 0x0008 (8 bytes)
    G2Quat destQuat;
    // offset: 0x0010 (6 bytes)
    G2SVector3 srcScale;
    // offset: 0x0016 (6 bytes)
    G2SVector3 destScale;
    // offset: 0x001C (6 bytes)
    G2SVector3 srcTrans;
    // offset: 0x0022 (6 bytes)
    G2SVector3 destTrans;
} G2AnimQuatInfo;

// size: 0xA4
typedef struct _G2AnimInterpStateBlock_Type
{
    // offset: 0x0000 (4 bytes)
    struct _G2AnimInterpStateBlock_Type *next;
    // offset: 0x0004 (160 bytes)
    G2AnimQuatInfo quatInfo[4];
} G2AnimInterpStateBlock;

// size: 0xC
typedef struct _G2AnimInterpInfo_Type
{
    // offset: 0x0000
    short duration;
    // offset: 0x0002
    short targetTime;
    // offset: 0x0004 (4 bytes)
    G2AnimAlphaTable *alphaTable;
    // offset: 0x0008 (4 bytes)
    G2AnimInterpStateBlock *stateBlockList;
} G2AnimInterpInfo;

// size: 0x30
typedef struct _G2AnimSection_Type
{
    // offset: 0x0000
    unsigned char flags;
    // offset: 0x0001
    unsigned char sectionID;
    // offset: 0x0002
    unsigned char firstSeg;
    // offset: 0x0003
    unsigned char segCount;
    // offset: 0x0004
    short elapsedTime;
    // offset: 0x0006
    short storedTime;
    // offset: 0x0008
    short loopStartTime;
    // offset: 0x000A
    short loopEndTime;
    // offset: 0x000C
    short *swAlarmTable;
    // offset: 0x0010
    long speedAdjustment;
    // offset: 0x0014
    unsigned short keylistID;
    // offset: 0x0018
    unsigned long alarmFlags;
    // offset: 0x001C
    long (*callback)();
    // offset: 0x0020
    void *callbackData;
    // offset: 0x0024 (4 bytes)
    G2AnimKeylist *keylist;
    // offset: 0x0028 (4 bytes)
    G2AnimChanStatusBlock *chanStatusBlockList;
    // offset: 0x002C (4 bytes)
    G2AnimInterpInfo *interpInfo;
} G2AnimSection;

// size: 0xB4
typedef struct _G2Anim_Type
{
    // offset: 0x0000
    unsigned char sectionCount;
    // offset: 0x0001
    unsigned char masterSection;
    // offset: 0x0002
    unsigned short controllerList;
    // offset: 0x0004
    unsigned short disabledControllerList;
    // offset: 0x0006
    unsigned short pad;
    // offset: 0x0008 (6 bytes)
    G2SVector3 rootTrans;
    // offset: 0x000E
    unsigned short flags;
    // offset: 0x0010 (4 bytes)
    Model *modelData;
    // offset: 0x0014 (4 bytes)
    G2Matrix *segMatrices;
    // offset: 0x0018 (12 bytes)
    unsigned long disabledBits[3];
    // offset: 0x0024 (144 bytes)
    G2AnimSection section[3];
} G2Anim;

// size: 0xC
typedef struct EventAliasCommandStruct
{
    // offset: 0x0000 (4 bytes)
    struct _Instance *hostInstance;
    // offset: 0x0004
    short newanim;
    // offset: 0x0006
    short newframe;
    // offset: 0x0008
    short interpframes;
    // offset: 0x000A
    short speed;
} EventAliasCommandStruct;

// size: 0x29C
typedef struct _Instance
{
    // offset: 0x0000 (8 bytes)
    NodeType node;
    // offset: 0x0008 (4 bytes)
    struct _Instance *next;
    // offset: 0x000C (4 bytes)
    struct _Instance *prev;
    // offset: 0x0010
    long instanceID;
    // offset: 0x0014
    long flags;
    // offset: 0x0018
    long flags2;
    // offset: 0x001C (4 bytes)
    Object *object;
    // offset: 0x0020 (4 bytes)
    Intro *intro;
    // offset: 0x0024
    void *data;
    // offset: 0x0028
    void *introData;
    // offset: 0x002C (4 bytes)
    struct _Instance *parent;
    // offset: 0x0030
    long introNum;
    // offset: 0x0034
    long birthStreamUnitID;
    // offset: 0x0038
    long currentStreamUnitID;
    // offset: 0x003C
    long introUniqueID;
    // offset: 0x0040 (4 bytes)
    MATRIX *matrix;
    // offset: 0x0044 (4 bytes)
    MATRIX *oldMatrix;
    // offset: 0x0048 (12 bytes)
    SoundInstance soundInstanceTbl[4];
    // offset: 0x0054
    unsigned long maxCheckDistance;
    // offset: 0x0058
    unsigned long checkMask;
    // offset: 0x005C (6 bytes)
    Position position;
    // offset: 0x0062
    unsigned char lightGroup;
    // offset: 0x0064 (6 bytes)
    Position oldPos;
    // offset: 0x006A
    unsigned char spectralLightGroup;
    // offset: 0x006C
    short currentTextureAnimFrame;
    // offset: 0x006E (6 bytes)
    Position initialPos;
    // offset: 0x0074 (8 bytes)
    Rotation rotation;
    // offset: 0x007C (8 bytes)
    Rotation oldRotation;
    // offset: 0x0084 (8 bytes)
    SVector scale;
    // offset: 0x008C
    long attachedID;
    // offset: 0x0090
    long attachedSegment;
    // offset: 0x0094 (32 bytes)
    MATRIX relativeMatrix;
    // offset: 0x00B4 (4 bytes)
    TFace *tface;
    // offset: 0x00B8 (4 bytes)
    TFace *oldTFace;
    // offset: 0x00BC
    void *tfaceLevel;
    // offset: 0x00C0
    void *collideInfo;
    // offset: 0x00C4
    long bspTree;
    // offset: 0x00C8
    short cachedTFace;
    // offset: 0x00CA
    short cachedBSPTree;
    // offset: 0x00CC
    void *cachedTFaceLevel;
    // offset: 0x00D0 (8 bytes)
    Normal oldWNormal;
    // offset: 0x00D8 (8 bytes)
    Normal wNormal;
    // offset: 0x00E0 (8 bytes)
    SVector offset;
    // offset: 0x00E8
    void *dsignal;
    // offset: 0x00EC
    void *shadow0;
    // offset: 0x00F0
    void *shadow1;
    // offset: 0x00F4
    void *extraLight;
    // offset: 0x00F8 (6 bytes)
    Position extraLightDir;
    // offset: 0x00FE
    short extraLightScale;
    // offset: 0x0100 (4 bytes)
    HModel *hModelList;
    // offset: 0x0104
    short fadeValue;
    // offset: 0x0106
    short lightMatrix;
    // offset: 0x0108
    void (*processFunc)();
    // offset: 0x010C
    void (*collideFunc)();
    // offset: 0x0110
    void (*additionalCollideFunc)();
    // offset: 0x0114
    void *additionalDrawFunc;
    // offset: 0x0118
    unsigned long (*queryFunc)();
    // offset: 0x011C
    void (*messageFunc)();
    // offset: 0x0120 (6 bytes)
    Position shadowPosition;
    // offset: 0x0126
    short currentModel;
    // offset: 0x0128 (4 bytes)
    TFace *waterFace;
    // offset: 0x012C (4 bytes)
    TFace *oldWaterFace;
    // offset: 0x0130 (4 bytes)
    Terrain *waterFaceTerrain;
    // offset: 0x0134 (12 bytes)
    PlaneConstants halvePlane;
    // offset: 0x0140 (8 bytes)
    SVector splitPoint;
    // offset: 0x0148 (4 bytes)
    struct _Instance *LinkParent;
    // offset: 0x014C (4 bytes)
    struct _Instance *LinkChild;
    // offset: 0x0150 (4 bytes)
    struct _Instance *LinkSibling;
    // offset: 0x0154
    long ParentLinkNode;
    // offset: 0x0158
    void *InstanceState;
    // offset: 0x015C (16 bytes)
    char introName[16];
    // offset: 0x016C
    void *extraData;
    // offset: 0x0170
    short constrictAngle;
    // offset: 0x0172
    short petrifyValue;
    // offset: 0x0174
    long xVel;
    // offset: 0x0178
    long yVel;
    // offset: 0x017C
    long zVel;
    // offset: 0x0180
    long xAccl;
    // offset: 0x0184
    long yAccl;
    // offset: 0x0188
    long zAccl;
    // offset: 0x018C
    long maxXVel;
    // offset: 0x0190
    long maxYVel;
    // offset: 0x0194
    long maxZVel;
    // offset: 0x0198
    long currentMainState;
    // offset: 0x019C
    long currentSubState;
    // offset: 0x01A0
    long work0;
    // offset: 0x01A4
    long work1;
    // offset: 0x01A8
    long work2;
    // offset: 0x01AC
    long work3;
    // offset: 0x01B0
    long work4;
    // offset: 0x01B4
    long work5;
    // offset: 0x01B8
    long work6;
    // offset: 0x01BC
    long work7;
    // offset: 0x01C0
    long work8;
    // offset: 0x01C4
    long work9;
    // offset: 0x01C8 (180 bytes)
    G2Anim anim;
    // offset: 0x027C (4 bytes)
    G2Bool(*rebuildCallback)();
    // offset: 0x0280
    short splineFlags;
    // offset: 0x0282
    short targetFrame;
    // offset: 0x0284
    short clipBeg;
    // offset: 0x0286
    short clipEnd;
    // offset: 0x0288 (12 bytes)
    EventAliasCommandStruct aliasCommand;
    // offset: 0x0294
    long light_color;
    // offset: 0x0298 (4 bytes)
    CVECTOR *perVertexColor;
} Instance;

// size: 0x14
typedef struct LightInstance
{
    // offset: 0x0000 (4 bytes)
    Instance *lightInstance;
    // offset: 0x0004
    long r;
    // offset: 0x0008
    long g;
    // offset: 0x000C
    long b;
    // offset: 0x0010
    short radius;
    // offset: 0x0012
    unsigned char segment;
    // offset: 0x0013
    unsigned char flags;
} LightInstance;

// size: 0x20
typedef struct _GameTrackerASMData_Type
{
    // offset: 0x0000
    long drawBackFaces;
    // offset: 0x0004
    long dispPage;
    // offset: 0x0008
    short MorphTime;
    // offset: 0x000A
    short MorphType;
    // offset: 0x000C (20 bytes)
    LightInstance lightInstances[1];
} GameTrackerASMData;

// size: 0x20
typedef struct _GameData_Type
{
    // offset: 0x0000 (32 bytes)
    GameTrackerASMData asmData;
} GameData;

// size: 0x8
typedef struct MemHeader
{
    // offset: 0x0000
    unsigned short magicNumber;
    // offset: 0x0002
    unsigned char memStatus;
    // offset: 0x0003
    unsigned char memType;
    // offset: 0x0004
    unsigned long memSize;
} MemHeader;

// size: 0x4
typedef struct _RGBAColor
{
    // offset: 0x0000
    unsigned char r;
    // offset: 0x0001
    unsigned char g;
    // offset: 0x0002
    unsigned char b;
    // offset: 0x0003
    unsigned char a;
} RGBAColor;

// size: 0x8
typedef struct _PVertex
{
    // offset: 0x0000
    short x;
    // offset: 0x0002
    short y;
    // offset: 0x0004
    long otz;
} PVertex;

// size: 0x18C0
typedef struct _VertexPool
{
    // offset: 0x0000 (4224 bytes)
    PVertex vertex[528];
    // offset: 0x1080 (2112 bytes)
    CVECTOR color[528];
} VertexPool;

// size: 0x1770C
typedef struct _PrimPool
{
    // offset: 0x0000
    long numPrims;
    // offset: 0x0004
    unsigned long *nextPrim;
    // offset: 0x0008
    unsigned long *lastPrim;
    // offset: 0x000C (96000 bytes)
    unsigned long prim[24000];
} PrimPool;

// size: 0xC
typedef struct _Sphere
{
    // offset: 0x0000 (6 bytes)
    Position position;
    // offset: 0x0006
    unsigned short radius;
    // offset: 0x0008
    unsigned long radiusSquared;
} Sphere;

// size: 0x9C9C
typedef struct _InstancePool
{
    // offset: 0x0000
    long nextInstanceID;
    // offset: 0x0004
    long numFreeInstances;
    // offset: 0x0008 (40080 bytes)
    Instance instance[60];
    // offset: 0x9C98 (4 bytes)
    Instance *first_free;
} InstancePool;

// size: 0x10C
typedef struct _InstanceList
{
    // offset: 0x0000
    long numInstances;
    // offset: 0x0004 (4 bytes)
    Instance *first;
    // offset: 0x0008 (4 bytes)
    InstancePool *pool;
    // offset: 0x000C (256 bytes)
    NodeType group[32];
} InstanceList;

// size: 0x24
typedef struct _ObjectTracker
{
    // offset: 0x0000 (16 bytes)
    char name[16];
    // offset: 0x0010 (4 bytes)
    Object *object;
    // offset: 0x0014
    short objectStatus;
    // offset: 0x0016
    short numInUse;
    // offset: 0x0018
    void *vramBlock;
    // offset: 0x001C
    char numObjectsUsing;
    // offset: 0x001D (7 bytes)
    char objectsUsing[7];
} ObjectTracker;

// size: 0x18
typedef struct CDLight
{
    // offset: 0x0000 (8 bytes)
    NodeType node;
    // offset: 0x0008
    unsigned char r;
    // offset: 0x0009
    unsigned char g;
    // offset: 0x000A
    unsigned char b;
    // offset: 0x000B
    unsigned char flags;
    // offset: 0x000C (12 bytes)
    Sphere sphere;
} CDLight;

// size: 0x40
typedef struct LightGroup
{
    // offset: 0x0000 (32 bytes)
    MATRIX lightMatrix;
    // offset: 0x0020 (32 bytes)
    MATRIX colorMatrix;
} LightGroup;

// size: 0xC
typedef struct LightList
{
    // offset: 0x0000 (4 bytes)
    CVECTOR ambient;
    // offset: 0x0004
    long numLightGroups;
    // offset: 0x0008 (4 bytes)
    LightGroup *lightGroupList;
} LightList;

// size: 0x6
typedef struct _VMOffset
{
    // offset: 0x0000
    short dx;
    // offset: 0x0002
    short dy;
    // offset: 0x0004
    short dz;
} VMMoveOffset;

// size: 0x3
typedef struct _VMColorOffset
{
    // offset: 0x0000
    char dr;
    // offset: 0x0001
    char dg;
    // offset: 0x0002
    char db;
} VMColorOffset;

// size: 0xC
typedef struct _VMOffsetTable
{
    // offset: 0x0000
    long numVMOffsets;
    // offset: 0x0004 (6 bytes)
    union
    {
        // offset: 0x0000 (6 bytes)
        VMMoveOffset moveOffsets;
        // offset: 0x0000 (3 bytes)
        VMColorOffset colorOffsets;
    } offsets;
} VMOffsetTable;

// size: 0x8
typedef struct _VMInterpolated
{
    // offset: 0x0000
    short tvIdx;
    // offset: 0x0002
    short startIdx;
    // offset: 0x0004
    short endIdx;
    // offset: 0x0006
    short time;
} VMInterpolated;

// size: 0x3C
typedef struct _VMObject
{
    // offset: 0x0000
    unsigned short flags;
    // offset: 0x0002
    short bspIdx;
    // offset: 0x0004
    short materialIdx;
    // offset: 0x0006
    short spectralIdx;
    // offset: 0x0008
    short currentIdx;
    // offset: 0x000A
    short timeScale;
    // offset: 0x000C
    long timer;
    // offset: 0x0010 (6 bytes)
    Position position;
    // offset: 0x0016
    short radius;
    // offset: 0x0018
    long radiusSquared;
    // offset: 0x001C
    long numVMOffsetTables;
    // offset: 0x0020 (4 bytes)
    VMOffsetTable **vmoffsetTableList;
    // offset: 0x0024 (4 bytes)
    VMOffsetTable *curVMOffsetTable;
    // offset: 0x0028
    long numVMVertices;
    // offset: 0x002C
    void *vmvertexList;
    // offset: 0x0030
    long numVMInterpolated;
    // offset: 0x0034 (4 bytes)
    VMInterpolated *vminterpolatedList;
    // offset: 0x0038
    char *name;
} VMObject;

// size: 0x28
typedef struct SpotLight
{
    // offset: 0x0000 (8 bytes)
    NodeType node;
    // offset: 0x0008
    unsigned char r;
    // offset: 0x0009
    unsigned char g;
    // offset: 0x000A
    unsigned char b;
    // offset: 0x000B
    unsigned char flags;
    // offset: 0x000C (6 bytes)
    Position centroid;
    // offset: 0x0012
    short radius;
    // offset: 0x0014
    long radiusSquared;
    // offset: 0x0018 (6 bytes)
    Position position;
    // offset: 0x001E (6 bytes)
    Position direction;
    // offset: 0x0024
    short cosFalloffAngle;
    // offset: 0x0026
    short attenuationScale;
} SpotLight;

// size: 0x18
typedef struct PointLight
{
    // offset: 0x0000 (8 bytes)
    NodeType node;
    // offset: 0x0008
    unsigned char r;
    // offset: 0x0009
    unsigned char g;
    // offset: 0x000A
    unsigned char b;
    // offset: 0x000B
    unsigned char flags;
    // offset: 0x000C (6 bytes)
    Position position;
    // offset: 0x0012
    short radius;
    // offset: 0x0014
    long radiusSquared;
} PointLight;

// size: 0xC
typedef struct _VGroup
{
    // offset: 0x0000
    long id;
    // offset: 0x0004
    long numVertices;
    // offset: 0x0008 (4 bytes)
    TVertex **vertexList;
} VGroup;

// size: 0x8
typedef struct _HotSpot
{
    // offset: 0x0000
    short x;
    // offset: 0x0002
    short y;
    // offset: 0x0004
    short z;
    // offset: 0x0006
    short id;
} HotSpot;

// size: 0x2
typedef struct EventBasicObject
{
    // offset: 0x0000
    short id;
} EventBasicObject;

// size: 0x8
typedef struct ScriptPCode
{
    // offset: 0x0000
    unsigned short sizeOfPcodeStream;
    // offset: 0x0002
    unsigned short conditionBits;
    // offset: 0x0004
    short *data;
} ScriptPCode;

// size: 0x1C
typedef struct Event
{
    // offset: 0x0000
    short eventNumber;
    // offset: 0x0002
    short numInstances;
    // offset: 0x0004
    unsigned char numActions;
    // offset: 0x0005
    unsigned char processingPuppetShow;
    // offset: 0x0006 (10 bytes)
    short eventVariables[5];
    // offset: 0x0010 (4 bytes)
    EventBasicObject **instanceList;
    // offset: 0x0014 (4 bytes)
    ScriptPCode **conditionalList;
    // offset: 0x0018 (4 bytes)
    ScriptPCode **actionList;
} Event;

// size: 0x8
typedef struct EventPointers
{
    // offset: 0x0000
    long numPuzzles;
    // offset: 0x0004 (4 bytes)
    Event *eventInstances[1];
} EventPointers;

// size: 0x8
typedef struct _PlanMkr
{
    // offset: 0x0000 (6 bytes)
    Position pos;
    // offset: 0x0006
    short id;
} PlanMkr;

// size: 0x24
typedef struct _SFXMkr
{
    // offset: 0x0000
    unsigned char *soundData;
    // offset: 0x0004
    long uniqueID;
    // offset: 0x0008 (12 bytes)
    SoundInstance sfxTbl[4];
    // offset: 0x0014 (6 bytes)
    Position pos;
    // offset: 0x001A
    short pad;
    // offset: 0x001C
    long livesInOnePlace;
    // offset: 0x0020
    long inSpectral;
} SFXMkr;

// size: 0x4
typedef struct TClassAttr
{
    // offset: 0x0000
    short flags;
    // offset: 0x0002
    unsigned short sound;
} TClassAttr;

// size: 0x188
typedef struct Level
{
    // offset: 0x0000 (4 bytes)
    Terrain *terrain;
    // offset: 0x0004 (4 bytes)
    LightList *lightList;
    // offset: 0x0008
    long numVMObjects;
    // offset: 0x000C (4 bytes)
    VMObject *vmobjectList;
    // offset: 0x0010
    long numSpotLights;
    // offset: 0x0014 (4 bytes)
    SpotLight *spotLightList;
    // offset: 0x0018
    long numPointLights;
    // offset: 0x001C (4 bytes)
    PointLight *pointLightList;
    // offset: 0x0020
    long numSpotSpecturalLights;
    // offset: 0x0024 (4 bytes)
    SpotLight *spotSpecturalLightList;
    // offset: 0x0028
    long numPointSpecturalLights;
    // offset: 0x002C (4 bytes)
    PointLight *pointSpecturalLightList;
    // offset: 0x0030
    long numBGObjects;
    // offset: 0x0034 (4 bytes)
    BGObject *bgObjectList;
    // offset: 0x0038
    long waterZLevel;
    // offset: 0x003C
    unsigned char backColorR;
    // offset: 0x003D
    unsigned char backColorG;
    // offset: 0x003E
    unsigned char backColorB;
    // offset: 0x003F
    unsigned char cpad1;
    // offset: 0x0040
    unsigned char specturalColorR;
    // offset: 0x0041
    unsigned char specturalColorG;
    // offset: 0x0042
    unsigned char specturalColorB;
    // offset: 0x0043
    unsigned char cpad2;
    // offset: 0x0044
    unsigned short fogFar;
    // offset: 0x0046
    unsigned short fogNear;
    // offset: 0x0048
    unsigned short spectralFogFar;
    // offset: 0x004A
    unsigned short spectralFogNear;
    // offset: 0x004C
    unsigned short waterFogFar;
    // offset: 0x004E
    unsigned short waterFogNear;
    // offset: 0x0050
    long depthQBlendStart;
    // offset: 0x0054 (4 bytes)
    LightList *spectrallightList;
    // offset: 0x0058
    long numCameras;
    // offset: 0x005C
    void *cameraList;
    // offset: 0x0060
    long bspPlaneError;
    // offset: 0x0064
    long numVGroups;
    // offset: 0x0068 (4 bytes)
    VGroup *vGroupList;
    // offset: 0x006C
    long deathZ;
    // offset: 0x0070
    long flags;
    // offset: 0x0074 (4 bytes)
    MultiSignal *startSignal;
    // offset: 0x0078
    long numIntros;
    // offset: 0x007C (4 bytes)
    Intro *introList;
    // offset: 0x0080 (4 bytes)
    DrMoveAniTex *bgAniList;
    // offset: 0x0084
    long numHotSpots;
    // offset: 0x0088 (4 bytes)
    HotSpot *hotSpotList;
    // offset: 0x008C
    void *objectNameList;
    // offset: 0x0090
    long depthQFogStart;
    // offset: 0x0094
    long morphLastStep;
    // offset: 0x0098
    char *worldName;
    // offset: 0x009C (8 bytes)
    VramSize vramSize;
    // offset: 0x00A4
    unsigned short holdFogFar;
    // offset: 0x00A6
    unsigned short holdFogNear;
    // offset: 0x00A8
    long numberEnemyUnits;
    // offset: 0x00AC
    char **enemyUnitsNames;
    // offset: 0x00B0
    long **timesSignalList;
    // offset: 0x00B4 (4 bytes)
    MultiSignal *spectralSignal;
    // offset: 0x00B8 (4 bytes)
    MultiSignal *materialSignal;
    // offset: 0x00BC (4 bytes)
    MultiSignal *startUnitLoadedSignal;
    // offset: 0x00C0 (4 bytes)
    MultiSignal *startUnitMainSignal;
    // offset: 0x00C4 (4 bytes)
    MultiSignal *startGoingIntoWaterSignal;
    // offset: 0x00C8 (4 bytes)
    MultiSignal *startGoingOutOfWaterSignal;
    // offset: 0x00CC
    long unitFlags;
    // offset: 0x00D0 (4 bytes)
    MultiSignal *SignalListStart;
    // offset: 0x00D4 (4 bytes)
    MultiSignal *SignalListEnd;
    // offset: 0x00D8
    long numBSPLeaves;
    // offset: 0x00DC (4 bytes)
    EventPointers *PuzzleInstances;
    // offset: 0x00E0
    long NumberOfPlanMarkers;
    // offset: 0x00E4 (4 bytes)
    PlanMkr *PlanMarkerList;
    // offset: 0x00E8
    long NumberOfSFXMarkers;
    // offset: 0x00EC (4 bytes)
    SFXMkr *SFXMarkerList;
    // offset: 0x00F0
    unsigned long versionNumber;
    // offset: 0x00F4
    char *dynamicMusicName;
    // offset: 0x00F8
    long streamUnitID;
    // offset: 0x00FC (64 bytes)
    TClassAttr tClassAttr[16];
    // offset: 0x013C (24 bytes)
    short TODLighting[4][3];
    // offset: 0x0154 (12 bytes)
    unsigned char TODbackColor[4][3];
    // offset: 0x0160 (16 bytes)
    unsigned short TODfogNearFar[4][2];
    // offset: 0x0170
    short TODRedScale;
    // offset: 0x0172
    short TODGrnScale;
    // offset: 0x0174
    short TODBluScale;
    // offset: 0x0176
    short TODfogNear;
    // offset: 0x0178
    short TODfogFar;
    // offset: 0x017A
    short short_pad;
    // offset: 0x017C
    unsigned char TODbackColorR;
    // offset: 0x017D
    unsigned char TODbackColorG;
    // offset: 0x017E
    unsigned char TODbackColorB;
    // offset: 0x017F
    unsigned char cpad3;
    // offset: 0x0180 (4 bytes)
    LightGroup *razielLightGroup;
    // offset: 0x0184 (4 bytes)
    LightGroup *razielSpectralLightGroup;
} Level;

// size: 0x70
typedef struct _SavedCinematic
{
    // offset: 0x0000 (6 bytes)
    Position position;
    // offset: 0x0006 (6 bytes)
    Position focusPoint;
    // offset: 0x000C (6 bytes)
    Position base;
    // offset: 0x0012 (6 bytes)
    Position targetPos;
    // offset: 0x0018 (6 bytes)
    Position targetFocusPoint;
    // offset: 0x001E (6 bytes)
    Position targetBase;
    // offset: 0x0024 (8 bytes)
    Rotation baseRot;
    // offset: 0x002C (8 bytes)
    Rotation focusRotation;
    // offset: 0x0034 (8 bytes)
    Rotation targetFocusRotation;
    // offset: 0x003C
    short focusDistance;
    // offset: 0x003E
    short targetFocusDistance;
    // offset: 0x0040 (8 bytes)
    SVector focusPointVel;
    // offset: 0x0048 (8 bytes)
    SVector focusPointAccl;
    // offset: 0x0050 (8 bytes)
    SVector baseVel;
    // offset: 0x0058 (8 bytes)
    SVector baseAccl;
    // offset: 0x0060
    long maxVel;
    // offset: 0x0064 (4 bytes)
    MultiSpline *posSpline;
    // offset: 0x0068 (4 bytes)
    MultiSpline *targetSpline;
    // offset: 0x006C (4 bytes)
    Level *level;
} SavedCinematic;

// size: 0x47C
typedef struct LightInfo
{
    // offset: 0x0000
    long numPointLights;
    // offset: 0x0004 (768 bytes)
    PointLight pointLightPool[32];
    // offset: 0x0304 (8 bytes)
    NodeType usedPointLightList;
    // offset: 0x030C (8 bytes)
    NodeType freePointLightList;
    // offset: 0x0314 (8 bytes)
    NodeType activePointLightList;
    // offset: 0x031C
    long numSpotLights;
    // offset: 0x0320 (320 bytes)
    SpotLight spotLightPool[8];
    // offset: 0x0460 (8 bytes)
    NodeType usedSpotLightList;
    // offset: 0x0468 (8 bytes)
    NodeType freeSpotLightList;
    // offset: 0x0470 (8 bytes)
    NodeType activeSpotLightList;
    // offset: 0x0478
    long numSavedColors;
} LightInfo;

// size: 0x4
typedef struct _ColorType
{
    // offset: 0x0000
    unsigned char r;
    // offset: 0x0001
    unsigned char g;
    // offset: 0x0002
    unsigned char b;
    // offset: 0x0003
    unsigned char code;
} ColorType;

// size: 0xE0
typedef struct _CameraCore_Type
{
    // offset: 0x0000 (6 bytes)
    Position position;
    // offset: 0x0006
    short pad1;
    // offset: 0x0008 (64 bytes)
    MATRIX vvNormalWorVecMat[2];
    // offset: 0x0048 (24 bytes)
    long vvPlaneConsts[6];
    // offset: 0x0060 (4 bytes)
    Transform *wcTransform;
    // offset: 0x0064
    long farPlane;
    // offset: 0x0068
    long projDistance;
    // offset: 0x006C
    long nearPlane;
    // offset: 0x0070 (4 bytes)
    Transform *wcTransform2;
    // offset: 0x0074 (4 bytes)
    Transform *cwTransform2;
    // offset: 0x0078 (40 bytes)
    Normal viewVolumeNormal[5];
    // offset: 0x00A0
    int leftX;
    // offset: 0x00A4
    int rightX;
    // offset: 0x00A8
    int topY;
    // offset: 0x00AC
    int bottomY;
    // offset: 0x00B0 (8 bytes)
    Rotation rotation;
    // offset: 0x00B8
    short pad2;
    // offset: 0x00BA (8 bytes)
    SVector screenScale;
    // offset: 0x00C2
    short pad3;
    // offset: 0x00C4 (6 bytes)
    Position debugPos;
    // offset: 0x00CA
    short pad4;
    // offset: 0x00CC (8 bytes)
    Rotation debugRot;
    // offset: 0x00D4
    short pad5;
    // offset: 0x00D6 (6 bytes)
    Position nondebugPos;
    // offset: 0x00DC
    short pad6;
} CameraCore;

// size: 0x4
typedef struct SpuVolume
{
    // offset: 0x0000
    short left;
    // offset: 0x0002
    short right;
} SpuVolume;

// size: 0x40
typedef struct SpuVoiceAttr
{
    // offset: 0x0000
    unsigned long voice;
    // offset: 0x0004
    unsigned long mask;
    // offset: 0x0008 (4 bytes)
    SpuVolume volume;
    // offset: 0x000C (4 bytes)
    SpuVolume volmode;
    // offset: 0x0010 (4 bytes)
    SpuVolume volumex;
    // offset: 0x0014
    unsigned short pitch;
    // offset: 0x0016
    unsigned short note;
    // offset: 0x0018
    unsigned short sample_note;
    // offset: 0x001A
    short envx;
    // offset: 0x001C
    unsigned long addr;
    // offset: 0x0020
    unsigned long loop_addr;
    // offset: 0x0024
    long a_mode;
    // offset: 0x0028
    long s_mode;
    // offset: 0x002C
    long r_mode;
    // offset: 0x0030
    unsigned short ar;
    // offset: 0x0032
    unsigned short dr;
    // offset: 0x0034
    unsigned short sr;
    // offset: 0x0036
    unsigned short rr;
    // offset: 0x0038
    unsigned short sl;
    // offset: 0x003A
    unsigned short adsr1;
    // offset: 0x003C
    unsigned short adsr2;
} SpuVoiceAttr;

// size: 0xC
typedef struct SpuExtAttr
{
    // offset: 0x0000 (4 bytes)
    SpuVolume volume;
    // offset: 0x0004
    long reverb;
    // offset: 0x0008
    long mix;
} SpuExtAttr;

// size: 0x10
typedef struct SpuStVoiceAttr
{
    // offset: 0x0000
    char status;
    // offset: 0x0001
    char pad1;
    // offset: 0x0002
    char pad2;
    // offset: 0x0003
    char pad3;
    // offset: 0x0004
    long last_size;
    // offset: 0x0008
    unsigned long buf_addr;
    // offset: 0x000C
    unsigned long data_addr;
} SpuStVoiceAttr;

// size: 0x28
typedef struct AadSoundBankHdr
{
    // offset: 0x0000
    unsigned long bankID;
    // offset: 0x0004
    unsigned long headerSize;
    // offset: 0x0008
    int bankVersion;
    // offset: 0x000C
    int numPrograms;
    // offset: 0x0010
    int numTones;
    // offset: 0x0014
    int numWaves;
    // offset: 0x0018
    int numSequences;
    // offset: 0x001C
    int numLabels;
    // offset: 0x0020
    int reverbMode;
    // offset: 0x0024
    int reverbDepth;
} AadSoundBankHdr;

// size: 0x8
typedef struct AadProgramAtr
{
    // offset: 0x0000
    unsigned short numTones;
    // offset: 0x0002
    unsigned short firstTone;
    // offset: 0x0004
    unsigned char volume;
    // offset: 0x0005
    unsigned char panPosition;
    // offset: 0x0006
    unsigned short unused;
} AadProgramAtr;

// size: 0x10
typedef struct AadToneAtr
{
    // offset: 0x0000
    unsigned char priority;
    // offset: 0x0001
    unsigned char parentProgram;
    // offset: 0x0002
    unsigned char volume;
    // offset: 0x0003
    unsigned char panPosition;
    // offset: 0x0004
    unsigned char centerNote;
    // offset: 0x0005
    char centerFine;
    // offset: 0x0006
    unsigned char minNote;
    // offset: 0x0007
    unsigned char maxNote;
    // offset: 0x0008
    unsigned char mode;
    // offset: 0x0009
    unsigned char pitchBendMax;
    // offset: 0x000A
    unsigned short adsr1;
    // offset: 0x000C
    unsigned short adsr2;
    // offset: 0x000E
    short waveIndex;
} AadToneAtr;

// size: 0x14
typedef struct _AadDynSfxFileHdr
{
    // offset: 0x0000
    unsigned long snfID;
    // offset: 0x0004
    unsigned short snfVersion;
    // offset: 0x0006
    unsigned short uniqueID;
    // offset: 0x0008
    unsigned short handle;
    // offset: 0x000A
    unsigned short numSfxInFile;
    // offset: 0x000C (4 bytes)
    struct _AadDynSfxFileHdr *prevDynSfxFile;
    // offset: 0x0010 (4 bytes)
    struct _AadDynSfxFileHdr *nextDynSfxFile;
} AadDynSfxSnfFileHdr;

// size: 0x18
typedef struct AadDynSfxAttr
{
    // offset: 0x0000
    unsigned short sfxID;
    // offset: 0x0002
    unsigned short waveID;
    // offset: 0x0004 (16 bytes)
    AadToneAtr toneAttr;
    // offset: 0x0014
    unsigned long waveSize;
} AadDynSfxAttr;

// size: 0x18
typedef struct AadTempoCounter
{
    // offset: 0x0000
    unsigned long currentTick;
    // offset: 0x0004
    unsigned long currentError;
    // offset: 0x0008
    unsigned long tickTimeFixed;
    // offset: 0x000C
    unsigned long errorPerUpdate;
    // offset: 0x0010
    unsigned short ticksPerUpdate;
    // offset: 0x0012
    unsigned short ppqn;
    // offset: 0x0014
    unsigned long quarterNoteTime;
} AadTempoCounter;

// size: 0xC
typedef struct AadSeqEvent
{
    // offset: 0x0000
    unsigned long deltaTime;
    // offset: 0x0004
    unsigned char track;
    // offset: 0x0005
    unsigned char statusByte;
    // offset: 0x0006 (6 bytes)
    char dataByte[6];
} AadSeqEvent;

// size: 0x5D0
typedef struct _AadSequenceSlot
{
    // offset: 0x0000 (24 bytes)
    AadTempoCounter tempo;
    // offset: 0x0018 (768 bytes)
    AadSeqEvent eventQueue[4][16];
    // offset: 0x0318 (16 bytes)
    unsigned char eventIn[16];
    // offset: 0x0328 (16 bytes)
    unsigned char eventOut[16];
    // offset: 0x0338 (16 bytes)
    unsigned char eventsInQueue[16];
    // offset: 0x0348 (64 bytes)
    unsigned char *sequencePosition[16];
    // offset: 0x0388 (64 bytes)
    unsigned long lastEventExecutedTime[16];
    // offset: 0x03C8 (16 bytes)
    unsigned char runningStatus[16];
    // offset: 0x03D8 (16 bytes)
    unsigned char trackFlags[16];
    // offset: 0x03E8 (256 bytes)
    unsigned char *loopSequencePosition[4][16];
    // offset: 0x04E8 (16 bytes)
    unsigned char loopCurrentNestLevel[16];
    // offset: 0x04F8 (64 bytes)
    unsigned char loopCounter[4][16];
    // offset: 0x0538 (4 bytes)
    struct _AadSequenceSlot *selectedSlotPtr;
    // offset: 0x053C
    unsigned char selectedSlotNum;
    // offset: 0x053D
    unsigned char selectedChannel;
    // offset: 0x053E
    unsigned char sequenceNumberAssigned;
    // offset: 0x053F
    unsigned char thisSlotNumber;
    // offset: 0x0540
    unsigned short status;
    // offset: 0x0542
    unsigned short channelMute;
    // offset: 0x0544
    unsigned short delayedMuteMode;
    // offset: 0x0546
    unsigned short delayedMuteCmds;
    // offset: 0x0548
    unsigned short delayedUnMuteCmds;
    // offset: 0x054A
    unsigned short unused;
    // offset: 0x054C
    unsigned short enableSustainUpdate;
    // offset: 0x054E
    unsigned short ignoreTranspose;
    // offset: 0x0550
    unsigned char slotFlags;
    // offset: 0x0551
    unsigned char slotID;
    // offset: 0x0552
    unsigned char slotVolume;
    // offset: 0x0553
    unsigned char slotPan;
    // offset: 0x0554
    int *masterVolPtr;
    // offset: 0x0558
    int selectedDynamicBank;
    // offset: 0x055C
    int sequenceAssignedDynamicBank;
    // offset: 0x0560 (16 bytes)
    unsigned char currentDynamicBank[16];
    // offset: 0x0570 (32 bytes)
    unsigned short pitchWheel[16];
    // offset: 0x0590 (16 bytes)
    unsigned char currentProgram[16];
    // offset: 0x05A0 (16 bytes)
    unsigned char volume[16];
    // offset: 0x05B0 (16 bytes)
    unsigned char panPosition[16];
    // offset: 0x05C0 (16 bytes)
    char transpose[16];
} AadSequenceSlot;

// size: 0xC
typedef struct AadSfxCommand
{
    // offset: 0x0000
    unsigned char statusByte;
    // offset: 0x0001 (3 bytes)
    unsigned char dataByte[3];
    // offset: 0x0004
    unsigned long ulongParam;
    // offset: 0x0008
    short shortParam;
    // offset: 0x000A
    unsigned short unused;
} AadSfxCommand;

// size: 0x188
typedef struct AadSfxSlot
{
    // offset: 0x0000 (384 bytes)
    AadSfxCommand commandQueue[32];
    // offset: 0x0180
    unsigned char commandIn;
    // offset: 0x0181
    unsigned char commandOut;
    // offset: 0x0182
    unsigned short commandsInQueue;
    // offset: 0x0184
    unsigned short handleCounter;
    // offset: 0x0186
    unsigned char sfxVolume;
    // offset: 0x0187
    unsigned char sfxPan;
} AadSfxSlot;

// size: 0x1C
typedef struct AadSynthVoice
{
    // offset: 0x0000
    unsigned long voiceMask;
    // offset: 0x0004
    unsigned long handle;
    // offset: 0x0008
    unsigned char voiceID;
    // offset: 0x0009
    unsigned char priority;
    // offset: 0x000A
    unsigned char note;
    // offset: 0x000B
    unsigned char unused;
    // offset: 0x000C
    unsigned char program;
    // offset: 0x000D
    unsigned char baseProg;
    // offset: 0x000E
    unsigned char volume;
    // offset: 0x000F
    unsigned char updateVol;
    // offset: 0x0010
    unsigned char pan;
    // offset: 0x0011
    unsigned char voiceNum;
    // offset: 0x0012
    unsigned short flags;
    // offset: 0x0014 (4 bytes)
    AadToneAtr *toneAtr;
    // offset: 0x0018 (4 bytes)
    AadProgramAtr *progAtr;
} AadSynthVoice;

// size: 0x1C
typedef struct AadDynamicLoadRequest
{
    // offset: 0x0000
    int type;
    // offset: 0x0004
    int handle;
    // offset: 0x0008
    long directoryID;
    // offset: 0x000C
    long flags;
    // offset: 0x0010 (12 bytes)
    char fileName[12];
} AadDynamicLoadRequest;

// size: 0x58
typedef struct AadDynamicBankLoadInfo
{
    // offset: 0x0000 (32 bytes)
    char sndFileName[32];
    // offset: 0x0020 (32 bytes)
    char smpFileName[32];
    // offset: 0x0040
    int dynamicBankIndex;
    // offset: 0x0044
    int loadOption;
    // offset: 0x0048
    void (*userCallbackProc)();
    // offset: 0x004C
    unsigned long sramDataSize;
    // offset: 0x0050
    unsigned long nextSramAddr;
    // offset: 0x0054
    unsigned long flags;
} AadDynamicBankLoadInfo;

// size: 0x94
typedef struct AadDynamicSfxLoadInfo
{
    // offset: 0x0000 (40 bytes)
    char snfFileName[40];
    // offset: 0x0028 (40 bytes)
    char smfFileName[40];
    // offset: 0x0050
    int fileHandle;
    // offset: 0x0054
    long directoryID;
    // offset: 0x0058
    long loadFlags;
    // offset: 0x005C
    long flags;
    // offset: 0x0060
    int smfLoadingState;
    // offset: 0x0064
    unsigned long bytesToLoad;
    // offset: 0x0068
    int numSfxToLoad;
    // offset: 0x006C
    int error;
    // offset: 0x0070
    unsigned long waveTransferAddr;
    // offset: 0x0074
    unsigned long totalSramUsed;
    // offset: 0x0078 (4 bytes)
    AadDynSfxSnfFileHdr *snfFile;
    // offset: 0x007C (24 bytes)
    AadDynSfxAttr attr;
} AadDynamicSfxLoadInfo;

// size: 0x8
typedef struct AadNewSramBlockDesc
{
    // offset: 0x0000
    unsigned short waveID;
    // offset: 0x0002
    unsigned short address;
    // offset: 0x0004
    unsigned short size;
    // offset: 0x0006
    unsigned char prevIndex;
    // offset: 0x0007
    unsigned char nextIndex;
} AadNewSramBlockDesc;

// size: 0x14
typedef struct AadLoadedSfxToneAttr
{
    // offset: 0x0000
    unsigned short referenceCount;
    // offset: 0x0002
    unsigned short waveID;
    // offset: 0x0004 (16 bytes)
    AadToneAtr toneAttr;
} AadLoadedSfxToneAttr;

// size: 0x4
typedef struct AadLoadedSfxWaveAttr
{
    // offset: 0x0000
    unsigned short referenceCount;
    // offset: 0x0002
    unsigned char unused;
    // offset: 0x0003
    unsigned char sramHandle;
} AadLoadedSfxWaveAttr;

// size: 0x20
typedef struct AadSramDefragInfo
{
    // offset: 0x0000
    int status;
    // offset: 0x0004
    unsigned char *fragBuffer;
    // offset: 0x0008
    int masterListEntry;
    // offset: 0x000C
    int waveID;
    // offset: 0x0010
    unsigned long destSramAddr;
    // offset: 0x0014
    unsigned long srcSramAddr;
    // offset: 0x0018
    unsigned long moveSize;
    // offset: 0x001C
    unsigned long readSize;
} AadSramDefragInfo;

// size: 0xC
typedef struct AadVolFader
{
    // offset: 0x0000
    int volumeStep;
    // offset: 0x0004
    int targetVolume;
    // offset: 0x0008
    void (*fadeCompleteCallback)();
} AadVolFader;

// size: 0x8
typedef struct MusicLoadCmd
{
    // offset: 0x0000
    int type;
    // offset: 0x0004
    int data;
} MusicLoadCmd;

// size: 0x14
typedef struct gSoundData
{
    // offset: 0x0000
    unsigned long gMasterVol;
    // offset: 0x0004
    unsigned long gMusicVol;
    // offset: 0x0008
    unsigned long gSfxVol;
    // offset: 0x000C
    unsigned long gVoiceVol;
    // offset: 0x0010
    char gSfxOn;
    // offset: 0x0011
    char gMusicOn;
    // offset: 0x0012
    char gVoiceOn;
    // offset: 0x0013
    char soundsLoaded;
} gSoundData;

// size: 0x1C
typedef struct menu_format_t
{
    // offset: 0x0000
    int xpos;
    // offset: 0x0004
    int ypos;
    // offset: 0x0008
    int lineskip;
    // offset: 0x000C
    int itemskip;
    // offset: 0x0010
    int width;
    // offset: 0x0014
    int center;
    // offset: 0x0018
    int border;
} menu_format_t;

// size: 0x24
typedef struct menu_stack_t
{
    // offset: 0x0000
    int (*fn)();
    // offset: 0x0004
    int index;
    // offset: 0x0008 (28 bytes)
    menu_format_t format;
} menu_stack_t;

// size: 0x10
typedef struct menu_item_t
{
    // offset: 0x0000
    char *text;
    // offset: 0x0004
    int (*fn)();
    // offset: 0x0008
    long parameter;
    // offset: 0x000C
    long flags;
} menu_item_t;

// size: 0x388
typedef struct menu_t
{
    // offset: 0x0000
    int nmenus;
    // offset: 0x0004
    int nitems;
    // offset: 0x0008
    int nbytes;
    // offset: 0x000C (180 bytes)
    menu_stack_t stack[5];
    // offset: 0x00C0 (192 bytes)
    menu_item_t items[12];
    // offset: 0x0180 (512 bytes)
    char bytes[512];
    // offset: 0x0380
    void (*drawfn)();
    // offset: 0x0384
    void *opaque;
} menu_t;

// size: 0x24
typedef struct mcmenu_table_t
{
    // offset: 0x0000
    int (*data_size)();
    // offset: 0x0004
    void (*initialize)();
    // offset: 0x0008
    void (*terminate)();
    // offset: 0x000C
    void (*begin)();
    // offset: 0x0010
    void (*end)();
    // offset: 0x0014
    int (*set_buffer)();
    // offset: 0x0018
    int (*main)();
    // offset: 0x001C
    int (*pause)();
    // offset: 0x0020
    char *versionID;
} mcmenu_table_t;

// size: 0x18
typedef struct mcpsx_directory_t
{
    // offset: 0x0000 (20 bytes)
    char name[20];
    // offset: 0x0014
    int nbytes;
} mcpsx_directory_t;

// size: 0x38
typedef struct mcpsx_t
{
    // offset: 0x0000 (20 bytes)
    struct
    {
        // offset: 0x0000 (4 bytes)
        mcpsx_mode_t mode;
        // offset: 0x0004 (4 bytes)
        mcpsx_func_t func;
        // offset: 0x0008 (4 bytes)
        mcpsx_sync_t sync;
        // offset: 0x000C (4 bytes)
        mcpsx_err_t err;
        // offset: 0x0010
        int observed;
    } state;
    // offset: 0x0014 (32 bytes)
    struct
    {
        // offset: 0x0000
        long channel;
        // offset: 0x0004
        char *filename;
        // offset: 0x0008
        void *buffer;
        // offset: 0x000C
        long offset;
        // offset: 0x0010
        long nbytes;
        // offset: 0x0014
        long nblocks;
        // offset: 0x0018 (4 bytes)
        mcpsx_directory_t *directory;
        // offset: 0x001C
        long *nfilesptr;
    } params;
    // offset: 0x0034
    void *opaque;
} mcpsx_t;

// size: 0x34
typedef struct mcard_t
{
    // offset: 0x0000 (16 bytes)
    struct
    {
        // offset: 0x0000 (4 bytes)
        mcpsx_err_t err;
        // offset: 0x0004 (4 bytes)
        fsmcard_t fsm;
        // offset: 0x0008 (4 bytes)
        mcard_status_t status;
        // offset: 0x000C
        int not_exists;
    } state;
    // offset: 0x0010 (28 bytes)
    struct
    {
        // offset: 0x0000
        long parameter;
        // offset: 0x0004
        long *nfilesptr;
        // offset: 0x0008 (4 bytes)
        mcpsx_directory_t *directory;
        // offset: 0x000C
        char *filename;
        // offset: 0x0010
        void *buffer;
        // offset: 0x0014
        int nbytes;
        // offset: 0x0018
        int nblocks;
    } params;
    // offset: 0x002C (4 bytes)
    mcpsx_t *mcpsx;
    // offset: 0x0030
    void *opaque;
} mcard_t;

// size: 0x194
typedef struct mcmenu_t
{
    // offset: 0x0000 (20 bytes)
    struct
    {
        // offset: 0x0000 (4 bytes)
        fsm_t fsm;
        // offset: 0x0004 (4 bytes)
        fsm_t fsm_prev;
        // offset: 0x0008 (4 bytes)
        mcard_status_t status;
        // offset: 0x000C
        int running;
        // offset: 0x0010
        int slot;
    } state;
    // offset: 0x0014 (376 bytes)
    struct
    {
        // offset: 0x0000
        int nblocks;
        // offset: 0x0004
        int nbytes;
        // offset: 0x0008
        void *buffer;
        // offset: 0x000C (360 bytes)
        mcpsx_directory_t directory[15];
        // offset: 0x0174
        long nfiles;
    } params;
    // offset: 0x018C (4 bytes)
    mcard_t *mcard;
    // offset: 0x0190
    void *opaque;
} mcmenu_t;

// size: 0x10
typedef struct memcard_t
{
    // offset: 0x0000 (4 bytes)
    Object *object;
    // offset: 0x0004 (4 bytes)
    mcmenu_table_t *table;
    // offset: 0x0008 (4 bytes)
    mcmenu_t *mcmenu;
    // offset: 0x000C
    short running;
    // offset: 0x000E
    short wrongVerison;
} memcard_t;

// size: 0x270
typedef struct GameTracker
{
    // offset: 0x0000 (32 bytes)
    GameData gameData;
    // offset: 0x0020 (4 bytes)
    menu_t *menu;
    // offset: 0x0024 (4 bytes)
    memcard_t *memcard;
    // offset: 0x0028 (4 bytes)
    Level *level;
    // offset: 0x002C (4 bytes)
    Instance *playerInstance;
    // offset: 0x0030
    long drawPage;
    // offset: 0x0034 (4 bytes)
    InstanceList *instanceList;
    // offset: 0x0038 (4 bytes)
    InstancePool *instancePool;
    // offset: 0x003C (4 bytes)
    VertexPool *vertexPool;
    // offset: 0x0040 (4 bytes)
    PrimPool *primPool;
    // offset: 0x0044 (4 bytes)
    ObjectTracker *GlobalObjects;
    // offset: 0x0048 (40 bytes)
    long controlCommand[2][5];
    // offset: 0x0070 (40 bytes)
    long controlData[2][5];
    // offset: 0x0098 (40 bytes)
    long overrideData[2][5];
    // offset: 0x00C0
    long debugFlags;
    // offset: 0x00C4
    long debugFlags2;
    // offset: 0x00C8 (4 bytes)
    CVECTOR wipeColor;
    // offset: 0x00CC
    short wipeTime;
    // offset: 0x00CE
    short maxWipeTime;
    // offset: 0x00D0
    short wipeType;
    // offset: 0x00D2
    short numGSignals;
    // offset: 0x00D4 (64 bytes)
    void *gSignal[16];
    // offset: 0x0114 (4 bytes)
    LightInfo *lightInfo;
    // offset: 0x0118
    void *reqDisp;
    // offset: 0x011C
    long *drawTimerReturn;
    // offset: 0x0120
    long usecsStartDraw;
    // offset: 0x0124
    void *disp;
    // offset: 0x0128
    unsigned long displayFrameCount;
    // offset: 0x012C
    unsigned long frameCount;
    // offset: 0x0130
    unsigned long fps30Count;
    // offset: 0x0134
    unsigned long vblFrames;
    // offset: 0x0138
    unsigned long vblCount;
    // offset: 0x013C
    long numMatrices;
    // offset: 0x0140
    long gameFlags;
    // offset: 0x0144
    long streamFlags;
    // offset: 0x0148
    void *drawNonAnimatedSegmentFunc;
    // offset: 0x014C
    void *drawAnimatedModelFunc;
    // offset: 0x0150
    void *drawDisplayPolytopeListFunc;
    // offset: 0x0154
    void *drawBgFunc;
    // offset: 0x0158 (4 bytes)
    Level *mainDrawUnit;
    // offset: 0x015C (16 bytes)
    char baseAreaName[16];
    // offset: 0x016C
    short levelDone;
    // offset: 0x016E
    short levelChange;
    // offset: 0x0170
    short hideBG;
    // offset: 0x0172
    short gameMode;
    // offset: 0x0174
    long currentHotSpot;
    // offset: 0x0178
    long StreamUnitID;
    // offset: 0x017C
    short SwitchToNewStreamUnit;
    // offset: 0x017E
    short SwitchToNewWarpIndex;
    // offset: 0x0180 (16 bytes)
    char S_baseAreaName[16];
    // offset: 0x0190
    short toSignal;
    // offset: 0x0192
    short fromSignal;
    // offset: 0x0194
    char LastSignal;
    // offset: 0x0196
    short StreamNormalA;
    // offset: 0x0198
    short StreamNormalB;
    // offset: 0x019A
    short StreamNormalC;
    // offset: 0x019C
    long StreamNormalD;
    // offset: 0x01A0
    long moveRazielToStreamID;
    // offset: 0x01A4 (4 bytes)
    ColorType animObjLine;
    // offset: 0x01A8 (4 bytes)
    ColorType animObjShade;
    // offset: 0x01AC
    long maxIntroFXTime;
    // offset: 0x01B0 (20 bytes)
    gSoundData sound;
    // offset: 0x01C4
    short controllerMode;
    // offset: 0x01C6
    unsigned char plan_collide_override;
    // offset: 0x01C7
    unsigned char cheatMode;
    // offset: 0x01C8
    char currentLvl;
    // offset: 0x01C9
    char lastLvl;
    // offset: 0x01CC (4 bytes)
    Object *introFX;
    // offset: 0x01D0 (4 bytes)
    Intro *introFXIntro;
    // offset: 0x01D4
    unsigned long **drawOT;
    // offset: 0x01D8
    unsigned long **dispOT;
    // offset: 0x01DC (4 bytes)
    P_TAG *savedOTStart;
    // offset: 0x01E0 (4 bytes)
    P_TAG *savedOTEnd;
    // offset: 0x01E4
    long introWaitTime;
    // offset: 0x01E8
    long mirrorZPush;
    // offset: 0x01EC
    long defVVRemoveDist;
    // offset: 0x01F0
    long defRemoveDist;
    // offset: 0x01F4 (6 bytes)
    Position forcedStartPosition;
    // offset: 0x01FA
    short hudCollDisplay;
    // offset: 0x01FC
    long primMemUsed;
    // offset: 0x0200
    long cheatTimerCount;
    // offset: 0x0204
    long playerCheatFlags;
    // offset: 0x0208
    long savedPlayerCameraMode;
    // offset: 0x020C
    long debugDrawFlags;
    // offset: 0x0210
    void *planningPool;
    // offset: 0x0214
    void *enemyPlanPool;
    // offset: 0x0218
    unsigned char block_collide_override;
    // offset: 0x0219
    unsigned char raziel_collide_override;
    // offset: 0x021A
    short timeOfDay;
    // offset: 0x021C
    long decoupleGame;
    // offset: 0x0220
    long multGameTime;
    // offset: 0x0224
    short spectral_fadeValue;
    // offset: 0x0226
    short material_fadeValue;
    // offset: 0x0228
    unsigned long drawTime;
    // offset: 0x022C
    unsigned long currentTime;
    // offset: 0x0230
    unsigned long currentMaterialTime;
    // offset: 0x0234
    unsigned long currentSpectralTime;
    // offset: 0x0238
    unsigned long currentTimeOfDayTime;
    // offset: 0x023C
    unsigned long lastLoopTime;
    // offset: 0x0240
    unsigned long timeMult;
    // offset: 0x0244
    unsigned long globalTimeMult;
    // offset: 0x0248
    unsigned long spectralTimeMult;
    // offset: 0x024C
    unsigned long materialTimeMult;
    // offset: 0x0250
    unsigned long currentTicks;
    // offset: 0x0254
    unsigned long totalTime;
    // offset: 0x0258
    unsigned long idleTime;
    // offset: 0x025C
    long visibleInstances;
    // offset: 0x0260
    int gameFramePassed;
    // offset: 0x0264
    unsigned long timeSinceLastGameFrame;
    // offset: 0x0268
    long frameRateLock;
    // offset: 0x026C
    short frameRate24fps;
    // offset: 0x026E
    char monster_collide_override;
    // offset: 0x026F
    char pad;
} GameTracker;

// size: 0xC
typedef struct _HVertex
{
    // offset: 0x0000
    short x;
    // offset: 0x0002
    short y;
    // offset: 0x0004
    short z;
    // offset: 0x0006
    short normal;
    // offset: 0x0008
    long pad;
} HVertex;

// size: 0x8
typedef struct _MONAPI_Regenerator
{
    // offset: 0x0000
    unsigned long regenTime;
    // offset: 0x0004
    short introUniqueID;
    // offset: 0x0006
    short streamUnitID;
} MONAPI_Regenerator;

// size: 0x8
typedef struct __Event
{
    // offset: 0x0000
    int ID;
    // offset: 0x0004
    intptr_t Data;
} Message;

// size: 0x88
typedef struct __MessageQueue
{
    // offset: 0x0000
    int Head;
    // offset: 0x0004
    int Tail;
    // offset: 0x0008 (128 bytes)
    Message Queue[16];
} MessageQueue;

// size: 0x11C
typedef struct __State
{
    // offset: 0x0000
    void (*Process)();
    // offset: 0x0004 (136 bytes)
    MessageQueue Event;
    // offset: 0x008C (136 bytes)
    MessageQueue Defer;
    // offset: 0x0114
    int Data1;
    // offset: 0x0118
    int Data2;
} State;

// size: 0x4
/*typedef struct _physadr {
}*physadr;*/

// size: 0x18
typedef struct POLY_F4
{
    // offset: 0x0000
    unsigned long tag;
    // offset: 0x0004
    unsigned char r0;
    // offset: 0x0005
    unsigned char g0;
    // offset: 0x0006
    unsigned char b0;
    // offset: 0x0007
    unsigned char code;
    // offset: 0x0008
    short x0;
    // offset: 0x000A
    short y0;
    // offset: 0x000C
    short x1;
    // offset: 0x000E
    short y1;
    // offset: 0x0010
    short x2;
    // offset: 0x0012
    short y2;
    // offset: 0x0014
    short x3;
    // offset: 0x0016
    short y3;
} POLY_F4;

// size: 0x6
typedef struct _SmallRotation
{
    // offset: 0x0000
    short x;
    // offset: 0x0002
    short y;
    // offset: 0x0004
    short z;
} SmallRotation;

// size: 0x4C4
typedef struct Camera
{
    // offset: 0x0000 (224 bytes)
    CameraCore core;
    // offset: 0x00E0
    long shake;
    // offset: 0x00E4
    short shakeFrame;
    // offset: 0x00E6
    short shakeScale;
    // offset: 0x00E8
    long flags;
    // offset: 0x00EC
    long lock;
    // offset: 0x00F0
    short mode;
    // offset: 0x00F2
    short state;
    // offset: 0x00F4
    short posState;
    // offset: 0x00F6
    short rotState;
    // offset: 0x00F8
    short tiltState;
    // offset: 0x00FA
    short distanceState;
    // offset: 0x00FC
    short lagZ;
    // offset: 0x00FE
    short tilt;
    // offset: 0x0100 (6 bytes)
    Position focusPoint;
    // offset: 0x0106
    short focusDistance;
    // offset: 0x0108 (4 bytes)
    Instance *focusInstance;
    // offset: 0x010C (6 bytes)
    Position oldFocusInstancePos;
    // offset: 0x0112 (6 bytes)
    Position newFocusInstancePos;
    // offset: 0x0118 (8 bytes)
    Rotation oldFocusInstanceRot;
    // offset: 0x0120 (8 bytes)
    Rotation newFocusInstanceRot;
    // offset: 0x0128 (8 bytes)
    SVector focusInstanceVelVec;
    // offset: 0x0130 (6 bytes)
    Position focusLeadPivot;
    // offset: 0x0136 (6 bytes)
    Position focusOffset;
    // offset: 0x013C (8 bytes)
    Rotation focusRotation;
    // offset: 0x0144
    short tiltVel;
    // offset: 0x0146
    short tiltAccl;
    // offset: 0x0148 (8 bytes)
    SVector positionVel;
    // offset: 0x0150 (8 bytes)
    SVector positionAccl;
    // offset: 0x0158 (8 bytes)
    SVector focusPointVel;
    // offset: 0x0160 (8 bytes)
    SVector focusPointAccl;
    // offset: 0x0168
    short focusDistanceVel;
    // offset: 0x016A
    short focusDistanceAccl;
    // offset: 0x016C (8 bytes)
    SVector focusRotVel;
    // offset: 0x0174 (8 bytes)
    SVector focusRotAccl;
    // offset: 0x017C (8 bytes)
    SVector maxFocusVel;
    // offset: 0x0184 (8 bytes)
    SVector rotationVel;
    // offset: 0x018C (8 bytes)
    SVector rotationAccl;
    // offset: 0x0194
    short maxVel;
    // offset: 0x0196
    short maxRot;
    // offset: 0x0198 (6 bytes)
    Position targetPos;
    // offset: 0x019E
    short targetTilt;
    // offset: 0x01A0 (8 bytes)
    Rotation targetRotation;
    // offset: 0x01A8
    short targetFocusDistance;
    // offset: 0x01AA (6 bytes)
    Position targetFocusPoint;
    // offset: 0x01B0
    short collisionTargetFocusDistance;
    // offset: 0x01B2 (8 bytes)
    Rotation targetFocusRotation;
    // offset: 0x01BA
    short tfaceTilt;
    // offset: 0x01BC (8 bytes)
    Rotation collisionTargetFocusRotation;
    // offset: 0x01C4
    short smooth;
    // offset: 0x01C8 (4 bytes)
    CameraKey *cameraKey;
    // offset: 0x01CC (12 bytes)
    Sphere focusSphere;
    // offset: 0x01D8 (12 bytes)
    Sphere smallBaseSphere;
    // offset: 0x01E4 (12 bytes)
    Sphere posSphere;
    // offset: 0x01F0
    short lookTimer;
    // offset: 0x01F2
    short lookSavedMode;
    // offset: 0x01F4
    short presetIndex;
    // offset: 0x01F6 (6 bytes)
    short presetFocusDistance[3];
    // offset: 0x01FC
    long minFocusDistance;
    // offset: 0x0200
    long maxFocusDistance;
    // offset: 0x0204
    long minTilt;
    // offset: 0x0208
    long maxTilt;
    // offset: 0x020C (12 bytes)
    long pad[3];
    // offset: 0x0218
    long minXYDist;
    // offset: 0x021C
    long maxXYDist;
    // offset: 0x0220
    long oppositeError;
    // offset: 0x0224
    long tempsplinePointAhead;
    // offset: 0x0228
    long noTeleport;
    // offset: 0x022C (36 bytes)
    long focusDistanceList[3][3];
    // offset: 0x0250 (36 bytes)
    long tiltList[3][3];
    // offset: 0x0274
    long cineControl;
    // offset: 0x0278
    short stack;
    // offset: 0x027A
    short targetStack;
    // offset: 0x027C (12 bytes)
    long savedMode[3];
    // offset: 0x0288 (12 bytes)
    long savedTargetFocusDistance[3];
    // offset: 0x0294 (8 bytes)
    Rotation savedFocusRotation;
    // offset: 0x029C
    short savedFocusDistance;
    // offset: 0x029E
    short signalFocusDistance;
    // offset: 0x02A0 (336 bytes)
    SavedCinematic savedCinematic[3];
    // offset: 0x03F0 (8 bytes)
    Rotation savedfocusRotation;
    // offset: 0x03F8 (6 bytes)
    Position signalPos;
    // offset: 0x03FE
    short pad11;
    // offset: 0x0400 (8 bytes)
    Rotation signalRot;
    // offset: 0x0408
    short forced_movement;
    // offset: 0x040A
    short last_forced_movement;
    // offset: 0x040C
    short rotDirection;
    // offset: 0x0410
    unsigned long lastModTime;
    // offset: 0x0414 (8 bytes)
    Rotation lookRot;
    // offset: 0x041C (4 bytes)
    MultiSpline *Spline00;
    // offset: 0x0420 (4 bytes)
    MultiSpline *Spline01;
    // offset: 0x0424 (96 bytes)
    struct
    {
        // offset: 0x0000 (28 bytes)
        struct
        {
            // offset: 0x0000 (4 bytes)
            MultiSpline *posSpline;
            // offset: 0x0004 (4 bytes)
            MultiSpline *targetSpline;
            // offset: 0x0008 (8 bytes)
            SVector lastSplinePos;
            // offset: 0x0010
            short splinePointAhead;
            // offset: 0x0012
            short cinema_done;
            // offset: 0x0014
            int splinecam_currkey;
            // offset: 0x0018
            int splinecam_helpkey;
        } Cinematic;
        // offset: 0x001C (20 bytes)
        struct
        {
            // offset: 0x0000 (6 bytes)
            Position offset;
            // offset: 0x0006 (8 bytes)
            Rotation rotation;
            // offset: 0x0010
            long focusDistance;
        } Idle;
        // offset: 0x0030 (28 bytes)
        struct
        {
            // offset: 0x0000 (4 bytes)
            TFace *tface;
            // offset: 0x0004 (4 bytes)
            TFace *oldTFace;
            // offset: 0x0008 (8 bytes)
            SVector oldTargetRot;
            // offset: 0x0010
            long timer;
            // offset: 0x0014 (8 bytes)
            SVector oldFocusInstancePos;
        } Behind;
        // offset: 0x004C (20 bytes)
        struct
        {
            // offset: 0x0000
            long stopTimer;
            // offset: 0x0004
            long stopHit;
            // offset: 0x0008 (4 bytes)
            TFace *tface;
            // offset: 0x000C (4 bytes)
            TFace *oldTFace;
            // offset: 0x0010
            long hit;
        } Follow;
    } data;
    // offset: 0x0484
    long cuckooTimer;
    // offset: 0x0488
    short teleportZRot;
    // offset: 0x048A
    short extraZRot;
    // offset: 0x048C
    short extraXRot;
    // offset: 0x048E
    short leftTimer;
    // offset: 0x0490
    short rightTimer;
    // offset: 0x0492
    short collideRotControl;
    // offset: 0x0494
    short collision_lastPush;
    // offset: 0x0496
    short collision_timer;
    // offset: 0x0498
    unsigned long prev_instance_mode;
    // offset: 0x049C
    unsigned long instance_mode;
    // offset: 0x04A0
    long instance_xyvel;
    // offset: 0x04A4
    long instance_prev_xyvel;
    // offset: 0x04A8 (6 bytes)
    Position real_focuspoint;
    // offset: 0x04AE
    short always_rotate_flag;
    // offset: 0x04B0
    short follow_flag;
    // offset: 0x04B2
    short actual_x_rot;
    // offset: 0x04B4
    short actual_vel_x;
    // offset: 0x04B6
    short actual_acc_x;
    // offset: 0x04B8
    short x_rot_change;
    // offset: 0x04BA
    short focuspoint_fallz;
    // offset: 0x04BC
    short lead_angle;
    // offset: 0x04BE
    short lead_vel;
    // offset: 0x04C0
    short lead_accl;
    // offset: 0x04C2
    short lead_timer;
} Camera;

// size: 0x18
typedef struct DebugMenuLine
{
    // offset: 0x0000 (4 bytes)
    DEBUG_LINE_TYPE type;
    // offset: 0x0004
    long lower;
    // offset: 0x0008
    long upper;
    // offset: 0x000C
    char *text;
    // offset: 0x0010
    long *var_address;
    // offset: 0x0014
    long bit_mask;
} DebugMenuLine;

// size: 0x40
typedef struct _StreamUnit
{
    // offset: 0x0000
    long StreamUnitID;
    // offset: 0x0004
    short used;
    // offset: 0x0006
    short flags;
    // offset: 0x0008 (4 bytes)
    Level *level;
    // offset: 0x000C (16 bytes)
    char baseAreaName[16];
    // offset: 0x001C
    short toSignal;
    // offset: 0x001E
    short fromSignal;
    // offset: 0x0020
    long FrameCount;
    // offset: 0x0024 (12 bytes)
    short eventVariables[6];
    // offset: 0x0030
    long FogColor;
    // offset: 0x0034
    short UnitFogFar;
    // offset: 0x0036
    short UnitFogNear;
    // offset: 0x0038
    short TargetFogFar;
    // offset: 0x003A
    short TargetFogNear;
    // offset: 0x003C
    short sfxFileHandle;
    // offset: 0x003E
    short pad;
} StreamUnit;

// size: 0x1C
typedef struct _BlockVramEntry
{
    // offset: 0x0000 (4 bytes)
    struct _BlockVramEntry *next;
    // offset: 0x0004
    unsigned char type;
    // offset: 0x0005
    unsigned char flags;
    // offset: 0x0006
    short time;
    // offset: 0x0008
    long ID;
    // offset: 0x000C
    short x;
    // offset: 0x000E
    short y;
    // offset: 0x0010
    short w;
    // offset: 0x0012
    short h;
    // offset: 0x0014
    long area;
    // offset: 0x0018 (4 bytes)
    union
    {
        // offset: 0x0000 (4 bytes)
        StreamUnit *streamUnit;
        // offset: 0x0000 (4 bytes)
        ObjectTracker *streamObject;
        // offset: 0x0000 (4 bytes)
        struct _ButtonTexture *button;
    } udata;
} BlockVramEntry;

// size: 0x10
typedef struct _ButtonTexture
{
    // offset: 0x0000
    short tpage;
    // offset: 0x0002
    short clut;
    // offset: 0x0004
    short textureW;
    // offset: 0x0006
    short textureH;
    // offset: 0x0008 (4 bytes)
    BlockVramEntry *vramBlock;
    // offset: 0x000C
    long xshift;
} ButtonTexture;

// size: 0x6
typedef struct FontChar
{
    // offset: 0x0000
    short x;
    // offset: 0x0002
    short y;
    // offset: 0x0004
    char c;
    // offset: 0x0005
    char color;
} FontChar;

// size: 0x10
typedef struct _BigFileEntry
{
    // offset: 0x0000
    long fileHash;
    // offset: 0x0004
    long fileLen;
    // offset: 0x0008
    long filePos;
    // offset: 0x000C
    long checkSumFull;
} BigFileEntry;

// size: 0x8
typedef struct _BigFileDirEntry
{
    // offset: 0x0000
    short streamUnitID;
    // offset: 0x0002
    short numFiles;
    // offset: 0x0004
    long subDirOffset;
} BigFileDirEntry;

// size: 0x4
typedef struct _BigFileDir
{
    // offset: 0x0000
    long numFiles;
    // offset: 0x0004
    BigFileEntry fileList[0];
} BigFileDir;

// size: 0x24
typedef struct _BigFile
{
    // offset: 0x0000
    long bigfileBaseOffset;
    // offset: 0x0004 (4 bytes)
    BigFileDir *rootDir;
    // offset: 0x0008 (4 bytes)
    BigFileDir *currentDir;
    // offset: 0x000C (4 bytes)
    BigFileDir *cachedDir;
    // offset: 0x0010
    long currentDirID;
    // offset: 0x0014
    long cachedDirID;
    // offset: 0x0018
    long searchDirID;
    // offset: 0x001C
    long numSubDirs;
    // offset: 0x0020 (4 bytes)
    BigFileDirEntry *subDirList;
} BigFile;

// size: 0x64
typedef struct _NonBlockLoadEntry
{
    // offset: 0x0000
    long fileHash;
    // offset: 0x0004
    long dirHash;
    // offset: 0x0008
    long filePos;
    // offset: 0x000C
    long *loadAddr;
    // offset: 0x0010
    long loadSize;
    // offset: 0x0014
    long checksum;
    // offset: 0x0018
    long checksumType;
    // offset: 0x001C
    long memType;
    // offset: 0x0020
    long posInFile;
    // offset: 0x0024
    void *retFunc;
    // offset: 0x0028
    void *retData;
    // offset: 0x002C
    void *retData2;
    // offset: 0x0030
    void **retPointer;
    // offset: 0x0034 (48 bytes)
    char fileName[48];
} NonBlockLoadEntry;

// size: 0x2C
typedef struct _ReadQueueEntry
{
    // offset: 0x0000
    long readStartPos;
    // offset: 0x0004
    long readSize;
    // offset: 0x0008
    long readCurSize;
    // offset: 0x000C
    void *readStartDest;
    // offset: 0x0010
    void *readCurDest;
    // offset: 0x0014
    volatile long readStatus;
    // offset: 0x0018
    long checksum;
    // offset: 0x001C
    long checksumType;
    // offset: 0x0020
    void *retFunc;
    // offset: 0x0024
    void *retData;
    // offset: 0x0028
    void *retData2;
} ReadQueueEntry;

// size: 0x2C
typedef struct _FX_MATRIX
{
    // offset: 0x0000 (8 bytes)
    NodeType node;
    // offset: 0x0008
    long flags;
    // offset: 0x000C (32 bytes)
    Transform lwTransform;
} FX_MATRIX;

// size: 0x68
typedef struct _FX_PRIM
{
    // offset: 0x0000 (8 bytes)
    NodeType node;
    // offset: 0x0008 (4 bytes)
    TextureMT3 *texture;
    // offset: 0x000C
    long flags;
    // offset: 0x0010
    short timeToLive;
    // offset: 0x0012
    short curFrame;
    // offset: 0x0014
    // void *process;
    void (*process)();
    // offset: 0x0018 (4 bytes)
    FX_MATRIX *matrix;
    // offset: 0x001C
    long color;
    // offset: 0x0020
    long startColor;
    // offset: 0x0024
    long endColor;
    // offset: 0x0028
    short colorFadeValue;
    // offset: 0x002A
    short colorFadeStep;
    // offset: 0x002C (6 bytes)
    Position position;
    // offset: 0x0032
    short fadeStep;
    // offset: 0x0034 (8 bytes)
    short fadeValue[4];
    // offset: 0x003C (6 bytes)
    Position v0;
    // offset: 0x0042
    short work0;
    // offset: 0x0044 (6 bytes)
    Position v1;
    // offset: 0x004A
    short work1;
    // offset: 0x004C (6 bytes)
    Position v2;
    // offset: 0x0052
    short work2;
    // offset: 0x0054 (6 bytes)
    Position v3;
    // offset: 0x005A
    short work3;
    // offset: 0x005C (12 bytes)
    union
    {
        // offset: 0x0000 (8 bytes)
        struct
        {
            // offset: 0x0000 (4 bytes)
            Instance *parent;
            // offset: 0x0004
            long segment;
        } flame;
        // offset: 0x0000 (12 bytes)
        struct
        {
            // offset: 0x0000
            short xVel;
            // offset: 0x0002
            short yVel;
            // offset: 0x0004
            short zVel;
            // offset: 0x0006
            short xAccl;
            // offset: 0x0008
            short yAccl;
            // offset: 0x000A
            short zAccl;
        } phys;
    } duo;
} FX_PRIM;

// size: 0x10
typedef struct BASIC_LINE_G2
{
    // offset: 0x0000
    unsigned char r0;
    // offset: 0x0001
    unsigned char g0;
    // offset: 0x0002
    unsigned char b0;
    // offset: 0x0003
    unsigned char code;
    // offset: 0x0004
    short x0;
    // offset: 0x0006
    short y0;
    // offset: 0x0008
    unsigned char r1;
    // offset: 0x0009
    unsigned char g1;
    // offset: 0x000A
    unsigned char b1;
    // offset: 0x000B
    unsigned char p1;
    // offset: 0x000C
    short x1;
    // offset: 0x000E
    short y1;
} BASIC_LINE_G2;

// size: 0x20
typedef struct _BASIC_G4
{
    // offset: 0x0000
    unsigned char r0;
    // offset: 0x0001
    unsigned char g0;
    // offset: 0x0002
    unsigned char b0;
    // offset: 0x0003
    unsigned char code;
    // offset: 0x0004
    short x0;
    // offset: 0x0006
    short y0;
    // offset: 0x0008
    unsigned char r1;
    // offset: 0x0009
    unsigned char g1;
    // offset: 0x000A
    unsigned char b1;
    // offset: 0x000B
    unsigned char pad1;
    // offset: 0x000C
    short x1;
    // offset: 0x000E
    short y1;
    // offset: 0x0010
    unsigned char r2;
    // offset: 0x0011
    unsigned char g2;
    // offset: 0x0012
    unsigned char b2;
    // offset: 0x0013
    unsigned char pad2;
    // offset: 0x0014
    short x2;
    // offset: 0x0016
    short y2;
    // offset: 0x0018
    unsigned char r3;
    // offset: 0x0019
    unsigned char g3;
    // offset: 0x001A
    unsigned char b3;
    // offset: 0x001B
    unsigned char pad3;
    // offset: 0x001C
    short x3;
    // offset: 0x001E
    short y3;
} BASIC_G4;

// size: 0x14
typedef struct _BASIC_F4
{
    // offset: 0x0000
    unsigned char r0;
    // offset: 0x0001
    unsigned char g0;
    // offset: 0x0002
    unsigned char b0;
    // offset: 0x0003
    unsigned char code;
    // offset: 0x0004
    short x0;
    // offset: 0x0006
    short y0;
    // offset: 0x0008
    short x1;
    // offset: 0x000A
    short y1;
    // offset: 0x000C
    short x2;
    // offset: 0x000E
    short y2;
    // offset: 0x0010
    short x3;
    // offset: 0x0012
    short y3;
} BASIC_F4;

// size: 0x14
typedef struct __BubbleParams
{
    // offset: 0x0000
    short DisperseFrames;
    // offset: 0x0002
    short KillScale;
    // offset: 0x0004
    short MinSplashSize;
    // offset: 0x0006
    short MaxSpeed;
    // offset: 0x0008
    short MaxSpeedRange;
    // offset: 0x000A
    short ScaleRate;
    // offset: 0x000C
    short ScaleRateRange;
    // offset: 0x000E
    short StartScale;
    // offset: 0x0010
    short StartScaleRange;
    // offset: 0x0012
    short UniqueBubbles;
} BubbleParams;

// size: 0x30
typedef struct _GenericParticleParams
{
    // offset: 0x0000
    short size;
    // offset: 0x0002
    unsigned char StartOnInit;
    // offset: 0x0003
    unsigned char type;
    // offset: 0x0004
    short birthRadius;
    // offset: 0x0006
    char startSegment;
    // offset: 0x0007
    char endSegment;
    // offset: 0x0008 (6 bytes)
    Position direction;
    // offset: 0x000E
    unsigned char spectral_colorize;
    // offset: 0x000F
    unsigned char absolute_direction;
    // offset: 0x0010
    char accx;
    // offset: 0x0011
    char accy;
    // offset: 0x0012
    char accz;
    // offset: 0x0013
    char useInstanceModel;
    // offset: 0x0014
    unsigned char startColor_red;
    // offset: 0x0015
    unsigned char startColor_green;
    // offset: 0x0016
    unsigned char startColor_blue;
    // offset: 0x0017
    char model;
    // offset: 0x0018
    unsigned char endColor_red;
    // offset: 0x0019
    unsigned char endColor_green;
    // offset: 0x001A
    unsigned char endColor_blue;
    // offset: 0x001B
    char texture;
    // offset: 0x001C
    short lifeTime;
    // offset: 0x001E
    unsigned char primLifeTime;
    // offset: 0x001F
    char use_child;
    // offset: 0x0020
    short startFadeValue;
    // offset: 0x0022
    short fadeStep;
    // offset: 0x0024
    char numberBirthParticles;
    // offset: 0x0025
    char z_undulation_mode;
    // offset: 0x0026
    short scaleSpeed;
    // offset: 0x0028 (6 bytes)
    Position offset;
    // offset: 0x002E
    short startScale;
} GenericParticleParams;

// size: 0x14
typedef struct _GenericRibbonParams
{
    // offset: 0x0000
    char id;
    // offset: 0x0001
    char StartOnInit;
    // offset: 0x0002
    char startSegment;
    // offset: 0x0003
    char endSegment;
    // offset: 0x0004
    char type;
    // offset: 0x0005
    char use_child;
    // offset: 0x0006
    short ribbonLifeTime;
    // offset: 0x0008
    short faceLifeTime;
    // offset: 0x000A
    short startFadeValue;
    // offset: 0x000C
    long startColor;
    // offset: 0x0010
    long endColor;
} GenericRibbonParams;

// size: 0x18
typedef struct _GenericGlowParams
{
    // offset: 0x0000
    char StartOnInit;
    // offset: 0x0001
    unsigned char segment;
    // offset: 0x0002
    unsigned char segmentEnd;
    // offset: 0x0003
    char numSegments;
    // offset: 0x0004
    unsigned char color_num;
    // offset: 0x0005
    char use_child;
    // offset: 0x0006
    unsigned char numColors;
    // offset: 0x0007
    char id;
    // offset: 0x0008
    long atuColorCycleRate;
    // offset: 0x000C
    short width;
    // offset: 0x000E
    short height;
    // offset: 0x0010
    long lifetime;
    // offset: 0x0014
    short fadein_time;
    // offset: 0x0016
    short fadeout_time;
} GenericGlowParams;

// size: 0x28
typedef struct _GenericLightningParams
{
    // offset: 0x0000
    char type;
    // offset: 0x0001
    char use_child;
    // offset: 0x0002
    short lifeTime;
    // offset: 0x0004
    short deg;
    // offset: 0x0006
    short deg_inc;
    // offset: 0x0008 (6 bytes)
    Position start_offset;
    // offset: 0x000E
    char startSeg;
    // offset: 0x000F
    char endSeg;
    // offset: 0x0010 (6 bytes)
    Position end_offset;
    // offset: 0x0016
    char matrixSeg;
    // offset: 0x0017
    unsigned char segs;
    // offset: 0x0018
    short width;
    // offset: 0x001A
    short small_width;
    // offset: 0x001C
    short sine_size;
    // offset: 0x001E
    short variation;
    // offset: 0x0020
    long color;
    // offset: 0x0024
    long glow_color;
} GenericLightningParams;

// size: 0x30
typedef struct _GenericBlastringParams
{
    // offset: 0x0000
    char type;
    // offset: 0x0001
    char use_child;
    // offset: 0x0002
    short lifeTime;
    // offset: 0x0004 (6 bytes)
    Position offset;
    // offset: 0x000A
    short matrixSeg;
    // offset: 0x000C
    char segment;
    // offset: 0x000D
    char sortInWorld;
    // offset: 0x000E
    short radius;
    // offset: 0x0010
    short size1;
    // offset: 0x0012
    short size2;
    // offset: 0x0014
    short endRadius;
    // offset: 0x0016
    short colorchange_radius;
    // offset: 0x0018
    int vel;
    // offset: 0x001C
    int accl;
    // offset: 0x0020
    short height1;
    // offset: 0x0022
    short height2;
    // offset: 0x0024
    short height3;
    // offset: 0x0026
    char predator_offset;
    // offset: 0x0027
    char stay_in_place;
    // offset: 0x0028
    long startColor;
    // offset: 0x002C
    long endColor;
} GenericBlastringParams;

// size: 0xC
typedef struct _GenericFlashParams
{
    // offset: 0x0000
    short type;
    // offset: 0x0002
    short timeToColor;
    // offset: 0x0004
    long color;
    // offset: 0x0008
    short timeAtColor;
    // offset: 0x000A
    short timeFromColor;
} GenericFlashParams;

// size: 0x4
typedef struct XAFileInfo
{
    // offset: 0x0000
    int startPos;
} XAFileInfo;

// size: 0x8
typedef struct CdCommand
{
    // offset: 0x0000
    unsigned short flags;
    // offset: 0x0002
    unsigned char unused;
    // offset: 0x0003
    unsigned char cdCommand;
    // offset: 0x0004 (4 bytes)
    unsigned char cdCmdParam[4];
} CdCommand;

// size: 0x4
typedef struct VoiceCommand
{
    // offset: 0x0000
    unsigned char voiceCmd;
    // offset: 0x0001
    unsigned char nextVoiceStatus;
    // offset: 0x0002
    unsigned short voiceCmdParam;
} VoiceCommand;

// size: 0x70
typedef struct _LoadQueueEntry
{
    // offset: 0x0000 (4 bytes)
    struct _LoadQueueEntry *next;
    // offset: 0x0004
    short status;
    // offset: 0x0006
    char relocateBinary;
    // offset: 0x0007
    char mempackUsed;
    // offset: 0x0008 (100 bytes)
    NonBlockLoadEntry loadEntry;
    // offset: 0x006C
    long endLoadTime;
} LoadQueueEntry;

// size: 0x2
typedef struct SavedBasic
{
    // offset: 0x0000
    unsigned char savedID;
    // offset: 0x0001
    unsigned char shiftedSaveSize;
} SavedBasic;

// size: 0x28
typedef struct _SavedIntro
{
    // offset: 0x0000
    unsigned char savedID;
    // offset: 0x0001
    unsigned char shiftedSaveSize;
    // offset: 0x0002
    unsigned char lightGroup;
    // offset: 0x0003
    unsigned char specturalLightGroup;
    // offset: 0x0004 (8 bytes)
    char name[8];
    // offset: 0x000C
    short introUniqueID;
    // offset: 0x000E
    short streamUnitID;
    // offset: 0x0010
    short birthUnitID;
    // offset: 0x0012
    unsigned short attachedUniqueID;
    // offset: 0x0014 (6 bytes)
    SmallRotation smallRotation;
    // offset: 0x001A (6 bytes)
    Position position;
    // offset: 0x0020
    long flags;
    // offset: 0x0024
    long flags2;
} SavedIntro;

// size: 0x10
typedef struct SavedEvent
{
    // offset: 0x0000
    unsigned char savedID;
    // offset: 0x0001
    unsigned char shiftedSaveSize;
    // offset: 0x0002
    short areaID;
    // offset: 0x0004
    short eventNumber;
    // offset: 0x0006 (10 bytes)
    short eventVariables[5];
} SavedEvent;

// size: 0x8
typedef struct SavedBspTree
{
    // offset: 0x0000 (6 bytes)
    Position localOffset;
    // offset: 0x0006
    unsigned char importantFlagsSaved;
    // offset: 0x0007
    unsigned char bspIndex;
} SavedBspTree;

// size: 0xC
typedef struct EventWildCardObject
{
    // offset: 0x0000
    short id;
    // offset: 0x0002
    short spad;
    // offset: 0x0004
    long unitID;
    // offset: 0x0008
    char *objectName;
} EventWildCardObject;

// size: 0x18
typedef struct InstanceSpline
{
    // offset: 0x0000 (4 bytes)
    Instance *instance;
    // offset: 0x0004 (4 bytes)
    MultiSpline *spline;
    // offset: 0x0008
    long attribute;
    // offset: 0x000C
    long isParent;
    // offset: 0x0010
    long isClass;
    // offset: 0x0014
    long splineFlags;
} InstanceSpline;

// size: 0x14
typedef struct Vector3d
{
    // offset: 0x0000
    short vx;
    // offset: 0x0002
    short vy;
    // offset: 0x0004
    short vz;
    // offset: 0x0006
    short pad1;
    // offset: 0x0008
    short errorx;
    // offset: 0x000A
    short errory;
    // offset: 0x000C
    short errorz;
    // offset: 0x000E
    short pad2;
    // offset: 0x0010
    long streamUnitID;
} Vector3d;

// size: 0x14
typedef struct Rotation3d
{
    // offset: 0x0000
    short vx;
    // offset: 0x0002
    short vy;
    // offset: 0x0004
    short vz;
    // offset: 0x0006
    short pad1;
    // offset: 0x0008
    short errorx;
    // offset: 0x000A
    short errory;
    // offset: 0x000C
    short errorz;
    // offset: 0x000E
    short pad2;
    // offset: 0x0010
    long attribute;
} Rotation3d;

// size: 0x8
typedef struct InstanceObject
{
    // offset: 0x0000 (4 bytes)
    Instance *instance;
    // offset: 0x0004
    long attribute;
} InstanceObject;

// size: 0x8
typedef struct EventObject
{
    // offset: 0x0000 (4 bytes)
    Event *event;
    // offset: 0x0004
    long attribute;
} EventObject;

// size: 0x10
typedef struct SavedEventObject
{
    // offset: 0x0000 (4 bytes)
    SavedBasic *savedEvent;
    // offset: 0x0004
    long attribute;
    // offset: 0x0008
    long areaID;
    // offset: 0x000C
    long eventNumber;
} SavedEventObject;

// size: 0x4
typedef struct GameObject
{
    // offset: 0x0000
    long attribute;
} GameObject;

// size: 0x8
typedef struct SignalObject
{
    // offset: 0x0000 (4 bytes)
    MultiSignal *msignal;
    // offset: 0x0004
    long attribute;
} SignalObject;

// size: 0x8
typedef struct IntroObject
{
    // offset: 0x0000 (4 bytes)
    Intro *intro;
    // offset: 0x0004
    long attribute;
} IntroObject;

// size: 0x8
typedef struct InstanceAnimate
{
    // offset: 0x0000 (4 bytes)
    Instance *instance;
    // offset: 0x0004
    long attribute;
} InstanceAnimate;

// size: 0xC
typedef struct InstanceAnimateTexture
{
    // offset: 0x0000 (4 bytes)
    Instance *instance;
    // offset: 0x0004 (4 bytes)
    AniTex *aniTextures;
    // offset: 0x0008
    long attribute;
} InstanceAnimateTexture;

// size: 0x20
typedef struct ListObject
{
    // offset: 0x0000 (4 bytes)
    EventWildCardObject *eventInstance;
    // offset: 0x0004 (20 bytes)
    long attribute[5];
    // offset: 0x0018
    long numberOfAttributes;
    // offset: 0x001C
    long lineID;
} ListObject;

// size: 0x20
typedef struct SubListObject
{
    // offset: 0x0000 (4 bytes)
    Instance **instanceList;
    // offset: 0x0004
    long numberOfInstances;
    // offset: 0x0008
    long numberOfAttributes;
    // offset: 0x000C (20 bytes)
    long attribute[5];
} SubListObject;

// size: 0x10
typedef struct TGroupObject
{
    // offset: 0x0000 (4 bytes)
    BSPTree *bspTree;
    // offset: 0x0004 (4 bytes)
    StreamUnit *streamUnit;
    // offset: 0x0008
    long attribute;
    // offset: 0x000C (4 bytes)
    short arg[2];
} TGroupObject;

// size: 0x8
typedef struct ConstrictInfo
{
    // offset: 0x0000 (4 bytes)
    Instance *instance;
    // offset: 0x0004
    long attribute;
} ConstrictInfo;

// size: 0xC
typedef struct CameraObject
{
    // offset: 0x0000 (4 bytes)
    Camera *camera;
    // offset: 0x0004
    long frames;
    // offset: 0x0008
    long attribute;
} CameraObject;

// size: 0xC
typedef struct EventVmObject
{
    // offset: 0x0000 (4 bytes)
    VMObject *vmObjectPtr;
    // offset: 0x0004 (4 bytes)
    Level *level;
    // offset: 0x0008
    long attribute;
} EventVmObject;

// size: 0xC
typedef struct AreaObject
{
    // offset: 0x0000 (4 bytes)
    StreamUnit *streamUnit;
    // offset: 0x0004
    long attribute;
    // offset: 0x0008
    long unitID;
} AreaObject;

// size: 0x14
typedef struct SoundObject
{
    // offset: 0x0000
    short flags;
    // offset: 0x0002
    short attribute;
    // offset: 0x0004
    long soundNumber;
    // offset: 0x0008
    long value;
    // offset: 0x000C
    long duration;
    // offset: 0x0010 (4 bytes)
    union
    {
        // offset: 0x0000 (4 bytes)
        Instance *instance;
        // offset: 0x0000 (4 bytes)
        SFXMkr *sfxMarker;
    } data;
} SoundObject;

// size: 0x24
typedef struct StackType
{
    // offset: 0x0000
    long id;
    // offset: 0x0004 (32 bytes)
    union
    {
        // offset: 0x0000 (8 bytes)
        struct
        {
            // offset: 0x0000
            long instanceNumber;
            // offset: 0x0004
            long attribute;
        } Object;
        // offset: 0x0000 (8 bytes)
        struct
        {
            // offset: 0x0000 (4 bytes)
            SavedIntro *intro;
            // offset: 0x0004
            long attribute;
        } SavedIntroObject;
        // offset: 0x0000 (24 bytes)
        InstanceSpline instanceSpline;
        // offset: 0x0000 (12 bytes)
        struct
        {
            // offset: 0x0000
            long currentNumber;
            // offset: 0x0004
            long error;
            // offset: 0x0008
            short flags;
        } Number;
        // offset: 0x0000 (4 bytes)
        struct
        {
            // offset: 0x0000
            short vx;
            // offset: 0x0002
            short vy;
        } Vector2d;
        // offset: 0x0000 (4 bytes)
        struct
        {
            // offset: 0x0000
            char *pointer;
        } CharPointer;
        // offset: 0x0000 (4 bytes)
        struct
        {
            // offset: 0x0000
            short *pointer;
        } ShortPointer;
        // offset: 0x0000 (4 bytes)
        struct
        {
            // offset: 0x0000
            long *pointer;
        } LongPointer;
        // offset: 0x0000 (12 bytes)
        AreaObject areaObject;
        // offset: 0x0000 (8 bytes)
        InstanceObject instanceObject;
        // offset: 0x0000 (20 bytes)
        Vector3d vector3d;
        // offset: 0x0000 (20 bytes)
        Rotation3d rotation3d;
        // offset: 0x0000 (8 bytes)
        SignalObject signalObject;
        // offset: 0x0000 (8 bytes)
        EventObject eventObject;
        // offset: 0x0000 (4 bytes)
        GameObject gameObject;
        // offset: 0x0000 (32 bytes)
        ListObject listObject;
        // offset: 0x0000 (8 bytes)
        IntroObject introObject;
        // offset: 0x0000 (8 bytes)
        InstanceAnimate instanceAnimate;
        // offset: 0x0000 (12 bytes)
        InstanceAnimateTexture instanceAniTexture;
        // offset: 0x0000 (16 bytes)
        SavedEventObject savedEventObject;
        // offset: 0x0000 (32 bytes)
        SubListObject subListObject;
        // offset: 0x0000 (16 bytes)
        TGroupObject terrainGroup;
        // offset: 0x0000 (8 bytes)
        ConstrictInfo constrictInfo;
        // offset: 0x0000 (12 bytes)
        CameraObject cameraObject;
        // offset: 0x0000 (12 bytes)
        EventVmObject vmObject;
        // offset: 0x0000 (20 bytes)
        SoundObject soundObject;
    } data;
} StackType;

// size: 0x4
typedef struct __ReaverSaveDat
{
    // offset: 0x0000
    unsigned long pad;
} ReaverSaveData;

// size: 0x10
typedef struct GameCheat
{
    // offset: 0x0000 (12 bytes)
    unsigned char cheatKeys[12];
    // offset: 0x000C
    short cheatLen;
    // offset: 0x000E
    short cheatStage;
} GameCheat;

// size: 0x8
typedef struct debug_dispatch_t
{
    // offset: 0x0000 (4 bytes)
    DEBUG_LINE_TYPE type;
    // offset: 0x0004
    void (*fn)();
} debug_dispatch_t;

// size: 0xC
typedef struct debug_format_t
{
    // offset: 0x0000
    int xpos;
    // offset: 0x0004
    int ypos;
    // offset: 0x0008
    int is_centered;
} debug_format_t;

// size: 0x28
typedef struct POLY_GT3
{
    // offset: 0x0000
    unsigned long tag;
    // offset: 0x0004
    unsigned char r0;
    // offset: 0x0005
    unsigned char g0;
    // offset: 0x0006
    unsigned char b0;
    // offset: 0x0007
    unsigned char code;
    // offset: 0x0008
    short x0;
    // offset: 0x000A
    short y0;
    // offset: 0x000C
    unsigned char u0;
    // offset: 0x000D
    unsigned char v0;
    // offset: 0x000E
    unsigned short clut;
    // offset: 0x0010
    unsigned char r1;
    // offset: 0x0011
    unsigned char g1;
    // offset: 0x0012
    unsigned char b1;
    // offset: 0x0013
    unsigned char p1;
    // offset: 0x0014
    short x1;
    // offset: 0x0016
    short y1;
    // offset: 0x0018
    unsigned char u1;
    // offset: 0x0019
    unsigned char v1;
    // offset: 0x001A
    unsigned short tpage;
    // offset: 0x001C
    unsigned char r2;
    // offset: 0x001D
    unsigned char g2;
    // offset: 0x001E
    unsigned char b2;
    // offset: 0x001F
    unsigned char p2;
    // offset: 0x0020
    short x2;
    // offset: 0x0022
    short y2;
    // offset: 0x0024
    unsigned char u2;
    // offset: 0x0025
    unsigned char v2;
    // offset: 0x0026
    unsigned short pad2;
} POLY_GT3;

// size: 0x34
typedef struct POLY_GT4
{
    // offset: 0x0000
    unsigned long tag;
    // offset: 0x0004
    unsigned char r0;
    // offset: 0x0005
    unsigned char g0;
    // offset: 0x0006
    unsigned char b0;
    // offset: 0x0007
    unsigned char code;
    // offset: 0x0008
    short x0;
    // offset: 0x000A
    short y0;
    // offset: 0x000C
    unsigned char u0;
    // offset: 0x000D
    unsigned char v0;
    // offset: 0x000E
    unsigned short clut;
    // offset: 0x0010
    unsigned char r1;
    // offset: 0x0011
    unsigned char g1;
    // offset: 0x0012
    unsigned char b1;
    // offset: 0x0013
    unsigned char p1;
    // offset: 0x0014
    short x1;
    // offset: 0x0016
    short y1;
    // offset: 0x0018
    unsigned char u1;
    // offset: 0x0019
    unsigned char v1;
    // offset: 0x001A
    unsigned short tpage;
    // offset: 0x001C
    unsigned char r2;
    // offset: 0x001D
    unsigned char g2;
    // offset: 0x001E
    unsigned char b2;
    // offset: 0x001F
    unsigned char p2;
    // offset: 0x0020
    short x2;
    // offset: 0x0022
    short y2;
    // offset: 0x0024
    unsigned char u2;
    // offset: 0x0025
    unsigned char v2;
    // offset: 0x0026
    unsigned short pad2;
    // offset: 0x0028
    unsigned char r3;
    // offset: 0x0029
    unsigned char g3;
    // offset: 0x002A
    unsigned char b3;
    // offset: 0x002B
    unsigned char p3;
    // offset: 0x002C
    short x3;
    // offset: 0x002E
    short y3;
    // offset: 0x0030
    unsigned char u3;
    // offset: 0x0031
    unsigned char v3;
    // offset: 0x0032
    unsigned short pad3;
} POLY_GT4;

// size: 0xC
typedef struct _Vector
{
    // offset: 0x0000
    long x;
    // offset: 0x0004
    long y;
    // offset: 0x0008
    long z;
} Vector;

// size: 0x28
typedef struct _PFace
{
    // offset: 0x0000
    unsigned long *tag;
    // offset: 0x0004
    unsigned char r0;
    // offset: 0x0005
    unsigned char g0;
    // offset: 0x0006
    unsigned char b0;
    // offset: 0x0007
    unsigned char code;
    // offset: 0x0008
    short x0;
    // offset: 0x000A
    short y0;
    // offset: 0x000C
    unsigned char u0;
    // offset: 0x000D
    unsigned char v0;
    // offset: 0x000E
    short clut;
    // offset: 0x0010
    short x1;
    // offset: 0x0012
    short y1;
    // offset: 0x0014
    unsigned char u1;
    // offset: 0x0015
    unsigned char v1;
    // offset: 0x0016
    unsigned short tpage;
    // offset: 0x0018
    short x2;
    // offset: 0x001A
    short y2;
    // offset: 0x001C
    unsigned char u2;
    // offset: 0x001D
    unsigned char v2;
    // offset: 0x001E
    short pad1;
    // offset: 0x0020
    long pad2;
    // offset: 0x0024
    long pad3;
} PFace;

// size: 0x1004
typedef struct _PolytopeList
{
    // offset: 0x0000
    long numPolytopes;
    // offset: 0x0004 (4096 bytes)
    void *polytope[1024];
} PolytopeList;

// size: 0x4
typedef struct UVTYPE
{
    // offset: 0x0000
    unsigned char u;
    // offset: 0x0001
    unsigned char v;
    // offset: 0x0002
    short pad;
} UVTYPE;

// size: 0x20
typedef struct POLY_G3_SEMITRANS
{
    // offset: 0x0000
    unsigned long tag;
    // offset: 0x0004
    unsigned long dr_tpage;
    // offset: 0x0008
    unsigned char r0;
    // offset: 0x0009
    unsigned char g0;
    // offset: 0x000A
    unsigned char b0;
    // offset: 0x000B
    unsigned char code;
    // offset: 0x000C
    short x0;
    // offset: 0x000E
    short y0;
    // offset: 0x0010
    unsigned char r1;
    // offset: 0x0011
    unsigned char g1;
    // offset: 0x0012
    unsigned char b1;
    // offset: 0x0013
    unsigned char pad1;
    // offset: 0x0014
    short x1;
    // offset: 0x0016
    short y1;
    // offset: 0x0018
    unsigned char r2;
    // offset: 0x0019
    unsigned char g2;
    // offset: 0x001A
    unsigned char b2;
    // offset: 0x001B
    unsigned char pad2;
    // offset: 0x001C
    short x2;
    // offset: 0x001E
    short y2;
} POLY_G3_SEMITRANS;

// size: 0x28
typedef struct _POLY_NG4
{
    // offset: 0x0000
    unsigned long tag;
    // offset: 0x0004
    unsigned long drawTPage1;
    // offset: 0x0008 (32 bytes)
    BASIC_G4 p1;
} POLY_NG4;

// size: 0x1C
typedef struct _GT3ZVertexD3D_Type
{
    // offset: 0x0000
    float x;
    // offset: 0x0004
    float y;
    // offset: 0x0008
    float z;
    // offset: 0x000C
    float rhw;
    // offset: 0x0010
    unsigned int rgba;
    // offset: 0x0014
    float u;
    // offset: 0x0018
    float v;
} GT3ZVertexD3D;

// size: 0xD4
typedef struct SP_SUBDIV_STRUCT
{
    // offset: 0x0000 (8 bytes)
    SVector vertex0;
    // offset: 0x0008 (8 bytes)
    SVector vertex1;
    // offset: 0x0010 (8 bytes)
    SVector vertex2;
    // offset: 0x0018 (8 bytes)
    SVECTOR face_v0;
    // offset: 0x0020 (8 bytes)
    SVECTOR face_v1;
    // offset: 0x0028 (8 bytes)
    SVECTOR face_v2;
    // offset: 0x0030 (8 bytes)
    SVECTOR face_v01;
    // offset: 0x0038 (8 bytes)
    SVECTOR face_v12;
    // offset: 0x0040 (8 bytes)
    SVECTOR face_v20;
    // offset: 0x0048 (4 bytes)
    UVTYPE face_uv0;
    // offset: 0x004C (4 bytes)
    UVTYPE face_uv1;
    // offset: 0x0050 (4 bytes)
    UVTYPE face_uv2;
    // offset: 0x0054 (4 bytes)
    UVTYPE face_uv01;
    // offset: 0x0058 (4 bytes)
    UVTYPE face_uv12;
    // offset: 0x005C (4 bytes)
    UVTYPE face_uv20;
    // offset: 0x0060 (4 bytes)
    ColorType color0;
    // offset: 0x0064 (4 bytes)
    ColorType color1;
    // offset: 0x0068 (4 bytes)
    ColorType color2;
    // offset: 0x006C (4 bytes)
    ColorType color01;
    // offset: 0x0070 (4 bytes)
    ColorType color12;
    // offset: 0x0074 (4 bytes)
    ColorType color20;
    // offset: 0x0078 (48 bytes)
    Vector out[4];
    // offset: 0x00A8 (16 bytes)
    long color[4];
    // offset: 0x00B8 (12 bytes)
    long texinfo[3];
    // offset: 0x00C4 (16 bytes)
    DVECTOR sxy[4];
} SP_SUBDIV_STRUCT;

// size: 0x28
typedef struct POLY_FT4
{
    // offset: 0x0000
    unsigned long tag;
    // offset: 0x0004
    unsigned char r0;
    // offset: 0x0005
    unsigned char g0;
    // offset: 0x0006
    unsigned char b0;
    // offset: 0x0007
    unsigned char code;
    // offset: 0x0008
    short x0;
    // offset: 0x000A
    short y0;
    // offset: 0x000C
    unsigned char u0;
    // offset: 0x000D
    unsigned char v0;
    // offset: 0x000E
    unsigned short clut;
    // offset: 0x0010
    short x1;
    // offset: 0x0012
    short y1;
    // offset: 0x0014
    unsigned char u1;
    // offset: 0x0015
    unsigned char v1;
    // offset: 0x0016
    unsigned short tpage;
    // offset: 0x0018
    short x2;
    // offset: 0x001A
    short y2;
    // offset: 0x001C
    unsigned char u2;
    // offset: 0x001D
    unsigned char v2;
    // offset: 0x001E
    unsigned short pad1;
    // offset: 0x0020
    short x3;
    // offset: 0x0022
    short y3;
    // offset: 0x0024
    unsigned char u3;
    // offset: 0x0025
    unsigned char v3;
    // offset: 0x0026
    unsigned short pad2;
} POLY_FT4;

// size: 0x620
typedef struct FontTracker
{
    // offset: 0x0000 (1536 bytes)
    FontChar font_buffer[256];
    // offset: 0x0600
    long font_xpos;
    // offset: 0x0604
    long font_ypos;
    // offset: 0x0608
    long font_buffIndex;
    // offset: 0x060C
    short font_tpage;
    // offset: 0x060E
    short font_clut;
    // offset: 0x0610
    short font_vramU;
    // offset: 0x0612
    short font_vramV;
    // offset: 0x0614
    short font_width;
    // offset: 0x0616
    short sprite_sort_push;
    // offset: 0x0618
    short font_vramX;
    // offset: 0x061A
    short font_vramY;
    // offset: 0x061C
    char color_global;
    // offset: 0x061D
    char color_local;
} FontTracker;

// size: 0xC
typedef struct font_color_t
{
    // offset: 0x0000
    int r;
    // offset: 0x0004
    int g;
    // offset: 0x0008
    int b;
} font_color_t;

// size: 0x4
typedef struct FontPos
{
    // offset: 0x0000
    char x;
    // offset: 0x0001
    char y;
    // offset: 0x0002
    char w;
    // offset: 0x0003
    char h;
} FontPos;

// size: 0x4
typedef struct JoypadMappingStruct
{
    // offset: 0x0000
    unsigned short psxData;
    // offset: 0x0002
    unsigned short translation;
} JoypadMapping;

// size: 0x6
typedef struct AnalogueData
{
    // offset: 0x0000
    unsigned short pad;
    // offset: 0x0002
    unsigned char xR;
    // offset: 0x0003
    unsigned char yR;
    // offset: 0x0004
    unsigned char xL;
    // offset: 0x0005
    unsigned char yL;
} AnalogueData;

// size: 0x22
typedef struct ControllerPacket
{
    // offset: 0x0000
    unsigned char transStatus;
    // offset: 0x0001
    unsigned char dataFormat;
    // offset: 0x0002 (32 bytes)
    union
    {
        // offset: 0x0000
        unsigned short pad;
        // offset: 0x0000 (32 bytes)
        unsigned short multitap[16];
        // offset: 0x0000 (4 bytes)
        unsigned short mouse[2];
        // offset: 0x0000 (6 bytes)
        unsigned short negcon[3];
        // offset: 0x0000 (6 bytes)
        AnalogueData analogue;
    } data;
} ControllerPacket;

// size: 0x8
typedef struct _G2EulerAngles_Type
{
    // offset: 0x0000
    short x;
    // offset: 0x0002
    short y;
    // offset: 0x0004
    short z;
    // offset: 0x0006
    short order;
} G2EulerAngles;

// size: 0x8
typedef struct _AnimSoundData_Type
{
    // offset: 0x0000
    short sfxToneID;
    // offset: 0x0002
    short volume;
    // offset: 0x0004
    short pitch;
    // offset: 0x0006
    short minVolDistance;
} AnimSoundData;

// size: 0x8
typedef struct _InstanceHandle
{
    // offset: 0x0000 (4 bytes)
    Instance *instance;
    // offset: 0x0004
    long instanceID;
} InstanceHandle;

// size: 0x18
typedef struct evCollideInstanceStatsData
{
    // offset: 0x0000 (4 bytes)
    Instance *instance;
    // offset: 0x0004
    unsigned long distance;
    // offset: 0x0008 (8 bytes)
    SVector relativePosition;
    // offset: 0x0010
    long xyDistance;
    // offset: 0x0014
    long zDelta;
} evCollideInstanceStatsData;

// size: 0x4
typedef struct INICommand
{
    // offset: 0x0000
    short command;
    // offset: 0x0002
    short numParameters;
    // offset: 0x0004
    long parameter[0];
} INICommand;

// size: 0x8
typedef struct PhysObProperties
{
    // offset: 0x0000
    unsigned short version;
    // offset: 0x0002
    unsigned short family;
    // offset: 0x0004
    unsigned short ID;
    // offset: 0x0006
    unsigned short Type;
} PhysObProperties;

// size: 0x8
typedef struct PhysObSplinter
{
    // offset: 0x0000
    long numSplintData;
    // offset: 0x0004
    void *splinterData;
} PhysObSplinter;

// size: 0x8
typedef struct LightTableEntry
{
    // offset: 0x0000
    short r;
    // offset: 0x0002
    short g;
    // offset: 0x0004
    short b;
    // offset: 0x0006
    short radius;
} LightTableEntry;

// size: 0x8
typedef struct _PhysObLight
{
    // offset: 0x0000
    unsigned char length;
    // offset: 0x0001
    unsigned char segment;
    // offset: 0x0002
    short speed;
    // offset: 0x0004 (4 bytes)
    LightTableEntry *lightTable;
} PhysObLight;

// size: 0x1C
typedef struct PhysObWeaponAttributes
{
    // offset: 0x0000
    long Damage;
    // offset: 0x0004
    long AltDamage;
    // offset: 0x0008
    short knockBackDistance;
    // offset: 0x000A
    char knockBackFrames;
    // offset: 0x000B
    char dropSound;
    // offset: 0x000C
    char Class;
    // offset: 0x000D
    char ThrowSphere;
    // offset: 0x000E
    char LeftHandSphere;
    // offset: 0x000F
    char RightHandSphere;
    // offset: 0x0010 (4 bytes)
    PhysObLight *Light;
    // offset: 0x0014 (8 bytes)
    PhysObSplinter splinter;
} PhysObWeaponAttributes;

// size: 0xC
typedef struct __PhysObProjectileData
{
    // offset: 0x0000
    char model;
    // offset: 0x0001
    char startAnim;
    // offset: 0x0002
    char loopAnim;
    // offset: 0x0003
    char endAnim;
    // offset: 0x0004
    long flags;
    // offset: 0x0008 (4 bytes)
    PhysObWeaponAttributes *weapon;
} PhysObProjData;

// size: 0x4
typedef struct SwitchData
{
    // offset: 0x0000
    short state;
    // offset: 0x0002
    short accumulator;
} SwitchData;

// size: 0x4
typedef struct SavedIntroSmall
{
    // offset: 0x0000
    unsigned char savedID;
    // offset: 0x0001
    unsigned char shiftedSaveSize;
    // offset: 0x0002
    short introUniqueID;
} SavedIntroSmall;

// size: 0xC
typedef struct SavedIntroSpline
{
    // offset: 0x0000
    unsigned char savedID;
    // offset: 0x0001
    unsigned char shiftedSaveSize;
    // offset: 0x0002
    short introUniqueID;
    // offset: 0x0004
    short splineFlags;
    // offset: 0x0006
    short splineKeyFrame;
    // offset: 0x0008
    short splineClipBeg;
    // offset: 0x000A
    short splineClipEnd;
} SavedIntroSpline;

// size: 0x20
typedef struct _SavedIntroWithIntro
{
    // offset: 0x0000
    unsigned char savedID;
    // offset: 0x0001
    unsigned char shiftedSaveSize;
    // offset: 0x0002
    unsigned short attachedUniqueID;
    // offset: 0x0004
    short introOffset;
    // offset: 0x0006
    short birthUnitID;
    // offset: 0x0008
    short introUniqueID;
    // offset: 0x000A
    unsigned char lightGroup;
    // offset: 0x000B
    unsigned char specturalLightGroup;
    // offset: 0x000C
    long flags;
    // offset: 0x0010
    long flags2;
    // offset: 0x0014 (6 bytes)
    SmallRotation smallRotation;
    // offset: 0x001A (6 bytes)
    Position position;
} SavedIntroWithIntro;

// size: 0x10
typedef struct _MonsterAnim
{
    // offset: 0x0000
    unsigned short velocity;
    // offset: 0x0002
    unsigned short distance;
    // offset: 0x0004
    unsigned short playSpeed;
    // offset: 0x0006
    unsigned char interpFrames;
    // offset: 0x0007
    unsigned char startFrame;
    // offset: 0x0008
    unsigned char interpOut;
    // offset: 0x0009
    unsigned char alphaTable;
    // offset: 0x000A
    unsigned char alphaTableOut;
    // offset: 0x000B
    unsigned char controllingSection;
    // offset: 0x000C (3 bytes)
    char index[3];
    // offset: 0x000F
    char pad;
} MonsterAnim;

// size: 0x4
typedef struct _MonsterIdle
{
    // offset: 0x0000
    char anim;
    // offset: 0x0001
    char alertMod;
    // offset: 0x0002
    char probability;
    // offset: 0x0003
    char pad;
} MonsterIdle;

// size: 0x8
typedef struct _MonsterBehavior
{
    // offset: 0x0000
    char alertness;
    // offset: 0x0001
    char idleFreq;
    // offset: 0x0002
    char numIdles;
    // offset: 0x0003 (5 bytes)
    char idleList[5];
} MonsterBehavior;

// size: 0x12
typedef struct _MonsterSenses
{
    // offset: 0x0000
    short sightArc;
    // offset: 0x0002
    short sightRadius;
    // offset: 0x0004
    short sightElevation;
    // offset: 0x0006
    short soundArc;
    // offset: 0x0008
    short soundRadius;
    // offset: 0x000A
    short soundElevation;
    // offset: 0x000C
    unsigned short scentRadius;
    // offset: 0x000E
    short alarmRadius;
    // offset: 0x0010
    unsigned short allyRadius;
} MonsterSenses;

// size: 0x10
typedef struct _MonsterAllegiances
{
    // offset: 0x0000
    unsigned long enemies;
    // offset: 0x0004
    unsigned long allies;
    // offset: 0x0008
    unsigned long food;
    // offset: 0x000C
    unsigned long gods;
} MonsterAllegiances;

// size: 0x20
typedef struct _MonsterAttackAttributes
{
    // offset: 0x0000
    short attackRange;
    // offset: 0x0002
    short attackHeight;
    // offset: 0x0004
    short knockBackDistance;
    // offset: 0x0006
    char knockBackDuration;
    // offset: 0x0007
    char damage;
    // offset: 0x0008
    char sphereSegment;
    // offset: 0x0009
    char sphereOnFrame;
    // offset: 0x000A
    char sphereOnAnim;
    // offset: 0x000B
    char sphereOffFrame;
    // offset: 0x000C
    char sphereOffAnim;
    // offset: 0x000D
    char turnFrames;
    // offset: 0x000E
    char numAnims;
    // offset: 0x000F (12 bytes)
    char attackProbability[12];
    // offset: 0x001B (5 bytes)
    char animList[5];
} MonsterAttackAttributes;

// size: 0x10
typedef struct _MonsterMissile
{
    // offset: 0x0000
    unsigned short speed;
    // offset: 0x0002
    unsigned short range;
    // offset: 0x0004
    unsigned char frame;
    // offset: 0x0005
    unsigned char anim;
    // offset: 0x0006
    unsigned char segment;
    // offset: 0x0007
    unsigned char damage;
    // offset: 0x0008
    unsigned char type;
    // offset: 0x0009
    unsigned char graphic;
    // offset: 0x000A
    char gravity;
    // offset: 0x000B
    char reload;
    // offset: 0x000C
    char fireChance;
    // offset: 0x000D
    unsigned char numAnims;
    // offset: 0x000E (2 bytes)
    char animList[2];
} MonsterMissile;

// size: 0x2E
typedef struct _MonsterCombatAttributes
{
    // offset: 0x0000
    short stunTime;
    // offset: 0x0002
    short damageTime;
    // offset: 0x0004
    short recovery;
    // offset: 0x0006
    short surpriseTime;
    // offset: 0x0008
    short grabTime;
    // offset: 0x000A
    short combatRange;
    // offset: 0x000C
    short surpriseRange;
    // offset: 0x000E
    short allyRange;
    // offset: 0x0010
    short enemyAttackRange;
    // offset: 0x0012
    short enemyRunAttackRange;
    // offset: 0x0014
    short preferredCombatRange;
    // offset: 0x0016
    short suckPower;
    // offset: 0x0018
    short suckRange;
    // offset: 0x001A
    short suckTime;
    // offset: 0x001C
    char hitPoints;
    // offset: 0x001D
    char numAttacks;
    // offset: 0x001E
    char missileAttack;
    // offset: 0x001F
    char ambushAttack;
    // offset: 0x0020 (12 bytes)
    char evadeProbability[12];
    // offset: 0x002C (1 bytes)
    char attackList[1];
} MonsterCombatAttributes;

// size: 0x54
typedef struct _MonsterSubAttributes
{
    // offset: 0x0000
    char *animList;
    // offset: 0x0004 (4 bytes)
    MonsterSenses *senses;
    // offset: 0x0008 (4 bytes)
    MonsterCombatAttributes *combatAttributes;
    // offset: 0x000C (4 bytes)
    MonsterAllegiances *allegiances;
    // offset: 0x0010
    char *behaviorList;
    // offset: 0x0014
    short scale;
    // offset: 0x0016
    short fallDistance;
    // offset: 0x0018
    short defAmbushRange;
    // offset: 0x001A
    short fleeRange;
    // offset: 0x001C
    short speedPivotTurn;
    // offset: 0x001E
    short speedWalkTurn;
    // offset: 0x0020
    short speedRunTurn;
    // offset: 0x0022
    short speedFleeTurn;
    // offset: 0x0024
    short forgetTime;
    // offset: 0x0026
    unsigned char modelNum;
    // offset: 0x0027
    unsigned char physAbility;
    // offset: 0x0028
    unsigned char stunnable;
    // offset: 0x0029
    unsigned char grabable;
    // offset: 0x002A
    unsigned char numSections;
    // offset: 0x002B (3 bytes)
    unsigned char sectionEnd[3];
    // offset: 0x002E
    short defPlayerAttitude;
    // offset: 0x0030
    short defWanderRange;
    // offset: 0x0032
    short defGuardRange;
    // offset: 0x0034
    unsigned char defInitialBehavior;
    // offset: 0x0035
    unsigned char defTriggeredBehavior;
    // offset: 0x0036
    char defActiveBehavior;
    // offset: 0x0037
    unsigned char defSpectral;
    // offset: 0x0038
    unsigned char sunVuln;
    // offset: 0x0039
    unsigned char fireVuln;
    // offset: 0x003A
    unsigned char waterVuln;
    // offset: 0x003B
    unsigned char impaleVuln;
    // offset: 0x003C
    unsigned char soundVuln;
    // offset: 0x003D
    unsigned char weaponVuln;
    // offset: 0x003E
    unsigned char missileVuln;
    // offset: 0x003F
    unsigned char bruiseRed;
    // offset: 0x0040
    unsigned char bruiseGreen;
    // offset: 0x0041
    unsigned char bruiseBlue;
    // offset: 0x0042
    short minSpikeRange;
    // offset: 0x0044
    short maxSpikeRange;
    // offset: 0x0046
    short maxSpikeAngle;
    // offset: 0x0048
    short minSpikeHorzSpeed;
    // offset: 0x004A
    short maxSpikeHorzSpeed;
    // offset: 0x004C
    short maxSpikeVertSpeed;
    // offset: 0x004E
    short upOnGroundOffset;
    // offset: 0x0050
    short downOnGroundOffset;
} MonsterSubAttributes;

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

// size: 0x2C
typedef struct _PCollideInfo
{
    // offset: 0x0000
    short type;
    // offset: 0x0002
    short segment;
    // offset: 0x0004
    void *prim;
    // offset: 0x0008 (4 bytes)
    Instance *inst;
    // offset: 0x000C (8 bytes)
    SVECTOR wNormal;
    // offset: 0x0014 (4 bytes)
    SVECTOR *newPoint;
    // offset: 0x0018 (4 bytes)
    SVECTOR *oldPoint;
    // offset: 0x001C (8 bytes)
    SVECTOR cldPoint;
    // offset: 0x0024
    long collideType;
    // offset: 0x0028 (4 bytes)
    Instance *instance;
} PCollideInfo;

// size: 0x5C
typedef struct CameraCollisionInfo
{
    // offset: 0x0000 (20 bytes)
    TFace *tfaceList[5];
    // offset: 0x0014 (20 bytes)
    Terrain *tfaceTerrain[5];
    // offset: 0x0028 (12 bytes)
    short bspTree[6];
    // offset: 0x0034 (12 bytes)
    short lengthList[6];
    // offset: 0x0040
    long flags;
    // offset: 0x0044
    long line;
    // offset: 0x0048
    long numCollided;
    // offset: 0x004C
    long lenCenterToExtend;
    // offset: 0x0050 (4 bytes)
    Sphere *start;
    // offset: 0x0054 (4 bytes)
    Sphere *end;
    // offset: 0x0058
    long cldLines;
} CameraCollisionInfo;

// size: 0xC
typedef struct _LCollideInfo
{
    // offset: 0x0000 (4 bytes)
    TFace *tface;
    // offset: 0x0004 (4 bytes)
    Terrain *terrain;
    // offset: 0x0008
    int curTree;
} LCollideInfo;

// size: 0x14
typedef struct
{
    // offset: 0x0000 (20 bytes)
    long m[5];
} cmm;

// size: 0x18
typedef struct CdlFILE
{
    // offset: 0x0000 (4 bytes)
    CdlLOC pos;
    // offset: 0x0004
    unsigned long size;
    // offset: 0x0008 (16 bytes)
    char name[16];
} CdlFILE;

// size: 0xA0
typedef struct _LoadStatus
{
    // offset: 0x0000 (44 bytes)
    ReadQueueEntry currentQueueFile;
    // offset: 0x002C
    volatile long state;
    // offset: 0x0030
    long bytesTransferred;
    // offset: 0x0034
    long lastCheckPos;
    // offset: 0x0038
    long cdWaitTime;
    // offset: 0x003C
    unsigned char *compSrcAddr;
    // offset: 0x0040
    unsigned char *compDstAddr;
    // offset: 0x0044
    long compControlbits;
    // offset: 0x0048
    long compControl;
    // offset: 0x004C
    long compEndBytes;
    // offset: 0x0050
    long decompressLen;
    // offset: 0x0054
    long checksum;
    // offset: 0x0058
    long *checkAddr;
    // offset: 0x005C (36 bytes)
    BigFile bigFile;
    // offset: 0x0080
    void *buffer1;
    // offset: 0x0084
    void *buffer2;
    // offset: 0x0088
    int changeDir;
    // offset: 0x008C
    long endLoadTime;
    // offset: 0x0090
    long currentDirLoading;
    // offset: 0x0094
    long currentSector;
    // offset: 0x0098
    volatile long sectorTime;
    // offset: 0x009C
    long seekTime;
} LoadStatus;

// size: 0x8
typedef struct RedirectList
{
    // offset: 0x0000
    long numPointers;
    // offset: 0x0004
    long *data;
} RedirectList;

// size: 0x5C
typedef struct DRAWENV
{
    // offset: 0x0000 (8 bytes)
    RECT clip;
    // offset: 0x0008 (4 bytes)
    short ofs[2];
    // offset: 0x000C (8 bytes)
    RECT tw;
    // offset: 0x0014
    unsigned short tpage;
    // offset: 0x0016
    unsigned char dtd;
    // offset: 0x0017
    unsigned char dfe;
    // offset: 0x0018
    unsigned char isbg;
    // offset: 0x0019
    unsigned char r0;
    // offset: 0x001A
    unsigned char g0;
    // offset: 0x001B
    unsigned char b0;
    // offset: 0x001C (64 bytes)
    DR_ENV dr_env;
} DRAWENV;

// size: 0x14
typedef struct DISPENV
{
    // offset: 0x0000 (8 bytes)
    RECT disp;
    // offset: 0x0008 (8 bytes)
    RECT screen;
    // offset: 0x0010
    unsigned char isinter;
    // offset: 0x0011
    unsigned char isrgb24;
    // offset: 0x0012
    unsigned char pad0;
    // offset: 0x0013
    unsigned char pad1;
} DISPENV;

// size: 0x10
typedef struct BLK_FILL // this struct was modified from the original
{
    // offset: 0x0000
    union
    {
        // offset: 0x0000
        long color;
        // offset: 0x0000 (4 bytes)
        RGBAColor rgba;
    } tag; // modified field, assumed to contain union _77fake
    // offset: 0x0004
    unsigned char r0;
    // offset: 0x0005
    unsigned char g0;
    // offset: 0x0006
    unsigned char b0;
    // offset: 0x0007
    unsigned char code;
    // offset: 0x0008
    unsigned short x0;
    // offset: 0x000A
    unsigned short y0;
    // offset: 0x000C
    unsigned short w;
    // offset: 0x000E
    unsigned short h;
} BLK_FILL;

// size: 0x10
typedef struct MainTracker
{
    // offset: 0x0000
    long mainState;
    // offset: 0x0004
    long previousState;
    // offset: 0x0008
    long movieNum;
    // offset: 0x000C
    long done;
} MainTracker;

// size: 0x1C
typedef struct POLY_F4_SEMITRANS
{
    // offset: 0x0000
    unsigned long tag;
    // offset: 0x0004
    unsigned long dr_tpage;
    // offset: 0x0008
    unsigned char r0;
    // offset: 0x0009
    unsigned char g0;
    // offset: 0x000A
    unsigned char b0;
    // offset: 0x000B
    unsigned char code;
    // offset: 0x000C
    short x0;
    // offset: 0x000E
    short y0;
    // offset: 0x0010
    short x1;
    // offset: 0x0012
    short y1;
    // offset: 0x0014
    short x2;
    // offset: 0x0016
    short y2;
    // offset: 0x0018
    short x3;
    // offset: 0x001A
    short y3;
} POLY_F4_SEMITRANS;

// size: 0x38
typedef struct InterfaceItem
{
    // offset: 0x0000 (48 bytes)
    char name[48];
    // offset: 0x0030
    unsigned short timeout;
    // offset: 0x0032
    unsigned short buttonTimeout;
    // offset: 0x0034
    short itemType;
    // offset: 0x0036
    short nextItem;
} InterfaceItem;

// size: 0x6DA8
typedef struct _FXTracker
{
    // offset: 0x0000 (1408 bytes)
    FX_MATRIX matrixPool[32];
    // offset: 0x0580 (8 bytes)
    NodeType usedMatrixList;
    // offset: 0x0588 (8 bytes)
    NodeType freeMatrixList;
    // offset: 0x0590 (26624 bytes)
    FX_PRIM primPool[256];
    // offset: 0x6D90 (8 bytes)
    NodeType usedPrimList;
    // offset: 0x6D98 (8 bytes)
    NodeType usedPrimListSprite;
    // offset: 0x6DA0 (8 bytes)
    NodeType freePrimList;
} FXTracker;

// size: 0x5C
typedef struct StreamUnitPortal
{
    // offset: 0x0000 (16 bytes)
    char tolevelname[16];
    // offset: 0x0010
    long MSignalID;
    // offset: 0x0014
    long streamID;
    // offset: 0x0018
    short minx;
    // offset: 0x001A
    short miny;
    // offset: 0x001C
    short minz;
    // offset: 0x001E
    short flags;
    // offset: 0x0020
    short maxx;
    // offset: 0x0022
    short maxy;
    // offset: 0x0024
    short maxz;
    // offset: 0x0026
    short pad2;
    // offset: 0x0028 (4 bytes)
    StreamUnit *toStreamUnit;
    // offset: 0x002C (24 bytes)
    SVector t1[3];
    // offset: 0x0044 (24 bytes)
    SVector t2[3];
} StreamUnitPortal;

// size: 0xC
typedef struct SignalInfo
{
    // offset: 0x0000
    long (*signalHandleFunc)();
    // offset: 0x0004
    short length;
    // offset: 0x0006
    short onlyPlayer;
    // offset: 0x0008
    void (*signalRelocateFunc)();
} SignalInfo;

// size: 0xC
typedef struct _Triangle2D
{
    // offset: 0x0000
    short x0;
    // offset: 0x0002
    short y0;
    // offset: 0x0004
    short x1;
    // offset: 0x0006
    short y1;
    // offset: 0x0008
    short x2;
    // offset: 0x000A
    short y2;
} Triangle2D;

// size: 0x8
typedef struct _HNormal
{
    // offset: 0x0000
    short x;
    // offset: 0x0002
    short y;
    // offset: 0x0004
    short z;
    // offset: 0x0006
    short pad;
} HNormal;

// size: 0x20
typedef struct _HFaceInfo
{
    // offset: 0x0000
    long flags;
    // offset: 0x0004
    long attr;
    // offset: 0x0008 (4 bytes)
    HFace *hface;
    // offset: 0x000C (8 bytes)
    SVector normal;
    // offset: 0x0014 (4 bytes)
    HVertex *vertex0;
    // offset: 0x0018 (4 bytes)
    HVertex *vertex1;
    // offset: 0x001C (4 bytes)
    HVertex *vertex2;
} HFaceInfo;

// size: 0x30
typedef struct _CollideInfo
{
    // offset: 0x0000
    void *level;
    // offset: 0x0004
    char flags;
    // offset: 0x0005
    char segment;
    // offset: 0x0006
    char type0;
    // offset: 0x0007
    char type1;
    // offset: 0x0008
    void *prim0;
    // offset: 0x000C
    void *prim1;
    // offset: 0x0010
    void *inst0;
    // offset: 0x0014
    void *inst1;
    // offset: 0x0018 (8 bytes)
    SVECTOR point0;
    // offset: 0x0020 (8 bytes)
    SVECTOR point1;
    // offset: 0x0028 (6 bytes)
    Position offset;
    // offset: 0x002E
    short bspID;
} CollideInfo;

// size: 0x1C
typedef struct SCollideInfo
{
    // offset: 0x0000 (4 bytes)
    Sphere *sphere;
    // offset: 0x0004 (4 bytes)
    SVECTOR *oldPos;
    // offset: 0x0008
    void (*collideFunc)();
    // offset: 0x000C
    long id;
    // offset: 0x0010
    long segment;
    // offset: 0x0014 (4 bytes)
    Instance *instance;
    // offset: 0x0018
    void *prim;
} SCollideInfo;

// size: 0x7C
struct PandTScratch
{
    // offset: 0x0000 (32 bytes)
    MATRIX posMatrix;
    // offset: 0x0020 (8 bytes)
    SVector normal;
    // offset: 0x0028 (12 bytes)
    Vector dpv;
    // offset: 0x0034 (8 bytes)
    SVector planePoint;
    // offset: 0x003C
    long ignore_attr;
    // offset: 0x0040
    long accept_attr;
    // offset: 0x0044
    short *backface_flag;
    // offset: 0x0048 (8 bytes)
    SVector newPos;
    // offset: 0x0050 (8 bytes)
    SVector oldPos;
    // offset: 0x0058
    int flags;
    // offset: 0x005C
    int i;
    // offset: 0x0060
    int slack;
    // offset: 0x0064
    long in_spectral;
    // offset: 0x0068
    short *normalList;
    // offset: 0x006C (4 bytes)
    TVertex *vertexList;
    // offset: 0x0070 (4 bytes)
    TFace *result;
    // offset: 0x0074 (8 bytes)
    SVector line;
};

// size: 0xC
typedef struct
{
    // offset: 0x0000 (12 bytes)
    long m[3];
} tmm;

// size: 0x44
typedef struct SandHFScratch
{
    // offset: 0x0000 (8 bytes)
    SVector normal;
    // offset: 0x0008 (32 bytes)
    MATRIX posMatrix;
    // offset: 0x0028 (12 bytes)
    Vector dpv;
    // offset: 0x0034 (8 bytes)
    SVector triPoint;
    // offset: 0x003C (8 bytes)
    SVector planePoint;
} SandHFScratch;

// size: 0xD4
struct SandTScratch
{
    // offset: 0x0000 (32 bytes)
    MATRIX posMatrix;
    // offset: 0x0020 (12 bytes)
    Vector dpv;
    // offset: 0x002C (4 bytes)
    HNormal *normalList;
    // offset: 0x0030 (48 bytes)
    CollideInfo collideInfo;
    // offset: 0x0060 (32 bytes)
    HFaceInfo hfaceInfo;
    // offset: 0x0080 (8 bytes)
    SVector midPoint;
    // offset: 0x0088 (8 bytes)
    SVector spherePos;
    // offset: 0x0090
    void (*collideFunc)();
    // offset: 0x0094 (4 bytes)
    Instance *instance;
    // offset: 0x0098
    long edge;
    // offset: 0x009C
    long in_spectral;
    // offset: 0x00A0 (8 bytes)
    SVector oldPos;
    // offset: 0x00A8
    void *prim;
    // offset: 0x00AC (8 bytes)
    SVector normal;
    // offset: 0x00B4 (12 bytes)
    Sphere sphere;
    // offset: 0x00C0
    short result;
    // offset: 0x00C2
    short i;
    // offset: 0x00C4
    long collide_ignoreAttr;
    // offset: 0x00C8
    long collide_acceptAttr;
    // offset: 0x00CC
    long midRadius;
    // offset: 0x00D0 (4 bytes)
    TVertex *vertexList;
};

// size: 0x64
struct IandTScratch
{
    // offset: 0x0000 (32 bytes)
    MATRIX posMatrix;
    // offset: 0x0020 (8 bytes)
    SVector normal;
    // offset: 0x0028 (12 bytes)
    Vector dpv;
    // offset: 0x0034 (8 bytes)
    SVector planePoint;
    // offset: 0x003C (8 bytes)
    SVector line;
    // offset: 0x0044 (8 bytes)
    SVector oldPos;
    // offset: 0x004C (8 bytes)
    SVector newPos;
    // offset: 0x0054
    int i;
    // offset: 0x0058
    long in_spectral;
    // offset: 0x005C
    short *normalList;
    // offset: 0x0060 (4 bytes)
    TVertex *vertexList;
};

// size: 0x4C
struct PandTFScratch
{
    // offset: 0x0000 (32 bytes)
    MATRIX posMatrix;
    // offset: 0x0020 (8 bytes)
    SVector planePoint;
    // offset: 0x0028 (8 bytes)
    SVector newPos;
    // offset: 0x0030 (8 bytes)
    SVector oldPos;
    // offset: 0x0038 (8 bytes)
    SVector normal;
    // offset: 0x0040 (12 bytes)
    Vector dpv;
};

// size: 0x18
typedef struct _SBSPLeaf
{
    // offset: 0x0000 (8 bytes)
    Sphere_noSq sphere;
    // offset: 0x0008 (4 bytes)
    Intro **introList;
    // offset: 0x000C
    short numIntros;
    // offset: 0x000E
    short flags;
    // offset: 0x0010 (4 bytes)
    CDLight **lightList;
    // offset: 0x0014
    short numLights;
    // offset: 0x0016
    short pad;
} SBSPLeaf;

// size: 0xCC
struct BSPScratch
{
    // offset: 0x0000 (64 bytes)
    MATRIX vv[2];
    // offset: 0x0040 (32 bytes)
    MATRIX camMatrix;
    // offset: 0x0060 (48 bytes)
    VECTOR dpv[3];
    // offset: 0x0090 (24 bytes)
    long d[6];
    // offset: 0x00A8 (32 bytes)
    MATRIX boundMatrix;
    // offset: 0x00C8
    long in_spectral;
};

// size: 0x8
typedef struct _ObjectOwnerInfo
{
    // offset: 0x0000
    long magicnum;
    // offset: 0x0004
    short animID;
    // offset: 0x0006
    char objectName[0];
} ObjectOwnerInfo;

// size: 0x8
typedef struct ObjectAccess
{
    // offset: 0x0000
    char *objectName;
    // offset: 0x0004
    void *object;
} ObjectAccess;

// size: 0x20
typedef struct POLY_FT3
{
    // offset: 0x0000
    unsigned long tag;
    // offset: 0x0004
    unsigned char r0;
    // offset: 0x0005
    unsigned char g0;
    // offset: 0x0006
    unsigned char b0;
    // offset: 0x0007
    unsigned char code;
    // offset: 0x0008
    short x0;
    // offset: 0x000A
    short y0;
    // offset: 0x000C
    unsigned char u0;
    // offset: 0x000D
    unsigned char v0;
    // offset: 0x000E
    unsigned short clut;
    // offset: 0x0010
    short x1;
    // offset: 0x0012
    short y1;
    // offset: 0x0014
    unsigned char u1;
    // offset: 0x0015
    unsigned char v1;
    // offset: 0x0016
    unsigned short tpage;
    // offset: 0x0018
    short x2;
    // offset: 0x001A
    short y2;
    // offset: 0x001C
    unsigned char u2;
    // offset: 0x001D
    unsigned char v2;
    // offset: 0x001E
    unsigned short pad1;
} POLY_FT3;

// size: 0x8
typedef struct _Mirror
{
    // offset: 0x0000
    short a;
    // offset: 0x0002
    short b;
    // offset: 0x0004
    short c;
    // offset: 0x0006
    short d;
} Mirror;

// size: 0x2C
typedef struct _FXHalvePlane
{
    // offset: 0x0000
    void *next;
    // offset: 0x0004
    void *continue_process;
    // offset: 0x0008 (4 bytes)
    Instance *instance;
    // offset: 0x000C
    unsigned char effectType;
    // offset: 0x000D
    unsigned char type;
    // offset: 0x000E
    short lifeTime;
    // offset: 0x0010
    long diffTime;
    // offset: 0x0014 (12 bytes)
    PlaneConstants ringPlane;
    // offset: 0x0020
    long currentColor;
    // offset: 0x0024
    long *colorArray;
    // offset: 0x0028
    short numColors;
    // offset: 0x002A
    short colorBlendLife;
} FXHalvePlane;

// size: 0x2C
typedef struct _FXGlowEffect
{
    // offset: 0x0000
    void *next;
    // offset: 0x0004
    void *continue_process;
    // offset: 0x0008 (4 bytes)
    Instance *instance;
    // offset: 0x000C
    unsigned char effectType;
    // offset: 0x000D
    unsigned char type;
    // offset: 0x000E
    short lifeTime;
    // offset: 0x0010
    unsigned long diffTime;
    // offset: 0x0014
    long *colorArray;
    // offset: 0x0018
    long currentColor;
    // offset: 0x001C
    short numColors;
    // offset: 0x001E
    short colorBlendCycle;
    // offset: 0x0020
    short width;
    // offset: 0x0022
    short height;
    // offset: 0x0024
    unsigned char numSegments;
    // offset: 0x0025
    unsigned char SegmentInc;
    // offset: 0x0026
    short segment;
    // offset: 0x0028
    short fadein_time;
    // offset: 0x002A
    short fadeout_time;
} FXGlowEffect;

// size: 0x6C
typedef struct TransformAnimatedInstanceVerticesWork_t
{
    // offset: 0x0000 (32 bytes)
    Transform scTransform;
    // offset: 0x0020 (32 bytes)
    MATRIX mirrorMatrix;
    // offset: 0x0040 (32 bytes)
    MATRIX llm;
    // offset: 0x0060 (12 bytes)
    Vector vector;
} TransformAnimatedInstanceVerticesWork_t;

// size: 0x8
typedef struct GenericTune
{
    // offset: 0x0000
    long flags;
    // offset: 0x0004
    void *shatterData;
} GenericTune;

// size: 0x14
typedef struct FXSplinter
{
    // offset: 0x0000
    short flags;
    // offset: 0x0002
    short soundFx;
    // offset: 0x0004
    short chunkVelXY;
    // offset: 0x0006
    short chunkVelZ;
    // offset: 0x0008
    short chunkVelRng;
    // offset: 0x000A
    short triVelRng;
    // offset: 0x000C
    short lifetime;
    // offset: 0x000E
    short faceLimit;
    // offset: 0x0010
    short rotRateRng;
    // offset: 0x0012
    short gravityZ;
} FXSplinter;

// size: 0x8
typedef struct DR_TPAGE
{
    // offset: 0x0000
    unsigned long tag;
    // offset: 0x0004 (4 bytes)
    unsigned long code[1];
} DR_TPAGE;

// size: 0x40
typedef struct _POLY_SG4
{
    // offset: 0x0000
    unsigned long tag;
    // offset: 0x0004
    unsigned long drawTPage1;
    // offset: 0x0008 (32 bytes)
    BASIC_G4 p1;
    // offset: 0x0028
    unsigned long drawTPage2;
    // offset: 0x002C (20 bytes)
    BASIC_F4 p2;
} POLY_SG4;

// size: 0x10
typedef struct _FXGeneralEffect
{
    // offset: 0x0000
    void *next;
    // offset: 0x0004
    // void *continue_process;
    void (*continue_process)();
    // offset: 0x0008 (4 bytes)
    Instance *instance;
    // offset: 0x000C
    unsigned char effectType;
    // offset: 0x000D
    unsigned char type;
    // offset: 0x000E
    short lifeTime;
} FXGeneralEffect;

// size: 0x2C
typedef struct _FXRibbon
{
    // offset: 0x0000
    void *next;
    // offset: 0x0004
    void *continue_process;
    // offset: 0x0008 (4 bytes)
    Instance *instance;
    // offset: 0x000C
    unsigned char effectType;
    // offset: 0x000D
    unsigned char type;
    // offset: 0x000E
    short lifeTime;
    // offset: 0x0010
    short numberVerts;
    // offset: 0x0012
    short endIndex;
    // offset: 0x0014 (4 bytes)
    SVECTOR *vertexPool;
    // offset: 0x0018
    short startSegment;
    // offset: 0x001A
    short endSegment;
    // offset: 0x001C
    long startColor;
    // offset: 0x0020
    long endColor;
    // offset: 0x0024
    short colorStepValue;
    // offset: 0x0026
    short faceLifeTime;
    // offset: 0x0028
    short startFadeValue;
    // offset: 0x002A
    short fadeStep;
} FXRibbon;

// size: 0x4C
typedef struct _FXParticle
{
    // offset: 0x0000
    void *next;
    // offset: 0x0004
    void *continue_process;
    // offset: 0x0008 (4 bytes)
    Instance *instance;
    // offset: 0x000C
    unsigned char effectType;
    // offset: 0x000D
    unsigned char type;
    // offset: 0x000E
    short lifeTime;
    // offset: 0x0010 (4 bytes)
    TextureMT3 *texture;
    // offset: 0x0014
    void *fxprim_modify_process;
    // offset: 0x0018
    void *fxprim_process;
    // offset: 0x001C
    short size;
    // offset: 0x001E
    short primLifeTime;
    // offset: 0x0020
    short birthRadius;
    // offset: 0x0022
    short scaleSpeed;
    // offset: 0x0024 (6 bytes)
    Position direction;
    // offset: 0x002A
    unsigned short flag_bits;
    // offset: 0x002C (6 bytes)
    Position acceleration;
    // offset: 0x0032
    short startScale;
    // offset: 0x0034 (6 bytes)
    Position offset;
    // offset: 0x003A
    char startSegment;
    // offset: 0x003B
    char z_undulate;
    // offset: 0x003C
    long startColor;
    // offset: 0x0040
    long endColor;
    // offset: 0x0044
    short colorStepValue;
    // offset: 0x0046
    char numberBirthParticles;
    // offset: 0x0047
    char endSegment;
    // offset: 0x0048
    short startFadeValue;
    // offset: 0x004A
    short fadeStep;
} FXParticle;

// size: 0x18
typedef struct _FXLightBeam
{
    // offset: 0x0000
    void *next;
    // offset: 0x0004
    void *continue_process;
    // offset: 0x0008 (4 bytes)
    Instance *instance;
    // offset: 0x000C
    unsigned char effectType;
    // offset: 0x000D
    unsigned char type;
    // offset: 0x000E
    short lifeTime;
    // offset: 0x0010
    short startSeg;
    // offset: 0x0012
    short endSeg;
    // offset: 0x0014
    long color;
} FXLightBeam;

// size: 0x78
typedef struct _FXBlastringEffect
{
    // offset: 0x0000
    void *next;
    // offset: 0x0004
    void *continue_process;
    // offset: 0x0008 (4 bytes)
    Instance *instance;
    // offset: 0x000C
    unsigned char effectType;
    // offset: 0x000D
    unsigned char type;
    // offset: 0x000E
    short lifeTime;
    // offset: 0x0010 (8 bytes)
    SVector position;
    // offset: 0x0018 (32 bytes)
    MATRIX matrix;
    // offset: 0x0038
    short segment;
    // offset: 0x003A
    short pad;
    // offset: 0x003C
    int radius;
    // offset: 0x0040
    int size1;
    // offset: 0x0044
    int size2;
    // offset: 0x0048
    int startRadius;
    // offset: 0x004C
    int endRadius;
    // offset: 0x0050
    int colorchange_radius;
    // offset: 0x0054
    int vel;
    // offset: 0x0058
    int accl;
    // offset: 0x005C
    int height1;
    // offset: 0x0060
    int height2;
    // offset: 0x0064
    int height3;
    // offset: 0x0068
    char predator_offset;
    // offset: 0x0069
    char predator_offset_orig;
    // offset: 0x006A
    char stay_in_place;
    // offset: 0x006B
    char sortInWorld;
    // offset: 0x006C
    long color;
    // offset: 0x0070
    long startColor;
    // offset: 0x0074
    long endColor;
} FXBlastringEffect;

// size: 0x2C
typedef struct _FXForceFieldEffect
{
    // offset: 0x0000
    void *next;
    // offset: 0x0004
    void *continue_process;
    // offset: 0x0008 (4 bytes)
    Instance *instance;
    // offset: 0x000C
    unsigned char effectType;
    // offset: 0x000D
    unsigned char type;
    // offset: 0x000E
    short lifeTime;
    // offset: 0x0010 (8 bytes)
    SVector offset;
    // offset: 0x0018
    short size;
    // offset: 0x001A
    short size_diff;
    // offset: 0x001C
    short size_change;
    // offset: 0x001E
    short deg;
    // offset: 0x0020
    short deg_change;
    // offset: 0x0022
    short start_fade;
    // offset: 0x0024
    short end_fade;
    // offset: 0x0028
    long color;
} FXForceFieldEffect;

// size: 0x3C
typedef struct _FXLightning
{
    // offset: 0x0000
    void *next;
    // offset: 0x0004
    void *continue_process;
    // offset: 0x0008 (4 bytes)
    Instance *instance;
    // offset: 0x000C
    unsigned char effectType;
    // offset: 0x000D
    unsigned char type;
    // offset: 0x000E
    short lifeTime;
    // offset: 0x0010
    short deg;
    // offset: 0x0012
    short deg_inc;
    // offset: 0x0014 (6 bytes)
    Position start_offset;
    // offset: 0x001A
    short startSeg;
    // offset: 0x001C (4 bytes)
    Instance *end_instance;
    // offset: 0x0020 (6 bytes)
    Position end_offset;
    // offset: 0x0026
    short endSeg;
    // offset: 0x0028
    short matrixSeg;
    // offset: 0x002A
    short width;
    // offset: 0x002C
    short small_width;
    // offset: 0x002E
    short segs;
    // offset: 0x0030
    short sine_size;
    // offset: 0x0032
    short variation;
    // offset: 0x0034
    long color;
    // offset: 0x0038
    long glow_color;
} FXLightning;

// size: 0x24
typedef struct _FXFlash
{
    // offset: 0x0000
    void *next;
    // offset: 0x0004
    void *continue_process;
    // offset: 0x0008 (4 bytes)
    Instance *instance;
    // offset: 0x000C
    unsigned char effectType;
    // offset: 0x000D
    unsigned char type;
    // offset: 0x000E
    short lifeTime;
    // offset: 0x0010
    long timeToColor;
    // offset: 0x0014
    long timeAtColor;
    // offset: 0x0018
    long timeFromColor;
    // offset: 0x001C
    long currentTime;
    // offset: 0x0020
    long color;
} FXFlash;

// size: 0x1C
typedef struct GenericFXObject
{
    // offset: 0x0000 (4 bytes)
    GenericParticleParams *ParticleList;
    // offset: 0x0004 (4 bytes)
    GenericRibbonParams *RibbonList;
    // offset: 0x0008 (4 bytes)
    GenericGlowParams *GlowList;
    // offset: 0x000C (4 bytes)
    GenericLightningParams *LightningList;
    // offset: 0x0010 (4 bytes)
    GenericBlastringParams *BlastList;
    // offset: 0x0014 (4 bytes)
    GenericFlashParams *FlashList;
    // offset: 0x0018
    long *ColorList;
} GenericFXObject;

// size: 0xA
typedef struct evObjectDraftData
{
    // offset: 0x0000
    short force;
    // offset: 0x0002
    unsigned short radius;
    // offset: 0x0004
    unsigned short radiusCoef;
    // offset: 0x0006
    unsigned short height;
    // offset: 0x0008
    unsigned short maxVelocity;
} evObjectDraftData;

// size: 0x8C
typedef struct _GlyphTuneData
{
    // offset: 0x0000
    short glyph_size;
    // offset: 0x0002
    short glyph_darkness;
    // offset: 0x0004 (8 bytes)
    char glyph_costs[8];
    // offset: 0x000C (16 bytes)
    short glyph_range[8];
    // offset: 0x001C (112 bytes)
    long color_array[28];
} GlyphTuneData;

// size: 0x24
typedef struct __ReaverData
{
    // offset: 0x0000
    char ReaverPickedUp;
    // offset: 0x0001
    char ReaverOn;
    // offset: 0x0002
    short ReaverSize;
    // offset: 0x0004
    short CurrentReaver;
    // offset: 0x0006
    short ReaverDeg;
    // offset: 0x0008
    long ReaverChargeTime;
    // offset: 0x000C
    long ReaverShockAmount;
    // offset: 0x0010
    long ReaverGlowColor;
    // offset: 0x0014
    long ReaverBladeColor;
    // offset: 0x0018
    long ReaverBladeGlowColor;
    // offset: 0x001C
    short ReaverScale;
    // offset: 0x001E
    short ReaverTargetScale;
    // offset: 0x0020 (4 bytes)
    ReaverSaveData saveData;
} ReaverData;

// size: 0xC0
typedef struct __ReaverTuneData
{
    // offset: 0x0000
    unsigned long spectralGlowColor;
    // offset: 0x0004
    unsigned long materialGlowColor;
    // offset: 0x0008
    unsigned long sunlightGlowColor;
    // offset: 0x000C
    unsigned long waterGlowColor;
    // offset: 0x0010
    unsigned long stoneGlowColor;
    // offset: 0x0014
    unsigned long fireGlowColor;
    // offset: 0x0018
    unsigned long spiritGlowColor;
    // offset: 0x001C
    unsigned long soundGlowColor;
    // offset: 0x0020
    unsigned long spectralInnerColor;
    // offset: 0x0024
    unsigned long materialInnerColor;
    // offset: 0x0028
    unsigned long sunlightInnerColor;
    // offset: 0x002C
    unsigned long waterInnerColor;
    // offset: 0x0030
    unsigned long stoneInnerColor;
    // offset: 0x0034
    unsigned long fireInnerColor;
    // offset: 0x0038
    unsigned long spiritInnerColor;
    // offset: 0x003C
    unsigned long soundInnerColor;
    // offset: 0x0040
    unsigned long spectralInnerGlowColor;
    // offset: 0x0044
    unsigned long materialInnerGlowColor;
    // offset: 0x0048
    unsigned long sunlightInnerGlowColor;
    // offset: 0x004C
    unsigned long waterInnerGlowColor;
    // offset: 0x0050
    unsigned long stoneInnerGlowColor;
    // offset: 0x0054
    unsigned long fireInnerGlowColor;
    // offset: 0x0058
    unsigned long spiritInnerGlowColor;
    // offset: 0x005C
    unsigned long soundInnerGlowColor;
    // offset: 0x0060 (96 bytes)
    unsigned long icon_colors[24];
} ReaverTuneData;

// size: 0x1C
typedef struct _POLY_TF4
{
    // offset: 0x0000
    unsigned long tag;
    // offset: 0x0004
    unsigned long drawTPage;
    // offset: 0x0008 (20 bytes)
    BASIC_F4 p1;
} POLY_TF4;

// size: 0x44
typedef struct _POLY_2G4
{
    // offset: 0x0000
    unsigned long tag;
    // offset: 0x0004 (32 bytes)
    BASIC_G4 p1;
    // offset: 0x0024 (32 bytes)
    BASIC_G4 p2;
} POLY_2G4;

// size: 0x48
typedef struct _POLY_2G4T
{
    // offset: 0x0000
    unsigned long tag;
    // offset: 0x0004
    unsigned long drawTPage1;
    // offset: 0x0008 (32 bytes)
    BASIC_G4 p1;
    // offset: 0x0028 (32 bytes)
    BASIC_G4 p2;
} POLY_2G4T;

// size: 0x24
typedef struct ObjectFunc
{
    // offset: 0x0000
    char *scriptName;
    // offset: 0x0004
    void (*initFunc)();
    // offset: 0x0008
    void (*processFunc)();
    // offset: 0x000C
    void (*collideFunc)();
    // offset: 0x0010
    unsigned long (*queryFunc)();
    // offset: 0x0014
    void (*messageFunc)();
    // offset: 0x0018
    void (*additionalCollideFunc)();
    // offset: 0x001C
    void (*relocateTuneFunc)();
    // offset: 0x0020
    void (*relocateInstObFunc)();
} ObjectFunc;

// size: 0x14
typedef struct evAnimationInstanceSwitchData
{
    // offset: 0x0000 (4 bytes)
    Instance *instance;
    // offset: 0x0004
    int anim;
    // offset: 0x0008
    int frame;
    // offset: 0x000C
    int frames;
    // offset: 0x0010
    int mode;
} evAnimationInstanceSwitchData;

// size: 0x8
typedef struct evControlSaveDataData
{
    // offset: 0x0000
    long length;
    // offset: 0x0004
    void *data;
} evControlSaveDataData;

// size: 0x18
typedef struct AadInitAttr
{
    // offset: 0x0000
    int updateMode;
    // offset: 0x0004
    int numSlots;
    // offset: 0x0008
    void (*nonBlockLoadProc)();
    // offset: 0x000C
    void (*nonBlockBufferedLoadProc)();
    // offset: 0x0010
    void *(*memoryMallocProc)();
    // offset: 0x0014
    void (*memoryFreeProc)();
} AadInitAttr;

// size: 0x4C
typedef struct MusicLoadInfo
{
    // offset: 0x0000
    int state;
    // offset: 0x0004
    int nextState;
    // offset: 0x0008
    int errorStatus;
    // offset: 0x000C
    int bankLoaded;
    // offset: 0x0010
    int checkMusicDelay;
    // offset: 0x0014
    int currentMusicPlane;
    // offset: 0x0018 (8 bytes)
    char currentMusicName[8];
    // offset: 0x0020 (32 bytes)
    MusicLoadCmd commandQueue[4];
    // offset: 0x0040
    int commandIn;
    // offset: 0x0044
    int commandOut;
    // offset: 0x0048
    int numCmdsInQueue;
} MusicLoadInfo;

// size: 0x12
typedef struct ObjectPeriodicSound
{
    // offset: 0x0000
    unsigned char type;
    // offset: 0x0001
    unsigned char numSfxIDs;
    // offset: 0x0002
    unsigned char soundInst;
    // offset: 0x0003
    unsigned char flags;
    // offset: 0x0004
    unsigned short minVolDistance;
    // offset: 0x0006
    short pitch;
    // offset: 0x0008
    unsigned short pitchVariation;
    // offset: 0x000A
    unsigned char maxVolume;
    // offset: 0x000B
    unsigned char maxVolVariation;
    // offset: 0x000C
    unsigned char initialDelay;
    // offset: 0x000D
    unsigned char initialDelayVariation;
    // offset: 0x000E
    unsigned char onTime;
    // offset: 0x000F
    unsigned char onTimeVariation;
    // offset: 0x0010
    unsigned char offTime;
    // offset: 0x0011
    unsigned char offTimeVariation;
} ObjectPeriodicSound;

// size: 0xE
typedef struct ObjectEventSound
{
    // offset: 0x0000
    unsigned char type;
    // offset: 0x0001
    unsigned char numSfxIDs;
    // offset: 0x0002
    unsigned char soundInst;
    // offset: 0x0003
    unsigned char flags;
    // offset: 0x0004
    unsigned short minVolDistance;
    // offset: 0x0006
    short pitch;
    // offset: 0x0008
    unsigned short pitchVariation;
    // offset: 0x000A
    unsigned char maxVolume;
    // offset: 0x000B
    unsigned char maxVolVariation;
    // offset: 0x000C
    unsigned char initialDelay;
    // offset: 0x000D
    unsigned char initialDelayVariation;
} ObjectEventSound;

// size: 0xE
typedef struct ObjectOneShotSound
{
    // offset: 0x0000
    unsigned char type;
    // offset: 0x0001
    unsigned char numSfxIDs;
    // offset: 0x0002
    unsigned char soundInst;
    // offset: 0x0003
    unsigned char flags;
    // offset: 0x0004
    unsigned short minVolDistance;
    // offset: 0x0006
    short pitch;
    // offset: 0x0008
    unsigned short pitchVariation;
    // offset: 0x000A
    unsigned char maxVolume;
    // offset: 0x000B
    unsigned char maxVolVariation;
    // offset: 0x000C
    unsigned char initialDelay;
    // offset: 0x000D
    unsigned char initialDelayVariation;
} ObjectOneShotSound;

// size: 0x24
typedef struct SoundEffectChannel
{
    // offset: 0x0000
    unsigned char inUse;
    // offset: 0x0001
    unsigned char unused;
    // offset: 0x0002
    unsigned short unused1;
    // offset: 0x0004
    unsigned long handle;
    // offset: 0x0008
    short volume;
    // offset: 0x000A
    short volumeChangeTime;
    // offset: 0x000C
    short volumeChangeTimeSave;
    // offset: 0x000E
    short volumeChangePerUpdate;
    // offset: 0x0010
    short volumeChangeSign;
    // offset: 0x0012
    short volumeChangeErrPerUpdate;
    // offset: 0x0014
    short volumeChangeError;
    // offset: 0x0016
    short pitch;
    // offset: 0x0018
    short pitchChangeTime;
    // offset: 0x001A
    short pitchChangeTimeSave;
    // offset: 0x001C
    short pitchChangePerUpdate;
    // offset: 0x001E
    short pitchChangeSign;
    // offset: 0x0020
    short pitchChangeErrPerUpdate;
    // offset: 0x0022
    short pitchChangeError;
} SoundEffectChannel;

// size: 0x10
typedef struct AadSequenceHdr
{
    // offset: 0x0000
    unsigned long sequenceID;
    // offset: 0x0004
    unsigned long quarterNoteTime;
    // offset: 0x0008
    unsigned short ppqn;
    // offset: 0x000A
    unsigned short version;
    // offset: 0x000C
    unsigned short numTracks;
    // offset: 0x000E
    unsigned short unused;
} AadSequenceHdr;

// size: 0x8
typedef struct AadTempo
{
    // offset: 0x0000
    unsigned long quarterNoteTime;
    // offset: 0x0004
    int ppqn;
} AadTempo;

// size: 0x4
typedef struct AadVolume
{
    // offset: 0x0000
    short left;
    // offset: 0x0002
    short right;
} AadVolume;

// size: 0x14
typedef struct NewMemTracker
{
    // offset: 0x0000 (4 bytes)
    MemHeader *rootNode;
    // offset: 0x0004
    unsigned long totalMemory;
    // offset: 0x0008
    unsigned long currentMemoryUsed;
    // offset: 0x000C
    char *lastMemoryAddress;
    // offset: 0x0010
    long doingGarbageCollection;
} NewMemTracker;

// size: 0x1C88
typedef struct AadMemoryStruct
{
    // offset: 0x0000
    unsigned long updateCounter;
    // offset: 0x0004
    int numSlots;
    // offset: 0x0008
    int updateMode;
    // offset: 0x000C
    int masterVolume;
    // offset: 0x0010 (12 bytes)
    AadVolFader masterVolFader;
    // offset: 0x001C
    int sfxMasterVol;
    // offset: 0x0020
    int musicMasterVol;
    // offset: 0x0024
    int flags;
    // offset: 0x0028 (12 bytes)
    AadVolFader musicMasterVolFader;
    // offset: 0x0034 (32 bytes)
    AadSequenceSlot *sequenceSlots[8];
    // offset: 0x0054 (392 bytes)
    AadSfxSlot sfxSlot;
    // offset: 0x01DC (672 bytes)
    AadSynthVoice synthVoice[24];
    // offset: 0x047C (24 bytes)
    char voiceStatus[24];
    // offset: 0x0494 (48 bytes)
    unsigned short voicePitchSave[24];
    // offset: 0x04C4
    unsigned long voiceKeyOffRequest;
    // offset: 0x04C8
    unsigned long voiceKeyOnRequest;
    // offset: 0x04CC
    unsigned long voiceReverbRequest;
    // offset: 0x04D0 (8 bytes)
    AadSoundBankHdr *dynamicSoundBankHdr[2];
    // offset: 0x04D8 (8 bytes)
    AadProgramAtr *dynamicProgramAtr[2];
    // offset: 0x04E0 (8 bytes)
    AadToneAtr *dynamicToneAtr[2];
    // offset: 0x04E8 (8 bytes)
    unsigned long *dynamicWaveAddr[2];
    // offset: 0x04F0 (8 bytes)
    unsigned char **dynamicSequenceAddressTbl[2];
    // offset: 0x04F8 (8 bytes)
    unsigned long *dynamicSequenceLabelOffsetTbl[2];
    // offset: 0x0500 (8 bytes)
    int dynamicBankStatus[2];
    // offset: 0x0508 (8 bytes)
    unsigned char *dynamicSoundBankData[2];
    // offset: 0x0510 (8 bytes)
    unsigned long dynamicSoundBankSramData[2];
    // offset: 0x0518 (448 bytes)
    AadDynamicLoadRequest loadRequestQueue[16];
    // offset: 0x06D8 (88 bytes)
    AadDynamicBankLoadInfo dynamicBankLoadInfo;
    // offset: 0x0730
    void (*nonBlockLoadProc)();
    // offset: 0x0734
    void (*nonBlockBufferedLoadProc)();
    // offset: 0x0738
    void *(*memoryMallocProc)();
    // offset: 0x073C
    void (*memoryFreeProc)();
    // offset: 0x0740
    int nextLoadReqIn;
    // offset: 0x0744
    int nextLoadReqOut;
    // offset: 0x0748
    int numLoadReqsQueued;
    // offset: 0x074C (148 bytes)
    AadDynamicSfxLoadInfo dynamicSfxLoadInfo;
    // offset: 0x07E0 (4 bytes)
    AadDynSfxSnfFileHdr *firstDynSfxFile;
    // offset: 0x07E4
    int nextFileHandle;
    // offset: 0x07E8 (32 bytes)
    AadSramDefragInfo sramDefragInfo;
    // offset: 0x0808 (512 bytes)
    unsigned char sfxToneMasterList[512];
    // offset: 0x0A08 (512 bytes)
    unsigned char sfxWaveMasterList[512];
    // offset: 0x0C08 (2560 bytes)
    AadLoadedSfxToneAttr sfxToneAttrTbl[128];
    // offset: 0x1608 (480 bytes)
    AadLoadedSfxWaveAttr sfxWaveAttrTbl[120];
    // offset: 0x17E8
    int nextToneIndex;
    // offset: 0x17EC
    int nextWaveIndex;
    // offset: 0x17F0 (1024 bytes)
    AadNewSramBlockDesc sramDescriptorTbl[128];
    // offset: 0x1BF0
    int nextSramDescIndex;
    // offset: 0x1BF4
    int firstSramBlockDescIndex;
    // offset: 0x1BF8
    void (*controller11Callback)();
    // offset: 0x1BFC
    void (*endSequenceCallback)();
    // offset: 0x1C00
    long controller11CallbackData;
    // offset: 0x1C04
    long endSequenceCallbackData;
    // offset: 0x1C08 (128 bytes)
    unsigned char userVariables[128];
} AadMemoryStruct;

// size: 0x58
typedef struct AadSlotAttributes
{
    // offset: 0x0000
    int sequenceNumberAssigned;
    // offset: 0x0004
    int sequenceAssignedDynamicBank;
    // offset: 0x0008
    int playingStatus;
    // offset: 0x000C
    int channelMute;
    // offset: 0x0010
    int slotVolume;
    // offset: 0x0014
    int slotPan;
    // offset: 0x0018 (64 bytes)
    int eventsInQueue[16];
} AadSlotAttributes;

// size: 0x24
typedef struct POLY_G4
{
    // offset: 0x0000
    unsigned long tag;
    // offset: 0x0004
    unsigned char r0;
    // offset: 0x0005
    unsigned char g0;
    // offset: 0x0006
    unsigned char b0;
    // offset: 0x0007
    unsigned char code;
    // offset: 0x0008
    short x0;
    // offset: 0x000A
    short y0;
    // offset: 0x000C
    unsigned char r1;
    // offset: 0x000D
    unsigned char g1;
    // offset: 0x000E
    unsigned char b1;
    // offset: 0x000F
    unsigned char pad1;
    // offset: 0x0010
    short x1;
    // offset: 0x0012
    short y1;
    // offset: 0x0014
    unsigned char r2;
    // offset: 0x0015
    unsigned char g2;
    // offset: 0x0016
    unsigned char b2;
    // offset: 0x0017
    unsigned char pad2;
    // offset: 0x0018
    short x2;
    // offset: 0x001A
    short y2;
    // offset: 0x001C
    unsigned char r3;
    // offset: 0x001D
    unsigned char g3;
    // offset: 0x001E
    unsigned char b3;
    // offset: 0x001F
    unsigned char pad3;
    // offset: 0x0020
    short x3;
    // offset: 0x0022
    short y3;
} POLY_G4;

// size: 0xC
typedef struct DR_AREA
{
    // offset: 0x0000
    unsigned long tag;
    // offset: 0x0004 (8 bytes)
    unsigned long code[2];
} DR_AREA;

// size: 0x8
typedef struct evPositionData
{
    // offset: 0x0000
    short x;
    // offset: 0x0002
    short y;
    // offset: 0x0004
    short z;
    // offset: 0x0006
    short pad;
} evPositionData;

// size: 0x400
typedef struct STracker
{
    // offset: 0x0000 (1024 bytes)
    StreamUnit StreamList[16];
} STracker;

// size: 0x18
typedef struct FaceExtents
{
    // offset: 0x0000
    long smallx;
    // offset: 0x0004
    long bigx;
    // offset: 0x0008
    long smally;
    // offset: 0x000C
    long bigy;
    // offset: 0x0010
    long smallz;
    // offset: 0x0014
    long bigz;
} FaceExtents;

// size: 0x14
typedef struct WarpRoom
{
    // offset: 0x0000 (16 bytes)
    char name[16];
    // offset: 0x0010 (4 bytes)
    StreamUnit *streamUnit;
} WarpRoom;

// size: 0x1C
typedef struct PlanningNode
{
    // offset: 0x0000 (6 bytes)
    Position pos;
    // offset: 0x0006
    unsigned short flags;
    // offset: 0x0008
    unsigned long connectionStatus;
    // offset: 0x000C
    unsigned long connections;
    // offset: 0x0010
    unsigned short cost;
    // offset: 0x0012
    unsigned short parent;
    // offset: 0x0014
    unsigned short nodeType;
    // offset: 0x0016
    unsigned short id;
    // offset: 0x0018
    long streamUnitID;
} PlanningNode;

// size: 0x14
typedef struct VramBuffer
{
    // offset: 0x0000
    long flags;
    // offset: 0x0004
    short x;
    // offset: 0x0006
    short y;
    // offset: 0x0008
    short w;
    // offset: 0x000A
    short h;
    // offset: 0x000C
    short yOffset;
    // offset: 0x000E
    short lengthOfLeftOverData;
    // offset: 0x0010
    short *lineOverFlow;
} VramBuffer;

// size: 0x14
typedef struct WarpGateLoadInformation
{
    // offset: 0x0000
    int warpgate_in_use;
    // offset: 0x0004
    char loading;
    // offset: 0x0005
    char blocked;
    // offset: 0x0006
    short fadeValue;
    // offset: 0x0008
    long curTime;
    // offset: 0x000C
    long maxTime;
    // offset: 0x0010 (4 bytes)
    Instance *warpFaceInstance;
} WarpGateLoadInformation;

// size: 0x10
typedef struct evObjectData
{
    // offset: 0x0000
    short x;
    // offset: 0x0002
    short y;
    // offset: 0x0004
    short PathNumber;
    // offset: 0x0006
    short LinkNode;
    // offset: 0x0008 (4 bytes)
    Instance *Force;
    // offset: 0x000C
    int rc;
} evObjectData;

// size: 0xC
typedef struct evPhysicsGravityData
{
    // offset: 0x0000
    short UpperOffset;
    // offset: 0x0002
    short LowerOffset;
    // offset: 0x0004
    short x;
    // offset: 0x0006
    short y;
    // offset: 0x0008
    short z;
    // offset: 0x000A
    short slipSlope;
} evPhysicsGravityData;

// size: 0x28
typedef struct evPhysicsSlideData
{
    // offset: 0x0000 (8 bytes)
    SVector ForwardVector;
    // offset: 0x0008 (8 bytes)
    SVector ForwardNormal;
    // offset: 0x0010 (8 bytes)
    SVector DropNormal;
    // offset: 0x0018
    short Segment;
    // offset: 0x001A
    short DropOffset;
    // offset: 0x001C
    short UpperOffset;
    // offset: 0x001E
    short Height;
    // offset: 0x0020 (4 bytes)
    Instance *UpperInstance;
    // offset: 0x0024
    int Dot;
} evPhysicsSlideData;

// size: 0x1C
typedef struct evObjectThrowData
{
    // offset: 0x0000
    short type;
    // offset: 0x0002
    short spinType;
    // offset: 0x0004 (8 bytes)
    union
    {
        // offset: 0x0000 (4 bytes)
        Instance *target;
        // offset: 0x0000 (6 bytes)
        Position position;
        // offset: 0x0000 (8 bytes)
        Rotation direction;
        // offset: 0x0000 (6 bytes)
        Position throwVector;
    } data;
    // offset: 0x000C (8 bytes)
    SVector angularVel;
    // offset: 0x0014
    unsigned short speed;
    // offset: 0x0016
    short gravity;
    // offset: 0x0018
    short initialXRot;
    // offset: 0x001A
    short zVel;
} evObjectThrowData;

// size: 0x10
typedef struct evObjectBirthProjectileData
{
    // offset: 0x0000 (4 bytes)
    Instance *instance;
    // offset: 0x0004
    int joint;
    // offset: 0x0008
    int type;
    // offset: 0x000C (4 bytes)
    Instance *birthInstance;
} evObjectBirthProjectileData;

// size: 0x24
typedef struct PhysObWeaponProperties
{
    // offset: 0x0000 (8 bytes)
    PhysObProperties Properties;
    // offset: 0x0008 (28 bytes)
    PhysObWeaponAttributes WeaponAttributes;
} PhysObWeaponProperties;

// size: 0x2C
typedef struct PhysObInteractProperties
{
    // offset: 0x0000 (8 bytes)
    PhysObProperties Properties;
    // offset: 0x0008
    unsigned short conditions;
    // offset: 0x000A
    unsigned short auxConditions;
    // offset: 0x000C
    unsigned short action;
    // offset: 0x000E
    unsigned short auxAction;
    // offset: 0x0010
    unsigned char startAnim;
    // offset: 0x0011
    unsigned char endAnim;
    // offset: 0x0012
    unsigned char razielAnim;
    // offset: 0x0013
    unsigned char razielAuxAnim;
    // offset: 0x0014
    unsigned char frame;
    // offset: 0x0015
    unsigned char startAnimMode;
    // offset: 0x0016
    unsigned short distance;
    // offset: 0x0018
    unsigned short newType;
    // offset: 0x001A
    unsigned short newClass;
    // offset: 0x001C
    unsigned short mode;
    // offset: 0x001E
    unsigned short engageXYDistance;
    // offset: 0x0020
    short engageZMinDelta;
    // offset: 0x0022
    short engageZMaxDelta;
    // offset: 0x0024
    unsigned char engageYCone;
    // offset: 0x0025
    unsigned char engageZCone;
    // offset: 0x0026
    unsigned short pad;
    // offset: 0x0028 (4 bytes)
    PhysObWeaponAttributes *weapon;
} PhysObInteractProperties;

// size: 0x10
typedef struct __PhysObProjectileProperties
{
    // offset: 0x0000 (8 bytes)
    PhysObProperties Properties;
    // offset: 0x0008
    long totalProjectiles;
    // offset: 0x000C (4 bytes)
    PhysObProjData *data;
} PhysObProjectileProperties;

// size: 0x14
typedef struct PhysObDraftProperties
{
    // offset: 0x0000 (8 bytes)
    PhysObProperties Properties;
    // offset: 0x0008
    short force;
    // offset: 0x000A
    unsigned short radius;
    // offset: 0x000C
    unsigned short radiusCoef;
    // offset: 0x000E
    short maxVelocity;
    // offset: 0x0010
    unsigned long height;
} PhysObDraftProperties;

// size: 0x20
typedef struct PhysObSwitchProperties
{
    // offset: 0x0000 (8 bytes)
    PhysObProperties Properties;
    // offset: 0x0008
    short Distance;
    // offset: 0x000A
    short Class;
    // offset: 0x000C
    unsigned char onAnim;
    // offset: 0x000D
    unsigned char offAnim;
    // offset: 0x000E
    unsigned char failedOnAnim;
    // offset: 0x000F
    unsigned char enableAnim;
    // offset: 0x0010
    unsigned char shutAnim;
    // offset: 0x0011
    unsigned char onWalkTimer;
    // offset: 0x0012
    unsigned char offWalkTimer;
    // offset: 0x0013
    unsigned char walkMode;
    // offset: 0x0014
    unsigned char razielOnAnim;
    // offset: 0x0015
    unsigned char razielOffAnim;
    // offset: 0x0016
    unsigned char razielFailedOnAnim;
    // offset: 0x0017
    unsigned char razielEnableAnim;
    // offset: 0x0018
    unsigned char startMode;
    // offset: 0x0019
    unsigned char startAnim;
    // offset: 0x001A
    unsigned short engageXYDistance;
    // offset: 0x001C
    char engageZMinDelta;
    // offset: 0x001D
    char engageZMaxDelta;
    // offset: 0x001E
    unsigned char engageYCone;
    // offset: 0x001F
    unsigned char engageZCone;
} PhysObSwitchProperties;

// size: 0x14
typedef struct PhysObCollectibleProperties
{
    // offset: 0x0000 (8 bytes)
    PhysObProperties Properties;
    // offset: 0x0008
    short collectClass;
    // offset: 0x000A
    short collectValue;
    // offset: 0x000C
    unsigned long distance;
    // offset: 0x0010
    unsigned char idleAnim;
    // offset: 0x0011
    unsigned char collectAnim;
    // offset: 0x0012
    unsigned short spatialNode;
} PhysObCollectibleProperties;

// size: 0xC
typedef struct PhysObAnimatedProperties
{
    // offset: 0x0000 (8 bytes)
    PhysObProperties Properties;
    // offset: 0x0008
    unsigned short flags;
    // offset: 0x000A
    unsigned short pad;
} PhysObAnimatedProperties;

// size: 0x48
typedef struct PhysObData
{
    // offset: 0x0000
    long Mode;
    // offset: 0x0004
    short xForce;
    // offset: 0x0006
    short yForce;
    // offset: 0x0008 (4 bytes)
    Instance *Force;
    // offset: 0x000C
    short LinkNode;
    // offset: 0x000E
    short px;
    // offset: 0x0010
    short py;
    // offset: 0x0012
    short pz;
    // offset: 0x0014
    short rx1;
    // offset: 0x0016
    short ry1;
    // offset: 0x0018
    short rz1;
    // offset: 0x001A
    short Segment1;
    // offset: 0x001C
    short rx2;
    // offset: 0x001E
    short ry2;
    // offset: 0x0020
    short rz2;
    // offset: 0x0022
    short Segment2;
    // offset: 0x0024
    short Steps;
    // offset: 0x0026
    short Step;
    // offset: 0x0028
    short PathNumber;
    // offset: 0x002A
    short Pad1;
    // offset: 0x002C (4 bytes)
    HInfo *RightCollision;
    // offset: 0x0030 (4 bytes)
    HInfo *LeftCollision;
    // offset: 0x0034
    short burnAmpl;
    // offset: 0x0036
    short throwFlags;
    // offset: 0x0038
    short initialXRot;
    // offset: 0x003A
    short xRotVel;
    // offset: 0x003C
    short yRotVel;
    // offset: 0x003E
    short zRotVel;
    // offset: 0x0040 (4 bytes)
    Instance *throwingInstance;
    // offset: 0x0044
    int physObTimer;
} PhysObData;

// size: 0x4
typedef struct BreakOffData
{
    // offset: 0x0000
    short NewType;
    // offset: 0x0002
    short NewClass;
} BreakOffData;

// size: 0x4
typedef struct ProjectileData
{
    // offset: 0x0000
    int Type;
} ProjectileData;

// size: 0x8
typedef struct _PhysObSaveData
{
    // offset: 0x0000 (4 bytes)
    SwitchData switchData;
    // offset: 0x0004
    long Mode;
} PhysObSaveData;

// size: 0x35C
typedef struct __CharacterState
{
    // offset: 0x0000 (4 bytes)
    Instance *CharacterInstance;
    // offset: 0x0004
    int TotalSections;
    // offset: 0x0008 (852 bytes)
    State SectionList[3];
} CharacterState;

// size: 0x10
typedef struct evMonsterHitData
{
    // offset: 0x0000 (4 bytes)
    Instance *sender;
    // offset: 0x0004 (4 bytes)
    Instance *lastHit;
    // offset: 0x0008
    short knockBackDistance;
    // offset: 0x000A
    short knockBackDuration;
    // offset: 0x000C
    long power;
} evMonsterHitData;

// size: 0x10
typedef struct evMonsterThrownData
{
    // offset: 0x0000 (4 bytes)
    Instance *sender;
    // offset: 0x0004 (8 bytes)
    Rotation direction;
    // offset: 0x000C
    short power;
} evMonsterThrownData;

// size: 0xC
typedef struct evMonsterAlarmData
{
    // offset: 0x0000 (4 bytes)
    Instance *sender;
    // offset: 0x0004 (6 bytes)
    Position position;
    // offset: 0x000A
    short type;
} evMonsterAlarmData;

// size: 0xC
typedef struct evMonsterSoulSuckData
{
    // offset: 0x0000 (4 bytes)
    Instance *sender;
    // offset: 0x0004 (6 bytes)
    Position Destination;
} evMonsterSoulSuckData;

// size: 0xC
typedef struct evPhysicsDropOffData
{
    // offset: 0x0000
    short xOffset;
    // offset: 0x0002
    short yOffset;
    // offset: 0x0004
    short DropOffset;
    // offset: 0x0006
    short slipSlope;
    // offset: 0x0008
    int UpperOffset;
} evPhysicsDropOffData;

// size: 0x24
typedef struct evPhysicsEdgeData
{
    // offset: 0x0000
    short UpperOffset;
    // offset: 0x0002
    short ForwardOffset;
    // offset: 0x0004
    short AboveOffset;
    // offset: 0x0006
    short XDistance;
    // offset: 0x0008
    short YDistance;
    // offset: 0x000A
    short ZDistance;
    // offset: 0x000C (4 bytes)
    SVector *Normal1;
    // offset: 0x0010 (4 bytes)
    SVector *Normal2;
    // offset: 0x0014 (4 bytes)
    SVector *Delta;
    // offset: 0x0018 (4 bytes)
    Instance *instance;
    // offset: 0x001C
    int zRot;
    // offset: 0x0020
    int rc;
} evPhysicsEdgeData;

// size: 0x2C
typedef struct evPhysicsWallCrawlData
{
    // offset: 0x0000
    short Segment;
    // offset: 0x0002
    short Length;
    // offset: 0x0004
    short ForwardOffset;
    // offset: 0x0006
    short NormalDistance;
    // offset: 0x0008 (8 bytes)
    SVector DropNormal;
    // offset: 0x0010 (8 bytes)
    SVector ForwardNormal;
    // offset: 0x0018 (6 bytes)
    Position NewPosition;
    // offset: 0x001E (8 bytes)
    Rotation DropRotation;
    // offset: 0x0026
    short ForwardXRotation;
    // offset: 0x0028
    int rc;
} evPhysicsWallCrawlData;

// size: 0x18
typedef struct evPhysicsSwimData
{
    // offset: 0x0000
    short CheckDepth;
    // offset: 0x0002
    short SwimDepth;
    // offset: 0x0004
    short WadeDepth;
    // offset: 0x0006
    short TreadDepth;
    // offset: 0x0008 (4 bytes)
    SVector *iVelocity;
    // offset: 0x000C
    short Depth;
    // offset: 0x000E
    short WaterDepth;
    // offset: 0x0010
    short rc;
    // offset: 0x0012
    short pad;
    // offset: 0x0014
    int WaterLevel;
} evPhysicsSwimData;

// size: 0x14
typedef struct evPhysicsDropHeightData
{
    // offset: 0x0000 (6 bytes)
    Position origin;
    // offset: 0x0006
    short mode;
    // offset: 0x0008
    short DropOffset;
    // offset: 0x000A
    short result;
    // offset: 0x000C (4 bytes)
    BSPTree *bsp;
    // offset: 0x0010 (4 bytes)
    TFace *tface;
} evPhysicsDropHeightData;

// size: 0x18
typedef struct evPhysicsLinkedMoveData
{
    // offset: 0x0000 (4 bytes)
    Instance *instance;
    // offset: 0x0004
    int segment;
    // offset: 0x0008 (8 bytes)
    SVector posDelta;
    // offset: 0x0010 (8 bytes)
    SVector rotDelta;
} evPhysicsLinkedMoveData;

// size: 0x14
typedef struct evFXHitData
{
    // offset: 0x0000 (8 bytes)
    SVector location;
    // offset: 0x0008 (8 bytes)
    SVector velocity;
    // offset: 0x0010
    short amount;
    // offset: 0x0012
    short type;
} evFXHitData;

// size: 0x10
typedef struct evAnimationControllerDoneData
{
    // offset: 0x0000 (4 bytes)
    Instance *instance;
    // offset: 0x0004
    long segment;
    // offset: 0x0008
    long type;
    // offset: 0x000C
    int data;
} evAnimationControllerDoneData;

// size: 0x14
typedef struct evMonsterImpaleData
{
    // offset: 0x0000 (4 bytes)
    Instance *weapon;
    // offset: 0x0004 (8 bytes)
    Rotation direction;
    // offset: 0x000C (6 bytes)
    Position position;
    // offset: 0x0012
    short distance;
} evMonsterImpaleData;

// size: 0x10
typedef struct evObjectBreakOffData
{
    // offset: 0x0000 (4 bytes)
    Instance *force;
    // offset: 0x0004
    unsigned short node;
    // offset: 0x0006
    short distance;
    // offset: 0x0008
    unsigned short animation;
    // offset: 0x000A
    unsigned short frame;
    // offset: 0x000C
    unsigned short type;
    // offset: 0x000E
    unsigned short action;
} evObjectBreakOffData;

// size: 0x8
typedef struct evObjectAbsorbData
{
    // offset: 0x0000 (4 bytes)
    Instance *force;
    // offset: 0x0004
    unsigned short node;
    // offset: 0x0006
    unsigned short steps;
} evObjectAbsorbData;

// size: 0x8
typedef struct evObjectIdleData
{
    // offset: 0x0000
    long mode;
    // offset: 0x0004 (4 bytes)
    Instance *instance;
} evObjectIdleData;

// size: 0xC
typedef struct evControlInitHangData
{
    // offset: 0x0000 (4 bytes)
    Instance *instance;
    // offset: 0x0004
    int frame;
    // offset: 0x0008
    int frames;
} evControlInitHangData;

// size: 0xC
typedef struct evControlInitIdleData
{
    // offset: 0x0000
    int mode;
    // offset: 0x0004
    int frame;
    // offset: 0x0008
    int frames;
} evControlInitIdleData;

// size: 0x10
typedef struct evActionLookAroundData
{
    // offset: 0x0000
    short *rotx;
    // offset: 0x0004
    short *rotz;
    // offset: 0x0008
    short minx;
    // offset: 0x000A
    short maxx;
    // offset: 0x000C
    short minz;
    // offset: 0x000E
    short maxz;
} evActionLookAroundData;

// size: 0x18
typedef struct evActionPlayHostAnimationData
{
    // offset: 0x0000 (4 bytes)
    Instance *instance;
    // offset: 0x0004 (4 bytes)
    Instance *host;
    // offset: 0x0008
    int newAnim;
    // offset: 0x000C
    int newFrame;
    // offset: 0x0010
    int frames;
    // offset: 0x0014
    int mode;
} evActionPlayHostAnimationData;

// size: 0xC
typedef struct evShadowSegmentData
{
    // offset: 0x0000
    unsigned long totalShadowSegments;
    // offset: 0x0004 (8 bytes)
    unsigned short shadowSegments[4];
} evShadowSegmentData;

// size: 0x4
typedef struct LitShaftProperties
{
    // offset: 0x0000
    short fadeValue;
    // offset: 0x0002
    short pad;
} LitShaftProperties;

// size: 0xA
typedef struct SavedEventSmallVars
{
    // offset: 0x0000
    unsigned char savedID;
    // offset: 0x0001
    unsigned char shiftedSaveSize;
    // offset: 0x0002
    short areaID;
    // offset: 0x0004
    char eventNumber;
    // offset: 0x0005 (5 bytes)
    char eventVariables[5];
} SavedEventSmallVars;

// size: 0x1C
typedef struct WaterLevelProcess
{
    // offset: 0x0000
    long flags;
    // offset: 0x0004 (4 bytes)
    BSPTree *bspTree;
    // offset: 0x0008 (4 bytes)
    StreamUnit *streamUnit;
    // offset: 0x000C
    short oldWaterZ;
    // offset: 0x000E
    short oldGlobalOffset;
    // offset: 0x0010
    short zValueFrom;
    // offset: 0x0012
    short zValueTo;
    // offset: 0x0014
    long maxSteps;
    // offset: 0x0018
    long curStep;
} WaterLevelProcess;

// size: 0x14
typedef struct EventInstanceObject
{
    // offset: 0x0000
    short id;
    // offset: 0x0002
    short flags;
    // offset: 0x0004
    long unitID;
    // offset: 0x0008
    long introUniqueID;
    // offset: 0x000C (4 bytes)
    Instance *instance;
    // offset: 0x0010 (4 bytes)
    union
    {
        // offset: 0x0000 (4 bytes)
        Intro *intro;
        // offset: 0x0000 (4 bytes)
        SFXMkr *sfxMarker;
    } data;
} EventInstanceObject;

// size: 0xC
typedef struct EventEventObject
{
    // offset: 0x0000
    short id;
    // offset: 0x0002
    short eventNumber;
    // offset: 0x0004
    long unitID;
    // offset: 0x0008 (4 bytes)
    Event *event;
} EventEventObject;

// size: 0x10
typedef struct EventTGroupObject
{
    // offset: 0x0000
    short id;
    // offset: 0x0002
    short tgroupNumber;
    // offset: 0x0004
    long unitID;
    // offset: 0x0008 (4 bytes)
    BSPTree *bspTree;
    // offset: 0x000C (4 bytes)
    StreamUnit *stream;
} EventTGroupObject;

// size: 0x484
typedef struct _PCodeStack
{
    // offset: 0x0000
    long topOfStack;
    // offset: 0x0004 (1152 bytes)
    StackType stack[32];
} PCodeStack;

// size: 0x18
typedef struct EventTimer
{
    // offset: 0x0000
    long flags : 1;
    long time : 31;
    // offset: 0x0004 (4 bytes)
    Event *event;
    // offset: 0x0008 (4 bytes)
    ScriptPCode *actionScript;
    // offset: 0x000C
    short *scriptPos;
    // offset: 0x0010 (4 bytes)
    Level *level;
    // offset: 0x0014
    long nextEventIndex;
} EventTimer;

// size: 0x8
typedef struct SignalResetStruct
{
    // offset: 0x0000 (4 bytes)
    MultiSignal *mSignal;
    // offset: 0x0004
    long timeLeft;
} SignalResetStruct;

// size: 0x10
typedef struct HintSystemStruct
{
    // offset: 0x0000
    short flags;
    // offset: 0x0002
    short hintNumber;
    // offset: 0x0004
    short stringNumber;
    // offset: 0x0006
    short pad;
    // offset: 0x0008
    long fadeTimer;
    // offset: 0x000C
    long spawningUnitID;
} HintSystemStruct;

// size: 0x9C
typedef struct __GlyphData
{
    // offset: 0x0000
    void (*process)();
    // offset: 0x0004 (136 bytes)
    MessageQueue messages;
    // offset: 0x008C
    short selectedGlyph;
    // offset: 0x008E
    short glyph_open;
    // offset: 0x0090
    short glyph_time;
    // offset: 0x0092
    short glyph_radius;
    // offset: 0x0094
    short glyph_scale;
    // offset: 0x0096
    short glyph_rotation;
    // offset: 0x0098
    short target_glyph_rotation;
    // offset: 0x009A
    short glyph_movement;
} GlyphData;

// size: 0x50
typedef struct _MonsterAttributes
{
    // offset: 0x0000
    unsigned long magicnum;
    // offset: 0x0004
    void *tunData;
    // offset: 0x0008
    char *auxAnimList;
    // offset: 0x000C
    char *ambientAnimList;
    // offset: 0x0010
    long whatAmI;
    // offset: 0x0014
    char numAuxAnims;
    // offset: 0x0015
    char numAmbientAnims;
    // offset: 0x0016
    unsigned char defaultAge;
    // offset: 0x0017
    char pupateObject;
    // offset: 0x0018
    unsigned char damageFXSegment;
    // offset: 0x0019
    unsigned char headSegment;
    // offset: 0x001A
    unsigned char neckSegment;
    // offset: 0x001B
    unsigned char leftShoulderSegment;
    // offset: 0x001C
    unsigned char rightShoulderSegment;
    // offset: 0x001D
    unsigned char waistSegment;
    // offset: 0x001E
    unsigned char leftKneeSegment;
    // offset: 0x001F
    unsigned char rightKneeSegment;
    // offset: 0x0020
    unsigned char leftFootSegment;
    // offset: 0x0021
    unsigned char rightFootSegment;
    // offset: 0x0022
    unsigned char spineSegment;
    // offset: 0x0023
    unsigned char leftWeaponSegment;
    // offset: 0x0024
    unsigned char rightWeaponSegment;
    // offset: 0x0025
    unsigned char grabSegment;
    // offset: 0x0026
    unsigned char bloodImpaleFrame;
    // offset: 0x0027
    unsigned char bloodConeFrame;
    // offset: 0x0028
    char numSubAttributes;
    // offset: 0x0029
    char numCombatAttributes;
    // offset: 0x002A
    char numAttackAttributes;
    // offset: 0x002B
    char numMissiles;
    // offset: 0x002C
    char numAnims;
    // offset: 0x002D
    char numIdles;
    // offset: 0x002E
    char numBehaviors;
    // offset: 0x002F
    char numShatters;
    // offset: 0x0030 (4 bytes)
    MonsterSubAttributes **subAttributesList;
    // offset: 0x0034 (4 bytes)
    MonsterCombatAttributes **combatAttributesList;
    // offset: 0x0038 (4 bytes)
    MonsterAttackAttributes *attackAttributesList;
    // offset: 0x003C (4 bytes)
    MonsterMissile *missileList;
    // offset: 0x0040 (4 bytes)
    MonsterAnim *animList;
    // offset: 0x0044 (4 bytes)
    MonsterIdle *idleList;
    // offset: 0x0048 (4 bytes)
    MonsterBehavior *behaviorList;
    // offset: 0x004C
    void *shatterList;
} MonsterAttributes;

// size: 0x24
typedef struct _MonsterIR
{
    // offset: 0x0000 (4 bytes)
    struct _MonsterIR *next;
    // offset: 0x0004 (4 bytes)
    Instance *instance;
    // offset: 0x0008
    long handle;
    // offset: 0x000C (8 bytes)
    SVector relativePosition;
    // offset: 0x0014
    short distance;
    // offset: 0x0016
    unsigned short mirFlags;
    // offset: 0x0018
    unsigned short mirConditions;
    // offset: 0x001C
    unsigned long forgetTimer;
    // offset: 0x0020
    unsigned long idleTime;
} MonsterIR;

// size: 0x1AC
typedef struct _MonsterVars
{
    // offset: 0x0000
    unsigned long mvFlags;
    // offset: 0x0004
    unsigned long auxFlags;
    // offset: 0x0008 (136 bytes)
    MessageQueue messageQueue;
    // offset: 0x0090 (4 bytes)
    MonsterIR *monsterIRList;
    // offset: 0x0094 (36 bytes)
    G2AnimInterpInfo interpInfo[3];
    // offset: 0x00B8 (4 bytes)
    MonsterAttackAttributes *attackType;
    // offset: 0x00BC (4 bytes)
    MonsterAnim *anim;
    // offset: 0x00C0
    void *messageData;
    // offset: 0x00C4 (4 bytes)
    MonsterIR *enemy;
    // offset: 0x00C8 (4 bytes)
    MonsterIR *leader;
    // offset: 0x00CC (4 bytes)
    MonsterIR *ally;
    // offset: 0x00D0 (4 bytes)
    Instance *held;
    // offset: 0x00D4
    long heldID;
    // offset: 0x00D8
    long soulID;
    // offset: 0x00DC
    void *effect;
    // offset: 0x00E0
    unsigned long radarMap;
    // offset: 0x00E4
    unsigned long radarCounter;
    // offset: 0x00E8 (32 bytes)
    short radarDistance[16];
    // offset: 0x0108
    unsigned long mode;
    // offset: 0x010C
    unsigned long damageTimer;
    // offset: 0x0110
    unsigned long stunTimer;
    // offset: 0x0114
    unsigned long generalTimer;
    // offset: 0x0118
    unsigned long generalTimer2;
    // offset: 0x011C
    unsigned long effectTimer;
    // offset: 0x0120 (4 bytes)
    Position *lookAtPos;
    // offset: 0x0124 (6 bytes)
    Position destination;
    // offset: 0x012A
    short speed;
    // offset: 0x012C (6 bytes)
    Position lastValidPos;
    // offset: 0x0132 (6 bytes)
    Position lookAtPosData;
    // offset: 0x0138
    short fleeMarker;
    // offset: 0x013A
    short wanderRange;
    // offset: 0x013C
    short guardRange;
    // offset: 0x013E
    short ambushRange;
    // offset: 0x0140
    short ambushArc;
    // offset: 0x0142
    short ambushElevation;
    // offset: 0x0144
    short hitPoints;
    // offset: 0x0146
    short maxHitPoints;
    // offset: 0x0148
    short soulJuice;
    // offset: 0x014A
    short regenTime;
    // offset: 0x014C
    short mainColorVertex;
    // offset: 0x014E
    short lookAngleX;
    // offset: 0x0150
    short lookAngleZ;
    // offset: 0x0152
    unsigned short avoidMask;
    // offset: 0x0154
    unsigned short damageType;
    // offset: 0x0156
    char ambushMarker;
    // offset: 0x0157
    char ambushJumpType;
    // offset: 0x0158
    unsigned char causeOfDeath;
    // offset: 0x0159
    char previousMainState;
    // offset: 0x015A
    unsigned char behaviorState;
    // offset: 0x015B
    unsigned char initialBehavior;
    // offset: 0x015C
    unsigned char triggeredBehavior;
    // offset: 0x015D
    char activeBehavior;
    // offset: 0x015E
    unsigned char age;
    // offset: 0x015F
    char attackState;
    // offset: 0x0160
    unsigned char ambient;
    // offset: 0x0161
    char alertness;
    // offset: 0x0162
    char alertCount;
    // offset: 0x0163
    char senseIndex;
    // offset: 0x0164
    char lastSideMove;
    // offset: 0x0165
    char chance;
    // offset: 0x0166
    char pathSlotID;
    // offset: 0x0168 (4 bytes)
    MonsterSubAttributes *subAttr;
    // offset: 0x016C (4 bytes)
    MonsterIR *freeIRs;
    // offset: 0x0170
    void *extraVars;
    // offset: 0x0174 (20 bytes)
    short validUnits[10];
    // offset: 0x0188 (24 bytes)
    short patrolMarkers[12];
    // offset: 0x01A0
    short *currentMarker;
    // offset: 0x01A4
    long terrainImpaleID;
    // offset: 0x01A8
    short targetFade;
    // offset: 0x01AA
    short fadeRate;
} MonsterVars;

// size: 0x8
typedef struct burntTuneType
{
    // offset: 0x0000
    int burntDist;
    // offset: 0x0004
    int fadeDelta;
} burntTuneType;

// size: 0x8
typedef struct _MonsterState
{
    // offset: 0x0000
    void (*entryFunction)();
    // offset: 0x0004
    void (*stateFunction)();
} MonsterState;

// size: 0xC
typedef struct _MonsterStateChoice
{
    // offset: 0x0000
    int state;
    // offset: 0x0004 (8 bytes)
    MonsterState functions;
} MonsterStateChoice;

// size: 0x20
typedef struct _MonsterFunctionTable
{
    // offset: 0x0000
    void (*initFunc)();
    // offset: 0x0004
    void (*cleanUpFunc)();
    // offset: 0x0008
    void (*damageEffectFunc)();
    // offset: 0x000C
    unsigned long (*queryFunc)();
    // offset: 0x0010
    void (*messageFunc)();
    // offset: 0x0014 (4 bytes)
    MonsterStateChoice *stateFuncs;
    // offset: 0x0018
    char *versionID;
    // offset: 0x001C
    char *localVersionID;
} MonsterFunctionTable;

// size: 0x2
typedef struct MissileGraphics
{
    // offset: 0x0000
    unsigned char object;
    // offset: 0x0001
    unsigned char model;
} MissileGraphics;

// size: 0x10
typedef struct evMonsterHitTerrainData
{
    // offset: 0x0000 (4 bytes)
    TFace *tface;
    // offset: 0x0004 (8 bytes)
    Normal normal;
    // offset: 0x000C
    short faceFlags;
    // offset: 0x000E
    short bspFlags;
} evMonsterHitTerrainData;

// size: 0x8
typedef struct evMonsterHitObjectData
{
    // offset: 0x0000 (4 bytes)
    Instance *instance;
    // offset: 0x0004
    int hitType;
} evMonsterHitObjectData;

// size: 0x10
typedef struct _MonsterSaveInfo
{
    // offset: 0x0000
    long mvFlags;
    // offset: 0x0004
    long auxFlags;
    // offset: 0x0008
    short soulID;
    // offset: 0x000A
    short soulJuice;
    // offset: 0x000C
    unsigned int age : 3;
    unsigned int state : 6;
    unsigned int behaviorState : 5;
    unsigned int anim : 6;
    unsigned int animLooping : 1;
    unsigned int causeOfDeath : 3;
} MonsterSaveInfo;

// size: 0x12
typedef struct evPhysicsLOSData
{
    // offset: 0x0000 (6 bytes)
    Position destination;
    // offset: 0x0006
    short arc;
    // offset: 0x0008 (6 bytes)
    Position origin;
    // offset: 0x000E
    short flags;
    // offset: 0x0010
    short result;
} evPhysicsLOSData;

// size: 0x8
typedef struct MonsterChoice
{
    // offset: 0x0000
    long whatAmI;
    // offset: 0x0004 (4 bytes)
    MonsterFunctionTable *table;
} MonsterChoice;

// size: 0x6
typedef struct RadarInfo
{
    // offset: 0x0000
    short angle;
    // offset: 0x0002
    short x;
    // offset: 0x0004
    short y;
} RadarInfo;

// size: 0x10
typedef struct burntMessType
{
    // offset: 0x0000
    int closestvert;
    // offset: 0x0004
    int closestdist;
    // offset: 0x0008
    int closestseg;
    // offset: 0x000C (4 bytes)
    burntTuneType *burntTune;
} burntMessType;

// size: 0x14
typedef struct bloodyMessType
{
    // offset: 0x0000
    int closestvert;
    // offset: 0x0004
    int closestdist;
    // offset: 0x0008
    int closestseg;
    // offset: 0x000C
    int bloodiedAVert;
    // offset: 0x0010
    int bloodIntensity;
} bloodyMessType;

/*struct _G2PoolMem_Type {
};*/

// size: 0x10
typedef struct _G2PoolMemPool_Type
{
    // offset: 0x0000
    unsigned short blockSize;
    // offset: 0x0002
    unsigned short stackTop;
    // offset: 0x0004
    unsigned short stackSize;
    // offset: 0x0008
    unsigned short *stack;
    // offset: 0x000C (4 bytes)
    struct _G2PoolMem_Type *blockPool;
} G2PoolMemPool;

// size: 0x1C
typedef struct _walbossAttributes
{
    // offset: 0x0000
    unsigned char segmentToAttach;
    // offset: 0x0001
    unsigned char frameToBirth;
    // offset: 0x0002
    unsigned char frameToExpel;
    // offset: 0x0003
    unsigned char maxEggHits;
    // offset: 0x0004
    short maxAngle;
    // offset: 0x0006
    short timeForHitAgony;
    // offset: 0x0008
    short segmentForBodyTwist;
    // offset: 0x000A
    short timeToThornPrep;
    // offset: 0x000C
    short timeToThornPoke;
    // offset: 0x000E
    short timeToEggThrob;
    // offset: 0x0010
    short timeToEggExplode;
    // offset: 0x0012
    short razielStunTime;
    // offset: 0x0014
    short eggIgniteDist;
    // offset: 0x0016
    short angleForShortAttack;
    // offset: 0x0018
    short closeAttackStart;
    // offset: 0x001A
    short junk;
    // offset: 0x001C
    struct
    {
        // offset: 0x0000
        short plusDelta;
        // offset: 0x0002
        short minusDelta;
        // offset: 0x0004
        short validAtHitPoint;
    } attackDeltas[0];
} WalbossAttributes;

// size: 0x24
typedef struct _G2AnimController_Type
{
    // offset: 0x0000
    unsigned short next;
    // offset: 0x0002
    unsigned char type;
    // offset: 0x0003
    unsigned char segNumber;
    // offset: 0x0004
    void *callbackData;
    // offset: 0x0008
    unsigned short flags;
    // offset: 0x000A
    short duration;
    // offset: 0x000C
    short elapsedTime;
    // offset: 0x0010 (4 bytes)
    G2AnimAlphaTable *alphaTable;
    // offset: 0x0014 (16 bytes)
    union
    {
        // offset: 0x0000 (8 bytes)
        struct
        {
            // offset: 0x0000
            unsigned long (*function)();
            // offset: 0x0004
            void *fnData;
        } callback;
        // offset: 0x0000 (12 bytes)
        struct
        {
            // offset: 0x0000 (6 bytes)
            G2SVector3 base;
            // offset: 0x0006 (6 bytes)
            G2SVector3 offset;
        } vector;
        // offset: 0x0000 (16 bytes)
        struct
        {
            // offset: 0x0000 (8 bytes)
            G2Quat src;
            // offset: 0x0008 (8 bytes)
            G2Quat dest;
        } quat;
    } data;
} G2AnimController;

// size: 0xC
typedef struct _G2AnimSegKeyflagInfo_Type
{
    // offset: 0x0000
    unsigned long *stream;
    // offset: 0x0004
    unsigned long flags;
    // offset: 0x0008
    int bitCount;
} G2AnimSegKeyflagInfo;

// size: 0x10
typedef struct _G2AnimDecompressChannelInfo_Type
{
    // offset: 0x0000 (4 bytes)
    G2AnimKeylist *keylist;
    // offset: 0x0004
    unsigned short *chanData;
    // offset: 0x0008
    int storedKey;
    // offset: 0x000C
    int targetKey;
} G2AnimDecompressChannelInfo;

// size: 0x18
typedef struct _G2AnimSegValue_Type
{
    // offset: 0x0000 (8 bytes)
    union
    {
        // offset: 0x0000 (6 bytes)
        G2SVector3 rot;
        // offset: 0x0000 (8 bytes)
        G2Quat quat;
    } rotQuat;
    // offset: 0x0008 (6 bytes)
    G2SVector3 scale;
    // offset: 0x000E
    unsigned short pad;
    // offset: 0x0010 (6 bytes)
    G2SVector3 trans;
    // offset: 0x0016
    unsigned short bIsQuat;
} G2AnimSegValue;

// size: 0x10
typedef struct _G2AnimChanStatusBlockPool_Type
{
    // offset: 0x0000
    unsigned short blockSize;
    // offset: 0x0002
    unsigned short stackTop;
    // offset: 0x0004
    unsigned short stackSize;
    // offset: 0x0008
    unsigned short *stack;
    // offset: 0x000C (4 bytes)
    G2AnimChanStatusBlock *blockPool;
} G2AnimChanStatusBlockPool;

// size: 0x10
typedef struct _G2AnimControllerPool_Type
{
    // offset: 0x0000
    unsigned short blockSize;
    // offset: 0x0002
    unsigned short stackTop;
    // offset: 0x0004
    unsigned short stackSize;
    // offset: 0x0008
    unsigned short *stack;
    // offset: 0x000C (4 bytes)
    G2AnimController *blockPool;
} G2AnimControllerPool;

// size: 0x10
typedef struct _G2AnimInterpStateBlockPool_Type
{
    // offset: 0x0000
    unsigned short blockSize;
    // offset: 0x0002
    unsigned short stackTop;
    // offset: 0x0004
    unsigned short stackSize;
    // offset: 0x0008
    unsigned short *stack;
    // offset: 0x000C (4 bytes)
    G2AnimInterpStateBlock *blockPool;
} G2AnimInterpStateBlockPool;

// size: 0x20
typedef struct PoolManagementData
{
    // offset: 0x0000
    char state;
    // offset: 0x0001
    unsigned char numNodesInPool;
    // offset: 0x0002 (6 bytes)
    Position playerPosAtLastPlanMkrUpdate;
    // offset: 0x0008 (4 bytes)
    PlanningNode *expansionNode1;
    // offset: 0x000C (4 bytes)
    PlanningNode *expansionNode2;
    // offset: 0x0010 (4 bytes)
    unsigned short (*distanceMatrix)[32];
    // offset: 0x0014
    unsigned short pairType;
    // offset: 0x0016 (6 bytes)
    Position peakPos;
    // offset: 0x001C
    unsigned char currentNode;
    // offset: 0x001D
    unsigned char currentBit;
} PoolManagementData;

// size: 0x44
typedef struct EnemyPlanData
{
    // offset: 0x0000 (8 bytes)
    unsigned char nodeSkipArray[8];
    // offset: 0x0008 (8 bytes)
    unsigned char nodeTypeArray[8];
    // offset: 0x0010 (48 bytes)
    Position wayPointArray[8];
    // offset: 0x0040
    short goalUnitID;
    // offset: 0x0042
    unsigned char numWayPoints;
} EnemyPlanData;

// size: 0x10
typedef struct _PlanCollideInfo
{
    // offset: 0x0000
    long StreamUnitID;
    // offset: 0x0004 (4 bytes)
    TFace *tFace;
    // offset: 0x0008 (6 bytes)
    Position collidePos;
} PlanCollideInfo;

// size: 0x4
typedef struct _fth
{
    // offset: 0x0000
    char x;
    // offset: 0x0001
    char y;
    // offset: 0x0002
    char z;
    // offset: 0x0003
    char nodeType;
} FindTerrainHit;

// size: 0x8
typedef struct _G2AppDataVM_Type
{
    // offset: 0x0000
    int argc;
    // offset: 0x0004
    int *argv;
} G2AppDataVM;

// size: 0x5E
typedef struct EnemyPlanSlotData
{
    // offset: 0x0000
    char slotInUse;
    // offset: 0x0001
    char state;
    // offset: 0x0002
    char wayPointBeingServoedTo;
    // offset: 0x0004 (6 bytes)
    Position startPos;
    // offset: 0x000A (6 bytes)
    Position goalPos;
    // offset: 0x0010 (6 bytes)
    Position oldCurrentPos;
    // offset: 0x0016 (68 bytes)
    EnemyPlanData planData;
    // offset: 0x005A
    char targetType;
    // offset: 0x005C
    short timer;
} EnemyPlanSlotData;

// size: 0x14
typedef struct __Force
{
    // offset: 0x0000
    int Type;
    // offset: 0x0004 (12 bytes)
    Vector LinearForce;
    // offset: 0x0010
    short Friction;
} Force;

// size: 0x4
typedef struct __Idle
{
    // offset: 0x0000
    unsigned char anim;
    // offset: 0x0001
    unsigned char frame;
    // offset: 0x0002
    unsigned char frames;
    // offset: 0x0003
    unsigned char type;
} Idle;

// size: 0x8
typedef struct __VAnim
{
    // offset: 0x0000
    unsigned char anim0;
    // offset: 0x0001
    unsigned char anim1;
    // offset: 0x0002
    unsigned char anim2;
    // offset: 0x0003
    unsigned char frames;
    // offset: 0x0004
    unsigned char mode;
    // offset: 0x0005
    unsigned char alpha;
    // offset: 0x0006
    unsigned short frame;
} VAnim;

// size: 0x10
typedef struct __SAnim
{
    // offset: 0x0000 (4 bytes)
    VAnim *anim;
    // offset: 0x0004 (4 bytes)
    struct __SAnim *nextAnim;
    // offset: 0x0008
    short mode;
    // offset: 0x000A
    short data;
    // offset: 0x000C
    short speedAdjust;
    // offset: 0x000E
    short pad;
} SAnim;

// size: 0x24
typedef struct __AttackItem
{
    // offset: 0x0000
    unsigned char anim;
    // offset: 0x0001
    unsigned char framesIn;
    // offset: 0x0002
    unsigned char alphaIn;
    // offset: 0x0003
    unsigned char framesOut;
    // offset: 0x0004
    unsigned char switchDelay;
    // offset: 0x0005
    unsigned char ignoreDelay;
    // offset: 0x0006
    unsigned char segmentToGlow;
    // offset: 0x0007
    unsigned char startCollisionFrame;
    // offset: 0x0008
    unsigned char handsToCollide;
    // offset: 0x0009
    unsigned char endCollisionFrame;
    // offset: 0x000A
    unsigned char ribbonStartFrame;
    // offset: 0x000B
    unsigned char ribbonStartSegment;
    // offset: 0x000C
    unsigned char ribbonEndSegment;
    // offset: 0x000D
    unsigned char ribbonLifeTime;
    // offset: 0x000E
    unsigned char ribbonFaceLifeTime;
    // offset: 0x000F
    unsigned char knockBackFrames;
    // offset: 0x0010
    unsigned char glowFadeInFrames;
    // offset: 0x0011
    unsigned char glowFadeOutFrames;
    // offset: 0x0012
    unsigned short ribbonStartOpacity;
    // offset: 0x0014
    unsigned long ribbonStartColor;
    // offset: 0x0018
    unsigned long ribbonEndColor;
    // offset: 0x001C
    unsigned long glowColor;
    // offset: 0x0020
    unsigned short knockBackDistance;
    // offset: 0x0022
    unsigned short hitPowerScale;
} AttackItem;

// size: 0x20
typedef struct __ThrowItem
{
    // offset: 0x0000
    unsigned char prepAnim;
    // offset: 0x0001
    unsigned char prepAnimFramesIn;
    // offset: 0x0002
    unsigned char prepAnimAlphaIn;
    // offset: 0x0003
    unsigned char coilAnim;
    // offset: 0x0004
    unsigned char coilAnimFramesIn;
    // offset: 0x0005
    unsigned char coilAnimAlphaIn;
    // offset: 0x0006
    unsigned char throwAnim;
    // offset: 0x0007
    unsigned char throwAnimFramesInFactor;
    // offset: 0x0008
    unsigned char throwFrameToLaunch;
    // offset: 0x0009
    unsigned char framesOut;
    // offset: 0x000A
    short launchPointX;
    // offset: 0x000C
    short launchPointY;
    // offset: 0x000E
    short launchPointZ;
    // offset: 0x0010
    short maxXRotation;
    // offset: 0x0012
    short minXRotation;
    // offset: 0x0014
    short maxZRotation;
    // offset: 0x0016
    short minZRotation;
    // offset: 0x0018
    short velocity;
    // offset: 0x001A
    short gravity;
    // offset: 0x001C
    short coilRot;
    // offset: 0x001E
    short selfAdjustingFlag;
} ThrowItem;

// size: 0x8
typedef struct __EngagedInstance
{
    // offset: 0x0000 (4 bytes)
    Instance *instance;
    // offset: 0x0004
    unsigned long distance;
} EngagedInstance;

// size: 0x4
typedef struct __FitPoint
{
    // offset: 0x0000
    short x;
    // offset: 0x0002
    short y;
} FitPoint;

// size: 0x54
typedef struct __SensoryData
{
    // offset: 0x0000
    int Flags;
    // offset: 0x0004 (8 bytes)
    SVector ForwardNormal;
    // offset: 0x000C (8 bytes)
    SVector AboveNormal;
    // offset: 0x0014 (8 bytes)
    SVector BelowNormal;
    // offset: 0x001C (8 bytes)
    SVector BelowPoint;
    // offset: 0x0024 (8 bytes)
    SVector Delta;
    // offset: 0x002C (4 bytes)
    Instance *HitMonster;
    // offset: 0x0030 (4 bytes)
    Instance *CurrentAutoFace;
    // offset: 0x0034 (4 bytes)
    Instance *LastAutoFace;
    // offset: 0x0038 (4 bytes)
    Instance *Portal;
    // offset: 0x003C (4 bytes)
    EngagedInstance *EngagedList;
    // offset: 0x0040
    unsigned long EngagedMask;
    // offset: 0x0044 (12 bytes)
    Vector lookAtPoint;
    // offset: 0x0050
    unsigned long heldClass;
} SensoryData;

// size: 0x5B8
typedef struct __Player
{
    // offset: 0x0000
    intptr_t footPrint;
    // offset: 0x0004 (860 bytes)
    CharacterState State;
    // offset: 0x0360
    int Mode;
    // offset: 0x0364
    int Anim;
    // offset: 0x0368
    int RotationSegment;
    // offset: 0x036C (8 bytes)
    SVector iVelocity;
    // offset: 0x0374
    int Magnitude;
    // offset: 0x0378
    int input;
    // offset: 0x037C
    int lastInput;
    // offset: 0x0380
    int ZDirection;
    // offset: 0x0384
    short Bearing;
    // offset: 0x0386
    short LastBearing;
    // offset: 0x0388
    int steeringMode;
    // offset: 0x038C
    short steeringVelocity;
    // offset: 0x038E
    short steeringAcceleration;
    // offset: 0x0390
    int steeringLockRotation;
    // offset: 0x0394
    short movementMinRate;
    // offset: 0x0396
    short movementMaxRate;
    // offset: 0x0398
    short movementMinAnalog;
    // offset: 0x039A
    short movementMaxAnalog;
    // offset: 0x039C (84 bytes)
    SensoryData Senses;
    // offset: 0x03F0 (4 bytes)
    Instance *attachedPlatform;
    // offset: 0x03F4
    short dropOffHeight;
    // offset: 0x03F6
    short fallZVelocity;
    // offset: 0x03F8
    short HealthScale;
    // offset: 0x03FA
    short HealthBalls;
    // offset: 0x03FC
    int HitPoints;
    // offset: 0x0400
    int DamageFrequency;
    // offset: 0x0404
    int invincibleTimer;
    // offset: 0x0408
    int Abilities;
    // offset: 0x040C (4 bytes)
    Instance *GlyphSystem;
    // offset: 0x0410
    unsigned short GlyphManaBalls;
    // offset: 0x0412
    unsigned short GlyphManaMax;
    // offset: 0x0414 (4 bytes)
    FitPoint *constrictData;
    // offset: 0x0418
    short constrictIndex;
    // offset: 0x041A
    short constrictWaitIndex;
    // offset: 0x041C (8 bytes)
    SVector constrictCenter;
    // offset: 0x0424
    short constrictFlag;
    // offset: 0x0426
    short constrictGoodCircle;
    // offset: 0x0428
    unsigned long constrictXTotal;
    // offset: 0x042C
    unsigned long constrictYTotal;
    // offset: 0x0430 (4 bytes)
    Instance *soulReaver;
    // offset: 0x0434
    unsigned long currentSoulReaver;
    // offset: 0x0438
    int CurrentPlane;
    // offset: 0x043C
    int slipSlope;
    // offset: 0x0440
    unsigned long idleCount;
    // offset: 0x0444 (4 bytes)
    Instance *idleInstance;
    // offset: 0x0448 (4 bytes)
    AttackItem *attack;
    // offset: 0x044C
    int attackFlags;
    // offset: 0x0450
    int currentAttack;
    // offset: 0x0454 (4 bytes)
    Instance *attackCurrentHit;
    // offset: 0x0458 (4 bytes)
    Instance *attackLastHit;
    // offset: 0x045C (4 bytes)
    FXGlowEffect *glowEffect;
    // offset: 0x0460 (4 bytes)
    Instance *attackedBy;
    // offset: 0x0464 (4 bytes)
    ThrowItem *throwData;
    // offset: 0x0468
    int throwMode;
    // offset: 0x046C (6 bytes)
    Position throwTarget;
    // offset: 0x0474 (4 bytes)
    Instance *throwInstance;
    // offset: 0x0478
    short throwXRot;
    // offset: 0x047A
    short throwZRot;
    // offset: 0x047C
    int throwReturnRot;
    // offset: 0x0480 (136 bytes)
    MessageQueue padCommands;
    // offset: 0x0508
    void (*returnState)();
    // offset: 0x050C
    unsigned long passedMask;
    // offset: 0x0510 (6 bytes)
    G2SVector3 extraRot;
    // offset: 0x0518
    long blankPad;
    // offset: 0x051C
    int effectsFlags;
    // offset: 0x0520
    short effectsFadeSource;
    // offset: 0x0522
    short effectsFadeDest;
    // offset: 0x0524
    int effectsFadeStep;
    // offset: 0x0528
    int effectsFadeSteps;
    // offset: 0x052C
    short autoFaceZone;
    // offset: 0x052E
    short autoFaceAngle;
    // offset: 0x0530
    int autoFaceLastRootAngle;
    // offset: 0x0534
    int autoFaceRootAngle;
    // offset: 0x0538
    int autoFaceTrueAngle;
    // offset: 0x053C
    int autoFaceAnim;
    // offset: 0x0540
    int autoFaceLastAnim;
    // offset: 0x0544
    int nothingCounter;
    // offset: 0x0548
    int timeAccumulator;
    // offset: 0x054C (6 bytes)
    Position puppetMoveToPoint;
    // offset: 0x0552 (6 bytes)
    Position puppetRotToPoint;
    // offset: 0x0558
    int soundModifier;
    // offset: 0x055C
    short alarmTable;
    // offset: 0x055E
    short terminator;
    // offset: 0x0560
    short swimSpeedAdjustment;
    // offset: 0x0562
    short swimTargetSpeed;
    // offset: 0x0564
    unsigned long soundHandle;
    // offset: 0x0568
    long soundTimer;
    // offset: 0x056C
    long soundTotalTime;
    // offset: 0x0570
    unsigned long soundDistance;
    // offset: 0x0574
    short soundStartPitch;
    // offset: 0x0576
    short soundEndPitch;
    // offset: 0x0578
    short soundStartVolume;
    // offset: 0x057A
    short soundEndVolume;
    // offset: 0x057C
    unsigned long soundHandle2;
    // offset: 0x0580
    long soundTimer2;
    // offset: 0x0584
    long soundTotalTime2;
    // offset: 0x0588
    unsigned long soundDistance2;
    // offset: 0x058C
    short soundStartPitch2;
    // offset: 0x058E
    short soundEndPitch2;
    // offset: 0x0590
    short soundStartVolume2;
    // offset: 0x0592
    short soundEndVolume2;
    // offset: 0x0594
    long soundTimerNext;
    // offset: 0x0598
    unsigned long soundTimerData;
    // offset: 0x059C
    unsigned long forcedGlideSpeed;
    // offset: 0x05A0 (8 bytes)
    SVector collisionEdgeOffset;
    // offset: 0x05A8 (4 bytes)
    SAnim *currentSAnim;
    // offset: 0x05AC
    unsigned long playerEvent;
    // offset: 0x05B0
    unsigned long playerEventHistory;
    // offset: 0x05B4
    int currentHint;
} Player;

// size: 0x14
typedef struct __CannedSound
{
    // offset: 0x0000
    short bank;
    // offset: 0x0002
    short sound;
    // offset: 0x0004
    short startPitch;
    // offset: 0x0006
    short endPitch;
    // offset: 0x0008
    short startVolume;
    // offset: 0x000A
    short endVolume;
    // offset: 0x000C
    long time;
    // offset: 0x0010
    long distance;
} CannedSound;

// size: 0x4
typedef struct __AlarmData
{
    // offset: 0x0000
    short command;
    // offset: 0x0002
    short data;
} AlarmData;

// size: 0x18
typedef struct _SoundRamp
{
    // offset: 0x0000
    unsigned long soundHandle;
    // offset: 0x0004
    long soundTimer;
    // offset: 0x0008
    long soundTotalTime;
    // offset: 0x000C
    unsigned long soundDistance;
    // offset: 0x0010
    short soundStartPitch;
    // offset: 0x0012
    short soundEndPitch;
    // offset: 0x0014
    short soundStartVolume;
    // offset: 0x0016
    short soundEndVolume;
} SoundRamp;

// size: 0x18
typedef struct PlayerSaveData
{
    // offset: 0x0000
    unsigned long abilities;
    // offset: 0x0004
    unsigned long currentPlane;
    // offset: 0x0008
    unsigned long healthScale;
    // offset: 0x000C
    unsigned long healthBalls;
    // offset: 0x0010
    unsigned short manaBalls;
    // offset: 0x0012
    unsigned short manaMax;
    // offset: 0x0014
    unsigned long playerEventHistory;
} PlayerSaveData;

// size: 0xA0
typedef struct RazielData
{
    // offset: 0x0000
    unsigned long version;
    // offset: 0x0004
    unsigned long nonBurningRibbonStartColor;
    // offset: 0x0008
    unsigned long nonBurningRibbonEndColor;
    // offset: 0x000C (4 bytes)
    Idle ***idleList;
    // offset: 0x0010 (4 bytes)
    AttackItem ***attackList;
    // offset: 0x0014 (4 bytes)
    ThrowItem **throwList;
    // offset: 0x0018 (4 bytes)
    VAnim *virtualAnimations;
    // offset: 0x001C (4 bytes)
    VAnim *virtualAnimSingle;
    // offset: 0x0020 (4 bytes)
    SAnim **stringAnimations;
    // offset: 0x0024
    short throwFadeValue;
    // offset: 0x0026
    short throwFadeInRate;
    // offset: 0x0028
    int throwFadeOutRate;
    // offset: 0x002C
    int throwManualDistance;
    // offset: 0x0030
    short healthMaterialRate;
    // offset: 0x0032
    short healthSpectralRate;
    // offset: 0x0034
    short healthInvinciblePostHit;
    // offset: 0x0036
    short healthInvinciblePostShunt;
    // offset: 0x0038
    short forceMinPitch;
    // offset: 0x003A
    short forceMaxPitch;
    // offset: 0x003C
    short forceMinVolume;
    // offset: 0x003E
    short forceMaxVolume;
    // offset: 0x0040
    unsigned long forceRampTime;
    // offset: 0x0044
    int SwimPhysicsFallDamping;
    // offset: 0x0048
    int SwimPhysicsWaterFrequency;
    // offset: 0x004C
    int SwimPhysicsWaterAmplitude;
    // offset: 0x0050
    int SwimPhysicsUnderDeceleration;
    // offset: 0x0054
    int SwimPhysicsUnderKickVelocity;
    // offset: 0x0058
    int SwimPhysicsUnderKickAccel;
    // offset: 0x005C
    int SwimPhysicsUnderVelocity;
    // offset: 0x0060
    int SwimPhysicsUnderKickDecel;
    // offset: 0x0064
    int SwimPhysicsUnderStealthAdjust;
    // offset: 0x0068
    int SwimPhysicsCoilVelocity;
    // offset: 0x006C
    int SwimPhysicsCoilDecelerationIn;
    // offset: 0x0070
    int SwimPhysicsCoilDecelerationOut;
    // offset: 0x0074
    int SwimPhysicsShotVelocity;
    // offset: 0x0078
    int SwimPhysicsShotAccelerationIn;
    // offset: 0x007C
    int SwimPhysicsShotAccelerationOut;
    // offset: 0x0080
    int SwimPhysicsSurfVelocity;
    // offset: 0x0084
    int SwimPhysicsSurfAccelerationIn;
    // offset: 0x0088
    int SwimPhysicsSurfAccelerationOut;
    // offset: 0x008C
    int SwimPhysicsSurfKickVelocity;
    // offset: 0x0090
    int SwimPhysicsSurfKickAccel;
    // offset: 0x0094
    int SwimPhysicsSurfMinRotation;
    // offset: 0x0098
    int SwimPhysicsSurfMaxRotation;
    // offset: 0x009C
    int SwimPhysicsSurfKickDecel;
} RazielData;

// size: 0x4
typedef struct __razController
{
    // offset: 0x0000
    unsigned short segment;
    // offset: 0x0002
    unsigned short type;
} razController;

// size: 0xC
typedef struct DR_STP
{
    // offset: 0x0000
    unsigned long tag;
    // offset: 0x0004 (8 bytes)
    unsigned long code[2];
} DR_STP;

// size: 0x8
typedef struct UW_ScreenXY
{
    // offset: 0x0000
    short sx;
    // offset: 0x0002
    short sy;
    // offset: 0x0004
    short dx;
    // offset: 0x0006
    short dy;
} UW_ScreenXY;

// size: 0x10
typedef struct SavedInfoTracker
{
    // offset: 0x0000
    char *MemoryCardBuffer;
    // offset: 0x0004
    char *InfoStart;
    // offset: 0x0008
    char *InfoEnd;
    // offset: 0x000C
    char *EndOfMemory;
} SavedInfoTracker;

// size: 0x8
typedef struct SavedLevel
{
    // offset: 0x0000
    unsigned char savedID;
    // offset: 0x0001
    unsigned char shiftedSaveSize;
    // offset: 0x0002
    short areaID;
    // offset: 0x0004
    short waterZ;
    // offset: 0x0006
    short numberBSPTreesSaved;
} SavedLevel;

// size: 0x10
typedef struct ActualSavedLevel
{
    // offset: 0x0000
    unsigned char savedID;
    // offset: 0x0001
    unsigned char shiftedSaveSize;
    // offset: 0x0002
    short areaID;
    // offset: 0x0004
    short waterZ;
    // offset: 0x0006
    short numberBSPTreesSaved;
    // offset: 0x0008 (8 bytes)
    SavedBspTree bspTreeArray[1];
} ActualSavedLevel;

// size: 0x342
typedef struct SavedDeadDeadBits
{
    // offset: 0x0000
    unsigned char savedID;
    // offset: 0x0001
    unsigned char shiftedSaveSize;
    // offset: 0x0002 (832 bytes)
    char deadDeadBits[832];
} SavedDeadDeadBits;

// size: 0x78
typedef struct _GlobalSaveTracker
{
    // offset: 0x0000
    short savedID;
    // offset: 0x0002
    short saveSize;
    // offset: 0x0004
    short flags;
    // offset: 0x0006
    short saveVersion;
    // offset: 0x0008
    unsigned long currentTime;
    // offset: 0x000C
    short humanOpinionOfRaziel;
    // offset: 0x000E
    char numRegens;
    // offset: 0x0010 (80 bytes)
    MONAPI_Regenerator regenEntries[10];
    // offset: 0x0060 (20 bytes)
    gSoundData sound;
    // offset: 0x0074
    short sizeUsedInBlock;
    // offset: 0x0076
    short CurrentBirthID;
} GlobalSaveTracker;

// size: 0x4
typedef struct CdlFILTER
{
    // offset: 0x0000
    unsigned char file;
    // offset: 0x0001
    unsigned char chan;
    // offset: 0x0002
    unsigned short pad;
} CdlFILTER;

// size: 0x28
typedef struct SpuCommonAttr
{
    // offset: 0x0000
    unsigned long mask;
    // offset: 0x0004 (4 bytes)
    SpuVolume mvol;
    // offset: 0x0008 (4 bytes)
    SpuVolume mvolmode;
    // offset: 0x000C (4 bytes)
    SpuVolume mvolx;
    // offset: 0x0010 (12 bytes)
    SpuExtAttr cd;
    // offset: 0x001C (12 bytes)
    SpuExtAttr ext;
} SpuCommonAttr;

// size: 0x2
typedef struct XAVoiceListEntry
{
    // offset: 0x0000
    unsigned short length;
} XAVoiceListEntry;

// size: 0x128
typedef struct XAVoiceTracker
{
    // offset: 0x0000 (4 bytes)
    CdlLOC currentPos;
    // offset: 0x0004
    int currentSector;
    // offset: 0x0008
    int endSector;
    // offset: 0x000C
    void (*prevCallback)();
    // offset: 0x0010 (8 bytes)
    unsigned char cdResult[8];
    // offset: 0x0018 (64 bytes)
    CdCommand cdCmdQueue[8];
    // offset: 0x0058 (8 bytes)
    unsigned short requestQueue[4];
    // offset: 0x0060 (64 bytes)
    VoiceCommand voiceCmdQueue[16];
    // offset: 0x00A0
    unsigned char cdCmdIn;
    // offset: 0x00A1
    unsigned char cdCmdOut;
    // offset: 0x00A2
    unsigned char cdCmdsQueued;
    // offset: 0x00A3
    unsigned char reqIn;
    // offset: 0x00A4
    unsigned char reqOut;
    // offset: 0x00A5
    unsigned char reqsQueued;
    // offset: 0x00A6
    unsigned char voiceCmdIn;
    // offset: 0x00A7
    unsigned char voiceCmdOut;
    // offset: 0x00A8
    unsigned char voiceCmdsQueued;
    // offset: 0x00A9
    unsigned char unused1;
    // offset: 0x00AA
    unsigned char voiceStatus;
    // offset: 0x00AB
    unsigned char cdStatus;
    // offset: 0x00AC
    short fileNum;
    // offset: 0x00AE
    short unused2;
    // offset: 0x00B0 (120 bytes)
    XAFileInfo xaFileInfo[30];
} XAVoiceTracker;

// size: 0x8
typedef struct cinema_fn_table_t
{
    // offset: 0x0000
    int (*play)();
    // offset: 0x0004
    char *versionID;
} cinema_fn_table_t;

// size: 0x10
typedef struct LocalizationHeader
{
    // offset: 0x0000 (4 bytes)
    language_t language;
    // offset: 0x0004
    int numXAfiles;
    // offset: 0x0008
    int XATableOffset;
    // offset: 0x000C
    int numStrings;
} LocalizationHeader;

// size: 0x10
typedef struct Extents2d
{
    // offset: 0x0000
    int xmin;
    // offset: 0x0004
    int xmax;
    // offset: 0x0008
    int ymin;
    // offset: 0x000C
    int ymax;
} Extents2d;

// size: 0x8
typedef struct menu_sound_entry_t
{
    // offset: 0x0000 (4 bytes)
    menu_sound_t sound;
    // offset: 0x0004
    int sfx;
} menu_sound_entry_t;

// size: 0x10
typedef struct menuface_t
{
    // offset: 0x0000
    short x;
    // offset: 0x0002
    short y;
    // offset: 0x0004
    char w;
    // offset: 0x0005
    char h;
    // offset: 0x0006
    char frames;
    // offset: 0x0008
    short curFrame;
    // offset: 0x000A
    short transitionDir;
    // offset: 0x000C
    unsigned short delay;
    // offset: 0x000E
    unsigned short loaded;
} menuface_t;

// size: 0x200
typedef struct mcmenu
{
    // offset: 0x0000 (404 bytes)
    mcmenu_t dummy1;
    // offset: 0x0194 (52 bytes)
    mcard_t dummy2;
    // offset: 0x01C8 (56 bytes)
    mcpsx_t dummy3;
} mcmenu;

// size: 0x4
union _77fake
{
    // offset: 0x0000
    long color;
    // offset: 0x0000 (4 bytes)
    RGBAColor rgba;
};

typedef char *caddr_t;

typedef long *qaddr_t;

typedef unsigned long ino_t;

typedef long swblk_t;

typedef long time_t;

typedef long off_t;

// typedef void (*CdlCB)();

typedef unsigned char u_byte;

typedef char int8;

typedef short int16;

typedef long int32;

typedef long int64;

typedef unsigned char uint8;

typedef unsigned short uint16;

typedef unsigned long uint32;

typedef unsigned long uint64;

typedef char BYTE;

typedef unsigned char UBYTE;

typedef unsigned long ULONG;

typedef unsigned short USHORT;

typedef unsigned long bool;

typedef long ListOrderFunc();

typedef unsigned long *OTag;

typedef unsigned char G2UInt8;

typedef unsigned short G2UInt16;

typedef unsigned long G2UInt32;

typedef char G2Int8;

typedef short G2Int16;

typedef long G2Int32;

typedef long G2fp4dot12;

typedef long G2fp8dot24;

typedef short G2TimerAtomicUnit;

typedef int G2AnimKeyframeNumber;

typedef int G2AnimKeylistID;

typedef int G2AnimSectionID;

typedef unsigned char G2AnimSegmentID;

typedef unsigned char G2AnimControllerType;

typedef unsigned short G2AnimChanMask;

typedef long (*G2AnimCallback)();

typedef unsigned long (*G2AnimControllerFn)();

typedef short G2AnimChanKeyData;

typedef short G2AnimChanFinalValue;

typedef enum _G2Bool_Enum(*G2InstanceRebuildCallback)();

typedef long lfixed;

typedef short sfixed;

typedef short MatEntry;

typedef short Fraction;

typedef long VectorCoord;

typedef short SVectorCoord;

typedef short NormalCoord;

typedef short TrigVal;

typedef short Angle;

typedef short Matrix[3][3];

typedef void InitFunc();

typedef void ProcessFunc();

typedef void CollideFunc();

typedef unsigned long QueryFunc();

typedef void MessageFunc();

typedef void RelocateTuneFunc();

typedef void (*SpuIRQCallbackProc)();

typedef void (*SpuTransferCallbackProc)();

typedef void (*SpuStCallbackProc)();

typedef int AADRESULT;

typedef int AadFileHandle;

typedef unsigned long AadChannelList;

typedef void (*AadFadeCompleteCallbackProc)();

typedef void (*AadLockVoiceCallbackProc)();

typedef void (*AadController11CallbackProc)();

typedef void (*AadEndSequenceCallbackProc)();

typedef void (*AadLoadDynamicSoundBankReturnProc)();

typedef void (*AadLoadDynamicSfxReturnProc)();

typedef void *(*AadMemoryMallocProc)();

typedef void (*AadMemoryFreeProc)();

typedef void (*AadNonBlockLoadReturnProc)();

typedef void (*AadNonBlockLoadProc)();

typedef void (*AadNonBlockBufferedLoadReturnProc)();

typedef void (*AadNonBlockBufferedLoadProc)();

typedef unsigned char AadNewSramBlockHandle;

typedef unsigned long AadSfxHandle;

typedef unsigned int AadSfxToneID;

typedef void (*EndSequenceCallbackProc)();

typedef void (*Controller11CallbackProc)();

typedef unsigned long *DrawNonAnimatedSegmentFunc();

typedef unsigned long *DrawAnimatedModelFunc();

typedef unsigned long *DrawDisplayPolytopeListFunc();

typedef unsigned long *AdditionalDrawFunc();

typedef void BG_DisplayFunc();

typedef void RelocateInstObFunc();

typedef void (*VoidProc)();

typedef void (*StateHandler)();

typedef void FX_PRIM_MODIFY_PROCESS();

typedef void FX_PROCESS();

typedef void FX_PARTICLE_PROCESS();

typedef void FX_GENERAL_PROCESS();

typedef void FX_SETUP();

typedef long TFract;

typedef void (*LoadReturnProc)();

typedef void (*BufferedLoadReturnProc)();

typedef void option_fn_t();

typedef int menu_fn_t();

typedef unsigned char CheatKey;

typedef void debug_dispatch_fn_t();

typedef void BSPLeafFunc();

typedef void G2AppData;

typedef void (*processVert_t)();

typedef long LightFunc();

typedef void GlyphProcessFunction();

typedef void SignalGlobalValueFunc();

typedef long HandleSignalFunc();

typedef long SignalHandleFunc();

typedef void RelocateSignalFunc();

typedef long CollideBoundFunc();

typedef void asm_fn();

typedef long hook_fn_1();

typedef long hook_fn_2();

typedef long hook_fn_3();

typedef long hook_fn_4();

typedef long hook_fn_5();

typedef long hook_fn_6();

typedef long hook_fn_7();

typedef long hook_fn_8();

typedef void (*MonsterFunc)();

typedef void (*MonsterDamageFunc)();

typedef void G2PoolMem;

typedef void (*G2AnimApplyControllerFn)();

typedef void (*VoiceCmdProc)();

typedef int PlayCinematic_t();

typedef int menu_item_fn_t();

typedef void menu_draw_fn_t();

typedef int mcard_filename_fn_t();

#endif
