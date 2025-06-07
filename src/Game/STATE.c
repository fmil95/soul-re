#include "Game/STATE.h"
#include "Game/G2/ANIMG2.h"
#include "Game/G2/ANMCTRLR.h"
#include "Game/MEMPACK.h"
#include "Game/G2/INSTNCG2.h"
#include "Game/INSTANCE.h"
#include "Game/G2/ANMG2ILF.h"

static char circBuf[4096];

static G2AnimAlphaTable *G2AlphaTables[7] = {0};

static void *circWhere = circBuf;

void InitMessageQueue(MessageQueue *In)
{
    In->Head = 0;
    In->Tail = 0;
}

Message *DeMessageQueue(MessageQueue *In)
{
    int head;

    head = In->Head;

    if (In->Head == In->Tail)
    {
        return NULL;
    }
    else if (++In->Head == 16)
    {
        In->Head = 0;
    }

    return &In->Queue[head];
}

void PurgeMessageQueue(MessageQueue *In)
{
    In->Tail = 0;
    In->Head = 0;
}

Message *PeekMessageQueue(MessageQueue *In)
{
    if (In->Head == In->Tail)
    {
        return NULL;
    }

    return &In->Queue[In->Head];
}

void EnMessageQueue(MessageQueue *In, Message *Element)
{
    EnMessageQueueData(In, Element->ID, Element->Data);
}

void EnMessageQueueData(MessageQueue *In, int ID, intptr_t Data)
{
    int i;

    In->Queue[In->Tail].ID = ID;
    In->Queue[In->Tail].Data = Data;

    if (++In->Tail == 16)
    {
        In->Tail = 0;
    }

    i = In->Head;

    if (i == In->Tail)
    {
        do
        {
            if (++i == 16)
            {
                i = 0;
            }
        } while (i != In->Tail);
    }
}

void *CIRC_Alloc(int size)
{
    void *ret;

    size = (size + 3) & ~3;

    if ((circBuf + sizeof(circBuf)) < ((char *)circWhere + size))
    {
        ret = circBuf;

        circWhere = circBuf + size;
    }
    else
    {
        ret = (char *)circWhere;

        circWhere = (char *)circWhere + size;
    }

    return ret;
}

intptr_t SetCollideInfoData(CollideInfo *srcCI)
{
    CollideInfo *Ptr;

    Ptr = (CollideInfo *)CIRC_Alloc(sizeof(CollideInfo));

    *Ptr = *srcCI;

    return (intptr_t)Ptr;
}

intptr_t SetMonsterHitData(Instance *Sender, Instance *lastHit, int Power, int knockBackDistance, int knockBackFrames)
{
    evMonsterHitData *Ptr;

    Ptr = (evMonsterHitData *)CIRC_Alloc(sizeof(evMonsterHitData));

    Ptr->sender = Sender;

    Ptr->lastHit = lastHit;

    Ptr->power = Power;

    Ptr->knockBackDistance = knockBackDistance;
    Ptr->knockBackDuration = knockBackFrames;

    return (intptr_t)Ptr;
}

intptr_t SetMonsterThrownData(Instance *Sender, Rotation *Direction, int Power)
{
    evMonsterThrownData *Ptr;

    Ptr = (evMonsterThrownData *)CIRC_Alloc(sizeof(evMonsterThrownData));

    Ptr->sender = Sender;

    Ptr->direction.x = Direction->x;
    Ptr->direction.y = Direction->y;
    Ptr->direction.z = Direction->z;

    Ptr->power = Power;

    return (intptr_t)Ptr;
}

intptr_t SetMonsterAlarmData(Instance *sender, Position *position, int type)
{
    evMonsterAlarmData *Ptr;

    Ptr = (evMonsterAlarmData *)CIRC_Alloc(sizeof(evMonsterAlarmData));

    Ptr->sender = sender;

    Ptr->position.x = position->x;
    Ptr->position.y = position->y;
    Ptr->position.z = position->z;

    Ptr->type = type;

    return (intptr_t)Ptr;
}

intptr_t SetMonsterSoulSuckData(Instance *Sender, int x, int y, int z)
{
    evMonsterSoulSuckData *Ptr;

    Ptr = (evMonsterSoulSuckData *)CIRC_Alloc(sizeof(evMonsterSoulSuckData));

    Ptr->sender = Sender;

    Ptr->Destination.x = x;
    Ptr->Destination.y = y;
    Ptr->Destination.z = z;

    return (intptr_t)Ptr;
}

intptr_t SetMonsterImpaleData(Instance *weapon, Rotation *direction, Position *position, int distance)
{
    evMonsterImpaleData *Ptr;

    Ptr = (evMonsterImpaleData *)CIRC_Alloc(sizeof(evMonsterImpaleData));

    Ptr->weapon = weapon;

    Ptr->direction.x = direction->x;
    Ptr->direction.y = direction->y;
    Ptr->direction.z = direction->z;

    Ptr->position.x = position->x;
    Ptr->position.y = position->y;
    Ptr->position.z = position->z;

    Ptr->distance = distance;

    return (intptr_t)Ptr;
}

intptr_t SetObjectData(int x, int y, int PathNumber, Instance *Force, int node)
{
    evObjectData *Ptr;

    Ptr = (evObjectData *)CIRC_Alloc(sizeof(evObjectData));

    Ptr->x = x;
    Ptr->y = y;

    Ptr->PathNumber = PathNumber;

    Ptr->LinkNode = node;

    Ptr->Force = Force;

    return (intptr_t)Ptr;
}

intptr_t SetPositionData(int x, int y, int z)
{
    evPositionData *Ptr;

    Ptr = (evPositionData *)CIRC_Alloc(sizeof(evPositionData));

    Ptr->x = x;
    Ptr->y = y;
    Ptr->z = z;

    return (intptr_t)Ptr;
}

intptr_t SetPhysicsGravityData(int UpperOffset, int LowerOffset, int x, int y, int z, int slope)
{
    evPhysicsGravityData *Ptr;

    Ptr = (evPhysicsGravityData *)CIRC_Alloc(sizeof(evPhysicsGravityData));

    Ptr->UpperOffset = UpperOffset;
    Ptr->LowerOffset = LowerOffset;

    Ptr->x = x;
    Ptr->y = y;
    Ptr->z = z;

    Ptr->slipSlope = slope;

    return (intptr_t)Ptr;
}

intptr_t SetPhysicsEdgeData(int UpperOffset, int ForwardOffset, int AboveOffset, int x, int y, int z, SVector *Normal1, SVector *Normal2, SVector *Delta)
{
    evPhysicsEdgeData *Ptr;

    Ptr = (evPhysicsEdgeData *)CIRC_Alloc(sizeof(evPhysicsEdgeData));

    Ptr->UpperOffset = UpperOffset;
    Ptr->ForwardOffset = ForwardOffset;
    Ptr->AboveOffset = AboveOffset;

    Ptr->XDistance = x;
    Ptr->YDistance = y;
    Ptr->ZDistance = z;

    Ptr->Normal1 = Normal1;
    Ptr->Normal2 = Normal2;

    Ptr->Delta = Delta;

    return (intptr_t)Ptr;
}

intptr_t SetPhysicsSwimData(int CheckDepth, SVector *iVelocity, int SwimDepth, int WadeDepth, int TreadDepth)
{
    evPhysicsSwimData *Ptr;

    Ptr = (evPhysicsSwimData *)CIRC_Alloc(sizeof(evPhysicsSwimData));

    Ptr->CheckDepth = CheckDepth;

    Ptr->iVelocity = iVelocity;

    Ptr->SwimDepth = SwimDepth;
    Ptr->WadeDepth = WadeDepth;
    Ptr->TreadDepth = TreadDepth;

    return (intptr_t)Ptr;
}

intptr_t SetPhysicsDropOffData(int xOffset, int yOffset, int DropOffset, int slipSlope, int UpperOffset)
{
    evPhysicsDropOffData *Ptr;

    Ptr = (evPhysicsDropOffData *)CIRC_Alloc(sizeof(evPhysicsDropOffData));

    Ptr->xOffset = xOffset;
    Ptr->yOffset = yOffset;

    Ptr->DropOffset = DropOffset;

    Ptr->slipSlope = slipSlope;

    Ptr->UpperOffset = UpperOffset;

    return (intptr_t)Ptr;
}

intptr_t SetPhysicsSlideData(int Segment, int ForwardVectorX, int ForwardVectorY, int ForwardVectorZ, int DropOffset, int UpperOffset, int Height)
{
    evPhysicsSlideData *Ptr;

    Ptr = (evPhysicsSlideData *)CIRC_Alloc(sizeof(evPhysicsSlideData));

    Ptr->Segment = Segment;

    Ptr->ForwardVector.x = ForwardVectorX;
    Ptr->ForwardVector.y = ForwardVectorY;
    Ptr->ForwardVector.z = ForwardVectorZ;

    Ptr->DropOffset = DropOffset;
    Ptr->UpperOffset = UpperOffset;

    Ptr->Height = Height;

    return (intptr_t)Ptr;
}

intptr_t SetPhysicsWallCrawlData(int Segment, int Length, int ForwardOffset, int NormalDistance)
{
    evPhysicsWallCrawlData *Ptr;

    Ptr = (evPhysicsWallCrawlData *)CIRC_Alloc(sizeof(evPhysicsWallCrawlData));

    Ptr->Segment = Segment;

    Ptr->Length = Length;

    Ptr->ForwardOffset = ForwardOffset;

    Ptr->NormalDistance = NormalDistance;

    return (intptr_t)Ptr;
}

intptr_t SetPhysicsLinkedMoveData(Instance *instance, int segment, SVector *posDelta, SVector *rotDelta)
{
    evPhysicsLinkedMoveData *Ptr;

    Ptr = (evPhysicsLinkedMoveData *)CIRC_Alloc(sizeof(evPhysicsLinkedMoveData));

    Ptr->segment = segment;

    Ptr->instance = instance;

    if (posDelta != NULL)
    {
        Ptr->posDelta = *posDelta;
    }

    if (rotDelta != NULL)
    {
        Ptr->rotDelta = *rotDelta;
    }

    return (intptr_t)Ptr;
}

intptr_t SetPhysicsDropHeightData(Position *offset, int dropOffset, int mode)
{
    evPhysicsDropHeightData *ptr;

    ptr = (evPhysicsDropHeightData *)CIRC_Alloc(sizeof(evPhysicsDropHeightData));

    ptr->DropOffset = dropOffset;

    ptr->mode = mode;

    ptr->origin.x = offset->x;
    ptr->origin.y = offset->y;
    ptr->origin.z = offset->z + 25;

    return (intptr_t)ptr;
}

intptr_t SetAnimationControllerDoneData(Instance *instance, long segment, long type, intptr_t data)
{
    evAnimationControllerDoneData *Ptr;

    Ptr = (evAnimationControllerDoneData *)CIRC_Alloc(sizeof(evAnimationControllerDoneData));

    Ptr->instance = instance;

    Ptr->segment = segment;

    Ptr->type = type;

    Ptr->data = data;

    return (intptr_t)Ptr;
}

intptr_t SetAnimationInstanceSwitchData(Instance *instance, int anim, int frame, int frames, int mode)
{
    evAnimationInstanceSwitchData *Ptr;

    Ptr = (evAnimationInstanceSwitchData *)CIRC_Alloc(sizeof(evAnimationInstanceSwitchData));

    Ptr->instance = instance;

    Ptr->anim = anim;

    Ptr->frame = frame;

    Ptr->frames = frames;

    Ptr->mode = mode;

    return (intptr_t)Ptr;
}

intptr_t SetFXHitData(Instance *hitter, int segment, int amount, int type)
{
    evFXHitData *Ptr;

    Ptr = (evFXHitData *)CIRC_Alloc(sizeof(evFXHitData));

    Ptr->amount = amount;

    Ptr->type = type;

    if (hitter != NULL)
    {
        Ptr->location.x = hitter->matrix[segment].t[0];
        Ptr->velocity.x = Ptr->location.x - hitter->oldMatrix[segment].t[0];

        Ptr->location.y = hitter->matrix[segment].t[1];
        Ptr->velocity.y = Ptr->location.y - hitter->oldMatrix[segment].t[1];

        Ptr->location.z = hitter->matrix[segment].t[2];
        Ptr->velocity.z = Ptr->location.z - hitter->oldMatrix[segment].t[2];
    }

    return (intptr_t)Ptr;
}

intptr_t SetObjectThrowData(void *target, SVector *angularVel, unsigned short type, unsigned short spinType, unsigned short speed, short gravity, short zVel, short initialXRot)
{
    evObjectThrowData *Ptr;

    Ptr = (evObjectThrowData *)CIRC_Alloc(sizeof(evObjectThrowData));

    Ptr->type = type;

    Ptr->spinType = spinType;

    if (target == NULL)
    {
        Ptr->type = 0;
    }
    else
    {
        switch (type)
        {
        case 0:
            break;
        case 1:
            Ptr->data.target = (Instance *)target;
            break;
        case 3:
            Ptr->data.direction = *(Rotation *)target;
            break;
        case 2:
        case 4:
            Ptr->data.throwVector = *(Position *)target;
            break;
        }
    }

    switch (spinType)
    {
    case 0:
        break;
    case 1:
        break;
    case 2:
        if (angularVel == NULL)
        {
            Ptr->spinType = 0;
        }
        else
        {
            Ptr->angularVel = *angularVel;
        }

        break;
    }

    Ptr->speed = speed;

    Ptr->gravity = gravity;

    Ptr->zVel = zVel;

    Ptr->initialXRot = initialXRot;

    return (intptr_t)Ptr;
}

intptr_t SetObjectBreakOffData(Instance *force, short node, short distance, short animation, int frame, int type, int action)
{
    evObjectBreakOffData *Ptr;

    Ptr = (evObjectBreakOffData *)CIRC_Alloc(sizeof(evObjectBreakOffData));

    Ptr->force = force;

    Ptr->node = node;

    Ptr->distance = distance;

    Ptr->animation = animation;

    Ptr->frame = frame;

    Ptr->type = type;

    Ptr->action = action;

    return (intptr_t)Ptr;
}

intptr_t SetControlInitHangData(Instance *instance, long frame, long frames)
{
    evControlInitHangData *Ptr;

    Ptr = (evControlInitHangData *)CIRC_Alloc(sizeof(evControlInitHangData));

    Ptr->instance = instance;

    Ptr->frame = frame;

    Ptr->frames = frames;

    return (intptr_t)Ptr;
}

intptr_t SetControlInitIdleData(int mode, long frame, long frames)
{
    evControlInitIdleData *Ptr;

    Ptr = (evControlInitIdleData *)CIRC_Alloc(sizeof(evControlInitIdleData));

    Ptr->mode = mode;

    Ptr->frame = frame;

    Ptr->frames = frames;

    return (intptr_t)Ptr;
}

intptr_t SetObjectDraftData(short force, unsigned short radius, unsigned short radiusCoef, unsigned short height, int maxVelocity)
{
    evObjectDraftData *Ptr;

    Ptr = (evObjectDraftData *)CIRC_Alloc(sizeof(evObjectDraftData));

    Ptr->force = force;

    Ptr->radius = radius;
    Ptr->radiusCoef = radiusCoef;

    Ptr->height = height;

    Ptr->maxVelocity = maxVelocity;

    return (intptr_t)Ptr;
}

intptr_t SetObjectAbsorbData(Instance *force, unsigned short node, unsigned short steps)
{
    evObjectAbsorbData *Ptr;

    Ptr = (evObjectAbsorbData *)CIRC_Alloc(sizeof(evObjectAbsorbData));

    Ptr->force = force;

    Ptr->node = node;

    Ptr->steps = steps;

    return (intptr_t)Ptr;
}

intptr_t SetControlSaveDataData(long length, void *data)
{
    evControlSaveDataData *Ptr;

    Ptr = (evControlSaveDataData *)CIRC_Alloc(sizeof(evControlSaveDataData));

    Ptr->length = length;

    Ptr->data = data;

    return (intptr_t)Ptr;
}

intptr_t SetObjectIdleData(long mode, Instance *instance)
{
    evObjectIdleData *Ptr;

    Ptr = (evObjectIdleData *)CIRC_Alloc(sizeof(evObjectIdleData));

    Ptr->mode = mode;

    Ptr->instance = instance;

    return (intptr_t)Ptr;
}

intptr_t SetActionPlayHostAnimationData(Instance *instance, Instance *host, int newAnim, int newFrame, int frames, int mode)
{
    evActionPlayHostAnimationData *Ptr;

    Ptr = (evActionPlayHostAnimationData *)CIRC_Alloc(sizeof(evActionPlayHostAnimationData));

    Ptr->instance = instance;

    Ptr->host = host;

    Ptr->newAnim = newAnim;

    Ptr->newFrame = newFrame;

    Ptr->frames = frames;

    Ptr->mode = mode;

    return (intptr_t)Ptr;
}

intptr_t SetObjectBirthProjectileData(Instance *instance, int joint, int type)
{
    evObjectBirthProjectileData *Ptr;

    Ptr = (evObjectBirthProjectileData *)CIRC_Alloc(sizeof(evObjectBirthProjectileData));

    Ptr->instance = instance;

    Ptr->joint = joint;

    Ptr->type = type;

    Ptr->birthInstance = NULL;

    return (intptr_t)Ptr;
}

intptr_t SetShadowSegmentData(unsigned long total)
{
    evShadowSegmentData *Ptr;

    Ptr = (evShadowSegmentData *)CIRC_Alloc(sizeof(evShadowSegmentData));

    Ptr->totalShadowSegments = total;

    return (intptr_t)Ptr;
}

void G2EmulationInit()
{
    G2AlphaTables[0] = NULL;

    if (G2AlphaTables[1] == NULL)
    {
        G2AlphaTables[1] = (G2AnimAlphaTable *)MEMPACK_Malloc(18, 25); // TODO: find struct sizeof for allocSize parameter

        G2AlphaTables[1]->size = 4;

        G2AlphaTables[1]->data[0] = 0;
        G2AlphaTables[1]->data[1] = 256;
        G2AlphaTables[1]->data[2] = 1024;
        G2AlphaTables[1]->data[3] = 4096;
    }

    if (G2AlphaTables[2] == NULL)
    {
        G2AlphaTables[2] = (G2AnimAlphaTable *)MEMPACK_Malloc(18, 25);

        G2AlphaTables[2]->size = 4;

        G2AlphaTables[2]->data[0] = 0;
        G2AlphaTables[2]->data[1] = 3072;
        G2AlphaTables[2]->data[2] = 3840;
        G2AlphaTables[2]->data[3] = 4096;
    }

    if (G2AlphaTables[3] == NULL)
    {
        G2AlphaTables[3] = (G2AnimAlphaTable *)MEMPACK_Malloc(6, 25); // TODO: find struct sizeof for allocSize parameter

        G2AlphaTables[3]->size = 1;

        G2AlphaTables[3]->data[0] = 4096;
    }

    if (G2AlphaTables[4] == NULL)
    {
        G2AlphaTables[4] = (G2AnimAlphaTable *)MEMPACK_Malloc(18, 25);

        G2AlphaTables[4]->size = 4;

        G2AlphaTables[4]->data[0] = 0;
        G2AlphaTables[4]->data[1] = 256;
        G2AlphaTables[4]->data[2] = 3840;
        G2AlphaTables[4]->data[3] = 4096;
    }

    if (G2AlphaTables[5] == NULL)
    {
        G2AlphaTables[5] = (G2AnimAlphaTable *)MEMPACK_Malloc(18, 25);

        G2AlphaTables[5]->size = 4;

        G2AlphaTables[5]->data[0] = 0;
        G2AlphaTables[5]->data[1] = 128;
        G2AlphaTables[5]->data[2] = 512;
        G2AlphaTables[5]->data[3] = 4096;
    }

    if (G2AlphaTables[6] == NULL)
    {
        G2AlphaTables[6] = (G2AnimAlphaTable *)MEMPACK_Malloc(18, 25);

        G2AlphaTables[6]->size = 4;

        G2AlphaTables[6]->data[0] = 0;
        G2AlphaTables[6]->data[1] = 3584;
        G2AlphaTables[6]->data[2] = 3968;
        G2AlphaTables[6]->data[3] = 4096;
    }
}

void G2EmulationInstancePlayAnimation(Instance *instance)
{
    int i;
    G2Anim *anim;

    anim = &instance->anim;

    for (i = 0; i < anim->sectionCount; i++)
    {
        G2AnimSection_NextKeyframe(&anim->section[i & 0xFF]);
    }
}

void G2EmulatePlayAnimation(CharacterState *In)
{
    G2EmulationInstancePlayAnimation(In->CharacterInstance);
}

void G2EmulationInstanceToInstanceSwitchAnimation(Instance *instance, Instance *host, int CurrentSection, int NewAnim, int NewFrame, int Frames, int Mode)
{
    G2AnimSection *animSection;
    G2AnimKeylist *keylist;

    animSection = &instance->anim.section[CurrentSection];

    keylist = G2Instance_GetKeylist(host, NewAnim);

    G2AnimSection_SetAlphaTable(animSection, NULL);

    G2AnimSection_InterpToKeylistFrame(animSection, keylist, NewAnim, NewFrame, Frames * 100);

    if (Mode == 0)
    {
        G2AnimSection_SetPaused(animSection);
    }
    else
    {
        G2AnimSection_SetUnpaused(animSection);

        if (Mode == 2)
        {
            G2AnimSection_SetLooping(animSection);
        }
        else
        {
            G2AnimSection_SetNoLooping(animSection);
        }
    }
}

void G2EmulationInstanceSwitchAnimation(Instance *instance, int CurrentSection, int NewAnim, int NewFrame, int Frames, int Mode)
{
    G2AnimSection *animSection;
    G2AnimKeylist *keylist;

    animSection = &instance->anim.section[CurrentSection];

    keylist = G2Instance_GetKeylist(instance, NewAnim);

    G2AnimSection_SetAlphaTable(animSection, NULL);

    G2AnimSection_InterpToKeylistFrame(animSection, keylist, NewAnim, NewFrame, Frames * 100);

    if (Mode == 0)
    {
        G2AnimSection_SetPaused(animSection);
    }
    else
    {
        G2AnimSection_SetUnpaused(animSection);

        if (Mode == 2)
        {
            G2AnimSection_SetLooping(animSection);
        }
        else
        {
            G2AnimSection_SetNoLooping(animSection);
        }
    }
}

void G2EmulationInstanceSwitchAnimationAlpha(Instance *instance, int CurrentSection, int NewAnim, int NewFrame, int Frames, int Mode, int AlphaTable)
{
    G2AnimSection *animSection;

    animSection = &instance->anim.section[CurrentSection & 0xFF];

    G2EmulationInstanceSwitchAnimation(instance, CurrentSection, NewAnim, NewFrame, Frames, Mode);

    G2AnimSection_SetAlphaTable(animSection, G2AlphaTables[AlphaTable]);
}

void G2EmulationSwitchAnimation(CharacterState *In, int CurrentSection, int NewAnim, int NewFrame, int Frames, int Mode)
{
    if (NewAnim < 0)
    {
        NewAnim = 0;
    }

    G2EmulationInstanceSwitchAnimation(In->CharacterInstance, CurrentSection, NewAnim, NewFrame, Frames, Mode);
}

void G2EmulationSwitchAnimationAlpha(CharacterState *In, int CurrentSection, int NewAnim, int NewFrame, int Frames, int Mode, int AlphaTable)
{
    G2AnimSection *animSection;

    animSection = &In->CharacterInstance->anim.section[CurrentSection & 0xFF];

    G2EmulationInstanceSwitchAnimation(In->CharacterInstance, CurrentSection, NewAnim, NewFrame, Frames, Mode);

    G2AnimSection_SetAlphaTable(animSection, G2AlphaTables[AlphaTable]);
}

void G2EmulationSwitchAnimationSync(CharacterState *In, int SlaveSectionID, int MasterSectionID, int Frames)
{
    Instance *instance;
    G2AnimSection *masterAnimSection;
    G2AnimSection *slaveAnimSection;
    G2AnimKeylist *keylist;
    int keylistID;
    State *masterSection;
    State *slaveSection;
    int targetFrame;

    instance = In->CharacterInstance;

    masterAnimSection = &instance->anim.section[MasterSectionID & 0xFF];
    slaveAnimSection = &instance->anim.section[SlaveSectionID & 0xFF];

    masterSection = &In->SectionList[MasterSectionID];
    slaveSection = &In->SectionList[SlaveSectionID];

    keylist = masterAnimSection->keylist;
    keylistID = masterAnimSection->keylistID;

    targetFrame = (G2AnimSection_GetKeyframeNumber(masterAnimSection) + Frames) % G2AnimKeylist_GetKeyframeCount(keylist);

    G2AnimSection_SetAlphaTable(slaveAnimSection, NULL);

    G2AnimSection_InterpToKeylistFrame(slaveAnimSection, keylist, keylistID, targetFrame, Frames * 100);

    if (!(masterAnimSection->flags & 0x2))
    {
        G2AnimSection_SetNoLooping(slaveAnimSection);
    }
    else
    {
        G2AnimSection_SetLooping(slaveAnimSection);
    }

    if (!(masterAnimSection->flags & 0x1))
    {
        G2AnimSection_SetUnpaused(slaveAnimSection);
    }
    else
    {
        G2AnimSection_SetPaused(slaveAnimSection);
    }

    StateSwitchStateData(In, SlaveSectionID, masterSection->Process, 0);

    slaveSection->Data1 = masterSection->Data1;
    slaveSection->Data2 = masterSection->Data2;
}

void G2EmulationInstanceToInstanceSwitchAnimationCharacter(Instance *instance, Instance *host, int NewAnim, int NewFrame, int Frames, int Mode)
{
    G2AnimKeylist *keylist1;

    keylist1 = G2Instance_GetKeylist(host, NewAnim);

    G2Anim_SetAlphaTable(&instance->anim, NULL);

    G2Anim_InterpToKeylistFrame(&instance->anim, keylist1, NewAnim, NewFrame, Frames * 100);

    if (Mode == 0)
    {
        G2Anim_SetPaused(&instance->anim);
    }
    else
    {
        G2Anim_SetUnpaused(&instance->anim);

        if (Mode == 2)
        {
            G2Anim_SetLooping(&instance->anim);
        }
        else
        {
            G2Anim_SetNoLooping(&instance->anim);
        }
    }
}

void G2EmulationSwitchAnimationCharacter(CharacterState *In, int NewAnim, int NewFrame, int Frames, int Mode)
{
    Instance *instance;
    G2AnimKeylist *keylist;

    if (NewAnim < 0)
    {
        NewAnim = 0;
    }

    instance = In->CharacterInstance;

    keylist = G2Instance_GetKeylist(instance, NewAnim);

    G2Anim_SetAlphaTable(&instance->anim, NULL);

    G2Anim_InterpToKeylistFrame(&instance->anim, keylist, NewAnim, NewFrame, Frames * 100);

    if (Mode == 0)
    {
        G2Anim_SetPaused(&instance->anim);
    }
    else
    {
        G2Anim_SetUnpaused(&instance->anim);

        if (Mode == 2)
        {
            G2Anim_SetLooping(&instance->anim);
        }
        else
        {
            G2Anim_SetNoLooping(&instance->anim);
        }
    }
}

void G2EmulationInstanceSetAnimation(Instance *instance, int CurrentSection, int NewAnim, int NewFrame, int Frames)
{
    G2AnimSection *animSection;
    G2AnimKeylist *keylist;

    animSection = &instance->anim.section[CurrentSection & 0xFF];

    keylist = G2Instance_GetKeylist(instance, NewAnim);

    G2AnimSection_SetAlphaTable(animSection, NULL);

    G2AnimSection_InterpToKeylistFrame(animSection, keylist, NewAnim, NewFrame, Frames * 100);
}

void G2EmulationSetAnimation(CharacterState *In, int CurrentSection, int NewAnim, int NewFrame, int Frames)
{
    if (NewAnim < 0)
    {
        NewAnim = 0;
    }

    G2EmulationInstanceSetAnimation(In->CharacterInstance, CurrentSection, NewAnim, NewFrame, Frames);
}

void G2EmulationInstanceSetMode(Instance *instance, int CurrentSection, int Mode)
{
    G2AnimSection *animSection;

    animSection = &instance->anim.section[CurrentSection & 0xFF];

    if (Mode == 0)
    {
        G2AnimSection_SetPaused(animSection);
    }
    else
    {
        G2AnimSection_SetUnpaused(animSection);

        if (Mode == 1)
        {
            G2AnimSection_SetNoLooping(animSection);
        }
        else
        {
            G2AnimSection_SetLooping(animSection);
        }
    }
}

void G2EmulationSetMode(CharacterState *In, int CurrentSection, int Mode)
{
    G2EmulationInstanceSetMode(In->CharacterInstance, CurrentSection, Mode);
}

void G2EmulationInstanceSetAnimSpeed(Instance *instance, int CurrentSection, int speed)
{
    G2AnimSection *animSection;

    animSection = &instance->anim.section[CurrentSection & 0xFF];

    animSection->speedAdjustment = speed;
}

int G2EmulationInstanceQueryAnimation(Instance *instance, int CurrentSection)
{
    G2AnimSection *animSection;

    animSection = &instance->anim.section[CurrentSection & 0xFF];

    return animSection->keylistID;
}

int G2EmulationQueryAnimation(CharacterState *In, int CurrentSection)
{
    return G2EmulationInstanceQueryAnimation(In->CharacterInstance, CurrentSection);
}

int G2EmulationInstanceQueryFrame(Instance *instance, int CurrentSection)
{
    return G2AnimSection_GetKeyframeNumber(&instance->anim.section[CurrentSection & 0xFF]);
}

int G2EmulationInstanceQueryLastFrame(Instance *instance, int CurrentSection)
{
    return G2AnimSection_GetStoredKeyframeNumber(&instance->anim.section[CurrentSection & 0xFF]);
}

int G2EmulationInstanceQueryPassedFrame(Instance *instance, int CurrentSection, int frame)
{
    G2AnimSection *animSection;
    short temp; // not from decls.h

    animSection = &instance->anim.section[CurrentSection & 0xFF];

    if (G2AnimSection_IsInInterpolation(animSection) == G2FALSE)
    {
        if (frame != 0)
        {
            temp = frame * G2Anim_GetKeylist(&instance->anim)->s0TailTime;

            return (animSection->storedTime < temp) && (animSection->elapsedTime >= temp);
        }

        return 1;
    }

    return 0;
}

int G2EmulationQueryFrame(CharacterState *In, int CurrentSection)
{
    return G2EmulationInstanceQueryFrame(In->CharacterInstance, CurrentSection);
}

int G2EmulationInstanceQueryMode(Instance *instance, int CurrentSection)
{
    G2AnimSection *animSection;
    int temp; // not from decls.h

    animSection = &instance->anim.section[CurrentSection & 0xFF];

    temp = 0;

    if (!(animSection->flags & 0x1))
    {
        if (!(animSection->flags & 0x2))
        {
            temp = 1;
            (void)temp;

            return 1;
        }

        return 2;
    }

    return 0;
}

int G2EmulationQueryMode(CharacterState *In, int CurrentSection)
{
    return G2EmulationInstanceQueryMode(In->CharacterInstance, CurrentSection);
}

void G2EmulationInstanceSetStartAndEndSegment(Instance *instance, int CurrentSection, short Start, short End)
{
    G2AnimSection *animSection;

    animSection = &instance->anim.section[CurrentSection & 0xFF];

    animSection->firstSeg = (unsigned char)Start;

    animSection->segCount = (End - Start) + 1;
}

void G2EmulationSetStartAndEndSegment(CharacterState *In, int CurrentSection, short Start, short End)
{
    G2EmulationInstanceSetStartAndEndSegment(In->CharacterInstance, CurrentSection, Start, End);
}

void G2EmulationInstanceSetTotalSections(Instance *instance, short Total)
{
    G2Anim *anim;

    anim = &instance->anim;

    while (anim->sectionCount < Total)
    {
        G2Anim_AddSection(anim, 0, 0);
    }

    G2Anim_SetCallback(anim, (void *)INSTANCE_DefaultAnimCallback, instance);
}

void G2EmulationSetTotalSections(CharacterState *In, short Total)
{
    In->TotalSections = Total;

    G2EmulationInstanceSetTotalSections(In->CharacterInstance, Total);
}

void G2EmulationInstanceInitSection(Instance *instance, int CurrentSection, void *callback, void *data)
{
    G2AnimSection *animSection;

    animSection = &instance->anim.section[CurrentSection & 0xFF];

    animSection->callback = (void *)callback;
    animSection->callbackData = data;
}

void G2EmulationSetInterpController_Vector(Instance *instance, long segment, long type, G2SVector3 *vec, int Frames, intptr_t Data)
{
    int temp, temp2; // not from decls.h

    temp = segment & 0xFF;

    temp2 = type & 0xFF;

    G2Anim_SetInterpController_Vector(&instance->anim, temp, temp2, vec, Frames * 100);
    G2Anim_SetControllerCallbackData(&instance->anim, temp, temp2, (void *)Data);
}

void StateSwitchStateDataDefault(CharacterState *In, int CurrentSection, void (*NewProcess)(), intptr_t Data)
{
    void (*process)();

    process = In->SectionList[CurrentSection].Process;

    StateSwitchStateData(In, CurrentSection, NewProcess, Data);

    process(In, CurrentSection, 0);
}

void StateSwitchStateCharacterDataDefault(CharacterState *In, void (*NewProcess)(), intptr_t Data)
{
    int i;

    for (i = 0; i < In->TotalSections; i++)
    {
        StateSwitchStateDataDefault(In, i, NewProcess, Data);
    }
}

void StateSwitchStateData(CharacterState *In, int CurrentSection, void (*NewProcess)(), intptr_t Data)
{
    PurgeMessageQueue(&In->SectionList[CurrentSection].Event);

    EnMessageQueueData(&In->SectionList[CurrentSection].Event, 0x100001, Data);

    In->SectionList[CurrentSection].Process = NewProcess;

    In->SectionList[CurrentSection].Process(In, CurrentSection, 0);

    EnMessageQueueData(&In->SectionList[CurrentSection].Event, 0x100004, 0);
    EnMessageQueueData(&In->SectionList[CurrentSection].Event, 0x100004, 0);
}

void StateSwitchStateCharacterData(CharacterState *In, void (*NewProcess)(), intptr_t Data)
{
    int i;

    for (i = 0; i < In->TotalSections; i++)
    {
        PurgeMessageQueue(&In->SectionList[i].Event);

        EnMessageQueueData(&In->SectionList[i].Event, 0x100004, 0);

        In->SectionList[i].Process(In, i, 0);

        PurgeMessageQueue(&In->SectionList[i].Event);

        EnMessageQueueData(&In->SectionList[i].Event, 0x100001, Data);

        In->SectionList[i].Process = NewProcess;

        In->SectionList[i].Process(In, i, 0);
    }
}

void StateGovernState(CharacterState *In, int Frames)
{
    State *pSectionA;
    State *pSectionB;
    G2AnimSection *animSectionA;
    G2AnimSection *animSectionB;
    G2AnimKeylist *keylist;
    int keylistID;
    int i;

    for (i = 1; i < 3; i++)
    {
        pSectionA = &In->SectionList[i - 1];
        pSectionB = &In->SectionList[i];

        if (pSectionA->Process == pSectionB->Process)
        {
            animSectionA = &In->CharacterInstance->anim.section[(i - 1) & 0xFF];
            animSectionB = &In->CharacterInstance->anim.section[i & 0xFF];

            if ((animSectionA->keylistID == animSectionB->keylistID) && (G2AnimSection_IsInInterpolation(animSectionA) == G2FALSE) && (G2AnimSection_IsInInterpolation(animSectionB) == G2FALSE) && (G2AnimSection_GetKeyframeNumber(animSectionA) != G2AnimSection_GetKeyframeNumber(animSectionB)))
            {
                keylist = animSectionA->keylist;
                keylistID = animSectionA->keylistID;

                G2AnimSection_InterpToKeylistFrame(animSectionB, keylist, keylistID, (G2AnimSection_GetKeyframeNumber(animSectionA) + Frames) % G2AnimKeylist_GetKeyframeCount(keylist), Frames * 100);
            }
        }
    }
}
