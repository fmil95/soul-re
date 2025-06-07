#include "Game/GAMELOOP.h"
#include "Game/SAVEINFO.h"
#include "Game/DEBUG.h"
#include "Game/RAZIEL/RAZIEL.h"
#include "Game/PSX/MAIN.h"
#include "Game/CAMERA.h"
#include "Game/SOUND.h"
#include "Game/FONT.h"
#include "Game/MATH3D.h"
#include "Game/MEMPACK.h"
#include "Game/EVENT.h"
#include "Game/STRMLOAD.h"
#include "Game/STREAM.h"
#include "Game/LIGHT3D.h"
#include "Game/INSTANCE.h"
#include "Game/STATE.h"
#include "Game/MENU/MENU.h"

typedef int (*fptr)();

DebugMenuLine levelSelectMenu[];
DebugMenuLine debugRazielMenu[];
DebugMenuLine cameraMenu[];
DebugMenuLine fogMenu[];
DebugMenuLine debugHealthSystemMenu[];
DebugMenuLine debugSpecialAbilitiesMenu[];
DebugMenuLine debugForgedAbilitiesMenu[];
DebugMenuLine debugGlyphAbilitiesMenu[];
DebugMenuLine debugImbueSoulReaverMenu[];
DebugMenuLine mainMenu[];
DebugMenuLine BossAreasMenu[];
DebugMenuLine level2SelectMenu[];
DebugMenuLine pauseMenu[];

static char pauseFormatString[20] = "-abs 256 40 -center";
static char mainFormatString[20] = "-abs 160 40 -center";
static char *the_format_string = mainFormatString;
long menuPauseSel = 0;
long debug_cameraMode = 0xC;
long debugVoiceIndex = 0;
unsigned long debugTimeMult = 0x1000;
long debugFogFar = 0x2CEC;
long debugFogNear = 0x2904;
long debugGameTime = 0x4B0;
long debugFogRed = 0;
long debugFogGrn = 0;
long debugFogBlu = 0;
long debugHealthLevel = 1;
long debugManaLevel = 1;

DebugMenuLine standardMenu[] = {
    {DEBUG_LINE_TYPE_ACTION, 0, 0, "FILL ER UP", (long *)DEBUG_FillUpHealth, 0},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "keep info between loads", &gameTrackerX.streamFlags, 0x200000},
    {DEBUG_LINE_TYPE_MENU, 0, 0, "LEVELS", (long *)levelSelectMenu, 0},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "SHORT SHORT STATS", &gameTrackerX.debugFlags, 0x04000000},
    {DEBUG_LINE_TYPE_MENU, 0, 0, "RAZIEL MENU...", (long *)debugRazielMenu, 0},
    {DEBUG_LINE_TYPE_MENU, 0, 0, "CAMERA MENU...", (long *)cameraMenu, 0},
    {DEBUG_LINE_TYPE_MENU, 0, 0, "FOG MENU...", (long *)fogMenu, (intptr_t)DEBUG_FogLoad},
    {DEBUG_LINE_TYPE_ACTION, 0, 0, "VIEW VRAM", (long *)DEBUG_SetViewVram, 0},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "Show Warp Gate Info", &gameTrackerX.debugFlags2, 0x01000000},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "Activate All WarpGates", &gameTrackerX.streamFlags, 0x400000},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "MUSIC ON", &gameTrackerX.debugFlags2, 0x1000},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine saveMenu[] = {
    {DEBUG_LINE_TYPE_MENU, 0, 0, "MAIN MENU...", (long *)standardMenu, 0},
    {DEBUG_LINE_TYPE_ACTION, 0, 0, "Save Current Game", (long *)SAVE_DebugSaveGame, 0},
    {DEBUG_LINE_TYPE_ACTION, 0, 0, "Restore Game", (long *)SAVE_LoadSaveGame, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine eventMenu[] = {
    {DEBUG_LINE_TYPE_MENU, 0, 0, "MAIN MENU...", (long *)standardMenu, 0},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "Show Event Vars", &gameTrackerX.debugFlags, 0x40000000},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "SHOW EVENT DEBUG", &gameTrackerX.debugFlags2, 0x100},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "SHOW Spline Info", &gameTrackerX.debugFlags2, 0x400000},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "SHOW GAME TIME", &gameTrackerX.debugFlags2, 0x10000},
    {DEBUG_LINE_TYPE_LONG, 0, 0x4000, "GAME TIME MULT", &gameTrackerX.multGameTime, (intptr_t)DEBUG_KeepGameTime},
    {DEBUG_LINE_TYPE_LONG, 0, 0x960, "SET GAME TIME", &debugGameTime, (intptr_t)DEBUG_SetGameTime},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "Show Warp Gate Info", &gameTrackerX.debugFlags2, 0x01000000},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "Activate All WarpGates", &gameTrackerX.streamFlags, 0x400000},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine fogMenu[] = {
    {DEBUG_LINE_TYPE_MENU, 0, 0, "MAIN MENU...", (long *)standardMenu, 0},
    {DEBUG_LINE_TYPE_LONG, -1, 0x2CEC, "FOG FAR", &debugFogFar, (intptr_t)DEBUG_UpdateFog},
    {DEBUG_LINE_TYPE_LONG, -1, 0x2CEC, "FOG NEAR", &debugFogNear, (intptr_t)DEBUG_UpdateFog},
    {DEBUG_LINE_TYPE_LONG, 0, 0xFF, "FOG RED", &debugFogRed, (intptr_t)DEBUG_UpdateFog},
    {DEBUG_LINE_TYPE_LONG, 0, 0xFF, "FOG GREEN", &debugFogGrn, (intptr_t)DEBUG_UpdateFog},
    {DEBUG_LINE_TYPE_LONG, 0, 0xFF, "FOG BLUE", &debugFogBlu, (intptr_t)DEBUG_UpdateFog},
    {DEBUG_LINE_TYPE_ACTION, 0, 0, "RESTORE FOG", (long *)DEBUG_FogRestore, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine goodiesMenu[] = {
    {DEBUG_LINE_TYPE_MENU, 0, 0, "MAIN MENU...", (long *)standardMenu, 0},
    {DEBUG_LINE_TYPE_MENU, 0, 0, "FOG MENU...", (long *)fogMenu, (intptr_t)DEBUG_FogLoad},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "SHRINK CELS", &gameTrackerX.debugFlags, 0x40},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "DECOUPLE", &gameTrackerX.decoupleGame, 1},
    {DEBUG_LINE_TYPE_LONG, 0, 2, "FRAME RATE LOCK", &gameTrackerX.frameRateLock, 0},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "IGNORE OBJECT SCRIPTS", &gameTrackerX.debugFlags, 0x10},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "C CODE", &gameTrackerX.debugFlags, 0x800},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "STOP ACTION", &gameTrackerX.debugFlags, 0x200000},
    {DEBUG_LINE_TYPE_ACTION, 0, 0, "VIEW VRAM", (long *)DEBUG_SetViewVram, 0},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "CAPTURE SCREEN", &gameTrackerX.debugFlags, 0x4000},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "SIGNAL STATS", &gameTrackerX.debugFlags, 0x800000},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "SIGNAL LOGIC VALUES", &gameTrackerX.debugFlags, 0x01000000},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "NO OPTIONAL SIGNALS", &gameTrackerX.gameFlags, 0x40},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "VV STATS", &gameTrackerX.debugFlags, 0x02000000},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "VOICE ON", &gameTrackerX.debugFlags, 0x80000},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "SHOW MUSIC VARS", &gameTrackerX.debugFlags, 0x10000000},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine debugRazielMenu[] = {
    {DEBUG_LINE_TYPE_MENU, 0, 0, "MAIN MENU...", (long *)standardMenu, 0},
    {DEBUG_LINE_TYPE_MENU, 0, 0, "HEALTH SYSTEM    ...", (long *)debugHealthSystemMenu, 0},
    {DEBUG_LINE_TYPE_MENU, 0, 0, "Special Abilities...", (long *)debugSpecialAbilitiesMenu, 0},
    {DEBUG_LINE_TYPE_MENU, 0, 0, "Forged  Abilities...", (long *)debugForgedAbilitiesMenu, 0},
    {DEBUG_LINE_TYPE_MENU, 0, 0, "Glyph   Abilities...", (long *)debugGlyphAbilitiesMenu, 0},
    {DEBUG_LINE_TYPE_MENU, 0, 0, "IMBUE SOUL REAVER...", (long *)debugImbueSoulReaverMenu, 0},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "SHIFT ANY TIME", (long *)&debugRazielFlags1, 0x50},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine debugHealthSystemMenu[] = {
    {DEBUG_LINE_TYPE_MENU, 0, 0, "MAIN MENU...", (long *)standardMenu, 0},
    {DEBUG_LINE_TYPE_MENU, 0, 0, "RAZIEL MENU...", (long *)debugRazielMenu, 0},
    {DEBUG_LINE_TYPE_LONG, 1, 4, "HEALTH LEVEL", &debugHealthLevel, (intptr_t)DEBUG_UpdateHealth},
    {DEBUG_LINE_TYPE_LONG, 1, 6, "MANA LEVEL", &debugManaLevel, (intptr_t)DEBUG_UpdateMana},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "INVINCIBLE", &gameTrackerX.debugFlags2, 0x100000},
    {DEBUG_LINE_TYPE_ACTION, 0, 0, "HURT RAZIEL", (long *)RAZIEL_DebugHurtRaziel, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine debugSpecialAbilitiesMenu[] = {
    {DEBUG_LINE_TYPE_MENU, 0, 0, "MAIN MENU...", (long *)standardMenu, 0},
    {DEBUG_LINE_TYPE_MENU, 0, 0, "RAZIEL MENU...", (long *)debugRazielMenu, 0},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "ALL", (long *)&debugRazielFlags1, 0x3F},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "PASS THROUGH BARRIERS", (long *)&debugRazielFlags1, 1},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "WALL CRAWLING", (long *)&debugRazielFlags1, 0xB},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "FORCE", (long *)&debugRazielFlags1, 0xF},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "SOUL REAVER", (long *)&debugRazielFlags1, 9},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "SWIM", (long *)&debugRazielFlags1, 0x1F},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "CONSTRICT", (long *)&debugRazielFlags1, 0x3F},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine debugForgedAbilitiesMenu[] = {
    {DEBUG_LINE_TYPE_MENU, 0, 0, "MAIN MENU...", (long *)standardMenu, 0},
    {DEBUG_LINE_TYPE_MENU, 0, 0, "RAZIEL MENU...", (long *)debugRazielMenu, 0},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "ALL", (long *)&debugRazielFlags1, 0x3FC00},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "Spectral Reaver", (long *)&debugRazielFlags1, 0x400},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "Material Reaver", (long *)&debugRazielFlags1, 0x800},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "Fire Reaver", (long *)&debugRazielFlags1, 0x8000},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine debugGlyphAbilitiesMenu[] = {
    {DEBUG_LINE_TYPE_MENU, 0, 0, "MAIN MENU...", (long *)standardMenu, 0},
    {DEBUG_LINE_TYPE_MENU, 0, 0, "RAZIEL MENU...", (long *)debugRazielMenu, 0},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "ALL", (long *)&debugRazielFlags1, 0x01FC0000},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "Sunlight Glyph", (long *)&debugRazielFlags1, 0x400000},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "Water Glyph", (long *)&debugRazielFlags1, 0x200000},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "Fire Glyph", (long *)&debugRazielFlags1, 0x800000},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "Stone Glyph", (long *)&debugRazielFlags1, 0x80000},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "Force Glyph", (long *)&debugRazielFlags1, 0x40000},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "Sound Glyph", (long *)&debugRazielFlags1, 0x100000},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "Plane Shift Glyph", (long *)&debugRazielFlags1, 0x01000000},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine debugImbueSoulReaverMenu[] = {
    {DEBUG_LINE_TYPE_MENU, 0, 0, "MAIN MENU...", (long *)standardMenu, 0},
    {DEBUG_LINE_TYPE_MENU, 0, 0, "RAZIEL MENU...", (long *)debugRazielMenu, 0},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "Hold Soul Reaver", (long *)&debugRazielFlags1, 8},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "Spectral Reaver", (long *)&debugRazielFlags2, 0x400},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "Material Reaver", (long *)&debugRazielFlags2, 0x800},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "Fire Reaver", (long *)&debugRazielFlags2, 0x8000},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine cheatsMenu[] = {
    {DEBUG_LINE_TYPE_ACTION, 0, 0, "Start Game", (long *)MAIN_StartGame, 0},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "Activate All WarpGates", &gameTrackerX.streamFlags, 0x400000},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "Disable Monsters Loading", &gameTrackerX.debugFlags2, 0x8000},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "Skip First Puppet Show", &gameTrackerX.streamFlags, 0x800000},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "Skip All Puppet Shows", &gameTrackerX.debugFlags2, 0x800000},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "Show Warp Gate Info", &gameTrackerX.debugFlags2, 0x01000000},
    {DEBUG_LINE_TYPE_MENU, 0, 0, "Main Menu", (long *)mainMenu, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine optsMenu[] = {
    {DEBUG_LINE_TYPE_FORMAT, 0, 0, mainFormatString, NULL, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine gpadConfigMenu[] = {
    {DEBUG_LINE_TYPE_FORMAT, 0, 0, mainFormatString, NULL, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine keybConfigMenu[] = {
    {DEBUG_LINE_TYPE_FORMAT, 0, 0, mainFormatString, NULL, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine demoMenu[] = {
    {DEBUG_LINE_TYPE_FORMAT, 0, 0, mainFormatString, NULL, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine mainMenu[] = {
    {DEBUG_LINE_TYPE_FORMAT, 0, 0, mainFormatString, NULL, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine cameraMenu[] = {
    {DEBUG_LINE_TYPE_MENU, 0, 0, "MAIN MENU...", (long *)standardMenu, 0},
    {DEBUG_LINE_TYPE_LONG, 0, 0x400, "Center Delay", (long *)&CameraCenterDelay, 0},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "Camera Override", &gameTrackerX.debugFlags2, 0x02000000},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine AIMenu[] = {
    {DEBUG_LINE_TYPE_MENU, 0, 0, "MAIN MENU...", (long *)standardMenu, 0},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "DISABLE MONSTER AI", &gameTrackerX.debugFlags2, 8},
    {DEBUG_LINE_TYPE_BIT, 0, 0, "MONSTER STATS", &gameTrackerX.debugFlags2, 2},
    {DEBUG_LINE_TYPE_ACTION, 0, 0, "CINEMATIC SWITCH", (long *)DEBUG_SendCinematicSwitch, 0},
    {DEBUG_LINE_TYPE_ACTION, 0, 0, "SEND CINEMATIC COMMAND", (long *)DEBUG_SendMoveTo, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine AlukaMenu[] = {
    {DEBUG_LINE_TYPE_MENU, 0, 0, "AREA MENU...", (long *)levelSelectMenu, 0},
    {DEBUG_LINE_TYPE_ACTION, 1, 0, "ALUKA 1", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 4, 0, "ALUKA 4", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 6, 0, "ALUKA 6", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 8, 0, "ALUKA 8", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 0xC, 0, "ALUKA 12", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 0x13, 0, "ALUKA 19", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 0x1B, 0, "ALUKA 27", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 0x1D, 0, "ALUKA 29", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine AshVillageMenu[] = {
    {DEBUG_LINE_TYPE_MENU, 0, 0, "AREA MENU...", (long *)levelSelectMenu, 0},
    {DEBUG_LINE_TYPE_ACTION, 1, 0, "NIGHTA 1", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 2, 0, "NIGHTA 2", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 4, 0, "NIGHTA 4", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 3, 0, "NIGHTB 3", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 5, 0, "NIGHTB 5", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine OracleMenu[] = {
    {DEBUG_LINE_TYPE_MENU, 0, 0, "AREA MENU...", (long *)levelSelectMenu, 0},
    {DEBUG_LINE_TYPE_ACTION, 3, 0, "ORACLE 3", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 5, 0, "ORACLE 5", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 0xA, 0, "ORACLE 10", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 0xD, 0, "ORACLE 13", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 0xF, 0, "ORACLE 15", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 0x11, 0, "ORACLE 17", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 0x12, 0, "ORACLE 18", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 0x16, 0, "ORACLE 22", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine PillarsMenu[] = {
    {DEBUG_LINE_TYPE_MENU, 0, 0, "AREA MENU...", (long *)levelSelectMenu, 0},
    {DEBUG_LINE_TYPE_ACTION, 3, 0, "Pillars 3", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 4, 0, "Pillars 4", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 9, 0, "Pillars 9", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 3, 0, "Tompil 3", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine SilencedCathedralMenu[] = {
    {DEBUG_LINE_TYPE_MENU, 0, 0, "AREA MENU...", (long *)levelSelectMenu, 0},
    {DEBUG_LINE_TYPE_ACTION, 1, 0, "Cathy 1", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 3, 0, "Cathy 3", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 5, 0, "Cathy 5", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 8, 0, "Cathy 8", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 0x13, 0, "Cathy 19", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 0x2A, 0, "Cathy 42", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 0x2F, 0, "Cathy 47", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 0x31, 0, "Cathy 49", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 0x44, 0, "Cathy 68", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine SkinnerMenu[] = {
    {DEBUG_LINE_TYPE_MENU, 0, 0, "AREA MENU...", (long *)levelSelectMenu, 0},
    {DEBUG_LINE_TYPE_ACTION, 1, 0, "OUT 1", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 4, 0, "OUT 4", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 7, 0, "SKINNR 7", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 1, 0, "SKINNR 1", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 9, 0, "SKINNR 9", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine StoneMenu[] = {
    {DEBUG_LINE_TYPE_MENU, 0, 0, "AREA MENU...", (long *)levelSelectMenu, 0},
    {DEBUG_LINE_TYPE_ACTION, 1, 0, "STONE 1", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 5, 0, "STONE 5", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 0xA, 0, "STONE 10", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine SunLightMenu[] = {
    {DEBUG_LINE_TYPE_MENU, 0, 0, "AREA MENU...", (long *)levelSelectMenu, 0},
    {DEBUG_LINE_TYPE_ACTION, 1, 0, "FILL 1", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 1, 0, "INTVALY 1", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 1, 0, "SUNRM 1", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 1, 0, "PISTON 1", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 1, 0, "HTORM 1", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine TombMenu[] = {
    {DEBUG_LINE_TYPE_MENU, 0, 0, "AREA MENU...", (long *)levelSelectMenu, 0},
    {DEBUG_LINE_TYPE_ACTION, 2, 0, "BOSS 2", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 1, 0, "TOMB 1", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 1, 0, "CONECTC 1", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 1, 0, "Add 1", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine WaterMenu[] = {
    {DEBUG_LINE_TYPE_MENU, 0, 0, "AREA MENU...", (long *)levelSelectMenu, 0},
    {DEBUG_LINE_TYPE_ACTION, 1, 0, "TOWER 1", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 7, 0, "TOWER 7", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine CityMenu[] = {
    {DEBUG_LINE_TYPE_MENU, 0, 0, "AREA MENU...", (long *)levelSelectMenu, 0},
    {DEBUG_LINE_TYPE_ACTION, 2, 0, "CITY 2", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 9, 0, "CITY 9", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 0xE, 0, "CITY 14", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine CliffMenu[] = {
    {DEBUG_LINE_TYPE_MENU, 0, 0, "AREA MENU...", (long *)levelSelectMenu, 0},
    {DEBUG_LINE_TYPE_ACTION, 1, 0, "CLIFF 1", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine UnderMenu[] = {
    {DEBUG_LINE_TYPE_MENU, 0, 0, "AREA MENU...", (long *)levelSelectMenu, 0},
    {DEBUG_LINE_TYPE_ACTION, 1, 0, "UNDER 1", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine MorlockMenu[] = {
    {DEBUG_LINE_TYPE_MENU, 0, 0, "AREA MENU...", (long *)levelSelectMenu, 0},
    {DEBUG_LINE_TYPE_ACTION, 1, 0, "Mrlock 1", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine HubAMenu[] = {
    {DEBUG_LINE_TYPE_MENU, 0, 0, "AREA MENU...", (long *)levelSelectMenu, 0},
    {DEBUG_LINE_TYPE_ACTION, 1, 0, "Huba 1", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 6, 0, "Huba 6", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 0xA, 0, "Huba 10", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine HubBMenu[] = {
    {DEBUG_LINE_TYPE_MENU, 0, 0, "AREA MENU...", (long *)levelSelectMenu, 0},
    {DEBUG_LINE_TYPE_ACTION, 1, 0, "Hubb 1", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine TrainingMenu[] = {
    {DEBUG_LINE_TYPE_MENU, 0, 0, "AREA MENU...", (long *)levelSelectMenu, 0},
    {DEBUG_LINE_TYPE_ACTION, 1, 0, "Train 1", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 7, 0, "Train 7", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 9, 0, "Train 9", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine DarkEdenMenu[] = {
    {DEBUG_LINE_TYPE_MENU, 0, 0, "AREA MENU...", (long *)levelSelectMenu, 0},
    {DEBUG_LINE_TYPE_ACTION, 3, 0, "Fire 3", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine AnteRoomsMenu[] = {
    {DEBUG_LINE_TYPE_MENU, 0, 0, "Boss Menu...", (long *)BossAreasMenu, 0},
    {DEBUG_LINE_TYPE_ACTION, 0xC, 0, "Skinnr 12", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 0x36, 0, "Cathy 54", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 2, 0, "Pillars 2", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 0x2E, 0, "Aluka 46", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 8, 0, "Nightb 8", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 2, 0, "Chrono 2", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine BossAreasMenu[] = {
    {DEBUG_LINE_TYPE_MENU, 0, 0, "More Areas Menu...", (long *)level2SelectMenu, 0},
    {DEBUG_LINE_TYPE_MENU, 0, 0, "Anterooms...", (long *)AnteRoomsMenu, 0},
    {DEBUG_LINE_TYPE_ACTION, 9, 0, "Skinnr 9", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 0x37, 0, "Cathy 55", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 1, 0, "Pillars 1", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 6, 0, "Aluka 6", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 3, 0, "Nightb 3", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ACTION, 1, 0, "Chrono 1", (long *)DEBUG_LevelSelectNew, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine level2SelectMenu[] = {
    {DEBUG_LINE_TYPE_MENU, 0, 0, "AREA MENU...", (long *)levelSelectMenu, 0},
    {DEBUG_LINE_TYPE_MENU, 0, 0, "City...", (long *)CityMenu, 0},
    {DEBUG_LINE_TYPE_MENU, 0, 0, "Cliff...", (long *)CliffMenu, 0},
    {DEBUG_LINE_TYPE_MENU, 0, 0, "Under...", (long *)UnderMenu, 0},
    {DEBUG_LINE_TYPE_MENU, 0, 0, "Morlock...", (long *)MorlockMenu, 0},
    {DEBUG_LINE_TYPE_MENU, 0, 0, "HubA...", (long *)HubAMenu, 0},
    {DEBUG_LINE_TYPE_MENU, 0, 0, "HubB...", (long *)HubBMenu, 0},
    {DEBUG_LINE_TYPE_MENU, 0, 0, "Training...", (long *)TrainingMenu, 0},
    {DEBUG_LINE_TYPE_MENU, 0, 0, "Dark Eden...", (long *)DarkEdenMenu, 0},
    {DEBUG_LINE_TYPE_MENU, 0, 0, "Boss Areas...", (long *)BossAreasMenu, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine levelSelectMenu[] = {
    {DEBUG_LINE_TYPE_MENU, 0, 0, "MAIN MENU...", (long *)standardMenu, 0},
    {DEBUG_LINE_TYPE_ACTION, 0, 0, "RELOAD CURRENT LEVEL", (long *)DEBUG_ReloadCurrentLevel, 0},
    {DEBUG_LINE_TYPE_MENU, 0, 0, "Aluka...", (long *)AlukaMenu, 0},
    {DEBUG_LINE_TYPE_MENU, 0, 0, "Ash Village...", (long *)AshVillageMenu, 0},
    {DEBUG_LINE_TYPE_MENU, 0, 0, "Oracle's Cave...", (long *)OracleMenu, 0},
    {DEBUG_LINE_TYPE_MENU, 0, 0, "Pillars...", (long *)PillarsMenu, 0},
    {DEBUG_LINE_TYPE_MENU, 0, 0, "Silenced Cathedral...", (long *)SilencedCathedralMenu, 0},
    {DEBUG_LINE_TYPE_MENU, 0, 0, "Skinner...", (long *)SkinnerMenu, 0},
    {DEBUG_LINE_TYPE_MENU, 0, 0, "Stone Glyph...", (long *)StoneMenu, 0},
    {DEBUG_LINE_TYPE_MENU, 0, 0, "Sunlight Glyph... ", (long *)SunLightMenu, 0},
    {DEBUG_LINE_TYPE_MENU, 0, 0, "Tomb Of Seven...", (long *)TombMenu, 0},
    {DEBUG_LINE_TYPE_MENU, 0, 0, "Water Glyph...", (long *)WaterMenu, 0},
    {DEBUG_LINE_TYPE_MENU, 0, 0, "More Areas..", (long *)level2SelectMenu, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine mcardMenu[] = {
    {DEBUG_LINE_TYPE_FORMAT, 0, 0, mainFormatString, NULL, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine mcardSaveMenu[] = {
    {DEBUG_LINE_TYPE_FORMAT, 0, 0, pauseFormatString, NULL, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine pauseSaveMenu[] = {
    {DEBUG_LINE_TYPE_FORMAT, 0, 0, pauseFormatString, NULL, 0},
    {DEBUG_LINE_TYPE_MENU, 0, 0, "Back...", (long *)pauseMenu, 0},
    {DEBUG_LINE_TYPE_ACTION, 0, 0, "Save Current Game", (long *)SAVE_DebugSaveGame, 0},
    {DEBUG_LINE_TYPE_ACTION, 0, 0, "Restore Game", (long *)SAVE_LoadSaveGame, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
DebugMenuLine pauseMenu[] = {
    {DEBUG_LINE_TYPE_FORMAT, 0, 0, pauseFormatString, NULL, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
    {DEBUG_LINE_TYPE_ENDLIST, 0, 0, "", &gameTrackerX.debugFlags, 0},
};
static debug_dispatch_t debug_dispatch_table[] = {
    {DEBUG_LINE_TYPE_BIT, handle_line_type_bit},
    {DEBUG_LINE_TYPE_LONG, handle_line_type_long},
    {DEBUG_LINE_TYPE_ACTION, handle_line_type_action},
    {DEBUG_LINE_TYPE_ACTION_WITH_LINE, handle_line_type_action_with_line},
    {DEBUG_LINE_TYPE_MENU, handle_line_type_menu},
};

/*Unused*/ static char D_800CFF80[] = "dbg_cam_";
/*Unused*/ static char D_800CFF8C[] = "box_____";
/*Unused*/ static char D_800CFF98[] = "sphere__";

DebugMenuLine *currentMenu = standardMenu;
static long debugMenuChoice = 0;
static long cheatCodeLastCommand = 0;
static long CheatKeyMap[10] = {4, 8, 2, 1, 0x10, 0x40, 0x80, 0xC00, 0x400, 0x800};
static GameCheat GameCheats[23] = {
    {{2, 4, 7, 1, 1, 2, 2, 0, 4, 2, 1, 0}, 0xB, 0},
    {{3, 3, 2, 1, 1, 0, 5, 1, 0, 2, 0, 0}, 0xA, 0},
    {{2, 3, 1, 3, 2, 0, 5, 1, 2, 0, 0, 0}, 0x9, 0},
    {{6, 1, 3, 3, 4, 0, 0, 1, 3, 0, 0, 0}, 0x9, 0},
    {{2, 3, 1, 1, 5, 3, 3, 2, 0, 0, 0, 0}, 0x8, 0},
    {{6, 5, 1, 4, 0, 0, 1, 3, 0, 0, 0, 0}, 0x8, 0},
    {{0, 5, 0, 1, 1, 3, 3, 0, 0, 0, 0, 0}, 0x8, 0},
    {{4, 1, 2, 1, 3, 4, 0, 0, 0, 0, 0, 0}, 0x7, 0},
    {{2, 5, 5, 0, 1, 4, 3, 0, 0, 0, 0, 0}, 0x7, 0},
    {{3, 5, 2, 1, 5, 0, 3, 0, 0, 0, 0, 0}, 0x7, 0},
    {{2, 5, 3, 0, 2, 1, 1, 0, 0, 0, 0, 0}, 0x7, 0},
    {{1, 1, 2, 5, 3, 3, 2, 0, 0, 0, 0, 0}, 0x7, 0},
    {{3, 3, 1, 3, 4, 7, 1, 0, 0, 0, 0, 0}, 0x7, 0},
    {{2, 5, 3, 0, 3, 0, 0, 0, 0, 0, 0, 0}, 0x6, 0},
    {{1, 6, 0, 4, 3, 2, 0, 0, 0, 0, 0, 0}, 0x6, 0},
    {{1, 5, 2, 3, 2, 3, 0, 0, 0, 0, 0, 0}, 0x6, 0},
    {{1, 1, 0, 4, 1, 2, 0, 0, 0, 0, 0, 0}, 0x6, 0},
    {{4, 2, 7, 1, 3, 2, 0, 0, 0, 0, 0, 0}, 0x6, 0},
    {{0, 1, 5, 0, 1, 0, 0, 0, 0, 0, 0, 0}, 0x6, 0},
    {{0, 5, 3, 3, 2, 0, 0, 0, 0, 0, 0, 0}, 0x5, 0},
    {{4, 1, 2, 5, 3, 0, 0, 0, 0, 0, 0, 0}, 0x5, 0},
    {{2, 0, 4, 2, 3, 0, 0, 0, 0, 0, 0, 0}, 0x5, 0},
    {{2, 5, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0}, 0x5, 0},
};

DebugMenuLine *the_previous_menu = NULL;
static int cem_x_base = 0x28;
static int cem_y_base = 0x14;
static int cem_cursor_width = 0x14;
static int cem_line_width = 0xF0;
static int cem_line_leading = 0xC;
static int cem_item_leading = 0x10;

DebugMenuLine debugSoundMenu[0]; // TODO: decls.h has an array of size 1 for this variable (gp_rel issue)

short debugOrgFogNear;

unsigned char debugOrgFogRed;

unsigned char debugOrgFogGrn;

unsigned char debugOrgFogBlu;

short debugOrgFogFar;

DebugMenuLine statsMenu[1];

unsigned long debugRazielFlags1;

unsigned long debugRazielFlags2;

unsigned long debugRazielFlags3;

void DEBUG_SetGameTime(long *var)
{
    GAMELOOP_SetGameTime(*var);
}

void DEBUG_KeepGameTime(long *var)
{
    long curTOD;

    (void)var;

    curTOD = gameTrackerX.timeOfDay;

    DEBUG_SetGameTime(&curTOD);
}

void DEBUG_UpdateFog(long *var)
{
    StreamUnit *currentUnit;

    (void)var;

    currentUnit = FindStreamUnitFromLevel(gameTrackerX.level);

    gameTrackerX.level->holdFogFar = currentUnit->TargetFogFar = gameTrackerX.level->fogFar = (unsigned short)debugFogFar;
    gameTrackerX.level->holdFogNear = currentUnit->TargetFogNear = gameTrackerX.level->fogNear = (unsigned short)debugFogNear;

    *(int *)&gameTrackerX.level->backColorR = currentUnit->FogColor = (debugFogBlu << 16) | (debugFogGrn << 8) | debugFogRed;

    LIGHT_CalcDQPTable(gameTrackerX.level);
}

void DEBUG_UpdateHealth(long *var)
{
    (void)var;
    RAZIEL_DebugHealthSetScale(debugHealthLevel);
}

void DEBUG_UpdateMana(long *var)
{
    (void)var;
    RAZIEL_DebugManaSetMax(debugManaLevel);
}

void DEBUG_FillUpHealth(long *var)
{
    (void)var;
    RAZIEL_DebugHealthFillUp();

    RAZIEL_DebugManaFillUp();
}

void DEBUG_FogLoad()
{
    StreamUnit *currentUnit;

    currentUnit = FindStreamUnitFromLevel(gameTrackerX.level);

    debugOrgFogFar = debugFogFar = currentUnit->TargetFogFar;
    debugOrgFogNear = debugFogNear = currentUnit->TargetFogNear;

    debugOrgFogBlu = debugFogBlu = (currentUnit->FogColor >> 16) & 0xFF;
    debugOrgFogGrn = debugFogGrn = (currentUnit->FogColor >> 8) & 0xFF;
    debugOrgFogRed = debugFogRed = (currentUnit->FogColor >> 0) & 0xFF;
}

void DEBUG_FogRestore()
{
    debugFogFar = debugOrgFogFar;
    debugFogNear = debugOrgFogNear;

    debugFogRed = debugOrgFogRed;
    debugFogGrn = debugOrgFogGrn;
    debugFogBlu = debugOrgFogBlu;

    DEBUG_UpdateFog(NULL);
}

void DEBUG_SendCinematicSwitch()
{
    gameTrackerX.debugFlags2 ^= 0x200;

    if ((gameTrackerX.debugFlags2 & 0x200))
    {
        INSTANCE_Broadcast(NULL, 0xE, 0x4000E, 1);
    }
    else
    {
        INSTANCE_Broadcast(NULL, 0xE, 0x4000E, 0);
    }
}

void DEBUG_SendMoveTo()
{
    INSTANCE_Broadcast(NULL, 0xE, 0x4000C, SetPositionData((gameTrackerX.playerInstance->position.x + (rand() & 2047)) - 1024, (gameTrackerX.playerInstance->position.y + (rand() & 2047)) - 1024, gameTrackerX.playerInstance->position.z));
}

void process_cheat_codes(GameTracker *gt, long *ctrl)
{
    int i;
    GameCheat *cheat;
    unsigned long padPress;
    unsigned long padOn;

    (void)gt;

    padOn = ctrl[0];
    padPress = ctrl[1];

    if ((cheatCodeLastCommand == 0) && (padPress != 0))
    {
        int cheatTriggered;

        cheatTriggered = 0;

        if ((padOn & 0x300))
        {
            padOn &= ~0x300;

            for (i = 0; i < 23; i++)
            {
                unsigned long keyMask;

                cheat = &GameCheats[i];

                keyMask = CheatKeyMap[cheat->cheatKeys[cheat->cheatStage]];

                if (((padPress & keyMask)) && (!(padOn & ~keyMask)))
                {
                    cheat->cheatStage++;
                }
                else
                {
                    cheat->cheatStage = 0;
                }

                if (cheat->cheatStage == cheat->cheatLen)
                {
                    cheat->cheatStage = 0;

                    cheatTriggered = 1;

                    switch (i)
                    {
                    case 13:
                        DEBUG_FillUpHealth(&debugHealthLevel);
                        break;
                    case 14:
                        debugHealthLevel++;

                        if (debugHealthLevel >= 5)
                        {
                            debugHealthLevel = 4;
                        }

                        DEBUG_UpdateHealth(&debugHealthLevel);
                        break;
                    case 15:
                        debugHealthLevel = 4;

                        DEBUG_UpdateHealth(&debugHealthLevel);
                        break;
                    case 19:
                        RAZIEL_DebugHurtRaziel(cheat);
                        break;
                    case 16:
                        RAZIEL_DebugManaFillUp(cheat);
                        break;
                    case 7:
                        debugManaLevel = 6;

                        DEBUG_UpdateMana(&debugManaLevel);
                        break;
                    case 4:
                        debugRazielFlags1 |= 0x20;
                    case 9:
                        debugRazielFlags1 |= 0x10;
                    case 0:
                        debugRazielFlags1 |= 0x8;
                    case 18:
                        debugRazielFlags1 |= 0x4;
                    case 17:
                        debugRazielFlags1 |= 0x2;
                    case 8:
                        debugRazielFlags1 |= 0x1;
                        break;
                    case 2:
                        debugRazielFlags1 |= 0x8000;
                        break;
                    case 20:
                        debugRazielFlags2 |= 0x8000;
                        break;
                    case 3:
                        debugRazielFlags2 |= 0x10000;
                        break;
                    case 5:
                        debugRazielFlags2 |= 0x1000;
                        break;
                    case 21:
                        debugRazielFlags3 |= 0x40000;
                        break;
                    case 10:
                        debugRazielFlags3 |= 0x80000;
                        break;
                    case 11:
                        debugRazielFlags3 |= 0x100000;
                        break;
                    case 22:
                        debugRazielFlags3 |= 0x200000;
                        break;
                    case 12:
                        debugRazielFlags3 |= 0x400000;
                        break;
                    case 6:
                        debugRazielFlags3 |= 0x800000;
                        break;
                    case 1:
                        debugRazielFlags1 |= 0x5F;
                        break;
                    }
                }
            }

            if (cheatTriggered != 0)
            {
                SndPlayVolPan(21, 80, 64, 50);
            }
        }
    }

    cheatCodeLastCommand = padPress;
}

void DEBUG_Process(GameTracker *gameTracker)
{
    long *controlCommand;
    long oldFlags;
    long oldFlags2;

    oldFlags = gameTracker->debugFlags;
    oldFlags2 = gameTracker->debugFlags2;

    controlCommand = &gameTracker->controlCommand[0][0];

    if (gameTracker->gameMode == 0)
    {
        if ((oldFlags & 0x8))
        {
            DEBUG_ProcessSecondController(gameTracker);
        }

        if (gameTracker->cheatMode == 1)
        {
            DEBUG_ProcessCheat(gameTracker);
        }
    }
    else if (gameTracker->gameMode == 4)
    {
        DEBUG_Menu(gameTracker);

        if (((gameTracker->debugFlags2 & 0x40000)) && (!(oldFlags2 & 0x40000)))
        {
            gameTracker->debugFlags |= 0x8;
        }

        if ((!(gameTracker->debugFlags2 & 0x40000)) && ((oldFlags2 & 0x40000)))
        {
            gameTracker->debugFlags &= ~0x8;
        }

        if (((gameTracker->debugFlags & 0x8)) && (!(oldFlags & 0x8)))
        {
            theCamera.core.debugPos.x = theCamera.core.position.x;
            theCamera.core.debugPos.y = theCamera.core.position.y;
            theCamera.core.debugPos.z = theCamera.core.position.z;

            theCamera.core.debugRot.x = theCamera.core.rotation.x;
            theCamera.core.debugRot.y = theCamera.core.rotation.y;
            theCamera.core.debugRot.z = theCamera.core.rotation.z;
        }
    }
    else if (gameTracker->gameMode == 6)
    {
        process_cheat_codes(gameTracker, controlCommand);

        DEBUG_Menu(gameTracker);
    }
    else if (gameTracker->gameMode == 7)
    {
        DEBUG_ViewVram(gameTracker);
    }

    if (((gameTracker->debugFlags & 0x4000)) && ((controlCommand[1] & 0x400)))
    {
        DEBUG_CaptureScreen(gameTracker);
    }
}

void DEBUG_Draw(GameTracker *gameTracker, unsigned long **ot)
{
    (void)ot;

    if ((gameTracker->gameMode == 0) || (gameTracker->cheatMode == 1) || (gameTracker->gameMode == 4))
    {
        DEBUG_DisplayStatus(gameTracker);
    }
}

long DEBUG_MenuCountLength(DebugMenuLine *menu)
{
    int length;
    DebugMenuLine *curLine;

    for (length = 0, curLine = menu; curLine != NULL; curLine++, length++)
    {
        if (curLine->type == DEBUG_LINE_TYPE_ENDLIST)
        {
            break;
        }
    }

    return length;
}

void DEBUG_ExitMenus()
{
    currentMenu[DEBUG_MenuCountLength(currentMenu)].lower = debugMenuChoice;

    if (gameTrackerX.sound.gMusicOn == 0)
    {
        SOUND_MusicOff();

        SOUND_ShutdownMusic();
    }
    else
    {
        SOUND_MusicOn();
    }

    if (gameTrackerX.sound.gSfxOn == 0)
    {
        SOUND_SfxOff();
    }
    else
    {
        SOUND_SfxOn();
    }
}

DebugMenuLine *get_last_menu_line(DebugMenuLine *line)
{
    while (line->type != DEBUG_LINE_TYPE_ENDLIST)
    {
        line++;
    }

    return line;
}

int num_menu_items(DebugMenuLine *menu)
{
    int nitems;

    nitems = 0;

    while (menu->type != DEBUG_LINE_TYPE_ENDLIST)
    {
        nitems++;
        menu++;
    }

    return nitems;
}

void maybe_change_menu_choice(GameTracker *gt, DebugMenuLine *menu)
{
    long *command;
    int choice;
    int nitems;
    int incr;
    int temp; // not from decls.h

    command = gt->controlCommand[0];

    choice = debugMenuChoice;

    nitems = num_menu_items(menu);

    if (!((unsigned long)command[1] & 0x1))
    {
        incr = (unsigned long)command[1] >> 1;

        incr &= 0x1;
    }
    else
    {
        incr = -1;
    }

    if ((incr != 0) && (choice >= 0))
    {
        temp = debugMenuChoice;

        while (1)
        {
            choice = (choice + nitems + incr) % nitems;

            if (choice != temp)
            {
                if (menu[choice].type >= DEBUG_LINE_TYPE_FORMAT)
                {
                    continue;
                }

                if (choice != debugMenuChoice)
                {
                    SndPlay(5);

                    debugMenuChoice = choice;
                }
            }

            break;
        }
    }
}

void handle_line_type_long(GameTracker *gt, DebugMenuLine *line)
{
    long *command;
    int incr;

    command = &gt->controlCommand[0][0];

    if ((command[1] & 0xC))
    {
        if ((gt->controlCommand[0][0] & 0x400))
        {
            incr = 10;
        }
        else if ((gt->controlCommand[0][0] & 0x800))
        {
            incr = 100;
        }
        else if ((gt->controlCommand[0][0] & 0x200))
        {
            incr = 1000;
        }
        else
        {
            incr = 1;
        }

        if ((command[1] & 0x4))
        {
            incr = -incr;
        }

        *line->var_address += incr;

        if (*line->var_address < line->lower)
        {
            *line->var_address = line->lower;
        }

        if (*line->var_address > line->upper)
        {
            *line->var_address = line->upper;
        }

        if (line->bit_mask != 0)
        {
            ((fptr)line->bit_mask)(line->var_address);
        }
    }
}

void handle_line_type_bit(GameTracker *gt, DebugMenuLine *line)
{
    if ((gt->controlCommand[0][1] & 0x4))
    {
        *line->var_address |= line->bit_mask;
    }
    else if ((gt->controlCommand[0][1] & 0x8))
    {
        *line->var_address &= ~line->bit_mask;
    }
}

void handle_line_type_action(GameTracker *gt, DebugMenuLine *line)
{
    int ok;

    (void)ok;

    if ((gt->controlCommand[0][1] & 0x80))
    {
        ((fptr)line->var_address)();
    }
}

void handle_line_type_action_with_line(GameTracker *gt, DebugMenuLine *line)
{
    option_ctrl_t ctrl;

    if ((gt->controlCommand[0][1] & 0x80))
    {
        ctrl = option_ctrl_select;
    }
    else if ((gt->controlCommand[0][1] & 0x4))
    {
        ctrl = option_ctrl_left;
    }
    else if ((gt->controlCommand[0][1] & 0x8))
    {
        ctrl = option_ctrl_right;
    }
    else
    {
        ctrl = option_ctrl_none;
    }

    if (ctrl != option_ctrl_none)
    {
        ((fptr)line->var_address)(gt, line);
    }
}

void handle_line_type_menu(GameTracker *gt, DebugMenuLine *line)
{
    int ok;

    (void)ok;

    if ((gt->controlCommand[0][1] & 0x80))
    {
        if (line->bit_mask != 0)
        {
            ((fptr)line->bit_mask)();
        }

        get_last_menu_line(line)->lower = debugMenuChoice;

        the_previous_menu = currentMenu;

        currentMenu = (DebugMenuLine *)line->var_address;

        debugMenuChoice = get_last_menu_line(currentMenu)->lower;
    }
}

void process_menu_line(GameTracker *gt, DebugMenuLine *menu)
{
    DebugMenuLine *line;
    debug_dispatch_t *dispatch;

    line = &menu[debugMenuChoice];

    if (line->type < DEBUG_LINE_TYPE_ENDLIST)
    {
        dispatch = &debug_dispatch_table[line->type];

        if (dispatch->fn != NULL)
        {
            dispatch->fn(gt, line);
        }
    }
}

int pre_process_functions(GameTracker *gt, DebugMenuLine *menu)
{
    (void)menu;

    if (gt->playerInstance != NULL)
    {
        gt->playerInstance->flags |= 0x100;
    }

    return 0;
}

void post_process_functions(GameTracker *gt, DebugMenuLine *menu)
{
    if (menu == debugSoundMenu)
    {
        SOUND_SetMusicVolume(-1);
        SOUND_SetSfxVolume(-1);
        SOUND_SetVoiceVolume(-1);

        if ((gt->debugFlags & 0x80000))
        {
            gt->sound.gVoiceOn = 1;
        }
        else
        {
            gt->sound.gVoiceOn = 0;
        }

        if ((gt->debugFlags2 & 0x1000))
        {
            gt->sound.gMusicOn = 1;
        }
        else
        {
            gt->sound.gMusicOn = 0;
        }

        if ((gt->debugFlags2 & 0x2000))
        {
            gt->sound.gSfxOn = 1;
        }
        else
        {
            gt->sound.gSfxOn = 0;
        }
    }
}

void set_debug_leading()
{
    cem_line_leading = 10;
    cem_item_leading = 12;
}

void set_user_leading()
{
    cem_line_leading = 12;
    cem_item_leading = 16;
}

int isdigit(char c)
{
    return (c >= '0') && (c <= '9');
}

void adjust_format(char *str, debug_format_t *adj)
{
    while (*str != 0)
    {
        char *p;
        int x;
        int y;

        if ((strncmp(str, "-abs ", strlen("-abs ")) == 0) || (strncmp(str, "-rel ", strlen("-rel ")) == 0))
        {
            p = str + 5;
            x = 0;
            y = 0;
            while (isdigit(*p))
            {
                x = (x * 10) + *p - '0';
                p++;
            }
            if (*p != 0)
            {
                p++;
            }
            while (isdigit(*p))
            {
                y = (y * 10) + *p - '0';
                p++;
            }
            if (*p != 0)
            {
                p++;
            }
            if (strncmp(str, "-abs ", strlen("-abs ")) == 0)
            {
                adj->xpos = x;
                adj->ypos = y;
            }
            else
            {
                adj->xpos = adj->xpos + x;
                adj->ypos = adj->ypos + y;
            }
            str = p;
            continue;
        }
        if (strncmp(str, "-center", strlen("-center")) != 0)
        {
            printf("unknown format control: %s\n", str);
            return;
        }
        adj->is_centered = 1;
        str += 7;
    }
}

char *find_eol(char *text)
{
    for (; *text != '\0'; text++)
    {
        if (*text == '\n')
        {
            break;
        }
    }

    return text;
}

void draw_menu_item(GameTracker *gt, debug_format_t *fmt, char *text)
{
    (void)gt;

    while (1)
    {
        char *eol;
        char c;

        eol = find_eol(text);

        c = eol[0];

        eol[0] = 0;

        if (fmt->is_centered != 0)
        {
            FONT_SetCursor(fmt->xpos - (FONT_GetStringWidth(text) >> 1), fmt->ypos);
        }
        else
        {
            FONT_SetCursor(fmt->xpos, fmt->ypos);
        }

        if (currentMenu->type != DEBUG_LINE_TYPE_FORMAT)
        {
            FONT_Print(text);
        }
        else
        {
            FONT_Print2(text);
        }

        if (c == 0)
        {
            break;
        }

        text = &eol[1];

        eol[0] = c;

        fmt->ypos += cem_line_leading;
    }

    fmt->ypos += cem_item_leading;
}

void draw_menu(GameTracker *gt, DebugMenuLine *menu)
{
    debug_format_t fmt2; // not from decls.h
    debug_format_t fmt;
    int i;

    fmt.xpos = cem_x_base;
    fmt.ypos = cem_y_base;
    fmt.is_centered = 0;

    fmt2 = fmt;

    if (menu[0].type == DEBUG_LINE_TYPE_FORMAT)
    {
        menu[0].text = the_format_string;
        set_user_leading();
    }
    else
    {
        set_debug_leading();
    }

    for (i = 0; menu[i].type != DEBUG_LINE_TYPE_ENDLIST; i++)
    {
        int xpos = fmt2.xpos;
        int ypos = fmt2.ypos;

        if (menu[i].type == DEBUG_LINE_TYPE_FORMAT)
        {
            adjust_format(menu[i].text, &fmt2);
        }
        else
        {
            if (debugMenuChoice == i)
            {
                if (fmt2.is_centered != 0)
                {
                    FONT_SetCursor((xpos - (cem_line_width >> 1)) - cem_cursor_width, ypos);
                }
                else
                {
                    FONT_SetCursor(xpos - cem_cursor_width, ypos);
                }
                FONT_Print(">");
            }

            draw_menu_item(gt, &fmt2, menu[i].text);

            if (fmt2.is_centered != 0)
            {
                FONT_SetCursor(xpos + (cem_line_width >> 1), ypos);
            }
            else
            {
                FONT_SetCursor(xpos + cem_line_width, ypos);
            }
            switch (menu[i].type)
            {
            case DEBUG_LINE_TYPE_BIT:
                if ((menu[i].var_address[0] & menu[i].bit_mask) == menu[i].bit_mask)
                {
                    FONT_Print("YES");
                }
                else
                {
                    FONT_Print("NO");
                }
                break;
            case DEBUG_LINE_TYPE_LONG:
                FONT_Print("%d", menu[i].var_address[0]);
                break;
            default:
                break;
            }
        }
    }
}

void DEBUG_Menu(GameTracker *gt)
{
    DebugMenuLine *menu;
    int choice;

    menu = currentMenu;
    choice = debugMenuChoice;

    if (menu == mainMenu || menu == pauseMenu)
    {
        menu_process(gt->menu);
    }
    else
    {
        if (pre_process_functions(gt, menu) == 0)
        {
            while (menu[debugMenuChoice].type >= DEBUG_LINE_TYPE_FORMAT)
            {
                debugMenuChoice++;
            }

            draw_menu(gt, menu);
            maybe_change_menu_choice(gt, menu);

            if (debugMenuChoice == choice)
            {
                process_menu_line(gt, menu);

                if (currentMenu == menu)
                {
                    post_process_functions(gt, menu);
                }
            }
        }
    }
}

void DEBUG_DisplayStatus(GameTracker *gameTracker)
{
    int deg;
    long numberInQueue;

    STREAM_GetLevelWithID(gameTracker->playerInstance->currentStreamUnitID);

    if ((gameTracker->debugFlags & 0x40000000))
    {
        EVENT_PrintVars();
    }

    if ((gameTracker->debugFlags & 0x4000004))
    {
        if ((gameTracker->debugFlags & 0x4000000))
        {
            FONT_Print("$@EF\n FRTE %d ", (1000000 / gameTracker->totalTime) + 1);
            FONT_Print(" INS  %d", gameTracker->visibleInstances);
            FONT_Print("/%d\n", gameTracker->instanceList->numInstances + 1);
        }
        else
        {
            FONT_Print("$@KG\n FRTE %d\n", (1000000 / gameTracker->totalTime) + 1);
        }

        FONT_Print(" Focus XYZ(%d,%d,%d)\n", theCamera.focusInstance->position.x, theCamera.focusInstance->position.y, theCamera.focusInstance->position.z);

        if ((gameTracker->debugFlags & 0x4))
        {
            if (gameTracker->idleTime != 0)
            {
                FONT_Print(" IDLE %d PCT\n", (gameTracker->idleTime * 100) / 33333);
            }
            else
            {
                FONT_Print(" IDLE ZERO\n");
            }

            FONT_Print(" DRAW %d\n", gameTracker->drawTime);
            FONT_Print(" Far Plane =%d\n", theCamera.core.farPlane);
            FONT_Print(" Fog Near = %d Fog Far = %d\n", gameTracker->level->fogNear, gameTracker->level->fogFar);
            FONT_Print("Military Time %04d\n", gameTrackerX.timeOfDay);
        }

        FONT_Print(" FMEM %d  FreeSaveMem %d\n", MEMPACK_ReportFreeMemory(), SAVE_SizeOfFreeSpace());
        FONT_Print(" AREA DRM = %s\n", gameTracker->baseAreaName);

        deg = theCamera.core.rotation.x;

        if (deg > 2048)
        {
            deg = (4096 - deg);
        }
        else
        {
            deg = -deg;
        }

        deg = (deg * 360) / 4096;

        FONT_Print(" CAM TILT %d DIST %d\n", deg, theCamera.targetFocusDistance);
    }

    if ((gameTracker->debugFlags < 0) && (STREAM_IsCdBusy(&numberInQueue) != 0))
    {
        FONT_Print("Loading From CD: In Queue(%d)\n", numberInQueue);
    }
}

void DEBUG_DrawShrinkCels(unsigned long **polyAddr)
{
    (void)polyAddr;
}

void DEBUG_ContinueGame()
{
    gameTrackerX.gameFlags |= 0x40000000;
}

void DEBUG_ExitGame()
{
    SOUND_StopAllSound();

    gameTrackerX.levelChange = 1;

    gameTrackerX.gameMode = 0;

    gameTrackerX.levelDone = 2;

    gameTrackerX.gameFlags |= 0x1;
}

void DEBUG_ReloadCurrentLevel()
{
    if (gameTrackerX.levelChange == 0)
    {
        if ((gameTrackerX.streamFlags & 0x200000))
        {
            SAVE_SaveGame();
        }

        gameTrackerX.gameFlags |= 0x1;

        SOUND_ResetAllSound();

        gameTrackerX.levelChange = 1;
        gameTrackerX.levelDone = 4;
    }

    gameTrackerX.gameMode = 0;
}

void DEBUG_LevelSelectNew()
{
    char *name;
    short number;
    char *p;
    char saveChar;

    saveChar = 0;

    p = currentMenu[debugMenuChoice].text;

    number = (short)currentMenu[debugMenuChoice].lower;

    name = p;

    for (; *p != '\0'; p++)
    {
        if ((*p == ' ') || (*p == '\t') || (*p == '\n'))
        {
            saveChar = *p;

            *p = 0;
            break;
        }
    }

    if ((gameTrackerX.streamFlags & 0x200000))
    {
        SAVE_SaveGame();
    }

    GAMELOOP_RequestLevelChange(name, number, &gameTrackerX);

    gameTrackerX.levelDone = 4;

    if (saveChar != 0)
    {
        *p = saveChar;
    }

    gameTrackerX.gameMode = 0;
}

void DEBUG_SetViewVram()
{
    gameTrackerX.gameMode = 7;
}

void DEBUG_EndViewVram(GameTracker *gameTracker)
{
    (void)gameTracker;
    SetDefDispEnv(&disp[0], 0, 0, 512, 240);
    SetDefDispEnv(&disp[1], 0, 256, 512, 240);
}

/*Unused*/ short AXIS_LENGTH = 0x012C;
/*Unused*/ static char D_800D02C8[] = "hub";
/*Unused*/ static char D_800D02CC[] = "\nSound RAM usage...\n\n";
/*Unused*/ static char D_800D02E4[] = "Sfx ID %3d loaded  Ref count=%d  Using wave ID %3d  \n";
/*Unused*/ static char D_800D031C[] = "Sfx ID %3d **LOAD ERROR**\n";
/*Unused*/ static char D_800D0338[] = "\n%ld sfx loaded (%ld waves) using %ld bytes SRAM\nFree Sram:%ld  Largest Free Sram Block:%ld\n";

void DEBUG_ViewVram(GameTracker *gameTracker)
{
    long *controlCommand;
    static int xPos = 0;
    static int yPos = 0;

    controlCommand = &gameTracker->controlCommand[0][0];

    if (((controlCommand[1] & 0x1)) && (yPos >= 0))
    {
        yPos -= 32;
    }

    if (((controlCommand[1] & 0x2)) && (yPos < 272))
    {
        yPos += 32;
    }

    if (((controlCommand[1] & 0x4)) && (xPos >= 0))
    {
        xPos -= 32;
    }

    if (((controlCommand[1] & 0x8)) && (xPos < 512))
    {
        xPos += 32;
    }

    SetDefDispEnv(&disp[0], xPos, yPos, 512, 240);
    SetDefDispEnv(&disp[1], xPos, yPos, 512, 240);

    gameTracker->playerInstance->flags |= 0x100;
}

void DEBUG_CaptureScreen(GameTracker *gameTracker)
{
    (void)gameTracker;
}

void DEBUG_PageFlip()
{
    POLY_F4 poly;
    unsigned long **drawot;

    DrawSync(0);

    VSync(0);
    VSync(0);
    VSync(0);
    VSync(0);
    VSync(0);

    DrawSyncCallback(NULL);

    VSyncCallback(NULL);

    ResetPrimPool();

    drawot = gameTrackerX.drawOT;

    gameTrackerX.drawPage = 0;

    PutDrawEnv(draw);

    ClearOTagR((u_long *)drawot, 3072);

    DrawSync(0);

    setPolyF4(&poly);

    setRGB0(&poly, 32, 32, 32);
    setXY4(&poly, 0, 14, 511, 14, 0, (short)fontTracker.font_ypos - 2, 511, (short)fontTracker.font_ypos - 2);

    DrawPrim(&poly);

    FONT_Flush();

    DrawOTag((u_long *)drawot);
    DrawSync(0);

    PutDispEnv((DISPENV *)gameTrackerX.disp);
}

void DEBUG_FatalError(const char *fmt, ...)
{
    char msg[256];
    va_list ap;

    FONT_Flush();

    va_start(ap, fmt);
    vsprintf(msg, fmt, ap);
    FONT_Print(msg);
    va_end(ap);

    DEBUG_PageFlip();
    _break(0x407);
}

void DEBUG_ProcessSecondController(GameTracker *gameTracker)
{
    (void)gameTracker;
}

void DEBUG_ProcessCheat(GameTracker *gameTracker)
{
    long angleRelCamera;

    angleRelCamera = 0;

    if (((gameTracker->controlCommand[0][0] & 0xA01) != 0xA01) && ((gameTracker->controlCommand[0][0] & 0xA02) != 0xA02))
    {
        if ((gameTracker->controlCommand[0][0] & 0x5) == 0x5)
        {
            angleRelCamera = 2560;
        }
        else if ((gameTracker->controlCommand[0][0] & 0x9) == 0x9)
        {
            angleRelCamera = 1536;
        }
        else if ((gameTracker->controlCommand[0][0] & 0x6) == 0x6)
        {
            angleRelCamera = 3584;
        }
        else if ((gameTracker->controlCommand[0][0] & 0xA) == 0xA)
        {
            angleRelCamera = 512;
        }
        else if ((gameTracker->controlCommand[0][0] & 0x2))
        {
            angleRelCamera = 4096;
        }
        else if ((gameTracker->controlCommand[0][0] & 0x4))
        {
            angleRelCamera = 3072;
        }
        else if ((gameTracker->controlCommand[0][0] & 0x8))
        {
            angleRelCamera = 1024;
        }
        else if ((gameTracker->controlCommand[0][0] & 0x1))
        {
            angleRelCamera = 2048;
        }
    }

    if (angleRelCamera != 0)
    {
        SVECTOR v = {0};
        VECTOR dv = {0};
        MATRIX rotate_mat;

        v.vy = -256;

        MATH3D_SetUnityMatrix(&rotate_mat);

        RotMatrixZ(theCamera.core.rotation.z + angleRelCamera, &rotate_mat);

        ApplyMatrix(&rotate_mat, &v, &dv);

        gameTracker->playerInstance->position.x += (short)dv.vx;
        gameTracker->playerInstance->position.y += (short)dv.vy;
    }
}

void DEBUG_DoAreaProtection()
{
    while (checkagain() != 0)
    {
    }
}
