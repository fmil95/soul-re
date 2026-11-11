#ifndef _MENUUTIL_H_
#define _MENUUTIL_H_

#include "common.h"

menu_ctrl_t menu_get_ctrl(void *gt);
void menu_print(int xpos, int ypos, char *text, int color);
int menu_text_width(char *text);
void menu_sound(menu_sound_t sound);

#endif
