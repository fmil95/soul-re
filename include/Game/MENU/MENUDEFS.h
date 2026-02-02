#ifndef _MENUDEFS_H_
#define _MENUDEFS_H_

#include "common.h"

long *mainMenuScreen;

int menudefs_pause_menu(void *gt, int index);
void play_movie_dup1(char *name);
int options_menu(void *gt, int index);
char *flashStart();
void menudefs_reset_hack_attract_mode();
int menudefs_main_menu(void *gt, int index);

#endif
