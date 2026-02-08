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
#include "Game/TIMER.h"
#include "Game/G2/MAING2.h"
#include "Game/STREAM.h"
#include "Game/LOCAL/LOCALSTR.h"
#include "Game/PSX/AADSFX.h"

MainTracker mainTrackerX;

int devstation;

static int mainMenuSfx;

short mainMenuTimeOut;

DebugMenuLine mainMenu[8924 + 8];

unsigned long __timerEvent;

long gTimerEnabled;

static char D_800D099C[] = "CD ERROR: file %s not in bigfile\n"; /* unused */

static char D_800D09C0[] = "CD ERROR: file %x not in bigfile\n"; /* unused */

int _ramsize = 0x200000; /* unused */

int _stacksize = 0x4000; /* unused */

char mainOptionsInit = 0;

short gEndGameNow = 0;

int pcverify = 0; /* unused */

int nosound = 0;

int nomusic = 0;

InterfaceItem InterfaceItems[6] = 
{ 
    { "\\PUBLOGO.STR;1",                         0,     0, 0,  1 },
	{ "\\CRYLOGO.STR;1",                         0,     0, 0,  5 },
	{ "\\KAININT.STR;1",                         0,     0, 0, -1 },
	{ "\\VERSE.STR;1",                           0,     0, 0,  4 },
	{ "\\CREDITS.STR;1",                         0,     0, 0, -1 },
    { "\\kain2\\game\\psx\\mainmenu\\legal.tim", 165, 165, 1, -1 }, 
};

long DoMainMenu = 0;

long mainMenuMode = 0;

intptr_t *mainMenuScreen = NULL;

intptr_t *controlsScreen = NULL; /* unused */

short mainMenuFadeInTime = 0; /* unused */

short mainMenuFading = 0;

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

void ProcessArgs(char *baseAreaName, GameTracker *gameTracker)
{
    char levelNum[32];
    char worldName[32];
    intptr_t *argData;

    argData = (intptr_t*)LOAD_ReadFile("\\kain2\\game\\psx\\kain2.arg", 10);

    if (argData != NULL)
    {
        ExtractWorldName(worldName, (char *)argData);
        ExtractLevelNum(levelNum, (char *)argData);

        sprintf(baseAreaName, "%s%s", worldName, levelNum);

        if (FindTextInLine("-NOSOUND", (char*)argData) != 0) 
        {
            nosound = 1;
            nomusic = 1;
        }

        if (FindTextInLine("-NOMUSIC", (char*)argData) != 0) 
        {
            nomusic = 1;
        }

        if (FindTextInLine("-TIMEOUT", (char*)argData) != 0)
        {
            gameTracker->debugFlags |= 0x20000;
        }

        if (FindTextInLine("-MAINMENU", (char*)argData) != 0) 
        {
            DoMainMenu = 1;
        }

        if (FindTextInLine("-INSPECTRAL", (char*)argData) != 0) 
        {
            gameTrackerX.gameData.asmData.MorphType = 1;
        }

        if (FindTextInLine("-VOICE", (char*)argData) != 0)
        {
            gameTracker->debugFlags |= 0x80000;
        }

        if (FindTextInLine("-DEBUG_CD", (char*)argData) != 0) 
        {
            gameTracker->debugFlags |= 0x80000000;
        }

        if (FindTextInLine("-LOADGAME", (char*)argData) != 0)
        {
            gameTrackerX.streamFlags |= 0x200000;
        }

        if (FindTextInLine("-ALLWARP", (char*)argData) != 0) 
        {
            gameTrackerX.streamFlags |= 0x400000;
        }

        gameTracker->debugFlags |= 0x80000;

        MEMPACK_Free((char *)argData);
    }
    else
    {
        memcpy(baseAreaName, "under1", sizeof("under1"));
    }
}

int D_800D0BF4[2] = { 0x00000200, 0x02000200 };

INCLUDE_ASM("asm/nonmatchings/Game/PSX/MAIN", InitDisplay);
/*void InitDisplay()
{
    int i;
    RECT r = { 0x0200, 0x0000, 0x0200, 0x0200 };

    ResetGraph(3);

    SetGraphDebug(0);

    SetDefDrawEnv(&draw[0], 0, 0, 512, 240);
    SetDefDispEnv(&disp[0], 0, 0, 512, 240);

    SetDefDrawEnv(&draw[1], 0, 256, 512, 240);
    SetDefDispEnv(&disp[1], 0, 256, 512, 240);

    draw[0].dtd = draw[1].dtd = 1;
    draw[0].dfe = draw[1].dfe = 1;

    draw[0].isbg = draw[1].isbg = 0;

    setRGB0(&draw[0], 0, 0, 0);
    setRGB0(&draw[1], 0, 0, 0);

    for (i = 0; i < 2; i++)
    {
        clearRect[i].tag.bytes[3] = 3;

        clearRect[i].code = 2;

        setXY0(&clearRect[i], 0, i * 256);
        setWH(&clearRect[i], 512, 240);

        setRGB0(&clearRect[i], 0, 0, 0);
    }

    ClearDisplay();

    ClearOTagR((unsigned long*)gameTrackerX.drawOT, 3072);
    ClearOTagR((unsigned long*)gameTrackerX.dispOT, 3072);

    ClearImage(&r, 0, 255, 0);
}*/

void StartTimer()
{
    EnterCriticalSection();

    __timerEvent = OpenEvent(0xF2000000, 2, 4096, TimerTick);

    EnableEvent(__timerEvent);

    SetRCnt(0xF2000000, 0xFFFF, 4097);

    StartRCnt(0xF2000000);

    ExitCriticalSection();

    gTimerEnabled = 1;
}

void VblTick()
{
    if (devstation != 0)
    {
        _break(0x400);
    }

    gameTrackerX.vblFrames++;
    gameTrackerX.vblCount++;

    if ((gameTrackerX.reqDisp != NULL) && (gameTrackerX.vblFrames > (unsigned long)gameTrackerX.frameRateLock))
    {
        PutDispEnv(gameTrackerX.reqDisp);

        gameTrackerX.reqDisp = NULL;

        gameTrackerX.vblFrames = 0;
    }
}

void DrawCallback()
{
    if (gameTrackerX.drawTimerReturn != NULL)
    {
        *gameTrackerX.drawTimerReturn = TIMER_TimeDiff(gameTrackerX.usecsStartDraw);

        gameTrackerX.drawTimerReturn = NULL;

        gameTrackerX.reqDisp = gameTrackerX.disp + (gameTrackerX.gameData.asmData.dispPage * 20);
    }
}

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
        loadingScreen = LOAD_ReadFile("\\kain2\\game\\psx\\loading.tim\0\\kain2\\game\\psx\\warning.tim", 11);
    }

    if (loadingScreen != NULL)
    {
        screen_to_vram(loadingScreen, gameTrackerX.gameData.asmData.dispPage);

        MEMPACK_Free((char*)loadingScreen);
    }
}

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

    mainMenuScreen = (intptr_t*)MAIN_LoadTim("\\kain2\\game\\psx\\frontend\\title1.tim\0\\kain2\\game\\psx\\mainmenu\\features.tim");

    VRAM_EnableTerrainArea();

    menuface_initialize();

    currentMenu = mainMenu;

    gameTrackerX.gameMode = 4;

    menu_set(gameTrackerX.menu, menudefs_main_menu);
}

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

int MainG2(void *appData)
{
    MainTracker *mainTracker;
    GameTracker *gameTracker;
    long menuPos;

    menuPos = 0;

    CheckForDevStation();

    mainTracker = &mainTrackerX;
    gameTracker = &gameTrackerX;

    mainOptionsInit = 0;

    if (MainG2_InitEngine(appData, 512, 240, NULL) != 0)
    {
        MEMPACK_Init();
        LOAD_InitCd();

        StartTimer();

        STREAM_InitLoader("\\BIGFILE.DAT;1", "");

        localstr_set_language(language_default);

        GAMELOOP_SystemInit(gameTracker);

        gameTracker->currentLvl = gameTracker->lastLvl = -1;

        gameTracker->disp = disp;

        ProcessArgs(gameTracker->baseAreaName, gameTracker);

        InitMainTracker(mainTracker);

        MAIN_DoMainInit();

        mainTracker->mainState = 6;

        mainTracker->movieNum = 0;

        do
        {
            mainTracker->previousState = mainTracker->mainState;

            switch (mainTracker->mainState)
            {
            case 6:
                CINE_Load();

                while (mainTracker->movieNum >= 0)
                {
                    if (CINE_Loaded() != 0)
                    {
                        CINE_Play(InterfaceItems[mainTracker->movieNum].name, 0xFFFF, 2);

                        ClearDisplay();
                    }

                    mainTracker->movieNum = InterfaceItems[mainTracker->movieNum].nextItem;

                    if (InterfaceItems[mainTracker->movieNum].itemType != 0)
                    {
                        mainTracker->mainState = 4;
                        break;
                    }
                }

                CINE_Unload();

                if (mainTracker->movieNum < 0)
                {
                    mainTracker->mainState = 8;
                }

                if (nosound == 0)
                {
                    SOUND_StopAllSound();
                }

                break;
            case 4:
                LOAD_ChangeDirectory("Menustuff");

                while ((unsigned long)mainTracker->movieNum < 6)
                {
                    InterfaceItem *item;
                    int timer;

                    item = &InterfaceItems[mainTracker->movieNum];

                    gameTrackerX.gameFlags &= ~0x1;

                    show_screen(item->name);

                    for (timer = 0; timer++ < item->timeout; )
                    {
                        GAMEPAD_Process(gameTracker);

                        if ((timer > item->buttonTimeout) && ((gameTracker->controlCommand[0][1] & 0x80)))
                        {
                            break;
                        }

                        VSync(0);
                    }

                    mainTracker->movieNum = item->nextItem;

                    if ((mainTracker->movieNum >= 0) && (InterfaceItems[mainTracker->movieNum].itemType != 1))
                    {
                        mainTracker->mainState = 6;
                        break;
                    }
                }

                FONT_ReloadFont();

                if (mainTracker->mainState != 6)
                {
                    if (DoMainMenu == 0)
                    {
                        MAIN_ResetGame();

                        gameTrackerX.gameMode = 0;

                        mainMenuFading = 1;

                        MAIN_StartGame();
                    }
                    else
                    {
                        mainTracker->mainState = 8;
                    }
                }

                break;
            case 8:
                gameTrackerX.gameData.asmData.MorphType = 0;

                ProcessArgs(gameTracker->baseAreaName, gameTracker);

                MAIN_ResetGame();

                LOAD_ChangeDirectory("Menustuff");

                MAIN_MainMenuInit();
                MAIN_InitVolume();

                SAVE_ClearMemory(&gameTrackerX);

                mainTracker->mainState = 9;

                FONT_ReloadFont();
                break;
            case 9:
                menuPos = MAIN_DoMainMenu(gameTracker, mainTracker, menuPos);
                break;
            case 2:
                if ((gameTrackerX.streamFlags & 0x1000000))
                {
                    play_movie(InterfaceItems[2].name);

                    gameTrackerX.streamFlags &= ~0x1000000;
                }

                if ((gameTrackerX.streamFlags & 0x200000))
                {
                    gameTrackerX.streamFlags &= ~0x200000;
                }

                if (nosound == 0)
                {
                    MAIN_InitVolume();
                }

                MAIN_ShowLoadingScreen();

                FONT_ReloadFont();

                DrawSync(0);

                gameTracker->frameCount = 0;

                STREAM_Init();

                GAMELOOP_LevelLoadAndInit(gameTracker->baseAreaName, gameTracker);

                gameTracker->levelDone = 0;

                mainTracker->mainState = 1;

                while (STREAM_PollLoadQueue() != 0);

                gameTrackerX.vblFrames = 0;
                break;
            case 1:
                SOUND_UpdateSound();

                if ((gameTracker->debugFlags & 0x80000))
                {
                    VOICEXA_Tick();
                }

                PSX_GameLoop(gameTracker);

                if (gameTracker->levelDone != 0)
                {
                    FadeOutSayingLoading(gameTracker);

                    aadStopAllSfx();

                    STREAM_DumpAllLevels(0, 0);

                    RemoveAllObjects(gameTracker);

                    while ((aadGetNumLoadsQueued() != 0) || (aadMem->sramDefragInfo.status != 0))
                    {
                        SOUND_UpdateSound();

                        STREAM_PollLoadQueue();
                    }

                    SOUND_ShutdownMusic();

                    MEMPACK_FreeByType(14);

                    GAMELOOP_ResetGameStates();

                    MEMPACK_DoGarbageCollection();

                    if (gameTracker->levelDone == 2)
                    {
                        mainTracker->mainState = 8;
                    }
                    else if (gameTracker->levelDone == 3)
                    {
                        mainTracker->mainState = 6;

                        mainTracker->movieNum = 3;
                    }
                    else if (gameTracker->levelDone == 4)
                    {
                        mainTracker->mainState = 2;

                        if (!(gameTrackerX.streamFlags & 0x200000))
                        {
                            SAVE_ClearMemory(&gameTrackerX);
                        }
                    }
                    else
                    {
                        mainTracker->mainState = 2;
                    }
                }

                break;
            case 7:
                mainTracker->done = 1;
                break;
            case 3:
            case 5:
            }

            STREAM_PollLoadQueue();
        } while (mainTracker->done == 0);

        SOUND_StopAllSound();

        SOUND_Free();

        SetDispMask(0);

        DrawSync(0);
        VSync(0);

        DrawSyncCallback(NULL);
        VSyncCallback(NULL);

        EnterCriticalSection();

        StopRCnt(0xF2000000);

        DisableEvent(__timerEvent);
        CloseEvent(__timerEvent);

        ExitCriticalSection();

        VSync(5);

        StopCallback();

        PadStopCom();

        ResetGraph(3);
    }

    MainG2_ShutDownEngine(appData);

    return 0;
}
