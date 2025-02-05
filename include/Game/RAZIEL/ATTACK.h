#ifndef ATTACK_H_
#define ATTACK_H_

#include "common.h"

void PointAt(Instance *instance, Position *Target, Rotation *Rot1);
void StateHandlerHitReaction(CharacterState *In, int CurrentSection, intptr_t Data);
void LimitRotation(Rotation *rot);
void StateHandlerGrab(CharacterState *In, int CurrentSection, intptr_t Data);
void StateHandlerCannedReaction(CharacterState *In, int CurrentSection, intptr_t Data);
void StateHandlerThrow2(CharacterState *In, int CurrentSection, intptr_t Data);
int StateHandlerDecodeHold(int *Message, int *Data);

#endif
