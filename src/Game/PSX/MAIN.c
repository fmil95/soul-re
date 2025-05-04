#include "Game/PSX/MAIN.h"
#include "Game/GAMELOOP.h"
#include "Game/RAZIEL/RAZLIB.h"
#include "Game/PSX/AADLIB.h"
#include "Game/LOAD3D.h"
#include "Game/MEMPACK.h"
#include "Game/CINEMA/CINEPSX.h"
#include "Game/DEBUG.h"
#include "Game/MENU/MENU.h"

short mainMenuFading;

MainTracker mainTrackerX;

int nosound;

int nomusic;

int devstation;

INCLUDE_ASM("asm/nonmatchings/Game/PSX/MAIN", ClearDisplay);

void screen_to_vram(long *screen, int buffer)
{
    LOAD_LoadTIM2(screen, 0, buffer * 256, 512, 256);
}

void show_screen(char *name)
{
    long *screen;

    screen = LOAD_ReadFile(name, 11);

    if (screen != NULL)
    {
        screen_to_vram(screen, gameTrackerX.gameData.asmData.dispPage);

        MEMPACK_Free((char *)screen);
    }
}

void play_movie(char *name)
{
    if (CINE_Load() != 0)
    {
        CINE_Play(name, 0xFFFF, 2);

        ClearDisplay();

        CINE_Unload();
    }
}

void InitMainTracker(MainTracker *mainTracker)
{
    mainTracker->mainState = 0;
    mainTracker->previousState = 0;

    mainTracker->done = 0;
}

INCLUDE_ASM("asm/nonmatchings/Game/PSX/MAIN", FindTextInLine);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/MAIN", ExtractWorldName);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/MAIN", ExtractLevelNum);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/MAIN", ProcessArgs);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/MAIN", InitDisplay);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/MAIN", StartTimer);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/MAIN", VblTick);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/MAIN", DrawCallback);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/MAIN", FadeOutSayingLoading);

void CheckForDevStation()
{
    long *volatile a1;
    long *volatile a2;

    a1 = (long *)0x80180000;
    a2 = (long *)0x80380000;

    *a1 = 0;
    *a2 = 0x12345678;

    if (*a1 == *a2)
    {
        devstation = 0;
    }
    else
    {
        devstation = 1;
    }
}

INCLUDE_ASM("asm/nonmatchings/Game/PSX/MAIN", MAIN_ShowLoadingScreen);

long *MAIN_LoadTim(char *name)
{
    return LOAD_ReadFile(name, 11);
}

void init_menus(GameTracker *gt)
{
    menu_t *menu;

    menu = (menu_t *)MEMPACK_Malloc(menu_data_size(), 45);

    menu_initialize(menu, gt);

    gt->menu = menu;
}

void MAIN_DoMainInit()
{
    InitDisplay();
    InitGeom();
    SetGeomOffset(256, 120);
    SetGeomScreen(320);
    VRAM_InitVramBlockCache();
    FONT_Init();
    gameTrackerX.reqDisp = NULL;
    VSyncCallback(VblTick);
    DrawSyncCallback(DrawCallback);
    GAMEPAD_Init();
    SOUND_Init();
    VOICEXA_Init();

    if (nosound != 0)
    {
        SOUND_SfxOff();
        gameTrackerX.sound.gSfxOn = 0;
        gameTrackerX.sound.gVoiceOn = 0;
    }

    if (nomusic != 0)
    {
        SOUND_MusicOff();
        gameTrackerX.sound.gMusicOn = 0;
    }

    if (!(gameTrackerX.debugFlags & 0x80000))
    {
        gameTrackerX.sound.gVoiceOn = 0;
    }

    init_menus(&gameTrackerX);
    SAVE_Init(&gameTrackerX);
    srand(0);
}

void MAIN_InitVolume()
{
    aadInitVolume();

    aadStartMasterVolumeFade(gameTrackerX.sound.gMasterVol, 256, NULL);

    gameTrackerX.sound.soundsLoaded = 1;

    aadSetNoUpdateMode(0);
}

void MAIN_ResetGame()
{
    GAMELOOP_SetScreenWipe(0, 0, -1);

    gameTrackerX.gameFlags &= ~0x80;
    gameTrackerX.gameFlags &= ~0x10;
    gameTrackerX.gameFlags &= ~0x1;

    RAZIEL_StartNewGame();
}

INCLUDE_ASM("asm/nonmatchings/Game/PSX/MAIN", MAIN_MainMenuInit);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/MAIN", MAIN_FreeMainMenuStuff);

void MAIN_StartGame()
{
    if (mainMenuFading != 0)
    {
        mainTrackerX.mainState = 2;

        MAIN_FreeMainMenuStuff();

        gEndGameNow = 0;

        mainMenuFading = 0;

        currentMenu = (DebugMenuLine *)&standardMenu;
    }
    else
    {
        gameTrackerX.gameMode = 0;

        currentMenu = NULL;

        mainMenuFading = 1;

        GAMELOOP_SetScreenWipe(-30, 30, 10);
    }
}

INCLUDE_ASM("asm/nonmatchings/Game/PSX/MAIN", MAIN_DoMainMenu);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/MAIN", MainG2);
