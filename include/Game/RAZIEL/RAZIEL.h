#ifndef RAZIEL_H_
#define RAZIEL_H_

#include "common.h"

void RAZIEL_DebugHurtRaziel();
void RAZIEL_DebugHealthSetScale(long healthScale);
void RAZIEL_DebugManaSetMax(long manaScale);
void RAZIEL_DebugHealthFillUp();
void RAZIEL_DebugManaFillUp();
void RAZIEL_ResetLookAround(Instance *instance);
void RAZIEL_SetLookAround(Instance *instance);
void RAZIEL_TurnHead(Instance *instance, short *rotx, short *rotz, GameTracker *gameTracker);
long RAZIEL_OkToLookAround(Instance *playerInstance);
Instance *RAZIEL_QueryEngagedInstance(int index);
void StateInitIdle(CharacterState *In, int CurrentSection, intptr_t Ptr);
void StateHandlerDeCompression(CharacterState *In, int CurrentSection, intptr_t Data);
void ProcessHints();
void ProcessTimers(Instance *instance);
int SetStates(Instance *instance, GameTracker *GT, long *controlCommand, int AnalogLength);
long RazielAnimCallbackDuringPause(G2Anim *anim, int sectionID, G2AnimCallbackMsg message, long messageDataA, long messageDataB, void *data);
void StateHandlerDropAction(CharacterState *In, int CurrentSection, intptr_t Data);
void StateHandlerIdle(CharacterState *In, int CurrentSection, intptr_t Data);
void DefaultStateHandler(CharacterState *In, int CurrentSection, intptr_t Data);
void StateHandlerLookAround(CharacterState *In, int CurrentSection, intptr_t Data);
void StateHandlerFall(CharacterState *In, int CurrentSection, intptr_t Data);
void StateHandlerMove(CharacterState *In, int CurrentSection, intptr_t Data);
void StateHandlerStopMove(CharacterState *In, int CurrentSection, intptr_t Data);
void StateHandlerAutoFace(CharacterState *In, int CurrentSection, intptr_t Data);
void StateHandlerCompression(CharacterState *In, int CurrentSection, intptr_t Data);
void DisableWristCollision(Instance *instance, int Side);
void StateHandlerGlyphs(CharacterState *In, int CurrentSection, intptr_t Data);
void StateHandlerSoulSuck(CharacterState *In, int CurrentSection, intptr_t Data);
void SetTimer(int ticks);
long RazielAnimCallback(G2Anim *anim, int sectionID, G2AnimCallbackMsg message, long messageDataA, long messageDataB, void *data);
void InitGlyphSystem(Instance *instance);
void RazielCollide(Instance *instance, GameTracker *gameTracker);
void RelocateConstrict(SVector *offset);
void TrailWaterFX(Instance *instance, int Segment, int Bubbles, int Type);
void StateHandlerCrouch(CharacterState *In, int CurrentSection, intptr_t Data);
void StateHandlerStartMove(CharacterState *In, int CurrentSection, intptr_t Data);
void StateHandlerJump(CharacterState *In, int CurrentSection, intptr_t Data);
int GetControllerMessages(long *controlCommand);
void ProcessConstrict();
void ProcessEffects(Instance *instance);
void ProcessInteractiveMusic(Instance *instance);
void ProcessSpecialAbilities(Instance *instance);
void StateHandlerGlide(CharacterState *In, int CurrentSection, intptr_t Data);
int SetupReaction(Instance *player, Instance *instance);
void StateHandlerStartTurn(CharacterState *In, int CurrentSection, intptr_t Data);
void StateHandlerPushObject(CharacterState *In, int CurrentSection, intptr_t Data);
void StateHandlerDragObject(CharacterState *In, int CurrentSection, intptr_t Data);
void StateHandlerPullSwitch(CharacterState *In, int CurrentSection, intptr_t Data);
void StateHandlerBreakOff(CharacterState *In, int CurrentSection, intptr_t Data);
int CheckHolding(Instance *instance);
void EnableWristCollision(Instance *instance, int Side);
void StateHandlerHang(CharacterState *In, int CurrentSection, intptr_t Data);
void RazielInit(Instance *instance, GameTracker *gameTracker);
void RazielProcess(Instance *playerInstance, GameTracker *gameTracker);
unsigned long RazielQuery(Instance *instance, unsigned long Query);
void RazielPost(Instance *instance, unsigned long Message, uintptr_t Data);
void RazielAdditionalCollide(Instance *instance, GameTracker *gameTracker);

#endif
