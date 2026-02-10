#ifndef _REAVER_H_
#define _REAVER_H_

#include "common.h"

void CollideReaverProjectile(Instance *instance, GameTracker *gameTracker);
void _SoulReaverAnimate(Instance *instance);
unsigned long REAVER_GetGlowColor(Instance *instance);
int SoulReaverFire();
void SoulReaverInit(Instance *instance, GameTracker *gameTracker);
void SoulReaverProcess(Instance *instance, GameTracker *gameTracker);
void SoulReaverCollide(Instance *instance, GameTracker *gameTracker);
unsigned long SoulReaverQuery(Instance *instance, unsigned long query);
void SoulReaverPost(Instance *instance, unsigned long message, unsigned long data);

#endif
