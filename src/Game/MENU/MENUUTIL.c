#include "Game/MENU/MENUUTIL.h"
#include "Game/FONT.h"
#include "Game/SOUND.h"

STATIC menu_sound_entry_t the_menu_sounds[8924 + 6];

menu_ctrl_t menu_get_ctrl(void *gt)
{
    menu_ctrl_t ctrl;

    if (!(((GameTracker *)gt)->controlCommand[0][1] & 0x1))
    {
        if (!(((GameTracker *)gt)->controlCommand[0][1] & 0x2))
        {
            ctrl = menu_ctrl_left;

            if (!(((GameTracker *)gt)->controlCommand[0][1] & 0x4))
            {
                ctrl = menu_ctrl_right;

                if (!(((GameTracker *)gt)->controlCommand[0][1] & 0x8))
                {
                    if ((!(((GameTracker *)gt)->controlCommand[0][1] & 0x80)) || ((((GameTracker *)gt)->controlCommand[0][0] & 0x300)))
                    {
                        ctrl = menu_ctrl_cancel;

                        if (!(((GameTracker *)gt)->controlCommand[0][1] & 0x10))
                        {
                            ctrl = menu_ctrl_none;

                            if ((((GameTracker *)gt)->controlCommand[0][1] & 0x4000))
                            {
                                ctrl = menu_ctrl_start;
                            }
                        }
                    }
                    else
                    {
                        ctrl = menu_ctrl_engage;
                    }
                }
            }
        }
        else
        {
            ctrl = menu_ctrl_down;
        }
    }
    else
    {
        ctrl = menu_ctrl_up;
    }

    return ctrl;
}

void menu_print(int xpos, int ypos, char *text, int color)
{
    FONT_SetCursor(xpos, ypos);

    if (color != 0)
    {
        FONT_SetColorIndex(color);
    }

    FONT_Print2(text);

    if (color != 0)
    {
        FONT_SetColorIndex(0);
    }
}

int menu_text_width(char *text)
{
    FONT_GetStringWidth(text);
}

void menu_sound(menu_sound_t sound)
{
    menu_sound_entry_t *entry;

    entry = &the_menu_sounds[sound];

    SndPlay(entry->sfx);
}
