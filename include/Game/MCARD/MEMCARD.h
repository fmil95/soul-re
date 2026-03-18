#ifndef _MEMCARD_H_
#define _MEMCARD_H_

#include "common.h"

void memcard_end(memcard_t *memcard);
int maybe_start(memcard_t *memcard);
int memcard_initialize(memcard_t *memcard, void *gt, int nblocks, void *buffer, int nbytes);
int memcard_main_menu(void *gt, int index);
int MEMCARD_IsWrongVersion(memcard_t *memcard);
int memcard_pause_menu(void *gt, int index);
void memcard_pop(void *opaque);  
void *gt2mcmenu(void *gt);

extern memcard_t gMemcard;

#endif
