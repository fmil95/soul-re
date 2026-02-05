#ifndef RAZLIB_H_
#define RAZLIB_H_

#include "common.h"

void razAlignYRotMove(Instance *dest, short distance, Position *position, Rotation *rotation, int extraZ);
void razAlignYRotInterp(Instance *source, Position *dest, unsigned char segNumber, int Frames);
void razClearPlayerEvent();
void StateHandlerPickupObject(CharacterState *In, int CurrentSection, intptr_t Data);
void razSwitchVAnim(Instance *instance, int section, VAnim *vAnim, int frame, int frames);
Instance *razGetHeldWeapon();
void razSetupSoundRamp(Instance *instance, SoundRamp *sound, int sfx, int startPitch, int endPitch, int startVolume, int endVolume, int timer, int distance);
void razSpectralShift();
void razSetPlayerEventHistory(unsigned long event);
int razReaverOn();
Instance *razGetHeldItem();
void razResetMotion(Instance *instance);
void razSetFadeEffect(short source, short dest, int steps);
void razResetPauseTranslation(Instance *instance);
void razSetPauseTranslation(Instance *instance);
int razSwitchVAnimCharacterGroup(Instance *instance, int animGroup, int *frame, int *frames);
void razLaunchForce(Instance *instance);
void razSwitchVAnimCharacterSingle(Instance *instance, int anim, int *frame, int *frames);
void razAlignYRotMoveInterp(Instance *source, Instance *dest, short distance, unsigned char segNumber, int Frames, short extraZ);
void razMaterialShift();
int razInBaseArea(char *name, int length);
void razReaverImbue(int reaverType);
int razReaverOff();
int razPlaneShift(Instance *instance);
void razReaverBladeOff();
void razSetWallCrawlNodes(Instance *instance, evPhysicsWallCrawlData *data);
int razZeroAxis(long *x, long *y, int radius);
void razReaverPickup(Instance *instance, Instance *soulReaver);
int razConstrictAngle(Instance *instance);
void razLaunchBubbles(int segments, int count, int type);
void razSetSwimVelocity(Instance *instance, int vel, int accl);
int razPickupAndGrab(CharacterState *In, int CurrentSection);
int razGetReaverFromMask(int reaverMask);
void razReaverBladeOn();
void razSetCowlNoDraw(int mode);
void razAttachControllers();
int razApplyMotion(CharacterState *In, int CurrentSection);
int razSwitchVAnimGroup(Instance *instance, int section, int animGroup, int frame, int frames);
int razUpdateSoundRamp(Instance *instance, SoundRamp *sound);
void razSelectMotionAnim(CharacterState *In, int CurrentSection, int Frames, int *Anim);
void razCenterWithBlock(Instance *inst, Instance *target, int dist);
void RAZIEL_StartNewGame();
void razSwitchVAnimSingle(Instance *instance, int section, int anim, int frame, int frames);
int razSideMoveSpiderCheck(Instance *instance, int x);

#endif
