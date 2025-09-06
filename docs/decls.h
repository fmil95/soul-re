#include "types.h"

// address: 0x800C8DB0
// size: 0xC0
extern struct DebugMenuLine mainMenu[8];

// address: 0x800C8090
// size: 0x120
extern struct DebugMenuLine standardMenu[12];

// address: 0x800C9EF0
// size: 0xA8
extern struct DebugMenuLine pauseMenu[7];

// address: 0x800C83C0
// size: 0x198
extern struct DebugMenuLine goodiesMenu[17];

// address: 0x800C8960
// size: 0xA8
extern struct DebugMenuLine debugImbueSoulReaverMenu[7];

// address: 0x800C8858
// size: 0x108
extern struct DebugMenuLine debugGlyphAbilitiesMenu[11];

// address: 0x800C87B0
// size: 0xA8
extern struct DebugMenuLine debugForgedAbilitiesMenu[7];

// address: 0x800C86C0
// size: 0xF0
extern struct DebugMenuLine debugSpecialAbilitiesMenu[10];

// address: 0x800C8618
// size: 0xA8
extern struct DebugMenuLine debugHealthSystemMenu[7];

// address: 0x800C8558
// size: 0xC0
extern struct DebugMenuLine debugRazielMenu[8];

// address: 0x800C9AA0
// size: 0x108
extern struct DebugMenuLine level2SelectMenu[11];

// address: 0x800C9BA8
// size: 0x150
extern struct DebugMenuLine levelSelectMenu[14];

// address: 0x800C8210
// size: 0xF0
extern struct DebugMenuLine eventMenu[10];

// address: 0x800C99C8
// size: 0xD8
extern struct DebugMenuLine BossAreasMenu[9];

// address: 0x800C81B0
// size: 0x60
extern struct DebugMenuLine saveMenu[4];

// address: 0x800C9CF8
// size: 0xC0
extern struct DebugMenuLine mcardMenu[8];

// address: 0x800C8300
// size: 0xC0
extern struct DebugMenuLine fogMenu[8];

// address: 0x800C8ED0
// size: 0x90
extern struct DebugMenuLine AIMenu[6];

// address: 0x800C8E70
// size: 0x60
extern struct DebugMenuLine cameraMenu[4];

// address: 0x800C8A08
// size: 0xC0
extern struct DebugMenuLine cheatsMenu[8];

// address: 0x800C8AC8
// size: 0x168
extern struct DebugMenuLine optsMenu[15];

// address: 0x800C8C30
// size: 0x60
extern struct DebugMenuLine gpadConfigMenu[4];

// address: 0x800C8C90
// size: 0x60
extern struct DebugMenuLine keybConfigMenu[4];

// address: 0x800C8CF0
// size: 0xC0
extern struct DebugMenuLine demoMenu[8];

// address: 0x800C8F60
// size: 0xF0
extern struct DebugMenuLine AlukaMenu[10];

// address: 0x800C9050
// size: 0xA8
extern struct DebugMenuLine AshVillageMenu[7];

// address: 0x800C90F8
// size: 0xF0
extern struct DebugMenuLine OracleMenu[10];

// address: 0x800C91E8
// size: 0x90
extern struct DebugMenuLine PillarsMenu[6];

// address: 0x800C9278
// size: 0x108
extern struct DebugMenuLine SilencedCathedralMenu[11];

// address: 0x800C9380
// size: 0xA8
extern struct DebugMenuLine SkinnerMenu[7];

// address: 0x800C9428
// size: 0x78
extern struct DebugMenuLine StoneMenu[5];

// address: 0x800C94A0
// size: 0xA8
extern struct DebugMenuLine SunLightMenu[7];

// address: 0x800C9548
// size: 0x90
extern struct DebugMenuLine TombMenu[6];

// address: 0x800C95D8
// size: 0x60
extern struct DebugMenuLine WaterMenu[4];

// address: 0x800C9638
// size: 0x78
extern struct DebugMenuLine CityMenu[5];

// address: 0x800C96B0
// size: 0x48
extern struct DebugMenuLine CliffMenu[3];

// address: 0x800C96F8
// size: 0x48
extern struct DebugMenuLine UnderMenu[3];

// address: 0x800C9740
// size: 0x48
extern struct DebugMenuLine MorlockMenu[3];

// address: 0x800C9788
// size: 0x78
extern struct DebugMenuLine HubAMenu[5];

// address: 0x800C9800
// size: 0x48
extern struct DebugMenuLine HubBMenu[3];

// address: 0x800C9848
// size: 0x78
extern struct DebugMenuLine TrainingMenu[5];

// address: 0x800C98C0
// size: 0x48
extern struct DebugMenuLine DarkEdenMenu[3];

// address: 0x800C9908
// size: 0xC0
extern struct DebugMenuLine AnteRoomsMenu[8];

// address: 0x800C9DB8
// size: 0xC0
extern struct DebugMenuLine mcardSaveMenu[8];

// address: 0x800C9E78
// size: 0x78
extern struct DebugMenuLine pauseSaveMenu[5];

// address: 0x800C9F98
// size: 0x28
static struct debug_dispatch_t debug_dispatch_table[5];

// address: 0x800CFFA4
// size: 0x18
extern struct DebugMenuLine *currentMenu;

// address: 0x800CF59C
// size: 0x14
static char pauseFormatString[20];

// address: 0x800CF5B0
// size: 0x14
static char mainFormatString[20];

// address: 0x800CF5C4
static char *the_format_string;

// address: 0x800CF5C8
extern long menuPauseSel;

// address: 0x800CF5CC
extern long debug_cameraMode;

// address: 0x800CF5D0
extern long debugVoiceIndex;

// address: 0x800CF5D4
extern unsigned long debugTimeMult;

// address: 0x800CF5D8
extern long debugFogFar;

// address: 0x800CF5DC
extern long debugFogNear;

// address: 0x800CF5E0
extern long debugGameTime;

// address: 0x800CF5E4
extern long debugFogRed;

// address: 0x800CF5E8
extern long debugFogGrn;

// address: 0x800CF5EC
extern long debugFogBlu;

// address: 0x800CF5F0
extern long debugHealthLevel;

// address: 0x800CF5F4
extern long debugManaLevel;

// address: 0x800CFFA8
static long debugMenuChoice;

// address: 0x800CFFAC
static long cheatCodeLastCommand;

// address: 0x800CFFB0
// size: 0x28
static long CheatKeyMap[10];

// address: 0x800CFFD8
// size: 0x170
static struct GameCheat GameCheats[23];

// address: 0x800D0148
// size: 0x18
extern struct DebugMenuLine *the_previous_menu;

// address: 0x800D014C
static int cem_x_base;

// address: 0x800D0150
static int cem_y_base;

// address: 0x800D0154
static int cem_cursor_width;

// address: 0x800D0158
static int cem_line_width;

// address: 0x800D015C
static int cem_line_leading;

// address: 0x800D0160
static int cem_item_leading;

// address: 0x800D02C4
extern short AXIS_LENGTH;

// address: 0x800D20F0
extern unsigned long debugRazielFlags1;

// address: 0x800D20F4
extern unsigned long debugRazielFlags2;

// address: 0x800D20F8
extern unsigned long debugRazielFlags3;

// address: 0x800D20D4
extern short debugOrgFogFar;

// address: 0x800D20C4
extern short debugOrgFogNear;

// address: 0x800D20C8
extern unsigned char debugOrgFogRed;

// address: 0x800D20CC
extern unsigned char debugOrgFogGrn;

// address: 0x800D20D0
extern unsigned char debugOrgFogBlu;

// address: 0x800D20AC
// size: 0x18
extern struct DebugMenuLine debugSoundMenu[1];

// address: 0x800D20D8
// size: 0x18
extern struct DebugMenuLine statsMenu[1];

// address: 0x800D0604
extern long draw_belowSplit;

// address: 0x800D0600
extern short forceDrawInFront;

// address: 0x800D0608
// size: 0x8
extern struct SVECTOR *face_v0;

// address: 0x800D060C
// size: 0x8
extern struct SVECTOR *face_v1;

// address: 0x800D0610
// size: 0x8
extern struct SVECTOR *face_v2;

// address: 0x800D0614
// size: 0x8
extern struct SVECTOR *face_v01;

// address: 0x800D0618
// size: 0x8
extern struct SVECTOR *face_v12;

// address: 0x800D061C
// size: 0x8
extern struct SVECTOR *face_v20;

// address: 0x800D0620
// size: 0x4
extern struct UVTYPE *face_uv0;

// address: 0x800D0624
// size: 0x4
extern struct UVTYPE *face_uv1;

// address: 0x800D0628
// size: 0x4
extern struct UVTYPE *face_uv2;

// address: 0x800D062C
// size: 0x4
extern struct UVTYPE *face_uv01;

// address: 0x800D0630
// size: 0x4
extern struct UVTYPE *face_uv12;

// address: 0x800D0634
// size: 0x4
extern struct UVTYPE *face_uv20;

// address: 0x800D2720
extern long modelDQP;

// address: 0x800D271C
extern unsigned char *draw_stack;

// address: 0x800D2724
// size: 0x58
extern struct SVECTOR shadow_vertices[11];

// address: 0x800C9FC0
// size: 0x120
extern struct FontPos fontPos[72];

// address: 0x800CA0E0
// size: 0x114
extern char charMap[3][92];

// address: 0x800D0638
// size: 0x3C
static struct font_color_t the_font_color_table[5];

// address: 0x800D068C
// size: 0x80
extern unsigned char fontTransTable[128];

// address: 0x800D2984
// size: 0x620
extern struct FontTracker fontTracker;

// address: 0x800D297C
// size: 0x1C
extern struct _BlockVramEntry_duplicate_s8 *FONT_vramBlock;

// address: 0x800D2980
extern unsigned short SpecialFogClut;

// address: 0x800D277C
// size: 0x200
static char fp_str[512];

// address: 0x800CA21C
// size: 0x2
extern unsigned char dualshock_motors[2];

// address: 0x800CA220
// size: 0x6
extern unsigned char dualshock_align[6];

// address: 0x800D07C0
static short align_flag;

// address: 0x800D07C2
static short dualshock_onflag;

// address: 0x800D07C4
static int dualShock;

// address: 0x800D0808
// size: 0x4
extern unsigned short lastData[2];

// address: 0x800D080C
static int gpSaved;

// address: 0x800D3108
extern int gamePadControllerOut;

// address: 0x800D30F8
// size: 0x10
extern long gDummyCommand[2][2];

// address: 0x800D3060
// size: 0x2
extern unsigned char controllerType[2];

// address: 0x800D2FFC
static int ignoreFind;

// address: 0x800D3068
// size: 0x22
extern struct ControllerPacket gpbuffer1;

// address: 0x800D30B0
// size: 0x22
extern struct ControllerPacket gpbuffer2;

// address: 0x800D308C
// size: 0x22
extern struct ControllerPacket readGPBuffer1;

// address: 0x800D30D4
// size: 0x22
extern struct ControllerPacket readGPBuffer2;

// address: 0x800D3000
static long dualshock0_time;

// address: 0x800D3004
static long dualshock1_time;

// address: 0x800D3064
// size: 0x4
extern unsigned short psxData[2];

// address: 0x800D3008
// size: 0x28
static long overrideData[5][2];

// address: 0x800D3038
// size: 0x28
static long overrideCommand[5][2];

// address: 0x800D03A0
// size: 0x3C
extern long camera_modeToIndex[15];

// address: 0x800D03DC
static short shorten_count;

// address: 0x800D03DE
static short shorten_flag;

// address: 0x800D03E0
static short camera_still;

// address: 0x800D03E4
static long roll_target;

// address: 0x800D03E8
static long current_roll_amount;

// address: 0x800D03EC
static int roll_inc;

// address: 0x800D03FE
static short CameraLookStickyFlag;

// address: 0x800D0402
static short panic_count;

// address: 0x800D2228
// size: 0x4C4
extern struct Camera_duplicate_s2 theCamera;

// address: 0x800D210C
// size: 0x20
static struct MATRIX wcTransformX;

// address: 0x800D212C
// size: 0x20
static struct MATRIX wcTransform2X;

// address: 0x800D214C
// size: 0x20
static struct MATRIX cwTransform2X;

// address: 0x800D216C
static short combat_cam_distance;

// address: 0x800D216E
static short combat_cam_angle;

// address: 0x800D2170
static short combat_cam_weight;

// address: 0x800D2172
static short combat_cam_debounce;

// address: 0x800D26F4
extern int CameraCenterDelay;

// address: 0x800D2174
static short CenterFlag;

// address: 0x800D217C
// size: 0x80
static struct _SVector camera_shakeOffset[16];

// address: 0x800D21FC
// size: 0x8
static struct _SVector camera_plane;

// address: 0x800D2204
// size: 0x8
static struct _SVector left_point;

// address: 0x800D220C
// size: 0x8
static struct _SVector right_point;

// address: 0x800D2214
static short hitline_rot;

// address: 0x800D2218
static long ACE_amount;

// address: 0x800D221C
// size: 0x8
static struct _Rotation splinecam_helprot;

// address: 0x800D2224
static short Camera_lookHeight;

// address: 0x800D2226
static short Camera_lookDist;

// address: 0x800D26EC
extern long campoint_hw;

// address: 0x800D26F0
extern long campoint_hh;

// address: 0x800D08C8
// size: 0x40
static struct LightGroup default_lightgroup;

// address: 0x800D3110
extern void *light_light;

// address: 0x800D3118
// size: 0x47C
extern struct LightInfo *light_lightInfo;

// address: 0x800D3114
// size: 0x54
extern struct _Terrain *light_terrain;

// address: 0x800D310C
extern long (*light_sourceAndVertexFunc)();

// address: 0x800D0980
// size: 0x1C
extern char HashExtensions[4][7];

// address: 0x800D312C
// size: 0xA0
static struct _LoadStatus loadStatus;

// address: 0x800D31CC
extern long crap1;

// address: 0x800D09E4
extern int _ramsize;

// address: 0x800D09E8
extern int _stacksize;

// address: 0x800D09EC
extern char mainOptionsInit;

// address: 0x800D09EE
extern short gEndGameNow;

// address: 0x800D09F0
extern int pcverify;

// address: 0x800D09F4
extern int nosound;

// address: 0x800D09F8
extern int nomusic;

// address: 0x800D09FC
// size: 0x150
extern struct InterfaceItem InterfaceItems[6];

// address: 0x800D0B4C
extern long DoMainMenu;

// address: 0x800D0B50
extern long mainMenuMode;

// address: 0x800D0B54
extern long *mainMenuScreen;

// address: 0x800D0B58
extern long *controlsScreen;

// address: 0x800D0B5C
extern short mainMenuFadeInTime;

// address: 0x800D0B5E
extern short mainMenuFading;

// address: 0x800D3360
// size: 0x270
extern struct GameTracker gameTrackerX;

// address: 0x800D3220
// size: 0xB8
extern struct DRAWENV draw[2];

// address: 0x800D31F4
// size: 0x28
extern struct DISPENV disp[2];

// address: 0x800D333C
// size: 0x20
extern struct BLK_FILL clearRect[2];

// address: 0x800D35D0
// size: 0x10
extern struct MainTracker mainTrackerX;

// address: 0x800D321C
extern int devstation;

// address: 0x800D3338
extern unsigned long __timerEvent;

// address: 0x800D335C
extern short mainMenuTimeOut;

// address: 0x800D31D0
static int mainMenuSfx;

// address: 0x800D35E0
// size: 0x80
extern struct _ButtonTexture PauseButtons[2][4];

// address: 0x800D32D8
// size: 0x60
extern struct _ButtonTexture BmenuLines[2][3];

// address: 0x800D31D4
// size: 0x20
extern struct _ButtonTexture BexitLine[2];

// address: 0x800D070C
extern long cameraMode;

// address: 0x800D0710
extern long playerCameraMode;

// address: 0x800D0714
// size: 0x4C
extern struct Object *fontsObject;

// address: 0x800D2FDC
// size: 0x6DA8
extern struct _FXTracker *gFXT;

// address: 0x800D2FEC
extern unsigned long StackSave;

// address: 0x800D2FA4
// size: 0x8
static unsigned long **gOt[2];

// address: 0x800D2FF4
extern char *primBase;

// address: 0x800D2FAC
// size: 0x8
static struct _PrimPool *primPool[2];

// address: 0x800D2FB4
// size: 0x1004
static struct _PolytopeList *gPolytopeList;

// address: 0x800D2FB8
// size: 0x10C
static struct _InstanceList *instanceList;

// address: 0x800D2FBC
// size: 0x9C9C
static struct _InstancePool *instancePool;

// address: 0x800D2FC0
// size: 0x18C0
static struct _VertexPool *gVertexPool;

// address: 0x800D2FC4
// size: 0x47C
static struct LightInfo *gLightInfo;

// address: 0x800D2FF0
// size: 0x6DA8
extern struct _FXTracker *fxTracker;

// address: 0x800D2FC8
static void *enemyPlanPool;

// address: 0x800D2FCC
static void *planningPool;

// address: 0x800D2FD0
// size: 0x24
static struct _ObjectTracker *GlobalObjects;

// address: 0x800D2FE4
extern short RENDER_currentStreamUnitID;

// address: 0x800D2FD4
static short pause_redraw_flag;

// address: 0x800D2FD8
static void *pause_redraw_prim;

// address: 0x800D2FE0
extern int InStreamUnit;

// address: 0x800D2FE8
extern unsigned long **hackOT;

// address: 0x800D0408
// size: 0x4
extern void (*HandleGlobalValueSignal[1])();

// address: 0x800D0424
extern long SignalEndGame;

// address: 0x800D0428
// size: 0x144
extern struct SignalInfo signalInfoList[27];

// address: 0x800D056C
// size: 0x20
extern long dyna_clddyna[8];

// address: 0x800D058C
// size: 0x20
extern long stat_clddyna[8];

// address: 0x800D05AC
// size: 0x20
extern long dyna_cldstat[8];

// address: 0x800D05CC
extern long collide_ignoreAttr;

// address: 0x800D05D0
extern long collide_acceptAttr;

// address: 0x800D2708
// size: 0x8
extern struct _SVector collide_localPoint;

// address: 0x800D2710
// size: 0x20
extern struct MATRIX *collide_lwTransform;

// address: 0x800D26F8
// size: 0x8
extern struct _SVector *collide_point0;

// address: 0x800D26FC
// size: 0x8
extern struct _SVector *collide_point1;

// address: 0x800D2714
// size: 0x8
extern struct _SVector *collide_normal0;

// address: 0x800D2718
// size: 0x8
extern struct _SVector *collide_normal1;

// address: 0x800D2700
extern long collide_t0;

// address: 0x800D2704
extern long collide_t1;

// address: 0x800D0D60
extern long gTimerEnabled;

// address: 0x800D3660
// size: 0x47C
extern struct LightInfo *sbsp_lightInfo;

// address: 0x800CA23C
// size: 0xE0
extern struct ObjectAccess objectAccess[28];

// address: 0x800CA228
// size: 0x14
extern char *selectMenuText[5];

// address: 0x800D0CFC
// size: 0x12
extern short identityMatrix[3][3];

// address: 0x800D0FD4
extern long FRAC_BITS;

// address: 0x800D0FFC
// size: 0x10
static struct _FXGeneralEffect *FX_GeneralEffectTracker;

// address: 0x800D1000
// size: 0x2C
static struct _FXRibbon *FX_ConstrictRibbon;

// address: 0x800D1004
static short FX_ConstrictStage;

// address: 0x800D1008
// size: 0x29C
static struct _Instance *FX_ConstrictInstance;

// address: 0x800D100C
static short snow_amount;

// address: 0x800D100E
static short rain_amount;

// address: 0x800D1010
static short current_rain_fade;

// address: 0x800D1014
// size: 0x29C
static struct _Instance *FX_reaver_instance;

// address: 0x800D103C
static short windx;

// address: 0x800D103E
static short windy;

// address: 0x800D1040
static short wind_deg;

// address: 0x800D1042
static short wind_speed;

// address: 0x800D1044
// size: 0x18
extern long FX_ColorArray[6];

// address: 0x800D105C
static short Spiral_Number;

// address: 0x800D105E
static short Spiral_Degrees;

// address: 0x800D1060
static long Spiral_Current;

// address: 0x800D1064
static long Spiral_Max;

// address: 0x800D3948
// size: 0x6
static struct _Position FX_ConstrictPosition;

// address: 0x800D3950
// size: 0x6
static struct _Position *FX_ConstrictPositionPtr;

// address: 0x800D3954
// size: 0x68
static struct _FX_PRIM_duplicate_s24 *FX_LastUsedPrim;

// address: 0x800D3958
static short FX_Frames;

// address: 0x800D395A
static short FX_TimeCount;

// address: 0x800D3968
// size: 0x104
static struct DVECTOR Spiral_Array[65];

// address: 0x800D3A78
// size: 0x100
static struct DVECTOR Spiral_OffsetP[64];

// address: 0x800D3B78
// size: 0x100
static struct DVECTOR Spiral_OffsetM[64];

// address: 0x800D3C78
static int Spiral_Glow_X;

// address: 0x800D3C7C
static int Spiral_Glow_Y;

// address: 0x800D3C80
static int Spiral_Glow_Size;

// address: 0x800D3C84
static int Spiral_Mod;

// address: 0x800CA31C
// size: 0x120
extern struct ObjectFunc objectFunc[8];

// address: 0x800D0F20
extern int gSramFullAlarm;

// address: 0x800D0F24
static int gSramFullMsgCnt;

// address: 0x800D0F28
// size: 0xC
static long objectOneShotTriggerTbl[3];

// address: 0x800D38BC
extern int gSramTotalUsed;

// address: 0x800D38B8
extern int gSramUsedBlocks;

// address: 0x800D38C0
extern int gSramTotalFree;

// address: 0x800D3670
extern int gSramLargestFree;

// address: 0x800D38B4
extern int gSramFreeBlocks;

// address: 0x800D3674
// size: 0x240
extern struct SoundEffectChannel soundEffectChannelTbl[16];

// address: 0x800D38C4
// size: 0x4C
extern struct MusicLoadInfo musicInfo;

// address: 0x800D75BC
// size: 0x33C8
extern char soundBuffer[13256];

// address: 0x800D13BC
// size: 0x24
static void (*sfxCmdFunction[9])();

// address: 0x800D13E0
// size: 0x8
static struct AadProgramAtr aadDummySfxProgram;

// address: 0x800D3C88
// size: 0x14
static struct NewMemTracker newMemTracker;

// address: 0x800CC3F4
extern unsigned long aadGp;

// address: 0x800D1158
// size: 0x8
static unsigned short aadHblanksPerUpdate[4];

// address: 0x800D1160
// size: 0x10
static unsigned long aadUpdateRate[4];

// address: 0x800D1170
// size: 0x1C88
extern struct AadMemoryStruct *aadMem;

// address: 0x800D1174
static int gDefragRequest;

// address: 0x800D11F4
// size: 0x28
static unsigned long aadReverbModeSize[10];

// address: 0x800D3C9C
static unsigned long __hblankEvent;

// address: 0x800D3CA0
static unsigned char *smfDataPtr;

// address: 0x800D3CA4
static unsigned long smfBytesLeft;

// address: 0x800D3CA8
// size: 0x94
static struct AadDynamicSfxLoadInfo *smfInfo;

// address: 0x800D121C
// size: 0x8
static char midiDataByteCount[8];

// address: 0x800D1224
// size: 0x20
static void (*midiEventFunction[8])();

// address: 0x800D1244
// size: 0x40
static void (*midiControlFunction[16])();

// address: 0x800D1284
// size: 0x138
static void (*midiMetaEventFunction[78])();

// address: 0x800CA454
// size: 0x7A0
extern struct _Normal gNormalList[244];

// address: 0x800CABF4
// size: 0x1800
extern short ecostable[3072];

// address: 0x800D106C
static long vmRealClock;

// address: 0x800D1070
static long vmClock;

// address: 0x800D13E8
// size: 0xAA
extern unsigned short aadPitchTable[85];

// address: 0x800D1494
// size: 0x154
extern unsigned long aadStepsPerCent[85];

// address: 0x800D15E8
// size: 0xAA
extern short aadStepsPerSemitone[85];

// address: 0x800D1694
extern long CurrentWarpNumber;

// address: 0x800D1698
// size: 0x118
extern struct WarpRoom WarpRoomArray[14];

// address: 0x800D3CD4
// size: 0x400
extern struct STracker StreamTracker;

// address: 0x800D3CB0
// size: 0x14
extern struct _294fake WarpGateLoadInfo;

// address: 0x800D40D4
// size: 0xC
extern struct _TFace *MORPH_SavedFace;

// address: 0x800D3CC4
// size: 0x188
extern struct Level *MORPH_SavedLevel;

// address: 0x800D3CC8
// size: 0x6
extern short MORPH_Component[3];

// address: 0x800D3CD0
// size: 0x4
extern short MORPH_Track[2];

// address: 0x800D3CAC
static int s_zval;

// address: 0x800D1A1C
// size: 0x28
extern long RazielCommands[10];

// address: 0x800D1A44
static int Pending;

// address: 0x800D1A48
static int Up;

// address: 0x800D1A4C
static int Down;

// address: 0x800D1A50
// size: 0x1C
static struct _G2AnimAlphaTable_Type *G2AlphaTables[7];

// address: 0x800D1A6C
static void *circWhere;

// address: 0x800D47E0
// size: 0x1000
static char circBuf[4096];

// address: 0x800D1A70
extern long VRAM_NeedToUpdateMorph;

// address: 0x800D61B8
extern short M_TrackClutUpdate;

// address: 0x800D61E0
// size: 0x1C
extern struct _BlockVramEntry_duplicate_s8 *openVramBlocks;

// address: 0x800D61DC
// size: 0x1C
extern struct _BlockVramEntry_duplicate_s8 *usedVramBlocks;

// address: 0x800D57E0
// size: 0x9D8
extern struct _BlockVramEntry_duplicate_s8 vramBlockList[90];

// address: 0x800D61BC
extern long numOfBlocksUsed;

// address: 0x800D61C0
// size: 0x1C
extern struct _BlockVramEntry_duplicate_s8 TerrainVramBlock;

// address: 0x800D1978
static int loadFromHead;

// address: 0x800D197C
static int gCurDir;

// address: 0x800D40D8
// size: 0x70
static struct _LoadQueueEntry *loadFree;

// address: 0x800D40DC
// size: 0x70
static struct _LoadQueueEntry *loadHead;

// address: 0x800D40E0
// size: 0x70
static struct _LoadQueueEntry *loadTail;

// address: 0x800D40E4
static int numLoads;

// address: 0x800DA984
// size: 0x1180
static struct _LoadQueueEntry LoadQueue[40];

// address: 0x800D19F0
static short EventAbortLine;

// address: 0x800D19F2
static short EventJustRecievedTimer;

// address: 0x800D19F4
static long MoveCodeStreamExtra;

// address: 0x800D19F8
static long CurrentEventLine;

// address: 0x800D19FC
static long EventCurrentEventIndex;

// address: 0x800D1A00
extern long WaterInUse;

// address: 0x800D1A04
extern long NumSignalsToReset;

// address: 0x800D44AC
extern long WaitingForVoiceNumber;

// address: 0x800D43CC
extern long WaitingToLoadSound;

// address: 0x800D44A8
extern long WaitingToLoadSFX;

// address: 0x800D4420
extern short MoviePlayed;

// address: 0x800D4424
extern short MovieToPlay;

// address: 0x800D40E8
// size: 0x1C
static struct Event *currentEventInstance;

// address: 0x800D40EC
// size: 0x8
static struct ScriptPCode *currentActionScript;

// address: 0x800D40F0
static short *EventAbortedPosition;

// address: 0x800D43D0
// size: 0x50
extern long eventListNumInstances[20];

// address: 0x800D44C0
// size: 0x320
extern struct _Instance *eventListArray[10][20];

// address: 0x800D40F8
// size: 0x8C
static struct WaterLevelProcess WaterLevelArray[5];

// address: 0x800D4184
// size: 0x188
static struct Level *CurrentPuzzleLevel;

// address: 0x800D4188
// size: 0x240
static struct EventTimer eventTimerArray[24];

// address: 0x800D43C8
static long numActiveEventTimers;

// address: 0x800D4428
// size: 0x80
extern struct SignalResetStruct ResetSignalArray[16];

// address: 0x800D44B0
// size: 0x10
extern struct HintSystemStruct gHintSystem;

// address: 0x800D626C
static short FireReaverFlag;

// address: 0x800D1A88
static short fx_going;

// address: 0x800D1A8C
// size: 0x78
extern struct _FXBlastringEffect *fx_blastring;

// address: 0x800D1A90
static short HUD_Captured;

// address: 0x800D6270
static short MANNA_Position;

// address: 0x800D6272
static short MANNA_Pos_vel;

// address: 0x800D6274
static short glyph_time;

// address: 0x800D6276
static short glyph_trigger;

// address: 0x800D6278
static short glyph_cost;

// address: 0x800D627A
static short HUD_Count;

// address: 0x800D627C
static short HUD_Count_Overall;

// address: 0x800D6280
static int blast_range;

// address: 0x800D6284
static int fx_radius_old;

// address: 0x800D6288
static int MANNA_Pickup_Time;

// address: 0x800D628C
static short HUD_Rotation;

// address: 0x800D628E
static short HUD_Rot_vel;

// address: 0x800D6290
static short HUD_Position;

// address: 0x800D6292
static short HUD_Pos_vel;

// address: 0x800D6294
static short HUD_State;

// address: 0x800D6296
static short HUD_Wait;

// address: 0x800D6298
// size: 0x8
static struct _SVector HUD_Cap_Pos;

// address: 0x800D62A0
// size: 0x8
static struct _SVector HUD_Cap_Vel;

// address: 0x800D62A8
static int warpDraw;

// address: 0x800D62AC
static int glowdeg;

// address: 0x800D62B0
extern int hud_warp_arrow_flash;

// address: 0x800CC45C
// size: 0x48
extern struct _MonsterStateChoice HUMAN_StateChoiceTable[6];

// address: 0x800D1A9C
// size: 0x20
extern struct _MonsterFunctionTable HUMAN_FunctionTable;

// address: 0x800CC4A4
// size: 0x8
static struct MissileGraphics MISSILE_objectTable[4];

// address: 0x800CC4D4
// size: 0x30
static struct _207fake functionChoiceTable[6];

// address: 0x800CC504
// size: 0xF8
static struct _MonsterState DefaultStateTable[31];

// address: 0x800D1B48
// size: 0x20
static struct _MonsterFunctionTable DefaultFunctionTable;

// address: 0x800CC5FC
// size: 0x30
extern struct _MonsterStateChoice SLUAGH_StateChoiceTable[4];

// address: 0x800D1B68
// size: 0x20
extern struct _MonsterFunctionTable SLUAGH_FunctionTable;

// address: 0x800CC62C
// size: 0x54
extern struct _MonsterStateChoice SOUL_StateChoiceTable[7];

// address: 0x800D1B90
// size: 0x20
extern struct _MonsterFunctionTable SOUL_FunctionTable;

// address: 0x800D1AE8
static int gNumMonsters;

// address: 0x800D1AEC
static int gNumSpectralMonsters;

// address: 0x800D1AF0
static int gNumMaterialMonsters;

// address: 0x800D1AF4
// size: 0x30
static struct RadarInfo radarDir[8];

// address: 0x800D1B24
// size: 0x8
static unsigned char radarDirIndex[8];

// address: 0x800D62B4
// size: 0xA0
static struct _MonsterVars *monsterSenseArray[40];

// address: 0x800D6354
// size: 0x28
static char monsterSensed[40];

// address: 0x800D637C
static int lastSenseFrame;

// address: 0x800D1B30
// size: 0x8
static struct burntTuneType burntTest;

// address: 0x800CC680
// size: 0x54
extern struct _MonsterStateChoice VWRAITH_StateChoiceTable[7];

// address: 0x800D1BB0
// size: 0x20
extern struct _MonsterFunctionTable VWRAITH_FunctionTable;

// address: 0x800CC6D4
// size: 0x3C
extern struct _MonsterStateChoice WORSHIP_StateChoiceTable[5];

// address: 0x800D1BD0
// size: 0x20
extern struct _MonsterFunctionTable WORSHIP_FunctionTable;

// address: 0x800CC710
// size: 0x40
static int _indexTable[16];

// address: 0x800CC750
// size: 0x100
static int _stepSizeTable[64];

// address: 0x800D6380
// size: 0x10
static struct _G2AnimChanStatusBlockPool_Type _chanStatusBlockPool;

// address: 0x800D6390
// size: 0x780
static struct _G2AnimSegValue_Type _segValues[80];

// address: 0x800D6B10
// size: 0x10
static struct _G2AnimControllerPool_Type _controllerPool;

// address: 0x800D6B20
// size: 0x10
static struct _G2AnimInterpStateBlockPool_Type _interpStateBlockPool;

// address: 0x800D6B84
// size: 0x20
extern struct PoolManagementData *poolManagementData;

// address: 0x800D6B38
// size: 0x20
static struct PoolManagementData poolManagementDataStorage;

// address: 0x800D6B58
// size: 0x28
static long timerArray[10];

// address: 0x800D6B80
static long timerIndex;

// address: 0x800D1C18
// size: 0x18
static struct _fth directionList[6];

// address: 0x800D6B30
// size: 0x8
static struct _G2AppDataVM_Type _appDataVM;

// address: 0x800D6B88
// size: 0x14
static struct __Force *ExternalForces_addr_800D6B88;

// address: 0x800D6B98
// size: 0x20
static struct MATRIX TempMat_addr_800D6B98;

// address: 0x800D1C30
extern int LoopCounter;

// address: 0x800D1C34
static int WaterStatus;

// address: 0x800D1C38
// size: 0x4
static unsigned char PickupList[4];

// address: 0x800D1C3C
// size: 0x26
static unsigned short cowlList[19];

// address: 0x800D1C64
// size: 0x60
extern struct __razController controllerList[24];

// address: 0x800D1CC4
// size: 0x50
extern struct __CannedSound cannedSound[4];

// address: 0x800D1D14
static int AlgoControlFlag;

// address: 0x800D1D18
static int ZoneDelta;

// address: 0x800D1DA4
// size: 0x4
extern struct __Idle **currentIdleList;

// address: 0x800D1DA8
extern int currentIdleListCount;

// address: 0x800D1DAC
extern int currentIdleCount;

// address: 0x800D1DB0
static int BlockCount;

// address: 0x800D1DB4
static int LastBlock;

// address: 0x800D6BC8
// size: 0x5B8
static struct __Player Raziel;

// address: 0x800D7180
// size: 0xA0
static struct RazielData *PlayerData;

// address: 0x800D7188
// size: 0x24
static struct _G2AnimInterpInfo_Type razInterpInfo[3];

// address: 0x800D71B8
// size: 0x80
static struct __FitPoint constrictData[32];

// address: 0x800D7238
// size: 0x20
static struct MATRIX TempMat_addr_800D7238;

// address: 0x800D7258
// size: 0x270
static struct GameTracker *gameTracker;

// address: 0x800D725C
static long *PadData;

// address: 0x800D7260
// size: 0x6
static struct _G2SVector3_Type ExtraRotData;

// address: 0x800D7268
// size: 0x6
static struct _G2SVector3_Type *ExtraRot;

// address: 0x800D7360
extern int ControlFlag;

// address: 0x800D7278
// size: 0x50
static struct __Force ExternalForces_addr_800D7278[4];

// address: 0x800D72C8
static int PhysicsMode;

// address: 0x800D72D8
// size: 0x78
static struct __EngagedInstance EngagedList[15];

// address: 0x800D7350
// size: 0x8
static struct _Normal Norm;

// address: 0x800D735C
extern int AutoFaceAngle;

// address: 0x800D7358
static int LastRC;

// address: 0x800D1DC4
// size: 0x8
static struct UW_ScreenXY *ScreenMorphArray;

// address: 0x800D7364
static long UW_angle;

// address: 0x800D7368
static long UW_scalex;

// address: 0x800D736C
static long UW_scalexInc;

// address: 0x800D7370
static long UW_angleInc;

// address: 0x800D1DD8
// size: 0x78
extern struct _GlobalSaveTracker *GlobalSave;

// address: 0x800D1DDC
static long numbufferedIntros;

// address: 0x800D1DE0
static int the_header_size;

// address: 0x800D1DE4
// size: 0x28
extern long SaveArraySize[10];

// address: 0x800D7374
// size: 0x10
static struct SavedInfoTracker savedInfoTracker;

// address: 0x800D7384
// size: 0x100
static struct SavedBasic *bufferSavedIntroArray[64];

// address: 0x800D1EA4
// size: 0xC
extern char monVersion[12];

// address: 0x800D1EC8
// size: 0x14
static void (*voiceCmdTbl[5])();

// address: 0x800D7488
// size: 0x128
extern struct XAVoiceTracker voiceTracker;

// address: 0x800CC874
// size: 0x8
static struct cinema_fn_table_t *the_cine_table;

// address: 0x800CC878
// size: 0x24
static struct _ObjectTracker *the_cine_tracker;

// address: 0x800DBB04
extern int StCdIntrFlag;

// address: 0x800D1EDC
// size: 0x10
static struct LocalizationHeader *LocalizationTable;

// address: 0x800D1EE0
static char **LocalStrings;

// address: 0x800D1EE4
// size: 0x2
extern struct XAVoiceListEntry *voiceList;

// address: 0x800D75B0
// size: 0x4
static enum language_t the_language;

// address: 0x800CC87C
// size: 0x30
static struct menu_sound_entry_t the_menu_sounds[6];

// address: 0x800CC8AC
// size: 0x10
static char *the_attract_movies[4];

// address: 0x800D1F90
static int MAIN_YPOS;

// address: 0x800D1F94
static int PAUSE_YPOS;

// address: 0x800D1F98
static int MAIN_XPOS;

// address: 0x800D1F9C
static int MAIN_WIDTH;

// address: 0x800D1FA0
static int PAUSE_XPOS;

// address: 0x800D1FA4
static int PAUSE_WIDTH;

// address: 0x800D1FA8
static int LINESKIP;

// address: 0x800D1FAC
static int ITEMSKIP;

// address: 0x800D1FB0
static int hack_reset_attract;

// address: 0x800D1FB4
static int StartGameFading;

// address: 0x800D1FB8
static int hack_attract;

// address: 0x800D1FBC
static int hack_attract_movie;

// address: 4
extern long *mainMenuScreen;

// address: 0x800D2004
// size: 0x80
static struct menuface_t MenuFaces[8];

// address: 0x800D2084
static int hack_initialized;

// address: 0x800D75B8
// size: 0x10
static struct _ButtonTexture *FaceButtons;

// address: 36
// size: 0x24
extern struct mcmenu_table_t the_mcmenu_table;

// address: 0x800DBB08
// size: 0x200
static struct _216fake gMcmenu;

// address: 0x800DBD08
// size: 0x10
extern struct memcard_t gMemcard;

// path: C:\kain2\game\PLAYER.C
// address: 0x80012D54
// line start: 25
// line end:   29
void PLAYER_TurnHead(struct _Instance *instance, short *rotx, short *rotz, struct GameTracker *gameTracker);

// path: C:\kain2\game\PLAYER.C
// address: 0x80012DD4
// line start: 33
// line end:   37
long PLAYER_OkToLookAround(struct _Instance *instance);

// path: C:\kain2\game\PLAYER.C
// address: 0x80012E20
// line start: 41
// line end:   45
void PLAYER_SetLookAround(struct _Instance *instance);

// path: C:\kain2\game\PLAYER.C
// address: 0x80012E6C
// line start: 49
// line end:   53
void PLAYER_ReSetLookAround(struct _Instance *instance);

// path: C:\kain2\game\DEBUG.C
// address: 0x80012EB8
// line start: 1224
// line end:   1225
void DEBUG_SetGameTime(long *var);

// path: C:\kain2\game\DEBUG.C
// address: 0x80012EDC
// line start: 1229
// line end:   1232
void DEBUG_KeepGameTime(long *var)
{ //line: 1
	// address: -16
	auto long curTOD;
} //line: 1


// path: C:\kain2\game\DEBUG.C
// address: 0x80012F04
// line start: 1235
// line end:   1244
void DEBUG_UpdateFog(long *var)
{ //line: 1
	// register: $v0
	// size: 0x40
	register struct _StreamUnit *currentUnit;
} //line: 1


// path: C:\kain2\game\DEBUG.C
// address: 0x80012F8C
// line start: 1250
// line end:   1251
void DEBUG_UpdateHealth(long *var);

// path: C:\kain2\game\DEBUG.C
// address: 0x80012FB0
// line start: 1255
// line end:   1256
void DEBUG_UpdateMana(long *var);

// path: C:\kain2\game\DEBUG.C
// address: 0x80012FD4
// line start: 1264
// line end:   1267
void DEBUG_FillUpHealth(long *var)
{ //line: 1
} //line: 1


// path: C:\kain2\game\DEBUG.C
// address: 0x80012FFC
// line start: 1270
// line end:   1280
void DEBUG_FogLoad()
{ //line: 1
	// register: $v0
	// size: 0x40
	register struct _StreamUnit *currentUnit;
} //line: 1


// path: C:\kain2\game\DEBUG.C
// address: 0x80013064
// line start: 1283
// line end:   1289
void DEBUG_FogRestore();

// path: C:\kain2\game\DEBUG.C
// address: 0x800130AC
// line start: 1298
// line end:   1308
void DEBUG_SendCinematicSwitch();

// path: C:\kain2\game\DEBUG.C
// address: 0x80013108
// line start: 1312
// line end:   1313
void DEBUG_SendMoveTo();

// path: C:\kain2\game\DEBUG.C
// address: 0x8001317C
// line start: 1463
// line end:   1583
void process_cheat_codes(struct GameTracker *gt, long *ctrl)
{ //line: 1
	// register: $s0
	register int i;
	// register: $a0
	// size: 0x10
	register struct GameCheat *cheat;
	// register: $s2
	register unsigned long padPress;
	// register: $s1
	register unsigned long padOn;
	{ //line: 13
		// register: $s3
		register int cheatTriggered;
		{ //line: 16
			// register: $v1
			register unsigned long keyMask;
			{ //line: 36
			} //line: 110
		} //line: 112
	} //line: 118
} //line: 120


// path: C:\kain2\game\DEBUG.C
// address: 0x80013444
// line start: 1590
// line end:   1654
void DEBUG_Process(struct GameTracker *gameTracker)
{ //line: 1
	// register: $s3
	register long *controlCommand;
	// register: $s2
	register long oldFlags;
	// register: $s1
	register long oldFlags2;
} //line: 65


// path: C:\kain2\game\DEBUG.C
// address: 0x800135F0
// line start: 1665
// line end:   1682
void DEBUG_Draw(struct GameTracker *gameTracker, unsigned long **ot);

// path: C:\kain2\game\DEBUG.C
// address: 0x80013638
// line start: 1688
// line end:   1699
long DEBUG_MenuCountLength(struct DebugMenuLine *menu)
{ //line: 2
	// register: $v1
	register int length;
	// register: $a0
	// size: 0x18
	register struct DebugMenuLine *curLine;
} //line: 11


// path: C:\kain2\game\DEBUG.C
// address: 0x80013664
// line start: 1702
// line end:   1731
void DEBUG_ExitMenus()
{ //line: 1
} //line: 30


// path: C:\kain2\game\DEBUG.C
// address: 0x800136FC
// size: 0x18
// line start: 1738
// line end:   1742
struct DebugMenuLine *get_last_menu_line(struct DebugMenuLine *line);

// path: C:\kain2\game\DEBUG.C
// address: 0x8001372C
// line start: 1746
// line end:   1752
int num_menu_items(struct DebugMenuLine *menu)
{ //line: 2
	// register: $a1
	register int nitems;
} //line: 6


// path: C:\kain2\game\DEBUG.C
// address: 0x8001375C
// line start: 1755
// line end:   1773
void maybe_change_menu_choice(struct GameTracker *gt, struct DebugMenuLine *menu)
{ //line: 1
	// register: $s0
	register long *command;
	// register: $s1
	register int choice;
	// register: $a0
	register int nitems;
	// register: $v1
	register int incr;
} //line: 19


// path: C:\kain2\game\DEBUG.C
// address: 0x80013824
// line start: 1778
// line end:   1800
void handle_line_type_long(struct GameTracker *gt, struct DebugMenuLine *line)
{ //line: 1
	// register: $v1
	register long *command;
	{ //line: 5
		// register: $a2
		register int incr;
	} //line: 22
} //line: 23


// path: C:\kain2\game\DEBUG.C
// address: 0x80013910
// line start: 1803
// line end:   1811
void handle_line_type_bit(struct GameTracker *gt, struct DebugMenuLine *line)
{ //line: 2
} //line: 9


// path: C:\kain2\game\DEBUG.C
// address: 0x80013968
// line start: 1814
// line end:   1823
void handle_line_type_action(struct GameTracker *gt, struct DebugMenuLine *line)
{ //line: 1
	// register: $v0
	register int ok;
} //line: 10


// path: C:\kain2\game\DEBUG.C
// address: 0x800139A4
// line start: 1826
// line end:   1837
void handle_line_type_action_with_line(struct GameTracker *gt, struct DebugMenuLine *line)
{ //line: 1
	// register: $a2
	// size: 0x4
	register enum option_ctrl_t ctrl;
} //line: 12


// path: C:\kain2\game\DEBUG.C
// address: 0x80013A08
// line start: 1842
// line end:   1858
void handle_line_type_menu(struct GameTracker *gt, struct DebugMenuLine *line)
{ //line: 1
	// register: $v0
	register int ok;
} //line: 17


// path: C:\kain2\game\DEBUG.C
// address: 0x80013A88
// line start: 1878
// line end:   1887
void process_menu_line(struct GameTracker *gt, struct DebugMenuLine *menu)
{ //line: 1
	// register: $a1
	// size: 0x18
	register struct DebugMenuLine *line;
	{ //line: 4
		// register: $v1
		// size: 0x8
		register struct debug_dispatch_t *dispatch;
	} //line: 9
} //line: 10


// path: C:\kain2\game\DEBUG.C
// address: 0x80013AEC
// line start: 1892
// line end:   1896
int pre_process_functions(struct GameTracker *gt, struct DebugMenuLine *menu);

// path: C:\kain2\game\DEBUG.C
// address: 0x80013B14
// line start: 1900
// line end:   1920
void post_process_functions(struct GameTracker *gt, struct DebugMenuLine *menu);

// path: C:\kain2\game\DEBUG.C
// address: 0x80013BB8
// line start: 1955
// line end:   1957
void set_debug_leading();

// path: C:\kain2\game\DEBUG.C
// address: 0x80013BD0
// line start: 1961
// line end:   1964
void set_user_leading();

// path: C:\kain2\game\DEBUG.C
// address: 0x80013BE8
// line start: 1981
// line end:   1982
int isdigit(char c);

// path: C:\kain2\game\DEBUG.C
// address: 0x80013BF8
// line start: 1986
// line end:   2013
void adjust_format(char *ctrl, struct debug_format_t *fmt)
{ //line: 1
	{ //line: 5
		// register: $s0
		register char *p;
		// register: $s2
		register int x;
		// register: $s3
		register int y;
	} //line: 19
} //line: 28


// path: C:\kain2\game\DEBUG.C
// address: 0x80013DA0
// line start: 2016
// line end:   2020
char *find_eol(char *text);

// path: C:\kain2\game\DEBUG.C
// address: 0x80013DD8
// line start: 2024
// line end:   2051
void draw_menu_item(struct GameTracker *gt, struct debug_format_t *fmt, char *text)
{ //line: 2
	{ //line: 3
		// register: $s1
		register char *eol;
		// register: $s3
		register char c;
		{ //line: 7
		} //line: 7
	} //line: 23
} //line: 27


// path: C:\kain2\game\DEBUG.C
// address: 0x80013ED8
// line start: 2054
// line end:   2112
void draw_menu(struct GameTracker *gt, struct DebugMenuLine *menu)
{ //line: 1
	// address: -56
	// size: 0xC
	auto struct debug_format_t fmt;
	// register: $s3
	register int i;
	{ //line: 14
		// register: $s2
		register int xpos;
		// register: $s1
		register int ypos;
	} //line: 58
} //line: 59


// path: C:\kain2\game\DEBUG.C
// address: 0x800140E4
// line start: 2115
// line end:   2138
void DEBUG_Menu(struct GameTracker *gt)
{ //line: 1
	// register: $s0
	// size: 0x18
	register struct DebugMenuLine *menu;
	// register: $s2
	register int choice;
} //line: 24


// path: C:\kain2\game\DEBUG.C
// address: 0x800141F8
// line start: 2154
// line end:   2476
void DEBUG_DisplayStatus(struct GameTracker *gameTracker)
{ //line: 1
	{ //line: 260
		// register: $a0
		register int deg;
	} //line: 273
	{ //line: 279
		// address: -24
		auto long numberInQueue;
	} //line: 286
} //line: 323


// path: C:\kain2\game\DEBUG.C
// address: 0x80014460
// line start: 3553
// line end:   3672
void DEBUG_DrawShrinkCels(unsigned long **polyAddr);

// path: C:\kain2\game\DEBUG.C
// address: 0x80014468
// line start: 3725
// line end:   3727
void DEBUG_ContinueGame();

// path: C:\kain2\game\DEBUG.C
// address: 0x80014480
// line start: 3746
// line end:   3752
void DEBUG_ExitGame();

// path: C:\kain2\game\DEBUG.C
// address: 0x800144BC
// line start: 3757
// line end:   3773
void DEBUG_ReloadCurrentLevel();

// path: C:\kain2\game\DEBUG.C
// address: 0x80014520
// line start: 3777
// line end:   3812
void DEBUG_LevelSelectNew()
{ //line: 1
	// register: $s3
	register char *name;
	// register: $s2
	register short number;
	// register: $s0
	register char *p;
	// register: $s1
	register char saveChar;
} //line: 35


// path: C:\kain2\game\DEBUG.C
// address: 0x8001461C
// line start: 3924
// line end:   3925
void DEBUG_SetViewVram();

// path: C:\kain2\game\DEBUG.C
// address: 0x8001462C
// line start: 3929
// line end:   3931
void DEBUG_EndViewVram(struct GameTracker *gameTracker);

// path: C:\kain2\game\DEBUG.C
// address: 0x8001467C
// line start: 3935
// line end:   3967
void DEBUG_ViewVram(struct GameTracker *gameTracker)
{ //line: 1
	// register: $a0
	register long *controlCommand;
	// address: 3580
	static int xPos;
	// address: 3584
	static int yPos;
} //line: 27


// path: C:\kain2\game\DEBUG.C
// address: 0x8001479C
// line start: 3973
// line end:   4022
void DEBUG_CaptureScreen(struct GameTracker *gameTracker);

// path: C:\kain2\game\DEBUG.C
// address: 0x800147A4
// line start: 4035
// line end:   4070
void DEBUG_PageFlip()
{ //line: 1
	// address: -32
	// size: 0x18
	auto struct POLY_F4 poly;
	// register: $s0
	register unsigned long **drawot;
} //line: 1


// path: C:\kain2\game\DEBUG.C
// address: 0x800148AC
// line start: 4079
// line end:   4100
void DEBUG_FatalError(char *fmt)
{ //line: 1
	// address: -264
	// size: 0x100
	auto char msg[256];
} //line: 1


// path: C:\kain2\game\DEBUG.C
// address: 0x80014900
// line start: 4436
// line end:   4522
void DEBUG_ProcessSecondController(struct GameTracker *gameTracker);

// path: C:\kain2\game\DEBUG.C
// address: 0x80014908
// line start: 4527
// line end:   4564
void DEBUG_ProcessCheat(struct GameTracker *gameTracker)
{ //line: 1
	// register: $s2
	register long angleRelCamera;
	{ //line: 27
		// address: -80
		// size: 0x8
		auto struct SVECTOR v;
		// address: -72
		// size: 0x10
		auto struct VECTOR dv;
		// address: -56
		// size: 0x20
		auto struct MATRIX rotate_mat;
	} //line: 37
} //line: 38


// path: C:\kain2\game\DEBUG.C
// address: 0x80014A7C
// line start: 4582
// line end:   4595
void DEBUG_DoAreaProtection()
{ //line: 3
} //line: 14


// path: C:\kain2\game\DRAW.C
// address: 0x8002A910
// line start: 61
// line end:   64
void fDRAW_SPLIT_INTPL_XYZ(struct _SVector *newVertex, struct _SVector *pvb, struct _SVector *pvc);

// path: C:\kain2\game\DRAW.C
// address: 0x8002A98C
// line start: 454
// line end:   470
void DRAW_InitShadow()
{ //line: 1
	// register: $s3
	register int n;
	// register: $s4
	register int inc;
	// register: $s2
	register int deg;
} //line: 14


// path: C:\kain2\game\DRAW.C
// address: 0x8002AA54
// line start: 478
// line end:   537
unsigned long *DRAW_DrawShadow(struct _PrimPool *primPool, struct _Model *model, unsigned long **ot, long fadeValue)
{ //line: 1
	// register: $a1
	// size: 0x20
	register struct POLY_G3_SEMITRANS *sg3;
	// address: -24
	// size: 0x8
	auto struct SVECTOR zero_vertex;
	// address: -8
	auto long clip;
	// register: $t0
	register long n;
	// address: -16
	auto long p;
	// address: -4
	auto long otz;
	// address: -12
	auto unsigned long color;
} //line: 60


// path: C:\kain2\game\DRAW.C
// address: 0x8002AC14
// line start: 3208
// line end:   3227
void DRAW_FlatQuad(struct CVECTOR *color, short x0, short y0, short x1, int y1, int x2, int y2, int x3, int y3, struct _PrimPool *primPool, unsigned long **ot)
{ //line: 1
	// register: $t0
	register unsigned long *prim;
} //line: 20


// path: C:\kain2\game\DRAW.C
// address: 0x8002ACBC
// line start: 3245
// line end:   3271
void DRAW_TranslucentQuad(short x0, short y0, short x1, short y1, int x2, int y2, int x3, int y3, int r, int g, int b, int abr, struct _PrimPool *primPool, unsigned long **ot)
{ //line: 1
	// register: $t0
	register unsigned long *prim;
} //line: 27


// path: C:\kain2\game\DRAW.C
// address: 0x8002ADA0
// line start: 3515
// line end:   3566
void DRAW_DrawButton(struct _ButtonTexture *button, short x, short y, unsigned long **ot)
{ //line: 1
	// register: $fp
	// size: 0x1770C
	register struct _PrimPool *primPool;
	// register: $s4
	register unsigned long *prim;
	// register: $s0
	register short w;
	// register: $s3
	register short h;
	// register: $s2
	register short offsetx;
	// register: $s1
	register short offsety;
} //line: 52


// path: C:\kain2\game\DRAW.C
// address: 0x8002AF08
// line start: 3573
// line end:   3620
void DRAW_LoadButton(long *addr, struct _ButtonTexture *button)
{ //line: 1
	// address: -32
	// size: 0x8
	auto struct RECT vramRect;
	// register: $s3
	register long *paletteAddr;
	// register: $s2
	register short paletteW;
	// register: $s4
	register short paletteH;
} //line: 47


// path: C:\kain2\game\DRAW.C
// address: 0x8002B0A8
// line start: 3636
// line end:   3637
void DRAW_FreeButton(struct _ButtonTexture *button);

// path: C:\kain2\game\DRAW.C
// address: 0x8002B0CC
// line start: 3649
// line end:   3711
void DRAW_RingLine(struct _PrimPool *primPool, unsigned long **ot, long color)
{ //line: 1
	// register: $a3
	register unsigned long *prim;
	// address: -24
	auto long flag;
	// address: -20
	auto long clip;
	// address: -16
	auto long z0;
	// address: -12
	auto long z1;
	// address: -8
	auto long z2;
	// register: $a0
	register long otz;
	// address: -4
	auto long p;
} //line: 63


// path: C:\kain2\game\DRAW.C
// address: 0x8002B1FC
// line start: 3714
// line end:   3725
void DRAW_RingPoint(struct _PrimPool *primPool, unsigned long **ot, long color, struct _SVector *vel, struct _SVector *acc)
{ //line: 1
	// address: -24
	// size: 0x8
	auto struct _SVector outpoint;
} //line: 12


// path: C:\kain2\game\DRAW.C
// address: 0x8002B30C
// line start: 3736
// line end:   3837
unsigned long *DRAW_DrawRingPoints(struct _Model *model, struct _VertexPool *vertexPool, struct MATRIX *pcTransform, struct _PrimPool *primPool, unsigned long **ot, long color, int ring_type)
{ //line: 1
	// register: $s1
	// size: 0xC
	register struct _MFace *mface;
	// register: $s4
	// size: 0xC
	register struct _MFace *endMFace;
	// register: $s3
	// size: 0x8
	register struct _PVertex *pvertexList;
	// address: -96
	// size: 0xC
	auto struct _SVector *pvertex[3];
	// address: -80
	// size: 0x18
	auto struct _SVector newVertex[3];
	// register: $a0
	register long outcode;
	// register: $v1
	register int first;
	// register: $a0
	register int second;
	// address: -56
	// size: 0x8
	auto struct _SVector vel;
	// address: -48
	// size: 0x8
	auto struct _SVector acc;
} //line: 101


// path: C:\kain2\game\DRAW.C
// address: 0x8002B628
// line start: 3879
// line end:   3921
void DRAW_GlowQuad(struct _PrimPool *primPool, unsigned long **ot, long otz, long color, struct _Vector *v0, struct _Vector *v1, struct _Vector *v2, struct _Vector *v3)
{ //line: 1
	// register: $t1
	register unsigned long *prim;
	// register: $t0
	// size: 0x28
	register struct _POLY_NG4 *sg4;
} //line: 43


// path: C:\kain2\game\DRAW.C
// address: 0x8002B88C
// line start: 3935
// line end:   4027
void DRAW_CreateAGlowingCircle(struct _Vector *f1, long z, struct _PrimPool *primPool, unsigned long **ot, long otz, long color, long w, long h, long angle)
{ //line: 1
	// register: $t0
	register long hold;
	// register: $s3
	register long x;
	// register: $v0
	register long y;
	// register: $a2
	register long diagScale;
	// address: -232
	// size: 0xC
	auto struct _Vector width;
	// address: -216
	// size: 0xC
	auto struct _Vector height;
	// address: -200
	// size: 0xC
	auto struct _Vector diag1;
	// address: -184
	// size: 0xC
	auto struct _Vector diag2;
	// address: -168
	// size: 0x6C
	auto struct _Vector points[9];
	{ //line: 14
		// register: $v0
		register long _x0;
		// register: $v1
		register long _y0;
		// register: $t3
		register long _z0;
		// register: $s1
		register long _x1;
		// register: $s7
		register long _y1;
		// register: $s0
		register long _z1;
		// register: $t6
		// size: 0xC
		register struct _Vector *_v;
		// address: -48
		// size: 0xC
		auto struct _Vector *_v1;
	} //line: 14
	{ //line: 14
		// register: $v0
		register long _x0;
		// register: $v1
		register long _y0;
		// register: $t3
		register long _z0;
		// register: $s4
		register long _y1;
		// register: $s5
		register long _z1;
		// register: $s2
		// size: 0xC
		register struct _Vector *_v1;
	} //line: 14
	{ //line: 14
		// register: $v0
		register long _x0;
		// register: $v1
		register long _y0;
		// register: $t3
		register long _z0;
		// register: $a0
		register long _x1;
		// register: $a0
		register long _y1;
		// register: $a1
		register long _z1;
	} //line: 14
	{ //line: 14
		// register: $v1
		register long _x0;
		// register: $a0
		register long _y0;
		// register: $t2
		register long _z0;
		// register: $t4
		// size: 0xC
		register struct _Vector *_v;
	} //line: 14
	{ //line: 14
		// register: $v1
		register long _x0;
		// register: $a0
		register long _y0;
		// register: $t2
		register long _z0;
	} //line: 14
	{ //line: 14
		// register: $v1
		register long _x0;
		// register: $a0
		register long _y0;
		// register: $t2
		register long _z0;
		// register: $v0
		register long _x1;
		// register: $v0
		register long _y1;
		// register: $a1
		register long _z1;
	} //line: 14
	{ //line: 14
		// register: $v0
		register long _x0;
		// register: $v1
		register long _y0;
		// register: $t0
		register long _z0;
		// register: $t1
		register long _y1;
		// register: $t3
		// size: 0xC
		register struct _Vector *_v;
	} //line: 14
	{ //line: 14
		// register: $v0
		register long _x0;
		// register: $v1
		register long _y0;
		// register: $t0
		register long _z0;
		// register: $s6
		// size: 0xC
		register struct _Vector *_v;
	} //line: 14
	{ //line: 14
		// register: $v0
		register long _x0;
		// register: $v1
		register long _y0;
		// register: $t0
		register long _z0;
		// register: $t1
		register long _y1;
		// address: -52
		// size: 0xC
		auto struct _Vector *_v;
	} //line: 14
	{ //line: 14
		// register: $v0
		register long _x0;
		// register: $v1
		register long _y0;
		// register: $t0
		register long _z0;
		// register: $s4
		// size: 0xC
		register struct _Vector *_v;
	} //line: 14
	{ //line: 14
		// register: $t0
		register long _x0;
		// register: $t2
		register long _y0;
		// register: $t1
		register long _z0;
		// register: $v0
		register long _x1;
		// register: $v0
		register long _y1;
		// register: $v1
		register long _z1;
		// register: $s5
		// size: 0xC
		register struct _Vector *_v;
	} //line: 14
	{ //line: 14
		// register: $t0
		register long _x0;
		// register: $t2
		register long _y0;
		// register: $t1
		register long _z0;
		// register: $v0
		register long _x1;
		// register: $v0
		register long _y1;
		// register: $v1
		register long _z1;
		// register: $s3
		// size: 0xC
		register struct _Vector *_v;
	} //line: 14
	{ //line: 14
		// register: $t0
		register long _x0;
		// register: $t2
		register long _y0;
		// register: $t1
		register long _z0;
		// register: $v0
		register long _x1;
		// register: $v0
		register long _y1;
		// register: $v1
		register long _z1;
		// register: $s2
		// size: 0xC
		register struct _Vector *_v;
	} //line: 14
	{ //line: 14
		// register: $t0
		register long _x0;
		// register: $t2
		register long _y0;
		// register: $t1
		register long _z0;
		// register: $v0
		register long _x1;
		// register: $v0
		register long _y1;
		// register: $v1
		register long _z1;
		// register: $s1
		// size: 0xC
		register struct _Vector *_v;
	} //line: 14
} //line: 93


// path: C:\kain2\game\DRAW.C
// address: 0x8002BE38
// line start: 4038
// line end:   4078
unsigned long *DRAW_DrawGlowPoints2(struct _Instance *instance, long seg1, long seg2, struct _PrimPool *primPool, unsigned long **ot, long color, long height)
{ //line: 1
	// address: -88
	// size: 0x6
	auto struct _Position glowPoints1;
	// address: -80
	// size: 0x6
	auto struct _Position glowPoints2;
	// address: -72
	// size: 0xC
	auto struct _Vector f1;
	// address: -56
	// size: 0xC
	auto struct _Vector f2;
	// address: -40
	// size: 0xC
	auto struct _Vector f3;
	// register: $s2
	register long otz;
	// register: $s0
	register long otz2;
	// register: $a1
	register long z;
	// register: $s1
	register long length;
	// register: $v1
	register long angle;
} //line: 41


// path: C:\kain2\game\DRAW.C
// address: 0x8002BFA4
// line start: 4089
// line end:   4115
unsigned long *DRAW_DrawGlowPoint(struct _Instance *instance, long seg1, struct _PrimPool *primPool, unsigned long **ot, long color, int width, int height)
{ //line: 1
	// address: -48
	// size: 0x6
	auto struct _Position glowPoints1;
	// address: -40
	// size: 0xC
	auto struct _Vector f1;
	// register: $t0
	register long otz;
	// register: $a1
	register long z;
	// register: $s2
	register long angle;
} //line: 26


// path: C:\kain2\game\DRAW.C
// address: 0x8002C074
// line start: 4192
// line end:   4408
int DRAW_DisplayTFace_zclipped_C(struct SVECTOR *vertex0, struct SVECTOR *vertex1, struct SVECTOR *vertex2, struct UVTYPE *uv0, struct UVTYPE *uv1, struct UVTYPE *uv2, long color0, long color1, long color2, struct _PrimPool *primPool, unsigned long **ot, int ndiv)
{ //line: 1
	// register: $t2
	// size: 0xD4
	register struct SP_SUBDIV_STRUCT *sp;
	// register: $s3
	register int maxz;
	// register: $a1
	register int point;
	// register: $t7
	register int n;
	// register: $t9
	// size: 0x34
	register struct POLY_GT4 *prim;
	// register: $t8
	register long *ptr;
	// register: $s0
	register long clip;
	{ //line: 41
		// register: $v1
		register int next_point_in_vv;
		// register: $t1
		register int current_point_in_vv;
		// register: $t6
		register int next;
		{ //line: 54
			// register: $a0
			register long clip_tmp;
			// address: -48
			auto long zval;
			// register: $v0
			// size: 0x8
			register struct SVECTOR *tmpptr;
		} //line: 80
		{ //line: 84
			// register: $v1
			register long zn;
			// register: $a0
			register long znext;
			// register: $t1
			register long interp1;
			// register: $a2
			register long interp2;
			// register: $v1
			register long clip_tmp;
			{ //line: 88
				// register: $v1
				register long x;
				// register: $v1
				register long y;
			} //line: 99
		} //line: 113
	} //line: 114
	{ //line: 135
		// register: $t0
		register int next;
		// register: $t3
		register int flag;
	} //line: 166
	{ //line: 169
		// address: -44
		auto long opz;
	} //line: 184
	{ //line: 195
		// register: $a3
		register long temp1;
		// register: $t0
		register long temp2;
		// register: $t1
		register long temp3;
	} //line: 216
} //line: 217


// path: C:\kain2\game\DRAW.C
// address: 0x8002C8B4
// line start: 4420
// line end:   4555
long *DRAW_Zclip_subdiv(struct POLY_GT3 *texture, unsigned long **ot, int ndiv)
{ //line: 1
	// register: $s7
	// size: 0xD4
	register struct SP_SUBDIV_STRUCT *sp;
	// register: $fp
	// size: 0x1770C
	register struct _PrimPool *primPool;
	// register: $s0
	register int clip;
} //line: 136


// path: C:\kain2\game\DRAW.C
// address: 0x8002CDB8
// line start: 4569
// line end:   4598
void DRAW_LoadingMessage()
{ //line: 3
	// register: $s0
	register unsigned long **drawot;
} //line: 14


// path: C:\kain2\game\FONT.C
// address: 0x8002CED4
// line start: 58
// line end:   71
void FONT_MakeSpecialFogClut(int x, int y)
{ //line: 1
	// register: $a0
	register int n;
	// address: -48
	// size: 0x20
	auto unsigned short cl[16];
	// address: -16
	// size: 0x8
	auto struct RECT myrect;
} //line: 11


// path: C:\kain2\game\FONT.C
// address: 0x8002CF58
// line start: 91
// line end:   134
void FONT_Init()
{ //line: 1
	// register: $s0
	register unsigned long *timAddr;
	// address: -16
	auto short x;
	// address: -14
	auto short y;
} //line: 35


// path: C:\kain2\game\FONT.C
// address: 0x8002D078
// line start: 137
// line end:   146
void FONT_ReloadFont()
{ //line: 1
	// register: $s0
	register unsigned long *timAddr;
} //line: 1


// path: C:\kain2\game\FONT.C
// address: 0x8002D0DC
// line start: 149
// line end:   157
void FONT_DrawChar(struct FontChar *fontChar)
{ //line: 1
	// register: $v1
	register char c;
	// register: $a1
	register long x;
	// register: $a2
	register long y;
} //line: 1


// path: C:\kain2\game\FONT.C
// address: 0x8002D110
// line start: 471
// line end:   536
long FONT_Get2DImageIndex(unsigned char c);

// path: C:\kain2\game\FONT.C
// address: 0x8002D128
// line start: 558
// line end:   656
void drawChar2DPoly(long fpi, long x, long y)
{ //line: 1
	// register: $s1
	register unsigned long **drawOT;
	// register: $a3
	// size: 0x28
	register struct POLY_FT4 *textPoly;
	// register: $t8
	register long xl;
	// register: $s0
	register long xr;
	// register: $a0
	register long yt;
	// register: $a2
	register long yb;
	// register: $a0
	register long w;
	// register: $v0
	register long h;
	// register: $v1
	register int u;
	// register: $a0
	register int v;
	// register: $t3
	register int u0;
	// register: $t2
	register int v0;
	// register: $t5
	register int u1;
	// register: $t1
	register int v1;
	// register: $t7
	register int u2;
	// register: $t0
	register int v2;
	// register: $t6
	register int u3;
	// register: $t4
	register int v3;
	{ //line: 48
	} //line: 57
	{ //line: 60
		// register: $v0
		register int holdu;
		// register: $v1
		register int holdv;
	} //line: 69
	{ //line: 78
		// register: $v0
		// size: 0xC
		register struct font_color_t *color;
	} //line: 81
} //line: 82


// path: C:\kain2\game\FONT.C
// address: 0x8002D328
// line start: 660
// line end:   701
void FONT_DrawChar2D(unsigned char c, long x, long y)
{ //line: 1
	// register: $s0
	register long w;
	// register: $s6
	register long h;
	// register: $a1
	register long w1;
	// register: $s1
	register long w2;
	// register: $s2
	register long w3;
	// register: $a1
	register long xoff;
	// register: $a2
	register long yoff;
	// register: $a0
	register int i1;
	// register: $s4
	register int i2;
	// register: $s5
	register int i3;
} //line: 42


// path: C:\kain2\game\FONT.C
// address: 0x8002D55C
// line start: 704
// line end:   745
long FONT_CharSpacing(char c, long fontXSize)
{ //line: 1
	// register: $a0
	register long index;
	// register: $v1
	register long w;
	// register: $a2
	register long w1;
	// register: $a1
	register long w2;
	// register: $v1
	register long w3;
	// register: $v1
	register char i1;
	// register: $a3
	register char i2;
	// register: $t1
	register char i3;
	{ //line: 13
		// register: $a0
		register int holdw;
		// register: $a0
		register int holdw2;
	} //line: 37
} //line: 42


// path: C:\kain2\game\FONT.C
// address: 0x8002D6A0
// line start: 1059
// line end:   1078
void FONT_AddCharToBuffer(char c, long x, long y)
{ //line: 1
	// register: $a0
	// size: 0x6
	register struct FontChar *fontChar;
} //line: 20


// path: C:\kain2\game\FONT.C
// address: 0x8002D70C
// line start: 1096
// line end:   1112
void FONT_Print(char *fmt)
{ //line: 1
	// register: $v1
	register char *hold;
	// register: $s0
	register void *ap;
} //line: 15


// path: C:\kain2\game\FONT.C
// address: 0x8002D79C
// line start: 1115
// line end:   1121
void FONT_Print2(char *fmt)
{ //line: 1
	// register: $s0
	register void *ap;
} //line: 1


// path: C:\kain2\game\FONT.C
// address: 0x8002D7EC
// line start: 1168
// line end:   1176
int FONT_GetStringWidth(char *str)
{ //line: 1
	// register: $s1
	register int w;
	// register: $s2
	register int len;
	// register: $s0
	register int i;
} //line: 8


// path: C:\kain2\game\FONT.C
// address: 0x8002D860
// line start: 1212
// line end:   1238
void FONT_Flush()
{ //line: 1
	// register: $s1
	register long i;
	// register: $s0
	// size: 0x6
	register struct FontChar *fontChar;
} //line: 27


// path: C:\kain2\game\FONT.C
// address: 0x8002D8F0
// line start: 1241
// line end:   1250
void FONT_SetCursor(short x, short y);

// path: C:\kain2\game\FONT.C
// address: 0x8002D910
// line start: 1280
// line end:   1333
void FONT_VaReallyPrint(char *fmt, void *ap)
{ //line: 1
	// register: $s2
	register char *p;
	// register: $s3
	register long *xpos;
	// register: $s4
	register long *ypos;
	{ //line: 22
		// register: $s0
		register unsigned char w;
		// register: $s1
		register unsigned char h;
	} //line: 24
} //line: 54


// path: C:\kain2\game\FONT.C
// address: 0x8002DAA0
// line start: 1336
// line end:   1339
void FONT_FontPrintCentered(char *text, long y);

// path: C:\kain2\game\FONT.C
// address: 0x8002DAF8
// line start: 1343
// line end:   1345
void FONT_SetColorIndex(int color);

// path: C:\kain2\game\FONT.C
// address: 0x8002DB04
// line start: 1349
// line end:   1357
void FONT_SetColorIndexCol(int color, int r, int g, int b)
{ //line: 2
	// register: $v0
	// size: 0xC
	register struct font_color_t *fcol;
} //line: 5


// path: C:\kain2\game\GAMEPAD.C
// address: 0x80031244
// line start: 55
// line end:   232
void GAMEPAD_Commands(long (*command)[5], long (*data)[5], long pad)
{ //line: 1
	// register: $t2
	register long analogX;
	// register: $t1
	register long analogY;
	// size: 0x4
	static short lastPad[2];
} //line: 178


// path: C:\kain2\game\GAMEPAD.C
// address: 0x80031600
// line start: 258
// line end:   259
int GAMEPAD_ControllerIsDualShock();

// path: C:\kain2\game\GAMEPAD.C
// address: 0x8003160C
// line start: 263
// line end:   264
int GAMEPAD_DualShockEnabled();

// path: C:\kain2\game\GAMEPAD.C
// address: 0x80031618
// line start: 268
// line end:   273
void GAMEPAD_DisableDualShock();

// path: C:\kain2\game\GAMEPAD.C
// address: 0x80031658
// line start: 278
// line end:   280
void GAMEPAD_EnableDualShock();

// path: C:\kain2\game\GAMEPAD.C
// address: 0x8003166C
// line start: 285
// line end:   326
void GAMEPAD_HandleDualShock()
{ //line: 1
	// register: $s3
	register int decrement_amount;
	{ //line: 10
		// register: $s0
		register int timeout;
	} //line: 17
} //line: 42


// path: C:\kain2\game\GAMEPAD.C
// address: 0x80031764
// line start: 333
// line end:   344
void GAMEPAD_Shock(int motor0_speed, int motor0_time, int motor1_speed, int motor1_time);

// path: C:\kain2\game\GAMEPAD.C
// address: 0x800317A8
// line start: 349
// line end:   358
void GAMEPAD_Shock0(int motor0_speed, int motor0_time);

// path: C:\kain2\game\GAMEPAD.C
// address: 0x800317E4
// line start: 363
// line end:   372
void GAMEPAD_Shock1(int motor1_speed, int motor1_time);

// path: C:\kain2\game\GAMEPAD.C
// address: 0x80031820
// line start: 377
// line end:   433
void GAMEPAD_Detect()
{ //line: 1
	// register: $v1
	register int padState;
	// register: $s0
	register int count;
	// register: $s1
	register int count1;
} //line: 55


// path: C:\kain2\game\GAMEPAD.C
// address: 0x80031928
// line start: 436
// line end:   471
void GAMEPAD_Init();

// path: C:\kain2\game\GAMEPAD.C
// address: 0x800319B0
// line start: 476
// line end:   547
void PSXPAD_TranslateData(long *data, unsigned short padData, unsigned short lastData)
{ //line: 1
	// register: $t1
	register int i;
	// address: -64
	// size: 0x40
	auto struct JoypadMappingStruct table[16];
	{ //line: 55
		// register: $a3
		register unsigned short padButton;
		// register: $v1
		register unsigned short logicalButton;
	} //line: 71
} //line: 72


// path: C:\kain2\game\GAMEPAD.C
// address: 0x80031AD4
// line start: 553
// line end:   564
unsigned short GAMEPAD_RemapAnalogueButtons(unsigned short in)
{ //line: 2
} //line: 4


// path: C:\kain2\game\GAMEPAD.C
// address: 0x80031B20
// line start: 567
// line end:   573
void GAMEPAD_DetectInit()
{ //line: 1
	// register: $s0
	register int orgdualshock_onflag;
} //line: 7


// path: C:\kain2\game\GAMEPAD.C
// address: 0x80031B68
// line start: 576
// line end:   720
void GAMEPAD_GetData(long (*data)[5])
{ //line: 1
	// register: $a0
	register long analogue_x;
	// register: $v1
	register long analogue_y;
	// register: $v1
	register int padState;
} //line: 145


// path: C:\kain2\game\GAMEPAD.C
// address: 0x80031FA4
// line start: 723
// line end:   731
void GAMEPAD_DisplayControllerStatus(int msgY)
{ //line: 1
	// register: $s0
	register char *noCtrlStr;
} //line: 9


// path: C:\kain2\game\GAMEPAD.C
// address: 0x80032004
// line start: 737
// line end:   789
void GAMEPAD_Process(struct GameTracker *gameTracker);

// path: C:\kain2\game\GAMEPAD.C
// address: 0x8003205C
// line start: 797
// line end:   800
void GAMEPAD_SaveControllers();

// path: C:\kain2\game\GAMEPAD.C
// address: 0x800320F8
// line start: 804
// line end:   812
void GAMEPAD_RestoreControllers();

// path: C:\kain2\game\INSTANCE.C
// address: 0x800321B4
// line start: 51
// line end:   76
void INSTANCE_Deactivate(struct _Instance *instance)
{ //line: 1
	// register: $a2
	// size: 0x4C
	register struct Object *object;
} //line: 26


// path: C:\kain2\game\INSTANCE.C
// address: 0x80032270
// line start: 80
// line end:   102
void INSTANCE_Reactivate(struct _Instance *instance)
{ //line: 1
	// register: $a2
	// size: 0x4C
	register struct Object *object;
} //line: 23


// path: C:\kain2\game\INSTANCE.C
// address: 0x80032308
// line start: 106
// line end:   110
void INSTANCE_ForceActive(struct _Instance *instance);

// path: C:\kain2\game\INSTANCE.C
// address: 0x8003233C
// line start: 116
// line end:   118
void INSTANCE_DeactivatedProcess(struct _Instance *instance, struct GameTracker *gameTracker);

// path: C:\kain2\game\INSTANCE.C
// address: 0x80032344
// line start: 122
// line end:   219
void INSTANCE_DeactivateFarInstances(struct GameTracker *gameTracker)
{ //line: 1
	// register: $v0
	// size: 0x10C
	register struct _InstanceList *instanceList;
	// register: $s0
	// size: 0x29C
	register struct _Instance *instance;
	// register: $s2
	// size: 0x8
	register struct SVECTOR *line;
	// register: $v1
	register long distSq;
	// register: $a0
	register int numInstances;
	// register: $s1
	register int numToProcess;
	// register: $v1
	register int cntInst;
	static int lastInst;
} //line: 98


// path: C:\kain2\game\INSTANCE.C
// address: 0x800325C0
// line start: 243
// line end:   278
void INSTANCE_InitInstanceList(struct _InstanceList *list, struct _InstancePool *pool)
{ //line: 2
	// register: $a3
	register long i;
} //line: 35


// path: C:\kain2\game\INSTANCE.C
// address: 0x80032660
// size: 0x29C
// line start: 334
// line end:   374
struct _Instance *INSTANCE_NewInstance(struct _InstanceList *list)
{ //line: 2
	// register: $v0
	// size: 0x29C
	register struct _Instance *temp;
	// register: $a1
	// size: 0x29C
	register struct _Instance *instance;
} //line: 41


// path: C:\kain2\game\INSTANCE.C
// address: 0x800326EC
// line start: 377
// line end:   417
long INSTANCE_InstanceGroupNumber(struct _Instance *instance)
{ //line: 1
	// register: $s0
	register long result;
} //line: 40


// path: C:\kain2\game\INSTANCE.C
// address: 0x80032808
// line start: 436
// line end:   439
void INSTANCE_InsertInstanceGroup(struct _InstanceList *list, struct _Instance *instance)
{ //line: 1
} //line: 1


// path: C:\kain2\game\INSTANCE.C
// address: 0x80032850
// line start: 485
// line end:   597
void INSTANCE_ReallyRemoveInstance(struct _InstanceList *list, struct _Instance *instance, long reset)
{ //line: 1
	// register: $v1
	// size: 0x29C
	register struct _Instance *temp;
	{ //line: 103
		// register: $a0
		register int i;
	} //line: 112
} //line: 113


// path: C:\kain2\game\INSTANCE.C
// address: 0x80032ABC
// line start: 601
// line end:   635
void INSTANCE_CleanUpInstanceList(struct _InstanceList *list, long reset)
{ //line: 1
	// register: $a1
	// size: 0x29C
	register struct _Instance *instance;
	// register: $s0
	// size: 0x29C
	register struct _Instance *next;
} //line: 35


// path: C:\kain2\game\INSTANCE.C
// address: 0x80032B78
// line start: 706
// line end:   747
long INSTANCE_Introduced(struct Intro *intro, short streamUnitID)
{ //line: 1
	// register: $v1
	// size: 0x29C
	register struct _Instance *instance;
	// register: $v1
	// size: 0x29C
	register struct _Instance *next;
	// register: $s1
	register long ret;
} //line: 41


// path: C:\kain2\game\INSTANCE.C
// address: 0x80032C30
// size: 0x4
// line start: 751
// line end:   761
struct INICommand *INSTANCE_GetIntroCommand(struct INICommand *command, int cmd);

// path: C:\kain2\game\INSTANCE.C
// address: 0x80032C80
// size: 0x4
// line start: 766
// line end:   767
struct INICommand *INSTANCE_FindIntroCommand(struct _Instance *instance, int cmd);

// path: C:\kain2\game\INSTANCE.C
// address: 0x80032CA4
// line start: 773
// line end:   795
void INSTANCE_ProcessIntro(struct _Instance *instance)
{ //line: 2
	{ //line: 2
		// register: $a1
		// size: 0x4
		register struct INICommand *command;
	} //line: 22
} //line: 23


// path: C:\kain2\game\INSTANCE.C
// address: 0x80032D20
// line start: 798
// line end:   810
void INSTANCE_InitEffects(struct _Instance *instance, struct Object *object)
{ //line: 1
	// register: $s0
	register int i;
} //line: 13


// path: C:\kain2\game\INSTANCE.C
// address: 0x80032DA8
// size: 0x29C
// line start: 813
// line end:   1118
struct _Instance *INSTANCE_IntroduceInstance(struct Intro *intro, short streamUnitID)
{ //line: 1
	// register: $s6
	// size: 0x4C
	register struct Object *object;
	// register: $s0
	// size: 0x29C
	register struct _Instance *instance;
	// register: $s5
	// size: 0x29C
	register struct _Instance *attachInst;
	// register: $s1
	// size: 0x24
	register struct _ObjectTracker *objectTracker;
	{ //line: 16
		// register: $v0
		// size: 0x4
		register struct INICommand *index;
		// register: $s4
		register long attachedUniqueID;
		{ //line: 153
		} //line: 163
		{ //line: 213
			// register: $s1
			// size: 0xC
			register struct SavedIntroSpline *savedIntroSpline;
			// register: $s2
			// size: 0x50
			register struct MultiSpline *spline;
		} //line: 263
		{ //line: 265
			// register: $v0
			// size: 0x4
			register struct SavedIntroSmall *savedIntroSmall;
			{ //line: 279
			} //line: 286
		} //line: 288
	} //line: 304
} //line: 306


// path: C:\kain2\game\INSTANCE.C
// address: 0x800334C8
// line start: 1124
// line end:   1156
void INSTANCE_AdditionalCollideFunctions(struct _InstanceList *instanceList)
{ //line: 1
	// register: $s0
	// size: 0x29C
	register struct _Instance *instance;
} //line: 32


// path: C:\kain2\game\INSTANCE.C
// address: 0x800335F0
// line start: 1159
// line end:   1168
long INSTANCE_GetSplineFrameNumber(struct _Instance *instance, struct MultiSpline *spline)
{ //line: 1
} //line: 1


// path: C:\kain2\game\INSTANCE.C
// address: 0x80033628
// line start: 1172
// line end:   1504
void INSTANCE_ProcessFunctions(struct _InstanceList *instanceList)
{ //line: 1
	{ //line: 7
		// register: $s0
		// size: 0x29C
		register struct _Instance *instance;
		{ //line: 29
			// register: $s2
			register int hidden;
			// register: $s1
			register int burning;
		} //line: 52
		{ //line: 78
			// register: $s2
			register long prevFrame;
			// register: $s4
			// size: 0x50
			register struct MultiSpline *multi;
			{ //line: 85
				// register: $s6
				register long endOfSpline;
				// register: $s7
				register short direction;
				{ //line: 124
					// register: $s3
					register long frame;
					// register: $s1
					register long maxFrames;
				} //line: 203
				{ //line: 214
				} //line: 219
			} //line: 279
		} //line: 281
		{ //line: 304
			// register: $s1
			register int killTest;
		} //line: 311
	} //line: 323
} //line: 333


// path: C:\kain2\game\INSTANCE.C
// address: 0x80033EB0
// size: 0x29C
// line start: 1508
// line end:   1579
struct _Instance *INSTANCE_BirthObject(struct _Instance *parent, struct Object *object, int modelNum)
{ //line: 1
	// register: $s0
	// size: 0x29C
	register struct _Instance *instance;
	// register: $s1
	register int i;
} //line: 72


// path: C:\kain2\game\INSTANCE.C
// address: 0x80034120
// line start: 1597
// line end:   1606
void INSTANCE_BuildStaticShadow(struct _Instance *instance);

// path: C:\kain2\game\INSTANCE.C
// address: 0x80034128
// line start: 1631
// line end:   1771
void INSTANCE_DefaultInit(struct _Instance *instance, struct Object *object, int modelNum)
{ //line: 1
	// register: $t3
	register int i;
	// register: $a3
	register int j;
	// register: $a2
	register int numHPrims;
	// register: $s0
	register int numModels;
	// register: $a2
	// size: 0x8
	register struct _HPrim_duplicate_s3 *hprim;
	// register: $t7
	// size: 0x38
	register struct _Model **pModel;
	// register: $t8
	// size: 0x8
	register struct _HModel *hmodel;
	{ //line: 54
		// register: $v0
		// size: 0x38
		register struct _Model *model;
		// register: $a1
		// size: 0x18
		register struct _Segment *seg;
		{ //line: 59
			// register: $a0
			// size: 0x18
			register struct _HInfo *hinfo;
		} //line: 63
	} //line: 64
	{ //line: 73
		// register: $s0
		// size: 0x38
		register struct _Model *model;
		// register: $t4
		// size: 0x18
		register struct _Segment *seg;
		{ //line: 84
			// register: $a1
			// size: 0x18
			register struct _HInfo *hinfo;
			{ //line: 88
				// register: $v1
				register int k;
				// register: $t0
				// size: 0xC
				register struct _HFace *hface;
				// register: $t1
				// size: 0x14
				register struct _HSphere *hsphere;
				// register: $t2
				// size: 0x1C
				register struct _HBox *hbox;
			} //line: 127
		} //line: 128
	} //line: 129
} //line: 141


// path: C:\kain2\game\INSTANCE.C
// address: 0x800344E4
// line start: 1775
// line end:   1817
void INSTANCE_PlainDeath(struct _Instance *instance)
{ //line: 1
	{ //line: 32
		// register: $a0
		// size: 0x29C
		register struct _Instance *oldOn;
	} //line: 39
} //line: 41


// path: C:\kain2\game\INSTANCE.C
// address: 0x80034614
// line start: 1820
// line end:   1826
void INSTANCE_KillInstance(struct _Instance *instance);

// path: C:\kain2\game\INSTANCE.C
// address: 0x80034648
// line start: 1839
// line end:   1852
unsigned long INSTANCE_Query(struct _Instance *Inst, int Query)
{ //line: 1
	// register: $v0
	register unsigned long (*Func)();
} //line: 14


// path: C:\kain2\game\INSTANCE.C
// address: 0x80034684
// line start: 1857
// line end:   1867
void INSTANCE_Post(struct _Instance *Inst, int Message, int Data)
{ //line: 1
	// register: $s1
	register void (*Func)();
} //line: 11


// path: C:\kain2\game\INSTANCE.C
// address: 0x800346E8
// line start: 1874
// line end:   1887
void INSTANCE_Broadcast(struct _Instance *sender, long whatAmIMask, int Message, int Data)
{ //line: 1
	// register: $s0
	// size: 0x29C
	register struct _Instance *instance;
	// register: $s1
	register int plane;
} //line: 14


// path: C:\kain2\game\INSTANCE.C
// address: 0x80034794
// line start: 1893
// line end:   1894
int INSTANCE_InPlane(struct _Instance *instance, int plane);

// path: C:\kain2\game\INSTANCE.C
// address: 0x800347F0
// line start: 1920
// line end:   1938
long INSTANCE_FindWithID(long uniqueID)
{ //line: 2
	// register: $v1
	// size: 0x29C
	register struct _Instance *instance;
	// register: $v1
	// size: 0x29C
	register struct _Instance *next;
	// register: $a1
	register long ret;
} //line: 18


// path: C:\kain2\game\INSTANCE.C
// address: 0x80034830
// size: 0x29C
// line start: 1977
// line end:   2021
struct _Instance *INSTANCE_FindWithName(long areaID, char *instanceName, struct _Instance *startInstance)
{ //line: 1
	// register: $s0
	// size: 0x29C
	register struct _Instance *instance;
	// register: $s2
	// size: 0x29C
	register struct _Instance *ret;
	// register: $s1
	// size: 0x29C
	register struct _Instance *next;
} //line: 44


// path: C:\kain2\game\INSTANCE.C
// address: 0x80034904
// size: 0x4C
// line start: 2047
// line end:   2068
struct Intro *INSTANCE_FindIntro(long areaID, long introUniqueID)
{ //line: 1
	// register: $s0
	// size: 0x4C
	register struct Intro *ret;
	// register: $a0
	register long i;
	// register: $v1
	// size: 0x188
	register struct Level *level;
} //line: 21


// path: C:\kain2\game\INSTANCE.C
// address: 0x80034988
// size: 0x29C
// line start: 2072
// line end:   2080
struct _Instance *INSTANCE_Find(long introUniqueID)
{ //line: 2
	// register: $v1
	// size: 0x29C
	register struct _Instance *instance;
} //line: 8


// path: C:\kain2\game\INSTANCE.C
// address: 0x800349C8
// size: 0x29C
// line start: 2084
// line end:   2304
struct _Instance *INSTANCE_IntroduceSavedInstance(struct _SavedIntro *savedIntro, struct _StreamUnit *streamUnit, int *deleted)
{ //line: 1
	// register: $s1
	// size: 0x24
	register struct _ObjectTracker *objectTracker;
	// register: $s7
	// size: 0x4C
	register struct Object *object;
	// register: $s0
	// size: 0x29C
	register struct _Instance *instance;
	// register: $s5
	// size: 0x6
	register struct _Position *levelOffset;
	// register: $v0
	// size: 0x188
	register struct Level *level;
	// register: $s4
	// size: 0x29C
	register struct _Instance *attachInst;
	{ //line: 17
		// address: -56
		// size: 0xC
		auto char savedName[12];
		{ //line: 43
			// register: $v0
			// size: 0x188
			register struct Level *birthLevel;
			// register: $s1
			// size: 0x4C
			register struct Intro *oldIntro;
			// register: $s3
			register int nosave;
			// register: $s1
			register int remove;
			{ //line: 69
				// register: $v1
				// size: 0x4C
				register struct Intro *intro;
				// register: $a0
				register int i;
			} //line: 80
			{ //line: 97
				// register: $v0
				register short _x0;
				// register: $a0
				register short _y0;
				// register: $v1
				register short _z0;
				// register: $a1
				register short _x1;
				// register: $a2
				register short _y1;
				// register: $a3
				register short _z1;
				// register: $v0
				// size: 0x6
				register struct _Position *_v;
				// register: $v1
				// size: 0x6
				register struct _Position *_v0;
			} //line: 97
			{ //line: 179
			} //line: 190
		} //line: 199
	} //line: 211
} //line: 220


// path: C:\kain2\game\INSTANCE.C
// address: 0x80034E34
// size: 0x29C
// line start: 2307
// line end:   2529
struct _Instance *INSTANCE_IntroduceSavedInstanceWithIntro(struct _SavedIntroWithIntro *savedIntro, struct _StreamUnit *streamUnit, int *deleted)
{ //line: 1
	// register: $s1
	// size: 0x24
	register struct _ObjectTracker *objectTracker;
	// register: $s7
	// size: 0x4C
	register struct Object *object;
	// register: $s0
	// size: 0x29C
	register struct _Instance *instance;
	// register: $fp
	// size: 0x6
	register struct _Position *levelOffset;
	// register: $v1
	// size: 0x188
	register struct Level *level;
	// register: $s2
	// size: 0x4C
	register struct Intro *intro;
	// register: $s5
	// size: 0x29C
	register struct _Instance *attachedInst;
	{ //line: 45
		// register: $s1
		register int remove;
		// register: $s2
		register int nosave;
		{ //line: 49
			// register: $v0
			register short _x0;
			// register: $a0
			register short _y0;
			// register: $v1
			register short _z0;
			// register: $a1
			register short _x1;
			// register: $a2
			register short _y1;
			// register: $a3
			register short _z1;
			// register: $v0
			// size: 0x6
			register struct _Position *_v;
			// register: $v1
			// size: 0x6
			register struct _Position *_v0;
		} //line: 49
		{ //line: 186
		} //line: 197
	} //line: 206
} //line: 222


// path: C:\kain2\game\INSTANCE.C
// address: 0x800352A8
// line start: 2576
// line end:   2618
void INSTANCE_SpatialRelationships(struct _InstanceList *instanceList)
{ //line: 1
	// register: $s1
	// size: 0x29C
	register struct _Instance *instance;
	// register: $s0
	// size: 0x29C
	register struct _Instance *checkee;
	{ //line: 10
		// register: $s0
		// size: 0x20
		register struct MATRIX *lookMatrix;
		// register: $s3
		register long checkMask;
		{ //line: 19
			// address: -80
			// size: 0x20
			auto struct MATRIX invMatrix;
			{ //line: 28
				// address: -48
				// size: 0x18
				auto struct evCollideInstanceStatsData data;
				// register: $a2
				// size: 0x20
				register struct MATRIX *mat;
			} //line: 38
		} //line: 40
	} //line: 41
} //line: 43


// path: C:\kain2\game\INSTANCE.C
// address: 0x8003540C
// line start: 2623
// line end:   2647
int INSTANCE_SetStatsData(struct _Instance *instance, struct _Instance *checkee, struct _Vector *checkPoint, struct evCollideInstanceStatsData *data, struct MATRIX *mat)
{ //line: 1
	// register: $s4
	register long distance;
	{ //line: 6
		// register: $s0
		// size: 0x8
		register struct SVECTOR *delta;
	} //line: 23
} //line: 25


// path: C:\kain2\game\INSTANCE.C
// address: 0x8003551C
// line start: 2652
// line end:   2668
void INSTANCE_LinkToParent(struct _Instance *instance, struct _Instance *parent, int node);

// path: C:\kain2\game\INSTANCE.C
// address: 0x800355D8
// line start: 2673
// line end:   2712
void INSTANCE_UnlinkFromParent(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x29C
	register struct _Instance *parent;
	{ //line: 5
		// address: -24
		// size: 0x8
		auto struct _G2EulerAngles_Type ea;
		{ //line: 13
			// register: $v1
			// size: 0x29C
			register struct _Instance *sibling;
		} //line: 19
	} //line: 39
} //line: 40


// path: C:\kain2\game\INSTANCE.C
// address: 0x8003573C
// line start: 2715
// line end:   2732
void INSTANCE_UnlinkChildren(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x29C
	register struct _Instance *child;
	{ //line: 6
		// register: $s0
		// size: 0x29C
		register struct _Instance *sibling;
	} //line: 6
} //line: 17


// path: C:\kain2\game\INSTANCE.C
// address: 0x800357A8
// line start: 2738
// line end:   2746
void INSTANCE_UpdateFamilyStreamUnitID(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x29C
	register struct _Instance *child;
} //line: 9


// path: C:\kain2\game\INSTANCE.C
// address: 0x80035800
// line start: 2749
// line end:   2759
void INSTANCE_ReallyRemoveAllChildren(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x29C
	register struct _Instance *child;
	{ //line: 6
		// register: $s0
		// size: 0x29C
		register struct _Instance *sibling;
	} //line: 6
} //line: 11


// path: C:\kain2\game\INSTANCE.C
// address: 0x8003585C
// size: 0x29C
// line start: 2765
// line end:   2775
struct _Instance *INSTANCE_GetChildLinkedToSegment(struct _Instance *instance, int segment)
{ //line: 2
	// register: $v1
	// size: 0x29C
	register struct _Instance *child;
} //line: 10


// path: C:\kain2\game\INSTANCE.C
// address: 0x80035894
// line start: 2781
// line end:   2791
int INSTANCE_Linked(struct _Instance *instance1, struct _Instance *instance2)
{ //line: 2
	// register: $a0
	// size: 0x29C
	register struct _Instance *root1;
	// register: $a1
	// size: 0x29C
	register struct _Instance *root2;
} //line: 10


// path: C:\kain2\game\INSTANCE.C
// address: 0x800358E4
// line start: 2799
// line end:   2824
int INSTANCE_GetFadeValue(struct _Instance *instance)
{ //line: 1
	// register: $s0
	register int fadeValue;
} //line: 25


// path: C:\kain2\game\INSTANCE.C
// address: 0x800359C8
// line start: 2835
// line end:   2870
unsigned long INSTANCE_DefaultAnimCallback(struct _G2Anim_Type *anim, int sectionID, enum _G2AnimCallbackMsg_Enum_duplicate_e5 message, long messageDataA, long messageDataB, struct _Instance *instance)
{ //line: 1
	// register: $s2
	// size: 0x8
	register struct _AnimSoundData_Type *soundData;
	{ //line: 13
		// register: $s0
		register int id;
		// register: $s1
		register int vol;
	} //line: 22
} //line: 36


// path: C:\kain2\game\CAMERA.C
// address: 0x80014AA4
// line start: 230
// line end:   260
void CAMERA_CalculateViewVolumeNormals(struct Camera_duplicate_s2 *camera)
{ //line: 1
	// register: $v0
	register short projDistance;
	// address: -40
	// size: 0x8
	auto struct _Normal n0;
	// address: -32
	// size: 0x8
	auto struct _Normal n1;
	// address: -24
	// size: 0x8
	auto struct _Normal n2;
	// address: -16
	// size: 0x8
	auto struct _Normal n3;
	// register: $a0
	register int x1;
	// register: $a2
	register int x2;
	// register: $v1
	register int y1;
	// register: $a1
	register int y2;
} //line: 1


// path: C:\kain2\game\CAMERA.C
// address: 0x80014DE0
// line start: 263
// line end:   270
void CAMERA_CalcVVClipInfo(struct Camera_duplicate_s2 *camera);

// path: C:\kain2\game\CAMERA.C
// address: 0x80014F00
// line start: 274
// line end:   276
void CAMERA_SetViewVolume(struct Camera_duplicate_s2 *camera);

// path: C:\kain2\game\CAMERA.C
// address: 0x80014F2C
// line start: 280
// line end:   299
void CAMERA_SetProjDistance(struct Camera_duplicate_s2 *camera, long distance)
{ //line: 1
	// register: $s0
	// size: 0x188
	register struct Level *level;
	// register: $s2
	register int i;
} //line: 20


// path: C:\kain2\game\CAMERA.C
// address: 0x80014FD0
// line start: 302
// line end:   309
void CAMERA_CreateNewFocuspoint(struct Camera_duplicate_s2 *camera)
{ //line: 1
	// address: -24
	// size: 0x8
	auto struct _SVector sv;
	{ //line: 1
		// register: $v1
		register short _x0;
		// register: $a0
		register short _y0;
		// register: $a1
		register short _z0;
		// register: $v0
		register short _x1;
		// register: $a2
		register short _y1;
		// register: $a3
		register short _z1;
		// register: $v0
		// size: 0x8
		register struct _SVector *_v;
	} //line: 1
	{ //line: 1
		// register: $v0
		register short _x0;
		// register: $v0
		register short _y0;
		// register: $v1
		register short _z0;
		// register: $a2
		// size: 0x6
		register struct _Position *_v;
	} //line: 1
} //line: 1


// path: C:\kain2\game\CAMERA.C
// address: 0x80015074
// line start: 315
// line end:   356
void CAMERA_SaveMode(struct Camera_duplicate_s2 *camera, long mode)
{ //line: 2
	// register: $t4
	register long i;
} //line: 42


// path: C:\kain2\game\CAMERA.C
// address: 0x800153AC
// line start: 359
// line end:   485
void CAMERA_RestoreMode(struct Camera_duplicate_s2 *camera)
{ //line: 1
	{ //line: 13
		// register: $s1
		register long mode;
	} //line: 126
} //line: 127


// path: C:\kain2\game\CAMERA.C
// address: 0x80015854
// line start: 489
// line end:   519
void CAMERA_Save(struct Camera_duplicate_s2 *camera, long save)
{ //line: 1
	{ //line: 4
		// register: $a2
		register int i;
	} //line: 16
} //line: 31


// path: C:\kain2\game\CAMERA.C
// address: 0x80015924
// line start: 524
// line end:   556
void CAMERA_Restore(struct Camera_duplicate_s2 *camera, long restore);

// path: C:\kain2\game\CAMERA.C
// address: 0x800159E0
// size: 0x8
// line start: 564
// line end:   575
struct _SVector *SplineGetNextPointDC(struct Spline *spline, struct SplineDef *def)
{ //line: 1
	// size: 0x8
	static struct _SVector point;
} //line: 12


// path: C:\kain2\game\CAMERA.C
// address: 0x80015A3C
// line start: 578
// line end:   771
void CAMERA_SetMode(struct Camera_duplicate_s2 *camera, long mode)
{ //line: 1
	// register: $s0
	register int oldMode;
	// address: -56
	// size: 0x8
	auto struct SplineDef curPositional;
	// address: -48
	// size: 0x8
	auto struct _SVector sv;
	{ //line: 47
		// register: $v0
		register short _x1;
		// register: $a0
		register short _y1;
		// register: $a1
		register short _z1;
		// register: $v0
		// size: 0x8
		register struct _Rotation *_v0;
		// register: $v1
		// size: 0x8
		register struct _Rotation *_v1;
	} //line: 47
	{ //line: 47
		// register: $v0
		register short _x1;
		// register: $a0
		register short _y1;
		// register: $v1
		register short _z1;
		// register: $v0
		// size: 0x8
		register struct _Rotation *_v0;
	} //line: 47
	{ //line: 47
		// register: $v0
		register short _x0;
		// register: $a1
		register short _y0;
		// register: $v1
		register short _z0;
		// register: $a2
		register short _x1;
		// register: $a3
		register short _y1;
		// register: $t0
		register short _z1;
		// register: $a0
		// size: 0x8
		register struct _SVector *_v;
		// register: $v1
		// size: 0x6
		register struct _Position *_v0;
	} //line: 47
	{ //line: 107
		// address: -32
		// size: 0x8
		auto struct _SVector *camPos;
		// register: $a0
		// size: 0x8
		register struct _SVector *targetPos;
		{ //line: 112
			// address: -40
			// size: 0x6
			auto struct _Position pos;
		} //line: 114
		{ //line: 128
			// register: $v0
			register short _x1;
			// register: $v1
			register short _y1;
			// register: $a0
			register short _z1;
			// register: $v0
			// size: 0x6
			register struct _Position *_v0;
		} //line: 128
		{ //line: 138
			// register: $v0
			register short _x1;
			// register: $v1
			register short _y1;
			// register: $a0
			register short _z1;
			// register: $v0
			// size: 0x6
			register struct _Position *_v0;
		} //line: 138
	} //line: 142
} //line: 194


// path: C:\kain2\game\CAMERA.C
// address: 0x80015EEC
// line start: 774
// line end:   1012
void CAMERA_Initialize(struct Camera_duplicate_s2 *camera)
{ //line: 1
	// register: $s2
	register long i;
} //line: 219


// path: C:\kain2\game\CAMERA.C
// address: 0x800161EC
// line start: 1018
// line end:   1041
void CAMERA_SetInstanceFocus(struct Camera_duplicate_s2 *camera, struct _Instance *instance);

// path: C:\kain2\game\CAMERA.C
// address: 0x8001624C
// line start: 1052
// line end:   1056
void CAMERA_SetZRotation(struct Camera_duplicate_s2 *camera, short zrot);

// path: C:\kain2\game\CAMERA.C
// address: 0x80016270
// line start: 1107
// line end:   1108
long CAMERA_LengthSVector(struct _SVector *sv);

// path: C:\kain2\game\CAMERA.C
// address: 0x800162C4
// line start: 1116
// line end:   1169
void CAMERA_SetValue(struct Camera_duplicate_s2 *camera, long index, long value)
{ //line: 2
	// register: $t1
	register long min;
	// register: $t0
	register long max;
	// register: $a3
	register long minTilt;
	// register: $a1
	register long maxTilt;
} //line: 54


// path: C:\kain2\game\CAMERA.C
// address: 0x800166F0
// line start: 1173
// line end:   1185
short CAMERA_AngleDifference(short angle0, short angle1);

// path: C:\kain2\game\CAMERA.C
// address: 0x80016768
// line start: 1190
// line end:   1191
short CAMERA_SignedAngleDifference(short angle0, short angle1);

// path: C:\kain2\game\CAMERA.C
// address: 0x80016798
// line start: 1209
// line end:   1228
unsigned long CAMERA_QueryMode(struct Camera_duplicate_s2 *camera)
{ //line: 1
	// register: $s0
	register unsigned long mode;
} //line: 19


// path: C:\kain2\game\CAMERA.C
// address: 0x80016834
// line start: 1233
// line end:   1265
void CAMERA_SetMaxVel(struct Camera_duplicate_s2 *camera)
{ //line: 1
	// register: $s0
	register long extraVel;
	// register: $v0
	register long targetMaxVel;
	// address: -24
	// size: 0x8
	auto struct _SVector cam_dist;
	// address: 8
	static long maxVelAccl;
	// address: 12
	static long maxVelVel;
	{ //line: 1
		// register: $v0
		register short _x0;
		// register: $v1
		register short _y0;
		// register: $a0
		register short _z0;
		// register: $a1
		register short _x1;
		// register: $a3
		register short _y1;
		// register: $a2
		register short _z1;
		// register: $a1
		// size: 0x8
		register struct _SVector *_v;
		// register: $a0
		// size: 0x6
		register struct _Position *_v0;
		// register: $a2
		// size: 0x6
		register struct _Position *_v1;
	} //line: 1
} //line: 33


// path: C:\kain2\game\CAMERA.C
// address: 0x80016940
// line start: 1268
// line end:   1274
void CAMERA_SetTarget(struct Camera_duplicate_s2 *camera, struct _Position *pos)
{ //line: 1
	// address: -24
	// size: 0x8
	auto struct _SVector sv;
	{ //line: 1
		// register: $v0
		register short _x0;
		// register: $v1
		register short _y0;
		// register: $a1
		register short _z0;
		// register: $a2
		register short _x1;
		// register: $a3
		register short _y1;
		// register: $t0
		register short _z1;
		// register: $a0
		// size: 0x8
		register struct _SVector *_v;
	} //line: 1
} //line: 1


// path: C:\kain2\game\CAMERA.C
// address: 0x800169BC
// line start: 1279
// line end:   1313
void CAMERA_CalcPosition(struct _Position *position, struct _Position *base, struct _Rotation *rotation, short distance)
{ //line: 1
	// address: -96
	// size: 0x8
	auto struct SVECTOR sv;
	// address: -88
	// size: 0x10
	auto struct VECTOR v;
	// address: -72
	// size: 0x20
	auto struct MATRIX matrix;
	// address: -40
	// size: 0xC
	auto struct _Vector vectorPos;
	{ //line: 1
		// register: $v1
		register short _x1;
		// register: $a0
		register short _y1;
		// register: $v0
		register short _z1;
		// register: $v0
		// size: 0xC
		register struct _Vector *_v1;
	} //line: 1
} //line: 1


// path: C:\kain2\game\CAMERA.C
// address: 0x80016AE4
// line start: 1320
// line end:   1425
void CAMERA_SetFocus(struct Camera_duplicate_s2 *camera, struct _Position *targetfocusPoint)
{ //line: 1
	// register: $s0
	// size: 0x29C
	register struct _Instance *focusInstance;
	// register: $v0
	// size: 0x38
	register struct _Model *model;
	// address: -64
	// size: 0x8
	auto struct _SVector temp1;
	// address: -56
	// size: 0x8
	auto struct _SVector offset;
	// register: $v1
	// size: 0x8
	register struct _SVector *segPosVector;
	// address: -48
	// size: 0xC
	auto struct _Vector temp2;
	{ //line: 16
		// register: $v0
		register short _x1;
		// register: $a0
		register short _y1;
		// register: $v1
		register short _z1;
		// register: $s2
		// size: 0x8
		register struct _SVector *_v0;
	} //line: 16
	{ //line: 16
		// register: $v1
		register short _x1;
		// register: $a0
		register short _y1;
		// register: $t1
		register short _z1;
		// register: $v0
		// size: 0x6
		register struct _Position *_v1;
	} //line: 16
	{ //line: 26
		{ //line: 28
			// register: $v0
			register short _x0;
			// register: $a2
			register short _y0;
			// register: $a3
			register short _x1;
			// register: $t0
			register short _y1;
			// register: $v1
			register short _z1;
		} //line: 28
	} //line: 36
	{ //line: 48
		// register: $v0
		// size: 0x29C
		register struct _Instance *instance;
		// address: -32
		// size: 0x8
		auto struct _SVector output;
		{ //line: 54
			// register: $v0
			register short _x1;
			// register: $v1
			register short _y1;
			// register: $a0
			register short _z1;
		} //line: 54
		{ //line: 61
			// register: $v1
			register short _x1;
			// register: $a0
			register short _y1;
			// register: $v0
			register short _z1;
			// register: $v0
			// size: 0x6
			register struct _Position *_v1;
		} //line: 61
	} //line: 61
	{ //line: 66
		// register: $v1
		register short _x1;
		// register: $a0
		register short _y1;
		// register: $v0
		register short _z1;
		// register: $v0
		// size: 0x6
		register struct _Position *_v1;
	} //line: 66
	{ //line: 69
		// register: $v0
		register short _x0;
		// register: $v0
		register short _y0;
		// register: $v1
		register short _z0;
		// register: $v1
		register short _x1;
		// register: $a0
		register short _y1;
		// register: $a1
		register short _z1;
	} //line: 69
} //line: 106


// path: C:\kain2\game\CAMERA.C
// address: 0x80016D60
// line start: 1428
// line end:   1429
void CAMERA_Lock(struct Camera_duplicate_s2 *camera, long lock);

// path: C:\kain2\game\CAMERA.C
// address: 0x80016D74
// line start: 1433
// line end:   1434
void CAMERA_Unlock(struct Camera_duplicate_s2 *camera, long unlock);

// path: C:\kain2\game\CAMERA.C
// address: 0x80016D88
// line start: 1438
// line end:   1446
void CAMERA_SetSmoothValue(struct Camera_duplicate_s2 *camera, long smooth);

// path: C:\kain2\game\CAMERA.C
// address: 0x80016DD8
// line start: 1451
// line end:   1453
void CAMERA_SetTimer(struct Camera_duplicate_s2 *camera, long time);

// path: C:\kain2\game\CAMERA.C
// address: 0x80016DF8
// line start: 1457
// line end:   1471
void CAMERA_Adjust_tilt(struct Camera_duplicate_s2 *camera, long tilt);

// path: C:\kain2\game\CAMERA.C
// address: 0x80016E34
// line start: 1475
// line end:   1489
void CAMERA_Adjust_distance(struct Camera_duplicate_s2 *camera, long dist);

// path: C:\kain2\game\CAMERA.C
// address: 0x80016EB0
// line start: 1493
// line end:   1508
void CAMERA_Adjust_rotation(struct Camera_duplicate_s2 *camera, long rotation);

// path: C:\kain2\game\CAMERA.C
// address: 0x80016F34
// line start: 1512
// line end:   1524
void CAMERA_Adjust_roll(long roll_degrees, int frames);

// path: C:\kain2\game\CAMERA.C
// address: 0x80016FB8
// line start: 1528
// line end:   1587
void CAMERA_Adjust(struct Camera_duplicate_s2 *camera, long adjust)
{ //line: 1
	{ //line: 5
		{ //line: 8
			// address: -24
			// size: 0x8
			auto struct _SVector dv;
			{ //line: 9
				// register: $v0
				register short _x0;
				// register: $v1
				register short _y0;
				// register: $a1
				register short _z0;
				// register: $a2
				register short _x1;
				// register: $t0
				register short _y1;
				// register: $a3
				register short _z1;
				// register: $a0
				// size: 0x8
				register struct _SVector *_v;
				// register: $a3
				// size: 0x8
				register struct _SVector *_v1;
			} //line: 9
		} //line: 24
	} //line: 55
} //line: 60


// path: C:\kain2\game\CAMERA.C
// address: 0x80017078
// line start: 1591
// line end:   1592
void CAMERA_ChangeTo(struct Camera_duplicate_s2 *camera, struct _CameraKey *cameraKey);

// path: C:\kain2\game\CAMERA.C
// address: 0x80017080
// line start: 1596
// line end:   1616
void CAMERA_SetShake(struct Camera_duplicate_s2 *camera, long shake, long scale)
{ //line: 1
	// register: $a0
	register int shock;
	// register: $a1
	register int duration;
} //line: 20


// path: C:\kain2\game\CAMERA.C
// address: 0x80017130
// line start: 1619
// line end:   1623
void Decouple_AngleMoveToward(short *current_ptr, short destination, short step);

// path: C:\kain2\game\CAMERA.C
// address: 0x80017180
// line start: 1627
// line end:   1685
void CriticalDampValue(long dampMode, short *sourceVal, short targetVal, short *vel, short *accl, int smooth)
{ //line: 1
	// register: $t0
	register short maxVel;
	// register: $a0
	register short useVel;
} //line: 59


// path: C:\kain2\game\CAMERA.C
// address: 0x80017334
// line start: 1739
// line end:   1871
void CriticalDampPosition(long dampMode, struct _Position *position, struct _Position *targetPos, struct _SVector *vel, struct _SVector *accl, int smooth)
{ //line: 1
	// register: $s0
	register long length;
	// address: -48
	// size: 0xC
	auto struct _Vector vector;
	// address: -32
	// size: 0x8
	auto struct _SVector svector;
	// register: $s4
	register short maxVel;
	// register: $a3
	register int shift;
	{ //line: 22
		// register: $v0
		register long _x0;
		// register: $a2
		register long _y0;
		// register: $a1
		register long _z0;
		// register: $v1
		register long _x1;
		// register: $v1
		register long _y1;
		// register: $a0
		register long _z1;
		// register: $v1
		// size: 0xC
		register struct _Vector *_v;
	} //line: 22
	{ //line: 37
		// register: $v0
		register long _x0;
		// register: $a2
		register long _y0;
		// register: $a1
		register long _z0;
		// register: $v1
		register long _x1;
		// register: $v1
		register long _y1;
		// register: $a0
		register long _z1;
		// register: $v1
		// size: 0xC
		register struct _Vector *_v;
	} //line: 37
	{ //line: 71
		// register: $v0
		register short _x0;
		// register: $v1
		register short _y0;
		// register: $a1
		register short _z0;
		// register: $a2
		register short _x1;
		// register: $a3
		register short _y1;
		// register: $t0
		register short _z1;
		// register: $s2
		// size: 0x8
		register struct _SVector *_v;
	} //line: 71
	{ //line: 75
		// register: $v0
		register short _x1;
		// register: $v1
		register short _y1;
		// register: $a0
		register short _z1;
	} //line: 75
	{ //line: 79
		// register: $v0
		register short _x0;
		// register: $v0
		register short _y0;
		// register: $v1
		register short _z0;
		// register: $a0
		register short _y1;
		// register: $a1
		register short _z1;
	} //line: 79
	{ //line: 86
		// register: $v0
		register short _x0;
		// register: $v1
		register short _y0;
		// register: $a0
		register short _z0;
		// register: $a1
		register short _x1;
		// register: $a2
		register short _y1;
		// register: $a3
		register short _z1;
		// register: $a1
		// size: 0x8
		register struct _SVector *_v;
	} //line: 86
	{ //line: 100
		// register: $v0
		register short _x1;
		// register: $v1
		register short _y1;
		// register: $a0
		register short _z1;
	} //line: 100
	{ //line: 131
		// register: $v0
		register short _x1;
		// register: $v1
		register short _y1;
		// register: $a0
		register short _z1;
	} //line: 131
} //line: 133


// path: C:\kain2\game\CAMERA.C
// address: 0x800179F8
// line start: 1874
// line end:   1892
void CriticalDampAngle(long dampMode, short *currentVal, short target, short *vel, short *accl, int smooth)
{ //line: 1
	// address: -16
	auto short current;
} //line: 16


// path: C:\kain2\game\CAMERA.C
// address: 0x80017A98
// line start: 1895
// line end:   1909
short CAMERA_CalcZRotation(struct _Position *target, struct _Position *position)
{ //line: 1
	// address: -24
	// size: 0x8
	auto struct _SVector sv;
	// address: -16
	// size: 0x8
	auto struct _SVector onPlane;
	{ //line: 1
		// register: $a2
		register short _x0;
		// register: $v0
		register short _y0;
		// register: $v1
		register short _z0;
		// register: $a1
		register short _x1;
		// register: $a3
		register short _y1;
		// register: $t0
		register short _z1;
		// register: $a0
		// size: 0x8
		register struct _SVector *_v;
	} //line: 1
} //line: 1


// path: C:\kain2\game\CAMERA.C
// address: 0x80017B10
// line start: 1912
// line end:   1925
void CAMERA_CalcRotation(struct _Rotation *rotation, struct _Position *target, struct _Position *position)
{ //line: 1
	// address: -24
	// size: 0x8
	auto struct _SVector sv;
	// address: -16
	// size: 0x8
	auto struct _SVector onPlane;
	{ //line: 1
		// register: $a3
		register short _x0;
		// register: $v0
		register short _y0;
		// register: $v1
		register short _z0;
		// register: $a0
		register short _x1;
		// register: $a2
		register short _y1;
		// register: $a1
		register short _z1;
		// register: $a0
		// size: 0x8
		register struct _SVector *_v;
	} //line: 1
} //line: 1


// path: C:\kain2\game\CAMERA.C
// address: 0x80017BAC
// line start: 1929
// line end:   1946
void CAMERA_CalcFSRotation(struct Camera_duplicate_s2 *camera, struct _Rotation *rotation, struct _Position *target, struct _Position *position)
{ //line: 1
	// address: -32
	// size: 0x8
	auto struct _SVector sv;
	// address: -24
	// size: 0x8
	auto struct _SVector onPlane;
	// address: -16
	// size: 0x8
	auto struct _SVector sv2;
	{ //line: 1
		// register: $a1
		register short _x0;
		// register: $v1
		register short _y0;
		// register: $a0
		register short _z0;
		// register: $v0
		register short _x1;
		// register: $a3
		register short _y1;
		// register: $a2
		register short _z1;
		// register: $v0
		// size: 0x8
		register struct _SVector *_v;
	} //line: 1
	{ //line: 1
		// register: $v0
		// size: 0x8
		register struct _SVector *_v0;
	} //line: 1
} //line: 1


// path: C:\kain2\game\CAMERA.C
// address: 0x80017C58
// line start: 1949
// line end:   1974
void CAMERA_Relocate(struct Camera_duplicate_s2 *camera, struct _SVector *offset, int streamSignalFlag);

// path: C:\kain2\game\CAMERA.C
// address: 0x80017DA4
// size: 0xC
// line start: 2018
// line end:   2380
struct _TFace *CAMERA_SphereToSphereWithLines(struct Camera_duplicate_s2 *camera, struct CameraCollisionInfo *colInfo, int secondcheck_flag)
{ //line: 1
	// address: -68
	auto long minLength;
	// address: -344
	// size: 0x8
	auto struct _SVector sv;
	// address: -336
	// size: 0x28
	auto struct _SVector startPt[5];
	// address: -296
	// size: 0x28
	auto struct _SVector endPt[5];
	// address: -256
	// size: 0x8
	auto struct _SVector startLine;
	// address: -248
	// size: 0xC
	auto struct _Vector adjStartLine;
	// address: -232
	// size: 0x8
	auto struct _SVector endLine;
	// address: -224
	// size: 0xC
	auto struct _Vector adjEndLine;
	// address: -208
	// size: 0xC
	auto struct _Vector CamLineNormalized;
	// address: -192
	// size: 0x8
	auto struct _Rotation rotation;
	// address: -184
	// size: 0x20
	auto struct MATRIX matrix;
	// address: -64
	// size: 0xC
	auto struct _TFace *result;
	// address: -60
	auto long i;
	// address: -56
	auto long init;
	// address: -52
	// size: 0x188
	auto struct Level *level;
	// register: $v0
	// size: 0x29C
	register struct _Instance *focusInstance;
	// address: -152
	// size: 0xC
	auto struct _Vector ACE_vect;
	// address: -136
	// size: 0xC
	auto struct _LCollideInfo lcol;
	// register: $s2
	register int ACE_force;
	// address: -48
	auto int in_warpRoom;
	{ //line: 116
		// register: $v1
		register short _x0;
		// register: $a0
		register short _y0;
		// register: $v0
		register short _z0;
		// register: $a1
		register short _x1;
		// register: $a2
		register short _y1;
		// register: $a3
		register short _z1;
		// register: $v1
		// size: 0x8
		register struct _SVector *_v;
		// register: $v0
		// size: 0x6
		register struct _Position *_v0;
	} //line: 116
	{ //line: 116
		// register: $v1
		register short _x0;
		// register: $a0
		register short _y0;
		// register: $v0
		register short _z0;
		// register: $a1
		register short _x1;
		// register: $a2
		register short _y1;
		// register: $a3
		register short _z1;
		// register: $v1
		// size: 0x8
		register struct _SVector *_v;
		// register: $v0
		// size: 0x6
		register struct _Position *_v0;
	} //line: 116
	{ //line: 129
		// register: $v1
		register short _x0;
		// register: $a0
		register short _y0;
		// register: $v0
		register short _z0;
		// register: $a1
		register short _x1;
		// register: $a2
		register short _y1;
		// register: $a3
		register short _z1;
		// register: $s3
		// size: 0x8
		register struct _SVector *_v;
		// register: $v0
		// size: 0x6
		register struct _Position *_v0;
	} //line: 129
	{ //line: 129
		// register: $v1
		register short _x0;
		// register: $a1
		register short _y0;
		// register: $v0
		register short _z0;
		// register: $a2
		register short _x1;
		// register: $a3
		register short _y1;
		// register: $t0
		register short _z1;
		// register: $v1
		// size: 0x8
		register struct _SVector *_v;
		// register: $v0
		// size: 0x6
		register struct _Position *_v0;
	} //line: 129
	{ //line: 129
		// register: $v1
		register short _x0;
		// register: $a1
		register short _y0;
		// register: $a2
		register short _z0;
		// register: $a3
		register short _x1;
		// register: $t0
		register short _y1;
		// register: $t1
		register short _z1;
		// register: $v0
		// size: 0x8
		register struct _SVector *_v;
		// register: $v0
		// size: 0x6
		register struct _Position *_v1;
	} //line: 129
	{ //line: 129
		// register: $v1
		register short _x0;
		// register: $a1
		register short _y0;
		// register: $a2
		register short _z0;
		// register: $a3
		register short _x1;
		// register: $t0
		register short _y1;
		// register: $t1
		register short _z1;
		// register: $v0
		// size: 0x8
		register struct _SVector *_v;
		// register: $v0
		// size: 0x6
		register struct _Position *_v1;
	} //line: 129
	{ //line: 129
		// register: $v1
		register short _x0;
		// register: $a2
		register short _y0;
		// register: $v0
		register short _z0;
		// register: $a3
		register short _x1;
		// register: $t0
		register short _y1;
		// register: $t1
		register short _z1;
		// register: $v1
		// size: 0x8
		register struct _SVector *_v;
		// register: $v0
		// size: 0x6
		register struct _Position *_v0;
	} //line: 129
	{ //line: 129
		// register: $v1
		register short _x0;
		// register: $a3
		register short _y0;
		// register: $v0
		register short _z0;
		// register: $t0
		register short _x1;
		// register: $t1
		register short _y1;
		// register: $t2
		register short _z1;
		// register: $v1
		// size: 0x8
		register struct _SVector *_v;
		// register: $v0
		// size: 0x6
		register struct _Position *_v0;
	} //line: 129
	{ //line: 129
		// register: $v0
		register short _x0;
		// register: $v1
		register short _y0;
		// register: $a3
		register short _z0;
		// register: $t0
		register short _x1;
		// register: $t2
		register short _y1;
		// register: $t3
		register short _z1;
		// register: $v0
		// size: 0x8
		register struct _SVector *_v;
		// register: $a3
		// size: 0x8
		register struct _SVector *_v0;
		// register: $t1
		// size: 0x8
		register struct _SVector *_v1;
	} //line: 129
	{ //line: 129
		// register: $v0
		register short _x0;
		// register: $v1
		register short _y0;
		// register: $a3
		register short _z0;
		// register: $t0
		register short _x1;
		// register: $t2
		register short _y1;
		// register: $t1
		register short _z1;
		// register: $v0
		// size: 0x8
		register struct _SVector *_v;
	} //line: 129
	{ //line: 129
		// register: $v1
		register short _x1;
		// register: $a0
		register short _y1;
		// register: $v0
		register short _z1;
		// register: $v0
		// size: 0x6
		register struct _Position *_v1;
	} //line: 129
	{ //line: 129
		// register: $v1
		register short _x1;
		// register: $a0
		register short _y1;
		// register: $a2
		register short _z1;
		// register: $v0
		// size: 0x6
		register struct _Position *_v1;
	} //line: 129
	{ //line: 215
		// register: $fp
		register int flag;
		// address: -72
		auto short backface_flag;
		// address: -120
		// size: 0x2C
		auto struct _PCollideInfo pCollideInfo;
		{ //line: 240
			// register: $s2
			register int n;
			{ //line: 245
				// register: $s1
				// size: 0x188
				register struct Level *thislevel;
			} //line: 262
		} //line: 263
		{ //line: 288
			// register: $v0
			register short _x0;
			// register: $v1
			register short _y0;
			// register: $a0
			register short _z0;
			// register: $a1
			register short _x1;
			// register: $a2
			register short _y1;
			// register: $a3
			register short _z1;
			// address: -44
			// size: 0x8
			auto struct _SVector *_v;
		} //line: 288
		{ //line: 319
			// register: $v0
			register short _x0;
			// register: $v1
			register short _y0;
			// register: $a0
			register short _z0;
			// register: $a1
			register short _x1;
			// register: $a2
			register short _y1;
			// register: $a3
			register short _z1;
		} //line: 319
	} //line: 344
} //line: 363


// path: C:\kain2\game\CAMERA.C
// address: 0x800188E0
// line start: 2383
// line end:   2393
long CAMERA_CalcTilt(struct _Normal *normal, short zRot)
{ //line: 1
	// address: -64
	// size: 0x20
	auto struct MATRIX matrix;
	// address: -32
	// size: 0x10
	auto struct VECTOR newNormal;
} //line: 1


// path: C:\kain2\game\CAMERA.C
// address: 0x80018954
// line start: 2400
// line end:   2410
void CAMERA_SetLookFocusAndBase(struct _Instance *focusInstance, struct _Position *focusPoint)
{ //line: 1
	// address: -8
	// size: 0x6
	auto struct _Position lookFocus;
} //line: 1


// path: C:\kain2\game\CAMERA.C
// address: 0x8001899C
// line start: 2413
// line end:   2417
void CAMERA_SetLookFocusAndDistance(struct Camera_duplicate_s2 *camera, struct VECTOR *focuspoint, int distance);

// path: C:\kain2\game\CAMERA.C
// address: 0x800189C4
// line start: 2421
// line end:   2543
void CAMERA_LookProcess(struct Camera_duplicate_s2 *camera)
{ //line: 1
	// register: $s1
	// size: 0x29C
	register struct _Instance *focusInstance;
	// register: $t0
	register int smooth;
	// register: $a3
	register int distance;
	// register: $a0
	register long dampMode;
	{ //line: 46
		// register: $v1
		register short _x1;
		// register: $a0
		register short _y1;
		// register: $a1
		register short _z1;
		// register: $v0
		// size: 0x6
		register struct _Position *_v0;
		// register: $v0
		// size: 0x6
		register struct _Position *_v1;
	} //line: 46
	{ //line: 67
		// register: $v0
		register short _x1;
		// register: $v1
		register short _y1;
		// register: $a0
		register short _z1;
	} //line: 67
	{ //line: 67
		// register: $v0
		register short _x1;
		// register: $v1
		register short _y1;
		// register: $a1
		register short _z1;
		// register: $v0
		// size: 0x8
		register struct _Rotation *_v0;
	} //line: 67
	{ //line: 67
		// register: $v1
		register short _x1;
		// register: $a0
		register short _y1;
		// register: $v0
		// size: 0x8
		register struct _Rotation *_v0;
	} //line: 67
} //line: 73


// path: C:\kain2\game\CAMERA.C
// address: 0x80018C18
// line start: 2547
// line end:   2555
void CAMERA_Normalize(struct _SVector *svector)
{ //line: 1
	// register: $a1
	register long len;
} //line: 9


// path: C:\kain2\game\CAMERA.C
// address: 0x80018C90
// line start: 2558
// line end:   2669
void CAMERA_HandleTransitions(struct Camera_duplicate_s2 *camera)
{ //line: 1
} //line: 112


// path: C:\kain2\game\CAMERA.C
// address: 0x80018E44
// line start: 2674
// line end:   2701
void CAMERA_CalcFocusOffset(struct _SVector *offset, struct Camera_duplicate_s2 *camera)
{ //line: 1
	// address: -24
	// size: 0xC
	auto struct _Vector adjustedOffset;
	// address: -8
	// size: 0x8
	auto struct _SVector temp;
	// register: $a0
	// size: 0x29C
	register struct _Instance *focusInstance;
	{ //line: 25
		// register: $v0
		register short _x1;
		// register: $v1
		register short _y1;
		// register: $a0
		register short _z1;
	} //line: 25
} //line: 25


// path: C:\kain2\game\CAMERA.C
// address: 0x80018EF8
// line start: 2704
// line end:   2723
void CAMERA_CalcFocusOffsetForSwim(struct _SVector *offset, struct Camera_duplicate_s2 *camera)
{ //line: 1
	// address: -32
	// size: 0xC
	auto struct _Vector adjustedOffset;
	// address: -16
	// size: 0x8
	auto struct _SVector temp;
	// register: $v1
	// size: 0x29C
	register struct _Instance *focusInstance;
	{ //line: 1
		// register: $v1
		register short _x1;
		// register: $a0
		register short _y1;
		// register: $v0
		register short _z1;
		// register: $v0
		// size: 0xC
		register struct _Vector *_v1;
	} //line: 1
} //line: 1


// path: C:\kain2\game\CAMERA.C
// address: 0x80018F64
// line start: 2728
// line end:   2747
short CAMERA_CalcIntersectAngle(struct _SVector *linept, struct _SVector *vertex0, struct _SVector *vertex1, short *high, short *low)
{ //line: 1
	// address: -24
	// size: 0x8
	auto struct _SVector point;
	// register: $a0
	register short zrot;
	// register: $v0
	register long camera_plane_d;
} //line: 20


// path: C:\kain2\game\CAMERA.C
// address: 0x80019054
// line start: 2750
// line end:   2779
short CAMERA_GetLineAngle(struct Camera_duplicate_s2 *camera, struct CameraCollisionInfo *colInfo, struct _SVector *linept, int line)
{ //line: 1
	// register: $t1
	// size: 0x54
	register struct _Terrain *terrain;
	// register: $s1
	// size: 0x8
	register struct _SVector *vertex0;
	// register: $s0
	// size: 0x8
	register struct _SVector *vertex1;
	// register: $s3
	// size: 0x8
	register struct _SVector *vertex2;
	// address: -48
	// size: 0x8
	auto struct _SVector new_linept;
	// address: -40
	auto short high;
	// address: -38
	auto short low;
} //line: 30


// path: C:\kain2\game\CAMERA.C
// address: 0x8001921C
// line start: 2794
// line end:   2833
long CAMERA_ACForcedMovement(struct Camera_duplicate_s2 *camera, struct CameraCollisionInfo *colInfo)
{ //line: 1
	// register: $v0
	register long dp;
	// address: -32
	// size: 0x8
	auto struct _Normal normal;
	// address: -24
	// size: 0x8
	auto struct _SVector sv;
	{ //line: 1
		// register: $a1
		register short _x0;
		// register: $a2
		register short _y0;
		// register: $v0
		register short _z0;
		// register: $a3
		register short _x1;
		// register: $t0
		register short _y1;
		// register: $v1
		register short _z1;
		// register: $a0
		// size: 0x8
		register struct _SVector *_v;
		// register: $v0
		// size: 0x6
		register struct _Position *_v0;
		// register: $v1
		// size: 0x6
		register struct _Position *_v1;
	} //line: 1
} //line: 39


// path: C:\kain2\game\CAMERA.C
// address: 0x800192EC
// line start: 2836
// line end:   2845
void CAMERA_update_dist_debounced(struct Camera_duplicate_s2 *camera, short dist);

// path: C:\kain2\game\CAMERA.C
// address: 0x8001935C
// line start: 2848
// line end:   2866
short CAMERA_dampgetline(short angle)
{ //line: 1
	// address: 80
	static short target_angle;
	// address: 82
	static short angle_vel;
	// address: 84
	static short angle_accl;
} //line: 18


// path: C:\kain2\game\CAMERA.C
// address: 0x80019408
// line start: 2874
// line end:   3124
long CAMERA_ACNoForcedMovement(struct Camera_duplicate_s2 *camera, struct CameraCollisionInfo *colInfo)
{ //line: 1
	// register: $s2
	register long hit;
	// register: $s3
	register short playerSamePos;
	// register: $a0
	register short angle;
	{ //line: 27
		// register: $a0
		register int n;
		// register: $t1
		register int flag;
		// register: $a1
		register short dist;
	} //line: 83
} //line: 251


// path: C:\kain2\game\CAMERA.C
// address: 0x8001990C
// line start: 3131
// line end:   3200
long CAMERA_AbsoluteCollision(struct Camera_duplicate_s2 *camera, struct CameraCollisionInfo *colInfo)
{ //line: 1
	// register: $a3
	register long hit;
} //line: 70


// path: C:\kain2\game\CAMERA.C
// address: 0x80019A34
// line start: 3206
// line end:   3250
short CAMERA_update_z_damped(struct Camera_duplicate_s2 *camera, short current, short target)
{ //line: 1
	// address: 86
	static short upvel;
	// address: 88
	static short upaccl;
	// address: 90
	static short upmaxVel;
	// address: -16
	auto short current_tmp;
} //line: 45


// path: C:\kain2\game\CAMERA.C
// address: 0x80019CA4
// line start: 3311
// line end:   3392
void CAMERA_CombatCamDist(struct Camera_duplicate_s2 *camera)
{ //line: 1
	// address: -48
	// size: 0x4
	auto struct DVECTOR xy;
	// address: -40
	// size: 0x4
	auto struct DVECTOR xy2;
	// address: -32
	// size: 0x8
	auto struct _SVector position;
	// address: -24
	auto long z;
	// register: $s1
	// size: 0x29C
	register struct _Instance *instance;
} //line: 82


// path: C:\kain2\game\CAMERA.C
// address: 0x80019F68
// line start: 3396
// line end:   3604
void CAMERA_GenericCameraProcess(struct Camera_duplicate_s2 *camera)
{ //line: 1
	// register: $s1
	// size: 0x29C
	register struct _Instance *focusInstance;
	// address: -32
	// size: 0x6
	auto struct _Position targetCamPos;
	{ //line: 53
		// register: $a0
		register short angle;
		// address: -24
		// size: 0x8
		auto struct _Rotation test_rot;
	} //line: 71
	{ //line: 79
		// register: $v0
		// size: 0x29C
		register struct _Instance *warpInstance;
		// register: $v0
		register int tmp;
	} //line: 85
	{ //line: 117
		// register: $v1
		register int mod;
	} //line: 132
	{ //line: 140
		// register: $a3
		register int dist;
	} //line: 158
	{ //line: 172
		// register: $v1
		register short _x1;
		// register: $a0
		register short _y1;
		// register: $a1
		register short _z1;
		// register: $v0
		// size: 0x6
		register struct _Position *_v0;
		// register: $v0
		// size: 0x6
		register struct _Position *_v1;
	} //line: 172
	{ //line: 181
		// address: -24
		// size: 0x6
		auto struct _Position target;
	} //line: 201
} //line: 204


// path: C:\kain2\game\CAMERA.C
// address: 0x8001A3E4
// line start: 3608
// line end:   3740
void CAMERA_CinematicProcess(struct Camera_duplicate_s2 *camera)
{ //line: 1
	// register: $s0
	// size: 0x8
	register struct _SVector *camPos;
	// register: $s3
	// size: 0x8
	register struct _SVector *camTarget;
	// register: $s4
	// size: 0x50
	register struct MultiSpline *posSpline;
	// register: $s1
	// size: 0x50
	register struct MultiSpline *targetSpline;
	{ //line: 24
		// register: $v0
		register short _x1;
		// register: $v1
		register short _y1;
		// register: $a0
		register short _z1;
	} //line: 24
	{ //line: 28
		// register: $v1
		register short _x1;
		// register: $a0
		register short _y1;
		// register: $v0
		register short _z1;
		// register: $v0
		// size: 0x8
		register struct _SVector *_v1;
	} //line: 28
	{ //line: 33
		// register: $v0
		register short _x1;
		// register: $v1
		register short _y1;
		// register: $a0
		register short _z1;
		// register: $v0
		// size: 0x8
		register struct _SVector *_v0;
	} //line: 33
	{ //line: 41
		// register: $v0
		register short _x1;
		// register: $v1
		register short _y1;
		// register: $a2
		register short _z1;
		// register: $v0
		// size: 0x6
		register struct _Position *_v0;
	} //line: 41
	{ //line: 54
		// register: $v0
		register short _x1;
		// register: $v1
		register short _y1;
		// register: $a0
		register short _z1;
		// register: $v0
		// size: 0x6
		register struct _Position *_v0;
	} //line: 54
	{ //line: 102
	} //line: 119
	{ //line: 122
		// register: $v0
		register short _x1;
		// register: $v1
		register short _y1;
		// register: $a0
		register short _z1;
		// register: $v0
		// size: 0x8
		register struct _Rotation *_v0;
	} //line: 122
} //line: 122


// path: C:\kain2\game\CAMERA.C
// address: 0x8001A900
// line start: 3744
// line end:   3755
int CAMERA_GetDistSq(struct _SVector *point1, struct _SVector *point2)
{ //line: 1
	// address: -16
	// size: 0xC
	auto struct _Vector d;
} //line: 1


// path: C:\kain2\game\CAMERA.C
// address: 0x8001A97C
// line start: 3759
// line end:   3780
void CAMERA_NearestPointOnLineVec(struct _SVector *linePoint, struct _SVector *start, struct _SVector *line, struct _Position *point)
{ //line: 1
	// address: -64
	// size: 0x20
	auto struct MATRIX nmat;
	// address: -32
	// size: 0xC
	auto struct _Vector dpv;
	// register: $a0
	register long t;
} //line: 19


// path: C:\kain2\game\CAMERA.C
// address: 0x8001AAA8
// line start: 3784
// line end:   3787
int CAMERA_CheckPoint(int linePoint, int linept1, int linept2);

// path: C:\kain2\game\CAMERA.C
// address: 0x8001AAEC
// line start: 3790
// line end:   3794
int CAMERA_CheckIfPointOnLine(struct _SVector *linePoint, struct _SVector *linept1, struct _SVector *linept2);

// path: C:\kain2\game\CAMERA.C
// address: 0x8001AB7C
// line start: 3798
// line end:   3835
int CAMERA_FindLinePoint(struct _Position *point, struct _SVector *linept1, struct _SVector *linept2, int target_dist_sq, struct _SVector *results)
{ //line: 1
	// address: -56
	// size: 0x8
	auto struct _SVector outPoint;
	// address: -48
	// size: 0x8
	auto struct _SVector line;
	// register: $s1
	register int calc;
	// register: $s4
	register int hits;
	{ //line: 1
		// register: $v0
		register short _x0;
		// register: $v1
		register short _y0;
		// register: $a3
		register short _z0;
		// register: $t0
		register short _x1;
		// register: $t1
		register short _y1;
		// register: $t2
		register short _z1;
		// register: $s2
		// size: 0x8
		register struct _SVector *_v;
	} //line: 1
	{ //line: 15
		// address: -40
		// size: 0x8
		auto struct _SVector linePoint;
		// register: $s3
		register int n;
		{ //line: 29
			// register: $v0
			register short _x1;
			// register: $v1
			register short _y1;
			// register: $a0
			register short _z1;
		} //line: 29
	} //line: 35
} //line: 37


// path: C:\kain2\game\CAMERA.C
// address: 0x8001AD40
// line start: 3838
// line end:   3892
void CAMERA_SplineGetNearestPoint2(struct Camera_duplicate_s2 *camera, struct Spline *spline, struct _SVector *point, int *currkey, struct _SVector *ret_dpoint)
{ //line: 1
	// address: -56
	// size: 0x20
	auto struct SplineKey *key;
	// register: $s2
	register int n;
	// address: -52
	auto int target_dist_sq;
	// register: $fp
	register long dist;
	{ //line: 1
		// register: $v0
		register short _x1;
		// register: $v1
		register short _y1;
		// register: $a0
		register short _z1;
	} //line: 1
	{ //line: 20
		// address: -88
		// size: 0x8
		auto struct _SVector point1;
		// address: -80
		// size: 0x8
		auto struct _SVector point2;
		// address: -72
		// size: 0x10
		auto struct _SVector results[2];
		// register: $s1
		register int i;
		// register: $s3
		register int hits;
		{ //line: 24
			// register: $v1
			register short _x1;
			// register: $a2
			register short _y1;
			// register: $v0
			register short _z1;
			// register: $s7
			// size: 0x8
			register struct _SVector *_v0;
			// register: $v0
			// size: 0x6
			register struct vecS *_v1;
		} //line: 24
		{ //line: 24
			// register: $v0
			register short _x1;
			// register: $a2
			register short _y1;
			// register: $v1
			register short _z1;
			// register: $s6
			// size: 0x8
			register struct _SVector *_v0;
			// register: $v1
			// size: 0x6
			register struct vecS *_v1;
		} //line: 24
		{ //line: 31
			// register: $a1
			register int tmpdist;
			{ //line: 42
				// register: $v0
				register short _x1;
				// register: $v1
				register short _y1;
				// register: $a0
				register short _z1;
			} //line: 42
		} //line: 46
	} //line: 54
} //line: 55


// path: C:\kain2\game\CAMERA.C
// address: 0x8001AF28
// line start: 3895
// line end:   3974
void CAMERA_SplineGetNearestPoint(struct Spline *spline, struct _SVector *point, int *currkey, struct _SVector *ret_dpoint)
{ //line: 1
	// address: -96
	// size: 0x8
	auto struct _SVector dpoint;
	// address: -88
	// size: 0x8
	auto struct _SVector dpoint2;
	// address: -80
	// size: 0x8
	auto struct _SVector dpoint3;
	// address: -72
	// size: 0x8
	auto struct _SVector point0;
	// address: -64
	// size: 0x8
	auto struct _SVector point1;
	// address: -56
	// size: 0x8
	auto struct _SVector point2;
	// address: -48
	// size: 0x8
	auto struct _SVector point3;
	// register: $s0
	register int current_keyframe;
	// register: $fp
	register int dist;
	// register: $s5
	register int ret;
	// register: $s4
	register int ret2;
	// register: $s0
	register int ret3;
	// register: $s1
	// size: 0x20
	register struct SplineKey *key;
	// register: $s2
	register int circular_spline;
	// register: $s2
	register int prev_key;
	// register: $s3
	register int next_key;
	{ //line: 26
		// register: $v1
		register short _x1;
		// register: $t0
		register short _y1;
		// register: $a3
		register short _z1;
		// register: $a1
		// size: 0x8
		register struct _SVector *_v0;
		// register: $a3
		// size: 0x6
		register struct vecS *_v1;
	} //line: 26
	{ //line: 26
		// register: $v0
		register short _x1;
		// register: $t0
		register short _y1;
		// register: $v1
		register short _z1;
		// register: $a2
		// size: 0x8
		register struct _SVector *_v0;
		// register: $v1
		// size: 0x6
		register struct vecS *_v1;
	} //line: 26
	{ //line: 41
		// register: $v0
		register short _x1;
		// register: $t0
		register short _y1;
		// register: $v1
		register short _z1;
		// register: $a2
		// size: 0x8
		register struct _SVector *_v0;
		// register: $v1
		// size: 0x6
		register struct vecS *_v1;
	} //line: 41
	{ //line: 54
		// register: $v0
		register short _x1;
		// register: $t0
		register short _y1;
		// register: $v1
		register short _z1;
		// register: $a1
		// size: 0x8
		register struct _SVector *_v0;
		// register: $v1
		// size: 0x6
		register struct vecS *_v1;
	} //line: 54
} //line: 80


// path: C:\kain2\game\CAMERA.C
// address: 0x8001B2C0
// line start: 3981
// line end:   4025
void CAMERA_SplineHelpMove(struct Camera_duplicate_s2 *camera)
{ //line: 1
	// address: -48
	// size: 0x8
	auto struct _SVector camPos;
	// address: -40
	// size: 0x6
	auto struct _Position pos;
	// address: -32
	// size: 0x8
	auto struct _Rotation rotation;
	// register: $s3
	// size: 0x29C
	register struct _Instance *focusInstance;
	// register: $s2
	// size: 0x50
	register struct MultiSpline *posSpline;
} //line: 45


// path: C:\kain2\game\CAMERA.C
// address: 0x8001B430
// line start: 4028
// line end:   4176
void CAMERA_SplineProcess(struct Camera_duplicate_s2 *camera)
{ //line: 1
	// address: -48
	// size: 0x8
	auto struct _SVector camPos;
	// address: -40
	// size: 0x8
	auto struct _SVector sv;
	// register: $s4
	register short targetFocusDistance;
	// register: $s3
	// size: 0x50
	register struct MultiSpline *posSpline;
	// register: $s0
	register short smooth;
	// register: $s2
	register short dist_smooth;
	// address: 92
	static short hold_flag;
	// address: -32
	// size: 0x8
	auto struct _Rotation targetFocusRotation;
	{ //line: 86
		// register: $v0
		register short _x0;
		// register: $a1
		register short _y0;
		// register: $a2
		register short _z0;
		// register: $v1
		register short _x1;
		// register: $v1
		register short _y1;
		// register: $v0
		register short _z1;
		// register: $a0
		// size: 0x8
		register struct _SVector *_v;
		// register: $v0
		// size: 0x8
		register struct _SVector *_v1;
	} //line: 86
} //line: 149


// path: C:\kain2\game\CAMERA.C
// address: 0x8001B784
// line start: 4201
// line end:   4211
void CAMERA_ShakeCamera(struct Camera_duplicate_s2 *camera);

// path: C:\kain2\game\CAMERA.C
// address: 0x8001B86C
// line start: 4220
// line end:   4382
void CAMERA_Process(struct Camera_duplicate_s2 *camera)
{ //line: 1
	// register: $a1
	// size: 0x29C
	register struct _Instance *focusInstance;
	{ //line: 60
		// register: $v0
		register short _x0;
		// register: $v1
		register short _y0;
		// register: $a0
		register short _z0;
		// register: $a1
		register short _x1;
		// register: $a3
		register short _y1;
		// register: $a2
		register short _z1;
		// register: $v0
		// size: 0x8
		register struct _SVector *_v;
		// register: $a0
		// size: 0x6
		register struct _Position *_v0;
		// register: $a2
		// size: 0x6
		register struct _Position *_v1;
	} //line: 60
} //line: 163


// path: C:\kain2\game\CAMERA.C
// address: 0x8001BAB8
// line start: 4404
// line end:   4421
void CAMERA_CenterCamera(struct Camera_duplicate_s2 *camera)
{ //line: 1
	{ //line: 5
		// register: $s0
		register int tmp1;
		// register: $v0
		register int tmp2;
	} //line: 9
} //line: 16


// path: C:\kain2\game\CAMERA.C
// address: 0x8001BBA0
// line start: 4436
// line end:   4438
void CAMERA_SetLookRot(struct Camera_duplicate_s2 *camera, int x_rotation, int z_rotation);

// path: C:\kain2\game\CAMERA.C
// address: 0x8001BBAC
// line start: 4442
// line end:   4456
void CAMERA_StartLookaroundMode(struct Camera_duplicate_s2 *camera);

// path: C:\kain2\game\CAMERA.C
// address: 0x8001BC3C
// line start: 4460
// line end:   4464
void CAMERA_StartSwimThrowMode(struct Camera_duplicate_s2 *camera);

// path: C:\kain2\game\CAMERA.C
// address: 0x8001BC7C
// line start: 4468
// line end:   4470
void CAMERA_EndSwimThrowMode(struct Camera_duplicate_s2 *camera);

// path: C:\kain2\game\CAMERA.C
// address: 0x8001BCA0
// line start: 4474
// line end:   4477
void CAMERA_ForceEndLookaroundMode(struct Camera_duplicate_s2 *camera);

// path: C:\kain2\game\CAMERA.C
// address: 0x8001BCDC
// line start: 4483
// line end:   4717
void CAMERA_Control(struct Camera_duplicate_s2 *camera, struct _Instance *playerInstance)
{ //line: 1
	// register: $s2
	register long *controlCommand;
	// register: $s1
	register int lookmode;
	// register: $s4
	// size: 0x29C
	register struct _Instance *focusInstance;
} //line: 235


// path: C:\kain2\game\CAMERA.C
// address: 0x8001C224
// line start: 4721
// line end:   4782
void CAMERA_EndLook(struct Camera_duplicate_s2 *camera)
{ //line: 1
	// register: $s1
	// size: 0x29C
	register struct _Instance *focusInstance;
} //line: 61


// path: C:\kain2\game\CAMERA.C
// address: 0x8001C318
// line start: 4786
// line end:   4809
void CAMERA_ChangeToUnderWater(struct Camera_duplicate_s2 *camera, struct _Instance *instance);

// path: C:\kain2\game\CAMERA.C
// address: 0x8001C3C4
// line start: 4813
// line end:   4828
void CAMERA_ChangeToOutOfWater(struct Camera_duplicate_s2 *camera, struct _Instance *instance);

// path: C:\kain2\game\CAMERA.C
// address: 0x8001C438
// line start: 4929
// line end:   4976
void CAMERA_UpdateFocusDistance(struct Camera_duplicate_s2 *camera)
{ //line: 1
	// register: $v1
	register int smooth;
	// register: $a0
	register long dampMode;
} //line: 48


// path: C:\kain2\game\CAMERA.C
// address: 0x8001C508
// line start: 4979
// line end:   5010
void CAMERA_UpdateFocusTilt(struct Camera_duplicate_s2 *camera);

// path: C:\kain2\game\CAMERA.C
// address: 0x8001C5D4
// line start: 5013
// line end:   5031
void CAMERA_UpdateFocusRoll(struct Camera_duplicate_s2 *camera)
{ //line: 1
	// register: $s0
	register long tmp_inc;
} //line: 18


// path: C:\kain2\game\CAMERA.C
// address: 0x8001C6F0
// line start: 5037
// line end:   5072
void CAMERA_UpdateFocusRotate(struct Camera_duplicate_s2 *camera)
{ //line: 1
	{ //line: 4
		// register: $v1
		register int dampspeed;
		// register: $a0
		register long dampmode;
	} //line: 29
} //line: 36


// path: C:\kain2\game\CAMERA.C
// address: 0x8001C7DC
// line start: 5079
// line end:   5172
void CAMERA_UpdateFocusRotationX(struct Camera_duplicate_s2 *camera, struct _Instance *focusInstance)
{ //line: 1
	// register: $a1
	register short cameraPlayerRotX;
	// register: $s1
	register short dist;
	// register: $s2
	register short tfaceFlag;
	{ //line: 23
		// address: -24
		// size: 0x8
		auto struct _Normal normal;
		// register: $v1
		register int mult;
	} //line: 49
	{ //line: 53
		// register: $t0
		register int tmpsmooth;
	} //line: 68
} //line: 94


// path: C:\kain2\game\CAMERA.C
// address: 0x8001CA58
// line start: 5178
// line end:   5254
void CAMERA_FollowPlayerTilt(struct Camera_duplicate_s2 *camera, struct _Instance *focusInstance)
{ //line: 1
	// register: $s2
	register int speed;
	// register: $s3
	register int mode;
	// register: $a0
	register long focusInstanceStatus;
	{ //line: 25
		// address: -40
		// size: 0x8
		auto struct _SVector offset;
		// address: -32
		// size: 0x8
		auto struct _Normal normal;
		{ //line: 49
			// register: $v0
			register int waterZ;
			// register: $v0
			register int target;
			// register: $s0
			register int height;
			{ //line: 57
				// register: $v0
				register int fdsq;
			} //line: 65
		} //line: 66
	} //line: 67
} //line: 72


// path: C:\kain2\game\CAMERA.C
// address: 0x8001CC68
// line start: 5259
// line end:   5297
void CAMERA_FollowGoBehindPlayerWithTimer(struct Camera_duplicate_s2 *camera)
{ //line: 1
	// register: $s1
	// size: 0x29C
	register struct _Instance *focusInstance;
} //line: 39


// path: C:\kain2\game\CAMERA.C
// address: 0x8001CD64
// line start: 5301
// line end:   5308
void CAMERA_FollowGoBehindPlayer(struct Camera_duplicate_s2 *camera)
{ //line: 1
	// register: $v0
	// size: 0x29C
	register struct _Instance *focusInstance;
} //line: 1


// path: C:\kain2\game\CAMERA.C
// address: 0x8001CDA0
// line start: 5313
// line end:   5370
void CAMERA_CalculateLead(struct Camera_duplicate_s2 *camera)
{ //line: 1
	// register: $s0
	register short lead_target;
	// register: $v1
	register short lead_smooth;
	{ //line: 7
		// register: $s0
		register int speedxy;
		// register: $a0
		register short angle;
		{ //line: 19
			// register: $v1
			register int calc;
		} //line: 36
	} //line: 41
} //line: 55


// path: C:\kain2\game\CAMERA.C
// address: 0x8001CF48
// line start: 5384
// line end:   5505
void CAMERA_CalcFollowPosition(struct Camera_duplicate_s2 *camera, struct _Rotation *rotation)
{ //line: 1
	// register: $s2
	// size: 0x29C
	register struct _Instance *focusInstance;
	{ //line: 1
		// register: $v0
		register short _x1;
		// register: $v1
		register short _y1;
		// register: $a0
		register short _z1;
		// register: $v0
		// size: 0x8
		register struct _Rotation *_v0;
	} //line: 1
	{ //line: 20
		// register: $s0
		register short target_rotx;
		// register: $s3
		register int hypotXY;
		// register: $v1
		register int smooth;
		// register: $a0
		register int diff;
		// address: -40
		// size: 0xC
		auto struct _Vector dpv;
		// register: $a0
		register int zdiff;
		{ //line: 42
			// register: $a1
			register int velz;
			{ //line: 53
				// register: $a0
				register int ground;
				// register: $v1
				register int pos;
			} //line: 68
		} //line: 68
	} //line: 116
} //line: 121


// path: C:\kain2\game\CAMERA.C
// address: 0x8001D244
// line start: 5509
// line end:   5541
void CAMERA_SetupColInfo(struct Camera_duplicate_s2 *camera, struct CameraCollisionInfo *colInfo, struct _Position *targetCamPos)
{ //line: 1
	// address: 96
	static short toggle;
	{ //line: 6
		// register: $v1
		register short _x1;
		// register: $a0
		register short _y1;
		// register: $a1
		register short _z1;
		// register: $v0
		// size: 0x6
		register struct _Position *_v0;
		// register: $v0
		// size: 0x6
		register struct _Position *_v1;
	} //line: 6
	{ //line: 10
		// register: $v1
		register short _x1;
		// register: $a0
		register short _y1;
		// register: $a1
		register short _z1;
		// register: $v0
		// size: 0x6
		register struct _Position *_v0;
		// register: $v0
		// size: 0x6
		register struct _Position *_v1;
	} //line: 10
	{ //line: 12
		// register: $v0
		register short _x1;
		// register: $a0
		register short _y1;
		// register: $a1
		register short _z1;
		// register: $v1
		// size: 0x6
		register struct _Position *_v0;
	} //line: 12
} //line: 33


// path: C:\kain2\game\CAMERA.C
// address: 0x8001D304
// line start: 5550
// line end:   5562
void CAMERA_DoPanicCheck(struct Camera_duplicate_s2 *camera, struct CameraCollisionInfo *tmpcolInfo, struct _Rotation *rotation, short *best_z, short *max_dist)
{ //line: 1
	// address: -32
	// size: 0x6
	auto struct _Position targetCamPos;
	{ //line: 1
		// register: $v1
		register short _x1;
		// register: $a3
		register short _y1;
		// register: $t0
		register short _z1;
		// register: $v0
		// size: 0x6
		register struct _Position *_v0;
		// register: $v0
		// size: 0x6
		register struct _Position *_v1;
	} //line: 1
} //line: 13


// path: C:\kain2\game\CAMERA.C
// address: 0x8001D3D4
// line start: 5565
// line end:   5614
void CAMERA_Panic(struct Camera_duplicate_s2 *camera, short min_dist)
{ //line: 1
	// address: -160
	// size: 0x6
	auto struct _Position targetCamPos;
	// address: -152
	// size: 0x8
	auto struct _Rotation rotation;
	// register: $s1
	register int n;
	// address: -48
	auto short best_z;
	// address: -46
	auto short max_dist;
	// address: -144
	// size: 0x5C
	auto struct CameraCollisionInfo tmpcolInfo;
	// register: $s7
	register short free_count1;
	// register: $s6
	register short free_count2;
} //line: 50


// path: C:\kain2\game\CAMERA.C
// address: 0x8001D584
// line start: 5619
// line end:   5870
long CAMERA_DoCameraCollision2(struct Camera_duplicate_s2 *camera, struct _Position *targetCamPos, int simpleflag)
{ //line: 1
	// register: $s0
	register int secondcheck_flag;
	// register: $s2
	register long hit;
	// address: -120
	// size: 0x5C
	auto struct CameraCollisionInfo colInfo;
	// address: 100
	static long collisiontimeDown;
	{ //line: 91
		// register: $v1
		register short _x1;
		// register: $a3
		register short _y1;
		// register: $t0
		register short _z1;
		// register: $v0
		// size: 0x6
		register struct _Position *_v0;
		// register: $v0
		// size: 0x6
		register struct _Position *_v1;
	} //line: 91
	{ //line: 147
		// register: $s0
		register int speed;
	} //line: 167
	{ //line: 197
		// register: $s0
		register int angle1;
		// register: $v0
		register int angle2;
	} //line: 213
} //line: 252


// path: C:\kain2\game\CAMERA.C
// address: 0x8001DB88
// line start: 5876
// line end:   5877
int CAMERA_FocusInstanceMoved(struct Camera_duplicate_s2 *camera);

// path: C:\kain2\game\LIGHT3D.C
// address: 0x80035ACC
// line start: 70
// line end:   194
void LIGHT_GetLightMatrix(struct _Instance *instance, struct Level *level, struct MATRIX *lightM, struct MATRIX *colorM)
{ //line: 2
	// register: $t0
	// size: 0x20
	register struct MATRIX *lightGroup;
	// register: $v1
	// size: 0xC
	register struct LightList *lightList;
	// register: $t1
	register int lightGrp;
	{ //line: 50
		// register: $t2
		// size: 0x20
		register struct MATRIX *tlightGroup;
		// register: $t3
		// size: 0xC
		register struct LightList *tlightList;
		// register: $t1
		register int tlightGrp;
		// register: $t4
		// size: 0x20
		register struct MATRIX *start;
		// register: $t3
		// size: 0x20
		register struct MATRIX *end;
		// register: $t1
		register int i;
		// register: $a2
		register int j;
		// register: $t2
		register long ratio;
	} //line: 116
} //line: 125


// path: C:\kain2\game\LIGHT3D.C
// address: 0x80035E40
// line start: 200
// line end:   275
void LIGHT_PresetInstanceLight(struct _Instance *instance, short attenuate, struct MATRIX *lm)
{ //line: 1
	// address: -80
	// size: 0x20
	auto struct MATRIX cm;
	// register: $a0
	register long scale;
	// address: -48
	// size: 0xC
	auto long scaleRGB[3];
	// register: $t3
	register int i;
	// register: $a3
	register int j;
	// register: $s2
	// size: 0x18
	register struct CDLight *extraLight;
	// register: $s3
	// size: 0x188
	register struct Level *level;
	// address: -32
	// size: 0x6
	auto short tempRGB[3];
	{ //line: 54
		// register: $v0
		register short *todRGB;
		{ //line: 65
		} //line: 71
	} //line: 72
} //line: 75


// path: C:\kain2\game\LIGHT3D.C
// address: 0x80036050
// line start: 284
// line end:   327
void LIGHT_GetAmbient(struct _ColorType *color, struct _Instance *instance)
{ //line: 2
	// register: $v1
	register int lightval;
} //line: 43


// path: C:\kain2\game\LIGHT3D.C
// address: 0x80036080
// line start: 330
// line end:   540
void LIGHT_CalcLightValue(struct _TFace *tface, struct _Instance *instance, struct _Terrain *terrain)
{ //line: 1
	// address: -88
	// size: 0x4
	auto struct _ColorType color;
	// register: $a2
	register short fadespeed;
	{ //line: 12
		// address: -80
		// size: 0x4
		auto struct _ColorType color1;
		// address: -72
		// size: 0x4
		auto struct _ColorType color2;
		// register: $v1
		register long n;
		// register: $a0
		register long count;
		// register: $a2
		register long edge;
		// register: $v1
		register int x1;
		// register: $a0
		register int x2;
		// register: $s2
		register int interp1;
		// register: $s1
		register int interp2;
		// register: $a3
		register int interp;
		// register: $v0
		register short *temp;
		// register: $s5
		register short *vertex0;
		// register: $s4
		register short *vertex1;
		// register: $s3
		register short *vertex2;
		// address: -64
		// size: 0x6
		auto short position[3];
		// address: -56
		// size: 0x8
		auto struct _SVector normal;
		// register: $a1
		// size: 0x24
		register struct BSPTree *bsp;
		// register: $a3
		register int major;
		// register: $s6
		register int minor;
		{ //line: 115
			// register: $v0
			register long r;
			// register: $a1
			register long g;
			// register: $v1
			register long b;
			// register: $a0
			register int lum;
		} //line: 136
	} //line: 136
	{ //line: 151
		// register: $t0
		register int i;
		// register: $t4
		// size: 0x14
		register struct LightInstance *li;
		// register: $t3
		register long dist;
		// register: $t2
		// size: 0x14
		register struct LightInstance *tli;
		{ //line: 158
			// register: $a1
			// size: 0x29C
			register struct _Instance *inst;
			{ //line: 161
				// register: $a3
				register short tdist;
				// address: -64
				// size: 0x6
				auto struct _Position pos;
				// register: $a1
				// size: 0x20
				register struct MATRIX *mat;
			} //line: 178
		} //line: 179
	} //line: 203
} //line: 211


// path: C:\kain2\game\LIGHT3D.C
// address: 0x800367D0
// line start: 543
// line end:   554
void LIGHT_SetAmbientInstance(struct _Instance *instance, struct Level *level)
{ //line: 1
} //line: 1


// path: C:\kain2\game\LIGHT3D.C
// address: 0x80036800
// line start: 557
// line end:   668
void LIGHT_SetMatrixForLightGroupInstance(struct _Instance *instance, struct Level *level)
{ //line: 1
	// register: $s0
	// size: 0x20
	register struct MATRIX *lgt;
	// address: -128
	// size: 0x20
	auto struct MATRIX lgt_cat;
	// address: -96
	// size: 0x20
	auto struct MATRIX lm;
	// address: -64
	// size: 0x20
	auto struct MATRIX cm;
	// address: -32
	// size: 0x10
	auto struct VECTOR half;
	// register: $s0
	// size: 0xC
	register struct LightList *lightList;
	// register: $s1
	register int lightGrp;
	{ //line: 103
		// size: 0x14
		typedef struct _229fake_duplicate_s3 cmm;
	} //line: 103
} //line: 112


// path: C:\kain2\game\LIGHT3D.C
// address: 0x80036BC8
// line start: 717
// line end:   790
void LIGHT_DrawShadow(struct MATRIX *wcTransform, struct _Instance *instance, struct _PrimPool *primPool, unsigned long **ot)
{ //line: 1
	// address: -112
	// size: 0x8
	auto struct SVECTOR face_orient;
	// address: -104
	// size: 0x20
	auto struct MATRIX rot;
	// address: -72
	// size: 0x20
	auto struct MATRIX scTransform;
	// address: -40
	// size: 0xC
	auto struct _Vector scale;
	// register: $s0
	// size: 0x29C
	register struct _Instance *playerInstance;
} //line: 74


// path: C:\kain2\game\LIGHT3D.C
// address: 0x80036EAC
// line start: 793
// line end:   896
void LIGHT_CalcShadowPositions(struct GameTracker *gameTracker)
{ //line: 1
	// register: $v0
	// size: 0x10C
	register struct _InstanceList *instanceList;
	// register: $s0
	// size: 0x29C
	register struct _Instance *instance;
	// address: -72
	// size: 0x2C
	auto struct _PCollideInfo pcollideInfo;
	// address: -24
	// size: 0x6
	auto struct _Position newPos;
	// address: -16
	// size: 0x6
	auto struct _Position oldPos;
	// register: $v0
	// size: 0x188
	register struct Level *level;
} //line: 104


// path: C:\kain2\game\LIGHT3D.C
// address: 0x800371AC
// line start: 1348
// line end:   1349
void LIGHT_Restore(struct LightInfo *lightInfo);

// path: C:\kain2\game\LIGHT3D.C
// address: 0x800371B4
// line start: 1586
// line end:   1646
void LIGHT_CalcDQPTable(struct Level *level)
{ //line: 1
	// register: $a1
	register long dqa;
	// register: $t0
	register long limit;
	{ //line: 14
	} //line: 21
	{ //line: 24
	} //line: 31
} //line: 61


// path: C:\kain2\game\LOAD3D.C
// address: 0x800372C8
// line start: 231
// line end:   239
void LOAD_InitCd();

// path: C:\kain2\game\LOAD3D.C
// address: 0x800372F8
// line start: 255
// line end:   262
void LOAD_CdSeekCallback(unsigned char intr, unsigned char *result);

// path: C:\kain2\game\LOAD3D.C
// address: 0x80037344
// line start: 267
// line end:   329
void LOAD_CdDataReady()
{ //line: 1
	{ //line: 8
		// register: $s0
		// size: 0x2C
		register struct _ReadQueueEntry *currentQueueFile;
		// register: $a1
		register long actualReadSize;
		{ //line: 25
			// register: $s1
			register int status;
		} //line: 57
	} //line: 58
} //line: 63


// path: C:\kain2\game\LOAD3D.C
// address: 0x80037464
// line start: 333
// line end:   393
void LOAD_CdReadReady(unsigned char intr, unsigned char *result)
{ //line: 1
	static int crap;
	{ //line: 11
		// address: -40
		// size: 0xC
		auto long crap[3];
		// register: $s0
		register int bytes;
		{ //line: 33
			// address: -24
			// size: 0x4
			auto struct CdlLOC loc;
		} //line: 36
	} //line: 36
} //line: 58


// path: C:\kain2\game\LOAD3D.C
// address: 0x800375C0
// line start: 396
// line end:   407
void LOAD_UpdateCheckSum(long bytes);

// path: C:\kain2\game\LOAD3D.C
// address: 0x80037604
// line start: 411
// line end:   505
void LOAD_DoCDReading()
{ //line: 1
	// register: $a0
	register long bytesLoaded;
	// register: $v0
	register long readSoFar;
	// register: $s0
	register long state;
	// register: $v1
	register long lastCheck;
} //line: 95


// path: C:\kain2\game\LOAD3D.C
// address: 0x800376B0
// line start: 513
// line end:   520
void LOAD_DoCDBufferedReading();

// path: C:\kain2\game\LOAD3D.C
// address: 0x800376CC
// line start: 524
// line end:   546
void LOAD_SetupFileToDoCDReading()
{ //line: 1
	{ //line: 1
		// address: -16
		// size: 0x4
		auto struct CdlLOC loc;
	} //line: 16
} //line: 17


// path: C:\kain2\game\LOAD3D.C
// address: 0x80037748
// line start: 599
// line end:   621
void LOAD_SetupFileToDoBufferedCDReading()
{ //line: 1
	{ //line: 1
		// address: -16
		// size: 0x4
		auto struct CdlLOC loc;
	} //line: 17
} //line: 18


// path: C:\kain2\game\LOAD3D.C
// address: 0x800377BC
// line start: 624
// line end:   697
void LOAD_ProcessReadQueue()
{ //line: 1
	{ //line: 41
		// register: $v0
		register long cdWaitTimeDiff;
		{ //line: 47
			// address: -16
			// size: 0x4
			auto struct CdlLOC loc;
		} //line: 48
	} //line: 72
} //line: 74


// path: C:\kain2\game\LOAD3D.C
// address: 0x80037960
// line start: 702
// line end:   746
char *LOAD_ReadFileFromCD(char *filename, int memType)
{ //line: 1
	// address: -40
	// size: 0x18
	auto struct CdlFILE fp;
	// register: $s0
	register int i;
	// register: $s0
	register char *readBuffer;
} //line: 45


// path: C:\kain2\game\LOAD3D.C
// address: 0x80037A40
// line start: 749
// line end:   758
void LOAD_CdReadFromBigFile(long fileOffset, unsigned long *loadAddr, long bytes, long chksumLevel, long checksum);

// path: C:\kain2\game\LOAD3D.C
// address: 0x80037A6C
// size: 0x4
// line start: 762
// line end:   821
struct _BigFileDir *LOAD_ReadDirectory(struct _BigFileDirEntry *dirEntry)
{ //line: 1
	// register: $s2
	// size: 0x4
	register struct _BigFileDir *dir;
	// register: $s1
	register long sizeOfDir;
} //line: 1


// path: C:\kain2\game\LOAD3D.C
// address: 0x80037AD4
// line start: 824
// line end:   927
void LOAD_InitCdLoader(char *bigFileName, char *voiceFileName)
{ //line: 1
	// address: -40
	// size: 0x18
	auto struct CdlFILE fp;
	// register: $s1
	register long i;
	{ //line: 68
		// register: $s0
		register char *ptr;
	} //line: 92
} //line: 104


// path: C:\kain2\game\LOAD3D.C
// address: 0x80037C28
// line start: 930
// line end:   953
int LOAD_SetupFileInfo(struct _NonBlockLoadEntry *loadEntry)
{ //line: 1
	// register: $v1
	// size: 0x10
	register struct _BigFileEntry *fileInfo;
} //line: 24


// path: C:\kain2\game\LOAD3D.C
// address: 0x80037CB0
// line start: 957
// line end:   970
void LOAD_NonBlockingReadFile(struct _NonBlockLoadEntry *loadEntry);

// path: C:\kain2\game\LOAD3D.C
// address: 0x80037D34
// line start: 977
// line end:   1002
void LOAD_CD_ReadPartOfFile(struct _NonBlockLoadEntry *loadEntry)
{ //line: 1
	// register: $v1
	// size: 0x2C
	register struct _ReadQueueEntry *currentQueueReq;
} //line: 26


// path: C:\kain2\game\LOAD3D.C
// address: 0x80037DD4
// line start: 1016
// line end:   1060
long LOAD_HashName(char *string)
{ //line: 1
	// register: $s5
	register long sum;
	// register: $s6
	register long xor;
	// register: $s4
	register long length;
	// register: $s7
	register long ext;
	// register: $v1
	register char c;
	// register: $s2
	register long strl;
	// register: $fp
	register long endPos;
	// register: $s0
	register long i;
	// register: $s3
	register char *pos;
} //line: 44


// path: C:\kain2\game\LOAD3D.C
// address: 0x80037F28
// line start: 1063
// line end:   1101
long LOAD_HashUnit(char *name)
{ //line: 2
	// register: $v1
	register int val;
	// register: $t0
	register int last;
	// register: $a2
	register int hash;
	// register: $a3
	register int num;
	// register: $t1
	register int flag;
	// register: $a0
	register char *c;
} //line: 36


// path: C:\kain2\game\LOAD3D.C
// address: 0x80037FCC
// size: 0x10
// line start: 1104
// line end:   1124
struct _BigFileEntry *LOAD_GetBigFileEntryByHash(long hash)
{ //line: 2
	// register: $a1
	register int i;
	// register: $v1
	// size: 0x10
	register struct _BigFileEntry *entry;
} //line: 21


// path: C:\kain2\game\LOAD3D.C
// address: 0x80038054
// size: 0x10
// line start: 1127
// line end:   1132
struct _BigFileEntry *LOAD_GetBigFileEntry(char *fileName)
{ //line: 1
} //line: 1


// path: C:\kain2\game\LOAD3D.C
// address: 0x8003807C
// line start: 1183
// line end:   1189
long LOAD_DoesFileExist(char *fileName)
{ //line: 1
	// register: $v0
	// size: 0x10
	register struct _BigFileEntry *entry;
} //line: 6


// path: C:\kain2\game\LOAD3D.C
// address: 0x800380B0
// line start: 1194
// line end:   1223
void LOAD_LoadTIM(long *addr, long x_pos, long y_pos, long clut_x, long clut_y)
{ //line: 1
	// address: -24
	// size: 0x8
	auto struct RECT rect;
	// register: $s0
	register long *clutAddr;
} //line: 30


// path: C:\kain2\game\LOAD3D.C
// address: 0x80038150
// line start: 1226
// line end:   1237
void LOAD_LoadTIM2(long *addr, long x_pos, long y_pos, long width, long height)
{ //line: 1
	// address: -16
	// size: 0x8
	auto struct RECT rect;
} //line: 1


// path: C:\kain2\game\LOAD3D.C
// address: 0x80038198
// line start: 1264
// line end:   1285
long LOAD_RelocBinaryData(long *data, long fileSize)
{ //line: 1
	// register: $v1
	register long *lastMoveDest;
	// register: $s3
	register long tableSize;
	// address: -32
	// size: 0x8
	auto struct RedirectList redirectListX;
	// register: $a0
	// size: 0x8
	register struct RedirectList *redirectList;
} //line: 21


// path: C:\kain2\game\LOAD3D.C
// address: 0x80038250
// line start: 1357
// line end:   1367
void LOAD_CleanUpBuffers();

// path: C:\kain2\game\LOAD3D.C
// address: 0x80038298
// line start: 1371
// line end:   1377
void *LOAD_InitBuffers();

// path: C:\kain2\game\LOAD3D.C
// address: 0x800382D4
// line start: 1381
// line end:   1388
void LOAD_InitCdStreamMode()
{ //line: 1
	// address: -16
	auto unsigned char cdMode;
} //line: 1


// path: C:\kain2\game\LOAD3D.C
// address: 0x8003831C
// line start: 1391
// line end:   1403
void LOAD_DumpCurrentDir();

// path: C:\kain2\game\LOAD3D.C
// address: 0x8003836C
// line start: 1407
// line end:   1446
int LOAD_ChangeDirectoryByID(int id)
{ //line: 1
	{ //line: 5
		// register: $v1
		register int i;
		{ //line: 12
			// register: $a0
			// size: 0x4
			register struct _BigFileDir *dir;
		} //line: 18
	} //line: 37
} //line: 40


// path: C:\kain2\game\LOAD3D.C
// address: 0x80038474
// line start: 1454
// line end:   1455
void LOAD_SetSearchDirectory(long id);

// path: C:\kain2\game\LOAD3D.C
// address: 0x80038480
// line start: 1459
// line end:   1460
long LOAD_GetSearchDirectory();

// path: C:\kain2\game\LOAD3D.C
// address: 0x8003848C
// line start: 1464
// line end:   1465
int LOAD_ChangeDirectoryFlag();

// path: C:\kain2\game\LOAD3D.C
// address: 0x80038498
// line start: 1469
// line end:   1477
void LOAD_UpdateBigFilePointers(struct _BigFileDir *oldDir, struct _BigFileDir *newDir);

// path: C:\kain2\game\LOAD3D.C
// address: 0x800384C8
// line start: 1484
// line end:   1490
int LOAD_IsFileLoading();

// path: C:\kain2\game\LOAD3D.C
// address: 0x800384D4
// line start: 1526
// line end:   1531
void LOAD_StopLoad();

// path: C:\kain2\game\PSX\MAIN.C
// address: 0x800384F8
// line start: 136
// line end:   143
void ClearDisplay();

// path: C:\kain2\game\PSX\MAIN.C
// address: 0x800385B4
// line start: 150
// line end:   154
void screen_to_vram(long *screen, int buffer);

// path: C:\kain2\game\PSX\MAIN.C
// address: 0x800385E4
// line start: 159
// line end:   166
void show_screen(char *name)
{ //line: 1
	// register: $s0
	register long *screen;
} //line: 8


// path: C:\kain2\game\PSX\MAIN.C
// address: 0x80038628
// line start: 213
// line end:   223
void play_movie_addr_80038628(char *name);

// path: C:\kain2\game\PSX\MAIN.C
// address: 0x80038670
// line start: 231
// line end:   234
void InitMainTracker(struct MainTracker *mainTracker);

// path: C:\kain2\game\PSX\MAIN.C
// address: 0x80038680
// line start: 238
// line end:   253
char *FindTextInLine(char *search_match, char *search_str)
{ //line: 2
	// register: $a2
	register char *match_pos;
} //line: 16


// path: C:\kain2\game\PSX\MAIN.C
// address: 0x80038708
// line start: 256
// line end:   261
void ExtractWorldName(char *worldName, char *levelName);

// path: C:\kain2\game\PSX\MAIN.C
// address: 0x80038754
// line start: 265
// line end:   274
void ExtractLevelNum(char *levelNum, char *levelName);

// path: C:\kain2\game\PSX\MAIN.C
// address: 0x800387C0
// line start: 363
// line end:   580
void ProcessArgs(char *baseAreaName, struct GameTracker *gameTracker)
{ //line: 1
	// address: -88
	// size: 0x20
	auto char levelNum[32];
	// address: -56
	// size: 0x20
	auto char worldName[32];
	// register: $s1
	register long *argData;
} //line: 218


// path: C:\kain2\game\PSX\MAIN.C
// address: 0x800389D8
// line start: 583
// line end:   658
void InitDisplay()
{ //line: 1
	// register: $a1
	register int i;
	// address: -16
	// size: 0x8
	auto struct RECT r;
} //line: 62


// path: C:\kain2\game\PSX\MAIN.C
// address: 0x80038B54
// line start: 661
// line end:   679
void StartTimer();

// path: C:\kain2\game\PSX\MAIN.C
// address: 0x80038BC0
// line start: 734
// line end:   749
void VblTick();

// path: C:\kain2\game\PSX\MAIN.C
// address: 0x80038C2C
// line start: 754
// line end:   761
void DrawCallback();

// path: C:\kain2\game\PSX\MAIN.C
// address: 0x80038C84
// line start: 765
// line end:   798
void FadeOutSayingLoading(struct GameTracker *gameTracker)
{ //line: 1
	// register: $s2
	// size: 0x1C
	register struct POLY_F4_SEMITRANS *transPrim;
	// register: $s3
	register unsigned long **drawot;
	// register: $s0
	register long fadeTime;
} //line: 34


// path: C:\kain2\game\PSX\MAIN.C
// address: 0x80038DD4
// line start: 801
// line end:   826
void CheckForDevStation()
{ //line: 1
	// address: -8
	auto long *a1;
	// address: -4
	auto long *a2;
} //line: 26


// path: C:\kain2\game\PSX\MAIN.C
// address: 0x80038E34
// line start: 829
// line end:   855
void MAIN_ShowLoadingScreen()
{ //line: 1
	// register: $s0
	register long *loadingScreen;
	// address: -88
	// size: 0x5
	auto char langChar[5];
	// register: $v0
	register int lang;
	{ //line: 9
		// address: -80
		// size: 0x40
		auto char filename[64];
	} //line: 10
} //line: 27


// path: C:\kain2\game\PSX\MAIN.C
// address: 0x80038EE4
// line start: 858
// line end:   864
long *MAIN_LoadTim(char *name)
{ //line: 1
} //line: 1


// path: C:\kain2\game\PSX\MAIN.C
// address: 0x80038F04
// line start: 869
// line end:   876
void init_menus(struct GameTracker *gt)
{ //line: 1
	// register: $s0
	register struct menu_t *menu;
} //line: 1


// path: C:\kain2\game\PSX\MAIN.C
// address: 0x80038F50
// line start: 881
// line end:   911
void MAIN_DoMainInit();

// path: C:\kain2\game\PSX\MAIN.C
// address: 0x80039044
// line start: 928
// line end:   932
void MAIN_InitVolume();

// path: C:\kain2\game\PSX\MAIN.C
// address: 0x80039084
// line start: 936
// line end:   942
void MAIN_ResetGame();

// path: C:\kain2\game\PSX\MAIN.C
// address: 0x800390D4
// line start: 946
// line end:   980
void MAIN_MainMenuInit()
{ //line: 1
	{ //line: 1
		// address: -72
		// size: 0x40
		auto char sfxFileName[64];
	} //line: 21
} //line: 23


// path: C:\kain2\game\PSX\MAIN.C
// address: 0x8003924C
// line start: 983
// line end:   1005
void MAIN_FreeMainMenuStuff();

// path: C:\kain2\game\PSX\MAIN.C
// address: 0x800392BC
// line start: 1018
// line end:   1033
void MAIN_StartGame();

// path: C:\kain2\game\PSX\MAIN.C
// address: 0x80039328
// line start: 1057
// line end:   1146
long MAIN_DoMainMenu(struct GameTracker *gameTracker, struct MainTracker *mainTracker, long menuPos)
{ //line: 1
	// register: $s1
	register unsigned long **drawot;
} //line: 89


// path: C:\kain2\game\PSX\MAIN.C
// address: 0x800393FC
// line start: 1157
// line end:   1712
int MainG2(void *appData)
{ //line: 1
	// register: $s2
	// size: 0x10
	register struct MainTracker *mainTracker;
	// register: $s3
	// size: 0x270
	register struct GameTracker *gameTracker;
	// register: $s5
	register long menuPos;
	// address: 5880
	bool exit;
	{ //line: 188
		// register: $s1
		// size: 0x38
		register struct InterfaceItem *item;
		// register: $s0
		register int timer;
	} //line: 248
	{ //line: 428
	} //line: 498
} //line: 549


// path: C:\kain2\game\GAMELOOP.C
// address: 0x8002DB28
// line start: 143
// line end:   185
void GAMELOOP_AllocStaticMemory();

// path: C:\kain2\game\GAMELOOP.C
// address: 0x8002DC18
// line start: 189
// line end:   279
void GAMELOOP_InitGameTracker()
{ //line: 1
	{ //line: 1
		// register: $t4
		register int i;
	} //line: 75
} //line: 81


// path: C:\kain2\game\GAMELOOP.C
// address: 0x8002DD18
// line start: 285
// line end:   293
void GAMELOOP_SystemInit(struct GameTracker *gameTracker);

// path: C:\kain2\game\GAMELOOP.C
// address: 0x8002DD50
// line start: 298
// line end:   300
void GAMELOOP_ResetGameStates();

// path: C:\kain2\game\GAMELOOP.C
// address: 0x8002DD70
// line start: 304
// line end:   321
void GAMELOOP_ClearGameTracker();

// path: C:\kain2\game\GAMELOOP.C
// address: 0x8002DDB8
// line start: 334
// line end:   342
void GAMELOOP_CalcGameTime()
{ //line: 2
	// register: $a1
	register long time;
} //line: 6


// path: C:\kain2\game\GAMELOOP.C
// address: 0x8002DE64
// line start: 345
// line end:   356
void GAMELOOP_SetGameTime(long timeOfDay)
{ //line: 1
	// register: $v1
	register long tim;
} //line: 7


// path: C:\kain2\game\GAMELOOP.C
// address: 0x8002DEF8
// line start: 362
// line end:   388
int GAMELOOP_GetTimeOfDay()
{ //line: 2
	// register: $v1
	register int timeOfDay;
} //line: 27


// path: C:\kain2\game\GAMELOOP.C
// address: 0x8002DF44
// line start: 391
// line end:   411
int GAMELOOP_GetTimeOfDayIdx(int timeOfDay);

// path: C:\kain2\game\GAMELOOP.C
// address: 0x8002DF88
// line start: 819
// line end:   825
int GAMELOOP_WaitForLoad();

// path: C:\kain2\game\GAMELOOP.C
// address: 0x8002DFC0
// size: 0x40
// line start: 836
// line end:   911
struct _StreamUnit *LoadLevels(char *baseAreaName, struct GameTracker *gameTracker)
{ //line: 1
	// address: -32
	// size: 0x8
	auto struct _SVector offset;
	// register: $s0
	// size: 0x40
	register struct _StreamUnit *streamUnit;
	// address: 12
	// size: 0x10
	static char oldArea[16];
	{ //line: 19
		// register: $v0
		register int num;
		// register: $s1
		register int waitFor;
	} //line: 41
	{ //line: 69
		// register: $a2
		register short _x0;
		// register: $a3
		register short _y0;
		// register: $v0
		register short _z0;
		// register: $v1
		register short _x1;
		// register: $t1
		register short _y1;
		// register: $t0
		register short _z1;
		// register: $a1
		// size: 0x8
		register struct _SVector *_v;
		// register: $v0
		// size: 0x6
		register struct _Position *_v0;
		// register: $t0
		// size: 0x6
		register struct _Position *_v1;
	} //line: 69
} //line: 69


// path: C:\kain2\game\GAMELOOP.C
// address: 0x8002E1C0
// line start: 914
// line end:   929
void GAMELOOP_InitStandardObjects()
{ //line: 1
	// size: 0x28
	static char *sobjects[10];
	// register: $s1
	register int i;
} //line: 16


// path: C:\kain2\game\GAMELOOP.C
// address: 0x8002E214
// line start: 933
// line end:   1116
void GAMELOOP_LevelLoadAndInit(char *baseAreaName, struct GameTracker *gameTracker)
{ //line: 1
	// register: $s1
	register long i;
	// register: $s2
	// size: 0x40
	register struct _StreamUnit *streamUnit;
	{ //line: 79
		// register: $v0
		register short _x1;
		// register: $a0
		register short _y1;
		// register: $v1
		register short _z1;
		// register: $v0
		// size: 0x6
		register struct _Position *_v0;
		// register: $v1
		// size: 0x6
		register struct _Position *_v1;
	} //line: 79
} //line: 184


// path: C:\kain2\game\GAMELOOP.C
// address: 0x8002E524
// line start: 1120
// line end:   1129
void GAMELOOP_StreamLevelLoadAndInit(char *baseAreaName, struct GameTracker *gameTracker, int toSignalNum, int fromSignalNum)
{ //line: 1
} //line: 1


// path: C:\kain2\game\GAMELOOP.C
// address: 0x8002E544
// line start: 1139
// line end:   1142
void GAMELOOP_SetScreenWipe(int time, int maxTime, int type);

// path: C:\kain2\game\GAMELOOP.C
// address: 0x8002E558
// line start: 1147
// line end:   1495
void GAMELOOP_HandleScreenWipes(unsigned long **drawot)
{ //line: 1
	// register: $s0
	register long temp;
	// register: $s3
	// size: 0x1770C
	register struct _PrimPool *primPool;
} //line: 349


// path: C:\kain2\game\GAMELOOP.C
// address: 0x8002E9B8
// line start: 1639
// line end:   1686
void UpdateFogSettings(struct _StreamUnit *currentUnit, struct Level *level)
{ //line: 1
	// register: $a2
	register int changed;
	// register: $s1
	register int setflag;
	// register: $a1
	register int FogFar;
	// register: $a0
	register int FogNear;
} //line: 48


// path: C:\kain2\game\GAMELOOP.C
// address: 0x8002EAE0
// line start: 1689
// line end:   1695
int CheckForNoBlend(struct _ColorType *Color);

// path: C:\kain2\game\GAMELOOP.C
// address: 0x8002EB28
// line start: 1699
// line end:   1711
void BlendToColor(struct _ColorType *target, struct _ColorType *current, struct _ColorType *dest);

// path: C:\kain2\game\GAMELOOP.C
// address: 0x8002EC1C
// line start: 1716
// line end:   1953
void MainRenderLevel(struct _StreamUnit *currentUnit, unsigned long **drawot)
{ //line: 1
	// register: $s0
	// size: 0x188
	register struct Level *level;
	// register: $s5
	// size: 0x270
	register struct GameTracker *gameTracker;
	// register: $s4
	// size: 0x54
	register struct _Terrain *terrain;
	// register: $s1
	register int curTree;
	// address: -48
	auto long BackColor;
	// address: -96
	// size: 0x6
	auto struct _Position cam_pos_save;
	// address: -88
	// size: 0x20
	auto struct MATRIX cam_mat_save;
	// register: $s2
	// size: 0x29C
	register struct _Instance *saveLightInstance;
	{ //line: 53
		// register: $a2
		register int time;
	} //line: 59
	{ //line: 67
		// register: $v1
		register int tod;
	} //line: 81
	{ //line: 159
		// address: -56
		// size: 0x8
		auto struct _SVector tmp;
		// register: $s0
		// size: 0x24
		register struct BSPTree *bsp;
	} //line: 216
} //line: 236


// path: C:\kain2\game\GAMELOOP.C
// address: 0x8002F11C
// line start: 1959
// line end:   1971
void StreamIntroInstancesForUnit(struct _StreamUnit *currentUnit)
{ //line: 1
} //line: 13


// path: C:\kain2\game\GAMELOOP.C
// address: 0x8002F15C
// line start: 1976
// line end:   2140
long StreamRenderLevel(struct _StreamUnit *currentUnit, struct Level *mainLevel, unsigned long **drawot, long portalFogColor)
{ //line: 1
	// register: $s4
	// size: 0x270
	register struct GameTracker *gameTracker;
	// register: $s1
	// size: 0x188
	register struct Level *level;
	// register: $s2
	// size: 0x54
	register struct _Terrain *terrain;
	// register: $s1
	register int curTree;
	// register: $fp
	register int farplanesave;
	// address: -88
	// size: 0x6
	auto struct _Position cam_pos_save;
	// address: -80
	// size: 0x20
	auto struct MATRIX cam_mat_save;
	{ //line: 86
		// address: -48
		// size: 0x8
		auto struct _SVector tmp;
		// register: $s0
		// size: 0x24
		register struct BSPTree *bsp;
	} //line: 134
	{ //line: 152
	} //line: 162
} //line: 164


// path: C:\kain2\game\GAMELOOP.C
// address: 0x8002F4A4
// line start: 2164
// line end:   2183
void GAMELOOP_FlipScreenAndDraw(struct GameTracker *gameTracker, unsigned long **drawot);

// path: C:\kain2\game\GAMELOOP.C
// address: 0x8002F558
// line start: 2196
// line end:   2219
void GAMELOOP_AddClearPrim(unsigned long **drawot, int override)
{ //line: 1
	{ //line: 5
		// register: $a0
		// size: 0x10
		register struct BLK_FILL *blkfill;
	} //line: 5
	{ //line: 21
		// register: $v1
		// size: 0x10
		register struct BLK_FILL *blkfill;
	} //line: 23
} //line: 24


// path: C:\kain2\game\GAMELOOP.C
// address: 0x8002F600
// line start: 2222
// line end:   2246
void GAMELOOP_SwitchTheDrawBuffer(unsigned long **drawot);

// path: C:\kain2\game\GAMELOOP.C
// address: 0x8002F680
// line start: 2254
// line end:   2270
void GAMELOOP_SetupRenderFunction(struct GameTracker *gameTracker);

// path: C:\kain2\game\GAMELOOP.C
// address: 0x8002F69C
// size: 0x40
// line start: 2280
// line end:   2325
struct _StreamUnit *GAMELOOP_GetMainRenderUnit()
{ //line: 1
	// register: $s0
	// size: 0x40
	register struct _StreamUnit *streamUnit;
	{ //line: 12
		// register: $s1
		// size: 0x29C
		register struct _Instance *focusInstance;
		{ //line: 33
			// register: $v0
			// size: 0x40
			register struct _StreamUnit *cameraUnit;
		} //line: 41
	} //line: 42
} //line: 46


// path: C:\kain2\game\GAMELOOP.C
// address: 0x8002F75C
// line start: 2332
// line end:   2680
void GAMELOOP_DisplayFrame(struct GameTracker *gameTracker)
{ //line: 1
	// address: -64
	auto unsigned long **drawot;
	// address: -60
	// size: 0x188
	auto struct Level *mainLevel;
	// address: -56
	// size: 0x5C
	auto struct StreamUnitPortal *streamPortal;
	// address: -52
	auto int numportals;
	// register: $s6
	register int d;
	// register: $fp
	// size: 0x40
	register struct _StreamUnit *mainStreamUnit;
	// address: -48
	auto void *savedNextPrim;
	{ //line: 112
		// register: $s3
		// size: 0x40
		register struct _StreamUnit *toStreamUnit;
		// address: -44
		auto long toStreamUnitID;
		// register: $s4
		// size: 0x5C
		register struct StreamUnitPortal *streamPortal2;
		// register: $s5
		register int i;
		// register: $s2
		register int draw;
		// address: -72
		// size: 0x8
		auto struct RECT cliprect;
		{ //line: 153
			// register: $s0
			register int streamID;
			// register: $v0
			// size: 0x29C
			register struct _Instance *instance;
		} //line: 163
	} //line: 232
} //line: 349


// path: C:\kain2\game\GAMELOOP.C
// address: 0x8002FF18
// line start: 2687
// line end:   2737
void GAMELOOP_DrawSavedOT(unsigned long **newOT)
{ //line: 1
	// register: $a1
	// size: 0x8
	register struct P_TAG *tag;
	// register: $a2
	register int y;
	{ //line: 9
		// register: $v1
		register int tpage;
	} //line: 17
	{ //line: 22
		// register: $v1
		register int tpage;
	} //line: 30
	{ //line: 35
	} //line: 46
} //line: 49


// path: C:\kain2\game\GAMELOOP.C
// address: 0x80030090
// line start: 2741
// line end:   2762
void ResetPrimPool();

// path: C:\kain2\game\GAMELOOP.C
// address: 0x8003013C
// line start: 2770
// line end:   2798
void Switch_For_Redraw()
{ //line: 2
	// register: $v1
	register unsigned long **temp;
} //line: 27


// path: C:\kain2\game\GAMELOOP.C
// address: 0x800301B8
// line start: 2801
// line end:   2802
void GAMELOOP_Set_Pause_Redraw();

// path: C:\kain2\game\GAMELOOP.C
// address: 0x800301C8
// line start: 2806
// line end:   2856
void SaveOT()
{ //line: 1
	// register: $a0
	// size: 0x8
	register struct P_TAG *tag;
	// register: $s0
	// size: 0x8
	register struct P_TAG *last;
	// register: $s1
	// size: 0x8
	register struct P_TAG *lastlast;
} //line: 51


// path: C:\kain2\game\GAMELOOP.C
// address: 0x80030368
// line start: 2859
// line end:   2869
void ResetDrawPage()
{ //line: 1
	{ //line: 1
		// register: $v0
		register unsigned long **temp;
	} //line: 1
} //line: 1


// path: C:\kain2\game\GAMELOOP.C
// address: 0x800303A8
// line start: 2891
// line end:   2892
void GAMELOOP_Set24FPS();

// path: C:\kain2\game\GAMELOOP.C
// address: 0x800303B8
// line start: 2897
// line end:   2898
void GAMELOOP_Reset24FPS();

// path: C:\kain2\game\GAMELOOP.C
// address: 0x800303C4
// line start: 2902
// line end:   2991
void GAMELOOP_DoTimeProcess()
{ //line: 1
	// register: $s1
	register int holdTime;
	{ //line: 36
		// register: $a1
		register int lockRate;
		// register: $a0
		register unsigned long last;
	} //line: 73
} //line: 89


// path: C:\kain2\game\GAMELOOP.C
// address: 0x800305C8
// line start: 2998
// line end:   3380
void GAMELOOP_Process(struct GameTracker *gameTracker)
{ //line: 1
	// register: $s2
	register int d;
	{ //line: 43
		// register: $s0
		register int useTime;
		{ //line: 46
			// register: $v0
			// size: 0x188
			register struct Level *level;
		} //line: 51
	} //line: 56
	{ //line: 160
		// register: $s0
		register int i;
		{ //line: 167
			// register: $a2
			// size: 0x24
			register struct _SFXMkr *sfxMkr;
		} //line: 179
	} //line: 181
} //line: 383


// path: C:\kain2\game\GAMELOOP.C
// address: 0x80030BFC
// line start: 3385
// line end:   3411
void GAMELOOP_ModeStartRunning();

// path: C:\kain2\game\GAMELOOP.C
// address: 0x80030CD4
// line start: 3415
// line end:   3441
void GAMELOOP_ModeStartPause();

// path: C:\kain2\game\GAMELOOP.C
// address: 0x80030DAC
// line start: 3467
// line end:   3634
void GAMELOOP_ChangeMode()
{ //line: 1
	// register: $s0
	register long *controlCommand;
} //line: 168


// path: C:\kain2\game\GAMELOOP.C
// address: 0x80031158
// line start: 3638
// line end:   3647
void GAMELOOP_RequestLevelChange(char *name, short number, struct GameTracker *gameTracker);

// path: C:\kain2\game\GAMELOOP.C
// address: 0x800311D8
// line start: 3660
// line end:   3663
void PSX_GameLoop(struct GameTracker *gameTracker);

// path: C:\kain2\game\GAMELOOP.C
// address: 0x80031204
// size: 0x20
// line start: 3670
// line end:   3688
struct MATRIX *GAMELOOP_GetMatrices(int numMatrices)
{ //line: 2
	// register: $a1
	// size: 0x20
	register struct MATRIX *matrix;
	// register: $v1
	// size: 0x1770C
	register struct _PrimPool *pool;
} //line: 19


// path: C:\kain2\game\GAMELOOP.C
// address: 0x80031238
// size: 0x270
// line start: 3694
// line end:   3695
struct GameTracker *GAMELOOP_GetGT();

// path: C:\kain2\game\SIGNAL.C
// address: 0x8001DBF8
// line start: 76
// line end:   81
long SIGNAL_HandleLightGroup(struct _Instance *instance, struct Signal_duplicate_s3 *signal);

// path: C:\kain2\game\SIGNAL.C
// address: 0x8001DC14
// line start: 85
// line end:   87
long SIGNAL_HandleCameraAdjust(struct _Instance *instance, struct Signal_duplicate_s3 *signal);

// path: C:\kain2\game\SIGNAL.C
// address: 0x8001DC3C
// line start: 91
// line end:   93
long SIGNAL_HandleCamera(struct _Instance *instance, struct Signal_duplicate_s3 *signal);

// path: C:\kain2\game\SIGNAL.C
// address: 0x8001DC64
// line start: 97
// line end:   98
void SIGNAL_RelocateCamera(struct Signal_duplicate_s3 *signal, long offset);

// path: C:\kain2\game\SIGNAL.C
// address: 0x8001DC80
// line start: 102
// line end:   106
long SIGNAL_HandleCameraMode(struct _Instance *instance, struct Signal_duplicate_s3 *signal);

// path: C:\kain2\game\SIGNAL.C
// address: 0x8001DCA8
// line start: 110
// line end:   112
long SIGNAL_HandleCameraLock(struct _Instance *instance, struct Signal_duplicate_s3 *signal);

// path: C:\kain2\game\SIGNAL.C
// address: 0x8001DCD0
// line start: 116
// line end:   118
long SIGNAL_HandleCameraUnlock(struct _Instance *instance, struct Signal_duplicate_s3 *signal);

// path: C:\kain2\game\SIGNAL.C
// address: 0x8001DCF8
// line start: 122
// line end:   124
long SIGNAL_HandleCameraSmooth(struct _Instance *instance, struct Signal_duplicate_s3 *signal);

// path: C:\kain2\game\SIGNAL.C
// address: 0x8001DD20
// line start: 128
// line end:   130
long SIGNAL_HandleCameraTimer(struct _Instance *instance, struct Signal_duplicate_s3 *signal);

// path: C:\kain2\game\SIGNAL.C
// address: 0x8001DD48
// line start: 134
// line end:   136
long SIGNAL_HandleCameraSave(struct _Instance *instance, struct Signal_duplicate_s3 *signal);

// path: C:\kain2\game\SIGNAL.C
// address: 0x8001DD70
// line start: 140
// line end:   142
long SIGNAL_HandleCameraRestore(struct _Instance *instance, struct Signal_duplicate_s3 *signal);

// path: C:\kain2\game\SIGNAL.C
// address: 0x8001DD98
// line start: 146
// line end:   152
long SIGNAL_HandleCameraValue(struct _Instance *instance, struct Signal_duplicate_s3 *signal);

// path: C:\kain2\game\SIGNAL.C
// address: 0x8001DDC8
// line start: 157
// line end:   263
long SIGNAL_HandleStreamLevel(struct _Instance *instance, struct Signal_duplicate_s3 *signal)
{ //line: 1
	// address: -48
	// size: 0x10
	auto char areaName[16];
	// register: $s5
	register int signalnum;
	// register: $s0
	register char *commapos;
	// register: $s1
	register long newStreamID;
	// address: 4
	static int lastTimeCrossed;
	// register: $s3
	register long doingWarpRoom;
	{ //line: 20
		// register: $v0
		// size: 0x40
		register struct _StreamUnit *curStreamUnit;
	} //line: 42
	{ //line: 89
	} //line: 104
} //line: 107


// path: C:\kain2\game\SIGNAL.C
// address: 0x8001E024
// line start: 267
// line end:   286
long SIGNAL_HandleFogNear(struct _Instance *instance, struct Signal_duplicate_s3 *signal)
{ //line: 1
	// register: $s0
	// size: 0x188
	register struct Level *level;
} //line: 1


// path: C:\kain2\game\SIGNAL.C
// address: 0x8001E078
// line start: 289
// line end:   307
long SIGNAL_HandleFogFar(struct _Instance *instance, struct Signal_duplicate_s3 *signal)
{ //line: 1
	// register: $s0
	// size: 0x188
	register struct Level *level;
} //line: 1


// path: C:\kain2\game\SIGNAL.C
// address: 0x8001E0D4
// line start: 316
// line end:   318
long SIGNAL_HandleCameraShake(struct _Instance *instance, struct Signal_duplicate_s3 *signal);

// path: C:\kain2\game\SIGNAL.C
// address: 0x8001E104
// line start: 322
// line end:   324
long SIGNAL_HandleCallSignal(struct _Instance *instance, struct Signal_duplicate_s3 *signal);

// path: C:\kain2\game\SIGNAL.C
// address: 0x8001E12C
// line start: 328
// line end:   331
long SIGNAL_HandleStopPlayerControl(struct _Instance *instance, struct Signal_duplicate_s3 *signal);

// path: C:\kain2\game\SIGNAL.C
// address: 0x8001E144
// line start: 335
// line end:   338
long SIGNAL_HandleStartPlayerControl(struct _Instance *instance, struct Signal_duplicate_s3 *signal);

// path: C:\kain2\game\SIGNAL.C
// address: 0x8001E164
// line start: 502
// line end:   503
void SIGNAL_RelocateCameraSpline(struct Signal_duplicate_s3 *signal, long offset);

// path: C:\kain2\game\SIGNAL.C
// address: 0x8001E180
// line start: 507
// line end:   533
long SIGNAL_HandleCameraSpline(struct _Instance *instance, struct Signal_duplicate_s3 *signal);

// path: C:\kain2\game\SIGNAL.C
// address: 0x8001E200
// line start: 539
// line end:   543
long SIGNAL_HandleScreenWipe(struct _Instance *instance, struct Signal_duplicate_s3 *signal);

// path: C:\kain2\game\SIGNAL.C
// address: 0x8001E238
// line start: 547
// line end:   557
long SIGNAL_HandleBlendStart(struct _Instance *instance, struct Signal_duplicate_s3 *signal);

// path: C:\kain2\game\SIGNAL.C
// address: 0x8001E240
// line start: 561
// line end:   565
long SIGNAL_HandleScreenWipeColor(struct _Instance *instance, struct Signal_duplicate_s3 *signal);

// path: C:\kain2\game\SIGNAL.C
// address: 0x8001E26C
// line start: 569
// line end:   580
long SIGNAL_HandleSetSlideAngle(struct _Instance *instance, struct Signal_duplicate_s3 *signal)
{ //line: 1
	// register: $a2
	register long slideAngle;
} //line: 11


// path: C:\kain2\game\SIGNAL.C
// address: 0x8001E29C
// line start: 583
// line end:   591
long SIGNAL_HandleResetSlideAngle(struct _Instance *instance, struct Signal_duplicate_s3 *signal);

// path: C:\kain2\game\SIGNAL.C
// address: 0x8001E2C8
// line start: 595
// line end:   610
long SIGNAL_HandleSetCameraTilt(struct _Instance *instance, struct Signal_duplicate_s3 *signal)
{ //line: 1
	// register: $v0
	register long tilt;
} //line: 15


// path: C:\kain2\game\SIGNAL.C
// address: 0x8001E354
// line start: 613
// line end:   624
long SIGNAL_HandleSetCameraDistance(struct _Instance *instance, struct Signal_duplicate_s3 *signal)
{ //line: 1
	// register: $a1
	register long distance;
} //line: 11


// path: C:\kain2\game\SIGNAL.C
// address: 0x8001E384
// line start: 744
// line end:   745
long SIGNAL_HandleEnd(struct _Instance *instance, struct Signal_duplicate_s3 *signal);

// path: C:\kain2\game\SIGNAL.C
// address: 0x8001E38C
// line start: 844
// line end:   899
void COLLIDE_HandleSignal(struct _Instance *instance, struct Signal_duplicate_s3 *signal, long numSignals, int dontForceDoSignal)
{ //line: 1
	// register: $v1
	register long signalNumber;
} //line: 56


// path: C:\kain2\game\SIGNAL.C
// address: 0x8001E490
// line start: 902
// line end:   920
long SIGNAL_IsThisStreamAWarpGate(struct Signal_duplicate_s3 *signal)
{ //line: 1
	// register: $s0
	register long result;
	// address: -40
	// size: 0x20
	auto char areaName[32];
	// register: $v0
	register char *commapos;
} //line: 18


// path: C:\kain2\game\SIGNAL.C
// address: 0x8001E4F0
// line start: 923
// line end:   966
long SIGNAL_IsStreamSignal(struct Signal_duplicate_s3 *signal, long *isWarpGate)
{ //line: 1
	// register: $s3
	register long result;
	// register: $s2
	register long done;
	// register: $s0
	register long signalNumber;
} //line: 43


// path: C:\kain2\game\SIGNAL.C
// address: 0x8001E5C8
// line start: 969
// line end:   970
void SIGNAL_HandleSignal(struct _Instance *instance, struct Signal_duplicate_s3 *signal, int dontForceDoSignal);

// path: C:\kain2\game\SIGNAL.C
// address: 0x8001E5EC
// size: 0x388
// line start: 976
// line end:   999
struct _MultiSignal *SIGNAL_RelocateSignal(struct _MultiSignal *multiSignal, long offset)
{ //line: 1
	// register: $s1
	register int i;
	// register: $s0
	// size: 0x1C
	register struct Signal_duplicate_s3 *signal;
} //line: 23


// path: C:\kain2\game\SIGNAL.C
// address: 0x8001E6C8
// size: 0x388
// line start: 1002
// line end:   1045
struct _MultiSignal *SIGNAL_FindSignal(struct Level *level, long id)
{ //line: 2
	// register: $a2
	register int i;
	// register: $a3
	// size: 0x1C
	register struct Signal_duplicate_s3 *signal;
	// register: $v1
	// size: 0x388
	register struct _MultiSignal *msignal;
	// register: $v1
	register long signalNumber;
} //line: 43


// path: C:\kain2\game\SIGNAL.C
// address: 0x8001E778
// line start: 1049
// line end:   1059
void SIGNAL_OutOfWater(struct _Instance *instance)
{ //line: 1
	// register: $v0
	// size: 0x188
	register struct Level *level;
} //line: 11


// path: C:\kain2\game\SIGNAL.C
// address: 0x8001E7C8
// line start: 1062
// line end:   1071
void SIGNAL_InWater(struct _Instance *instance)
{ //line: 1
	// register: $v0
	// size: 0x188
	register struct Level *level;
} //line: 10


// path: C:\kain2\game\COLLIDE.C
// address: 0x8001E818
// line start: 243
// line end:   287
int COLLIDE_PointInTriangle(struct _SVector *v0, struct _SVector *v1, struct _SVector *v2, struct _SVector *point, struct _SVector *normal)
{ //line: 1
	// register: $t2
	// size: 0xC
	register struct _Triangle2D *triangle;
	// register: $t1
	register int nx;
	// register: $v1
	register int ny;
	// register: $t0
	register short tx;
	// register: $a3
	register short ty;
	{ //line: 1
		// register: $t2
		// size: 0xC
		register struct _Triangle2D *tri;
		// register: $t0
		register short tx;
		// register: $a3
		register short ty;
		{ //line: 1
			// register: $t3
			register int inside_flag;
			// register: $t7
			register int line_flag;
			// register: $t2
			// size: 0x4
			register struct DVECTOR *vert1;
			// register: $t1
			// size: 0x4
			register struct DVECTOR *vert0;
			// register: $t5
			register int yflag1;
			// register: $v1
			register int yflag0;
			// register: $t6
			register int j;
			{ //line: 1
				// register: $v0
				register long ydist;
				// register: $v0
				register long xdist;
				// register: $v1
				register long xdiff;
				// register: $a1
				register long ydiff;
				// register: $v1
				register long ix;
			} //line: 1
		} //line: 1
	} //line: 1
} //line: 45


// path: C:\kain2\game\COLLIDE.C
// address: 0x8001EABC
// line start: 296
// line end:   301
int COLLIDE_PointInTriangle2DPub(short *v0, short *v1, short *v2, short *point)
{ //line: 1
	// address: -16
	// size: 0x8
	auto struct _SVector normal;
} //line: 1


// path: C:\kain2\game\COLLIDE.C
// address: 0x8001EB08
// line start: 312
// line end:   339
long COLLIDE_GetNormal(short nNum, short *nrmlArray, struct _SVector *nrml)
{ //line: 2
	// register: $a1
	register short *sPtr;
	// register: $v1
	register long bitMask;
} //line: 27


// path: C:\kain2\game\COLLIDE.C
// address: 0x8001EBA0
// line start: 368
// line end:   398
void COLLIDE_MakeNormal(struct _Terrain *terrain, struct _TFace *tface, struct _SVector *normal)
{ //line: 1
	// register: $v1
	// size: 0x8
	register struct _SVector *vertex0;
	// register: $v0
	// size: 0x8
	register struct _SVector *vertex1;
	// register: $a2
	// size: 0x8
	register struct _SVector *vertex2;
	// register: $a0
	register int len;
	// register: $v0
	// size: 0xC
	register struct _Vector *a;
	// register: $v0
	// size: 0xC
	register struct _Vector *b;
	// register: $t0
	// size: 0xC
	register struct _Vector *n;
	{ //line: 1
		// register: $t1
		register long _x0;
		// register: $t2
		register long _y0;
		// register: $t0
		register long _z0;
		// register: $a0
		register long _x1;
		// register: $v0
		register long _y1;
		// register: $a0
		register long _z1;
	} //line: 1
	{ //line: 1
		// register: $a0
		register long _x0;
		// register: $a1
		register long _y0;
		// register: $a3
		register long _z0;
		// register: $v0
		register long _x1;
		// register: $v0
		register long _y1;
		// register: $v0
		register long _z1;
	} //line: 1
} //line: 31


// path: C:\kain2\game\COLLIDE.C
// address: 0x8001ED78
// line start: 415
// line end:   447
void COLLIDE_UpdateAllTransforms(struct _Instance *instance, struct SVECTOR *offset)
{ //line: 2
	// register: $v1
	// size: 0x20
	register struct MATRIX *swTransform;
	{ //line: 6
		// register: $a2
		register int i;
		// register: $v0
		register long numMatrices;
		// register: $t1
		register long ox;
		// register: $t0
		register long oy;
		// register: $a1
		register long oz;
	} //line: 32
} //line: 33


// path: C:\kain2\game\COLLIDE.C
// address: 0x8001EE48
// line start: 450
// line end:   480
void COLLIDE_MoveAllTransforms(struct _Instance *instance, struct _Position *offset)
{ //line: 2
	// register: $v1
	// size: 0x20
	register struct MATRIX *swTransform;
	{ //line: 6
		// register: $a2
		register int i;
		// register: $t1
		register long ox;
		// register: $t0
		register long oy;
		// register: $a1
		register long oz;
		// register: $v0
		register long numMatrices;
	} //line: 30
} //line: 31


// path: C:\kain2\game\COLLIDE.C
// address: 0x8001EF18
// line start: 590
// line end:   591
long COLLIDE_WithinYZBounds(struct _SVector *point, struct _HBox *hbox);

// path: C:\kain2\game\COLLIDE.C
// address: 0x8001EF74
// line start: 596
// line end:   597
long COLLIDE_WithinXZBounds(struct _SVector *point, struct _HBox *hbox);

// path: C:\kain2\game\COLLIDE.C
// address: 0x8001EFD0
// line start: 602
// line end:   603
long COLLIDE_WithinXYBounds(struct _SVector *point, struct _HBox *hbox);

// path: C:\kain2\game\COLLIDE.C
// address: 0x8001F02C
// line start: 611
// line end:   642
void COLLIDE_LineWithBoxFace(short startDist, long lineDist, short planeDist, struct _SVector *start, struct _Vector *line, struct _HBox *hbox, long (*collideBoundFunc)(), struct _SVector *normal)
{ //line: 1
	// address: -24
	// size: 0x8
	auto struct _SVector point;
	// address: -16
	auto long t;
	{ //line: 1
		// register: $t1
		// size: 0x8
		register struct _SVector *point;
		// register: $s0
		// size: 0x8
		register struct _SVector *normal;
		// register: $a2
		// size: 0x8
		register struct _SVector *point0;
		// register: $a3
		// size: 0x8
		register struct _SVector *normal0;
		// register: $a1
		// size: 0x8
		register struct _SVector *point1;
		// register: $t0
		// size: 0x8
		register struct _SVector *normal1;
		{ //line: 1
			{ //line: 1
				// register: $a0
				register short _z1;
				// register: $v1
				register short _y1;
				// register: $v0
				register short _x1;
			} //line: 1
			{ //line: 1
				// register: $a0
				register short _z1;
				// register: $v1
				register short _y1;
				// register: $v0
				register short _x1;
			} //line: 1
			{ //line: 1
				// register: $a0
				register short _z1;
				// register: $v1
				register short _y1;
				// register: $v0
				register short _x1;
			} //line: 1
			{ //line: 1
				// register: $a0
				register short _z1;
				// register: $v1
				register short _y1;
				// register: $v0
				register short _x1;
			} //line: 1
			{ //line: 1
				// register: $a0
				register short _z1;
				// register: $v1
				register short _y1;
				// register: $v0
				register short _x1;
			} //line: 1
			{ //line: 1
				// register: $a0
				register short _z1;
				// register: $v1
				register short _y1;
				// register: $v0
				register short _x1;
			} //line: 1
		} //line: 1
	} //line: 1
} //line: 32


// path: C:\kain2\game\COLLIDE.C
// address: 0x8001F1D8
// line start: 650
// line end:   710
long COLLIDE_IntersectLineAndBox(struct _SVector *point0, struct _SVector *normal0, struct _SVector *point1, struct _SVector *normal1, struct _SVector *end, struct _SVector *start, struct _HBox *hbox)
{ //line: 1
	// address: -64
	// size: 0x8
	auto struct _SVector normal;
	// address: -56
	// size: 0xC
	auto struct _Vector line;
	{ //line: 1
		// register: $a1
		register long _x0;
		// register: $a2
		register long _y0;
		// register: $a0
		register long _z0;
		// register: $v0
		register long _x1;
		// register: $v0
		register long _y1;
		// register: $v1
		register long _z1;
		// register: $s3
		// size: 0xC
		register struct _Vector *_v;
	} //line: 1
} //line: 61


// path: C:\kain2\game\COLLIDE.C
// address: 0x8001F454
// size: 0xC
// line start: 770
// line end:   771
struct _TFace *COLLIDE_PointAndTerrain(struct _Terrain *terrain, struct _PCollideInfo *pcollideInfo, struct _LCollideInfo *lcol);

// path: C:\kain2\game\COLLIDE.C
// address: 0x8001F484
// size: 0xC
// line start: 780
// line end:   1128
struct _TFace *COLLIDE_PointAndTerrainFunc(struct _Terrain *terrain, struct _PCollideInfo *pCollideInfo, int Flags, short *Backface_Flag, long ignoreAttr, long acceptAttr, struct _LCollideInfo *lcolinfo)
{ //line: 1
	// register: $s0
	// size: 0x7C
	register struct PandTScratch *CSpad;
	// register: $s1
	register void **stack;
	// register: $a1
	// size: 0x2C
	register struct _BSPNode *bspNode;
	// address: -48
	auto int curTree;
	// address: -80
	// size: 0x8
	auto struct _SVector *NewPos;
	// register: $a0
	// size: 0x8
	register struct _SVector *OldPos;
	// register: $s7
	// size: 0x29C
	register struct _Instance *instance;
	{ //line: 1
		// register: $a3
		register short _x1;
		// register: $t0
		register short _y1;
		// register: $t1
		register short _z1;
		// register: $v1
		// size: 0x8
		register struct _SVector *_v0;
	} //line: 1
	{ //line: 1
		// register: $v0
		register short _x1;
		// register: $v1
		register short _y1;
		// register: $a0
		register short _z1;
		// register: $a2
		// size: 0x8
		register struct _SVector *_v0;
	} //line: 1
	{ //line: 1
		// register: $a1
		// size: 0x8
		register struct _SVector *_v;
	} //line: 1
	{ //line: 57
		// register: $s4
		// size: 0x24
		register struct BSPTree *bsp;
		{ //line: 85
			// register: $v0
			register short _x0;
			// register: $v1
			register short _y0;
			// register: $a0
			register short _z0;
			// register: $a1
			register short _x1;
			// register: $a2
			register short _y1;
			// register: $t0
			register short _z1;
			// register: $s6
			// size: 0x8
			register struct _SVector *_v;
			// register: $a3
			// size: 0x6
			register struct _Position *_v1;
		} //line: 85
		{ //line: 85
			// register: $v0
			register short _x0;
			// register: $v1
			register short _y0;
			// register: $a0
			register short _z0;
			// register: $a1
			register short _x1;
			// register: $a2
			register short _y1;
			// register: $a3
			register short _z1;
			// register: $s5
			// size: 0x8
			register struct _SVector *_v;
		} //line: 85
		{ //line: 118
			// register: $s3
			// size: 0xC
			register struct _TFace *tface;
			{ //line: 125
				// register: $a0
				// size: 0x8
				register struct _SVector *vertex0;
				{ //line: 1
					// register: $a0
					register short *nrmlArray;
					// register: $a1
					// size: 0x8
					register struct _SVector *nrml;
					{ //line: 1
						// register: $v1
						register short *sPtr;
					} //line: 1
				} //line: 1
				{ //line: 1
					// register: $t0
					// size: 0x8
					register struct _SVector *point;
					// register: $a3
					// size: 0x8
					register struct _SVector *start;
					// register: $a2
					// size: 0x8
					register struct _SVector *line;
					{ //line: 1
						// register: $v1
						register long len;
						// register: $a1
						register long t;
					} //line: 1
				} //line: 1
				{ //line: 170
					// register: $v0
					register short _x0;
					// register: $v1
					register short _y0;
					// register: $a1
					register short _z0;
					// register: $a0
					register short _x1;
					// register: $a3
					register short _y1;
					// register: $a2
					register short _z1;
					// register: $a2
					// size: 0x6
					register struct _Position *_v1;
				} //line: 170
				{ //line: 235
					// register: $v0
					register short _x0;
					// register: $v1
					register short _y0;
					// register: $a1
					register short _z0;
					// register: $a0
					register short _x1;
					// register: $a2
					register short _y1;
					// register: $a3
					register short _z1;
					// register: $fp
					// size: 0x8
					register struct _SVector *_v;
				} //line: 235
			} //line: 256
		} //line: 269
		{ //line: 273
			// register: $v1
			register int plane_front_error;
			// register: $a0
			register int plane_back_error;
		} //line: 340
		{ //line: 344
			// register: $v0
			register short _x0;
			// register: $v1
			register short _y0;
			// register: $a0
			register short _z0;
			// register: $a1
			register short _x1;
			// register: $a2
			register short _y1;
			// register: $t0
			register short _z1;
			// register: $a3
			// size: 0x6
			register struct _Position *_v1;
		} //line: 344
		{ //line: 344
			// register: $v0
			register short _x0;
			// register: $v1
			register short _y0;
			// register: $a0
			register short _z0;
			// register: $a1
			register short _x1;
			// register: $a2
			register short _y1;
			// register: $a3
			register short _z1;
		} //line: 345
	} //line: 346
	{ //line: 347
		// register: $v1
		register short _x1;
		// register: $a0
		register short _y1;
		// register: $v0
		register short _z1;
		// register: $v0
		// size: 0x8
		register struct _SVector *_v1;
	} //line: 347
} //line: 347


// path: C:\kain2\game\COLLIDE.C
// address: 0x8002001C
// line start: 1330
// line end:   1377
int COLLIDE_PointAndHFace(struct _SVector *newPos, struct _SVector *oldPos, struct _HFace *hface, struct _Model *model, struct _SVector *hfNormal)
{ //line: 1
	// register: $s1
	// size: 0x20
	register struct MATRIX *posMatrix;
	// register: $s3
	// size: 0x8
	register struct _SVector *normal;
	// register: $s2
	// size: 0xC
	register struct _Vector *dpv;
	// address: -48
	// size: 0x8
	auto struct _SVector *planePoint;
	// register: $s0
	// size: 0x8
	register struct _SVector *vertex0;
	// register: $s1
	// size: 0x8
	register struct _SVector *vertex1;
	{ //line: 11
		// register: $v0
		register short _x0;
		// register: $v1
		register short _y0;
		// register: $a1
		register short _z0;
		// register: $a2
		register short _x1;
		// register: $a3
		register short _y1;
		// register: $t0
		register short _z1;
	} //line: 11
	{ //line: 11
		// register: $v0
		register short _x0;
		// register: $v1
		register short _y0;
		// register: $a1
		register short _z0;
		// register: $a2
		register short _x1;
		// register: $a3
		register short _y1;
		// register: $t0
		register short _z1;
		// register: $t1
		// size: 0x8
		register struct _SVector *_v;
	} //line: 11
	{ //line: 11
		// register: $v0
		register short _x1;
		// register: $v1
		register short _y1;
		// register: $a1
		register short _z1;
		// register: $t3
		// size: 0x8
		register struct _SVector *_v0;
	} //line: 11
} //line: 48


// path: C:\kain2\game\COLLIDE.C
// address: 0x80020264
// line start: 1389
// line end:   1581
void COLLIDE_PointAndInstance(struct _PCollideInfo *pcollideInfo, struct _Instance *instance)
{ //line: 1
	// address: -92
	// size: 0x20
	auto struct MATRIX *swTransform;
	// address: -88
	// size: 0x20
	auto struct MATRIX *wsTransform;
	// register: $fp
	// size: 0x20
	register struct MATRIX *swNormMat;
	// address: -84
	// size: 0xC
	auto struct _Vector *oldPosVec;
	// address: -80
	// size: 0xC
	auto struct _Vector *lNormal;
	// address: -76
	// size: 0xC
	auto struct _Vector *wNormal;
	// register: $s2
	// size: 0xC
	register struct _Vector *dv;
	// register: $s5
	// size: 0xC
	register struct _Vector *newPosVec;
	// address: -72
	// size: 0x8
	auto struct _SVector *oldPos;
	// register: $s3
	// size: 0x8
	register struct _SVector *newPos;
	// address: -68
	// size: 0x8
	auto struct _SVector *point;
	// address: -64
	auto long *flag;
	// address: -60
	// size: 0x38
	auto struct _Model *model;
	// register: $v0
	// size: 0x8
	register struct _HModel *hmodel;
	// address: -56
	// size: 0x8
	auto struct _HPrim_duplicate_s3 *hprim;
	// address: -52
	auto int i;
	// address: -48
	auto long collideType;
	{ //line: 27
		// register: $s0
		// size: 0x1C
		register struct _HBox *hbox;
		// register: $s1
		// size: 0xC
		register struct _HFace *hface;
		// register: $s0
		// size: 0x14
		register struct _HSphere *hsphere;
		{ //line: 30
			// size: 0xC
			typedef struct _258fake tmm;
		} //line: 30
		{ //line: 63
			// register: $t0
			register long len;
			{ //line: 64
				// address: -104
				auto long a;
				// address: -100
				auto long b;
				// address: -96
				auto long c;
			} //line: 64
		} //line: 70
		{ //line: 99
			// address: -136
			// size: 0x8
			auto struct _SVector hfNormal;
		} //line: 105
		{ //line: 120
			// address: -136
			// size: 0x8
			auto struct _SVector hbNormal;
			// address: -128
			// size: 0x8
			auto struct _SVector point0;
			// address: -120
			// size: 0x8
			auto struct _SVector point1;
			// address: -112
			// size: 0x8
			auto struct _SVector normal1;
			{ //line: 162
				// register: $v0
				register short _x1;
				// register: $v1
				register short _y1;
				// register: $a3
				register short _z1;
			} //line: 162
		} //line: 172
	} //line: 191
} //line: 193


// path: C:\kain2\game\COLLIDE.C
// address: 0x80020A98
// line start: 1584
// line end:   1632
void COLLIDE_PointAndInstanceTrivialReject(struct _PCollideInfo *pcollideInfo, struct _Instance *instance)
{ //line: 1
	// register: $s2
	// size: 0xC
	register struct _Vector *dv;
	// address: -32
	// size: 0x8
	auto struct _SVector linePoint;
	{ //line: 12
		// register: $a2
		register long _x0;
		// register: $v1
		register long _y0;
		// register: $v0
		register long _z0;
		// register: $v0
		register long _x1;
		// register: $v0
		register long _y1;
		// register: $a0
		register long _z1;
		// register: $a0
		// size: 0x8
		register struct _SVector *_v0;
	} //line: 12
} //line: 49


// path: C:\kain2\game\COLLIDE.C
// address: 0x80020C1C
// line start: 1648
// line end:   1812
void COLLIDE_PointAndWorld(struct _PCollideInfo *pcollideInfo, struct Level *level)
{ //line: 1
	// register: $s0
	// size: 0x29C
	register struct _Instance *instance;
	// register: $s3
	// size: 0x10C
	register struct _InstanceList *instanceList;
	// address: -56
	// size: 0xC
	auto struct _LCollideInfo lcol;
	// register: $s1
	register int i;
	// register: $s7
	register int in_warpRoom;
	{ //line: 12
		// register: $s3
		// size: 0xC
		register struct _TFace *tface;
		// register: $s4
		// size: 0x54
		register struct _Terrain *terrain;
		{ //line: 42
		} //line: 46
		{ //line: 51
			{ //line: 56
				// register: $s0
				// size: 0x188
				register struct Level *thislevel;
			} //line: 80
		} //line: 91
	} //line: 92
} //line: 165


// path: C:\kain2\game\COLLIDE.C
// address: 0x80021004
// line start: 1893
// line end:   1941
long COLLIDE_ClosestPointInBoxToPoint(struct _Position *boxPoint, struct _HBox *hbox, struct _SVector *point)
{ //line: 2
	// register: $a3
	register long inside;
} //line: 48


// path: C:\kain2\game\COLLIDE.C
// address: 0x800210D8
// line start: 1944
// line end:   1969
long COLLIDE_SphereAndPoint(struct _Sphere *sphere, struct _SVector *point, struct _SVector *normal)
{ //line: 1
	// register: $a2
	register long len;
	// register: $s0
	// size: 0xC
	register struct _Vector *line;
	{ //line: 1
		// register: $a2
		register long _x0;
		// register: $a1
		register long _y0;
		// register: $a0
		register long _z0;
		// register: $v0
		register long _x1;
		// register: $v0
		register long _y1;
		// register: $v0
		register long _z1;
	} //line: 1
	{ //line: 9
		// address: -40
		auto long a;
		// address: -36
		auto long b;
		// address: -32
		auto long c;
	} //line: 9
} //line: 26


// path: C:\kain2\game\COLLIDE.C
// address: 0x8002131C
// line start: 1975
// line end:   2107
long COLLIDE_SphereAndHBox(struct _HBox *hbox, struct _Sphere *sphere, struct _Position *oldPos, struct _SVector *normal)
{ //line: 1
	// address: -96
	// size: 0x8
	auto struct _SVector point;
	// address: -88
	// size: 0x8
	auto struct _SVector point0;
	// address: -80
	// size: 0x8
	auto struct _SVector point1;
	// address: -72
	// size: 0x8
	auto struct _SVector normal0;
	// address: -64
	// size: 0x8
	auto struct _SVector normal1;
	// address: -56
	// size: 0x8
	auto struct _SVector sphereNormal;
	// address: -48
	// size: 0x8
	auto struct _SVector dv;
	// address: -40
	// size: 0x8
	auto struct _SVector absdv;
	{ //line: 14
		// register: $a2
		register long numIntersects;
		{ //line: 18
			// register: $v0
			register short _x1;
			// register: $v1
			register short _y1;
			// register: $a0
			register short _z1;
		} //line: 18
		{ //line: 18
			// register: $a0
			register short _x1;
			// register: $v1
			register short _y1;
			// register: $a1
			register short _z1;
			// register: $v0
			// size: 0x8
			register struct _SVector *_v0;
		} //line: 18
		{ //line: 25
			// register: $v0
			register short _y0;
			// register: $v1
			register short _z0;
			// register: $v0
			register short _x1;
			// register: $a2
			register short _y1;
			// register: $a3
			register short _z1;
			// register: $a1
			// size: 0x8
			register struct _SVector *_v;
			// register: $v1
			// size: 0x8
			register struct _SVector *_v0;
		} //line: 25
	} //line: 101
	{ //line: 105
		{ //line: 112
			// register: $v0
			register short _x1;
			// register: $v1
			register short _y1;
			// register: $a0
			register short _z1;
			// register: $v0
			// size: 0x8
			register struct _SVector *_v0;
			// register: $v0
			// size: 0x8
			register struct _SVector *_v1;
		} //line: 112
		{ //line: 112
			// register: $v1
			register short _x1;
			// register: $a0
			register short _y1;
			// register: $a1
			register short _z1;
		} //line: 112
		{ //line: 121
			// register: $v0
			register short _x1;
			// register: $v1
			register short _y1;
			// register: $a0
			register short _z1;
		} //line: 121
		{ //line: 121
			// register: $a0
			register short _x1;
			// register: $v1
			register short _y1;
			// register: $a1
			register short _z1;
			// register: $v0
			// size: 0x8
			register struct _SVector *_v0;
		} //line: 121
	} //line: 131
} //line: 133


// path: C:\kain2\game\COLLIDE.C
// address: 0x80021794
// line start: 2419
// line end:   2747
void COLLIDE_Instance1SpheresToInstance2(struct _Instance *instance1, struct _Instance *instance2, long sphereToSphere)
{ //line: 1
	// address: -132
	// size: 0x20
	auto struct MATRIX *wsTransform2;
	// address: -128
	// size: 0x20
	auto struct MATRIX *oldWSTransform2;
	// address: -124
	// size: 0x6
	auto struct _Position *spherePos;
	// register: $s7
	// size: 0xC
	register struct _Vector *line;
	// address: -120
	// size: 0xC
	auto struct _Vector *offset;
	// address: -116
	// size: 0xC
	auto struct _Vector *tempVec;
	// address: -112
	// size: 0xC
	auto struct _Vector *sSpherePos1;
	// address: -108
	// size: 0xC
	auto struct _Vector *oldSSpherePos1;
	// address: -104
	// size: 0x8
	auto struct _SVector *svec;
	// register: $s3
	// size: 0xC
	register struct _Sphere *sSphere1;
	// address: -100
	// size: 0xC
	auto struct _Sphere *oldSSphere1;
	// address: -96
	// size: 0xC
	auto struct _Sphere *wSphere1;
	// address: -92
	// size: 0xC
	auto struct _Sphere *wSphere2;
	// address: -88
	// size: 0xC
	auto struct _Sphere *oldWSphere1;
	// address: -84
	// size: 0xC
	auto struct _Sphere *oldWSphere2;
	// register: $s2
	// size: 0x30
	register struct _CollideInfo *collideInfo;
	// address: -80
	// size: 0x20
	auto struct _HFaceInfo *hfaceInfo;
	// address: -76
	// size: 0x20
	auto struct MATRIX *swTransform1;
	// address: -72
	// size: 0x20
	auto struct MATRIX *swTransform2;
	// register: $s0
	// size: 0x20
	register struct MATRIX *oldSWTransform1;
	// register: $s4
	// size: 0x20
	register struct MATRIX *oldSWTransform2;
	// register: $s6
	// size: 0x14
	register struct _HSphere *hsphere1;
	// register: $s0
	// size: 0x14
	register struct _HSphere *hsphere2;
	// register: $s0
	// size: 0xC
	register struct _HFace *hface;
	// register: $a3
	register void (*collideFunc)();
	// address: -152
	auto long flag;
	// address: -136
	auto long edge;
	// register: $a0
	// size: 0x8
	register struct _HModel *hmodel1;
	// address: -68
	// size: 0x8
	auto struct _HModel *hmodel2;
	// address: -64
	// size: 0x8
	auto struct _HPrim_duplicate_s3 *hprim1;
	// address: -60
	// size: 0x8
	auto struct _HPrim_duplicate_s3 *hprim2;
	// address: -56
	auto int i;
	{ //line: 59
		// address: -52
		auto int i;
		{ //line: 105
			// register: $a3
			register long len;
			{ //line: 106
				// address: -148
				auto long a;
				// address: -144
				auto long b;
				// address: -140
				auto long c;
			} //line: 106
			{ //line: 112
				// register: $v1
				register short _x1;
				// register: $a0
				register short _y1;
				// register: $v0
				register short _z1;
				// register: $v0
				// size: 0x6
				register struct _Position *_v1;
			} //line: 112
			{ //line: 117
				// register: $v0
				register short _x1;
				// register: $v1
				register short _y1;
				// register: $a0
				register short _z1;
			} //line: 117
		} //line: 142
		{ //line: 158
			// register: $s1
			// size: 0x38
			register struct _Model *model2;
			{ //line: 257
				// register: $s4
				// size: 0x1C
				register struct _HBox *hbox;
			} //line: 323
		} //line: 324
	} //line: 327
} //line: 329


// path: C:\kain2\game\COLLIDE.C
// address: 0x800224E4
// line start: 2761
// line end:   2827
void COLLIDE_Instances(struct _Instance *instance1, struct _Instance *instance2)
{ //line: 1
	// register: $a1
	register long lx;
	// register: $a3
	register long ly;
	// register: $a2
	register long lz;
	// register: $v0
	register long mrmr;
	{ //line: 45
		// register: $a1
		register long x;
		// register: $a3
		register long y;
		// register: $a2
		register long z;
	} //line: 45
} //line: 67


// path: C:\kain2\game\COLLIDE.C
// address: 0x8002262C
// line start: 2839
// line end:   2925
void COLLIDE_InstanceList(struct _InstanceList *instanceList)
{ //line: 1
	// register: $s1
	// size: 0x29C
	register struct _Instance *instance;
	// register: $s0
	// size: 0x29C
	register struct _Instance *instance2;
	// register: $s2
	// size: 0x29C
	register struct _Instance *playerInstance;
	// register: $s4
	register long i;
	// register: $s2
	register long j;
} //line: 87


// path: C:\kain2\game\COLLIDE.C
// address: 0x800228D4
// line start: 3061
// line end:   3200
long COLLIDE_SphereAndHFace(struct _Sphere *sphere, struct _Position *oldPos, struct _HFaceInfo *hfaceInfo, struct _SVector *intersect, long *edge)
{ //line: 1
	// register: $s0
	// size: 0x44
	register struct SandHFScratch *CSpad;
	// register: $a0
	// size: 0x8
	register struct _SVector *vertex0;
	// register: $s6
	register long result;
	// register: $v0
	register long behind;
	// register: $s1
	register unsigned long d0sq;
	{ //line: 65
		// address: -64
		// size: 0xC
		auto struct _Vector dv;
		{ //line: 67
			// register: $v0
			register short _x1;
			// register: $v1
			register short _y1;
			// register: $a0
			register short _z1;
			// register: $v0
			// size: 0x8
			register struct _SVector *_v0;
		} //line: 67
	} //line: 80
	{ //line: 82
		// register: $v1
		register long y;
		// register: $v0
		register long z;
	} //line: 82
	{ //line: 86
		// register: $a0
		register unsigned long d1sq;
		{ //line: 88
			// register: $v1
			register long y;
			// register: $v0
			register long z;
		} //line: 88
		{ //line: 100
			// register: $v1
			register long y;
			// register: $v0
			register long z;
		} //line: 100
	} //line: 109
	{ //line: 113
		// register: $a2
		register long len;
		// address: -64
		// size: 0xC
		auto struct _Vector dv;
		{ //line: 115
			// register: $a3
			register long _x0;
			// register: $t0
			register long _y0;
			// register: $t1
			register long _z0;
			// register: $v1
			register long _x1;
			// register: $v0
			register long _y1;
			// register: $v1
			register long _z1;
			// register: $v0
			// size: 0xC
			register struct _Vector *_v;
			// register: $t2
			// size: 0x8
			register struct _SVector *_v1;
		} //line: 115
		{ //line: 115
			// address: -48
			auto long a;
			// address: -44
			auto long b;
			// address: -40
			auto long c;
		} //line: 115
	} //line: 137
} //line: 140


// path: C:\kain2\game\COLLIDE.C
// address: 0x80022FB0
// line start: 3243
// line end:   3556
long COLLIDE_SAndT(struct SCollideInfo *scollideInfo, struct Level *level)
{ //line: 1
	// register: $s0
	// size: 0xD4
	register struct SandTScratch *CSpad;
	// register: $s1
	register void **stack;
	// register: $a1
	// size: 0x2C
	register struct _BSPNode *bspNode;
	// register: $s5
	// size: 0x54
	register struct _Terrain *terrain;
	// address: -44
	auto long curTree;
	{ //line: 37
		// register: $v1
		register short _x1;
		// register: $a0
		register short _y1;
		// register: $v0
		register short _z1;
		// register: $s2
		// size: 0x8
		register struct _SVector *_v0;
		// register: $v0
		// size: 0x8
		register struct _SVector *_v1;
	} //line: 37
	{ //line: 37
		// address: -56
		auto long a;
		// address: -52
		auto long b;
		// address: -48
		auto long c;
	} //line: 37
	{ //line: 71
		// register: $s4
		// size: 0x24
		register struct BSPTree *bsp;
		{ //line: 86
			// register: $v0
			register short _x0;
			// register: $v1
			register short _y0;
			// register: $a0
			register short _z0;
			// register: $a1
			register short _x1;
			// register: $a2
			register short _y1;
			// register: $a3
			register short _z1;
			// register: $fp
			// size: 0x8
			register struct _SVector *_v;
			// register: $t0
			// size: 0x6
			register struct _Position *_v1;
		} //line: 86
		{ //line: 86
			// register: $v0
			register short _x0;
			// register: $v1
			register short _y0;
			// register: $a0
			register short _z0;
			// register: $a1
			register short _x1;
			// register: $a2
			register short _y1;
			// register: $a3
			register short _z1;
			// register: $s7
			// size: 0x8
			register struct _SVector *_v;
		} //line: 86
		{ //line: 86
			// register: $v0
			register short _x0;
			// register: $v1
			register short _y0;
			// register: $a0
			register short _z0;
			// register: $a1
			register short _x1;
			// register: $a2
			register short _y1;
			// register: $a3
			register short _z1;
			// register: $s6
			// size: 0x6
			register struct _Position *_v;
		} //line: 86
		{ //line: 1
			// register: $t0
			// size: 0x8
			register struct _SVector *point;
			// register: $a3
			// size: 0xC
			register struct _BoundingBox *box;
		} //line: 1
		{ //line: 122
			// register: $s3
			// size: 0xC
			register struct _TFace *tface;
			{ //line: 142
				// register: $a3
				// size: 0x8
				register struct _SVector *vertex0;
				{ //line: 1
					// register: $a0
					register short *nrmlArray;
					// register: $a1
					// size: 0x8
					register struct _SVector *nrml;
					{ //line: 1
						// register: $v1
						register short *sPtr;
					} //line: 1
				} //line: 1
				{ //line: 174
				} //line: 226
			} //line: 227
		} //line: 230
		{ //line: 241
			// register: $v0
			register int plane_front_error;
			// register: $a3
			register int plane_back_error;
		} //line: 304
		{ //line: 308
			// register: $v0
			register short _x0;
			// register: $v1
			register short _y0;
			// register: $a0
			register short _z0;
			// register: $a1
			register short _x1;
			// register: $a2
			register short _y1;
			// register: $a3
			register short _z1;
			// register: $t0
			// size: 0x6
			register struct _Position *_v1;
		} //line: 308
		{ //line: 308
			// register: $v0
			register short _x0;
			// register: $v1
			register short _y0;
			// register: $a0
			register short _z0;
			// register: $a1
			register short _x1;
			// register: $a2
			register short _y1;
			// register: $a3
			register short _z1;
		} //line: 308
		{ //line: 308
			// register: $v0
			register short _x0;
			// register: $v1
			register short _y0;
			// register: $a0
			register short _z0;
			// register: $a1
			register short _x1;
			// register: $a2
			register short _y1;
			// register: $a3
			register short _z1;
		} //line: 310
	} //line: 311
	{ //line: 312
		// register: $a0
		register short _x1;
		// register: $a1
		register short _y1;
		// register: $v1
		register short _z1;
		// register: $v0
		// size: 0x6
		register struct _Position *_v0;
		// register: $v1
		// size: 0x6
		register struct _Position *_v1;
	} //line: 312
} //line: 314


// path: C:\kain2\game\COLLIDE.C
// address: 0x80023A9C
// line start: 3770
// line end:   3802
long COLLIDE_SphereAndTerrain(struct SCollideInfo *scollideInfo, struct Level *level)
{ //line: 1
	// register: $s3
	register int result;
	// register: $s1
	register int d;
	{ //line: 1
		// register: $s2
		register int in_warpRoom;
		// register: $v0
		// size: 0x40
		register struct _StreamUnit *stream;
	} //line: 30
} //line: 32


// path: C:\kain2\game\COLLIDE.C
// address: 0x80023B84
// line start: 4238
// line end:   4320
void COLLIDE_InstanceTerrain(struct _Instance *instance, struct Level *level)
{ //line: 1
	// address: -60
	// size: 0xC
	auto struct _Vector *newPosVec;
	// address: -56
	// size: 0xC
	auto struct _Vector *oldPosVec;
	// register: $fp
	// size: 0x8
	register struct _SVector *oldPos;
	// address: -96
	// size: 0x1C
	auto struct SCollideInfo scollideInfoX;
	// register: $s3
	// size: 0x1C
	register struct SCollideInfo *scollideInfo;
	// register: $s5
	// size: 0xC
	register struct _Sphere *wSphere;
	// register: $s0
	// size: 0x20
	register struct MATRIX *swTransform;
	// register: $s1
	// size: 0x20
	register struct MATRIX *oldSWTransform;
	// address: -52
	auto void (*collideFunc)();
	// register: $s2
	// size: 0x14
	register struct _HSphere *hsphere;
	// address: -64
	auto long flags;
	{ //line: 35
		// register: $s7
		register int i;
		// register: $v0
		// size: 0x8
		register struct _HModel *hmodel;
		// register: $s6
		// size: 0x8
		register struct _HPrim_duplicate_s3 *hprim;
	} //line: 79
} //line: 83


// path: C:\kain2\game\COLLIDE.C
// address: 0x80023DF0
// line start: 4331
// line end:   4531
long COLLIDE_LineWithSignals(struct _SVector *startPoint, struct _SVector *endPoint, struct _MultiSignal **signalList, long maxSignals, struct Level *level)
{ //line: 1
	// register: $s0
	// size: 0x64
	register struct IandTScratch *CSpad;
	// register: $s2
	register void **stack;
	// register: $a1
	// size: 0x2C
	register struct _BSPNode *bspNode;
	// register: $s3
	// size: 0xC
	register struct _TFace *tface;
	// register: $s4
	// size: 0x54
	register struct _Terrain *terrain;
	// address: -56
	auto long curTree;
	// register: $fp
	register long numSignalsCollidedWith;
	{ //line: 46
		// register: $v0
		register short _x0;
		// register: $v1
		register short _y0;
		// register: $a0
		register short _z0;
		// register: $a1
		register short _x1;
		// register: $a2
		register short _y1;
		// register: $a3
		register short _z1;
		// register: $v0
		// size: 0x8
		register struct _SVector *_v;
		// register: $t1
		// size: 0x8
		register struct _SVector *_v0;
		// register: $t0
		// size: 0x8
		register struct _SVector *_v1;
	} //line: 46
	{ //line: 59
		// register: $v1
		register short _x0;
		// register: $a0
		register short _y0;
		// register: $a1
		register short _z0;
		// register: $v0
		register short _x1;
		// register: $a3
		register short _y1;
		// register: $a2
		register short _z1;
		// register: $s7
		// size: 0x8
		register struct _SVector *_v;
		// register: $a2
		// size: 0x6
		register struct _Position *_v1;
	} //line: 59
	{ //line: 59
		// register: $v1
		register short _x0;
		// register: $a0
		register short _y0;
		// register: $a1
		register short _z0;
		// register: $v0
		register short _x1;
		// register: $a3
		register short _y1;
		// register: $a2
		register short _z1;
		// register: $s6
		// size: 0x8
		register struct _SVector *_v;
		// register: $a2
		// size: 0x6
		register struct _Position *_v1;
	} //line: 59
	{ //line: 100
		// register: $a0
		// size: 0x8
		register struct _SVector *vertex0;
		{ //line: 1
			// register: $a0
			register short *nrmlArray;
			// register: $a1
			// size: 0x8
			register struct _SVector *nrml;
			{ //line: 1
				// register: $v1
				register short *sPtr;
			} //line: 1
		} //line: 1
		{ //line: 118
			{ //line: 1
				// register: $t0
				// size: 0x8
				register struct _SVector *point;
				// register: $a3
				// size: 0x8
				register struct _SVector *start;
				// register: $a2
				// size: 0x8
				register struct _SVector *line;
				{ //line: 1
					// register: $v1
					register long len;
					// register: $a1
					register long t;
				} //line: 1
			} //line: 1
			{ //line: 128
				// register: $v1
				// size: 0x388
				register struct _MultiSignal *msignal;
			} //line: 139
		} //line: 140
	} //line: 141
	{ //line: 148
		// register: $v1
		register int plane_front_error;
		// register: $a0
		register int plane_back_error;
	} //line: 193
	{ //line: 195
		// register: $v1
		register short _x0;
		// register: $a0
		register short _y0;
		// register: $a1
		register short _z0;
		// register: $v0
		register short _x1;
		// register: $a3
		register short _y1;
		// register: $a2
		register short _z1;
		// register: $a2
		// size: 0x6
		register struct _Position *_v1;
	} //line: 195
	{ //line: 195
		// register: $v1
		register short _x0;
		// register: $a0
		register short _y0;
		// register: $a1
		register short _z0;
		// register: $v0
		register short _x1;
		// register: $a3
		register short _y1;
		// register: $a2
		register short _z1;
		// register: $a2
		// size: 0x6
		register struct _Position *_v1;
	} //line: 196
} //line: 200


// path: C:\kain2\game\COLLIDE.C
// address: 0x800245B4
// line start: 4548
// line end:   4629
void COLLIDE_InstanceTerrainSignal(struct _Instance *instance, struct Level *level)
{ //line: 1
	// address: -72
	// size: 0x8
	auto struct _SVector startPoint;
	// address: -64
	// size: 0x8
	auto struct _SVector endPoint;
	// register: $s0
	// size: 0x38
	register struct _Model *model;
	// register: $s4
	register long numSignals;
	// register: $s1
	register long d;
	// address: -56
	// size: 0x20
	auto struct _MultiSignal *signalListArray[8];
	{ //line: 68
		// register: $s0
		// size: 0x388
		register struct _MultiSignal *msignal;
	} //line: 77
} //line: 82


// path: C:\kain2\game\COLLIDE.C
// address: 0x800247FC
// size: 0x40
// line start: 4634
// line end:   4805
struct _StreamUnit *COLLIDE_CameraWithStreamSignals(struct Camera_duplicate_s2 *camera)
{ //line: 1
	// address: -112
	// size: 0x8
	auto struct _SVector startPoint;
	// address: -104
	// size: 0x8
	auto struct _SVector endPoint;
	// register: $v1
	// size: 0x38
	register struct _Model *model;
	// register: $s3
	register long numSignals;
	// register: $s1
	register long i;
	// register: $s2
	register long numStreamSignals;
	// address: -96
	// size: 0x20
	auto struct _MultiSignal *signalListArray[8];
	// address: -64
	// size: 0x20
	auto struct _StreamUnit *streamSignalUnits[8];
	// register: $a1
	// size: 0x40
	register struct _StreamUnit *playerStreamUnit;
	// register: $v0
	// size: 0x188
	register struct Level *level;
	// register: $s4
	register long playerStreamUnitID;
	// register: $s0
	// size: 0x29C
	register struct _Instance *instance;
	{ //line: 103
		// register: $v0
		// size: 0x388
		register struct _MultiSignal *msignal;
		// address: -32
		auto long isWarpGateSignal;
		{ //line: 111
			// register: $v1
			// size: 0x40
			register struct _StreamUnit *cameraStreamUnit;
			// register: $a0
			register long cameraStreamID;
			{ //line: 120
				// register: $v1
				register int number;
			} //line: 124
		} //line: 140
	} //line: 141
} //line: 172


// path: C:\kain2\game\COLLIDE.C
// address: 0x80024A8C
// line start: 4809
// line end:   4825
void COLLIDE_InstanceListWithSignals(struct _InstanceList *instanceList)
{ //line: 1
	// register: $s0
	// size: 0x29C
	register struct _Instance *instance;
	// register: $v0
	// size: 0x188
	register struct Level *level;
} //line: 17


// path: C:\kain2\game\COLLIDE.C
// address: 0x80024B00
// line start: 4828
// line end:   4867
void COLLIDE_InstanceListTerrain(struct _InstanceList *instanceList)
{ //line: 1
	// register: $s2
	register long i;
	// register: $s0
	// size: 0x29C
	register struct _Instance *instance;
	// register: $v0
	// size: 0x188
	register struct Level *level;
} //line: 40


// path: C:\kain2\game\COLLIDE.C
// address: 0x80024BB0
// line start: 4959
// line end:   4974
void COLLIDE_SegmentCollisionOn(struct _Instance *instance, int segment)
{ //line: 2
	{ //line: 4
		// register: $a2
		register int i;
		// register: $v0
		// size: 0x8
		register struct _HModel *hmodel;
		// register: $v1
		// size: 0x8
		register struct _HPrim_duplicate_s3 *hprim;
	} //line: 15
} //line: 16


// path: C:\kain2\game\COLLIDE.C
// address: 0x80024C28
// line start: 4980
// line end:   4999
void COLLIDE_SegmentCollisionOff(struct _Instance *instance, int segment)
{ //line: 2
	{ //line: 4
		// register: $a2
		register int i;
		// register: $a3
		register int enabled;
		// register: $v0
		// size: 0x8
		register struct _HModel *hmodel;
		// register: $v1
		// size: 0x8
		register struct _HPrim_duplicate_s3 *hprim;
	} //line: 19
} //line: 20


// path: C:\kain2\game\COLLIDE.C
// address: 0x80024CC4
// line start: 5027
// line end:   5072
long COLLIDE_FindCollisionFaceNormal(struct _CollideInfo *collideInfo, struct _Normal *normal)
{ //line: 1
	// register: $a2
	register long valid_normal;
	{ //line: 10
		// register: $a1
		// size: 0xC
		register struct _TFace *tface;
	} //line: 14
	{ //line: 21
		{ //line: 25
			// address: -24
			// size: 0x8
			auto struct SVECTOR *lNormal;
			// register: $s1
			// size: 0x29C
			register struct _Instance *inst1;
		} //line: 26
	} //line: 26
	{ //line: 39
		// register: $v0
		// size: 0x29C
		register struct _Instance *inst1;
	} //line: 43
} //line: 45


// path: C:\kain2\game\COLLIDE.C
// address: 0x80024E00
// line start: 5077
// line end:   5095
short *COLLIDE_GetBSPTreeFlag(struct _CollideInfo *collideInfo)
{ //line: 2
	// register: $v0
	// size: 0x188
	register struct Level *level;
	// register: $v0
	// size: 0x54
	register struct _Terrain *terrain;
	// register: $v0
	// size: 0x24
	register struct BSPTree *bspTree;
} //line: 14


// path: C:\kain2\game\COLLIDE.C
// address: 0x80024E28
// line start: 5101
// line end:   5107
void COLLIDE_SetBSPTreeFlag(struct _CollideInfo *collideInfo, short flag)
{ //line: 1
	// register: $v0
	register short *bspTreeFlags;
} //line: 1


// path: C:\kain2\game\COLLIDE.C
// address: 0x80024E5C
// line start: 5133
// line end:   5203
int COLLIDE_PointAndTfaceFunc(struct _Terrain *terrain, struct BSPTree *bsp, struct _SVector *orgNewPos, struct _SVector *orgOldPos, struct _TFace *tface, long ignoreAttr, long flags)
{ //line: 1
	// register: $s0
	// size: 0x4C
	register struct PandTFScratch *CSpad;
	// address: -48
	auto int result;
	{ //line: 20
		// register: $s3
		// size: 0x8
		register struct _SVector *vertex0;
		// register: $s4
		// size: 0x8
		register struct _SVector *vertex1;
		{ //line: 23
			// register: $v0
			register short _x0;
			// register: $v1
			register short _y0;
			// register: $a1
			register short _z0;
			// register: $a2
			register short _x1;
			// register: $t0
			register short _y1;
			// register: $t2
			register short _z1;
			// register: $t1
			// size: 0x8
			register struct _SVector *_v;
			// register: $a3
			// size: 0x6
			register struct _Position *_v1;
		} //line: 23
		{ //line: 23
			// register: $v0
			register short _x0;
			// register: $v1
			register short _y0;
			// register: $a1
			register short _z0;
			// register: $a2
			register short _x1;
			// register: $t0
			register short _y1;
			// register: $a3
			register short _z1;
			// register: $t3
			// size: 0x8
			register struct _SVector *_v;
		} //line: 23
		{ //line: 23
			// register: $a1
			register short *nrmlArray;
			// register: $a0
			// size: 0x8
			register struct _SVector *nrml;
			{ //line: 23
				// register: $v1
				register short *sPtr;
			} //line: 33
		} //line: 33
		{ //line: 64
			// register: $v0
			register short _x0;
			// register: $v1
			register short _y0;
			// register: $a0
			register short _z0;
			// register: $a1
			register short _x1;
			// register: $a3
			register short _y1;
			// register: $a2
			register short _z1;
			// register: $a2
			// size: 0x6
			register struct _Position *_v1;
		} //line: 64
	} //line: 68
} //line: 71


// path: C:\kain2\game\TIMER.C
// address: 0x8003DDBC
// line start: 41
// line end:   68
unsigned long TIMER_GetTimeMS()
{ //line: 1
	// register: $s1
	register unsigned long ticks;
	// register: $s0
	register unsigned long mticks;
} //line: 1


// path: C:\kain2\game\TIMER.C
// address: 0x8003DE4C
// line start: 72
// line end:   122
unsigned long TIMER_TimeDiff(unsigned long x)
{ //line: 1
	// register: $v1
	register unsigned long intrs;
	// register: $a2
	register unsigned long ticks;
	// register: $a1
	register unsigned long prevIntrs;
	// register: $a0
	register unsigned long prevTicks;
	// register: $v1
	register unsigned long diffIntrs;
	// register: $s0
	register unsigned long diffTicks;
	// register: $v1
	register unsigned long timeDiff;
} //line: 50


// path: C:\kain2\game\BSP.C
// address: 0x8003DF34
// line start: 527
// line end:   560
void SBSP_IntroduceInstances(struct _Terrain *terrain, int unitID)
{ //line: 1
	// register: $s1
	register int i;
	// register: $s0
	// size: 0x4C
	register struct Intro *intro;
} //line: 34


// path: C:\kain2\game\BSP.C
// address: 0x8003DF98
// line start: 563
// line end:   570
void SBSP_IntroduceInstancesAndLights(struct _Terrain *terrain, struct _CameraCore_Type *cameraCore, struct LightInfo *lightInfo, int unitID);

// path: C:\kain2\game\OBTABLE.C
// address: 0x8003DFB8
// line start: 88
// line end:   125
void OBTABLE_InstanceInit(struct _Instance *instance)
{ //line: 1
	// register: $a0
	register long id;
} //line: 38


// path: C:\kain2\game\OBTABLE.C
// address: 0x8003E09C
// line start: 144
// line end:   158
void OBTABLE_GetInstanceCollideFunc(struct _Instance *instance)
{ //line: 2
	// register: $a1
	register long id;
} //line: 15


// path: C:\kain2\game\OBTABLE.C
// address: 0x8003E0E4
// line start: 176
// line end:   189
void OBTABLE_GetInstanceAdditionalCollideFunc(struct _Instance *instance)
{ //line: 2
	// register: $a1
	register long id;
} //line: 14


// path: C:\kain2\game\OBTABLE.C
// address: 0x8003E124
// line start: 206
// line end:   219
void OBTABLE_GetInstanceProcessFunc(struct _Instance *instance)
{ //line: 2
	// register: $a1
	register long id;
} //line: 14


// path: C:\kain2\game\OBTABLE.C
// address: 0x8003E16C
// line start: 236
// line end:   249
void OBTABLE_GetInstanceQueryFunc(struct _Instance *instance)
{ //line: 2
	// register: $a1
	register long id;
} //line: 14


// path: C:\kain2\game\OBTABLE.C
// address: 0x8003E1B4
// line start: 266
// line end:   279
void OBTABLE_GetInstanceMessageFunc(struct _Instance *instance)
{ //line: 2
	// register: $a1
	register long id;
} //line: 14


// path: C:\kain2\game\OBTABLE.C
// address: 0x8003E1FC
// line start: 283
// line end:   349
void OBTABLE_InitObjectWithID(struct Object *object)
{ //line: 1
	// register: $s1
	register long id;
	{ //line: 6
		// register: $a0
		// size: 0x8
		register struct ObjectAccess *oa;
	} //line: 66
} //line: 67


// path: C:\kain2\game\OBTABLE.C
// address: 0x8003E3D4
// line start: 372
// line end:   377
void OBTABLE_ClearObjectReferences()
{ //line: 2
	// register: $a0
	// size: 0x8
	register struct ObjectAccess *oa;
} //line: 6


// path: C:\kain2\game\OBTABLE.C
// address: 0x8003E408
// line start: 380
// line end:   391
void OBTABLE_RemoveObjectEntry(struct Object *object)
{ //line: 2
	// register: $a1
	// size: 0x8
	register struct ObjectAccess *oa;
} //line: 12


// path: C:\kain2\game\OBTABLE.C
// address: 0x8003E450
// size: 0x4C
// line start: 394
// line end:   411
struct Object *OBTABLE_FindObject(char *objectName)
{ //line: 1
	// register: $s0
	// size: 0x4C
	register struct Object *object;
	// register: $v0
	// size: 0x24
	register struct _ObjectTracker *otr;
	// register: $s2
	register int i;
} //line: 18


// path: C:\kain2\game\OBTABLE.C
// address: 0x8003E4FC
// line start: 414
// line end:   425
void OBTABLE_ChangeObjectAccessPointers(struct Object *oldObject, struct Object *newObject)
{ //line: 2
	// register: $a2
	// size: 0x8
	register struct ObjectAccess *oa;
} //line: 12


// path: C:\kain2\game\OBTABLE.C
// address: 0x8003E544
// line start: 428
// line end:   444
void OBTABLE_RelocateObjectTune(struct Object *object, long offset)
{ //line: 1
	// register: $a2
	register long id;
	{ //line: 14
	} //line: 16
} //line: 17


// path: C:\kain2\game\OBTABLE.C
// address: 0x8003E5A8
// line start: 447
// line end:   458
void OBTABLE_RelocateInstanceObject(struct _Instance *instance, long offset)
{ //line: 1
	// register: $a2
	register long id;
} //line: 12


// path: C:\kain2\game\OBTABLE.C
// address: 0x8003E604
// line start: 461
// line end:   520
void OBTABLE_InitAnimPointers(struct _ObjectTracker *objectTracker)
{ //line: 1
	// register: $s3
	// size: 0x4C
	register struct Object *object;
	{ //line: 6
		// register: $s2
		register int i;
		// register: $s4
		register char *earlyOut;
		// register: $s1
		// size: 0x14
		register struct _G2AnimKeylist_Type **keyPtr;
		{ //line: 12
			// register: $s0
			// size: 0x8
			register struct _ObjectOwnerInfo *oi;
			{ //line: 16
				// register: $a2
				// size: 0x24
				register struct _ObjectTracker *otr;
				// register: $a3
				// size: 0x4C
				register struct Object *ownerOb;
				// register: $a1
				register int j;
				// register: $a0
				register int objectIndex;
			} //line: 43
		} //line: 44
	} //line: 53
} //line: 60


// path: C:\kain2\game\PIPE3D.C
// address: 0x8003A7D0
// line start: 68
// line end:   82
void PIPE3D_AspectAdjustMatrix(struct MATRIX *matrix);

// path: C:\kain2\game\PIPE3D.C
// address: 0x8003A858
// line start: 88
// line end:   151
void PIPE3D_CalculateWCTransform(struct _CameraCore_Type *cameraCore)
{ //line: 1
	// address: -112
	// size: 0x20
	auto struct MATRIX user_rotation;
	// address: -80
	// size: 0x20
	auto struct MATRIX first;
	// register: $s3
	// size: 0x20
	register struct MATRIX *cam_wcTrans;
	// address: -48
	// size: 0x8
	auto struct SVECTOR v0;
	// address: -40
	// size: 0x10
	auto struct VECTOR v1;
} //line: 45


// path: C:\kain2\game\PIPE3D.C
// address: 0x8003ABD4
// line start: 154
// line end:   171
void PIPE3D_InvertTransform(struct MATRIX *target, struct MATRIX *source)
{ //line: 1
	// address: -64
	// size: 0x10
	auto struct VECTOR sourceTrans;
	// address: -48
	// size: 0x20
	auto struct MATRIX normMat;
} //line: 14


// path: C:\kain2\game\PIPE3D.C
// address: 0x8003AC68
// line start: 174
// line end:   180
long PIPE3D_MatrixColumnLength(struct MATRIX *transform, long column)
{ //line: 1
} //line: 1


// path: C:\kain2\game\PIPE3D.C
// address: 0x8003ACA4
// line start: 183
// line end:   200
void PIPE3D_NormalizeMatrix(struct MATRIX *target, struct MATRIX *source)
{ //line: 1
	// address: -32
	// size: 0x10
	auto struct VECTOR scalevec;
	// register: $v1
	register long scale;
	{ //line: 13
		// size: 0xC
		typedef struct _239fake tmm;
	} //line: 13
	{ //line: 13
		// size: 0x14
		typedef struct _240fake_duplicate_s1 cmm;
	} //line: 13
} //line: 17


// path: C:\kain2\game\PIPE3D.C
// address: 0x8003AD90
// line start: 753
// line end:   839
void PIPE3D_TransformVerticesToWorld(struct _Instance *instance, struct _SVector *poolVertex, long *vtxSegment, struct _Vector *Average)
{ //line: 1
	// register: $s1
	// size: 0x20
	register struct MATRIX *segMatrix;
	// register: $s7
	// size: 0x38
	register struct _Model *model;
	// address: -48
	// size: 0x8
	auto struct _MVertex *vertexList;
	// register: $s3
	register long i;
	// register: $v1
	// size: 0x18
	register struct _Segment *segment;
	// address: -44
	// size: 0x8
	auto struct _SVector *orgPoolVertex;
	// address: -64
	// size: 0x8
	auto struct _SVector minV;
	// address: -56
	// size: 0x8
	auto struct _SVector maxV;
	{ //line: 20
		// register: $s0
		// size: 0x8
		register struct _MVertex *firstVertex;
		// register: $s6
		// size: 0x8
		register struct _MVertex *lastVertex;
		// register: $s0
		// size: 0x8
		register struct _MVertex *modelVertex;
	} //line: 62
} //line: 87


// path: C:\kain2\game\PIPE3D.C
// address: 0x8003B0BC
// line start: 856
// line end:   1081
void PIPE3D_InstanceTransformAndDraw(struct _Instance *instance, struct _CameraCore_Type *cameraCore, struct _VertexPool *vertexPool, struct _PrimPool *primPool, unsigned long **ot, struct _Mirror *mirror)
{ //line: 1
	// register: $v0
	// size: 0x4C
	register struct Object *object;
	// register: $s4
	// size: 0x38
	register struct _Model *model;
	// register: $s3
	// size: 0x20
	register struct MATRIX *matrixPool;
	// address: -80
	// size: 0x20
	auto struct MATRIX lm;
	// register: $s1
	register long flags;
	{ //line: 37
		// register: $s0
		// size: 0x8
		register struct _MVertex *vertexList;
		// register: $s5
		// size: 0x8
		register struct _PVertex *poolVertex;
		// register: $s6
		// size: 0x4
		register struct CVECTOR *vertexColor;
		// register: $v1
		register long spadOffset;
		// register: $a1
		register long spadFree;
		// register: $a0
		register long allocSize;
		{ //line: 180
			// address: -44
			auto long BackColorSave;
			// register: $s0
			register long BlendStartSave;
			// address: -48
			auto int pval;
		} //line: 220
	} //line: 225
} //line: 226


// path: C:\kain2\game\PIPE3D.C
// address: 0x8003B3B0
// line start: 1452
// line end:   1585
void PIPE3D_InstanceListTransformAndDrawFunc(struct _StreamUnit *unit, unsigned long **ot, struct _CameraCore_Type *cameraCore, struct _Instance *instance)
{ //line: 1
	// register: $s3
	// size: 0x18C0
	register struct _VertexPool *vertexPool;
	// register: $s2
	// size: 0x1770C
	register struct _PrimPool *primPool;
	// address: -72
	// size: 0x20
	auto struct VECTOR dpv[2];
	// register: $a1
	register long maxRad;
	// register: $s5
	// size: 0x188
	register struct Level *level;
	// address: -40
	// size: 0x8
	auto struct SVECTOR bsPos;
} //line: 134


// path: C:\kain2\game\PIPE3D.C
// address: 0x8003B72C
// line start: 1588
// line end:   1602
void PIPE3D_InstanceListTransformAndDraw(struct _StreamUnit *unit, struct GameTracker *gameTracker, unsigned long **ot, struct _CameraCore_Type *cameraCore)
{ //line: 1
	// register: $s0
	// size: 0x29C
	register struct _Instance *instance;
	// register: $s2
	register int id;
	// register: $s1
	// size: 0x29C
	register struct _Instance *player;
} //line: 15


// path: C:\kain2\game\PIPE3D.C
// address: 0x8003B824
// line start: 1606
// line end:   1644
void PIPE3D_TransformFromZAxis(struct MATRIX *transform, struct _SVector *normal)
{ //line: 1
	// address: -40
	// size: 0x8
	auto struct _G2EulerAngles_Type ea1;
	// address: -32
	// size: 0x8
	auto struct _SVector xprod;
	// address: -24
	// size: 0x8
	auto struct _SVector yprod;
} //line: 39


// path: C:\kain2\game\PIPE3D.C
// address: 0x8003BA14
// line start: 1649
// line end:   1663
void PIPE3D_CalcWorldToSplitPlaneTransform(struct MATRIX *wpTransform, struct _SVector *normal, struct _SVector *translation)
{ //line: 1
	// address: -40
	// size: 0x8
	auto struct _SVector svector;
	// address: -32
	// size: 0xC
	auto struct _Vector vector;
} //line: 1


// path: C:\kain2\game\PIPE3D.C
// address: 0x8003BAE8
// line start: 1688
// line end:   1884
long PIPE3D_TransformAnimatedSplitInstanceVertices(struct _MVertex *vertexList, struct _PVertex *poolVertex, struct _Model *model, struct MATRIX *wcTransform, struct MATRIX *matrixPool, struct _Mirror *mirror, struct MATRIX *lm, struct CVECTOR *vertexColor, struct CVECTOR *vertexSrcCol)
{ //line: 1
	// register: $s7
	// size: 0x6C
	register struct TransformAnimatedInstanceVerticesWork_t *w;
	// register: $s2
	// size: 0x20
	register struct MATRIX *segMatrix;
	// address: -64
	auto long i;
	// address: -60
	// size: 0x18
	auto struct _Segment *segment;
	// address: -80
	// size: 0x4
	auto struct CVECTOR defaultRGBCD;
	{ //line: 22
	} //line: 24
	{ //line: 31
		// address: -56
		// size: 0x8
		auto struct _MVertex *firstVertex;
		// register: $fp
		// size: 0x8
		register struct _MVertex *lastVertex;
		// register: $a2
		// size: 0x8
		register struct _MVertex *modelVertex;
		// register: $t1
		// size: 0x8
		register struct _Normal *n0;
		// register: $t2
		// size: 0x8
		register struct _Normal *n1;
		// register: $t3
		// size: 0x8
		register struct _Normal *n2;
		// address: -72
		// size: 0x4
		auto struct CVECTOR white;
		// register: $s1
		// size: 0x4
		register struct CVECTOR *c0;
		// register: $s6
		// size: 0x4
		register struct CVECTOR *c1;
		// register: $s5
		// size: 0x4
		register struct CVECTOR *c2;
		// address: -52
		auto long vtxcolflgs;
	} //line: 191
} //line: 195


// path: C:\kain2\game\PIPE3D.C
// address: 0x8003C0B4
// line start: 1891
// line end:   1924
void PIPE3D_TransformSplitInstanceVertices(struct _MVertex *vertexList, struct _PVertex *pvertex, struct _Model *model, struct MATRIX *wpTransform, struct MATRIX *matrixPool, struct _Mirror *mirror)
{ //line: 1
	// register: $fp
	// size: 0x20
	register struct MATRIX *spTransform;
	// register: $s1
	// size: 0xC
	register struct _Vector *vector;
	// register: $s5
	register long i;
	// register: $v1
	// size: 0x18
	register struct _Segment *segment;
	{ //line: 13
		// register: $s0
		// size: 0x8
		register struct _MVertex *firstVertex;
		// register: $s4
		// size: 0x8
		register struct _MVertex *lastVertex;
		// register: $s0
		// size: 0x8
		register struct _MVertex *modelVertex;
	} //line: 31
} //line: 34


// path: C:\kain2\game\PIPE3D.C
// address: 0x8003C228
// line start: 1927
// line end:   1946
void PIPE3D_AnimateTextures(struct AniTex *aniTextures, long req_frame)
{ //line: 1
	// register: $t0
	// size: 0xC
	register struct AniTexInfo *ani_tex_info;
	// register: $a0
	// size: 0x10
	register struct TextureMT3 *dest;
	// register: $v1
	// size: 0x10
	register struct TextureMT3 *src;
	// register: $a3
	register long i;
} //line: 20


// path: C:\kain2\game\PIPE3D.C
// address: 0x8003C2C8
// line start: 2020
// line end:   2075
void PIPE3D_AnimateTerrainTextures(struct DrMoveAniTex *aniTextures, long req_frame, struct _PrimPool *primPool, unsigned long **drawot)
{ //line: 2
	// register: $t3
	register unsigned long *prim;
	// register: $t1
	// size: 0x28
	register struct DrMoveAniTexDestInfo *dest;
	// register: $t2
	// size: 0x8
	register struct DrMoveAniTexSrcInfo *src;
	// register: $t5
	register long i;
	// register: $a3
	register unsigned long **otl;
} //line: 56


// path: C:\kain2\game\PIPE3D.C
// address: 0x8003C460
// line start: 2103
// line end:   2171
void PIPE3D_HalvePlaneInstanceTransformAndDraw(struct _Instance *instance, struct MATRIX *wcTransform, struct _VertexPool *vertexPool, struct _PrimPool *primPool, unsigned long **ot, struct _Mirror *mirror)
{ //line: 1
	// register: $v1
	// size: 0x4C
	register struct Object *object;
	// register: $s3
	// size: 0x38
	register struct _Model *model;
	// register: $s4
	// size: 0x20
	register struct MATRIX *matrixPool;
	// address: -184
	// size: 0x20
	auto struct MATRIX wpTransform;
	// address: -152
	// size: 0x20
	auto struct MATRIX pwTransform;
	// address: -120
	// size: 0x20
	auto struct MATRIX pcTransform;
	// address: -88
	// size: 0x20
	auto struct MATRIX lm;
	// register: $s7
	// size: 0x8
	register struct _MVertex *vertexList;
	// address: -56
	// size: 0x8
	auto struct _SVector normalX;
	// register: $a1
	// size: 0x8
	register struct _SVector *normal;
	// address: -48
	// size: 0x8
	auto struct _SVector translation;
} //line: 69


// path: C:\kain2\game\PIPE3D.C
// address: 0x8003C654
// line start: 2220
// line end:   2277
void PIPE3D_HalvePlaneGetRingPoints(struct _Instance *instance, struct MATRIX *wcTransform, struct _VertexPool *vertexPool, struct _PrimPool *primPool, unsigned long **ot, struct _FXHalvePlane *ring)
{ //line: 1
	// register: $v1
	// size: 0x4C
	register struct Object *object;
	// register: $s1
	// size: 0x38
	register struct _Model *model;
	// register: $s3
	// size: 0x20
	register struct MATRIX *matrixPool;
	// address: -152
	// size: 0x20
	auto struct MATRIX wpTransform;
	// address: -120
	// size: 0x20
	auto struct MATRIX pwTransform;
	// address: -88
	// size: 0x20
	auto struct MATRIX pcTransform;
	// register: $s4
	// size: 0x8
	register struct _MVertex *vertexList;
	// register: $s5
	// size: 0x8
	register struct _PVertex *poolVertex;
	// address: -56
	// size: 0x8
	auto struct _SVector normalX;
	// register: $a1
	// size: 0x8
	register struct _SVector *normal;
	// address: -48
	// size: 0x8
	auto struct _SVector translation;
	// register: $a0
	// size: 0xC
	register struct _PlaneConstants *halvePlane;
} //line: 58


// path: C:\kain2\game\PIPE3D.C
// address: 0x8003C824
// line start: 2294
// line end:   2381
void PIPE3D_DoGlow(struct _Instance *instance, struct MATRIX *wcTransform, struct _VertexPool *vertexPool, struct _PrimPool *primPool, unsigned long **ot, struct _FXGlowEffect *glow)
{ //line: 1
	// register: $a2
	register long currentColorID;
	// register: $a1
	register long previousColorID;
	// register: $a0
	register long value;
	// register: $a3
	register long fade;
	// register: $s0
	register long fadeflag;
	{ //line: 31
		// address: -32
		auto long color;
		{ //line: 76
			// register: $s0
			register long i;
		} //line: 86
	} //line: 87
} //line: 88


// path: C:\kain2\game\PIPE3D.C
// address: 0x8003CC00
// line start: 2388
// line end:   2416
long PIPE3D_Segment2ScreenPt(struct _Instance *instance, struct MATRIX *wcTransform, int segIndex, struct _Position *pos)
{ //line: 1
	// address: -56
	// size: 0x20
	auto struct MATRIX scTransform;
	// address: -24
	// size: 0x6
	auto struct _Position vOrigin;
	// address: -16
	auto long face_z;
} //line: 1


// path: C:\kain2\game\RESOLVE.C
// address: 0x8003CC84
// line start: 55
// line end:   76
void RESOLVE_Pointers(struct RedirectList *redirectList, long *data, long *baseAddr)
{ //line: 5
	// register: $a0
	register long *rdList;
	// register: $a3
	register int i;
	{ //line: 9
		// register: $v1
		register long *handle;
	} //line: 10
} //line: 22


// path: C:\kain2\game\SCRIPT.C
// address: 0x8003CCBC
// line start: 51
// line end:   61
void SCRIPT_CombineEulerAngles(struct _Rotation *combinedRotation, struct _Rotation *inputRotation1, struct _Rotation *inputRotation2)
{ //line: 1
	// address: -88
	// size: 0x20
	auto struct MATRIX rotMatrix1;
	// address: -56
	// size: 0x20
	auto struct MATRIX rotMatrix2;
	// address: -24
	// size: 0x8
	auto struct _G2EulerAngles_Type ea;
	{ //line: 1
		// register: $v0
		register short _x1;
		// register: $v1
		register short _y1;
		// register: $a0
		register short _z1;
	} //line: 1
} //line: 1


// path: C:\kain2\game\SCRIPT.C
// address: 0x8003CD3C
// line start: 163
// line end:   258
void SCRIPT_InstanceSplineInit(struct _Instance *instance)
{ //line: 1
	// register: $s3
	// size: 0x8
	register struct Spline *spline;
	// register: $a0
	// size: 0x8
	register struct RSpline *rspline;
	// register: $s4
	// size: 0x8
	register struct Spline *sspline;
	// register: $s0
	// size: 0x50
	register struct MultiSpline *multi;
	// register: $s5
	// size: 0x8
	register struct SplineDef *sd;
	// register: $s1
	// size: 0x8
	register struct SplineDef *rsd;
	// register: $s6
	// size: 0x8
	register struct SplineDef *ssd;
	// address: -40
	auto unsigned long isParent;
	// address: -36
	auto unsigned long isClass;
	{ //line: 24
		// register: $a0
		// size: 0x8
		register struct _G2Quat_Type *q;
		// address: -80
		// size: 0x8
		auto struct _G2EulerAngles_Type ea;
		{ //line: 32
			// address: -72
			// size: 0x20
			auto struct MATRIX introTransform;
		} //line: 41
		{ //line: 46
			// address: -72
			// size: 0x8
			auto struct _Rotation combinedRotation;
			{ //line: 46
				// register: $v0
				register short _x1;
				// register: $v1
				register short _y1;
				// register: $a0
				register short _z1;
			} //line: 57
		} //line: 58
	} //line: 60
	{ //line: 64
		// register: $a3
		// size: 0x8
		register struct _SVector *start_point;
		{ //line: 72
			// register: $v0
			register short _x0;
			// register: $a0
			register short _y0;
			// register: $v1
			register short _z0;
			// register: $a1
			register short _x1;
			// register: $a2
			register short _y1;
			// register: $a3
			register short _z1;
			// register: $v0
			// size: 0x6
			register struct _Position *_v;
			// register: $v1
			// size: 0x6
			register struct _Position *_v0;
		} //line: 72
	} //line: 81
	{ //line: 85
		// register: $v1
		// size: 0x8
		register struct _SVector *start_point;
	} //line: 95
} //line: 96


// path: C:\kain2\game\SCRIPT.C
// address: 0x8003CF8C
// line start: 266
// line end:   302
short SCRIPTCountFramesInSpline(struct _Instance *instance)
{ //line: 1
	// register: $s3
	register short kf;
	// register: $s2
	register short frames;
	// register: $a0
	// size: 0x20
	register struct SplineKey *key;
	// register: $s0
	// size: 0x8
	register struct Spline *spline;
	// register: $s1
	// size: 0x8
	register struct RSpline *rspline;
	{ //line: 16
		// register: $a0
		// size: 0xA
		register struct SplineRotKey *rkey;
	} //line: 22
	{ //line: 25
		// register: $v0
		// size: 0x50
		register struct MultiSpline *multi;
	} //line: 34
} //line: 36


// path: C:\kain2\game\SCRIPT.C
// address: 0x8003D0DC
// size: 0x8
// line start: 317
// line end:   324
struct Spline *ScriptGetPosSpline(struct _Instance *instance)
{ //line: 1
	// register: $v0
	// size: 0x50
	register struct MultiSpline *multi;
} //line: 8


// path: C:\kain2\game\SCRIPT.C
// address: 0x8003D114
// size: 0x8
// line start: 326
// line end:   333
struct RSpline *ScriptGetRotSpline(struct _Instance *instance)
{ //line: 1
	// register: $v0
	// size: 0x50
	register struct MultiSpline *multi;
} //line: 8


// path: C:\kain2\game\SCRIPT.C
// address: 0x8003D14C
// line start: 338
// line end:   341
int SCRIPT_GetSplineFrameNumber(struct _Instance *instance, struct SplineDef *splineDef);

// path: C:\kain2\game\SCRIPT.C
// address: 0x8003D1AC
// size: 0x50
// line start: 346
// line end:   373
struct MultiSpline *SCRIPT_GetMultiSpline(struct _Instance *instance, unsigned long *isParent, unsigned long *isClass)
{ //line: 2
	// register: $a3
	// size: 0x50
	register struct MultiSpline *multi;
} //line: 27


// path: C:\kain2\game\SCRIPT.C
// address: 0x8003D258
// size: 0x8
// line start: 376
// line end:   385
struct SplineDef *SCRIPT_GetPosSplineDef(struct _Instance *instance, struct MultiSpline *multi, unsigned long isParent, unsigned long isClass)
{ //line: 1
	// register: $v0
	// size: 0x8
	register struct SplineDef *splineDef;
} //line: 10


// path: C:\kain2\game\SCRIPT.C
// address: 0x8003D284
// size: 0x8
// line start: 388
// line end:   397
struct SplineDef *SCRIPT_GetRotSplineDef(struct _Instance *instance, struct MultiSpline *multi, unsigned long isParent, unsigned long isClass)
{ //line: 1
	// register: $v0
	// size: 0x8
	register struct SplineDef *splineDef;
} //line: 10


// path: C:\kain2\game\SCRIPT.C
// address: 0x8003D2B0
// size: 0x8
// line start: 400
// line end:   409
struct SplineDef *SCRIPT_GetScaleSplineDef(struct _Instance *instance, struct MultiSpline *multi, unsigned long isParent, unsigned long isClass)
{ //line: 1
	// register: $v0
	// size: 0x8
	register struct SplineDef *splineDef;
} //line: 10


// path: C:\kain2\game\SCRIPT.C
// address: 0x8003D2DC
// line start: 412
// line end:   441
void SCRIPT_RelativisticSpline(struct _Instance *instance, struct _SVector *point)
{ //line: 1
	// address: -64
	// size: 0x8
	auto struct _SVector pt;
	{ //line: 11
		// address: -56
		// size: 0x20
		auto struct MATRIX segMatrix;
		// address: -24
		// size: 0x6
		auto struct _Position newPt;
		// address: -16
		// size: 0x6
		auto struct _Position localPt;
		{ //line: 14
			// register: $a1
			register short _x1;
			// register: $v1
			register short _y1;
			// register: $a0
			register short _z1;
			// register: $v0
			// size: 0x8
			register struct _SVector *_v0;
		} //line: 14
		{ //line: 14
			// register: $v1
			register short _y0;
			// register: $v0
			register short _z0;
			// register: $a1
			register short _x1;
			// register: $a3
			register short _y1;
			// register: $a2
			register short _z1;
			// register: $a0
			// size: 0x6
			register struct _Position *_v;
			// register: $v0
			// size: 0x6
			register struct _Position *_v0;
			// register: $a2
			// size: 0x6
			register struct _Position *_v1;
		} //line: 14
	} //line: 14
	{ //line: 28
		// register: $v0
		register short _x0;
		// register: $a0
		register short _y0;
		// register: $v1
		register short _z0;
		// register: $a1
		register short _x1;
		// register: $a2
		register short _y1;
		// register: $a3
		register short _z1;
		// register: $v0
		// size: 0x6
		register struct _Position *_v;
		// register: $v1
		// size: 0x6
		register struct _Position *_v0;
	} //line: 28
} //line: 30


// path: C:\kain2\game\SCRIPT.C
// address: 0x8003D4CC
// line start: 444
// line end:   553
void SCRIPT_InstanceSplineSet(struct _Instance *instance, short frameNum, struct SplineDef *splineDef, struct SplineDef *rsplineDef, struct SplineDef *ssplineDef)
{ //line: 1
	// register: $s0
	// size: 0x8
	register struct Spline *spline;
	// register: $s3
	// size: 0x8
	register struct RSpline *rspline;
	// register: $s7
	// size: 0x8
	register struct Spline *sspline;
	// register: $s1
	// size: 0x50
	register struct MultiSpline *multi;
	// address: -44
	auto unsigned long isClass;
	// address: -48
	auto unsigned long isParent;
	// address: -96
	// size: 0x8
	auto struct _SVector point;
	{ //line: 62
		// address: -88
		// size: 0x8
		auto struct _G2Quat_Type q;
		{ //line: 65
			// address: -80
			// size: 0x20
			auto struct MATRIX introTransform;
		} //line: 71
	} //line: 71
	{ //line: 79
		// address: -88
		// size: 0x8
		auto struct _Rotation rot;
		{ //line: 83
			// address: -80
			// size: 0x8
			auto struct _Rotation combinedRotation;
			{ //line: 83
				// register: $v0
				register short _x1;
				// register: $v1
				register short _y1;
				// register: $a0
				register short _z1;
			} //line: 93
		} //line: 94
	} //line: 96
	{ //line: 100
		// address: -88
		// size: 0x8
		auto struct _SVector scale;
	} //line: 108
} //line: 110


// path: C:\kain2\game\SCRIPT.C
// address: 0x8003D7A4
// line start: 556
// line end:   688
long SCRIPT_SplineProcess(struct _Instance *instance, struct MultiSpline *multi, struct SplineDef *splineDef, struct SplineDef *rsplineDef, struct SplineDef *ssplineDef, int direction, int isClass)
{ //line: 1
	// register: $s3
	register long retVal;
	// register: $s5
	// size: 0x8
	register struct Spline *spline;
	// register: $s1
	// size: 0x8
	register struct RSpline *rspline;
	// register: $a0
	// size: 0x8
	register struct Spline *sspline;
	// register: $s2
	// size: 0x8
	register struct _SVector *point;
	// address: -96
	// size: 0x8
	auto struct _Rotation rot;
	// register: $s6
	register long timeOff;
	{ //line: 18
		// address: -88
		// size: 0x8
		auto struct _SVector pt;
	} //line: 38
	{ //line: 61
		// address: -80
		// size: 0x8
		auto struct _G2Quat_Type q;
		{ //line: 64
			// address: -72
			// size: 0x20
			auto struct MATRIX introTransform;
		} //line: 70
	} //line: 76
	{ //line: 82
		// address: -72
		// size: 0x8
		auto struct _Rotation combinedRotation;
		{ //line: 82
			// register: $v0
			register short _x1;
			// register: $v1
			register short _y1;
			// register: $a0
			register short _z1;
		} //line: 82
	} //line: 82
	{ //line: 106
		// address: -80
		// size: 0x8
		auto struct _SVector pt;
	} //line: 131
} //line: 132


// path: C:\kain2\game\SCRIPT.C
// address: 0x8003DA8C
// line start: 716
// line end:   746
long SCRIPT_InstanceSplineProcess(struct _Instance *instance, struct SplineDef *splineDef, struct SplineDef *rsplineDef, struct SplineDef *ssplineDef, int direction)
{ //line: 1
	// register: $s1
	// size: 0x50
	register struct MultiSpline *multi;
	// address: -28
	auto unsigned long isClass;
	// address: -32
	auto unsigned long isParent;
} //line: 31


// path: C:\kain2\game\SCRIPT.C
// address: 0x8003DB94
// line start: 1038
// line end:   1041
void SCRIPT_FadeOutProcess(struct _Instance *instance);

// path: C:\kain2\game\SCRIPT.C
// address: 0x8003DBDC
// line start: 1046
// line end:   1126
void ScriptKillInstance(struct _Instance *instance, int effect)
{ //line: 1
	// register: $s1
	// size: 0x4C
	register struct Object *object;
	{ //line: 23
		// register: $v0
		// size: 0x4C
		register struct Object *object;
	} //line: 28
	{ //line: 34
		// address: -24
		// size: 0x8
		auto struct SVECTOR v;
	} //line: 41
	{ //line: 47
		// register: $a0
		// size: 0x14
		register struct FXSplinter *splinterData;
		// register: $a1
		register int facadeFlg;
		// register: $a2
		register short shardFlags;
		{ //line: 54
			// register: $v1
			// size: 0x8
			register struct GenericTune *tune;
		} //line: 62
	} //line: 68
} //line: 81


// path: C:\kain2\game\MATH3D.C
// address: 0x80039A58
// line start: 220
// line end:   255
void MATH3D_Sort3VectorCoords(long *a, long *b, long *c)
{ //line: 2
	// register: $t0
	register long a1;
	// register: $v1
	register long b1;
	// register: $a3
	register long c1;
} //line: 36


// path: C:\kain2\game\MATH3D.C
// address: 0x80039ADC
// line start: 258
// line end:   309
long MATH3D_LengthXYZ(long x, long y, long z)
{ //line: 2
	// register: $v0
	register long t;
} //line: 51


// path: C:\kain2\game\MATH3D.C
// address: 0x80039BAC
// line start: 312
// line end:   316
long MATH3D_LengthXY(long x, long y);

// path: C:\kain2\game\MATH3D.C
// address: 0x80039C0C
// line start: 328
// line end:   344
void MATH3D_Normalize(struct _Normal *normal)
{ //line: 1
	// register: $a1
	register long length;
} //line: 17


// path: C:\kain2\game\MATH3D.C
// address: 0x80039C9C
// line start: 349
// line end:   400
short MATH3D_FastAtan2(long y, long x)
{ //line: 2
	// register: $a3
	register long ax;
	// register: $a2
	register long ay;
} //line: 52


// path: C:\kain2\game\MATH3D.C
// address: 0x80039DB8
// line start: 456
// line end:   505
long MATH3D_FastSqrt(long square)
{ //line: 2
	{ //line: 2
		// register: $t0
		register unsigned long result;
		// register: $v0
		register long remainder;
		// register: $a1
		register long mask;
		// register: $a2
		register long shift;
		// register: $a3
		register long mask_squared;
		// register: $v1
		register long result_shift;
	} //line: 48
} //line: 50


// path: C:\kain2\game\MATH3D.C
// address: 0x80039E98
// line start: 511
// line end:   547
long MATH3D_FastSqrt0(long square)
{ //line: 2
	{ //line: 2
		// register: $t0
		register unsigned long result;
		// register: $v0
		register long remainder;
		// register: $a2
		register long mask;
		// register: $a1
		register long shift;
		// register: $a3
		register long mask_squared;
		// register: $v1
		register long result_shift;
	} //line: 35
} //line: 37


// path: C:\kain2\game\MATH3D.C
// address: 0x80039F28
// line start: 607
// line end:   613
long MATH3D_DistanceBetweenPositions(struct _Position *pos1, struct _Position *pos2)
{ //line: 1
} //line: 1


// path: C:\kain2\game\MATH3D.C
// address: 0x80039F74
// line start: 616
// line end:   635
short MATH3D_AngleBetweenVectors(struct _SVector *vector1, struct _SVector *vector2)
{ //line: 1
	// register: $s0
	register long projection_length;
} //line: 20


// path: C:\kain2\game\MATH3D.C
// address: 0x8003A060
// line start: 638
// line end:   656
void MATH3D_RotMatAboutVec(struct _SVector *vec, struct MATRIX *mat, short angle)
{ //line: 1
	// register: $s0
	register long length;
	// address: -96
	// size: 0x8
	auto struct SVECTOR rot_angs;
	// address: -88
	// size: 0x20
	auto struct MATRIX mat1;
	// address: -56
	// size: 0x20
	auto struct MATRIX mat2;
} //line: 19


// path: C:\kain2\game\MATH3D.C
// address: 0x8003A130
// line start: 659
// line end:   664
void MATH3D_SetUnityMatrix(struct MATRIX *mat);

// path: C:\kain2\game\MATH3D.C
// address: 0x8003A14C
// line start: 675
// line end:   694
void AngleMoveToward(short *current_ptr, short destination, short step)
{ //line: 1
	// register: $a0
	register long diff;
	// register: $s0
	register short current;
} //line: 20


// path: C:\kain2\game\MATH3D.C
// address: 0x8003A1F8
// line start: 700
// line end:   706
short AngleDiff(short current, short destination)
{ //line: 2
} //line: 6


// path: C:\kain2\game\MATH3D.C
// address: 0x8003A21C
// line start: 712
// line end:   718
short MATH3D_AngleFromPosToPos(struct _Position *from, struct _Position *to)
{ //line: 1
} //line: 1


// path: C:\kain2\game\MATH3D.C
// address: 0x8003A254
// line start: 723
// line end:   732
void MATH3D_ZYXtoXYZ(struct _Rotation *rot)
{ //line: 1
	// address: -48
	// size: 0x20
	auto struct MATRIX tempMat;
	// address: -16
	// size: 0x8
	auto struct _G2EulerAngles_Type ea;
} //line: 1


// path: C:\kain2\game\MATH3D.C
// address: 0x8003A2B0
// line start: 738
// line end:   747
short MATH3D_ElevationFromPosToPos(struct _Position *from, struct _Position *to)
{ //line: 1
} //line: 1


// path: C:\kain2\game\MATH3D.C
// address: 0x8003A334
// line start: 754
// line end:   757
void MATH3D_RotationFromPosToPos(struct _Position *from, struct _Position *to, struct _Rotation *rot);

// path: C:\kain2\game\MATH3D.C
// address: 0x8003A388
// line start: 762
// line end:   778
int MATH3D_veclen2(int ix, int iy)
{ //line: 1
	// register: $a0
	register int t;
} //line: 14


// path: C:\kain2\game\MATH3D.C
// address: 0x8003A3E8
// line start: 784
// line end:   828
void MATH3D_RotateAxisToVector(struct MATRIX *dest, struct MATRIX *src, struct _SVector *vec, enum MATH3D_AXIS axis)
{ //line: 1
	// address: -64
	// size: 0x20
	auto struct MATRIX xform;
	// address: -32
	// size: 0x8
	auto struct _G2Quat_Type rot;
	// register: $s0
	register long len;
	// register: $s1
	register int theta;
	// register: $v0
	register int sintheta;
	// register: $a3
	register int px;
	// register: $v1
	register int py;
	// register: $a1
	register int pz;
} //line: 36


// path: C:\kain2\game\MATH3D.C
// address: 0x8003A640
// line start: 859
// line end:   873
int MATH3D_ConeDetect(struct _SVector *pos, int arc, int elevation)
{ //line: 1
	// register: $s1
	register long y;
	// register: $s3
	register long x;
	{ //line: 7
	} //line: 13
} //line: 15


// path: C:\kain2\game\MATH3D.C
// address: 0x8003A6F8
// line start: 976
// line end:   984
void MATH3D_CrossProduct(struct _SVector *t, struct _SVector *r, struct _SVector *s);

// path: C:\kain2\game\MATH3D.C
// address: 0x8003A79C
// line start: 989
// line end:   1005
unsigned long MATH3D_SquareLength(long x, long y, long z);

// path: C:\kain2\game\SPLINE.C
// address: 0x80041224
// line start: 70
// line end:   95
void _SplineS2Pos(struct vecS *p, long s, struct SplineKey *key, struct SplineKey *key2)
{ //line: 1
	// register: $a3
	register long s2;
	// register: $t0
	register long s3;
	// register: $a2
	register long h0;
	// register: $a1
	register long h1;
	// register: $a0
	register long h2;
	// register: $t0
	register long h3;
} //line: 26


// path: C:\kain2\game\SPLINE.C
// address: 0x80041394
// line start: 132
// line end:   164
void SplineSetDefDenom(struct Spline *spline, struct SplineDef *def, int denomFlag)
{ //line: 8
	// register: $v1
	register unsigned long denom;
} //line: 33


// path: C:\kain2\game\SPLINE.C
// address: 0x80041458
// line start: 248
// line end:   285
unsigned short SplineGetFrameNumber(struct Spline *spline, struct SplineDef *def)
{ //line: 1
	// register: $s0
	register unsigned short frame;
	// register: $v1
	register unsigned int i;
	// register: $a2
	// size: 0x20
	register struct SplineKey *key;
	// address: -16
	// size: 0x8
	auto struct SplineDef temp;
} //line: 37


// path: C:\kain2\game\SPLINE.C
// address: 0x800414EC
// line start: 288
// line end:   347
short SplineSetDef2FrameNumber(struct Spline *spline, struct SplineDef *def, unsigned short frame_number)
{ //line: 1
	// register: $t3
	register unsigned long isRot;
	// register: $t4
	register short status;
	// register: $t0
	// size: 0x20
	register struct SplineKey *key;
	// register: $a3
	// size: 0xA
	register struct SplineRotKey *rkey;
	// register: $t2
	register unsigned short frame;
	// register: $t1
	register short deltaFrame;
} //line: 59


// path: C:\kain2\game\SPLINE.C
// address: 0x8004161C
// line start: 355
// line end:   366
short SplineIsWhere(struct Spline *spline, struct SplineDef *def)
{ //line: 2
	// register: $v1
	register short curr;
} //line: 12


// path: C:\kain2\game\SPLINE.C
// address: 0x80041684
// line start: 368
// line end:   384
short SplineMultiIsWhere(struct MultiSpline *multi)
{ //line: 1
	// register: $v1
	register short where;
} //line: 16


// path: C:\kain2\game\SPLINE.C
// address: 0x80041710
// size: 0x8
// line start: 423
// line end:   431
struct _G2Quat_Type *SplineGetFirstRot(struct RSpline *rspline, struct SplineDef *def);

// path: C:\kain2\game\SPLINE.C
// address: 0x80041738
// size: 0x8
// line start: 435
// line end:   442
struct _SVector *SplineGetFirstPoint(struct Spline *spline, struct SplineDef *def);

// path: C:\kain2\game\SPLINE.C
// address: 0x80041760
// size: 0x8
// line start: 499
// line end:   510
struct _SVector *SplineGetNextPoint(struct Spline *spline, struct SplineDef *def)
{ //line: 1
	// address: 8
	// size: 0x8
	static struct _SVector point;
} //line: 12


// path: C:\kain2\game\SPLINE.C
// address: 0x800417B8
// size: 0x8
// line start: 513
// line end:   524
struct _SVector *SplineGetPreviousPoint(struct Spline *spline, struct SplineDef *def)
{ //line: 1
	// address: 16
	// size: 0x8
	static struct _SVector point;
} //line: 12


// path: C:\kain2\game\SPLINE.C
// address: 0x80041810
// size: 0x8
// line start: 539
// line end:   685
struct _SVector *SplineGetNearestPoint(struct Spline *spline, struct _SVector *point, struct SplineDef *def)
{ //line: 1
	// address: 24
	// size: 0x8
	static struct _SVector dpoint;
	// register: $a1
	register long dist;
	// register: $v0
	register long dist1;
	// register: $s6
	register long closest_dist;
	// register: $s3
	register int closest_keyframe;
	// register: $s5
	register int adjacent_keyframe;
	// register: $s2
	register int closest_ib;
	// address: -56
	auto int saved_closest_key_dist;
	// register: $s0
	register int i;
	// register: $s7
	register int frame_count;
	// address: -52
	auto int swapped_keys;
	// register: $a3
	// size: 0x20
	register struct SplineKey *key;
	// address: -80
	// size: 0xC
	auto struct _Vector d;
	// address: -64
	// size: 0x8
	auto struct SplineDef tempdef;
} //line: 142


// path: C:\kain2\game\SPLINE.C
// address: 0x80041BCC
// line start: 933
// line end:   1000
unsigned long SplineGetData(struct Spline *spline, struct SplineDef *def, void *p)
{ //line: 1
	// register: $s3
	register unsigned long gotDataOk;
	// register: $s2
	register unsigned long isRot;
	// register: $v1
	register int count;
	// address: -40
	// size: 0x8
	auto struct _G2Quat_Type quat;
	// address: -32
	// size: 0x8
	auto struct _G2EulerAngles_Type ea;
	{ //line: 32
	} //line: 33
	{ //line: 43
	} //line: 44
	{ //line: 57
		// register: $v1
		register short _x1;
		// register: $a0
		register short _y1;
		// register: $v0
		register short _z1;
		// register: $v0
		// size: 0x6
		register struct vecS *_v1;
	} //line: 57
} //line: 67


// path: C:\kain2\game\SPLINE.C
// address: 0x80041DDC
// line start: 1003
// line end:   1039
unsigned long SplineGetQuatData(struct Spline *spline, struct SplineDef *def, void *p)
{ //line: 1
	// register: $s2
	register unsigned long gotDataOk;
	// register: $a0
	register int count;
	// address: -32
	// size: 0x8
	auto struct _G2Quat_Type quat;
	{ //line: 20
	} //line: 21
} //line: 36


// path: C:\kain2\game\SPLINE.C
// address: 0x80041F10
// line start: 1052
// line end:   1106
unsigned long SplineGetNext(struct Spline *spline, struct SplineDef *def)
{ //line: 1
	// register: $s3
	register unsigned long movedSplineOk;
	// register: $s2
	register int count;
} //line: 54


// path: C:\kain2\game\SPLINE.C
// address: 0x8004202C
// line start: 1118
// line end:   1184
unsigned long SplineGetPrev(struct Spline *spline, struct SplineDef *def)
{ //line: 1
	// register: $s2
	register unsigned long movedSplineOk;
	// register: $a1
	register unsigned long isRot;
	// register: $v0
	register int count;
} //line: 66


// path: C:\kain2\game\SPLINE.C
// address: 0x80042158
// line start: 1205
// line end:   1271
unsigned long SplineGetOffsetNext(struct Spline *spline, struct SplineDef *def, long fracOffset)
{ //line: 1
	// register: $s3
	register unsigned long movedSplineOk;
	// register: $s4
	register unsigned long isRot;
	// register: $s0
	register int count;
} //line: 66


// path: C:\kain2\game\SPLINE.C
// address: 0x8004233C
// line start: 1284
// line end:   1350
unsigned long SplineGetOffsetPrev(struct Spline *spline, struct SplineDef *def, long fracOffset)
{ //line: 1
	// register: $s2
	register unsigned long movedSplineOk;
	// register: $a1
	register unsigned long isRot;
	// register: $v1
	register int count;
} //line: 66


// path: C:\kain2\game\SPLINE.C
// address: 0x800424B0
// size: 0x8
// line start: 1354
// line end:   1365
struct _SVector *SplineGetOffsetNextPoint(struct Spline *spline, struct SplineDef *def, long offset)
{ //line: 1
	// address: 40
	// size: 0x8
	static struct _SVector point;
} //line: 12


// path: C:\kain2\game\SPLINE.C
// address: 0x80042508
// size: 0x8
// line start: 1368
// line end:   1379
struct _SVector *SplineGetOffsetPreviousPoint(struct Spline *spline, struct SplineDef *def, long offset)
{ //line: 1
	// address: 48
	// size: 0x8
	static struct _SVector point;
} //line: 12


// path: C:\kain2\game\FX.C
// address: 0x80042560
// line start: 120
// line end:   175
void FX_Init(struct _FXTracker *fxTracker)
{ //line: 1
	// register: $s0
	// size: 0x2C
	register struct _FX_MATRIX *fxMatrix;
	// register: $s2
	// size: 0x2C
	register struct _FX_MATRIX *endFXMatrix;
	// register: $s0
	// size: 0x68
	register struct _FX_PRIM_duplicate_s24 *fxPrim;
	// register: $s2
	// size: 0x68
	register struct _FX_PRIM_duplicate_s24 *endFXPrim;
} //line: 27


// path: C:\kain2\game\FX.C
// address: 0x80042640
// line start: 191
// line end:   203
void FX_Die(struct _FX_PRIM_duplicate_s24 *fxPrim, struct _FXTracker *fxTracker);

// path: C:\kain2\game\FX.C
// address: 0x800426B0
// size: 0x2C
// line start: 208
// line end:   218
struct _FX_MATRIX *FX_GetMatrix(struct _FXTracker *fxTracker)
{ //line: 1
	// register: $s0
	// size: 0x2C
	register struct _FX_MATRIX *fxMatrix;
} //line: 10


// path: C:\kain2\game\FX.C
// address: 0x80042700
// size: 0x68
// line start: 222
// line end:   244
struct _FX_PRIM_duplicate_s24 *FX_GetPrim(struct _FXTracker *fxTracker)
{ //line: 1
	// register: $a0
	// size: 0x68
	register struct _FX_PRIM_duplicate_s24 *fxPrim;
} //line: 22


// path: C:\kain2\game\FX.C
// address: 0x80042770
// size: 0x4C
// line start: 247
// line end:   268
struct _FXParticle *FX_GetParticle(struct _Instance *instance, short startSegment)
{ //line: 1
	// register: $v1
	// size: 0x4C
	register struct _FXParticle *particle;
} //line: 21


// path: C:\kain2\game\FX.C
// address: 0x8004280C
// line start: 271
// line end:   292
void FX_AniTexSetup(struct _FX_PRIM_duplicate_s24 *fxPrim, struct _MFace *mface, struct _Model *model, struct _FXTracker *fxTracker);

// path: C:\kain2\game\FX.C
// address: 0x80042888
// line start: 360
// line end:   383
void FX_StandardProcess(struct _FX_PRIM_duplicate_s24 *fxPrim, struct _FXTracker *fxTracker);

// path: C:\kain2\game\FX.C
// address: 0x800428A8
// line start: 396
// line end:   444
void FX_ShatterProcess(struct _FX_PRIM_duplicate_s24 *fxPrim, struct _FXTracker *fxTracker)
{ //line: 1
	// address: -64
	// size: 0x20
	auto struct MATRIX matrix;
	// address: -32
	// size: 0x8
	auto struct _Rotation rotation;
	// address: -24
	// size: 0x8
	auto struct _Rotation rot_temp;
} //line: 49


// path: C:\kain2\game\FX.C
// address: 0x80042B50
// line start: 583
// line end:   772
void FX_DFacadeProcess(struct _FX_PRIM_duplicate_s24 *fxPrim, struct _FXTracker *fxTracker)
{ //line: 1
	{ //line: 13
		// register: $v1
		// size: 0x20
		register struct MATRIX *swTransform;
	} //line: 14
	{ //line: 173
		// address: -16
		// size: 0x8
		auto struct _Rotation rot;
	} //line: 181
} //line: 190


// path: C:\kain2\game\FX.C
// address: 0x80042D24
// size: 0x68
// line start: 856
// line end:   885
struct _FX_PRIM_duplicate_s24 *_FX_BuildSingleFaceWithModel(struct _Model *model, struct _MFace *mface, struct SVECTOR *center, struct SVECTOR *vel, struct SVECTOR *accl, struct _FXTracker *fxTracker, void (*fxSetup)(), void (*fxProcess)(), struct _FX_MATRIX *fxMatrix, int timeToLive)
{ //line: 1
	// register: $s1
	// size: 0x68
	register struct _FX_PRIM_duplicate_s24 *fxPrim;
	// register: $s3
	// size: 0x8
	register struct _MVertex *vertexList;
} //line: 29


// path: C:\kain2\game\FX.C
// address: 0x80042E20
// size: 0x68
// line start: 890
// line end:   891
struct _FX_PRIM_duplicate_s24 *FX_BuildSingleFaceWithModel(struct _Model *model, struct _MFace *mface, struct SVECTOR *center, struct SVECTOR *vel, struct SVECTOR *accl, struct _FXTracker *fxTracker, void (*fxSetup)(), void (*fxProcess)(), int timeToLive);

// path: C:\kain2\game\FX.C
// address: 0x80042E70
// line start: 917
// line end:   925
void _FX_SetupLighting(struct _Instance *instance)
{ //line: 1
	// address: -40
	// size: 0x20
	auto struct MATRIX lm;
} //line: 1


// path: C:\kain2\game\FX.C
// address: 0x80042EC0
// line start: 928
// line end:   961
long _FX_DoLighting(struct _MFace *mface)
{ //line: 1
	// register: $a2
	register long modelDQP;
	// address: -8
	auto long color;
	// register: $a1
	// size: 0x10
	register struct TextureMT3 *texture;
} //line: 33


// path: C:\kain2\game\FX.C
// address: 0x80042FD8
// line start: 976
// line end:   1242
void _FX_BuildSegmentedSplinters(struct _Instance *instance, struct SVECTOR *center, struct SVECTOR *vel, struct SVECTOR *accl, struct FXSplinter *splintDef, struct _FXTracker *fxTracker, void (*fxSetup)(), void (*fxProcess)(), int shardFlags)
{ //line: 1
	// register: $s4
	register long j;
	// address: -80
	auto long maxTimeToLive;
	// address: -76
	auto long faceSkip;
	// register: $v0
	register long numFaces;
	// register: $s5
	// size: 0xC
	register struct _MFace *mface;
	// address: -72
	// size: 0xC
	auto struct _MFace *endMFace;
	// address: -68
	// size: 0x38
	auto struct _Model *model;
	// address: -64
	// size: 0x8
	auto struct _MVertex *vertexList;
	// register: $s0
	// size: 0xC
	register struct _MFace *faceList;
	// register: $s3
	// size: 0x2C
	register struct _FX_MATRIX *fxMatrix;
	// address: -960
	// size: 0xF0
	auto struct _FX_MATRIX *fxMatList[60];
	// address: -720
	// size: 0x1E0
	auto struct _SVector veloc[60];
	// register: $s7
	// size: 0x8
	register struct _SVector *curVel;
	// register: $fp
	register int lastFxMat;
	// address: -240
	// size: 0x80
	auto unsigned char matIdx[128];
	// register: $s2
	// size: 0x68
	register struct _FX_PRIM_duplicate_s24 *fxPrim;
	// register: $s0
	// size: 0x8
	register struct _SVector *vertex0;
	// register: $s1
	// size: 0x8
	register struct _SVector *vertex1;
	// register: $s4
	// size: 0x8
	register struct _SVector *vertex2;
	// address: -60
	// size: 0x8
	auto struct _SVector *poolOfVertices;
	// address: -56
	auto long *vtxSegment;
	// address: -112
	// size: 0xC
	auto struct _Vector Center;
	// register: $s6
	register int seg0;
	// register: $a0
	register int seg1;
	// address: -96
	// size: 0x8
	auto struct _SVector offset;
	{ //line: 83
		// register: $v0
		register short _y0;
		// register: $v1
		register short _z0;
		// register: $a1
		register short _x1;
		// register: $a3
		register short _y1;
		// register: $a2
		register short _z1;
		// address: -48
		// size: 0x8
		auto struct _SVector *_v;
		// register: $a2
		// size: 0xC
		register struct _Vector *_v1;
	} //line: 83
	{ //line: 96
	} //line: 107
	{ //line: 160
		// register: $a1
		register short _x0;
		// register: $v1
		register short _y0;
		// register: $v0
		register short _z0;
		// register: $a0
		register short _x1;
		// register: $a3
		register short _y1;
		// register: $a2
		register short _z1;
		// register: $v0
		// size: 0x6
		register struct _Position *_v0;
		// register: $a2
		// size: 0xC
		register struct _Vector *_v1;
	} //line: 160
	{ //line: 189
		// register: $v1
		// size: 0x10
		register struct TextureMT3 *texture;
	} //line: 189
	{ //line: 219
	} //line: 224
	{ //line: 241
	} //line: 247
} //line: 267


// path: C:\kain2\game\FX.C
// address: 0x80043A5C
// line start: 1254
// line end:   1529
void _FX_BuildNonSegmentedSplinters(struct _Instance *instance, struct SVECTOR *center, struct SVECTOR *vel, struct SVECTOR *accl, struct FXSplinter *splintDef, struct _FXTracker *fxTracker, void (*fxSetup)(), void (*fxProcess)(), int shardFlags)
{ //line: 1
	// register: $s2
	register long j;
	// address: -72
	auto long maxTimeToLive;
	// address: -68
	auto long faceSkip;
	// register: $v0
	register long numFaces;
	// register: $s5
	// size: 0xC
	register struct _MFace *mface;
	// address: -64
	// size: 0xC
	auto struct _MFace *endMFace;
	// register: $v0
	register short whichMatrix;
	// address: -60
	// size: 0x38
	auto struct _Model *model;
	// address: -56
	// size: 0x8
	auto struct _MVertex *vertexList;
	// register: $s4
	// size: 0xC
	register struct _MFace *faceList;
	// register: $s0
	// size: 0x2C
	register struct _FX_MATRIX *fxMatrix;
	// address: -200
	// size: 0x10
	auto struct _FX_MATRIX *fxMatList[4];
	// register: $s6
	// size: 0x8
	register struct _SVector *curVel;
	// register: $s2
	// size: 0x68
	register struct _FX_PRIM_duplicate_s24 *fxPrim;
	// register: $s1
	// size: 0x8
	register struct _SVector *vertex0;
	// register: $s3
	// size: 0x8
	register struct _SVector *vertex1;
	// register: $s4
	// size: 0x8
	register struct _SVector *vertex2;
	// register: $fp
	// size: 0x8
	register struct _SVector *poolOfVertices;
	// register: $s1
	// size: 0x20
	register struct MATRIX *swTransform;
	// address: -184
	// size: 0x20
	auto struct MATRIX ourM;
	// address: -152
	// size: 0xC
	auto struct _Vector Center;
	// address: -136
	// size: 0x8
	auto struct _SVector offset;
	// address: -128
	// size: 0x8
	auto struct _Normal faceNorm;
	// address: -120
	// size: 0x8
	auto struct _SVector sv_temp;
	// address: -112
	// size: 0x20
	auto struct _SVector sv_vels[4];
	{ //line: 42
		// register: $v0
		register short start;
	} //line: 68
	{ //line: 98
		// register: $v0
		register short _x1;
		// register: $v1
		register short _y1;
		// register: $a0
		register short _z1;
		// register: $s3
		// size: 0x8
		register struct _SVector *_v0;
		// register: $a1
		// size: 0x8
		register struct _SVector *_v1;
	} //line: 98
	{ //line: 160
		// register: $a1
		register short _x0;
		// register: $v1
		register short _y0;
		// register: $v0
		register short _z0;
		// register: $a0
		register short _x1;
		// register: $a3
		register short _y1;
		// register: $a2
		register short _z1;
		// address: -48
		// size: 0x8
		auto struct _SVector *_v;
		// register: $v0
		// size: 0x6
		register struct _Position *_v0;
		// register: $a2
		// size: 0xC
		register struct _Vector *_v1;
	} //line: 160
	{ //line: 189
		// register: $v1
		// size: 0x10
		register struct TextureMT3 *texture;
	} //line: 189
	{ //line: 219
	} //line: 219
	{ //line: 245
	} //line: 251
} //line: 276


// path: C:\kain2\game\FX.C
// address: 0x80044648
// line start: 1541
// line end:   1555
void _FX_BuildSplinters(struct _Instance *instance, struct SVECTOR *center, struct SVECTOR *vel, struct SVECTOR *accl, struct FXSplinter *splintDef, struct _FXTracker *fxTracker, void (*fxSetup)(), void (*fxProcess)(), int shardFlags);

// path: C:\kain2\game\FX.C
// address: 0x8004479C
// line start: 1566
// line end:   1737
void _FX_Build(struct _Instance *instance, struct SVECTOR *center, struct SVECTOR *vel, struct SVECTOR *accl, struct _FXTracker *fxTracker, void (*fxSetup)(), void (*fxProcess)(), int shardFlags);

// path: C:\kain2\game\FX.C
// address: 0x800448B0
// line start: 1741
// line end:   1742
void FX_Build(struct _Instance *instance, struct SVECTOR *center, struct SVECTOR *vel, struct SVECTOR *accl, struct _FXTracker *fxTracker, void (*fxSetup)(), void (*fxProcess)());

// path: C:\kain2\game\FX.C
// address: 0x800448E8
// line start: 1747
// line end:   1763
void FX_UpdatePos(struct _FX_PRIM_duplicate_s24 *fxPrim, struct _SVector *offset, int spriteflag)
{ //line: 1
	{ //line: 1
		// register: $v0
		register short _x0;
		// register: $v1
		register short _y0;
		// register: $a0
		register short _z0;
		// register: $a3
		register short _x1;
		// register: $t0
		register short _y1;
		// register: $t1
		register short _z1;
		// register: $a1
		// size: 0x6
		register struct _Position *_v;
	} //line: 1
} //line: 17


// path: C:\kain2\game\FX.C
// address: 0x80044A3C
// line start: 1767
// line end:   1816
void FX_Relocate(struct _SVector *offset)
{ //line: 1
	// register: $s0
	// size: 0x68
	register struct _FX_PRIM_duplicate_s24 *fxPrim;
	// register: $s2
	// size: 0x6DA8
	register struct _FXTracker *fxTracker;
	// register: $a2
	// size: 0x10
	register struct _FXGeneralEffect *currentEffect;
	{ //line: 29
		// register: $a3
		register int i;
		// register: $t1
		register int end;
		// register: $t0
		// size: 0x2C
		register struct _FXRibbon *currentRibbon;
	} //line: 39
	{ //line: 42
	} //line: 46
} //line: 50


// path: C:\kain2\game\FX.C
// address: 0x80044BEC
// line start: 1822
// line end:   1837
void FX_UpdateTexturePointers(struct _FX_PRIM_duplicate_s24 *fxPrim, struct Object *oldObject, long sizeOfObject, long offset);

// path: C:\kain2\game\FX.C
// address: 0x80044C4C
// line start: 1842
// line end:   1867
void FX_RelocateFXPointers(struct Object *oldObject, struct Object *newObject, long sizeOfObject)
{ //line: 1
	// register: $s0
	// size: 0x6DA8
	register struct _FXTracker *fxTracker;
	// register: $a0
	// size: 0x10
	register struct _FXGeneralEffect *currentEffect;
	// register: $s1
	register long offset;
	{ //line: 20
	} //line: 23
} //line: 26


// path: C:\kain2\game\FX.C
// address: 0x80044D1C
// line start: 1872
// line end:   1960
void FX_ProcessList(struct _FXTracker *fxTracker)
{ //line: 1
	// register: $a0
	// size: 0x68
	register struct _FX_PRIM_duplicate_s24 *fxPrim;
	// register: $s0
	// size: 0x68
	register struct _FX_PRIM_duplicate_s24 *nextFXPrim;
	// register: $s0
	// size: 0x2C
	register struct _FX_MATRIX *fxMatrix;
	// register: $s1
	// size: 0x2C
	register struct _FX_MATRIX *nextFXMatrix;
	{ //line: 71
		// register: $a0
		// size: 0x10
		register struct _FXGeneralEffect *currentEffect;
		// register: $s0
		// size: 0x10
		register struct _FXGeneralEffect *nextEffect;
	} //line: 81
} //line: 88


// path: C:\kain2\game\FX.C
// address: 0x80044F18
// line start: 1963
// line end:   1970
void FX_DrawReaver(struct _PrimPool *primPool, unsigned long **ot, struct MATRIX *wcTransform);

// path: C:\kain2\game\FX.C
// address: 0x80044F64
// line start: 1985
// line end:   2562
void FX_DrawList(struct _FXTracker *fxTracker, struct GameTracker *gameTracker, unsigned long **ot, struct MATRIX *wcTransform)
{ //line: 1
	// register: $a1
	// size: 0x10
	register struct TextureMT3 *texture;
	// register: $fp
	// size: 0x1770C
	register struct _PrimPool *primPool;
	// register: $s1
	register long *prim;
	// register: $s0
	// size: 0x68
	register struct _FX_PRIM_duplicate_s24 *fxPrim;
	// register: $s6
	// size: 0x68
	register struct _FX_PRIM_duplicate_s24 *nextFXPrim;
	// address: -104
	// size: 0x8
	auto struct SVECTOR sv0;
	// address: -96
	// size: 0x8
	auto struct SVECTOR sv1;
	// address: -88
	// size: 0x8
	auto struct SVECTOR sv2;
	// address: -52
	auto long otz;
	// address: -64
	auto long sz0;
	// address: -60
	auto long sz1;
	// address: -56
	auto long sz2;
	// address: -80
	// size: 0x4
	auto char whitec[4];
	// address: -48
	auto int sizex;
	// register: $t4
	register int sizey;
	// register: $t2
	register int matrix_wc;
	{ //line: 22
		// register: $s3
		register long flags;
		{ //line: 110
			// register: $a2
			// size: 0x28
			register struct _POLY_NG4 *ng4;
			{ //line: 112
				// register: $a0
				register int n;
				// register: $a1
				register long *ptr;
			} //line: 137
		} //line: 159
		{ //line: 176
		} //line: 178
		{ //line: 266
			// register: $a2
			// size: 0x28
			register struct POLY_FT4 *ft4;
			// register: $t1
			register unsigned short uMin;
			// register: $t0
			register unsigned short uMax;
			// register: $a3
			register unsigned short vMin;
			// register: $v1
			register unsigned short vMax;
		} //line: 297
		{ //line: 304
			// register: $t0
			// size: 0x20
			register struct POLY_FT3 *ft3;
			{ //line: 317
				// register: $a3
				register short uMin;
				// register: $a2
				register short uMax;
			} //line: 325
		} //line: 331
	} //line: 335
	{ //line: 346
		// address: -72
		// size: 0x4
		auto struct DVECTOR xy_pos;
		// register: $t2
		register long flags;
		{ //line: 415
			// register: $a1
			// size: 0x40
			register struct _POLY_SG4 *sg4;
			{ //line: 417
				// register: $a3
				register int n;
				// register: $t2
				register long *src;
				// register: $t1
				register long *dst;
				// register: $t3
				register long *ptr;
			} //line: 448
		} //line: 470
		{ //line: 482
		} //line: 486
		{ //line: 514
			// register: $t3
			register unsigned short uMin;
			// register: $t1
			register unsigned short uMax;
			// register: $a3
			register unsigned short vMin;
			// register: $v1
			register unsigned short vMax;
			// register: $a2
			// size: 0x28
			register struct POLY_FT4 *ft4;
		} //line: 552
	} //line: 571
} //line: 575


// path: C:\kain2\game\FX.C
// address: 0x8004616C
// line start: 2727
// line end:   2793
void FX_SimpleQuadSetup(struct _FX_PRIM_duplicate_s24 *fxPrim, void (*fxProcess)(), struct _FX_MATRIX *fxMatrix, struct _Instance *instance, struct _MFace *mface, struct _MVertex *vertexList, struct SVECTOR *center, struct SVECTOR *vel, struct SVECTOR *accl, struct _FXTracker *fxTracker, int timeToLive)
{ //line: 1
	// register: $s2
	// size: 0x8
	register struct _MVertex *vertex1;
	// register: $s3
	// size: 0x8
	register struct _MVertex *vertex2;
	// register: $s4
	// size: 0x8
	register struct _MVertex *vertex3;
	{ //line: 1
		// register: $v0
		register short _x1;
		// register: $v1
		register short _y1;
		// register: $a0
		register short _z1;
		// register: $v0
		// size: 0x6
		register struct _Position *_v0;
	} //line: 1
	{ //line: 1
		// register: $v0
		register short _x1;
		// register: $v1
		register short _y1;
		// register: $a0
		register short _z1;
		// register: $v0
		// size: 0x8
		register struct _SVector *_v0;
	} //line: 1
	{ //line: 1
		// register: $v0
		register short _x1;
		// register: $v1
		register short _y1;
		// register: $a0
		register short _z1;
		// register: $v0
		// size: 0x8
		register struct _SVector *_v0;
	} //line: 1
	{ //line: 1
		// register: $v0
		register short _x1;
		// register: $v1
		register short _y1;
		// register: $a0
		register short _z1;
		// register: $v0
		// size: 0x8
		register struct _SVector *_v0;
	} //line: 1
	{ //line: 1
		// register: $v0
		register short _x1;
		// register: $v1
		register short _y1;
		// register: $a0
		register short _z1;
		// register: $v0
		// size: 0x8
		register struct _SVector *_v0;
	} //line: 1
} //line: 65


// path: C:\kain2\game\FX.C
// address: 0x800463A8
// line start: 2796
// line end:   2802
void FX_WaterRingProcess(struct _FX_PRIM_duplicate_s24 *fxPrim, struct _FXTracker *fxTracker);

// path: C:\kain2\game\FX.C
// address: 0x80046424
// line start: 2806
// line end:   2863
void FX_WaterBubbleProcess(struct _FX_PRIM_duplicate_s24 *fxPrim, struct _FXTracker *fxTracker)
{ //line: 1
	// register: $a1
	// size: 0x68
	register struct _FX_PRIM_duplicate_s24 *temp;
	{ //line: 27
		// register: $v1
		// size: 0x4C
		register struct Object *waterfx;
		// register: $s1
		// size: 0x38
		register struct _Model *wxring;
	} //line: 57
} //line: 58


// path: C:\kain2\game\FX.C
// address: 0x8004663C
// line start: 2867
// line end:   2869
void FX_Sprite_Insert(struct NodeType *list, struct _FX_PRIM_duplicate_s24 *fxPrim);

// path: C:\kain2\game\FX.C
// address: 0x80046674
// size: 0x10
// line start: 2873
// line end:   2884
struct TextureMT3 *FX_GetTextureObject(struct Object *object, int modelnum, int texnum)
{ //line: 2
	// register: $v0
	// size: 0x38
	register struct _Model *model;
	// register: $v0
	// size: 0x10
	register struct TextureMT3 *texture;
} //line: 6


// path: C:\kain2\game\FX.C
// address: 0x800466B0
// line start: 2887
// line end:   2918
void FX_MakeWaterBubble(struct _SVector *position, struct _SVector *vel, struct _SVector *accl, long splashZ, struct __BubbleParams *BP)
{ //line: 1
	// register: $s3
	// size: 0x4C
	register struct Object *waterfx;
	// register: $s1
	// size: 0x68
	register struct _FX_PRIM_duplicate_s24 *fxPrim;
} //line: 32


// path: C:\kain2\game\FX.C
// address: 0x80046860
// line start: 2977
// line end:   2993
void FX_DrawScreenPoly(int transtype, unsigned long color, int zdepth)
{ //line: 1
	// register: $t0
	register unsigned long **drawot;
	// register: $a3
	// size: 0x1C
	register struct _POLY_TF4 *poly;
} //line: 17


// path: C:\kain2\game\FX.C
// address: 0x8004690C
// size: 0x34
// line start: 2997
// line end:   3028
struct POLY_GT4 *FX_SetupPolyGT4(int x1, int y1, int x2, int y2, int otz, struct TextureMT3 *texture, long color0, long color1, long color2, long color3)
{ //line: 1
	// register: $t0
	// size: 0x34
	register struct POLY_GT4 *poly;
	// register: $t3
	register unsigned long **drawot;
} //line: 32


// path: C:\kain2\game\FX.C
// address: 0x80046A10
// line start: 3058
// line end:   3074
void FX_MakeWarpArrow(int x, int y, int xsize, int ysize, int fade)
{ //line: 1
	// register: $a0
	// size: 0x4C
	register struct Object *particle;
	// register: $s0
	register long color;
	// register: $v1
	// size: 0x34
	register struct POLY_GT4 *poly;
} //line: 17


// path: C:\kain2\game\FX.C
// address: 0x80046AF0
// line start: 3086
// line end:   3123
void FX_MakeMannaIcon(int x, int y, int xsize, int ysize)
{ //line: 1
	// register: $a0
	// size: 0x4C
	register struct Object *manna;
	// register: $s4
	// size: 0x10
	register struct TextureMT3 *texture;
	// register: $v1
	// size: 0x34
	register struct POLY_GT4 *poly;
	// register: $s0
	register int n;
	// register: $a0
	register int newx;
	// register: $a1
	register int newy;
	// register: $a2
	register int sizex;
	// register: $v0
	register int sizey;
	// register: $v1
	register unsigned long color;
} //line: 38


// path: C:\kain2\game\FX.C
// address: 0x80046C9C
// line start: 3197
// line end:   3249
void FX_MakeGlyphIcon(struct _Position *position, struct Object *glyphObject, int size, int glyphnum, int enabled)
{ //line: 1
	// register: $t0
	register int sizex;
	// register: $v1
	// size: 0x34
	register struct POLY_GT4 *poly;
	// register: $s2
	// size: 0x10
	register struct TextureMT3 *texture;
	// address: -40
	// size: 0x4
	auto struct DVECTOR xy_pos;
	// register: $t1
	register int otz;
	// register: $v0
	register long *color_array;
	// register: $v1
	// size: 0x8C
	register struct _GlyphTuneData *glyphtunedata;
	{ //line: 14
		// register: $a0
		// size: 0x4C
		register struct Object *object;
	} //line: 17
} //line: 53


// path: C:\kain2\game\FX.C
// address: 0x80046E60
// line start: 3253
// line end:   3290
void FX_SoulDustProcess(struct _FX_PRIM_duplicate_s24 *fxPrim, struct _FXTracker *fxTracker)
{ //line: 1
	// register: $v1
	// size: 0x20
	register struct MATRIX *swTransform;
	// address: -24
	// size: 0x6
	auto struct _Position position;
	// address: -16
	auto long color;
	// address: -12
	auto long black;
	// register: $a3
	register int fade;
} //line: 38


// path: C:\kain2\game\FX.C
// address: 0x80047014
// line start: 3292
// line end:   3343
void FX_MakeSoulDust(struct _Instance *instance, short segment)
{ //line: 1
	// register: $s1
	// size: 0x68
	register struct _FX_PRIM_duplicate_s24 *fxPrim;
	// address: -32
	// size: 0x8
	auto struct SVECTOR location;
	// register: $s2
	// size: 0x4C
	register struct Object *particle;
} //line: 52


// path: C:\kain2\game\FX.C
// address: 0x80047254
// line start: 3367
// line end:   3381
void FX_WaterTrailProcess(struct _FX_PRIM_duplicate_s24 *fxPrim, struct _FXTracker *fxTracker);

// path: C:\kain2\game\FX.C
// address: 0x8004736C
// line start: 3386
// line end:   3444
void FX_MakeWaterTrail(struct _Instance *instance, int depth)
{ //line: 1
	// register: $a1
	// size: 0x4C
	register struct Object *waterfx;
	// register: $a0
	// size: 0x38
	register struct _Model *wxtrail;
	// address: -48
	// size: 0x8
	auto struct _SVector position;
	// register: $s0
	register int zvel;
	{ //line: 37
		// register: $s2
		register int n;
		// register: $s0
		register int deg;
		// address: -40
		// size: 0x8
		auto struct _SVector vel;
		// address: -32
		// size: 0x8
		auto struct _SVector accel;
		// address: -24
		// size: 0x8
		auto struct _SVector startpos;
		{ //line: 45
			// register: $s1
			register int sinVal;
			// register: $s0
			register int cosVal;
			// register: $v1
			register int spd;
		} //line: 53
	} //line: 58
} //line: 59


// path: C:\kain2\game\FX.C
// address: 0x800475A0
// size: 0x2C
// line start: 3480
// line end:   3562
struct _FXRibbon *FX_StartRibbon(struct _Instance *instance, short startSegment, short endSegment, short type, int ribbonLifeTime, int faceLifeTime, int startFadeValue, long startColor, long endColor)
{ //line: 1
	// register: $a0
	// size: 0x20
	register struct MATRIX *swTransform;
	// register: $s0
	// size: 0x2C
	register struct _FXRibbon *ribbon;
	// register: $a2
	register int i;
	// register: $s1
	register int number;
} //line: 83


// path: C:\kain2\game\FX.C
// address: 0x80047870
// line start: 3584
// line end:   3623
void FX_RibbonProcess(struct _FX_PRIM_duplicate_s24 *fxPrim, struct _FXTracker *fxTracker)
{ //line: 1
	// register: $a1
	register int d;
	// register: $a3
	register long fade;
} //line: 40


// path: C:\kain2\game\FX.C
// address: 0x80047964
// line start: 3627
// line end:   3692
void FX_ConstrictProcess(struct _FX_PRIM_duplicate_s24 *fxPrim, struct _FXTracker *fxTracker);

// path: C:\kain2\game\FX.C
// address: 0x80047C08
// line start: 3696
// line end:   3710
void FX_StartConstrict(struct _Instance *instance, struct _SVector *constrict_point, short startSegment, short endSegment);

// path: C:\kain2\game\FX.C
// address: 0x80047C98
// line start: 3713
// line end:   3730
void FX_EndConstrict(int ConstrictEnemyFlag, struct _Instance *instance)
{ //line: 1
	{ //line: 8
		// register: $v1
		register short _x1;
		// register: $a0
		register short _y1;
		// register: $a1
		register short _z1;
		// register: $v0
		// size: 0x6
		register struct _Position *_v0;
		// register: $v0
		// size: 0x6
		register struct _Position *_v1;
	} //line: 8
	{ //line: 12
		// register: $v1
		register short _x1;
		// register: $a0
		register short _y1;
		// register: $a1
		register short _z1;
		// register: $v0
		// size: 0x6
		register struct _Position *_v0;
		// register: $v0
		// size: 0x6
		register struct _Position *_v1;
	} //line: 12
} //line: 16


// path: C:\kain2\game\FX.C
// address: 0x80047D04
// line start: 3733
// line end:   3742
void FX_SubDividePrim(struct _FX_PRIM_duplicate_s24 *fxPrim1, struct _FX_PRIM_duplicate_s24 *fxPrim2)
{ //line: 1
	// address: -32
	// size: 0x8
	auto struct SVECTOR mp0;
	// address: -24
	// size: 0x8
	auto struct SVECTOR mp1;
} //line: 1


// path: C:\kain2\game\FX.C
// address: 0x80047DC8
// line start: 3781
// line end:   3947
void FX_ContinueRibbon(struct _FXRibbon *ribbon, struct _FXTracker *fxTracker)
{ //line: 1
	// register: $a1
	// size: 0x20
	register struct MATRIX *swTransform;
	// register: $s3
	register int i;
	// register: $s7
	register int i2;
	// register: $v1
	register int i3;
	// register: $a2
	register int startIndex;
	// address: -48
	auto int period;
	// register: $s5
	register int d;
	// register: $s0
	// size: 0x68
	register struct _FX_PRIM_duplicate_s24 *fxPrim;
	// register: $s1
	// size: 0x68
	register struct _FX_PRIM_duplicate_s24 *fxPrim2;
	// address: -80
	// size: 0x20
	auto struct SVECTOR Quad[4];
	{ //line: 128
		// register: $v1
		register int fade;
	} //line: 152
} //line: 167


// path: C:\kain2\game\FX.C
// address: 0x80048398
// line start: 3953
// line end:   4189
void FX_StandardFXPrimProcess(struct _FX_PRIM_duplicate_s24 *fxPrim, struct _FXTracker *fxTracker)
{ //line: 1
	// register: $a2
	register long flags;
	{ //line: 16
		// address: -16
		auto long start;
		// address: -12
		auto long end;
	} //line: 31
	{ //line: 35
		// register: $v1
		register int current_scale;
	} //line: 45
	{ //line: 50
		// register: $v1
		// size: 0x20
		register struct MATRIX *swTransform;
	} //line: 51
	{ //line: 220
		// address: -24
		// size: 0x8
		auto struct _Rotation rot;
	} //line: 228
} //line: 237


// path: C:\kain2\game\FX.C
// address: 0x80048708
// line start: 4192
// line end:   4207
void FX_AttachedParticlePrimProcess(struct _FX_PRIM_duplicate_s24 *fxPrim, struct _FXTracker *fxTracker)
{ //line: 1
	// register: $a3
	// size: 0x20
	register struct MATRIX *swTransform;
	// register: $t0
	// size: 0x20
	register struct MATRIX *swTransformOld;
	// register: $v1
	// size: 0x29C
	register struct _Instance *instance;
} //line: 15


// path: C:\kain2\game\FX.C
// address: 0x8004879C
// line start: 4210
// line end:   4243
void FX_FlamePrimProcess(struct _FX_PRIM_duplicate_s24 *fxPrim, struct _FXTracker *fxTracker)
{ //line: 1
	// register: $a0
	// size: 0x20
	register struct MATRIX *swTransform;
	// register: $a2
	// size: 0x20
	register struct MATRIX *swTransformOld;
	// register: $v0
	// size: 0x29C
	register struct _Instance *instance;
	// address: -16
	// size: 0x8
	auto struct _SVector movement;
	// register: $a0
	register int total;
} //line: 34


// path: C:\kain2\game\FX.C
// address: 0x80048970
// line start: 4256
// line end:   4298
void FX_DFacadeParticleSetup(struct _FX_PRIM_duplicate_s24 *fxPrim, struct SVECTOR *center, short halveWidth, short halveHeight, long color, struct SVECTOR *vel, struct SVECTOR *accl, struct _FXTracker *fxTracker, int timeToLive)
{ //line: 1
	{ //line: 1
		// register: $v0
		register short _x1;
		// register: $v1
		register short _y1;
		// register: $a0
		register short _z1;
		// register: $v0
		// size: 0x6
		register struct _Position *_v0;
	} //line: 1
} //line: 42


// path: C:\kain2\game\FX.C
// address: 0x80048A58
// size: 0x68
// line start: 4302
// line end:   4333
struct _FX_PRIM_duplicate_s24 *FX_Dot(struct _SVector *location, struct _SVector *vel, struct _SVector *accel, int scale_speed, long color, long size, int lifetime, int texture_num)
{ //line: 1
	// register: $s0
	// size: 0x68
	register struct _FX_PRIM_duplicate_s24 *fxPrim;
} //line: 31


// path: C:\kain2\game\FX.C
// address: 0x80048BC4
// line start: 4336
// line end:   4348
void FX_Blood(struct _SVector *location, struct _SVector *input_vel, struct _SVector *accel, int amount, long color, long size)
{ //line: 1
	// register: $s0
	register int i;
	{ //line: 6
		// address: -40
		// size: 0x8
		auto struct _SVector vel;
	} //line: 10
} //line: 13


// path: C:\kain2\game\FX.C
// address: 0x80048D00
// line start: 4351
// line end:   4352
void FX_Blood2(struct _SVector *location, struct _SVector *input_vel, struct _SVector *accel, int amount, long color, long dummyCrapShouldRemove);

// path: C:\kain2\game\FX.C
// address: 0x80048D2C
// line start: 4383
// line end:   4408
void FX_Blood_Impale(struct _Instance *locinst, short locseg, struct _Instance *instance, short segment)
{ //line: 1
	// address: -40
	// size: 0x8
	auto struct _SVector location;
	// address: -32
	// size: 0x8
	auto struct _SVector accel;
	// address: -24
	// size: 0x8
	auto struct _SVector vel;
	// address: -16
	// size: 0x8
	auto struct _SVector input_vel;
	// register: $s0
	register int i;
} //line: 26


// path: C:\kain2\game\FX.C
// address: 0x80048EF8
// size: 0x4C
// line start: 4415
// line end:   4443
struct _FXParticle *FX_BloodCone(struct _Instance *instance, short startSegment, long time)
{ //line: 1
	// register: $s0
	// size: 0x4C
	register struct _FXParticle *currentParticle;
	// register: $s1
	// size: 0x4C
	register struct Object *particle;
} //line: 29


// path: C:\kain2\game\FX.C
// address: 0x80048FC4
// size: 0x4C
// line start: 4447
// line end:   4472
struct _FXParticle *FX_GetTorchParticle(struct _Instance *instance, short startSegment, int tex, int birthRadius, int num)
{ //line: 1
	// register: $s0
	// size: 0x4C
	register struct _FXParticle *currentParticle;
} //line: 25


// path: C:\kain2\game\FX.C
// address: 0x800490A8
// size: 0x4C
// line start: 4475
// line end:   4523
struct _FXParticle *FX_TorchFlame(struct _Instance *instance, short startSegment)
{ //line: 1
	// register: $s0
	// size: 0x4C
	register struct _FXParticle *currentParticle;
	// register: $s3
	// size: 0x4C
	register struct Object *particle;
} //line: 49


// path: C:\kain2\game\FX.C
// address: 0x80049204
// line start: 4526
// line end:   4531
int FX_GetMorphFadeVal()
{ //line: 2
	// register: $a0
	register int fade;
} //line: 5


// path: C:\kain2\game\FX.C
// address: 0x80049244
// line start: 4550
// line end:   4566
void FX_ConvertCamPersToWorld(struct SVECTOR *campos, struct SVECTOR *worldpos);

// path: C:\kain2\game\FX.C
// address: 0x8004935C
// line start: 4570
// line end:   4573
void FX_GetRandomScreenPt(struct SVECTOR *point);

// path: C:\kain2\game\FX.C
// address: 0x800493E8
// line start: 4577
// line end:   4610
void FX_ProcessSnow(struct _FX_PRIM_duplicate_s24 *fxPrim, struct _FXTracker *fxTracker)
{ //line: 1
	{ //line: 4
		// address: -24
		// size: 0x8
		auto struct SVECTOR position;
	} //line: 20
} //line: 34


// path: C:\kain2\game\FX.C
// address: 0x8004951C
// line start: 4613
// line end:   4654
void FX_ContinueSnow(struct _FXTracker *fxTracker)
{ //line: 1
	// register: $s0
	// size: 0x68
	register struct _FX_PRIM_duplicate_s24 *fxPrim;
	// address: -40
	// size: 0x8
	auto struct SVECTOR position;
	// address: -32
	// size: 0x8
	auto struct SVECTOR vel;
	{ //line: 18
		// address: -24
		// size: 0x8
		auto struct SVECTOR campos;
	} //line: 40
} //line: 42


// path: C:\kain2\game\FX.C
// address: 0x80049674
// line start: 4657
// line end:   4677
void FX_UpdateWind(struct _FXTracker *fxTracker)
{ //line: 1
	// register: $s0
	register short change;
} //line: 18


// path: C:\kain2\game\FX.C
// address: 0x800497EC
// line start: 4682
// line end:   4704
void FX_ProcessRain(struct _FX_PRIM_duplicate_s24 *fxPrim, struct _FXTracker *fxTracker)
{ //line: 1
	// register: $a3
	register int zvel;
} //line: 23


// path: C:\kain2\game\FX.C
// address: 0x800498BC
// line start: 4707
// line end:   4797
void FX_ContinueRain(struct _FXTracker *fxTracker)
{ //line: 1
	// register: $s0
	// size: 0x68
	register struct _FX_PRIM_duplicate_s24 *fxPrim;
	// address: -56
	// size: 0x8
	auto struct SVECTOR campos;
	// register: $s4
	register int n;
	// register: $s6
	register int rain_pct;
	// register: $s2
	register long waterZLevel;
	// register: $s1
	register int slack;
	// register: $v0
	register int fade;
	{ //line: 46
		// address: -48
		// size: 0x8
		auto struct _SVector worldpos;
		// register: $s3
		register int zvel;
	} //line: 89
} //line: 91


// path: C:\kain2\game\FX.C
// address: 0x80049BF4
// line start: 4803
// line end:   4832
void FX_MakeSpark(struct _Instance *instance, struct _Model *model, int segment)
{ //line: 1
	// register: $s0
	// size: 0x4C
	register struct _FXParticle *currentParticle;
	// register: $s1
	// size: 0x4C
	register struct Object *particle;
} //line: 30


// path: C:\kain2\game\FX.C
// address: 0x80049CB0
// line start: 4835
// line end:   5034
void FX_ContinueParticle(struct _FXParticle *currentParticle, struct _FXTracker *fxTracker)
{ //line: 1
	// register: $s1
	// size: 0x68
	register struct _FX_PRIM_duplicate_s24 *fxPrim;
	// address: -64
	// size: 0x10
	auto struct VECTOR movement;
	// register: $fp
	register int i;
	// address: -44
	auto int num;
	// register: $s4
	// size: 0x20
	register struct MATRIX *swTransform;
	// register: $a1
	// size: 0x20
	register struct MATRIX *swOldTransform;
	// register: $s5
	register long birthRadius;
	// register: $s6
	// size: 0x29C
	register struct _Instance *instance;
	// register: $s3
	register int InstanceFade;
	// address: -48
	auto unsigned long black;
	{ //line: 121
		// register: $v1
		register int tmp_blue;
		// register: $s0
		// size: 0x4
		register struct CVECTOR *ptr;
		{ //line: 128
			// register: $v0
			register int fade;
		} //line: 134
	} //line: 135
} //line: 200


// path: C:\kain2\game\FX.C
// address: 0x8004A4F0
// line start: 5037
// line end:   5044
void FX_UpdraftPrimModify(struct _FX_PRIM_duplicate_s24 *fxPrim, struct _Instance *instance, struct _FXParticle *particle, struct _FXTracker *fxTracker)
{ //line: 1
	// register: $s2
	register int deg;
} //line: 7


// path: C:\kain2\game\FX.C
// address: 0x8004A634
// line start: 5047
// line end:   5067
void FX_MakeParticleTexFX(struct _FX_PRIM_duplicate_s24 *fxPrim, struct _SVector *position, struct Object *object, int modelnum, int texnum, struct _SVector *vel, struct _SVector *accl, long color, int size, int life);

// path: C:\kain2\game\FX.C
// address: 0x8004A73C
// line start: 5070
// line end:   5081
void FX_MakeHitFX(struct _SVector *position)
{ //line: 1
	// register: $s0
	// size: 0x68
	register struct _FX_PRIM_duplicate_s24 *fxPrim;
} //line: 12


// path: C:\kain2\game\FX.C
// address: 0x8004A7D0
// line start: 5108
// line end:   5115
void FX_ContinueLightning(struct _FXLightning *zap, struct _FXTracker *fxTracker);

// path: C:\kain2\game\FX.C
// address: 0x8004A830
// line start: 5120
// line end:   5121
void FX_SetReaverInstance(struct _Instance *instance);

// path: C:\kain2\game\FX.C
// address: 0x8004A83C
// line start: 5128
// line end:   5255
void FX_SoulReaverBlade(struct _Instance *instance, unsigned long **drawot)
{ //line: 1
	// address: -136
	// size: 0x8
	auto struct SVECTOR location;
	// register: $s3
	register int i;
	// register: $s2
	register int size;
	// register: $s7
	register int reaverScale;
	// register: $s2
	// size: 0x4C
	register struct Object *sreaver;
	// register: $s1
	// size: 0x24
	register struct __ReaverData *data;
	// register: $s4
	register short deg;
	// register: $fp
	register long color;
	// register: $v0
	// size: 0x10
	register struct TextureMT3 *texture;
	// address: -128
	// size: 0x20
	auto struct MATRIX mat;
	// address: -96
	// size: 0x4
	auto struct DVECTOR xy_pos;
	// address: -48
	auto long sizex;
	// register: $a0
	register long sizey;
	// address: -44
	auto long otz;
	// register: $s5
	// size: 0x28
	register struct POLY_FT4 *poly;
	// address: -88
	// size: 0x28
	auto struct POLY_FT4 poly2;
	{ //line: 58
		// register: $s0
		register int scale1;
	} //line: 124
} //line: 128


// path: C:\kain2\game\FX.C
// address: 0x8004AC2C
// line start: 5268
// line end:   5269
void FX_ReaverBladeInit();

// path: C:\kain2\game\FX.C
// address: 0x8004AC34
// line start: 5275
// line end:   5349
void FX_SoulReaverWinding(struct _Instance *instance, struct _PrimPool *primPool, unsigned long **ot, struct MATRIX *wcTransform)
{ //line: 1
	// address: -96
	// size: 0x20
	auto struct MATRIX mat;
	// register: $a1
	// size: 0x20
	register struct MATRIX *swTransform;
	// address: -64
	// size: 0x8
	auto struct _SVector start;
	// address: -56
	// size: 0x8
	auto struct _SVector end;
	// register: $s4
	// size: 0x24
	register struct __ReaverData *data;
	// register: $s5
	register long color;
	// address: -48
	auto long glow_color;
} //line: 75


// path: C:\kain2\game\FX.C
// address: 0x8004AE84
// line start: 5356
// line end:   5386
void FX_UpdateInstanceWaterSplit(struct _Instance *instance)
{ //line: 1
	// address: -16
	// size: 0x8
	auto struct _SVector normal;
} //line: 28


// path: C:\kain2\game\FX.C
// address: 0x8004AF34
// line start: 5389
// line end:   5394
void FX_GetPlaneEquation(struct _SVector *normal, struct _SVector *poPlane, struct _PlaneConstants *plane);

// path: C:\kain2\game\FX.C
// address: 0x8004AFA8
// line start: 5432
// line end:   5479
void FX_DoInstancePowerRing(struct _Instance *instance, long atuTime, long *color, long numColors, int follow_halveplane)
{ //line: 1
	// register: $s0
	// size: 0x2C
	register struct _FXHalvePlane *ring;
	// address: -40
	// size: 0x8
	auto struct _SVector normal;
	// address: -32
	// size: 0x8
	auto struct _SVector point;
	// register: $a1
	register long i;
} //line: 48


// path: C:\kain2\game\FX.C
// address: 0x8004B128
// line start: 5484
// line end:   5544
void FX_UpdatePowerRing(struct _FXHalvePlane *ring)
{ //line: 1
	// register: $v1
	// size: 0x29C
	register struct _Instance *instance;
	// register: $s1
	// size: 0xC
	register struct _PlaneConstants *cPlane;
	// register: $s5
	register long offset;
	// register: $s4
	register long offset2;
	// register: $s6
	register long cutX;
	// register: $s7
	register long cutY;
	// register: $s3
	register long cutZ;
	// register: $a2
	register long colorIndex;
	// register: $a1
	register long percent;
} //line: 60


// path: C:\kain2\game\FX.C
// address: 0x8004B3AC
// line start: 5629
// line end:   5648
void FX_UpdateInstanceSplitRing(struct _FXHalvePlane *ring, struct _FXTracker *fxTracker);

// path: C:\kain2\game\FX.C
// address: 0x8004B43C
// line start: 5652
// line end:   5656
void FX_UpdateGlowEffect(struct _FXGlowEffect *effect, struct _FXTracker *fxTracker);

// path: C:\kain2\game\FX.C
// address: 0x8004B46C
// line start: 5660
// line end:   5664
void FX_InsertGeneralEffect(void *ptr)
{ //line: 2
} //line: 3


// path: C:\kain2\game\FX.C
// address: 0x8004B47C
// line start: 5668
// line end:   5700
void FX_DeleteGeneralEffect(struct _FXGeneralEffect *effect)
{ //line: 1
	// register: $v0
	// size: 0x10
	register struct _FXGeneralEffect *currentEffect;
	// register: $v1
	// size: 0x10
	register struct _FXGeneralEffect *previousEffect;
} //line: 33


// path: C:\kain2\game\FX.C
// address: 0x8004B518
// size: 0x2C
// line start: 5706
// line end:   5759
struct _FXGlowEffect *FX_DoInstanceOneSegmentGlow(struct _Instance *instance, long segment, long *color, long numColors, long atuColorCycleRate, long width, long height)
{ //line: 1
	// register: $a2
	// size: 0x2C
	register struct _FXGlowEffect *glowEffect;
	{ //line: 33
		// register: $a1
		register int i;
	} //line: 46
} //line: 52


// path: C:\kain2\game\FX.C
// address: 0x8004B678
// line start: 5763
// line end:   5765
void FX_SetGlowFades(struct _FXGlowEffect *glowEffect, int fadein, int fadeout);

// path: C:\kain2\game\FX.C
// address: 0x8004B694
// size: 0x2C
// line start: 5769
// line end:   5782
struct _FXGlowEffect *FX_DoInstanceTwoSegmentGlow(struct _Instance *instance, long segment, long segmentEnd, long *color, long numColors, long atuColorCycleRate, long height)
{ //line: 1
	// register: $v0
	// size: 0x2C
	register struct _FXGlowEffect *glowEffect;
	// register: $s0
	register int inc;
} //line: 9


// path: C:\kain2\game\FX.C
// address: 0x8004B6EC
// size: 0x2C
// line start: 5785
// line end:   5793
struct _FXGlowEffect *FX_DoInstanceManySegmentGlow(struct _Instance *instance, long segment, long numSegments, long *color, long numColors, long atuColorCycleRate, long height)
{ //line: 1
	// register: $v0
	// size: 0x2C
	register struct _FXGlowEffect *glowEffect;
} //line: 1


// path: C:\kain2\game\FX.C
// address: 0x8004B734
// size: 0x2C
// line start: 5796
// line end:   5803
struct _FXGlowEffect *FX_DoInstanceOneSegmentGlowWithTime(struct _Instance *instance, long segment, long *color, long numColors, long atuColorCycleRate, long width, long height, long ATULifeTime)
{ //line: 1
	// register: $v0
	// size: 0x2C
	register struct _FXGlowEffect *glowEffect;
} //line: 1


// path: C:\kain2\game\FX.C
// address: 0x8004B77C
// line start: 5808
// line end:   5832
void FX_StopAllGlowEffects(struct _Instance *instance, int fadeout_time)
{ //line: 1
	// register: $a0
	// size: 0x2C
	register struct _FXGlowEffect *currentEffect;
	// register: $s1
	// size: 0x2C
	register struct _FXGlowEffect *previousEffect;
	{ //line: 16
	} //line: 17
} //line: 25


// path: C:\kain2\game\FX.C
// address: 0x8004B810
// line start: 5835
// line end:   5858
void FX_StopGlowEffect(struct _FXGlowEffect *glowEffect, int fadeout_time)
{ //line: 1
	{ //line: 10
		// register: $a0
		// size: 0x10
		register struct _FXGeneralEffect *currentEffect;
		// register: $s0
		// size: 0x10
		register struct _FXGeneralEffect *previousEffect;
	} //line: 23
} //line: 24


// path: C:\kain2\game\FX.C
// address: 0x8004B898
// line start: 5861
// line end:   5927
void FX_DrawLightning(struct _FXLightning *zap, struct MATRIX *wcTransform, unsigned long **ot)
{ //line: 1
	// address: -80
	// size: 0x8
	auto struct _SVector start;
	// address: -72
	// size: 0x8
	auto struct _SVector end;
	// address: -64
	// size: 0x8
	auto struct _SVector offset;
	// register: $s0
	// size: 0x20
	register struct MATRIX *swtransform;
	// address: -56
	// size: 0x20
	auto struct MATRIX mat;
} //line: 63


// path: C:\kain2\game\FX.C
// address: 0x8004BB4C
// line start: 5931
// line end:   5988
void FX_DrawAllGeneralEffects(struct MATRIX *wcTransform, struct _VertexPool *vertexPool, struct _PrimPool *primPool, unsigned long **ot)
{ //line: 1
	// register: $a0
	// size: 0x29C
	register struct _Instance *instance;
	// register: $s0
	// size: 0x10
	register struct _FXGeneralEffect *currentEffect;
	{ //line: 14
		// register: $t0
		// size: 0x2C
		register struct _FXGlowEffect *currentGlow;
	} //line: 22
	{ //line: 26
	} //line: 27
	{ //line: 36
	} //line: 38
	{ //line: 46
	} //line: 48
} //line: 58


// path: C:\kain2\game\FX.C
// address: 0x8004BD24
// line start: 5999
// line end:   6075
void FX_ContinueBlastRing(struct _FXBlastringEffect *blast, struct _FXTracker *fxTracker)
{ //line: 1
	// register: $a2
	register int fade;
	// register: $v0
	register int tm;
	{ //line: 12
		// register: $v0
		register int rad;
		// register: $a0
		register int crad;
		// register: $v1
		register int endrad;
	} //line: 44
	{ //line: 48
		// register: $a0
		register unsigned long *colorPtr;
		// address: -16
		auto unsigned long black;
		// register: $a3
		register int fade;
	} //line: 56
} //line: 77


// path: C:\kain2\game\FX.C
// address: 0x8004BF38
// size: 0x78
// line start: 6106
// line end:   6147
struct _FXBlastringEffect *FX_DoBlastRing(struct _Instance *instance, struct _SVector *position, struct MATRIX *mat, int segment, int radius, int endRadius, int colorChangeRadius, int size1, int size2, int vel, int accl, int height1, int height2, int height3, long startColor, long endColor, int pred_offset, int lifeTime, int sortInWorld)
{ //line: 1
	// register: $s0
	// size: 0x78
	register struct _FXBlastringEffect *blast;
} //line: 41


// path: C:\kain2\game\FX.C
// address: 0x8004C10C
// line start: 6150
// line end:   6192
void FX_DrawBlastring(struct MATRIX *wcTransform, struct _FXBlastringEffect *blast)
{ //line: 1
	// register: $s2
	register int radius;
	// address: -64
	// size: 0x8
	auto struct _SVector position;
	// address: -56
	// size: 0x20
	auto struct MATRIX mat;
	{ //line: 8
		// register: $s0
		// size: 0x20
		register struct MATRIX *swtransform;
	} //line: 21
} //line: 43


// path: C:\kain2\game\FX.C
// address: 0x8004C2D8
// line start: 6196
// line end:   6202
void FX_ContinueFlash(struct _FXFlash *flash, struct _FXTracker *fxTracker);

// path: C:\kain2\game\FX.C
// address: 0x8004C328
// line start: 6206
// line end:   6237
void FX_DrawFlash(struct _FXFlash *flash)
{ //line: 1
	// register: $a1
	register int time;
	// register: $a2
	register int div;
	// register: $s0
	register int transtype;
	// register: $a3
	register int interp;
	// address: -16
	auto unsigned long color;
	// address: -12
	auto unsigned long black;
} //line: 31


// path: C:\kain2\game\FX.C
// address: 0x8004C43C
// line start: 6240
// line end:   6251
void FX_RelocateGeneric(struct Object *object, long offset)
{ //line: 2
	// register: $v0
	// size: 0x1C
	register struct GenericFXObject *GFXO;
} //line: 11


// path: C:\kain2\game\FX.C
// address: 0x8004C4D8
// size: 0x4C
// line start: 6254
// line end:   6347
struct _FXParticle *FX_StartGenericParticle(struct _Instance *instance, int num, int segOverride, int lifeOverride, int InitFlag)
{ //line: 1
	// register: $s0
	// size: 0x4C
	register struct _FXParticle *currentParticle;
	// register: $v1
	// size: 0x1C
	register struct GenericFXObject *GFXO;
	// register: $s1
	// size: 0x30
	register struct _GenericParticleParams *GPP;
	// register: $s2
	// size: 0x4C
	register struct Object *texture_obj;
	// register: $a2
	// size: 0x4C
	register struct Object *particle;
	{ //line: 66
		// register: $v0
		// size: 0x10
		register struct TextureMT3 *texture;
	} //line: 70
	{ //line: 80
		// register: $a0
		register int tmp_blue;
		// register: $v1
		// size: 0x4
		register struct CVECTOR *ptr;
	} //line: 89
} //line: 94


// path: C:\kain2\game\FX.C
// address: 0x8004C828
// line start: 6350
// line end:   6385
void FX_StartGenericRibbon(struct _Instance *instance, int num, int segOverride, int endOverride, int InitFlag)
{ //line: 1
	// register: $v1
	// size: 0x4C
	register struct Object *particle;
	// register: $v1
	// size: 0x1C
	register struct GenericFXObject *GFXO;
	// register: $v1
	// size: 0x14
	register struct _GenericRibbonParams *GRP;
} //line: 36


// path: C:\kain2\game\FX.C
// address: 0x8004C8E4
// line start: 6388
// line end:   6462
void FX_StartGenericGlow(struct _Instance *instance, int num, int segOverride, int seg2Override, int InitFlag)
{ //line: 1
	// register: $v1
	// size: 0x4C
	register struct Object *particle;
	// register: $t1
	// size: 0x1C
	register struct GenericFXObject *GFXO;
	// register: $s0
	// size: 0x18
	register struct _GenericGlowParams *GGP;
	// register: $a0
	// size: 0x2C
	register struct _FXGlowEffect *glowEffect;
	// register: $v1
	register long *color;
	{ //line: 28
		// register: $a1
		register int seg;
	} //line: 30
	{ //line: 34
		// register: $a1
		register int seg;
		// register: $a2
		register int segEnd;
	} //line: 45
	{ //line: 49
		// register: $a1
		register int seg;
		// register: $a2
		register int numSeg;
	} //line: 68
} //line: 75


// path: C:\kain2\game\FX.C
// address: 0x8004CA8C
// size: 0x3C
// line start: 6483
// line end:   6519
struct _FXLightning *FX_CreateLightning(struct _Instance *instance, int lifeTime, int deg, int deg_inc, int width, int small_width, int segs, int sine_size, int variation, unsigned long color, unsigned long glow_color)
{ //line: 1
	// register: $s0
	// size: 0x3C
	register struct _FXLightning *zap;
} //line: 36


// path: C:\kain2\game\FX.C
// address: 0x8004CB94
// line start: 6540
// line end:   6565
void FX_SetLightingPos(struct _FXLightning *zap, struct _Instance *startInstance, int startSeg, struct _Position *startOffset, struct _Instance *endInstance, int endSeg, struct _Position *endOffset, int matrixSeg);

// path: C:\kain2\game\FX.C
// address: 0x8004CC44
// size: 0x3C
// line start: 6569
// line end:   6634
struct _FXLightning *FX_StartGenericLightning(struct _Instance *instance, int num, int segOverride, int endSegOverride)
{ //line: 1
	// register: $s1
	// size: 0x3C
	register struct _FXLightning *zap;
	// register: $v1
	// size: 0x1C
	register struct GenericFXObject *GFXO;
	// register: $s0
	// size: 0x28
	register struct _GenericLightningParams *GLP;
	// register: $v1
	// size: 0x4C
	register struct Object *particle;
	{ //line: 27
		// register: $a2
		register int startSeg;
		// register: $a3
		register int endSeg;
		{ //line: 53
			// register: $a0
			register int tmp_blue;
			// register: $v1
			// size: 0x4
			register struct CVECTOR *ptr;
		} //line: 62
	} //line: 63
} //line: 66


// path: C:\kain2\game\FX.C
// address: 0x8004CDE8
// size: 0x78
// line start: 6638
// line end:   6701
struct _FXBlastringEffect *FX_StartGenericBlastring(struct _Instance *instance, int num, int segOverride, int matrixSegOverride)
{ //line: 1
	// register: $v0
	// size: 0x78
	register struct _FXBlastringEffect *blast;
	// register: $v1
	// size: 0x1C
	register struct GenericFXObject *GFXO;
	// register: $s0
	// size: 0x30
	register struct _GenericBlastringParams *GBP;
	// register: $v1
	// size: 0x4C
	register struct Object *particle;
	// address: -64
	// size: 0x8
	auto struct _SVector position;
	// register: $s2
	register int segment;
	// register: $s3
	register int matrix_segment;
	// address: -56
	// size: 0x20
	auto struct MATRIX mat;
	// register: $v0
	// size: 0x20
	register struct MATRIX *swTransform;
} //line: 64


// path: C:\kain2\game\FX.C
// address: 0x8004CFF0
// size: 0x24
// line start: 6704
// line end:   6735
struct _FXFlash *FX_StartGenericFlash(struct _Instance *instance, int num)
{ //line: 1
	// register: $v1
	// size: 0x1C
	register struct GenericFXObject *GFXO;
	// register: $s1
	// size: 0xC
	register struct _GenericFlashParams *GFP;
	// register: $v1
	// size: 0x4C
	register struct Object *particle;
	// register: $s0
	// size: 0x24
	register struct _FXFlash *flash;
} //line: 32


// path: C:\kain2\game\FX.C
// address: 0x8004D0D0
// line start: 6738
// line end:   6776
long FX_GetHealthColor(int currentHealth)
{ //line: 21
	// size: 0x18
	static long HealthColors[6];
	// register: $v1
	register long color;
} //line: 38


// path: C:\kain2\game\FX.C
// address: 0x8004D124
// line start: 6780
// line end:   6781
void FX_Start_Snow(int percent);

// path: C:\kain2\game\FX.C
// address: 0x8004D158
// line start: 6785
// line end:   6787
void FX_Start_Rain(int percent);

// path: C:\kain2\game\FX.C
// address: 0x8004D194
// line start: 6792
// line end:   6816
void FX_StartLightbeam(struct _Instance *instance, int startSeg, int endSeg, int color_num)
{ //line: 1
	// register: $v0
	// size: 0x4C
	register struct Object *particle;
	// register: $v0
	// size: 0x18
	register struct _FXLightBeam *beam;
	// register: $v0
	// size: 0x1C
	register struct GenericFXObject *GFXO;
	// register: $s0
	register long color;
} //line: 25


// path: C:\kain2\game\FX.C
// address: 0x8004D238
// line start: 6831
// line end:   7035
void FX_StartInstanceEffect(struct _Instance *instance, struct ObjectEffect *effect, int InitFlag)
{ //line: 1
	// address: -32
	auto long color;
	{ //line: 34
		// register: $a2
		register long numberOfSegments;
	} //line: 54
	{ //line: 119
		// register: $a0
		// size: 0x4C
		register struct _FXParticle *currentParticle;
		// register: $s1
		// size: 0xA
		register struct evObjectDraftData *draft;
	} //line: 128
	{ //line: 155
		// register: $s0
		// size: 0x14
		register struct FXSplinter *splinterData;
		// register: $a0
		// size: 0x8
		register struct GenericTune *tune;
		// register: $s1
		register short shardFlags;
		{ //line: 161
			// register: $v0
			// size: 0x8
			register struct PhysObSplinter *splintDef;
		} //line: 164
	} //line: 175
	{ //line: 189
		// register: $a0
		register int shock;
		// register: $a1
		register int amt;
	} //line: 199
} //line: 205


// path: C:\kain2\game\FX.C
// address: 0x8004D658
// line start: 7040
// line end:   7053
void FX_EndInstanceEffects(struct _Instance *instance)
{ //line: 1
	// register: $a0
	// size: 0x10
	register struct _FXGeneralEffect *currentEffect;
	// register: $s0
	// size: 0x10
	register struct _FXGeneralEffect *nextEffect;
} //line: 14


// path: C:\kain2\game\FX.C
// address: 0x8004D6AC
// line start: 7057
// line end:   7070
void FX_EndInstanceParticleEffects(struct _Instance *instance)
{ //line: 1
	// register: $a0
	// size: 0x10
	register struct _FXGeneralEffect *currentEffect;
	// register: $s0
	// size: 0x10
	register struct _FXGeneralEffect *nextEffect;
} //line: 14


// path: C:\kain2\game\FX.C
// address: 0x8004D71C
// line start: 7081
// line end:   7111
void FX_GetSpiralPoint(int radius, int deg, int *x, int *y)
{ //line: 1
	// register: $a0
	register int prevx;
	// register: $v0
	register int prevy;
} //line: 15


// path: C:\kain2\game\FX.C
// address: 0x8004D7F0
// line start: 7125
// line end:   7133
void FX_GetLinePoint(int radius, int next_radius, int deg, int next_deg, int *pntx, int *pnty, int part)
{ //line: 1
	// address: -32
	auto int x1;
	// address: -28
	auto int y1;
	// address: -24
	auto int x2;
	// address: -20
	auto int y2;
} //line: 8


// path: C:\kain2\game\FX.C
// address: 0x8004D8BC
// line start: 7137
// line end:   7202
void FX_CalcSpiral(int degchange)
{ //line: 1
	// register: $s2
	register int radius;
	// register: $s1
	register int deg;
	// register: $s3
	register int n;
	// address: -72
	auto int pntx;
	// address: -68
	auto int pnty;
	// address: -64
	auto int px;
	// address: -60
	auto int py;
	// address: -56
	auto int mx;
	// address: -52
	auto int my;
	// register: $s0
	register int mod;
	// register: $s4
	register int next_deg;
	// register: $s5
	register int next_radius;
	// register: $s7
	register int minx;
	// register: $s6
	register int maxx;
	// address: -48
	auto int miny;
	// register: $fp
	register int maxy;
} //line: 63


// path: C:\kain2\game\FX.C
// address: 0x8004DBB4
// line start: 7220
// line end:   7474
void FX_Spiral(struct _PrimPool *primPool, unsigned long **ot)
{ //line: 1
	// register: $s2
	// size: 0x44
	register struct _POLY_2G4 *poly;
	// register: $s3
	register long prev;
	// register: $s7
	register long offp;
	// register: $s4
	register long offm;
	// register: $s1
	register int n;
	// register: $v0
	// size: 0x8
	register struct DR_TPAGE *drtpage;
	// register: $fp
	register int health;
	// register: $t1
	register int health_mod;
	// address: -76
	auto long no_color;
	// address: -80
	auto long color;
	// address: 108
	static short cnt;
	// address: -72
	auto int current_cnt;
	// register: $a0
	register int max64;
	// register: $t3
	register long SPIRAL_COLOR;
	// register: $t2
	register long SPIRAL_COLOR2;
	// address: -68
	auto long SPIRAL_COLOR3;
	// register: $s5
	register long SPIRAL_COLOR_END;
	// register: $a2
	register long SPIRAL_NOCOLOR;
	{ //line: 137
		// register: $v1
		register int tmp;
	} //line: 151
	{ //line: 234
		// address: 110
		static short deg;
		// address: -96
		// size: 0xC
		auto struct _Vector f1;
	} //line: 249
} //line: 255


// path: C:\kain2\game\FX.C
// address: 0x8004DFD4
// line start: 7477
// line end:   7524
void FX_Health_Spiral(int number, int current_health, int max_health)
{ //line: 1
	// register: $v1
	register int degchange;
} //line: 48


// path: C:\kain2\game\FX.C
// address: 0x8004E120
// line start: 7527
// line end:   7531
void FX_Spiral_Init();

// path: C:\kain2\game\FX.C
// address: 0x8004E160
// line start: 7556
// line end:   7634
void FX_DrawModel(struct Object *object, int model_num, struct _SVector *rotation, struct _SVector *position, struct _SVector *offset, int transflag)
{ //line: 1
	// register: $v0
	// size: 0x38
	register struct _Model *model;
	// register: $t0
	// size: 0xC
	register struct _MFace *mface;
	// register: $t3
	// size: 0x8
	register struct _MVertex *vertexList;
	// register: $a1
	// size: 0x10
	register struct TextureMT3 *texture;
	// address: -88
	// size: 0x20
	auto struct MATRIX matrix;
	// register: $a3
	register int i;
	// register: $s4
	// size: 0x28
	register struct POLY_GT3 *poly;
	// register: $s5
	register unsigned long **drawot;
	// address: -56
	// size: 0x8
	auto struct _SVector output;
	// register: $t1
	register long color;
	{ //line: 44
		// address: -48
		auto long clip;
	} //line: 72
} //line: 79


// path: C:\kain2\game\FX.C
// address: 0x8004E3D8
// line start: 7649
// line end:   7664
void fx_calc_points(struct _SVector *points, int degrees, int radius, int radius2, int radius3)
{ //line: 1
	// register: $s0
	register int cosval;
	// register: $v0
	register int sinval;
} //line: 1


// path: C:\kain2\game\FX.C
// address: 0x8004E4E8
// line start: 7667
// line end:   7676
long fx_get_startz(struct _SVector *position)
{ //line: 1
	// address: -40
	// size: 0x20
	auto struct MATRIX tmpmat;
} //line: 9


// path: C:\kain2\game\FX.C
// address: 0x8004E540
// line start: 7701
// line end:   7777
void FX_DrawRing(struct MATRIX *wcTransform, struct _SVector *position, struct MATRIX *matrix, int radius, int radius2, int radius3, int z1, int z2, int z3, long color, int sortInWorld)
{ //line: 1
	// register: $s3
	register int n;
	// register: $s1
	// size: 0x48
	register struct _POLY_2G4T *poly;
	// address: -44
	auto unsigned long **drawot;
	// address: -96
	// size: 0x18
	auto struct _SVector points[3];
	// address: -72
	// size: 0xC
	auto long sxy[3];
	// register: $s6
	register long degrees;
	// register: $s4
	register long color_black;
	// address: -56
	auto long sz0;
	// address: -52
	auto long sz1;
	// address: -48
	auto long sz2;
	// register: $s7
	register long startz;
} //line: 77


// path: C:\kain2\game\FX.C
// address: 0x8004E7EC
// line start: 7781
// line end:   7786
void fx_setTex(struct DVECTOR *x, unsigned char *uv, int tx, int offset);

// path: C:\kain2\game\FX.C
// address: 0x8004E830
// line start: 7843
// line end:   7934
void FX_DrawRing2(struct MATRIX *wcTransform, struct _SVector *position, struct MATRIX *matrix, int radius, int radius2, int radius3, int z1, int z2, int z3, long offset, int sortInWorld)
{ //line: 1
	// register: $s3
	register int n;
	// register: $s2
	// size: 0x28
	register struct POLY_FT4 *poly;
	// address: -60
	auto unsigned long **drawot;
	// address: -112
	// size: 0x18
	auto struct _SVector points[3];
	// address: -88
	// size: 0xC
	auto long sxy[3];
	// register: $fp
	register long degrees;
	// address: -72
	auto long sz0;
	// address: -68
	auto long sz1;
	// address: -64
	auto long sz2;
	// address: -56
	auto long startz;
	// register: $s0
	register short tx;
	// register: $s0
	register long dispYPos;
	// register: $s4
	register int num_faces;
	// address: -52
	auto int deg_change;
} //line: 92


// path: C:\kain2\game\FX.C
// address: 0x8004EC08
// line start: 7960
// line end:   8014
void FX_DrawFField(struct MATRIX *wcTransform, struct _FXForceFieldEffect *field)
{ //line: 1
	// register: $a0
	// size: 0x29C
	register struct _Instance *instance;
	// address: -64
	// size: 0x20
	auto struct MATRIX tmpmat;
	// address: -32
	// size: 0x8
	auto struct _SVector position;
	// register: $s1
	register int size;
	// register: $v0
	register short fade;
	// address: -20
	auto long color;
	// address: -24
	auto long black;
} //line: 55


// path: C:\kain2\game\FX.C
// address: 0x8004EDFC
// size: 0x2C
// line start: 8017
// line end:   8041
struct _FXForceFieldEffect *FX_StartFField(struct _Instance *instance, int size, struct _Position *offset, int size_diff, int size_change, int deg_change, long color)
{ //line: 1
	// register: $s0
	// size: 0x2C
	register struct _FXForceFieldEffect *field;
	{ //line: 6
		// register: $v0
		register short _x1;
		// register: $v1
		register short _y1;
		// register: $a1
		register short _z1;
		// register: $v0
		// size: 0x8
		register struct _SVector *_v0;
	} //line: 6
} //line: 24


// path: C:\kain2\game\FX.C
// address: 0x8004EED0
// line start: 8044
// line end:   8064
void FX_EndFField(struct _Instance *instance)
{ //line: 2
	// register: $v1
	// size: 0x10
	register struct _FXGeneralEffect *currentEffect;
	{ //line: 6
	} //line: 18
} //line: 21


// path: C:\kain2\game\FX.C
// address: 0x8004EF50
// line start: 8067
// line end:   8111
void FX_Draw_Glowing_Line(unsigned long **ot, long otz, struct DVECTOR *sxy0, struct DVECTOR *sxy1, struct DVECTOR *xy0, struct DVECTOR *xy1, long color, long color2)
{ //line: 1
	// register: $t0
	// size: 0x48
	register struct _POLY_2G4T *poly;
	// register: $t2
	// size: 0x1770C
	register struct _PrimPool *primPool;
	// register: $t5
	register int negflag;
} //line: 45


// path: C:\kain2\game\FX.C
// address: 0x8004F0EC
// line start: 8115
// line end:   8222
void FX_Lightning(struct MATRIX *wcTransform, unsigned long **ot, struct MATRIX *mat, short deg, struct _SVector *start, struct _SVector *end, int width, int small_width, int segs, int sine_size, int variation, long color, long glow_color)
{ //line: 1
	// register: $s5
	register int sz0;
	// address: -56
	auto int sz1;
	// register: $s1
	register long otz;
	// register: $v1
	register int length;
	// address: -112
	// size: 0x4
	auto struct DVECTOR sxy0;
	// address: -104
	// size: 0x4
	auto struct DVECTOR sxy1;
	// address: -96
	// size: 0x4
	auto struct DVECTOR xy0;
	// address: -88
	// size: 0x4
	auto struct DVECTOR xy1;
	// address: -80
	// size: 0x4
	auto struct DVECTOR small_xy0;
	// address: -72
	// size: 0x4
	auto struct DVECTOR small_xy1;
	// register: $s3
	register int n;
	// address: -52
	auto int increment;
	// register: $s2
	register int rsin_nd2;
	{ //line: 35
		// address: -64
		// size: 0x8
		auto struct SVECTOR point;
		{ //line: 43
			// register: $s0
			register int rsin_n4;
		} //line: 47
		{ //line: 51
			// register: $s1
			register int tmpdeg;
		} //line: 55
		{ //line: 69
			// register: $s2
			register int fx;
			// register: $s0
			register int fy;
		} //line: 77
	} //line: 99
} //line: 108


// path: C:\kain2\game\FX.C
// address: 0x8004F5DC
// line start: 8228
// line end:   8383
void FX_LightHouse(struct MATRIX *wcTransform, unsigned long **ot, struct _Instance *instance, int startSeg, int endSeg, int segs, long beam_color)
{ //line: 1
	// register: $s4
	register int sz0;
	// address: -72
	auto int sz1;
	// register: $a1
	register long otz;
	// register: $s0
	register int width;
	// address: -136
	// size: 0x8
	auto struct _SVector start;
	// address: -128
	// size: 0x8
	auto struct _SVector end;
	// address: -120
	// size: 0x4
	auto struct DVECTOR sxy0;
	// address: -112
	// size: 0x4
	auto struct DVECTOR sxy1;
	// address: -104
	// size: 0x4
	auto struct DVECTOR xy0;
	// address: -96
	// size: 0x4
	auto struct DVECTOR xy1;
	// register: $a3
	register int fade;
	// register: $s2
	register int n;
	// register: $s6
	register int increment;
	// address: -64
	auto long black;
	// address: -68
	auto long color;
	// address: -56
	auto long color1;
	// address: -60
	auto long color2;
	// address: -52
	auto int fx;
	// address: -48
	auto int fy;
	// register: $s7
	register int length;
	// register: $s3
	register int segcnt;
	// address: -44
	auto int end_width;
	// register: $fp
	register int flag;
	// register: $v1
	// size: 0x20
	register struct MATRIX *swTransform;
	{ //line: 60
		// address: -88
		// size: 0x8
		auto struct SVECTOR point;
		{ //line: 71
			// register: $v1
			register int newlength;
			// register: $s1
			register int newfx;
			// register: $s0
			register int newfy;
		} //line: 85
	} //line: 92
	{ //line: 103
		// address: -80
		// size: 0x8
		auto struct SVECTOR point;
		{ //line: 125
			// register: $v0
			register int x;
		} //line: 137
	} //line: 146
} //line: 156


// path: C:\kain2\game\FX.C
// address: 0x8004FA58
// line start: 8387
// line end:   8402
void FX_StartPassthruFX(struct _Instance *instance, struct _SVector *normal, struct _SVector *point)
{ //line: 1
	// address: -24
	auto long color;
} //line: 1


// path: C:\kain2\game\FX.C
// address: 0x8004FB34
// line start: 8405
// line end:   8406
void FX_EndPassthruFX(struct _Instance *instance);

// path: C:\kain2\game\GENERIC.C
// address: 0x8003E7A0
// line start: 28
// line end:   71
void GenericInit(struct _Instance *instance, struct GameTracker *gameTracker)
{ //line: 1
	// register: $v1
	// size: 0x8
	register struct Spline *spline;
	// register: $s1
	// size: 0x4C
	register struct Object *object;
	{ //line: 28
		{ //line: 38
			// size: 0xC
			static struct _G2AnimInterpInfo_Type crap;
		} //line: 40
	} //line: 41
} //line: 44


// path: C:\kain2\game\GENERIC.C
// address: 0x8003E8DC
// line start: 74
// line end:   128
void GenericCollide(struct _Instance *instance, struct GameTracker *gameTracker);

// path: C:\kain2\game\GENERIC.C
// address: 0x8003E8E4
// line start: 134
// line end:   143
void GenericProcess(struct _Instance *instance, struct GameTracker *gameTracker)
{ //line: 1
	// register: $v1
	// size: 0x4C
	register struct Object *object;
} //line: 10


// path: C:\kain2\game\GENERIC.C
// address: 0x8003E938
// line start: 149
// line end:   216
unsigned long GenericQuery(struct _Instance *instance, unsigned long query)
{ //line: 1
	// register: $a0
	register long ret;
	{ //line: 32
		// register: $v0
		// size: 0x8
		register struct evControlSaveDataData *pdata;
	} //line: 42
	{ //line: 46
		// register: $a0
		// size: 0x4C
		register struct Object *object;
	} //line: 54
} //line: 67


// path: C:\kain2\game\GENERIC.C
// address: 0x8003EAB0
// line start: 222
// line end:   274
void GenericMessage(struct _Instance *instance, unsigned long message, unsigned long data)
{ //line: 1
	{ //line: 6
		// register: $s1
		// size: 0x14
		register struct evAnimationInstanceSwitchData *Ptr;
	} //line: 17
	{ //line: 23
	} //line: 24
	{ //line: 38
	} //line: 46
} //line: 53


// path: C:\kain2\game\GENERIC.C
// address: 0x8003EC2C
// line start: 277
// line end:   284
void GenericRelocateTune(struct Object *object, long offset)
{ //line: 2
	// register: $v1
	// size: 0x8
	register struct GenericTune *tune;
} //line: 8


// path: C:\kain2\game\SOUND.C
// address: 0x8003EC58
// size: 0x24
// line start: 40
// line end:   55
struct SoundEffectChannel *SndOpenSfxChannel(unsigned char *channelNum)
{ //line: 2
	// register: $a1
	register int i;
} //line: 16


// path: C:\kain2\game\SOUND.C
// address: 0x8003ECA0
// line start: 59
// line end:   61
void SndCloseSfxChannel(int channelNum);

// path: C:\kain2\game\SOUND.C
// address: 0x8003ECC8
// size: 0x24
// line start: 66
// line end:   70
struct SoundEffectChannel *SndGetSfxChannel(int channelNum);

// path: C:\kain2\game\SOUND.C
// address: 0x8003ECF0
// line start: 97
// line end:   140
void SOUND_ProcessInstanceSounds(unsigned char *sfxFileData, struct SoundInstance *soundInstTbl, struct _Position *position, int livesInOnePlace, int inSpectral, int hidden, int burning, long *triggerFlags)
{ //line: 1
	// register: $s3
	register int numSounds;
	// register: $v0
	register int numSfxIDs;
	// register: $s2
	register int i;
} //line: 44


// path: C:\kain2\game\SOUND.C
// address: 0x8003EE78
// line start: 149
// line end:   210
void SOUND_EndInstanceSounds(unsigned char *sfxFileData, struct SoundInstance *soundInstTbl)
{ //line: 1
	// register: $s3
	register int numSounds;
	// register: $v0
	register int numSfxIDs;
	// register: $s2
	register int i;
	// register: $v0
	// size: 0x24
	register struct SoundEffectChannel *channel;
} //line: 62


// path: C:\kain2\game\SOUND.C
// address: 0x8003EFE4
// line start: 218
// line end:   258
int isOkayToPlaySound(int flags, int spectralPlane, int hidden, int burning)
{ //line: 1
	{ //line: 6
		// register: $v0
		register int mask;
		// register: $v1
		register int tod;
	} //line: 27
} //line: 41


// path: C:\kain2\game\SOUND.C
// address: 0x8003F0F8
// line start: 266
// line end:   274
void setPeriodicSoundStateOff(struct SoundInstance *soundInst, struct ObjectPeriodicSound *sound);

// path: C:\kain2\game\SOUND.C
// address: 0x8003F184
// line start: 280
// line end:   442
void processPeriodicSound(struct _Position *position, int livesInOnePlane, int inSpectral, int hidden, int burning, struct SoundInstance *soundInst, struct ObjectPeriodicSound *sound)
{ //line: 1
	// register: $s0
	// size: 0x24
	register struct SoundEffectChannel *channel;
	// register: $v1
	register int sfxIDNum;
	// register: $a1
	register int sfxToneID;
	// register: $s2
	register int spectralPlane;
} //line: 163


// path: C:\kain2\game\SOUND.C
// address: 0x8003F624
// line start: 449
// line end:   563
void processEventSound(struct _Position *position, struct SoundInstance *soundInst, struct ObjectEventSound *sound)
{ //line: 1
	// register: $s0
	// size: 0x24
	register struct SoundEffectChannel *channel;
	// register: $v1
	register int sfxIDNum;
	// register: $a1
	register int sfxToneID;
	// register: $s4
	register int spectralPlane;
} //line: 115


// path: C:\kain2\game\SOUND.C
// address: 0x8003F9C8
// line start: 566
// line end:   570
void SOUND_StartInstanceSound(struct SoundInstance *soundInst);

// path: C:\kain2\game\SOUND.C
// address: 0x8003F9E8
// line start: 573
// line end:   586
void SOUND_StopInstanceSound(struct SoundInstance *soundInst)
{ //line: 1
	// register: $v0
	// size: 0x24
	register struct SoundEffectChannel *channel;
} //line: 12


// path: C:\kain2\game\SOUND.C
// address: 0x8003FA44
// line start: 594
// line end:   651
int SOUND_IsInstanceSoundLoaded(unsigned char *sfxFileData, long soundNumber)
{ //line: 1
	// register: $s1
	register unsigned short *sfxIDList;
	// register: $a3
	register int numSounds;
	// register: $s0
	register int numSfxIDs;
	// register: $a2
	register int i;
	// register: $v0
	register int status;
} //line: 58


// path: C:\kain2\game\SOUND.C
// address: 0x8003FB40
// line start: 665
// line end:   686
void SOUND_SetInstanceSoundPitch(struct SoundInstance *soundInst, long pitchChangeAmt, long time)
{ //line: 1
	// register: $a0
	// size: 0x24
	register struct SoundEffectChannel *channel;
} //line: 22


// path: C:\kain2\game\SOUND.C
// address: 0x8003FC00
// line start: 694
// line end:   719
void SOUND_SetInstanceSoundVolume(struct SoundInstance *soundInst, long volumeChangeAmt, long time)
{ //line: 1
	// register: $a0
	// size: 0x24
	register struct SoundEffectChannel *channel;
} //line: 26


// path: C:\kain2\game\SOUND.C
// address: 0x8003FD04
// line start: 741
// line end:   847
void processOneShotSound(struct _Position *position, int hidden, int burning, long *triggerFlags, struct SoundInstance *soundInst, struct ObjectOneShotSound *sound)
{ //line: 1
	// register: $s0
	// size: 0x24
	register struct SoundEffectChannel *channel;
	// register: $v1
	register int sfxIDNum;
	// register: $a1
	register int sfxToneID;
	// register: $s3
	register long triggerMask;
	// register: $a1
	register int spectralPlane;
} //line: 107


// path: C:\kain2\game\SOUND.C
// address: 0x8004004C
// line start: 863
// line end:   955
unsigned long SOUND_Play3dSound(struct _Position *position, int sfxToneID, int pitch, int maxVolume, int minVolDist)
{ //line: 1
	// register: $s1
	register long dx;
	// register: $s0
	register long dy;
	// register: $a0
	register long dz;
	// register: $s3
	register long objDist;
	// register: $s2
	register long workMinVolDist;
	// register: $v0
	register int angle;
	// register: $a0
	register int quadrant;
	// register: $v1
	register int qpos;
	// register: $a2
	register int pan;
	// register: $a1
	register int volume;
} //line: 93


// path: C:\kain2\game\SOUND.C
// address: 0x80040288
// line start: 962
// line end:   1053
unsigned long SOUND_Update3dSound(struct _Position *position, unsigned long handle, int pitch, int maxVolume, int minVolDist)
{ //line: 1
	// register: $s1
	register long dx;
	// register: $s0
	register long dy;
	// register: $a0
	register long dz;
	// register: $s3
	register long objDist;
	// register: $s2
	register long workMinVolDist;
	// register: $v0
	register int angle;
	// register: $a0
	register int quadrant;
	// register: $v1
	register int qpos;
	// register: $a2
	register int pan;
	// register: $a1
	register int volume;
} //line: 92


// path: C:\kain2\game\SOUND.C
// address: 0x800404BC
// line start: 1087
// line end:   1088
void SOUND_HandleGlobalValueSignal(int name, long data);

// path: C:\kain2\game\SOUND.C
// address: 0x800404C4
// line start: 1100
// line end:   1148
void SOUND_Init()
{ //line: 1
	// address: -32
	// size: 0x18
	auto struct AadInitAttr initAttr;
} //line: 1


// path: C:\kain2\game\SOUND.C
// address: 0x80040578
// line start: 1195
// line end:   1197
void SOUND_Free();

// path: C:\kain2\game\SOUND.C
// address: 0x8004059C
// line start: 1228
// line end:   1229
void SOUND_SetMusicVariable(int variable, int value);

// path: C:\kain2\game\SOUND.C
// address: 0x800405BC
// line start: 1238
// line end:   1241
void SOUND_SetMusicVolume(int newVolume);

// path: C:\kain2\game\SOUND.C
// address: 0x800405F0
// line start: 1245
// line end:   1248
void SOUND_SetSfxVolume(int newVolume);

// path: C:\kain2\game\SOUND.C
// address: 0x80040624
// line start: 1252
// line end:   1254
void SOUND_SetVoiceVolume(int newVolume);

// path: C:\kain2\game\SOUND.C
// address: 0x80040644
// line start: 1268
// line end:   1273
void SOUND_PauseAllSound();

// path: C:\kain2\game\SOUND.C
// address: 0x80040674
// line start: 1277
// line end:   1282
void SOUND_ResumeAllSound();

// path: C:\kain2\game\SOUND.C
// address: 0x800406A4
// line start: 1286
// line end:   1293
void SOUND_StopAllSound();

// path: C:\kain2\game\SOUND.C
// address: 0x800406E4
// line start: 1297
// line end:   1302
void SOUND_ResetAllSound();

// path: C:\kain2\game\SOUND.C
// address: 0x80040714
// line start: 1306
// line end:   1321
void SOUND_MusicOff()
{ //line: 1
	// register: $s0
	register int slotNumber;
} //line: 16


// path: C:\kain2\game\SOUND.C
// address: 0x80040778
// line start: 1324
// line end:   1329
void SOUND_MusicOn()
{ //line: 1
	// register: $s0
	register int slotNumber;
} //line: 6


// path: C:\kain2\game\SOUND.C
// address: 0x800407B0
// line start: 1333
// line end:   1335
void SOUND_SfxOff();

// path: C:\kain2\game\SOUND.C
// address: 0x800407D0
// line start: 1339
// line end:   1341
void SOUND_SfxOn();

// path: C:\kain2\game\SOUND.C
// address: 0x800407D8
// line start: 1361
// line end:   1362
int SndIsPlaying(unsigned long handle);

// path: C:\kain2\game\SOUND.C
// address: 0x800407F8
// line start: 1366
// line end:   1367
int SndIsPlayingOrRequested(unsigned long handle);

// path: C:\kain2\game\SOUND.C
// address: 0x80040818
// line start: 1376
// line end:   1377
int SndTypeIsPlayingOrRequested(unsigned int sfxToneID);

// path: C:\kain2\game\SOUND.C
// address: 0x80040838
// line start: 1387
// line end:   1391
unsigned long SndPlay(unsigned int sample);

// path: C:\kain2\game\SOUND.C
// address: 0x80040870
// line start: 1395
// line end:   1396
void SndEndLoop(unsigned long handle);

// path: C:\kain2\game\SOUND.C
// address: 0x80040890
// line start: 1400
// line end:   1410
unsigned long SndPlayVolPan(unsigned int sample, unsigned short vol, unsigned short pan, short pitch)
{ //line: 1
} //line: 11


// path: C:\kain2\game\SOUND.C
// address: 0x800408CC
// line start: 1421
// line end:   1425
unsigned long SndUpdateVolPanPitch(unsigned long handle, unsigned short vol, unsigned short pan, short pitch);

// path: C:\kain2\game\SOUND.C
// address: 0x80040914
// line start: 1454
// line end:   1456
void musicLoadReturnFunc(int dynamicBankIndex, int errorStatus);

// path: C:\kain2\game\SOUND.C
// address: 0x80040928
// line start: 1461
// line end:   1462
void musicFadeoutReturnFunc();

// path: C:\kain2\game\SOUND.C
// address: 0x8004093C
// line start: 1467
// line end:   1482
void musicEndCallbackFunc(long userData, int slot, int loopFlag);

// path: C:\kain2\game\SOUND.C
// address: 0x8004099C
// line start: 1487
// line end:   1497
void SOUND_PutMusicCommand(int cmdType, int cmdData)
{ //line: 2
	// register: $v0
	// size: 0x8
	register struct MusicLoadCmd *cmd;
} //line: 11


// path: C:\kain2\game\SOUND.C
// address: 0x800409E4
// line start: 1501
// line end:   1507
void SOUND_MusicInit();

// path: C:\kain2\game\SOUND.C
// address: 0x80040A10
// line start: 1512
// line end:   1513
int SOUND_IsMusicLoading();

// path: C:\kain2\game\SOUND.C
// address: 0x80040A40
// line start: 1533
// line end:   1723
void SOUND_ProcessMusicLoad()
{ //line: 1
	// address: -88
	// size: 0x8
	auto char musicName[8];
	// address: -80
	// size: 0x20
	auto char sndFileName[32];
	// address: -48
	// size: 0x20
	auto char smpFileName[32];
	{ //line: 13
		// register: $v1
		// size: 0x8
		register struct MusicLoadCmd *cmd;
	} //line: 25
	{ //line: 29
		// register: $v1
		// size: 0x188
		register struct Level *level;
	} //line: 97
} //line: 191


// path: C:\kain2\game\SOUND.C
// address: 0x80040F3C
// line start: 1730
// line end:   1758
void SOUND_UpdateSound();

// path: C:\kain2\game\SOUND.C
// address: 0x80040FE8
// line start: 1763
// line end:   1769
void SOUND_PlaneShift(int newPlane);

// path: C:\kain2\game\SOUND.C
// address: 0x80041024
// line start: 1793
// line end:   1821
void SOUND_ShutdownMusic()
{ //line: 1
} //line: 21


// path: C:\kain2\game\SOUND.C
// address: 0x800410D8
// line start: 1826
// line end:   1879
void SOUND_SetMusicModifier(long modifier);

// path: C:\kain2\game\SOUND.C
// address: 0x80041198
// line start: 1883
// line end:   1925
void SOUND_ResetMusicModifier(long modifier);

// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80055E58
// line start: 23
// line end:   52
void aadSubstituteVariables(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 2
	// register: $a2
	register unsigned char trackFlags;
} //line: 30


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80055F10
// line start: 58
// line end:   64
void metaCmdSelectChannel(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 2
	// register: $v1
	register int channelNumber;
} //line: 7


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80055F30
// line start: 68
// line end:   84
void metaCmdSelectSlot(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 2
	// register: $v1
	register int slotNumber;
} //line: 17


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80055F80
// line start: 88
// line end:   106
void metaCmdAssignSequence(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 1
	// register: $s1
	register int sequenceNumber;
	{ //line: 1
		// register: $s0
		register int bank;
	} //line: 13
} //line: 19


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80055FF4
// line start: 110
// line end:   113
void metaCmdUsePrimaryTempo(struct AadSeqEvent *event, struct _AadSequenceSlot *slot);

// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80055FFC
// line start: 117
// line end:   120
void metaCmdUseSecondaryTempo(struct AadSeqEvent *event, struct _AadSequenceSlot *slot);

// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056004
// line start: 124
// line end:   133
void metaCmdSetTempo(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 1
	// address: -16
	// size: 0x8
	auto struct AadTempo tempo;
} //line: 1


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056070
// line start: 137
// line end:   146
void metaCmdChangeTempo(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 1
	// address: -16
	// size: 0x8
	auto struct AadTempo tempo;
	// register: $a2
	// size: 0x5D0
	register struct _AadSequenceSlot *selectedSlot;
} //line: 1


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x800560E0
// line start: 152
// line end:   178
void metaCmdSetTempoFromSequence(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 1
	// register: $s1
	register int sequenceNumber;
	// address: -24
	// size: 0x8
	auto struct AadTempo tempo;
	{ //line: 1
		// register: $s0
		register int bank;
	} //line: 18
} //line: 27


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056160
// line start: 182
// line end:   183
void metaCmdStartSlot(struct AadSeqEvent *event, struct _AadSequenceSlot *slot);

// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056184
// line start: 189
// line end:   190
void metaCmdStopSlot(struct AadSeqEvent *event, struct _AadSequenceSlot *slot);

// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x800561A8
// line start: 196
// line end:   197
void metaCmdPauseSlot(struct AadSeqEvent *event, struct _AadSequenceSlot *slot);

// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x800561CC
// line start: 203
// line end:   204
void metaCmdResumeSlot(struct AadSeqEvent *event, struct _AadSequenceSlot *slot);

// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x800561F0
// line start: 210
// line end:   219
void metaCmdSetSlotBendRange(struct AadSeqEvent *event, struct _AadSequenceSlot *slot);

// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x800561F8
// line start: 223
// line end:   225
void metaCmdSetChannelBendRange(struct AadSeqEvent *event, struct _AadSequenceSlot *slot);

// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056200
// line start: 229
// line end:   237
void metaCmdSetSlotVolume(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 1
	// register: $v1
	register int volume;
} //line: 1


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056234
// line start: 241
// line end:   249
void metaCmdSetSlotPan(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 1
	// register: $v1
	register int pan;
} //line: 1


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056268
// line start: 253
// line end:   261
void metaCmdSetChannelVolume(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 1
	// register: $a0
	register int volume;
} //line: 1


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x800562A4
// line start: 265
// line end:   273
void metaCmdSetChannelPan(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 1
	// register: $a0
	register int pan;
} //line: 1


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x800562E0
// line start: 277
// line end:   283
void metaCmdEnableSustainUpdate(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 2
	// register: $a0
	register int channel;
} //line: 5


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056300
// line start: 287
// line end:   293
void metaCmdDisableSustainUpdate(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 2
	// register: $v0
	register int channel;
} //line: 5


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056324
// line start: 297
// line end:   303
void metaCmdMuteChannel(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 1
} //line: 1


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056350
// line start: 307
// line end:   313
void metaCmdUnMuteChannel(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 1
} //line: 1


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x8005637C
// line start: 317
// line end:   323
void metaCmdMuteChannelList(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 1
} //line: 1


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x800563AC
// line start: 327
// line end:   333
void metaCmdUnMuteChannelList(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 1
} //line: 1


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x800563DC
// line start: 337
// line end:   344
void metaCmdSetChannelMute(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 1
	// register: $s0
	register unsigned long muteChannelMask;
} //line: 1


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056430
// line start: 349
// line end:   356
void metaCmdDelayMute(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 2
	// register: $v1
	register unsigned long channelMask;
} //line: 4


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056454
// line start: 362
// line end:   375
void metaCmdUpdateMute(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 1
	// register: $s0
	register unsigned long channelMask;
	// register: $a1
	register unsigned long mask;
} //line: 14


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x800564E8
// line start: 379
// line end:   389
void metaCmdChannelVolumeFade(struct AadSeqEvent *event, struct _AadSequenceSlot *slot);

// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x800564F0
// line start: 393
// line end:   403
void metaCmdChannelPanFade(struct AadSeqEvent *event, struct _AadSequenceSlot *slot);

// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x800564F8
// line start: 407
// line end:   416
void metaCmdSlotVolumeFade(struct AadSeqEvent *event, struct _AadSequenceSlot *slot);

// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056500
// line start: 420
// line end:   429
void metaCmdSlotPanFade(struct AadSeqEvent *event, struct _AadSequenceSlot *slot);

// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056508
// line start: 433
// line end:   439
void metaCmdSetChannelProgram(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 2
	// register: $a0
	register int program;
} //line: 5


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056520
// line start: 443
// line end:   463
void metaCmdSetChannelBasePriority(struct AadSeqEvent *event, struct _AadSequenceSlot *slot);

// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056528
// line start: 467
// line end:   474
void metaCmdSetChannelTranspose(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 2
	// register: $a2
	register int channel;
	// register: $v1
	register int transpose;
} //line: 4


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056540
// line start: 478
// line end:   484
void metaCmdIgnoreChannelTranspose(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 2
	// register: $a0
	register int channel;
} //line: 5


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056560
// line start: 488
// line end:   494
void metaCmdRespectChannelTranspose(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 2
	// register: $v0
	register int channel;
} //line: 5


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056584
// line start: 498
// line end:   509
void metaCmdSetChannelPitchMap(struct AadSeqEvent *event, struct _AadSequenceSlot *slot);

// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x8005658C
// line start: 513
// line end:   521
void metaCmdIgnoreChannelPitchMap(struct AadSeqEvent *event, struct _AadSequenceSlot *slot);

// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056594
// line start: 525
// line end:   533
void metaCmdRespectChannelPitchMap(struct AadSeqEvent *event, struct _AadSequenceSlot *slot);

// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x8005659C
// line start: 537
// line end:   545
void metaCmdGetSequenceAssigned(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 2
	// register: $a0
	register int variableNum;
} //line: 9


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x800565CC
// line start: 549
// line end:   566
void metaCmdGetTempo(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 2
	// register: $a2
	register int variableNum1;
	// register: $a3
	register int variableNum2;
	// register: $t0
	register int variableNum3;
	// register: $a0
	register unsigned long quarterNoteTime;
} //line: 18


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056630
// line start: 570
// line end:   578
void metaCmdGetSlotStatus(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 2
	// register: $a0
	register int variableNum;
} //line: 9


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056660
// line start: 582
// line end:   594
void metaCmdGetChannelMute(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 2
	// register: $a2
	register int variableNum1;
	// register: $a0
	register int variableNum2;
} //line: 13


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x800566B0
// line start: 598
// line end:   607
void metaCmdGetChannelVolume(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 2
	// register: $a0
	register int variableNum;
	// register: $v1
	register int channel;
} //line: 10


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x800566E8
// line start: 611
// line end:   619
void metaCmdGetChannelPan(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 2
	// register: $a0
	register int variableNum;
	// register: $v1
	register int channel;
} //line: 9


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056720
// line start: 623
// line end:   633
void metaCmdGetChannelTranspose(struct AadSeqEvent *event, struct _AadSequenceSlot *slot);

// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056728
// line start: 637
// line end:   645
void metaCmdGetChannelProgram(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 2
	// register: $a0
	register int variableNum;
	// register: $v1
	register int channel;
} //line: 9


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056760
// line start: 649
// line end:   666
void metaCmdGetChannelBasePriority(struct AadSeqEvent *event, struct _AadSequenceSlot *slot);

// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056768
// line start: 670
// line end:   679
void metaCmdGetChannelBendRange(struct AadSeqEvent *event, struct _AadSequenceSlot *slot);

// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056770
// line start: 683
// line end:   691
void metaCmdGetSlotVolume(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 2
	// register: $a0
	register int variableNum;
} //line: 9


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x800567A0
// line start: 695
// line end:   702
void metaCmdGetSlotPan(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 2
	// register: $a0
	register int variableNum;
} //line: 8


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x800567D0
// line start: 706
// line end:   714
void metaCmdSetVariable(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 2
	// register: $a0
	register int value;
	// register: $v1
	register int destVariable;
} //line: 9


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x800567FC
// line start: 718
// line end:   727
void metaCmdCopyVariable(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 2
	// register: $v1
	register int srcVariable;
	// register: $a0
	register int destVariable;
} //line: 10


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056838
// line start: 731
// line end:   739
void metaCmdAddVariable(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 2
	// register: $a0
	register int value;
	// register: $a1
	register int destVariable;
} //line: 9


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056870
// line start: 743
// line end:   751
void metaCmdSubtractVariable(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 2
	// register: $a0
	register int value;
	// register: $a1
	register int destVariable;
} //line: 9


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x800568A8
// line start: 755
// line end:   763
void metaCmdSetVariableBits(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 2
	// register: $a0
	register int value;
	// register: $a1
	register int destVariable;
} //line: 9


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x800568E0
// line start: 767
// line end:   775
void metaCmdClearVariableBits(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 2
	// register: $a0
	register int value;
	// register: $v1
	register int destVariable;
} //line: 9


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056918
// line start: 779
// line end:   793
void aadGotoSequencePosition(struct _AadSequenceSlot *slot, int track, unsigned char *newPosition);

// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056984
// line start: 800
// line end:   837
void aadGotoSequenceLabel(struct _AadSequenceSlot *slot, int track, int labelNumber)
{ //line: 1
	// register: $v0
	// size: 0x10
	register struct AadSequenceHdr *seqHdr;
	// register: $a0
	register unsigned long trackOffset;
	// register: $v0
	register int bank;
} //line: 35


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056A3C
// line start: 842
// line end:   859
void metaCmdLoopStart(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 2
	// register: $a2
	register int nestLevel;
	// register: $a3
	register int track;
} //line: 17


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056AAC
// line start: 863
// line end:   887
void metaCmdLoopEnd(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 1
	// register: $v1
	register int prevNestLevel;
	// register: $a1
	register int track;
} //line: 25


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056B3C
// line start: 891
// line end:   894
void metaCmdLoopBreak(struct AadSeqEvent *event, struct _AadSequenceSlot *slot);

// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056B44
// line start: 898
// line end:   905
void metaCmdDefineLabel(struct AadSeqEvent *event, struct _AadSequenceSlot *slot);

// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056B4C
// line start: 909
// line end:   915
void metaCmdGotoLabel(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 1
} //line: 1


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056B7C
// line start: 919
// line end:   925
void metaCmdSetSequencePosition(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 1
} //line: 1


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056BAC
// line start: 929
// line end:   939
void metaCmdBranchIfVarEqual(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 1
	// register: $v1
	register int variableNum;
	// register: $v1
	register int value;
	// register: $a2
	register int labelNum;
} //line: 11


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056BF8
// line start: 943
// line end:   953
void metaCmdBranchIfVarNotEqual(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 1
	// register: $v1
	register int variableNum;
	// register: $v1
	register int value;
	// register: $a2
	register int labelNum;
} //line: 11


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056C44
// line start: 957
// line end:   967
void metaCmdBranchIfVarLess(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 1
	// register: $v1
	register int variableNum;
	// register: $v1
	register int value;
	// register: $a2
	register int labelNum;
} //line: 11


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056C94
// line start: 971
// line end:   981
void metaCmdBranchIfVarGreater(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 1
	// register: $v1
	register int variableNum;
	// register: $v1
	register int value;
	// register: $a2
	register int labelNum;
} //line: 11


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056CE4
// line start: 985
// line end:   995
void metaCmdBranchIfVarLessOrEqual(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 1
	// register: $v1
	register int variableNum;
	// register: $v1
	register int value;
	// register: $a2
	register int labelNum;
} //line: 11


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056D34
// line start: 999
// line end:   1009
void metaCmdBranchIfVarGreaterOrEqual(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 1
	// register: $v1
	register int variableNum;
	// register: $v1
	register int value;
	// register: $a2
	register int labelNum;
} //line: 11


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056D84
// line start: 1013
// line end:   1022
void metaCmdBranchIfVarBitsSet(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 1
	// register: $v1
	register int variableNum;
	// register: $v1
	register int mask;
	// register: $a2
	register int labelNum;
} //line: 10


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056DD4
// line start: 1026
// line end:   1035
void metaCmdBranchIfVarBitsClear(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 1
	// register: $v1
	register int variableNum;
	// register: $v1
	register int mask;
	// register: $a2
	register int labelNum;
} //line: 10


// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056E24
// line start: 1039
// line end:   1040
void metaCmdSubstituteVariableParam1(struct AadSeqEvent *event, struct _AadSequenceSlot *slot);

// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056E44
// line start: 1046
// line end:   1047
void metaCmdSubstituteVariableParam2(struct AadSeqEvent *event, struct _AadSequenceSlot *slot);

// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056E64
// line start: 1053
// line end:   1054
void metaCmdSubstituteVariableParam3(struct AadSeqEvent *event, struct _AadSequenceSlot *slot);

// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056E84
// line start: 1059
// line end:   1066
void metaCmdEndSequence(struct AadSeqEvent *event, struct _AadSequenceSlot *slot);

// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056EE0
// line start: 1177
// line end:   1178
void metaCmdPlaySoundEffect(struct AadSeqEvent *event, struct _AadSequenceSlot *slot);

// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056EE8
// line start: 1180
// line end:   1181
void metaCmdStopSoundEffect(struct AadSeqEvent *event, struct _AadSequenceSlot *slot);

// path: C:\kain2\game\PSX\AADSQCMD.C
// address: 0x80056EF0
// line start: 1183
// line end:   1184
void metaCmdSetSoundEffectVolumePan(struct AadSeqEvent *event, struct _AadSequenceSlot *slot);

// path: C:\kain2\game\PSX\AADSFX.C
// address: 0x80056EF8
// line start: 19
// line end:   32
unsigned long aadPlaySfx(unsigned int toneID, int volume, int pan, int pitchOffset)
{ //line: 1
	// register: $s0
	register unsigned long handle;
} //line: 1


// path: C:\kain2\game\PSX\AADSFX.C
// address: 0x80056F5C
// line start: 37
// line end:   39
unsigned long aadStopSfx(unsigned long handle);

// path: C:\kain2\game\PSX\AADSFX.C
// address: 0x80056F98
// line start: 45
// line end:   46
void aadStopAllSfx();

// path: C:\kain2\game\PSX\AADSFX.C
// address: 0x80056FC8
// line start: 53
// line end:   68
int aadIsSfxPlaying(unsigned long handle)
{ //line: 2
	// register: $v1
	// size: 0x1C
	register struct AadSynthVoice *voice;
	// register: $a1
	register int i;
} //line: 16


// path: C:\kain2\game\PSX\AADSFX.C
// address: 0x80057038
// line start: 72
// line end:   88
int aadIsSfxPlayingOrRequested(unsigned long handle)
{ //line: 1
	// register: $a1
	register int commandOut;
	// register: $a2
	register int i;
	// register: $v1
	// size: 0xC
	register struct AadSfxCommand *sfxCmd;
} //line: 17


// path: C:\kain2\game\PSX\AADSFX.C
// address: 0x800570F0
// line start: 93
// line end:   107
int aadIsSfxTypePlaying(unsigned int toneID)
{ //line: 2
	// register: $v1
	// size: 0x1C
	register struct AadSynthVoice *voice;
	// register: $a1
	register int i;
} //line: 15


// path: C:\kain2\game\PSX\AADSFX.C
// address: 0x80057164
// line start: 111
// line end:   128
int aadIsSfxTypePlayingOrRequested(unsigned int sfxToneID)
{ //line: 1
	// register: $a1
	register int commandOut;
	// register: $a2
	register int i;
	// register: $v1
	// size: 0xC
	register struct AadSfxCommand *sfxCmd;
} //line: 18


// path: C:\kain2\game\PSX\AADSFX.C
// address: 0x8005721C
// line start: 160
// line end:   162
unsigned long aadSetSfxVolPanPitch(unsigned long handle, int volume, int pan, int pitch);

// path: C:\kain2\game\PSX\AADSFX.C
// address: 0x80057260
// line start: 168
// line end:   171
unsigned long createSfxHandle(unsigned int toneID);

// path: C:\kain2\game\PSX\AADSFX.C
// address: 0x800572A8
// line start: 177
// line end:   208
void aadPutSfxCommand(int statusByte, int dataByte0, int dataByte1, unsigned long ulongParam, int shortParam)
{ //line: 1
	// register: $v1
	// size: 0xC
	register struct AadSfxCommand *sfxCmd;
} //line: 32


// path: C:\kain2\game\PSX\AADSFX.C
// address: 0x8005739C
// line start: 229
// line end:   233
void aadExecuteSfxCommand(struct AadSfxCommand *sfxCmd);

// path: C:\kain2\game\PSX\AADSFX.C
// address: 0x800573E8
// line start: 248
// line end:   339
void sfxCmdPlayTone(struct AadSfxCommand *sfxCmd)
{ //line: 1
	// register: $s7
	register unsigned long handle;
	// register: $fp
	// size: 0x8
	register struct AadProgramAtr *progAtr;
	// register: $s2
	// size: 0x10
	register struct AadToneAtr *toneAtr;
	// register: $s1
	// size: 0x1C
	register struct AadSynthVoice *voice;
	// register: $s5
	register unsigned short midiNote;
	// register: $s6
	register unsigned long waveAddr;
	// register: $s3
	// size: 0x14
	register struct AadLoadedSfxToneAttr *sfxToneAttr;
	// register: $v0
	// size: 0x4
	register struct AadLoadedSfxWaveAttr *sfxWaveAttr;
	// register: $v1
	register int i;
} //line: 92


// path: C:\kain2\game\PSX\AADSFX.C
// address: 0x80057554
// line start: 344
// line end:   368
void sfxCmdStopTone(struct AadSfxCommand *sfxCmd)
{ //line: 2
	// register: $t0
	register unsigned long handle;
	// register: $a0
	// size: 0x1C
	register struct AadSynthVoice *voice;
	// register: $a3
	register unsigned short i;
	// register: $v1
	register unsigned long vmask;
} //line: 22


// path: C:\kain2\game\PSX\AADSFX.C
// address: 0x8005760C
// line start: 372
// line end:   395
void sfxCmdStopAllTones(struct AadSfxCommand *sfxCmd)
{ //line: 2
	// register: $a0
	// size: 0x1C
	register struct AadSynthVoice *voice;
	// register: $v1
	register unsigned long vmask;
	// register: $a3
	register unsigned short i;
} //line: 21


// path: C:\kain2\game\PSX\AADSFX.C
// address: 0x800576B0
// line start: 399
// line end:   435
void sfxCmdSetToneVolumeAndPan(struct AadSfxCommand *sfxCmd)
{ //line: 1
	// register: $a3
	register unsigned long handle;
	// register: $t0
	// size: 0x1C
	register struct AadSynthVoice *voice;
	// register: $t1
	register unsigned short i;
	// address: -16
	// size: 0x4
	auto struct AadVolume newVoiceVol;
	{ //line: 19
		// register: $v0
		register unsigned long tmp;
	} //line: 19
	{ //line: 20
		// register: $v1
		register unsigned long masterVolumeSquared;
	} //line: 20
	{ //line: 21
		// register: $v0
		register unsigned long tmp;
	} //line: 21
	{ //line: 22
		// register: $v1
		register unsigned long masterVolumeSquared;
	} //line: 22
	{ //line: 22
		// register: $v1
		register unsigned long masterVolumeSquared;
	} //line: 22
	{ //line: 22
		// register: $v0
		register unsigned long masterVolumeSquared;
	} //line: 22
	{ //line: 22
		// register: $v0
		register unsigned long masterVolumeSquared;
	} //line: 22
} //line: 37


// path: C:\kain2\game\PSX\AADSFX.C
// address: 0x80057A24
// line start: 439
// line end:   493
void sfxCmdSetToneVolPanPitch(struct AadSfxCommand *sfxCmd)
{ //line: 1
	// register: $a2
	register unsigned long handle;
	// register: $s0
	// size: 0x1C
	register struct AadSynthVoice *voice;
	// register: $s1
	register unsigned short i;
	// address: -24
	// size: 0x4
	auto struct AadVolume newVoiceVol;
	// register: $a1
	register unsigned short newPitch;
	// register: $a1
	register unsigned short finePitch;
	// register: $a0
	register unsigned short pitchIndex;
	{ //line: 20
		// register: $v0
		register unsigned long tmp;
	} //line: 20
	{ //line: 21
		// register: $v1
		register unsigned long masterVolumeSquared;
	} //line: 21
	{ //line: 22
		// register: $v0
		register unsigned long tmp;
	} //line: 22
	{ //line: 23
		// register: $v1
		register unsigned long masterVolumeSquared;
	} //line: 23
	{ //line: 23
		// register: $v1
		register unsigned long masterVolumeSquared;
	} //line: 23
	{ //line: 23
		// register: $v0
		register unsigned long masterVolumeSquared;
	} //line: 23
	{ //line: 23
		// register: $v0
		register unsigned long masterVolumeSquared;
	} //line: 23
} //line: 55


// path: C:\kain2\game\PSX\AADSFX.C
// address: 0x80057E90
// line start: 498
// line end:   512
void sfxCmdLockVoice(struct AadSfxCommand *sfxCmd)
{ //line: 1
	// register: $s0
	register void (*callbackProc)();
	// register: $v1
	// size: 0x1C
	register struct AadSynthVoice *voice;
} //line: 15


// path: C:\kain2\game\PSX\AADSFX.C
// address: 0x80057EDC
// line start: 516
// line end:   532
void sfxCmdSetVoiceAttr(struct AadSfxCommand *sfxCmd)
{ //line: 1
	// register: $s0
	register unsigned long v;
	// register: $v1
	register unsigned long vmask;
	// register: $s1
	// size: 0x40
	register struct SpuVoiceAttr *voiceAttr;
} //line: 12


// path: C:\kain2\game\PSX\AADSFX.C
// address: 0x80057F68
// line start: 535
// line end:   536
void sfxCmdSetVoiceKeyOn(struct AadSfxCommand *sfxCmd);

// path: C:\kain2\game\PSX\AADSFX.C
// address: 0x80057F84
// line start: 541
// line end:   543
void sfxCmdSetVoiceKeyOff(struct AadSfxCommand *sfxCmd);

// path: C:\kain2\game\MEMPACK.C
// address: 0x80050090
// line start: 194
// line end:   234
void MEMPACK_Init();

// path: C:\kain2\game\MEMPACK.C
// address: 0x800500F8
// size: 0x8
// line start: 250
// line end:   273
struct MemHeader *MEMPACK_GetSmallestBlockTopBottom(long allocSize)
{ //line: 2
	// register: $v1
	// size: 0x8
	register struct MemHeader *address;
	// register: $a1
	// size: 0x8
	register struct MemHeader *bestAddress;
} //line: 23


// path: C:\kain2\game\MEMPACK.C
// address: 0x80050160
// size: 0x8
// line start: 279
// line end:   309
struct MemHeader *MEMPACK_GetSmallestBlockBottomTop(long allocSize)
{ //line: 2
	// register: $v1
	// size: 0x8
	register struct MemHeader *address;
	// register: $a1
	// size: 0x8
	register struct MemHeader *bestAddress;
} //line: 30


// path: C:\kain2\game\MEMPACK.C
// address: 0x800501CC
// line start: 312
// line end:   326
long MEMPACK_RelocatableType(long memType);

// path: C:\kain2\game\MEMPACK.C
// address: 0x80050204
// line start: 333
// line end:   452
char *MEMPACK_Malloc(unsigned long allocSize, unsigned char memType)
{ //line: 1
	// register: $s1
	// size: 0x8
	register struct MemHeader *bestAddress;
	// register: $s3
	register long relocatableMemory;
	{ //line: 29
		// register: $s0
		register int curMem;
	} //line: 40
	{ //line: 67
		// register: $a1
		// size: 0x8
		register struct MemHeader *address;
		{ //line: 87
			// register: $a0
			register long topOffset;
		} //line: 89
	} //line: 89
} //line: 120


// path: C:\kain2\game\MEMPACK.C
// address: 0x800503CC
// line start: 457
// line end:   470
void MEMORY_MergeAddresses(struct MemHeader *firstAddress, struct MemHeader *secondAddress);

// path: C:\kain2\game\MEMPACK.C
// address: 0x80050414
// line start: 476
// line end:   503
void MEMPACK_Return(char *address, long takeBackSize)
{ //line: 1
	// register: $a0
	// size: 0x8
	register struct MemHeader *memAddress;
	// register: $a1
	// size: 0x8
	register struct MemHeader *nextAddress;
} //line: 28


// path: C:\kain2\game\MEMPACK.C
// address: 0x80050498
// line start: 535
// line end:   579
void MEMPACK_Free(char *address)
{ //line: 1
	// register: $s0
	// size: 0x8
	register struct MemHeader *memAddress;
	// register: $v1
	// size: 0x8
	register struct MemHeader *secondAddress;
} //line: 45


// path: C:\kain2\game\MEMPACK.C
// address: 0x80050548
// line start: 582
// line end:   603
void MEMPACK_FreeByType(unsigned char memType)
{ //line: 1
	// register: $a0
	// size: 0x8
	register struct MemHeader *address;
	// register: $s0
	register int freed;
} //line: 22


// path: C:\kain2\game\MEMPACK.C
// address: 0x800505E0
// line start: 607
// line end:   614
unsigned long MEMPACK_Size(char *address)
{ //line: 2
} //line: 7


// path: C:\kain2\game\MEMPACK.C
// address: 0x800505EC
// line start: 617
// line end:   618
unsigned long MEMPACK_ReportFreeMemory();

// path: C:\kain2\game\MEMPACK.C
// address: 0x800505FC
// line start: 685
// line end:   720
void MEMPACK_ReportMemory2()
{ //line: 2
	// register: $v1
	// size: 0x8
	register struct MemHeader *address;
} //line: 36


// path: C:\kain2\game\MEMPACK.C
// address: 0x8005062C
// line start: 723
// line end:   794
void MEMPACK_ReportMemory()
{ //line: 2
	// register: $v1
	// size: 0x8
	register struct MemHeader *address;
	// register: $a1
	register long i;
	// register: $a0
	register long firstTime;
} //line: 72


// path: C:\kain2\game\MEMPACK.C
// address: 0x800506C8
// line start: 797
// line end:   804
void MEMPACK_SetMemoryBeingStreamed(char *address)
{ //line: 2
} //line: 7


// path: C:\kain2\game\MEMPACK.C
// address: 0x800506D4
// line start: 807
// line end:   814
void MEMPACK_SetMemoryDoneStreamed(char *address)
{ //line: 2
} //line: 7


// path: C:\kain2\game\MEMPACK.C
// address: 0x800506E0
// line start: 822
// line end:   848
long MEMPACK_MemoryValidFunc(char *address)
{ //line: 2
} //line: 27


// path: C:\kain2\game\MEMPACK.C
// address: 0x80050714
// line start: 851
// line end:   921
char *MEMPACK_GarbageCollectMalloc(unsigned long *allocSize, unsigned char memType, unsigned long *freeSize)
{ //line: 1
	// register: $s0
	// size: 0x8
	register struct MemHeader *bestAddress;
} //line: 71


// path: C:\kain2\game\MEMPACK.C
// address: 0x8005088C
// line start: 924
// line end:   937
void MEMPACK_GarbageSplitMemoryNow(unsigned long allocSize, struct MemHeader *bestAddress, long memType, unsigned long freeSize)
{ //line: 2
	{ //line: 4
		// register: $v0
		// size: 0x8
		register struct MemHeader *address;
	} //line: 12
} //line: 14


// path: C:\kain2\game\MEMPACK.C
// address: 0x800508B0
// line start: 943
// line end:   979
void MEMPACK_GarbageCollectFree(struct MemHeader *memAddress)
{ //line: 1
	// register: $v1
	// size: 0x8
	register struct MemHeader *secondAddress;
} //line: 37


// path: C:\kain2\game\MEMPACK.C
// address: 0x80050960
// line start: 1001
// line end:   1134
void MEMPACK_DoGarbageCollection()
{ //line: 1
	// register: $s0
	// size: 0x8
	register struct MemHeader *relocateAddress;
	// register: $s1
	register long foundOpening;
	// register: $s5
	register long done;
	// register: $s3
	register long addressSize;
	// register: $s2
	register long addressMemType;
	// address: -40
	auto long holdSize;
	// address: -36
	auto long freeSize;
	// register: $s0
	register char *oldAddress;
	// register: $s1
	register char *newAddress;
} //line: 132


// path: C:\kain2\game\MEMPACK.C
// address: 0x80050B78
// line start: 1165
// line end:   1430
void MEMPACK_RelocateAreaType(struct MemHeader *newAddress, long offset, struct Level *oldLevel)
{ //line: 1
	// register: $s0
	// size: 0x188
	register struct Level *level;
	// register: $a0
	// size: 0x388
	register struct _MultiSignal *msignal;
	// register: $s2
	register long sizeOfLevel;
	// register: $t0
	register long i;
	// register: $a3
	register long d;
	{ //line: 66
		// register: $t1
		// size: 0x54
		register struct _Terrain *terrain;
		{ //line: 94
			// register: $v0
			// size: 0x4C
			register struct Intro *intro;
			{ //line: 104
				// register: $a2
				// size: 0x50
				register struct MultiSpline *multiSpline;
			} //line: 127
		} //line: 129
		{ //line: 147
			// register: $v0
			// size: 0x28
			register struct DrMoveAniTexDestInfo **dest;
		} //line: 157
		{ //line: 200
			// register: $a1
			// size: 0x3C
			register struct _VMObject *vmo;
		} //line: 211
		{ //line: 198
			// register: $t2
			// size: 0x24
			register struct BSPTree *bsp;
			// register: $a2
			// size: 0x2C
			register struct _BSPNode *node;
			// register: $a1
			// size: 0x30
			register struct _BSPLeaf *leaf;
		} //line: 238
	} //line: 240
} //line: 258


// path: C:\kain2\game\MEMPACK.C
// address: 0x8005145C
// line start: 1434
// line end:   1454
void MEMPACK_RelocateG2AnimKeylistType(struct _G2AnimKeylist_Type **pKeylist, int offset, char *start, char *end)
{ //line: 1
	{ //line: 4
		// register: $a2
		register int j;
		// register: $a3
		// size: 0x14
		register struct _G2AnimKeylist_Type *keylist;
	} //line: 20
} //line: 21


// path: C:\kain2\game\MEMPACK.C
// address: 0x800514FC
// line start: 1457
// line end:   1610
void MEMPACK_RelocateObjectType(struct MemHeader *newAddress, long offset, struct Object *oldObject)
{ //line: 1
	// register: $a1
	// size: 0x29C
	register struct _Instance *instance;
	// register: $s1
	// size: 0x4C
	register struct Object *object;
	// register: $s2
	register int i;
	// register: $a2
	register int j;
	// register: $a3
	register int d;
	// register: $s4
	register int sizeOfObject;
	// register: $t0
	// size: 0x38
	register struct _Model *model;
	{ //line: 47
		// register: $a0
		// size: 0xC
		register struct _MFace *mface;
	} //line: 52
	{ //line: 56
		// register: $v0
		// size: 0x18
		register struct _Segment *segment;
		{ //line: 60
			// register: $v1
			// size: 0x18
			register struct _HInfo *hInfo;
		} //line: 64
	} //line: 65
	{ //line: 67
		// register: $a0
		// size: 0xC
		register struct AniTexInfo *aniTexInfo;
	} //line: 76
	{ //line: 80
		// register: $v0
		// size: 0x50
		register struct MultiSpline *multiSpline;
	} //line: 102
} //line: 152


// path: C:\kain2\game\MEMPACK.C
// address: 0x80051A6C
// line start: 1619
// line end:   1626
void MEMPACK_RelocateCDMemory(struct MemHeader *newAddress, long offset, struct _BigFileDir *oldDir)
{ //line: 1
	// register: $a1
	// size: 0x4
	register struct _BigFileDir *newDir;
} //line: 1


// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80051A90
// line start: 40
// line end:   41
unsigned long aadGetMemorySize(struct AadInitAttr *attributes);

// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80051AB4
// line start: 46
// line end:   182
int aadInit(struct AadInitAttr *attributes, unsigned char *memoryPtr)
{ //line: 1
	// register: $v1
	// size: 0x5D0
	register struct _AadSequenceSlot *slot;
	// register: $v0
	register unsigned long size;
	// register: $a2
	register int slotNumber;
	// register: $a1
	register int i;
} //line: 137


// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80051D5C
// line start: 186
// line end:   193
void aadInstallUpdateFunc(long (*updateFuncPtr)(), int hblanksPerUpdate);

// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80051DD8
// line start: 199
// line end:   204
void aadInitVolume();

// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80051E0C
// line start: 210
// line end:   213
void aadSetMasterVolume(int volume);

// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80051E3C
// line start: 219
// line end:   223
void aadStartMasterVolumeFade(int targetVolume, int volumeStep, void (*fadeCompleteCallback)());

// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80051E54
// line start: 229
// line end:   231
void aadSetSfxMasterVolume(int volume);

// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80051E64
// line start: 237
// line end:   246
void aadSetMusicMasterVolume(int volume)
{ //line: 1
	// register: $s0
	register int slotNumber;
} //line: 10


// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80051EC8
// line start: 249
// line end:   253
void aadStartMusicMasterVolFade(int targetVolume, int volumeStep, void (*fadeCompleteCallback)());

// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80051EE0
// line start: 285
// line end:   304
void aadShutdown();

// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80051F68
// line start: 321
// line end:   335
long aadSlotUpdateWrapper()
{ //line: 1
	// register: $s0
	register unsigned long curGp;
} //line: 1


// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80051FAC
// line start: 338
// line end:   584
void aadSlotUpdate()
{ //line: 1
	// register: $s2
	// size: 0x5D0
	register struct _AadSequenceSlot *slot;
	// register: $a1
	// size: 0xC
	register struct AadSeqEvent *seqEventPtr;
	// register: $fp
	register int slotNumber;
	// register: $a3
	register int i;
	// register: $v1
	register int fadeComplete;
	// register: $s3
	register int track;
	// register: $s0
	register int newVol;
	// register: $a2
	register int slotDone;
	{ //line: 16
		// register: $a2
		register unsigned long vmask;
	} //line: 31
} //line: 247


// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x800524E4
// line start: 629
// line end:   630
unsigned long aadCreateFourCharID(char a, char b, char c, char d);

// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x8005250C
// line start: 637
// line end:   691
int aadLoadDynamicSoundBank(char *sndFileName, char *smpFileName, int dynamicBankIndex, int loadOption, void (*retProc)())
{ //line: 1
	// register: $a0
	register int i;
	// register: $s1
	// size: 0x58
	register struct AadDynamicBankLoadInfo *info;
} //line: 55


// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80052664
// line start: 697
// line end:   764
void aadLoadDynamicSoundBankReturn(void *loadedDataPtr, void *data, void *data2)
{ //line: 1
	// register: $s1
	register int dynamicBankIndex;
	// register: $s0
	register int error;
} //line: 68


// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80052734
// line start: 775
// line end:   874
void aadLoadDynamicSoundBankReturn2(void *loadedDataPtr, long loadedDataSize, short status, void *data1, void *data2)
{ //line: 1
	// register: $s1
	register unsigned char *dataPtr;
	// register: $s0
	// size: 0x58
	register struct AadDynamicBankLoadInfo *info;
	// register: $s3
	register int dynamicBankIndex;
	// register: $v0
	register int error;
	{ //line: 84
		// register: $a2
		register int i;
	} //line: 98
} //line: 100


// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80052960
// line start: 893
// line end:   917
int aadFreeDynamicSoundBank(int dynamicBankIndex);

// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x800529DC
// line start: 923
// line end:   971
int aadOpenDynamicSoundBank(unsigned char *soundBank, int dynamicBankIndex)
{ //line: 1
	// register: $s0
	// size: 0x28
	register struct AadSoundBankHdr *soundBankHdr;
	// register: $v0
	// size: 0x8
	register struct AadProgramAtr *programAtr;
	// register: $v0
	// size: 0x10
	register struct AadToneAtr *toneAtr;
	// register: $v0
	register unsigned long *waveAddr;
	// register: $a0
	register unsigned long *sequenceOffsetTbl;
	// register: $a1
	register unsigned long *sequenceLabelOffsetTbl;
	// register: $a3
	register unsigned char *sequenceBase;
	// register: $t2
	register int i;
} //line: 49


// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80052AFC
// line start: 989
// line end:   1012
int aadLoadDynamicSfx(char *fileName, long directoryID, long flags)
{ //line: 1
	// register: $s0
	// size: 0x1C
	register struct AadDynamicLoadRequest *loadReq;
} //line: 24


// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80052BB0
// line start: 1039
// line end:   1071
int aadFreeDynamicSfx(int handle)
{ //line: 2
	// register: $a1
	// size: 0x1C
	register struct AadDynamicLoadRequest *loadReq;
	// register: $a2
	register int i;
} //line: 33


// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80052C8C
// line start: 1116
// line end:   1118
void aadRelocateMusicMemoryBegin();

// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80052CA8
// line start: 1124
// line end:   1183
void aadRelocateMusicMemoryEnd(void *oldAddress, int offset)
{ //line: 1
	// register: $t6
	register int bank;
	{ //line: 9
		// register: $t4
		register int slotNumber;
		// register: $a2
		register int i;
		// register: $a0
		// size: 0x28
		register struct AadSoundBankHdr *bankHdr;
		{ //line: 28
			// register: $t2
			// size: 0x5D0
			register struct _AadSequenceSlot *slot;
			{ //line: 31
				// register: $t0
				register int track;
			} //line: 47
		} //line: 48
	} //line: 51
} //line: 54


// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80052E54
// line start: 1188
// line end:   1224
void aadRelocateSfxMemory(void *oldAddress, int offset)
{ //line: 2
	// register: $v1
	// size: 0x14
	register struct _AadDynSfxFileHdr *snfFile;
} //line: 37


// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80052EBC
// line start: 1228
// line end:   1230
int aadGetNumLoadsQueued();

// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80052ED0
// line start: 1236
// line end:   1238
void aadPurgeLoadQueue();

// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80052EE8
// line start: 1248
// line end:   1384
void aadProcessLoadQueue()
{ //line: 1
	// register: $s0
	// size: 0x94
	register struct AadDynamicSfxLoadInfo *info;
	// register: $a0
	register int i;
	// register: $v0
	register char *p;
	{ //line: 26
		// register: $s2
		// size: 0x1C
		register struct AadDynamicLoadRequest *loadReq;
		{ //line: 37
			// address: -32
			// size: 0xC
			auto char areaName[12];
			{ //line: 58
			} //line: 58
			{ //line: 63
			} //line: 65
		} //line: 84
		{ //line: 89
			// register: $s2
			register int i;
			// register: $s1
			// size: 0x14
			register struct _AadDynSfxFileHdr *snfFile;
			// register: $v1
			register unsigned short *sfxIDListPtr;
		} //line: 121
	} //line: 129
} //line: 137


// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80053228
// line start: 1388
// line end:   1404
void aadLoadDynamicSfxAbort(struct AadDynamicSfxLoadInfo *info, int error);

// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x800532AC
// line start: 1414
// line end:   1416
void aadLoadDynamicSfxDone(struct AadDynamicSfxLoadInfo *info);

// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x800532B4
// line start: 1429
// line end:   1516
void aadLoadDynamicSfxReturn(void *loadedDataPtr, void *data, void *data2)
{ //line: 1
	// register: $v1
	// size: 0x14
	register struct _AadDynSfxFileHdr *p;
	// register: $s0
	// size: 0x94
	register struct AadDynamicSfxLoadInfo *info;
} //line: 88


// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x8005342C
// line start: 1532
// line end:   1635
int aadWaveMalloc(unsigned short waveID, unsigned long waveSize)
{ //line: 1
	// register: $a2
	// size: 0x8
	register struct AadNewSramBlockDesc *sramDesc;
	// register: $t1
	// size: 0x8
	register struct AadNewSramBlockDesc *bestFit;
	// register: $a2
	// size: 0x8
	register struct AadNewSramBlockDesc *next;
	// register: $t2
	// size: 0x8
	register struct AadNewSramBlockDesc *sramDescTbl;
	// register: $t4
	register unsigned long safeWaveSize;
	// register: $a3
	register int i;
	// register: $t0
	register int sramDescIndex;
	// register: $t3
	register int bestFitIndex;
} //line: 104


// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80053674
// line start: 1640
// line end:   1651
unsigned long aadGetSramBlockAddr(int handle)
{ //line: 2
	// register: $v1
	// size: 0x8
	register struct AadNewSramBlockDesc *sramDesc;
} //line: 12


// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x800536A0
// line start: 1673
// line end:   1710
void aadWaveFree(int handle)
{ //line: 2
	// register: $a1
	// size: 0x8
	register struct AadNewSramBlockDesc *sramDesc;
	// register: $a3
	// size: 0x8
	register struct AadNewSramBlockDesc *sramDescTbl;
	{ //line: 13
		// register: $a2
		// size: 0x8
		register struct AadNewSramBlockDesc *next;
	} //line: 22
	{ //line: 26
		// register: $a0
		// size: 0x8
		register struct AadNewSramBlockDesc *prev;
	} //line: 35
} //line: 38


// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x800537B0
// line start: 1715
// line end:   1740
void aadFreeSingleDynSfx(int sfxID)
{ //line: 1
	// register: $v1
	register int ti;
	// register: $v1
	register int wi;
	// register: $a1
	// size: 0x14
	register struct AadLoadedSfxToneAttr *toneAttr;
	// register: $v1
	// size: 0x4
	register struct AadLoadedSfxWaveAttr *waveAttr;
} //line: 26


// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80053878
// line start: 1744
// line end:   1788
void setSramFullAlarm()
{ //line: 2
	// register: $v1
	// size: 0x8
	register struct AadNewSramBlockDesc *sramDescTbl;
	// register: $a1
	// size: 0x8
	register struct AadNewSramBlockDesc *sramDesc;
	// register: $t0
	register long totalUsed;
	// register: $t1
	register long totalFree;
	// register: $t2
	register long largestFree;
	// register: $t3
	register long numFreeBlocks;
	// register: $a3
	register long numUsedBlocks;
	// register: $a2
	register int i;
} //line: 39


// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80053954
// line start: 1800
// line end:   1913
void aadLoadSingleDynSfx(struct AadDynamicSfxLoadInfo *info)
{ //line: 1
	// register: $a0
	register int i;
	// register: $a1
	// size: 0x14
	register struct AadLoadedSfxToneAttr *toneAttr;
	// register: $s0
	// size: 0x4
	register struct AadLoadedSfxWaveAttr *waveAttr;
	// register: $s1
	// size: 0x18
	register struct AadDynSfxAttr *attr;
} //line: 114


// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80053C24
// line start: 1929
// line end:   1932
void HackCallback();

// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80053C5C
// line start: 1938
// line end:   2090
void aadLoadDynamicSfxReturn2(void *loadedDataPtr, long loadedDataSize, short status, void *data1, void *data2)
{ //line: 1
	// register: $s4
	register unsigned char *dataPtr;
	// register: $s3
	register unsigned long dataOffset;
	// register: $s2
	register unsigned long bytesRemaining;
	// register: $s1
	// size: 0x94
	register struct AadDynamicSfxLoadInfo *info;
	// register: $s0
	register unsigned long n;
} //line: 153


// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80053F24
// line start: 2143
// line end:   2187
int aadCheckSramFragmented()
{ //line: 2
	// register: $v1
	// size: 0x8
	register struct AadNewSramBlockDesc *sramDescTbl;
	// register: $a1
	// size: 0x8
	register struct AadNewSramBlockDesc *sramDesc;
	// register: $t0
	register long totalFree;
	// register: $t1
	register long smallestFree;
	// register: $a3
	register long numFreeBlocks;
	// register: $a2
	register int i;
	// register: $v1
	register int defragNeeded;
} //line: 44


// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80053FD8
// line start: 2206
// line end:   2398
void aadProcessSramDefrag()
{ //line: 1
	// register: $s3
	// size: 0x20
	register struct AadSramDefragInfo *info;
	// register: $s5
	// size: 0x8
	register struct AadNewSramBlockDesc *sramDescTbl;
	// register: $s1
	// size: 0x8
	register struct AadNewSramBlockDesc *firstBlock;
	// register: $s2
	// size: 0x8
	register struct AadNewSramBlockDesc *secondBlock;
	// register: $s0
	register int n;
	// register: $a0
	register int waveID;
	// register: $s4
	register int firstBlockIndex;
	// register: $s6
	register int secondBlockIndex;
	{ //line: 103
		// register: $a0
		// size: 0x8
		register struct AadNewSramBlockDesc *next;
	} //line: 115
} //line: 193


// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x8005433C
// line start: 2408
// line end:   2416
int aadIsSfxLoaded(unsigned int toneID);

// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80054378
// line start: 2681
// line end:   2746
void aadInitSequenceSlot(struct _AadSequenceSlot *slot)
{ //line: 1
	// register: $t0
	// size: 0x10
	register struct AadSequenceHdr *seqHdr;
	// register: $v0
	register unsigned long trackOffset;
	// register: $a1
	register int i;
	// register: $a0
	register int bank;
} //line: 59


// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80054484
// line start: 2750
// line end:   2757
int aadWaitForSramTransferComplete()
{ //line: 1
	// register: $s0
	register int n;
} //line: 8


// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x800544C4
// line start: 2761
// line end:   2771
void aadInitReverb();

// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80054548
// line start: 2777
// line end:   2785
void aadShutdownReverb();

// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80054580
// line start: 2791
// line end:   2792
int aadGetReverbMode();

// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80054588
// line start: 2813
// line end:   2814
unsigned long aadGetReverbSize();

// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x800545B8
// line start: 2818
// line end:   2819
int aadGetReverbDepth();

// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x800545C0
// line start: 2831
// line end:   2837
int aadGetNumDynamicSequences(int bank);

// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x800545F0
// line start: 2848
// line end:   2894
int aadAssignDynamicSequence(int bank, int sequenceNumber, int slotNumber)
{ //line: 1
	// address: -32
	// size: 0x8
	auto struct AadTempo tempo;
	// register: $s0
	// size: 0x5D0
	register struct _AadSequenceSlot *slot;
	// register: $v1
	register int i;
} //line: 47


// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x800546CC
// size: 0x8
// line start: 2898
// line end:   2913
struct AadTempo *aadGetTempoFromDynamicSequence(int bank, int sequenceNumber, struct AadTempo *tempo)
{ //line: 2
	// register: $v1
	// size: 0x10
	register struct AadSequenceHdr *seqHdr;
} //line: 15


// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x8005471C
// line start: 3032
// line end:   3049
void aadSetSlotTempo(int slotNumber, struct AadTempo *tempo)
{ //line: 2
	// register: $a0
	// size: 0x5D0
	register struct _AadSequenceSlot *slot;
	// register: $v1
	register unsigned long tickTime;
	// register: $a2
	register unsigned long tickTimeRemainder;
} //line: 8


// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x800547D0
// line start: 3053
// line end:   3066
void aadStartSlot(int slotNumber)
{ //line: 1
	// register: $s0
	// size: 0x5D0
	register struct _AadSequenceSlot *slot;
} //line: 14


// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x8005484C
// line start: 3070
// line end:   3084
void aadStopSlot(int slotNumber)
{ //line: 1
	// register: $a0
	// size: 0x5D0
	register struct _AadSequenceSlot *slot;
} //line: 15


// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x800548B8
// line start: 3088
// line end:   3102
void aadStopAllSlots()
{ //line: 1
	// register: $s1
	// size: 0x5D0
	register struct _AadSequenceSlot *slot;
	// register: $s0
	register int slotNumber;
} //line: 15


// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80054948
// line start: 3106
// line end:   3113
void aadDisableSlot(int slotNumber);

// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80054998
// line start: 3117
// line end:   3121
void aadEnableSlot(int slotNumber);

// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x800549D8
// line start: 3125
// line end:   3132
void aadPauseSlot(int slotNumber);

// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80054A28
// line start: 3136
// line end:   3151
void aadResumeSlot(int slotNumber)
{ //line: 2
	// register: $a1
	// size: 0x5D0
	register struct _AadSequenceSlot *slot;
	// register: $a0
	register int track;
} //line: 16


// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80054A98
// line start: 3155
// line end:   3156
int aadGetSlotStatus(int slotNumber);

// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80054AB8
// line start: 3181
// line end:   3205
void aadAllNotesOff(int slotNumber)
{ //line: 2
	// register: $a0
	// size: 0x1C
	register struct AadSynthVoice *voice;
	// register: $a1
	register unsigned long vmask;
	// register: $a3
	register int i;
	// register: $t0
	// size: 0x5D0
	register struct _AadSequenceSlot *slot;
} //line: 25


// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80054B58
// line start: 3223
// line end:   3259
void aadMuteChannels(struct _AadSequenceSlot *slot, unsigned long channelList)
{ //line: 2
	// register: $a2
	// size: 0x1C
	register struct AadSynthVoice *voice;
	// register: $t2
	register unsigned long vmask;
	// register: $a2
	register unsigned long delayedMute;
	// register: $t1
	register int channel;
	// register: $t0
	register int i;
} //line: 37


// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80054C34
// line start: 3263
// line end:   3274
void aadUnMuteChannels(struct _AadSequenceSlot *slot, unsigned long channelList)
{ //line: 2
	// register: $a2
	register unsigned long delayedUnMute;
} //line: 10


// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80054C6C
// line start: 3376
// line end:   3383
void (*aadInstallEndSequenceCallback(void (*callbackProc)(), long data))()
{ //line: 2
	// register: $v0
	register void (*previousCallbackProc)();
} //line: 4


// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80054C84
// line start: 3387
// line end:   3389
void aadSetUserVariable(int variableNumber, int value);

// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80054C98
// line start: 3417
// line end:   3424
void aadSetNoUpdateMode(int noUpdate);

// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80054CD8
// line start: 3438
// line end:   3454
void aadPauseSound()
{ //line: 1
	// register: $s0
	register int i;
} //line: 17


// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80054D70
// line start: 3457
// line end:   3459
void aadCancelPauseSound();

// path: C:\kain2\game\PSX\AADLIB.C
// address: 0x80054D8C
// line start: 3464
// line end:   3479
void aadResumeSound()
{ //line: 1
	// register: $s0
	register int i;
} //line: 16


// path: C:\kain2\game\PSX\AADSEQEV.C
// address: 0x80054E18
// line start: 24
// line end:   98
int aadQueueNextEvent(struct _AadSequenceSlot *slot, int track)
{ //line: 1
	// address: -16
	// size: 0xC
	auto struct AadSeqEvent seqEvent;
	// register: $a2
	register unsigned char *seqData;
	// register: $v1
	register unsigned long deltaTime;
	// register: $a0
	register int c;
	// register: $a3
	register int n;
	// register: $a0
	register int i;
} //line: 75


// path: C:\kain2\game\PSX\AADSEQEV.C
// address: 0x8005500C
// line start: 103
// line end:   122
void aadExecuteEvent(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 1
	// register: $v1
	register int eventType;
} //line: 20


// path: C:\kain2\game\PSX\AADSEQEV.C
// address: 0x800550A4
// line start: 127
// line end:   131
void midiNoteOff(struct AadSeqEvent *event, struct _AadSequenceSlot *slot);

// path: C:\kain2\game\PSX\AADSEQEV.C
// address: 0x800550AC
// line start: 139
// line end:   248
void midiNoteOn(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 1
	// register: $s4
	// size: 0x8
	register struct AadProgramAtr *progAtr;
	// register: $v1
	// size: 0x10
	register struct AadToneAtr *toneAtrTbl;
	// register: $s0
	// size: 0x1C
	register struct AadSynthVoice *voice;
	// register: $s7
	register int channel;
	// register: $s6
	register int midiNote;
	// address: -56
	auto int transposedNote;
	// register: $s2
	register int t;
	// address: -52
	auto int dynBank;
	{ //line: 81
		// register: $a1
		register unsigned long waveStartAddr;
	} //line: 106
} //line: 110


// path: C:\kain2\game\PSX\AADSEQEV.C
// address: 0x80055420
// line start: 253
// line end:   281
void aadUpdateChannelVolPan(struct _AadSequenceSlot *slot, int channel)
{ //line: 1
	// register: $s0
	// size: 0x1C
	register struct AadSynthVoice *voice;
	// register: $s2
	register int i;
	// address: -40
	// size: 0x4
	auto struct AadVolume newVoiceVol;
	{ //line: 14
		// register: $v0
		register unsigned long tmp;
	} //line: 14
	{ //line: 15
		// register: $v1
		register unsigned long masterVolumeSquared;
	} //line: 15
	{ //line: 16
		// register: $v0
		register unsigned long tmp;
	} //line: 16
	{ //line: 17
		// register: $v1
		register unsigned long masterVolumeSquared;
	} //line: 17
	{ //line: 17
		// register: $v0
		register unsigned long masterVolumeSquared;
	} //line: 17
	{ //line: 17
		// register: $v0
		register unsigned long masterVolumeSquared;
	} //line: 17
	{ //line: 17
		// register: $v0
		register unsigned long masterVolumeSquared;
	} //line: 17
} //line: 29


// path: C:\kain2\game\PSX\AADSEQEV.C
// address: 0x800557A4
// line start: 286
// line end:   313
void aadUpdateSlotVolPan(struct _AadSequenceSlot *slot)
{ //line: 1
	// register: $t0
	// size: 0x1C
	register struct AadSynthVoice *voice;
	// register: $t1
	register int channel;
	// register: $s0
	register int i;
	// address: -24
	// size: 0x4
	auto struct AadVolume newVoiceVol;
	{ //line: 16
		// register: $v0
		register unsigned long tmp;
	} //line: 16
	{ //line: 17
		// register: $v1
		register unsigned long masterVolumeSquared;
	} //line: 17
	{ //line: 18
		// register: $v0
		register unsigned long tmp;
	} //line: 18
	{ //line: 19
		// register: $v1
		register unsigned long masterVolumeSquared;
	} //line: 19
	{ //line: 19
		// register: $v0
		register unsigned long masterVolumeSquared;
	} //line: 19
	{ //line: 19
		// register: $v0
		register unsigned long masterVolumeSquared;
	} //line: 19
	{ //line: 19
		// register: $v0
		register unsigned long masterVolumeSquared;
	} //line: 19
} //line: 28


// path: C:\kain2\game\PSX\AADSEQEV.C
// address: 0x80055B00
// line start: 318
// line end:   365
void aadUpdateChannelPitchBend(struct _AadSequenceSlot *slot, int channel)
{ //line: 1
	// register: $a0
	// size: 0x1C
	register struct AadSynthVoice *voice;
	// register: $s0
	register int i;
	// register: $a1
	register int finePitch;
	// register: $a1
	register int newPitch;
	// register: $s2
	register int pitchWheelPos;
	// register: $a0
	register int pitchIndex;
	// register: $a2
	register long pitchValueBendAmount;
} //line: 48


// path: C:\kain2\game\PSX\AADSEQEV.C
// address: 0x80055CB0
// line start: 369
// line end:   370
void midiPolyphonicAftertouch(struct AadSeqEvent *event, struct _AadSequenceSlot *slot);

// path: C:\kain2\game\PSX\AADSEQEV.C
// address: 0x80055CB8
// line start: 374
// line end:   381
void midiControlChange(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 1
	// register: $v0
	register int controlNumber;
} //line: 1


// path: C:\kain2\game\PSX\AADSEQEV.C
// address: 0x80055CF4
// line start: 385
// line end:   392
void midiProgramChange(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 2
	// register: $v0
	register int channel;
} //line: 4


// path: C:\kain2\game\PSX\AADSEQEV.C
// address: 0x80055D0C
// line start: 396
// line end:   397
void midiChannelAftertouch(struct AadSeqEvent *event, struct _AadSequenceSlot *slot);

// path: C:\kain2\game\PSX\AADSEQEV.C
// address: 0x80055D14
// line start: 401
// line end:   409
void midiPitchWheelControl(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 1
	// register: $a1
	register int channel;
} //line: 1


// path: C:\kain2\game\PSX\AADSEQEV.C
// address: 0x80055D5C
// line start: 413
// line end:   416
void midiMetaEvent(struct AadSeqEvent *event, struct _AadSequenceSlot *slot);

// path: C:\kain2\game\PSX\AADSEQEV.C
// address: 0x80055D64
// line start: 421
// line end:   442
void midiControlBankSelect(struct AadSeqEvent *event, struct _AadSequenceSlot *slot);

// path: C:\kain2\game\PSX\AADSEQEV.C
// address: 0x80055D6C
// line start: 446
// line end:   457
void midiControlVolume(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 1
	// register: $a1
	register int channel;
} //line: 12


// path: C:\kain2\game\PSX\AADSEQEV.C
// address: 0x80055DBC
// line start: 461
// line end:   472
void midiControlPan(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 1
	// register: $a1
	register int channel;
} //line: 12


// path: C:\kain2\game\PSX\AADSEQEV.C
// address: 0x80055E0C
// line start: 476
// line end:   487
void midiControlCallback(struct AadSeqEvent *event, struct _AadSequenceSlot *slot)
{ //line: 1
} //line: 12


// path: C:\kain2\game\PSX\AADSEQEV.C
// address: 0x80055E50
// line start: 491
// line end:   492
void midiControlDummy(struct AadSeqEvent *event, struct _AadSequenceSlot *slot);

// path: C:\kain2\game\LIST.C
// address: 0x8004FB54
// line start: 46
// line end:   53
void LIST_InsertFunc(struct NodeType *list, struct NodeType *node);

// path: C:\kain2\game\LIST.C
// address: 0x8004FB80
// line start: 57
// line end:   72
void LIST_DeleteFunc(struct NodeType *node);

// path: C:\kain2\game\LIST.C
// address: 0x8004FBEC
// size: 0x8
// line start: 76
// line end:   85
struct NodeType *LIST_GetFunc(struct NodeType *list)
{ //line: 1
} //line: 10


// path: C:\kain2\game\VM.C
// address: 0x8004FC24
// line start: 105
// line end:   107
void VM_Tick(long time);

// path: C:\kain2\game\VM.C
// address: 0x8004FC44
// line start: 295
// line end:   407
void VM_UpdateMorph(struct Level *level, int initFlg)
{ //line: 1
	// register: $s3
	// size: 0x3C
	register struct _VMObject *vmobject;
	// register: $s4
	register int i;
	{ //line: 10
		// register: $a0
		// size: 0xC
		register struct _VMOffsetTable_duplicate_s3 *curTable;
		{ //line: 13
		} //line: 42
		{ //line: 46
			// register: $s2
			register short ratio;
			// register: $t1
			register int j;
			// register: $s1
			register long num;
			{ //line: 58
				// register: $a0
				register long len;
			} //line: 65
			{ //line: 67
				// register: $a1
				// size: 0x6
				register struct _VMOffset *material;
				// register: $a0
				// size: 0x6
				register struct _VMOffset *spectral;
				// register: $v1
				// size: 0x6
				register struct _VMOffset *offset;
			} //line: 79
			{ //line: 79
				// register: $a3
				register short dr;
				// register: $a0
				register short dg;
				// register: $v1
				register short db;
				// register: $a1
				// size: 0x3
				register struct _VMColorOffset *material;
				// register: $a0
				// size: 0x3
				register struct _VMColorOffset *spectral;
				// register: $v1
				// size: 0x3
				register struct _VMColorOffset *offset;
			} //line: 108
		} //line: 110
	} //line: 111
} //line: 113


// path: C:\kain2\game\VM.C
// address: 0x8005001C
// line start: 521
// line end:   535
void VM_VMObjectSetTable(struct Level *level, struct _VMObject *vmobject, int table)
{ //line: 1
	// register: $a0
	// size: 0xC
	register struct _VMOffsetTable_duplicate_s3 *curTable;
} //line: 9


// path: C:\kain2\game\PSX\AADVOICE.C
// address: 0x80057FB4
// size: 0x1C
// line start: 28
// line end:   89
struct AadSynthVoice *aadAllocateVoice(int priority)
{ //line: 2
	// register: $a2
	register int i;
	// register: $t2
	register int lowestPriSus;
	// register: $t1
	register int lowestPriRel;
	// register: $t4
	// size: 0x1C
	register struct AadSynthVoice *lowestPriSusVoice;
	// register: $t3
	// size: 0x1C
	register struct AadSynthVoice *lowestPriRelVoice;
	// register: $a1
	// size: 0x1C
	register struct AadSynthVoice *voice;
} //line: 62


// path: C:\kain2\game\PSX\AADVOICE.C
// address: 0x800580A8
// line start: 109
// line end:   133
void SpuSetVoiceADSR1ADSR2(int vNum, unsigned short adsr1, unsigned short adsr2)
{ //line: 1
	// register: $t3
	register unsigned short sl;
	// register: $v1
	register unsigned short dr;
	// register: $t4
	register unsigned short ar;
	// register: $t1
	register unsigned short arm;
	// register: $t2
	register unsigned short rr;
	// register: $t0
	register unsigned short rrm;
	// register: $a3
	register unsigned short sr;
	// register: $a1
	register unsigned short srm;
} //line: 24


// path: C:\kain2\game\PSX\AADVOICE.C
// address: 0x80058150
// line start: 156
// line end:   223
void aadPlayTone(struct AadToneAtr *toneAtr, unsigned long waveStartAddr, struct AadProgramAtr *progAtr, int midiNote, int volume, int masterVolume, int masterPan, int slotVolume, int masterMasterVol, struct AadSynthVoice *voice, int pitchOffset)
{ //line: 1
	// address: -32
	// size: 0x4
	auto struct AadVolume voiceVol;
	// register: $a1
	register int pitch;
	// register: $a1
	register int finePitch;
	// register: $a3
	register int pitchIndex;
	{ //line: 10
		// register: $v0
		register unsigned long tmp;
	} //line: 10
	{ //line: 11
		// register: $v1
		register unsigned long masterVolumeSquared;
	} //line: 11
	{ //line: 12
		// register: $v0
		register unsigned long tmp;
	} //line: 12
	{ //line: 14
		// register: $v1
		register unsigned long masterVolumeSquared;
	} //line: 14
	{ //line: 14
		// register: $a1
		register unsigned long masterVolumeSquared;
	} //line: 14
	{ //line: 14
		// register: $v1
		register unsigned long masterVolumeSquared;
	} //line: 14
	{ //line: 14
		// register: $v1
		register unsigned long masterVolumeSquared;
	} //line: 14
} //line: 49


// path: C:\kain2\game\PSX\AADVOICE.C
// address: 0x80058580
// line start: 246
// line end:   315
void aadPlayTonePitchBend(struct AadToneAtr *toneAtr, unsigned long waveStartAddr, struct AadProgramAtr *progAtr, int midiNote, int volume, int masterVolume, int masterPan, int slotVolume, int masterMasterVol, struct AadSynthVoice *voice, int pitchWheelPos)
{ //line: 1
	// address: -32
	// size: 0x4
	auto struct AadVolume voiceVol;
	// register: $a1
	register int pitch;
	// register: $a1
	register int finePitch;
	// register: $a3
	register int pitchIndex;
	// register: $t0
	register long pitchValueBendAmount;
	{ //line: 12
		// register: $v0
		register unsigned long tmp;
	} //line: 12
	{ //line: 13
		// register: $v1
		register unsigned long masterVolumeSquared;
	} //line: 13
	{ //line: 14
		// register: $v0
		register unsigned long tmp;
	} //line: 14
	{ //line: 16
		// register: $v1
		register unsigned long masterVolumeSquared;
	} //line: 16
	{ //line: 16
		// register: $a1
		register unsigned long masterVolumeSquared;
	} //line: 16
	{ //line: 16
		// register: $v1
		register unsigned long masterVolumeSquared;
	} //line: 16
	{ //line: 16
		// register: $v1
		register unsigned long masterVolumeSquared;
	} //line: 16
} //line: 67


// path: C:\kain2\game\STREAM.C
// address: 0x80058A00
// line start: 162
// line end:   186
void STREAM_FillOutFileNames(char *baseAreaName, char *dramName, char *vramName, char *sfxName)
{ //line: 1
	// address: -40
	// size: 0x10
	auto char text[16];
	// register: $v0
	register char *number;
} //line: 25


// path: C:\kain2\game\STREAM.C
// address: 0x80058AC0
// line start: 192
// line end:   198
void STREAM_AbortAreaLoad(char *baseAreaName)
{ //line: 1
	// address: -88
	// size: 0x50
	auto char vramName[80];
} //line: 1


// path: C:\kain2\game\STREAM.C
// address: 0x80058B08
// line start: 204
// line end:   213
void STREAM_Init()
{ //line: 2
	// register: $a0
	register int i;
} //line: 10


// path: C:\kain2\game\STREAM.C
// address: 0x80058B34
// line start: 216
// line end:   227
int FindObjectName(char *name)
{ //line: 1
	// register: $s0
	register int i;
	// register: $s1
	// size: 0x24
	register struct _ObjectTracker *otr;
} //line: 12


// path: C:\kain2\game\STREAM.C
// address: 0x80058BA0
// size: 0x24
// line start: 230
// line end:   239
struct _ObjectTracker *FindObjectInTracker(struct Object *object)
{ //line: 2
	// register: $a2
	register int i;
	// register: $a1
	// size: 0x24
	register struct _ObjectTracker *otr;
} //line: 10


// path: C:\kain2\game\STREAM.C
// address: 0x80058BF0
// size: 0x40
// line start: 258
// line end:   272
struct _StreamUnit *FindStreamUnitFromLevel(struct Level *level)
{ //line: 2
	// register: $a2
	// size: 0x40
	register struct _StreamUnit *ret;
	// register: $a1
	register long i;
} //line: 14


// path: C:\kain2\game\STREAM.C
// address: 0x80058C40
// line start: 328
// line end:   389
void STREAM_LoadObjectReturn(void *loadData, void *data, void *data2)
{ //line: 1
	// register: $s0
	// size: 0x4C
	register struct Object *object;
	// register: $s1
	// size: 0x24
	register struct _ObjectTracker *objectTracker;
	{ //line: 39
		// address: -80
		// size: 0x40
		auto char objDsfxFileName[64];
	} //line: 51
} //line: 62


// path: C:\kain2\game\STREAM.C
// address: 0x80058D30
// line start: 392
// line end:   429
void STREAM_DumpMonster(struct _ObjectTracker *dumpee)
{ //line: 1
	// register: $s1
	// size: 0x4C
	register struct Object *object;
	// register: $a1
	// size: 0x29C
	register struct _Instance *instance;
	{ //line: 21
		// register: $s0
		// size: 0x29C
		register struct _Instance *next;
	} //line: 28
} //line: 31


// path: C:\kain2\game\STREAM.C
// address: 0x80058DF8
// line start: 432
// line end:   440
int STREAM_InList(char *name, char **nameList)
{ //line: 1
	// register: $s0
	register char **mon;
} //line: 9


// path: C:\kain2\game\STREAM.C
// address: 0x80058E60
// line start: 443
// line end:   448
int STREAM_IsSpecialMonster(char *name)
{ //line: 1
	// size: 0x18
	static char *mon[6];
} //line: 1


// path: C:\kain2\game\STREAM.C
// address: 0x80058E84
// line start: 451
// line end:   466
void STREAM_DumpSomeMonsters()
{ //line: 1
	// register: $s1
	// size: 0x24
	register struct _ObjectTracker *otr;
	// register: $s2
	register int i;
} //line: 16


// path: C:\kain2\game\STREAM.C
// address: 0x80058F14
// line start: 468
// line end:   471
void STREAM_NoMonsters();

// path: C:\kain2\game\STREAM.C
// address: 0x80058F44
// line start: 474
// line end:   476
void STREAM_YesMonsters();

// path: C:\kain2\game\STREAM.C
// address: 0x80058F60
// line start: 481
// line end:   487
int STREAM_IsMonster(char *name)
{ //line: 1
	// address: 24
	// size: 0x4C
	static char *monnames[19];
} //line: 1


// path: C:\kain2\game\STREAM.C
// address: 0x80058F84
// line start: 524
// line end:   537
int STREAM_TryAndDumpANonResidentObject()
{ //line: 1
	// register: $s1
	// size: 0x24
	register struct _ObjectTracker *otr;
	// register: $s0
	register int i;
} //line: 14


// path: C:\kain2\game\STREAM.C
// address: 0x80058FD4
// line start: 540
// line end:   632
int InsertGlobalObject(char *name, struct GameTracker *gameTracker)
{ //line: 1
	// address: -152
	// size: 0x40
	auto char string[64];
	// address: -88
	// size: 0x40
	auto char vramname[64];
	// register: $s1
	register int i;
	// register: $s0
	// size: 0x24
	register struct _ObjectTracker *otr;
} //line: 92


// path: C:\kain2\game\STREAM.C
// address: 0x80059170
// size: 0x24
// line start: 643
// line end:   648
struct _ObjectTracker *STREAM_GetObjectTracker(char *name)
{ //line: 1
	// register: $v1
	register int i;
} //line: 6


// path: C:\kain2\game\STREAM.C
// address: 0x800591BC
// line start: 651
// line end:   688
void LoadLevelObjects(struct _StreamUnit *stream)
{ //line: 1
	// register: $s0
	register int objlist_pos;
	{ //line: 11
		// address: -48
		// size: 0x14
		auto char name[20];
	} //line: 13
	{ //line: 16
		// register: $s1
		// size: 0x188
		register struct Level *level;
		// register: $s2
		register int i;
	} //line: 37
} //line: 38


// path: C:\kain2\game\STREAM.C
// address: 0x800592E0
// line start: 691
// line end:   709
long STREAM_IsAnInstanceUsingObject(struct Object *object)
{ //line: 2
	// register: $v1
	// size: 0x29C
	register struct _Instance *instance;
	// register: $v1
	// size: 0x29C
	register struct _Instance *next;
	// register: $a1
	register long ret;
} //line: 18


// path: C:\kain2\game\STREAM.C
// address: 0x80059320
// line start: 712
// line end:   717
void STREAM_StreamLoadObjectAbort(void *loadData, void *data, void *data2)
{ //line: 1
	// register: $s0
	// size: 0x24
	register struct _ObjectTracker *objectTracker;
} //line: 5


// path: C:\kain2\game\STREAM.C
// address: 0x80059350
// line start: 720
// line end:   729
void STREAM_DumpLoadingObjects()
{ //line: 1
	// register: $s1
	register int i;
	// register: $s0
	// size: 0x24
	register struct _ObjectTracker *tracker;
} //line: 10


// path: C:\kain2\game\STREAM.C
// address: 0x800593B0
// line start: 735
// line end:   778
void STREAM_DumpObject(struct _ObjectTracker *objectTracker)
{ //line: 1
	// register: $s0
	// size: 0x4C
	register struct Object *object;
	{ //line: 7
		// address: -80
		// size: 0x40
		auto char dramName[64];
	} //line: 11
} //line: 44


// path: C:\kain2\game\STREAM.C
// address: 0x80059498
// line start: 782
// line end:   802
int STREAM_IsObjectInAnyUnit(struct _ObjectTracker *tracker)
{ //line: 1
	// register: $s1
	register int d;
	{ //line: 8
		// register: $s0
		register unsigned char *objlist;
	} //line: 18
} //line: 21


// path: C:\kain2\game\STREAM.C
// address: 0x80059558
// line start: 805
// line end:   901
void STREAM_RemoveAllObjectsNotInUse()
{ //line: 1
	// register: $s3
	register int i;
	// register: $a3
	register int abort;
	// register: $s1
	// size: 0x24
	register struct _ObjectTracker *tracker;
	// register: $s4
	// size: 0x24
	register struct _ObjectTracker *trackerList;
	{ //line: 10
		// register: $s2
		// size: 0x4C
		register struct Object *object;
	} //line: 25
	{ //line: 33
		// register: $a0
		register int j;
	} //line: 46
	{ //line: 54
		// register: $t0
		register int j;
		// register: $a3
		// size: 0x24
		register struct _ObjectTracker *otr;
		{ //line: 58
			// register: $a0
			register int k;
			{ //line: 67
				// register: $a1
				register int l;
			} //line: 74
		} //line: 76
	} //line: 79
} //line: 97


// path: C:\kain2\game\STREAM.C
// address: 0x80059818
// line start: 905
// line end:   915
void RemoveAllObjects(struct GameTracker *gameTracker)
{ //line: 1
	// register: $s1
	register int i;
	// register: $s0
	// size: 0x24
	register struct _ObjectTracker *tracker;
} //line: 11


// path: C:\kain2\game\STREAM.C
// address: 0x80059870
// size: 0x188
// line start: 921
// line end:   937
struct Level *STREAM_GetLevelWithID(long id)
{ //line: 2
	// register: $a2
	// size: 0x188
	register struct Level *retLevel;
	// register: $a1
	register long i;
} //line: 16


// path: C:\kain2\game\STREAM.C
// address: 0x800598C8
// size: 0x40
// line start: 940
// line end:   954
struct _StreamUnit *STREAM_GetStreamUnitWithID(long id)
{ //line: 2
	// register: $a2
	// size: 0x40
	register struct _StreamUnit *retUnit;
	// register: $a1
	register long i;
} //line: 14


// path: C:\kain2\game\STREAM.C
// address: 0x80059918
// line start: 957
// line end:   1010
void STREAM_CalculateWaterLevel(struct Level *level)
{ //line: 1
	// register: $t2
	// size: 0x54
	register struct _Terrain *terrain;
	// register: $t0
	register int i;
	// register: $a3
	// size: 0xC
	register struct _TFace *tface;
	// register: $t1
	register long waterZLevel;
} //line: 54


// path: C:\kain2\game\STREAM.C
// address: 0x80059A48
// line start: 1016
// line end:   1017
int STREAM_IsMorphInProgress();

// path: C:\kain2\game\STREAM.C
// address: 0x80059A5C
// line start: 1022
// line end:   1049
long STREAM_GetWaterZLevel(struct Level *level, struct _Instance *instance)
{ //line: 2
	// register: $a1
	register long waterZLevel;
} //line: 27


// path: C:\kain2\game\STREAM.C
// address: 0x80059ADC
// line start: 1052
// line end:   1070
void STREAM_SetMainFog(struct _StreamUnit *streamUnit)
{ //line: 2
	// register: $v1
	// size: 0x188
	register struct Level *level;
} //line: 17


// path: C:\kain2\game\STREAM.C
// address: 0x80059B30
// line start: 1073
// line end:   1086
void STREAM_SetStreamFog(struct _StreamUnit *streamUnit, short fogNear, short fogFar)
{ //line: 1
} //line: 13


// path: C:\kain2\game\STREAM.C
// address: 0x80059BC0
// line start: 1130
// line end:   1302
void STREAM_ConnectStream(struct _StreamUnit *streamUnit)
{ //line: 1
	// address: -48
	// size: 0x40
	auto struct _StreamUnit *mainUnit;
	// register: $a0
	// size: 0x5C
	register struct StreamUnitPortal *streamPortal;
	// register: $v1
	register int numportals;
	// register: $s4
	// size: 0x5C
	register struct StreamUnitPortal *streamPortal2;
	// register: $fp
	register int numportals2;
	// register: $s0
	register int i;
	// register: $s3
	register int j;
	// address: -72
	// size: 0x10
	auto char text[16];
	// register: $v0
	register char *commapos;
	// register: $s2
	register int signalID;
	{ //line: 25
		// register: $s0
		// size: 0x40
		register struct _StreamUnit *connectStream;
		{ //line: 59
			// address: -56
			// size: 0x8
			auto struct _SVector offset;
		} //line: 70
	} //line: 79
	{ //line: 84
		// address: -44
		auto long d;
		// register: $s5
		// size: 0x40
		register struct _StreamUnit *connectStream;
		{ //line: 99
			// register: $s0
			register long hookedUp;
			{ //line: 139
				// address: -56
				// size: 0x8
				auto struct _SVector offset;
			} //line: 150
		} //line: 159
	} //line: 159
} //line: 173


// path: C:\kain2\game\STREAM.C
// address: 0x80059FCC
// line start: 1305
// line end:   1312
void STREAM_StreamLoadLevelAbort(void *loadData, void *data, void *data2)
{ //line: 1
	// register: $s0
	// size: 0x40
	register struct _StreamUnit *streamUnit;
} //line: 5


// path: C:\kain2\game\STREAM.C
// address: 0x8005A004
// line start: 1318
// line end:   1335
void STREAM_DoObjectLoadAndDump(struct _StreamUnit *streamUnit)
{ //line: 1
	// register: $a0
	register int i;
} //line: 18


// path: C:\kain2\game\STREAM.C
// address: 0x8005A04C
// line start: 1339
// line end:   1418
void STREAM_FinishLoad(struct _StreamUnit *streamUnit)
{ //line: 1
	// register: $s0
	// size: 0x188
	register struct Level *level;
	// address: -96
	// size: 0x50
	auto char sfxName[80];
} //line: 69


// path: C:\kain2\game\STREAM.C
// address: 0x8005A198
// line start: 1422
// line end:   1454
void STREAM_LoadLevelReturn(void *loadData, void *data, void *data2)
{ //line: 1
} //line: 1


// path: C:\kain2\game\STREAM.C
// address: 0x8005A1F8
// line start: 1457
// line end:   1554
void STREAM_StreamLoadLevelReturn(void *loadData, void *data, void *data2)
{ //line: 1
	// register: $s0
	// size: 0x188
	register struct Level *level;
	// register: $s1
	// size: 0x40
	register struct _StreamUnit *streamUnit;
} //line: 98


// path: C:\kain2\game\STREAM.C
// address: 0x8005A2E0
// line start: 1582
// line end:   1674
void STREAM_UpdateLevelPointer(struct Level *oldLevel, struct Level *newLevel, long sizeOfLevel)
{ //line: 1
	// register: $t0
	register long i;
	// register: $t1
	register long offset;
	// register: $v0
	// size: 0x270
	register struct GameTracker *gameTracker;
	{ //line: 29
		// register: $a3
		// size: 0x29C
		register struct _Instance *instance;
	} //line: 69
} //line: 91


// path: C:\kain2\game\STREAM.C
// address: 0x8005A5D0
// size: 0x40
// line start: 1678
// line end:   1698
struct _StreamUnit *STREAM_WhichUnitPointerIsIn(void *pointer)
{ //line: 1
	// register: $s2
	register int i;
	// register: $v1
	register int size;
	// register: $s0
	// size: 0x188
	register struct Level *level;
} //line: 21


// path: C:\kain2\game\STREAM.C
// address: 0x8005A67C
// line start: 1701
// line end:   1786
void STREAM_UpdateObjectPointer(struct Object *oldObject, struct Object *newObject, long sizeOfObject)
{ //line: 1
	// register: $a3
	register long i;
	// register: $a1
	register long d;
	// register: $s0
	// size: 0x270
	register struct GameTracker *gameTracker;
	// register: $s1
	register long offset;
	// register: $t0
	// size: 0x24
	register struct _ObjectTracker *otr;
	{ //line: 21
		// register: $a1
		register int j;
		// register: $a2
		// size: 0x4C
		register struct Object *object;
	} //line: 31
	{ //line: 49
		// register: $s0
		// size: 0x29C
		register struct _Instance *instance;
	} //line: 78
} //line: 86


// path: C:\kain2\game\STREAM.C
// address: 0x8005A91C
// line start: 1789
// line end:   1803
void STREAM_UpdateInstanceCollisionInfo(struct _HModel *oldHModel, struct _HModel *newHModel)
{ //line: 2
	// register: $v1
	// size: 0x29C
	register struct _Instance *instance;
} //line: 15


// path: C:\kain2\game\STREAM.C
// address: 0x8005A960
// line start: 1844
// line end:   1874
void STREAM_LoadMainVram(struct GameTracker *gameTracker, char *baseAreaName, struct _StreamUnit *streamUnit)
{ //line: 1
	// address: -176
	// size: 0x50
	auto char dramName[80];
	// address: -96
	// size: 0x50
	auto char vramName[80];
	// register: $v0
	// size: 0x14
	register struct VramBuffer *vramBuffer;
	// register: $s0
	// size: 0x188
	register struct Level *level;
} //line: 1


// path: C:\kain2\game\STREAM.C
// address: 0x8005AA08
// line start: 1892
// line end:   1908
void STREAM_MoveIntoNewStreamUnit();

// path: C:\kain2\game\STREAM.C
// address: 0x8005AA9C
// size: 0x40
// line start: 1912
// line end:   2098
struct _StreamUnit *STREAM_LoadLevel(char *baseAreaName, struct StreamUnitPortal *streamPortal, int loadnext)
{ //line: 1
	// register: $s2
	register int i;
	// register: $s5
	register long streamID;
	// register: $s1
	// size: 0x40
	register struct _StreamUnit *streamUnit;
	{ //line: 42
	} //line: 45
	{ //line: 54
		// register: $s0
		// size: 0x188
		register struct Level *level;
	} //line: 64
	{ //line: 126
		// address: -112
		// size: 0x50
		auto char dramName[80];
	} //line: 183
} //line: 186


// path: C:\kain2\game\STREAM.C
// address: 0x8005ACC0
// line start: 2103
// line end:   2124
void RemoveIntroducedLights(struct Level *level)
{ //line: 1
	// register: $s0
	register int i;
} //line: 22


// path: C:\kain2\game\STREAM.C
// address: 0x8005ADA4
// line start: 2127
// line end:   2155
void STREAM_RemoveInstancesWithIDInInstanceList(struct _InstanceList *list, long id, struct Level *level)
{ //line: 1
	// register: $s0
	// size: 0x29C
	register struct _Instance *instance;
	// register: $s1
	// size: 0x29C
	register struct _Instance *next;
} //line: 29


// path: C:\kain2\game\STREAM.C
// address: 0x8005AE50
// line start: 2158
// line end:   2175
void STREAM_MarkUnitNeeded(long streamID)
{ //line: 2
	// register: $a1
	register int i;
} //line: 18


// path: C:\kain2\game\STREAM.C
// address: 0x8005AE9C
// line start: 2178
// line end:   2267
void STREAM_DumpUnit(struct _StreamUnit *streamUnit, long doSave)
{ //line: 1
	// register: $s0
	register int i;
	// register: $a0
	register int j;
	// register: $a1
	register int numportals;
	{ //line: 30
		// address: -104
		// size: 0x50
		auto char dramName[80];
	} //line: 41
	{ //line: 58
		// register: $a1
		// size: 0x24
		register struct _SFXMkr *sfxMkr;
	} //line: 58
} //line: 90


// path: C:\kain2\game\STREAM.C
// address: 0x8005B080
// line start: 2270
// line end:   2282
void STREAM_DumpAllUnitsNotNeeded()
{ //line: 1
	// register: $s1
	register int i;
} //line: 13


// path: C:\kain2\game\STREAM.C
// address: 0x8005B0E8
// line start: 2285
// line end:   2298
void STREAM_DumpAllLevels(long IDNoRemove, int DoSave)
{ //line: 1
	// register: $s1
	register int i;
} //line: 14


// path: C:\kain2\game\STREAM.C
// address: 0x8005B164
// line start: 2302
// line end:   2314
void STREAM_LoadCurrentWarpRoom(struct StreamUnitPortal *streamPortal, struct _StreamUnit *mainStreamUnit);

// path: C:\kain2\game\STREAM.C
// address: 0x8005B220
// line start: 2319
// line end:   2332
void WARPGATE_RelocateLoadedWarpRooms(struct _StreamUnit *mainUnit, struct StreamUnitPortal *streamPortal)
{ //line: 1
	// register: $s2
	register int i;
} //line: 14


// path: C:\kain2\game\STREAM.C
// address: 0x8005B2C8
// line start: 2336
// line end:   2348
long WARPGATE_GetWarpRoomIndex(char *name)
{ //line: 1
	// register: $s0
	register int i;
} //line: 13


// path: C:\kain2\game\STREAM.C
// address: 0x8005B328
// line start: 2351
// line end:   2359
void WARPGATE_UpdateAddToArray(struct _StreamUnit *streamUnit)
{ //line: 1
	// register: $v1
	register int i;
} //line: 9


// path: C:\kain2\game\STREAM.C
// address: 0x8005B374
// line start: 2362
// line end:   2369
void WARPGATE_RemoveFromArray(struct _StreamUnit *streamUnit)
{ //line: 1
} //line: 8


// path: C:\kain2\game\STREAM.C
// address: 0x8005B3A4
// line start: 2373
// line end:   2388
void WARPGATE_Init()
{ //line: 2
	// register: $a0
	register int n;
} //line: 16


// path: C:\kain2\game\STREAM.C
// address: 0x8005B3EC
// line start: 2392
// line end:   2394
void WARPGATE_StartUsingWarpgate();

// path: C:\kain2\game\STREAM.C
// address: 0x8005B424
// line start: 2399
// line end:   2401
void WARPGATE_EndUsingWarpgate();

// path: C:\kain2\game\STREAM.C
// address: 0x8005B45C
// line start: 2406
// line end:   2407
int WARPGATE_IsWarpgateInUse();

// path: C:\kain2\game\STREAM.C
// address: 0x8005B468
// line start: 2412
// line end:   2413
int WARPGATE_IsWarpgateActive();

// path: C:\kain2\game\STREAM.C
// address: 0x8005B474
// line start: 2419
// line end:   2420
int WARPGATE_IsWarpgateUsable();

// path: C:\kain2\game\STREAM.C
// address: 0x8005B488
// line start: 2426
// line end:   2427
int WARPGATE_IsWarpgateReady();

// path: C:\kain2\game\STREAM.C
// address: 0x8005B49C
// line start: 2433
// line end:   2434
int WARPGATE_IsWarpgateSpectral();

// path: C:\kain2\game\STREAM.C
// address: 0x8005B4D8
// line start: 2440
// line end:   2460
int WARPGATE_IsObjectOnWarpSide(struct _Instance *instance)
{ //line: 2
	// register: $a0
	register int side;
} //line: 21


// path: C:\kain2\game\STREAM.C
// address: 0x8005B53C
// line start: 2464
// line end:   2487
void WARPGATE_IsItActive(struct _StreamUnit *streamUnit)
{ //line: 2
	// register: $t1
	// size: 0x188
	register struct Level *level;
	// register: $a1
	register int d;
} //line: 24


// path: C:\kain2\game\STREAM.C
// address: 0x8005B604
// line start: 2490
// line end:   2510
long WARPGATE_IsUnitWarpRoom(struct _StreamUnit *streamUnit)
{ //line: 2
	// register: $v0
	// size: 0x188
	register struct Level *level;
	// register: $a1
	register long isWarpRoom;
	// register: $v1
	// size: 0x5C
	register struct StreamUnitPortal *streamPortal;
	// register: $a2
	register long numPortals;
	// register: $a0
	register long d;
} //line: 20


// path: C:\kain2\game\STREAM.C
// address: 0x8005B65C
// line start: 2513
// line end:   2525
void WARPGATE_FixUnit(struct _StreamUnit *streamUnit)
{ //line: 1
} //line: 13


// path: C:\kain2\game\STREAM.C
// address: 0x8005B690
// line start: 2529
// line end:   2539
void STREAM_MarkWarpUnitsNeeded()
{ //line: 2
	// register: $a0
	register int i;
} //line: 11


// path: C:\kain2\game\STREAM.C
// address: 0x8005B6D0
// line start: 2545
// line end:   2571
long WARPGATE_IncrementIndex()
{ //line: 1
	// register: $s1
	register long result;
} //line: 26


// path: C:\kain2\game\STREAM.C
// address: 0x8005B7E8
// line start: 2574
// line end:   2579
void WARPGATE_CalcWarpFade(int timeInc);

// path: C:\kain2\game\STREAM.C
// address: 0x8005B858
// line start: 2583
// line end:   2611
long WARPGATE_DecrementIndex()
{ //line: 1
	// register: $s0
	register long result;
} //line: 28


// path: C:\kain2\game\STREAM.C
// address: 0x8005B930
// line start: 2615
// line end:   2760
void PreloadAllConnectedUnits(struct _StreamUnit *streamUnit, struct _SVector *offset)
{ //line: 1
	// register: $s1
	register int i;
	// address: -48
	// size: 0x10
	auto char text[16];
	// register: $s3
	register int numportals;
	// register: $v0
	register char *commapos;
	// register: $s0
	// size: 0x5C
	register struct StreamUnitPortal *stream;
} //line: 146


// path: C:\kain2\game\STREAM.C
// address: 0x8005BAF4
// line start: 2826
// line end:   2858
void RelocateLevel(struct Level *level, struct _SVector *offset)
{ //line: 1
	// register: $t2
	register int curTree;
	{ //line: 24
		// register: $v1
		register short _x0;
		// register: $a0
		register short _y0;
		// register: $a1
		register short _z0;
		// register: $a2
		register short _x1;
		// register: $t0
		register short _y1;
		// register: $t1
		register short _z1;
		// register: $a3
		// size: 0x6
		register struct _Position *_v;
	} //line: 26
} //line: 33


// path: C:\kain2\game\STREAM.C
// address: 0x8005BC34
// line start: 2861
// line end:   2874
void RelocateCameras(struct _CameraKey *cameraList, long numCameras, struct _SVector *offset)
{ //line: 2
	// register: $a3
	register int i;
} //line: 14


// path: C:\kain2\game\STREAM.C
// address: 0x8005BCC8
// line start: 2877
// line end:   2905
void RelocateSavedCameras(struct Camera_duplicate_s2 *camera, struct Level *level, struct _SVector *offset)
{ //line: 1
	// register: $t6
	register int i;
	{ //line: 11
		// register: $v0
		register short _x0;
		// register: $v0
		register short _y0;
		// register: $v1
		register short _z0;
		// register: $v1
		register short _x1;
		// register: $a1
		register short _y1;
		// register: $a2
		register short _z1;
		// register: $a0
		// size: 0x6
		register struct _Position *_v;
	} //line: 13
	{ //line: 13
		// register: $v0
		register short _x0;
		// register: $v1
		register short _y0;
		// register: $a1
		register short _z0;
		// register: $a2
		register short _x1;
		// register: $t0
		register short _y1;
		// register: $t1
		register short _z1;
		// register: $a3
		// size: 0x6
		register struct _Position *_v;
	} //line: 13
	{ //line: 13
		// register: $v0
		register short _x0;
		// register: $v1
		register short _y0;
		// register: $a1
		register short _z0;
		// register: $a2
		register short _x1;
		// register: $t0
		register short _y1;
		// register: $t1
		register short _z1;
		// register: $a3
		// size: 0x6
		register struct _Position *_v;
	} //line: 13
	{ //line: 13
		// register: $v0
		register short _x0;
		// register: $v1
		register short _y0;
		// register: $a1
		register short _z0;
		// register: $a2
		register short _x1;
		// register: $t0
		register short _y1;
		// register: $t1
		register short _z1;
		// register: $a3
		// size: 0x6
		register struct _Position *_v;
	} //line: 22
} //line: 29


// path: C:\kain2\game\STREAM.C
// address: 0x8005BE00
// line start: 2908
// line end:   2914
void RelocateLevelWithInstances(struct Level *level, struct _SVector *offset);

// path: C:\kain2\game\STREAM.C
// address: 0x8005BE38
// line start: 2919
// line end:   3050
void RelocateTerrain(struct _Terrain *terrain, struct _SVector *offset)
{ //line: 1
	// register: $s1
	register int i;
} //line: 128


// path: C:\kain2\game\STREAM.C
// address: 0x8005BF28
// line start: 3053
// line end:   3076
void RelocateVMObjects(struct _VMObject *vobjectlist, long numvmobjs, struct _SVector *offset)
{ //line: 2
	// register: $a3
	register int i;
} //line: 24


// path: C:\kain2\game\STREAM.C
// address: 0x8005BF80
// line start: 3108
// line end:   3120
void RelocateBGObjects(struct _BGObject *BGObjList, long numBGObjs, struct _SVector *offset)
{ //line: 1
	// register: $t1
	register int i;
	// register: $a3
	register int d;
} //line: 13


// path: C:\kain2\game\STREAM.C
// address: 0x8005C024
// line start: 3123
// line end:   3141
void RelocatePlanPool(struct PlanningNode *planPool, struct _SVector *offset)
{ //line: 1
	// register: $a1
	register int i;
	// register: $s1
	register short ox;
	// register: $s2
	register short oy;
	// register: $s3
	register short oz;
} //line: 16


// path: C:\kain2\game\STREAM.C
// address: 0x8005C0D8
// line start: 3144
// line end:   3158
void RelocatePlanMarkers(struct _PlanMkr *planMkrList, int numPlanMkrs, struct _SVector *offset)
{ //line: 1
	// register: $a1
	register int i;
	// register: $t1
	register short ox;
	// register: $t0
	register short oy;
	// register: $a2
	register short oz;
} //line: 15


// path: C:\kain2\game\STREAM.C
// address: 0x8005C12C
// line start: 3161
// line end:   3175
void RelocateSFXMarkers(struct _SFXMkr *sfxMkrList, int numSFXMkrs, struct _SVector *offset)
{ //line: 2
	// register: $a1
	register int i;
	// register: $t0
	register short ox;
	// register: $a3
	register short oy;
	// register: $a2
	register short oz;
} //line: 15


// path: C:\kain2\game\STREAM.C
// address: 0x8005C178
// line start: 3178
// line end:   3199
void STREAM_AdjustMultiSpline(struct MultiSpline *multi, struct _SVector *offset)
{ //line: 1
	// register: $a2
	register int i;
} //line: 22


// path: C:\kain2\game\STREAM.C
// address: 0x8005C228
// line start: 3202
// line end:   3204
void STREAM_RelocateInstance(struct _Instance *instance, struct _SVector *offset);

// path: C:\kain2\game\STREAM.C
// address: 0x8005C270
// line start: 3210
// line end:   3232
void STREAM_OffsetInstancePosition(struct _Instance *instance, struct _SVector *offset, int streamSignalFlag);

// path: C:\kain2\game\STREAM.C
// address: 0x8005C388
// line start: 3237
// line end:   3246
void STREAM_SetInstancePosition(struct _Instance *instance, struct evPositionData *data)
{ //line: 1
	// address: -16
	// size: 0x8
	auto struct _SVector offset;
} //line: 1


// path: C:\kain2\game\STREAM.C
// address: 0x8005C3E0
// line start: 3248
// line end:   3259
void RelocateInstances(struct _SVector *offset)
{ //line: 1
	// register: $s0
	// size: 0x29C
	register struct _Instance *instance;
} //line: 11


// path: C:\kain2\game\STREAM.C
// address: 0x8005C43C
// line start: 3262
// line end:   3288
void RelocateStreamPortals(struct StreamUnitPortal *StreamUnitList, int NumStreamUnits, struct _SVector *offset)
{ //line: 2
	// register: $t1
	register int i;
	// register: $t0
	register int d;
} //line: 27


// path: C:\kain2\game\STREAM.C
// address: 0x8005C56C
// line start: 3299
// line end:   3353
void STREAM_PackVRAMObject(struct _ObjectTracker *objectTracker)
{ //line: 1
	// register: $a0
	// size: 0x8
	register struct VramSize *vramSize;
	{ //line: 10
		// register: $v0
		// size: 0x14
		register struct VramBuffer *vramBuffer;
		{ //line: 18
			// address: -80
			// size: 0x40
			auto char fileName[64];
			// register: $s0
			// size: 0x1C
			register struct _BlockVramEntry_duplicate_s8 *vramBlock;
		} //line: 28
	} //line: 47
} //line: 55


// path: C:\kain2\game\STREAM.C
// address: 0x8005C66C
// line start: 3383
// line end:   3428
void MORPH_SetupInstanceFlags(struct _Instance *instance);

// path: C:\kain2\game\STREAM.C
// address: 0x8005C794
// line start: 3432
// line end:   3443
void MORPH_SetupInstanceListFlags()
{ //line: 1
	// register: $s0
	// size: 0x29C
	register struct _Instance *instance;
} //line: 12


// path: C:\kain2\game\STREAM.C
// address: 0x8005C7DC
// line start: 3446
// line end:   3470
void MORPH_InMorphInstanceListFlags()
{ //line: 2
	// register: $v1
	// size: 0x29C
	register struct _Instance *instance;
} //line: 25


// path: C:\kain2\game\STREAM.C
// address: 0x8005C844
// line start: 3473
// line end:   3488
void MORPH_InMorphDoFadeValues()
{ //line: 2
	// register: $a0
	register int fade1;
	// register: $v0
	register int fade2;
} //line: 16


// path: C:\kain2\game\STREAM.C
// address: 0x8005C89C
// line start: 3495
// line end:   3554
void MORPH_UpdateTimeMult()
{ //line: 3
	{ //line: 6
		// register: $v1
		register short ratio;
	} //line: 44
} //line: 60


// path: C:\kain2\game\STREAM.C
// address: 0x8005CA10
// line start: 3559
// line end:   3716
void MORPH_UpdateNormals(struct Level *BaseLevel)
{ //line: 1
	// register: $v1
	// size: 0xC
	register struct _TFace *face;
	// register: $a2
	register long faceCount;
	// register: $a1
	// size: 0xC
	register struct _TVertex *v;
	// register: $a3
	// size: 0xE
	register struct _MorphVertex *mv;
	// register: $a0
	// size: 0x2
	register struct _MorphColor *mc;
	// register: $v1
	register short h1;
	// register: $a1
	register short *morphNormals;
	{ //line: 50
		// register: $a2
		// size: 0xC
		register struct _TVertex *endv;
	} //line: 71
	{ //line: 71
		// register: $v1
		// size: 0x2C
		register struct _BSPNode *node;
		// register: $a1
		// size: 0x30
		register struct _BSPLeaf *leaf;
		// address: -32
		// size: 0x8
		auto struct _Sphere_noSq hsphere;
		// address: -24
		// size: 0xC
		auto struct _BoundingBox hbox;
		// register: $a0
		// size: 0x54
		register struct _Terrain *terrain;
		// register: $t0
		register long curTree;
	} //line: 122
	{ //line: 124
		// register: $s0
		// size: 0x29C
		register struct _Instance *instance;
		{ //line: 130
			// address: -32
			// size: 0x8
			auto struct SVECTOR realDiff;
			// address: -24
			// size: 0x6
			auto struct _Position oldPos;
		} //line: 154
	} //line: 157
} //line: 158


// path: C:\kain2\game\STREAM.C
// address: 0x8005CEC4
// line start: 3719
// line end:   3887
void MORPH_BringBackNormals(struct Level *BaseLevel)
{ //line: 1
	// register: $v1
	// size: 0xC
	register struct _TFace *face;
	// register: $a2
	register long faceCount;
	// register: $a1
	// size: 0xC
	register struct _TVertex *v;
	// register: $a0
	// size: 0xE
	register struct _MorphVertex *mv;
	// register: $v0
	// size: 0x2
	register struct _MorphColor *mc;
	// register: $v1
	register short h1;
	// register: $a1
	register short *morphNormals;
	{ //line: 56
		// register: $a2
		// size: 0xC
		register struct _TVertex *endv;
	} //line: 67
	{ //line: 82
		// register: $v1
		// size: 0x2C
		register struct _BSPNode *node;
		// register: $a1
		// size: 0x30
		register struct _BSPLeaf *leaf;
		// address: -32
		// size: 0x8
		auto struct _Sphere_noSq hsphere;
		// address: -24
		// size: 0xC
		auto struct _BoundingBox hbox;
		// register: $a0
		// size: 0x54
		register struct _Terrain *terrain;
		// register: $t0
		register long curTree;
	} //line: 132
	{ //line: 135
		// register: $s0
		// size: 0x29C
		register struct _Instance *instance;
		{ //line: 141
			// address: -32
			// size: 0x8
			auto struct SVECTOR realDiff;
			// address: -24
			// size: 0x6
			auto struct _Position oldPos;
		} //line: 165
	} //line: 168
} //line: 169


// path: C:\kain2\game\STREAM.C
// address: 0x8005D334
// line start: 3890
// line end:   4016
void MORPH_AddOffsets(struct Level *BaseLevel, int time)
{ //line: 1
	// register: $a3
	// size: 0xC
	register struct _TVertex *v;
	// register: $a2
	// size: 0xE
	register struct _MorphVertex *mv;
	// register: $t2
	// size: 0x2
	register struct _MorphColor *mc;
	// register: $v0
	register long m;
	// register: $s1
	register long fixed_time;
	// register: $s0
	// size: 0x29C
	register struct _Instance *instance;
	{ //line: 49
		// address: -40
		// size: 0x8
		auto struct SVECTOR diff;
		// address: -32
		// size: 0x8
		auto struct SVECTOR realDiff;
		// address: -24
		// size: 0x6
		auto struct _Position oldPos;
	} //line: 68
	{ //line: 77
		// register: $a2
		register long r0;
		// register: $a1
		register long g0;
		// register: $a0
		register long b0;
		// register: $v0
		register long r1;
		// register: $v0
		register long g1;
		// register: $v1
		register long b1;
		{ //line: 87
			// register: $t4
			// size: 0xC
			register struct _TVertex *endv;
		} //line: 102
	} //line: 125
} //line: 127


// path: C:\kain2\game\STREAM.C
// address: 0x8005D700
// line start: 4019
// line end:   4143
void MORPH_SubtractOffsets(struct Level *BaseLevel, int time)
{ //line: 1
	// register: $a3
	// size: 0xC
	register struct _TVertex *v;
	// register: $a2
	// size: 0xE
	register struct _MorphVertex *mv;
	// register: $t2
	// size: 0x2
	register struct _MorphColor *mc;
	// register: $v0
	register long m;
	// register: $s1
	register long fixed_time;
	// register: $s0
	// size: 0x29C
	register struct _Instance *instance;
	{ //line: 47
		// address: -40
		// size: 0x8
		auto struct SVECTOR diff;
		// address: -32
		// size: 0x8
		auto struct SVECTOR realDiff;
		// address: -24
		// size: 0x6
		auto struct _Position oldPos;
	} //line: 67
	{ //line: 76
		// register: $a2
		register long r0;
		// register: $a1
		register long g0;
		// register: $a0
		register long b0;
		// register: $v0
		register long r1;
		// register: $v0
		register long g1;
		// register: $v1
		register long b1;
		{ //line: 85
			// register: $t4
			// size: 0xC
			register struct _TVertex *endv;
		} //line: 100
	} //line: 123
} //line: 125


// path: C:\kain2\game\STREAM.C
// address: 0x8005DAD4
// line start: 4149
// line end:   4216
void MORPH_GetComponentsForTrackingPoint(struct _TFace *face, struct Level *level)
{ //line: 1
	// address: -32
	// size: 0xC
	auto struct _SVector *v[3];
	// register: $a2
	// size: 0x6
	register struct _Position *offset;
	// address: -16
	// size: 0x6
	auto struct _Position player;
	// register: $a1
	register int n;
	// register: $t0
	register int saved_div;
	// register: $a0
	register int next;
	// register: $t1
	register int side;
	// register: $t3
	register int track;
	// address: -8
	// size: 0x8
	auto int x[2];
	// register: $a0
	// size: 0xC
	register struct _TVertex *vertexList;
	{ //line: 13
		// register: $v0
		register short _x0;
		// register: $a1
		register short _y0;
		// register: $a0
		register short _z0;
		// register: $v1
		register short _x1;
		// register: $a3
		register short _y1;
		// register: $a2
		register short _z1;
		// register: $v0
		// size: 0x6
		register struct _Position *_v;
		// register: $a0
		// size: 0x6
		register struct _Position *_v0;
	} //line: 13
	{ //line: 29
		// register: $v1
		register int next;
		{ //line: 36
			// register: $a0
			register int div;
		} //line: 41
	} //line: 42
} //line: 68


// path: C:\kain2\game\STREAM.C
// address: 0x8005DDB4
// line start: 4219
// line end:   4231
void MORPH_AveragePoint(struct _SVector *start, struct _SVector *end, int interp, struct _SVector *out);

// path: C:\kain2\game\STREAM.C
// address: 0x8005DE2C
// line start: 4235
// line end:   4268
void MORPH_UpdateTrackingPoint(struct _TFace *face, struct Level *level)
{ //line: 1
	// address: -56
	// size: 0xC
	auto struct _SVector *v[3];
	// address: -40
	// size: 0x8
	auto struct _SVector p1;
	// address: -32
	// size: 0x8
	auto struct _SVector p2;
	// address: -24
	// size: 0x8
	auto struct _SVector p3;
	// register: $a3
	// size: 0x6
	register struct _Position *offset;
	// register: $a1
	register int next;
	// register: $a0
	// size: 0xC
	register struct _TVertex *vertexList;
	{ //line: 21
		// register: $v0
		register short _x0;
		// register: $a0
		register short _y0;
		// register: $a1
		register short _z0;
		// register: $v1
		register short _x1;
		// register: $t0
		register short _y1;
		// register: $a3
		register short _z1;
	} //line: 21
	{ //line: 21
		// register: $a2
		// size: 0x6
		register struct _Position *_v0;
	} //line: 21
} //line: 34


// path: C:\kain2\game\STREAM.C
// address: 0x8005DFD0
// line start: 4271
// line end:   4327
void MORPH_ToggleMorph()
{ //line: 1
	{ //line: 19
		// register: $s0
		// size: 0x188
		register struct Level *level;
		// register: $s2
		register int i;
	} //line: 55
} //line: 57


// path: C:\kain2\game\STREAM.C
// address: 0x8005E11C
// line start: 4330
// line end:   4387
void MORPH_DoStep(struct _StreamUnit *streamUnit, char *baseAreaName)
{ //line: 1
	// register: $s0
	// size: 0x188
	register struct Level *level;
} //line: 58


// path: C:\kain2\game\STREAM.C
// address: 0x8005E1F8
// line start: 4390
// line end:   4412
void MORPH_SetFog(struct _StreamUnit *streamUnit, int mainUnitFlag)
{ //line: 1
	// register: $v1
	register long time;
	// register: $a2
	// size: 0x188
	register struct Level *level;
	// register: $a1
	register int fogNear;
	// register: $v0
	register int fogFar;
} //line: 23


// path: C:\kain2\game\STREAM.C
// address: 0x8005E2CC
// line start: 4415
// line end:   4427
void MORPH_UpdateTextures()
{ //line: 1
	// register: $a0
	register long time;
} //line: 11


// path: C:\kain2\game\STREAM.C
// address: 0x8005E324
// line start: 4430
// line end:   4488
void MORPH_Continue()
{ //line: 1
	// register: $s2
	register int i;
	// register: $v0
	register long mode;
} //line: 59


// path: C:\kain2\game\STREAM.C
// address: 0x8005E498
// line start: 4490
// line end:   4492
void STREAM_MORPH_Relocate();

// path: C:\kain2\game\STREAM.C
// address: 0x8005E4A8
// line start: 4653
// line end:   4689
int AddVertex(struct VECTOR *v0, struct RECT *rect)
{ //line: 1
	// address: -8
	// size: 0x8
	auto struct SVECTOR v;
	// register: $v0
	register int x;
	// register: $v1
	register int y;
	// register: $a1
	register int z;
	// register: $a0
	register int scr_x;
	// register: $a1
	register int scr_y;
} //line: 37


// path: C:\kain2\game\STREAM.C
// address: 0x8005E634
// line start: 4692
// line end:   4693
int GetPlaneDist(int k, int j, int i, struct VECTOR *v);

// path: C:\kain2\game\STREAM.C
// address: 0x8005E6C0
// line start: 4701
// line end:   4712
void CalcVert(struct VECTOR *v, struct VECTOR *v1, struct VECTOR *v2, int dist1, int dist2, int k, int j)
{ //line: 1
	// register: $a3
	register int diff;
} //line: 1


// path: C:\kain2\game\STREAM.C
// address: 0x8005E748
// line start: 4715
// line end:   4797
int AddClippedTri(struct SVECTOR *iv, struct RECT *cliprect, int *minz)
{ //line: 1
	// register: $s7
	register int clip0;
	// register: $s2
	register int clip1;
	// register: $s5
	register int i;
	// address: -64
	auto int j;
	// address: -60
	auto int k;
	// address: -56
	auto int l;
	// register: $s6
	register int l_1;
	// register: $fp
	register int numvert;
	// register: $s3
	register int newnumvert;
	// address: -576
	// size: 0x100
	auto struct VECTOR v0[16];
	// address: -320
	// size: 0x100
	auto struct VECTOR v1[16];
	// address: -52
	// size: 0x10
	auto struct VECTOR *src;
	// address: -48
	// size: 0x10
	auto struct VECTOR *dst;
	// register: $v0
	// size: 0x10
	register struct VECTOR *tmp;
	{ //line: 77
		// register: $v0
		register int z;
	} //line: 81
} //line: 83


// path: C:\kain2\game\STREAM.C
// address: 0x8005E9F0
// line start: 4800
// line end:   4965
int STREAM_GetClipRect(struct StreamUnitPortal *portal, struct RECT *rect)
{ //line: 1
	// register: $v0
	register int v1x;
	// register: $t2
	register int v1y;
	// register: $a3
	register int v1z;
	// register: $t0
	register int v2x;
	// register: $v1
	register int v2y;
	// register: $t1
	register int v2z;
	// register: $s7
	register int nx;
	// register: $s6
	register int ny;
	// address: -56
	auto int nz;
	// register: $s4
	register int nx2;
	// register: $s2
	register int ny2;
	// register: $s1
	register int nz2;
	// address: -52
	auto int len;
	// register: $fp
	register int side;
	// register: $s0
	register int side2;
	// register: $s0
	register int retval;
	// address: -48
	auto int horizontal_flag;
	// address: -44
	auto int fullscreen_flag;
	// address: -64
	auto int minz;
	// address: -60
	auto int minz2;
	{ //line: 89
		// register: $v0
		register int dot;
	} //line: 99
} //line: 166


// path: C:\kain2\game\STREAM.C
// address: 0x8005EFB8
// line start: 4969
// line end:   5075
long GetFogColor(struct StreamUnitPortal *portal, struct _StreamUnit *mainStreamUnit, struct Level *mainLevel)
{ //line: 1
	// address: -36
	auto long z1;
	// address: -32
	auto long z2;
	// address: -28
	auto long z3;
	// register: $a1
	register long zval;
	// register: $v1
	register long zval2;
	// register: $s0
	register long interp;
	// address: -20
	auto long Color;
	// address: -24
	auto long MainColor;
	// address: -40
	auto long LevelColor;
	// register: $a1
	// size: 0x188
	register struct Level *level;
	{ //line: 13
		// register: $a2
		register int time;
	} //line: 19
} //line: 107


// path: C:\kain2\game\STREAM.C
// address: 0x8005F2DC
// line start: 5078
// line end:   5119
void DrawFogRectangle(struct RECT *cliprect, struct _PrimPool *primPool, int otzpos, unsigned long **drawot, long color)
{ //line: 1
	// register: $t0
	// size: 0x24
	register struct POLY_G4 *polyg4;
} //line: 42


// path: C:\kain2\game\STREAM.C
// address: 0x8005F3C8
// line start: 5126
// line end:   5143
void STREAM_RenderAdjacantUnit(unsigned long **curOT, struct StreamUnitPortal *curStreamPortal, struct _StreamUnit *toStreamUnit, struct _StreamUnit *mainStreamUnit, struct RECT *cliprect)
{ //line: 1
	{ //line: 5
		// register: $s0
		register long portalFogColor;
	} //line: 17
} //line: 18


// path: C:\kain2\game\STREAM.C
// address: 0x8005F474
// size: 0x24
// line start: 5146
// line end:   5167
struct BSPTree *STREAM_GetBspTree(struct _StreamUnit *streamUnit, long bspNumber)
{ //line: 2
	// register: $v0
	// size: 0x188
	register struct Level *level;
	// register: $v0
	// size: 0x54
	register struct _Terrain *terrain;
	// register: $a0
	register int d;
} //line: 22


// path: C:\kain2\game\STREAM.C
// address: 0x8005F4D4
// line start: 5171
// line end:   5188
void WARPGATE_BlockWarpGateEntrance(struct _StreamUnit *streamUnit, long collideOn)
{ //line: 1
	// register: $v1
	// size: 0x24
	register struct BSPTree *bspTree;
} //line: 18


// path: C:\kain2\game\STREAM.C
// address: 0x8005F544
// line start: 5192
// line end:   5212
void WARPGATE_DrawWarpGateRim(struct _StreamUnit *streamUnit, long drawOn)
{ //line: 1
	// register: $v1
	// size: 0x24
	register struct BSPTree *bspTree;
} //line: 21


// path: C:\kain2\game\STREAM.C
// address: 0x8005F59C
// line start: 5215
// line end:   5239
void WARPGATE_HideAllCloudCovers()
{ //line: 2
	// register: $a1
	// size: 0x4C
	register struct Object *warpFaceObject;
	{ //line: 8
		// register: $v1
		// size: 0x29C
		register struct _Instance *instance;
		// register: $a0
		// size: 0x29C
		register struct _Instance *next;
	} //line: 23
} //line: 25


// path: C:\kain2\game\STREAM.C
// address: 0x8005F5FC
// size: 0x29C
// line start: 5242
// line end:   5271
struct _Instance *WARPGATE_UnHideCloudCoverInUnit(long streamUnitID)
{ //line: 2
	// register: $a3
	// size: 0x4C
	register struct Object *warpFaceObject;
	// register: $a2
	// size: 0x29C
	register struct _Instance *result;
	{ //line: 9
		// register: $v1
		// size: 0x29C
		register struct _Instance *instance;
		// register: $a1
		// size: 0x29C
		register struct _Instance *next;
	} //line: 27
} //line: 29


// path: C:\kain2\game\STREAM.C
// address: 0x8005F674
// line start: 5275
// line end:   5384
void STREAM_RenderWarpGate(unsigned long **mainOT, struct StreamUnitPortal *curStreamPortal, struct _StreamUnit *mainStreamUnit, struct RECT *cliprect)
{ //line: 1
	// register: $s0
	// size: 0x40
	register struct _StreamUnit *toStreamUnit;
} //line: 110


// path: C:\kain2\game\STREAM.C
// address: 0x8005F9AC
// line start: 5388
// line end:   5511
void WARPGATE_RenderWarpUnit(unsigned long **mainOT, struct StreamUnitPortal *curStreamPortal, struct _StreamUnit *mainStreamUnit, struct RECT *cliprect)
{ //line: 1
	// register: $s3
	register unsigned long **curOT;
	// register: $s0
	// size: 0xC
	register struct DR_AREA *PortalClip;
	// register: $s1
	// size: 0x40
	register struct _StreamUnit *toStreamUnit;
	{ //line: 34
		// address: -48
		// size: 0x8
		auto struct RECT PortalRect;
		// register: $s0
		register long portalFogColor;
		{ //line: 103
			// register: $a0
			register unsigned long *hld;
		} //line: 121
	} //line: 122
} //line: 124


// path: C:\kain2\game\STREAM.C
// address: 0x8005FD84
// line start: 5570
// line end:   5596
void STREAM_DumpNonResidentObjects()
{ //line: 1
	// register: $s0
	// size: 0x29C
	register struct _Instance *instance;
	{ //line: 8
		// register: $s2
		// size: 0x29C
		register struct _Instance *next;
		// register: $s1
		// size: 0x4C
		register struct Object *object;
	} //line: 23
} //line: 26


// path: C:\kain2\game\STREAM.C
// address: 0x8005FE34
// line start: 5602
// line end:   5662
int STREAM_TryAndDumpNonResident(struct _ObjectTracker *otr)
{ //line: 1
	// register: $s0
	// size: 0x29C
	register struct _Instance *instance;
	// register: $s1
	// size: 0x29C
	register struct _Instance *next;
} //line: 61


// path: C:\kain2\game\PHYSOBS.C
// address: 0x80068CA4
// line start: 208
// line end:   246
void PHYSOB_PlayDropSound(struct _Instance *instance)
{ //line: 1
	// register: $a1
	// size: 0x8
	register struct PhysObProperties *Prop;
	// register: $v1
	register int sound;
} //line: 39


// path: C:\kain2\game\PHYSOBS.C
// address: 0x80068DC4
// size: 0x29C
// line start: 259
// line end:   280
struct _Instance *PHYSOBS_IsAPushBlockAttached(struct _Instance *block)
{ //line: 1
	// register: $s0
	// size: 0x29C
	register struct _Instance *instance;
	// register: $s1
	// size: 0x29C
	register struct _Instance *next;
	// register: $s2
	// size: 0x29C
	register struct _Instance *result;
} //line: 21


// path: C:\kain2\game\PHYSOBS.C
// address: 0x80068E4C
// size: 0x29C
// line start: 284
// line end:   303
struct _Instance *PHYSOBS_IsAnythingAttached(struct _Instance *block)
{ //line: 2
	// register: $v1
	// size: 0x29C
	register struct _Instance *instance;
	// register: $a1
	// size: 0x29C
	register struct _Instance *next;
	// register: $a2
	// size: 0x29C
	register struct _Instance *result;
} //line: 19


// path: C:\kain2\game\PHYSOBS.C
// address: 0x80068E94
// line start: 307
// line end:   318
int CheckPhysOb(struct _Instance *instance)
{ //line: 2
	// register: $v0
	// size: 0x8
	register struct PhysObProperties *Prop;
} //line: 12


// path: C:\kain2\game\PHYSOBS.C
// address: 0x80068EC0
// line start: 322
// line end:   344
int CheckPhysObAbility(struct _Instance *instance, unsigned short ability)
{ //line: 1
	// register: $a2
	// size: 0x8
	register struct PhysObProperties *Prop;
	{ //line: 13
	} //line: 17
} //line: 23


// path: C:\kain2\game\PHYSOBS.C
// address: 0x80068F3C
// line start: 348
// line end:   364
int CheckPhysObFamily(struct _Instance *instance, unsigned short family)
{ //line: 2
	// register: $a0
	// size: 0x8
	register struct PhysObProperties *Prop;
} //line: 17


// path: C:\kain2\game\PHYSOBS.C
// address: 0x80068F80
// line start: 402
// line end:   421
int GetPhysicalAbility(struct _Instance *instance)
{ //line: 2
	// register: $a1
	// size: 0x8
	register struct PhysObProperties *Prop;
	{ //line: 11
	} //line: 14
} //line: 20


// path: C:\kain2\game\PHYSOBS.C
// address: 0x80068FD8
// line start: 426
// line end:   449
int AnyBlocksInMotion()
{ //line: 2
	// register: $a0
	// size: 0x29C
	register struct _Instance *instance;
	{ //line: 8
		// register: $v1
		// size: 0x8
		register struct PhysObProperties *Prop;
		// register: $v0
		// size: 0x48
		register struct PhysObData *Data;
	} //line: 19
} //line: 24


// path: C:\kain2\game\PHYSOBS.C
// address: 0x80069078
// line start: 456
// line end:   553
void SetThrowDirection(struct _Instance *instance, struct _Instance *parent, struct evObjectThrowData *throwData, struct PhysObData *Data)
{ //line: 1
	{ //line: 17
		// register: $a1
		register long val;
		// register: $s0
		// size: 0x29C
		register struct _Instance *itarget;
		// register: $t0
		// size: 0x20
		register struct MATRIX *matrix;
	} //line: 43
	{ //line: 51
		// register: $a1
		register long val;
		// register: $s0
		// size: 0x6
		register struct _Position *ptarget;
	} //line: 75
} //line: 98


// path: C:\kain2\game\PHYSOBS.C
// address: 0x80069394
// line start: 560
// line end:   724
void ThrowPhysOb(struct _Instance *instance, struct evObjectThrowData *throwData)
{ //line: 1
	// register: $s2
	// size: 0x48
	register struct PhysObData *Data;
	// register: $s4
	register int collFlg;
	// register: $s6
	register int endAnimFlg;
	{ //line: 10
		// register: $s5
		// size: 0x29C
		register struct _Instance *parent;
		// register: $s1
		// size: 0x8
		register struct PhysObProperties *Prop;
		{ //line: 47
			// register: $v1
			// size: 0x8
			register struct _SVector *sv;
		} //line: 53
		{ //line: 66
			// register: $s0
			// size: 0xC
			register struct __PhysObProjectileData *ProjData;
			{ //line: 83
			} //line: 86
		} //line: 90
		{ //line: 108
			// address: -96
			// size: 0x2C
			auto struct _PCollideInfo pcollideInfo;
			// address: -48
			// size: 0x6
			auto struct _Position newPos;
			// address: -40
			// size: 0x6
			auto struct _Position oldPos;
			// register: $v0
			// size: 0x20
			register struct MATRIX *mat;
			{ //line: 133
				// register: $a2
				register short _x0;
				// register: $a3
				register short _y0;
				// register: $t0
				register short _z0;
				// register: $v0
				register short _x1;
				// register: $v1
				register short _y1;
				// register: $a0
				register short _z1;
			} //line: 133
			{ //line: 133
				// register: $v0
				register short _x0;
				// register: $v1
				register short _y0;
				// register: $a0
				register short _z0;
				// register: $a1
				// size: 0x6
				register struct _Position *_v;
			} //line: 133
		} //line: 133
	} //line: 160
} //line: 165


// path: C:\kain2\game\PHYSOBS.C
// address: 0x80069774
// line start: 729
// line end:   780
int PushPhysOb(struct _Instance *instance, short x, short y, short PathNumber, struct _Instance *Force)
{ //line: 1
	// register: $s0
	// size: 0x48
	register struct PhysObData *Data;
	// register: $v1
	// size: 0x8
	register struct PhysObProperties *Prop;
	// register: $s2
	register int result;
} //line: 51


// path: C:\kain2\game\PHYSOBS.C
// address: 0x800698D0
// line start: 784
// line end:   804
void ResetSwitchPhysOb(struct _Instance *instance)
{ //line: 1
	// register: $v1
	// size: 0x48
	register struct PhysObData *Data;
	// register: $a1
	// size: 0x20
	register struct PhysObSwitchProperties *Prop;
} //line: 21


// path: C:\kain2\game\PHYSOBS.C
// address: 0x80069968
// line start: 809
// line end:   869
int SwitchPhysOb(struct _Instance *instance)
{ //line: 1
	// register: $v0
	// size: 0x48
	register struct PhysObData *Data;
	// register: $s0
	// size: 0x20
	register struct PhysObSwitchProperties *Prop;
	// register: $s3
	// size: 0x4
	register struct SwitchData *switchData;
} //line: 61


// path: C:\kain2\game\PHYSOBS.C
// address: 0x80069B44
// line start: 874
// line end:   984
int InteractPhysOb(struct _Instance *instance, struct _Instance *Force, int LinkNode, int Action)
{ //line: 1
	// register: $s5
	// size: 0x48
	register struct PhysObData *Data;
	// register: $s2
	// size: 0x2C
	register struct PhysObInteractProperties *interactProp;
	{ //line: 11
		// register: $s6
		// size: 0x4
		register struct BreakOffData *BreakOff;
		{ //line: 22
			// register: $s0
			// size: 0x29C
			register struct _Instance *lightInst;
		} //line: 28
		{ //line: 34
			// register: $s0
			// size: 0x29C
			register struct _Instance *lightInst;
		} //line: 43
		{ //line: 80
			// register: $s0
			register int s;
		} //line: 85
		{ //line: 85
			// register: $s0
			register int s;
		} //line: 91
	} //line: 109
} //line: 111


// path: C:\kain2\game\PHYSOBS.C
// address: 0x80069EA4
// line start: 985
// line end:   1148
void ResetOrientation(struct _Instance *instance)
{ //line: 1
	// address: -40
	// size: 0x8
	auto struct _G2EulerAngles_Type ea;
	// register: $a1
	// size: 0x48
	register struct PhysObData *Data;
	// address: -32
	// size: 0x6
	auto struct _G2SVector3_Type vec;
	// address: -24
	// size: 0x6
	auto struct _G2SVector3_Type vec2;
	// register: $a0
	register int dp;
	// register: $a2
	register int fixxy;
	// register: $a3
	register int fixz;
	// register: $v1
	register int dx;
	// register: $v0
	register int dx2;
	// register: $v0
	register int dy;
	// register: $v1
	register int dy2;
	// register: $v0
	register int dz;
	// register: $v1
	register int dz2;
	{ //line: 130
		// register: $v0
		register short _x0;
		// register: $v1
		register short _y0;
		// register: $a0
		register short _z0;
		// register: $a1
		register short _x1;
		// register: $a3
		register short _y1;
		// register: $a2
		register short _z1;
		// register: $a1
		// size: 0x6
		register struct _G2SVector3_Type *_v;
		// register: $a0
		// size: 0x8
		register struct _G2EulerAngles_Type *_v0;
		// register: $a2
		// size: 0x6
		register struct _G2SVector3_Type *_v1;
	} //line: 130
	{ //line: 132
		// register: $v1
		register short _x0;
		// register: $v0
		register short _y0;
		// register: $a0
		register short _z0;
		// register: $t0
		register short _x1;
		// register: $a2
		register short _y1;
		// register: $a1
		register short _z1;
		// register: $a3
		// size: 0x6
		register struct _G2SVector3_Type *_v;
		// register: $a1
		// size: 0x6
		register struct _G2SVector3_Type *_v1;
	} //line: 132
} //line: 156


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006A3C0
// line start: 1150
// line end:   1151
void FinishPush(struct _Instance *instance);

// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006A3E0
// line start: 1156
// line end:   1197
void PhysOb_AlignPush(struct _Instance *instance, int x, int y, int path, struct PhysObData *Data)
{ //line: 1
	// address: -96
	// size: 0x6
	auto struct _G2SVector3_Type vec;
	// register: $s2
	register int rotZ;
	// address: -88
	// size: 0x12
	auto short temp[3][3];
	// address: -64
	// size: 0x12
	auto short temp2[3][3];
	// address: -40
	// size: 0x8
	auto struct _G2EulerAngles_Type ea;
} //line: 29


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006A550
// line start: 1202
// line end:   1249
int FlipPhysOb(struct _Instance *instance, short x, short y, struct _Instance *Force)
{ //line: 1
	// register: $s0
	// size: 0x48
	register struct PhysObData *Data;
	// register: $v0
	// size: 0x8
	register struct PhysObProperties *Prop;
	// register: $s2
	register int result;
} //line: 47


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006A694
// line start: 1292
// line end:   1353
int CanBePickedUp(struct _Instance *instance, struct _Instance *Force, int LinkNode)
{ //line: 1
	{ //line: 24
		// address: -80
		// size: 0x2C
		auto struct _PCollideInfo pcollideInfo;
		// address: -32
		// size: 0x6
		auto struct _Position newPos;
		// address: -24
		// size: 0x6
		auto struct _Position oldPos;
		{ //line: 47
		} //line: 56
	} //line: 58
} //line: 62


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006A82C
// line start: 1357
// line end:   1409
int PickUpPhysOb(struct _Instance *instance, short Steps, struct _Instance *Force, int LinkNode)
{ //line: 1
	// register: $s0
	// size: 0x48
	register struct PhysObData *Data;
	{ //line: 41
		// register: $v0
		// size: 0x8
		register struct _PhysObLight *pLight;
	} //line: 47
} //line: 53


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006A92C
// size: 0x29C
// line start: 1439
// line end:   1465
struct _Instance *PHYSOB_BirthCollectible(struct _Instance *parent, int x, int y, int z, int type, int lifeTime)
{ //line: 1
	// register: $a1
	// size: 0x4C
	register struct Object *object;
	// register: $a2
	// size: 0x29C
	register struct _Instance *instance;
	{ //line: 14
	} //line: 23
} //line: 26


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006A9DC
// size: 0x10
// line start: 1469
// line end:   1476
struct evObjectBirthProjectileData *PHYSOB_BirthProjectile(struct _Instance *parent, int joint, int type)
{ //line: 1
	// register: $s0
	// size: 0x10
	register struct evObjectBirthProjectileData *rc;
} //line: 1


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006AA34
// size: 0x29C
// line start: 1480
// line end:   1573
struct _Instance *BirthProjectilePhysOb(struct _Instance *instance, int grabJoint, int type)
{ //line: 1
	// register: $a1
	// size: 0x4C
	register struct Object *ForceOb;
	// register: $s0
	// size: 0x29C
	register struct _Instance *iForce;
	// register: $s2
	// size: 0x48
	register struct PhysObData *Data;
	// register: $s1
	// size: 0x8
	register struct PhysObProperties *Prop;
	{ //line: 15
		// register: $v1
		// size: 0x10
		register struct __PhysObProjectileProperties *ProjProp;
		// register: $v1
		// size: 0xC
		register struct __PhysObProjectileData *ProjData;
		{ //line: 30
			// address: -32
			// size: 0x6
			auto struct _Position offset;
			// register: $v1
			// size: 0x2C
			register struct _FXForceFieldEffect *field;
		} //line: 38
		{ //line: 51
			// register: $v1
			// size: 0xC
			register struct __PhysObProjectileData *ProjData;
		} //line: 63
	} //line: 85
} //line: 94


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006AC08
// line start: 1602
// line end:   1627
void PHYSOB_SetLightTable(struct _PhysObLight *pLight, struct LightInstance *li, short burnAmplitude)
{ //line: 3
	// register: $a3
	register long idx;
	// register: $t2
	register long frac;
	// register: $t1
	// size: 0x8
	register struct LightTableEntry *ltable;
	// register: $a0
	// size: 0x8
	register struct LightTableEntry *ltable2;
} //line: 23


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006AD70
// line start: 1631
// line end:   1647
void PHYSOB_EndLighting(struct _Instance *instance, struct _PhysObLight *pLight)
{ //line: 4
	{ //line: 6
	} //line: 16
} //line: 17


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006AD94
// line start: 1650
// line end:   1680
void PHYSOB_StartLighting(struct _Instance *instance, struct _PhysObLight *pLight)
{ //line: 1
	// register: $a2
	// size: 0x48
	register struct PhysObData *Data;
	{ //line: 16
		// register: $a1
		// size: 0x14
		register struct LightInstance *li;
	} //line: 30
} //line: 31


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006AE00
// line start: 1683
// line end:   1699
void PHYSOB_StartBurnFX(struct _Instance *instance)
{ //line: 1
	// register: $v0
	// size: 0x48
	register struct PhysObData *Data;
	// register: $s1
	// size: 0x4C
	register struct Object *object;
	// register: $s0
	register int i;
} //line: 17


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006AE8C
// line start: 1702
// line end:   1705
void PHYSOB_StartBurning(struct _Instance *instance, struct _PhysObLight *pLight);

// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006AEC8
// line start: 1710
// line end:   1717
void PHYSOB_StopLighting(struct _Instance *instance, struct _PhysObLight *pLight)
{ //line: 2
	// register: $v0
	// size: 0x48
	register struct PhysObData *Data;
} //line: 2


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006AEE8
// line start: 1722
// line end:   1730
void PHYSOB_StopBurning(struct _Instance *instance, struct _PhysObLight *pLight)
{ //line: 2
	// register: $a0
	// size: 0x48
	register struct PhysObData *Data;
} //line: 4


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006AF0C
// line start: 1734
// line end:   1744
void PHYSOB_EndBurning(struct _Instance *instance, struct _PhysObLight *pLight)
{ //line: 1
	// register: $s1
	// size: 0x48
	register struct PhysObData *Data;
} //line: 1


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006AF6C
// line start: 1752
// line end:   1765
void StopPhysOb(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x48
	register struct PhysObData *Data;
} //line: 1


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006AFA8
// line start: 1775
// line end:   2161
void InitPhysicalObject(struct _Instance *instance, struct GameTracker *gameTracker)
{ //line: 1
	// register: $s3
	// size: 0x48
	register struct PhysObData *Data;
	// register: $s4
	// size: 0x8
	register struct PhysObProperties *Prop;
	{ //line: 9
		// register: $v0
		// size: 0x2C
		register struct PhysObInteractProperties *interactProp;
	} //line: 24
	{ //line: 178
		// register: $s0
		// size: 0x14
		register struct PhysObCollectibleProperties *collectibleProp;
	} //line: 197
	{ //line: 205
		// register: $s1
		// size: 0x2C
		register struct PhysObInteractProperties *interact;
		{ //line: 225
			// register: $s0
			register int s;
		} //line: 230
		{ //line: 230
			// register: $s0
			register int s;
		} //line: 236
	} //line: 262
	{ //line: 276
		// address: -80
		// size: 0x6
		auto struct _G2SVector3_Type vec;
		// address: -72
		// size: 0x20
		auto struct _G2Matrix_Type mat;
		// address: -40
		// size: 0x8
		auto struct _G2EulerAngles_Type ea;
	} //line: 276
	{ //line: 314
		// register: $s1
		// size: 0x4
		register struct SwitchData *switchData;
		// register: $s0
		// size: 0x20
		register struct PhysObSwitchProperties *switchProp;
	} //line: 338
	{ //line: 352
		// register: $a1
		// size: 0x8
		register struct _PhysObLight *pLight;
		{ //line: 355
			// register: $s0
			// size: 0x4
			register struct INICommand *index;
		} //line: 368
	} //line: 370
} //line: 387


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006B824
// line start: 2168
// line end:   2653
void ProcessPhysicalObject(struct _Instance *instance, struct GameTracker *gameTracker)
{ //line: 1
	// register: $s1
	// size: 0x48
	register struct PhysObData *Data;
	{ //line: 10
		// register: $a0
		// size: 0x8
		register struct _PhysObLight *pLight;
		{ //line: 17
			// register: $a1
			// size: 0x14
			register struct LightInstance *li;
		} //line: 23
	} //line: 24
	{ //line: 52
		// register: $v0
		// size: 0x24
		register struct PhysObWeaponProperties *properties;
		{ //line: 58
			// register: $a1
			// size: 0x8
			register struct _PhysObLight *pLight;
		} //line: 65
	} //line: 65
	{ //line: 82
	} //line: 102
	{ //line: 107
		// register: $v0
		// size: 0xC
		register struct PhysObAnimatedProperties *Prop;
	} //line: 115
	{ //line: 119
	} //line: 132
	{ //line: 186
		// register: $a2
		// size: 0x14
		register struct PhysObCollectibleProperties *collectibleProp;
	} //line: 193
	{ //line: 198
		// register: $s2
		// size: 0x188
		register struct Level *level;
		{ //line: 201
			// register: $a1
			// size: 0x8
			register struct _PhysObLight *pLight;
		} //line: 208
	} //line: 209
	{ //line: 213
		// register: $v1
		// size: 0x188
		register struct Level *level;
		{ //line: 221
			// register: $s2
			// size: 0x10
			register struct __PhysObProjectileProperties *ProjProp;
			// register: $v1
			// size: 0xC
			register struct __PhysObProjectileData *ProjData;
		} //line: 247
	} //line: 248
	{ //line: 254
		// register: $s2
		// size: 0xC
		register struct evPhysicsGravityData *gravityData;
		// register: $s3
		register int rc;
		// register: $a0
		register int upperOffset;
		// register: $a1
		register short lowerOffset;
		// register: $s4
		register int wasFalling;
		{ //line: 337
		} //line: 341
		{ //line: 368
		} //line: 372
		{ //line: 398
			// address: -96
			// size: 0x2C
			auto struct _PCollideInfo CInfo;
			// address: -48
			// size: 0x8
			auto struct SVECTOR Old;
			// address: -40
			// size: 0x8
			auto struct SVECTOR New;
			// register: $a2
			register short len;
			// register: $a3
			register long mult;
		} //line: 432
	} //line: 432
} //line: 486


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006C458
// line start: 2659
// line end:   2928
unsigned long PhysicalObjectQuery(struct _Instance *instance, unsigned long Query)
{ //line: 1
	// register: $s2
	// size: 0x48
	register struct PhysObData *Data;
	// register: $s1
	register int trueValue;
	{ //line: 29
	} //line: 38
	{ //line: 45
	} //line: 48
	{ //line: 53
		// address: -24
		// size: 0x8
		auto struct _G2EulerAngles_Type ea;
	} //line: 64
	{ //line: 73
		// register: $a0
		// size: 0x8
		register struct PhysObProperties *Prop;
		{ //line: 77
		} //line: 80
		{ //line: 82
		} //line: 85
		{ //line: 87
		} //line: 90
	} //line: 98
	{ //line: 108
		// register: $v0
		// size: 0x14
		register struct PhysObCollectibleProperties *collectibleProp;
	} //line: 111
	{ //line: 157
		// register: $t0
		// size: 0x14
		register struct PhysObDraftProperties *DraftProp;
		// register: $v0
		// size: 0x4
		register struct INICommand *index;
	} //line: 169
	{ //line: 177
		// register: $a1
		// size: 0x8
		register struct evControlSaveDataData *pdata;
		// register: $a2
		// size: 0x8
		register struct _PhysObSaveData *data;
	} //line: 198
	{ //line: 206
	} //line: 218
	{ //line: 221
		// register: $v0
		// size: 0x20
		register struct PhysObSwitchProperties *Prop;
		{ //line: 226
			// register: $v0
			// size: 0x48
			register struct PhysObData *Data;
		} //line: 238
	} //line: 239
	{ //line: 249
		// register: $v0
		// size: 0x48
		register struct PhysObData *physobData;
	} //line: 261
} //line: 270


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006C948
// line start: 2933
// line end:   3258
void PhysicalObjectPost(struct _Instance *instance, unsigned long Message, unsigned long Data)
{ //line: 1
	// register: $s2
	// size: 0x10
	register struct evObjectData *Ptr;
	// register: $s1
	// size: 0x48
	register struct PhysObData *PData;
	{ //line: 58
	} //line: 64
	{ //line: 84
		// register: $s1
		// size: 0x20
		register struct PhysObSwitchProperties *Prop;
		{ //line: 88
		} //line: 97
		{ //line: 104
			// register: $a0
			// size: 0x48
			register struct PhysObData *Data;
		} //line: 113
	} //line: 113
	{ //line: 121
		// register: $v0
		// size: 0x8
		register struct _PhysObSaveData *saveData;
		{ //line: 136
			// register: $a1
			// size: 0x8
			register struct _PhysObLight *pLight;
		} //line: 143
		{ //line: 146
			// register: $v1
			// size: 0x20
			register struct PhysObSwitchProperties *Prop;
		} //line: 162
		{ //line: 164
		} //line: 168
	} //line: 174
	{ //line: 177
	} //line: 184
	{ //line: 191
		// register: $s0
		// size: 0x14
		register struct PhysObCollectibleProperties *collectibleProp;
	} //line: 222
	{ //line: 224
	} //line: 236
	{ //line: 262
		// register: $a1
		// size: 0x8
		register struct _PhysObLight *pLight;
		{ //line: 276
			// register: $v1
			// size: 0x48
			register struct PhysObData *podata;
		} //line: 276
	} //line: 287
	{ //line: 296
	} //line: 297
	{ //line: 304
	} //line: 311
	{ //line: 315
		// register: $v0
		// size: 0x8
		register struct PhysObProperties *Prop;
	} //line: 321
} //line: 326


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006D238
// line start: 3261
// line end:   3284
long PhysobAnimCallback(struct _G2Anim_Type *anim, int sectionID, enum _G2AnimCallbackMsg_Enum_duplicate_e5 message, long messageDataA, long messageDataB, void *data)
{ //line: 1
	{ //line: 3
		// register: $s0
		// size: 0x48
		register struct PhysObData *Data;
	} //line: 20
} //line: 24


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006D2DC
// line start: 3296
// line end:   3389
void CheckForceCollision(struct _Instance *instance, struct _Instance *hitinst, struct _TFace *tface, struct _CollideInfo *collideInfo, struct _SVector *dir, int obliqueFlg)
{ //line: 1
	// register: $s3
	// size: 0x48
	register struct PhysObData *Data;
	{ //line: 7
		// register: $s1
		// size: 0x8
		register struct PhysObProperties *Prop;
		// register: $s0
		// size: 0x4
		register struct ProjectileData *ProjIData;
		// register: $s1
		// size: 0xC
		register struct __PhysObProjectileData *ProjData;
		// register: $s5
		register int killImmediately;
	} //line: 93
} //line: 94


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006D548
// line start: 3393
// line end:   3435
int GetObliqueDirection(struct _Instance *instance, struct _SVector *dir)
{ //line: 1
	// address: -8
	// size: 0x8
	auto struct _SVector vel;
	// register: $a3
	register int obliqueFlg;
	{ //line: 10
		// register: $a2
		register long axVel;
		// register: $v1
		register long ayVel;
	} //line: 34
} //line: 42


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006D624
// line start: 3440
// line end:   3721
void CollidePhysicalObject(struct _Instance *instance, struct GameTracker *gameTracker)
{ //line: 1
	// register: $s3
	// size: 0x48
	register struct PhysObData *Data;
	// register: $s2
	// size: 0x30
	register struct _CollideInfo *collideInfo;
	// address: -48
	auto int obliqueFlg;
	// address: -136
	// size: 0x8
	auto struct _SVector vel;
	// address: -128
	// size: 0x8
	auto struct _SVector dir;
	// register: $s5
	// size: 0x29C
	register struct _Instance *hitinst;
	// register: $s4
	// size: 0xC
	register struct _TFace *tface;
	// register: $s6
	// size: 0x8
	register struct _Normal *norm;
	// address: -120
	// size: 0x8
	auto struct _SVector normal;
	// register: $s0
	register int stickFlg;
	// register: $s7
	register int drawFlg;
	// register: $fp
	register int projectile;
	{ //line: 26
		// register: $v1
		// size: 0x188
		register struct Level *level;
	} //line: 36
	{ //line: 43
		// register: $s0
		// size: 0x24
		register struct BSPTree *bsp;
		// register: $a0
		// size: 0x188
		register struct Level *level;
	} //line: 50
	{ //line: 73
		// address: -112
		// size: 0x6
		auto struct _Position delta;
		{ //line: 75
			// register: $t0
			register short _x0;
			// register: $t1
			register short _y0;
			// register: $t2
			register short _z0;
			// register: $v0
			register short _x1;
			// register: $v1
			register short _y1;
			// register: $a2
			register short _z1;
			// register: $a1
			// size: 0x6
			register struct _Position *_v;
			// register: $v0
			// size: 0x6
			register struct _Position *_v0;
			// register: $a3
			// size: 0x6
			register struct _Position *_v1;
		} //line: 75
		{ //line: 75
			// register: $v0
			register short _x0;
			// register: $v1
			register short _y0;
			// register: $a2
			register short _z0;
		} //line: 75
	} //line: 81
	{ //line: 125
	} //line: 130
	{ //line: 139
		// address: -104
		// size: 0x2C
		auto struct _PCollideInfo CInfo;
		// address: -56
		// size: 0x8
		auto struct _SVector newPos;
	} //line: 158
	{ //line: 178
		// address: -112
		// size: 0x6
		auto struct _Position delta;
		{ //line: 178
			// register: $v0
			register short _x0;
			// register: $v1
			register short _y0;
			// register: $a2
			register short _z0;
			// register: $t1
			register short _y1;
			// register: $t2
			register short _z1;
			// register: $a3
			// size: 0x6
			register struct _Position *_v;
		} //line: 178
	} //line: 198
	{ //line: 201
	} //line: 207
	{ //line: 240
		// address: -112
		// size: 0x30
		auto struct _CollideInfo parentCI;
	} //line: 246
	{ //line: 254
		// register: $v0
		// size: 0x8
		register struct PhysObSplinter *splintDef;
		// register: $v1
		// size: 0x14
		register struct FXSplinter *splinterData;
	} //line: 277
} //line: 282


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006DDF8
// size: 0x1C
// line start: 3725
// line end:   3746
struct PhysObWeaponAttributes *PhysObGetWeapon(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1C
	register struct PhysObWeaponAttributes *weapon;
	{ //line: 7
		// register: $v0
		// size: 0x24
		register struct PhysObWeaponProperties *Prop;
	} //line: 9
	{ //line: 11
		// register: $v0
		// size: 0x2C
		register struct PhysObInteractProperties *Prop;
	} //line: 13
	{ //line: 15
		// register: $a0
		// size: 0x10
		register struct __PhysObProjectileProperties *Prop;
	} //line: 19
} //line: 20


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006DE9C
// size: 0x8
// line start: 3750
// line end:   3760
struct _PhysObLight *PhysObGetLight(struct _Instance *instance)
{ //line: 1
	// register: $v0
	// size: 0x1C
	register struct PhysObWeaponAttributes *weapon;
	// register: $s0
	// size: 0x8
	register struct _PhysObLight *pLight;
} //line: 10


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006DED4
// size: 0x8
// line start: 3764
// line end:   3774
struct PhysObSplinter *PhysObGetSplinter(struct _Instance *instance)
{ //line: 1
	// register: $v0
	// size: 0x1C
	register struct PhysObWeaponAttributes *weapon;
	// register: $s0
	// size: 0x8
	register struct PhysObSplinter *pSplinter;
} //line: 10


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006DF08
// line start: 3779
// line end:   3797
void TurnOnCollisionPhysOb(struct _Instance *instance, int coll)
{ //line: 1
	// register: $s0
	// size: 0x1C
	register struct PhysObWeaponAttributes *weapon;
	// register: $v1
	// size: 0x48
	register struct PhysObData *Data;
} //line: 19


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006DFAC
// line start: 3803
// line end:   3819
void TurnOffCollisionPhysOb(struct _Instance *instance, int coll)
{ //line: 1
	// register: $s0
	// size: 0x1C
	register struct PhysObWeaponAttributes *weapon;
} //line: 17


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006E034
// line start: 3827
// line end:   3936
int GetPhysObCollisionType(struct _Instance *instance)
{ //line: 1
	// register: $s4
	// size: 0x30
	register struct _CollideInfo *collideInfo;
	// register: $s0
	// size: 0x14
	register struct _HSphere *S;
	// register: $s2
	// size: 0x29C
	register struct _Instance *target;
	// register: $s0
	// size: 0x1C
	register struct PhysObWeaponAttributes *weapon;
	// register: $s5
	// size: 0x48
	register struct PhysObData *Data;
	{ //line: 20
		// register: $v1
		// size: 0x29C
		register struct _Instance *inst;
	} //line: 20
	{ //line: 88
		// register: $s3
		register int damage;
	} //line: 94
} //line: 110


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006E370
// line start: 3940
// line end:   3975
void ExecuteThrow(struct _Instance *instance)
{ //line: 1
	// register: $s2
	// size: 0x48
	register struct PhysObData *Data;
	{ //line: 9
		// register: $s0
		register short angley;
		// address: -32
		// size: 0x6
		auto struct _Position zero;
		// address: -24
		// size: 0x6
		auto struct _Position velocity;
	} //line: 10
} //line: 36


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006E480
// line start: 3980
// line end:   4014
void ExecuteDrag(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x48
	register struct PhysObData *Data;
} //line: 35


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006E528
// line start: 4019
// line end:   4056
void ExecuteSlideToStop(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x48
	register struct PhysObData *Data;
} //line: 38


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006E5E8
// line start: 4061
// line end:   4103
void ExecuteFlip(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x48
	register struct PhysObData *Data;
} //line: 43


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006E6D8
// line start: 4164
// line end:   4165
int CheckSlope(int Znormal, int Slope, int Slop);

// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006E6F8
// line start: 4173
// line end:   4208
int CheckBlockSlope(struct _Instance *instance, int x, int y, int hOff, int vOff, int dH, int dV)
{ //line: 1
	// address: -72
	// size: 0x2C
	auto struct _PCollideInfo CInfo;
	// address: -24
	// size: 0x8
	auto struct SVECTOR Old;
	// address: -16
	// size: 0x8
	auto struct SVECTOR New;
	// register: $t3
	register int xOff;
	// register: $t4
	register int yOff;
	// register: $t2
	register int dX;
	// register: $t6
	register int dY;
	// register: $v0
	// size: 0x20
	register struct MATRIX *mat;
} //line: 1


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006E7B8
// line start: 4216
// line end:   4247
int PHYSOBS_FigureDragForSlope(struct _Instance *instance, int pathNumber, int *result)
{ //line: 1
	// register: $s0
	register int didSomething;
} //line: 31


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006E838
// line start: 4251
// line end:   4421
int PHYSOB_CheckThrownLineCollision(struct _Instance *instance, struct _Instance *parent)
{ //line: 1
	// address: -160
	// size: 0x2C
	auto struct _PCollideInfo pcollideInfo;
	// address: -112
	// size: 0x6
	auto struct _Position newPos;
	// address: -104
	// size: 0x6
	auto struct _Position oldPos;
	// address: -96
	// size: 0x6
	auto struct _Position orgNew;
	// register: $v1
	// size: 0x20
	register struct MATRIX *mat;
	// register: $v0
	// size: 0x1C
	register struct PhysObWeaponAttributes *weapon;
	{ //line: 25
		// address: -88
		// size: 0x8
		auto struct _SVector line;
		// address: -80
		// size: 0x8
		auto struct _SVector delta;
		// register: $s0
		register long dp;
		{ //line: 57
			// register: $a0
			register short _x0;
			// register: $v0
			register short _y0;
			// register: $v1
			register short _z0;
			// register: $a1
			register short _y1;
			// register: $a2
			register short _z1;
			// register: $s1
			// size: 0x8
			register struct _SVector *_v;
			// register: $v1
			// size: 0x6
			register struct _Position *_v0;
			// register: $s2
			// size: 0x6
			register struct _Position *_v1;
		} //line: 57
		{ //line: 61
			// register: $v0
			register short _x0;
			// register: $v1
			register short _y0;
			// register: $a0
			register short _z0;
			// register: $a1
			register short _y1;
			// register: $a2
			register short _z1;
		} //line: 61
		{ //line: 71
			// register: $a2
			register short _x1;
			// register: $v1
			register short _y1;
			// register: $a3
			register short _z1;
			// register: $v0
			// size: 0x6
			register struct _Position *_v0;
			// register: $v0
			// size: 0x6
			register struct _Position *_v1;
		} //line: 71
	} //line: 83
	{ //line: 94
		// register: $a0
		// size: 0xC
		register struct _TFace *tface;
		// register: $a1
		// size: 0x188
		register struct Level *level;
	} //line: 100
	{ //line: 108
		// register: $t0
		register short _x0;
		// register: $t1
		register short _y0;
		// register: $t2
		register short _z0;
		// register: $v0
		register short _x1;
		// register: $v1
		register short _y1;
		// register: $a2
		register short _z1;
		// register: $a1
		// size: 0x6
		register struct _Position *_v;
		// register: $v0
		// size: 0x6
		register struct _Position *_v0;
	} //line: 108
	{ //line: 108
		// register: $v0
		register short _x0;
		// register: $v1
		register short _y0;
		// register: $a2
		register short _z0;
		// register: $a3
		// size: 0x6
		register struct _Position *_v;
	} //line: 108
	{ //line: 108
		// register: $s0
		// size: 0x48
		register struct PhysObData *Data;
		{ //line: 122
			// register: $v0
			// size: 0x8
			register struct PhysObSplinter *splintDef;
			// register: $v1
			// size: 0x14
			register struct FXSplinter *splinterData;
		} //line: 137
		{ //line: 140
			// address: -80
			// size: 0x8
			auto struct _SVector dir;
			// register: $s1
			// size: 0x29C
			register struct _Instance *hitInst;
			// register: $s2
			// size: 0xC
			register struct _TFace *tface;
			// address: -72
			// size: 0x30
			auto struct _CollideInfo collideInfo;
		} //line: 151
	} //line: 151
	{ //line: 159
		// register: $v0
		// size: 0x188
		register struct Level *level;
		// register: $a0
		register int bspID;
		// register: $v0
		// size: 0x24
		register struct BSPTree *bsp;
	} //line: 170
} //line: 171


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006ED30
// line start: 4428
// line end:   4557
int PHYSOB_CheckDroppedLineCollision(struct _Instance *instance, struct _Instance *parent)
{ //line: 1
	// address: -208
	// size: 0x2C
	auto struct _PCollideInfo pcollideInfo;
	// address: -160
	// size: 0x6
	auto struct _Position parentPos;
	// address: -152
	// size: 0x6
	auto struct _Position newPos;
	// address: -144
	// size: 0x6
	auto struct _Position oldPos;
	// address: -136
	// size: 0x8
	auto struct _SVector offset;
	// address: -128
	// size: 0x6
	auto struct _Position basePos;
	// register: $s2
	register int seg;
	// register: $s5
	register int collided;
	{ //line: 34
		// register: $v1
		// size: 0x20
		register struct MATRIX *mat;
		{ //line: 36
			// register: $v1
			register short _y0;
			// register: $a0
			register short _z0;
			// register: $t0
			register short _y1;
			// register: $t1
			register short _z1;
			// register: $s1
			// size: 0x8
			register struct _SVector *_v;
			// register: $s0
			// size: 0x6
			register struct _Position *_v0;
			// register: $s4
			// size: 0x6
			register struct _Position *_v1;
		} //line: 36
		{ //line: 47
			// register: $v1
			register short _y0;
			// register: $a1
			register short _z0;
		} //line: 47
	} //line: 63
	{ //line: 64
		// address: -120
		// size: 0x8
		auto struct _SVector origin;
		// address: -112
		// size: 0x8
		auto struct _SVector node2;
		// address: -104
		// size: 0x8
		auto struct _SVector orgDir;
		// address: -96
		// size: 0x8
		auto struct _SVector newDir;
		// address: -88
		// size: 0x8
		auto struct _SVector axis;
		// register: $s0
		register short angle;
		// address: -80
		// size: 0x20
		auto struct MATRIX mat;
		// address: -48
		// size: 0x8
		auto struct _G2EulerAngles_Type ea;
		{ //line: 64
			// register: $v0
			register short _y0;
			// register: $v1
			register short _z0;
			// register: $a3
			register short _y1;
			// register: $a1
			register short _z1;
			// register: $s0
			// size: 0x8
			register struct _SVector *_v;
			// register: $v1
			// size: 0x8
			register struct _SVector *_v0;
			// register: $a1
			// size: 0x8
			register struct _SVector *_v1;
		} //line: 64
		{ //line: 64
			// address: -40
			// size: 0x8
			auto struct _SVector delta;
			// register: $a0
			register int i;
			// register: $a2
			// size: 0x38
			register struct _Model *model;
			// register: $a1
			// size: 0x20
			register struct MATRIX *m;
		} //line: 114
	} //line: 115
} //line: 130


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006F1EC
// line start: 4562
// line end:   4580
int PHYSOB_CheckDirectedLineCollision(struct _Instance *instance, int xoffset, int yoffset, int startZOffset)
{ //line: 1
	// address: -72
	// size: 0x2C
	auto struct _PCollideInfo pcollideInfo;
	// address: -24
	// size: 0x6
	auto struct _Position newPos;
	// address: -16
	// size: 0x6
	auto struct _Position oldPos;
	// register: $v0
	// size: 0x20
	register struct MATRIX *mat;
} //line: 1


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006F260
// line start: 4583
// line end:   4603
long PHYSOBS_CheckForStackedForwardHits(struct _Instance *block, long xoffset, long yoffset)
{ //line: 1
	// register: $s0
	// size: 0x29C
	register struct _Instance *attachedBlock;
	// register: $s1
	register long result;
} //line: 20


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006F2DC
// line start: 4606
// line end:   4614
void PHYSOBS_SetNewAnim(struct _Instance *instance, struct PhysObData *Data, unsigned long modeBits, int path, int resetflg);

// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006F354
// line start: 4619
// line end:   4641
long PHYSOB_CheckForEnemyInBlkSpot(struct _Instance *instance, int dx, int dy)
{ //line: 1
	// register: $v0
	register int x;
	// register: $v1
	register int y;
	// register: $v0
	register int z;
	// register: $s6
	register int x0;
	// register: $s5
	register int x1;
	// register: $s4
	register int y0;
	// register: $s3
	register int y1;
	// register: $s2
	register int z0;
	// register: $s1
	register int z1;
	// register: $s0
	// size: 0x29C
	register struct _Instance *inst;
} //line: 23


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006F488
// line start: 4645
// line end:   5029
long PHYSOBS_CheckForValidMove(struct _Instance *instance)
{ //line: 1
	// register: $s1
	register int rc;
	// register: $s0
	// size: 0x48
	register struct PhysObData *Data;
	// address: -32
	// size: 0x28
	auto struct evPhysicsSlideData *Ptr;
	// address: -28
	auto int result;
	{ //line: 86
		// register: $v0
		register int Height;
	} //line: 99
	{ //line: 323
		// address: -24
		// size: 0x28
		auto struct evPhysicsSlideData *Ptr;
	} //line: 368
} //line: 384


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006F9D4
// line start: 5033
// line end:   5149
void ExecuteGravitate(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x48
	register struct PhysObData *Data;
	// register: $s4
	// size: 0x29C
	register struct _Instance *Force;
	// address: -48
	// size: 0x8
	auto struct _G2EulerAngles_Type ea;
	// address: -40
	// size: 0xC
	auto struct _Vector Position;
	// register: $s3
	// size: 0x38
	register struct _Model *model;
	// register: $s2
	// size: 0x20
	register struct MATRIX *forceMatrix;
	// register: $v1
	register int scale;
} //line: 117


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006FD48
// line start: 5153
// line end:   5163
void ExecuteFollow(struct _Instance *instance)
{ //line: 2
	// register: $a1
	// size: 0x48
	register struct PhysObData *Data;
	// register: $a2
	// size: 0x29C
	register struct _Instance *Force;
} //line: 5


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006FDA0
// line start: 5175
// line end:   5185
void PHYSOB_Normalize(struct _SVector *v)
{ //line: 1
	// register: $a1
	register long len;
} //line: 8


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8006FE3C
// line start: 5190
// line end:   5249
int PHYSOB_MoveTowardsAlign(struct _Instance *instance, struct _SVector *orgVec, struct _SVector *endVec)
{ //line: 1
	// address: -104
	// size: 0x20
	auto struct MATRIX xform;
	// address: -72
	// size: 0x20
	auto struct MATRIX dest;
	// register: $s4
	// size: 0x20
	register struct MATRIX *src;
	// address: -40
	// size: 0x8
	auto struct _G2Quat_Type rot;
	// register: $s0
	register long len;
	// register: $s1
	register int theta;
	// register: $v0
	register int sintheta;
	// address: -32
	// size: 0x8
	auto struct _G2EulerAngles_Type ea;
} //line: 45


// path: C:\kain2\game\PHYSOBS.C
// address: 0x80070114
// line start: 5253
// line end:   5480
int PHYSOB_ReAlignFalling(struct _Instance *instance, int zEndOff)
{ //line: 1
	// register: $s6
	// size: 0x38
	register struct _Model *model;
	// register: $s2
	register int i;
	// address: -208
	// size: 0x6
	auto struct _Position seg0;
	// address: -200
	// size: 0x6
	auto struct _Position segn;
	// address: -192
	// size: 0x6
	auto struct _Position start0;
	// address: -184
	// size: 0x6
	auto struct _Position startn;
	// register: $s3
	register int startFlg;
	// register: $s7
	register short zOff;
	{ //line: 52
		// address: -176
		// size: 0x2C
		auto struct _PCollideInfo pcollideInfo;
		// address: -128
		// size: 0x6
		auto struct _Position newPos;
		// address: -120
		// size: 0x6
		auto struct _Position oldPos;
		// register: $s1
		register short x;
		// register: $s0
		register short y;
		// register: $v0
		// size: 0x20
		register struct MATRIX *mat;
		{ //line: 71
			// register: $a1
			register short ax;
			// register: $a2
			register short ay;
		} //line: 101
		{ //line: 109
		} //line: 137
	} //line: 139
	{ //line: 146
		// register: $a3
		register short _x0;
		// register: $v0
		register short _y0;
		// register: $v1
		register short _z0;
		// register: $a1
		register short _x1;
		// register: $t0
		register short _y1;
		// register: $a2
		register short _z1;
		// register: $a0
		// size: 0x6
		register struct _Position *_v;
		// register: $a2
		// size: 0x6
		register struct _Position *_v1;
	} //line: 146
	{ //line: 146
		// register: $v0
		register short _x0;
		// register: $v1
		register short _y0;
		// register: $a0
		register short _z0;
		// register: $a2
		register short _x1;
		// register: $t1
		register short _y1;
		// register: $t0
		register short _z1;
		// register: $a1
		// size: 0x6
		register struct _Position *_v;
		// register: $t0
		// size: 0x6
		register struct _Position *_v1;
	} //line: 146
	{ //line: 168
		// address: -112
		// size: 0x2C
		auto struct _PCollideInfo pcollideInfo;
		// address: -64
		// size: 0x6
		auto struct _Position newPos;
		// address: -56
		// size: 0x6
		auto struct _Position oldPos;
		// address: -48
		// size: 0x6
		auto struct _Position deltaPos;
		// register: $a2
		// size: 0x20
		register struct MATRIX *mat;
	} //line: 224
} //line: 228


// path: C:\kain2\game\PHYSOBS.C
// address: 0x80070638
// line start: 5484
// line end:   5549
void DropPhysOb(struct _Instance *instance, int flags)
{ //line: 1
	// register: $s0
	// size: 0x48
	register struct PhysObData *Data;
	// register: $s2
	// size: 0x29C
	register struct _Instance *parent;
} //line: 61


// path: C:\kain2\game\PHYSOBS.C
// address: 0x80070734
// line start: 5557
// line end:   5613
void PhysicalRelocateTune(struct Object *object, long offset)
{ //line: 2
	// register: $t0
	// size: 0x24
	register struct PhysObWeaponProperties *properties;
	// register: $v0
	// size: 0x8
	register struct _PhysObLight *pLight;
	{ //line: 19
		// register: $a2
		// size: 0x2C
		register struct PhysObInteractProperties *prop;
		// register: $a3
		// size: 0x1C
		register struct PhysObWeaponAttributes *weapon;
	} //line: 32
	{ //line: 34
		// register: $a0
		// size: 0x10
		register struct __PhysObProjectileProperties *prop;
		{ //line: 40
			// register: $t0
			register int i;
		} //line: 54
	} //line: 55
} //line: 57


// path: C:\kain2\game\PHYSOBS.C
// address: 0x800708CC
// line start: 5628
// line end:   5644
int PHYSOBS_CheckObjectAxisAlignment(struct MATRIX *m0, struct MATRIX *m1, struct _SVector *axis)
{ //line: 1
	// address: -64
	// size: 0x10
	auto struct VECTOR r0;
	// address: -48
	// size: 0x10
	auto struct VECTOR r1;
	// address: -32
	// size: 0x8
	auto struct _SVector v0;
	// address: -24
	// size: 0x8
	auto struct _SVector v1;
} //line: 1


// path: C:\kain2\game\PHYSOBS.C
// address: 0x8007099C
// line start: 5649
// line end:   5653
int PHYSOB_CheckSlide(struct _Instance *instance, int x, int y, struct evPhysicsSlideData **data);

// path: C:\kain2\game\PHYSOBS.C
// address: 0x80070A2C
// line start: 5659
// line end:   5663
int PHYSOB_CheckSlide2(struct _Instance *instance, int x, int y, struct evPhysicsSlideData **data);

// path: C:\kain2\game\PHYSOBS.C
// address: 0x80070ABC
// line start: 5668
// line end:   5672
int PHYSOB_CheckDropOnSlope(struct _Instance *instance, int x, int y, struct evPhysicsSlideData **data);

// path: C:\kain2\game\PHYSOBS.C
// address: 0x80070B50
// line start: 5678
// line end:   5680
int PHYSOB_CheckSlideNormal(struct _Instance *instance, int x, int y, struct evPhysicsSlideData **data);

// path: C:\kain2\game\PHYSOBS.C
// address: 0x80070BA4
// line start: 5686
// line end:   5701
void PHYSOB_NormalToAxis(short *x, short *y)
{ //line: 2
	// register: $v1
	register int x1;
	// register: $a2
	register int y1;
} //line: 16


// path: C:\kain2\game\RAZCNTRL.C
// address: 0x80070C08
// line start: 67
// line end:   84
void ProcessRazControl(long *command);

// path: C:\kain2\game\STATE.C
// address: 0x80070C90
// line start: 88
// line end:   89
void InitMessageQueue(struct __MessageQueue *In);

// path: C:\kain2\game\STATE.C
// address: 0x80070C9C
// size: 0x8
// line start: 95
// line end:   108
struct __Event *DeMessageQueue(struct __MessageQueue *In)
{ //line: 2
} //line: 14


// path: C:\kain2\game\STATE.C
// address: 0x80070CD8
// line start: 157
// line end:   158
void PurgeMessageQueue(struct __MessageQueue *In);

// path: C:\kain2\game\STATE.C
// address: 0x80070CE4
// size: 0x8
// line start: 164
// line end:   170
struct __Event *PeekMessageQueue(struct __MessageQueue *In);

// path: C:\kain2\game\STATE.C
// address: 0x80070D0C
// line start: 197
// line end:   198
void EnMessageQueue(struct __MessageQueue *In, struct __Event *Element);

// path: C:\kain2\game\STATE.C
// address: 0x80070D38
// line start: 204
// line end:   250
void EnMessageQueueData(struct __MessageQueue *In, int ID, int Data)
{ //line: 2
	// register: $v1
	register int i;
} //line: 47


// path: C:\kain2\game\STATE.C
// address: 0x80070DB4
// line start: 268
// line end:   287
void *CIRC_Alloc(int size)
{ //line: 2
	// register: $v1
	register void *ret;
} //line: 19


// path: C:\kain2\game\STATE.C
// address: 0x80070DF8
// line start: 298
// line end:   304
int SetCollideInfoData(struct _CollideInfo *srcCI)
{ //line: 1
	// register: $v0
	// size: 0x30
	register struct _CollideInfo *Ptr;
} //line: 7


// path: C:\kain2\game\STATE.C
// address: 0x80070E54
// line start: 315
// line end:   325
int SetMonsterHitData(struct _Instance *Sender, struct _Instance *lastHit, int Power, int knockBackDistance, int knockBackFrames)
{ //line: 1
	// register: $v0
	// size: 0x10
	register struct evMonsterHitData *Ptr;
} //line: 1


// path: C:\kain2\game\STATE.C
// address: 0x80070EC0
// line start: 332
// line end:   342
int SetMonsterThrownData(struct _Instance *Sender, struct _Rotation *Direction, int Power)
{ //line: 1
	// register: $v0
	// size: 0x10
	register struct evMonsterThrownData *Ptr;
} //line: 1


// path: C:\kain2\game\STATE.C
// address: 0x80070F28
// line start: 348
// line end:   357
int SetMonsterAlarmData(struct _Instance *sender, struct _Position *position, int type)
{ //line: 1
	// register: $v0
	// size: 0xC
	register struct evMonsterAlarmData *Ptr;
} //line: 1


// path: C:\kain2\game\STATE.C
// address: 0x80070F90
// line start: 365
// line end:   374
int SetMonsterSoulSuckData(struct _Instance *Sender, int x, int y, int z)
{ //line: 1
	// register: $v0
	// size: 0xC
	register struct evMonsterSoulSuckData *Ptr;
} //line: 1


// path: C:\kain2\game\STATE.C
// address: 0x80070FEC
// line start: 382
// line end:   395
int SetMonsterImpaleData(struct _Instance *weapon, struct _Rotation *direction, struct _Position *position, int distance)
{ //line: 1
	// register: $v0
	// size: 0x14
	register struct evMonsterImpaleData *Ptr;
} //line: 1


// path: C:\kain2\game\STATE.C
// address: 0x80071084
// line start: 403
// line end:   413
int SetObjectData(int x, int y, int PathNumber, struct _Instance *Force, int node)
{ //line: 1
	// register: $v0
	// size: 0x10
	register struct evObjectData *Ptr;
} //line: 1


// path: C:\kain2\game\STATE.C
// address: 0x800710F0
// line start: 420
// line end:   428
int SetPositionData(int x, int y, int z)
{ //line: 1
	// register: $v0
	// size: 0x8
	register struct evPositionData *Ptr;
} //line: 1


// path: C:\kain2\game\STATE.C
// address: 0x8007113C
// line start: 438
// line end:   449
int SetPhysicsGravityData(int UpperOffset, int LowerOffset, int x, int y, int z, int slope)
{ //line: 1
	// register: $v0
	// size: 0xC
	register struct evPhysicsGravityData *Ptr;
} //line: 1


// path: C:\kain2\game\STATE.C
// address: 0x800711B8
// line start: 462
// line end:   476
int SetPhysicsEdgeData(int UpperOffset, int ForwardOffset, int AboveOffset, int x, int y, int z, struct _SVector *Normal1, struct _SVector *Normal2, struct _SVector *Delta)
{ //line: 1
	// register: $v0
	// size: 0x24
	register struct evPhysicsEdgeData *Ptr;
} //line: 1


// path: C:\kain2\game\STATE.C
// address: 0x80071264
// line start: 485
// line end:   495
int SetPhysicsSwimData(int CheckDepth, struct _SVector *iVelocity, int SwimDepth, int WadeDepth, int TreadDepth)
{ //line: 1
	// register: $v0
	// size: 0x18
	register struct evPhysicsSwimData *Ptr;
} //line: 1


// path: C:\kain2\game\STATE.C
// address: 0x800712D0
// line start: 503
// line end:   513
int SetPhysicsDropOffData(int xOffset, int yOffset, int DropOffset, int slipSlope, int UpperOffset)
{ //line: 1
	// register: $v0
	// size: 0xC
	register struct evPhysicsDropOffData *Ptr;
} //line: 1


// path: C:\kain2\game\STATE.C
// address: 0x8007133C
// line start: 523
// line end:   535
int SetPhysicsSlideData(int Segment, int ForwardVectorX, int ForwardVectorY, int ForwardVectorZ, int DropOffset, int UpperOffset, int Height)
{ //line: 1
	// register: $v0
	// size: 0x28
	register struct evPhysicsSlideData *Ptr;
} //line: 1


// path: C:\kain2\game\STATE.C
// address: 0x800713C8
// line start: 544
// line end:   553
int SetPhysicsWallCrawlData(int Segment, int Length, int ForwardOffset, int NormalDistance)
{ //line: 1
	// register: $v0
	// size: 0x2C
	register struct evPhysicsWallCrawlData *Ptr;
} //line: 1


// path: C:\kain2\game\STATE.C
// address: 0x80071424
// line start: 561
// line end:   573
int SetPhysicsLinkedMoveData(struct _Instance *instance, int segment, struct _SVector *posDelta, struct _SVector *rotDelta)
{ //line: 1
	// register: $v0
	// size: 0x18
	register struct evPhysicsLinkedMoveData *Ptr;
} //line: 13


// path: C:\kain2\game\STATE.C
// address: 0x800714C4
// line start: 582
// line end:   592
int SetPhysicsDropHeightData(struct _Position *offset, int dropOffset, int mode)
{ //line: 1
	// register: $v0
	// size: 0x14
	register struct evPhysicsDropHeightData *ptr;
} //line: 1


// path: C:\kain2\game\STATE.C
// address: 0x80071534
// line start: 602
// line end:   611
int SetAnimationControllerDoneData(struct _Instance *instance, long segment, long type, int data)
{ //line: 1
	// register: $v0
	// size: 0x10
	register struct evAnimationControllerDoneData *Ptr;
} //line: 1


// path: C:\kain2\game\STATE.C
// address: 0x80071590
// line start: 620
// line end:   630
int SetAnimationInstanceSwitchData(struct _Instance *instance, int anim, int frame, int frames, int mode)
{ //line: 1
	// register: $v0
	// size: 0x14
	register struct evAnimationInstanceSwitchData *Ptr;
} //line: 1


// path: C:\kain2\game\STATE.C
// address: 0x800715FC
// line start: 639
// line end:   656
int SetFXHitData(struct _Instance *hitter, int segment, int amount, int type)
{ //line: 1
	// register: $a1
	// size: 0x14
	register struct evFXHitData *Ptr;
} //line: 17


// path: C:\kain2\game\STATE.C
// address: 0x800716F8
// line start: 693
// line end:   746
int SetObjectThrowData(void *target, struct _SVector *angularVel, unsigned short type, unsigned short spinType, int speed, int gravity, int zVel, int initialXRot)
{ //line: 1
	// register: $a0
	// size: 0x1C
	register struct evObjectThrowData *Ptr;
} //line: 53


// path: C:\kain2\game\STATE.C
// address: 0x80071864
// line start: 750
// line end:   763
int SetObjectBreakOffData(struct _Instance *force, short node, short distance, short animation, int frame, int type, int action)
{ //line: 1
	// register: $v0
	// size: 0x10
	register struct evObjectBreakOffData *Ptr;
} //line: 1


// path: C:\kain2\game\STATE.C
// address: 0x800718F0
// line start: 767
// line end:   777
int SetControlInitHangData(struct _Instance *instance, long frame, long frames)
{ //line: 1
	// register: $v0
	// size: 0xC
	register struct evControlInitHangData *Ptr;
} //line: 1


// path: C:\kain2\game\STATE.C
// address: 0x8007193C
// line start: 781
// line end:   791
int SetControlInitIdleData(int mode, long frame, long frames)
{ //line: 1
	// register: $v0
	// size: 0xC
	register struct evControlInitIdleData *Ptr;
} //line: 1


// path: C:\kain2\game\STATE.C
// address: 0x80071988
// line start: 795
// line end:   807
int SetObjectDraftData(short force, unsigned short radius, unsigned short radiusCoef, unsigned short height, int maxVelocity)
{ //line: 1
	// register: $v0
	// size: 0xA
	register struct evObjectDraftData *Ptr;
} //line: 1


// path: C:\kain2\game\STATE.C
// address: 0x800719F4
// line start: 811
// line end:   821
int SetObjectAbsorbData(struct _Instance *force, unsigned short node, unsigned short steps)
{ //line: 1
	// register: $v0
	// size: 0x8
	register struct evObjectAbsorbData *Ptr;
} //line: 1


// path: C:\kain2\game\STATE.C
// address: 0x80071A40
// line start: 827
// line end:   836
int SetControlSaveDataData(long length, void *data)
{ //line: 1
	// register: $v0
	// size: 0x8
	register struct evControlSaveDataData *Ptr;
} //line: 1


// path: C:\kain2\game\STATE.C
// address: 0x80071A7C
// line start: 840
// line end:   849
int SetObjectIdleData(long mode, struct _Instance *instance)
{ //line: 1
	// register: $v0
	// size: 0x8
	register struct evObjectIdleData *Ptr;
} //line: 1


// path: C:\kain2\game\STATE.C
// address: 0x80071AB8
// line start: 869
// line end:   883
int SetActionPlayHostAnimationData(struct _Instance *instance, struct _Instance *host, int newAnim, int newFrame, int frames, int mode)
{ //line: 1
	// register: $v0
	// size: 0x18
	register struct evActionPlayHostAnimationData *Ptr;
} //line: 1


// path: C:\kain2\game\STATE.C
// address: 0x80071B34
// line start: 888
// line end:   901
int SetObjectBirthProjectileData(struct _Instance *instance, int joint, int type)
{ //line: 1
	// register: $v0
	// size: 0x10
	register struct evObjectBirthProjectileData *Ptr;
} //line: 1


// path: C:\kain2\game\STATE.C
// address: 0x80071B84
// line start: 907
// line end:   916
int SetShadowSegmentData(unsigned long total)
{ //line: 1
	// register: $v0
	// size: 0xC
	register struct evShadowSegmentData *Ptr;
} //line: 1


// path: C:\kain2\game\STATE.C
// address: 0x80071BB0
// line start: 921
// line end:   971
void G2EmulationInit();

// path: C:\kain2\game\STATE.C
// address: 0x80071D9C
// line start: 978
// line end:   989
void G2EmulationInstancePlayAnimation(struct _Instance *instance)
{ //line: 1
	// register: $s0
	register int i;
	// register: $s1
	// size: 0xB4
	register struct _G2Anim_Type *anim;
} //line: 12


// path: C:\kain2\game\STATE.C
// address: 0x80071E04
// line start: 995
// line end:   999
void G2EmulatePlayAnimation(struct __CharacterState *In);

// path: C:\kain2\game\STATE.C
// address: 0x80071E28
// line start: 1006
// line end:   1027
void G2EmulationInstanceToInstanceSwitchAnimation(struct _Instance *instance, struct _Instance *host, int CurrentSection, int NewAnim, int NewFrame, int Frames, int Mode)
{ //line: 1
	// register: $s4
	// size: 0x30
	register struct _G2AnimSection_Type *animSection;
	// register: $s1
	// size: 0x14
	register struct _G2AnimKeylist_Type *keylist;
} //line: 22


// path: C:\kain2\game\STATE.C
// address: 0x80071F20
// line start: 1032
// line end:   1053
void G2EmulationInstanceSwitchAnimation(struct _Instance *instance, int CurrentSection, int NewAnim, int NewFrame, int Frames, int Mode)
{ //line: 1
	// register: $s4
	// size: 0x30
	register struct _G2AnimSection_Type *animSection;
	// register: $s1
	// size: 0x14
	register struct _G2AnimKeylist_Type *keylist;
} //line: 22


// path: C:\kain2\game\STATE.C
// address: 0x80072014
// line start: 1057
// line end:   1063
void G2EmulationInstanceSwitchAnimationAlpha(struct _Instance *instance, int CurrentSection, int NewAnim, int NewFrame, int Frames, int Mode, int AlphaTable)
{ //line: 1
	// register: $s1
	// size: 0x30
	register struct _G2AnimSection_Type *animSection;
} //line: 1


// path: C:\kain2\game\STATE.C
// address: 0x80072080
// line start: 1067
// line end:   1076
void G2EmulationSwitchAnimation(struct __CharacterState *In, int CurrentSection, int NewAnim, int NewFrame, int Frames, int Mode);

// path: C:\kain2\game\STATE.C
// address: 0x800720C4
// line start: 1081
// line end:   1087
void G2EmulationSwitchAnimationAlpha(struct __CharacterState *In, int CurrentSection, int NewAnim, int NewFrame, int Frames, int Mode, int AlphaTable)
{ //line: 1
	// register: $s1
	// size: 0x30
	register struct _G2AnimSection_Type *animSection;
} //line: 1


// path: C:\kain2\game\STATE.C
// address: 0x80072138
// line start: 1093
// line end:   1132
void G2EmulationSwitchAnimationSync(struct __CharacterState *In, int SlaveSectionID, int MasterSectionID, int Frames)
{ //line: 1
	// register: $a1
	// size: 0x29C
	register struct _Instance *instance;
	// register: $s6
	// size: 0x30
	register struct _G2AnimSection_Type *masterAnimSection;
	// register: $s5
	// size: 0x30
	register struct _G2AnimSection_Type *slaveAnimSection;
	// register: $s1
	// size: 0x14
	register struct _G2AnimKeylist_Type *keylist;
	// register: $s3
	register int keylistID;
	// register: $s7
	// size: 0x11C
	register struct __State *masterSection;
	// register: $s2
	// size: 0x11C
	register struct __State *slaveSection;
} //line: 37


// path: C:\kain2\game\STATE.C
// address: 0x80072304
// line start: 1137
// line end:   1159
void G2EmulationInstanceToInstanceSwitchAnimationCharacter(struct _Instance *instance, struct _Instance *host, int NewAnim, int NewFrame, int Frames, int Mode)
{ //line: 1
	// register: $s0
	// size: 0x14
	register struct _G2AnimKeylist_Type *keylist1;
} //line: 23


// path: C:\kain2\game\STATE.C
// address: 0x800723F0
// line start: 1164
// line end:   1191
void G2EmulationSwitchAnimationCharacter(struct __CharacterState *In, int NewAnim, int NewFrame, int Frames, int Mode)
{ //line: 1
	// register: $s0
	// size: 0x29C
	register struct _Instance *instance;
	// register: $s0
	// size: 0x14
	register struct _G2AnimKeylist_Type *keylist;
} //line: 28


// path: C:\kain2\game\STATE.C
// address: 0x800724E4
// line start: 1203
// line end:   1213
void G2EmulationInstanceSetAnimation(struct _Instance *instance, int CurrentSection, int NewAnim, int NewFrame, int Frames)
{ //line: 1
	// register: $s2
	// size: 0x30
	register struct _G2AnimSection_Type *animSection;
	// register: $s1
	// size: 0x14
	register struct _G2AnimKeylist_Type *keylist;
} //line: 1


// path: C:\kain2\game\STATE.C
// address: 0x8007258C
// line start: 1217
// line end:   1226
void G2EmulationSetAnimation(struct __CharacterState *In, int CurrentSection, int NewAnim, int NewFrame, int Frames);

// path: C:\kain2\game\STATE.C
// address: 0x800725C4
// line start: 1232
// line end:   1248
void G2EmulationInstanceSetMode(struct _Instance *instance, int CurrentSection, int Mode)
{ //line: 1
	// register: $s0
	// size: 0x30
	register struct _G2AnimSection_Type *animSection;
} //line: 17


// path: C:\kain2\game\STATE.C
// address: 0x80072644
// line start: 1253
// line end:   1255
void G2EmulationSetMode(struct __CharacterState *In, int CurrentSection, int Mode);

// path: C:\kain2\game\STATE.C
// address: 0x80072668
// line start: 1262
// line end:   1267
void G2EmulationInstanceSetAnimSpeed(struct _Instance *instance, int CurrentSection, int speed)
{ //line: 2
	// register: $v0
	// size: 0x30
	register struct _G2AnimSection_Type *animSection;
} //line: 4


// path: C:\kain2\game\STATE.C
// address: 0x80072684
// line start: 1272
// line end:   1278
int G2EmulationInstanceQueryAnimation(struct _Instance *instance, int CurrentSection)
{ //line: 3
	// register: $v0
	// size: 0x30
	register struct _G2AnimSection_Type *animSection;
} //line: 5


// path: C:\kain2\game\STATE.C
// address: 0x800726A4
// line start: 1283
// line end:   1285
int G2EmulationQueryAnimation(struct __CharacterState *In, int CurrentSection);

// path: C:\kain2\game\STATE.C
// address: 0x800726C8
// line start: 1291
// line end:   1297
int G2EmulationInstanceQueryFrame(struct _Instance *instance, int CurrentSection)
{ //line: 1
} //line: 1


// path: C:\kain2\game\STATE.C
// address: 0x800726FC
// line start: 1303
// line end:   1308
int G2EmulationInstanceQueryLastFrame(struct _Instance *instance, int CurrentSection)
{ //line: 1
} //line: 1


// path: C:\kain2\game\STATE.C
// address: 0x80072730
// line start: 1314
// line end:   1328
int G2EmulationInstanceQueryPassedFrame(struct _Instance *instance, int CurrentSection, int frame)
{ //line: 1
	// register: $s0
	// size: 0x30
	register struct _G2AnimSection_Type *animSection;
} //line: 15


// path: C:\kain2\game\STATE.C
// address: 0x800727D8
// line start: 1333
// line end:   1335
int G2EmulationQueryFrame(struct __CharacterState *In, int CurrentSection);

// path: C:\kain2\game\STATE.C
// address: 0x800727FC
// line start: 1341
// line end:   1355
int G2EmulationInstanceQueryMode(struct _Instance *instance, int CurrentSection)
{ //line: 3
	// register: $v0
	// size: 0x30
	register struct _G2AnimSection_Type *animSection;
} //line: 15


// path: C:\kain2\game\STATE.C
// address: 0x80072844
// line start: 1360
// line end:   1362
int G2EmulationQueryMode(struct __CharacterState *In, int CurrentSection);

// path: C:\kain2\game\STATE.C
// address: 0x80072868
// line start: 1368
// line end:   1374
void G2EmulationInstanceSetStartAndEndSegment(struct _Instance *instance, int CurrentSection, short Start, short End)
{ //line: 3
	// register: $a0
	// size: 0x30
	register struct _G2AnimSection_Type *animSection;
} //line: 5


// path: C:\kain2\game\STATE.C
// address: 0x80072894
// line start: 1379
// line end:   1381
void G2EmulationSetStartAndEndSegment(struct __CharacterState *In, int CurrentSection, short Start, short End);

// path: C:\kain2\game\STATE.C
// address: 0x800728C4
// line start: 1387
// line end:   1399
void G2EmulationInstanceSetTotalSections(struct _Instance *instance, short Total)
{ //line: 1
	// register: $s0
	// size: 0xB4
	register struct _G2Anim_Type *anim;
} //line: 10


// path: C:\kain2\game\STATE.C
// address: 0x80072948
// line start: 1404
// line end:   1408
void G2EmulationSetTotalSections(struct __CharacterState *In, short Total);

// path: C:\kain2\game\STATE.C
// address: 0x80072978
// line start: 1414
// line end:   1420
void G2EmulationInstanceInitSection(struct _Instance *instance, int CurrentSection, void *callback, void *data)
{ //line: 3
	// register: $a0
	// size: 0x30
	register struct _G2AnimSection_Type *animSection;
} //line: 5


// path: C:\kain2\game\STATE.C
// address: 0x8007299C
// line start: 1440
// line end:   1451
void G2EmulationSetInterpController_Vector(struct _Instance *instance, long segment, long type, struct _G2SVector3_Type *vec, int Frames, int Data);

// path: C:\kain2\game\STATE.C
// address: 0x80072A24
// line start: 1456
// line end:   1464
void StateSwitchStateDataDefault(struct __CharacterState *In, int CurrentSection, void (*NewProcess)(), int Data)
{ //line: 1
	// register: $s2
	register void (*process)();
} //line: 1


// path: C:\kain2\game\STATE.C
// address: 0x80072A8C
// line start: 1465
// line end:   1473
void StateSwitchStateCharacterDataDefault(struct __CharacterState *In, void (*NewProcess)(), int Data)
{ //line: 1
	// register: $s0
	register int i;
} //line: 9


// path: C:\kain2\game\STATE.C
// address: 0x80072B04
// line start: 1478
// line end:   1487
void StateSwitchStateData(struct __CharacterState *In, int CurrentSection, void (*NewProcess)(), int Data);

// path: C:\kain2\game\STATE.C
// address: 0x80072BD0
// line start: 1492
// line end:   1508
void StateSwitchStateCharacterData(struct __CharacterState *In, void (*NewProcess)(), int Data)
{ //line: 1
	// register: $s1
	register int i;
} //line: 17


// path: C:\kain2\game\STATE.C
// address: 0x80072CC4
// line start: 1513
// line end:   1562
void StateGovernState(struct __CharacterState *In, int Frames)
{ //line: 1
	// register: $v0
	// size: 0x11C
	register struct __State *pSectionA;
	// register: $v1
	// size: 0x11C
	register struct __State *pSectionB;
	// register: $s1
	// size: 0x30
	register struct _G2AnimSection_Type *animSectionA;
	// register: $s3
	// size: 0x30
	register struct _G2AnimSection_Type *animSectionB;
	// register: $s1
	// size: 0x14
	register struct _G2AnimKeylist_Type *keylist;
	// register: $s2
	register int keylistID;
	// register: $s4
	register int i;
} //line: 50


// path: C:\kain2\game\VRAM.C
// address: 0x80072E44
// line start: 75
// line end:   75
void VRAM_PrintVramBlock(struct _BlockVramEntry_duplicate_s8 *vblock);

// path: C:\kain2\game\VRAM.C
// address: 0x80072E4C
// line start: 110
// line end:   147
void VRAM_PrintInfo()
{ //line: 2
	// register: $v0
	// size: 0x1C
	register struct _BlockVramEntry_duplicate_s8 *vblock;
} //line: 38


// path: C:\kain2\game\VRAM.C
// address: 0x80072E94
// line start: 153
// line end:   181
void VRAM_InitVramBlockCache()
{ //line: 1
	// register: $v1
	register int i;
} //line: 26


// path: C:\kain2\game\VRAM.C
// address: 0x80072EF4
// line start: 184
// line end:   185
void VRAM_EnableTerrainArea();

// path: C:\kain2\game\VRAM.C
// address: 0x80072F24
// line start: 189
// line end:   190
void VRAM_DisableTerrainArea();

// path: C:\kain2\game\VRAM.C
// address: 0x80072F50
// line start: 195
// line end:   260
int VRAM_ConcatanateMemory(struct _BlockVramEntry_duplicate_s8 *curBlock)
{ //line: 1
	// register: $s0
	// size: 0x1C
	register struct _BlockVramEntry_duplicate_s8 *nextBlock;
} //line: 66


// path: C:\kain2\game\VRAM.C
// address: 0x80073138
// line start: 263
// line end:   264
void VRAM_GarbageCollect();

// path: C:\kain2\game\VRAM.C
// address: 0x8007316C
// line start: 270
// line end:   300
int VRAM_InsertFreeBlock(struct _BlockVramEntry_duplicate_s8 *block)
{ //line: 1
	// register: $v1
	// size: 0x1C
	register struct _BlockVramEntry_duplicate_s8 *next;
	// register: $a1
	// size: 0x1C
	register struct _BlockVramEntry_duplicate_s8 *prev;
} //line: 31


// path: C:\kain2\game\VRAM.C
// address: 0x800731FC
// line start: 304
// line end:   329
void VRAM_DeleteFreeBlock(struct _BlockVramEntry_duplicate_s8 *block)
{ //line: 2
	// register: $v1
	// size: 0x1C
	register struct _BlockVramEntry_duplicate_s8 *next;
	// register: $a1
	// size: 0x1C
	register struct _BlockVramEntry_duplicate_s8 *prev;
} //line: 26


// path: C:\kain2\game\VRAM.C
// address: 0x80073264
// line start: 333
// line end:   345
void VRAM_InsertUsedBlock(struct _BlockVramEntry_duplicate_s8 *block);

// path: C:\kain2\game\VRAM.C
// address: 0x80073298
// line start: 349
// line end:   371
void VRAM_DeleteUsedBlock(struct _BlockVramEntry_duplicate_s8 *block)
{ //line: 2
	// register: $v1
	// size: 0x1C
	register struct _BlockVramEntry_duplicate_s8 *next;
	// register: $a1
	// size: 0x1C
	register struct _BlockVramEntry_duplicate_s8 *prev;
} //line: 23


// path: C:\kain2\game\VRAM.C
// address: 0x800732FC
// size: 0x1C
// line start: 374
// line end:   388
struct _BlockVramEntry_duplicate_s8 *VRAM_GetOpenBlock()
{ //line: 2
	// register: $a0
	register int i;
} //line: 15


// path: C:\kain2\game\VRAM.C
// address: 0x80073330
// line start: 391
// line end:   427
int VRAM_DeleteFreeVram(short x, short y, short w, short h)
{ //line: 1
	// register: $t1
	// size: 0x1C
	register struct _BlockVramEntry_duplicate_s8 *prev;
	// register: $a2
	// size: 0x1C
	register struct _BlockVramEntry_duplicate_s8 *vblock;
	// register: $t0
	// size: 0x1C
	register struct _BlockVramEntry_duplicate_s8 *nextVBlock;
	// address: -24
	// size: 0x8
	auto struct _BlockVramEntry_duplicate_s8 *blockLists[2];
	// register: $t2
	register int i;
	// register: $t3
	register int delCount;
} //line: 36


// path: C:\kain2\game\VRAM.C
// address: 0x80073460
// line start: 430
// line end:   479
int VRAM_InsertFreeVram(short x, short y, short w, short h, int flags)
{ //line: 1
	// register: $a0
	// size: 0x1C
	register struct _BlockVramEntry_duplicate_s8 *useBlock;
} //line: 49


// path: C:\kain2\game\VRAM.C
// address: 0x80073600
// size: 0x1C
// line start: 489
// line end:   706
struct _BlockVramEntry_duplicate_s8 *VRAM_CheckVramSlot(short *x, short *y, short w, short h, int type, int startY)
{ //line: 1
	// register: $s1
	// size: 0x1C
	register struct _BlockVramEntry_duplicate_s8 *vblock;
	// register: $s4
	register short hldx;
	// register: $s5
	register short hldy;
	// register: $s0
	register short hldw;
	// register: $fp
	register short hldh;
	// register: $t1
	register long fits;
	// register: $s0
	register long offset;
	// register: $s3
	// size: 0x1C
	register struct _BlockVramEntry_duplicate_s8 *vblockright;
	// register: $s2
	register long offsetright;
	{ //line: 36
		// register: $v0
		register int newx;
	} //line: 50
	{ //line: 55
		// register: $v0
		register int xval;
	} //line: 71
} //line: 217


// path: C:\kain2\game\VRAM.C
// address: 0x80073A3C
// line start: 710
// line end:   735
void VRAM_ClearVramBlock(struct _BlockVramEntry_duplicate_s8 *block);

// path: C:\kain2\game\VRAM.C
// address: 0x80073A70
// line start: 790
// line end:   848
void AdjustVramCoordsObject(int oldx, int oldy, int newx, int newy, struct Object *object)
{ //line: 1
	// register: $t1
	// size: 0x10
	register struct TextureMT3 *texture;
	// register: $v0
	register int oldclutxoffset;
	// register: $a0
	register int oldclutyoffset;
	// register: $a0
	register int newclut;
	// register: $t4
	register int d;
	// register: $v1
	register int oldtpagexoffset;
	// register: $v0
	register int oldtpageyoffset;
	// register: $a3
	register int newtpage;
	// register: $t3
	// size: 0x38
	register struct _Model *model;
	// register: $s5
	register short diffy;
	// register: $s6
	register short diffx;
} //line: 59


// path: C:\kain2\game\VRAM.C
// address: 0x80073C5C
// size: 0x1C
// line start: 887
// line end:   919
struct _BlockVramEntry_duplicate_s8 *VRAM_InsertionSort(struct _BlockVramEntry_duplicate_s8 *rootNode, struct _BlockVramEntry_duplicate_s8 *newBlock)
{ //line: 3
	// register: $v1
	// size: 0x1C
	register struct _BlockVramEntry_duplicate_s8 *next;
	// register: $a2
	// size: 0x1C
	register struct _BlockVramEntry_duplicate_s8 *prev;
} //line: 31


// path: C:\kain2\game\VRAM.C
// address: 0x80073CB8
// line start: 931
// line end:   1101
void VRAM_RearrangeVramsLayer(long whichLayer)
{ //line: 1
	// register: $s0
	// size: 0x1C
	register struct _BlockVramEntry_duplicate_s8 *vblock;
	// register: $s2
	// size: 0x1C
	register struct _BlockVramEntry_duplicate_s8 *sortedBlocks;
	// register: $s1
	// size: 0x1C
	register struct _BlockVramEntry_duplicate_s8 *curBlock;
	// address: -1592
	// size: 0xC0
	auto struct _BlockVramEntry_duplicate_s8 *savedVramBlocksPTR[48];
	// address: -1400
	// size: 0x540
	auto struct _BlockVramEntry_duplicate_s8 savedVramBlocks[48];
	// address: -56
	// size: 0x8
	auto struct RECT vramLoc;
	// register: $s4
	register long numBlocks;
	// register: $s3
	register long d;
	// address: -48
	auto short x;
	// address: -46
	auto short y;
	// register: $s5
	register short dispYPos;
	{ //line: 58
		// register: $s6
		register int startY;
	} //line: 166
} //line: 169


// path: C:\kain2\game\VRAM.C
// address: 0x80074070
// line start: 1131
// line end:   1265
void VRAM_TransferBufferToVram(void *dataPtr, long dataSize, short status, void *data1, void *data2)
{ //line: 1
	// register: $s1
	// size: 0x14
	register struct VramBuffer *vramControl;
	// address: -48
	// size: 0x8
	auto struct RECT rect;
	// register: $s5
	register long *nextOTag;
	// register: $fp
	register long *drawTimerReturn;
	{ //line: 30
	} //line: 59
	{ //line: 64
	} //line: 84
	{ //line: 120
	} //line: 123
} //line: 135


// path: C:\kain2\game\VRAM.C
// address: 0x800742E8
// line start: 1271
// line end:   1274
void VRAM_LoadReturn(void *dataPtr, void *data1, void *data2)
{ //line: 1
} //line: 1


// path: C:\kain2\game\VRAM.C
// address: 0x80074308
// line start: 1277
// line end:   1315
long VRAM_GetObjectVramSpace(struct VramSize *vramSize, struct _ObjectTracker *objectTracker)
{ //line: 1
	// address: -40
	// size: 0x8
	auto struct RECT rect;
	// register: $s4
	register long result;
	// register: $s0
	// size: 0x1C
	register struct _BlockVramEntry_duplicate_s8 *lastVramBlockUsed;
} //line: 38


// path: C:\kain2\game\VRAM.C
// address: 0x80074400
// line start: 1321
// line end:   1328
void VRAM_InitMorphPalettes();

// path: C:\kain2\game\VRAM.C
// address: 0x80074408
// line start: 1355
// line end:   1456
void VRAM_UpdateMorphPalettes();

// path: C:\kain2\game\VRAM.C
// address: 0x80074410
// line start: 1513
// line end:   1539
void MORPH_ChangeAreaPalettes(long time);

// path: C:\kain2\game\PSX\SUPPORT.C
// address: 0x80074418
// line start: 57
// line end:   73
int printf(char *fmt)
{ //line: 1
	// register: $s0
	register int len;
	// address: -264
	// size: 0x100
	auto char string[256];
} //line: 1


// path: C:\kain2\game\PSX\SUPPORT.C
// address: 0x80074468
// line start: 79
// line end:   88
int sprintf(char *string, char *fmt)
{ //line: 1
} //line: 1


// path: C:\kain2\game\PSX\SUPPORT.C
// address: 0x80074498
// line start: 110
// line end:   225
int vsprintf(char *str, char *fmtstr, void *argptr)
{ //line: 1
	// register: $s4
	register int scopy;
	// register: $s3
	register char *fmt;
	{ //line: 11
		// register: $s0
		register int fsize;
		// register: $s6
		register int pad;
		// register: $fp
		register int left_just;
		// register: $s2
		register int ts_len;
		// register: $s1
		register char *temp_str;
		// address: -56
		// size: 0x10
		auto char buf[16];
	} //line: 111
} //line: 113


// path: C:\kain2\game\PSX\SUPPORT.C
// address: 0x80074714
// line start: 231
// line end:   271
char *my_itoa(unsigned long value, char *str, int radix)
{ //line: 1
	// register: $a0
	register char *p;
	// register: $a3
	register char *q;
	// address: -32
	// size: 0x1F
	auto char digits[31];
} //line: 41


// path: C:\kain2\game\PSX\SUPPORT.C
// address: 0x800747E4
// line start: 295
// line end:   315
int atoi(char *str)
{ //line: 1
	// register: $a2
	register int val;
	// register: $a3
	register int neg;
} //line: 21


// path: C:\kain2\game\PSX\SUPPORT.C
// address: 0x80074854
// line start: 319
// line end:   320
int mytolower(int c);

// path: C:\kain2\game\PSX\SUPPORT.C
// address: 0x80074870
// line start: 325
// line end:   337
int strcmpi(char *s1, char *s2)
{ //line: 2
	// register: $s0
	register int c1;
	// register: $v0
	register int c2;
} //line: 13


// path: C:\kain2\game\STRMLOAD.C
// address: 0x8005FF80
// line start: 36
// line end:   37
void STREAM_NextLoadFromHead();

// path: C:\kain2\game\STRMLOAD.C
// address: 0x8005FF90
// line start: 41
// line end:   42
void STREAM_NextLoadAsNormal();

// path: C:\kain2\game\STRMLOAD.C
// address: 0x8005FF9C
// line start: 46
// line end:   62
void STREAM_InitLoader(char *bigFileName, char *voiceFileName)
{ //line: 1
	// register: $a1
	register int i;
} //line: 16


// path: C:\kain2\game\STRMLOAD.C
// address: 0x8005FFF8
// line start: 66
// line end:   75
void STREAM_RemoveQueueHead()
{ //line: 2
	// register: $a0
	// size: 0x70
	register struct _LoadQueueEntry *entry;
} //line: 7


// path: C:\kain2\game\STRMLOAD.C
// address: 0x80060038
// line start: 78
// line end:   91
void STREAM_RemoveQueueEntry(struct _LoadQueueEntry *entry, struct _LoadQueueEntry *prev);

// path: C:\kain2\game\STRMLOAD.C
// address: 0x80060094
// size: 0x70
// line start: 95
// line end:   113
struct _LoadQueueEntry *STREAM_AddQueueEntryToTail()
{ //line: 1
	// register: $s0
	// size: 0x70
	register struct _LoadQueueEntry *entry;
} //line: 16


// path: C:\kain2\game\STRMLOAD.C
// address: 0x80060100
// size: 0x70
// line start: 116
// line end:   143
struct _LoadQueueEntry *STREAM_AddQueueEntryToHead()
{ //line: 1
	// register: $s0
	// size: 0x70
	register struct _LoadQueueEntry *entry;
} //line: 25


// path: C:\kain2\game\STRMLOAD.C
// address: 0x800601B4
// line start: 150
// line end:   154
int STREAM_IsCdBusy(long *numberInQueue);

// path: C:\kain2\game\STRMLOAD.C
// address: 0x800601D4
// line start: 167
// line end:   400
int STREAM_PollLoadQueue()
{ //line: 1
	{ //line: 6
		// register: $s1
		// size: 0x70
		register struct _LoadQueueEntry *queueEntry;
		{ //line: 36
			// register: $v0
			register long size;
		} //line: 42
		{ //line: 84
			// register: $s0
			// size: 0x70
			register struct _LoadQueueEntry *newQueue;
		} //line: 84
		{ //line: 116
			// register: $s0
			// size: 0x70
			register struct _LoadQueueEntry *newQueue;
		} //line: 116
	} //line: 231
} //line: 234


// path: C:\kain2\game\STRMLOAD.C
// address: 0x800605C0
// size: 0x70
// line start: 403
// line end:   438
struct _LoadQueueEntry *STREAM_SetUpQueueEntry(char *fileName, void *retFunc, void *retData, void *retData2, void **retPointer, int fromhead)
{ //line: 1
	// register: $s0
	// size: 0x70
	register struct _LoadQueueEntry *currentEntry;
} //line: 35


// path: C:\kain2\game\STRMLOAD.C
// address: 0x800606C0
// line start: 442
// line end:   459
void STREAM_QueueNonblockingLoads(char *fileName, unsigned char memType, void *retFunc, void *retData, void *retData2, void **retPointer, long relocateBinary)
{ //line: 1
	// register: $v1
	// size: 0x70
	register struct _LoadQueueEntry *currentEntry;
	// register: $v0
	register int fromhead;
} //line: 18


// path: C:\kain2\game\STRMLOAD.C
// address: 0x80060740
// line start: 463
// line end:   472
void LOAD_LoadToAddress(char *fileName, void *loadAddr, long relocateBinary)
{ //line: 1
	// register: $v0
	// size: 0x70
	register struct _LoadQueueEntry *currentEntry;
} //line: 10


// path: C:\kain2\game\STRMLOAD.C
// address: 0x800607A8
// line start: 486
// line end:   488
void LOAD_NonBlockingBinaryLoad(char *fileName, void *retFunc, void *retData, void *retData2, void **retPointer, int memType);

// path: C:\kain2\game\STRMLOAD.C
// address: 0x800607EC
// line start: 493
// line end:   495
void LOAD_NonBlockingFileLoad(char *fileName, void *retFunc, void *retData, void *retData2, void **retPointer, int memType);

// path: C:\kain2\game\STRMLOAD.C
// address: 0x8006082C
// line start: 499
// line end:   500
void LOAD_NonBlockingBufferedLoad(char *fileName, void *retFunc, void *retData, void *retData2);

// path: C:\kain2\game\STRMLOAD.C
// address: 0x80060868
// line start: 504
// line end:   514
int LOAD_IsXAInQueue()
{ //line: 2
	// register: $v1
	// size: 0x70
	register struct _LoadQueueEntry *entry;
} //line: 11


// path: C:\kain2\game\STRMLOAD.C
// address: 0x800608B0
// line start: 517
// line end:   524
void LOAD_PlayXA(int number)
{ //line: 1
	// register: $v0
	// size: 0x70
	register struct _LoadQueueEntry *currentEntry;
} //line: 1


// path: C:\kain2\game\STRMLOAD.C
// address: 0x800608F8
// line start: 530
// line end:   538
long *LOAD_ReadFile(char *fileName, unsigned char memType)
{ //line: 1
	// address: -16
	auto void *loadAddr;
} //line: 8


// path: C:\kain2\game\STRMLOAD.C
// address: 0x80060944
// line start: 541
// line end:   554
void LOAD_ChangeDirectory(char *name)
{ //line: 1
	// register: $s0
	// size: 0x70
	register struct _LoadQueueEntry *currentEntry;
} //line: 1


// path: C:\kain2\game\STRMLOAD.C
// address: 0x800609A4
// line start: 557
// line end:   578
void LOAD_AbortDirectoryChange(char *name)
{ //line: 1
	{ //line: 5
		// register: $a0
		// size: 0x70
		register struct _LoadQueueEntry *entry;
		// register: $a1
		// size: 0x70
		register struct _LoadQueueEntry *prev;
		// register: $v1
		register long hash;
	} //line: 20
} //line: 22


// path: C:\kain2\game\STRMLOAD.C
// address: 0x80060A2C
// line start: 581
// line end:   611
void LOAD_AbortFileLoad(char *fileName, void *retFunc)
{ //line: 1
	{ //line: 4
		// register: $s0
		// size: 0x70
		register struct _LoadQueueEntry *entry;
		// register: $s1
		// size: 0x70
		register struct _LoadQueueEntry *prev;
		// register: $v1
		register long hash;
	} //line: 30
} //line: 31


// path: C:\kain2\game\SCRIPT2.C
// address: 0x80060AFC
// line start: 127
// line end:   146
void LitShaftInit(struct _Instance *instance, struct GameTracker *gameTracker)
{ //line: 1
	// register: $s0
	// size: 0x4
	register struct LitShaftProperties *Prop;
	{ //line: 6
		// register: $v1
		register int curTimeOfDay;
	} //line: 19
} //line: 20


// path: C:\kain2\game\EVENT.C
// address: 0x80060B8C
// line start: 151
// line end:   187
void EVENT_UpdateResetSignalArrayAndWaterMovement(struct Level *oldLevel, struct Level *newLevel, long sizeOfLevel)
{ //line: 1
	// register: $a1
	register long offset;
	// register: $t0
	register long i;
	{ //line: 22
		// register: $t1
		// size: 0x1C
		register struct WaterLevelProcess *curWater;
	} //line: 35
} //line: 37


// path: C:\kain2\game\EVENT.C
// address: 0x80060C68
// line start: 191
// line end:   212
void EVENT_ResetAllOneTimeVariables()
{ //line: 2
	// register: $a2
	register int i;
} //line: 22


// path: C:\kain2\game\EVENT.C
// address: 0x80060CD0
// line start: 215
// line end:   239
void EVENT_AddSignalToReset(struct _MultiSignal *mSignal)
{ //line: 2
	// register: $a1
	register int i;
} //line: 25


// path: C:\kain2\game\EVENT.C
// address: 0x80060D2C
// line start: 266
// line end:   267
void HINT_ResetHint();

// path: C:\kain2\game\EVENT.C
// address: 0x80060D54
// line start: 271
// line end:   291
void HINT_StartHint(short hintNumber)
{ //line: 2
	// register: $a1
	register int h;
} //line: 14


// path: C:\kain2\game\EVENT.C
// address: 0x80060DA0
// line start: 294
// line end:   315
void HINT_StopHint();

// path: C:\kain2\game\EVENT.C
// address: 0x80060E04
// line start: 319
// line end:   323
void HINT_KillSpecificHint(short hintNumber);

// path: C:\kain2\game\EVENT.C
// address: 0x80060E34
// line start: 327
// line end:   330
long HINT_GetCurrentHint();

// path: C:\kain2\game\EVENT.C
// address: 0x80060E54
// line start: 339
// line end:   392
void EVENT_ProcessTimers()
{ //line: 1
	// register: $s2
	register int i;
	// register: $s0
	// size: 0x18
	register struct EventTimer *eventTimer;
} //line: 54


// path: C:\kain2\game\EVENT.C
// address: 0x80060F70
// line start: 394
// line end:   467
void EVENT_ProcessHints()
{ //line: 1
	{ //line: 6
		// address: -136
		// size: 0x80
		auto char string[128];
		// register: $s0
		register long y;
	} //line: 73
} //line: 74


// path: C:\kain2\game\EVENT.C
// address: 0x80061130
// size: 0x18
// line start: 470
// line end:   489
struct EventTimer *EVENT_GetNextTimer()
{ //line: 1
	// register: $a0
	register int i;
	{ //line: 6
		// register: $v1
		// size: 0x18
		register struct EventTimer *eventTimer;
	} //line: 18
} //line: 20


// path: C:\kain2\game\EVENT.C
// address: 0x8006119C
// line start: 492
// line end:   497
void EVENT_RemoveTimer(struct EventTimer *timer);

// path: C:\kain2\game\EVENT.C
// address: 0x800611CC
// line start: 502
// line end:   504
void EVENT_InitTimers();

// path: C:\kain2\game\EVENT.C
// address: 0x800611F8
// line start: 508
// line end:   516
void EVENT_InitTerrainMovement()
{ //line: 1
	// register: $s1
	register long i;
} //line: 8


// path: C:\kain2\game\EVENT.C
// address: 0x80061248
// line start: 519
// line end:   527
void EVENT_Init();

// path: C:\kain2\game\EVENT.C
// address: 0x80061294
// size: 0x1C
// line start: 531
// line end:   546
struct WaterLevelProcess *EVENT_GetNextTerrainMove()
{ //line: 2
	// register: $a0
	register int i;
} //line: 16


// path: C:\kain2\game\EVENT.C
// address: 0x800612CC
// line start: 566
// line end:   607
void EVENT_ProcessMovingWater()
{ //line: 2
	// register: $a3
	register long i;
	// register: $t0
	register long inUse;
	{ //line: 7
		// register: $a2
		// size: 0x1C
		register struct WaterLevelProcess *curWater;
	} //line: 36
} //line: 42


// path: C:\kain2\game\EVENT.C
// address: 0x800613FC
// line start: 611
// line end:   613
void EVENT_DoProcess();

// path: C:\kain2\game\EVENT.C
// address: 0x80061424
// line start: 617
// line end:   696
void EVENT_BSPProcess(struct _StreamUnit *streamUnit)
{ //line: 1
	// register: $s5
	register long curTree;
	// register: $v0
	// size: 0x188
	register struct Level *level;
	// register: $s6
	// size: 0x54
	register struct _Terrain *terrain;
	{ //line: 13
		// register: $a3
		// size: 0x24
		register struct BSPTree *bspTree;
		{ //line: 21
			// register: $s1
			// size: 0x29C
			register struct _Instance *instance;
			{ //line: 25
				// register: $s4
				register short dx;
				// register: $s3
				register short dy;
				// register: $s2
				register short dz;
				{ //line: 42
					// register: $s0
					// size: 0x29C
					register struct _Instance *attachedInstance;
					// address: -48
					// size: 0x8
					auto struct SVECTOR offset;
				} //line: 73
			} //line: 76
		} //line: 77
	} //line: 78
} //line: 80


// path: C:\kain2\game\EVENT.C
// address: 0x80061664
// line start: 699
// line end:   788
void EVENT_Process(struct Event *eventInstance, long startIndex)
{ //line: 1
	// register: $s1
	register long i;
} //line: 90


// path: C:\kain2\game\EVENT.C
// address: 0x800617C4
// line start: 791
// line end:   854
void EVENT_ProcessPuppetShow(struct Event *eventInstance, long startIndex)
{ //line: 1
	// register: $s0
	register long i;
	// register: $s3
	register long conditionIsStatisfied;
} //line: 64


// path: C:\kain2\game\EVENT.C
// address: 0x8006199C
// line start: 857
// line end:   887
void EVENT_ProcessEvents(struct EventPointers *eventPointers, struct Level *level)
{ //line: 1
	// register: $s0
	register long i;
} //line: 31


// path: C:\kain2\game\EVENT.C
// address: 0x80061A60
// line start: 890
// line end:   940
long EVENT_DoAction(struct Event *eventInstance, struct ScriptPCode *actionScript, short *scriptData)
{ //line: 1
	// register: $s3
	register long retValue;
	// address: -32
	auto long operateOnStack;
	// address: -1192
	// size: 0x484
	auto struct _PCodeStack stack;
	{ //line: 17
		// register: $a1
		// size: 0x18
		register struct EventTimer *timer;
	} //line: 37
} //line: 50


// path: C:\kain2\game\EVENT.C
// address: 0x80061BD8
// line start: 944
// line end:   1000
long EVENT_IsConditionTrue(struct Event *eventInstance, struct ScriptPCode *conditionScript)
{ //line: 1
	// register: $s0
	register long retValue;
	// address: -28
	auto long error;
	// address: -32
	auto long operateOnStack;
	// address: -24
	auto short flags;
	// address: -1192
	// size: 0x484
	auto struct _PCodeStack stack;
	// register: $s1
	register short *scriptData;
} //line: 56


// path: C:\kain2\game\EVENT.C
// address: 0x80061CC8
// line start: 1003
// line end:   1046
long EVENT_WriteEventObject(struct StackType_duplicate_s8 *stackEntry, long areaID, struct Event *event, long number)
{ //line: 1
	// register: $a0
	register long retValue;
	{ //line: 16
		// register: $v1
		// size: 0x2
		register struct SavedBasic *savedEvent;
	} //line: 40
} //line: 43


// path: C:\kain2\game\EVENT.C
// address: 0x80061D78
// size: 0x388
// line start: 1049
// line end:   1105
struct _MultiSignal *EVENT_ResolveObjectSignal(struct _StreamUnit *stream, long signalNumber)
{ //line: 1
	// register: $a2
	// size: 0x388
	register struct _MultiSignal *retValue;
	// register: $a0
	// size: 0x188
	register struct Level *level;
} //line: 56


// path: C:\kain2\game\EVENT.C
// address: 0x80061E10
// size: 0x4C
// line start: 1108
// line end:   1116
struct Intro *EVENT_ResolveObjectIntro(struct EventInstanceObject *instanceObject)
{ //line: 1
} //line: 1


// path: C:\kain2\game\EVENT.C
// address: 0x80061E3C
// size: 0x24
// line start: 1119
// line end:   1139
struct _SFXMkr *EVENT_ResolveSFXMarker(struct _StreamUnit *stream, struct EventInstanceObject *instanceObject)
{ //line: 2
	// register: $a0
	register int i;
	// register: $t0
	// size: 0x188
	register struct Level *level;
	// register: $a2
	// size: 0x24
	register struct _SFXMkr *result;
} //line: 20


// path: C:\kain2\game\EVENT.C
// address: 0x80061E9C
// line start: 1142
// line end:   1155
void EVENT_AddGameObjectToStack(struct _PCodeStack *stack)
{ //line: 2
	{ //line: 4
		// register: $v0
		// size: 0x24
		register struct StackType_duplicate_s8 *stackEntry;
	} //line: 8
} //line: 14


// path: C:\kain2\game\EVENT.C
// address: 0x80061EE8
// line start: 1158
// line end:   1172
void EVENT_AddPlayerObjectToStack(struct _PCodeStack *stack)
{ //line: 1
	{ //line: 4
		// register: $v0
		// size: 0x24
		register struct StackType_duplicate_s8 *stackEntry;
	} //line: 9
} //line: 15


// path: C:\kain2\game\EVENT.C
// address: 0x80061F40
// line start: 1175
// line end:   1372
void EVENT_AddObjectToStack(struct _PCodeStack *stack, long item)
{ //line: 1
	// register: $a0
	// size: 0x2
	register struct EventBasicObject *basicObject;
	// register: $s1
	register int topOfStack;
	{ //line: 13
		// register: $s0
		// size: 0x24
		register struct StackType_duplicate_s8 *stackEntry;
		{ //line: 19
		} //line: 35
		{ //line: 43
		} //line: 60
		{ //line: 75
		} //line: 77
		{ //line: 85
		} //line: 87
		{ //line: 95
		} //line: 145
		{ //line: 157
		} //line: 179
	} //line: 187
} //line: 193


// path: C:\kain2\game\EVENT.C
// address: 0x80062174
// line start: 1375
// line end:   1388
void EVENT_AddCharPointerToStack(struct _PCodeStack *stack, char *pointer)
{ //line: 2
	{ //line: 4
		// register: $v0
		// size: 0x24
		register struct StackType_duplicate_s8 *stackEntry;
	} //line: 8
} //line: 14


// path: C:\kain2\game\EVENT.C
// address: 0x800621BC
// line start: 1391
// line end:   1404
void EVENT_AddShortPointerToStack(struct _PCodeStack *stack, short *pointer)
{ //line: 2
	{ //line: 4
		// register: $v0
		// size: 0x24
		register struct StackType_duplicate_s8 *stackEntry;
	} //line: 8
} //line: 14


// path: C:\kain2\game\EVENT.C
// address: 0x80062204
// line start: 1407
// line end:   1422
void EVENT_AddNumberToStack(struct _PCodeStack *stack, long item, long flags)
{ //line: 2
	{ //line: 4
		// register: $v0
		// size: 0x24
		register struct StackType_duplicate_s8 *stackEntry;
	} //line: 10
} //line: 16


// path: C:\kain2\game\EVENT.C
// address: 0x80062254
// line start: 1425
// line end:   1429
void EVENT_ChangeOperandToNumber(struct StackType_duplicate_s8 *operand, long item, long flags);

// path: C:\kain2\game\EVENT.C
// address: 0x8006226C
// line start: 1433
// line end:   1441
void EVENT_ChangeOperandVector3d(struct StackType_duplicate_s8 *operand, short x, short y, short z, long streamUnitID);

// path: C:\kain2\game\EVENT.C
// address: 0x8006229C
// line start: 1445
// line end:   1455
void EVENT_Addvector3dToStack(struct _PCodeStack *stack, short x, short y, short z, long streamUnitID);

// path: C:\kain2\game\EVENT.C
// address: 0x80062314
// line start: 1460
// line end:   1468
void EVENT_ChangeOperandRotation3d(struct StackType_duplicate_s8 *operand, struct Rotation3d *rotation);

// path: C:\kain2\game\EVENT.C
// address: 0x8006236C
// line start: 1486
// line end:   1515
long EVENT_AddSubListObjectToStack(struct _PCodeStack *stack, long listNumber)
{ //line: 2
	{ //line: 9
		// register: $a3
		// size: 0x24
		register struct StackType_duplicate_s8 *stackEntry;
	} //line: 23
} //line: 29


// path: C:\kain2\game\EVENT.C
// address: 0x800623F8
// line start: 1565
// line end:   1583
void EVENT_StackDuplicate(struct _PCodeStack *stack);

// path: C:\kain2\game\EVENT.C
// address: 0x80062490
// line start: 1587
// line end:   1634
long EVENT_TransformTGroupAttribute(struct _PCodeStack *stack, struct StackType_duplicate_s8 *stackObject, long item, short *codeStream)
{ //line: 2
	// register: $a0
	register long retValue;
} //line: 47


// path: C:\kain2\game\EVENT.C
// address: 0x80062520
// line start: 1637
// line end:   1683
long EVENT_TransformConstrictAttribute(struct _PCodeStack *stack, struct StackType_duplicate_s8 *stackObject, long item)
{ //line: 1
	// register: $v1
	register long retValue;
	// register: $a1
	// size: 0x29C
	register struct _Instance *instance;
	{ //line: 11
	} //line: 14
	{ //line: 25
	} //line: 34
} //line: 46


// path: C:\kain2\game\EVENT.C
// address: 0x8006258C
// line start: 1689
// line end:   2162
long EVENT_TransformInstanceAttribute(struct _PCodeStack *stack, struct StackType_duplicate_s8 *stackObject, long item, short *codeStream)
{ //line: 1
	// register: $a0
	register long retValue;
	// register: $a1
	register long x;
	// register: $a2
	register long y;
	// register: $a3
	register long z;
	// register: $s0
	// size: 0x29C
	register struct _Instance *instance;
	{ //line: 12
		// register: $v0
		// size: 0x8
		register struct evPositionData *position;
	} //line: 27
	{ //line: 33
		// register: $v1
		// size: 0x8
		register struct evPositionData *rotation;
		// address: -40
		// size: 0x14
		auto struct Rotation3d vector;
	} //line: 52
	{ //line: 80
		// register: $v1
		register int status;
	} //line: 117
	{ //line: 139
	} //line: 141
	{ //line: 152
	} //line: 152
	{ //line: 159
	} //line: 159
	{ //line: 166
	} //line: 166
	{ //line: 173
		// register: $s0
		// size: 0x29C
		register struct _Instance *tmpI;
		// register: $a1
		register long value;
		{ //line: 185
		} //line: 194
	} //line: 200
	{ //line: 209
	} //line: 211
	{ //line: 222
	} //line: 224
	{ //line: 233
	} //line: 235
	{ //line: 262
	} //line: 264
	{ //line: 272
	} //line: 275
	{ //line: 298
		// register: $s2
		register long value;
		{ //line: 301
			// register: $v0
			// size: 0x40
			register struct _StreamUnit *streamUnit;
		} //line: 308
	} //line: 318
	{ //line: 342
		// register: $a1
		register long value;
	} //line: 349
	{ //line: 355
		// register: $a1
		register long value;
	} //line: 362
	{ //line: 368
		// register: $a1
		register long value;
	} //line: 376
	{ //line: 382
		// register: $a1
		register long value;
	} //line: 390
	{ //line: 396
		// register: $a1
		register long value;
	} //line: 404
} //line: 474


// path: C:\kain2\game\EVENT.C
// address: 0x80062BF0
// line start: 2165
// line end:   2253
long EVENT_TransformSoundObjectAttribute(struct _PCodeStack *stack, struct SoundObject_duplicate_s8 *soundObject, long item, short *codeStream)
{ //line: 1
	// register: $v1
	register long retValue;
	{ //line: 9
		// register: $v1
		register int status;
	} //line: 42
} //line: 88


// path: C:\kain2\game\EVENT.C
// address: 0x80062D08
// line start: 2256
// line end:   2289
long EVENT_GetGameValue(struct GameObject *gameObject)
{ //line: 1
	// register: $v1
	register long value;
} //line: 33


// path: C:\kain2\game\EVENT.C
// address: 0x80062DA4
// line start: 2293
// line end:   2572
long EVENT_TransformGameAttribute(struct _PCodeStack *stack, struct StackType_duplicate_s8 *stackObject, long item, short *codeStream)
{ //line: 1
	// register: $s0
	register long value;
	// register: $a0
	register long retValue;
	{ //line: 66
		// register: $a2
		// size: 0x18
		register struct EventTimer *timer;
		// register: $v1
		register short time;
	} //line: 94
	{ //line: 113
		// register: $v1
		register long wipeType;
		// register: $a0
		register long wipeTime;
	} //line: 135
	{ //line: 141
		// register: $a1
		register long motor0Time;
		// register: $a0
		register long motor0Speed;
		// register: $a3
		register long motor1Time;
		// register: $a2
		register long motor1Speed;
	} //line: 165
	{ //line: 180
		// register: $s1
		register short rand1;
		// register: $s0
		register short rand2;
	} //line: 193
	{ //line: 198
	} //line: 200
	{ //line: 208
		// register: $a1
		register int number;
	} //line: 210
	{ //line: 218
	} //line: 220
	{ //line: 227
	} //line: 229
} //line: 279


// path: C:\kain2\game\EVENT.C
// address: 0x80063224
// line start: 2576
// line end:   2631
long EVENT_TransformAreaAttribute(struct _PCodeStack *stack, struct StackType_duplicate_s8 *stackObject, long item, short *codeStream)
{ //line: 1
	// register: $v1
	register long retValue;
	// register: $a1
	register long offset;
	// register: $t0
	// size: 0x40
	register struct _StreamUnit *streamUnit;
} //line: 55


// path: C:\kain2\game\EVENT.C
// address: 0x80063300
// line start: 2635
// line end:   2687
long EVENT_TransformEventAttribute(struct _PCodeStack *stack, struct StackType_duplicate_s8 *stackObject, long item, short *codeStream)
{ //line: 1
	// register: $v1
	register long retValue;
	// register: $a1
	register long offset;
	// register: $t0
	// size: 0x1C
	register struct Event *event;
} //line: 52


// path: C:\kain2\game\EVENT.C
// address: 0x80063390
// line start: 2690
// line end:   2749
long EVENT_TransformSavedEventAttribute(struct _PCodeStack *stack, struct StackType_duplicate_s8 *stackObject, long item, short *codeStream)
{ //line: 1
	// register: $v1
	register long retValue;
	// register: $a1
	register long offset;
	{ //line: 19
	} //line: 21
	{ //line: 28
	} //line: 30
} //line: 59


// path: C:\kain2\game\EVENT.C
// address: 0x8006344C
// line start: 2752
// line end:   2773
long EVENT_TransformSubListObjectAttribute(struct _PCodeStack *stack, struct StackType_duplicate_s8 *stackObject, long item)
{ //line: 2
	// register: $a0
	register long retValue;
} //line: 21


// path: C:\kain2\game\EVENT.C
// address: 0x80063480
// line start: 2776
// line end:   2797
long EVENT_TransformListObjectAttribute(struct _PCodeStack *stack, struct StackType_duplicate_s8 *stackObject, long item)
{ //line: 2
	// register: $a0
	register long retValue;
} //line: 21


// path: C:\kain2\game\EVENT.C
// address: 0x800634B4
// line start: 2800
// line end:   2873
long EVENT_TransformCameraObjectAttribute(struct _PCodeStack *stack, struct StackType_duplicate_s8 *stackObject, long item, short *codeStream)
{ //line: 1
	// register: $v1
	register long retValue;
	// register: $t0
	// size: 0x4C4
	register struct Camera_duplicate_s2 *camera;
	{ //line: 12
		// register: $a1
		register short time;
	} //line: 26
	{ //line: 42
	} //line: 44
} //line: 73


// path: C:\kain2\game\EVENT.C
// address: 0x800635E0
// line start: 2876
// line end:   2928
long EVENT_TransformSplineAttribute(struct _PCodeStack *stack, struct StackType_duplicate_s8 *stackObject, long item, short *codeStream)
{ //line: 1
	// register: $v1
	register long retValue;
	// register: $s0
	// size: 0x29C
	register struct _Instance *instance;
	{ //line: 23
		// register: $v0
		register long maxKeyFrames;
	} //line: 34
} //line: 52


// path: C:\kain2\game\EVENT.C
// address: 0x800636E4
// line start: 2931
// line end:   3034
long EVENT_TransformIntroAttribute(struct _PCodeStack *stack, struct StackType_duplicate_s8 *stackObject, long item)
{ //line: 1
	// register: $v0
	register long retValue;
	// register: $s0
	register long x;
	// register: $s1
	register long y;
	// register: $s2
	register long z;
	// register: $a3
	// size: 0x4C
	register struct Intro *intro;
	{ //line: 12
	} //line: 27
	{ //line: 34
		// address: -48
		// size: 0x14
		auto struct Rotation3d vector;
	} //line: 37
} //line: 104


// path: C:\kain2\game\EVENT.C
// address: 0x80063800
// line start: 3037
// line end:   3052
long EVENT_ParseOperand2(struct StackType_duplicate_s8 *operand2, long *error, long *trueValue)
{ //line: 1
	// register: $v0
	register long number;
	{ //line: 6
		// address: -24
		auto short flags;
	} //line: 14
} //line: 16


// path: C:\kain2\game\EVENT.C
// address: 0x80063858
// line start: 3055
// line end:   3130
long EVENT_DoVMObjectAction(struct EventVmObject *vmobject, struct StackType_duplicate_s8 *operand2)
{ //line: 1
	// register: $s1
	register long result;
	// address: -20
	auto long trueValue;
	// register: $a2
	register long number;
	// address: -24
	auto long error;
} //line: 75


// path: C:\kain2\game\EVENT.C
// address: 0x80063A18
// line start: 3133
// line end:   3163
long EVENT_GetVMObjectValue(struct EventVmObject *vmobject)
{ //line: 2
	// register: $a1
	register long value;
	// register: $a2
	register long trueValue;
} //line: 30


// path: C:\kain2\game\EVENT.C
// address: 0x80063AE0
// line start: 3166
// line end:   3299
long EVENT_DoGameAction(struct GameObject *gameObject, struct StackType_duplicate_s8 *operand2)
{ //line: 1
	// address: -32
	auto long error;
	// register: $s0
	register long number;
	// address: -28
	auto long trueValue;
} //line: 133


// path: C:\kain2\game\EVENT.C
// address: 0x80063DD4
// line start: 3302
// line end:   3335
long EVENT_DoSignalAction(struct SignalObject *signalObject, struct StackType_duplicate_s8 *operand2)
{ //line: 1
	// address: -12
	auto long trueValue;
	// register: $v1
	register long number;
	// address: -16
	auto long error;
} //line: 33


// path: C:\kain2\game\EVENT.C
// address: 0x80063E50
// line start: 3338
// line end:   3367
long EVENT_TransformSignalAttribute(struct _PCodeStack *stack, struct StackType_duplicate_s8 *stackObject, long item)
{ //line: 1
	// register: $v1
	register long retValue;
	// register: $a1
	// size: 0x388
	register struct _MultiSignal *msignal;
} //line: 29


// path: C:\kain2\game\EVENT.C
// address: 0x80063EA8
// line start: 3371
// line end:   3392
long EVENT_TransformRotation3dAttribute(struct _PCodeStack *stack, struct StackType_duplicate_s8 *stackObject, long item, short *codeStream)
{ //line: 2
	// register: $v0
	register long retValue;
} //line: 22


// path: C:\kain2\game\EVENT.C
// address: 0x80063EE0
// line start: 3395
// line end:   3448
long EVENT_TransformVector3dAttribute(struct _PCodeStack *stack, struct StackType_duplicate_s8 *stackObject, long item, short *codeStream)
{ //line: 1
	// register: $a2
	register long retValue;
} //line: 53


// path: C:\kain2\game\EVENT.C
// address: 0x80063FDC
// line start: 3451
// line end:   3468
void EVENT_ModifyObjectToStackWithAttribute(struct _PCodeStack *stack, long item, short *codeStream);

// path: C:\kain2\game\EVENT.C
// address: 0x8006400C
// line start: 3474
// line end:   3528
long EVENT_DoInstanceAnimateTextureAction(struct InstanceAnimateTexture *instanceAniTexture, struct StackType_duplicate_s8 *operand2)
{ //line: 1
	// address: -20
	auto long trueValue;
	// register: $a2
	register long number;
	// register: $s0
	register long result;
	// address: -24
	auto long error;
} //line: 54


// path: C:\kain2\game\EVENT.C
// address: 0x80064114
// line start: 3531
// line end:   3535
void EVENT_ResetAllSplineFlags(struct MultiSpline *spline);

// path: C:\kain2\game\EVENT.C
// address: 0x8006419C
// line start: 3539
// line end:   3543
void EVENT_SetSplineLoop(struct MultiSpline *spline);

// path: C:\kain2\game\EVENT.C
// address: 0x80064224
// line start: 3547
// line end:   3713
long EVENT_DoSplineAction(struct InstanceSpline *instanceSpline, struct StackType_duplicate_s8 *operand2)
{ //line: 1
	// address: -28
	auto long trueValue;
	// register: $s0
	register long number;
	// register: $s4
	register long result;
	// address: -32
	auto long error;
	{ //line: 12
		// register: $s1
		// size: 0x29C
		register struct _Instance *instance;
		// register: $s2
		// size: 0x50
		register struct MultiSpline *spline;
		{ //line: 64
			// register: $a0
			register long curKeyFrame;
			{ //line: 72
				// register: $v1
				register long maxKeyFrames;
			} //line: 80
		} //line: 124
	} //line: 164
} //line: 166


// path: C:\kain2\game\EVENT.C
// address: 0x800645F0
// line start: 3716
// line end:   3937
long EVENT_DoAnimateAction(struct InstanceAnimate *instanceAnimate, struct StackType_duplicate_s8 *operand2)
{ //line: 1
	// address: -28
	auto long trueValue;
	// register: $a0
	register long number;
	// register: $s2
	register long result;
	// address: -32
	auto long error;
	// register: $s0
	// size: 0x29C
	register struct _Instance *instance;
	{ //line: 66
		// register: $v0
		// size: 0x29C
		register struct _Instance *hostInstance;
	} //line: 67
} //line: 221


// path: C:\kain2\game\EVENT.C
// address: 0x800648C4
// line start: 3941
// line end:   4427
long EVENT_DoInstanceAction(struct InstanceObject *instanceObject, struct StackType_duplicate_s8 *operand2, short *codeStream)
{ //line: 1
	// address: -36
	auto long trueValue;
	// register: $s0
	register long number;
	// address: -40
	auto long error;
	// register: $s4
	register long result;
	// register: $s1
	// size: 0x29C
	register struct _Instance *instance;
	{ //line: 132
		// register: $a0
		register int x;
		// register: $a1
		register int y;
	} //line: 150
	{ //line: 157
		// register: $s2
		// size: 0x4C
		register struct Object *object;
		// register: $s0
		register int i;
	} //line: 176
	{ //line: 197
		// register: $a0
		// size: 0x4C
		register struct Intro *intro;
	} //line: 208
	{ //line: 240
		// register: $s0
		// size: 0x14
		register struct Vector3d *vector3d;
	} //line: 243
	{ //line: 258
	} //line: 264
	{ //line: 270
	} //line: 276
	{ //line: 285
	} //line: 290
	{ //line: 296
	} //line: 302
	{ //line: 308
	} //line: 314
	{ //line: 336
		// register: $a2
		// size: 0x18
		register struct EventTimer *timer;
	} //line: 341
	{ //line: 375
	} //line: 380
	{ //line: 428
	} //line: 433
} //line: 486


// path: C:\kain2\game\EVENT.C
// address: 0x80065368
// line start: 4430
// line end:   4539
long EVENT_GetTGroupValue(struct TGroupObject_duplicate_s0 *terrainGroup)
{ //line: 2
	// register: $a2
	register long value;
	// register: $a3
	register long trueValue;
	// register: $a1
	// size: 0x24
	register struct BSPTree *bspTree;
} //line: 109


// path: C:\kain2\game\EVENT.C
// address: 0x800654D4
// line start: 4542
// line end:   4671
long EVENT_DoTGroupAction(struct TGroupObject_duplicate_s0 *terrainGroup, struct StackType_duplicate_s8 *operand2)
{ //line: 1
	// address: -20
	auto long trueValue;
	// register: $v0
	register long number;
	// address: -24
	auto long error;
	// register: $s2
	register long result;
	{ //line: 12
		// register: $s0
		// size: 0x24
		register struct BSPTree *bspTree;
		{ //line: 20
			// register: $a0
			// size: 0x1C
			register struct WaterLevelProcess *curWater;
		} //line: 36
		{ //line: 44
			// register: $a0
			register long offsetz;
		} //line: 57
	} //line: 127
} //line: 129


// path: C:\kain2\game\EVENT.C
// address: 0x800657DC
// line start: 4674
// line end:   4875
long EVENT_DoCameraAction(struct CameraObject *cameraObject, struct StackType_duplicate_s8 *operand2, short *codeStream)
{ //line: 1
	// address: -20
	auto long trueValue;
	// register: $a1
	register long number;
	// address: -24
	auto long error;
	// register: $s1
	// size: 0x4C4
	register struct Camera_duplicate_s2 *camera;
	{ //line: 34
		// register: $a0
		// size: 0x4C
		register struct Intro *intro;
	} //line: 64
	{ //line: 73
		// register: $a1
		register long angle;
	} //line: 94
	{ //line: 106
		// register: $v0
		register long angle;
	} //line: 116
	{ //line: 130
		// register: $v0
		register long angle;
	} //line: 139
	{ //line: 167
	} //line: 171
} //line: 201


// path: C:\kain2\game\EVENT.C
// address: 0x80065B5C
// line start: 4878
// line end:   4933
long EVENT_DoObjectSoundAction(struct SoundObject_duplicate_s8 *soundObject, struct StackType_duplicate_s8 *operand2)
{ //line: 1
	// address: -20
	auto long trueValue;
	// register: $v1
	register long number;
	// address: -24
	auto long error;
	// register: $s2
	register long result;
	// register: $a3
	// size: 0x29C
	register struct _Instance *instance;
	{ //line: 16
		// register: $s1
		// size: 0x3
		register struct SoundInstance *soundInstance;
	} //line: 54
} //line: 55


// path: C:\kain2\game\EVENT.C
// address: 0x80065C90
// line start: 4936
// line end:   4991
long EVENT_DoSoundMarkerAction(struct SoundObject_duplicate_s8 *soundObject, struct StackType_duplicate_s8 *operand2)
{ //line: 1
	// address: -20
	auto long trueValue;
	// register: $v1
	register long number;
	// address: -24
	auto long error;
	// register: $s2
	register long result;
	// register: $a3
	// size: 0x24
	register struct _SFXMkr *sfxMarker;
	{ //line: 16
		// register: $s1
		// size: 0x3
		register struct SoundInstance *soundInstance;
	} //line: 54
} //line: 55


// path: C:\kain2\game\EVENT.C
// address: 0x80065DC4
// line start: 4994
// line end:   5028
long EVENT_GetSoundValue(struct SoundObject_duplicate_s8 *soundObject)
{ //line: 2
} //line: 34


// path: C:\kain2\game\EVENT.C
// address: 0x80065DCC
// line start: 5031
// line end:   5075
long EVENT_DoAreaAction(struct AreaObject_duplicate_s0 *areaObject, struct StackType_duplicate_s8 *operand2)
{ //line: 1
	// address: -20
	auto long trueValue;
	// register: $v1
	register long number;
	// address: -24
	auto long error;
	// register: $s0
	// size: 0x40
	register struct _StreamUnit *streamUnit;
} //line: 44


// path: C:\kain2\game\EVENT.C
// address: 0x80065E78
// line start: 5078
// line end:   5168
long EVENT_DoIntroAction(struct IntroObject *introObject, struct StackType_duplicate_s8 *operand2)
{ //line: 1
	// address: -20
	auto long trueValue;
	// register: $a2
	register long number;
	// address: -24
	auto long error;
	// register: $s2
	register long result;
	{ //line: 12
		// register: $s0
		// size: 0x4C
		register struct Intro *intro;
		{ //line: 62
			// register: $a2
			register int i;
			// register: $v1
			register int j;
			// register: $a1
			register int id;
			{ //line: 65
				{ //line: 68
					// register: $v0
					// size: 0x4C
					register struct Intro *intro1;
				} //line: 77
			} //line: 78
		} //line: 81
	} //line: 88
} //line: 90


// path: C:\kain2\game\EVENT.C
// address: 0x80066088
// line start: 5185
// line end:   5202
void EVENT_DoStackOperationEquals(struct _PCodeStack *stack, short *codeStream)
{ //line: 1
	// address: -88
	// size: 0x24
	auto struct StackType_duplicate_s8 operand1;
	// address: -48
	// size: 0x24
	auto struct StackType_duplicate_s8 operand2;
} //line: 15


// path: C:\kain2\game\EVENT.C
// address: 0x80066174
// line start: 5205
// line end:   5266
long EVENT_CompareVector3d(struct Vector3d *vector1, struct Vector3d *vector2)
{ //line: 2
	// register: $t2
	register long retValue;
	{ //line: 31
		// register: $v0
		register long dist;
	} //line: 41
	{ //line: 47
		// register: $v0
		register long dist;
	} //line: 57
} //line: 61


// path: C:\kain2\game\EVENT.C
// address: 0x80066330
// line start: 5275
// line end:   5373
long EVENT_CompareRotationVectors(struct Rotation3d *rot1, struct Rotation3d *rot2, long trueValue)
{ //line: 1
	// address: -104
	// size: 0x20
	auto struct MATRIX matrix1;
	// address: -72
	// size: 0x20
	auto struct MATRIX matrix2;
	// register: $s2
	register long result;
	// register: $v1
	register long c1;
	// register: $s0
	register long attribute;
	// address: -40
	// size: 0x8
	auto struct _SVector axis;
	// register: $s4
	register long doSoft;
} //line: 98


// path: C:\kain2\game\EVENT.C
// address: 0x80066528
// line start: 5390
// line end:   5456
long EVENT_CompareListWithOperation(struct _PCodeStack *stack, struct ListObject *listObject, struct StackType_duplicate_s8 *operand2, long operation)
{ //line: 1
	// register: $s4
	register long retValue;
	// register: $s0
	register long d;
	// address: -96
	// size: 0x24
	auto struct StackType_duplicate_s8 operand1;
	// address: -56
	// size: 0x10
	auto char objectName[16];
	// register: $v0
	register char *temp;
	// register: $s2
	// size: 0x29C
	register struct _Instance *instance;
	// register: $s5
	register long areaID;
} //line: 66


// path: C:\kain2\game\EVENT.C
// address: 0x8006671C
// line start: 5460
// line end:   5494
long EVENT_CompareSubListWithOperation(struct _PCodeStack *stack, struct SubListObject *subListObject, struct StackType_duplicate_s8 *operand2, long operation)
{ //line: 1
	// register: $s4
	register long i;
	// register: $s5
	register long retValue;
	// register: $s0
	register long d;
	// address: -80
	// size: 0x24
	auto struct StackType_duplicate_s8 operand1;
} //line: 34


// path: C:\kain2\game\EVENT.C
// address: 0x800668B8
// line start: 5497
// line end:   5703
long EVENT_CompareOperandsWithOperation(struct _PCodeStack *stack, struct StackType_duplicate_s8 *operand1, struct StackType_duplicate_s8 *operand2, long operation)
{ //line: 1
	// register: $s1
	register long number;
	// register: $s0
	register long number1;
	// register: $a0
	register long number2;
	// address: -32
	auto long error;
	// register: $s2
	register long trueValue;
	// address: -28
	auto short flags1;
	// address: -26
	auto short flags2;
} //line: 206


// path: C:\kain2\game\EVENT.C
// address: 0x80066BF0
// line start: 5709
// line end:   5736
void EVENT_DoStackMathOperation(struct _PCodeStack *stack, long operation)
{ //line: 1
	// address: -128
	// size: 0x24
	auto struct StackType_duplicate_s8 operand1;
	// address: -88
	// size: 0x24
	auto struct StackType_duplicate_s8 operand2;
	{ //line: 19
		// address: -48
		// size: 0x24
		auto struct StackType_duplicate_s8 holdOperand;
	} //line: 24
} //line: 26


// path: C:\kain2\game\EVENT.C
// address: 0x80066DD8
// line start: 5739
// line end:   5857
short *EVENT_ParseOpcode(struct _PCodeStack *stack, short *codeStream, long *operateOnStack)
{ //line: 1
	// register: $v0
	register short pcode;
} //line: 117


// path: C:\kain2\game\EVENT.C
// address: 0x80067054
// line start: 5860
// line end:   5939
long EVENT_GetIntroValue(struct IntroObject *introObject)
{ //line: 2
	// register: $a1
	register long value;
	// register: $a2
	register long trueValue;
} //line: 79


// path: C:\kain2\game\EVENT.C
// address: 0x800670E8
// line start: 5942
// line end:   5961
void EVENT_DoSubListAction(struct SubListObject *subListObject, struct StackType_duplicate_s8 *operand2, struct _PCodeStack *stack, short *codeStream)
{ //line: 1
	// register: $s2
	register long i;
	// register: $s0
	register long d;
	// address: -72
	// size: 0x24
	auto struct StackType_duplicate_s8 operand1;
} //line: 20


// path: C:\kain2\game\EVENT.C
// address: 0x800671E0
// line start: 5965
// line end:   6100
long EVENT_GetInstanceValue(struct InstanceObject *instanceObject)
{ //line: 1
	// register: $s0
	register long value;
	// register: $a0
	// size: 0x29C
	register struct _Instance *instance;
} //line: 135


// path: C:\kain2\game\EVENT.C
// address: 0x80067358
// line start: 6103
// line end:   6112
long EVENT_GetSplineFrameNumber(struct InstanceSpline *instanceSpline)
{ //line: 1
} //line: 1


// path: C:\kain2\game\EVENT.C
// address: 0x8006739C
// line start: 6116
// line end:   6192
long EVENT_GetSplineValue(struct InstanceSpline *instanceSpline)
{ //line: 1
	// register: $a1
	register long value;
	// register: $a0
	// size: 0x29C
	register struct _Instance *instance;
	{ //line: 50
		// register: $v0
		// size: 0x50
		register struct MultiSpline *spline;
	} //line: 69
} //line: 76


// path: C:\kain2\game\EVENT.C
// address: 0x80067498
// line start: 6196
// line end:   6245
long EVENT_GetAnimateTextureValue(struct InstanceAnimateTexture *instanceAniTexture)
{ //line: 2
	// register: $a1
	register long value;
	// register: $a2
	register long trueValue;
	// register: $a3
	// size: 0x29C
	register struct _Instance *instance;
	{ //line: 30
		// register: $a0
		// size: 0xC
		register struct AniTexInfo *ani_tex_info;
	} //line: 31
} //line: 49


// path: C:\kain2\game\EVENT.C
// address: 0x80067548
// line start: 6249
// line end:   6289
long EVENT_GetAnimateValue(struct InstanceAnimate *instanceAnimate)
{ //line: 1
	// register: $a1
	register long value;
	// register: $a0
	// size: 0x29C
	register struct _Instance *instance;
} //line: 40


// path: C:\kain2\game\EVENT.C
// address: 0x800675C0
// line start: 6294
// line end:   6309
long EVENT_TransformObjectOnStack(struct _PCodeStack *stack, long item, short *codeStream)
{ //line: 1
	// register: $v0
	register long retValue;
} //line: 16


// path: C:\kain2\game\EVENT.C
// address: 0x80067614
// line start: 6314
// line end:   6354
void EVENT_UpdatePuzzlePointers(struct EventPointers *events, long offset)
{ //line: 2
	// register: $t3
	register long d;
	// register: $t1
	register long d2;
	// register: $a3
	// size: 0x1C
	register struct Event *curEvent;
} //line: 41


// path: C:\kain2\game\EVENT.C
// address: 0x800677F0
// line start: 6359
// line end:   6425
void EVENT_SaveEventsFromLevel(long levelID, struct Level *level)
{ //line: 1
	// register: $s4
	// size: 0x8
	register struct EventPointers *eventPointers;
	// register: $s0
	// size: 0x1C
	register struct Event *eventInstance;
	// register: $s3
	register long i;
	// register: $a1
	register long d;
	// register: $s1
	register long saveEvent;
	{ //line: 14
		// register: $s2
		register long useBigSave;
		{ //line: 40
			// register: $v0
			// size: 0x10
			register struct SavedEvent *savedEvent;
		} //line: 50
		{ //line: 53
			// register: $a2
			// size: 0xA
			register struct SavedEventSmallVars *savedEvent;
		} //line: 63
	} //line: 65
} //line: 67


// path: C:\kain2\game\EVENT.C
// address: 0x8006796C
// line start: 6429
// line end:   6491
void EVENT_LoadEventsForLevel(long levelID, struct Level *level)
{ //line: 1
	// register: $v1
	register long d;
	// register: $s0
	// size: 0x8
	register struct EventPointers *eventPointers;
	// register: $t0
	// size: 0x1C
	register struct Event *eventInstance;
	// register: $a1
	// size: 0x2
	register struct SavedBasic *savedEvent;
	{ //line: 15
		// register: $t1
		register long eventNumber;
	} //line: 55
} //line: 63


// path: C:\kain2\game\EVENT.C
// address: 0x80067AA8
// size: 0x2
// line start: 6495
// line end:   6518
struct SavedBasic *EVENT_CreateSaveEvent(long levelID, long eventNumber)
{ //line: 1
	// register: $a1
	// size: 0xA
	register struct SavedEventSmallVars *savedEvent;
	// register: $a0
	register long d;
} //line: 23


// path: C:\kain2\game\EVENT.C
// address: 0x80067B14
// line start: 6522
// line end:   6554
void EVENT_RemoveInstanceFromInstanceList(struct _Instance *instance)
{ //line: 2
	// register: $t3
	register int d;
	// register: $t0
	register int i;
	// register: $a1
	register int i2;
	// register: $t2
	// size: 0x8
	register struct EventPointers *puzzle;
	// register: $v0
	// size: 0x2
	register struct EventBasicObject **basicEventObject;
} //line: 33


// path: C:\kain2\game\EVENT.C
// address: 0x80067BFC
// line start: 6558
// line end:   6581
void EVENT_UpdatePuzzleWithInstance(struct EventPointers *puzzle, struct _Instance *instance)
{ //line: 2
	// register: $t2
	register int i;
	// register: $t0
	register int i2;
	// register: $v0
	// size: 0x2
	register struct EventBasicObject **basicEventObject;
} //line: 24


// path: C:\kain2\game\EVENT.C
// address: 0x80067CA0
// line start: 6584
// line end:   6616
void EVENT_AddInstanceToInstanceList(struct _Instance *instance)
{ //line: 1
	// register: $s1
	register int d;
	// register: $a1
	register int curTree;
	// register: $a0
	// size: 0x8
	register struct EventPointers *puzzle;
	// register: $s0
	// size: 0x188
	register struct Level *level;
	{ //line: 20
		// register: $a0
		// size: 0x24
		register struct BSPTree *bspTree;
	} //line: 30
} //line: 33


// path: C:\kain2\game\EVENT.C
// address: 0x80067D8C
// size: 0x3C
// line start: 6619
// line end:   6639
struct _VMObject *EVENT_FindVMObject(struct _StreamUnit *stream, char *vmoName)
{ //line: 1
	// register: $s2
	register int i;
	// register: $s3
	// size: 0x3C
	register struct _VMObject *vmObject;
	// register: $s1
	// size: 0x188
	register struct Level *level;
} //line: 20


// path: C:\kain2\game\EVENT.C
// address: 0x80067E2C
// line start: 6642
// line end:   6812
void EVENT_FixPuzzlesForStream(struct _StreamUnit *stream)
{ //line: 1
	// register: $s5
	// size: 0x8
	register struct EventPointers *puzzle;
	// register: $s6
	// size: 0x2
	register struct EventBasicObject **basicEventObject;
	// register: $a1
	// size: 0x29C
	register struct _Instance *instance;
	// register: $s0
	// size: 0x29C
	register struct _Instance *next;
	// register: $s4
	register int i;
	// register: $s2
	register int i2;
	// register: $a2
	register int p;
	{ //line: 28
		// register: $v0
		// size: 0x40
		register struct _StreamUnit *newStream;
	} //line: 36
	{ //line: 43
		// register: $s1
		// size: 0xC
		register struct EventEventObject *eventEventObject;
		// register: $v0
		// size: 0x40
		register struct _StreamUnit *newStream;
		{ //line: 50
			// register: $v1
			// size: 0x8
			register struct EventPointers *puzzleInstances;
		} //line: 67
	} //line: 68
	{ //line: 73
		// register: $s1
		// size: 0x10
		register struct EventTGroupObject *tgroupEventObject;
		// register: $v0
		// size: 0x40
		register struct _StreamUnit *newStream;
		{ //line: 78
			// register: $v0
			// size: 0x54
			register struct _Terrain *terrain;
		} //line: 100
	} //line: 101
	{ //line: 106
		// register: $v0
		// size: 0x40
		register struct _StreamUnit *newStream;
	} //line: 114
	{ //line: 123
		// register: $v1
		register void *pointer;
		{ //line: 133
			// register: $v0
			// size: 0x40
			register struct _StreamUnit *stream2;
		} //line: 141
	} //line: 141
	{ //line: 151
		// register: $v0
		// size: 0x40
		register struct _StreamUnit *newStream;
	} //line: 162
} //line: 171


// path: C:\kain2\game\EVENT.C
// address: 0x80068110
// line start: 6815
// line end:   6966
void EVENT_AddStreamToInstanceList(struct _StreamUnit *stream)
{ //line: 1
	// register: $s6
	register int d;
	// register: $s4
	register int i;
	// register: $s2
	register int i2;
	// register: $a2
	register int p;
	// register: $s5
	// size: 0x8
	register struct EventPointers *puzzle;
	// register: $s7
	// size: 0x2
	register struct EventBasicObject **basicEventObject;
	// register: $a1
	// size: 0xC
	register struct EventEventObject *eventEventObject;
	// register: $t0
	// size: 0x10
	register struct EventTGroupObject *tgroupEventObject;
	{ //line: 38
		// register: $v1
		// size: 0x8
		register struct EventPointers *puzzleInstances;
	} //line: 56
	{ //line: 63
		// register: $v0
		// size: 0x54
		register struct _Terrain *terrain;
	} //line: 87
	{ //line: 93
	} //line: 98
	{ //line: 107
		// register: $v1
		register void *pointer;
	} //line: 121
	{ //line: 130
	} //line: 139
} //line: 151


// path: C:\kain2\game\EVENT.C
// address: 0x800683F8
// line start: 6970
// line end:   7067
void EVENT_RemoveStreamToInstanceList(struct _StreamUnit *stream)
{ //line: 1
	// register: $t1
	register int d;
	// register: $s1
	register int i;
	// register: $a1
	register int i2;
	// register: $t0
	// size: 0x8
	register struct EventPointers *puzzle;
	// register: $v0
	// size: 0x2
	register struct EventBasicObject **basicEventObject;
	{ //line: 56
	} //line: 61
	{ //line: 68
	} //line: 77
} //line: 98


// path: C:\kain2\game\EVENT.C
// address: 0x800685BC
// line start: 7072
// line end:   7172
void EVENT_RelocateInstanceList(struct Level *oldLevel, struct Level *newLevel, long sizeOfLevel)
{ //line: 2
	// register: $t6
	register int d;
	// register: $t4
	register int i;
	// register: $a3
	register int i2;
	// register: $t5
	// size: 0x8
	register struct EventPointers *puzzle;
	// register: $v0
	// size: 0x2
	register struct EventBasicObject **basicEventObject;
	// register: $t0
	register long offset;
	{ //line: 22
	} //line: 27
	{ //line: 34
	} //line: 39
	{ //line: 46
	} //line: 51
	{ //line: 58
	} //line: 64
	{ //line: 71
	} //line: 79
} //line: 101


// path: C:\kain2\game\EVENT.C
// address: 0x800687BC
// line start: 7175
// line end:   7204
void EVENT_PrintVars();

// path: C:\kain2\game\EVENT.C
// address: 0x800687C4
// line start: 7209
// line end:   7318
void EVENT_ExecuteActionCommand(struct StackType_duplicate_s8 *operand1, struct StackType_duplicate_s8 *operand2, struct _PCodeStack *stack, short *codeStream)
{ //line: 1
	// address: -16
	auto long error;
	// register: $a0
	register long value;
	// address: -12
	auto short flags;
} //line: 110


// path: C:\kain2\game\EVENT.C
// address: 0x8006898C
// line start: 7322
// line end:   7418
long EVENT_GetScalerValueFromOperand(struct StackType_duplicate_s8 *operand, long *error, short *flags)
{ //line: 1
	// register: $a1
	register long value;
} //line: 96


// path: C:\kain2\game\EVENT.C
// address: 0x80068AFC
// line start: 7424
// line end:   7517
long EVENT_TransformOperand(struct StackType_duplicate_s8 *stackObject, struct _PCodeStack *stack, long item, short *codeStream)
{ //line: 1
	// register: $v0
	register long retValue;
} //line: 94


// path: C:\kain2\game\PHYSICS.C
// address: 0x800748D0
// line start: 57
// line end:   64
void SetNoPtCollideInFamily(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x29C
	register struct _Instance *child;
} //line: 8


// path: C:\kain2\game\PHYSICS.C
// address: 0x80074918
// line start: 67
// line end:   74
void ResetNoPtCollideInFamily(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x29C
	register struct _Instance *child;
} //line: 8


// path: C:\kain2\game\PHYSICS.C
// address: 0x80074964
// line start: 79
// line end:   82
void PHYSICS_CheckLineInWorld(struct _Instance *instance, struct _PCollideInfo *pcollideInfo);

// path: C:\kain2\game\PHYSICS.C
// address: 0x80074988
// line start: 85
// line end:   113
void PHYSICS_CheckLineInWorldMask(struct _Instance *instance, struct _PCollideInfo *pcollideInfo)
{ //line: 1
	// register: $s0
	// size: 0x188
	register struct Level *level;
} //line: 28


// path: C:\kain2\game\PHYSICS.C
// address: 0x800749FC
// line start: 124
// line end:   265
int PhysicsCheckLinkedMove(struct _Instance *instance, int Data, short Mode)
{ //line: 1
	{ //line: 4
		// register: $s3
		// size: 0x18
		register struct evPhysicsLinkedMoveData *ptr;
		// register: $s0
		// size: 0x29C
		register struct _Instance *on;
		// address: -144
		// size: 0x20
		auto struct MATRIX work;
		// register: $s4
		// size: 0x20
		register struct MATRIX *current;
		// address: -112
		// size: 0x10
		auto struct VECTOR delta;
		// address: -64
		auto long onx;
		// address: -60
		auto long ony;
		// address: -56
		auto long onz;
		// register: $s5
		register long ix;
		// address: -52
		auto long iy;
		// address: -48
		auto long iz;
		{ //line: 22
			// register: $a0
			// size: 0x29C
			register struct _Instance *oldOn;
		} //line: 29
		{ //line: 58
			// address: -96
			// size: 0x8
			auto struct _G2EulerAngles_Type newRot;
			// address: -88
			// size: 0x10
			auto struct VECTOR newRelPos;
			// register: $s6
			register long oldPosX;
			// register: $fp
			register long oldPosY;
			// register: $s7
			register long oldPosZ;
			// register: $s1
			register int moved;
			// register: $s0
			register int rotated;
		} //line: 121
	} //line: 136
} //line: 142


// path: C:\kain2\game\PHYSICS.C
// address: 0x80074E58
// line start: 272
// line end:   278
void PhysicsDefaultLinkedMoveResponse(struct _Instance *instance, struct evPhysicsLinkedMoveData *Data, int updateTransforms);

// path: C:\kain2\game\PHYSICS.C
// address: 0x80074EE0
// line start: 296
// line end:   564
int PhysicsCheckGravity(struct _Instance *instance, int Data, short Mode)
{ //line: 1
	// register: $s3
	// size: 0xC
	register struct evPhysicsGravityData *Ptr;
	// address: -120
	// size: 0x8
	auto struct SVECTOR D;
	// address: -112
	// size: 0x8
	auto struct SVECTOR N;
	// register: $v0
	register short Dot;
	// register: $s4
	register int rc;
	// address: -104
	// size: 0x2C
	auto struct _PCollideInfo CInfo;
	// address: -56
	// size: 0x8
	auto struct SVECTOR Old;
	// address: -48
	// size: 0x8
	auto struct SVECTOR New;
	// register: $fp
	register int slide;
	{ //line: 30
		// register: $v0
		register int stillOnOldTFace;
		// register: $s2
		// size: 0x188
		register struct Level *level;
		// register: $s1
		// size: 0xC
		register struct _TFace *tface;
	} //line: 65
	{ //line: 208
		// register: $a0
		// size: 0x29C
		register struct _Instance *oldOn;
	} //line: 215
} //line: 268


// path: C:\kain2\game\PHYSICS.C
// address: 0x80075698
// line start: 570
// line end:   577
void PhysicsDefaultGravityResponse(struct _Instance *instance, struct evPhysicsGravityData *Data);

// path: C:\kain2\game\PHYSICS.C
// address: 0x80075710
// line start: 589
// line end:   874
int PhysicsCheckEdgeGrabbing(struct _Instance *instance, struct GameTracker *gameTracker, int Data, short Mode)
{ //line: 1
	// register: $s1
	// size: 0x24
	register struct evPhysicsEdgeData *Ptr;
	// register: $s6
	register int rc;
	// address: -120
	// size: 0x10
	auto struct VECTOR OutTrans;
	// register: $v0
	// size: 0x8
	register struct SVECTOR *ExtraRot;
	// address: -104
	// size: 0x2C
	auto struct _PCollideInfo CInfo;
	// address: -56
	// size: 0x8
	auto struct SVECTOR Old;
	// address: -48
	// size: 0x8
	auto struct SVECTOR New;
	// size: 0x20
	static struct MATRIX TempMat;
	// address: 32
	// size: 0x20
	static struct MATRIX *pTempMat;
	// register: $fp
	register int wallCrawl;
	// register: $s3
	register int freeSpot;
} //line: 284


// path: C:\kain2\game\PHYSICS.C
// address: 0x80075DC8
// line start: 886
// line end:   970
void PhysicsDefaultEdgeGrabResponse(struct _Instance *instance, struct evPhysicsEdgeData *Data, int blockFlag)
{ //line: 1
	// address: -88
	// size: 0x8
	auto struct _SVector normal;
	// address: -80
	// size: 0x8
	auto struct _G2EulerAngles_Type ea1;
	// address: -72
	// size: 0x10
	auto struct VECTOR OutTrans;
	// address: -56
	// size: 0x20
	auto struct MATRIX TempMat;
	// address: -24
	// size: 0x8
	auto struct SVECTOR New;
	{ //line: 50
		// register: $v0
		register long dp;
	} //line: 50
} //line: 50


// path: C:\kain2\game\PHYSICS.C
// address: 0x8007606C
// line start: 983
// line end:   1167
int PhysicsCheckSliding(struct _Instance *instance, int Data, short Mode)
{ //line: 1
	// register: $s0
	// size: 0x28
	register struct evPhysicsSlideData *Ptr;
	// register: $s1
	register int rc;
	// address: -120
	// size: 0x10
	auto struct VECTOR OutTrans;
	// address: -104
	// size: 0x8
	auto struct _SVector normal;
	// address: -96
	// size: 0x2C
	auto struct _PCollideInfo CInfo;
	// address: -48
	// size: 0x8
	auto struct SVECTOR Old;
	// address: -40
	// size: 0x8
	auto struct SVECTOR New;
	// address: 36
	// size: 0x20
	static struct MATRIX *pTempMat;
	{ //line: 32
	} //line: 34
	{ //line: 41
		// register: $a3
		register int Temp;
	} //line: 49
} //line: 183


// path: C:\kain2\game\PHYSICS.C
// address: 0x80076518
// line start: 1177
// line end:   1218
int PhysicsUpdateTface(struct _Instance *instance, int Data)
{ //line: 1
	// address: -72
	// size: 0x2C
	auto struct _PCollideInfo CInfo;
	// address: -24
	// size: 0x8
	auto struct SVECTOR Old;
	// address: -16
	// size: 0x8
	auto struct SVECTOR New;
} //line: 42


// path: C:\kain2\game\PHYSICS.C
// address: 0x80076614
// line start: 1232
// line end:   1403
int PhysicsCheckBlockers(struct _Instance *instance, struct GameTracker *gameTracker, int Data, short Mode)
{ //line: 1
	// register: $s1
	// size: 0x24
	register struct evPhysicsEdgeData *Ptr;
	// address: -120
	// size: 0x10
	auto struct VECTOR OutTrans;
	// address: -104
	// size: 0x2C
	auto struct _PCollideInfo CInfo;
	// address: -56
	// size: 0x8
	auto struct SVECTOR Old;
	// address: -48
	// size: 0x8
	auto struct SVECTOR New;
	// address: 40
	// size: 0x20
	static struct MATRIX *pTempMat;
	{ //line: 44
		// register: $s0
		// size: 0xC
		register struct _TFace *tface;
		{ //line: 46
			// register: $a0
			register int Dot;
			// address: -40
			// size: 0x8
			auto struct SVECTOR Force;
		} //line: 91
		{ //line: 119
			// address: -32
			// size: 0xC
			auto struct _HFace *hface;
		} //line: 125
	} //line: 169
} //line: 172


// path: C:\kain2\game\PHYSICS.C
// address: 0x800769B4
// line start: 1486
// line end:   1580
int PhysicsCheckSwim(struct _Instance *instance, int Data, short Mode)
{ //line: 1
	// register: $s4
	// size: 0x18
	register struct evPhysicsSwimData *Ptr;
	// register: $s0
	register int rc;
	// register: $s0
	register int Depth;
	// register: $v1
	register int WaterDepth;
	// address: -96
	// size: 0x2C
	auto struct _PCollideInfo CInfo;
	// address: -48
	// size: 0x8
	auto struct SVECTOR Old;
	// address: -40
	// size: 0x8
	auto struct SVECTOR New;
	// register: $s2
	register long waterZLevel;
} //line: 90


// path: C:\kain2\game\PHYSICS.C
// address: 0x80076BA4
// line start: 1584
// line end:   1638
int PhysicsDefaultCheckSwimResponse(struct _Instance *instance, struct evPhysicsSwimData *Data)
{ //line: 1
	// register: $s0
	register int rc;
	// register: $s1
	register long waterZLevel;
} //line: 54


// path: C:\kain2\game\PHYSICS.C
// address: 0x80076D9C
// line start: 1856
// line end:   1873
void PhysicsForceSetWater(struct _Instance *instance, int *Time, int Depth, int rate, int maxAmplitude)
{ //line: 1
	// register: $s0
	register int Amplitude;
} //line: 18


// path: C:\kain2\game\PHYSICS.C
// address: 0x80076E74
// line start: 1887
// line end:   1905
int PhysicsCheckLOS(struct _Instance *instance, int Data, int Mode)
{ //line: 1
	// address: -56
	// size: 0x2C
	auto struct _PCollideInfo CInfo;
} //line: 1


// path: C:\kain2\game\PHYSICS.C
// address: 0x80076EA4
// line start: 1915
// line end:   2048
int PhysicsCheckDropHeight(struct _Instance *instance, int Data, int Mode)
{ //line: 1
	// register: $s0
	// size: 0x14
	register struct evPhysicsDropHeightData *data;
	// register: $v0
	// size: 0x188
	register struct Level *level;
	// address: -104
	// size: 0x8
	auto struct SVECTOR newPos;
	// address: -96
	// size: 0x8
	auto struct SVECTOR oldPos;
	// register: $s1
	register int rc;
	// register: $v1
	register int lowZ;
	// address: -88
	// size: 0x2C
	auto struct _PCollideInfo CInfo;
	// address: 48
	// size: 0x20
	static struct MATRIX TempMat;
	// address: 80
	// size: 0x20
	static struct MATRIX *pTempMat;
	{ //line: 14
		// address: -40
		// size: 0x10
		auto struct VECTOR outTrans;
		// register: $v0
		// size: 0x8
		register struct SVECTOR *ExtraRot;
	} //line: 43
	{ //line: 67
		// register: $a0
		register long waterZLevel;
	} //line: 85
} //line: 133


// path: C:\kain2\game\PHYSICS.C
// address: 0x80077134
// line start: 2060
// line end:   2154
int PhysicsCheckDropOff(struct _Instance *instance, int Data, short Mode)
{ //line: 1
	// register: $s1
	// size: 0xC
	register struct evPhysicsDropOffData *Ptr;
	// register: $s2
	register int rc;
	// address: -104
	// size: 0x10
	auto struct VECTOR OutTrans;
	// register: $v0
	// size: 0x8
	register struct SVECTOR *ExtraRot;
	// address: -88
	// size: 0x2C
	auto struct _PCollideInfo CInfo;
	// address: -40
	// size: 0x8
	auto struct SVECTOR New;
	// address: -32
	// size: 0x8
	auto struct SVECTOR Old;
	// address: 96
	// size: 0x20
	static struct MATRIX TempMat;
	// address: 128
	// size: 0x20
	static struct MATRIX *pTempMat;
} //line: 95


// path: C:\kain2\game\PHYSICS.C
// address: 0x80077354
// line start: 2209
// line end:   2489
int PhysicsFollowWall(struct _Instance *instance, struct GameTracker *gameTracker, int Data, short Mode)
{ //line: 1
	// address: -168
	// size: 0x10
	auto struct VECTOR OutTrans;
	// register: $s1
	// size: 0x2C
	register struct evPhysicsWallCrawlData *Ptr;
	// address: -152
	// size: 0x2C
	auto struct _PCollideInfo CInfo;
	// address: -104
	// size: 0x8
	auto struct SVECTOR New;
	// address: -96
	// size: 0x8
	auto struct SVECTOR Old;
	// address: 132
	// size: 0x20
	static struct MATRIX *pTempMat;
	{ //line: 72
		// address: -88
		// size: 0x6
		auto struct _Position A;
		// address: -80
		// size: 0x6
		auto struct _Position B;
		// address: -72
		// size: 0x20
		auto struct MATRIX mat;
	} //line: 132
} //line: 279


// path: C:\kain2\game\PHYSICS.C
// address: 0x80077864
// line start: 2636
// line end:   2655
void PhysicsMoveLocalZClamp(struct _Instance *instance, long segment, long time, long clamp)
{ //line: 1
	// address: -48
	// size: 0x6
	auto struct _Position pos;
	// address: -40
	// size: 0x8
	auto struct SVECTOR v;
	// address: -32
	// size: 0x8
	auto struct SVECTOR dv;
} //line: 20


// path: C:\kain2\game\PHYSICS.C
// address: 0x8007793C
// line start: 2662
// line end:   2703
void PhysicsMove(struct _Instance *instance, struct _Position *position, long time)
{ //line: 2
	// register: $t0
	register long xVel;
	// register: $t1
	register long yVel;
	// register: $t2
	register long zVel;
	// register: $t6
	register long xat;
	// register: $t4
	register long yat;
	// register: $t5
	register long zat;
} //line: 38


// path: C:\kain2\game\PHYSICS.C
// address: 0x80077AD8
// line start: 2734
// line end:   2738
void PhysicsSetVelFromZRot(struct _Instance *instance, short angle, long magnitude);

// path: C:\kain2\game\PHYSICS.C
// address: 0x80077B48
// line start: 2745
// line end:   2762
void PhysicsSetVelFromRot(struct _Instance *instance, struct _Rotation *rot, long magnitude)
{ //line: 1
	// address: -56
	// size: 0x20
	auto struct MATRIX mat;
	// address: -24
	// size: 0x8
	auto struct SVECTOR flatPt;
	// address: -16
	// size: 0x8
	auto struct SVECTOR newPt;
} //line: 1


// path: C:\kain2\game\PHYSICS.C
// address: 0x80077BB8
// line start: 2765
// line end:   2814
void PHYSICS_SetVAndAFromRot(struct _Instance *instance, struct _Rotation *rot, long v, long a)
{ //line: 1
	// address: -72
	// size: 0x8
	auto struct SVECTOR flatPt;
	// address: -64
	// size: 0x20
	auto struct MATRIX mat;
	{ //line: 15
		// address: -32
		// size: 0x8
		auto struct SVECTOR newPt;
	} //line: 17
	{ //line: 34
		// address: -24
		// size: 0x8
		auto struct SVECTOR newPt;
	} //line: 36
} //line: 50


// path: C:\kain2\game\PHYSICS.C
// address: 0x80077CA8
// line start: 2822
// line end:   2826
long PHYSICS_FindAFromDAndT(long d, long t);

// path: C:\kain2\game\PHYSICS.C
// address: 0x80077CE4
// line start: 2835
// line end:   2841
long PHYSICS_FindVFromAAndD(long a, long d)
{ //line: 1
	// register: $a0
	register long vsq;
} //line: 7


// path: C:\kain2\game\PHYSICS.C
// address: 0x80077D1C
// line start: 2876
// line end:   2892
void PHYSICS_StopIfCloseToTarget(struct _Instance *instance, int x, int y, int z);

// path: C:\kain2\game\PHYSICS.C
// address: 0x80077DFC
// line start: 2898
// line end:   2913
int PHYSICS_CheckForTerrainCollide(struct _Instance *instance, struct SVECTOR *startVec, struct SVECTOR *endVec, int segment)
{ //line: 1
	// address: -56
	// size: 0x2C
	auto struct _PCollideInfo CInfo;
	// register: $a1
	// size: 0x20
	register struct MATRIX *pTempMat;
} //line: 1


// path: C:\kain2\game\PHYSICS.C
// address: 0x80077E44
// line start: 2917
// line end:   2932
int PHYSICS_CheckForObjectCollide(struct _Instance *instance, struct SVECTOR *startVec, struct SVECTOR *endVec, int segment)
{ //line: 1
	// address: -56
	// size: 0x2C
	auto struct _PCollideInfo CInfo;
	// register: $a1
	// size: 0x20
	register struct MATRIX *pTempMat;
} //line: 1


// path: C:\kain2\game\PHYSICS.C
// address: 0x80077E88
// line start: 2938
// line end:   2962
int PHYSICS_CheckForValidMove(struct _Instance *instance, struct SVECTOR *startVec, struct SVECTOR *endVec, int segment)
{ //line: 1
	// address: -56
	// size: 0x2C
	auto struct _PCollideInfo CInfo;
	// register: $a1
	// size: 0x20
	register struct MATRIX *pTempMat;
	// register: $a0
	register int rc;
} //line: 24


// path: C:\kain2\game\PHYSICS.C
// address: 0x80077EF4
// line start: 2966
// line end:   2988
int PHYSICS_CheckFaceStick(struct _PCollideInfo *CInfo)
{ //line: 1
	// register: $a1
	register int rc;
	{ //line: 7
		// register: $a2
		// size: 0xC
		register struct _TFace *tface;
	} //line: 9
	{ //line: 13
		// address: -8
		// size: 0xC
		auto struct _HFace *hface;
	} //line: 21
} //line: 22


// path: C:\kain2\game\PHYSICS.C
// address: 0x80077F58
// line start: 2992
// line end:   3030
int PHYSICS_CheckDontGrabEdge(struct _PCollideInfo *CInfo)
{ //line: 1
	// register: $s1
	register int rc;
	{ //line: 6
		// register: $a0
		// size: 0xC
		register struct _TFace *tface;
		// register: $v0
		// size: 0x24
		register struct BSPTree *bsp;
	} //line: 15
	{ //line: 20
		// register: $v0
		// size: 0xC
		register struct _HFace *hface;
	} //line: 24
	{ //line: 29
	} //line: 37
} //line: 38


// path: C:\kain2\game\PHYSICS.C
// address: 0x80078080
// line start: 3035
// line end:   3041
void PHYSICS_GenericLineCheckSetup(short x, short y, short z, struct SVECTOR *inVec);

// path: C:\kain2\game\PHYSICS.C
// address: 0x80078090
// line start: 3053
// line end:   3055
void PHYSICS_GenericLineCheck(struct _Instance *instance, struct MATRIX *transMat, struct MATRIX *rotMat, struct _PCollideInfo *cInfo);

// path: C:\kain2\game\PHYSICS.C
// address: 0x800780B4
// line start: 3057
// line end:   3085
void PHYSICS_GenericLineCheckMask(struct _Instance *instance, struct MATRIX *transMat, struct MATRIX *rotMat, struct _PCollideInfo *cInfo)
{ //line: 1
	// register: $t0
	// size: 0x8
	register struct SVECTOR *startVec;
	// register: $t1
	// size: 0x8
	register struct SVECTOR *endVec;
	// address: -24
	// size: 0x10
	auto struct VECTOR outVec;
} //line: 1


// path: C:\kain2\game\REAVER.C
// address: 0x80079FF0
// line start: 55
// line end:   109
void SoulReaverInit(struct _Instance *instance, struct GameTracker *gameTracker)
{ //line: 1
	// register: $v1
	// size: 0x24
	register struct __ReaverData *data;
} //line: 54


// path: C:\kain2\game\REAVER.C
// address: 0x8007A090
// line start: 114
// line end:   235
void SoulReaverCollide(struct _Instance *instance, struct GameTracker *gameTracker)
{ //line: 1
	// register: $s0
	// size: 0x30
	register struct _CollideInfo *collideInfo;
	// register: $v0
	// size: 0x14
	register struct _HSphere *S;
	// register: $s3
	// size: 0x29C
	register struct _Instance *target;
	// register: $v0
	// size: 0x24
	register struct __ReaverData *reaverData;
	{ //line: 16
		// register: $s1
		register long type;
	} //line: 89
	{ //line: 115
		// register: $v1
		// size: 0x29C
		register struct _Instance *inst;
	} //line: 115
} //line: 122


// path: C:\kain2\game\REAVER.C
// address: 0x8007A200
// line start: 240
// line end:   248
void SoulReaverProcess(struct _Instance *instance, struct GameTracker *gameTracker)
{ //line: 1
	// register: $v0
	// size: 0x24
	register struct __ReaverData *data;
} //line: 8


// path: C:\kain2\game\REAVER.C
// address: 0x8007A24C
// line start: 256
// line end:   349
void CollideReaverProjectile(struct _Instance *instance, struct GameTracker *gameTracker)
{ //line: 1
	// register: $s1
	// size: 0x30
	register struct _CollideInfo *collideInfo;
	// register: $s3
	// size: 0x29C
	register struct _Instance *target;
	// register: $s0
	register long type;
	// register: $a0
	register long reavType;
	{ //line: 23
		// register: $v1
		// size: 0x188
		register struct Level *level;
	} //line: 32
	{ //line: 86
		// register: $v1
		// size: 0x29C
		register struct _Instance *inst;
	} //line: 86
} //line: 94


// path: C:\kain2\game\REAVER.C
// address: 0x8007A3A8
// line start: 354
// line end:   380
unsigned long SoulReaverQuery(struct _Instance *instance, unsigned long query)
{ //line: 3
	{ //line: 12
		// register: $a0
		// size: 0x24
		register struct __ReaverData *reaverData;
		// register: $v1
		register unsigned long retval;
	} //line: 21
} //line: 27


// path: C:\kain2\game\REAVER.C
// address: 0x8007A42C
// line start: 383
// line end:   409
void SoulReaverImbue(struct _Instance *instance, int number)
{ //line: 1
	// register: $a1
	register long color;
	// register: $v0
	// size: 0xC0
	register struct __ReaverTuneData *tuneData;
} //line: 1


// path: C:\kain2\game\REAVER.C
// address: 0x8007A4B8
// line start: 415
// line end:   460
void SoulReaverCharge(struct _Instance *instance, struct __ReaverData *data)
{ //line: 1
	// register: $v1
	register long color;
	// register: $a0
	register long shock;
	// register: $s1
	// size: 0xC0
	register struct __ReaverTuneData *tuneData;
} //line: 46


// path: C:\kain2\game\REAVER.C
// address: 0x8007A5DC
// line start: 463
// line end:   467
void StopSoulReaverCharge(struct __ReaverData *data, struct _Instance *instance);

// path: C:\kain2\game\REAVER.C
// address: 0x8007A61C
// line start: 475
// line end:   614
void SoulReaverPost(struct _Instance *instance, unsigned long message, unsigned long data)
{ //line: 1
	// register: $s0
	// size: 0x24
	register struct __ReaverData *reaverData;
	{ //line: 51
		// address: -40
		// size: 0x8
		auto struct SVECTOR oldVector;
		// address: -32
		// size: 0x8
		auto struct SVECTOR startPos;
		{ //line: 55
			// register: $v0
			// size: 0x24
			register struct __ReaverData *reaverData;
			// register: $s0
			// size: 0x10
			register struct evObjectBirthProjectileData *pData;
			{ //line: 95
				// register: $v1
				register short _x1;
				// register: $a0
				register short _y1;
				// register: $a2
				register short _z1;
				// register: $v0
				// size: 0x6
				register struct _Position *_v0;
			} //line: 95
			{ //line: 95
				// register: $v1
				register short _x1;
				// register: $a0
				register short _y1;
				// register: $a1
				register short _z1;
				// register: $v0
				// size: 0x6
				register struct _Position *_v0;
			} //line: 95
		} //line: 95
	} //line: 95
} //line: 140


// path: C:\kain2\game\REAVER.C
// address: 0x8007A978
// line start: 617
// line end:   625
unsigned long REAVER_GetGlowColor(struct _Instance *instance)
{ //line: 2
	// register: $v1
	// size: 0xC0
	register struct __ReaverTuneData *tuneData;
	// register: $v0
	// size: 0x24
	register struct __ReaverData *data;
} //line: 6


// path: C:\kain2\game\REAVER.C
// address: 0x8007A99C
// line start: 635
// line end:   687
void _SoulReaverAnimate(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x24
	register struct __ReaverData *data;
	// register: $s1
	// size: 0xC0
	register struct __ReaverTuneData *tuneData;
} //line: 52


// path: C:\kain2\game\REAVER.C
// address: 0x8007AB58
// line start: 690
// line end:   691
int SoulReaverFire();

// path: C:\kain2\game\GLYPH.C
// address: 0x8007AB64
// line start: 126
// line end:   174
void GlyphInit(struct _Instance *instance, struct GameTracker *gameTracker)
{ //line: 1
	// register: $s1
	// size: 0x9C
	register struct __GlyphData *data;
} //line: 44


// path: C:\kain2\game\GLYPH.C
// address: 0x8007AC80
// line start: 179
// line end:   181
void GlyphCollide(struct _Instance *instance, struct GameTracker *gameTracker);

// path: C:\kain2\game\GLYPH.C
// address: 0x8007AC88
// line start: 186
// line end:   209
void GlyphProcess(struct _Instance *instance, struct GameTracker *gameTracker)
{ //line: 1
	// register: $v0
	// size: 0x9C
	register struct __GlyphData *data;
} //line: 1


// path: C:\kain2\game\GLYPH.C
// address: 0x8007AD10
// line start: 216
// line end:   226
unsigned long GlyphQuery(struct _Instance *instance, unsigned long query);

// path: C:\kain2\game\GLYPH.C
// address: 0x8007AD30
// line start: 233
// line end:   250
void GlyphPost(struct _Instance *instance, unsigned long message, unsigned long messageData)
{ //line: 1
	// register: $a0
	// size: 0x9C
	register struct __GlyphData *data;
} //line: 18


// path: C:\kain2\game\GLYPH.C
// address: 0x8007AD64
// line start: 261
// line end:   278
void _GlyphSwitchProcess(struct _Instance *instance, void (*process)())
{ //line: 1
	// register: $s2
	// size: 0x9C
	register struct __GlyphData *data;
} //line: 1


// path: C:\kain2\game\GLYPH.C
// address: 0x8007AE04
// line start: 282
// line end:   290
int GlyphIsGlyphOpen(struct _Instance *instance)
{ //line: 2
	// register: $v0
	// size: 0x9C
	register struct __GlyphData *data;
} //line: 5


// path: C:\kain2\game\GLYPH.C
// address: 0x8007AE18
// line start: 295
// line end:   302
int _GlyphIsGlyphSet(int glyph)
{ //line: 1
	// register: $v1
	register unsigned long abilities;
} //line: 1


// path: C:\kain2\game\GLYPH.C
// address: 0x8007AE5C
// line start: 304
// line end:   305
int _GlyphIsGlyphUsable(int glyph);

// path: C:\kain2\game\GLYPH.C
// address: 0x8007AE98
// line start: 308
// line end:   315
int _GlyphIsAnyGlyphSet()
{ //line: 1
	// register: $v0
	register unsigned long abilities;
} //line: 1


// path: C:\kain2\game\GLYPH.C
// address: 0x8007AEC8
// line start: 318
// line end:   319
int _GlyphCost(struct _GlyphTuneData *glyphtunedata, int glyphNum);

// path: C:\kain2\game\GLYPH.C
// address: 0x8007AED8
// line start: 331
// line end:   351
void _GlyphDefaultProcess(struct _Instance *instance, int data1, int data2)
{ //line: 1
	// register: $v0
	// size: 0x8
	register struct __Event *Ptr;
	// register: $v0
	// size: 0x9C
	register struct __GlyphData *data;
} //line: 21


// path: C:\kain2\game\GLYPH.C
// address: 0x8007AF70
// line start: 354
// line end:   369
void HUD_GetPlayerScreenPt(struct DVECTOR *center)
{ //line: 1
	// address: -16
	// size: 0x6
	auto struct _Position position;
} //line: 1


// path: C:\kain2\game\GLYPH.C
// address: 0x8007B000
// line start: 372
// line end:   499
void GlyphDrawMenu(struct _Instance *instance)
{ //line: 1
	// address: -72
	// size: 0x6
	auto struct _Position place;
	// address: -64
	// size: 0x4
	auto struct DVECTOR center;
	// register: $s5
	register int n;
	// register: $s3
	register int rot;
	// register: $s6
	register int radius;
	// register: $s0
	register int glyph_rotation_speed;
	// register: $fp
	register int MANNA_Count;
	// register: $s4
	// size: 0x9C
	register struct __GlyphData *data;
	// register: $s7
	// size: 0x8C
	register struct _GlyphTuneData *glyphtunedata;
	{ //line: 23
		// register: $v0
		register int diff;
	} //line: 39
	{ //line: 50
		// register: $v1
		register int enabled;
		// register: $s2
		register int scale_modify;
		// register: $s1
		register int num;
	} //line: 92
	{ //line: 101
		// address: -56
		// size: 0xC
		auto struct _Vector f1;
	} //line: 117
	{ //line: 122
	} //line: 124
} //line: 124


// path: C:\kain2\game\GLYPH.C
// address: 0x8007B364
// line start: 502
// line end:   503
long GlyphTime(int time);

// path: C:\kain2\game\GLYPH.C
// address: 0x8007B380
// line start: 509
// line end:   548
void ShrinkGlyphMenu(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x9C
	register struct __GlyphData *data;
	// register: $a0
	register int time;
	// address: -24
	auto short accl;
} //line: 39


// path: C:\kain2\game\GLYPH.C
// address: 0x8007B4A4
// line start: 551
// line end:   580
void EnlargeGlyphMenu(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x9C
	register struct __GlyphData *data;
	// register: $a1
	register int time;
	// address: -24
	auto short accl;
} //line: 24


// path: C:\kain2\game\GLYPH.C
// address: 0x8007B5A0
// line start: 588
// line end:   624
void _GlyphOffProcess(struct _Instance *instance, int data1, int data2)
{ //line: 1
	// register: $v0
	// size: 0x8
	register struct __Event *Ptr;
	// register: $s0
	// size: 0x9C
	register struct __GlyphData *data;
} //line: 36


// path: C:\kain2\game\GLYPH.C
// address: 0x8007B6D8
// line start: 629
// line end:   733
void _GlyphSelectProcess(struct _Instance *instance, int data1, int data2)
{ //line: 1
	// register: $v0
	// size: 0x8
	register struct __Event *Ptr;
	// register: $s1
	// size: 0x9C
	register struct __GlyphData *data;
	// register: $s4
	// size: 0x8C
	register struct _GlyphTuneData *glyphtunedata;
	// register: $s2
	register int dontdraw_flag;
	{ //line: 72
		// register: $s0
		register int MANNA_Count;
	} //line: 81
} //line: 104


// path: C:\kain2\game\GLYPH.C
// address: 0x8007BBA0
// line start: 739
// line end:   773
void Glyph_StartSpell(struct _Instance *instance, int glyphnum)
{ //line: 1
	// register: $a2
	register int message;
} //line: 34


// path: C:\kain2\game\GLYPH.C
// address: 0x8007BC48
// line start: 776
// line end:   860
void Glyph_Broadcast(struct _Instance *sender, int glyphnum)
{ //line: 1
	// register: $s2
	// size: 0x29C
	register struct _Instance *instance;
	// register: $s7
	register int plane;
	// register: $s4
	register long whatAmIMask;
	// register: $s3
	register int Message;
	// address: -48
	auto int radius;
	// register: $fp
	register int radius_old;
	// register: $s5
	register int range;
	{ //line: 53
		// register: $s0
		register int dist;
		// register: $v0
		register int old_dist;
		// register: $s0
		register int old_x;
		// register: $s1
		register int old_y;
		// register: $v0
		register int new_x;
		// register: $v1
		register int new_y;
		// register: $s0
		register long whatAmI;
	} //line: 84
} //line: 85


// path: C:\kain2\game\GLYPH.C
// address: 0x8007BEA0
// line start: 863
// line end:   963
void Glyph_DoSpell(struct _Instance *instance, int glyphnum)
{ //line: 1
	// register: $fp
	register int fx_radius;
	// register: $v0
	register int fx_accl;
	// register: $s5
	register int fx_vel;
	// register: $a3
	register long fx_color;
	// register: $s7
	register int fx_height1;
	// register: $s6
	register int fx_height2;
	// register: $s4
	register int fx_height3;
	// register: $s3
	register int fx_rad2;
	// register: $s2
	register int fx_rad3;
	// register: $s0
	register int pred_offset;
	// register: $s1
	register int color_change_radius;
	// address: -80
	// size: 0x20
	auto struct MATRIX mat;
	// register: $s0
	// size: 0x8C
	register struct _GlyphTuneData *glyphtunedata;
} //line: 100


// path: C:\kain2\game\GLYPH.C
// address: 0x8007C144
// line start: 966
// line end:   969
void Glyph_EndFX();

// path: C:\kain2\game\GLYPH.C
// address: 0x8007C158
// line start: 973
// line end:   1001
void Glyph_DoFX(struct _Instance *instance);

// path: C:\kain2\game\GLYPH.C
// address: 0x8007C208
// line start: 1006
// line end:   1044
void _GlyphGenericProcess(struct _Instance *instance, int data1, int data2)
{ //line: 1
	// register: $v0
	// size: 0x8
	register struct __Event *Ptr;
	// register: $s1
	// size: 0x9C
	register struct __GlyphData *data;
	{ //line: 30
		// register: $s0
		// size: 0x8C
		register struct _GlyphTuneData *glyphtunedata;
	} //line: 36
} //line: 38


// path: C:\kain2\game\GLYPH.C
// address: 0x8007C340
// line start: 1047
// line end:   1048
void GlyphTrigger();

// path: C:\kain2\game\GLYPH.C
// address: 0x8007C35C
// line start: 1064
// line end:   1066
void MANNA_Pickup();

// path: C:\kain2\game\GLYPH.C
// address: 0x8007C378
// line start: 1070
// line end:   1078
void HEALTHU_Pickup(struct _Instance *instance);

// path: C:\kain2\game\GLYPH.C
// address: 0x8007C404
// line start: 1086
// line end:   1090
void HUD_Damp(short *val, short target, short *vel, short max)
{ //line: 1
	// address: -16
	auto short accl;
} //line: 1


// path: C:\kain2\game\GLYPH.C
// address: 0x8007C44C
// line start: 1093
// line end:   1104
void HUD_Init();

// path: C:\kain2\game\GLYPH.C
// address: 0x8007C484
// line start: 1108
// line end:   1110
void HUD_Setup_Chit_Count(int chits);

// path: C:\kain2\game\GLYPH.C
// address: 0x8007C4BC
// line start: 1114
// line end:   1199
void HUD_Update()
{ //line: 1
	// address: -16
	auto short accl;
} //line: 86


// path: C:\kain2\game\GLYPH.C
// address: 0x8007C6F0
// line start: 1211
// line end:   1343
void HUD_Draw()
{ //line: 1
	// address: -80
	// size: 0x8
	auto struct _SVector Rotation;
	// address: -72
	// size: 0x8
	auto struct _SVector Pos;
	// address: -64
	// size: 0x8
	auto struct _SVector offset;
	// register: $s0
	register int n;
	{ //line: 7
		// address: -56
		// size: 0x4
		auto struct DVECTOR center;
		// register: $a1
		register int glow;
		// register: $t0
		register int left;
		// register: $s0
		register int right;
	} //line: 45
	{ //line: 53
		// register: $s2
		register int oldx;
		// register: $s3
		register int oldy;
		// register: $s1
		register int MANNA_Count;
		// register: $s0
		register int MANNA_Max;
	} //line: 76
	{ //line: 82
		// address: -48
		// size: 0x8
		auto struct _SVector targetPos;
		// address: -40
		// size: 0x8
		auto struct _SVector accl;
		// address: -32
		// size: 0x8
		auto struct _SVector HUD_Cap_Rot;
	} //line: 103
	{ //line: 114
	} //line: 124
} //line: 133


// path: C:\kain2\game\RELMOD.C
// address: 0x8007CBA4
// line start: 11
// line end:   37
void RELMOD_InitModulePointers(int baseaddr, int *relocs)
{ //line: 2
	// register: $a2
	register unsigned int *rel_addr;
} //line: 27


// path: C:\kain2\game\RELMOD.C
// address: 0x8007CC88
// line start: 42
// line end:   68
void RELMOD_RelocModulePointers(int baseaddr, int offset, int *relocs)
{ //line: 2
	// register: $v0
	register int oldbaseaddr;
	// register: $a3
	register int *rel_addr;
} //line: 27


// path: C:\kain2\game\MONSTER\HUMAN.C
// address: 0x8007CD70
// line start: 92
// line end:   103
void HUMAN_WaitForWeapon(struct _Instance *instance, struct GameTracker *gameTracker);

// path: C:\kain2\game\MONSTER\HUMAN.C
// address: 0x8007CDE0
// size: 0x29C
// line start: 110
// line end:   149
struct _Instance *HUMAN_CreateWeapon(struct _Instance *instance, int weaponid, int segment)
{ //line: 1
	// register: $a1
	// size: 0x4C
	register struct Object *weapon;
	{ //line: 6
		// register: $s0
		// size: 0x29C
		register struct _Instance *iweapon;
	} //line: 31
} //line: 40


// path: C:\kain2\game\MONSTER\HUMAN.C
// address: 0x8007CEB4
// line start: 155
// line end:   193
void HUMAN_Init(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $v0
	// size: 0x50
	register struct _MonsterAttributes *ma;
	{ //line: 7
		// register: $v0
		register int opinion;
		// register: $a1
		// size: 0x10
		register struct _MonsterAllegiances *allegiances;
	} //line: 25
} //line: 33


// path: C:\kain2\game\MONSTER\HUMAN.C
// address: 0x8007CFAC
// line start: 199
// line end:   200
void HUMAN_CleanUp(struct _Instance *instance);

// path: C:\kain2\game\MONSTER\HUMAN.C
// address: 0x8007CFCC
// line start: 207
// line end:   260
unsigned long HUMAN_Query(struct _Instance *instance, unsigned long query)
{ //line: 1
	// register: $a3
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $v1
	// size: 0x50
	register struct _MonsterAttributes *ma;
	// register: $a1
	register unsigned long ret;
} //line: 54


// path: C:\kain2\game\MONSTER\HUMAN.C
// address: 0x8007D0BC
// line start: 266
// line end:   284
void HUMAN_DeadEntry(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 8


// path: C:\kain2\game\MONSTER\HUMAN.C
// address: 0x8007D134
// line start: 287
// line end:   315
void HUMAN_Dead(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 29


// path: C:\kain2\game\MONSTER\HUMAN.C
// address: 0x8007D21C
// line start: 321
// line end:   332
void HUMAN_StunnedEntry(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 12


// path: C:\kain2\game\MONSTER\HUMAN.C
// address: 0x8007D294
// line start: 335
// line end:   353
void HUMAN_Stunned(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 19


// path: C:\kain2\game\MONSTER\HUMAN.C
// address: 0x8007D33C
// line start: 359
// line end:   367
void HUMAN_EmbraceEntry(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 8


// path: C:\kain2\game\MONSTER\HUMAN.C
// address: 0x8007D398
// line start: 370
// line end:   425
void HUMAN_Embrace(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $a1
	// size: 0x8
	register struct __Event *message;
	// register: $s3
	register int letgo;
	// register: $s0
	register int juice;
} //line: 56


// path: C:\kain2\game\MONSTER\HUMAN.C
// address: 0x8007D560
// line start: 431
// line end:   437
void HUMAN_IdleEntry(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 1


// path: C:\kain2\game\MONSTER\HUMAN.C
// address: 0x8007D5A8
// line start: 440
// line end:   490
void HUMAN_Idle(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $a3
	// size: 0x50
	register struct _MonsterAttributes *ma;
	// register: $v1
	// size: 0x24
	register struct _MonsterIR *ally;
	// register: $a0
	// size: 0x24
	register struct _MonsterIR *enemy;
} //line: 51


// path: C:\kain2\game\MONSTER\HUMAN.C
// address: 0x8007D760
// line start: 496
// line end:   534
void HUMAN_Flee(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s2
	// size: 0x24
	register struct _MonsterIR *enemy;
	{ //line: 9
		// register: $v0
		// size: 0x50
		register struct _MonsterAttributes *ma;
	} //line: 21
} //line: 39


// path: C:\kain2\game\MONSTER\HUMAN.C
// address: 0x8007D8B8
// line start: 540
// line end:   564
void HUMAN_GetAngry()
{ //line: 1
	// register: $s0
	// size: 0x29C
	register struct _Instance *instance;
	{ //line: 8
		// register: $v0
		// size: 0x1AC
		register struct _MonsterVars *mv;
		// register: $a0
		// size: 0x10
		register struct _MonsterAllegiances *allegiances;
		// register: $v1
		// size: 0x24
		register struct _MonsterIR *mir;
	} //line: 23
} //line: 25


// path: C:\kain2\game\MONSTER\HUMAN.C
// address: 0x8007D980
// line start: 567
// line end:   579
int HUMAN_TypeOfHuman(struct _Instance *instance)
{ //line: 1
	// register: $v1
	register int type;
	// register: $a0
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 13


// path: C:\kain2\game\MONSTER\MISSILE.C
// address: 0x8007D9EC
// line start: 53
// line end:   61
void MISSILE_Process(struct _Instance *instance, struct GameTracker *gameTracker);

// path: C:\kain2\game\MONSTER\MISSILE.C
// address: 0x8007DA44
// line start: 65
// line end:   69
void MISSILE_Collide(struct _Instance *instance, struct GameTracker *gameTracker);

// path: C:\kain2\game\MONSTER\MISSILE.C
// address: 0x8007DA80
// size: 0x29C
// line start: 73
// line end:   86
struct _Instance *MISSILE_Find(struct _Instance *instance, struct _MonsterMissile *missiledef)
{ //line: 2
	// register: $v1
	// size: 0x4C
	register struct Object *ob;
	// register: $a2
	// size: 0x29C
	register struct _Instance *missile;
} //line: 14


// path: C:\kain2\game\MONSTER\MISSILE.C
// address: 0x8007DAFC
// size: 0x29C
// line start: 89
// line end:   158
struct _Instance *MISSILE_Birth(struct _Instance *instance, struct _MonsterMissile *missiledef)
{ //line: 1
	// register: $a1
	// size: 0x29C
	register struct _Instance *missile;
	{ //line: 33
		// register: $v0
		// size: 0x20
		register struct MATRIX *matrix;
	} //line: 37
} //line: 70


// path: C:\kain2\game\MONSTER\MISSILE.C
// address: 0x8007DBD0
// size: 0x29C
// line start: 161
// line end:   192
struct _Instance *MISSILE_Fire(struct _Instance *instance, struct _MonsterMissile *missiledef, void *target, int type)
{ //line: 1
	// register: $s0
	// size: 0x29C
	register struct _Instance *miss;
	{ //line: 8
		// register: $a3
		register int spin;
		// address: -32
		// size: 0x8
		auto struct _SVector rotVel;
	} //line: 29
} //line: 31


// path: C:\kain2\game\MONSTER\MISSILE.C
// address: 0x8007DC90
// size: 0x29C
// line start: 200
// line end:   201
struct _Instance *MISSILE_FireAtInstance(struct _Instance *instance, struct _MonsterMissile *missiledef, struct _Instance *target);

// path: C:\kain2\game\MONSTER\MONAPI.C
// address: 0x8007DCB0
// line start: 165
// line end:   236
void MonsterProcess(struct _Instance *instance, struct GameTracker *gameTracker)
{ //line: 1
	// register: $s0
	// size: 0x8
	register struct _MonsterState *state;
	// register: $s2
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $v0
	// size: 0x50
	register struct _MonsterAttributes *attributes;
} //line: 72


// path: C:\kain2\game\MONSTER\MONAPI.C
// address: 0x8007DDE8
// line start: 243
// line end:   355
void MonsterInit(struct _Instance *instance, struct GameTracker *gameTracker)
{ //line: 1
	// register: $s2
	// size: 0x50
	register struct _MonsterAttributes *attributes;
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
	{ //line: 19
	} //line: 39
	{ //line: 106
	} //line: 111
} //line: 113


// path: C:\kain2\game\MONSTER\MONAPI.C
// address: 0x8007DFF0
// line start: 358
// line end:   381
void SendHitObject(struct _Instance *instance, struct _Instance *hit, int type)
{ //line: 1
	// register: $a1
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $a0
	register int i;
	{ //line: 7
		// register: $v0
		// size: 0x8
		register struct evMonsterHitObjectData *data1;
	} //line: 10
	{ //line: 19
		// register: $v0
		// size: 0x8
		register struct evMonsterHitObjectData *data;
	} //line: 23
} //line: 24


// path: C:\kain2\game\MONSTER\MONAPI.C
// address: 0x8007E0C4
// line start: 388
// line end:   580
void MonsterCollide(struct _Instance *instance, struct GameTracker *gameTracker)
{ //line: 1
	// register: $s1
	// size: 0x30
	register struct _CollideInfo *collideInfo;
	// register: $s6
	// size: 0x1AC
	register struct _MonsterVars *mv;
	{ //line: 10
		// register: $v0
		// size: 0x14
		register struct _HSphere *s0;
		{ //line: 24
			// register: $v0
			// size: 0x14
			register struct _HSphere *s1;
			{ //line: 27
				// register: $s3
				// size: 0x29C
				register struct _Instance *inst1;
				// register: $s0
				register int power;
			} //line: 48
		} //line: 48
		{ //line: 64
			// register: $s7
			register int move;
			{ //line: 70
				// register: $s0
				// size: 0x14
				register struct _HSphere *hsphere;
			} //line: 78
			{ //line: 108
				// register: $s3
				// size: 0x24
				register struct BSPTree *bsp;
				{ //line: 115
					// register: $s0
					// size: 0x10
					register struct evMonsterHitTerrainData *data;
					// register: $s5
					// size: 0x188
					register struct Level *level;
					// register: $v0
					// size: 0x4C
					register struct Intro *impaler;
					// register: $s2
					// size: 0xC
					register struct _TFace *tface;
					{ //line: 146
						// address: -88
						// size: 0x30
						auto struct _CollideInfo parentCI;
					} //line: 153
				} //line: 157
			} //line: 162
		} //line: 188
	} //line: 189
} //line: 193


// path: C:\kain2\game\MONSTER\MONAPI.C
// address: 0x8007E5C8
// line start: 586
// line end:   588
void MonsterAdditionalCollide(struct _Instance *instance, struct GameTracker *gameTracker);

// path: C:\kain2\game\MONSTER\MONAPI.C
// address: 0x8007E5F8
// line start: 595
// line end:   706
unsigned long MonsterQuery(struct _Instance *instance, unsigned long query)
{ //line: 1
	// register: $a2
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s0
	// size: 0x50
	register struct _MonsterAttributes *ma;
	// register: $a0
	register unsigned long ret;
	{ //line: 13
		// register: $v0
		// size: 0xC
		register struct evShadowSegmentData *shadowData;
	} //line: 14
	{ //line: 89
		// register: $s0
		// size: 0x10
		register struct _MonsterSaveInfo *saveInfo;
	} //line: 89
} //line: 112


// path: C:\kain2\game\MONSTER\MONAPI.C
// address: 0x8007E858
// line start: 713
// line end:   809
void MonsterMessage(struct _Instance *instance, unsigned long message, unsigned long data)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	{ //line: 21
	} //line: 24
	{ //line: 49
	} //line: 51
	{ //line: 56
	} //line: 58
} //line: 97


// path: C:\kain2\game\MONSTER\MONAPI.C
// address: 0x8007EAF4
// line start: 815
// line end:   845
void AnimDistanceAndVel(struct Object *object, struct _MonsterAnim *mAnim)
{ //line: 1
	// address: -216
	// size: 0xB4
	auto struct _G2Anim_Type anim;
	// register: $s0
	// size: 0x30
	register struct _G2AnimSection_Type *animSection;
	// register: $s2
	// size: 0x14
	register struct _G2AnimKeylist_Type *keylist;
	// register: $s0
	register long total;
	{ //line: 21
		// address: -32
		// size: 0x6
		auto struct _G2SVector3_Type dist;
	} //line: 28
} //line: 30


// path: C:\kain2\game\MONSTER\MONAPI.C
// address: 0x8007EC5C
// line start: 969
// line end:   1017
void TranslateAnimList(struct Object *object, struct _MonsterAnim *animList, int numAnims)
{ //line: 1
	// register: $s2
	register int i;
	{ //line: 4
		// register: $s3
		// size: 0x10
		register struct _MonsterAnim *animPtr;
		{ //line: 12
			// register: $a0
			register int index;
			// register: $s4
			// size: 0x14
			register struct _G2AnimKeylist_Type *keylist;
		} //line: 47
	} //line: 48
} //line: 49


// path: C:\kain2\game\MONSTER\MONAPI.C
// address: 0x8007ED58
// line start: 1027
// line end:   1160
void MonsterTranslateAnim(struct Object *object)
{ //line: 1
	// register: $s0
	// size: 0x50
	register struct _MonsterAttributes *attributes;
	// register: $a3
	register int i;
	{ //line: 16
		// register: $v0
		register void *relocModule;
	} //line: 41
	{ //line: 68
		// register: $v1
		// size: 0x20
		register struct _MonsterAttackAttributes *attackAttr;
	} //line: 96
	{ //line: 99
		// register: $v1
		// size: 0x2E
		register struct _MonsterCombatAttributes *combatAttr;
		{ //line: 101
			// register: $a2
			register int largest;
			// register: $a1
			register int j;
			{ //line: 108
				// register: $v0
				// size: 0x20
				register struct _MonsterAttackAttributes *attack;
			} //line: 111
		} //line: 114
	} //line: 118
} //line: 134


// path: C:\kain2\game\MONSTER\MONAPI.C
// address: 0x8007EF10
// line start: 1166
// line end:   1216
void MonsterRelocateTune(struct Object *object, long offset)
{ //line: 2
	// register: $t0
	// size: 0x50
	register struct _MonsterAttributes *attributes;
	{ //line: 6
		// register: $a3
		register int i;
		// register: $a2
		// size: 0x54
		register struct _MonsterSubAttributes *subAttr;
		// register: $t1
		// size: 0x54
		register struct _MonsterSubAttributes **pSubAttr;
		// register: $v0
		// size: 0x2E
		register struct _MonsterCombatAttributes **pCombatAttr;
		{ //line: 25
			// register: $v1
			register int j;
			// register: $a0
			// size: 0x54
			register struct _MonsterSubAttributes **oldSubAttr;
		} //line: 46
	} //line: 50
} //line: 51


// path: C:\kain2\game\MONSTER\MONAPI.C
// address: 0x8007F12C
// line start: 1222
// line end:   1244
void MonsterRelocateInstanceObject(struct _Instance *instance, long offset)
{ //line: 2
	// register: $v0
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 23


// path: C:\kain2\game\MONSTER\MONAPI.C
// address: 0x8007F250
// line start: 1294
// line end:   1311
void MONAPI_TurnHead(struct _Instance *instance, short *rotx, short *rotz, struct GameTracker *gameTracker);

// path: C:\kain2\game\MONSTER\MONAPI.C
// address: 0x8007F2CC
// line start: 1315
// line end:   1321
void MONAPI_SetLookAround(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 1


// path: C:\kain2\game\MONSTER\MONAPI.C
// address: 0x8007F2FC
// line start: 1324
// line end:   1330
void MONAPI_ResetLookAround(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 1


// path: C:\kain2\game\MONSTER\MONAPI.C
// address: 0x8007F32C
// line start: 1333
// line end:   1334
long MONAPI_OkToLookAround(struct _Instance *instance);

// path: C:\kain2\game\MONSTER\MONAPI.C
// address: 0x8007F340
// line start: 1338
// line end:   1342
void MONAPI_DeleteRegen(struct _MONAPI_Regenerator *regen);

// path: C:\kain2\game\MONSTER\MONAPI.C
// address: 0x8007F38C
// line start: 1349
// line end:   1400
void MONAPI_ProcessGenerator()
{ //line: 1
	// register: $s2
	register int i;
	// register: $s0
	// size: 0x8
	register struct _MONAPI_Regenerator *regen;
	{ //line: 9
		// register: $v1
		register unsigned long time;
		{ //line: 18
			// register: $v0
			// size: 0x188
			register struct Level *level;
			{ //line: 21
				// register: $a0
				// size: 0x4C
				register struct Intro *intro;
				// register: $a1
				register int j;
				// register: $a2
				register long id;
			} //line: 39
		} //line: 42
	} //line: 50
} //line: 52


// path: C:\kain2\game\MONSTER\MONAPI.C
// address: 0x8007F4CC
// line start: 1403
// line end:   1415
void MONAPI_AddToGenerator(struct _Instance *instance)
{ //line: 1
	{ //line: 5
		// register: $s1
		// size: 0x1AC
		register struct _MonsterVars *mv;
		// register: $s0
		// size: 0x8
		register struct _MONAPI_Regenerator *regen;
	} //line: 12
} //line: 13


// path: C:\kain2\game\MONSTER\MONAPI.C
// address: 0x8007F56C
// line start: 1421
// line end:   1433
void MONAPI_CheckGenerator(struct _Instance *instance)
{ //line: 1
	// register: $a1
	register int i;
	// register: $v1
	// size: 0x8
	register struct _MONAPI_Regenerator *regen;
	// register: $a0
	register int id;
} //line: 13


// path: C:\kain2\game\MONSTER\MONMSG.C
// address: 0x80084984
// line start: 32
// line end:   52
int MON_GroundMoveQueueHandler(struct _Instance *instance)
{ //line: 1
	// register: $s3
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $a1
	// size: 0x8
	register struct __Event *message;
	// register: $s0
	register int ret;
} //line: 20


// path: C:\kain2\game\MONSTER\MONMSG.C
// address: 0x80084A0C
// line start: 60
// line end:   164
void MON_PupateMessageHandler(struct _Instance *instance, struct __Event *message)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $v1
	register long id;
	{ //line: 12
		// register: $s1
		// size: 0xC
		register struct evMonsterAlarmData *data;
	} //line: 19
	{ //line: 26
		// address: -40
		// size: 0x14
		auto struct evFXHitData data;
	} //line: 33
} //line: 105


// path: C:\kain2\game\MONSTER\MONMSG.C
// address: 0x80084C50
// line start: 167
// line end:   217
void MON_IdleMessageHandler(struct _Instance *instance, struct __Event *message)
{ //line: 1
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $v1
	register long id;
	{ //line: 32
		// register: $v0
		// size: 0x24
		register struct _MonsterIR *mir;
	} //line: 35
	{ //line: 41
		// register: $v0
		// size: 0x24
		register struct _MonsterIR *mir;
	} //line: 44
} //line: 51


// path: C:\kain2\game\MONSTER\MONMSG.C
// address: 0x80084D94
// line start: 220
// line end:   770
void MON_DefaultMessageHandler(struct _Instance *instance, struct __Event *message)
{ //line: 1
	// register: $a1
	// size: 0x50
	register struct _MonsterAttributes *ma;
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $a0
	register long id;
	{ //line: 12
		// register: $s5
		// size: 0x10
		register struct evMonsterHitData *data;
		// register: $s4
		register int fatal;
		// register: $s0
		register int damage;
		{ //line: 76
			// register: $s0
			// size: 0x2E
			register struct _MonsterCombatAttributes *combat;
		} //line: 76
	} //line: 109
	{ //line: 113
		// register: $a1
		register int state;
		// register: $s0
		// size: 0x10
		register struct evMonsterHitData *data;
	} //line: 152
	{ //line: 175
		// register: $v0
		// size: 0xC
		register struct evMonsterSoulSuckData *data;
	} //line: 179
	{ //line: 217
		// register: $s0
		register int vuln;
	} //line: 251
	{ //line: 266
	} //line: 270
	{ //line: 310
		// register: $s0
		// size: 0xC
		register struct evMonsterAlarmData *data;
		// register: $s3
		// size: 0x29C
		register struct _Instance *enemy;
		{ //line: 317
			{ //line: 322
				// register: $v0
				// size: 0x24
				register struct _MonsterIR *mir;
			} //line: 327
		} //line: 328
	} //line: 332
	{ //line: 338
		// register: $v0
		// size: 0x24
		register struct _MonsterIR *mir;
	} //line: 340
	{ //line: 346
		// register: $v1
		register int hp;
	} //line: 349
	{ //line: 382
		// address: -80
		// size: 0x14
		auto struct evFXHitData data;
	} //line: 389
	{ //line: 448
		// address: -56
		// size: 0x14
		auto struct evFXHitData data;
	} //line: 449
	{ //line: 463
	} //line: 466
	{ //line: 481
		// register: $a0
		// size: 0x8
		register struct evPositionData *posData;
	} //line: 481
	{ //line: 489
		// register: $v1
		// size: 0x8
		register struct evPositionData *posData;
	} //line: 489
	{ //line: 499
		// register: $v1
		// size: 0x8
		register struct evPositionData *posData;
	} //line: 499
	{ //line: 512
		// size: 0x24
		static int deaths[9];
	} //line: 523
	{ //line: 529
		// register: $s0
		// size: 0x10
		register struct evMonsterHitData *data;
	} //line: 529
	{ //line: 536
		// register: $s1
		register int i;
		// register: $s0
		// size: 0x18
		register struct evActionPlayHostAnimationData *data;
	} //line: 540
	{ //line: 544
		// register: $v0
		// size: 0x14
		register struct evAnimationInstanceSwitchData *data;
	} //line: 546
} //line: 551


// path: C:\kain2\game\MONSTER\MONMSG.C
// address: 0x80085B8C
// line start: 773
// line end:   779
void MON_PupateQueueHandler(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $v0
	// size: 0x8
	register struct __Event *message;
} //line: 7


// path: C:\kain2\game\MONSTER\MONMSG.C
// address: 0x80085BD8
// line start: 782
// line end:   788
void MON_IdleQueueHandler(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $v0
	// size: 0x8
	register struct __Event *message;
} //line: 7


// path: C:\kain2\game\MONSTER\MONMSG.C
// address: 0x80085C24
// line start: 791
// line end:   797
void MON_DefaultQueueHandler(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $v0
	// size: 0x8
	register struct __Event *message;
} //line: 7


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x8007F5D8
// line start: 148
// line end:   168
void MON_TurnOffWeaponSpheres(struct _Instance *instance)
{ //line: 1
	// register: $s2
	// size: 0x1AC
	register struct _MonsterVars *mv;
	{ //line: 5
		// register: $s0
		// size: 0x29C
		register struct _Instance *weapon;
	} //line: 9
	{ //line: 12
		// register: $a1
		register int i;
		// register: $v1
		// size: 0x8
		register struct _HPrim_duplicate_s3 *hprim;
		// register: $v1
		// size: 0x8
		register struct _HModel *hmodel;
	} //line: 20
} //line: 21


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x8007F6D0
// line start: 174
// line end:   191
void MON_TurnOnWeaponSpheres(struct _Instance *instance)
{ //line: 2
	// register: $a2
	// size: 0x1AC
	register struct _MonsterVars *mv;
	{ //line: 5
		// register: $a1
		register int i;
		// register: $a0
		// size: 0x8
		register struct _HPrim_duplicate_s3 *hprim;
		// register: $v0
		// size: 0x8
		register struct _HModel *hmodel;
	} //line: 17
} //line: 18


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x8007F788
// line start: 198
// line end:   223
void MON_TurnOnWeaponSphere(struct _Instance *instance, int segment)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	{ //line: 6
		// register: $v0
		// size: 0x29C
		register struct _Instance *weapon;
	} //line: 10
	{ //line: 16
		// register: $a2
		register int i;
		// register: $a0
		// size: 0x8
		register struct _HPrim_duplicate_s3 *hprim;
		// register: $v0
		// size: 0x8
		register struct _HModel *hmodel;
	} //line: 25
} //line: 26


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x8007F890
// line start: 229
// line end:   242
void MON_TurnOffBodySpheres(struct _Instance *instance)
{ //line: 2
	// register: $a2
	// size: 0x1AC
	register struct _MonsterVars *mv;
	{ //line: 5
		// register: $a1
		register int i;
		// register: $v1
		// size: 0x8
		register struct _HPrim_duplicate_s3 *hprim;
		// register: $v1
		// size: 0x8
		register struct _HModel *hmodel;
	} //line: 13
} //line: 14


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x8007F938
// line start: 248
// line end:   261
void MON_TurnOnBodySpheres(struct _Instance *instance)
{ //line: 2
	// register: $a2
	// size: 0x1AC
	register struct _MonsterVars *mv;
	{ //line: 5
		// register: $a1
		register int i;
		// register: $a0
		// size: 0x8
		register struct _HPrim_duplicate_s3 *hprim;
		// register: $v0
		// size: 0x8
		register struct _HModel *hmodel;
	} //line: 13
} //line: 14


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x8007F9EC
// line start: 267
// line end:   269
void MON_TurnOffAllSpheres(struct _Instance *instance);

// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x8007FA18
// line start: 276
// line end:   278
void MON_TurnOnAllSpheres(struct _Instance *instance);

// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x8007FA44
// line start: 318
// line end:   356
void MON_SwitchState(struct _Instance *instance, enum MonsterState state)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 39


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x8007FB40
// line start: 360
// line end:   375
void MON_SwitchStateDoEntry(struct _Instance *instance, enum MonsterState state)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 16


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x8007FBA8
// line start: 388
// line end:   389
int MON_TransNodeAnimation(struct _Instance *instance);

// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x8007FBD0
// size: 0x10
// line start: 397
// line end:   403
struct _MonsterAnim *MON_GetAnim(struct _Instance *instance, char *animList, int index)
{ //line: 2
	// register: $a1
	register int whichAnim;
} //line: 7


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x8007FC04
// line start: 409
// line end:   467
void MON_PlayAnimID(struct _Instance *instance, int index, int mode)
{ //line: 1
	// register: $s2
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s1
	// size: 0x10
	register struct _MonsterAnim *manim;
	// register: $v0
	// size: 0x50
	register struct _MonsterAttributes *ma;
	// register: $a2
	register int anim;
	// register: $s6
	register int anim0;
	// register: $s0
	register int i;
	// register: $s5
	register int interpFrames;
	// register: $s4
	register int alphaTable;
} //line: 57


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x8007FDBC
// line start: 470
// line end:   471
void MON_PlayAnimFromList(struct _Instance *instance, char *animList, int animtype, int mode);

// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x8007FDE4
// line start: 475
// line end:   479
int MON_AnimIDPlaying(struct _Instance *instance, int index)
{ //line: 2
	// register: $v0
	// size: 0x50
	register struct _MonsterAttributes *ma;
} //line: 4


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x8007FE08
// line start: 482
// line end:   484
void MON_PlayAnimIDIfNotPlaying(struct _Instance *instance, int index, int mode);

// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x8007FE58
// line start: 491
// line end:   492
int MON_AnimPlayingFromList(struct _Instance *instance, char *animList, int animtype);

// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x8007FE80
// line start: 496
// line end:   501
void MON_PlayAnimFromListIfNotPlaying(struct _Instance *instance, char *animList, int animtype, int mode)
{ //line: 1
	// register: $s0
	register int index;
} //line: 6


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x8007FED8
// line start: 508
// line end:   509
void MON_PlayAnim(struct _Instance *instance, enum MonsterAnim animtype, int mode);

// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x8007FF0C
// line start: 516
// line end:   517
int MON_AnimPlaying(struct _Instance *instance, enum MonsterAnim animtype);

// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x8007FF40
// line start: 524
// line end:   525
void MON_PlayAnimIfNotPlaying(struct _Instance *instance, enum MonsterAnim animtype, int mode);

// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x8007FF74
// line start: 566
// line end:   591
long MON_AnimCallback(struct _G2Anim_Type *anim, int sectionID, enum _G2AnimCallbackMsg_Enum_duplicate_e5 message, long messageDataA, long messageDataB, void *data)
{ //line: 1
	// register: $a3
	// size: 0x29C
	register struct _Instance *instance;
	// register: $t0
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 25


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x80080024
// line start: 597
// line end:   628
void MON_AnimInit(struct _Instance *instance)
{ //line: 1
	// register: $s2
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s3
	// size: 0x30
	register struct _G2AnimSection_Type *animSection;
	// register: $s0
	register int i;
	// register: $s6
	register int startSection;
	// register: $s7
	register int numSections;
	{ //line: 14
		// register: $a3
		register int sectionEnd;
	} //line: 24
} //line: 29


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x8008019C
// line start: 638
// line end:   644
short MON_FacingOffset(struct _Instance *instance, struct _Instance *target)
{ //line: 1
} //line: 1


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x800801D8
// line start: 651
// line end:   689
int MON_CheckConditions(struct _Instance *instance, struct _MonsterIR *mir, char *probArray)
{ //line: 2
	// register: $v0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $a2
	register char *probability;
	// register: $a1
	register int chance;
	// register: $t1
	register int i;
	// register: $a3
	register int prob;
	// register: $t0
	register int nprob;
	{ //line: 12
		// register: $v0
		register int conditionProb;
	} //line: 23
} //line: 26


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x800802B0
// line start: 695
// line end:   722
int MON_ShouldIAttackInstance(struct _Instance *instance, struct _Instance *ei)
{ //line: 1
	{ //line: 4
		// register: $s1
		// size: 0x29C
		register struct _Instance *enemyAttackee;
		// register: $v1
		register long mode;
		// register: $s3
		// size: 0x1AC
		register struct _MonsterVars *mv;
		{ //line: 18
		} //line: 24
	} //line: 25
} //line: 28


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x800803D4
// line start: 731
// line end:   808
int MON_ShouldIAttack(struct _Instance *instance, struct _MonsterIR *enemy, struct _MonsterAttackAttributes *attack)
{ //line: 1
	// register: $s4
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $a0
	register int rv;
	// register: $s2
	// size: 0x29C
	register struct _Instance *ei;
	{ //line: 20
		// register: $v1
		register long distance;
	} //line: 72
} //line: 78


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x8008057C
// size: 0x20
// line start: 818
// line end:   872
struct _MonsterAttackAttributes *MON_ChooseAttack(struct _Instance *instance, struct _MonsterIR *enemy)
{ //line: 2
	// register: $t1
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $t5
	// size: 0x20
	register struct _MonsterAttackAttributes *bestAttack;
	// register: $t6
	// size: 0x50
	register struct _MonsterAttributes *ma;
	// register: $t0
	// size: 0x2E
	register struct _MonsterCombatAttributes *combat;
	// register: $t3
	register long distance;
	// register: $t2
	register long zdiff;
	// register: $t4
	register long smallest;
	// register: $a3
	register int i;
	// register: $t0
	register char *attackIndex;
	{ //line: 32
		// register: $a2
		// size: 0x20
		register struct _MonsterAttackAttributes *attack;
		// register: $a0
		register long delta;
	} //line: 46
} //line: 51


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x800806DC
// line start: 878
// line end:   905
int MON_ShouldIEvade(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s3
	register int rv;
	{ //line: 8
		// register: $s2
		// size: 0x24
		register struct _MonsterIR *enemy;
	} //line: 21
} //line: 25


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x80080788
// line start: 913
// line end:   932
int MON_ChooseLeftOrRight(struct _Instance *instance, struct _MonsterIR *enemy)
{ //line: 1
	{ //line: 10
		// register: $s0
		register int leftDist;
		// register: $v1
		register int rightDist;
	} //line: 19
} //line: 20


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x8008082C
// line start: 935
// line end:   963
int MON_ChooseEvadeMove(struct _Instance *instance)
{ //line: 1
	// register: $v0
	register int anim;
	// register: $v0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $v1
	register int leftOrRight;
} //line: 29


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x80080888
// line start: 969
// line end:   1177
int MON_ChooseCombatMove(struct _Instance *instance, int reason)
{ //line: 1
	// register: $s3
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s4
	// size: 0x54
	register struct _MonsterSubAttributes *subAttr;
	// register: $s1
	register int anim;
	// register: $v0
	register int data;
	// register: $s0
	// size: 0x24
	register struct _MonsterIR *enemy;
	// register: $a0
	register int infront;
	// register: $s0
	// size: 0x10
	register struct _MonsterAnim *manim;
	{ //line: 63
		{ //line: 67
			// register: $v0
			register int dir;
		} //line: 76
	} //line: 99
	{ //line: 103
		// register: $s0
		register short zrot;
		{ //line: 139
			// address: -56
			// size: 0x6
			auto struct _Position temp;
			// address: -48
			// size: 0x8
			auto struct SVECTOR New;
			// address: -40
			// size: 0x10
			auto struct VECTOR OutTrans;
		} //line: 179
	} //line: 204
} //line: 208


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x80080CD0
// line start: 1185
// line end:   1243
void MON_PlayRandomIdle(struct _Instance *instance, int mode)
{ //line: 1
	// register: $s3
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s2
	// size: 0x50
	register struct _MonsterAttributes *ma;
	// register: $s0
	// size: 0x8
	register struct _MonsterBehavior *behavior;
	// register: $s1
	// size: 0x4
	register struct _MonsterIdle *idle;
	// register: $a2
	register int chance;
	// register: $a0
	register int i;
	// register: $a1
	register char *idleIndex;
} //line: 59


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x80080E74
// line start: 1249
// line end:   1258
void MON_PlayCombatIdle(struct _Instance *instance, int mode)
{ //line: 1
	// register: $a1
	register int anim;
} //line: 9


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x80080EB4
// line start: 1290
// line end:   1301
void MON_GetRandomPoint(struct _Position *out, struct _Position *in, short r)
{ //line: 1
	// register: $s0
	register int ang;
} //line: 1


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x80080F60
// line start: 1307
// line end:   1359
int MON_GetRandomDestinationInWorld(struct _Instance *instance, struct _Position *in, short r)
{ //line: 1
	{ //line: 4
		// register: $s1
		// size: 0x1AC
		register struct _MonsterVars *mv;
		// address: -48
		// size: 0x12
		auto struct evPhysicsLOSData data;
		// register: $s0
		register int result;
		// register: $s3
		register int avoidEnemy;
		{ //line: 15
		} //line: 21
		{ //line: 35
		} //line: 41
	} //line: 50
} //line: 53


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x80081148
// line start: 1453
// line end:   1461
void MON_MoveForward(struct _Instance *instance)
{ //line: 1
	// register: $v0
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 1


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x80081198
// line start: 1468
// line end:   1479
int MON_TurnToPosition(struct _Instance *instance, struct _Position *position, short turnspeed)
{ //line: 1
} //line: 1


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x80081208
// line start: 1485
// line end:   1491
void MON_MoveToPosition(struct _Instance *instance, struct _Position *position, short turnSpeed);

// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x8008124C
// line start: 1504
// line end:   1542
int MON_OnGround(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// address: -32
	// size: 0xC
	auto struct evPhysicsGravityData data;
	// register: $v1
	register int minUpper;
	// register: $a1
	register long xyDist;
} //line: 28


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x8008133C
// line start: 1549
// line end:   1550
void MON_ApplyPhysics(struct _Instance *instance);

// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x80081360
// line start: 1559
// line end:   1590
void MON_ChangeBehavior(struct _Instance *instance, int behavior)
{ //line: 1
	{ //line: 4
		// register: $s1
		// size: 0x1AC
		register struct _MonsterVars *mv;
		// register: $a1
		// size: 0x4
		register enum MonsterState state;
	} //line: 31
} //line: 32


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x800813E8
// line start: 1596
// line end:   1765
void MON_CheckEnvironment(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s4
	// size: 0x50
	register struct _MonsterAttributes *ma;
	// register: $v0
	register int inwater;
	// address: -40
	// size: 0x8
	auto struct _SVector Hack;
	{ //line: 41
		// register: $s2
		register int moveback;
		// register: $s3
		register int onground;
		{ //line: 68
			// register: $a1
			register int offset;
			// register: $v0
			register int result;
			// address: -32
			// size: 0x6
			auto struct _Position pos;
		} //line: 88
		{ //line: 100
			// register: $v0
			register int result;
			// register: $a1
			register int offset;
		} //line: 116
		{ //line: 123
			// register: $v1
			register int result;
		} //line: 152
	} //line: 165
} //line: 170


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x80081824
// line start: 1768
// line end:   1779
void MON_CheckTerrainAndRespond(struct _Instance *instance, struct BSPTree *bsp, struct _TFace *tface)
{ //line: 1
	// register: $s0
	register unsigned long results;
} //line: 12


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x800818A8
// line start: 1785
// line end:   1814
unsigned long MON_CheckTerrain(struct _Instance *instance, struct BSPTree *bsp, struct _TFace *tface)
{ //line: 1
	// register: $s0
	register unsigned long rv;
	{ //line: 7
		// register: $v1
		// size: 0x188
		register struct Level *level;
	} //line: 27
} //line: 29


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x80081968
// line start: 1823
// line end:   1850
int MON_CheckPointSuitability(struct _Instance *instance, struct _Position *origin, struct _Position *destination)
{ //line: 1
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s3
	register int rc;
	// register: $a0
	register int result;
	// register: $s0
	// size: 0x14
	register struct evPhysicsDropHeightData *data;
} //line: 27


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x80081A40
// line start: 1925
// line end:   1935
unsigned long MON_GetTime(struct _Instance *instance);

// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x80081A94
// line start: 1943
// line end:   2010
void MON_BirthSoul(struct _Instance *instance, int link)
{ //line: 1
	// register: $s2
	// size: 0x1AC
	register struct _MonsterVars *mv;
	{ //line: 8
		// register: $a1
		// size: 0x4C
		register struct Object *soulob;
		{ //line: 15
			// register: $s0
			// size: 0x29C
			register struct _Instance *isoul;
			{ //line: 36
				// register: $v1
				// size: 0x20
				register struct MATRIX *matrix;
			} //line: 40
		} //line: 46
	} //line: 49
	{ //line: 52
		// register: $a0
		// size: 0x29C
		register struct _Instance *isoul;
	} //line: 66
} //line: 68


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x80081BF8
// line start: 2017
// line end:   2196
void MON_ProcessIntro(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s3
	register long initialBehavior;
	// register: $s5
	register long triggeredBehavior;
	// register: $s6
	register long activeBehavior;
	// register: $s7
	register long guardRange;
	// register: $fp
	register long wanderRange;
	// register: $t1
	register long ambushRange;
	// register: $s4
	register long hitPoints;
	// address: -56
	auto int spectral;
	// register: $s1
	register long flags;
	// register: $v0
	// size: 0x50
	register struct _MonsterAttributes *ma;
	{ //line: 21
		// register: $a2
		// size: 0x4
		register struct INICommand *command;
		{ //line: 75
			// register: $a0
			register short *pmarker;
			// register: $a1
			register int i;
			// register: $v1
			register long *param;
		} //line: 90
		{ //line: 99
			// register: $a0
			register int i;
			// register: $a1
			register short *unit;
		} //line: 109
	} //line: 139
} //line: 180


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x80082124
// line start: 2202
// line end:   2259
void MON_SetDefaults(struct _Instance *instance)
{ //line: 2
	// register: $a1
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $a3
	// size: 0x50
	register struct _MonsterAttributes *ma;
	// register: $a2
	// size: 0x54
	register struct _MonsterSubAttributes *subAttr;
} //line: 58


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x80082324
// line start: 2265
// line end:   2269
void MON_GetPlanSlot(struct _MonsterVars *mv);

// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x80082368
// line start: 2276
// line end:   2387
int MON_DefaultPlanMovement(struct _Instance *instance, int anim, long distance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s1
	register int rc;
	// register: $v1
	register int felloff;
	// register: $s3
	register long length;
	// register: $s6
	// size: 0x10
	register struct _MonsterAnim *manim;
	{ //line: 30
		// address: -40
		// size: 0x6
		auto struct _Position pos;
		// register: $v1
		register int planresult;
		{ //line: 71
			// register: $a2
			register short turnSpeed;
		} //line: 88
	} //line: 88
} //line: 112


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x80082608
// line start: 2390
// line end:   2403
void MON_DropAllObjects(struct _Instance *instance)
{ //line: 1
	// register: $a0
	// size: 0x29C
	register struct _Instance *child;
	{ //line: 6
		// register: $s0
		// size: 0x29C
		register struct _Instance *next;
	} //line: 12
} //line: 14


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x80082678
// line start: 2406
// line end:   2425
void MON_EnableHeadMove(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x50
	register struct _MonsterAttributes *ma;
} //line: 20


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x80082720
// line start: 2428
// line end:   2440
void MON_DisableHeadMove(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x50
	register struct _MonsterAttributes *ma;
} //line: 13


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x800827AC
// line start: 2446
// line end:   2469
void MON_LookInDirection(struct _Instance *instance, short tx, short tz)
{ //line: 1
	// address: -32
	// size: 0x6
	auto struct _G2SVector3_Type Rot;
	// register: $s3
	// size: 0x50
	register struct _MonsterAttributes *ma;
} //line: 24


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x80082920
// line start: 2477
// line end:   2506
void MON_LookAtPos(struct _Instance *instance, struct _Position *position)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $a0
	register short z;
} //line: 29


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x800829FC
// line start: 2510
// line end:   2538
void MON_ProcessLookAt(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 29


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x80082AF0
// line start: 2544
// line end:   2575
int MON_TakeDamage(struct _Instance *instance, int damage, int type)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	{ //line: 8
		// register: $v1
		register long whatAmI;
	} //line: 28
} //line: 32


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x80082B9C
// line start: 2581
// line end:   2609
void MON_SetUpSaveInfo(struct _Instance *instance, struct _MonsterSaveInfo *saveData)
{ //line: 1
	// register: $a2
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $t1
	// size: 0x50
	register struct _MonsterAttributes *ma;
} //line: 29


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x80082D08
// line start: 2615
// line end:   2686
void MON_GetSaveInfo(struct _Instance *instance, struct _MonsterSaveInfo *saveData)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s3
	// size: 0x50
	register struct _MonsterAttributes *ma;
	{ //line: 59
		// register: $a2
		register int mode;
	} //line: 70
} //line: 72


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x80082F40
// line start: 2692
// line end:   2747
void MON_KillMonster(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	{ //line: 8
		// register: $v0
		// size: 0x50
		register struct _MonsterAttributes *attr;
		// register: $v1
		// size: 0x14
		register struct FXSplinter *splintDef;
	} //line: 16
} //line: 55


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x80083118
// line start: 2753
// line end:   2794
int MON_ShouldIAmbushEnemy(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s2
	// size: 0x24
	register struct _MonsterIR *enemy;
	{ //line: 7
		// register: $s1
		// size: 0x29C
		register struct _Instance *inst;
		// address: -32
		// size: 0x6
		auto struct _Position pos;
	} //line: 24
} //line: 42


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x80083284
// line start: 2800
// line end:   2839
int MON_ShouldIFireAtTarget(struct _Instance *instance, struct _MonsterIR *target)
{ //line: 2
	// register: $a2
	// size: 0x1AC
	register struct _MonsterVars *mv;
	{ //line: 8
		// register: $v1
		// size: 0x50
		register struct _MonsterAttributes *ma;
		// register: $a0
		// size: 0x10
		register struct _MonsterMissile *missileAttack;
		{ //line: 13
			// register: $a1
			register long distance;
			{ //line: 17
				// register: $v1
				// size: 0x24
				register struct _MonsterIR *known;
			} //line: 34
		} //line: 35
	} //line: 36
} //line: 40


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x80083374
// line start: 2845
// line end:   2869
int MON_ShouldIFlee(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $v1
	// size: 0x24
	register struct _MonsterIR *enemy;
} //line: 25


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x80083474
// line start: 2875
// line end:   2893
void MON_RelocateCoords(struct _Instance *instance, struct _SVector *offset)
{ //line: 1
	// register: $a0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $a1
	register int ox;
	// register: $a2
	register int oy;
	// register: $a3
	register int oz;
} //line: 19


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x80083500
// line start: 2899
// line end:   2915
int MON_ValidUnit(struct _Instance *instance, unsigned long unitId)
{ //line: 2
	// register: $v0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $a0
	register short *unit;
} //line: 17


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x8008354C
// line start: 2921
// line end:   2930
int MON_ValidPosition(struct _Instance *instance)
{ //line: 1
	{ //line: 2
		// register: $v1
		// size: 0x1AC
		register struct _MonsterVars *mv;
	} //line: 9
} //line: 10


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x800835A4
// line start: 2934
// line end:   2938
void MON_SphereWorldPos(struct MATRIX *mat, struct _HSphere *sphere, struct _Position *ret);

// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x80083614
// size: 0x8
// line start: 2943
// line end:   2973
struct _HPrim_duplicate_s3 *MON_FindSphereForTerrain(struct _Instance *instance)
{ //line: 2
	// register: $v0
	// size: 0x8
	register struct _HModel *hmodel;
	// register: $a3
	// size: 0x8
	register struct _HPrim_duplicate_s3 *usePrim;
	// register: $a0
	// size: 0x8
	register struct _HPrim_duplicate_s3 *currentP;
	// register: $v0
	// size: 0x14
	register struct _HSphere *tempS;
	// register: $a2
	register int maxRad;
	// register: $a1
	register int i;
} //line: 30


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x800836A0
// size: 0x4C
// line start: 2977
// line end:   3021
struct Intro *MON_FindNearestImpalingIntro(int unitID, struct _Position *position)
{ //line: 1
	// register: $v1
	// size: 0x4C
	register struct Intro *current;
	// register: $s2
	// size: 0x4C
	register struct Intro *res;
	// register: $v0
	// size: 0x188
	register struct Level *level;
	// register: $s3
	register int min_dist;
	// register: $v1
	register int dist;
	// register: $s1
	register int i;
} //line: 44


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x800837D0
// size: 0x4C
// line start: 3025
// line end:   3067
struct Intro *MON_TestForTerrainImpale(struct _Instance *instance, struct _Terrain *terrain)
{ //line: 1
	// register: $v0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s1
	// size: 0x4C
	register struct Intro *current;
	// register: $v1
	// size: 0x8
	register struct _HPrim_duplicate_s3 *prim;
	// register: $a1
	// size: 0x14
	register struct _HSphere *sphere;
	// register: $s2
	register int i;
	// register: $s3
	register int radius;
	// address: -32
	// size: 0x6
	auto struct _Position spherePos;
} //line: 43


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x800838CC
// line start: 3070
// line end:   3104
void MON_MoveInstanceToImpalePoint(struct _Instance *instance)
{ //line: 1
	// register: $v1
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $t3
	// size: 0x4C
	register struct Intro *impaler;
	// address: -16
	// size: 0x6
	auto struct _Position offset;
	{ //line: 25
		// register: $v0
		register short _x0;
		// register: $v1
		register short _y0;
		// register: $a2
		register short _z0;
		// register: $t0
		register short _y1;
		// register: $t2
		register short _z1;
		// register: $a3
		// size: 0x6
		register struct _Position *_v;
		// register: $a1
		// size: 0x6
		register struct _Position *_v1;
	} //line: 25
	{ //line: 25
		// register: $v1
		register short _x1;
		// register: $a2
		register short _y1;
		// register: $a3
		register short _z1;
		// register: $v0
		// size: 0x8
		register struct _Rotation *_v0;
		// register: $v0
		// size: 0x8
		register struct _Rotation *_v1;
	} //line: 25
} //line: 35


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x800839B0
// line start: 3119
// line end:   3235
int MON_ReachableIntro(struct _Instance *instance, struct _Position *pos, struct _Position *introPos, struct _Rotation *introRot, int checkOrientation)
{ //line: 1
	// register: $s2
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s5
	register int angle;
	// register: $s6
	register int angle2;
	// register: $v1
	register int angle3;
	// register: $v0
	register int flightAngle;
	// register: $v0
	register int length;
	// register: $s4
	register int length2;
	// register: $s0
	register int newFVel;
	// register: $s1
	register int newZVel;
	// register: $s1
	register int zDiff;
	// register: $s3
	register int zAccl;
	// register: $v1
	register int tempFVel;
	// register: $s1
	register int maxForwardVel;
	// register: $s3
	register int res;
} //line: 117


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x80083D78
// line start: 3239
// line end:   3293
int MON_SetVelocityTowardsImpalingObject(struct _Instance *instance, int checkOrientation)
{ //line: 1
	// register: $s0
	// size: 0x188
	register struct Level *level;
	// register: $s1
	// size: 0x4C
	register struct Intro *currentI;
	// register: $v0
	// size: 0x8
	register struct _HPrim_duplicate_s3 *usePrim;
	// register: $s2
	register int i;
	// address: -32
	// size: 0x6
	auto struct _Position spherePos;
	{ //line: 15
		// register: $s0
		// size: 0x29C
		register struct _Instance *target;
		{ //line: 40
			// register: $v1
			// size: 0x8
			register struct PhysObProperties *prop;
			{ //line: 43
			} //line: 48
		} //line: 49
	} //line: 51
} //line: 55


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x80083EEC
// line start: 3303
// line end:   3320
void MON_TurnOffSphereCollisions(struct _Instance *instance)
{ //line: 2
	// register: $a0
	register int i;
	// register: $v1
	// size: 0x8
	register struct _HPrim_duplicate_s3 *hprim;
	// register: $v1
	// size: 0x8
	register struct _HModel *hmodel;
} //line: 18


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x80083F48
// line start: 3323
// line end:   3340
void MON_TurnOnSphereCollisions(struct _Instance *instance)
{ //line: 2
	// register: $a0
	register int i;
	// register: $v1
	// size: 0x8
	register struct _HPrim_duplicate_s3 *hprim;
	// register: $v1
	// size: 0x8
	register struct _HModel *hmodel;
} //line: 18


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x80083FA4
// line start: 3343
// line end:   3437
void MON_ProcessSpecialFade(struct _Instance *instance)
{ //line: 1
	// register: $a0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $a1
	register int currentFadeValue;
	// register: $v1
	register int targetFadeValue;
	// register: $a3
	register int fadeRate;
	// register: $v1
	// size: 0x29C
	register struct _Instance *child;
} //line: 95


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x800841E0
// line start: 3440
// line end:   3454
void MON_StartSpecialFade(struct _Instance *instance, int fadeLevel, int fadeTime)
{ //line: 2
	// register: $a3
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $a1
	register int diff;
} //line: 15


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x80084248
// line start: 3457
// line end:   3476
void MON_UnlinkFromRaziel(struct _Instance *instance)
{ //line: 1
	// register: $v0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $v1
	// size: 0x8
	register struct evPositionData *data;
	// register: $s1
	// size: 0x29C
	register struct _Instance *enemy;
} //line: 19


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x800842E8
// line start: 3482
// line end:   3495
void MON_BurnInAir(struct _Instance *instance, int currentState)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 11


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x800843A4
// line start: 3499
// line end:   3511
void MON_BirthMana(struct _Instance *instance)
{ //line: 1
	{ //line: 4
		// register: $v0
		// size: 0x1AC
		register struct _MonsterVars *mv;
		{ //line: 8
			// register: $a0
			register int type;
			// register: $a3
			// size: 0x20
			register struct MATRIX *mat;
		} //line: 11
	} //line: 12
} //line: 13


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x8008445C
// line start: 3514
// line end:   3525
void MON_SoulSucked(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 12


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x800844D4
// line start: 3531
// line end:   3564
int MON_SetUpKnockBack(struct _Instance *instance, struct _Instance *enemy, struct evMonsterHitData *data)
{ //line: 1
	// register: $s5
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s0
	register long d;
	// register: $s4
	register long t;
	// register: $s3
	register long a;
	// register: $s0
	register long v;
	// register: $s0
	register int front;
} //line: 30


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x800845D8
// line start: 3566
// line end:   3611
void MON_DoDrainEffects(struct _Instance *instance, struct _Instance *ei)
{ //line: 1
	// register: $s3
	// size: 0x50
	register struct _MonsterAttributes *ma;
	// register: $s2
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $t2
	// size: 0x20
	register struct MATRIX *mat;
	// address: -56
	// size: 0x8
	auto struct _SVector location;
	// address: -48
	// size: 0x8
	auto struct _SVector position;
	// address: -40
	// size: 0x8
	auto struct _SVector vel;
	// address: -32
	// size: 0x8
	auto struct _SVector accel;
} //line: 46


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x80084864
// line start: 3614
// line end:   3633
void MON_SetFXHitData(struct _Instance *instance, struct evFXHitData *data, int type, int amount)
{ //line: 2
	{ //line: 6
		// register: $v0
		// size: 0x20
		register struct MATRIX *mat;
	} //line: 6
} //line: 17


// path: C:\kain2\game\MONSTER\MONLIB.C
// address: 0x800848F0
// line start: 3636
// line end:   3650
void MON_LaunchMonster(struct _Instance *instance, int zDirection, int power, int loft);

// path: C:\kain2\game\MONSTER\MONTABLE.C
// address: 0x8008D044
// line start: 376
// line end:   389
void MONTABLE_SetupTablePointer(struct Object *object)
{ //line: 2
	// register: $a1
	register long whatAmI;
} //line: 14


// path: C:\kain2\game\MONSTER\MONTABLE.C
// address: 0x8008D094
// size: 0x8
// line start: 392
// line end:   405
struct _MonsterState *MONTABLE_GetStateFuncs(struct _Instance *instance, int state)
{ //line: 2
	// register: $v0
	// size: 0x20
	register struct _MonsterFunctionTable *ft;
	{ //line: 6
		// register: $a0
		// size: 0xC
		register struct _MonsterStateChoice *choice;
	} //line: 11
} //line: 14


// path: C:\kain2\game\MONSTER\MONTABLE.C
// address: 0x8008D104
// line start: 408
// line end:   415
void (*MONTABLE_GetDamageEffectFunc(struct _Instance *instance))()
{ //line: 2
	// register: $v0
	// size: 0x20
	register struct _MonsterFunctionTable *ft;
} //line: 8


// path: C:\kain2\game\MONSTER\MONTABLE.C
// address: 0x8008D138
// line start: 418
// line end:   425
void (*MONTABLE_GetInitFunc(struct _Instance *instance))()
{ //line: 2
	// register: $v0
	// size: 0x20
	register struct _MonsterFunctionTable *ft;
} //line: 8


// path: C:\kain2\game\MONSTER\MONTABLE.C
// address: 0x8008D16C
// line start: 428
// line end:   435
void (*MONTABLE_GetCleanUpFunc(struct _Instance *instance))()
{ //line: 2
	// register: $v0
	// size: 0x20
	register struct _MonsterFunctionTable *ft;
} //line: 8


// path: C:\kain2\game\MONSTER\MONTABLE.C
// address: 0x8008D1A0
// line start: 438
// line end:   443
void MONTABLE_SetQueryFunc(struct _Instance *instance)
{ //line: 2
	// register: $v0
	// size: 0x20
	register struct _MonsterFunctionTable *ft;
} //line: 6


// path: C:\kain2\game\MONSTER\MONTABLE.C
// address: 0x8008D1D4
// line start: 446
// line end:   451
void MONTABLE_SetMessageFunc(struct _Instance *instance)
{ //line: 2
	// register: $v0
	// size: 0x20
	register struct _MonsterFunctionTable *ft;
} //line: 6


// path: C:\kain2\game\MONSTER\SLUAGH.C
// address: 0x8008D208
// line start: 48
// line end:   78
unsigned long SLUAGH_Query(struct _Instance *instance, unsigned long query)
{ //line: 1
	// register: $v1
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $a2
	// size: 0x50
	register struct _MonsterAttributes *ma;
	// register: $v0
	register unsigned long ret;
} //line: 31


// path: C:\kain2\game\MONSTER\SLUAGH.C
// address: 0x8008D29C
// line start: 84
// line end:   138
void SLUAGH_DamageEffect(struct _Instance *instance, struct evFXHitData *data)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	{ //line: 8
		// address: -40
		// size: 0x8
		auto struct _SVector accel;
	} //line: 8
	{ //line: 16
		// register: $s1
		// size: 0x2C
		register struct _FXGlowEffect *fx;
		{ //line: 20
			// address: -24
			auto long color;
			{ //line: 24
				// address: -40
				// size: 0x4
				auto struct _ColorType current;
				// address: -32
				// size: 0x4
				auto struct _ColorType target;
			} //line: 43
		} //line: 44
	} //line: 54
} //line: 55


// path: C:\kain2\game\MONSTER\SLUAGH.C
// address: 0x8008D490
// line start: 145
// line end:   164
void SLUAGH_Init(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s1
	// size: 0x50
	register struct _MonsterAttributes *ma;
	// address: -24
	auto long color;
} //line: 4


// path: C:\kain2\game\MONSTER\SLUAGH.C
// address: 0x8008D54C
// line start: 170
// line end:   192
void SLUAGH_DeathEntry(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 22


// path: C:\kain2\game\MONSTER\SLUAGH.C
// address: 0x8008D610
// line start: 195
// line end:   209
void SLUAGH_Death(struct _Instance *instance)
{ //line: 1
	{ //line: 7
		// register: $v0
		// size: 0x1AC
		register struct _MonsterVars *mv;
	} //line: 12
} //line: 14


// path: C:\kain2\game\MONSTER\SLUAGH.C
// address: 0x8008D698
// line start: 217
// line end:   228
void SLUAGH_AttackEntry(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 12


// path: C:\kain2\game\MONSTER\SLUAGH.C
// address: 0x8008D6FC
// line start: 231
// line end:   277
void SLUAGH_Attack(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
	{ //line: 6
		// register: $a1
		// size: 0x8
		register struct __Event *mess;
		{ //line: 22
			// register: $s0
			// size: 0x50
			register struct _MonsterAttributes *ma;
			// address: -24
			auto long color;
		} //line: 28
	} //line: 41
} //line: 47


// path: C:\kain2\game\MONSTER\SOUL.C
// address: 0x8008D890
// line start: 80
// line end:   94
void SOUL_QueueHandler(struct _Instance *instance)
{ //line: 1
	// register: $s2
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $a1
	// size: 0x8
	register struct __Event *message;
} //line: 15


// path: C:\kain2\game\MONSTER\SOUL.C
// address: 0x8008D92C
// line start: 100
// line end:   117
void SOUL_Physics(struct _Instance *instance, long time)
{ //line: 1
	// register: $v1
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $a1
	register int a;
} //line: 14


// path: C:\kain2\game\MONSTER\SOUL.C
// address: 0x8008D98C
// line start: 120
// line end:   144
void SOUL_Fade(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $a0
	register unsigned long time;
} //line: 25


// path: C:\kain2\game\MONSTER\SOUL.C
// address: 0x8008DA40
// line start: 147
// line end:   158
void SOUL_MoveToDest(struct _Instance *instance, long maxAccel, long time)
{ //line: 1
	// register: $a1
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 11


// path: C:\kain2\game\MONSTER\SOUL.C
// address: 0x8008DB30
// line start: 162
// line end:   187
void SOUL_MovePastWall(struct _Instance *instance, struct _Instance *sucker)
{ //line: 1
	// address: -88
	// size: 0x2C
	auto struct _PCollideInfo pcollideInfo;
	// address: -40
	// size: 0x6
	auto struct _Position newPos;
	// address: -32
	// size: 0x6
	auto struct _Position oldPos;
	// address: -24
	// size: 0x6
	auto struct _Position delta;
	// register: $a2
	// size: 0x20
	register struct MATRIX *to;
	// register: $v1
	// size: 0x20
	register struct MATRIX *from;
	{ //line: 1
		// register: $v0
		register short _x0;
		// register: $v1
		register short _y0;
		// register: $a2
		register short _z0;
		// register: $a3
		register short _x1;
		// register: $t1
		register short _y1;
		// register: $t0
		register short _z1;
		// register: $s1
		// size: 0x6
		register struct _Position *_v;
		// register: $t0
		// size: 0x6
		register struct _Position *_v1;
	} //line: 1
} //line: 1


// path: C:\kain2\game\MONSTER\SOUL.C
// address: 0x8008DC24
// line start: 193
// line end:   216
void SOUL_Init(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 24


// path: C:\kain2\game\MONSTER\SOUL.C
// address: 0x8008DCE8
// line start: 223
// line end:   226
void SOUL_CleanUp(struct _Instance *instance);

// path: C:\kain2\game\MONSTER\SOUL.C
// address: 0x8008DD08
// line start: 233
// line end:   241
void SOUL_BirthEntry(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 1


// path: C:\kain2\game\MONSTER\SOUL.C
// address: 0x8008DD5C
// line start: 244
// line end:   256
void SOUL_Birth(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 12


// path: C:\kain2\game\MONSTER\SOUL.C
// address: 0x8008DDC4
// line start: 262
// line end:   265
void SOUL_SoulSuckEntry(struct _Instance *instance);

// path: C:\kain2\game\MONSTER\SOUL.C
// address: 0x8008DDEC
// line start: 270
// line end:   297
long SOUL_CalcAccel(long delta, long vel, long magnitude)
{ //line: 2
	// register: $a2
	register long rv;
} //line: 27


// path: C:\kain2\game\MONSTER\SOUL.C
// address: 0x8008DE2C
// line start: 300
// line end:   455
void SOUL_SoulSuck(struct _Instance *instance)
{ //line: 1
	// register: $s3
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s0
	// size: 0x8
	register struct __Event *message;
	// register: $s4
	// size: 0x29C
	register struct _Instance *sucker;
	// register: $s6
	// size: 0x29C
	register struct _Instance *collidedWith;
	// register: $s5
	register int collisionCnt;
	{ //line: 17
		// register: $s1
		register long distance;
		// register: $s0
		// size: 0xC
		register struct evMonsterSoulSuckData *suckData;
		{ //line: 44
			// address: -48
			// size: 0x8
			auto struct _SVector dir;
			{ //line: 55
				// register: $s0
				register long animLen;
				// register: $v0
				register long curTim;
				{ //line: 66
					// register: $a3
					register short _x0;
					// register: $a0
					register short _y0;
					// register: $v1
					register short _z0;
					// register: $v0
					register short _x1;
					// register: $a2
					register short _y1;
					// register: $a1
					register short _z1;
					// register: $s7
					// size: 0x8
					register struct _SVector *_v;
					// register: $v1
					// size: 0x6
					register struct _Position *_v0;
					// register: $a1
					// size: 0x6
					register struct _Position *_v1;
				} //line: 66
			} //line: 76
		} //line: 76
	} //line: 85
	{ //line: 139
		// register: $v0
		// size: 0x29C
		register struct _Instance *body;
	} //line: 143
} //line: 156


// path: C:\kain2\game\MONSTER\SOUL.C
// address: 0x8008E3D4
// line start: 461
// line end:   473
void SOUL_WanderEntry(struct _Instance *instance)
{ //line: 1
	// register: $a2
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 13


// path: C:\kain2\game\MONSTER\SOUL.C
// address: 0x8008E444
// line start: 476
// line end:   517
void SOUL_Wander(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
	{ //line: 13
	} //line: 26
} //line: 41


// path: C:\kain2\game\MONSTER\SOUL.C
// address: 0x8008E5D4
// line start: 523
// line end:   526
void SOUL_FleeEntry(struct _Instance *instance);

// path: C:\kain2\game\MONSTER\SOUL.C
// address: 0x8008E604
// line start: 530
// line end:   560
void SOUL_Flee(struct _Instance *instance)
{ //line: 1
	// register: $v0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	{ //line: 10
		// register: $a0
		// size: 0x29C
		register struct _Instance *enemy;
		// register: $s1
		register int dx;
		// register: $v0
		register int dy;
	} //line: 25
} //line: 31


// path: C:\kain2\game\MONSTER\SOUL.C
// address: 0x8008E768
// line start: 566
// line end:   571
void SOUL_IdleEntry(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 1


// path: C:\kain2\game\MONSTER\SOUL.C
// address: 0x8008E80C
// line start: 574
// line end:   610
void SOUL_Idle(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $a1
	register long xAccl;
	// register: $a0
	register long yAccl;
} //line: 36


// path: C:\kain2\game\MONSTER\SOUL.C
// address: 0x8008E90C
// line start: 616
// line end:   630
void SOUL_ReanimateEntry(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $v0
	// size: 0x29C
	register struct _Instance *body;
} //line: 1


// path: C:\kain2\game\MONSTER\SOUL.C
// address: 0x8008E974
// line start: 633
// line end:   657
void SOUL_Reanimate(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	{ //line: 13
		// register: $v0
		// size: 0x29C
		register struct _Instance *body;
	} //line: 17
} //line: 25


// path: C:\kain2\game\MONSTER\SOUL.C
// address: 0x8008EA2C
// line start: 663
// line end:   665
void SOUL_Effect(struct _Instance *instance, struct evFXHitData *data);

// path: C:\kain2\game\MONSTER\MONSENSE.C
// address: 0x80085C70
// size: 0x24
// line start: 64
// line end:   72
struct _MonsterIR *MONSENSE_FindIR(struct _MonsterVars *mv, struct _Instance *instance)
{ //line: 2
	// register: $v1
	// size: 0x24
	register struct _MonsterIR *mir;
} //line: 8


// path: C:\kain2\game\MONSTER\MONSENSE.C
// address: 0x80085CA8
// line start: 97
// line end:   147
int MONSENSE_See(struct _Instance *instance, struct evCollideInstanceStatsData *data)
{ //line: 1
	// register: $v0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s0
	// size: 0x12
	register struct _MonsterSenses *senses;
	{ //line: 7
		// register: $a1
		register int arc;
		// register: $a2
		register int elevation;
		{ //line: 17
			// register: $s0
			// size: 0x29C
			register struct _Instance *target;
			{ //line: 21
				// address: -40
				// size: 0x12
				auto struct evPhysicsLOSData losData;
				{ //line: 25
					// register: $v0
					// size: 0x20
					register struct MATRIX *mat;
				} //line: 25
				{ //line: 31
					// register: $v1
					register short _x1;
					// register: $a0
					register short _y1;
					// register: $a1
					register short _z1;
					// register: $v0
					// size: 0x6
					register struct _Position *_v0;
					// register: $v0
					// size: 0x6
					register struct _Position *_v1;
				} //line: 31
				{ //line: 35
					// register: $v1
					// size: 0x20
					register struct MATRIX *mat;
				} //line: 35
				{ //line: 41
					// register: $v1
					register short _x1;
					// register: $a0
					register short _y1;
					// register: $a1
					register short _z1;
					// register: $v0
					// size: 0x6
					register struct _Position *_v0;
					// register: $v0
					// size: 0x6
					register struct _Position *_v1;
				} //line: 41
			} //line: 44
		} //line: 47
	} //line: 48
} //line: 51


// path: C:\kain2\game\MONSTER\MONSENSE.C
// address: 0x80085E44
// line start: 153
// line end:   170
int MONSENSE_Hear(struct _Instance *instance, struct evCollideInstanceStatsData *data)
{ //line: 1
	// register: $v0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s0
	// size: 0x12
	register struct _MonsterSenses *senses;
	// register: $a0
	register long mode;
} //line: 18


// path: C:\kain2\game\MONSTER\MONSENSE.C
// address: 0x80085ED4
// line start: 176
// line end:   183
int MONSENSE_Smell(struct _Instance *instance, struct evCollideInstanceStatsData *data)
{ //line: 2
	// register: $v0
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 2


// path: C:\kain2\game\MONSTER\MONSENSE.C
// address: 0x80085EFC
// size: 0x24
// line start: 186
// line end:   226
struct _MonsterIR *MONSENSE_FirstSense(struct _Instance *instance, struct _Instance *sensed)
{ //line: 1
	// register: $s0
	// size: 0x24
	register struct _MonsterIR *mir;
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
	{ //line: 8
		// register: $s2
		register long whatAmI;
		// register: $s3
		// size: 0x10
		register struct _MonsterAllegiances *allegiances;
	} //line: 38
} //line: 40


// path: C:\kain2\game\MONSTER\MONSENSE.C
// address: 0x80086094
// line start: 233
// line end:   367
void MONSENSE_SetupMIR(struct _Instance *instance, struct evCollideInstanceStatsData *data, int flags)
{ //line: 1
	// register: $s0
	// size: 0x24
	register struct _MonsterIR *mir;
	// register: $s4
	// size: 0x1AC
	register struct _MonsterVars *mv;
	{ //line: 20
		// register: $s1
		register long mode;
		{ //line: 23
			// register: $v1
			register short _x1;
			// register: $a0
			register short _y1;
			// register: $a1
			register short _z1;
			// register: $v0
			// size: 0x8
			register struct _SVector *_v0;
			// register: $v0
			// size: 0x8
			register struct _SVector *_v1;
		} //line: 23
		{ //line: 68
			// register: $s2
			register int run;
			// register: $a0
			register int distance;
			{ //line: 79
			} //line: 86
		} //line: 103
	} //line: 134
} //line: 135


// path: C:\kain2\game\MONSTER\MONSENSE.C
// address: 0x800864C0
// line start: 374
// line end:   418
void MONSENSE_SenseInstance(struct _Instance *instance, struct evCollideInstanceStatsData *data)
{ //line: 1
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s0
	register long flags;
	// register: $s4
	register unsigned long whatAmI;
	{ //line: 13
		// register: $s0
		// size: 0x8
		register struct PhysObProperties *prop;
		{ //line: 16
		} //line: 27
	} //line: 27
} //line: 45


// path: C:\kain2\game\MONSTER\MONSENSE.C
// address: 0x800866B4
// line start: 424
// line end:   470
void MONSENSE_StartMonsterIRList(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s0
	// size: 0x24
	register struct _MonsterIR *mir;
	// register: $s2
	// size: 0x24
	register struct _MonsterIR *newlist;
	// register: $s4
	register int enemyOnList;
	{ //line: 11
		// register: $s3
		// size: 0x24
		register struct _MonsterIR *next;
	} //line: 39
} //line: 47


// path: C:\kain2\game\MONSTER\MONSENSE.C
// address: 0x800867C8
// size: 0x24
// line start: 476
// line end:   488
struct _MonsterIR *MONSENSE_GetMonsterIR(struct _MonsterVars *mv)
{ //line: 2
	// register: $v1
	// size: 0x24
	register struct _MonsterIR *mir;
} //line: 12


// path: C:\kain2\game\MONSTER\MONSENSE.C
// address: 0x800867F0
// line start: 494
// line end:   506
void MONSENSE_InitIRList(struct _MonsterVars *mv, struct _MonsterIR *list, int num)
{ //line: 2
	// register: $a2
	register int i;
} //line: 11


// path: C:\kain2\game\MONSTER\MONSENSE.C
// address: 0x8008681C
// size: 0x24
// line start: 512
// line end:   548
struct _MonsterIR *MONSENSE_SetEnemy(struct _Instance *instance, struct _Instance *newenemy)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $a2
	// size: 0x24
	register struct _MonsterIR *mir;
	{ //line: 20
		// register: $v0
		register long angle;
	} //line: 26
} //line: 36


// path: C:\kain2\game\MONSTER\MONSENSE.C
// address: 0x8008693C
// line start: 554
// line end:   613
void MONSENSE_ProcessIRList(struct _Instance *instance)
{ //line: 1
	// register: $s4
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s0
	// size: 0x24
	register struct _MonsterIR *mir;
	// register: $s1
	// size: 0x24
	register struct _MonsterIR *closestEnemy;
	// register: $s2
	// size: 0x24
	register struct _MonsterIR *closestLeader;
	// register: $s3
	// size: 0x24
	register struct _MonsterIR *closestAlly;
	// register: $s6
	// size: 0x24
	register struct _MonsterIR *player;
	// register: $s5
	// size: 0x2E
	register struct _MonsterCombatAttributes *combatAttr;
} //line: 58


// path: C:\kain2\game\MONSTER\MONSENSE.C
// address: 0x80086B5C
// line start: 630
// line end:   696
void MONSENSE_SetupSenses(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s0
	// size: 0x10
	register struct _MonsterAllegiances *allegiances;
	// register: $v0
	register int num;
} //line: 67


// path: C:\kain2\game\MONSTER\MONSENSE.C
// address: 0x80086CC8
// line start: 702
// line end:   722
void MONSENSE_RemoveSenses(struct _Instance *instance)
{ //line: 2
	// register: $a0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $a1
	register int i;
} //line: 21


// path: C:\kain2\game\MONSTER\MONSENSE.C
// address: 0x80086D68
// line start: 847
// line end:   918
void MONSENSE_Radar(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// address: -96
	// size: 0x8
	auto struct SVECTOR newPos;
	// address: -88
	// size: 0x8
	auto struct SVECTOR oldPos;
	// address: -80
	// size: 0x2C
	auto struct _PCollideInfo pCollideInfo;
	// register: $s3
	register int index;
} //line: 72


// path: C:\kain2\game\MONSTER\MONSENSE.C
// address: 0x80086FA0
// line start: 924
// line end:   957
short MONSENSE_GetClosestFreeDirection(struct _Instance *instance, short angle, long range)
{ //line: 1
	// register: $t0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $a1
	register int bit;
	// register: $a0
	register int search;
} //line: 34


// path: C:\kain2\game\MONSTER\MONSENSE.C
// address: 0x800870A4
// line start: 963
// line end:   968
int MONSENSE_GetDistanceInDirection(struct _Instance *instance, short angle)
{ //line: 2
	// register: $v1
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $v0
	register int bit;
} //line: 3


// path: C:\kain2\game\MONSTER\MONSENSE.C
// address: 0x800870DC
// line start: 974
// line end:   1004
void MONSENSE_DoSenses(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// address: 68
	static int doneThisFrame;
} //line: 31


// path: C:\kain2\game\MONSTER\MONSENSE.C
// address: 0x800871D4
// line start: 1010
// line end:   1065
void MONSENSE_AdjustRadarFromObjects(struct _Instance *instance)
{ //line: 1
	// register: $s4
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s2
	// size: 0x29C
	register struct _Instance *physob;
	{ //line: 9
		// register: $s1
		// size: 0x8
		register struct PhysObProperties *prop;
		{ //line: 12
			{ //line: 18
				// register: $s0
				register long dist;
				{ //line: 23
					// register: $v1
					register int i;
					// register: $a1
					register int min;
					// register: $a2
					register int max;
					// register: $s0
					register int ang;
				} //line: 50
			} //line: 51
		} //line: 53
	} //line: 54
} //line: 56


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x800873B8
// line start: 101
// line end:   141
void MON_DoCombatTimers(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s1
	register unsigned long curtime;
	{ //line: 16
		// register: $v0
		// size: 0x2E
		register struct _MonsterCombatAttributes *combat;
	} //line: 25
} //line: 41


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x800875A0
// line start: 147
// line end:   188
void MON_ChangeHumanOpinion(struct _Instance *instance)
{ //line: 1
	// register: $v0
	register int good;
	{ //line: 25
		// register: $s0
		register int opinion;
	} //line: 41
} //line: 42


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x80087648
// line start: 192
// line end:   237
void MON_CutOut_Monster(struct _Instance *instance, int fade_amount, int startseg, int endseg)
{ //line: 1
	// address: -56
	// size: 0x8
	auto struct _SVector point;
	// address: -48
	// size: 0x8
	auto struct _SVector normal;
	// address: -40
	// size: 0x8
	auto struct _SVector p1;
	// address: -32
	// size: 0x8
	auto struct _SVector p2;
	// register: $t0
	register int tmp;
	// register: $a0
	// size: 0x20
	register struct MATRIX *mat;
	{ //line: 29
		// address: -24
		auto long color;
		{ //line: 29
			// register: $v0
			register short _x0;
			// register: $v1
			register short _y0;
			// register: $a1
			register short _z0;
			// register: $a2
			register short _x1;
			// register: $a3
			register short _y1;
			// register: $t0
			register short _z1;
			// register: $a0
			// size: 0x8
			register struct _SVector *_v;
		} //line: 29
	} //line: 41
} //line: 46


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008789C
// line start: 249
// line end:   277
void MON_DeadEntry(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 29


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008798C
// line start: 280
// line end:   521
void MON_Dead(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s5
	// size: 0x29C
	register struct _Instance *soul;
	// register: $v0
	// size: 0x8
	register struct __Event *message;
	// register: $s6
	register int reanimate;
	{ //line: 24
		// register: $s0
		// size: 0x50
		register struct _MonsterAttributes *ma;
		// register: $a1
		register int fade_amount;
	} //line: 36
	{ //line: 56
		// register: $s0
		// size: 0x38
		register struct _Model *model;
		// register: $s0
		register int i;
		// register: $v0
		// size: 0x4
		register struct CVECTOR *ptr;
	} //line: 64
	{ //line: 67
		// register: $s0
		// size: 0x29C
		register struct _Instance *imp;
		// register: $s3
		// size: 0x29C
		register struct _Instance *closest;
		{ //line: 71
			// register: $s4
			register long closestdist;
			{ //line: 80
				// register: $v1
				register long dist;
			} //line: 87
		} //line: 90
	} //line: 103
	{ //line: 122
		// address: -96
		// size: 0x8
		auto struct SVECTOR New;
		// address: -88
		// size: 0x8
		auto struct SVECTOR Old;
		// address: -80
		// size: 0x2C
		auto struct _PCollideInfo pcinfo;
	} //line: 141
	{ //line: 168
		// register: $a0
		// size: 0x50
		register struct _MonsterAttributes *ma;
	} //line: 202
} //line: 242


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x800880C0
// line start: 527
// line end:   548
void MON_MissileHitEntry(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 1


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008818C
// line start: 551
// line end:   573
void MON_MissileHit(struct _Instance *instance)
{ //line: 1
	// register: $s3
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s0
	// size: 0x50
	register struct _MonsterAttributes *ma;
	// register: $s2
	register int thisFrame;
	// register: $a0
	register int lastFrame;
} //line: 23


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008826C
// line start: 579
// line end:   580
void MON_BirthEntry(struct _Instance *instance);

// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x80088274
// line start: 583
// line end:   584
void MON_Birth(struct _Instance *instance);

// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x80088294
// line start: 591
// line end:   612
void MON_ParryEntry(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 1


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x800882E4
// line start: 615
// line end:   621
void MON_Parry(struct _Instance *instance);

// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x80088328
// line start: 628
// line end:   658
void MON_LandOnFeetEntry(struct _Instance *instance)
{ //line: 1
	// register: $v0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	{ //line: 18
		// register: $a1
		register int damage;
	} //line: 28
} //line: 29


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x800883EC
// line start: 661
// line end:   724
void MON_LandOnFeet(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	{ //line: 19
		// register: $a1
		register int state;
	} //line: 50
} //line: 64


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x80088578
// line start: 730
// line end:   731
void MON_LandInWaterEntry(struct _Instance *instance);

// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008858C
// line start: 735
// line end:   736
void MON_LandInWater(struct _Instance *instance);

// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x800885AC
// line start: 743
// line end:   752
void MON_BreakHoldEntry(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 10


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x80088618
// line start: 755
// line end:   813
void MON_BreakHold(struct _Instance *instance)
{ //line: 1
	{ //line: 8
		// register: $s3
		// size: 0x1AC
		register struct _MonsterVars *mv;
		// register: $a1
		// size: 0x8
		register struct __Event *message;
		{ //line: 18
			// register: $s0
			// size: 0x10
			register struct evMonsterThrownData *data;
		} //line: 18
		{ //line: 25
			// register: $v0
			// size: 0x10
			register struct evMonsterHitTerrainData *data;
		} //line: 27
	} //line: 53
} //line: 59


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x800887F4
// line start: 819
// line end:   842
void MON_ImpactEntry(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s2
	// size: 0x2E
	register struct _MonsterCombatAttributes *combat;
	// address: -40
	// size: 0x14
	auto struct evFXHitData data;
} //line: 23


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x800888D8
// line start: 845
// line end:   865
void MON_Impact(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 21


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x80088958
// line start: 871
// line end:   898
void MON_FallEntry(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 22


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x80088A18
// line start: 901
// line end:   962
void MON_Fall(struct _Instance *instance)
{ //line: 1
	// register: $s3
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $a1
	// size: 0x8
	register struct __Event *message;
	// register: $s2
	register int state;
	{ //line: 28
		// register: $s0
		register int x;
		// register: $v0
		register int y;
	} //line: 33
} //line: 62


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x80088B94
// line start: 968
// line end:   986
void MON_ThrownEntry(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 17


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x80088C30
// line start: 989
// line end:   1049
void MON_Thrown(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $v0
	// size: 0x4C
	register struct Intro *impaler;
	{ //line: 7
	} //line: 17
	{ //line: 23
		// register: $a1
		// size: 0x8
		register struct __Event *message;
		{ //line: 35
			// register: $v0
			// size: 0x10
			register struct evMonsterHitTerrainData *data;
		} //line: 35
	} //line: 60
} //line: 61


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x80088D88
// line start: 1055
// line end:   1070
void MON_ImpaleDeathEntry(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 1


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x80088E08
// line start: 1073
// line end:   1126
void MON_ImpaleDeath(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s2
	// size: 0x50
	register struct _MonsterAttributes *ma;
	// register: $v0
	// size: 0x8
	register struct __Event *message;
	{ //line: 8
		// register: $s3
		register int thisFrame;
		// register: $a0
		register int lastFrame;
	} //line: 19
} //line: 54


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x80089010
// line start: 1132
// line end:   1173
void MON_TerrainImpaleDeathEntry(struct _Instance *instance)
{ //line: 1
	// register: $s2
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s0
	// size: 0x50
	register struct _MonsterAttributes *ma;
	// register: $s0
	// size: 0x4C
	register struct Intro *impaler;
} //line: 41


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x80089164
// line start: 1176
// line end:   1241
void MON_TerrainImpaleDeath(struct _Instance *instance)
{ //line: 1
	// register: $s2
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $v1
	// size: 0x4C
	register struct Intro *impaler;
	{ //line: 25
	} //line: 29
	{ //line: 36
		// register: $s1
		// size: 0x50
		register struct _MonsterAttributes *ma;
		// register: $s0
		register int fade_amount;
	} //line: 54
} //line: 66


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008934C
// line start: 1247
// line end:   1255
void MON_SurprisedEntry(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 1


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x800893AC
// line start: 1258
// line end:   1279
void MON_Surprised(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 21


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x80089470
// line start: 1285
// line end:   1329
void MON_StunnedEntry(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s2
	// size: 0x2E
	register struct _MonsterCombatAttributes *combat;
} //line: 43


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x80089620
// line start: 1332
// line end:   1362
void MON_Stunned(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 31


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x80089730
// line start: 1368
// line end:   1395
void MON_GrabbedEntry(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 17


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x80089814
// line start: 1398
// line end:   1465
void MON_Grabbed(struct _Instance *instance)
{ //line: 1
	// register: $s2
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $a1
	// size: 0x8
	register struct __Event *message;
	// register: $v1
	register int time;
	{ //line: 26
		// register: $s0
		// size: 0x10
		register struct evMonsterThrownData *data;
	} //line: 26
	{ //line: 33
		// register: $v0
		// size: 0x10
		register struct evMonsterHitTerrainData *data;
	} //line: 35
} //line: 68


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x80089A2C
// line start: 1471
// line end:   1491
void MON_HitEntry(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $a0
	// size: 0x24
	register struct _MonsterIR *enemy;
} //line: 15


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x80089AD0
// line start: 1494
// line end:   1518
void MON_Hit(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 25


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x80089BB8
// line start: 1524
// line end:   1537
void MON_AttackEntry(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s2
	// size: 0x20
	register struct _MonsterAttackAttributes *attack;
} //line: 1


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x80089C2C
// line start: 1540
// line end:   1584
void MON_Attack(struct _Instance *instance)
{ //line: 1
	// register: $s3
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s4
	// size: 0x24
	register struct _MonsterIR *enemy;
	// register: $s0
	// size: 0x20
	register struct _MonsterAttackAttributes *attack;
	// register: $s1
	// size: 0x10
	register struct _MonsterAnim *anim;
} //line: 44


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x80089E0C
// line start: 1590
// line end:   1600
void MON_CombatEntry(struct _Instance *instance)
{ //line: 2
	// register: $a0
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 7


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x80089E38
// line start: 1603
// line end:   1709
void MON_Combat(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s3
	// size: 0x24
	register struct _MonsterIR *enemy;
	{ //line: 19
		// register: $s2
		// size: 0x2E
		register struct _MonsterCombatAttributes *combat;
		{ //line: 29
			// register: $a1
			register int reason;
			{ //line: 44
				// register: $v0
				register int anim;
			} //line: 48
			{ //line: 59
			} //line: 62
			{ //line: 73
				// register: $v0
				register int anim;
			} //line: 84
		} //line: 88
		{ //line: 92
			// register: $a1
			register int state;
		} //line: 103
	} //line: 104
} //line: 106


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008A120
// line start: 1715
// line end:   1724
void MON_ProjectileEntry(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $v1
	// size: 0x50
	register struct _MonsterAttributes *ma;
	// register: $a1
	// size: 0x10
	register struct _MonsterMissile *missileDef;
} //line: 1


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008A178
// line start: 1727
// line end:   1757
void MON_Projectile(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $v1
	// size: 0x50
	register struct _MonsterAttributes *ma;
	// register: $s0
	// size: 0x10
	register struct _MonsterMissile *missileDef;
} //line: 30


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008A2BC
// line start: 1766
// line end:   1791
void MON_IdleEntry(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	{ //line: 11
		// register: $v1
		// size: 0x50
		register struct _MonsterAttributes *ma;
	} //line: 16
} //line: 26


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008A374
// line start: 1794
// line end:   1848
void MON_Idle(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s4
	register int combatIdle;
	{ //line: 7
		// register: $s1
		register int state;
		// register: $s2
		// size: 0x24
		register struct _MonsterIR *enemy;
	} //line: 37
} //line: 54


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008A5B0
// line start: 1854
// line end:   1866
void MON_FleeEntry(struct _Instance *instance)
{ //line: 1
	// register: $a1
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 1


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008A604
// line start: 1869
// line end:   1903
void MON_Flee(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	{ //line: 27
	} //line: 34
} //line: 35


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008A748
// line start: 1909
// line end:   1927
void MON_PursueEntry(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 16


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008A7D8
// line start: 1930
// line end:   2170
void MON_Pursue(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $a1
	register int anim;
	// register: $v1
	register int planresult;
	// register: $s6
	register long range;
	// register: $s4
	// size: 0x24
	register struct _MonsterIR *target;
	// register: $s2
	register int pursueType;
	{ //line: 24
		{ //line: 30
		} //line: 36
		{ //line: 68
			// register: $a2
			// size: 0x2E
			register struct _MonsterCombatAttributes *combat;
			{ //line: 72
				// register: $a1
				// size: 0x24
				register struct _MonsterIR *ally;
			} //line: 93
			{ //line: 130
			} //line: 138
		} //line: 140
		{ //line: 152
			// register: $s5
			register int state;
		} //line: 223
	} //line: 240
} //line: 241


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008AC10
// line start: 2176
// line end:   2196
void MON_WanderEntry(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 17


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008ACB8
// line start: 2199
// line end:   2296
void MON_Wander(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
	{ //line: 6
	} //line: 11
	{ //line: 21
		// register: $s0
		register int markerID;
	} //line: 25
	{ //line: 33
		// register: $a1
		// size: 0x6
		register struct _Position *target;
		// register: $a2
		register short range;
	} //line: 47
	{ //line: 53
		// register: $v1
		register int planresult;
		// register: $a1
		register int state;
	} //line: 90
} //line: 98


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008AF30
// line start: 2302
// line end:   2304
void MON_HideEntry(struct _Instance *instance);

// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008AF50
// line start: 2308
// line end:   2357
void MON_Hide(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s2
	// size: 0x24
	register struct _MonsterIR *enemy;
} //line: 49


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008B0CC
// line start: 2363
// line end:   2389
void MON_SurpriseAttackEntry(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $v0
	// size: 0x2E
	register struct _MonsterCombatAttributes *combatAttr;
	// register: $v1
	// size: 0x50
	register struct _MonsterAttributes *ma;
} //line: 27


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008B160
// line start: 2392
// line end:   2393
void MON_SurpriseAttack(struct _Instance *instance);

// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008B180
// line start: 2400
// line end:   2401
void MON_EnvironmentDamageEntry(struct _Instance *instance);

// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008B188
// line start: 2404
// line end:   2405
void MON_EnvironmentDamage(struct _Instance *instance);

// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008B190
// line start: 2408
// line end:   2430
void MON_MonsterGlow(struct _Instance *instance, long color, int glowtime, int glowin, int glowfade)
{ //line: 1
	// register: $v0
	// size: 0x2C
	register struct _FXGlowEffect *glow;
	// register: $s4
	// size: 0x50
	register struct _MonsterAttributes *ma;
} //line: 9


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008B31C
// line start: 2436
// line end:   2521
void MON_GeneralDeathEntry(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s2
	// size: 0x50
	register struct _MonsterAttributes *ma;
} //line: 75


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008B568
// line start: 2524
// line end:   2577
void MON_GeneralDeath(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $v0
	// size: 0x8
	register struct __Event *message;
	// register: $s2
	register int dead;
} //line: 54


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008B6E0
// line start: 2583
// line end:   2595
void MON_NoticeEntry(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 13


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008B798
// line start: 2598
// line end:   2616
void MON_Notice(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 18


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008B838
// line start: 2622
// line end:   2629
void MON_PupateEntry(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 1


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008B898
// line start: 2632
// line end:   2727
void MON_Pupate(struct _Instance *instance)
{ //line: 1
	// register: $s3
	// size: 0x1AC
	register struct _MonsterVars *mv;
	{ //line: 7
		// register: $s0
		// size: 0x50
		register struct _MonsterAttributes *ma;
		// size: 0x4
		static unsigned char pupateObjects[4];
		// register: $s6
		// size: 0x4C
		register struct Object *cocoonOb;
		// register: $s1
		// size: 0x29C
		register struct _Instance *cocoon;
		// register: $s4
		// size: 0x29C
		register struct _Instance *closest;
		// register: $s5
		register long closestDist;
		// register: $s7
		register int burst;
		{ //line: 25
			// register: $v1
			register long dist;
		} //line: 43
	} //line: 80
} //line: 96


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008BBD8
// line start: 2733
// line end:   2739
void MON_EmbraceEntry(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 1


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008BC38
// line start: 2742
// line end:   2772
void MON_Embrace(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s1
	// size: 0x24
	register struct _MonsterIR *enemy;
	{ //line: 16
		// register: $s2
		// size: 0x29C
		register struct _Instance *ei;
	} //line: 20
} //line: 31


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008BDB8
// line start: 2857
// line end:   2862
void MON_PetrifiedEntry(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
} //line: 1


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008BDF4
// line start: 2865
// line end:   2888
void MON_Petrified(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $v0
	// size: 0x8
	register struct __Event *message;
	// register: $v1
	register int time;
} //line: 24


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008BF00
// line start: 2897
// line end:   2898
int MONSTER_CalcDamageIntensity(int hp, int maxHp);

// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008BF18
// line start: 2907
// line end:   2959
void MONSTER_ProcessClosestVerts(struct _Instance *instance, struct _SVector *location, void (*processVert_cb)(), void *cb_data)
{ //line: 1
	// register: $s2
	register int i;
	// register: $s7
	// size: 0x38
	register struct _Model *model;
	// register: $s4
	// size: 0x8
	register struct _MVertex *vertexList;
	// address: -96
	// size: 0xC
	auto struct _Vector locVec;
	// address: -48
	auto long flag;
	{ //line: 18
		// register: $s0
		// size: 0x8
		register struct _MVertex *firstVertex;
		// register: $s5
		// size: 0x8
		register struct _MVertex *lastVertex;
		// register: $s0
		// size: 0x8
		register struct _MVertex *modelVertex;
		// address: -80
		// size: 0x20
		auto struct MATRIX inverse;
		// register: $a3
		register int tmp;
		// register: $a0
		register int dist;
	} //line: 51
} //line: 53


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008C0E0
// line start: 2967
// line end:   3014
void ProcessBloodyMess(struct _Instance *instance, int vertidx, int segidx, int dist, void *cb_data)
{ //line: 1
	// register: $t0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	{ //line: 21
		// register: $a2
		register long scl;
		// register: $a0
		// size: 0x4
		register struct CVECTOR *cv;
	} //line: 47
} //line: 48


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008C1EC
// line start: 3017
// line end:   3026
void MONSTER_InitVertexColors(struct _Instance *instance, struct _Model *model)
{ //line: 1
	// register: $s0
	register int i;
	// register: $v0
	// size: 0x4
	register struct CVECTOR *ptr;
} //line: 10


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008C254
// line start: 3029
// line end:   3078
int MONSTER_StartVertexBlood(struct _Instance *instance, struct _SVector *location, int amount)
{ //line: 1
	// register: $s1
	// size: 0x38
	register struct _Model *model;
	// address: -88
	// size: 0x14
	auto struct bloodyMessType bmt;
	{ //line: 12
		// address: -64
		// size: 0x8
		auto struct _SVector localloc;
		{ //line: 23
			// register: $s1
			// size: 0x8
			register struct _MVertex *vertexList;
			// address: -56
			// size: 0xC
			auto struct _Vector locVec;
			// address: -40
			auto long flag;
			// register: $s0
			// size: 0x20
			register struct MATRIX *segMatrix;
		} //line: 23
	} //line: 46
} //line: 50


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008C3DC
// line start: 3081
// line end:   3156
void MONSTER_VertexBlood(struct _Instance *instance, int vert, int amount)
{ //line: 1
	// register: $v1
	register int i;
	// register: $t0
	// size: 0x38
	register struct _Model *model;
	{ //line: 7
		// register: $a3
		// size: 0x4
		register struct CVECTOR *c;
		{ //line: 16
			// register: $v1
			// size: 0x1AC
			register struct _MonsterVars *mv;
			// register: $a1
			register int current;
			// register: $a0
			register int wanted;
			// register: $a0
			register int col;
		} //line: 74
	} //line: 75
} //line: 76


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008C5A8
// line start: 3164
// line end:   3188
void ProcessBurntMess(struct _Instance *instance, int vertidx, int segidx, int dist, void *cb_data)
{ //line: 1
	// register: $a2
	register short scl;
	// register: $t0
	// size: 0x8
	register struct burntTuneType *burntTune;
} //line: 21


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008C654
// line start: 3195
// line end:   3239
int MONSTER_StartVertexBurnt(struct _Instance *instance, struct _SVector *location, struct burntTuneType *burntTune)
{ //line: 1
	// register: $s1
	// size: 0x38
	register struct _Model *model;
	// address: -80
	// size: 0x10
	auto struct burntMessType bmt;
	{ //line: 11
		// address: -64
		// size: 0x8
		auto struct _SVector localloc;
		{ //line: 21
			// register: $s1
			// size: 0x8
			register struct _MVertex *vertexList;
			// address: -56
			// size: 0xC
			auto struct _Vector locVec;
			// address: -40
			auto long flag;
			// register: $s0
			// size: 0x20
			register struct MATRIX *segMatrix;
		} //line: 21
	} //line: 42
} //line: 45


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008C7D4
// line start: 3242
// line end:   3264
void MONSTER_VertexBurnt(struct _Instance *instance, struct burntTuneType *burntTune)
{ //line: 2
	// register: $a3
	register int i;
	// register: $a2
	register int fade;
	// register: $t0
	// size: 0x38
	register struct _Model *model;
	{ //line: 11
		// register: $v1
		// size: 0x4
		register struct CVECTOR *c;
	} //line: 20
} //line: 23


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008C880
// line start: 3272
// line end:   3386
void MON_DamageEffect(struct _Instance *instance, struct evFXHitData *data)
{ //line: 1
	// address: -104
	// size: 0x8
	auto struct _SVector accel;
	// register: $s4
	// size: 0x1AC
	register struct _MonsterVars *mv;
	{ //line: 17
		// register: $s3
		// size: 0x50
		register struct _MonsterAttributes *ma;
		{ //line: 21
			// register: $s1
			// size: 0x20
			register struct MATRIX *mat;
			// address: -96
			// size: 0x8
			auto struct _SVector location;
			// address: -88
			// size: 0x8
			auto struct _SVector vel;
			// address: -80
			// size: 0x8
			auto struct _SVector accel;
		} //line: 36
		{ //line: 40
			// register: $s0
			// size: 0x20
			register struct MATRIX *mat;
			// address: -72
			// size: 0x8
			auto struct _SVector location;
			// address: -64
			// size: 0x8
			auto struct _SVector vel;
		} //line: 49
		{ //line: 53
			// register: $v0
			// size: 0x4C
			register struct Object *flame;
			{ //line: 60
				// register: $s0
				// size: 0x38
				register struct _Model *model;
			} //line: 66
		} //line: 68
		{ //line: 73
			// register: $s0
			// size: 0x20
			register struct MATRIX *mat;
			// address: -56
			// size: 0x8
			auto struct _SVector location;
			// address: -48
			// size: 0x8
			auto struct _SVector vel;
			// address: -40
			// size: 0x8
			auto struct _SVector accel;
			// register: $s1
			register int n;
			// register: $s4
			register int cnt;
		} //line: 92
	} //line: 93
} //line: 115


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008CCE4
// line start: 3392
// line end:   3466
void MON_DefaultInit(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x50
	register struct _MonsterAttributes *ma;
	// register: $s3
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $v0
	// size: 0x8
	register struct _HModel *hModel;
	// register: $v0
	// size: 0x8
	register struct _HPrim_duplicate_s3 *hprim;
	// register: $a0
	register int i;
	// register: $a1
	register int state;
	{ //line: 48
		// register: $s0
		// size: 0x188
		register struct Level *level;
	} //line: 56
} //line: 74


// path: C:\kain2\game\MONSTER\MONSTER.C
// address: 0x8008CF30
// line start: 3472
// line end:   3514
void MON_CleanUp(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $s1
	// size: 0x50
	register struct _MonsterAttributes *ma;
} //line: 43


// path: C:\kain2\game\G2\INSTNCG2.C
// address: 0x80094ED0
// line start: 71
// line end:   150
void G2Instance_BuildTransformsForList(struct _Instance *listHead)
{ //line: 1
	// register: $s0
	// size: 0x29C
	register struct _Instance *instance;
} //line: 80


// path: C:\kain2\game\G2\INSTNCG2.C
// address: 0x8009504C
// line start: 155
// line end:   163
void G2Instance_BuildTransforms(struct _Instance *instance);

// path: C:\kain2\game\G2\INSTNCG2.C
// address: 0x800950A8
// line start: 169
// line end:   177
void G2Instance_RebuildTransforms(struct _Instance *instance);

// path: C:\kain2\game\G2\INSTNCG2.C
// address: 0x80095104
// size: 0x14
// line start: 184
// line end:   185
struct _G2AnimKeylist_Type *G2Instance_GetKeylist(struct _Instance *instance, int id);

// path: C:\kain2\game\G2\INSTNCG2.C
// address: 0x80095124
// line start: 196
// line end:   345
void _G2Instance_RebuildAnimatedTransforms(struct _Instance *instance)
{ //line: 1
	// register: $s7
	// size: 0x38
	register struct _Model *model;
	// register: $s4
	// size: 0x20
	register struct _G2Matrix_Type *rootMatrix;
	// address: -120
	// size: 0x8
	auto struct _Rotation pre_facade_rot;
	// register: $a2
	// size: 0x20
	register struct _G2Matrix_Type *segMatrix;
	// address: -112
	// size: 0x20
	auto struct _G2Matrix_Type seg1RotMatrix;
	// address: -80
	// size: 0x20
	auto struct _G2Matrix_Type seg2RotMatrix;
	// address: -48
	// size: 0x6
	auto struct _G2SVector3_Type rotVector;
	// register: $s6
	register long otx;
	// register: $s5
	register long oty;
	// register: $s2
	register long otz;
	// register: $s1
	register long segIndex;
	{ //line: 24
	} //line: 33
	{ //line: 87
		// register: $v1
		// size: 0x10
		register struct VECTOR *ins_scale;
	} //line: 95
} //line: 150


// path: C:\kain2\game\G2\INSTNCG2.C
// address: 0x80095558
// line start: 348
// line end:   353
void G2Instance_ClearMatrices(struct _Instance *instance);

// path: C:\kain2\game\G2\INSTNCG2.C
// address: 0x800955AC
// line start: 359
// line end:   387
void _G2Instance_BuildAnimatedTransforms(struct _Instance *instance)
{ //line: 1
	// register: $v0
	// size: 0x20
	register struct MATRIX *rootMatrix;
	// register: $v0
	// size: 0x38
	register struct _Model *model;
} //line: 29


// path: C:\kain2\game\G2\INSTNCG2.C
// address: 0x80095668
// line start: 392
// line end:   510
void _G2Instance_RebuildNonAnimatedTransforms(struct _Instance *instance)
{ //line: 1
	// register: $s5
	// size: 0x10
	register struct VECTOR *scale;
	// register: $s7
	// size: 0x20
	register struct MATRIX *introTransform;
	// register: $s1
	// size: 0x20
	register struct MATRIX *segMatrix;
	// register: $fp
	// size: 0x38
	register struct _Model *model;
	// register: $s3
	// size: 0x18
	register struct _Segment *segment;
	// address: -48
	auto short scale_flag;
	// register: $s4
	register long i;
} //line: 119


// path: C:\kain2\game\G2\INSTNCG2.C
// address: 0x800958F4
// line start: 515
// line end:   577
void _G2Instance_BuildDeactivatedTransforms(struct _Instance *instance)
{ //line: 1
	// register: $a0
	// size: 0x20
	register struct MATRIX *segMatrix;
	// register: $a1
	// size: 0x20
	register struct MATRIX *startOldMatrix;
	// register: $s1
	register int numMatrices;
	// register: $a0
	// size: 0x38
	register struct _Model *model;
} //line: 63


// path: C:\kain2\game\G2\INSTNCG2.C
// address: 0x80095A74
// line start: 583
// line end:   610
void _G2Instance_BuildNonAnimatedTransforms(struct _Instance *instance)
{ //line: 1
	// register: $v1
	// size: 0x20
	register struct MATRIX *segMatrix;
	// register: $v0
	// size: 0x38
	register struct _Model *model;
} //line: 28


// path: C:\kain2\game\G2\INSTNCG2.C
// address: 0x80095B30
// line start: 619
// line end:   702
void _G2Instance_BuildFacadeTransforms(struct _Instance *instance, struct _Segment *segment, struct MATRIX *segMatrix, struct MATRIX *matrixPool, long scale_flag)
{ //line: 1
	// register: $s7
	// size: 0x6
	register struct _Position *cameraPos;
	// register: $s3
	// size: 0x8
	register struct SVECTOR *segmentPos;
	// register: $s4
	// size: 0x8
	register struct SVECTOR *segmentRot;
	// register: $fp
	// size: 0x10
	register struct VECTOR *scale;
	{ //line: 25
		// register: $s1
		// size: 0x8
		register struct SVECTOR *zvec;
		// register: $s2
		// size: 0x8
		register struct SVECTOR *camWorldPos;
		// register: $s6
		// size: 0x8
		register struct SVECTOR *camLocPos;
		// register: $s0
		register long sqrt;
	} //line: 57
	{ //line: 64
		// register: $s0
		// size: 0x10
		register struct VECTOR *xy;
	} //line: 83
} //line: 84


// path: C:\kain2\game\G2\MAING2.C
// address: 0x80095ECC
// size: 0x4
// line start: 45
// line end:   76
enum _G2Bool_Enum_duplicate_e80 MainG2_InitEngine(void *appData, unsigned int screenWidth, unsigned int screenHeight, char *filesystemName);

// path: C:\kain2\game\G2\MAING2.C
// address: 0x80095ED4
// line start: 82
// line end:   91
void MainG2_ShutDownEngine(void *appData);

// path: C:\kain2\game\G2\POOLMMG2.C
// address: 0x80095EDC
// line start: 9
// line end:   30
void G2PoolMem_InitPool(void *voidPool, int blockCount, int blockSize)
{ //line: 1
} //line: 1


// path: C:\kain2\game\G2\POOLMMG2.C
// address: 0x80095F48
// line start: 32
// line end:   47
void G2PoolMem_ResetPool(void *voidPool)
{ //line: 3
	// register: $a0
	// size: 0x10
	register struct _G2PoolMemPool_Type *pool;
	// register: $a1
	register int blockIndex;
} //line: 16


// path: C:\kain2\game\G2\POOLMMG2.C
// address: 0x80095F84
// line start: 64
// line end:   85
void *G2PoolMem_Allocate(void *voidPool)
{ //line: 1
	// register: $a0
	register int blockIndex;
} //line: 22


// path: C:\kain2\game\G2\POOLMMG2.C
// address: 0x80095FE0
// line start: 87
// line end:   103
void G2PoolMem_Free(void *voidPool, void *block)
{ //line: 3
	// register: $a1
	register int blockIndex;
} //line: 9


// path: C:\kain2\game\G2\QUATG2.C
// address: 0x80096018
// line start: 74
// line end:   80
void G2Quat_ToEuler(struct _G2Quat_Type *quat, struct _G2EulerAngles_Type *euler, int order)
{ //line: 1
	// address: -48
	// size: 0x20
	auto struct _G2Matrix_Type tempMatrix;
} //line: 1


// path: C:\kain2\game\G2\QUATG2.C
// address: 0x8009605C
// line start: 328
// line end:   389
void G2EulerAngles_FromMatrix(struct _G2EulerAngles_Type *euler, struct _G2Matrix_Type *matrix, long order)
{ //line: 1
	// register: $a2
	register long i;
	// register: $s5
	register long j;
	// address: -56
	auto long k;
	// address: -52
	auto long n;
	// register: $a1
	register long s;
	// address: -48
	auto long f;
	{ //line: 1
		// register: $v1
		register unsigned int o;
	} //line: 1
	{ //line: 8
		// register: $s0
		register long sy;
	} //line: 21
	{ //line: 28
		// register: $s5
		register long cy;
	} //line: 47
	{ //line: 56
		// register: $v1
		register short t;
	} //line: 59
} //line: 61


// path: C:\kain2\game\G2\TIMERG2.C
// address: 0x800963C0
// line start: 10
// line end:   20
short G2Timer_GetFrameTime()
{ //line: 2
	// register: $v1
	register short atime;
} //line: 10


// path: C:\kain2\game\MONSTER\VWRAITH.C
// address: 0x8008EA34
// line start: 89
// line end:   101
void VWRAITH_MoveVertical(struct _Instance *instance, long targetZ, int velocity);

// path: C:\kain2\game\MONSTER\VWRAITH.C
// address: 0x8008EA8C
// line start: 108
// line end:   128
void VWRAITH_Init(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// address: -24
	auto long color;
} //line: 3


// path: C:\kain2\game\MONSTER\VWRAITH.C
// address: 0x8008EB40
// line start: 131
// line end:   145
int VWRAITH_ShouldISwoop(struct _Instance *instance)
{ //line: 1
	// register: $a3
	// size: 0x50
	register struct _MonsterAttributes *ma;
	// register: $v1
	// size: 0x1AC
	register struct _MonsterVars *mv;
	{ //line: 7
		// register: $a1
		// size: 0x29C
		register struct _Instance *ei;
	} //line: 12
} //line: 15


// path: C:\kain2\game\MONSTER\VWRAITH.C
// address: 0x8008EBC4
// line start: 151
// line end:   171
void VWRAITH_PursueEntry(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	{ //line: 15
		// register: $v0
		// size: 0x50
		register struct _MonsterAttributes *ma;
	} //line: 20
} //line: 21


// path: C:\kain2\game\MONSTER\VWRAITH.C
// address: 0x8008EC5C
// line start: 174
// line end:   224
void VWRAITH_Pursue(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x50
	register struct _MonsterAttributes *ma;
	// register: $s0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	{ //line: 21
		// register: $s1
		// size: 0x50
		register struct _MonsterAttributes *ma;
		// register: $s3
		// size: 0x20
		register struct _MonsterAttackAttributes *attack;
	} //line: 44
} //line: 51


// path: C:\kain2\game\MONSTER\VWRAITH.C
// address: 0x8008EE2C
// line start: 230
// line end:   242
void VWRAITH_VerticalMove(struct _Instance *instance)
{ //line: 1
	// register: $v0
	// size: 0x1AC
	register struct _MonsterVars *mv;
	{ //line: 6
		// register: $v0
		// size: 0x50
		register struct _MonsterAttributes *ma;
		// register: $s1
		register long targetZ;
	} //line: 12
} //line: 13


// path: C:\kain2\game\MONSTER\VWRAITH.C
// address: 0x8008EEB0
// line start: 248
// line end:   249
void VWRAITH_CombatEntry(struct _Instance *instance);

// path: C:\kain2\game\MONSTER\VWRAITH.C
// address: 0x8008EED0
// line start: 253
// line end:   256
void VWRAITH_Combat(struct _Instance *instance);

// path: C:\kain2\game\MONSTER\VWRAITH.C
// address: 0x8008EEFC
// line start: 263
// line end:   264
void VWRAITH_EmbraceEntry(struct _Instance *instance);

// path: C:\kain2\game\MONSTER\VWRAITH.C
// address: 0x8008EF1C
// line start: 268
// line end:   271
void VWRAITH_Embrace(struct _Instance *instance);

// path: C:\kain2\game\MONSTER\WORSHIP.C
// address: 0x8008EF48
// line start: 47
// line end:   67
void WORSHIP_Init(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x50
	register struct _MonsterAttributes *ma;
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
	// register: $v0
	// size: 0x29C
	register struct _Instance *weapon;
} //line: 21


// path: C:\kain2\game\MONSTER\WORSHIP.C
// address: 0x8008EFE8
// line start: 73
// line end:   102
void WORSHIP_CombatEntry(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x1AC
	register struct _MonsterVars *mv;
	{ //line: 6
		// register: $s2
		// size: 0x50
		register struct _MonsterAttributes *ma;
		{ //line: 10
		} //line: 16
	} //line: 27
} //line: 29


// path: C:\kain2\game\MONSTER\MBMISS.C
// address: 0x8008F0E8
// line start: 126
// line end:   141
void WCBEGG_Message(struct _Instance *instance, unsigned long message, unsigned long data)
{ //line: 1
	{ //line: 6
		// register: $s0
		// size: 0x48
		register struct PhysObData *pod;
	} //line: 11
} //line: 16


// path: C:\kain2\game\MONSTER\MBMISS.C
// address: 0x8008F18C
// line start: 144
// line end:   175
int WCBEGG_ShouldIgniteEgg(struct _Instance *egg, struct _walbossAttributes *wa)
{ //line: 1
	// register: $s0
	// size: 0x10C
	register struct _InstanceList *instanceList;
	// register: $s0
	// size: 0x29C
	register struct _Instance *instance;
} //line: 32


// path: C:\kain2\game\MONSTER\MBMISS.C
// address: 0x8008F284
// line start: 178
// line end:   238
void WCBEGG_Process(struct _Instance *instance, struct GameTracker *gameTracker)
{ //line: 1
	// register: $s5
	// size: 0x48
	register struct PhysObData *data;
	// register: $s2
	register int time;
	// register: $s4
	register int lastTime;
	// register: $s0
	register int timeBetween;
	// register: $a0
	register int birthTime;
	// register: $v0
	// size: 0x4C
	register struct Object *walboss;
	// register: $s3
	// size: 0x1C
	register struct _walbossAttributes *wa;
} //line: 61


// path: C:\kain2\game\MONSTER\MBMISS.C
// address: 0x8008F478
// line start: 241
// line end:   288
void WCBEGG_ExplodeProcess(struct _Instance *instance, struct GameTracker *gameTracker)
{ //line: 1
	// register: $s1
	// size: 0x48
	register struct PhysObData *data;
	// register: $s2
	register int currentTime;
	// register: $v0
	register int time;
	// register: $v1
	// size: 0x4C
	register struct Object *walboss;
	// register: $v1
	// size: 0x1C
	register struct _walbossAttributes *wa;
} //line: 48


// path: C:\kain2\game\MONSTER\MBMISS.C
// address: 0x8008F5B8
// line start: 291
// line end:   332
void WCBEGG_SplitProcess(struct _Instance *instance, struct GameTracker *gameTracker)
{ //line: 1
	// register: $s1
	// size: 0x48
	register struct PhysObData *data;
	// register: $s2
	register int currentTime;
	// register: $v0
	register int time;
	// register: $v1
	// size: 0x4C
	register struct Object *walboss;
	// register: $v1
	// size: 0x1C
	register struct _walbossAttributes *wa;
} //line: 42


// path: C:\kain2\game\MONSTER\MBMISS.C
// address: 0x8008F6B4
// line start: 335
// line end:   345
void WCBEGG_CommonPostProcess(struct _Instance *instance, struct GameTracker *gameTracker)
{ //line: 1
	// register: $s1
	// size: 0x48
	register struct PhysObData *data;
} //line: 1


// path: C:\kain2\game\MONSTER\MBMISS.C
// address: 0x8008F744
// line start: 348
// line end:   365
void WCBEGG_CommonPostProcess2(struct _Instance *instance, struct GameTracker *gameTracker)
{ //line: 1
	// register: $s0
	// size: 0x48
	register struct PhysObData *data;
	// register: $v1
	register int time;
} //line: 18


// path: C:\kain2\game\MONSTER\MBMISS.C
// address: 0x8008F7B8
// line start: 368
// line end:   424
void WCBEGG_ExplodeCollide(struct _Instance *instance, struct GameTracker *gameTracker)
{ //line: 1
	// register: $a0
	// size: 0x30
	register struct _CollideInfo *collideInfo;
	// register: $v1
	// size: 0x14
	register struct _HSphere *s1;
	// register: $s0
	// size: 0x29C
	register struct _Instance *inst1;
	// register: $s1
	// size: 0x48
	register struct PhysObData *data;
} //line: 57


// path: C:\kain2\game\MONSTER\MBMISS.C
// address: 0x8008F93C
// line start: 427
// line end:   444
void WCBEGG_Collide(struct _Instance *instance, struct GameTracker *gameTracker)
{ //line: 1
	// register: $a0
	// size: 0x30
	register struct _CollideInfo *collideInfo;
	{ //line: 6
		// register: $v0
		// size: 0x29C
		register struct _Instance *inst1;
	} //line: 12
} //line: 18


// path: C:\kain2\game\MONSTER\MBMISS.C
// address: 0x8008F9C4
// line start: 450
// line end:   467
long WALBOSC_AnimCallback(struct _G2Anim_Type *anim, int sectionID, enum _G2AnimCallbackMsg_Enum_duplicate_e5 message, long messageDataA, long messageDataB, struct _Instance *instance);

// path: C:\kain2\game\MONSTER\MBMISS.C
// address: 0x8008FA44
// line start: 471
// line end:   495
void WALBOSC_Collide(struct _Instance *instance, struct GameTracker *gameTracker)
{ //line: 1
	// register: $s0
	// size: 0x30
	register struct _CollideInfo *collideInfo;
	// register: $v0
	// size: 0x14
	register struct _HSphere *s1;
	// register: $s3
	// size: 0x29C
	register struct _Instance *inst1;
	{ //line: 9
		// register: $s1
		// size: 0x24
		register struct PhysObWeaponProperties *Prop;
	} //line: 24
} //line: 25


// path: C:\kain2\game\MONSTER\MBMISS.C
// address: 0x8008FB04
// line start: 498
// line end:   504
void WALBOSC_Message(struct _Instance *instance, unsigned long message, unsigned long data);

// path: C:\kain2\game\G2\ANMG2ILF.C
// address: 0x8008FB50
// line start: 97
// line end:   105
short G2Anim_GetElapsedTime(struct _G2Anim_Type *anim)
{ //line: 3
	// register: $v0
	// size: 0x30
	register struct _G2AnimSection_Type *section;
} //line: 7


// path: C:\kain2\game\G2\ANMG2ILF.C
// address: 0x8008FB74
// size: 0x14
// line start: 136
// line end:   144
struct _G2AnimKeylist_Type *G2Anim_GetKeylist(struct _G2Anim_Type *anim)
{ //line: 3
	// register: $v0
	// size: 0x30
	register struct _G2AnimSection_Type *section;
} //line: 7


// path: C:\kain2\game\G2\ANMG2ILF.C
// address: 0x8008FB98
// line start: 165
// line end:   180
void G2Anim_GetRootMotionOverInterval(struct _G2Anim_Type *anim, short intervalStart, short intervalEnd, struct _G2SVector3_Type *motionVector)
{ //line: 1
} //line: 1


// path: C:\kain2\game\G2\ANMG2ILF.C
// address: 0x8008FBC8
// line start: 292
// line end:   306
void G2Anim_InterpToKeylistFrame(struct _G2Anim_Type *anim, struct _G2AnimKeylist_Type *keylist, int keylistID, int targetFrame, int duration)
{ //line: 1
	// register: $s0
	register int i;
} //line: 15


// path: C:\kain2\game\G2\ANMG2ILF.C
// address: 0x8008FC6C
// line start: 412
// line end:   421
void G2Anim_SetAlphaTable(struct _G2Anim_Type *anim, struct _G2AnimAlphaTable_Type *table)
{ //line: 1
	// register: $s0
	register int i;
} //line: 10


// path: C:\kain2\game\G2\ANMG2ILF.C
// address: 0x8008FCDC
// line start: 428
// line end:   438
void G2Anim_SetCallback(struct _G2Anim_Type *anim, long (*callback)(), void *data)
{ //line: 3
	// register: $v1
	register int i;
} //line: 11


// path: C:\kain2\game\G2\ANMG2ILF.C
// address: 0x8008FD18
// line start: 518
// line end:   526
void G2Anim_SetLooping(struct _G2Anim_Type *anim)
{ //line: 1
	// register: $s0
	register int i;
} //line: 9


// path: C:\kain2\game\G2\ANMG2ILF.C
// address: 0x8008FD78
// line start: 565
// line end:   573
void G2Anim_SetNoLooping(struct _G2Anim_Type *anim)
{ //line: 1
	// register: $s0
	register int i;
} //line: 9


// path: C:\kain2\game\G2\ANMG2ILF.C
// address: 0x8008FDD8
// line start: 591
// line end:   599
void G2Anim_SetPaused(struct _G2Anim_Type *anim)
{ //line: 1
	// register: $s0
	register int i;
} //line: 9


// path: C:\kain2\game\G2\ANMG2ILF.C
// address: 0x8008FE38
// line start: 618
// line end:   628
void G2Anim_SetSpeedAdjustment(struct _G2Anim_Type *anim, long adjustment)
{ //line: 3
	// register: $v1
	register int i;
} //line: 11


// path: C:\kain2\game\G2\ANMG2ILF.C
// address: 0x8008FE6C
// line start: 649
// line end:   657
void G2Anim_SetUnpaused(struct _G2Anim_Type *anim)
{ //line: 1
	// register: $s0
	register int i;
} //line: 9


// path: C:\kain2\game\G2\ANMG2ILF.C
// address: 0x8008FECC
// line start: 678
// line end:   688
void G2Anim_SwitchToKeylist(struct _G2Anim_Type *anim, struct _G2AnimKeylist_Type *keylist, int keylistID)
{ //line: 1
	// register: $s0
	register int i;
} //line: 11


// path: C:\kain2\game\G2\ANMG2ILF.C
// address: 0x8008FF4C
// line start: 797
// line end:   801
short G2AnimKeylist_GetDuration(struct _G2AnimKeylist_Type *keylist);

// path: C:\kain2\game\G2\ANMG2ILF.C
// address: 0x8008FF74
// line start: 811
// line end:   814
int G2AnimKeylist_GetKeyframeCount(struct _G2AnimKeylist_Type *keylist);

// path: C:\kain2\game\G2\ANMG2ILF.C
// address: 0x8008FFAC
// line start: 840
// line end:   845
void G2AnimSection_ClearAlarm(struct _G2AnimSection_Type *section, unsigned long flag);

// path: C:\kain2\game\G2\ANMG2ILF.C
// address: 0x8008FFC0
// line start: 870
// line end:   883
int G2AnimSection_GetKeyframeNumber(struct _G2AnimSection_Type *section);

// path: C:\kain2\game\G2\ANMG2ILF.C
// address: 0x8009001C
// line start: 974
// line end:   979
int G2AnimSection_GetStoredKeyframeNumber(struct _G2AnimSection_Type *section);

// path: C:\kain2\game\G2\ANMG2ILF.C
// address: 0x8009003C
// line start: 1005
// line end:   1018
void G2AnimSection_InterpToKeylistFrame(struct _G2AnimSection_Type *section, struct _G2AnimKeylist_Type *keylist, int keylistID, int targetFrame, int duration)
{ //line: 1
} //line: 1


// path: C:\kain2\game\G2\ANMG2ILF.C
// address: 0x8009007C
// size: 0x4
// line start: 1023
// line end:   1034
enum _G2Bool_Enum_duplicate_e80 G2AnimSection_IsInInterpolation(struct _G2AnimSection_Type *section)
{ //line: 3
	// register: $v0
	// size: 0xC
	register struct _G2AnimInterpInfo_Type *interpInfo;
} //line: 12


// path: C:\kain2\game\G2\ANMG2ILF.C
// address: 0x800900AC
// line start: 1081
// line end:   1087
short G2AnimSection_NextKeyframe(struct _G2AnimSection_Type *section);

// path: C:\kain2\game\G2\ANMG2ILF.C
// address: 0x80090108
// line start: 1112
// line end:   1117
void G2AnimSection_SetAlphaTable(struct _G2AnimSection_Type *section, struct _G2AnimAlphaTable_Type *table);

// path: C:\kain2\game\G2\ANMG2ILF.C
// address: 0x80090124
// line start: 1162
// line end:   1170
void G2AnimSection_SetInterpInfo(struct _G2AnimSection_Type *section, struct _G2AnimInterpInfo_Type *newInfoPtr);

// path: C:\kain2\game\G2\ANMG2ILF.C
// address: 0x80090154
// line start: 1208
// line end:   1214
void G2AnimSection_SetLooping(struct _G2AnimSection_Type *section);

// path: C:\kain2\game\G2\ANMG2ILF.C
// address: 0x80090194
// line start: 1220
// line end:   1225
void G2AnimSection_SetLoopRangeAll(struct _G2AnimSection_Type *section);

// path: C:\kain2\game\G2\ANMG2ILF.C
// address: 0x800901C4
// line start: 1261
// line end:   1264
void G2AnimSection_SetNoLooping(struct _G2AnimSection_Type *section);

// path: C:\kain2\game\G2\ANMG2ILF.C
// address: 0x800901D8
// line start: 1270
// line end:   1273
void G2AnimSection_SetNotRewinding(struct _G2AnimSection_Type *section);

// path: C:\kain2\game\G2\ANMG2ILF.C
// address: 0x800901EC
// line start: 1279
// line end:   1282
void G2AnimSection_SetPaused(struct _G2AnimSection_Type *section);

// path: C:\kain2\game\G2\ANMG2ILF.C
// address: 0x80090200
// line start: 1333
// line end:   1336
void G2AnimSection_SetUnpaused(struct _G2AnimSection_Type *section);

// path: C:\kain2\game\G2\ANMG2ILF.C
// address: 0x80090214
// line start: 1344
// line end:   1349
void G2AnimSection_SwitchToKeylist(struct _G2AnimSection_Type *section, struct _G2AnimKeylist_Type *keylist, int keylistID);

// path: C:\kain2\game\G2\ANMDECMP.C
// address: 0x80090234
// line start: 135
// line end:   192
void _G2Anim_DecompressChannel_AdaptiveDelta(struct _G2AnimDecompressChannelInfo_Type *dcInfo, struct _G2AnimChanStatus_Type *status)
{ //line: 1
	// register: $t7
	register unsigned short *chanData;
	// register: $t2
	register int index;
	// register: $t5
	register int keyData;
	// register: $v1
	register unsigned short step;
	// register: $a2
	register unsigned short predictedDelta;
	// register: $t6
	register int targetKey;
	// register: $t4
	register int storedKey;
	// register: $t8
	register int keyCount;
} //line: 54


// path: C:\kain2\game\G2\ANMDECMP.C
// address: 0x80090360
// line start: 198
// line end:   233
void _G2Anim_DecompressChannel_Linear(struct _G2AnimDecompressChannelInfo_Type *dcInfo, struct _G2AnimChanStatus_Type *status)
{ //line: 3
	// register: $a2
	register unsigned short *chanData;
	// register: $t0
	register short rangeBase;
	// register: $v0
	register short rangeInfo;
	// register: $v1
	register int rangeLength;
	// register: $a0
	register int rangeOffset;
	// register: $a3
	register int targetKey;
} //line: 34


// path: C:\kain2\game\G2\ANMDECMP.C
// address: 0x800903F4
// line start: 271
// line end:   283
void _G2Anim_InitializeChannel_AdaptiveDelta(struct _G2AnimDecompressChannelInfo_Type *dcInfo, struct _G2AnimChanStatus_Type *status)
{ //line: 3
	// register: $v1
	register unsigned short *chanData;
	// register: $v0
	register int keyCount;
} //line: 6


// path: C:\kain2\game\G2\ANMDECMP.C
// address: 0x8009042C
// line start: 289
// line end:   299
void _G2Anim_InitializeChannel_Linear(struct _G2AnimDecompressChannelInfo_Type *dcInfo, struct _G2AnimChanStatus_Type *status)
{ //line: 3
	// register: $v1
	register unsigned short *chanData;
	// register: $v0
	register int chanLength;
} //line: 6


// path: C:\kain2\game\G2\ANMCTRLR.C
// address: 0x80090458
// line start: 235
// line end:   248
void G2Anim_AttachControllerToSeg(struct _G2Anim_Type *anim, int segNumber, int type)
{ //line: 1
} //line: 1


// path: C:\kain2\game\G2\ANMCTRLR.C
// address: 0x80090490
// line start: 260
// line end:   290
void G2Anim_DetachControllerFromSeg(struct _G2Anim_Type *anim, int segNumber, int type)
{ //line: 1
	// register: $v0
	// size: 0x24
	register struct _G2AnimController_Type *controller;
	// address: -32
	auto unsigned short *controllerNextPtr;
} //line: 31


// path: C:\kain2\game\G2\ANMCTRLR.C
// address: 0x80090558
// line start: 342
// line end:   377
void G2Anim_EnableController(struct _G2Anim_Type *anim, int segNumber, int type)
{ //line: 1
	// register: $s0
	// size: 0x24
	register struct _G2AnimController_Type *controller;
	// register: $v1
	// size: 0x20
	register struct _G2Matrix_Type *segMatrix;
} //line: 36


// path: C:\kain2\game\G2\ANMCTRLR.C
// address: 0x80090634
// line start: 397
// line end:   410
void G2Anim_DisableController(struct _G2Anim_Type *anim, int segNumber, int type)
{ //line: 1
	// register: $v0
	// size: 0x24
	register struct _G2AnimController_Type *controller;
} //line: 14


// path: C:\kain2\game\G2\ANMCTRLR.C
// address: 0x80090674
// line start: 430
// line end:   474
void G2Anim_InterpDisableController(struct _G2Anim_Type *anim, int segNumber, int type, short duration)
{ //line: 1
	// register: $s1
	// size: 0x24
	register struct _G2AnimController_Type *controller;
	// address: -48
	// size: 0x6
	auto struct _G2SVector3_Type zeroVector;
	// address: -40
	// size: 0x8
	auto struct _G2Quat_Type targetQuat;
} //line: 45


// path: C:\kain2\game\G2\ANMCTRLR.C
// address: 0x80090794
// size: 0x4
// line start: 496
// line end:   512
enum _G2Bool_Enum_duplicate_e80 G2Anim_IsControllerActive(struct _G2Anim_Type *anim, int segNumber, int type)
{ //line: 1
	// register: $v0
	// size: 0x24
	register struct _G2AnimController_Type *controller;
} //line: 17


// path: C:\kain2\game\G2\ANMCTRLR.C
// address: 0x800907E0
// size: 0x4
// line start: 519
// line end:   539
enum _G2Bool_Enum_duplicate_e80 G2Anim_IsControllerInterpolating(struct _G2Anim_Type *anim, int segNumber, int type)
{ //line: 1
	// register: $v0
	// size: 0x24
	register struct _G2AnimController_Type *controller;
} //line: 21


// path: C:\kain2\game\G2\ANMCTRLR.C
// address: 0x80090834
// line start: 558
// line end:   602
void G2Anim_GetControllerCurrentInterpVector(struct _G2Anim_Type *anim, int segNumber, int type, struct _G2SVector3_Type *vector)
{ //line: 1
	// register: $v1
	// size: 0x24
	register struct _G2AnimController_Type *controller;
	// register: $a0
	// size: 0x6
	register struct _G2SVector3_Type *base;
	// register: $a1
	// size: 0x6
	register struct _G2SVector3_Type *offset;
	// register: $v1
	// size: 0x20
	register struct _G2Matrix_Type *segMatrix;
	{ //line: 21
		// register: $a0
		// size: 0x6
		register struct _G2SVector3_Type *source;
		// register: $s0
		// size: 0x6
		register struct _G2SVector3_Type *dest;
		{ //line: 21
			// register: $v1
			register unsigned short z;
			// register: $v0
			register unsigned long xy;
		} //line: 21
	} //line: 21
	{ //line: 22
		// register: $s0
		// size: 0x6
		register struct _G2SVector3_Type *dest;
		// register: $a0
		// size: 0x6
		register struct _G2SVector3_Type *base;
		// register: $a1
		// size: 0x6
		register struct _G2SVector3_Type *offset;
		// register: $v0
		register long alpha;
	} //line: 22
} //line: 45


// path: C:\kain2\game\G2\ANMCTRLR.C
// address: 0x80090994
// line start: 629
// line end:   640
void G2Anim_SetControllerCallbackData(struct _G2Anim_Type *anim, int segNumber, int type, void *callbackData)
{ //line: 1
} //line: 1


// path: C:\kain2\game\G2\ANMCTRLR.C
// address: 0x800909BC
// line start: 648
// line end:   660
void G2Anim_SetControllerAngleOrder(struct _G2Anim_Type *anim, int segNumber, int type, int order)
{ //line: 1
	// register: $v0
	// size: 0x24
	register struct _G2AnimController_Type *controller;
} //line: 1


// path: C:\kain2\game\G2\ANMCTRLR.C
// address: 0x800909F4
// line start: 668
// line end:   681
void G2Anim_SetController_Vector(struct _G2Anim_Type *anim, int segNumber, int type, struct _G2SVector3_Type *vector)
{ //line: 1
	// register: $v0
	// size: 0x24
	register struct _G2AnimController_Type *controller;
} //line: 1


// path: C:\kain2\game\G2\ANMCTRLR.C
// address: 0x80090A3C
// line start: 711
// line end:   752
void G2Anim_SetInterpController_Vector(struct _G2Anim_Type *anim, int segNumber, int type, struct _G2SVector3_Type *vector, int duration)
{ //line: 1
	// register: $s1
	// size: 0x24
	register struct _G2AnimController_Type *controller;
	// address: -48
	// size: 0x8
	auto struct _G2EulerAngles_Type eulerVector;
	// register: $s0
	// size: 0x6
	register struct _G2SVector3_Type *base;
	// register: $a1
	// size: 0x6
	register struct _G2SVector3_Type *offset;
	// address: -40
	// size: 0x8
	auto struct _G2Quat_Type quat;
} //line: 42


// path: C:\kain2\game\G2\ANMCTRLR.C
// address: 0x80090BA0
// line start: 780
// line end:   800
void G2Anim_SetInterpController_Quat(struct _G2Anim_Type *anim, int segNumber, int type, struct _G2Quat_Type *quat, int duration)
{ //line: 1
	// register: $s0
	// size: 0x24
	register struct _G2AnimController_Type *controller;
	{ //line: 1
		// register: $s2
		// size: 0x8
		register struct _G2Quat_Type *source;
		{ //line: 1
			// register: $a0
			register unsigned long zw;
			// register: $v1
			register unsigned long xy;
		} //line: 1
	} //line: 1
} //line: 1


// path: C:\kain2\game\G2\ANMCTRLR.C
// address: 0x80090C1C
// line start: 810
// line end:   819
void _G2Anim_ApplyControllersToStoredFrame(struct _G2Anim_Type *anim)
{ //line: 1
	// register: $s0
	register int i;
} //line: 10


// path: C:\kain2\game\G2\ANMCTRLR.C
// address: 0x80090C7C
// line start: 824
// line end:   910
void _G2Anim_BuildTransformsWithControllers(struct _G2Anim_Type *anim)
{ //line: 1
	// register: $a1
	// size: 0x18
	register struct _Segment *segment;
	// register: $s0
	// size: 0x24
	register struct _G2AnimController_Type *controller;
	// register: $s5
	// size: 0x20
	register struct _G2Matrix_Type *segMatrix;
	// register: $a1
	// size: 0x20
	register struct _G2Matrix_Type *parentMatrix;
	// register: $a3
	// size: 0x4
	register enum _G2Bool_Enum_duplicate_e80 bRootTransUpdated;
	// register: $s1
	register int segIndex;
	// register: $s7
	register int segCount;
	// address: -56
	// size: 0xC
	auto unsigned long disabledBits[3];
	// register: $s3
	register unsigned long disabledMask;
	// register: $v1
	register unsigned long parentMask;
	// register: $v0
	register unsigned long parentIndex;
} //line: 86


// path: C:\kain2\game\G2\ANMCTRLR.C
// address: 0x80090E5C
// line start: 919
// line end:   1006
void _G2Anim_BuildSegTransformWithControllers(struct _G2Matrix_Type *segMatrix, struct _G2Matrix_Type *parentMatrix, struct _G2AnimController_Type *controller, enum _G2Bool_Enum_duplicate_e80 bRootTransUpdated, int segIndex)
{ //line: 1
	// register: $s1
	// size: 0x18
	register struct _G2AnimSegValue_Type *segValue;
	// address: -48
	// size: 0xC
	auto struct _G2LVector3_Type scale;
	// register: $s3
	register unsigned long flags;
	{ //line: 58
		// register: $v0
		// size: 0x6
		register struct _G2SVector3_Type *svector;
		// register: $v1
		// size: 0xC
		register struct _G2LVector3_Type *lvector;
	} //line: 58
} //line: 85


// path: C:\kain2\game\G2\ANMCTRLR.C
// address: 0x80091168
// line start: 1014
// line end:   1096
unsigned long _G2AnimController_ApplyToSegValue(struct _G2AnimController_Type *controller, struct _G2AnimSegValue_Type *segValue, struct _G2Matrix_Type *segMatrix, struct _G2Matrix_Type *parentMatrix)
{ //line: 1
	// address: -64
	// size: 0x20
	auto struct _G2Matrix_Type tempMatrix;
	// address: -32
	// size: 0x6
	auto struct _G2SVector3_Type tempVector;
	// register: $s1
	register unsigned long flags;
	{ //line: 49
		{ //line: 49
			// register: $v1
			register unsigned short z;
			// register: $v0
			register unsigned long xy;
		} //line: 49
	} //line: 49
	{ //line: 67
		{ //line: 67
			// register: $v1
			register unsigned short z;
			// register: $v0
			register unsigned long xy;
		} //line: 67
	} //line: 67
} //line: 83


// path: C:\kain2\game\G2\ANMCTRLR.C
// address: 0x80091510
// line start: 1101
// line end:   1150
void _G2Anim_UpdateControllers(struct _G2Anim_Type *anim)
{ //line: 1
	// register: $s0
	// size: 0x24
	register struct _G2AnimController_Type *controller;
	// register: $v1
	// size: 0x30
	register struct _G2AnimSection_Type *section;
	// register: $s2
	register short elapsedTime;
} //line: 50


// path: C:\kain2\game\G2\ANMCTRLR.C
// address: 0x8009169C
// line start: 1157
// line end:   1174
void _G2Anim_CopyVectorWithOrder(struct _G2SVector3_Type *sourceVector, struct _G2EulerAngles_Type *destVector, int order);

// path: C:\kain2\game\G2\ANMCTRLR.C
// address: 0x80091700
// line start: 1181
// line end:   1259
void _G2AnimSection_ApplyControllersToStoredFrame(struct _G2AnimSection_Type *section)
{ //line: 1
	// register: $s7
	// size: 0xB4
	register struct _G2Anim_Type *anim;
	// register: $s0
	// size: 0x24
	register struct _G2AnimController_Type *controller;
	// register: $s1
	// size: 0x18
	register struct _G2AnimSegValue_Type *segValue;
	// register: $s6
	// size: 0x18
	register struct _Segment *segment;
	// address: -152
	// size: 0x20
	auto struct _G2Matrix_Type segMatrix;
	// address: -120
	// size: 0x20
	auto struct _G2Matrix_Type parentMatrix;
	// address: -88
	// size: 0x20
	auto struct _G2Matrix_Type invParentMatrix;
	// address: -56
	// size: 0x8
	auto struct _G2EulerAngles_Type eulerAngles;
	// register: $s2
	register int segIndex;
	// register: $fp
	register int segCount;
	{ //line: 56
		// address: -48
		// size: 0x8
		auto struct _G2Quat_Type tempQuat;
		{ //line: 58
			// register: $s1
			// size: 0x8
			register struct _G2Quat_Type *dest;
			{ //line: 58
				// register: $v1
				register unsigned long zw;
				// register: $v0
				register unsigned long xy;
			} //line: 58
		} //line: 58
	} //line: 58
	{ //line: 61
		// register: $s1
		// size: 0x6
		register struct _G2SVector3_Type *dest;
		{ //line: 61
			// register: $v0
			register unsigned long mask;
			// register: $v1
			register unsigned short z;
			// register: $a0
			register unsigned long xy;
		} //line: 61
	} //line: 61
} //line: 79


// path: C:\kain2\game\G2\ANMCTRLR.C
// address: 0x80091974
// line start: 1265
// line end:   1319
unsigned long _G2AnimController_ApplyWorldToParentMatrix(struct _G2AnimController_Type *controller, struct _G2Matrix_Type *parentMatrix)
{ //line: 1
	// address: -64
	// size: 0x20
	auto struct _G2Matrix_Type tempMatrix;
	// address: -32
	// size: 0x6
	auto struct _G2SVector3_Type tempSVector;
	// address: -24
	// size: 0xC
	auto struct _G2LVector3_Type tempLVector;
} //line: 52


// path: C:\kain2\game\G2\ANMCTRLR.C
// address: 0x80091AC8
// line start: 1325
// line end:   1375
void _G2AnimController_GetMatrix(struct _G2AnimController_Type *controller, struct _G2Matrix_Type *matrix)
{ //line: 1
	// address: -24
	// size: 0x8
	auto struct _G2Quat_Type tempQuat;
} //line: 51


// path: C:\kain2\game\G2\ANMCTRLR.C
// address: 0x80091BA0
// line start: 1381
// line end:   1393
void _G2AnimController_GetVector(struct _G2AnimController_Type *controller, struct _G2SVector3_Type *vector)
{ //line: 3
	{ //line: 3
		// register: $a1
		// size: 0x6
		register struct _G2SVector3_Type *dest;
		{ //line: 3
			// register: $v1
			register unsigned short z;
			// register: $v0
			register unsigned long xy;
		} //line: 3
	} //line: 3
	{ //line: 8
		// register: $a1
		// size: 0x6
		register struct _G2SVector3_Type *dest;
		// register: $a1
		// size: 0x6
		register struct _G2SVector3_Type *base;
		// register: $v0
		// size: 0x6
		register struct _G2SVector3_Type *offset;
		// register: $v1
		register long alpha;
	} //line: 8
} //line: 13


// path: C:\kain2\game\G2\ANMCTRLR.C
// address: 0x80091C28
// size: 0x24
// line start: 1400
// line end:   1414
struct _G2AnimController_Type *_G2Anim_FindController(struct _G2Anim_Type *anim, int segNumber, int type)
{ //line: 1
	// register: $v0
	// size: 0x24
	register struct _G2AnimController_Type *controller;
} //line: 15


// path: C:\kain2\game\G2\ANMCTRLR.C
// address: 0x80091C84
// size: 0x24
// line start: 1479
// line end:   1494
struct _G2AnimController_Type *_G2AnimController_Create(int segNumber, int type)
{ //line: 1
	// register: $s0
	// size: 0x24
	register struct _G2AnimController_Type *controller;
} //line: 1


// path: C:\kain2\game\G2\ANMCTRLR.C
// address: 0x80091CEC
// size: 0x24
// line start: 1499
// line end:   1515
struct _G2AnimController_Type *_G2AnimController_Destroy(struct _G2AnimController_Type *controller)
{ //line: 3
	// register: $s0
	register int nextControllerIndex;
} //line: 16


// path: C:\kain2\game\G2\ANMCTRLR.C
// address: 0x80091D50
// line start: 1521
// line end:   1553
void _G2AnimController_InsertIntoList(struct _G2AnimController_Type *controller, unsigned short *listPtr)
{ //line: 1
	// register: $a2
	// size: 0x24
	register struct _G2AnimController_Type *testController;
} //line: 32


// path: C:\kain2\game\G2\ANMCTRLR.C
// address: 0x80091E80
// line start: 1560
// line end:   1608
void _G2AnimController_GetCurrentInterpQuat(struct _G2AnimController_Type *controller, struct _G2Anim_Type *anim, struct _G2Quat_Type *quat)
{ //line: 1
	// address: -24
	// size: 0x8
	auto struct _G2EulerAngles_Type eulerVector;
	{ //line: 31
		// register: $s1
		// size: 0x8
		register struct _G2Quat_Type *quat;
	} //line: 31
} //line: 49


// path: C:\kain2\game\G2\ANMCTRLR.C
// address: 0x80091FBC
// line start: 1615
// line end:   1638
void _G2AnimController_GetSimpleWorldRotQuat(struct _G2AnimController_Type *controller, struct _G2Anim_Type *anim, struct _G2Quat_Type *quat)
{ //line: 1
	// register: $v0
	// size: 0x18
	register struct _Segment *segment;
	// register: $s0
	// size: 0x20
	register struct _G2Matrix_Type *parentMatrix;
	// address: -48
	// size: 0x20
	auto struct _G2Matrix_Type segMatrix;
	// register: $v0
	register int segNumber;
} //line: 1


// path: C:\kain2\game\G2\ANMCTRLR.C
// address: 0x8009203C
// size: 0x24
// line start: 1645
// line end:   1662
struct _G2AnimController_Type *_G2AnimControllerST_FindInList(int segNumber, int type, unsigned short *listPtr)
{ //line: 3
	// register: $a2
	// size: 0x24
	register struct _G2AnimController_Type *controller;
} //line: 18


// path: C:\kain2\game\G2\ANMCTRLR.C
// address: 0x800920AC
// size: 0x24
// line start: 1669
// line end:   1692
struct _G2AnimController_Type *_G2AnimControllerST_FindPtrInList(int segNumber, int type, unsigned short **listPtrPtr)
{ //line: 3
	// register: $a3
	// size: 0x24
	register struct _G2AnimController_Type *controller;
} //line: 24


// path: C:\kain2\game\G2\ANMCTRLR.C
// address: 0x8009214C
// size: 0x24
// line start: 1730
// line end:   1761
struct _G2AnimController_Type *_G2AnimControllerST_RemoveFromList(int segNumber, int type, unsigned short *listPtr)
{ //line: 3
	// register: $a3
	// size: 0x24
	register struct _G2AnimController_Type *controller;
	// register: $a1
	// size: 0x24
	register struct _G2AnimController_Type *stepController;
	// register: $a0
	// size: 0x24
	register struct _G2AnimController_Type *nextController;
} //line: 32


// path: C:\kain2\game\G2\ANMINTRP.C
// address: 0x8009221C
// line start: 49
// line end:   182
void G2AnimSection_InterpToKeylistAtTime(struct _G2AnimSection_Type *section, struct _G2AnimKeylist_Type *keylist, int keylistID, short targetTime, int duration)
{ //line: 1
	// register: $s5
	// size: 0xB4
	register struct _G2Anim_Type *anim;
	// register: $s3
	// size: 0xC
	register struct _G2AnimInterpInfo_Type *interpInfo;
	// register: $t0
	// size: 0xA4
	register struct _G2AnimInterpStateBlock_Type *stateBlockList;
	// register: $a2
	// size: 0x28
	register struct _G2AnimQuatInfo_Type *quatInfo;
	// register: $s0
	register unsigned long alarmFlags;
	// register: $s4
	register short elapsedTime;
	// register: $a3
	register int quatInfoChunkCount;
	// register: $s2
	register int segCount;
} //line: 134


// path: C:\kain2\game\G2\ANMINTRP.C
// address: 0x80092500
// line start: 193
// line end:   253
void _G2AnimSection_UpdateStoredFrameFromQuat(struct _G2AnimSection_Type *section)
{ //line: 1
	// register: $s1
	// size: 0x18
	register struct _G2AnimSegValue_Type *segValue;
	// register: $s0
	// size: 0xC
	register struct _G2AnimInterpInfo_Type *interpInfo;
	// register: $s3
	// size: 0xA4
	register struct _G2AnimInterpStateBlock_Type *stateBlockList;
	// register: $s0
	// size: 0x28
	register struct _G2AnimQuatInfo_Type *quatInfo;
	// register: $s5
	register long alpha;
	// address: -48
	// size: 0x8
	auto struct _G2Quat_Type newQuat;
	// register: $s4
	register int quatInfoChunkCount;
	// register: $s6
	register int segCount;
	{ //line: 30
		// register: $fp
		// size: 0x8
		register struct _G2Quat_Type *source;
		// register: $s1
		// size: 0x8
		register struct _G2Quat_Type *dest;
		{ //line: 30
			// register: $v1
			register unsigned long zw;
			// register: $v0
			register unsigned long xy;
		} //line: 30
	} //line: 30
	{ //line: 30
		// register: $a0
		// size: 0x6
		register struct _G2SVector3_Type *dest;
		// register: $v0
		// size: 0x6
		register struct _G2SVector3_Type *base;
		// register: $v1
		// size: 0x6
		register struct _G2SVector3_Type *offset;
		// register: $s5
		register long alpha;
	} //line: 30
	{ //line: 30
		// register: $a0
		// size: 0x6
		register struct _G2SVector3_Type *dest;
		// register: $v0
		// size: 0x6
		register struct _G2SVector3_Type *base;
		// register: $v1
		// size: 0x6
		register struct _G2SVector3_Type *offset;
		// register: $s5
		register long alpha;
	} //line: 30
} //line: 59


// path: C:\kain2\game\G2\ANMINTRP.C
// address: 0x800926F0
// line start: 258
// line end:   312
void _G2AnimSection_InterpStateToQuat(struct _G2AnimSection_Type *section)
{ //line: 1
	// register: $s0
	// size: 0xC
	register struct _G2AnimInterpInfo_Type *interpInfo;
	// register: $s3
	// size: 0xA4
	register struct _G2AnimInterpStateBlock_Type *stateBlockList;
	// register: $s0
	// size: 0x28
	register struct _G2AnimQuatInfo_Type *quatInfo;
	// register: $s1
	register long alpha;
	// address: -40
	// size: 0x8
	auto struct _G2Quat_Type newQuat;
	// register: $s2
	register int quatInfoChunkCount;
	// register: $s4
	register int segCount;
	{ //line: 30
		// register: $s5
		// size: 0x8
		register struct _G2Quat_Type *source;
		// register: $s0
		// size: 0x8
		register struct _G2Quat_Type *dest;
		{ //line: 30
			// register: $v1
			register unsigned long zw;
			// register: $v0
			register unsigned long xy;
		} //line: 30
	} //line: 30
	{ //line: 30
		// register: $v1
		// size: 0x6
		register struct _G2SVector3_Type *dest;
		// register: $v0
		// size: 0x6
		register struct _G2SVector3_Type *offset;
		// register: $s1
		register long alpha;
	} //line: 30
	{ //line: 30
		// register: $v1
		// size: 0x6
		register struct _G2SVector3_Type *dest;
		// register: $v0
		// size: 0x6
		register struct _G2SVector3_Type *offset;
		// register: $s1
		register long alpha;
	} //line: 30
} //line: 55


// path: C:\kain2\game\G2\ANMINTRP.C
// address: 0x80092880
// line start: 318
// line end:   383
void _G2AnimSection_SegValueToQuat(struct _G2AnimSection_Type *section, int zeroOne)
{ //line: 1
	// register: $s2
	// size: 0x18
	register struct _G2AnimSegValue_Type *segValue;
	// register: $v0
	// size: 0xC
	register struct _G2AnimInterpInfo_Type *interpInfo;
	// register: $s4
	// size: 0xA4
	register struct _G2AnimInterpStateBlock_Type *stateBlockList;
	// register: $s0
	// size: 0x28
	register struct _G2AnimQuatInfo_Type *quatInfo;
	// address: -40
	// size: 0x8
	auto struct _G2EulerAngles_Type preQuat;
	// register: $s3
	register int quatInfoChunkCount;
	// register: $s5
	register int segCount;
} //line: 66


// path: C:\kain2\game\G2\ANMINTRP.C
// address: 0x80092A0C
// size: 0xA4
// line start: 388
// line end:   424
struct _G2AnimInterpStateBlock_Type *_G2Anim_AllocateInterpStateBlockList(struct _G2AnimSection_Type *section)
{ //line: 1
	// register: $s2
	// size: 0xC
	register struct _G2AnimInterpInfo_Type *interpInfo;
	// register: $s1
	// size: 0xA4
	register struct _G2AnimInterpStateBlock_Type *newBlock;
	// register: $s0
	register int segCount;
} //line: 37


// path: C:\kain2\game\G2\ANMINTRP.C
// address: 0x80092AA4
// line start: 429
// line end:   440
void _G2Anim_FreeInterpStateBlockList(struct _G2AnimInterpStateBlock_Type *block)
{ //line: 1
	// register: $s0
	// size: 0xA4
	register struct _G2AnimInterpStateBlock_Type *nextBlock;
} //line: 12


// path: C:\kain2\game\G2\ANIMG2.C
// address: 0x80092AE4
// size: 0x4
// line start: 260
// line end:   296
enum _G2Bool_Enum_duplicate_e80 G2Anim_Install()
{ //line: 1
	// register: $a0
	// size: 0x24
	register struct _G2AnimController_Type *dummyController;
} //line: 1


// path: C:\kain2\game\G2\ANIMG2.C
// address: 0x80092B4C
// line start: 309
// line end:   337
void G2Anim_ResetInternalState()
{ //line: 1
	// register: $v0
	// size: 0x24
	register struct _G2AnimController_Type *dummyController;
} //line: 1


// path: C:\kain2\game\G2\ANIMG2.C
// address: 0x80092BA4
// line start: 374
// line end:   428
void G2Anim_Init(struct _G2Anim_Type *anim, struct _Model *modelData)
{ //line: 1
	// register: $v0
	// size: 0x30
	register struct _G2AnimSection_Type *section;
	// register: $a0
	register int sectionID;
} //line: 54


// path: C:\kain2\game\G2\ANIMG2.C
// address: 0x80092C38
// size: 0x30
// line start: 439
// line end:   478
struct _G2AnimSection_Type *G2Anim_AddSection(struct _G2Anim_Type *anim, int firstSegID, int segCount)
{ //line: 1
	// register: $s0
	// size: 0x30
	register struct _G2AnimSection_Type *section;
} //line: 1


// path: C:\kain2\game\G2\ANIMG2.C
// address: 0x80092CD4
// line start: 487
// line end:   526
void G2Anim_Free(struct _G2Anim_Type *anim)
{ //line: 1
	// register: $s1
	// size: 0x30
	register struct _G2AnimSection_Type *animSection;
	// register: $s2
	register int sectionID;
	// register: $s0
	// size: 0xC
	register struct _G2AnimInterpInfo_Type *interpInfo;
} //line: 40


// path: C:\kain2\game\G2\ANIMG2.C
// address: 0x80092D90
// line start: 531
// line end:   552
void G2Anim_Restore(struct _G2Anim_Type *anim)
{ //line: 1
	// register: $a0
	// size: 0x30
	register struct _G2AnimSection_Type *animSection;
	// register: $s0
	register int sectionID;
} //line: 22


// path: C:\kain2\game\G2\ANIMG2.C
// address: 0x80092E10
// line start: 562
// line end:   608
void G2Anim_BuildTransforms(struct _G2Anim_Type *anim)
{ //line: 1
	{ //line: 12
	} //line: 12
	{ //line: 24
		{ //line: 24
			// register: $v0
			register unsigned short z;
			// register: $v1
			register unsigned long xy;
		} //line: 24
	} //line: 24
	{ //line: 36
	} //line: 36
} //line: 36


// path: C:\kain2\game\G2\ANIMG2.C
// address: 0x80092EDC
// line start: 617
// line end:   702
void G2Anim_UpdateStoredFrame(struct _G2Anim_Type *anim)
{ //line: 1
	// register: $s0
	// size: 0x30
	register struct _G2AnimSection_Type *section;
	// register: $s3
	register short storedTime;
	// register: $s4
	register short elapsedTime;
	// address: -32
	// size: 0x6
	auto struct _G2SVector3_Type motionVector;
	// register: $s1
	register int sectionCount;
	// register: $v0
	// size: 0xC
	register struct _G2AnimInterpInfo_Type *interpInfo;
	{ //line: 52
		// register: $a3
		// size: 0x6
		register struct _G2SVector3_Type *vector;
	} //line: 52
} //line: 86


// path: C:\kain2\game\G2\ANIMG2.C
// address: 0x80093058
// size: 0x30
// line start: 712
// line end:   747
struct _G2AnimSection_Type *G2Anim_GetSectionWithSeg(struct _G2Anim_Type *anim, int segNumber)
{ //line: 3
	// register: $a3
	// size: 0x30
	register struct _G2AnimSection_Type *section;
	// register: $a2
	// size: 0x30
	register struct _G2AnimSection_Type *tempSection;
	// register: $a0
	// size: 0x30
	register struct _G2AnimSection_Type *endSection;
	// register: $v1
	register int firstSeg;
	// register: $v0
	register int lastSeg;
} //line: 35


// path: C:\kain2\game\G2\ANIMG2.C
// address: 0x800930C4
// size: 0x4
// line start: 758
// line end:   851
enum _G2Bool_Enum_duplicate_e80 G2Anim_SegmentHasActiveChannels(struct _G2Anim_Type *anim, int segNumber, unsigned short chanMask)
{ //line: 1
	// register: $a1
	// size: 0x30
	register struct _G2AnimSection_Type *section;
	// register: $a0
	register unsigned char *segChanFlagStream;
	// register: $a2
	register unsigned char activeChanBits;
	// register: $v1
	register unsigned short dataFlagBits;
	// register: $a1
	register unsigned short segFlagBits;
	// register: $t0
	register int flagBytesPerSeg;
} //line: 90


// path: C:\kain2\game\G2\ANIMG2.C
// address: 0x80093204
// line start: 866
// line end:   904
void G2Anim_GetSegChannelValue(struct _G2Anim_Type *anim, int segIndex, unsigned short *valueTable, unsigned short channelMask)
{ //line: 1
	// register: $v1
	register unsigned short *chanFinalValue;
	{ //line: 1
		{ //line: 1
			// register: $v1
			register unsigned short z;
			// register: $v0
			register unsigned long xy;
		} //line: 1
	} //line: 1
} //line: 39


// path: C:\kain2\game\G2\ANIMG2.C
// address: 0x800932B0
// line start: 922
// line end:   1014
void G2Anim_GetRootMotionFromTimeForDuration(struct _G2Anim_Type *anim, short durationStart, short duration, struct _G2SVector3_Type *motionVector)
{ //line: 1
	// address: -216
	// size: 0xB4
	auto struct _G2Anim_Type dummyAnim;
	// register: $s2
	// size: 0x30
	register struct _G2AnimSection_Type *section;
	// register: $s4
	// size: 0x14
	register struct _G2AnimKeylist_Type *keylist;
	// register: $s2
	register short storedKeyEndTime;
	// register: $s0
	register short timePerKey;
	// register: $a0
	register short keyTime;
	// register: $a1
	register long alpha;
	// register: $a0
	// size: 0xC
	register struct _G2AnimInterpInfo_Type *interpInfo;
	{ //line: 27
		// register: $s5
		// size: 0x6
		register struct _G2SVector3_Type *dest;
		// register: $v0
		// size: 0x6
		register struct _G2SVector3_Type *base;
		// register: $v1
		// size: 0x6
		register struct _G2SVector3_Type *offset;
		// register: $a1
		register long alpha;
	} //line: 27
	{ //line: 43
		// register: $s5
		// size: 0x6
		register struct _G2SVector3_Type *vector;
	} //line: 43
	{ //line: 79
		// register: $s5
		// size: 0x6
		register struct _G2SVector3_Type *dest;
		// register: $s5
		// size: 0x6
		register struct _G2SVector3_Type *base;
		// register: $a1
		register long alpha;
	} //line: 79
} //line: 93


// path: C:\kain2\game\G2\ANIMG2.C
// address: 0x80093588
// line start: 1121
// line end:   1207
void G2AnimSection_SwitchToKeylistAtTime(struct _G2AnimSection_Type *section, struct _G2AnimKeylist_Type *keylist, int keylistID, short targetTime)
{ //line: 1
	// register: $s2
	// size: 0xB4
	register struct _G2Anim_Type *anim;
	// address: -40
	// size: 0x6
	auto struct _G2SVector3_Type rootMotion;
	// register: $s0
	// size: 0xC
	register struct _G2AnimInterpInfo_Type *interpInfo;
	{ //line: 23
	} //line: 23
	{ //line: 76
		{ //line: 76
			// register: $v1
			register unsigned short z;
			// register: $v0
			register unsigned long xy;
		} //line: 76
	} //line: 76
} //line: 85


// path: C:\kain2\game\G2\ANIMG2.C
// address: 0x80093740
// line start: 1225
// line end:   1267
void G2AnimSection_JumpToTime(struct _G2AnimSection_Type *section, short targetTime)
{ //line: 1
	// register: $s1
	// size: 0xB4
	register struct _G2Anim_Type *anim;
	{ //line: 41
	} //line: 41
} //line: 43


// path: C:\kain2\game\G2\ANIMG2.C
// address: 0x800937E0
// line start: 1273
// line end:   1360
short G2AnimSection_UpdateOverInterval(struct _G2AnimSection_Type *section, short interval)
{ //line: 1
	// register: $s2
	// size: 0xB4
	register struct _G2Anim_Type *anim;
	// address: -32
	// size: 0x6
	auto struct _G2SVector3_Type motionVector;
	// register: $s1
	// size: 0xC
	register struct _G2AnimInterpInfo_Type *interpInfo;
	// register: $s3
	register short elapsedTime;
	{ //line: 51
		{ //line: 51
			// register: $v1
			register unsigned short z;
			// register: $v0
			register unsigned long xy;
		} //line: 51
	} //line: 51
} //line: 88


// path: C:\kain2\game\G2\ANIMG2.C
// address: 0x800939D4
// line start: 1381
// line end:   1657
short G2AnimSection_AdvanceOverInterval(struct _G2AnimSection_Type *section, short interval)
{ //line: 1
	// register: $a0
	// size: 0x14
	register struct _G2AnimKeylist_Type *keylist;
	// register: $s6
	register short newTime;
	// register: $s2
	register short extraTime;
	// register: $fp
	register short elapsedTime;
	// register: $s4
	register short endTime;
	// address: -48
	auto short loopExtraTime;
	// register: $s0
	register short *swAlarmTable;
	// register: $v1
	register short swAlarmTime;
	// register: $s7
	// size: 0xB4
	register struct _G2Anim_Type *anim;
	// address: -56
	// size: 0x6
	auto struct _G2SVector3_Type motionVector;
	// register: $s3
	register unsigned long message;
	{ //line: 233
	} //line: 233
	{ //line: 243
		{ //line: 243
			// register: $v1
			register unsigned short z;
			// register: $v0
			register unsigned long xy;
		} //line: 243
	} //line: 243
} //line: 277


// path: C:\kain2\game\G2\ANIMG2.C
// address: 0x80093DAC
// line start: 1671
// line end:   1866
short G2AnimSection_RewindOverInterval(struct _G2AnimSection_Type *section, short interval)
{ //line: 1
	// register: $s3
	register short newTime;
	// register: $s6
	register short extraTime;
	// register: $s7
	register short elapsedTime;
	// register: $s2
	register short endTime;
	// register: $s4
	// size: 0xB4
	register struct _G2Anim_Type *anim;
	// address: -48
	// size: 0x6
	auto struct _G2SVector3_Type motionVector;
	// register: $s5
	register unsigned long message;
	{ //line: 161
	} //line: 161
	{ //line: 170
		{ //line: 170
			// register: $v1
			register unsigned short z;
			// register: $v0
			register unsigned long xy;
		} //line: 170
	} //line: 170
} //line: 196


// path: C:\kain2\game\G2\ANIMG2.C
// address: 0x80093FEC
// line start: 1876
// line end:   1948
void _G2Anim_BuildTransformsNoControllers(struct _G2Anim_Type *anim)
{ //line: 1
	// register: $a1
	// size: 0x18
	register struct _Segment *segment;
	// register: $s5
	// size: 0x20
	register struct _G2Matrix_Type *segMatrix;
	// register: $a2
	// size: 0x4
	register enum _G2Bool_Enum_duplicate_e80 bRootTransUpdated;
	// register: $s3
	register int segIndex;
	// register: $s6
	register int segCount;
	// address: -56
	// size: 0xC
	auto unsigned long disabledBits[3];
	// register: $s1
	register unsigned long disabledMask;
	// register: $v1
	register unsigned long parentMask;
	// register: $v0
	register unsigned long parentIndex;
} //line: 73


// path: C:\kain2\game\G2\ANIMG2.C
// address: 0x80094140
// line start: 1959
// line end:   2057
void _G2Anim_BuildSegTransformNoControllers(struct _G2Matrix_Type *segMatrix, struct _G2Matrix_Type *parentMatrix, enum _G2Bool_Enum_duplicate_e80 bRootTransUpdated, int segIndex)
{ //line: 1
	// register: $s1
	// size: 0x18
	register struct _G2AnimSegValue_Type *segValue;
	// address: -40
	// size: 0xC
	auto struct _G2LVector3_Type scale;
	{ //line: 50
		// register: $v0
		// size: 0x6
		register struct _G2SVector3_Type *svector;
		// register: $v1
		// size: 0xC
		register struct _G2LVector3_Type *lvector;
	} //line: 50
} //line: 96


// path: C:\kain2\game\G2\ANIMG2.C
// address: 0x80094368
// line start: 2063
// line end:   2073
void _G2Anim_BuildSegLocalRotMatrix(struct _G2AnimSegValue_Type *segValue, struct _G2Matrix_Type *segMatrix)
{ //line: 1
	// address: -16
	// size: 0x6
	auto struct _G2SVector3_Type rot;
	{ //line: 7
		// register: $v1
		// size: 0x6
		register struct _G2SVector3_Type *source;
		// register: $a0
		// size: 0x6
		register struct _G2SVector3_Type *dest;
		{ //line: 7
			// register: $a2
			register unsigned long mask;
			// register: $v0
			register unsigned short z;
			// register: $v1
			register unsigned long xy;
		} //line: 7
	} //line: 7
} //line: 11


// path: C:\kain2\game\G2\ANIMG2.C
// address: 0x800943C8
// line start: 2113
// line end:   2124
void wombat(unsigned char *segKeyList, int flagBitOffset, struct _G2AnimSegKeyflagInfo_Type *kfInfo)
{ //line: 3
	// register: $a3
	register int flagDWordOffset;
	// register: $a1
	register int flagBitShift;
} //line: 5


// path: C:\kain2\game\G2\ANIMG2.C
// address: 0x80094400
// line start: 2129
// line end:   2160
unsigned long kangaroo(struct _G2AnimSegKeyflagInfo_Type *kfInfo)
{ //line: 3
	// register: $a2
	register unsigned long keyflags;
	// register: $v0
	register unsigned long tempFlags;
} //line: 31


// path: C:\kain2\game\G2\ANIMG2.C
// address: 0x80094488
// line start: 2167
// line end:   2178
void _G2Anim_InitializeSegValue(struct _G2Anim_Type *anim, struct _G2AnimSegValue_Type *segValue, int segIndex)
{ //line: 3
	// register: $v0
	// size: 0x18
	register struct _Segment *segment;
	{ //line: 3
		// register: $a1
		// size: 0x8
		register struct _G2Quat_Type *quat;
	} //line: 3
	{ //line: 3
	} //line: 3
	{ //line: 3
		{ //line: 3
			// register: $v0
			register unsigned long zpad;
			// register: $v1
			register unsigned long xy;
		} //line: 3
	} //line: 3
} //line: 3


// path: C:\kain2\game\G2\ANIMG2.C
// address: 0x800944DC
// line start: 2224
// line end:   2337
void _G2AnimSection_InitStatus(struct _G2AnimSection_Type *section, struct _G2Anim_Type *anim)
{ //line: 1
	// address: -104
	// size: 0x10
	auto struct _G2AnimDecompressChannelInfo_Type dcInfo;
	// register: $s6
	// size: 0x18
	register struct _G2AnimSegValue_Type *segValue;
	// register: $s5
	// size: 0x24
	register struct _G2AnimChanStatusBlock_Type **chanStatusNextBlockPtr;
	// register: $v0
	// size: 0x24
	register struct _G2AnimChanStatusBlock_Type *chanStatusBlock;
	// register: $s2
	// size: 0x4
	register struct _G2AnimChanStatus_Type *chanStatus;
	// register: $s3
	register int chanStatusChunkCount;
	// address: -88
	// size: 0xC
	auto struct _G2AnimSegKeyflagInfo_Type rotKfInfo;
	// address: -72
	// size: 0xC
	auto struct _G2AnimSegKeyflagInfo_Type scaleKfInfo;
	// address: -56
	// size: 0xC
	auto struct _G2AnimSegKeyflagInfo_Type transKfInfo;
	// register: $s0
	register int type;
	// register: $s1
	register unsigned long segChanFlags;
	// register: $s4
	register int segIndex;
	// register: $fp
	register int lastSeg;
	{ //line: 1
		// address: 4
		// size: 0xB4
		auto struct _G2Anim_Type *anim;
		// register: $s7
		// size: 0x30
		register struct _G2AnimSection_Type *section;
		{ //line: 1
			// register: $s2
			register int bitsPerFlagType;
			// register: $s0
			register int flagBitOffset;
			// register: $s3
			register unsigned long activeChanBits;
			// register: $s1
			register unsigned char *segKeyList;
		} //line: 1
	} //line: 1
} //line: 112


// path: C:\kain2\game\G2\ANIMG2.C
// address: 0x80094788
// line start: 2346
// line end:   2507
void FooBar(struct _G2AnimSection_Type *section, struct _G2Anim_Type *anim, int decompressedKey, int targetKey, long timeOffset)
{ //line: 1
	// address: -152
	// size: 0x10
	auto struct _G2AnimDecompressChannelInfo_Type dcInfo;
	// register: $s6
	// size: 0x18
	register struct _G2AnimSegValue_Type *segValue;
	// register: $s1
	register short *chanValue;
	// register: $s7
	// size: 0x24
	register struct _G2AnimChanStatusBlock_Type *chanStatusBlock;
	// register: $s3
	// size: 0x4
	register struct _G2AnimChanStatus_Type *chanStatus;
	// register: $s5
	register int chanStatusChunkCount;
	// address: -136
	// size: 0xC
	auto struct _G2AnimSegKeyflagInfo_Type rotKfInfo;
	// address: -120
	// size: 0xC
	auto struct _G2AnimSegKeyflagInfo_Type scaleKfInfo;
	// address: -104
	// size: 0xC
	auto struct _G2AnimSegKeyflagInfo_Type transKfInfo;
	// register: $s0
	register int type;
	// register: $s2
	register unsigned long segChanFlags;
	// register: $s4
	register int segIndex;
	// address: -48
	auto int lastSeg;
	// address: -88
	// size: 0x10
	auto struct _G2AnimDecompressChannelInfo_Type nextDCInfo;
	// address: -72
	// size: 0x10
	auto struct _G2AnimDecompressChannelInfo_Type initDCInfo;
	// address: -56
	// size: 0x4
	auto struct _G2AnimChanStatus_Type nextChanStatus;
	{ //line: 1
		// address: 4
		// size: 0xB4
		auto struct _G2Anim_Type *anim;
		// register: $s6
		// size: 0x30
		register struct _G2AnimSection_Type *section;
		{ //line: 1
			// register: $s2
			register int bitsPerFlagType;
			// register: $s0
			register int flagBitOffset;
			// register: $s3
			register unsigned long activeChanBits;
			// register: $s1
			register unsigned char *segKeyList;
		} //line: 1
	} //line: 1
} //line: 162


// path: C:\kain2\game\G2\ANIMG2.C
// address: 0x80094C10
// line start: 2513
// line end:   2552
void _G2AnimSection_UpdateStoredFrameFromData(struct _G2AnimSection_Type *section, struct _G2Anim_Type *anim)
{ //line: 1
	// register: $s2
	register short timePerKey;
	// register: $a2
	register long storedKey;
	// register: $s1
	register long targetKey;
	// register: $v0
	register long timeOffset;
} //line: 33


// path: C:\kain2\game\G2\ANIMG2.C
// address: 0x80094CE0
// size: 0xB4
// line start: 2557
// line end:   2569
struct _G2Anim_Type *_G2AnimSection_GetAnim(struct _G2AnimSection_Type *section)
{ //line: 3
} //line: 7


// path: C:\kain2\game\G2\ANIMG2.C
// address: 0x80094D00
// line start: 2576
// line end:   2616
void _G2AnimSection_TriggerEffects(struct _G2AnimSection_Type *section, short startTime, short endTime)
{ //line: 1
	// register: $s4
	// size: 0x14
	register struct _G2AnimKeylist_Type *keylist;
	// register: $s0
	// size: 0x4
	register struct _G2AnimFxHeader_Type *fxHeader;
	// register: $a0
	register int fxSectionID;
	// register: $s2
	register unsigned long fxSize;
} //line: 41


// path: C:\kain2\game\G2\ANIMG2.C
// address: 0x80094E2C
// line start: 2621
// line end:   2632
void _G2Anim_FreeChanStatusBlockList(struct _G2AnimChanStatusBlock_Type *block)
{ //line: 1
	// register: $s0
	// size: 0x24
	register struct _G2AnimChanStatusBlock_Type *nextBlock;
} //line: 12


// path: C:\kain2\game\G2\ANIMG2.C
// address: 0x80094E6C
// line start: 2638
// line end:   2656
long _G2AnimAlphaTable_GetValue(struct _G2AnimAlphaTable_Type *table, long trueAlpha)
{ //line: 1
	// register: $a0
	register long position;
	// register: $v1
	register long positionInt;
	// register: $a0
	register long positionFrac;
	// register: $a1
	register long value;
} //line: 19


// path: C:\kain2\game\PLAN\PLANAPI.C
// address: 0x80097CCC
// line start: 131
// line end:   150
void PLANAPI_ConvertPlanIntoEnmyPlanDataFormat(struct PlanningNode *goalNode, struct EnemyPlanData *planData, struct PlanningNode *planningPool)
{ //line: 1
	// register: $s0
	// size: 0x1C
	register struct PlanningNode *currentNode;
	// register: $a0
	register int i;
} //line: 20


// path: C:\kain2\game\PLAN\PLANAPI.C
// address: 0x80097D80
// line start: 157
// line end:   172
int PLANAPI_FindPathBetweenNodes(struct PlanningNode *startNode, struct PlanningNode *goalNode, struct EnemyPlanData *planData, int validNodeTypes)
{ //line: 1
	// register: $s1
	// size: 0x1C
	register struct PlanningNode *planningPool;
	// register: $s2
	register int successFlag;
} //line: 15


// path: C:\kain2\game\PLAN\PLANAPI.C
// address: 0x80097E08
// line start: 177
// line end:   221
void PLANAPI_DoTimingCalcsAndDrawing(long startTime, struct PlanningNode *planningPool)
{ //line: 1
	// register: $a1
	register int i;
	// register: $s1
	register long minTime;
	// register: $s0
	register long maxTime;
	{ //line: 22
	} //line: 44
} //line: 45


// path: C:\kain2\game\PLAN\PLANAPI.C
// address: 0x80097F24
// line start: 269
// line end:   293
int PLANAPI_AddNodeOfTypeToPool(struct _Position *pos, int type)
{ //line: 1
	// register: $s1
	// size: 0x1C
	register struct PlanningNode *planningPool;
	// register: $v1
	register int foundHit;
	// address: -24
	auto int nodePlacement;
	// address: -40
	// size: 0x10
	auto struct _PlanCollideInfo pci;
	{ //line: 15
		// register: $v0
		register short _x1;
		// register: $v1
		register short _y1;
		// register: $t0
		register short _z1;
		// register: $s0
		// size: 0x6
		register struct _Position *_v0;
	} //line: 15
} //line: 25


// path: C:\kain2\game\PLAN\PLANAPI.C
// address: 0x80098004
// line start: 306
// line end:   323
void PLANAPI_DeleteNodesFromPoolByType(int nodeSource)
{ //line: 1
	// register: $s2
	// size: 0x1C
	register struct PlanningNode *planningPool;
	// register: $s0
	// size: 0x1C
	register struct PlanningNode *nodeToDelete;
	// register: $s1
	register int i;
} //line: 18


// path: C:\kain2\game\PLAN\PLANAPI.C
// address: 0x800980A0
// line start: 327
// line end:   346
void PLANAPI_DeleteNodeFromPoolByUnit(long streamUnitID)
{ //line: 1
	// register: $s2
	// size: 0x1C
	register struct PlanningNode *planningPool;
	// register: $s0
	// size: 0x1C
	register struct PlanningNode *nodeToDelete;
	// register: $s1
	register int i;
} //line: 20


// path: C:\kain2\game\PLAN\PLANAPI.C
// address: 0x80098138
// line start: 388
// line end:   400
int PLANAPI_FindPathInGraphToTarget(struct _Position *startPos, struct EnemyPlanData *planData, int validNodeTypes)
{ //line: 1
	// register: $s0
	// size: 0x1C
	register struct PlanningNode *planningPool;
	// register: $s0
	// size: 0x1C
	register struct PlanningNode *startNode;
} //line: 1


// path: C:\kain2\game\PLAN\PLANAPI.C
// address: 0x8009819C
// line start: 405
// line end:   442
void PLANAPI_InitPlanning(void *planningPool)
{ //line: 2
	// register: $a1
	register int i;
	// register: $a0
	register int j;
} //line: 34


// path: C:\kain2\game\PLAN\PLANAPI.C
// address: 0x8009821C
// line start: 446
// line end:   459
short PLANAPI_PairType(struct PlanningNode *node1, struct PlanningNode *node2)
{ //line: 2
	// register: $a2
	register short placement1;
	// register: $a1
	register short placement2;
} //line: 11


// path: C:\kain2\game\PLAN\PLANAPI.C
// address: 0x80098264
// line start: 462
// line end:   484
int PLANAPI_PassThroughHit(struct PlanningNode *node1, struct PlanningNode *node2)
{ //line: 2
	// register: $a0
	register int src1;
	// register: $v1
	register int src2;
	// register: $v1
	register int res;
} //line: 23


// path: C:\kain2\game\PLAN\PLANAPI.C
// address: 0x800982D0
// line start: 491
// line end:   655
void PLANAPI_UpdatePlanningDatabase(struct GameTracker *gameTracker, struct _Instance *player)
{ //line: 1
	// register: $s3
	// size: 0x1C
	register struct PlanningNode *planningPool;
	// register: $s0
	// size: 0x1C
	register struct PlanningNode *node1;
	// register: $s1
	// size: 0x1C
	register struct PlanningNode *node2;
	// register: $s4
	register long startTime;
	// register: $s2
	register int pathExistsAbove;
	// register: $v1
	register int pathExistsBelow;
	{ //line: 46
		// register: $v0
		// size: 0x1C
		register struct PlanningNode *temp;
	} //line: 50
} //line: 161


// path: C:\kain2\game\PLAN\PLANAPI.C
// address: 0x80098718
// line start: 660
// line end:   661
int PLANAPI_NumNodesInPool(void *planningPool);

// path: C:\kain2\game\PLAN\PLANAPI.C
// address: 0x8009872C
// line start: 668
// line end:   712
void PLANAPI_InitPlanMkrList(struct _StreamUnit *streamUnit)
{ //line: 1
	// register: $s2
	register int i;
	// register: $v0
	register int terrainFoundFlag;
	// register: $v0
	// size: 0x188
	register struct Level *level;
	// register: $s3
	register unsigned short numPlanMkrs;
	// register: $v1
	// size: 0x8
	register struct _PlanMkr *planMkrList;
	{ //line: 15
		// address: -40
		// size: 0x10
		auto struct _PlanCollideInfo pci;
		{ //line: 16
			// register: $v0
			register short _x1;
			// register: $v1
			register short _y1;
			// register: $a0
			register short _z1;
			// register: $s1
			// size: 0x6
			register struct _Position *_v0;
		} //line: 16
		{ //line: 41
			// register: $v0
			register short _x1;
			// register: $v1
			register short _y1;
			// register: $a0
			register short _z1;
		} //line: 41
	} //line: 44
} //line: 45


// path: C:\kain2\game\PLAN\PLANAPI.C
// address: 0x80098850
// line start: 726
// line end:   749
int PLANAPI_GetFlags(int type)
{ //line: 2
	// register: $v0
	register int chk;
} //line: 24


// path: C:\kain2\game\PLAN\PLANAPI.C
// address: 0x800988B8
// line start: 752
// line end:   771
int PLANAPI_FindNodePositionInUnit(struct _StreamUnit *streamUnit, struct _Position *pos, int id, int type)
{ //line: 1
	{ //line: 4
		// register: $v0
		// size: 0x188
		register struct Level *level;
		// register: $s0
		register int numPlanMkrs;
		// register: $s1
		// size: 0x8
		register struct _PlanMkr *planMkr;
		// register: $s0
		register int i;
		{ //line: 14
			// register: $v1
			register short _x1;
			// register: $a0
			register short _y1;
			// register: $a1
			register short _z1;
		} //line: 14
	} //line: 18
} //line: 20


// path: C:\kain2\game\PLAN\PLANAPI.C
// address: 0x8009895C
// line start: 777
// line end:   820
int PLANAPI_FindClosestNodePositionInUnit(struct _StreamUnit *streamUnit, struct _Position *target, struct _Position *pos, int offset, int max, int type, int distanceCheck)
{ //line: 1
	// register: $v0
	// size: 0x188
	register struct Level *level;
	// register: $s0
	register int numPlanMkrs;
	// register: $s1
	// size: 0x8
	register struct _PlanMkr *planMkr;
	// register: $s4
	// size: 0x6
	register struct _Position *ptr;
	// register: $v1
	register int dist;
	// register: $s5
	register int min_dist;
	// register: $s7
	register int chk;
	// register: $s6
	register int res;
	// register: $s2
	register int i;
	{ //line: 40
		// register: $v0
		register short _x1;
		// register: $v1
		register short _y1;
		// register: $a0
		register short _z1;
	} //line: 40
} //line: 43


// path: C:\kain2\game\PLAN\PLANCOLL.C
// address: 0x80098AE0
// line start: 75
// line end:   97
int PLANCOLL_DoesLOSExistFinal(struct _Position *startPos, struct _Position *endPos, int collideType, int passThroughHit, int zoffset)
{ //line: 1
	// address: -72
	// size: 0x2C
	auto struct _PCollideInfo pcollideinfo;
	// address: -24
	// size: 0x8
	auto struct SVECTOR startPt;
	// address: -16
	// size: 0x8
	auto struct SVECTOR endPt;
	{ //line: 1
		// register: $v0
		register short _x1;
		// register: $a2
		register short _y1;
		// register: $a0
		register short _z1;
		// register: $v1
		// size: 0x6
		register struct _Position *_v0;
	} //line: 1
	{ //line: 1
		// register: $v0
		register short _x1;
		// register: $a0
		register short _y1;
		// register: $a1
		register short _z1;
		// register: $v0
		// size: 0x6
		register struct _Position *_v0;
	} //line: 1
} //line: 23


// path: C:\kain2\game\PLAN\PLANCOLL.C
// address: 0x80098BE4
// line start: 108
// line end:   145
int PLANCOLL_CheckUnderwaterPoint(struct _Position *position)
{ //line: 1
	// register: $s4
	// size: 0x40
	register struct _StreamUnit *streamUnit;
	// register: $s2
	// size: 0x188
	register struct Level *level;
	// register: $a3
	// size: 0x24
	register struct BSPTree *tree;
	// register: $s0
	// size: 0x8
	register struct _Sphere_noSq *sphere;
	// register: $a3
	// size: 0x6
	register struct _Position *offset;
	// register: $s3
	register int d;
	// address: -40
	// size: 0x6
	auto struct _Position center;
	{ //line: 16
		// register: $v1
		register short _x1;
		// register: $v0
		register short _y1;
		// register: $a0
		register short _z1;
		// register: $s6
		// size: 0x6
		register struct _Position *_v0;
	} //line: 16
} //line: 38


// path: C:\kain2\game\PLAN\PLANCOLL.C
// address: 0x80098D50
// line start: 162
// line end:   207
int PLANCOLL_FindTerrainHitFinal(struct _PlanCollideInfo *pci, int *placement, int distBefore, int distAfter, int start, int end)
{ //line: 1
	// address: -120
	// size: 0x2C
	auto struct _PCollideInfo pcollideinfo;
	// register: $s1
	// size: 0x4
	register struct _fth *current;
	// register: $s6
	// size: 0x4
	register struct _fth *last;
	// address: -72
	// size: 0x8
	auto struct SVECTOR startPt;
	// address: -64
	// size: 0x8
	auto struct SVECTOR endPt;
	// address: -56
	// size: 0x8
	auto struct _SVector normal;
	{ //line: 18
		// register: $a0
		register short _x1;
		// register: $v0
		register short _y1;
		// register: $v1
		register short _z1;
		// register: $s4
		// size: 0x6
		register struct _Position *_v0;
		// register: $s2
		// size: 0x6
		register struct _Position *_v1;
	} //line: 18
	{ //line: 18
		// register: $a1
		register short _x1;
		// register: $v0
		register short _y1;
		// register: $v1
		register short _z1;
		// register: $s3
		// size: 0x6
		register struct _Position *_v0;
	} //line: 18
	{ //line: 39
		// register: $v1
		register short _y1;
		// register: $a0
		register short _z1;
	} //line: 39
} //line: 46


// path: C:\kain2\game\PLAN\PLANCOLL.C
// address: 0x80099010
// line start: 214
// line end:   222
int PLANCOLL_DoesStraightLinePathExist(struct _Position *startPos, struct _Position *endPos, int collideType)
{ //line: 1
} //line: 7


// path: C:\kain2\game\PLAN\PLANCOLL.C
// address: 0x8009909C
// line start: 227
// line end:   251
int PLANCOLL_DoesWaterPathUpExist(struct _Position *startPos, struct _Position *endPos, int collideType, struct _Position *peakPos, int passThroughHit)
{ //line: 1
	// register: $s0
	register long time_ftop;
	// register: $a0
	register long time_ptow;
	// register: $a1
	register long time_tot;
	// address: -32
	// size: 0x6
	auto struct _Position diff;
} //line: 19


// path: C:\kain2\game\PSX\PSX_G2\MAINVM.C
// address: 0x80096400
// line start: 50
// line end:   69
int main();

// path: C:\kain2\game\PSX\PSX_G2\QUATVM.C
// address: 0x80096434
// line start: 93
// line end:   173
void G2Quat_Slerp_VM(long ratio, struct _G2Quat_Type *quatA, struct _G2Quat_Type *quatB, struct _G2Quat_Type *quatOut, int spin)
{ //line: 1
	// register: $s1
	register long beta;
	// register: $s0
	register short theta;
	// register: $a0
	register short cos_t;
	// register: $s4
	register long bflip;
	// address: -56
	// size: 0x10
	auto long foo[4];
	// register: $v1
	register long cosTemp1;
	// address: -40
	auto long cosTemp2;
} //line: 57


// path: C:\kain2\game\PLAN\ENMYPLAN.C
// address: 0x800966E4
// line start: 107
// line end:   116
void ENMYPLAN_InitEnemyPlanPool(void *enemyPlanPool)
{ //line: 2
	// register: $v0
	register int i;
} //line: 10


// path: C:\kain2\game\PLAN\ENMYPLAN.C
// address: 0x80096704
// line start: 131
// line end:   143
int ENMYPLAN_GetNextAvailablePlanningSlot()
{ //line: 2
	// register: $a1
	register int slotID;
	// register: $v1
	register int i;
} //line: 12


// path: C:\kain2\game\PLAN\ENMYPLAN.C
// address: 0x80096744
// line start: 167
// line end:   181
int ENMYPLAN_GetInitializedPlanningWorkspaceFinal()
{ //line: 1
	// register: $s0
	// size: 0x5E
	register struct EnemyPlanSlotData *pool;
	// register: $a0
	register int slotID;
} //line: 14


// path: C:\kain2\game\PLAN\ENMYPLAN.C
// address: 0x800967A0
// line start: 185
// line end:   202
void ENMYPLAN_ReleasePlanningWorkspace(int slotID)
{ //line: 1
	// register: $s1
	// size: 0x5E
	register struct EnemyPlanSlotData *pool;
} //line: 18


// path: C:\kain2\game\PLAN\ENMYPLAN.C
// address: 0x80096808
// line start: 208
// line end:   231
int ENMYPLAN_WayPointSkipped(struct _Position *currentPos, struct _Position *targetPos, struct _Position *nextTargetPos)
{ //line: 1
	// address: -40
	// size: 0x8
	auto long range[2];
	// address: -32
	// size: 0xC
	auto struct _Position vector[2];
	{ //line: 1
		// register: $v1
		register short _x0;
		// register: $v0
		register short _y0;
		// register: $a1
		register short _z0;
		// register: $a2
		register short _x1;
		// register: $a3
		register short _y1;
		// register: $t0
		register short _z1;
		// register: $a0
		// size: 0x6
		register struct _Position *_v;
	} //line: 1
	{ //line: 1
		// register: $a0
		register short _x0;
		// register: $v0
		register short _y0;
		// register: $v1
		register short _z0;
		// register: $a1
		register short _x1;
		// register: $a2
		register short _y1;
		// register: $a3
		register short _z1;
		// register: $a1
		// size: 0x6
		register struct _Position *_v;
	} //line: 1
} //line: 17


// path: C:\kain2\game\PLAN\ENMYPLAN.C
// address: 0x80096974
// line start: 235
// line end:   248
int ENMYPLAN_WayPointReached(struct _Position *currentPos, struct _Position *oldCurrentPos, struct _Position *targetPos)
{ //line: 1
	// address: -24
	// size: 0x8
	auto long range[2];
} //line: 14


// path: C:\kain2\game\PLAN\ENMYPLAN.C
// address: 0x80096A24
// line start: 267
// line end:   270
void ENMYPLAN_Replan(struct EnemyPlanSlotData *planSlot);

// path: C:\kain2\game\PLAN\ENMYPLAN.C
// address: 0x80096A50
// line start: 274
// line end:   308
int ENMYPLAN_PathClear(struct _Position *pos, struct _Position *target)
{ //line: 1
	// register: $a1
	register int len;
	// register: $s2
	register int oldPCO;
	// register: $a1
	// size: 0x6
	register struct _Position *tpos;
	// address: -24
	// size: 0x6
	auto struct _Position newTarget;
} //line: 30


// path: C:\kain2\game\PLAN\ENMYPLAN.C
// address: 0x80096BD0
// line start: 313
// line end:   539
int ENMYPLAN_MoveToTargetFinal(struct _Instance *instance, struct _Position *outputPos, int slotID, struct _Position *targetPos, int validNodeTypes)
{ //line: 1
	// register: $a0
	// size: 0x5E
	register struct EnemyPlanSlotData *pool;
	// register: $s0
	// size: 0x5E
	register struct EnemyPlanSlotData *planSlot;
	// register: $s5
	// size: 0x44
	register struct EnemyPlanData *planData;
	// register: $a0
	register int nextWayPointType;
	// register: $s2
	register int nextNextWayPointType;
	// register: $v0
	register int pathFound;
	// register: $s1
	register int nextWayPointIdx;
	// register: $s4
	register int nextNextWayPointIdx;
	// register: $a3
	register int nextWayPointSkip;
	// register: $v1
	register int i;
	// register: $s6
	// size: 0x6
	register struct _Position *nextWayPointPos;
	// register: $fp
	// size: 0x6
	register struct _Position *nextNextWayPointPos;
} //line: 227


// path: C:\kain2\game\PLAN\ENMYPLAN.C
// address: 0x800970DC
// line start: 542
// line end:   551
int ValidSlotAndState(struct EnemyPlanSlotData *pool, int slotID);

// path: C:\kain2\game\PLAN\ENMYPLAN.C
// address: 0x80097120
// line start: 555
// line end:   567
int ENMYPLAN_GetNodeTypeOfNextWaypoint(int slotID)
{ //line: 1
	// register: $s1
	// size: 0x5E
	register struct EnemyPlanSlotData *pool;
	// register: $v0
	// size: 0x5E
	register struct EnemyPlanSlotData *planSlot;
} //line: 13


// path: C:\kain2\game\PLAN\ENMYPLAN.C
// address: 0x8009718C
// line start: 570
// line end:   583
int ENMYPLAN_GetPosOfNextWaypoint(int slotID, struct _Position *pos)
{ //line: 1
	// register: $s1
	// size: 0x5E
	register struct EnemyPlanSlotData *pool;
	// register: $v1
	// size: 0x5E
	register struct EnemyPlanSlotData *planSlot;
	{ //line: 5
		// register: $a0
		register short _x1;
		// register: $a1
		register short _y1;
		// register: $v1
		register short _z1;
		// register: $v1
		// size: 0x6
		register struct _Position *_v1;
	} //line: 5
} //line: 14


// path: C:\kain2\game\PLAN\ENMYPLAN.C
// address: 0x8009722C
// line start: 641
// line end:   662
void ENMYPLAN_RelocatePlanPositions(int slotID, struct _Position *offset)
{ //line: 1
	// register: $v1
	// size: 0x5E
	register struct EnemyPlanSlotData *pool;
	// register: $t1
	// size: 0x5E
	register struct EnemyPlanSlotData *planSlot;
	// register: $a0
	// size: 0x6
	register struct _Position *pos;
	// register: $a3
	register int i;
	// register: $v1
	register int numWayPoints;
	{ //line: 18
		// register: $v0
		register short _x0;
		// register: $v0
		register short _y0;
		// register: $v1
		register short _z0;
		// register: $v1
		register short _x1;
		// register: $a1
		register short _y1;
		// register: $a2
		register short _z1;
	} //line: 18
} //line: 22


// path: C:\kain2\game\PLAN\PLAN.C
// address: 0x800972C8
// line start: 129
// line end:   147
long PLAN_CalcMinDistFromExistingNodes(struct _Position *pos, struct PlanningNode *planningPool, int distanceType)
{ //line: 1
	// register: $a2
	// size: 0x1C
	register struct PlanningNode *closestNode;
	// register: $v0
	register long minDist;
} //line: 19


// path: C:\kain2\game\PLAN\PLAN.C
// address: 0x80097364
// line start: 158
// line end:   220
void PLAN_UpdatePlanMkrNodes(struct PlanningNode *planningPool, struct _Position *playerPos)
{ //line: 1
	// register: $s4
	register int i;
	// address: -48
	auto int d;
	// register: $v1
	register long zDiff;
	// register: $s7
	// size: 0x40
	register struct _StreamUnit *streamUnit;
	{ //line: 11
		// register: $s5
		register int numPlanMkrs;
		// register: $s2
		// size: 0x8
		register struct _PlanMkr *planMkr;
		// address: -44
		auto long suID;
		{ //line: 23
			// register: $s0
			register long nodeType;
			// register: $s3
			register long nodeID;
		} //line: 45
	} //line: 48
} //line: 63


// path: C:\kain2\game\PLAN\PLAN.C
// address: 0x800975B0
// line start: 226
// line end:   264
void PLAN_UpdatePlayerNode(struct PlanningNode *planningPool, struct _Position *playerPos)
{ //line: 1
	// register: $s1
	// size: 0x1C
	register struct PlanningNode *playerNode;
	// address: -32
	auto int nodePlacement;
	// register: $s0
	register int foundHit;
	// address: -48
	// size: 0x10
	auto struct _PlanCollideInfo pci;
	{ //line: 19
		// register: $v0
		register short _x1;
		// register: $v1
		register short _y1;
		// register: $a3
		register short _z1;
		// register: $s0
		// size: 0x6
		register struct _Position *_v0;
	} //line: 19
} //line: 39


// path: C:\kain2\game\PLAN\PLAN.C
// address: 0x80097704
// line start: 273
// line end:   314
void PLAN_AddRandomNode(struct PlanningNode *planningPool, struct _Position *playerPos)
{ //line: 1
	// register: $s2
	register int i;
	// address: -56
	// size: 0x10
	auto struct _PlanCollideInfo pci;
	// register: $s4
	register int successFlag;
	{ //line: 9
		// register: $v0
		register short _x1;
		// register: $v1
		register short _y1;
		// register: $a0
		register short _z1;
		// register: $s1
		// size: 0x6
		register struct _Position *_v0;
	} //line: 9
	{ //line: 28
		{ //line: 34
			// address: -40
			// size: 0x8
			auto struct _SVector normal;
		} //line: 40
	} //line: 41
} //line: 42


// path: C:\kain2\game\PLAN\PLAN.C
// address: 0x80097910
// line start: 321
// line end:   326
void PLAN_DeleteRandomNode(struct PlanningNode *planningPool)
{ //line: 1
} //line: 1


// path: C:\kain2\game\PLAN\PLAN.C
// address: 0x80097944
// line start: 332
// line end:   344
void PLAN_DeleteOutOfRangeNodesOfSource(struct PlanningNode *planningPool, struct _Position *playerPos, int nodeSourceToCheck, long removeDist)
{ //line: 1
	// register: $s1
	register int i;
} //line: 13


// path: C:\kain2\game\PLAN\PLAN.C
// address: 0x80097A1C
// line start: 348
// line end:   359
void PLAN_AddOrRemoveRandomNodes(struct PlanningNode *planningPool, struct _Position *playerPos)
{ //line: 1
	// register: $v0
	register int numNodeError;
} //line: 12


// path: C:\kain2\game\PLAN\PLAN.C
// address: 0x80097A6C
// line start: 372
// line end:   380
void PLAN_AddInitialNodes(struct PlanningNode *planningPool, struct _Instance *player)
{ //line: 1
	// address: -40
	// size: 0x10
	auto struct _PlanCollideInfo pci;
	{ //line: 1
		// register: $v0
		register short _x1;
		// register: $v1
		register short _y1;
		// register: $t0
		register short _z1;
		// register: $s0
		// size: 0x6
		register struct _Position *_v0;
		// register: $s1
		// size: 0x6
		register struct _Position *_v1;
	} //line: 1
} //line: 1


// path: C:\kain2\game\PLAN\PLAN.C
// address: 0x80097B28
// line start: 385
// line end:   399
void PLAN_AddOrRemoveNodes(struct PlanningNode *planningPool, struct _Instance *player)
{ //line: 1
} //line: 14


// path: C:\kain2\game\PLAN\PLAN.C
// address: 0x80097C10
// size: 0x1C
// line start: 405
// line end:   423
struct PlanningNode *PLAN_FindNodeMostInNeedOfConnectivityExpansion(struct PlanningNode *planningPool)
{ //line: 1
	// register: $s2
	register int i;
	// register: $s1
	register int numConnections;
	// register: $s4
	register int minNumConnections;
	// register: $s3
	// size: 0x1C
	register struct PlanningNode *nodeToReturn;
} //line: 18


// path: C:\kain2\game\PLAN\PLANPOOL.C
// address: 0x800991D4
// line start: 122
// line end:   132
int PLANPOOL_NumNodesInPlan(struct PlanningNode *goalNode, struct PlanningNode *planningPool)
{ //line: 2
	// register: $a0
	// size: 0x1C
	register struct PlanningNode *currentNode;
	// register: $a2
	register int i;
} //line: 10


// path: C:\kain2\game\PLAN\PLANPOOL.C
// address: 0x80099220
// line start: 137
// line end:   146
int PLANPOOL_CountBitsSetInLong(long mylong)
{ //line: 2
	// register: $v1
	register int numBitsSet;
} //line: 9


// path: C:\kain2\game\PLAN\PLANPOOL.C
// address: 0x80099240
// line start: 150
// line end:   153
int PLANPOOL_NumConnectionsExaminedForNode(struct PlanningNode *node);

// path: C:\kain2\game\PLAN\PLANPOOL.C
// address: 0x80099264
// line start: 158
// line end:   160
int PLANPOOL_NumConnectionsForNode(struct PlanningNode *node);

// path: C:\kain2\game\PLAN\PLANPOOL.C
// address: 0x80099288
// size: 0x1C
// line start: 177
// line end:   185
struct PlanningNode *PLANPOOL_GetFirstNodeOfSource(struct PlanningNode *planningPool, char nodeSource)
{ //line: 1
	// register: $v1
	register int i;
} //line: 9


// path: C:\kain2\game\PLAN\PLANPOOL.C
// address: 0x800992D8
// line start: 189
// line end:   197
int PLANPOOL_NumberOfNodesOfType(struct PlanningNode *planningPool, char nodeType)
{ //line: 1
	// register: $a2
	register int i;
	// register: $v1
	register int numNodesOfType;
} //line: 8


// path: C:\kain2\game\PLAN\PLANPOOL.C
// address: 0x80099324
// line start: 201
// line end:   209
int PLANPOOL_NumberOfNodesOfSource(struct PlanningNode *planningPool, char nodeSource)
{ //line: 1
	// register: $a2
	register int i;
	// register: $v1
	register int numNodesOfSource;
} //line: 8


// path: C:\kain2\game\PLAN\PLANPOOL.C
// address: 0x80099374
// size: 0x1C
// line start: 213
// line end:   223
struct PlanningNode *PLANPOOL_GetNodeWithID(struct PlanningNode *planningPool, int type, int id, long suID)
{ //line: 2
	// register: $v1
	register int i;
} //line: 11


// path: C:\kain2\game\PLAN\PLANPOOL.C
// address: 0x800993E0
// size: 0x1C
// line start: 230
// line end:   247
struct PlanningNode *PLANPOOL_GetNodeByPosition(struct _Position *currentPos, struct PlanningNode *planningPool)
{ //line: 1
	// register: $s1
	register int i;
	// register: $v0
	register unsigned long zDiff;
	// register: $s4
	register unsigned long minZDiff;
	// register: $s2
	// size: 0x1C
	register struct PlanningNode *returnNode;
} //line: 17


// path: C:\kain2\game\PLAN\PLANPOOL.C
// address: 0x800994B4
// size: 0x1C
// line start: 251
// line end:   270
struct PlanningNode *PLANPOOL_GetClosestNode(struct _Position *pos, struct PlanningNode *planningPool, char distanceType)
{ //line: 1
	// register: $s2
	register int i;
	// register: $v1
	register long dist;
	// register: $s4
	register long minDist;
	// register: $s3
	// size: 0x1C
	register struct PlanningNode *closestNode;
} //line: 19


// path: C:\kain2\game\PLAN\PLANPOOL.C
// address: 0x800995B4
// line start: 277
// line end:   345
int PLANPOOL_AppropriatePair(struct PlanningNode *node1, struct PlanningNode *node2)
{ //line: 1
	// register: $v0
	// size: 0x1C
	register struct PlanningNode *temp;
	// register: $s0
	register int res;
	// register: $a0
	register int src;
	// register: $v1
	register int plc;
	// register: $a0
	register short type1;
	// register: $v1
	register short type2;
	// register: $t5
	register short placement1;
	// register: $t4
	register short placement2;
	// register: $t2
	register short source1;
	// register: $t3
	register short source2;
} //line: 68


// path: C:\kain2\game\PLAN\PLANPOOL.C
// address: 0x80099728
// line start: 350
// line end:   360
int PLANPOOL_AreTwoNodesConnected(struct PlanningNode *node1, struct PlanningNode *node2, struct PlanningNode *planningPool)
{ //line: 1
	// register: $a0
	register long node2Mask;
} //line: 11


// path: C:\kain2\game\PLAN\PLANPOOL.C
// address: 0x80099798
// line start: 365
// line end:   387
void PLANPOOL_MarkTwoNodesAsConnected(struct PlanningNode *node1, struct PlanningNode *node2, struct PlanningNode *planningPool)
{ //line: 1
	// register: $s2
	register int node1Index;
	// register: $s0
	register int node2Index;
} //line: 12


// path: C:\kain2\game\PLAN\PLANPOOL.C
// address: 0x80099894
// line start: 393
// line end:   405
void PLANPOOL_MarkTwoNodesAsNotConnected(struct PlanningNode *node1, struct PlanningNode *node2, struct PlanningNode *planningPool)
{ //line: 2
} //line: 3


// path: C:\kain2\game\PLAN\PLANPOOL.C
// address: 0x800998F0
// size: 0x1C
// line start: 415
// line end:   443
struct PlanningNode *PLANPOOL_GetClosestUnexploredValidNeighbor(struct PlanningNode *startNode, struct PlanningNode *planningPool)
{ //line: 1
	// register: $s3
	register int i;
	// register: $s2
	register unsigned long connectionStatus;
	// register: $v1
	register unsigned long dist;
	// register: $s5
	register unsigned long minDist;
	// register: $s4
	// size: 0x1C
	register struct PlanningNode *returnNode;
} //line: 29


// path: C:\kain2\game\PLAN\PLANPOOL.C
// address: 0x80099A00
// line start: 451
// line end:   468
void PLANPOOL_ChangeNodePosition(struct _Position *newPos, struct PlanningNode *nodeToChange, struct PlanningNode *planningPool)
{ //line: 1
	// register: $a0
	register int i;
	// register: $v1
	register unsigned char nodeToChangeMask;
} //line: 18


// path: C:\kain2\game\PLAN\PLANPOOL.C
// address: 0x80099AB8
// size: 0x1C
// line start: 472
// line end:   499
struct PlanningNode *PLANPOOL_AddNodeToPool(struct _Position *pos, struct PlanningNode *planningPool, short nodeType, short nodeID, long streamUnitID)
{ //line: 1
	// register: $t0
	// size: 0x1C
	register struct PlanningNode *nextAvailableNode;
	// register: $a1
	register int i;
	{ //line: 7
		// register: $v1
		register short _x1;
		// register: $a0
		register short _y1;
		// register: $a1
		register short _z1;
	} //line: 7
} //line: 27


// path: C:\kain2\game\PLAN\PLANPOOL.C
// address: 0x80099BAC
// line start: 510
// line end:   555
void PLANPOOL_DeleteNodeFromPool(struct PlanningNode *nodeToDelete, struct PlanningNode *planningPool)
{ //line: 2
	// register: $t3
	register int indexOfNodeToDelete;
	// register: $t2
	register int lastNodeIndex;
	// register: $v1
	register long oldConnectionMask;
	// register: $a3
	register long newConnectionMask;
	// register: $a2
	register int i;
	// register: $v0
	// size: 0x1C
	register struct PlanningNode *lastNode;
} //line: 46


// path: C:\kain2\game\PLAN\PLANSRCH.C
// address: 0x80099D78
// size: 0x1C
// line start: 78
// line end:   102
struct PlanningNode *PLANSRCH_FindNodeToExpand(struct PlanningNode *planningPool, struct PlanningNode *goalNode, int validNodeTypes)
{ //line: 1
	// register: $s2
	register int i;
	// register: $s3
	// size: 0x1C
	register struct PlanningNode *nodeToExpand;
	// register: $s4
	register unsigned long bestValueSoFar;
	// register: $v1
	register unsigned long valueForCurrentNode;
	// register: $s1
	// size: 0x1C
	register struct PlanningNode *currentNode;
} //line: 24


// path: C:\kain2\game\PLAN\PLANSRCH.C
// address: 0x80099E90
// line start: 109
// line end:   144
void PLANSRCH_ExpandNode(struct PlanningNode *planningPool, struct PlanningNode *nodeToExpand)
{ //line: 1
	// register: $t0
	register int i;
	// register: $t2
	register int connectionStatus;
	// register: $t1
	register int connections;
	// register: $t3
	register int nodeToExpandIndex;
	{ //line: 15
		// register: $a0
		register long newCost;
	} //line: 30
} //line: 36


// path: C:\kain2\game\PLAN\PLANSRCH.C
// address: 0x80099FB4
// line start: 149
// line end:   158
void PLANSRCH_InitNodesForSearch(struct PlanningNode *planningPool)
{ //line: 2
	// register: $v1
	register int i;
} //line: 10


// path: C:\kain2\game\PLAN\PLANSRCH.C
// address: 0x8009A000
// size: 0x1C
// line start: 170
// line end:   203
struct PlanningNode *PLANSRCH_FindPathInGraph(struct PlanningNode *planningPool, struct PlanningNode *startNode, struct PlanningNode *goalNode, int validNodeTypes)
{ //line: 1
	// register: $v0
	// size: 0x1C
	register struct PlanningNode *nodeToExpand;
} //line: 34


// path: C:\kain2\game\RAZIEL\CONTROL.C
// address: 0x8009A138
// line start: 27
// line end:   31
void SetPhysics(struct _Instance *instance, short gravity, long x, long y, long z);

// path: C:\kain2\game\RAZIEL\CONTROL.C
// address: 0x8009A1AC
// line start: 36
// line end:   51
void ResetPhysics(struct _Instance *instance, short gravity)
{ //line: 1
	// register: $s1
	register int i;
} //line: 9


// path: C:\kain2\game\RAZIEL\CONTROL.C
// address: 0x8009A268
// line start: 55
// line end:   60
void SetDampingPhysics(struct _Instance *instance, int damping);

// path: C:\kain2\game\RAZIEL\CONTROL.C
// address: 0x8009A310
// line start: 67
// line end:   90
void SetImpulsePhysics(struct _Instance *instance, struct __Player *player)
{ //line: 1
	{ //line: 3
		// register: $a3
		register int Dot;
		// register: $a2
		register int vLength;
	} //line: 20
} //line: 24


// path: C:\kain2\game\RAZIEL\CONTROL.C
// address: 0x8009A440
// line start: 110
// line end:   114
void SetDropPhysics(struct _Instance *instance, struct __Player *player);

// path: C:\kain2\game\RAZIEL\CONTROL.C
// address: 0x8009A478
// line start: 147
// line end:   157
void InitExternalForces(struct __Force *Forces, int MaxForces)
{ //line: 2
	// register: $a1
	register int i;
} //line: 10


// path: C:\kain2\game\RAZIEL\CONTROL.C
// address: 0x8009A4B8
// line start: 162
// line end:   168
void SetExternalForce(struct __Force *In, short x, short y, short z, int Space, int Friction);

// path: C:\kain2\game\RAZIEL\CONTROL.C
// address: 0x8009A4F8
// line start: 173
// line end:   180
void SetExternalTransitionForce(struct __Force *in, struct _Instance *instance, int time, int x, int y, int z);

// path: C:\kain2\game\RAZIEL\CONTROL.C
// address: 0x8009A560
// line start: 188
// line end:   288
void ProcessPhysics(struct __Player *player, struct __CharacterState *In, int CurrentSection, int Mode)
{ //line: 1
	// register: $s1
	register int time;
	{ //line: 45
	} //line: 56
	{ //line: 58
	} //line: 70
} //line: 101


// path: C:\kain2\game\RAZIEL\CONTROL.C
// address: 0x8009A758
// line start: 347
// line end:   398
void ApplyExternalLocalForces(struct __Player *player, struct _Instance *instance, struct __Force *Forces, int MaxForces, struct _Vector *Out)
{ //line: 1
	// register: $t1
	register int i;
	{ //line: 38
		// register: $a1
		register int friction;
	} //line: 47
} //line: 50


// path: C:\kain2\game\RAZIEL\PUPPET.C
// address: 0x8009A90C
// line start: 8
// line end:   98
void StateHandlerPuppetShow(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $s0
	// size: 0x8
	register struct __Event *Ptr;
	{ //line: 30
		// register: $v0
		// size: 0x18
		register struct evActionPlayHostAnimationData *data;
	} //line: 32
	{ //line: 45
		// register: $v1
		// size: 0x14
		register struct evAnimationInstanceSwitchData *data;
	} //line: 47
	{ //line: 59
		// register: $v0
		// size: 0x8
		register struct evPositionData *data;
	} //line: 59
	{ //line: 70
		// register: $v0
		// size: 0x8
		register struct evPositionData *data;
		{ //line: 70
			// register: $v1
			register short _x1;
			// register: $a0
			register short _y1;
			// register: $a1
			register short _z1;
			// register: $v0
			// size: 0x6
			register struct _Position *_v0;
		} //line: 70
	} //line: 70
} //line: 90


// path: C:\kain2\game\RAZIEL\PUPPET.C
// address: 0x8009ACA8
// line start: 102
// line end:   173
void StateHandlerMoveToPosition(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $s0
	// size: 0x8
	register struct __Event *Ptr;
	// register: $s0
	register long distance;
	// register: $v0
	register int motion;
	// register: $s5
	register int applyMotion;
	{ //line: 11
		// register: $v0
		// size: 0x8
		register struct evPositionData *moveToPoint;
		{ //line: 13
			// register: $v1
			register short _x1;
			// register: $a0
			register short _y1;
			// register: $v0
			register short _z1;
			// register: $s3
			// size: 0x6
			register struct _Position *_v0;
		} //line: 13
	} //line: 13
	{ //line: 32
		// register: $v0
		// size: 0x8
		register struct evPositionData *moveToPoint;
		{ //line: 34
			// register: $v1
			register short _x1;
			// register: $a0
			register short _y1;
			// register: $v0
			register short _z1;
		} //line: 34
	} //line: 38
} //line: 72


// path: C:\kain2\game\RAZIEL\PUPPET.C
// address: 0x8009AF50
// line start: 174
// line end:   239
void DefaultPuppetStateHandler(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $a1
	// size: 0x8
	register struct __Event *Ptr;
	{ //line: 20
		// register: $v0
		// size: 0x8
		register struct evPositionData *data;
	} //line: 20
	{ //line: 27
		// register: $v1
		// size: 0x8
		register struct evPositionData *data;
		{ //line: 29
			// register: $a0
			register long _x1;
			// register: $a2
			register long _y1;
			// register: $a1
			register long _z1;
			// register: $v0
			// size: 0xC
			register struct _Vector *_v0;
		} //line: 29
	} //line: 34
} //line: 66


// path: C:\kain2\game\RAZIEL\PUPPET.C
// address: 0x8009B204
// line start: 243
// line end:   340
void StateHandlerWarpGate(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $v0
	// size: 0x8
	register struct __Event *Ptr;
	// register: $s3
	register int anim;
	{ //line: 43
		// register: $v0
		// size: 0x29C
		register struct _Instance *heldInst;
	} //line: 50
	{ //line: 56
	} //line: 56
} //line: 98


// path: C:\kain2\game\RAZIEL\PUPPET.C
// address: 0x8009B68C
// line start: 345
// line end:   431
void StateHandlerForcedGlide(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $s0
	// size: 0x8
	register struct __Event *Ptr;
	// register: $s6
	register int Anim;
	// register: $s4
	register int extraProcess;
	{ //line: 77
	} //line: 83
} //line: 87


// path: C:\kain2\game\RAZIEL\ATTACK.C
// address: 0x8009BA7C
// line start: 16
// line end:   146
int StateHandlerDecodeHold(int *Message, int *Data)
{ //line: 1
	// register: $s4
	register int rc;
	// register: $s0
	register int WhoAmI;
	// register: $s1
	// size: 0x29C
	register struct _Instance *heldInstance;
	{ //line: 37
		// register: $v1
		register int hitState;
	} //line: 74
	{ //line: 101
		// register: $v1
		// size: 0x29C
		register struct _Instance *heldWeapon;
	} //line: 121
} //line: 131


// path: C:\kain2\game\RAZIEL\ATTACK.C
// address: 0x8009BDE4
// line start: 157
// line end:   647
void StateHandlerAttack2(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $s1
	// size: 0x8
	register struct __Event *Ptr;
	// address: -64
	auto int message;
	// address: -60
	auto int messageData;
	// address: -56
	auto int Frame;
	// address: -52
	auto int Anim;
	// address: -48
	auto int ignoreHit;
	// address: -44
	// size: 0x29C
	auto struct _Instance *ignoreInst;
	{ //line: 76
		// register: $s0
		// size: 0x29C
		register struct _Instance *inst;
		// register: $s3
		register unsigned long startColor;
		// register: $s2
		register unsigned long endColor;
	} //line: 122
	{ //line: 175
		// register: $s0
		// size: 0x29C
		register struct _Instance *weaponInst;
	} //line: 192
	{ //line: 249
		// register: $s0
		// size: 0x29C
		register struct _Instance *inst;
		// register: $s2
		register unsigned long startColor;
		// register: $s1
		register unsigned long endColor;
	} //line: 281
	{ //line: 329
		// register: $a0
		// size: 0x29C
		register struct _Instance *Inst;
	} //line: 343
	{ //line: 361
	} //line: 362
	{ //line: 387
	} //line: 394
	{ //line: 417
		// register: $s0
		// size: 0x10
		register struct evMonsterHitData *data;
	} //line: 431
	{ //line: 451
		// register: $s0
		// size: 0x10
		register struct evMonsterHitData *hitData;
	} //line: 457
	{ //line: 477
		// register: $v1
		// size: 0x30
		register struct _G2AnimSection_Type *animSection;
	} //line: 484
} //line: 491


// path: C:\kain2\game\RAZIEL\ATTACK.C
// address: 0x8009CEB4
// line start: 651
// line end:   766
void StateHandlerCannedReaction(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $v0
	// size: 0x8
	register struct __Event *Ptr;
	{ //line: 43
		// register: $v0
		// size: 0x29C
		register struct _Instance *Inst;
	} //line: 46
	{ //line: 56
		// register: $s0
		// size: 0x29C
		register struct _Instance *Inst;
	} //line: 73
} //line: 116


// path: C:\kain2\game\RAZIEL\ATTACK.C
// address: 0x8009D144
// line start: 770
// line end:   870
void StateHandlerStumble(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $s0
	// size: 0x8
	register struct __Event *Ptr;
	{ //line: 51
		// register: $v0
		// size: 0x18
		register struct evActionPlayHostAnimationData *data;
	} //line: 53
	{ //line: 61
		// register: $v0
		// size: 0x10
		register struct evMonsterHitData *data;
	} //line: 62
} //line: 101


// path: C:\kain2\game\RAZIEL\ATTACK.C
// address: 0x8009D568
// line start: 892
// line end:   968
void StateHandlerHitReaction(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $v0
	// size: 0x8
	register struct __Event *Ptr;
	{ //line: 10
		// register: $s0
		// size: 0x10
		register struct evMonsterHitData *data;
		{ //line: 14
			// register: $v0
			// size: 0x29C
			register struct _Instance *weapon;
		} //line: 35
	} //line: 42
} //line: 77


// path: C:\kain2\game\RAZIEL\ATTACK.C
// address: 0x8009D7E4
// line start: 1049
// line end:   1391
void StateHandlerThrow2(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $a1
	// size: 0x8
	register struct __Event *Ptr;
	// register: $s4
	register int Anim;
	{ //line: 144
		// register: $v1
		// size: 0x10
		register struct evMonsterHitData *data;
	} //line: 147
	{ //line: 220
		// register: $s0
		// size: 0x29C
		register struct _Instance *weaponInst;
		{ //line: 224
			// register: $a3
			register int spin_type;
			{ //line: 238
				// address: -64
				// size: 0x6
				auto struct _Position To;
				// address: -56
				// size: 0x8
				auto struct _Rotation Rot;
				// register: $v0
				// size: 0x20
				register struct MATRIX *matrix;
			} //line: 243
		} //line: 259
	} //line: 263
	{ //line: 310
		// address: -64
		// size: 0x6
		auto struct _Position To;
		// address: -48
		// size: 0x8
		auto struct _Rotation Rot;
		// register: $v0
		// size: 0x20
		register struct MATRIX *matrix;
	} //line: 320
} //line: 343


// path: C:\kain2\game\RAZIEL\ATTACK.C
// address: 0x8009E2F0
// line start: 1393
// line end:   1429
void PointAt(struct _Instance *instance, struct _Position *Target, struct _Rotation *Rot1)
{ //line: 1
	// address: -48
	// size: 0x6
	auto struct _Position From;
	// address: -40
	// size: 0x8
	auto struct SVECTOR v1;
	// address: -32
	// size: 0x10
	auto struct VECTOR v3;
	// register: $a0
	// size: 0x20
	register struct MATRIX *tempMat;
} //line: 33


// path: C:\kain2\game\RAZIEL\ATTACK.C
// address: 0x8009E410
// line start: 1431
// line end:   1477
void ThrowSetFocusPoint(struct _Instance *instance, struct _Rotation *rot)
{ //line: 1
	// register: $s0
	// size: 0x20
	register struct MATRIX *pTempMat;
	// register: $s1
	// size: 0x29C
	register struct _Instance *weaponInstance;
	// address: -48
	// size: 0x8
	auto struct SVECTOR v1;
	// address: -40
	// size: 0x10
	auto struct VECTOR v4;
} //line: 47


// path: C:\kain2\game\RAZIEL\ATTACK.C
// address: 0x8009E550
// line start: 1479
// line end:   1528
void LimitRotation(struct _Rotation *rot);

// path: C:\kain2\game\RAZIEL\ATTACK.C
// address: 0x8009E6C4
// line start: 1539
// line end:   1818
void StateHandlerGrab(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $s2
	// size: 0x8
	register struct __Event *Ptr;
	// register: $s4
	register int Anim;
	{ //line: 34
		// register: $a1
		// size: 0x29C
		register struct _Instance *Inst;
	} //line: 45
	{ //line: 71
		// register: $s0
		// size: 0x29C
		register struct _Instance *Inst;
	} //line: 89
	{ //line: 176
		// register: $s0
		// size: 0x29C
		register struct _Instance *Inst;
	} //line: 208
	{ //line: 215
		// register: $s0
		// size: 0x29C
		register struct _Instance *Inst;
	} //line: 227
	{ //line: 234
		// register: $s0
		// size: 0x29C
		register struct _Instance *Inst;
	} //line: 248
} //line: 280


// path: C:\kain2\game\RAZIEL\SWIM.C
// address: 0x8009EE2C
// line start: 5
// line end:   26
void StateHandlerInitSwim(struct __CharacterState *In, int CurrentSection, int Data);

// path: C:\kain2\game\RAZIEL\SWIM.C
// address: 0x8009EF38
// line start: 54
// line end:   272
void StateHandlerSwimCoil(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $s0
	// size: 0x8
	register struct __Event *Ptr;
	// register: $a0
	// size: 0x18
	register struct evPhysicsSwimData *SwimData;
	// register: $s6
	register int rc;
	// register: $s5
	register int release;
	{ //line: 183
		// register: $v1
		// size: 0x10
		register struct evMonsterHitData *data;
	} //line: 186
} //line: 219


// path: C:\kain2\game\RAZIEL\SWIM.C
// address: 0x8009F6B8
// line start: 277
// line end:   492
void StateHandlerSwimTread(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $a0
	// size: 0x8
	register struct __Event *Ptr;
	// register: $s0
	// size: 0x18
	register struct evPhysicsSwimData *SwimData;
	{ //line: 145
		// register: $s0
		// size: 0x24
		register struct evPhysicsEdgeData *data;
	} //line: 151
	{ //line: 171
		// register: $v1
		// size: 0x10
		register struct evMonsterHitData *data;
	} //line: 176
} //line: 216


// path: C:\kain2\game\RAZIEL\SWIM.C
// address: 0x8009FE80
// line start: 497
// line end:   613
void StateHandlerSwimDiveIn(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $a0
	// size: 0x8
	register struct __Event *Ptr;
	// register: $s3
	// size: 0x18
	register struct evPhysicsSwimData *SwimData;
	{ //line: 84
		// register: $v1
		// size: 0x10
		register struct evMonsterHitData *data;
	} //line: 89
} //line: 117


// path: C:\kain2\game\RAZIEL\SWIM.C
// address: 0x800A020C
// line start: 621
// line end:   961
void StateHandlerSwim(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $a0
	// size: 0x8
	register struct __Event *Ptr;
	// register: $s2
	// size: 0x18
	register struct evPhysicsSwimData *SwimData;
	// register: $fp
	register int tap;
	{ //line: 183
		// register: $s0
		// size: 0x29C
		register struct _Instance *Inst;
	} //line: 195
	{ //line: 327
		// register: $v1
		// size: 0x10
		register struct evMonsterHitData *data;
	} //line: 330
} //line: 341


// path: C:\kain2\game\RAZIEL\SPIDER.C
// address: 0x800A0D88
// line start: 12
// line end:   93
void StateHandlerWallGrab(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $a1
	// size: 0x8
	register struct __Event *Ptr;
	{ //line: 29
		// register: $a1
		// size: 0x2C
		register struct evPhysicsWallCrawlData *Data;
	} //line: 69
} //line: 82


// path: C:\kain2\game\RAZIEL\SPIDER.C
// address: 0x800A0FF8
// line start: 96
// line end:   419
void StateHandlerWallIdle(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $s3
	register int Anim;
	// register: $s0
	// size: 0x8
	register struct __Event *Ptr;
	// address: -56
	// size: 0x8
	auto struct SVECTOR startVec;
	// address: -48
	// size: 0x8
	auto struct SVECTOR endVec;
	// register: $s6
	register int extraProcess;
	{ //line: 44
		// register: $v1
		// size: 0x10
		register struct evMonsterHitData *data;
	} //line: 49
	{ //line: 95
		// register: $a1
		// size: 0x2C
		register struct evPhysicsWallCrawlData *Data;
	} //line: 143
	{ //line: 157
		// register: $v1
		register int rc;
	} //line: 163
	{ //line: 192
		// register: $v1
		register int rc;
	} //line: 198
	{ //line: 275
		// register: $v1
		register int rc;
	} //line: 320
} //line: 324


// path: C:\kain2\game\RAZIEL\SPIDER.C
// address: 0x800A18E0
// line start: 423
// line end:   504
void StateHandlerWallDismount(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $v0
	// size: 0x8
	register struct __Event *Ptr;
	// register: $s4
	register int Frame;
} //line: 82


// path: C:\kain2\game\RAZIEL\ALGOCTRL.C
// address: 0x800A1B54
// line start: 24
// line end:   90
void InitAlgorithmicWings(struct _Instance *instance)
{ //line: 1
	// address: -24
	// size: 0x8
	auto struct _G2EulerAngles_Type Rot;
	// register: $s1
	register unsigned char i;
} //line: 67


// path: C:\kain2\game\RAZIEL\ALGOCTRL.C
// address: 0x800A1CEC
// line start: 94
// line end:   113
void DeInitAlgorithmicWings(struct _Instance *instance)
{ //line: 1
	// register: $s0
	register unsigned char i;
} //line: 20


// path: C:\kain2\game\RAZIEL\ALGOCTRL.C
// address: 0x800A1D7C
// line start: 117
// line end:   142
void AlgorithmicWings(struct _Instance *instance, struct evAnimationControllerDoneData *ControllerData)
{ //line: 1
	// address: -24
	// size: 0x8
	auto struct _G2EulerAngles_Type Rot;
} //line: 26


// path: C:\kain2\game\RAZIEL\ALGOCTRL.C
// address: 0x800A1DEC
// line start: 150
// line end:   255
void AlgorithmicNeck(struct _Instance *Player, struct _Instance *Target)
{ //line: 1
	// address: -96
	// size: 0x6
	auto struct _Position From;
	// address: -88
	// size: 0x6
	auto struct _Position To;
	// address: -80
	// size: 0x8
	auto struct _Rotation Rot1;
	// register: $v1
	register int Diff;
	// address: -72
	// size: 0x20
	auto struct MATRIX matrix;
	{ //line: 12
		// address: -40
		// size: 0x18
		auto struct evCollideInstanceStatsData data;
	} //line: 33
	{ //line: 58
		// register: $v1
		// size: 0x20
		register struct MATRIX *matrix;
	} //line: 73
} //line: 106


// path: C:\kain2\game\RAZIEL\STEERING.C
// address: 0x800A206C
// line start: 20
// line end:   32
int UpdateZoneDelta(int rc, int LastRC);

// path: C:\kain2\game\RAZIEL\STEERING.C
// address: 0x800A20A8
// line start: 34
// line end:   113
int GetControllerInput(int *ZDirection, long *controlCommand)
{ //line: 1
	// register: $s0
	register int rc;
} //line: 80


// path: C:\kain2\game\RAZIEL\STEERING.C
// address: 0x800A2288
// line start: 117
// line end:   142
int DecodeDirection(int Source, int Destination, short *Difference, short *Zone)
{ //line: 1
	// register: $s1
	register int rc;
} //line: 25


// path: C:\kain2\game\RAZIEL\STEERING.C
// address: 0x800A2378
// line start: 188
// line end:   482
int ProcessMovement(struct _Instance *instance, long *controlCommand, struct GameTracker *GT)
{ //line: 1
	// address: -32
	auto int ZDirection;
	// register: $s2
	register int rc;
	// register: $v1
	register int lag;
	{ //line: 79
		// address: -28
		auto short diff;
		// address: -26
		auto short zone;
	} //line: 80
	{ //line: 129
		// address: -40
		// size: 0x6
		auto struct _G2SVector3_Type rot;
	} //line: 149
	{ //line: 151
	} //line: 168
	{ //line: 177
		// register: $s0
		register short angle;
	} //line: 188
	{ //line: 203
	} //line: 207
	{ //line: 238
		// address: -24
		auto short diff;
		// address: -22
		auto short zone;
	} //line: 239
	{ //line: 243
	} //line: 253
	{ //line: 255
	} //line: 266
	{ //line: 268
		// register: $s0
		register short angle;
	} //line: 279
} //line: 294


// path: C:\kain2\game\RAZIEL\STEERING.C
// address: 0x800A28BC
// line start: 487
// line end:   510
void SteerTurn(struct _Instance *instance, int rc)
{ //line: 1
	{ //line: 7
		// register: $v0
		register int rot;
	} //line: 19
} //line: 24


// path: C:\kain2\game\RAZIEL\STEERING.C
// address: 0x800A2950
// line start: 550
// line end:   576
void SteerMove(struct _Instance *instance, int rc);

// path: C:\kain2\game\RAZIEL\STEERING.C
// address: 0x800A2A00
// line start: 589
// line end:   676
int SteerAutoFace(struct _Instance *instance, long *controlCommand)
{ //line: 1
	// register: $s0
	register short angle;
	// register: $s2
	register int rc;
	// register: $a1
	// size: 0x29C
	register struct _Instance *target;
	// address: -32
	// size: 0x6
	auto struct _G2SVector3_Type autoFaceRot;
	// register: $s1
	register int diff;
	// register: $s2
	register int predict;
} //line: 86


// path: C:\kain2\game\RAZIEL\STEERING.C
// address: 0x800A2C94
// line start: 681
// line end:   733
void SteerSwim(struct _Instance *instance)
{ //line: 1
	// register: $a0
	register int step;
	// register: $a1
	register int velocity;
} //line: 50


// path: C:\kain2\game\RAZIEL\STEERING.C
// address: 0x800A2D84
// line start: 735
// line end:   741
void SteerWallcrawling(struct _Instance *instance);

// path: C:\kain2\game\RAZIEL\STEERING.C
// address: 0x800A2DB8
// line start: 748
// line end:   757
void SteerDisableAutoFace(struct _Instance *instance);

// path: C:\kain2\game\RAZIEL\STEERING.C
// address: 0x800A2E4C
// line start: 776
// line end:   883
void SteerSwitchMode(struct _Instance *instance, int mode)
{ //line: 1
	{ //line: 70
		// register: $v1
		register int rotx;
	} //line: 80
} //line: 107


// path: C:\kain2\game\RAZIEL\STEERING.C
// address: 0x800A30A8
// line start: 888
// line end:   917
void razInitWallCrawlSteering(struct _Instance *instance)
{ //line: 1
	// address: -24
	// size: 0x6
	auto struct _G2SVector3_Type vec;
} //line: 1


// path: C:\kain2\game\RAZIEL\STEERING.C
// address: 0x800A31E4
// line start: 922
// line end:   941
void razDeinitWallCrawlSteering(struct _Instance *instance);

// path: C:\kain2\game\RAZIEL\RAZDBUG.C
// address: 0x800A32BC
// line start: 240
// line end:   400
void DebugProcess(struct _Instance *instance, struct __Player *player);

// path: C:\kain2\game\RAZIEL\SENSES.C
// address: 0x800A32C4
// line start: 4
// line end:   24
int SetEngagedInstance(struct __Player *player, struct evCollideInstanceStatsData *Ptr, unsigned long Attribute)
{ //line: 1
	// register: $t1
	register int rc;
} //line: 19


// path: C:\kain2\game\RAZIEL\SENSES.C
// address: 0x800A335C
// line start: 29
// line end:   586
void UpdateEngagementList(struct evCollideInstanceStatsData *Ptr, struct __Player *player)
{ //line: 1
	// register: $s5
	register int WhoAmI;
	// register: $s1
	register int Ability;
	// register: $s6
	register int hidden;
	// register: $s1
	// size: 0x14
	register struct PhysObCollectibleProperties *collectibleProp;
	{ //line: 15
		// register: $a2
		// size: 0xA
		register struct evObjectDraftData *draft;
		{ //line: 21
			// register: $v0
			register int force;
		} //line: 32
	} //line: 34
	{ //line: 64
		// address: -64
		// size: 0x8
		auto struct SVECTOR startVec;
		// address: -56
		// size: 0x8
		auto struct SVECTOR endVec;
	} //line: 87
	{ //line: 91
		// register: $a1
		// size: 0x20
		register struct PhysObSwitchProperties *prop;
		{ //line: 94
		} //line: 112
	} //line: 113
	{ //line: 115
		// register: $s1
		// size: 0x2C
		register struct PhysObInteractProperties *interactProp;
		// register: $s2
		register int testConditions;
		{ //line: 137
			// register: $s0
			// size: 0x29C
			register struct _Instance *heldInst;
		} //line: 163
	} //line: 201
	{ //line: 206
		// register: $v1
		// size: 0x20
		register struct MATRIX *collectMat;
		// address: -64
		// size: 0x6
		auto struct _Position headPos;
		// address: -56
		// size: 0x6
		auto struct _Position collectPos;
		// register: $s0
		register long headDistance;
		// register: $a1
		register long rootDistance;
		{ //line: 257
			// register: $v1
			register unsigned long ability_bit;
		} //line: 265
	} //line: 293
	{ //line: 455
		// register: $a0
		// size: 0x29C
		register struct _Instance *Inst;
	} //line: 461
	{ //line: 468
		// address: -64
		// size: 0x10
		auto struct VECTOR in;
		// address: -48
		// size: 0x10
		auto struct VECTOR out;
		{ //line: 477
			// register: $v0
			register int rc;
		} //line: 509
	} //line: 510
} //line: 558


// path: C:\kain2\game\RAZIEL\SENSES.C
// address: 0x800A4460
// line start: 592
// line end:   604
long UpdateAutoFaceAngle(struct _Instance *player, struct _Instance *instance)
{ //line: 1
	// register: $v0
	register long rc;
} //line: 1


// path: C:\kain2\game\RAZIEL\HEALTH.C
// address: 0x800A44A0
// line start: 4
// line end:   25
void InitHealthSystem();

// path: C:\kain2\game\RAZIEL\HEALTH.C
// address: 0x800A4540
// line start: 32
// line end:   41
void GainHealth(int data);

// path: C:\kain2\game\RAZIEL\HEALTH.C
// address: 0x800A45CC
// line start: 47
// line end:   66
void LoseHealth(int amount);

// path: C:\kain2\game\RAZIEL\HEALTH.C
// address: 0x800A46C8
// line start: 72
// line end:   92
void DrainHealth(int amount);

// path: C:\kain2\game\RAZIEL\HEALTH.C
// address: 0x800A47E4
// line start: 97
// line end:   107
void BumpUpHealth();

// path: C:\kain2\game\RAZIEL\HEALTH.C
// address: 0x800A4810
// line start: 113
// line end:   117
int GetMaxHealth();

// path: C:\kain2\game\RAZIEL\HEALTH.C
// address: 0x800A4860
// line start: 130
// line end:   286
void ProcessHealth(struct _Instance *instance)
{ //line: 1
	{ //line: 96
	} //line: 109
} //line: 157


// path: C:\kain2\game\RAZIEL\HEALTH.C
// address: 0x800A4D0C
// line start: 290
// line end:   306
int HealthCheckForLowHealth();

// path: C:\kain2\game\RAZIEL\HEALTH.C
// address: 0x800A4D78
// line start: 312
// line end:   317
void DrainMana(int amount);

// path: C:\kain2\game\RAZIEL\HEALTH.C
// address: 0x800A4DBC
// line start: 322
// line end:   326
void SetMana(int amount);

// path: C:\kain2\game\RAZIEL\HEALTH.C
// address: 0x800A4DE4
// line start: 331
// line end:   341
void HealthInstantDeath(struct _Instance *instance);

// path: C:\kain2\game\RAZIEL\HEALTH.C
// address: 0x800A4E64
// line start: 350
// line end:   355
void RAZIEL_DebugHealthSetScale(long healthScale);

// path: C:\kain2\game\RAZIEL\HEALTH.C
// address: 0x800A4EBC
// line start: 361
// line end:   364
void RAZIEL_DebugManaSetMax(long manaScale);

// path: C:\kain2\game\RAZIEL\HEALTH.C
// address: 0x800A4ED0
// line start: 370
// line end:   376
void RAZIEL_DebugHealthFillUp();

// path: C:\kain2\game\RAZIEL\HEALTH.C
// address: 0x800A4F30
// line start: 382
// line end:   383
void RAZIEL_DebugManaFillUp();

// path: C:\kain2\game\RAZIEL\HEALTH.C
// address: 0x800A4F50
// line start: 390
// line end:   400
void razPlayUnderworldSounds(struct _Instance *instance);

// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A505C
// line start: 12
// line end:   40
void razAlignYMoveRot(struct _Instance *dest, short distance, struct _Position *position, struct _Rotation *rotation, int extraZ)
{ //line: 1
	// address: -72
	// size: 0x8
	auto struct SVECTOR d;
	// address: -64
	// size: 0x8
	auto struct SVECTOR dd;
	// address: -56
	// size: 0x20
	auto struct MATRIX mat;
} //line: 1


// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A5120
// line start: 54
// line end:   74
void razAlignYRotMove(struct _Instance *dest, short distance, struct _Position *position, struct _Rotation *rotation, int extraZ)
{ //line: 1
	// address: -88
	// size: 0x8
	auto struct SVECTOR d;
	// address: -80
	// size: 0x8
	auto struct SVECTOR dd;
	// address: -72
	// size: 0x8
	auto struct SVECTOR rot;
	// address: -64
	// size: 0x20
	auto struct MATRIX mat;
} //line: 1


// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A51F8
// line start: 86
// line end:   109
void razAlignYRotMoveInterp(struct _Instance *source, struct _Instance *dest, short distance, unsigned char segNumber, int Frames, int extraZ)
{ //line: 1
	// address: -40
	// size: 0x6
	auto struct _Position position;
	// address: -32
	// size: 0x8
	auto struct _Rotation rotation;
	// address: -24
	// size: 0x6
	auto struct _G2SVector3_Type v;
} //line: 1


// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A52EC
// line start: 114
// line end:   125
void razAlignYRotInterp(struct _Instance *source, struct _Position *dest, unsigned char segNumber, int Frames);

// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A5318
// line start: 195
// line end:   242
int razConstrictAngle(struct _Instance *instance)
{ //line: 1
	// register: $s0
	register int i;
	// register: $s2
	register int j;
	// register: $a2
	register int k;
	// register: $s1
	register int Total;
	// address: -32
	auto int outer;
	// register: $s3
	register int rc;
	// address: -40
	// size: 0x8
	auto struct _SVector point;
} //line: 48


// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A542C
// line start: 247
// line end:   280
void razRotateUpperBody(struct _Instance *instance, struct evActionLookAroundData *data)
{ //line: 1
	// register: $s0
	register int tx;
	// register: $s2
	register int tz;
	// address: -32
	// size: 0x6
	auto struct _G2SVector3_Type Rot;
} //line: 21


// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A55FC
// line start: 285
// line end:   295
void razSetFadeEffect(short source, short dest, int steps);

// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A5620
// line start: 302
// line end:   315
int razPlaneShift(struct _Instance *instance);

// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A567C
// line start: 320
// line end:   351
void razSpectralShift()
{ //line: 1
	// register: $v0
	// size: 0x29C
	register struct _Instance *inst;
} //line: 32


// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A578C
// line start: 355
// line end:   373
void razMaterialShift();

// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A5854
// line start: 378
// line end:   399
int RAZIEL_OkToShift();

// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A58E8
// line start: 404
// line end:   458
int razPickupAndGrab(struct __CharacterState *In, int CurrentSection)
{ //line: 1
	{ //line: 4
		// register: $a2
		register int index;
		{ //line: 18
			// register: $s0
			// size: 0x10
			register struct evObjectData *objData;
		} //line: 39
	} //line: 51
} //line: 55


// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A5AF8
// line start: 466
// line end:   498
int razZeroAxis(long *x, long *y, int radius)
{ //line: 1
	// register: $v0
	register unsigned long distance;
} //line: 33


// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A5B58
// line start: 503
// line end:   549
int razAdjustSpeed(struct _Instance *instance, int minSpeed)
{ //line: 1
	// register: $s0
	register long adjustment;
	{ //line: 1
		// register: $v1
		register int data;
	} //line: 22
} //line: 29


// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A5BF4
// line start: 553
// line end:   571
void razLaunchForce(struct _Instance *instance)
{ //line: 1
} //line: 1


// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A5C84
// size: 0x29C
// line start: 577
// line end:   600
struct _Instance *razGetHeldItem()
{ //line: 2
	// register: $v0
	// size: 0x29C
	register struct _Instance *instance;
} //line: 24


// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A5CBC
// size: 0x29C
// line start: 605
// line end:   622
struct _Instance *razGetHeldWeapon()
{ //line: 1
	// register: $s0
	// size: 0x29C
	register struct _Instance *instance;
} //line: 18


// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A5D1C
// line start: 626
// line end:   629
void razReaverBladeOff();

// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A5D4C
// line start: 634
// line end:   638
void razReaverBladeOn();

// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A5D8C
// line start: 644
// line end:   657
int razReaverOff()
{ //line: 1
	{ //line: 4
		// register: $s0
		// size: 0x29C
		register struct _Instance *heldWeapon;
	} //line: 12
} //line: 14


// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A5E14
// line start: 661
// line end:   681
int razReaverOn();

// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A5EF8
// line start: 686
// line end:   708
void razReaverPickup(struct _Instance *instance, struct _Instance *soulReaver);

// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A5FB4
// line start: 714
// line end:   718
void razReaverImbue(int reaverType);

// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A5FF4
// line start: 723
// line end:   733
int razGetReaverFromMask(int reaverMask)
{ //line: 2
	// register: $v0
	register int rc;
} //line: 11


// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A6018
// line start: 737
// line end:   748
void razReaverScale(int scale)
{ //line: 1
	// register: $s0
	// size: 0x29C
	register struct _Instance *Inst;
} //line: 12


// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A6080
// line start: 752
// line end:   785
void razGetForwardNormal(struct _Instance *inst, struct _Instance *target)
{ //line: 1
	// address: -72
	// size: 0x2C
	auto struct _PCollideInfo CInfo;
	// address: -24
	// size: 0x8
	auto struct SVECTOR Old;
	// address: -16
	// size: 0x8
	auto struct SVECTOR New;
} //line: 34


// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A6164
// line start: 789
// line end:   799
void razGetRotFromNormal(struct _SVector *normal, struct _Rotation *rot)
{ //line: 1
	// address: -24
	// size: 0x6
	auto struct _Position a;
	// address: -16
	// size: 0x6
	auto struct _Position b;
} //line: 1


// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A61B0
// line start: 803
// line end:   858
void razCenterWithBlock(struct _Instance *inst, struct _Instance *target, int dist)
{ //line: 1
	// address: -88
	// size: 0x8
	auto struct SVECTOR d;
	// address: -80
	// size: 0x8
	auto struct SVECTOR dd;
	// address: -72
	// size: 0x8
	auto struct _Rotation rot;
	// address: -64
	// size: 0x20
	auto struct MATRIX mat;
	// address: -32
	// size: 0x6
	auto struct _G2SVector3_Type v;
} //line: 52


// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A6380
// line start: 862
// line end:   875
void razSetPauseTranslation(struct _Instance *instance)
{ //line: 1
	// address: -16
	// size: 0x6
	auto struct _G2SVector3_Type Vec;
} //line: 11


// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A63F4
// line start: 879
// line end:   884
void razResetPauseTranslation(struct _Instance *instance);

// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A644C
// line start: 889
// line end:   1186
void razSelectMotionAnim(struct __CharacterState *In, int CurrentSection, int Frames, int *Anim)
{ //line: 1
	// address: -48
	// size: 0x6
	auto struct _G2SVector3_Type Vec;
	// register: $s0
	register int switchType;
	// register: $s1
	register int frame;
	{ //line: 242
		// register: $s0
		register int adjustment;
	} //line: 263
	{ //line: 268
		// register: $s1
		// size: 0x30
		register struct _G2AnimSection_Type *animSectionA;
		// register: $s2
		// size: 0x30
		register struct _G2AnimSection_Type *animSectionB;
		// register: $s3
		// size: 0x14
		register struct _G2AnimKeylist_Type *keylist;
		// register: $s5
		register int keylistID;
	} //line: 290
} //line: 298


// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A6ABC
// line start: 1191
// line end:   1250
int razApplyMotion(struct __CharacterState *In, int CurrentSection)
{ //line: 1
	// address: -24
	// size: 0x6
	auto struct _G2SVector3_Type Vec;
	{ //line: 6
		// register: $s0
		// size: 0x30
		register struct _G2AnimSection_Type *animSection;
		{ //line: 46
			// register: $s0
			register long adjustment;
		} //line: 56
	} //line: 57
} //line: 59


// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A6C4C
// line start: 1254
// line end:   1259
void razResetMotion(struct _Instance *instance);

// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A6CA0
// line start: 1293
// line end:   1297
void razSetDampingPhysics(struct _Instance *instance);

// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A6CD8
// line start: 1303
// line end:   1400
void razEnterWater(struct __CharacterState *In, int CurrentSection, struct evPhysicsSwimData *SwimData)
{ //line: 1
	// register: $s2
	// size: 0x29C
	register struct _Instance *Inst;
} //line: 98


// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A6F7C
// line start: 1428
// line end:   1443
void razSetSwimVelocity(struct _Instance *instance, int vel, int accl)
{ //line: 2
} //line: 16


// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A6FD0
// line start: 1447
// line end:   1503
void razSetWallCrawlNodes(struct _Instance *instance, struct evPhysicsWallCrawlData *data)
{ //line: 1
	// address: -24
	// size: 0x6
	auto struct _G2SVector3_Type vec;
	{ //line: 8
	} //line: 46
} //line: 57


// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A70BC
// line start: 1508
// line end:   1522
int razSwitchVAnimCharacterGroup(struct _Instance *instance, int animGroup, int *frame, int *frames)
{ //line: 1
	// register: $s0
	register int i;
	// register: $v1
	register int rc;
	// address: -40
	// size: 0xC
	auto int temp[3];
} //line: 14


// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A7178
// line start: 1527
// line end:   1549
int razSwitchVAnimGroup(struct _Instance *instance, int section, int animGroup, int frame, int frames)
{ //line: 1
	// register: $t0
	register int index;
	// register: $v1
	register int rc;
} //line: 21


// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A71D4
// line start: 1553
// line end:   1565
void razSwitchVAnimCharacterSingle(struct _Instance *instance, int anim, int *frame, int *frames)
{ //line: 1
	// register: $s0
	register int i;
	// address: -40
	// size: 0xC
	auto int temp[3];
} //line: 13


// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A728C
// line start: 1569
// line end:   1575
void razSwitchVAnimSingle(struct _Instance *instance, int section, int anim, int frame, int frames)
{ //line: 1
} //line: 1


// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A72C0
// line start: 1579
// line end:   1602
void razSwitchVAnim(struct _Instance *instance, int section, struct __VAnim *vAnim, int frame, int frames);

// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A738C
// line start: 1607
// line end:   1655
int razProcessSAnim(struct _Instance *instance, int mode)
{ //line: 1
	// register: $s2
	register int rc;
	// register: $s0
	register int i;
	{ //line: 26
		// register: $s1
		// size: 0x8
		register struct __VAnim *vanim;
	} //line: 37
} //line: 48


// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A7504
// line start: 1659
// line end:   1675
void razSwitchStringAnimation(struct _Instance *instance, int anim)
{ //line: 1
	// register: $s1
	// size: 0x8
	register struct __VAnim *vanim;
	// register: $s0
	register int i;
} //line: 17


// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A75CC
// line start: 1679
// line end:   1690
int CheckStringAnimation(struct _Instance *instance, int mode)
{ //line: 1
	// register: $s0
	register int rc;
} //line: 11


// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A762C
// line start: 1744
// line end:   1753
void razLaunchBubbles(int segments, int count, int type);

// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A76C4
// line start: 1757
// line end:   1766
void razSetCowlNoDraw(int mode)
{ //line: 1
} //line: 10


// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A7774
// line start: 1769
// line end:   1776
void razAttachControllers()
{ //line: 1
	// register: $s1
	register int i;
} //line: 8


// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A77DC
// line start: 1779
// line end:   1878
void razSetPlayerEvent()
{ //line: 1
	// register: $s0
	register void (*process)();
	// address: -16
	auto int message;
	// address: -12
	auto int data;
	{ //line: 25
	} //line: 34
} //line: 100


// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A7AE8
// line start: 1881
// line end:   1886
void razClearPlayerEvent();

// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A7B20
// line start: 1890
// line end:   1891
void razSetPlayerEventHistory(unsigned long event);

// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A7B38
// line start: 1897
// line end:   1916
int razSideMoveSpiderCheck(struct _Instance *instance, int x)
{ //line: 1
	// address: -32
	// size: 0x8
	auto struct SVECTOR startVec;
	// address: -24
	// size: 0x8
	auto struct SVECTOR endVec;
} //line: 20


// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A7C04
// size: 0x29C
// line start: 1934
// line end:   1940
struct _Instance *RAZIEL_QueryEngagedInstance(int index);

// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A7C3C
// line start: 1947
// line end:   1981
int razUpdateSoundRamp(struct _Instance *instance, struct _SoundRamp *sound)
{ //line: 1
	// register: $s1
	register int volume;
	// register: $t0
	register int time;
	{ //line: 16
		// register: $v1
		register int startPitch;
		// register: $v0
		register int endPitch;
		// register: $a1
		register int startVolume;
		// register: $v0
		register int endVolume;
	} //line: 31
} //line: 34


// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A7D40
// line start: 1987
// line end:   2003
void razSetupSoundRamp(struct _Instance *instance, struct _SoundRamp *sound, int sfx, int startPitch, int endPitch, int startVolume, int endVolume, int timer, int distance);

// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A7DE4
// line start: 2011
// line end:   2030
void RAZIEL_SetInteractiveMusic(int modifier, int action)
{ //line: 1
	// register: $s0
	register int soundFlag;
} //line: 20


// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A7E60
// line start: 2032
// line end:   2033
void RAZIEL_DebugHurtRaziel();

// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A7E80
// line start: 2037
// line end:   2038
void RAZIEL_StartNewGame();

// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A7EA8
// line start: 2046
// line end:   2059
int razInBaseArea(char *name, int length)
{ //line: 1
	// address: -32
	// size: 0x9
	auto char string[9];
} //line: 14


// path: C:\kain2\game\RAZIEL\RAZLIB.C
// address: 0x800A7F04
// line start: 2065
// line end:   2075
void razPrepGlyph()
{ //line: 1
	// register: $s0
	// size: 0x29C
	register struct _Instance *heldInst;
} //line: 10


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800A7F54
// line start: 189
// line end:   406
void InitStates(struct _Instance *PlayerInstance)
{ //line: 1
	// register: $s1
	register unsigned char i;
	{ //line: 30
		// register: $v0
		// size: 0x30
		register struct _G2AnimSection_Type *animSection;
	} //line: 31
} //line: 42


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800A823C
// line start: 456
// line end:   564
void StateInitIdle(struct __CharacterState *In, int CurrentSection, int Ptr)
{ //line: 1
	// register: $s0
	// size: 0xC
	register struct evControlInitIdleData *data;
	// register: $a0
	// size: 0x29C
	register struct _Instance *linkWeapon;
} //line: 109


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800A84E0
// line start: 614
// line end:   874
void StateHandlerIdle(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $s0
	// size: 0x8
	register struct __Event *Ptr;
	// register: $s5
	register int Anim;
	// register: $s7
	register int blockForwardMotion;
	{ //line: 153
		// register: $v0
		// size: 0x29C
		register struct _Instance *heldInst;
	} //line: 159
	{ //line: 197
		// register: $v0
		// size: 0x24
		register struct evPhysicsEdgeData *EdgeData;
	} //line: 204
} //line: 261


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800A8DA0
// line start: 879
// line end:   955
void StateHandlerLookAround(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $s0
	// size: 0x8
	register struct __Event *Ptr;
	{ //line: 8
		// register: $s0
		// size: 0x29C
		register struct _Instance *instance;
	} //line: 16
	{ //line: 23
		// register: $s0
		// size: 0x29C
		register struct _Instance *instance;
	} //line: 24
	{ //line: 36
		// address: -32
		auto int message;
		// address: -28
		auto int messageData;
	} //line: 54
} //line: 77


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800A90E8
// line start: 959
// line end:   1162
void StateHandlerCrouch(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $s1
	// size: 0x8
	register struct __Event *Ptr;
	// register: $s0
	register int Anim;
	// register: $s5
	// size: 0x29C
	register struct _Instance *heldInst;
	// address: -48
	auto int DropThisFrame;
	{ //line: 115
		// register: $s0
		// size: 0x10
		register struct evObjectData *data;
		// register: $s2
		register int i;
	} //line: 142
} //line: 204


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800A9860
// line start: 1166
// line end:   1236
void StateHandlerDropAction(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $v0
	// size: 0x8
	register struct __Event *Ptr;
} //line: 71


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800A9AE0
// line start: 1244
// line end:   1366
void StateHandlerSoulSuck(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $a0
	// size: 0x8
	register struct __Event *Ptr;
	// register: $s3
	register int Anim;
	{ //line: 108
		// register: $v0
		// size: 0x18
		register struct evPhysicsSwimData *SwimData;
	} //line: 115
} //line: 123


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800AA018
// line start: 1373
// line end:   1433
void StateHandlerStartTurn(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $v0
	// size: 0x8
	register struct __Event *Ptr;
} //line: 61


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800AA218
// line start: 1438
// line end:   1457
void StateInitStartMove(struct __CharacterState *In, int CurrentSection, int Frame);

// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800AA2C4
// line start: 1527
// line end:   1698
void StateHandlerStartMove(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $a1
	// size: 0x8
	register struct __Event *Ptr;
	{ //line: 63
		// register: $a0
		register int mode;
	} //line: 81
} //line: 172


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800AA964
// line start: 1702
// line end:   1761
void StateInitMove(struct __CharacterState *In, int CurrentSection, int Frames);

// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800AABA8
// line start: 1908
// line end:   2074
void StateHandlerMove(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $s0
	// size: 0x8
	register struct __Event *Ptr;
	// register: $s6
	register int Anim;
	{ //line: 66
		// register: $a2
		register int data;
	} //line: 78
} //line: 167


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800AB27C
// line start: 2078
// line end:   2171
void StateHandlerStopMove(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $a1
	// size: 0x8
	register struct __Event *Ptr;
} //line: 94


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800AB5AC
// line start: 2179
// line end:   2274
void StateHandlerCompression(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $v0
	// size: 0x8
	register struct __Event *Ptr;
} //line: 96


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800AB970
// line start: 2278
// line end:   2377
void StateHandlerJump(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $v0
	// size: 0x8
	register struct __Event *Ptr;
} //line: 100


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800ABDA8
// line start: 2381
// line end:   2586
void StateHandlerFall(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $s0
	// size: 0x8
	register struct __Event *Ptr;
	// register: $s7
	register int Moving;
	// register: $s6
	register int DeferFlag;
	// register: $s1
	// size: 0x18
	register struct evPhysicsSwimData *SwimData;
	{ //line: 19
	} //line: 25
} //line: 206


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800AC21C
// line start: 2591
// line end:   2647
void StateHandlerSlide(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $v0
	// size: 0x8
	register struct __Event *Ptr;
} //line: 57


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800AC3FC
// line start: 2658
// line end:   2757
void StateHandlerBlock(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $a0
	// size: 0x8
	register struct __Event *Ptr;
	// register: $s3
	register int Anim;
	{ //line: 39
		// address: -56
		// size: 0x8
		auto struct _Rotation rot;
	} //line: 40
	{ //line: 55
	} //line: 56
	{ //line: 69
		// register: $s1
		// size: 0x24
		register struct evPhysicsEdgeData *EdgeData;
		{ //line: 74
			// address: -56
			// size: 0x8
			auto struct SVECTOR startVec;
			// address: -48
			// size: 0x8
			auto struct SVECTOR endVec;
		} //line: 82
	} //line: 88
} //line: 100


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800AC818
// line start: 2761
// line end:   2849
void StateHandlerDeCompression(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $s1
	// size: 0x8
	register struct __Event *Ptr;
	{ //line: 10
	} //line: 49
} //line: 89


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800ACB88
// line start: 2855
// line end:   3052
void StateHandlerGlide(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $s0
	// size: 0x8
	register struct __Event *Ptr;
	// register: $s7
	register int Frame;
	// register: $s5
	register int Anim;
	// register: $s6
	register int moving;
	{ //line: 120
		// register: $a2
		// size: 0xA
		register struct evObjectDraftData *draft;
	} //line: 127
	{ //line: 132
		// register: $v0
		// size: 0x18
		register struct evPhysicsSwimData *SwimData;
	} //line: 145
} //line: 198


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800AD2F4
// line start: 3056
// line end:   3209
void StateHandlerHang(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $a0
	// size: 0x8
	register struct __Event *Ptr;
	// register: $s7
	register int Anim;
	{ //line: 13
		// register: $s0
		// size: 0xC
		register struct evControlInitHangData *data;
	} //line: 43
} //line: 154


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800AD898
// line start: 3213
// line end:   3284
void StateHandlerPushObject(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $v0
	// size: 0x8
	register struct __Event *Ptr;
	{ //line: 53
		// register: $s0
		// size: 0x10
		register struct evObjectData *data;
	} //line: 62
} //line: 72


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800ADBC4
// line start: 3288
// line end:   3404
void StateHandlerBreakOff(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $v0
	// size: 0x8
	register struct __Event *Ptr;
	{ //line: 8
		// register: $s0
		// size: 0x2C
		register struct PhysObInteractProperties *interactProp;
		// register: $s1
		// size: 0x29C
		register struct _Instance *Inst;
		// register: $a2
		register int anim;
	} //line: 57
	{ //line: 71
		// register: $v0
		// size: 0x2C
		register struct PhysObInteractProperties *interactProp;
		// register: $s1
		register int action;
		// register: $v0
		register int condition;
		// register: $s0
		// size: 0x29C
		register struct _Instance *Inst;
	} //line: 102
} //line: 117


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800ADFBC
// line start: 3408
// line end:   3590
void StateHandlerPullSwitch(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $s2
	// size: 0x8
	register struct __Event *Ptr;
	// address: -56
	auto int switchDone;
	// address: -52
	auto int hitPosted;
	{ //line: 13
		// register: $fp
		// size: 0x20
		register struct PhysObSwitchProperties *switchData;
		{ //line: 19
			// register: $s1
			register int switchStatus;
			// register: $s3
			register int switchSuccess;
			// register: $s2
			// size: 0x29C
			register struct _Instance *inst;
			// register: $s0
			// size: 0x20
			register struct PhysObSwitchProperties *switchProperties;
			// register: $s6
			register int extraZ;
			{ //line: 32
			} //line: 44
		} //line: 93
	} //line: 93
	{ //line: 99
		// register: $v1
		register int switchStatus;
		// register: $s0
		// size: 0x29C
		register struct _Instance *inst;
		// register: $s0
		// size: 0x20
		register struct PhysObSwitchProperties *switchProperties;
		// register: $s1
		register int switchClass;
	} //line: 140
	{ //line: 152
		// register: $s0
		// size: 0x10
		register struct evMonsterHitData *data;
	} //line: 163
} //line: 183


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800AE5CC
// line start: 3594
// line end:   3771
void StateHandlerDragObject(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $s1
	// size: 0x8
	register struct __Event *Ptr;
	// register: $s3
	// size: 0x29C
	register struct _Instance *instance;
	// register: $s0
	register int anim;
	// register: $s7
	register int hitPosted;
	{ //line: 44
		// address: -48
		// size: 0x28
		auto struct evPhysicsSlideData *slideData;
		{ //line: 49
			// register: $s0
			// size: 0x10
			register struct evObjectData *data;
		} //line: 106
	} //line: 107
	{ //line: 163
		// register: $v0
		// size: 0x10
		register struct evMonsterHitData *data;
	} //line: 164
} //line: 178


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800AECF4
// line start: 3775
// line end:   3831
void StateHandlerPickupObject(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $v0
	// size: 0x8
	register struct __Event *Ptr;
	{ //line: 9
		// address: -32
		// size: 0x4
		auto long colorArray[1];
	} //line: 9
} //line: 57


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800AEF10
// line start: 3832
// line end:   4038
void StateHandlerAutoFace(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $a0
	// size: 0x8
	register struct __Event *Ptr;
	// register: $s3
	register int Frames;
	// register: $s2
	register int Anim;
} //line: 207


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800AF650
// line start: 4045
// line end:   4300
void StateHandlerGlyphs(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $s0
	// size: 0x8
	register struct __Event *Ptr;
	// register: $s5
	register int hitPosted;
	{ //line: 205
		// register: $v0
		// size: 0x18
		register struct evActionPlayHostAnimationData *ptr;
	} //line: 212
	{ //line: 215
		// register: $v1
		// size: 0x10
		register struct evMonsterHitData *data;
	} //line: 224
} //line: 253


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800AFEE4
// line start: 4356
// line end:   4717
void DefaultStateHandler(struct __CharacterState *In, int CurrentSection, int Data)
{ //line: 1
	// register: $s0
	// size: 0x8
	register struct __Event *Ptr;
	{ //line: 20
		// address: -32
		auto int message;
		// address: -28
		auto int messageData;
		{ //line: 40
			// register: $s1
			register int i;
		} //line: 46
	} //line: 49
	{ //line: 211
		// register: $v1
		// size: 0xC
		register struct evPhysicsGravityData *ptr;
		// register: $s0
		register short zRot;
		{ //line: 217
			// address: -48
			// size: 0x6
			auto struct _Position pos1;
			// address: -40
			// size: 0x6
			auto struct _Position pos2;
			// register: $v0
			register int diff;
		} //line: 237
	} //line: 237
	{ //line: 270
		// register: $a0
		// size: 0x14
		register struct evFXHitData *BloodData;
		// address: -48
		// size: 0x8
		auto struct _SVector Accel;
	} //line: 271
	{ //line: 319
		// register: $s0
		// size: 0x24
		register struct evPhysicsEdgeData *data;
	} //line: 325
} //line: 362


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800B087C
// line start: 4722
// line end:   4925
long RazielAnimCallback(struct _G2Anim_Type *anim, int sectionID, enum _G2AnimCallbackMsg_Enum_duplicate_e5 message, long messageDataA, long messageDataB, void *data)
{ //line: 1
	// register: $a0
	// size: 0x11C
	register struct __State *pSection;
	// register: $a2
	// size: 0x30
	register struct _G2AnimSection_Type *animSection;
	{ //line: 19
		// register: $v1
		// size: 0x10
		register struct evAnimationControllerDoneData *ControllerData;
	} //line: 35
	{ //line: 49
		// register: $s0
		// size: 0x4
		register struct __AlarmData *data;
		// register: $a0
		// size: 0x29C
		register struct _Instance *inst;
		{ //line: 120
			// register: $a0
			register int test;
			{ //line: 140
				// register: $t0
				// size: 0x18
				register struct _SoundRamp *soundRamp;
			} //line: 155
		} //line: 155
	} //line: 164
	{ //line: 168
		{ //line: 173
			// register: $s0
			// size: 0x29C
			register struct _Instance *heldInstance;
		} //line: 179
	} //line: 188
} //line: 203


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800B0E80
// line start: 4926
// line end:   4957
long RazielAnimCallbackDuringPause(struct _G2Anim_Type *anim, int sectionID, enum _G2AnimCallbackMsg_Enum_duplicate_e5 message, long messageDataA, long messageDataB, void *data)
{ //line: 1
	{ //line: 10
		// register: $v1
		// size: 0x10
		register struct evAnimationControllerDoneData *ControllerData;
	} //line: 26
} //line: 31


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800B0F34
// line start: 5000
// line end:   5169
unsigned long RazielQuery(struct _Instance *instance, unsigned long Query)
{ //line: 1
	{ //line: 63
		// register: $a0
		// size: 0x8
		register struct _Normal *Ptr;
	} //line: 79
	{ //line: 82
		// register: $s0
		register unsigned long ability;
	} //line: 100
	{ //line: 102
		// address: -32
		// size: 0x18
		auto struct PlayerSaveData data;
	} //line: 119
	{ //line: 139
		// register: $v0
		// size: 0xC
		register struct evShadowSegmentData *shadowData;
	} //line: 146
} //line: 170


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800B1290
// line start: 5174
// line end:   5542
void RazielPost(struct _Instance *instance, unsigned long Message, unsigned long Data)
{ //line: 1
	// register: $s0
	register int i;
	{ //line: 8
	} //line: 26
	{ //line: 72
		// register: $s0
		// size: 0x18
		register struct PlayerSaveData *data;
	} //line: 117
	{ //line: 143
	} //line: 149
	{ //line: 169
	} //line: 185
	{ //line: 188
		// register: $v0
		// size: 0x30
		register struct _G2AnimSection_Type *animSection;
	} //line: 193
	{ //line: 197
		// register: $v0
		// size: 0x30
		register struct _G2AnimSection_Type *animSection;
	} //line: 202
	{ //line: 259
	} //line: 269
	{ //line: 271
	} //line: 288
	{ //line: 298
		// register: $s2
		register int i;
		// register: $v1
		// size: 0x29C
		register struct _Instance *heldWeapon;
	} //line: 321
	{ //line: 328
		// register: $v1
		// size: 0x29C
		register struct _Instance *heldWeapon;
	} //line: 351
} //line: 369


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800B1C10
// line start: 5547
// line end:   5837
int SetStates(struct _Instance *instance, struct GameTracker *GT, long *controlCommand, int AnalogLength)
{ //line: 1
	// register: $s1
	register int i;
	// register: $s2
	register int Event;
	// register: $s3
	register int Data1;
	static unsigned long LastTime;
	// register: $v0
	// size: 0x8
	register struct __Event *Ptr;
	{ //line: 111
		// register: $v0
		// size: 0x8
		register struct __Event *message;
	} //line: 149
	{ //line: 270
	} //line: 272
	{ //line: 278
	} //line: 287
} //line: 289


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800B22A0
// line start: 5846
// line end:   6010
void ProcessConstrict()
{ //line: 1
	{ //line: 42
		// register: $s0
		register int i;
		{ //line: 90
			// register: $v1
			register int thisIndex;
			// register: $v0
			register int nextIndex;
		} //line: 111
	} //line: 134
} //line: 165


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800B2574
// line start: 6014
// line end:   6042
void RelocateConstrict(struct _SVector *offset)
{ //line: 1
	// register: $a2
	register int i;
} //line: 29


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800B26D4
// line start: 6043
// line end:   6135
void ProcessEffects(struct _Instance *instance)
{ //line: 1
	// register: $s2
	// size: 0x29C
	register struct _Instance *heldInst;
	{ //line: 52
		// register: $v1
		register int step;
	} //line: 91
} //line: 93


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800B28E8
// line start: 6140
// line end:   6201
void ProcessHints()
{ //line: 1
	// register: $s0
	register long hint;
} //line: 62


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800B2A20
// line start: 6207
// line end:   6280
void ProcessInteractiveMusic(struct _Instance *instance)
{ //line: 1
	// register: $s1
	// size: 0x188
	register struct Level *level;
	{ //line: 18
	} //line: 25
} //line: 74


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800B2BD0
// line start: 6285
// line end:   6383
void ProcessTimers(struct _Instance *instance);

// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800B30C4
// line start: 6390
// line end:   6391
void SetTimer(int ticks);

// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800B30D4
// line start: 6398
// line end:   6493
void ProcessSpecialAbilities(struct _Instance *instance)
{ //line: 1
	// register: $s0
	register unsigned long reaver;
	{ //line: 8
		// register: $a0
		register unsigned long temp;
	} //line: 17
	{ //line: 41
		// register: $a1
		// size: 0x4C
		register struct Object *soulReaverOb;
		{ //line: 45
		} //line: 45
	} //line: 45
	{ //line: 66
		// register: $a1
		// size: 0x188
		register struct Level *level;
	} //line: 77
} //line: 96


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800B328C
// line start: 6528
// line end:   6643
int GetControllerMessages(long *controlCommand)
{ //line: 1
} //line: 55


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800B3484
// line start: 6667
// line end:   6910
void RazielAdditionalCollide(struct _Instance *instance, struct GameTracker *gameTracker)
{ //line: 1
	// register: $s0
	register int rc;
	// register: $s0
	register int Mode;
	// register: $a1
	register short Height;
	{ //line: 32
	} //line: 52
	{ //line: 65
		// register: $s1
		// size: 0x24
		register struct evPhysicsEdgeData *Data;
	} //line: 98
	{ //line: 116
		// register: $s0
		// size: 0x18
		register struct evPhysicsSwimData *swimData;
		// register: $s1
		// size: 0x29C
		register struct _Instance *Inst;
	} //line: 135
	{ //line: 237
	} //line: 242
} //line: 244


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800B3944
// line start: 6912
// line end:   6930
int GetEngageEvent(struct _Instance *instance)
{ //line: 1
	// register: $v1
	register int Ability;
} //line: 19


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800B399C
// line start: 6934
// line end:   6963
int SetupReaction(struct _Instance *player, struct _Instance *instance)
{ //line: 1
	// register: $s3
	register int FaceAngle;
	{ //line: 17
	} //line: 26
} //line: 27


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800B3A8C
// line start: 6987
// line end:   6988
int CheckHolding(struct _Instance *instance);

// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800B3A98
// line start: 6993
// line end:   6997
void DisableWristCollision(struct _Instance *instance, int Side);

// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800B3AE8
// line start: 7002
// line end:   7006
void EnableWristCollision(struct _Instance *instance, int Side);

// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800B3B38
// line start: 7139
// line end:   7209
int GetCollisionType(struct _Instance *instance)
{ //line: 1
	// register: $s0
	// size: 0x30
	register struct _CollideInfo *collideInfo;
	// register: $v0
	// size: 0x14
	register struct _HSphere *S;
	{ //line: 15
		// register: $v1
		// size: 0x29C
		register struct _Instance *inst;
	} //line: 15
} //line: 71


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800B3CE0
// line start: 7283
// line end:   7304
void InitGlyphSystem(struct _Instance *instance)
{ //line: 1
	// register: $a1
	// size: 0x4C
	register struct Object *GlyphOb;
	{ //line: 6
		// register: $v0
		// size: 0x29C
		register struct _Instance *iGlyph;
	} //line: 15
} //line: 22


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800B3D18
// line start: 7310
// line end:   7340
void mdRazielProcess(struct _Instance *playerInstance, struct GameTracker *gameTracker, long *controlCommand);

// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800B3DD0
// line start: 7344
// line end:   7356
void RazielProcess(struct _Instance *playerInstance, struct GameTracker *gameTracker)
{ //line: 1
} //line: 1


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800B3E14
// line start: 7359
// line end:   7410
void RazielInit(struct _Instance *instance, struct GameTracker *gameTracker);

// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800B3EDC
// line start: 7428
// line end:   7509
void RazielCollide(struct _Instance *instance, struct GameTracker *gameTracker)
{ //line: 1
	// register: $s2
	// size: 0x30
	register struct _CollideInfo *collideInfo;
	{ //line: 30
		// register: $s1
		// size: 0x8
		register struct SVECTOR *offset;
	} //line: 46
	{ //line: 64
		// register: $a1
		// size: 0x8
		register struct SVECTOR *offset;
	} //line: 80
} //line: 82


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800B4140
// line start: 7510
// line end:   7559
void RAZIEL_TurnHead(struct _Instance *instance, short *rotx, short *rotz, struct GameTracker *gameTracker)
{ //line: 1
	{ //line: 5
		// address: -40
		// size: 0x8
		auto struct _Rotation rot;
	} //line: 27
	{ //line: 30
		// address: -32
		// size: 0x10
		auto struct evActionLookAroundData data;
	} //line: 48
} //line: 50


// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800B4364
// line start: 7560
// line end:   7586
void RAZIEL_SetLookAround(struct _Instance *instance);

// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800B43DC
// line start: 7589
// line end:   7608
void RAZIEL_ResetLookAround(struct _Instance *instance);

// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800B4458
// line start: 7612
// line end:   7637
long RAZIEL_OkToLookAround(struct _Instance *playerInstance);

// path: C:\kain2\game\RAZIEL\RAZIEL.C
// address: 0x800B44B8
// line start: 7639
// line end:   7685
void TrailWaterFX(struct _Instance *instance, int Segment, int Bubbles, int Type)
{ //line: 1
	// address: -88
	// size: 0x8
	auto struct _SVector Pos;
	// address: -80
	// size: 0x8
	auto struct _SVector Vel;
	// address: -72
	// size: 0x8
	auto struct _SVector Accl;
	// register: $s4
	register int i;
	// register: $s1
	register int j;
	// register: $s5
	// size: 0x188
	register struct Level *level;
	// address: -64
	// size: 0x14
	auto struct __BubbleParams BP;
} //line: 47


// path: C:\kain2\game\UNDRWRLD.C
// address: 0x800B4838
// line start: 66
// line end:   84
void UNDERWORLD_StartProcess();

// path: C:\kain2\game\UNDRWRLD.C
// address: 0x800B48AC
// line start: 88
// line end:   127
long UNDERWORLD_RotateScreenStep(long time)
{ //line: 1
	// register: $t2
	register int row;
	// register: $a3
	register int col;
	// register: $s0
	register int sinAngle;
	// register: $t3
	register int cosAngle;
	// register: $v1
	register int hx;
	// register: $v0
	register int hy;
	{ //line: 16
		// register: $a0
		// size: 0x8
		register struct UW_ScreenXY *p;
		// register: $t0
		register int scaleY;
	} //line: 16
} //line: 39


// path: C:\kain2\game\UNDRWRLD.C
// address: 0x800B4A18
// line start: 163
// line end:   174
void UNDERWORLD_DoUV(unsigned char *uv, struct UW_ScreenXY *p0, int tx)
{ //line: 2
	// register: $v1
	register int u;
} //line: 12


// path: C:\kain2\game\UNDRWRLD.C
// address: 0x800B4A64
// size: 0x28
// line start: 177
// line end:   220
struct POLY_GT3 *UNDERWORLD_Poly(struct POLY_GT3 *last, struct UW_ScreenXY *p0, struct UW_ScreenXY *p1, struct UW_ScreenXY *p2, int drawY)
{ //line: 1
	// register: $s0
	register int tx;
	// register: $s1
	// size: 0x28
	register struct POLY_GT3 *poly;
	// register: $v1
	register int col;
} //line: 6


// path: C:\kain2\game\UNDRWRLD.C
// address: 0x800B4BDC
// line start: 224
// line end:   271
void UNDERWORLD_DisplayFrame(long *primStart, long drawY)
{ //line: 1
	// register: $s5
	register long row;
	// register: $s0
	register long col;
	// register: $v0
	register long d;
	// register: $s2
	// size: 0x8
	register struct UW_ScreenXY *p0;
	// register: $s1
	// size: 0x8
	register struct UW_ScreenXY *p1;
	// register: $s3
	// size: 0x8
	register struct UW_ScreenXY *p2;
	// register: $s4
	// size: 0x8
	register struct UW_ScreenXY *p3;
	// register: $a1
	// size: 0x28
	register struct POLY_GT3 *poly;
	// register: $fp
	// size: 0x28
	register struct POLY_GT3 *terminator;
} //line: 45


// path: C:\kain2\game\UNDRWRLD.C
// address: 0x800B4D34
// line start: 274
// line end:   294
void UNDERWORLD_SetupSource()
{ //line: 1
	// address: -32
	// size: 0x8
	auto struct RECT rect;
	// address: -24
	// size: 0xC
	auto struct DR_STP stp;
} //line: 1


// path: C:\kain2\game\UNDRWRLD.C
// address: 0x800B4DE0
// line start: 297
// line end:   323
void UNDERWORLD_InitDisplayProcess()
{ //line: 2
	// register: $t0
	register int row;
	// register: $a1
	register int col;
	{ //line: 11
		// register: $v0
		// size: 0x8
		register struct UW_ScreenXY *p;
	} //line: 11
} //line: 19


// path: C:\kain2\game\UNDRWRLD.C
// address: 0x800B4EA0
// size: 0x40
// line start: 326
// line end:   453
struct _StreamUnit *UNDERWORLD_LoadLevel(char *baseAreaName, struct GameTracker *gameTracker)
{ //line: 1
	// address: -72
	// size: 0x8
	auto struct _SVector offset;
	// register: $s3
	// size: 0x40
	register struct _StreamUnit *streamUnit;
	// register: $s1
	register long i;
	// register: $s2
	register long UW_time;
	{ //line: 37
		// register: $v1
		register short _x1;
		// register: $a0
		register short _y1;
		// register: $a1
		register short _z1;
		// register: $v0
		// size: 0x8
		register struct _SVector *_v0;
		// register: $v0
		// size: 0x6
		register struct _Position *_v1;
	} //line: 37
	{ //line: 88
		// address: -64
		// size: 0x18
		auto struct POLY_F4 poly;
		// address: -40
		// size: 0x8
		auto struct DR_TPAGE tpage;
	} //line: 108
} //line: 121


// path: C:\kain2\game\UNDRWRLD.C
// address: 0x800B5198
// line start: 456
// line end:   462
void UNDERWORLD_UpdatePlayer(struct Intro *playerIntro, struct _Instance *instance)
{ //line: 1
	// address: -16
	// size: 0x8
	auto struct _SVector offset;
	{ //line: 1
		// register: $v0
		register short _x0;
		// register: $v1
		register short _y0;
		// register: $a0
		register short _z0;
		// register: $a1
		register short _x1;
		// register: $t0
		register short _y1;
		// register: $a3
		register short _z1;
		// register: $a1
		// size: 0x8
		register struct _SVector *_v;
		// register: $a1
		// size: 0x6
		register struct _Position *_v0;
		// register: $a3
		// size: 0x6
		register struct _Position *_v1;
	} //line: 1
} //line: 1


// path: C:\kain2\game\SAVEINFO.C
// address: 0x800B51F8
// line start: 103
// line end:   120
void SAVE_GetInstanceRotation(struct _Instance *instance, struct _SmallRotation *vector)
{ //line: 1
	// register: $v1
	// size: 0x8
	register struct evPositionData *rotation;
} //line: 18


// path: C:\kain2\game\SAVEINFO.C
// address: 0x800B5280
// line start: 124
// line end:   143
void SAVE_ClearMemory(struct GameTracker *gameTracker)
{ //line: 1
	// register: $v0
	register char *buffer;
} //line: 1


// path: C:\kain2\game\SAVEINFO.C
// address: 0x800B5300
// line start: 148
// line end:   170
void SAVE_Init(struct GameTracker *gt)
{ //line: 1
	// register: $s1
	register void *buffer;
	{ //line: 10
	} //line: 12
} //line: 21


// path: C:\kain2\game\SAVEINFO.C
// address: 0x800B5384
// line start: 189
// line end:   232
void *SAVE_GetSavedBlock(long saveType, long extraSize)
{ //line: 1
	// register: $s2
	// size: 0x2
	register struct SavedBasic *savedInfo;
	// register: $s0
	register long sizeOfSave;
	// register: $s1
	register long done;
} //line: 43


// path: C:\kain2\game\SAVEINFO.C
// address: 0x800B54A0
// line start: 237
// line end:   259
long SAVE_PurgeAMemoryBlock()
{ //line: 1
	// register: $a0
	// size: 0x2
	register struct SavedBasic *curSave;
	// register: $a1
	register long result;
} //line: 22


// path: C:\kain2\game\SAVEINFO.C
// address: 0x800B5540
// line start: 265
// line end:   319
long SAVE_SaveableInstance(struct _Instance *instance)
{ //line: 2
	// register: $a1
	register long result;
} //line: 54


// path: C:\kain2\game\SAVEINFO.C
// address: 0x800B562C
// size: 0x28
// line start: 470
// line end:   509
struct _SavedIntro *SAVE_UpdateSavedIntro(struct _Instance *instance, struct Level *level, struct _SavedIntro *savedIntro, struct evControlSaveDataData *extraData)
{ //line: 1
	// register: $t1
	// size: 0x6
	register struct _Position *levelOffset;
	{ //line: 12
		// register: $v0
		register short _x0;
		// register: $a2
		register short _y0;
		// register: $v1
		register short _z0;
		// register: $a3
		register short _x1;
		// register: $t0
		register short _y1;
		// register: $t1
		register short _z1;
		// register: $v0
		// size: 0x6
		register struct _Position *_v;
		// register: $v1
		// size: 0x6
		register struct _Position *_v0;
	} //line: 12
} //line: 39


// path: C:\kain2\game\SAVEINFO.C
// address: 0x800B5738
// size: 0x20
// line start: 512
// line end:   552
struct _SavedIntroWithIntro *SAVE_UpdateSavedIntroWithIntro(struct _Instance *instance, struct Level *level, struct _SavedIntroWithIntro *savedIntro, struct evControlSaveDataData *extraData)
{ //line: 1
	// register: $t1
	// size: 0x6
	register struct _Position *levelOffset;
	{ //line: 12
		// register: $v0
		register short _x0;
		// register: $a2
		register short _y0;
		// register: $v1
		register short _z0;
		// register: $a3
		register short _x1;
		// register: $t0
		register short _y1;
		// register: $t1
		register short _z1;
		// register: $v0
		// size: 0x6
		register struct _Position *_v;
		// register: $v1
		// size: 0x6
		register struct _Position *_v0;
	} //line: 12
} //line: 40


// path: C:\kain2\game\SAVEINFO.C
// address: 0x800B5870
// size: 0x2
// line start: 555
// line end:   575
struct SavedBasic *SAVE_GetSavedEvent(long areaID, long eventNumber)
{ //line: 2
	// register: $v1
	// size: 0x2
	register struct SavedBasic *curSave;
} //line: 21


// path: C:\kain2\game\SAVEINFO.C
// address: 0x800B5918
// line start: 578
// line end:   587
void SAVE_DeleteSavedEvent(long areaID, long eventNumber)
{ //line: 1
	// register: $v0
	// size: 0x2
	register struct SavedBasic *savedEvent;
} //line: 10


// path: C:\kain2\game\SAVEINFO.C
// address: 0x800B5948
// size: 0x2
// line start: 590
// line end:   612
struct SavedBasic *SAVE_GetSavedNextEvent(long areaID, struct SavedBasic *curSave);

// path: C:\kain2\game\SAVEINFO.C
// address: 0x800B59EC
// line start: 618
// line end:   653
void SAVE_BufferIntro(struct SavedBasic *savedIntro)
{ //line: 2
	// register: $a1
	register long i;
} //line: 36


// path: C:\kain2\game\SAVEINFO.C
// address: 0x800B5A6C
// line start: 656
// line end:   712
void SAVE_IntroduceBufferIntros()
{ //line: 1
	// register: $s2
	register long i;
	// register: $v0
	// size: 0x40
	register struct _StreamUnit *streamUnit;
	{ //line: 7
		// address: -24
		auto int deleted;
		{ //line: 13
		} //line: 29
		{ //line: 35
		} //line: 54
	} //line: 56
} //line: 57


// path: C:\kain2\game\SAVEINFO.C
// address: 0x800B5B48
// line start: 715
// line end:   739
void SAVE_IntroForStreamID(struct _StreamUnit *streamUnit)
{ //line: 1
	// register: $s0
	// size: 0x2
	register struct SavedBasic *saveIntro;
	// register: $s1
	register long streamID;
	// address: -24
	auto int deleted;
} //line: 25


// path: C:\kain2\game\SAVEINFO.C
// address: 0x800B5C24
// line start: 742
// line end:   766
long SAVE_HasSavedIntro(struct Intro *intro, long currentStreamID)
{ //line: 2
	// register: $a3
	register long result;
	// register: $a1
	// size: 0x28
	register struct _SavedIntro *saveIntro;
} //line: 24


// path: C:\kain2\game\SAVEINFO.C
// address: 0x800B5CB4
// size: 0x8
// line start: 794
// line end:   810
struct SavedLevel *SAVE_HasSavedLevel(long areaID)
{ //line: 2
	// register: $v1
	// size: 0x8
	register struct SavedLevel *savedLevel;
} //line: 17


// path: C:\kain2\game\SAVEINFO.C
// address: 0x800B5D18
// line start: 813
// line end:   856
void SAVE_UpdateLevelWithSave(struct _StreamUnit *streamUnit)
{ //line: 1
	// register: $t2
	register long i;
	// register: $t3
	// size: 0x10
	register struct ActualSavedLevel *savedLevel;
	// register: $t4
	// size: 0x54
	register struct _Terrain *terrain;
	// register: $v1
	// size: 0x24
	register struct BSPTree *bspTree;
	// register: $s1
	register long Zoffset;
} //line: 44


// path: C:\kain2\game\SAVEINFO.C
// address: 0x800B5E64
// size: 0x8
// line start: 859
// line end:   942
struct SavedLevel *SAVE_CreatedSavedLevel(long areaID, struct Level *level)
{ //line: 1
	// register: $s0
	// size: 0x8
	register struct SavedLevel *savedLevel;
	// register: $a0
	// size: 0x10
	register struct ActualSavedLevel *slevel;
	// register: $a1
	register long doSave;
	// register: $a1
	register long i;
	// register: $v1
	// size: 0x24
	register struct BSPTree *bspTree;
	// register: $s4
	register long Zoffset;
	{ //line: 21
		// register: $s1
		register long numBSPTrees;
		{ //line: 26
		} //line: 71
	} //line: 72
} //line: 83


// path: C:\kain2\game\SAVEINFO.C
// address: 0x800B6020
// line start: 946
// line end:   987
void SAVE_DeleteBlock(struct SavedBasic *savedBlock)
{ //line: 1
	// register: $s0
	register long size;
	// register: $a1
	register char *nextBlock;
	{ //line: 13
		// register: $a3
		register int i;
	} //line: 31
} //line: 33


// path: C:\kain2\game\SAVEINFO.C
// address: 0x800B60AC
// line start: 990
// line end:   1100
void SAVE_Instance(struct _Instance *instance, struct Level *level)
{ //line: 1
	// register: $v0
	// size: 0x28
	register struct _SavedIntro *savedIntro;
	// register: $s0
	// size: 0x8
	register struct evControlSaveDataData *extraData;
	// register: $s2
	register long extraSize;
	// register: $v1
	register long saveType;
	{ //line: 12
		// register: $a0
		// size: 0x4
		register struct SavedIntroSmall *savedSmallIntro;
		{ //line: 19
		} //line: 25
	} //line: 25
	{ //line: 57
		// register: $v0
		// size: 0x20
		register struct _SavedIntroWithIntro *savedIntroWithIntro;
	} //line: 71
	{ //line: 78
		// register: $s0
		// size: 0xC
		register struct SavedIntroSpline *savedIntroSpline;
		{ //line: 85
			// register: $a1
			// size: 0x50
			register struct MultiSpline *multi;
		} //line: 105
	} //line: 106
} //line: 111


// path: C:\kain2\game\SAVEINFO.C
// address: 0x800B62E0
// line start: 1103
// line end:   1130
void SAVE_DeleteInstance(struct _Instance *instance)
{ //line: 1
	// register: $a0
	// size: 0x2
	register struct SavedBasic *saveIntro;
} //line: 28


// path: C:\kain2\game\SAVEINFO.C
// address: 0x800B63B0
// line start: 1133
// line end:   1178
void SAVE_SetDeadDeadBit(int uniqueID, long set)
{ //line: 2
	// register: $a2
	// size: 0x28
	register struct _SavedIntro *saveIntro;
	// register: $v1
	// size: 0x342
	register struct SavedDeadDeadBits *deadDeadBits;
	{ //line: 21
		// register: $a2
		register int deadByte;
		// register: $a0
		register int deadBit;
	} //line: 43
} //line: 46


// path: C:\kain2\game\SAVEINFO.C
// address: 0x800B6474
// line start: 1181
// line end:   1197
void SAVE_RestoreGlobalSavePointer()
{ //line: 2
	// register: $v1
	// size: 0x28
	register struct _SavedIntro *saveIntro;
} //line: 17


// path: C:\kain2\game\SAVEINFO.C
// address: 0x800B64CC
// line start: 1200
// line end:   1239
long SAVE_IsUniqueIDDeadDead(long uniqueID)
{ //line: 2
	// register: $v1
	// size: 0x28
	register struct _SavedIntro *saveIntro;
	// register: $a2
	// size: 0x342
	register struct SavedDeadDeadBits *deadDeadBits;
	// register: $a3
	register long result;
	{ //line: 23
		// register: $a1
		register int deadByte;
		// register: $v1
		register int deadBit;
	} //line: 37
} //line: 39


// path: C:\kain2\game\SAVEINFO.C
// address: 0x800B6578
// line start: 1242
// line end:   1243
long SAVE_IsIntroDeadDead(struct Intro *intro);

// path: C:\kain2\game\SAVEINFO.C
// address: 0x800B659C
// line start: 1248
// line end:   1250
void SAVE_DoInstanceDeadDead(struct _Instance *instance);

// path: C:\kain2\game\SAVEINFO.C
// address: 0x800B65CC
// line start: 1258
// line end:   1259
void SAVE_MarkDeadDead(struct _Instance *instance);

// path: C:\kain2\game\SAVEINFO.C
// address: 0x800B65E0
// line start: 1263
// line end:   1264
void SAVE_UndestroyInstance(struct _Instance *instance);

// path: C:\kain2\game\SAVEINFO.C
// address: 0x800B6604
// size: 0x4
// line start: 1268
// line end:   1287
struct SavedIntroSmall *SAVE_GetSavedSmallIntro(struct _Instance *instance)
{ //line: 2
	// register: $a1
	// size: 0x2
	register struct SavedBasic *curSave;
} //line: 20


// path: C:\kain2\game\SAVEINFO.C
// address: 0x800B6670
// size: 0xC
// line start: 1290
// line end:   1308
struct SavedIntroSpline *SAVE_GetIntroSpline(struct _Instance *instance)
{ //line: 2
	// register: $a1
	// size: 0x2
	register struct SavedBasic *curSave;
} //line: 19


// path: C:\kain2\game\SAVEINFO.C
// address: 0x800B66DC
// line start: 1312
// line end:   1320
void SAVE_UpdateGlobalSaveTracker();

// path: C:\kain2\game\SAVEINFO.C
// address: 0x800B6770
// line start: 1325
// line end:   1347
void SAVE_RestoreGlobalSaveTracker();

// path: C:\kain2\game\SAVEINFO.C
// address: 0x800B6840
// line start: 1353
// line end:   1386
void SAVE_SaveEverythingInMemory()
{ //line: 1
	// register: $s0
	// size: 0x29C
	register struct _Instance *instance;
	// register: $s1
	register long i;
	{ //line: 9
		// register: $s1
		// size: 0x29C
		register struct _Instance *next;
		// register: $v0
		// size: 0x188
		register struct Level *level;
	} //line: 18
} //line: 34


// path: C:\kain2\game\SAVEINFO.C
// address: 0x800B68F4
// line start: 1390
// line end:   1400
void SAVE_SaveGame();

// path: C:\kain2\game\SAVEINFO.C
// address: 0x800B6940
// line start: 1405
// line end:   1412
void SAVE_RestoreGame();

// path: C:\kain2\game\SAVEINFO.C
// address: 0x800B69A4
// line start: 1421
// line end:   1449
void SAVE_DebugSaveGame();

// path: C:\kain2\game\SAVEINFO.C
// address: 0x800B69AC
// line start: 1489
// line end:   1495
void SAVE_LoadSaveGame();

// path: C:\kain2\game\SAVEINFO.C
// address: 0x800B69E8
// line start: 1499
// line end:   1500
long SAVE_SizeOfFreeSpace();

// path: C:\kain2\game\VOICEXA.C
// address: 0x800B6AE0
// line start: 54
// line end:   94
void VOICEXA_Init()
{ //line: 1
	// register: $s1
	register int i;
	// address: -80
	// size: 0x18
	auto struct CdlFILE fp;
	// register: $a0
	// size: 0x128
	register struct XAVoiceTracker *vt;
	// address: -56
	// size: 0x20
	auto char fileName[32];
} //line: 41


// path: C:\kain2\game\VOICEXA.C
// address: 0x800B6BAC
// line start: 98
// line end:   111
void putCdCommand(struct XAVoiceTracker *vt, unsigned char cdCommand, int numParams, unsigned char *params)
{ //line: 1
	// register: $t0
	register int i;
} //line: 14


// path: C:\kain2\game\VOICEXA.C
// address: 0x800B6C30
// line start: 115
// line end:   136
void VOICEXA_CdSyncCallback(unsigned char status, unsigned char *result)
{ //line: 1
	// register: $a1
	// size: 0x128
	register struct XAVoiceTracker *vt;
} //line: 22


// path: C:\kain2\game\VOICEXA.C
// address: 0x800B6C9C
// line start: 141
// line end:   162
void processCdCommands(struct XAVoiceTracker *vt)
{ //line: 1
	// register: $s0
	// size: 0x8
	register struct CdCommand *cmd;
} //line: 22


// path: C:\kain2\game\VOICEXA.C
// address: 0x800B6D44
// line start: 186
// line end:   196
void putVoiceCommand(struct XAVoiceTracker *vt, unsigned char voiceCmd, unsigned char nextVoiceStatus, int voiceCmdParam);

// path: C:\kain2\game\VOICEXA.C
// address: 0x800B6DC0
// line start: 203
// line end:   220
void processVoiceCommands(struct XAVoiceTracker *vt)
{ //line: 1
	// register: $s0
	// size: 0x4
	register struct VoiceCommand *cmd;
} //line: 18


// path: C:\kain2\game\VOICEXA.C
// address: 0x800B6E70
// line start: 226
// line end:   298
void voiceCmdPlay(struct XAVoiceTracker *vt, short voiceIndex)
{ //line: 1
	// address: -88
	// size: 0x4
	auto struct CdlFILTER filter;
	// address: -80
	// size: 0x4
	auto struct CdlLOC pos;
	// address: -32
	auto unsigned char mode;
	// address: -72
	// size: 0x28
	auto struct SpuCommonAttr spuattr;
	// register: $s1
	// size: 0x2
	register struct XAVoiceListEntry *voice;
	// register: $s0
	// size: 0x4
	register struct XAFileInfo *file;
} //line: 73


// path: C:\kain2\game\VOICEXA.C
// address: 0x800B6FE0
// line start: 302
// line end:   317
void voiceCmdStop(struct XAVoiceTracker *vt, short cmdParam)
{ //line: 1
	// address: -48
	// size: 0x28
	auto struct SpuCommonAttr spuattr;
} //line: 16


// path: C:\kain2\game\VOICEXA.C
// address: 0x800B703C
// line start: 321
// line end:   324
void voiceCmdPause(struct XAVoiceTracker *vt, short cmdParam);

// path: C:\kain2\game\VOICEXA.C
// address: 0x800B7078
// line start: 329
// line end:   332
void voiceCmdResume(struct XAVoiceTracker *vt, short cmdParam);

// path: C:\kain2\game\VOICEXA.C
// address: 0x800B70AC
// line start: 336
// line end:   337
void voiceCmdNull(struct XAVoiceTracker *vt, short cmdParam);

// path: C:\kain2\game\VOICEXA.C
// address: 0x800B70B4
// line start: 341
// line end:   367
void VOICEXA_Play(int voiceIndex, int queueRequests)
{ //line: 1
	// register: $a0
	// size: 0x128
	register struct XAVoiceTracker *vt;
	// register: $a2
	// size: 0x4
	register struct XAFileInfo *file;
} //line: 27


// path: C:\kain2\game\VOICEXA.C
// address: 0x800B7178
// line start: 387
// line end:   403
int VOICEXA_FinalStatus(struct XAVoiceTracker *vt)
{ //line: 2
	// register: $v0
	register int tailIndex;
} //line: 17


// path: C:\kain2\game\VOICEXA.C
// address: 0x800B71BC
// line start: 406
// line end:   423
void VOICEXA_Pause()
{ //line: 1
	// register: $s0
	// size: 0x128
	register struct XAVoiceTracker *vt;
	// register: $a0
	register int finalStatus;
} //line: 18


// path: C:\kain2\game\VOICEXA.C
// address: 0x800B722C
// line start: 427
// line end:   444
void VOICEXA_Resume()
{ //line: 1
	// register: $s0
	// size: 0x128
	register struct XAVoiceTracker *vt;
	// register: $a0
	register int finalStatus;
} //line: 18


// path: C:\kain2\game\VOICEXA.C
// address: 0x800B729C
// line start: 464
// line end:   528
void VOICEXA_Tick()
{ //line: 1
	// register: $s0
	// size: 0x128
	register struct XAVoiceTracker *vt;
} //line: 65


// path: C:\kain2\game\VOICEXA.C
// address: 0x800B73F4
// line start: 546
// line end:   557
int VOICEXA_IsPlaying()
{ //line: 2
	// register: $a0
	// size: 0x128
	register struct XAVoiceTracker *vt;
} //line: 12


// path: C:\kain2\game\CINEMA\CINEPSX.C
// address: 0x800B7434
// line start: 40
// line end:   43
int CINE_CDIntrQuery();

// path: C:\kain2\game\CINEMA\CINEPSX.C
// address: 0x800B745C
// line start: 51
// line end:   55
unsigned short CINE_Pad(int pad);

// path: C:\kain2\game\CINEMA\CINEPSX.C
// address: 0x800B7480
// line start: 62
// line end:   76
void CINE_Play(char *strfile, unsigned short mask, int buffers)
{ //line: 1
	{ //line: 12
	} //line: 13
} //line: 15


// path: C:\kain2\game\CINEMA\CINEPSX.C
// address: 0x800B74E4
// line start: 79
// line end:   105
int CINE_Load()
{ //line: 1
	// register: $s1
	// size: 0x24
	register struct _ObjectTracker *tracker;
	// register: $s0
	register int attempts;
	{ //line: 16
	} //line: 26
} //line: 27


// path: C:\kain2\game\CINEMA\CINEPSX.C
// address: 0x800B7588
// line start: 108
// line end:   109
int CINE_Loaded();

// path: C:\kain2\game\CINEMA\CINEPSX.C
// address: 0x800B7598
// line start: 113
// line end:   121
void CINE_Unload();

// path: C:\kain2\game\CINEMA\CINEPSX.C
// address: 0x800B75E0
// line start: 126
// line end:   137
void CINE_PlayIngame(int number)
{ //line: 1
	// address: -32
	// size: 0x18
	auto char movie_name[24];
} //line: 12


// path: C:\kain2\game\LOCAL\LOCALSTR.C
// address: 0x800B7630
// size: 0x4
// line start: 50
// line end:   51
enum language_t localstr_get_language();

// path: C:\kain2\game\LOCAL\LOCALSTR.C
// address: 0x800B763C
// line start: 55
// line end:   91
void localstr_set_language(enum language_t lang)
{ //line: 1
	// register: $a1
	register int i;
} //line: 37


// path: C:\kain2\game\LOCAL\LOCALSTR.C
// address: 0x800B76CC
// line start: 96
// line end:   105
char *localstr_get(enum localstr_t id)
{ //line: 2
	// address: 28
	// size: 0x2
	static char BlankStr[2];
} //line: 10


// path: C:\kain2\game\MENU\MENU.C
// address: 0x800B76F8
// line start: 83
// line end:   86
int menu_data_size()
{ //line: 2
} //line: 3


// path: C:\kain2\game\MENU\MENU.C
// address: 0x800B7700
// line start: 89
// line end:   93
void menu_initialize(struct menu_t *menu, void *opaque);

// path: C:\kain2\game\MENU\MENU.C
// address: 0x800B7744
// line start: 97
// line end:   110
void menu_format(struct menu_t *menu, int center, int xpos, int ypos, int width, int lineskip, int itemskip, int border)
{ //line: 1
	{ //line: 1
		// register: $a0
		// size: 0x1C
		register struct menu_format_t *fmt;
	} //line: 4
} //line: 4


// path: C:\kain2\game\MENU\MENU.C
// address: 0x800B7794
// line start: 115
// line end:   119
void menu_set(struct menu_t *menu, int (*fn)());

// path: C:\kain2\game\MENU\MENU.C
// address: 0x800B77B8
// line start: 123
// line end:   131
void menu_push(struct menu_t *menu, int (*fn)())
{ //line: 2
	{ //line: 4
		// register: $a0
		// size: 0x24
		register struct menu_stack_t *stack;
	} //line: 4
} //line: 4


// path: C:\kain2\game\MENU\MENU.C
// address: 0x800B7828
// line start: 134
// line end:   136
void menu_pop(struct menu_t *menu);

// path: C:\kain2\game\MENU\MENU.C
// address: 0x800B783C
// line start: 152
// line end:   167
void menu_item_flags(struct menu_t *menu, int (*fn)(), long parameter, long flags, char *format)
{ //line: 1
	{ //line: 1
		// register: $s1
		// size: 0x10
		register struct menu_item_t *item;
	} //line: 1
} //line: 1


// path: C:\kain2\game\MENU\MENU.C
// address: 0x800B78CC
// line start: 170
// line end:   192
void menu_item(struct menu_t *menu, int (*fn)(), long parameter, char *format)
{ //line: 1
	{ //line: 1
		// register: $s1
		// size: 0x10
		register struct menu_item_t *item;
		{ //line: 11
		} //line: 13
	} //line: 22
} //line: 23


// path: C:\kain2\game\MENU\MENU.C
// address: 0x800B7970
// line start: 195
// line end:   210
void menu_build(struct menu_t *menu)
{ //line: 1
	{ //line: 1
		// register: $s0
		// size: 0x24
		register struct menu_stack_t *stack;
	} //line: 1
} //line: 1


// path: C:\kain2\game\MENU\MENU.C
// address: 0x800B79C8
// line start: 221
// line end:   254
void DisplayHintBox(int len, int y)
{ //line: 1
	// address: -24
	// size: 0x10
	auto struct Extents2d ext;
	// register: $s0
	register int i;
	{ //line: 18
	} //line: 18
} //line: 34


// path: C:\kain2\game\MENU\MENU.C
// address: 0x800B7AF4
// line start: 257
// line end:   299
void DisplayMenuBox(int x0, int x1, int y0, int y1)
{ //line: 1
	// address: -56
	// size: 0x10
	auto struct Extents2d ext;
	// size: 0x40
	static int dy[16];
	// register: $s3
	register int i;
	// register: $s6
	register int j;
	// register: $s2
	register int k;
	// register: $s7
	register int slice0;
	// register: $s5
	register unsigned long **ot;
	// register: $s4
	// size: 0x1770C
	register struct _PrimPool *primPool;
	{ //line: 19
		{ //line: 21
		} //line: 21
	} //line: 33
} //line: 43


// path: C:\kain2\game\MENU\MENU.C
// address: 0x800B7D2C
// line start: 311
// line end:   478
int menu_draw_item(struct menu_t *menu, int ypos, int xadj, int yadj, char *text, int color, long flags, struct Extents2d *e)
{ //line: 1
	// address: -64
	// size: 0x1C
	auto struct menu_format_t *fmt;
	// register: $a1
	register int numColumns;
	// register: $v1
	register int i;
	// register: $a0
	register int texLen;
	// register: $fp
	register int columnWidth;
	// register: $a1
	register char *columnText;
	// address: -320
	// size: 0x100
	auto char tmpBuff[256];
	// register: $s1
	register char *lineText;
	// register: $s5
	register int columnYPos;
	// address: -60
	auto int maxColumnYPos;
	// address: -56
	auto int center;
	// address: -52
	auto int leftEdge;
	// register: $s4
	register int xpos;
	{ //line: 92
		// register: $s6
		register char *eol;
		{ //line: 99
			// register: $s2
			register char *eop;
			{ //line: 101
				// register: $s0
				register char *tmp;
				// register: $v0
				register int wd;
			} //line: 116
			{ //line: 123
			} //line: 129
			{ //line: 132
				// register: $s0
				register int wd;
			} //line: 139
		} //line: 142
	} //line: 146
} //line: 167


// path: C:\kain2\game\MENU\MENU.C
// address: 0x800B8130
// line start: 481
// line end:   517
void menu_draw(struct menu_t *menu)
{ //line: 1
	{ //line: 1
		// address: -48
		// size: 0x10
		auto struct Extents2d ext;
		// register: $s6
		// size: 0x24
		register struct menu_stack_t *stack;
		// register: $s5
		register int index;
		// register: $s3
		register int ypos;
		// register: $s0
		register int i;
		{ //line: 15
			// register: $v1
			// size: 0x10
			register struct menu_item_t *item;
			// register: $t0
			register int color;
		} //line: 23
	} //line: 36
} //line: 37


// path: C:\kain2\game\MENU\MENU.C
// address: 0x800B8290
// line start: 524
// line end:   587
void menu_run(struct menu_t *menu)
{ //line: 1
	// register: $s1
	// size: 0x4
	register enum menu_ctrl_t_duplicate_e2 ctrl;
	// register: $s4
	// size: 0x24
	register struct menu_stack_t *stack;
	// register: $s0
	register int index;
	// register: $s3
	// size: 0x10
	register struct menu_item_t *item;
	{ //line: 22
		// register: $v0
		// size: 0x4
		register enum menu_sound_t_duplicate_e1 sound;
	} //line: 35
} //line: 64


// path: C:\kain2\game\MENU\MENU.C
// address: 0x800B8480
// line start: 592
// line end:   595
void menu_process(struct menu_t *menu);

// path: C:\kain2\game\MENU\MENUUTIL.C
// address: 0x800B84B4
// size: 0x4
// line start: 18
// line end:   38
enum menu_ctrl_t_duplicate_e2 menu_get_ctrl(void *gt)
{ //line: 2
	// register: $v1
	// size: 0x4
	register enum menu_ctrl_t_duplicate_e2 ctrl;
} //line: 20


// path: C:\kain2\game\MENU\MENUUTIL.C
// address: 0x800B8534
// line start: 41
// line end:   64
void menu_print(int xpos, int ypos, char *text, int color);

// path: C:\kain2\game\MENU\MENUUTIL.C
// address: 0x800B859C
// line start: 69
// line end:   72
int menu_text_width(char *text);

// path: C:\kain2\game\MENU\MENUUTIL.C
// address: 0x800B85BC
// line start: 92
// line end:   96
void menu_sound(enum menu_sound_t_duplicate_e1 sound)
{ //line: 1
	// register: $a0
	// size: 0x8
	register struct menu_sound_entry_t *entry;
} //line: 1


// path: C:\kain2\game\MENU\MENUDEFS.C
// address: 0x800B85F0
// line start: 161
// line end:   170
void do_check_controller(void *gt);

// path: C:\kain2\game\MENU\MENUDEFS.C
// address: 0x800B8624
// line start: 177
// line end:   190
int do_push_menu(void *gt, long menuparam, enum menu_ctrl_t_duplicate_e2 ctrl)
{ //line: 1
} //line: 14


// path: C:\kain2\game\MENU\MENUDEFS.C
// address: 0x800B865C
// line start: 193
// line end:   198
int do_pop_menu(void *gt, long param, enum menu_ctrl_t_duplicate_e2 ctrl);

// path: C:\kain2\game\MENU\MENUDEFS.C
// address: 0x800B8694
// line start: 204
// line end:   209
int do_function(void *gt, long fnparam, enum menu_ctrl_t_duplicate_e2 ctrl);

// path: C:\kain2\game\MENU\MENUDEFS.C
// address: 0x800B86C8
// line start: 218
// line end:   243
int do_start_game(void *gt, long parameter, enum menu_ctrl_t_duplicate_e2 ctrl);

// path: C:\kain2\game\MENU\MENUDEFS.C
// address: 0x800B872C
// line start: 247
// line end:   254
int do_save_menu(void *gt, long parameter, enum menu_ctrl_t_duplicate_e2 ctrl);

// path: C:\kain2\game\MENU\MENUDEFS.C
// address: 0x800B8768
// line start: 274
// line end:   282
void womp_background(char *tim_path);

// path: C:\kain2\game\MENU\MENUDEFS.C
// address: 0x800B87A0
// line start: 288
// line end:   307
void play_movie_addr_800B87A0(char *name)
{ //line: 1
} //line: 20


// path: C:\kain2\game\MENU\MENUDEFS.C
// address: 0x800B87E8
// line start: 328
// line end:   329
void menudefs_reset_hack_attract_mode();

// path: C:\kain2\game\MENU\MENUDEFS.C
// address: 0x800B880C
// line start: 333
// line end:   348
void check_hack_attract();

// path: C:\kain2\game\MENU\MENUDEFS.C
// address: 0x800B887C
// line start: 366
// line end:   374
int get_volume(void *gt, enum sfx_t sfx)
{ //line: 2
	// register: $a2
	register int raw;
} //line: 8


// path: C:\kain2\game\MENU\MENUDEFS.C
// address: 0x800B88E8
// line start: 377
// line end:   385
void set_volume(enum sfx_t sfx, int cooked)
{ //line: 1
	// register: $a0
	register int raw;
} //line: 9


// path: C:\kain2\game\MENU\MENUDEFS.C
// address: 0x800B8974
// line start: 388
// line end:   408
int do_sound_adjust(void *gt, long sfxparam, enum menu_ctrl_t_duplicate_e2 ctrl)
{ //line: 1
	// register: $a1
	register int volume;
} //line: 21


// path: C:\kain2\game\MENU\MENUDEFS.C
// address: 0x800B89F0
// line start: 411
// line end:   414
void sound_item(void *gt, char *text, enum sfx_t sfx)
{ //line: 1
} //line: 1


// path: C:\kain2\game\MENU\MENUDEFS.C
// address: 0x800B8A54
// line start: 1014
// line end:   1028
int menudefs_toggle_dualshock(void *gt, long param, enum menu_ctrl_t_duplicate_e2 ctrl);

// path: C:\kain2\game\MENU\MENUDEFS.C
// address: 0x800B8AB8
// line start: 1032
// line end:   1093
int options_menu(void *gt, int index)
{ //line: 1
	static int wasDualShock;
	// register: $s0
	register int dualShock;
} //line: 62


// path: C:\kain2\game\MENU\MENUDEFS.C
// address: 0x800B8C00
// line start: 1111
// line end:   1139
int main_menu(void *gt, int index);

// path: C:\kain2\game\MENU\MENUDEFS.C
// address: 0x800B8CC4
// line start: 1143
// line end:   1155
int do_main_menu(void *gt, long param, enum menu_ctrl_t_duplicate_e2 ctrl);

// path: C:\kain2\game\MENU\MENUDEFS.C
// address: 0x800B8D18
// line start: 1159
// line end:   1208
char *flashStart()
{ //line: 1
	{ //line: 25
		// address: 152
		static int counter;
		// register: $a0
		register int intpl;
		// address: -40
		// size: 0x18
		auto int fcols[3][2];
		// register: $s2
		register int r;
		// register: $s1
		register int g;
		// register: $s0
		register int b;
	} //line: 47
} //line: 50


// path: C:\kain2\game\MENU\MENUDEFS.C
// address: 0x800B8F1C
// line start: 1211
// line end:   1238
int menudefs_main_menu(void *gt, int index);

// path: C:\kain2\game\MENU\MENUDEFS.C
// address: 0x800B8FC4
// line start: 1242
// line end:   1249
int menudefs_confirmexit_menu(void *gt, int index);

// path: C:\kain2\game\MENU\MENUDEFS.C
// address: 0x800B906C
// line start: 1253
// line end:   1276
int menudefs_pause_menu(void *gt, int index);

// path: C:\kain2\game\MENU\MENUFACE.C
// address: 0x800B91B0
// line start: 25
// line end:   34
char *NextTimAddr(char *addr, int w, int h, enum bdepth bpp)
{ //line: 2
	// register: $v0
	register long addtl;
} //line: 9


// path: C:\kain2\game\MENU\MENUFACE.C
// address: 0x800B91F8
// line start: 70
// line end:   124
void menuface_initialize()
{ //line: 1
	// register: $s6
	register char *addr;
	// register: $s2
	register char *buttonAddr;
	// register: $s5
	register int i;
	// register: $s1
	register int j;
	{ //line: 13
	} //line: 50
} //line: 55


// path: C:\kain2\game\MENU\MENUFACE.C
// address: 0x800B9324
// line start: 127
// line end:   143
void menuface_terminate()
{ //line: 1
	// register: $s3
	register int i;
	// register: $s0
	register int j;
} //line: 17


// path: C:\kain2\game\MENU\MENUFACE.C
// address: 0x800B93D0
// line start: 149
// line end:   180
void MENUFACE_ChangeStateRandomly(int index)
{ //line: 1
	// register: $s1
	// size: 0x10
	register struct menuface_t *face;
	// register: $s3
	// size: 0x10
	register struct menuface_t *lastFace;
	{ //line: 14
	} //line: 19
} //line: 32


// path: C:\kain2\game\MENU\MENUFACE.C
// address: 0x800B94F4
// line start: 184
// line end:   207
void MENUFACE_RefreshFaces()
{ //line: 1
	// register: $s3
	register int i;
	// register: $s2
	// size: 0x10
	register struct menuface_t *face;
} //line: 24


// path: C:\kain2\game\MCARD\MEMCARD.C
// address: 0x800B959C
// line start: 58
// line end:   67
int MEMCARD_IsWrongVersion(struct memcard_t *memcard)
{ //line: 2
	// register: $v0
	register int result;
} //line: 10


// path: C:\kain2\game\MCARD\MEMCARD.C
// address: 0x800B95B0
// line start: 71
// line end:   114
void load(struct memcard_t *memcard)
{ //line: 1
	// register: $s0
	// size: 0x4C
	register struct Object *object;
} //line: 44


// path: C:\kain2\game\MCARD\MEMCARD.C
// address: 0x800B967C
// line start: 117
// line end:   127
void unload(struct memcard_t *memcard);

// path: C:\kain2\game\MCARD\MEMCARD.C
// address: 0x800B96D0
// line start: 133
// line end:   134
int memcard_data_size();

// path: C:\kain2\game\MCARD\MEMCARD.C
// address: 0x800B96D8
// line start: 148
// line end:   177
int memcard_initialize(struct memcard_t *memcard, void *gt, int nblocks, void *buffer, int nbytes)
{ //line: 1
	// register: $s1
	register int header_size;
} //line: 29


// path: C:\kain2\game\MCARD\MEMCARD.C
// address: 0x800B9798
// line start: 182
// line end:   186
void memcard_end(struct memcard_t *memcard);

// path: C:\kain2\game\MCARD\MEMCARD.C
// address: 0x800B97DC
// line start: 190
// line end:   205
int maybe_start(struct memcard_t *memcard);

// path: C:\kain2\game\MCARD\MEMCARD.C
// address: 0x800B9848
// line start: 209
// line end:   214
int memcard_main_menu(void *gt, int index)
{ //line: 1
	// register: $s0
	// size: 0x10
	register struct memcard_t *memcard;
} //line: 6


// path: C:\kain2\game\MCARD\MEMCARD.C
// address: 0x800B989C
// line start: 217
// line end:   222
int memcard_pause_menu(void *gt, int index)
{ //line: 1
	// register: $s0
	// size: 0x10
	register struct memcard_t *memcard;
} //line: 6


// path: C:\kain2\game\MCARD\MEMCARD.C
// address: 0x800B98F0
// line start: 229
// line end:   232
void *gt2mcmenu(void *gt)
{ //line: 2
	// register: $v0
	// size: 0x10
	register struct memcard_t *memcard;
} //line: 2


// path: C:\kain2\game\MCARD\MEMCARD.C
// address: 0x800B9904
// line start: 235
// line end:   239
void memcard_pop(void *opaque);

// path: C:\kain2\game\MCARD\MEMCARD.C
// address: 0x800B9940
// line start: 243
// line end:   248
void memcard_start(void *opaque);

// path: C:\kain2\game\MCARD\MEMCARD.C
// address: 0x800B9984
// line start: 252
// line end:   257
void memcard_load(void *opaque);

// path: C:\kain2\game\MCARD\MEMCARD.C
// address: 0x800B99D0
// line start: 261
// line end:   262
void memcard_save(void *opaque);

// path: C:\kain2\game\MCARD\MEMCARD.C
// address: 0x800B99F0
// line start: 266
// line end:   272
void memcard_item(void *opaque, int (*fn)(), long parameter, long flags, char *text);

// path: C:\kain2\game\MCARD\MCASSERT.C
// address: 0x800B9A38
// line start: 10
// line end:   16
void mcassert(char *exp, char *file, long line);

