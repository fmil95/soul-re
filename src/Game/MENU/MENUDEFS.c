#include "Game/MENU/MENUDEFS.h"
#include "Game/MENU/MENU.h"
#include "Game/MENU/MENUFACE.h"
#include "Game/GAMELOOP.h"
#include "Game/GAMEPAD.h"
#include "Game/MEMPACK.h"
#include "Game/PSX/MAIN.h"
#include "Game/MCARD/MEMCARD.h"
#include "Game/CINEMA/CINEPSX.h"
#include "Game/SOUND.h"
#include "Game/LOCAL/LOCALSTR.h"
#include "Game/DEBUG.h"
#include "Game/FONT.h"

static char *the_attract_movies[] = {
    "\\KAINDEM1.STR;1",
    "\\KAINDEM2.STR;1",
    "\\KAINDEM3.STR;1",
    "\\KAINDEM4.STR;1",
};

static int MAIN_YPOS = 30;

static int PAUSE_YPOS = 60;

static int MAIN_XPOS = 135;

static int MAIN_WIDTH = 172;

static int PAUSE_XPOS = 256;

static int PAUSE_WIDTH = 256;

static int LINESKIP = 14;

static int ITEMSKIP = 2;

static int hack_reset_attract = 1;

static int StartGameFading = 0;

static int hack_attract = 0;

static int hack_attract_movie = 0;

void do_check_controller(void *gt)
{
    if (((GameTracker *)gt)->gameMode == 6)
    {
        GAMEPAD_DisplayControllerStatus(170);
    }
    else
    {
        GAMEPAD_DisplayControllerStatus(200);
    }
}

int do_push_menu(void *gt, long menuparam, menu_ctrl_t ctrl)
{
    typedef int (*fn)(); // not from decls.h

    if (ctrl == menu_ctrl_engage)
    {
        menu_push(((GameTracker *)gt)->menu, (fn)menuparam);

        return 1;
    }

    return 0;
}

int do_pop_menu(void *gt, long param, menu_ctrl_t ctrl)
{
    (void)param;

    if (ctrl == menu_ctrl_engage)
    {
        menu_pop(((GameTracker *)gt)->menu);

        return 1;
    }

    return 0;
}

int do_function(void *gt, long fnparam, menu_ctrl_t ctrl)
{
    typedef int (*fn)(); // not from decls.h
    fn temp;             // not from decls.h

    (void)gt;

    if (ctrl == menu_ctrl_engage)
    {
        temp = (fn)fnparam;

        temp();

        return 1;
    }

    return 0;
}

int do_start_game(void *gt, long parameter, menu_ctrl_t ctrl)
{
    (void)parameter;

    if (ctrl == menu_ctrl_engage)
    {
        if (MEMCARD_IsWrongVersion(((GameTracker *)gt)->memcard) == 0)
        {
            menu_push(((GameTracker *)gt)->menu, memcard_main_menu);

            return 1;
        }
        else
        {
            MAIN_StartGame();

            return 1;
        }
    }

    return 0;
}

int do_save_menu(void *gt, long parameter, menu_ctrl_t ctrl)
{
    (void)parameter;

    if (ctrl == menu_ctrl_engage)
    {
        menu_push(((GameTracker *)gt)->menu, memcard_pause_menu);

        return 1;
    }

    return 0;
}

void womp_background(char *tim_path)
{
    MEMPACK_Free((char *)mainMenuScreen);

    mainMenuScreen = MAIN_LoadTim(tim_path);
}

void play_movie_dup1(char *name)
{
    if (CINE_Load() != 0)
    {
        CINE_Play(name, 0xFFFF, 2);

        CINE_Unload();

        MAIN_InitVolume();
    }
}

void menudefs_reset_hack_attract_mode()
{
    if (hack_attract > 0)
    {
        hack_attract = gameTrackerX.vblCount;
    }
}

void check_hack_attract()
{
    if ((hack_attract > 0) && ((unsigned int)(hack_attract + 2000) < gameTrackerX.vblCount))
    {
        hack_attract = 1;

        play_movie_dup1(the_attract_movies[hack_attract_movie]);

        hack_attract_movie = (hack_attract_movie + 1) & 0x3;

        hack_attract = gameTrackerX.vblCount;
    }
}

int get_volume(void *gt, sfx_t sfx)
{
    int raw;

    if (sfx == sfx_sound)
    {
        raw = ((GameTracker *)gt)->sound.gSfxVol;
    }
    else if (sfx == sfx_music)
    {
        raw = ((GameTracker *)gt)->sound.gMusicVol;
    }
    else if (sfx == sfx_voice)
    {
        raw = ((GameTracker *)gt)->sound.gVoiceVol;
    }
    else
    {
        raw = 3;
    }

    return (raw * 10) / 127;
}

void set_volume(sfx_t sfx, int cooked)
{
    int raw;

    raw = ((cooked * 127) + 9) / 10;

    switch (sfx)
    {
    case sfx_sound:
        SOUND_SetSfxVolume(raw);
        break;
    case sfx_music:
        SOUND_SetMusicVolume(raw);
        break;
    case sfx_voice:
        SOUND_SetVoiceVolume(raw);
        break;
    default:
        break;
    }
}

int do_sound_adjust(void *gt, long sfxparam, menu_ctrl_t ctrl)
{
    int volume;

    volume = get_volume(gt, (sfx_t)sfxparam);

    switch (ctrl)
    {
    case menu_ctrl_left:
        if (volume > 0)
        {
            set_volume((sfx_t)sfxparam, volume - 1);
        }

        return 1;
    case menu_ctrl_right:
        if (volume < 10)
        {
            set_volume((sfx_t)sfxparam, volume + 1);
        }

        return 1;
    default:
        return 0;
    }
}

void sound_item(void *gt, char *text, sfx_t sfx)
{
    menu_item(((GameTracker *)gt)->menu, do_sound_adjust, sfx, "%s %d", text, get_volume(gt, sfx));
}

int menudefs_toggle_dualshock(void *gt, long param, menu_ctrl_t ctrl)
{
    (void)gt;
    (void)param;

    if (((ctrl - 3) == menu_ctrl_none) || ((ctrl - 3) == menu_ctrl_up))
    {
        if (GAMEPAD_DualShockEnabled() != 0)
        {
            GAMEPAD_DisableDualShock();

            return 1;
        }

        GAMEPAD_EnableDualShock();

        GAMEPAD_Shock1(128, 32768);

        return 1;
    }

    return 0;
}

int options_menu(void *gt, int index)
{
    static int wasDualShock;
    int dualShock;

    hack_attract = 0;

    MENUFACE_ChangeStateRandomly(index);

    do_check_controller(gt);

    menu_item_flags(((GameTracker *)gt)->menu, NULL, 0, 4, localstr_get(LOCALSTR_options));

    sound_item(gt, localstr_get(LOCALSTR_sound), sfx_sound);
    sound_item(gt, localstr_get(LOCALSTR_music), sfx_music);
    sound_item(gt, localstr_get(LOCALSTR_voice), sfx_voice);

    dualShock = GAMEPAD_ControllerIsDualShock();

    if (dualShock != 0)
    {
        if (GAMEPAD_DualShockEnabled() != 0)
        {
            menu_item(((GameTracker *)gt)->menu, menudefs_toggle_dualshock, 0, localstr_get(LOCALSTR_vibration_on));
        }
        else
        {
            menu_item(((GameTracker *)gt)->menu, menudefs_toggle_dualshock, 0, localstr_get(LOCALSTR_vibration_off));
        }
    }

    menu_item(((GameTracker *)gt)->menu, do_pop_menu, 0, localstr_get(LOCALSTR_done));

    if ((dualShock != wasDualShock) && (index >= 4))
    {
        index = dualShock + 4;
    }

    wasDualShock = dualShock;

    return index < 0 ? 1 : index;
}

int main_menu(void *gt, int index)
{
    hack_attract = 0;

    menu_format(((GameTracker *)gt)->menu, 1, MAIN_XPOS, MAIN_YPOS, MAIN_WIDTH, LINESKIP, ITEMSKIP, 0);

    MENUFACE_ChangeStateRandomly(index);

    do_check_controller(gt);

    menu_item(((GameTracker *)gt)->menu, do_start_game, 0, localstr_get(LOCALSTR_start_game));
    menu_item(((GameTracker *)gt)->menu, do_push_menu, (long)&options_menu, localstr_get(LOCALSTR_options));

    return index < 0 ? 0 : index;
}

int do_main_menu(void *gt, long param, menu_ctrl_t ctrl)
{
    (void)param;

    if ((StartGameFading == 0) && ((ctrl == menu_ctrl_start) || (ctrl == menu_ctrl_engage)))
    {
        ((GameTracker *)gt)->wipeType = 10;

        ((GameTracker *)gt)->wipeTime = -20;
        ((GameTracker *)gt)->maxWipeTime = 20;

        StartGameFading = 1;

        return 1;
    }

    return 0;
}

char *flashStart()
{
    gameTrackerX.gameFramePassed = 1;

    if (StartGameFading == 1)
    {
        hack_reset_attract = 1;

        if (gameTrackerX.wipeTime == -1)
        {
            womp_background("\\kain2\\game\\psx\\bkgdmenu.tim");

            gameTrackerX.wipeType = 10;

            gameTrackerX.wipeTime = 20;
            gameTrackerX.maxWipeTime = 20;

            StartGameFading = 0;

            menu_pop(gameTrackerX.menu);

            menu_push(gameTrackerX.menu, main_menu);

            return NULL;
        }
    }
    else
    {
        static int counter = -1;
        int intpl;
        int fcols[2][3] = {{0xC0, 0xD2, 0xD2}, {0xC0, 0xC0, 0x40}};
        int r;
        int g;
        int b;

        counter = (counter + 1) % 60;

        if (counter < 10)
        {
            intpl = 0;
        }
        else if (counter < 30)
        {
            intpl = ((counter - 10) << 12) / 20;
        }
        else if (counter < 40)
        {
            intpl = 4096;
        }
        else
        {
            intpl = ((60 - counter) << 12) / 20;
        }

        r = ((fcols[0][0] * (4096 - intpl)) + (fcols[1][0] * intpl)) >> 12;
        g = ((fcols[0][1] * (4096 - intpl)) + (fcols[1][1] * intpl)) >> 12;
        b = ((fcols[0][2] * (4096 - intpl)) + (fcols[1][2] * intpl)) >> 12;

        FONT_SetColorIndex(4);
        FONT_SetColorIndexCol(4, r, g, b);
    }

    return localstr_get(LOCALSTR_press_start);
}

int menudefs_main_menu(void *gt, int index)
{
    int temp; // not from decls.h

    if (hack_reset_attract != 0)
    {
        hack_reset_attract = 0;

        hack_attract = gameTrackerX.vblCount;
    }

    check_hack_attract();

    menu_format(((GameTracker *)gt)->menu, 1, 366, 144, 100, LINESKIP, ITEMSKIP, 0);

    menu_item(((GameTracker *)gt)->menu, do_main_menu, 0, flashStart());

    temp = index;

    if (temp < 0)
    {
        temp = 0;
    }

    return temp;
}

int menudefs_confirmexit_menu(void *gt, int index)
{
    int temp; // not from decls.h

    hack_attract = 0;

    do_check_controller(gt);

    menu_item_flags(((GameTracker *)gt)->menu, NULL, 0, 4, localstr_get(LOCALSTR_query_quit));

    menu_item(((GameTracker *)gt)->menu, do_pop_menu, 0, localstr_get(LOCALSTR_no));
    menu_item(((GameTracker *)gt)->menu, do_function, (long)DEBUG_ExitGame, localstr_get(LOCALSTR_yes));

    temp = index;

    if (temp < 0)
    {
        temp = 1;
    }

    return temp;
}

int menudefs_pause_menu(void *gt, int index)
{
    int temp; // not from decls.h

    do_check_controller(gt);

    hack_attract = 0;

    menu_format(((GameTracker *)gt)->menu, 1, PAUSE_XPOS, PAUSE_YPOS, PAUSE_WIDTH, LINESKIP, ITEMSKIP, 1);

    menu_item_flags(((GameTracker *)gt)->menu, NULL, 0, 4, localstr_get(LOCALSTR_paused));

    menu_item(((GameTracker *)gt)->menu, do_function, (long)DEBUG_ContinueGame, localstr_get(LOCALSTR_resume_game));

    if (!(gameTrackerX.streamFlags & 0x4))
    {
        menu_item(((GameTracker *)gt)->menu, do_save_menu, 0, localstr_get(LOCALSTR_save_game));
        menu_item(((GameTracker *)gt)->menu, do_push_menu, (long)options_menu, localstr_get(LOCALSTR_options));
        menu_item(((GameTracker *)gt)->menu, do_push_menu, (long)menudefs_confirmexit_menu, localstr_get(LOCALSTR_quit_game));
    }

    temp = index;

    if (temp < 0)
    {
        temp = 1;
    }

    return temp;
}
