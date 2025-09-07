#ifndef _STATE_H_
#define _STATE_H_

#include "common.h"

Message *DeMessageQueue(MessageQueue *In);
void EnMessageQueueData(MessageQueue *In, int ID, intptr_t Data);
intptr_t SetMonsterHitData(Instance *Sender, Instance *lastHit, int Power, int knockBackDistance, int knockBackFrames);
intptr_t SetFXHitData(Instance *hitter, int segment, int amount, int type);
intptr_t SetObjectData(int x, int y, int PathNumber, Instance *Force, int node);
intptr_t SetObjectAbsorbData(Instance *force, unsigned short node, unsigned short steps);
void G2EmulationInstanceSetMode(Instance *instance, int CurrentSection, int Mode);
void G2EmulationInstanceSetAnimation(Instance *instance, int CurrentSection, int NewAnim, int NewFrame, int Frames);
int G2EmulationInstanceQueryFrame(Instance *instance, int CurrentSection);
int G2EmulationInstanceQueryAnimation(Instance *instance, int CurrentSection);
int G2EmulationInstanceQueryPassedFrame(Instance *instance, int CurrentSection, int frame);
void G2EmulationInstancePlayAnimation(Instance *instance);
intptr_t SetPositionData(int x, int y, int z);
void *CIRC_Alloc(int size);
void InitMessageQueue(MessageQueue *In);
void G2EmulationInstanceSetTotalSections(Instance *instance, short Total);
void G2EmulationInstanceSetStartAndEndSegment(Instance *instance, int CurrentSection, short Start, short End);
intptr_t SetControlSaveDataData(long length, void *data);
void StateSwitchStateData(CharacterState *In, int CurrentSection, void (*NewProcess)(), intptr_t Data);
void PurgeMessageQueue(MessageQueue *In);
Message *PeekMessageQueue(MessageQueue *In);
intptr_t SetActionPlayHostAnimationData(Instance *instance, Instance *host, int newAnim, int newFrame, int frames, int mode);
void G2EmulationInstanceSwitchAnimationAlpha(Instance *instance, int CurrentSection, int NewAnim, int NewFrame, int Frames, int Mode, int AlphaTable);
intptr_t SetObjectThrowData(void *target, SVector *angularVel, unsigned short type, unsigned short spinType, unsigned short speed, short gravity, short zVel, short initialXRot);
intptr_t SetObjectBreakOffData(Instance *force, short node, short distance, short animation, int frame, int type, int action);
intptr_t SetCollideInfoData(CollideInfo *srcCI);
intptr_t SetShadowSegmentData(unsigned long total);
void G2EmulationInstanceSetAnimSpeed(Instance *instance, int CurrentSection, int speed);
void G2EmulationInstanceInitSection(Instance *instance, int CurrentSection, void *callback, void *data);
intptr_t SetPhysicsSwimData(int CheckDepth, SVector *iVelocity, int SwimDepth, int WadeDepth, int TreadDepth);
intptr_t SetPhysicsDropHeightData(Position *offset, int dropOffset, int mode);
intptr_t SetPhysicsLinkedMoveData(Instance *instance, int segment, SVector *posDelta, SVector *rotDelta);
void G2EmulationSetInterpController_Vector(Instance *instance, long segment, long type, G2SVector3 *vec, int Frames, intptr_t Data);
intptr_t SetMonsterAlarmData(Instance *sender, Position *position, int type);
intptr_t SetMonsterSoulSuckData(Instance *Sender, int x, int y, int z);
intptr_t SetPhysicsSlideData(int Segment, int ForwardVectorX, int ForwardVectorY, int ForwardVectorZ, int DropOffset, int UpperOffset, int Height);
intptr_t SetObjectBirthProjectileData(Instance *instance, int joint, int type);
intptr_t SetObjectIdleData(long mode, Instance *instance);
intptr_t SetPhysicsGravityData(int UpperOffset, int LowerOffset, int x, int y, int z, int slope);
intptr_t SetObjectDraftData(short force, unsigned short radius, unsigned short radiusCoef, unsigned short height, int maxVelocity);
void G2EmulationSwitchAnimationSync(CharacterState *In, int SlaveSectionID, int MasterSectionID, int Frames);
intptr_t SetControlInitIdleData(int mode, long frame, long frames);
intptr_t SetMonsterImpaleData(Instance *weapon, Rotation *direction, Position *position, int distance);
void G2EmulationSwitchAnimation(CharacterState *In, int CurrentSection, int NewAnim, int NewFrame, int Frames, int Mode);
void StateSwitchStateCharacterData(CharacterState *In, void (*NewProcess)(), intptr_t Data);
void G2EmulationInstanceToInstanceSwitchAnimationCharacter(Instance *instance, Instance *host, int NewAnim, int NewFrame, int Frames, int Mode);
int G2EmulationQueryAnimation(CharacterState *In, int CurrentSection);
void G2EmulationSwitchAnimationAlpha(CharacterState *In, int CurrentSection, int NewAnim, int NewFrame, int Frames, int Mode, int AlphaTable);
void StateSwitchStateCharacterDataDefault(CharacterState *In, void (*NewProcess)(), intptr_t Data);
int G2EmulationQueryFrame(CharacterState *In, int CurrentSection);
int G2EmulationInstanceQueryLastFrame(Instance *instance, int CurrentSection);
void G2EmulationSwitchAnimationCharacter(CharacterState *In, int NewAnim, int NewFrame, int Frames, int Mode);
void G2EmulationSetTotalSections(CharacterState *In, short Total);
void G2EmulationSetStartAndEndSegment(CharacterState *In, int CurrentSection, short Start, short End);
void G2EmulationSetAnimation(CharacterState *In, int CurrentSection, int NewAnim, int NewFrame, int Frames);
void G2EmulationSetMode(CharacterState *In, int CurrentSection, int Mode);
void G2EmulationInit();
void G2EmulationInstanceToInstanceSwitchAnimation(Instance *instance, Instance *host, int CurrentSection, int NewAnim, int NewFrame, int Frames, int Mode);
intptr_t SetPhysicsEdgeData(int UpperOffset, int ForwardOffset, int AboveOffset, int x, int y, int z, SVector *Normal1, SVector *Normal2, SVector *Delta);
intptr_t SetAnimationControllerDoneData(Instance *instance, long segment, long type, intptr_t data);
void EnMessageQueue(MessageQueue *In, Message *Element);
intptr_t SetAnimationInstanceSwitchData(Instance *instance, int anim, int frame, int frames, int mode);
void StateGovernState(CharacterState *In, int Frames);
void G2EmulatePlayAnimation(CharacterState *In);
int G2EmulationQueryMode(CharacterState *In, int CurrentSection);
void StateSwitchStateDataDefault(CharacterState *In, int CurrentSection, void (*NewProcess)(), intptr_t Data);
intptr_t SetControlInitHangData(Instance *instance, long frame, long frames);
intptr_t SetPhysicsDropOffData(int xOffset, int yOffset, int DropOffset, int slipSlope, int UpperOffset);
intptr_t SetPhysicsWallCrawlData(int Segment, int Length, int ForwardOffset, int NormalDistance);

#endif
