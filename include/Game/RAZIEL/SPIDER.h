#ifndef SPIDER_H_
#define SPIDER_H_

#include "common.h"

void StateHandlerWallIdle(CharacterState *In, int CurrentSection, intptr_t Data);
void StateHandlerWallGrab(CharacterState *In, int CurrentSection, intptr_t Data);
void StateHandlerWallDismount(CharacterState *In, int CurrentSection, intptr_t Data);

#endif
