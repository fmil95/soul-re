#ifndef _MENU_H_
#define _MENU_H_

#include "common.h"

void menu_pop(menu_t *menu);
void menu_item_flags(menu_t *menu, int (*fn)(), long parameter, long flags, char *format, ...);
void menu_item(menu_t *menu, int (*fn)(), long parameter, char *format, ...);
void menu_push(menu_t *menu, int (*fn)());
void menu_build(menu_t *menu);
void menu_draw(menu_t *menu);
void menu_run(menu_t *menu);
void menu_process(menu_t *menu);
void DisplayHintBox(int len, int y);
void menu_set(menu_t *menu, int (*fn)());
void menu_format(menu_t *menu, int center, int xpos, int ypos, int width, int lineskip, int itemskip, int border);
int menu_data_size();
void menu_initialize(menu_t *menu, void *opaque);

#endif
