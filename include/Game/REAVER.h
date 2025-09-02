#ifndef _REAVER_H_
#define _REAVER_H_

#include "common.h"

void CollideReaverProjectile(Instance *instance, GameTracker *gameTracker);
void _SoulReaverAnimate(Instance *instance);
unsigned long REAVER_GetGlowColor(Instance *instance);
int SoulReaverFire();

#endif
