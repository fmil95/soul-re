#ifndef _SKINNER_H_
#define _SKINNER_H_

#include "common.h"

void SKINNER_HideEntry(Instance *instance);
void SKINNER_Hide(Instance *instance);
void SKINNER_PursueEntry(Instance *instance);
void SKINNER_Pursue(Instance *instance);
void SKINNER_SurpriseAttackEntry(Instance *instance);
void SKINNER_SurpriseAttack(Instance *instance);
void SKINNER_PupateEntry(Instance *instance);
void SKINNER_Pupate(Instance *instance);

enum SkinnerAnim
{
    SKINNER_ANIM_BURROW_IN = 0,
    SKINNER_ANIM_BURROW_OUT = 1,
    SKINNER_NUM_ANIMS = 2,
};

#endif
