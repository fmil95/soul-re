#include "Game/PSX/MAIN.h"
#include "Game/FONT.h"
#include "Game/GAMELOOP.h"
#include "Game/GAMEPAD.h"
#include "Game/SAVEINFO.h"
#include "Game/RAZIEL/RAZLIB.h"
#include "Game/PSX/AADLIB.h"
#include "Game/LOAD3D.h"
#include "Game/MEMPACK.h"
#include "Game/CINEMA/CINEPSX.h"
#include "Game/DEBUG.h"
#include "Game/VRAM.h"
#include "Game/MENU/MENU.h"
#include "Game/MENU/MENUDEFS.h"
#include "Game/MENU/MENUFACE.h"
#include "Game/PSX/AADLIB.h"
#include "Game/SOUND.h"
#include "Game/VOICEXA.h"
#include "Game/DRAW.h"
#include "Game/HASM.h"

short mainMenuFading;

MainTracker mainTrackerX;

int nosound;

int nomusic;

int devstation;

static int mainMenuSfx;

short mainMenuTimeOut;

long DoMainMenu;

DebugMenuLine mainMenu[8924 + 8];

long mainMenuMode;

intptr_t *mainMenuScreen;

void ClearDisplay(void)
{
    PutDrawEnv(&draw[gameTrackerX.gameData.asmData.dispPage]);
    clearRect[gameTrackerX.gameData.asmData.dispPage].r0 = 0;
    clearRect[gameTrackerX.gameData.asmData.dispPage].g0 = 0;
    clearRect[gameTrackerX.gameData.asmData.dispPage].b0 = 0;
    DrawPrim(&clearRect[gameTrackerX.gameData.asmData.dispPage]);
    DrawSync(0);
    PutDispEnv(&disp[gameTrackerX.gameData.asmData.dispPage]);
    SetDispMask(1);
}

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

char *FindTextInLine(char *search_match, char *search_str)
{
    char *match_pos;

    match_pos = search_match;

    while ((*search_str != '\0') && (*search_str != '\n') && (*search_str != '\r'))
    {
        if ((*search_str++ | ' ') == (*match_pos | ' '))
        {
            match_pos++;
        }
        else
        {
            match_pos = search_match;
        }

        if (*match_pos == '\0')
        {
            return search_str;
        }
    }

    return NULL;
}

void ExtractWorldName(char *worldName, char *levelName)
{
    for (; *levelName != '-'; levelName++)
    {
        if (((*levelName >= 'A') && (*levelName <= 'Z')) || ((*levelName >= 'a') && (*levelName <= 'z')))
        {
            *worldName++ = *levelName;
        }
        else
        {
            break;
        }
    }

    *worldName = '\0';
}

void ExtractLevelNum(char *levelNum, char *levelName)
{
    for (; *levelName != '-'; levelName++)
    {
        if ((*levelName == '0') || (*levelName == '1') || (*levelName == '2') || (*levelName == '3') || (*levelName == '4') || (*levelName == '5') || (*levelName == '6') || (*levelName == '7') || (*levelName == '8') || (*levelName == '9'))
        {
            break;
        }
    }

    while ((*levelName == '0') || (*levelName == '1') || (*levelName == '2') || (*levelName == '3') || (*levelName == '4') || (*levelName == '5') || (*levelName == '6') || (*levelName == '7') || (*levelName == '8') || (*levelName == '9'))
    {
        *levelNum++ = *levelName++;
    }

    *levelNum = '\0';
}

extern char D_800D0B60[];
extern char D_800D0B7C[];
extern char D_800D0B84[];
extern char D_800D0B90[];
extern char D_800D0B9C[];
extern char D_800D0BA8[];
extern char D_800D0BB4[];
extern char D_800D0BC0[];
extern char D_800D0BC8[];
extern char D_800D0BD4[];
extern char D_800D0BE0[];
extern char D_800D0BEC[];
void ProcessArgs(char *baseAreaName, GameTracker *gameTracker)
{
    char levelNum[32];
    char worldName[32];
    intptr_t *argData;

    // argData = (intptr_t*)LOAD_ReadFile("\\kain2\\game\\psx\\kain2.arg", 10);
    argData = (intptr_t *)LOAD_ReadFile(D_800D0B60, 10);

    if (argData != NULL)
    {
        ExtractWorldName(worldName, (char *)argData);
        ExtractLevelNum(levelNum, (char *)argData);

        // sprintf(baseAreaName, "%s%s", worldName, levelNum);
        sprintf(baseAreaName, D_800D0B7C, worldName, levelNum);

        // if (FindTextInLine("-NOSOUND", (char*)argData) != 0) 
        if (FindTextInLine(D_800D0B84, (char *)argData) != 0)
        {
            nosound = 1;
            nomusic = 1;
        }

        // if (FindTextInLine("-NOMUSIC", (char*)argData) != 0) 
        if (FindTextInLine(D_800D0B90, (char *)argData) != 0)
        {
            nomusic = 1;
        }

        // if (FindTextInLine("-TIMEOUT", (char*)argData) != 0)
        if (FindTextInLine(D_800D0B9C, (char *)argData) != 0)
        {
            gameTracker->debugFlags |= 0x20000;
        }

        // if (FindTextInLine("-MAINMENU", (char*)argData) != 0) 
        if (FindTextInLine(D_800D0BA8, (char *)argData) != 0)
        {
            DoMainMenu = 1;
        }

        // if (FindTextInLine("-INSPECTRAL", (char*)argData) != 0) 
        if (FindTextInLine(D_800D0BB4, (char *)argData) != 0)
        {
            gameTrackerX.gameData.asmData.MorphType = 1;
        }

        // if (FindTextInLine("-VOICE", (char*)argData) != 0)
        if (FindTextInLine(D_800D0BC0, (char *)argData) != 0)
        {
            gameTracker->debugFlags |= 0x80000;
        }

        // if (FindTextInLine("-DEBUG_CD", (char*)argData) != 0) 
        if (FindTextInLine(D_800D0BC8, (char *)argData) != 0)
        {
            gameTracker->debugFlags |= 0x80000000;
        }

        // if (FindTextInLine("-LOADGAME", (char*)argData) != 0)
        if (FindTextInLine(D_800D0BD4, (char *)argData) != 0)
        {
            gameTrackerX.streamFlags |= 0x200000;
        }

        // if (FindTextInLine("-ALLWARP", (char*)argData) != 0) 
        if (FindTextInLine(D_800D0BE0, (char *)argData) != 0)
        {
            gameTrackerX.streamFlags |= 0x400000;
        }

        gameTracker->debugFlags |= 0x80000;

        MEMPACK_Free((char *)argData);
    }
    else
    {
        // memcpy(baseAreaName, "under1", sizeof("under1"));
        memcpy(baseAreaName, D_800D0BEC, 7);
    }
}

INCLUDE_ASM("asm/nonmatchings/Game/PSX/MAIN", InitDisplay);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/MAIN", StartTimer);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/MAIN", VblTick);

INCLUDE_ASM("asm/nonmatchings/Game/PSX/MAIN", DrawCallback);

void FadeOutSayingLoading(GameTracker *gameTracker)
{
    POLY_F4_SEMITRANS *transPrim;
    unsigned long **drawot;
    long fadeTime;

    fadeTime = 0;

    drawot = gameTracker->drawOT;

    transPrim = (POLY_F4_SEMITRANS *)gameTracker->primPool->nextPrim;

    DRAW_TranslucentQuad(0, 0, 512, 0, 0, 240, 512, 240, 0, 0, 0, 2, gameTracker->primPool, drawot);

    FONT_Flush();

    while (fadeTime != 255)
    {
        fadeTime += 16;

        if (fadeTime >= 256)
        {
            fadeTime = 255;
        }

        gameTracker->drawPage = 1 - gameTracker->drawPage;

        transPrim->r0 = fadeTime;
        transPrim->g0 = fadeTime;
        transPrim->b0 = fadeTime;

        while (CheckVolatile(gameTracker->drawTimerReturn) != 0);

        PutDrawEnv(&draw[gameTracker->drawPage]);

        while (CheckVolatile(gameTracker->reqDisp) != 0);

        gameTracker->drawTimerReturn = (long *)&gameTracker->drawTime;

        gameTracker->gameData.asmData.dispPage = 1 - gameTracker->gameData.asmData.dispPage;

        VSync(0);

        DrawOTag((unsigned long *)&drawot[3071]);
    }
}

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
/* TODO: requires migration of .sdata
void MAIN_ShowLoadingScreen()
{
    long *loadingScreen;
    char langChar[5];
    int lang;
    char* temp; // not from decls.h

    temp = langChar;

    strcpy(temp, "FGIS");

    VSync(0);

    lang = localstr_get_language();

    if (lang != language_english)
    {
        char filename[64];

        sprintf(filename, "\\kain2\\game\\psx\\loading%c.tim", temp[lang - 1]);

        loadingScreen = LOAD_ReadFile(filename, 11);
    }
    else
    {
        loadingScreen = LOAD_ReadFile("\\kain2\\game\\psx\\loading.tim", 11);
    }

    if (loadingScreen != NULL)
    {
        screen_to_vram(loadingScreen, gameTrackerX.gameData.asmData.dispPage);

        MEMPACK_Free((char*)loadingScreen);
    }
}*/

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
/* TODO: requires migration of .sdata
void MAIN_MainMenuInit()
{
    char sfxFileName[64];
    char* temp; // not from decls.h

    mainMenuMode = 0;
    mainMenuTimeOut = 0;

    temp = sfxFileName;

    strcpy(temp, "\\kain2\\sfx\\object\\mainmenu\\mainmenu.snf");

    memset(&sfxFileName[strlen("\\kain2\\sfx\\object\\mainmenu\\mainmenu.snf") + 1], 0, sizeof(sfxFileName) - strlen("\\kain2\\sfx\\object\\mainmenu\\mainmenu.snf") - 1);

    mainMenuSfx = 0;

    if (LOAD_DoesFileExist(sfxFileName) != 0)
    {
        mainMenuSfx = aadLoadDynamicSfx("mainmenu", 0, 0);

        while (aadGetNumLoadsQueued() != 0)
        {
            aadProcessLoadQueue();
        }
    }

    mainMenuScreen = (intptr_t*)MAIN_LoadTim("\\kain2\\game\\psx\\frontend\\title1.tim");

    VRAM_EnableTerrainArea();

    menuface_initialize();

    currentMenu = mainMenu;

    gameTrackerX.gameMode = 4;

    menu_set(gameTrackerX.menu, menudefs_main_menu);
}*/

void MAIN_FreeMainMenuStuff(void)
{

    menuface_terminate();
    VRAM_DisableTerrainArea();

    if (mainMenuScreen != NULL)
    {
        MEMPACK_Free((char *)mainMenuScreen);
        mainMenuScreen = NULL;
    }

    aadFreeDynamicSfx(mainMenuSfx);
    while (aadGetNumLoadsQueued() != 0) { aadProcessLoadQueue(); }

}

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

long MAIN_DoMainMenu(GameTracker *gameTracker, MainTracker *mainTracker, long menuPos)
{

    unsigned long **drawot;

    (void)mainTracker;
    (void)menuPos;

    gameTrackerX.timeMult = 0x1000;
    drawot = gameTracker->drawOT;

    DrawPrim(&clearRect[gameTracker->drawPage]);
    GAMEPAD_Process(gameTracker);
    DEBUG_Process(gameTracker);

    if (mainMenuScreen != 0)
    {
        screen_to_vram(mainMenuScreen, gameTracker->drawPage);
    }

    GAMELOOP_HandleScreenWipes(drawot);
    MENUFACE_RefreshFaces();
    FONT_Flush();
    mainMenuTimeOut++;
    GAMELOOP_FlipScreenAndDraw(gameTracker, drawot);

    if (mainMenuFading != 0 && gameTracker->wipeTime == -1)
    {
        MAIN_StartGame();
    }

    return 0;
}

INCLUDE_ASM("asm/nonmatchings/Game/PSX/MAIN", MainG2);
