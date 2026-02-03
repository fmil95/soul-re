#include "Game/FX.h"
#include "Game/COLLIDE.h"
#include "Game/GLYPH.h"
#include "Game/STREAM.h"
#include "Game/DEBUG.h"
#include "Game/SOUND.h"
#include "Game/GAMELOOP.h"
#include "Game/PHYSICS.h"
#include "Game/RAZCNTRL.h"
#include "Game/MATH3D.h"
#include "Game/STATE.h"
#include "Game/REAVER.h"
#include "Game/CAMERA.h"
#include "Game/GAMEPAD.h"
#include "Game/INSTANCE.h"
#include "Game/MONSTER/MONLIB.h"
#include "Game/G2/ANIMG2.h"
#include "Game/G2/QUATG2.h"
#include "Game/RAZIEL/CONTROL.h"
#include "Game/RAZIEL/RAZIEL.h"
#include "Game/OBTABLE.h"
#include "Game/RAZIEL/HEALTH.h"
#include "Game/RAZIEL/SWIM.h"
#include "Game/RAZIEL/STEERING.h"
#include "Game/RAZIEL/SPIDER.h"
#include "Game/G2/ANMG2ILF.h"
#include "Game/EVENT.h"
#include "Game/RAZIEL/RAZLIB.h"
#include "Game/HASM.h"
#include "Game/PSX/MAIN.h"

/*.sbss*/
STATIC Player Raziel;

STATIC RazielData *PlayerData;

STATIC G2AnimInterpInfo razInterpInfo[3];

STATIC FitPoint constrictData[32];

STATIC MATRIX TempMat; /*unused*/

STATIC GameTracker *gameTracker;

STATIC long *PadData;

STATIC G2SVector3 ExtraRotData;

STATIC G2SVector3 *ExtraRot;

STATIC Force ExternalForces[4];

STATIC int PhysicsMode;

STATIC EngagedInstance EngagedList[15];

STATIC Normal Norm;

STATIC int LastRC;

STATIC int AutoFaceAngle;

int ControlFlag;

/*.sdata*/
int LoopCounter = 0;
static int WaterStatus = 0;
static unsigned char PickupList[4] = {0, 11, 11, 97};
static unsigned short cowlList[19] = {
    0x153,
    0x154,
    0x155,
    0x3A,
    0x3B,
    0x3C,
    0x3D,
    0x55,
    0x56,
    0x57,
    0x58,
    0xEB,
    0xEC,
    0xED,
    0xEE,
    0x1C6,
    0x1C7,
    0x1C8,
    0x1C9,
};

razController controllerList[24] = {
    {0, 0x26},
    {0, 0x64},
    {0, 0x20},
    {0, 0x22},
    {0, 0x8},
    {0, 0xE},
    {0x1, 0x26},
    {0x1, 0xE},
    {0x1, 0xA},
    {0x11, 0xE},
    {0x11, 0x8},
    {0x10, 0xE},
    {0xF, 0xE},
    {0xE, 0xE},
    {0xE, 0x8},
    {0x30, 0x8},
    {0x32, 0x4C},
    {0x33, 0x8},
    {0x34, 0x8},
    {0x35, 0x8},
    {0x3A, 0x4C},
    {0x3B, 0x8},
    {0x3C, 0x8},
    {0x3D, 0x8},
};

CannedSound cannedSound[4] = {
    {0, 51, -100, -100, 120, 0, 368640, 3500},
    {0, 353, 0, 0, 80, 0, 368640, 3500},
    {0, 8, -260, -260, 90, 90, 0, 3500},
    {0, 418, -200, -200, 95, 95, 0, 3500},
};

#include "Game/RAZIEL/PUPPET.c"
#include "Game/RAZIEL/ATTACK.c"
#include "Game/RAZIEL/SWIM.c"
#include "Game/RAZIEL/SPIDER.c"
#include "Game/RAZIEL/ALGOCTRL.c"
#include "Game/RAZIEL/STEERING.c"
#include "Game/RAZIEL/RAZDBUG.c"
#include "Game/RAZIEL/SENSES.c"
#include "Game/RAZIEL/HEALTH.c"
#include "Game/RAZIEL/RAZLIB.c"

Idle **currentIdleList = NULL;
int currentIdleListCount = 0;
int currentIdleCount = 0;

static int BlockCount = 0;
static int LastBlock = 0;

void InitStates(Instance *PlayerInstance)
{
    unsigned char i;
    G2AnimSection *animSection;
    typedef long fn(G2Anim *, int, G2AnimCallbackMsg, long, long, Instance *); // not from decls.h

    if (Raziel.footPrint == 0)
    {
        Raziel.footPrint = (intptr_t)PlayerInstance->object;
    }

    PlayerInstance->InstanceState = &Raziel.State.CharacterInstance;
    Raziel.State.CharacterInstance = PlayerInstance;
    PlayerData = (RazielData *)PlayerInstance->data;

    G2EmulationSetTotalSections(&Raziel.State, 3);
    G2EmulationSetStartAndEndSegment(&Raziel.State, 0, 0, 13);
    G2EmulationSetStartAndEndSegment(&Raziel.State, 1, 14, 49);
    G2EmulationSetStartAndEndSegment(&Raziel.State, 2, 50, 65);

    for (i = 0; i < 3; i++)
    {
        animSection = &PlayerInstance->anim.section[i];
        animSection->callback = (fn *)&RazielAnimCallback;
        animSection->callbackData = NULL;

        Raziel.State.SectionList[i].Process = &StateHandlerIdle;

        InitMessageQueue(&Raziel.State.SectionList[i].Event);
        InitMessageQueue(&Raziel.State.SectionList[i].Defer);
    }

    G2AnimSection_SetInterpInfo(&PlayerInstance->anim.section[0], &razInterpInfo[0]);
    G2AnimSection_SetInterpInfo(&PlayerInstance->anim.section[1], &razInterpInfo[1]);
    G2AnimSection_SetInterpInfo(&PlayerInstance->anim.section[2], &razInterpInfo[2]);

    Raziel.Mode = 1;

    gameTrackerX.debugFlags2 |= 0x800;

    InitHealthSystem();

    Raziel.Abilities = 0x1000000;
    debugRazielFlags1 = Raziel.Abilities;

    Raziel.GlyphManaBalls = 0;
    Raziel.GlyphManaMax = 0;
    Raziel.Abilities = 0;
    Raziel.RotationSegment = 0;
    Raziel.extraRot.x = 0;
    Raziel.blankPad = 0;

    PlayerInstance->rotation.y = 0;
    PlayerInstance->rotation.x = 0;

    Raziel.Senses.EngagedList = &EngagedList[0];
    Raziel.constrictData = &constrictData[0];
    Raziel.dropOffHeight = 256;
    Raziel.Senses.EngagedMask = 0;
    Raziel.idleInstance = NULL;
    Raziel.soulReaver = NULL;
    Raziel.fallZVelocity = -32;

    G2EmulationSetAnimation(&Raziel.State, 0, 0, 0, 0);
    G2EmulationSetMode(&Raziel.State, 0, 2);

    G2EmulationSetAnimation(&Raziel.State, 1, 0, 0, 0);
    G2EmulationSetMode(&Raziel.State, 1, 2);

    G2EmulationSetAnimation(&Raziel.State, 2, 0, 0, 0);
    G2EmulationSetMode(&Raziel.State, 2, 2);

    ControlFlag = 0x519;

    PlayerInstance->maxXVel = 256;
    PlayerInstance->maxYVel = 256;
    PlayerInstance->maxZVel = 256;

    razSetCowlNoDraw(1);

    InitExternalForces(&ExternalForces[0], 4);

    ResetPhysics(PlayerInstance, -16);

    ExtraRot = NULL;

    CAMERA_ChangeToOutOfWater(&theCamera, PlayerInstance);

    razAttachControllers();

    INSTANCE_Post(PlayerInstance, 0x100006, 0);

    PlayerInstance->checkMask = 0x803E002E;
    PlayerInstance->flags2 |= 0x400;

    G2EmulationInit();

    InitGlyphSystem(PlayerInstance);

    Raziel.slipSlope = 2896;
    Raziel.terminator = -1;

    gameTrackerX.raziel_collide_override = 0;
}

void StateInitIdle(CharacterState *In, int CurrentSection, intptr_t Ptr)
{
    evControlInitIdleData *data;
    Instance *linkWeapon;

    data = (evControlInitIdleData *)Ptr;

    if (data == NULL)
    {
        data = (evControlInitIdleData *)SetControlInitIdleData(0, 0, 3);
    }

    linkWeapon = razGetHeldWeapon();

    if (data->mode == 2)
    {
        if (razSwitchVAnimGroup(In->CharacterInstance, CurrentSection, 96, -1, -1) != 0)
        {
            G2EmulationSwitchAnimation(In, CurrentSection, 140, 0, 0, 1);
        }
    }
    else if (data->mode == 3)
    {
        if (razSwitchVAnimGroup(In->CharacterInstance, CurrentSection, 100, -1, -1) != 0)
        {
            G2EmulationSwitchAnimation(In, CurrentSection, 141, 0, 0, 1);
        }
    }
    else if (data->mode == 4)
    {
        if (razSwitchVAnimGroup(In->CharacterInstance, CurrentSection, 104, -1, -1) != 0)
        {
            G2EmulationSwitchAnimation(In, CurrentSection, 142, 0, 0, 1);
        }
    }
    else if ((linkWeapon == NULL) || (CurrentSection != 1))
    {
        if (data->mode == 0)
        {
            G2EmulationSwitchAnimation(In, CurrentSection, 0, 0, data->frames, 2);
        }
        else if (data->mode == 1)
        {
            G2EmulationSwitchAnimation(In, CurrentSection, 55, 0, data->frames, 2);
        }
    }
    else
    {
        if ((data->mode == 2) || (data->mode == 3) || (data->mode == 4))
        {
            data->mode = 0;
        }

        switch (Raziel.Senses.heldClass)
        {
        case 0:
            break;
        case 0x1:
            if (data->mode == 0)
            {
                G2EmulationSwitchAnimation(In, 1, 50, 0, data->frames, 2);
            }
            else
            {
                G2EmulationSwitchAnimation(In, 1, 127, 0, data->frames, 2);
            }

            break;
        case 0x1000:
            if (data->mode == 0)
            {
                G2EmulationSwitchAnimation(In, 1, 117, 0, data->frames, 2);

                razReaverScale(2800);
            }
            else
            {
                G2EmulationSwitchAnimation(In, 1, 137, 0, data->frames, 2);
            }

            break;
        case 0x2:
            if (data->mode == 0)
            {
                G2EmulationSwitchAnimation(In, 1, 84, 0, data->frames, 2);
            }
            else
            {
                G2EmulationSwitchAnimation(In, 1, 126, 0, data->frames, 2);
            }

            break;
        case 0x3:
            G2EmulationSwitchAnimation(In, 1, 98, 0, data->frames, 2);
            break;
        default:
            if ((Raziel.Senses.heldClass != (unsigned int)CurrentSection) && (Raziel.Senses.heldClass == (unsigned int)CurrentSection))
            {
                if (data->mode == 0)
                {
                    G2EmulationSwitchAnimation(In, 1, 50, 0, data->frames, 2);
                }
                else
                {
                    G2EmulationSwitchAnimation(In, 1, 127, 0, data->frames, 2);
                }
            }
        }
    }
}

void StateHandlerIdle(CharacterState *In, int CurrentSection, intptr_t Data)
{
    Message *Ptr;
    int Anim;
    int blockForwardMotion;
    Instance *heldInst;
    evPhysicsEdgeData *EdgeData;

    blockForwardMotion = 0;

    ControlFlag &= ~0x10;

    G2EmulationQueryFrame(In, CurrentSection);

    Anim = G2EmulationQueryAnimation(In, CurrentSection);

    while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
    {
        switch (Ptr->ID)
        {
        case 0x100001:
            if (CurrentSection == 0)
            {
                Raziel.Mode = 0x1;

                Raziel.idleCount = 0;

                ControlFlag = 0x2A109;

                PhysicsMode = 3;

                SteerSwitchMode(In->CharacterInstance, 0);

                Raziel.movementMinRate = 0;
            }

            StateInitIdle(In, CurrentSection, Ptr->Data);
            break;
        case 0x100004:
            if (CurrentSection == 0)
            {
                razReaverScale(4096);

                razResetPauseTranslation(In->CharacterInstance);

                COLLIDE_SegmentCollisionOn(In->CharacterInstance, 1);
            }

            break;
        case 0x8000000:
            if (Anim == 215)
            {
                ControlFlag &= ~0x800000;
            }

            if (Anim == 214)
            {
                G2EmulationSwitchAnimation(In, CurrentSection, 215, 0, 3, 1);
            }
            else
            {
                StateInitIdle(In, CurrentSection, SetControlInitIdleData(0, 0, 3));

                if (CurrentSection == 0)
                {
                    if (!(Raziel.playerEventHistory & 0x10000))
                    {
                        ControlFlag &= ~0x8000;
                    }

                    COLLIDE_SegmentCollisionOn(In->CharacterInstance, 1);
                }
            }

            break;
        case 0x80000000:
            if (!(Raziel.Senses.Flags & 0x80))
            {
                StateSwitchStateData(In, CurrentSection, StateHandlerAttack2, 0);
            }

            break;
        case 0x80000002:
            if ((CurrentSection == 0) && ((*PadData & RazielCommands[1])))
            {
                Raziel.returnState = NULL;

                StateSwitchStateCharacterData(In, StateHandlerSoulSuck, 0);
            }

            break;
        case 0x4010401:
            blockForwardMotion = 1;
            break;
        case 0x4010080:
            if (CurrentSection == 0)
            {
                if (Ptr->Data != 0)
                {
                    razResetPauseTranslation(In->CharacterInstance);
                }
                else
                {
                    razSetPauseTranslation(In->CharacterInstance);
                }
            }

            break;
        case 0x10000000:
            if ((*PadData & RazielCommands[7]))
            {
                StateSwitchStateData(In, CurrentSection, StateHandlerMove, 3);
            }
            else if (Raziel.Bearing < -512)
            {
                if (CurrentSection == 1)
                {
                    if (razGetHeldWeapon() == NULL)
                    {
                        G2EmulationSwitchAnimation(In, CurrentSection, 54, 0, 2, 1);
                    }
                }
                else
                {
                    G2EmulationSwitchAnimation(In, CurrentSection, 54, 0, 2, 1);
                }

                StateSwitchStateData(In, CurrentSection, StateHandlerStartTurn, 0);

                In->SectionList[CurrentSection].Data1 = 52;
            }
            else if (Raziel.Bearing > 512)
            {
                if (CurrentSection == 1)
                {
                    if (In->CharacterInstance->LinkChild == NULL)
                    {
                        G2EmulationSwitchAnimation(In, CurrentSection, 53, 0, 2, 1);
                    }
                }
                else
                {
                    G2EmulationSwitchAnimation(In, CurrentSection, 53, 0, 2, 1);
                }

                StateSwitchStateData(In, CurrentSection, StateHandlerStartTurn, 0);

                In->SectionList[CurrentSection].Data1 = 51;
            }
            else if (blockForwardMotion == 0)
            {
                if (Raziel.Magnitude < 4096)
                {
                    StateSwitchStateData(In, CurrentSection, StateHandlerMove, 3);
                }
                else
                {
                    StateSwitchStateData(In, CurrentSection, StateHandlerStartMove, 0);
                }
            }

            break;
        case 0x80000001:
            if (CurrentSection == 0)
            {
                Raziel.Mode = 0x10;

                ControlFlag |= 0x10;

                if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 16, NULL, NULL) != 0)
                {
                    G2EmulationSwitchAnimationCharacter(In, 34, 0, 2, 1);
                }

                StateSwitchStateCharacterData(In, StateHandlerCompression, 0);
            }

            break;
        case 0x2000000:
            if (((Raziel.Senses.EngagedMask & 0x1)) && (Raziel.Senses.heldClass != 0x3))
            {
                Raziel.Mode = 0x200;

                In->SectionList[CurrentSection].Data1 = 0;

                if (CurrentSection == 0)
                {
                    G2EmulationSwitchAnimationCharacter(In, 21, 0, 6, 1);

                    StateSwitchStateCharacterData(In, StateHandlerPushObject, 0);
                }

                ControlFlag &= ~0x1;
            }
            else if (((Raziel.Senses.EngagedMask & 0x8)) && (Raziel.Senses.heldClass != 0x3))
            {
                if (CurrentSection == 0)
                {
                    StateSwitchStateCharacterData(In, StateHandlerPullSwitch, 0);
                }
            }
            else if ((Raziel.Senses.EngagedMask & 0x2010))
            {
                if (CurrentSection == 0)
                {
                    if (Raziel.Senses.heldClass == 0x3)
                    {
                        heldInst = razGetHeldItem();

                        if (heldInst != NULL)
                        {
                            if ((INSTANCE_Query(heldInst, 2) & 0x20))
                            {
                                StateSwitchStateCharacterData(In, StateHandlerBreakOff, 0);
                            }
                        }
                    }
                    else
                    {
                        StateSwitchStateCharacterData(In, StateHandlerBreakOff, 0);
                    }
                }
            }
            else if ((Raziel.Senses.EngagedMask & 0x800))
            {
                if (CurrentSection == 1)
                {
                    razReaverPickup(In->CharacterInstance, Raziel.Senses.EngagedList[11].instance);
                }
            }
            else if ((Raziel.Senses.EngagedMask & 0x4000))
            {
                if (CurrentSection == 0)
                {
                    StateSwitchStateCharacterData(In, StateHandlerWarpGate, 0);
                }
            }
            else if ((razPickupAndGrab(In, CurrentSection) != 0) && (CurrentSection == 0) && (!(Raziel.Senses.Flags & 0x80)))
            {
                StateSwitchStateCharacterData(In, StateHandlerAttack2, 0);
            }

            break;
        case 0x4010400:
            EdgeData = (evPhysicsEdgeData *)Ptr->Data;

            if (((EdgeData->rc & 0x20000)) && ((Raziel.Abilities & 0x1)) && (Raziel.CurrentPlane == 2))
            {
                Raziel.playerEvent |= 0x10000;
            }

            break;
        case 0x80000010:
            if ((CurrentSection == 0) && ((Raziel.Senses.Flags & 0x4)))
            {
                if (((Raziel.Senses.Flags & 0x80)) || ((gameTrackerX.streamFlags & 0x4)))
                {
                    EnMessageQueueData(&In->SectionList[0].Defer, 0x80000010, 0);
                }
                else
                {
                    StateSwitchStateCharacterData(In, StateHandlerGlyphs, 0);
                }
            }

            break;
        case 0x2000001:
        case 0x2000002:
            break;
        default:
            DefaultStateHandler(In, CurrentSection, Data);
        }

        DeMessageQueue(&In->SectionList[CurrentSection].Event);
    }

    if ((CurrentSection == 0) && (CheckHolding(In->CharacterInstance) == 0) && ((Anim != 55) && (Anim != 214)))
    {
        Raziel.idleCount++;

        if ((Raziel.idleCount > 300) && (Raziel.idleCount == ((Raziel.idleCount / 900) * 900)))
        {
            if (Raziel.idleInstance != NULL)
            {
                G2EmulationInstanceToInstanceSwitchAnimationCharacter(In->CharacterInstance, Raziel.idleInstance, 1, 0, 3, 1);
            }
            else
            {
                G2EmulationSwitchAnimationCharacter(In, 24, 0, 3, 1);

                COLLIDE_SegmentCollisionOff(In->CharacterInstance, 1);
            }
        }
    }
}

void StateHandlerLookAround(CharacterState *In, int CurrentSection, intptr_t Data)
{
    Message *Ptr;

    (void)Data;

    while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
    {
        switch (Ptr->ID)
        {
        case 0x100001:
            if (CurrentSection == 1)
            {
                Instance *instance;

                instance = In->CharacterInstance;

                G2Anim_EnableController(&instance->anim, 17, 14);
                G2Anim_EnableController(&instance->anim, 16, 14);
                G2Anim_EnableController(&instance->anim, 14, 14);

                ControlFlag = 0x8001008;
            }

            if (G2EmulationQueryAnimation(In, CurrentSection) == 24)
            {
                StateInitIdle(In, CurrentSection, 0);
            }

            break;
        case 0x100004:
            if (CurrentSection == 1)
            {
                Instance *instance;

                instance = In->CharacterInstance;

                G2Anim_InterpDisableController(&instance->anim, 17, 14, 300);
                G2Anim_InterpDisableController(&instance->anim, 16, 14, 300);
                G2Anim_InterpDisableController(&instance->anim, 14, 14, 300);
            }

            break;
        case 0x80000020:
        {
            int message;
            int messageData;

            if (StateHandlerDecodeHold(&message, &messageData) != 0)
            {
                if ((message == 0x80000) && (CurrentSection == 0))
                {
                    razLaunchForce(In->CharacterInstance);

                    StateSwitchStateData(In, 0, StateHandlerThrow2, 0);
                }

                StateSwitchStateData(In, CurrentSection, StateHandlerThrow2, 0);

                if ((Raziel.Senses.heldClass != 0x1000) && (Raziel.Senses.heldClass != 0x8))
                {
                    razSetFadeEffect(In->CharacterInstance->fadeValue, PlayerData->throwFadeValue, PlayerData->throwFadeInRate);
                }

                Raziel.returnState = StateHandlerIdle;

                Raziel.throwMode = 0x2;
            }

            break;
        }
        case 0x100000:
            StateSwitchStateData(In, CurrentSection, Raziel.returnState, 0);
            break;
        case 0x1000000:
            if (CurrentSection == 0)
            {
                CAMERA_ForceEndLookaroundMode(&theCamera);
            }

            StateSwitchStateData(In, CurrentSection, StateHandlerHitReaction, Ptr->Data);
            break;
        case 0x40005:
        case 0x40025:
            StateSwitchStateData(In, CurrentSection, StateHandlerStumble, Ptr->Data);
            break;
        }

        DeMessageQueue(&In->SectionList[CurrentSection].Event);
    }

    if ((!(*PadData & RazielCommands[5])) || (!(*PadData & RazielCommands[4])))
    {
        EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x100000, 0);
    }
}

void StateHandlerCrouch(CharacterState *In, int CurrentSection, intptr_t Data)
{
    Message *Ptr;
    int Anim;
    Instance *heldInst;
    int DropThisFrame;
    evObjectData *data;
    int i;

    DropThisFrame = 0;

    heldInst = razGetHeldItem();

    while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
    {
        Anim = G2EmulationQueryAnimation(In, CurrentSection);

        switch (Ptr->ID)
        {
        case 0x100001:
            if (CurrentSection == 0)
            {
                Raziel.Mode = 0x40;

                ControlFlag = 0x8109;

                PhysicsMode = 3;

                SteerSwitchMode(In->CharacterInstance, 0);

                if (Ptr->Data != 0)
                {
                    if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 72, NULL, NULL) != 0)
                    {
                        G2EmulationSwitchAnimationCharacter(In, 3, 0, 3, 1);
                    }
                }
                else if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 76, NULL, NULL) != 0)
                {
                    G2EmulationSwitchAnimationCharacter(In, 4, 0, 8, 2);
                }

                if (heldInst != NULL)
                {
                    INSTANCE_Post(heldInst, 0x80002C, 0);
                }
            }

            if (Ptr->Data != 0)
            {
                In->SectionList[CurrentSection].Data2 = 72;
            }
            else
            {
                In->SectionList[CurrentSection].Data2 = 0;
            }

            break;
        case 0x100004:
            if (CurrentSection == 0)
            {
                COLLIDE_SegmentCollisionOn(In->CharacterInstance, 1);

                if (heldInst != NULL)
                {
                    INSTANCE_Post(heldInst, 0x200003, 7);
                    INSTANCE_Post(heldInst, 0x80002B, 0);
                }
            }

            break;
        case 0x8000000:
            if ((Anim == 5) || (Anim == 85))
            {
                StateSwitchStateData(In, CurrentSection, StateHandlerIdle, SetControlInitIdleData(0, 0, 3));

                ControlFlag &= ~0x8001000;

                In->SectionList[CurrentSection].Data1 = 1;
                break;
            }

            if (CurrentSection == 1)
            {
                if ((In->SectionList[CurrentSection].Data2 == 80) && (heldInst != NULL) && (DropThisFrame == 0))
                {
                    INSTANCE_Post(heldInst, 0x800008, 0);

                    razReaverOn();
                }

                In->SectionList[0].Data2 = 76;

                In->SectionList[CurrentSection].Data2 = 76;

                In->SectionList[2].Data2 = 76;

                if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 76, NULL, NULL) != 0)
                {
                    G2EmulationSwitchAnimationCharacter(In, 4, 0, 8, 2);
                }
            }

            break;
        case 0x100000:
            StateSwitchStateData(In, CurrentSection, StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
            break;
        case 0x2000000:
            if (((Raziel.Senses.EngagedMask & 0x4)) && (Raziel.Senses.heldClass != 0x3) && ((Anim != 5) && (Anim != 85)))
            {
                ControlFlag = (ControlFlag | 0x8041000) & ~0x8;

                if (CurrentSection == 0)
                {
                    for (i = 0; i < 3; i++)
                    {
                        In->SectionList[i].Data1 = 0;

                        PurgeMessageQueue(&In->SectionList[i].Event);
                    }

                    razCenterWithBlock(In->CharacterInstance, Raziel.Senses.EngagedList[2].instance, -141);

                    data = (evObjectData *)SetObjectData(-Raziel.Senses.ForwardNormal.x, -Raziel.Senses.ForwardNormal.y, 0, NULL, 0);

                    INSTANCE_Post(Raziel.Senses.EngagedList[2].instance, 0x800001, (intptr_t)data);

                    if (!(data->rc & 0x1))
                    {
                        INSTANCE_Post(In->CharacterInstance, 0x100000, 0);
                        break;
                    }

                    COLLIDE_SegmentCollisionOff(In->CharacterInstance, 1);

                    if ((data->rc & 0x8))
                    {
                        G2EmulationSwitchAnimationCharacter(In, 85, 0, 0, 1);
                    }
                    else
                    {
                        G2EmulationSwitchAnimationCharacter(In, 5, 0, 0, 1);
                    }

                    razSetPlayerEventHistory(0x2);
                }
            }

            break;
        case 0x80000001:
            if ((CurrentSection == 0) && ((Anim != 5) && (Anim != 85)))
            {
                Raziel.Mode = 0x20;

                if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 32, NULL, NULL) != 0)
                {
                    G2EmulationSwitchAnimationCharacter(In, 38, 0, 1, 1);
                }

                StateSwitchStateCharacterData(In, StateHandlerCompression, 0);
            }

            break;
        case 0x80000010:
            if ((CurrentSection == 0) && (In->CharacterInstance->tface != NULL))
            {
                EnMessageQueueData(&In->SectionList[0].Defer, 0x80000010, 0);

                ControlFlag |= 0x800000;
            }

            if (((Anim != 5) && (Anim != 85)) && (In->SectionList[CurrentSection].Data2 != 80))
            {
                StateSwitchStateData(In, CurrentSection, StateHandlerIdle, SetControlInitIdleData(0, 0, 3));

                Raziel.Mode = 0x1000000;
            }

            break;
        case 0x20000008:
            if (((Anim != 5) && (Anim != 85)) && (In->SectionList[CurrentSection].Data2 != 80))
            {
                StateSwitchStateData(In, CurrentSection, StateHandlerIdle, SetControlInitIdleData(0, 0, 3));

                Raziel.Mode = 0x1000000;
            }

            break;
        case 0x80000000:
            if (heldInst != NULL)
            {
                DropThisFrame = 1;

                razSwitchVAnimGroup(In->CharacterInstance, CurrentSection, 80, -1, -1);

                In->SectionList[CurrentSection].Data2 = 80;

                INSTANCE_Post(heldInst, 0x200005, 0);
            }

            break;
        case 0x10000000:
            if ((Raziel.Senses.heldClass != 0x3) && ((Anim != 5) && (Anim != 85)))
            {
                StateSwitchStateData(In, CurrentSection, StateHandlerMove, 3);
            }

            break;
        case 0x80000020:
            if ((Anim != 5) && (Anim != 85))
            {
                DefaultStateHandler(In, CurrentSection, Data);
            }

            break;
        case 0x4010400:
        case 0x4020000:
        case 0x80000004:
        case 0x80000008:
            break;
        default:
            DefaultStateHandler(In, CurrentSection, Data);
        }

        DeMessageQueue(&In->SectionList[CurrentSection].Event);
    }

    if ((!(*PadData & RazielCommands[6])) && (In->SectionList[CurrentSection].Process == StateHandlerCrouch))
    {
        EnMessageQueueData(&In->SectionList[CurrentSection].Event, 0x20000008, 0);
    }
}

void StateHandlerDropAction(CharacterState *In, int CurrentSection, intptr_t Data)
{
    Message *Ptr;

    while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
    {
        switch (Ptr->ID)
        {
        case 0x100001:
            if ((razGetHeldItem() != NULL) && (CurrentSection == 1))
            {
                switch (Raziel.Senses.heldClass)
                {
                case 0:
                    break;
                case 0x1:
                    G2EmulationSwitchAnimation(In, 1, 83, 0, 3, 1);
                    break;
                case 0x2:
                    G2EmulationSwitchAnimation(In, 1, 105, 0, 3, 1);
                    break;
                case 0x3:
                    G2EmulationSwitchAnimation(In, 1, 99, 0, 3, 1);
                    break;
                case 0x1000:
                    break;
                }

                INSTANCE_Post(razGetHeldItem(), 0x80002C, 0);
            }
            else if (CurrentSection == 2)
            {
                G2EmulationSwitchAnimationAlpha(In, 2, 69, 0, 3, 1, 1);
            }
            else if (CurrentSection == 0)
            {
                G2EmulationSwitchAnimationAlpha(In, 0, 3, 0, 3, 1, 1);

                Raziel.Mode = 0x40;
            }

            break;
        case 0x8000000:
            if (CurrentSection == 1)
            {
                if (razGetHeldItem() != NULL)
                {
                    INSTANCE_Post(razGetHeldItem(), 0x800008, 0);

                    razReaverOn();
                }

                StateSwitchStateCharacterData(In, StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
            }

            break;
        case 0x10000000:
        case 0x20000008:
        case 0x80000001:
            break;
        default:
            DefaultStateHandler(In, CurrentSection, Data);
        }

        DeMessageQueue(&In->SectionList[CurrentSection].Event);
    }
}

void StateHandlerSoulSuck(CharacterState *In, int CurrentSection, intptr_t Data)
{
    Message *Ptr;
    int Anim;
    evPhysicsSwimData *SwimData;

    if (!(LoopCounter & 0x3))
    {
        FX_MakeSoulDust(In->CharacterInstance, 16);
    }

    Anim = G2EmulationQueryAnimation(In, CurrentSection);

    while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
    {
        switch (Ptr->ID)
        {
        case 0x100001:
            if (CurrentSection == 0)
            {
                ControlFlag = 0x9;

                PhysicsMode = 3;

                if (((Raziel.Senses.EngagedMask & 0x1000)) && (Raziel.Senses.heldClass != 0x3))
                {
                    razAlignYRotMove(Raziel.Senses.EngagedList[12].instance, 220, &In->CharacterInstance->position, &In->CharacterInstance->rotation, 0);

                    INSTANCE_Post(Raziel.Senses.EngagedList[12].instance, 0x1000014, 1);
                }

                G2EmulationSwitchAnimationAlpha(In, CurrentSection, 78, 0, 4, 1, 4);
            }

            if (CurrentSection == 2)
            {
                G2EmulationSwitchAnimation(In, CurrentSection, 0, 0, 4, 2);
            }

            if (CurrentSection == 1)
            {
                if ((razGetHeldWeapon() != NULL) || ((Raziel.Senses.EngagedMask & 0x1000)))
                {
                    G2EmulationSwitchAnimation(In, CurrentSection, 94, 0, 4, 1);
                }
                else
                {
                    G2EmulationSwitchAnimation(In, CurrentSection, 78, 0, 4, 1);
                }

                Raziel.Mode |= 0x10000000;
            }

            break;
        case 0x100004:
            if (CurrentSection == 1)
            {
                razSetCowlNoDraw(1);

                ControlFlag &= ~0x40;

                Raziel.Mode &= ~0x10000000;

                if ((Raziel.Senses.EngagedMask & 0x1000))
                {
                    INSTANCE_Post(Raziel.Senses.EngagedList[12].instance, 0x1000014, 0);
                }
            }

            break;
        case 0x1000006:
        case 0x20000002:
            if (Anim == 79)
            {
                G2EmulationSwitchAnimation(In, CurrentSection, 47, 0, 3, 1);

                PurgeMessageQueue(&In->SectionList[CurrentSection].Event);
            }
            else if (Raziel.returnState == NULL)
            {
                StateSwitchStateData(In, CurrentSection, StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
            }
            else
            {
                StateSwitchStateData(In, CurrentSection, Raziel.returnState, 0);
            }

            if (((Raziel.Senses.EngagedMask & 0x1000)) && (CurrentSection == 0))
            {
                INSTANCE_Post(Raziel.Senses.EngagedList[12].instance, 0x1000014, 0);
            }

            break;
        case 0x8000000:
            if (Anim == 47)
            {
                if (Raziel.returnState != NULL)
                {
                    StateSwitchStateData(In, CurrentSection, Raziel.returnState, 0);
                }
                else
                {
                    StateSwitchStateData(In, CurrentSection, StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
                }
            }

            if (Anim == 78)
            {
                G2EmulationSwitchAnimation(In, CurrentSection, 79, 0, 0, 1);
            }

            if (Anim == 80)
            {
                if ((*PadData & RazielCommands[1]))
                {
                    G2EmulationSwitchAnimation(In, CurrentSection, 79, 0, 8, 1);
                    break;
                }

                if (Raziel.returnState != NULL)
                {
                    StateSwitchStateData(In, CurrentSection, Raziel.returnState, 0);
                }
                else
                {
                    StateSwitchStateData(In, CurrentSection, StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
                }
            }

            break;
        case 0x1000016:
            if (((gameTrackerX.debugFlags2 & 0x800)) && (CurrentSection == 1))
            {
                GainHealth(Ptr->Data);

                razSetPlayerEventHistory(0x1000);
            }

            break;
        case 0x1000009:
            G2EmulationSwitchAnimation(In, CurrentSection, 80, 0, 2, 1);
            break;
        case 0x4020000:
            SwimData = (evPhysicsSwimData *)Ptr->Data;

            if ((SwimData->rc & 0x40))
            {
                StateHandlerInitSwim(In, CurrentSection, 0);
            }

            break;
        case 0x1000001:
        case 0x80000010:
            break;
        default:
            DefaultStateHandler(In, CurrentSection, Data);
        }

        DeMessageQueue(&In->SectionList[CurrentSection].Event);
    }
}

void StateHandlerStartTurn(CharacterState *In, int CurrentSection, intptr_t Data)
{
    Message *Ptr;

    G2EmulationQueryFrame(In, CurrentSection);

    if (Raziel.Bearing == 0)
    {
        EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x100000, 0);
    }

    while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
    {
        switch (Ptr->ID)
        {
        case 0x100001:
            if (CurrentSection == 0)
            {
                Raziel.Mode = 0x4000;

                ControlFlag = 0x20109;

                PhysicsMode = 3;

                SteerSwitchMode(In->CharacterInstance, 1);
            }

            break;
        case 0:
        case 0x100000:
        case 0x8000000:
            if (CurrentSection == 0)
            {
                if (!(*PadData & 0x8000000F))
                {
                    StateSwitchStateCharacterData(In, StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
                }
                else
                {
                    StateSwitchStateCharacterData(In, StateHandlerStartMove, 0);
                }
            }

            break;
        case 0x80000001:
            if (CurrentSection == 0)
            {
                Raziel.Mode = 0x8;

                if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 0, NULL, NULL) != 0)
                {
                    G2EmulationSwitchAnimationCharacter(In, 26, 0, 0, 1);
                }

                StateSwitchStateCharacterData(In, StateHandlerCompression, 0);
            }

            break;
        default:
            DefaultStateHandler(In, CurrentSection, Data);
        }

        DeMessageQueue(&In->SectionList[CurrentSection].Event);
    }
}

void StateInitStartMove(CharacterState *In, int CurrentSection, int Frame)
{
    if ((*PadData & RazielCommands[7]))
    {
        StateSwitchStateData(In, CurrentSection, StateHandlerMove, 3);
    }

    if (razSwitchVAnimGroup(In->CharacterInstance, CurrentSection, 48, Frame, -1) != 0)
    {
        razSwitchVAnimSingle(In->CharacterInstance, CurrentSection, 0, Frame, -1);
    }

    ControlFlag |= 0x2000;
}

void StateHandlerStartMove(CharacterState *In, int CurrentSection, intptr_t Data)
{
    Message *Ptr;
    int mode;

    while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
    {
        switch (Ptr->ID)
        {
        case 0x100001:
            StateInitStartMove(In, CurrentSection, Ptr->Data);

            In->SectionList[CurrentSection].Data2 = 1;

            if (CurrentSection == 0)
            {
                ControlFlag = 0x2A119;

                Raziel.Mode = (Raziel.Mode & 0x200800) | 0x4;

                PhysicsMode = 3;

                SteerSwitchMode(In->CharacterInstance, 2);

                Raziel.movementMinRate = 3276;

                Raziel.movementMinAnalog = 1024;
                Raziel.movementMaxAnalog = 4096;

                Raziel.passedMask = 0;
            }

            break;
        case 0x8000000:
        case 0x8000001:
            StateSwitchStateData(In, CurrentSection, StateHandlerMove, 0);
            break;
        case 0x4010080:
            if (CurrentSection != 2)
            {
                if (Ptr->Data != 0)
                {
                    G2EmulationSetMode(In, CurrentSection, 1);
                }
                else
                {
                    G2EmulationSetMode(In, CurrentSection, 0);
                }
            }

            if (((Raziel.passedMask & 0x2)) && ((*PadData & 0x8000000F)))
            {
                G2EmulationSetMode(In, CurrentSection, 2);

                ControlFlag &= ~0x2000;
            }

            break;
        case 0x2000000:
            if ((Raziel.Senses.EngagedMask & 0x20))
            {
                razPickupAndGrab(In, CurrentSection);
                break;
            }
        case 0x80000002:
        case 0x80000010:
            if ((CurrentSection == 0) && (In->CharacterInstance->tface != NULL))
            {
                EnMessageQueueData(&In->SectionList[0].Defer, Ptr->ID, 0);

                ControlFlag |= 0x800000;
            }
        case 0:
            if (CurrentSection == 0)
            {
                if (((Raziel.passedMask & 0x7)) || (G2EmulationQueryMode(In, 0) == 0))
                {
                    mode = Raziel.passedMask;

                    if ((Raziel.passedMask & 0x1))
                    {
                        mode = 0x2;
                    }
                    else if ((Raziel.passedMask & 0x2))
                    {
                        mode = 0x3;
                    }
                    else
                    {
                        mode &= 0x4;
                    }

                    StateSwitchStateCharacterData(In, StateHandlerIdle, SetControlInitIdleData(mode, 5, 5));

                    ControlFlag &= ~0x2000;
                }

                ControlFlag |= 0x2000;
            }

            break;
        case 0x80000001:
            if (CurrentSection == 0)
            {
                Raziel.Mode = 0x8;

                if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 0, NULL, NULL) != 0)
                {
                    G2EmulationSwitchAnimationCharacter(In, 26, 0, 0, 1);
                }

                StateSwitchStateCharacterData(In, StateHandlerCompression, 0);

                ControlFlag &= ~0x2000;
            }

            break;
        case 0x80000000:
            if ((Raziel.passedMask & 0x4))
            {
                if (CurrentSection == 1)
                {
                    if (!(Raziel.Senses.Flags & 0x80))
                    {
                        StateSwitchStateData(In, 1, StateHandlerAttack2, 10);
                    }

                    break;
                }

                StateSwitchStateData(In, CurrentSection, StateHandlerMove, 0);
                break;
            }

            if ((CurrentSection == 0) && (!(Raziel.Senses.Flags & 0x80)))
            {
                StateSwitchStateCharacterData(In, StateHandlerAttack2, 0);
            }

            break;
        case 0x80000004:
            StateSwitchStateData(In, CurrentSection, StateHandlerMove, 3);
            break;
        case 0x4000001:
            if ((CurrentSection == 0) && (G2EmulationQueryFrame(In, 0) > 6))
            {
                PhysicsMode = 0;

                SetDropPhysics(In->CharacterInstance, &Raziel);

                if (In->CharacterInstance->zVel < -32)
                {
                    if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 24, NULL, NULL) != 0)
                    {
                        G2EmulationSwitchAnimationCharacter(In, 36, 0, 4, 1);
                    }

                    StateSwitchStateCharacterData(In, StateHandlerFall, 0);
                }
            }

            break;
        case 0x4010401:
            StateSwitchStateData(In, CurrentSection, StateHandlerIdle, SetControlInitIdleData(0, 5, 5));
            break;
        case 0x10000000:
            break;
        default:
            DefaultStateHandler(In, CurrentSection, Data);
        }

        DeMessageQueue(&In->SectionList[CurrentSection].Event);
    }

    if (((*PadData & 0x8000000F)) && (In->SectionList[CurrentSection].Data2 != 0))
    {
        In->SectionList[CurrentSection].Data2++;

        if (In->SectionList[CurrentSection].Data2 >= 8)
        {
            G2EmulationSetMode(In, CurrentSection, 1);

            ControlFlag &= ~0x2000;

            In->SectionList[CurrentSection].Data2 = 0;
        }
    }

    if ((G2EmulationQueryFrame(In, CurrentSection) > 10) && (CurrentSection == 0) && (CheckHolding(In->CharacterInstance) != 0))
    {
        if (In->SectionList[1].Process == StateHandlerStartMove)
        {
            StateSwitchStateData(In, 1, StateHandlerMove, 11);
        }

        if (In->SectionList[2].Process == StateHandlerStartMove)
        {
            StateSwitchStateData(In, 2, StateHandlerMove, 11);
        }
    }

    if (((Raziel.Magnitude != 0) && (Raziel.Magnitude < 4096)) && ((Raziel.passedMask & 0x1)))
    {
        StateSwitchStateData(In, CurrentSection, StateHandlerMove, 0);
    }
}

void StateInitMove(CharacterState *In, int CurrentSection, int Frames)
{
    if ((*PadData & RazielCommands[7]))
    {
        Raziel.Mode = 0x2;

        if ((ControlFlag & 0x800000))
        {
            ControlFlag = 0x800000;
        }
        else
        {
            ControlFlag = 0;
        }

        ControlFlag |= 0x22119;

        if (In->SectionList[CurrentSection].Data2 != 52)
        {
            razResetMotion(In->CharacterInstance);

            if (razSwitchVAnimGroup(In->CharacterInstance, CurrentSection, 52, -1, -1) != 0)
            {
                razSwitchVAnimSingle(In->CharacterInstance, CurrentSection, 1, -1, -1);
            }

            In->SectionList[CurrentSection].Data2 = 52;
        }
    }
    else if ((*PadData & RazielCommands[6]))
    {
        Raziel.Mode = 0x1000000;

        if ((ControlFlag & 0x800000))
        {
            ControlFlag = 0x800000;
        }
        else
        {
            ControlFlag = 0;
        }

        ControlFlag |= 0x2119;

        if (In->SectionList[CurrentSection].Data2 != 56)
        {
            razResetMotion(In->CharacterInstance);

            if (razSwitchVAnimGroup(In->CharacterInstance, CurrentSection, 56, -1, -1) != 0)
            {
                G2EmulationSwitchAnimation(In, CurrentSection, 74, 0, 3, 2);
            }

            In->SectionList[CurrentSection].Data2 = 56;
        }
    }
    else
    {
        Raziel.Mode = (Raziel.Mode & 0x200800) | 0x4;

        ControlFlag |= 0x28119;

        if ((Raziel.Abilities & 0x20))
        {
            ControlFlag |= 0x200000;
        }

        razSelectMotionAnim(In, CurrentSection, Frames, &In->SectionList[CurrentSection].Data2);
    }
}

void StateHandlerMove(CharacterState *In, int CurrentSection, intptr_t Data)
{
    Message *Ptr;
    int Anim;
    int data;

    Anim = G2EmulationQueryAnimation(In, CurrentSection);

    In->SectionList[CurrentSection].Data1++;

    while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
    {
        switch (Ptr->ID)
        {
        case 0x100001:
            In->SectionList[CurrentSection].Data2 = -1;

            StateInitMove(In, CurrentSection, Ptr->Data);

            Raziel.constrictFlag = 0x1;

            SteerSwitchMode(In->CharacterInstance, 2);

            In->SectionList[CurrentSection].Data1 = 0;

            Raziel.passedMask |= 0x1000;
            break;
        case 0x100004:
            FX_EndConstrict(0, NULL);

            In->SectionList[CurrentSection].Data1 = 0;
            break;
        case 0x4010080:
            if (CurrentSection == 0)
            {
                if (Ptr->Data != 0)
                {
                    razResetPauseTranslation(In->CharacterInstance);
                }
                else
                {
                    razSetPauseTranslation(In->CharacterInstance);
                }
            }

            break;
        case 0x2000000:
            if ((Raziel.Senses.EngagedMask & 0x20))
            {
                razPickupAndGrab(In, CurrentSection);
                break;
            }
        case 0x80000002:
            if ((*PadData & RazielCommands[6]))
            {
                break;
            }
        case 0x80000010:
            if (CurrentSection != 0)
            {
                break;
            }

            if (In->CharacterInstance->tface != NULL)
            {
                EnMessageQueueData(&In->SectionList[0].Defer, Ptr->ID, 0);

                ControlFlag |= 0x800000;
            }
        case 0:
            if (CurrentSection == 0)
            {
                if ((Raziel.steeringMode == 9) || (Raziel.steeringMode == 14) || (Raziel.steeringMode == 15))
                {
                    razApplyMotion(In, CurrentSection);
                }
                else if ((Raziel.Mode == 0x2) || (Anim == 123) || (Anim == 124))
                {
                    StateSwitchStateCharacterData(In, StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
                }
                else if (Raziel.Mode == 0x1000000)
                {
                    StateSwitchStateCharacterData(In, StateHandlerCrouch, 0);
                }
                else if ((Ptr->Data < 4) && (!(ControlFlag & 0x800000)))
                {
                    ControlFlag |= 0x2000;

                    if (!(*PadData & 0x8000000F))
                    {
                        EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0, Ptr->Data + 1);

                        razApplyMotion(In, 0);
                    }
                }
                else
                {
                    data = 0;

                    if ((Raziel.passedMask & 0x2000))
                    {
                        data = 30;
                    }

                    if ((Raziel.passedMask & 0x1000))
                    {
                        data = 60;
                    }

                    if (CurrentSection == 0)
                    {
                        StateSwitchStateCharacterData(In, StateHandlerStopMove, data);
                    }
                }
            }

            break;
        case 0x10000000:
            if (Raziel.Magnitude < 4096)
            {
                StateInitMove(In, CurrentSection, 3);
            }
            else
            {
                StateInitMove(In, CurrentSection, 0);
            }

            break;
        case 0x80000004:
            if (!(Raziel.Mode & 0x200000))
            {
                if (CurrentSection == 2)
                {
                    G2EmulationSwitchAnimation(In, 2, 0, 0, 3, 2);
                }
                else
                {
                    G2EmulationSwitchAnimation(In, CurrentSection, 1, 0, 3, 2);
                }

                Raziel.Mode = 0x2;

                ControlFlag |= 0x2000;
            }

            break;
        case 0x20000004:
        case 0x20000008:
            Raziel.Mode = 0x4;

            ControlFlag &= ~0x2000;
            break;
        case 0x80000001:
            if (CurrentSection == 0)
            {
                if ((*PadData & RazielCommands[6]))
                {
                    Raziel.Mode = 0x20;

                    if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 32, NULL, NULL) != 0)
                    {
                        G2EmulationSwitchAnimationCharacter(In, 26, 0, 1, 1);
                    }
                }
                else
                {
                    Raziel.Mode = 0x8;

                    if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 0, NULL, NULL) != 0)
                    {
                        G2EmulationSwitchAnimationCharacter(In, 26, 0, 0, 1);
                    }
                }

                StateSwitchStateCharacterData(In, StateHandlerCompression, 0);

                ControlFlag &= ~0x2000;
            }

            break;
        case 0x80000000:
            if (!(*PadData & RazielCommands[7]))
            {
                if (In->SectionList[CurrentSection].Data2 == 68)
                {
                    if ((CurrentSection == 1) && (!(Raziel.Senses.Flags & 0x80)))
                    {
                        StateSwitchStateData(In, 1, StateHandlerAttack2, 10);
                    }

                    Raziel.dropOffHeight = 200;

                    Raziel.fallZVelocity = -96;
                    break;
                }
                else if (In->SectionList[CurrentSection].Data2 == 56)
                {
                    if ((razGetHeldItem() != NULL) && (CurrentSection == 0))
                    {
                        StateSwitchStateCharacterData(In, StateHandlerDropAction, 0);
                    }
                }
                else if (!(Raziel.Senses.Flags & 0x80))
                {
                    StateSwitchStateData(In, CurrentSection, StateHandlerAttack2, 0);
                }
            }
            else if (!(Raziel.Senses.Flags & 0x80))
            {
                StateSwitchStateData(In, CurrentSection, StateHandlerAttack2, 0);
            }

            break;
        case 0x80000008:
            break;
        default:
            DefaultStateHandler(In, CurrentSection, Data);
        }

        DeMessageQueue(&In->SectionList[CurrentSection].Event);
    }

    if ((CurrentSection == 0) && (In->SectionList[CurrentSection].Process != StateHandlerMove))
    {
        razResetMotion(In->CharacterInstance);
    }
}

void StateHandlerStopMove(CharacterState *In, int CurrentSection, intptr_t Data)
{
    Message *Ptr;

    while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
    {
        switch (Ptr->ID)
        {
        case 0x100001:
            if (CurrentSection == 0)
            {
                Raziel.Mode = 0x4;

                if ((ControlFlag & 0x800000))
                {
                    ControlFlag = 0x800000;
                }
                else
                {
                    ControlFlag = 0;
                }

                ControlFlag |= 0x2119;

                PhysicsMode = 3;
            }

            if (Ptr->Data == 60)
            {
                if (razSwitchVAnimGroup(In->CharacterInstance, CurrentSection, 88, -1, -1) != 0)
                {
                    razSwitchVAnimSingle(In->CharacterInstance, CurrentSection, 2, -1, -1);
                }
            }
            else if (Ptr->Data == 30)
            {
                if (razSwitchVAnimGroup(In->CharacterInstance, CurrentSection, 92, -1, -1) != 0)
                {
                    razSwitchVAnimSingle(In->CharacterInstance, CurrentSection, 3, -1, -1);
                }
            }
            else
            {
                StateSwitchStateData(In, CurrentSection, StateHandlerIdle, SetControlInitIdleData(0, 0, 6));
            }

            break;
        case 0x10000000:
            StateSwitchStateData(In, CurrentSection, StateHandlerStartMove, 0);
            break;
        case 0x4010080:
            if (CurrentSection == 0)
            {
                if (Ptr->Data == 0)
                {
                    razSetPauseTranslation(In->CharacterInstance);

                    StateSwitchStateCharacterData(In, StateHandlerIdle, SetControlInitIdleData(0, 0, 5));
                }
                else
                {
                    razResetPauseTranslation(In->CharacterInstance);
                }
            }

            break;
        case 0x2000000:
        case 0x80000002:
        case 0x80000010:
            EnMessageQueueData(&In->SectionList[CurrentSection].Defer, Ptr->ID, 0);
            break;
        case 0x8000000:
        case 0x8000001:
            StateSwitchStateData(In, CurrentSection, StateHandlerIdle, SetControlInitIdleData(0, 0, 5));
            break;
        case 0x80000001:
            if (CurrentSection == 0)
            {
                Raziel.Mode = 0x10;

                if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 0, NULL, NULL) != 0)
                {
                    G2EmulationSwitchAnimationCharacter(In, 26, 0, 0, 1);
                }

                StateSwitchStateCharacterData(In, StateHandlerCompression, 0);
            }

            break;
        default:
            DefaultStateHandler(In, CurrentSection, Data);
        }

        DeMessageQueue(&In->SectionList[CurrentSection].Event);
    }
}

void StateHandlerCompression(CharacterState *In, int CurrentSection, intptr_t Data)
{
    Message *Ptr;

    while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
    {
        switch (Ptr->ID)
        {
        case 0x100001:
            if (CurrentSection == 0)
            {
                ControlFlag = 0x111;

                SetExternalForce(&ExternalForces[2], 0, 0, 0, 1, 0);

                In->SectionList->Data1 = 0;

                PhysicsMode = 3;

                Raziel.movementMinRate = 0;

                switch (Raziel.Mode)
                {
                case 0xF:
                    break;
                case 0x20:
                    SteerSwitchMode(In->CharacterInstance, 4);

                    Raziel.steeringLockRotation = In->CharacterInstance->rotation.z;

                    In->CharacterInstance->yVel = 21;

                    if (G2Anim_IsControllerActive(&In->CharacterInstance->anim, 1, 14) == G2FALSE)
                    {
                        G2Anim_EnableController(&In->CharacterInstance->anim, 1, 14);
                    }

                    break;
                case 0x10:
                    SteerSwitchMode(In->CharacterInstance, 4);

                    Raziel.steeringLockRotation = In->CharacterInstance->rotation.z;

                    In->CharacterInstance->yVel = 40;

                    if (G2Anim_IsControllerActive(&In->CharacterInstance->anim, 1, 14) == G2FALSE)
                    {
                        G2Anim_EnableController(&In->CharacterInstance->anim, 1, 14);
                    }
                }
            }

            break;
        case 0x8000000:
            switch (Raziel.Mode)
            {
            case 0x8:
                if (CurrentSection == 0)
                {
                    if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 4, NULL, NULL) != 0)
                    {
                        G2EmulationSwitchAnimationCharacter(In, 27, 0, 0, 1);
                    }

                    SetPhysics(In->CharacterInstance, -16, 0, 83, 154);
                }

                break;
            case 0x20:
                if (CurrentSection == 0)
                {
                    if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 36, NULL, NULL) != 0)
                    {
                        G2EmulationSwitchAnimationCharacter(In, 39, 0, 0, 1);
                    }

                    SetPhysics(In->CharacterInstance, -16, 0, 21, 195);

                    In->CharacterInstance->yVel = 0;
                }

                break;
            default:
                if (CurrentSection == 0)
                {
                    if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 20, NULL, NULL) != 0)
                    {
                        G2EmulationSwitchAnimationCharacter(In, 35, 0, 0, 1);
                    }

                    SetPhysics(In->CharacterInstance, -16, 0, 40, 154);

                    In->CharacterInstance->yVel = 0;

                    if (In->SectionList[CurrentSection].Data1 == 1)
                    {
                        In->CharacterInstance->zVel = (In->CharacterInstance->zVel * 2) / 3;
                    }
                }
            }

            StateSwitchStateData(In, CurrentSection, StateHandlerJump, 0);
            break;
        case 0x20000001:
            if (CurrentSection == 0)
            {
                if (Raziel.Mode == 0x10)
                {
                    In->SectionList[CurrentSection].Data1 = 1;
                }

                EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x20000001, 0);
            }

            break;
        case 0x80000001:
            EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x80000001, 0);
            break;
        case 0x4000001:
        case 0x4020000:
        case 0x80000000:
            break;
        default:
            DefaultStateHandler(In, CurrentSection, Data);
        }

        DeMessageQueue(&In->SectionList[CurrentSection].Event);
    }
}

void StateHandlerJump(CharacterState *In, int CurrentSection, intptr_t Data)
{
    Message *Ptr;

    while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
    {
        switch (Ptr->ID)
        {
        case 0x100001:
            if (CurrentSection == 0)
            {
                ControlFlag = 0x511;

                Raziel.alarmTable = 200;

                PhysicsMode = 0;

                In->CharacterInstance->anim.section[CurrentSection].swAlarmTable = &Raziel.alarmTable;
            }

            In->SectionList[CurrentSection].Data2 = 0;
            break;
        case 0x8000004:
            ControlFlag |= 0x8;
            break;
        case 0x8000000:
            if (CurrentSection == 0)
            {
                if (Raziel.Mode == 0x10)
                {
                    if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 24, NULL, NULL) != 0)
                    {
                        G2EmulationSwitchAnimationCharacter(In, 36, 0, 4, 1);
                    }
                }
                else if (Raziel.Mode == 0x20)
                {
                    if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 40, NULL, NULL) != 0)
                    {
                        G2EmulationSwitchAnimationCharacter(In, 40, 0, 10, 1);
                    }
                }
                else if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 8, NULL, NULL) != 0)
                {
                    G2EmulationSwitchAnimationCharacter(In, 28, 0, 7, 1);
                }
            }

            StateSwitchStateData(In, CurrentSection, StateHandlerFall, 0);

            if (!(*PadData & RazielCommands[3]))
            {
                In->SectionList[CurrentSection].Data2 = 1;
            }

            break;
        case 0x20000001:
            In->SectionList[CurrentSection].Data2 = 1;

            if (CurrentSection == 0)
            {
                if ((Raziel.Mode == 0x10) || (Raziel.Mode == 0x20))
                {
                    EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x20000001, 0);
                }
                else if (((In->SectionList[CurrentSection].Data1 != 0) || (In->SectionList[CurrentSection].Data1 = G2EmulationQueryFrame(In, CurrentSection) + 4,
                    In->SectionList[CurrentSection].Data1 != 0)) &&
                         (In->SectionList[CurrentSection].Data1 < G2EmulationQueryFrame(In, CurrentSection)))
                {
                    SetDropPhysics(In->CharacterInstance, &Raziel);

                    if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 8, NULL, NULL) != 0)
                    {
                        G2EmulationSwitchAnimationCharacter(In, 28, 0, 7, 1);
                    }

                    StateSwitchStateCharacterData(In, StateHandlerFall, In->SectionList[CurrentSection].Data2);
                }
                else
                {
                    EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x20000001, 0);
                }
            }

            break;
        case 0x80000001:
            if (G2EmulationQueryFrame(In, CurrentSection) >= 2)
            {
                if ((Raziel.Senses.heldClass != 0x3) && (CurrentSection == 0))
                {
                    StateSwitchStateCharacterData(In, StateHandlerGlide, 0);
                }
            }
            else
            {
                EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x80000001, 0);
            }

            break;
        case 0x2000000:
            razPickupAndGrab(In, CurrentSection);
            break;
        case 0x40005:
        case 0x1000001:
        case 0x4000001:
        case 0x4010008:
        case 0x4020000:
        case 0x80000000:
        case 0x80000008:
        case 0x80000020:
            break;
        default:
            DefaultStateHandler(In, CurrentSection, Data);
        }

        DeMessageQueue(&In->SectionList[CurrentSection].Event);
    }
}

void StateHandlerFall(CharacterState *In, int CurrentSection, intptr_t Data)
{
    Message *Ptr;
    int Moving;
    int DeferFlag;
    evPhysicsSwimData *SwimData;

    Moving = 0;

    DeferFlag = 0x1;

    if ((CurrentSection == 0) && ((STREAM_GetLevelWithID(In->CharacterInstance->currentStreamUnitID)->unitFlags & 0x1000)))
    {
        EnMessageQueueData(&In->SectionList[CurrentSection].Event, 0x100000, Moving);
    }

    while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
    {
        switch (Ptr->ID)
        {
        case 0x100001:
            if (CurrentSection == 0)
            {
                ControlFlag = 0x119;

                if (Raziel.Mode != 0x100)
                {
                    ControlFlag = 0x519;
                }

                In->SectionList[CurrentSection].Data1 = 0;

                Raziel.movementMinRate = 0;

                PhysicsMode = 0;
            }

            if ((Ptr->Data != 0) && ((*PadData & RazielCommands[3])))
            {
                EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x80000001, 0);
            }

            break;
        case 0x10000000:
            Moving = 1;
            break;
        case 0x100000:
            StateSwitchStateCharacterData(In, StateHandlerForcedGlide, 0);
            break;
        case 0x4010008:
            if (DeferFlag != 0)
            {
                EnMessageQueueData(&In->SectionList[CurrentSection].Event, 0x4010008, 0);

                DeferFlag = 0;
            }
            else
            {
                StateSwitchStateData(In, CurrentSection, StateHandlerDeCompression, Moving);
            }

            In->SectionList[CurrentSection].Data2 = 2;

            PhysicsMode = 3;

            ResetPhysics(In->CharacterInstance, -16);
            break;
        case 0x4020000:
            SwimData = (evPhysicsSwimData *)Ptr->Data;

            if ((SwimData->Depth < 0) && ((In->CharacterInstance->zVel == 0) && (In->CharacterInstance->zAccl >= 0)))
            {
                ResetPhysics(In->CharacterInstance, -16);
            }

            Raziel.Mode &= ~0x40000;

            razEnterWater(In, CurrentSection, (evPhysicsSwimData *)Ptr->Data);

            if (((SwimData->WaterDepth < 0) && (SwimData->WaterDepth != -32767)) && (Raziel.CurrentPlane == 1))
            {
                ControlFlag |= 0x2000000;
            }

            break;
        case 0x20000001:
            if (((Raziel.Mode != 0x10) && (Raziel.Mode != 0x20)) && (CurrentSection == 0))
            {
                SetDropPhysics(In->CharacterInstance, &Raziel);
            }

            break;
        case 0x80000001:
            if ((Raziel.Senses.heldClass != 0x3) && (!(ControlFlag & 0x2000000)) && (CurrentSection == 0))
            {
                StateSwitchStateCharacterData(In, StateHandlerGlide, 3);
            }

            break;
        case 0x2000000:
            razPickupAndGrab(In, CurrentSection);
            break;
        case 0x40005:
            if (Raziel.HitPoints < 100000)
            {
                StateSwitchStateData(In, CurrentSection, StateHandlerStumble, 0);
            }

            break;
        case 0x1000001:
        case 0x4000001:
        case 0x8000000:
        case 0x80000000:
        case 0x80000008:
        case 0x80000020:
            break;
        default:
            DefaultStateHandler(In, CurrentSection, Data);
        }

        DeMessageQueue(&In->SectionList[CurrentSection].Event);
    }
}

void StateHandlerSlide(CharacterState *In, int CurrentSection, intptr_t Data)
{
    Message *Ptr;

    while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
    {
        switch (Ptr->ID)
        {
        case 0x100001:
            if (CurrentSection == 0)
            {
                Raziel.Mode = 0x400000;

                ControlFlag = 0x509;

                PhysicsMode = 0;

                ResetPhysics(In->CharacterInstance, -16);

                Raziel.soundHandle = SOUND_Play3dSound(&In->CharacterInstance->position, 30, 0, 60, 3500);
            }

            break;
        case 0x100004:
            if (Raziel.soundHandle != 0)
            {
                SndEndLoop(Raziel.soundHandle);

                Raziel.soundHandle = 0;
            }

            break;
        case 0x100000:
            StateSwitchStateData(In, CurrentSection, StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
            break;
        case 0x4010008:
            StateSwitchStateData(In, CurrentSection, StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
            break;
        case 0x4010200:
        case 0x20000001:
        case 0x80000000:
        case 0x80000001:
        case 0x80000008:
        case 0x80000020:
            break;
        default:
            DefaultStateHandler(In, CurrentSection, Data);
        }

        DeMessageQueue(&In->SectionList[CurrentSection].Event);
    }
}

void StateHandlerBlock(CharacterState *In, int CurrentSection, intptr_t Data)
{
    Message *Ptr;
    int Anim;

    Anim = G2EmulationQueryAnimation(In, CurrentSection);

    if (CurrentSection == 0)
    {
        BlockCount++;
    }

    while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
    {
        switch (Ptr->ID)
        {
        case 0x100001:
            if (CurrentSection == 0)
            {
                Raziel.Mode = 0x800000;

                ControlFlag = 0x8009;

                PhysicsMode = 3;

                SteerSwitchMode(In->CharacterInstance, 0);

                G2EmulationSwitchAnimationCharacter(In, 81, 0, 10, 1);
            }

            In->SectionList[CurrentSection].Data1 = 0;
            break;
        case 0x100004:
            COLLIDE_SegmentCollisionOn(In->CharacterInstance, 1);

            BlockCount = 0;

            FX_EndPassthruFX(In->CharacterInstance);
            break;
        case 0x8000000:
            if (CurrentSection == 0)
            {
                if ((Anim == 82) || (Anim == 108))
                {
                    StateSwitchStateCharacterData(In, StateHandlerIdle, SetControlInitIdleData(0, 0, 3));

                    if (Anim == 108)
                    {
                        razSetPlayerEventHistory(0x10000);
                    }
                }
                else if (In->SectionList[CurrentSection].Data1 != 0)
                {
                    Rotation rot;

                    G2EmulationSwitchAnimationCharacter(In, 108, 0, 5, 1);

                    COLLIDE_SegmentCollisionOff(In->CharacterInstance, 1);

                    razGetRotFromNormal(&Raziel.Senses.ForwardNormal, &rot);

                    In->CharacterInstance->rotation.z = rot.z;
                }
                else
                {
                    G2EmulationSwitchAnimationCharacter(In, 82, 0, 3, 1);

                    Raziel.alarmTable = 1600;

                    In->CharacterInstance->anim.section->swAlarmTable = &Raziel.alarmTable;
                }
            }

            break;
        case 0x8000004:
            if ((Raziel.Senses.EngagedMask & 0x1))
            {
                razGetForwardNormal(In->CharacterInstance, Raziel.Senses.EngagedList->instance);

                SetupReaction(In->CharacterInstance, Raziel.Senses.EngagedList->instance);

                INSTANCE_Post(Raziel.Senses.EngagedList->instance, 0x800000, SetObjectData(-Raziel.Senses.ForwardNormal.x, -Raziel.Senses.ForwardNormal.y, 6, NULL, 0));
            }

            break;
        case 0:
            if ((CurrentSection == 0) && (Anim != 108))
            {
                StateSwitchStateCharacterData(In, StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
            }

            break;
        case 0x4010400:
        {
            evPhysicsEdgeData *EdgeData;

            EdgeData = (evPhysicsEdgeData *)Ptr->Data;

            if (((EdgeData->rc & 0x20000)) && (((Raziel.Abilities & 0x1)) && (Raziel.CurrentPlane == 2)))
            {
                SVECTOR startVec;
                SVECTOR endVec;

                PHYSICS_GenericLineCheckSetup(0, 0, 0, &startVec);
                PHYSICS_GenericLineCheckSetup(0, -320, 0, &endVec);

                if ((PHYSICS_CheckForObjectCollide(gameTrackerX.playerInstance, &startVec, &endVec, 1) == 0) && (In->SectionList[CurrentSection].Data1 == 0))
                {
                    FX_StartPassthruFX(In->CharacterInstance, EdgeData->Normal1, EdgeData->Delta);

                    In->SectionList[CurrentSection].Data1 = 1;
                }
            }

            break;
        }
        case 0x4000001:
        case 0x4010200:
        case 0x80000000:
            break;
        default:
            DefaultStateHandler(In, CurrentSection, Data);
        }

        DeMessageQueue(&In->SectionList[CurrentSection].Event);
    }
}

void StateHandlerDeCompression(CharacterState *In, int CurrentSection, intptr_t Data)
{
    Message *Ptr;
    short temp; // not from decls.h

    In->CharacterInstance->cachedTFace = -1;

    while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
    {
        switch (Ptr->ID)
        {
        case 0x100001:
            if (CurrentSection == 0)
            {
                In->SectionList[CurrentSection].Data1 = Raziel.Mode;

                ControlFlag = 0x119;

                Raziel.Mode |= 0x1000;

                PhysicsMode = 3;

                SteerSwitchMode(In->CharacterInstance, 0);
            }
            else
            {
                In->SectionList[CurrentSection].Data1 = In->SectionList[CurrentSection - 1].Data1;
            }

            if (CurrentSection == 0)
            {
                temp = G2EmulationQueryAnimation(In, 0);

                if (((Raziel.Mode & 0x20)) || (temp == 17) || (temp == 18) || (temp == 19) || (In->CharacterInstance->zVel < -256))
                {
                    if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 44, NULL, NULL) != 0)
                    {
                        G2EmulationSwitchAnimationCharacter(In, 41, 0, 1, 1);
                    }
                }
                else if (!(Raziel.Mode & 0x10))
                {
                    if (Ptr->Data != 0)
                    {
                        if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 12, NULL, NULL) != 0)
                        {
                            G2EmulationSwitchAnimationCharacter(In, 29, 0, 0, 1);
                        }
                    }
                    else if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 28, NULL, NULL) != 0)
                    {
                        G2EmulationSwitchAnimationCharacter(In, 37, 0, 1, 1);
                    }
                }
                else if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 28, NULL, NULL) != 0)
                {
                    G2EmulationSwitchAnimationCharacter(In, 37, 0, 1, 1);
                }
            }

            break;
        case 0x100004:
            SetPhysics(In->CharacterInstance, -16, 0, 0, 0);
            break;
        case 0x8000000:
            if ((*PadData & RazielCommands[6]))
            {
                if (CurrentSection == 0)
                {
                    StateSwitchStateCharacterData(In, StateHandlerCrouch, 0);
                }
            }
            else if ((*PadData & 0x8000000F))
            {
                if (CurrentSection == 0)
                {
                    if (G2EmulationQueryAnimation(In, 0) == 29)
                    {
                        StateSwitchStateCharacterData(In, StateHandlerMove, 0);
                    }
                    else
                    {
                        StateSwitchStateCharacterData(In, StateHandlerStartMove, 5);
                    }
                }
            }
            else
            {
                StateSwitchStateData(In, CurrentSection, StateHandlerIdle, SetControlInitIdleData(0, 0, 4));

                Raziel.Mode = 0x1;
            }

            In->SectionList[CurrentSection].Data1 = 0;
            break;
        case 0x10000000:
        case 0x80000000:
        case 0x80000020:
            break;
        default:
            DefaultStateHandler(In, CurrentSection, Data);
        }

        DeMessageQueue(&In->SectionList[CurrentSection].Event);
    }
}

void StateHandlerGlide(CharacterState *In, int CurrentSection, intptr_t Data)
{
    Message *Ptr;
    int Frame;
    int Anim;
    int moving;
    evObjectDraftData *draft;
    evPhysicsSwimData *SwimData;

    moving = 0;

    Anim = G2EmulationQueryAnimation(In, CurrentSection);

    Frame = G2EmulationQueryFrame(In, CurrentSection);

    if (CurrentSection == 0)
    {
        In->SectionList[CurrentSection].Data2 &= ~0x2;
    }

    while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
    {
        switch (Ptr->ID)
        {
        case 0x100001:
            if (CurrentSection == 0)
            {
                Raziel.Mode = 0x2000;

                ControlFlag = 0x518;

                PhysicsMode = 0;

                SteerSwitchMode(In->CharacterInstance, 8);

                DeInitAlgorithmicWings(In->CharacterInstance);

                if (In->CharacterInstance->zVel < 0)
                {
                    SetPhysics(In->CharacterInstance, 0, 0, 52, -24);
                }
            }

            In->SectionList[CurrentSection].Data1 = 1;
            In->SectionList[CurrentSection].Data2 = 0;

            G2EmulationSwitchAnimation(In, CurrentSection, 16, Ptr->Data, 5, 1);
            break;
        case 0x100004:
            if (CurrentSection == 0)
            {
                InitAlgorithmicWings(In->CharacterInstance);

                In->SectionList[CurrentSection].Data2 = 0;
            }

            break;
        case 0x8000000:
            if (In->SectionList[CurrentSection].Data1 != 0)
            {
                G2EmulationSwitchAnimationAlpha(In, CurrentSection, 18, 0, 5, 2, 4);

                In->SectionList[CurrentSection].Data1 = 0;
            }

            break;
        case 0x8000003:
            if (Anim == 16)
            {
                SetPhysics(In->CharacterInstance, 0, 0, 52, -24);
            }

            break;
        case 0x4010008:
            StateSwitchStateData(In, CurrentSection, StateHandlerDeCompression, 0);
            break;
        case 0x20000001:
            if (((Frame > 12) || (Anim != 16)) && (!(*PadData & RazielCommands[3])))
            {
                if (CurrentSection == 0)
                {
                    SetPhysics(In->CharacterInstance, -16, 0, 0, 0);

                    if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 24, NULL, NULL) != 0)
                    {
                        G2EmulationSwitchAnimationCharacter(In, 36, 0, 4, 1);
                    }
                }

                StateSwitchStateCharacterData(In, StateHandlerFall, 0);
            }
            else
            {
                EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x20000001, 0);
            }

            break;
        case 0x20000004:
            if (CurrentSection == 0)
            {
                SetExternalTransitionForce((Force *)&ExternalForces, In->CharacterInstance, 4, 0, 52, -24);
            }

            G2EmulationSwitchAnimation(In, CurrentSection, 18, 0, 5, 2);

            In->SectionList[CurrentSection].Data1 = 0;
            break;
        case 0x80000004:
            if (CurrentSection == 1)
            {
                SetExternalTransitionForce((Force *)&ExternalForces, In->CharacterInstance, 4, 0, 24, -24);
            }

            G2EmulationSwitchAnimation(In, CurrentSection, 17, 0, 5, 2);

            In->SectionList[CurrentSection].Data1 = 0;

            if ((In->SectionList[CurrentSection].Data2 & 0x1))
            {
                razSetPlayerEventHistory(0x4000);
            }

            break;
        case 0x10000000:
            if (Anim != 16)
            {
                if (Raziel.Bearing > 0)
                {
                    G2EmulationSwitchAnimation(In, CurrentSection, 43, 0, 6, 1);

                    In->SectionList[CurrentSection].Data1 = 2;
                }

                if (Raziel.Bearing < 0)
                {
                    G2EmulationSwitchAnimation(In, CurrentSection, 44, 0, 6, 1);

                    In->SectionList[CurrentSection].Data1 = 2;
                }

                if (Raziel.Bearing == 0)
                {
                    moving = 1;

                    if (In->SectionList[CurrentSection].Data1 == 2)
                    {
                        G2EmulationSwitchAnimation(In, CurrentSection, 18, 0, 5, 2);

                        In->SectionList[CurrentSection].Data1 = 0;
                    }
                }
            }

            moving = 1;
            break;
        case 0x4000007:
            if (CurrentSection == 0)
            {
                draft = (evObjectDraftData *)Ptr->Data;

                if (In->CharacterInstance->zVel > draft->maxVelocity)
                {
                    SetExternalForce(&ExternalForces[1], 0, 0, 0, 0, 0);
                }
                else
                {
                    SetExternalForce(&ExternalForces[1], 0, 0, draft->force, 0, 4092);
                }

                In->SectionList[CurrentSection].Data2 |= 0x3;

                Raziel.playerEvent |= 0x4000;
            }

            break;
        case 0x4020000:
            SwimData = (evPhysicsSwimData *)Ptr->Data;

            if (((SwimData->WaterDepth < 0) && (SwimData->WaterDepth != -32767)) && (Raziel.CurrentPlane == 1))
            {
                if (CurrentSection == 0)
                {
                    SetPhysics(In->CharacterInstance, -16, 0, 0, 0);
                }

                G2EmulationSwitchAnimation(In, CurrentSection, 19, 0, 4, 1);

                StateSwitchStateData(In, CurrentSection, StateHandlerFall, 0);
            }

            break;
        case 0x40005:
        case 0x1000001:
        case 0x4000001:
        case 0x80000000:
        case 0x80000008:
        case 0x80000020:
            break;
        default:
            DefaultStateHandler(In, CurrentSection, Data);
        }

        DeMessageQueue(&In->SectionList[CurrentSection].Event);
    }

    if ((CurrentSection == 0) && ((In->SectionList[CurrentSection].Data2 & 0x1)))
    {
        if (!(In->SectionList[CurrentSection].Data2 & 0x2))
        {
            if (moving != 0)
            {
                SetExternalTransitionForce((Force *)&ExternalForces, In->CharacterInstance, 10, 0, 52, -24);
            }
            else
            {
                SetExternalTransitionForce((Force *)&ExternalForces, In->CharacterInstance, 10, 0, 0, -24);
            }

            In->SectionList[CurrentSection].Data2 = 0;
        }
        else if (moving == 0)
        {
            In->CharacterInstance->yVel = 0;
        }
        else
        {
            In->CharacterInstance->yVel = 52;
        }
    }
}

void StateHandlerHang(CharacterState *In, int CurrentSection, intptr_t Data)
{
    Message *Ptr;
    int Anim;

    Anim = G2EmulationQueryAnimation(In, CurrentSection);

    while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
    {
        switch (Ptr->ID)
        {
        case 0x100001:
        {
            evControlInitHangData *data;

            data = (evControlInitHangData *)Ptr->Data;

            if (CurrentSection == 0)
            {
                if ((Raziel.Mode & 0x40000))
                {
                    CAMERA_ChangeToOutOfWater(&theCamera, In->CharacterInstance);
                }

                In->SectionList[CurrentSection].Data2 = In->CharacterInstance->attachedID;

                ControlFlag = 0x8001501;

                if (In->CharacterInstance->attachedID == 0)
                {
                    ControlFlag = 0xC001501;
                }

                In->CharacterInstance->attachedID = 0;

                Raziel.Mode = 0x100;

                if (data->instance != NULL)
                {
                    Raziel.attachedPlatform = data->instance;
                }

                SteerSwitchMode(In->CharacterInstance, 0);
            }

            if (Raziel.iVelocity.z < 0)
            {
                G2EmulationSwitchAnimation(In, CurrentSection, 7, 4, data->frames, 1);
            }
            else
            {
                G2EmulationSwitchAnimation(In, CurrentSection, 7, 0, data->frames, 1);
            }

            In->SectionList[CurrentSection].Data1 = 0;

            PhysicsMode = 3;

            SetPhysics(In->CharacterInstance, -16, 0, 0, 0);

            In->CharacterInstance->flags2 &= ~0x40;
            break;
        }
        case 0x100004:
            if (CurrentSection == 0)
            {
                Raziel.attachedPlatform = NULL;

                ControlFlag &= ~0x400;

                In->CharacterInstance->attachedID = 0;
            }

            In->CharacterInstance->flags2 |= 0x40;
            break;
        case 0x10000000:
            if ((Ptr->Data == 0x10000003) && (Anim == 6))
            {
                if (CurrentSection == 1)
                {
                    SetPhysics(In->CharacterInstance, -16, 0, 0, 0);

                    if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 24, NULL, NULL) != 0)
                    {
                        G2EmulationSwitchAnimationCharacter(In, 36, 0, 4, CurrentSection);
                    }

                    StateSwitchStateCharacterData(In, StateHandlerFall, 0);
                }
            }
            else if ((Ptr->Data == 0x10000001) || ((*PadData & 0x1)))
            {
                if (In->SectionList[CurrentSection].Data1 == 0)
                {
                    G2EmulationSwitchAnimation(In, CurrentSection, 8, 0, 2, 1);

                    PurgeMessageQueue(&In->SectionList[CurrentSection].Event);
                }

                In->SectionList[CurrentSection].Data1 = 1;
            }

            break;
        case 0x8000001:
            if (In->SectionList[CurrentSection].Data1 == 1)
            {
                EnMessageQueueData(&In->SectionList[CurrentSection].Event, 0x100000, 0);
            }

            break;
        case 0x8000003:
            if (CurrentSection == 0)
            {
                ControlFlag &= ~0x400;

                if (In->SectionList[CurrentSection].Data2 != 0)
                {
                    ControlFlag |= 0x4000000;
                }
            }

            break;
        case 0x8000000:
            if (In->SectionList[CurrentSection].Data1 == 1)
            {
                EnMessageQueueData(&In->SectionList[CurrentSection].Event, 0x100000, 0);

                In->CharacterInstance->rotation.x = 0;
                In->CharacterInstance->rotation.y = 0;
            }
            else if (CurrentSection == 2)
            {
                G2EmulationSwitchAnimation(In, 2, 0, 0, 3, CurrentSection);
            }
            else
            {
                G2EmulationSwitchAnimation(In, CurrentSection, 6, 0, 3, 0);
            }

            if (CurrentSection == 0)
            {
                ControlFlag &= ~0x400;
            }

            break;
        case 0x100000:
            if ((*PadData & 0x8000000F))
            {
                StateSwitchStateData(In, CurrentSection, StateHandlerStartMove, 10);

                In->SectionList[CurrentSection].Data1 = 0;
            }
            else
            {
                StateSwitchStateData(In, CurrentSection, StateHandlerIdle, SetControlInitIdleData(0, 0, 3));

                Raziel.Mode = 0x1;

                In->SectionList[CurrentSection].Data1 = 0;
            }

            break;
        case 0x100014:
            if (CurrentSection == 1)
            {
                SetPhysics(In->CharacterInstance, -16, 0, 0, 0);

                if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 24, NULL, NULL) != 0)
                {
                    G2EmulationSwitchAnimationCharacter(In, 36, 0, 4, CurrentSection);
                }

                StateSwitchStateCharacterData(In, StateHandlerFall, 0);
            }

            break;
        case 0x1000000:
        case 0x1000001:
        case 0x4000001:
        case 0x4000011:
        case 0x4010010:
        case 0x4010200:
        case 0x80000000:
        case 0x80000008:
        case 0x80000020:
            break;
        default:
            DefaultStateHandler(In, CurrentSection, Data);
        }

        DeMessageQueue(&In->SectionList[CurrentSection].Event);
    }
}

void StateHandlerPushObject(CharacterState *In, int CurrentSection, intptr_t Data)
{

    Message *Ptr;

    if (!(Raziel.Senses.EngagedMask & 3))
    {
        EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x100000, 0);
    }


    while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
    {
        switch (Ptr->ID)
        {
        case 0x100001:
            if (CurrentSection == 0)
            {
                Raziel.Mode = 0x400;
                ControlFlag = 0x08001108;
                PhysicsMode = 3;
                SteerSwitchMode(In->CharacterInstance, 0);
                razCenterWithBlock(In->CharacterInstance, Raziel.Senses.EngagedList->instance, -0x8D);
            }
            In->SectionList[CurrentSection].Data1 = 0;
            razSetPlayerEventHistory(1);
            break;
        case 0x100000:
            StateSwitchStateData(In, CurrentSection, &StateHandlerIdle, SetControlInitIdleData(0, 0, 6));
            break;
        case 0x08000000:
        case 0x08000001:
            if (!(*PadData & RazielCommands[2] && Raziel.Senses.EngagedMask & 2))
            {
                if (In->SectionList[CurrentSection].Data1 != 0)
                {
                    StateSwitchStateData(In, CurrentSection, &StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
                    ControlFlag |= 1;
                    break;
                }
                if (CurrentSection == 2)
                {
                    G2EmulationSwitchAnimation(In, 2, 0, 0, 3, 2);
                }
                else
                {
                    G2EmulationSwitchAnimation(In, CurrentSection, 0x17, 0, 3, 1);
                }
                In->SectionList[CurrentSection].Data1 = 1;

                if (CurrentSection == 0)
                {
                    Raziel.alarmTable = 400;
                    In->CharacterInstance->anim.section[0].swAlarmTable = &Raziel.alarmTable;
                }
            }
            else
            {
                StateSwitchStateData(In, CurrentSection, &StateHandlerDragObject, 0);
            }
            break;
        case 0x08000004:
        {
            evMonsterHitData *data;
            data = (evMonsterHitData *)SetObjectData(-Raziel.Senses.ForwardNormal.x, -Raziel.Senses.ForwardNormal.y, 6, 0, 0);
            INSTANCE_Post(Raziel.Senses.EngagedList->instance, 0x800000, (intptr_t)data);
            if (data->power == 0)
            {
                INSTANCE_Post(In->CharacterInstance, 0x100000, 0);
            }
            break;
        }
        case 0x80000008:
            break;
        default:
            DefaultStateHandler(In, CurrentSection, Data);
        }
        DeMessageQueue(&In->SectionList[CurrentSection].Event);
    }
}


void StateHandlerBreakOff(CharacterState *In, int CurrentSection, intptr_t Data)
{

    Message *Ptr;

    while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
    {

        switch (Ptr->ID)
        {

        case 0x100001:
            if (CurrentSection == 0)
            {

                PhysObInteractProperties *interactProp;
                Instance *Inst;
                int anim;

                if (Raziel.Senses.EngagedMask & 0x10)
                {
                    Inst = Raziel.Senses.EngagedList[4].instance;
                }
                else
                {
                    Inst = Raziel.Senses.EngagedList[13].instance;
                }

                interactProp = (PhysObInteractProperties *)INSTANCE_Query(Inst, 0x15);

                if (Raziel.Senses.EngagedMask & 0x10)
                {
                    anim = interactProp->razielAnim;
                }
                else
                {
                    anim = interactProp->razielAuxAnim;
                }

                In->SectionList[CurrentSection].Data2 = (intptr_t)Inst;

                if (anim == 0xFF)
                {
                    SetTimer(interactProp->frame + 1);
                }
                else
                {
                    G2EmulationInstanceToInstanceSwitchAnimationCharacter(In->CharacterInstance, Inst, anim, 0, 0, 1);
                    if (interactProp->frame != 0xFF)
                    {
                        G2AnimSection *temp; // not from decls.h

                        Raziel.alarmTable = interactProp->frame * 0x64;
                        temp = &In->CharacterInstance->anim.section[CurrentSection];
                        temp->swAlarmTable = &Raziel.alarmTable;
                    }
                }
                ControlFlag = 0x08041108;
                PhysicsMode = 3;
                switch (interactProp->mode)
                {
                case 0:
                    razAlignYRotMoveInterp(In->CharacterInstance, Inst, interactProp->distance, 0, 3, 0);
                    break;
                case 1:
                    razAlignYMoveRot(Inst, -interactProp->distance, &In->CharacterInstance->position, &In->CharacterInstance->rotation, 0);
                    break;
                case 3:
                    razAlignYRotMoveInterp(In->CharacterInstance, Inst, interactProp->distance, 0, interactProp->mode, 0x800);
                    break;
                case 4:
                    razAlignYMoveRot(Inst, -interactProp->distance, &In->CharacterInstance->position, &In->CharacterInstance->rotation, 0x800);
                    break;
                }
            }
            break;
        case 0x08000004:
            goto label;
        case 0x100015:
            if (CurrentSection == 0)
            {

                PhysObInteractProperties *interactProp;
                int action;
                int condition;
                Instance *Inst;

                StateSwitchStateCharacterData(In, &StateHandlerIdle, SetControlInitIdleData(0, 0, 3));

            label:
                interactProp = (PhysObInteractProperties *)INSTANCE_Query((Instance *)(intptr_t)In->SectionList[CurrentSection].Data2, 0x15);

                if (Raziel.Senses.EngagedMask & 0x10)
                {
                    action = interactProp->action;
                    condition = interactProp->conditions;
                }
                else
                {
                    action = interactProp->auxAction;
                    condition = interactProp->auxConditions;
                }

                if (condition & 0x1FE)
                {
                    Inst = Raziel.soulReaver;
                    razReaverImbue(action);
                }
                else
                {
                    Inst = In->CharacterInstance;
                }
                INSTANCE_Post((Instance *)(intptr_t)In->SectionList[CurrentSection].Data2, 0x800023, SetObjectBreakOffData(Inst, 0x31, 0, 0, 0, 0, (int)(short)action));
            }
            break;
        case 0x08000000:
        case 0x08000001:
            StateSwitchStateData(In, CurrentSection, &StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
            break;
        case 0x100004:
        case 0x80000000:
            break;
        default:
            DefaultStateHandler(In, CurrentSection, Data);
        }
        DeMessageQueue(&In->SectionList[CurrentSection].Event);
    }
}

void StateHandlerPullSwitch(CharacterState *In, int CurrentSection, intptr_t Data) 
{
    Message *Ptr;                              
    int switchDone;                            
    int hitPosted;                            
    short offset; // not from decls.h

    switchDone = 0;
    
    hitPosted = 0;
    
    G2EmulationQueryAnimation(In, CurrentSection);
    
    while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL) 
    {
        switch (Ptr->ID)
        {                          
        case 0x100001:
        {
            PhysObSwitchProperties *switchData;         
                
            if (CurrentSection == 0) 
            {
                int switchStatus;                        
                int switchSuccess;                       
                Instance *inst;                           
                PhysObSwitchProperties *switchProperties;  
                int extraZ;                                
                
                ControlFlag = 0x9041108;
                
                PhysicsMode = 3;
                
                SteerSwitchMode(In->CharacterInstance, 0);
                
                switchSuccess = 1;
                
                if ((Raziel.Senses.EngagedMask & 0x8))
                {
                    extraZ = 0;
                    
                    inst = Raziel.Senses.EngagedList[3].instance;
                    
                    switchProperties = (PhysObSwitchProperties*)INSTANCE_Query(inst, 23);
                    switchStatus = INSTANCE_Query(inst, 5);
                    switchData = (PhysObSwitchProperties*)INSTANCE_Query(inst, 28);
                    
                    if (INSTANCE_Query(inst, 4) == 9) 
                    {
                        offset = MON_FacingOffset(In->CharacterInstance, inst) & 0xFFF;
                        
                        if (offset > 2048)
                        {
                            offset |= 0xF000;
                        }
                        
                        if (((offset + 1024) < 0) || ((offset + 1024) > 2048)) 
                        { 
                            switchStatus |= 0x1;
                            
                            extraZ = 2048;
                        } 
                        else 
                        {
                            switchStatus &= ~0x1;
                        }
                        
                        INSTANCE_Post(inst, 0x80002A, switchStatus);
                    }
                    
                    if (!(switchStatus & 0x1)) 
                    {
                        if ((switchStatus & 0x2)) 
                        {
                            if (switchProperties->razielOnAnim == 0xFF)
                            {
                                switchSuccess = 0;
                            }
                            else 
                            {
                                G2EmulationInstanceToInstanceSwitchAnimationCharacter(In->CharacterInstance, Raziel.Senses.EngagedList[3].instance, switchProperties->razielOnAnim, 0, 0, 1);
                            }
                        }
                        else if (switchProperties->razielFailedOnAnim == 0xFF)
                        {
                            switchSuccess = 0;
                        }
                        else 
                        {
                            G2EmulationInstanceToInstanceSwitchAnimationCharacter(In->CharacterInstance, Raziel.Senses.EngagedList[3].instance, switchProperties->razielFailedOnAnim, 0, 0, 1);
                        }
                    }
                    else if ((switchStatus & 0x2)) 
                    {
                        if (switchProperties->razielOffAnim == 0xFF) 
                        {
                            switchSuccess = 0;
                        }
                        else 
                        {
                            G2EmulationInstanceToInstanceSwitchAnimationCharacter(In->CharacterInstance, Raziel.Senses.EngagedList[3].instance, switchProperties->razielOffAnim, 0, 0, 1);
                        }
                    }
                    else if (switchProperties->razielEnableAnim == 0xFF) 
                    {
                        switchSuccess = 0;
                    }
                    else 
                    {
                        G2EmulationInstanceToInstanceSwitchAnimationCharacter(In->CharacterInstance, Raziel.Senses.EngagedList[3].instance, switchProperties->razielEnableAnim, 0, 0, 1);
                    }
                    
                    if (switchSuccess == 0)
                    {
                        INSTANCE_Post(In->CharacterInstance, 0x100000, switchStatus);
                    } 
                    else 
                    {
                        razAlignYMoveRot(Raziel.Senses.EngagedList[3].instance, switchData->Distance, &In->CharacterInstance->position, &In->CharacterInstance->rotation, (short)extraZ);
                        
                        INSTANCE_Post(Raziel.Senses.EngagedList[3].instance, 0x800020, 0);
                        
                        if (INSTANCE_Query(inst, 4) == 9)
                        {
                            razSetPlayerEventHistory(0x8);
                        } 
                        else 
                        {
                            razSetPlayerEventHistory(0x4);
                        }
                    }
                }
            }
            
            break;
        }
        case 0x100000:
        {
            int switchStatus;                       
            Instance *inst;                          
            PhysObSwitchProperties *switchProperties; 
            int switchClass;                            
            
            inst = Raziel.Senses.EngagedList[3].instance;
            
            switchClass = INSTANCE_Query(inst, 4);
            switchProperties = (PhysObSwitchProperties*)INSTANCE_Query(Raziel.Senses.EngagedList[3].instance, 23);
            switchStatus = INSTANCE_Query(inst, 5);
            
            if (switchClass == 6) 
            {
                switchDone = 1;
                
                if ((!(*PadData & RazielCommands[2])) || (Ptr->Data != 0)) 
                {
                    StateSwitchStateData(In, CurrentSection, StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
                    
                    ControlFlag |= 0x1;
                    
                    if (CurrentSection == 0) 
                    {
                        INSTANCE_Post(Raziel.Senses.EngagedList[3].instance, 0x800020, 0);
                    }
                }
                else 
                {
                    EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x100000, 0);
                }
            } 
            else if (switchClass == 9) 
            {
                if ((switchStatus & 0x1)) 
                {
                    SetTimer(switchProperties->onWalkTimer + 6);
                } 
                else 
                {
                    SetTimer(switchProperties->offWalkTimer + 6);
                }
                
                G2EmulationSwitchAnimation(In, CurrentSection, 123, 0, 6, 2);
                
                if (CurrentSection == 0) 
                {
                    INSTANCE_Post(Raziel.Senses.EngagedList[3].instance, 0x80002D, 0x14);
                }
            }
            else 
            {
                StateSwitchStateData(In, CurrentSection, StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
                
                ControlFlag |= 0x1;
            }
            
            break;
        }
        case 0x100015:
            StateSwitchStateData(In, CurrentSection, StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
            break;
        case 0x8000000:
            if (CurrentSection == 2) 
            {
                G2EmulationSwitchAnimation(In, 2, 0, 0, 3, CurrentSection);
            } 
            else
            {
                EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x100000, 0);
            }
            
            break;
        case 0x1000000:
        {
            evMonsterHitData *data; 
                
            data = (evMonsterHitData*)Ptr->Data;
            
            if (switchDone != 0) 
            {
                EnMessageQueueData(&In->SectionList[CurrentSection].Event, 0x100000, 0x1);
            }
            
            if (hitPosted == 0) 
            {
                hitPosted = 1;
                
                EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x1000000, SetMonsterHitData(data->sender, data->lastHit, data->power, data->knockBackDistance, data->knockBackDuration));
            }
            
            break;
        }
            
        case 0x1000001:
        case 0x80000000:
        case 0x80000008:
        case 0x80000010:
        case 0x80000020:
            break;
        default:
            DefaultStateHandler(In, CurrentSection, Data);
            break;
        }
        
        DeMessageQueue(&In->SectionList[CurrentSection].Event);
    }
    
    if (CurrentSection == 0) 
    {
        razApplyMotion(In, 0);
    }
}

INCLUDE_ASM("asm/nonmatchings/Game/RAZIEL/RAZIEL", StateHandlerDragObject);

void StateHandlerPickupObject(CharacterState *In, int CurrentSection, intptr_t Data)
{
    Message *Ptr;

    while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
    {
        switch (Ptr->ID)
        {
        case 0x100001:
            if (CurrentSection == 1)
            {
                long colorArray[1];

                colorArray[0] = 0xFF5400;

                ControlFlag = 0x1108;

                Raziel.Mode |= 0x800;
                Raziel.Senses.Flags |= 0x80;

                PhysicsMode = 3;

                FX_DoInstanceOneSegmentGlowWithTime(In->CharacterInstance, 41, (long *)&colorArray, 1, 0, 75, 75, 10);

                razSetPlayerEventHistory(0x10);
            }

            break;
        case 0x100004:
            if (CurrentSection == 1)
            {
                ControlFlag &= ~0x1000;

                FX_StopAllGlowEffects(In->CharacterInstance, 0);
            }

            break;
        case 0x8000000:
            if (CurrentSection == 1)
            {
                FX_StopAllGlowEffects(In->CharacterInstance, 0);
            }

            if ((Raziel.Mode & 0x40000))
            {
                StateSwitchStateData(In, CurrentSection, StateHandlerSwim, 0);
            }
            else if ((Raziel.returnState == StateHandlerIdle) || (Raziel.returnState == NULL))
            {
                StateSwitchStateData(In, CurrentSection, StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
            }
            else
            {
                StateSwitchStateData(In, CurrentSection, Raziel.returnState, 0);
            }

            Raziel.returnState = NULL;
            break;
        case 0x80000000:
        case 0x80000020:
            break;
        default:
            DefaultStateHandler(In, CurrentSection, Data);
        }

        DeMessageQueue(&In->SectionList[CurrentSection].Event);
    }
}

void StateHandlerAutoFace(CharacterState *In, int CurrentSection, intptr_t Data)
{
    Message *Ptr;
    int Frames;
    int Anim;

    Anim = G2EmulationQueryAnimation(In, CurrentSection);
    Frames = 4;

    if (CurrentSection == 2)
    {
        Frames = 6;
    }

    while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
    {
        switch (Ptr->ID)
        {
        case 0x100001:
            ControlFlag = 0x1210B;
            if (*PadData & RazielCommands[7])
            {
                Raziel.Mode |= 0x02000002;
            }
            else
            {
                Raziel.Mode |= 0x02000001;
            }
            PhysicsMode = 3;
            if (CurrentSection == 0)
            {
                SteerSwitchMode(In->CharacterInstance, 5);
            }
            In->SectionList[CurrentSection].Data1 = 0;
            if (razSwitchVAnimGroup(In->CharacterInstance, CurrentSection, 0x54, -1, -1) != 0)
            {
                G2EmulationSwitchAnimation(In, CurrentSection, 0x37, 0, 6, 2);
            }
            razSetPlayerEventHistory(0x20);
            break;
        case 0x100004:
            if (CurrentSection == 0)
            {
                razResetPauseTranslation(In->CharacterInstance);
            }
            break;
        case 0x8000003:
            if (Anim == 0x37)
            {
                In->SectionList[CurrentSection].Data1 = 1;
            }
        case 0x80000001:
            break;
        case 0x4010080:
            if (CurrentSection == 0)
            {
                if (Ptr->Data != 0)
                {
                    razResetPauseTranslation(In->CharacterInstance);
                    break;
                }
                razSetPauseTranslation(In->CharacterInstance);
            }
            break;
        case 0x1000001:
            if (CurrentSection == 1 && In->CharacterInstance->LinkChild != 0)
            {
                Ptr->Data = 0;
            }

            switch (Ptr->Data)
            {
            case 0x10001001:
                if (Anim != 0xC)
                {
                    G2EmulationSwitchAnimation(In, CurrentSection, 0xC, 0, Frames, 2);
                    break;
                }
                if (PadData[1] & RazielCommands[3] && !(Raziel.Senses.Flags & 0x80))
                {
                    StateSwitchStateData(In, CurrentSection, StateHandlerAttack2, 5);
                    if (CurrentSection == 2)
                    {
                        StateSwitchStateData(In, 1, StateHandlerAttack2, 5);
                    }
                }
                break;
            case 0x10001003:
                if (Anim != 0xD && Anim != 0x5B)
                {
                    G2EmulationSwitchAnimation(In, CurrentSection, 0xD, 0, Frames, 2);
                    break;
                }
                if (PadData[1] & RazielCommands[3])
                {
                    if (Anim != 0x5B)
                    {
                        SteerSwitchMode(In->CharacterInstance, 0xE);
                        G2EmulationSwitchAnimation(In, CurrentSection, 0x5B, 0, 0, 1);
                    }
                }
                break;
            case 0x10001004:
                if (Anim != 0xE && Anim != 0x5C)
                {
                    G2EmulationSwitchAnimation(In, CurrentSection, 0xE, 0, Frames, 2);
                    break;
                }
                if (PadData[1] & RazielCommands[3])
                {
                    if (Anim != 0x5C)
                    {
                        SteerSwitchMode(In->CharacterInstance, 0xE);
                        G2EmulationSwitchAnimation(In, CurrentSection, 0x5C, 0, 0, 1);
                    }
                }
                break;
            case 0x10001002:
                if (Anim != 0xF && Anim != 0x5D)
                {
                    G2EmulationSwitchAnimation(In, CurrentSection, 0xF, 0, Frames, 2);
                }
                else if (PadData[1] & RazielCommands[3])
                {
                    if (Anim != 0x5D)
                    {
                        SteerSwitchMode(In->CharacterInstance, 0xE);
                        G2EmulationSwitchAnimation(In, CurrentSection, 0x5D, 0, 0, 1);
                    }
                }
            }

            break;
        case 0:
            if (Anim != 0x5B && Anim != 0x5C && Anim != 0x5D)
            {
                if (Raziel.nothingCounter < 6)
                {
                    if (Anim != 0x37)
                    {
                        StateInitIdle(In, CurrentSection, SetControlInitIdleData(1, 0, 6));
                    }
                }
                else
                {
                    StateSwitchStateData(In, CurrentSection, StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
                }
            }
            break;
        case 0x20000004:
            if (CurrentSection == 0)
            {
                Raziel.Senses.LastAutoFace = Raziel.Senses.EngagedList[6].instance;
            }
            break;
        case 0x80000002:
            if (Raziel.Senses.heldClass != 3)
            {
                Raziel.returnState = StateHandlerIdle;
                StateSwitchStateData(In, CurrentSection, StateHandlerSoulSuck, 0);
            }
            break;
        case 0x8000000:
            if (CurrentSection == 0)
            {
                SteerSwitchMode(In->CharacterInstance, 5);
            }
            In->SectionList[CurrentSection].Data1 = 0;
            G2EmulationSwitchAnimation(In, CurrentSection, 0x37, 0, 6, 2);
            break;
        case 0x100000:
            StateSwitchStateData(In, CurrentSection, StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
            break;
        case 0x10000000:
            if (!(ControlFlag & 2))
            {
                if (CurrentSection == 0)
                {
                    StateSwitchStateData(In, CurrentSection, StateHandlerStartMove, 0);
                    break;
                }
                EnMessageQueueData(&In->SectionList->Event, 0x100005, CurrentSection);
            }
            break;
        case 0x2000000:
            if (razPickupAndGrab(In, CurrentSection) != 0)
            {
                StateSwitchStateData(In, CurrentSection, StateHandlerAttack2, 0);
            }
            break;
        case 0x80000000:
            if (CurrentSection == 0 && !(Raziel.Senses.Flags & 0x80))
            {
                StateSwitchStateCharacterData(In, StateHandlerAttack2, 0);
            }
            break;
        default:
            DefaultStateHandler(In, CurrentSection, Data);
            break;
        }
        DeMessageQueue(&In->SectionList[CurrentSection].Event);
    }
    if (Raziel.nothingCounter == 6 || !(Raziel.Senses.EngagedMask & 0x40))
    {
        EnMessageQueueData(&In->SectionList[CurrentSection].Event, 0x100000, 0);
    }
}


void StateHandlerGlyphs(CharacterState *In, int CurrentSection, intptr_t Data)
{
    Message *Ptr;
    int hitPosted;
    evActionPlayHostAnimationData *ptr;
    evMonsterHitData *data;

    hitPosted = 0;

    G2EmulationQueryAnimation(In, CurrentSection);

    Raziel.invincibleTimer = 12288;

    while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
    {
        switch (Ptr->ID)
        {
        case 0x100001:
            if (CurrentSection == 0)
            {
                if (Ptr->Data == 0)
                {
                    INSTANCE_Post(Raziel.GlyphSystem, 0x80000010, (intptr_t)In->CharacterInstance);
                }

                Raziel.Mode |= 0x1;

                ControlFlag = 0x1100008;

                SteerSwitchMode(In->CharacterInstance, 16);

                razResetMotion(In->CharacterInstance);
            }

            In->SectionList[CurrentSection].Data1 = 1;

            PhysicsMode = 3;

            if (Ptr->Data != 0)
            {
                EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x80007, 0);
            }

            break;
        case 0x100004:
            if (CurrentSection == 0)
            {
                razReaverOn();

                if (GlyphIsGlyphOpen(Raziel.GlyphSystem) != 0)
                {
                    INSTANCE_Post(Raziel.GlyphSystem, 0x80000010, (intptr_t)In->CharacterInstance);
                }
            }

            Raziel.invincibleTimer = 0;
            break;
        case 0x8000003:
        case 0x8000000:
        case 0x100015:
            if (CurrentSection == 0)
            {
                CheckStringAnimation(In->CharacterInstance, Ptr->ID);
            }

            break;
        case 0x100000:
            if (In->SectionList[CurrentSection].Data1 == 0)
            {
                if (((Raziel.Mode & 0x40000)) && (Raziel.CurrentPlane == 1))
                {
                    StateSwitchStateData(In, CurrentSection, StateHandlerSwim, 0);

                    if (CurrentSection == 0)
                    {
                        razSetFadeEffect(4096, 0, 256);
                    }
                }
                else
                {
                    StateSwitchStateData(In, CurrentSection, StateHandlerIdle, SetControlInitIdleData(0, 0, 3));

                    if (CurrentSection == 0)
                    {
                        razSetFadeEffect(4096, 0, 256);
                    }
                }
            }
            else if ((!(Raziel.Mode & 0x40000)) && (In->SectionList[CurrentSection].Data1 != 2))
            {
                StateInitIdle(In, CurrentSection, SetControlInitIdleData(0, 0, 3));

                In->SectionList[CurrentSection].Data1 = 2;
            }

            break;
        case 0x80000010:
            if (In->SectionList[CurrentSection].Data1 != 0)
            {
                if (CurrentSection == 0)
                {
                    In->SectionList[CurrentSection].Data1 = 0;

                    INSTANCE_Post(Raziel.GlyphSystem, 0x80000010, (intptr_t)In->CharacterInstance);
                }

                if ((Raziel.Mode & 0x40000))
                {
                    StateSwitchStateData(In, CurrentSection, StateHandlerSwim, 0);
                }
                else
                {
                    StateSwitchStateData(In, CurrentSection, StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
                }
            }

            break;
        case 0x80006:
            if (CurrentSection == 0)
            {
                razPrepGlyph();

                razSwitchStringAnimation(In->CharacterInstance, 6);
            }

            In->SectionList[CurrentSection].Data1 = 0;
            break;
        case 0x80004:
            if (CurrentSection == 0)
            {
                razPrepGlyph();

                razSwitchStringAnimation(In->CharacterInstance, 5);
            }

            In->SectionList[CurrentSection].Data1 = 0;
            break;
        case 0x80003:
            if (CurrentSection == 0)
            {
                razPrepGlyph();

                razSwitchStringAnimation(In->CharacterInstance, 4);

                Raziel.effectsFlags |= 0x4;

                razSetupSoundRamp(In->CharacterInstance, (SoundRamp *)&Raziel.soundHandle, 19, 600, 750, 60, 120, 204800, 3500);

                Raziel.soundTimerNext = 204800;
                Raziel.soundTimerData = 1;
            }

            In->SectionList[CurrentSection].Data1 = 0;
            break;
        case 0x80005:
            if (CurrentSection == 0)
            {
                razPrepGlyph();

                razSwitchStringAnimation(In->CharacterInstance, 3);
            }

            In->SectionList[CurrentSection].Data1 = 0;
            break;
        case 0x80002:
            if (CurrentSection == 0)
            {
                razPrepGlyph();

                razSwitchStringAnimation(In->CharacterInstance, 2);
            }

            In->SectionList[CurrentSection].Data1 = 0;
            break;
        case 0x80001:
            if (CurrentSection == 0)
            {
                razPrepGlyph();

                razSwitchStringAnimation(In->CharacterInstance, 1);

                Raziel.effectsFlags |= 0x4;

                razSetupSoundRamp(In->CharacterInstance, (SoundRamp *)&Raziel.soundHandle, 12, 0, 125, 80, 80, 315392, 3500);

                Raziel.soundTimerNext = 315392;
                Raziel.soundTimerData = 3;
            }

            In->SectionList[CurrentSection].Data1 = 0;
            break;
        case 0x80007:
            if (In->SectionList[CurrentSection].Data1 != 0)
            {
                if (CurrentSection == 0)
                {
                    razPrepGlyph();

                    razSwitchStringAnimation(In->CharacterInstance, 0);
                }

                In->SectionList[CurrentSection].Data1 = 0;

                if (CurrentSection == 0)
                {
                    razPlaneShift(In->CharacterInstance);

                    if ((Raziel.Mode & 0x40000))
                    {
                        CAMERA_ChangeToOutOfWater(&theCamera, In->CharacterInstance);

                        SteerSwitchMode(In->CharacterInstance, 6);
                    }

                    SteerSwitchMode(In->CharacterInstance, 0);

                    if ((Raziel.playerEvent & 0x2000))
                    {
                        razSetPlayerEventHistory(0x2000);

                        HINT_KillSpecificHint(40);
                    }
                }

                Raziel.invincibleTimer = 122880;
            }

            break;
        case 0x2000000:
        case 0x80000000:
            if ((In->SectionList[CurrentSection].Data1 != 0) && (CurrentSection == 0))
            {
                INSTANCE_Post(Raziel.GlyphSystem, 0x80000000, 0);

                PurgeMessageQueue(&In->SectionList[CurrentSection].Event);
            }

            break;
        case 0x10000000:
            if (CurrentSection == 0)
            {
                if ((*PadData & 0x4))
                {
                    INSTANCE_Post(Raziel.GlyphSystem, 0x10000004, Ptr->Data);
                }

                if ((*PadData & 0x8))
                {
                    INSTANCE_Post(Raziel.GlyphSystem, 0x10000002, Ptr->Data);
                }
            }

            break;
        case 0x4000001:
            if (!(Raziel.Mode & 0x40000))
            {
                PhysicsMode = 0;

                SetDropPhysics(In->CharacterInstance, &Raziel);
            }

            break;
        case 0x40003:
            ptr = (evActionPlayHostAnimationData *)Ptr->Data;

            EnMessageQueueData(&In->SectionList[CurrentSection].Defer, Ptr->ID, SetActionPlayHostAnimationData(ptr->instance, ptr->host, ptr->newAnim, ptr->newFrame, ptr->frames, ptr->mode));
            break;
        case 0x1000000:
            data = (evMonsterHitData *)Ptr->Data;

            if ((hitPosted == 0) && (Raziel.invincibleTimer == 0))
            {
                EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x1000000, SetMonsterHitData(data->sender, data->lastHit, data->power, data->knockBackDistance, data->knockBackDuration));

                hitPosted = 1;
            }

            break;
        case 0x40025:
        case 0x40005:
            EnMessageQueueData(&In->SectionList[CurrentSection].Defer, Ptr->ID, Ptr->Data);
            break;
        case 0x1000001:
        case 0x4010200:
        case 0x4020000:
        case 0x80000008:
        case 0x80000020:
            break;
        default:
            DefaultStateHandler(In, CurrentSection, Data);
        }

        DeMessageQueue(&In->SectionList[CurrentSection].Event);
    }

    In->CharacterInstance->cachedTFace = -1;
}

void DefaultStateHandler(CharacterState *In, int CurrentSection, intptr_t Data)
{
    Message *Ptr;
    typedef void fn(); // not from decls.h

    (void)Data;

    if ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
    {
        switch (Ptr->ID)
        {
        case 0x8000000:
            StateSwitchStateData(In, CurrentSection, StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
            break;
        case 0x80000000:
            if ((!(Raziel.Mode & 0x40000)) && (!(Raziel.Senses.Flags & 0x80)))
            {
                StateSwitchStateDataDefault(In, CurrentSection, StateHandlerAttack2, 0);
            }

            break;
        case 0x80000020:
        {
            int message;
            int messageData;

            if ((CurrentSection == 0) && (!(Raziel.Senses.Flags & 0x80)) && (!(PadData[1] & RazielCommands[2])) && (StateHandlerDecodeHold(&message, &messageData) != 0))
            {
                if (message == 0x800010)
                {
                    StateSwitchStateCharacterDataDefault(In, StateHandlerThrow2, 0);
                }
                else if (message == 0x1000002)
                {
                    StateSwitchStateCharacterDataDefault(In, StateHandlerGrab, messageData);
                }
                else if (message == 0x80000)
                {
                    Raziel.playerEvent |= 0x400;

                    razSetPlayerEventHistory(0x400);

                    razLaunchForce(In->CharacterInstance);

                    StateSwitchStateCharacterDataDefault(In, StateHandlerThrow2, 0);
                }
                else
                {
                    int i;

                    StateSwitchStateCharacterDataDefault(In, StateHandlerAttack2, 0);

                    for (i = 0; i < 3; i++)
                    {
                        EnMessageQueueData((MessageQueue *)&In->SectionList[i].Event.Queue[16], 0x80000020, 0);
                    }
                }
            }

            break;
        }
        case 0x80000004:
            Raziel.Mode |= 0x2;
            break;
        case 0x20000004:
            Raziel.Mode = (Raziel.Mode | 0x1) & ~0x2;
            break;
        case 0x80000008:
            if (CurrentSection == 0)
            {
                StateSwitchStateCharacterDataDefault(In, StateHandlerCrouch, 1);
            }

            break;
        case 0x4000001:
            PhysicsMode = 0;

            SetDropPhysics(In->CharacterInstance, &Raziel);

            if ((In->CharacterInstance->zVel < Raziel.fallZVelocity) && (CurrentSection == 0))
            {
                if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 24, NULL, NULL) != 0)
                {
                    G2EmulationSwitchAnimationCharacter(In, 36, 0, 4, 1);
                }

                StateSwitchStateCharacterDataDefault(In, StateHandlerFall, 0);
            }

            break;
        case 0x4010008:
            if (PhysicsMode != 3)
            {
                PhysicsMode = 3;

                SetPhysics(In->CharacterInstance, -16, 0, 0, 0);
            }

            break;
        case 0x4020000:
            razEnterWater(In, CurrentSection, (evPhysicsSwimData *)Ptr->Data);
            break;
        case 0x1000001:
            StateSwitchStateDataDefault(In, CurrentSection, StateHandlerAutoFace, 0);
            break;
        case 0x1000000:
            if ((CurrentSection == 0) && (!(ControlFlag & 0x4000)) && (Raziel.invincibleTimer == 0))
            {
                StateSwitchStateCharacterDataDefault(In, StateHandlerHitReaction, Ptr->Data);

                CAMERA_ForceEndLookaroundMode(&theCamera);
            }

            break;
        case 0x4010200:
        {
            evPhysicsGravityData *ptr;
            short zRot;

            ptr = (evPhysicsGravityData *)Ptr->Data;

            if (CurrentSection == 0)
            {
                Position pos1;
                Position pos2;
                short diff; // changed decls.h type (int to short)

                pos1.x = 0;
                pos1.y = 0;
                pos2.z = 0; // bug in the original code?

                pos2.x = ptr->x;
                pos2.y = ptr->y;
                pos2.z = ptr->z;

                diff = MATH3D_AngleFromPosToPos(&pos1, &pos2);

                if (G2Anim_IsControllerActive(&In->CharacterInstance->anim, 1, 14) != G2FALSE)
                {
                    G2Anim_DisableController(&In->CharacterInstance->anim, 1, 14);

                    if (ExtraRot != NULL)
                    {
                        In->CharacterInstance->rotation.z += ExtraRot->z;
                    }

                    ExtraRot = NULL;
                }

                zRot = In->CharacterInstance->rotation.z;

                if (((zRot - diff) > -1024) && ((zRot - diff) < 1024))
                {
                    In->CharacterInstance->rotation.z = diff;

                    G2EmulationSwitchAnimationCharacter(&Raziel.State, 73, 0, 6, 2);

                    StateSwitchStateCharacterDataDefault(&Raziel.State, StateHandlerSlide, 0);
                }
                else
                {
                    In->CharacterInstance->rotation.z = diff + 2048;

                    G2EmulationSwitchAnimationCharacter(&Raziel.State, 77, 0, 6, 2);

                    StateSwitchStateCharacterDataDefault(&Raziel.State, StateHandlerSlide, 0);
                }
            }

            break;
        }
        case 0x4010400:
            if (CurrentSection == 0)
            {
                if ((LastBlock + 2) < LoopCounter)
                {
                    BlockCount = 0;
                }

                LastBlock = LoopCounter;

                BlockCount++;
            }

            if (BlockCount >= 16)
            {
                StateSwitchStateDataDefault(In, CurrentSection, StateHandlerBlock, 0);
            }

            break;
        case 0x400000:
            if (CurrentSection == 0)
            {
                evFXHitData *BloodData;
                SVector Accel = {0};

                BloodData = (evFXHitData *)Ptr->Data;

                FX_Blood2(&BloodData->location, &BloodData->velocity, &Accel, 64, 0xFF8010, 0xFF8010);
            }

            break;
        case 0x4000011:
            if ((((Raziel.Abilities & 0x2)) && (Raziel.Senses.heldClass != 0x3) && (Raziel.CurrentPlane == 1)) && (CurrentSection == 0) && ((razSideMoveSpiderCheck(In->CharacterInstance, -128) == 0) && (razSideMoveSpiderCheck(In->CharacterInstance, 128) == 0)))
            {
                StateSwitchStateCharacterDataDefault(In, StateHandlerWallGrab, 0);
            }

            break;
        case 0x40000:
            if (CurrentSection == 2)
            {
                G2EmulationSwitchAnimation(In, 2, 0, 0, 3, CurrentSection);
            }
            else
            {
                G2EmulationSwitchAnimation(In, CurrentSection, 23, 0, 3, 1);
            }

            break;
        case 0x100005:
            if (Ptr->Data == 1)
            {
                if (In->CharacterInstance->LinkChild != NULL)
                {
                    G2EmulationSwitchAnimation(In, Ptr->Data, 50, 0, 3, 2);

                    StateSwitchStateDataDefault(In, Ptr->Data, In->SectionList[CurrentSection].Process, 0);
                }
                else
                {
                    G2EmulationSwitchAnimationSync(In, Ptr->Data, 0, 3);
                }
            }
            else
            {
                G2EmulationSwitchAnimationSync(In, Ptr->Data, 0, 3);
            }

            break;
        case 0x100000:
            if (Ptr->Data != 0)
            {
                StateSwitchStateDataDefault(In, CurrentSection, (fn *)Ptr->Data, 0);
            }

            break;
        case 0x100006:
            InitAlgorithmicWings(In->CharacterInstance);
            break;
        case 0x4010010:
        {
            evPhysicsEdgeData *data;

            data = (evPhysicsEdgeData *)Ptr->Data;

            StateSwitchStateDataDefault(In, CurrentSection, StateHandlerHang, SetControlInitHangData(data->instance, 0, 3));

            In->CharacterInstance->rotation.z = data->zRot;
            break;
        }
        case 0x100009:
            if (Ptr->Data != 0)
            {
                if (CurrentSection == 0)
                {
                    Raziel.returnState = In->SectionList[CurrentSection].Process;

                    if (Raziel.returnState == StateHandlerSoulSuck)
                    {
                        Raziel.returnState = StateHandlerIdle;
                    }

                    In->SectionList[CurrentSection].Data1 = Raziel.Mode;

                    Raziel.Mode = 0x80000;
                }

                StateSwitchStateDataDefault(In, CurrentSection, StateHandlerLookAround, 0);
                break;
            }

            if (Raziel.returnState == NULL)
            {
                Raziel.returnState = StateHandlerIdle;
            }

            StateSwitchStateDataDefault(In, CurrentSection, Raziel.returnState, 0);
            break;
        case 0x40005:
        case 0x40025:
            StateSwitchStateDataDefault(In, CurrentSection, StateHandlerStumble, Ptr->Data);

            In->SectionList[CurrentSection].Data1 = Ptr->Data * 30;
            break;
        case 0x10002001:
        case 0x10002002:
            StateSwitchStateDataDefault(In, CurrentSection, StateHandlerGlyphs, 1);
            break;
        case 0x4000004:
            break;
        }
    }
}

long RazielAnimCallback(G2Anim *anim, int sectionID, G2AnimCallbackMsg message, long messageDataA, long messageDataB, void *data)
{
    State *pSection;
    G2AnimSection *animSection;

    pSection = &Raziel.State.SectionList[sectionID];

    animSection = &anim->section[sectionID];

    switch (message)
    {
    case 1:
        EnMessageQueueData(&pSection->Event, 0x8000000, animSection->keylistID);
        break;
    case 2:
        EnMessageQueueData(&pSection->Event, 0x8000001, animSection->keylistID);
        break;
    case 4:
    {
        evAnimationControllerDoneData *ControllerData;

        ControllerData = (evAnimationControllerDoneData *)SetAnimationControllerDoneData(Raziel.State.CharacterInstance, messageDataB, messageDataA, (intptr_t)data);

        if (ControllerData->data == 2)
        {
            AlgorithmicWings(Raziel.State.CharacterInstance, ControllerData);
        }
        else if (ControllerData->data == 0)
        {
            G2Anim_InterpDisableController(&ControllerData->instance->anim, ControllerData->segment, ControllerData->type, 300);
        }
        else if (ControllerData->data == 4)
        {
            G2Anim_DisableController(&ControllerData->instance->anim, ControllerData->segment, ControllerData->type);
        }

        break;
    }
    case 3:
        EnMessageQueueData(&pSection->Event, 0x8000003, animSection->keylistID);
        break;
    case 5:
        animSection->swAlarmTable = NULL;

        EnMessageQueueData(&pSection->Event, 0x8000004, 0);
        break;
    case 6:
        if (messageDataA == 2)
        {
            AlarmData *data;
            Instance *inst;

            data = (AlarmData *)messageDataB;

            inst = razGetHeldWeapon();

            switch (data->command)
            {
            case 1:
                if (inst != NULL)
                {
                    INSTANCE_Post(inst, 0x200002, data->data);
                }
                else
                {
                    EnableWristCollision(gameTrackerX.playerInstance, data->data);
                }

                break;
            case 2:
                if (inst != NULL)
                {
                    INSTANCE_Post(inst, 0x200003, data->data);
                }
                else
                {
                    DisableWristCollision(gameTrackerX.playerInstance, data->data);
                }

                ControlFlag |= 0x10000000;
                break;
            case 3:
                razSetCowlNoDraw(0);

                ControlFlag |= 0x40;
                break;
            case 4:
                razSetCowlNoDraw(1);

                ControlFlag &= ~0x40;
                break;
            case 5:
                if (data->data >= 32)
                {
                    Raziel.passedMask = ~0x0;
                }
                else
                {
                    Raziel.passedMask |= 1 << data->data;
                }

                break;
            case 6:
                if (data->data >= 32)
                {
                    Raziel.passedMask = 0;
                }
                else
                {
                    Raziel.passedMask &= ~(1 << data->data);
                }

                break;
            case 7:
                Raziel.effectsFadeSource = gameTrackerX.playerInstance->fadeValue;
                Raziel.effectsFadeDest = data->data;

                Raziel.effectsFlags |= 0x1;
                break;
            case 8:
                Raziel.effectsFadeSteps = 0;

                Raziel.effectsFlags |= 0x1;

                Raziel.effectsFadeStep = data->data;
                break;
            case 9:
                if (inst != NULL)
                {
                    INSTANCE_Post(inst, 0x200005, data->data);
                }

                break;
            case 10:
                if (inst != NULL)
                {
                    INSTANCE_Post(inst, 0x200006, data->data);
                }

                break;
            case 11:
            {
                int test;

                test = 0;

                switch (data->data)
                {
                case 0:
                case 1:
                    test = 1;
                    break;
                case 2:
                    if (Raziel.currentSoulReaver == 1)
                    {
                        test = 1;
                    }

                    break;
                case 3:
                    if (Raziel.currentSoulReaver == 6)
                    {
                        test = 1;
                    }

                    break;
                }

                if (test != 0)
                {
                    SoundRamp *soundRamp;

                    soundRamp = (SoundRamp *)&Raziel.soundHandle;

                    razSetupSoundRamp(gameTrackerX.playerInstance, &soundRamp[cannedSound[data->data].bank], cannedSound[data->data].sound, cannedSound[data->data].startPitch, cannedSound[data->data].endPitch, cannedSound[data->data].startVolume, cannedSound[data->data].endVolume, cannedSound[data->data].time, cannedSound[data->data].distance);

                    if (cannedSound[data->data].bank == 0)
                    {
                        Raziel.effectsFlags |= 0x4;
                    }

                    if (cannedSound[data->data].bank == 1)
                    {
                        Raziel.effectsFlags |= 0x8;
                    }
                }
            }

            break;
            }
        }
        else
        {
            if (messageDataA == 0)
            {
                if (((AlarmData *)messageDataB)->command == 45)
                {
                    if (!(ControlFlag & 0x100000))
                    {
                        Instance *heldInstance;

                        heldInstance = razGetHeldWeapon();

                        if ((heldInstance != NULL) && (((INSTANCE_Query(heldInstance, 2) & 0x20)) && ((INSTANCE_Query(heldInstance, 3) & 0x10000))))
                        {
                            INSTANCE_DefaultAnimCallback(anim, sectionID, message, 0, messageDataB, Raziel.State.CharacterInstance);
                        }
                    }
                }
                else
                {
                    INSTANCE_DefaultAnimCallback(anim, sectionID, message, 0, messageDataB, Raziel.State.CharacterInstance);
                }
            }
            else
            {
                INSTANCE_DefaultAnimCallback(anim, sectionID, message, messageDataA, messageDataB, Raziel.State.CharacterInstance);
            }
        }

        break;
    }

    return messageDataA;
}

long RazielAnimCallbackDuringPause(G2Anim *anim, int sectionID, G2AnimCallbackMsg message, long messageDataA, long messageDataB, void *data)
{
    evAnimationControllerDoneData *ControllerData;

    (void)anim;
    (void)sectionID;

    if (message == G2ANIM_MSG_SEGCTRLR_INTERPDONE)
    {
        ControllerData = (evAnimationControllerDoneData *)SetAnimationControllerDoneData(Raziel.State.CharacterInstance, messageDataB, messageDataA, (intptr_t)data);

        if (ControllerData->data == 2)
        {
            AlgorithmicWings(Raziel.State.CharacterInstance, ControllerData);
        }
        else if (ControllerData->data == 0)
        {
            G2Anim_InterpDisableController(&ControllerData->instance->anim, ControllerData->segment, ControllerData->type, 300);
        }
        else if (ControllerData->data == 4)
        {
            G2Anim_DisableController(&ControllerData->instance->anim, ControllerData->segment, ControllerData->type);
        }
    }

    return messageDataA;
}

unsigned long RazielQuery(Instance *instance, unsigned long Query)
{
    Normal *Ptr;
    unsigned long ability;
    PlayerSaveData data;
    evShadowSegmentData *shadowData;
    int temp; // not from decls.h

    switch (Query)
    {
    case 1:
        return 1;
    case 9:
        return WaterStatus;
    case 6:
        return SetPositionData(instance->position.x, instance->position.y, instance->position.z);
    case 8:
        if (ExtraRot != NULL)
        {
            return (unsigned long)ExtraRot;
        }
        else
        {
            return 0;
        }
    case 7:
        if (Raziel.steeringMode == 0x4)
        {
            return SetPositionData(instance->rotation.x, instance->rotation.y, Raziel.steeringLockRotation);
        }
        else
        {
            return SetPositionData(instance->rotation.x, instance->rotation.y, instance->rotation.z);
        }
    case 10:
        return Raziel.Mode;
    case 11:
        return Raziel.CurrentPlane;
    case 13:
        if (instance->oldMatrix != NULL)
        {
            return (unsigned long)instance->oldMatrix;
        }
        else
        {
            return 0;
        }
    case 12:
        if (instance->oldMatrix != NULL)
        {
            return (unsigned long)&instance->oldMatrix[15];
        }
        else
        {
            return 0;
        }
    case 16:
        Ptr = (Normal *)CIRC_Alloc(sizeof(Normal));

        if (Raziel.Mode == 0x40000)
        {
            Ptr->x = instance->matrix[1].m[0][2];
            Ptr->y = instance->matrix[1].m[1][2];
            Ptr->z = instance->matrix[1].m[2][2];
        }
        else
        {
            Ptr->x = -instance->matrix[0].m[0][1];
            Ptr->y = -instance->matrix[0].m[1][1];
            Ptr->z = -instance->matrix[0].m[2][1];
        }

        return (unsigned long)Ptr;
    case 19:
        temp = STREAM_GetLevelWithID(instance->currentStreamUnitID)->unitFlags;

        ability = Raziel.Abilities & 0x1FC0000;

        if (((temp & 0x800)) || (RAZIEL_OkToShift() == 0))
        {
            ability &= ~0x1000000;
        }

        if (((Raziel.Mode & 0x40000)) || (Raziel.CurrentPlane == 2))
        {
            ability &= 0x1000000;
        }

        return ability;
    case 24:
        temp = SetControlSaveDataData(sizeof(PlayerSaveData), &data);

        data.abilities = Raziel.Abilities;

        data.currentPlane = Raziel.CurrentPlane;

        data.healthScale = Raziel.HealthScale;
        data.healthBalls = Raziel.HealthBalls;

        data.manaBalls = Raziel.GlyphManaBalls;
        data.manaMax = Raziel.GlyphManaMax;

        data.playerEventHistory = Raziel.playerEventHistory;

        return temp;
    case 31:
        return Raziel.HealthBalls;
    case 32:
        return Raziel.GlyphManaBalls;
    case 45:
        return Raziel.GlyphManaMax;
    case 34:
        if ((Raziel.Senses.EngagedMask & 0x40))
        {
            return (unsigned long)Raziel.Senses.EngagedList[6].instance;
        }
        else
        {
            return 0;
        }
    case 36:
        return Raziel.Abilities;
    case 38:
        shadowData = (evShadowSegmentData *)SetShadowSegmentData(2);

        shadowData->shadowSegments[0] = 12;
        shadowData->shadowSegments[1] = 8;

        return (unsigned long)shadowData;
    case 39:
        return Raziel.Senses.EngagedMask;
    case 41:
        return Raziel.playerEvent;
    case 42:
        return Raziel.playerEventHistory;
    case 43:
        return (Raziel.HitPoints ^ GetMaxHealth()) == 0;
    case 44:
        return (unsigned long)razGetHeldItem();
    case 46:
        return Raziel.invincibleTimer;
    }

    return 0;
}

void RazielPost(Instance *instance, unsigned long Message, uintptr_t Data)
{
    int i;
    typedef long fn(G2Anim *, int, G2AnimCallbackMsg, long, long, Instance *);

    switch (Message)
    {
    case 0x200001:
        if (!((ControlFlag & 0x40000)) && (HealthCheckForLowHealth() == 0))
        {
            UpdateEngagementList((evCollideInstanceStatsData *)Data, &Raziel);
        }

        break;
    case 0x200000:
        if (!(ControlFlag & 0x40000))
        {
            Raziel.Senses.EngagedMask = 0;
        }

        Raziel.Senses.Flags &= ~0x20;
        Raziel.Senses.Flags &= ~0x40;
        break;
    case 0x200004:
        if ((ControlFlag & 0x40000000))
        {
            instance->collideInfo = (CollideInfo *)Data;

            ((CollideInfo *)Data)->offset.z = 0;

            RazielCollide(instance, &gameTrackerX);

            COLLIDE_UpdateAllTransforms(razGetHeldItem(), (SVECTOR *)&((CollideInfo *)instance->collideInfo)->offset);
        }

        break;
    case 0x100007:
    {
        PlayerSaveData *data;

        data = (PlayerSaveData *)((evControlSaveDataData *)Data)->data;

        Raziel.Abilities = data->abilities;

        debugRazielFlags1 = Raziel.Abilities;

        if ((razInBaseArea("under", 5)) != 0)
        {
            Raziel.CurrentPlane = 2;
        }
        else
        {
            Raziel.CurrentPlane = data->currentPlane;
        }

        Raziel.HealthScale = (short)data->healthScale;
        Raziel.HealthBalls = (short)data->healthBalls;

        HUD_Setup_Chit_Count(data->healthBalls);

        Raziel.GlyphManaBalls = data->manaBalls;
        Raziel.GlyphManaMax = data->manaMax;

        Raziel.soulReaver = NULL;

        if ((Raziel.Abilities & 0x8))
        {
            debugRazielFlags2 = 0;
        }

        Raziel.playerEventHistory = data->playerEventHistory;

        if (!(Raziel.playerEventHistory & 0x1000))
        {
            Raziel.HitPoints = 100;
        }
        else
        {
            Raziel.HitPoints = GetMaxHealth();
        }

        break;
    }
    case 0x4000005:
        Raziel.slipSlope = Data;
        break;
    case 0x4000006:
        Raziel.slipSlope = 2896;
        break;
    case 0x4000001:
        if (((ControlFlag & 8)) && (!(Raziel.Senses.Flags & 2)))
        {
            for (i = 0; i < 3; i++)
            {
                EnMessageQueueData(&Raziel.State.SectionList[i].Defer, Message, Data);
            }
        }

        break;
    case 0x40001:
        instance->currentStreamUnitID = Data;
        instance->tface = NULL;

        Raziel.GlyphSystem->currentStreamUnitID = Data;
        Raziel.GlyphSystem->tface = NULL;

        if (Raziel.soulReaver != NULL)
        {
            Raziel.soulReaver->currentStreamUnitID = Data;
            Raziel.soulReaver->tface = NULL;
        }

        break;
    case 0x800024:
        Raziel.idleInstance = ((evObjectIdleData *)Data)->instance;
        break;
    case 0x40006:
        DrainHealth(Data);
        break;
    case 0x40008:
        DrainMana(Data);
        break;
    case 0x40019:
        SetMana(Data);
        break;
    case 0x40004:
        G2EmulationSwitchAnimationCharacter(&Raziel.State, 128, 0, 3, 1);

        StateSwitchStateCharacterData(&Raziel.State, &StateHandlerCannedReaction, 0);
        break;
    case 0x100008:
        if ((ControlFlag & 0x200000))
        {
            RelocateConstrict((SVector *)Data);
        }

        Raziel.puppetMoveToPoint.x += ((SVector *)Data)->x;
        Raziel.puppetMoveToPoint.y += ((SVector *)Data)->y;
        Raziel.puppetMoveToPoint.z += ((SVector *)Data)->z;

        STREAM_MORPH_Relocate();
        break;
    case 0x10000A:
        if (Data != 0)
        {
            G2AnimSection *animSection;

            for (i = 0; i < 3; i++)
            {
                animSection = &instance->anim.section[i];

                animSection->callback = (fn *)&RazielAnimCallbackDuringPause;
                animSection->callbackData = NULL;
            }

            DeInitAlgorithmicWings(instance);

            razResetPauseTranslation(instance);
        }
        else
        {
            G2AnimSection *animSection;

            for (i = 0; i < 3; i++)
            {
                animSection = &instance->anim.section[i];

                animSection->callback = (fn *)&RazielAnimCallback;
                animSection->callbackData = NULL;
            }

            InitAlgorithmicWings(instance);
        }

        break;
    case 0x100010:
        if (Data != 0)
        {
            if (!(Raziel.Mode & 0x40000000))
            {
                Raziel.Mode = 0x40000000;

                ResetPhysics(instance, -16);

                for (i = 0; i < 3; i++)
                {
                    StateSwitchStateData(&Raziel.State, i, &StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
                }
            }
        }
        else if ((Raziel.Mode & 0x40000000))
        {
            Raziel.Mode &= ~0x40000000;
        }

        break;
    case 0x40011:
        HealthInstantDeath(instance);
        break;
    case 0x40012:
        StateSwitchStateCharacterData(&Raziel.State, &StateHandlerDropAction, 0);
        break;
    case 0x10002000:
        razPlaneShift(instance);
        break;
    case 0x40015:
        Raziel.Abilities = Raziel.Abilities | Data;

        debugRazielFlags1 = Raziel.Abilities;

        RAZIEL_DebugHealthFillUp();

        if ((Data & 0x3FC00) && (Raziel.soulReaver != NULL))
        {
            razReaverOn();

            razReaverImbue(razGetReaverFromMask(Data));
        }

        break;
    case 0x100011:
        EnMessageQueueData(&Raziel.State.SectionList[0].Defer, 0x100011, Data);
        break;
    case 0x100012:
        Raziel.Senses.heldClass = INSTANCE_Query((Instance *)Data, 4);

        if (Raziel.Senses.heldClass != 0x8)
        {
            razReaverBladeOff();
        }

        Raziel.Mode &= ~0x800;

        Raziel.Senses.Flags &= ~0x80;
        break;
    case 0x100013:
        if (Data != (uintptr_t)Raziel.soulReaver)
        {
            if (razReaverOn() == 0)
            {
                Raziel.Senses.heldClass = 0;
            }

            razReaverBladeOn();
        }

        Raziel.Senses.Flags &= ~0x80;
        break;
    case 0x40022:
        Raziel.forcedGlideSpeed = Data;
        break;
    case 0x40024:
        if (SndTypeIsPlayingOrRequested(1) == 0)
        {
            SOUND_Play3dSound(&gameTrackerX.playerInstance->position, 1, 0, 75, 3500);
        }

        break;
    case 0x4000E:
        if (Data != 0)
        {
            int i;
            Instance *heldWeapon;

            Raziel.returnState = StateHandlerPuppetShow;

            StateSwitchStateCharacterDataDefault(&Raziel.State, &StateHandlerPuppetShow, 0);

            InitAlgorithmicWings(instance);

            for (i = 0; i < 3; i++)
            {
                PurgeMessageQueue(&Raziel.State.SectionList[i].Event);

                PurgeMessageQueue(&Raziel.State.SectionList[i].Defer);
            }

            GAMELOOP_Reset24FPS();

            heldWeapon = razGetHeldWeapon();

            if ((heldWeapon != NULL) && (heldWeapon != Raziel.soulReaver))
            {
                razSetFadeEffect(0, 4096, 10);
            }
        }
        else
        {
            Instance *heldWeapon;

            Raziel.Senses.Flags &= ~0x10;

            ControlFlag &= ~0x20000;

            StateSwitchStateCharacterDataDefault(&Raziel.State, &StateHandlerIdle, SetControlInitIdleData(0, 0, 3));

            for (i = 0; i < 3; i++)
            {
                PurgeMessageQueue(&Raziel.State.SectionList[i].Event);

                PurgeMessageQueue(&Raziel.State.SectionList[i].Defer);
            }

            GAMELOOP_Set24FPS();

            heldWeapon = razGetHeldWeapon();

            if ((heldWeapon != NULL) && (heldWeapon != Raziel.soulReaver))
            {
                razSetFadeEffect(4096, 0, 10);
            }
        }

        break;
    case 0x100016:
        razSetupSoundRamp(instance, (SoundRamp *)&Raziel.soundHandle, 51, -200, -200, 120, 120, Data << 12, 3500);

        Raziel.soundTimerNext = Data << 12;
        Raziel.soundTimerData = 0;
        break;
    default:
        for (i = 0; i < 3; i++)
        {
            EnMessageQueueData(&Raziel.State.SectionList[i].Defer, Message, Data);
        }
    }
}

// Matches 100% on decomp.me but differs on this project
#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/Game/RAZIEL/RAZIEL", SetStates);
#else
int SetStates(Instance *instance, GameTracker *GT, long *controlCommand, int AnalogLength)
{
    int i;
    int Event;
    int Data1;
    STATIC unsigned long LastTime;
    Message *Ptr;

    (void)AnalogLength;

    Data1 = 0;

    LastTime = (GetRCnt(0xF2000000) & 0xFFFF) | (gameTimer << 16);

    PadData = controlCommand;

    gameTracker = GT;

    LoopCounter++;

    for (i = 0; i < 3; i++)
    {
        while ((Ptr = DeMessageQueue(&Raziel.State.SectionList[i].Defer)) != NULL)
        {
            EnMessageQueue(&Raziel.State.SectionList[i].Event, Ptr);
        }
    }

    if ((instance->offset.x != 0) || (instance->offset.y != 0) || (instance->offset.z != 0))
    {
        INSTANCE_Post(instance, 0x4000004, LoopCounter);

        SetImpulsePhysics(instance, &Raziel);
    }

    if (Raziel.Senses.HitMonster != NULL)
    {
        INSTANCE_Post(instance, 0x2000002, SetMonsterHitData(Raziel.Senses.HitMonster, NULL, 4096, Raziel.attack->knockBackDistance, Raziel.attack->knockBackFrames));

        DisableWristCollision(instance, 1);
        DisableWristCollision(instance, 2);
    }

    if (!(gameTrackerX.playerInstance->flags & 0x100))
    {
        Event = 0;

        if ((ControlFlag & 0x800000))
        {
            PadData = &Raziel.blankPad;

            for (i = 0; i < 3; i++)
            {
                EnMessageQueueData(&Raziel.State.SectionList[i].Event, Event, Data1);
            }
        }
        else
        {
            Message *message;

            GetControllerMessages(controlCommand);

            while ((message = DeMessageQueue(&Raziel.padCommands)) != NULL)
            {
                Event = message->ID;

                if ((unsigned int)Event == 0x80000000)
                {
                    if (!(Raziel.Senses.EngagedMask & 0x681F))
                    {
                        if (((Raziel.Senses.EngagedMask & 0x20)) && (razGetHeldItem() == NULL))
                        {
                            Event = 0x2000000;
                        }
                    }
                    else
                    {
                        Event = 0x2000000;
                    }
                }

                if (Event != 0)
                {
                    for (i = 0; i < 3; i++)
                    {
                        EnMessageQueueData(&Raziel.State.SectionList[i].Event, Event, Data1);
                    }
                }
            }

            Data1 = ProcessMovement(instance, controlCommand, GT);

            if (Data1 != 0)
            {
                if ((Data1 & 0x1000))
                {
                    Event = 0x1000001;
                }
                else
                {
                    Event = 0x10000000;
                }
            }
            else
            {
                Event = 0;
            }

            for (i = 0; i < 3; i++)
            {
                EnMessageQueueData(&Raziel.State.SectionList[i].Event, Event, Data1);
            }
        }
    }

    razSetPlayerEvent();

    G2EmulatePlayAnimation(&Raziel.State);

    for (i = 0; i < 3; i++)
    {
        Raziel.State.SectionList[i].Process(&Raziel.State, i, 1);
    }

    if (gameTracker->cheatMode == 1)
    {
        PhysicsMode = 3;
    }

    ProcessPhysics(&Raziel, &Raziel.State, 0, PhysicsMode);

    if ((*PadData & RazielCommands[7]))
    {
        Raziel.nothingCounter = 0;

        if (((Raziel.Senses.EngagedMask & 0x40)) && (!(Raziel.Mode & 0x2000000)))
        {
            for (i = 0; i < 3; i++)
            {
                EnMessageQueueData(&Raziel.State.SectionList[i].Event, 0x1000001, 0);
            }
        }

        ControlFlag |= 0x4;

        if ((Raziel.Senses.EngagedMask & 0x40))
        {
            Raziel.Senses.CurrentAutoFace = Raziel.Senses.EngagedList[6].instance;
        }
        else
        {
            Raziel.Senses.CurrentAutoFace = NULL;
        }
    }
    else
    {
        Raziel.nothingCounter++;

        if (Raziel.nothingCounter < 6)
        {
            if (((Raziel.Senses.EngagedMask & 0x40)) && (!(Raziel.Mode & 0x2000000)))
            {
                for (i = 0; i < 3; i++)
                {
                    EnMessageQueueData(&Raziel.State.SectionList[i].Event, 0x1000001, 0);
                }
            }
        }
        else if (Raziel.nothingCounter == 6)
        {
            Raziel.Senses.LastAutoFace = NULL;
            Raziel.Senses.CurrentAutoFace = NULL;
        }

        ControlFlag &= ~0x4;
    }

    AutoFaceAngle = -1;

    Raziel.iVelocity.x = instance->position.x - instance->oldPos.x - instance->offset.x;
    Raziel.iVelocity.y = instance->position.y - instance->oldPos.y - instance->offset.y;
    Raziel.iVelocity.z = instance->position.z - instance->oldPos.z - instance->offset.z;

    StateGovernState(&Raziel.State, 3);

    if (((gameTrackerX.debugFlags2 & 0x800)) && (!(ControlFlag & 0x1000000)))
    {
        ProcessHealth(instance);
    }

    if ((ControlFlag & 0x20000))
    {
        if ((Raziel.Senses.EngagedMask & 0x400))
        {
            AlgorithmicNeck(instance, Raziel.Senses.EngagedList[10].instance);
        }
        else if ((Raziel.Senses.Flags & 0x10))
        {
            AlgorithmicNeck(instance, NULL);
        }
    }

    if (((ControlFlag & 0x200000)) && (gameTrackerX.gameFramePassed != 0))
    {
        ProcessConstrict();
    }

    if (Raziel.effectsFlags != 0)
    {
        ProcessEffects(instance);
    }

    ProcessInteractiveMusic(instance);
    ProcessSpecialAbilities(instance);

    if (((Raziel.Senses.Flags & 0x40)) && (RAZIEL_OkToShift() != 0))
    {
        if (INSTANCE_Query(Raziel.Senses.Portal, 17) != 1)
        {
            FX_EndInstanceParticleEffects(Raziel.Senses.Portal);

            INSTANCE_Post(Raziel.Senses.Portal, 0x8000008, SetAnimationInstanceSwitchData(Raziel.Senses.Portal, 1, 0, 0, 2));
        }
    }
    else
    {
        if ((Raziel.Senses.Portal != NULL) && (INSTANCE_Query(Raziel.Senses.Portal, 17) != 0))
        {
            FX_EndInstanceParticleEffects(Raziel.Senses.Portal);

            INSTANCE_Post(Raziel.Senses.Portal, 0x8000008, SetAnimationInstanceSwitchData(Raziel.Senses.Portal, 0, 0, 0, 2));
        }

        Raziel.Senses.Portal = NULL;
    }

    return 1;
}
#endif

void ProcessConstrict()
{
    if ((Raziel.constrictFlag & 0x1))
    {
        Raziel.constrictIndex = 0;

        Raziel.constrictFlag = (Raziel.constrictFlag & ~0x1) | 0x4;

        Raziel.constrictWaitIndex = 0;

        Raziel.constrictXTotal = 0;
        Raziel.constrictYTotal = 0;

        Raziel.constrictGoodCircle = 0;
    }

    Raziel.constrictXTotal += Raziel.State.CharacterInstance->position.x;
    Raziel.constrictYTotal += Raziel.State.CharacterInstance->position.y;

    Raziel.constrictData[Raziel.constrictIndex].x = Raziel.State.CharacterInstance->position.x;
    Raziel.constrictData[Raziel.constrictIndex].y = Raziel.State.CharacterInstance->position.y;

    Raziel.constrictIndex++;

    if (Raziel.constrictIndex >= 32)
    {
        Raziel.constrictIndex = 0;
    }

    if ((Raziel.constrictFlag & 0x4))
    {
        if (Raziel.constrictWaitIndex == Raziel.constrictIndex)
        {
            Raziel.constrictFlag |= 0x22;
        }
        else
        {
            Raziel.constrictFlag &= ~0x20;
        }
    }

    if ((Raziel.constrictFlag & 0x2))
    {
        int i;

        Raziel.constrictCenter.x = Raziel.constrictXTotal / 32;
        Raziel.constrictCenter.y = Raziel.constrictYTotal / 32;
        Raziel.constrictCenter.z = Raziel.State.CharacterInstance->position.z + 256;

        Raziel.constrictXTotal -= Raziel.constrictData[Raziel.constrictIndex].x;
        Raziel.constrictYTotal -= Raziel.constrictData[Raziel.constrictIndex].y;

        Raziel.constrictGoodCircle = 1;

        for (i = 0; i < 32; i++)
        {
            if ((MATH3D_SquareLength(Raziel.constrictData[i].x - Raziel.constrictCenter.x, Raziel.constrictData[i].y - Raziel.constrictCenter.y, 0) - 1) > 819198)
            {
                Raziel.constrictGoodCircle = 0;
            }
        }

        if (Raziel.constrictGoodCircle != 0)
        {
            int thisIndex;
            int nextIndex;

            thisIndex = Raziel.constrictIndex - 1;
            nextIndex = Raziel.constrictIndex;

            if (thisIndex < 0)
            {
                thisIndex = 31;
            }

            if (MATH3D_SquareLength(Raziel.constrictData[thisIndex].x - Raziel.constrictData[nextIndex].x, Raziel.constrictData[thisIndex].y - Raziel.constrictData[nextIndex].y, 0) > 1440000)
            {
                Raziel.constrictGoodCircle = 0;
            }

            if (Raziel.constrictGoodCircle != 0)
            {
                Raziel.constrictXTotal = 0;
                Raziel.constrictYTotal = 0;

                Raziel.constrictFlag = (Raziel.constrictFlag & ~0x2) | 0x1C;

                Raziel.constrictWaitIndex = Raziel.constrictIndex;

                if (Raziel.constrictGoodCircle >= 2)
                {
                    gameTrackerX.streamFlags |= 0x4;

                    FX_EndConstrict(0x1, NULL);

                    Raziel.constrictFlag = 0x1;
                }

                Raziel.constrictGoodCircle++;
                return;
            }
        }

        Raziel.constrictGoodCircle = 1;

        Raziel.constrictFlag &= ~0x10;

        FX_EndConstrict(0, NULL);
    }
}

void RelocateConstrict(SVector *offset)
{
    int i;

    Raziel.constrictXTotal = 0;
    Raziel.constrictYTotal = 0;

    if ((Raziel.constrictFlag & 0x2))
    {
        for (i = 0; i < 32; i++)
        {
            Raziel.constrictData[i].x += offset->x;
            Raziel.constrictData[i].y += offset->y;

            Raziel.constrictXTotal += Raziel.constrictData[i].x;
            Raziel.constrictYTotal += Raziel.constrictData[i].y;
        }
    }
    else
    {
        i = Raziel.constrictWaitIndex + 1;

        if (i == 32)
        {
            i = 0;
        }

        for (; i != Raziel.constrictIndex;)
        {
            Raziel.constrictData[i].x += offset->x;
            Raziel.constrictData[i].y += offset->y;

            Raziel.constrictXTotal += Raziel.constrictData[i].x;
            Raziel.constrictYTotal += Raziel.constrictData[i].y;

            i++;

            if (i == 32)
            {
                i = 0;
            }
        }
    }
}

void ProcessEffects(Instance *instance)
{
    Instance *heldInst;
    int step;

    heldInst = razGetHeldItem();

    if (((Raziel.effectsFlags & 0x4)) && (razUpdateSoundRamp(instance, (SoundRamp *)&Raziel.soundHandle) == 0))
    {
        SndEndLoop(Raziel.soundHandle);

        Raziel.soundHandle = 0;

        Raziel.effectsFlags &= ~0x4;
    }

    if (((Raziel.effectsFlags & 0x8)) && (razUpdateSoundRamp(instance, (SoundRamp *)&Raziel.soundHandle2) == 0))
    {
        SndEndLoop(Raziel.soundHandle2);

        Raziel.soundHandle2 = 0;

        Raziel.effectsFlags &= ~0x8;
    }

    if ((Raziel.effectsFlags & 0x2))
    {
        if (Raziel.throwInstance != NULL)
        {
            instance = Raziel.throwInstance;
        }
        else if (heldInst != NULL)
        {
            if ((Raziel.Senses.heldClass == 0x1) || (Raziel.Senses.heldClass == 0x2) || (Raziel.Senses.heldClass == 0x3))
            {
                instance = heldInst;
            }
            else
            {
                Raziel.effectsFlags &= ~0x2;

                Raziel.throwInstance = NULL;
            }
        }
    }

    if ((Raziel.effectsFlags & 0x1))
    {
        Raziel.effectsFadeSteps += Raziel.effectsFadeStep * gameTrackerX.timeMult;

        step = Raziel.effectsFadeSteps / 4096;

        instance->fadeValue += step;

        if (step > 0)
        {
            if (instance->fadeValue > Raziel.effectsFadeDest)
            {
                instance->fadeValue = Raziel.effectsFadeDest;

                Raziel.effectsFlags &= ~0x1;

                if (instance == Raziel.throwInstance)
                {
                    Raziel.effectsFlags &= ~0x2;
                }
            }
        }
        else if ((step < 0) && (instance->fadeValue < Raziel.effectsFadeDest))
        {
            instance->fadeValue = Raziel.effectsFadeDest;

            Raziel.effectsFlags &= ~0x1;

            if (instance == Raziel.throwInstance)
            {
                Raziel.effectsFlags &= ~0x2;

                Raziel.throwInstance = NULL;
            }
        }
    }
}

void ProcessHints()
{
    long hint;

    hint = HINT_GetCurrentHint();

    if (!(Raziel.playerEventHistory & 0x2000))
    {
        if ((Raziel.playerEvent & 0x2000))
        {
            if (hint == -1)
            {
                HINT_StartHint(12);
            }

            if ((hint == 12) && (Raziel.State.SectionList->Process == StateHandlerGlyphs))
            {
                HINT_KillSpecificHint(hint);

                HINT_StartHint(40);
            }

            if ((hint == 40) && (Raziel.State.SectionList->Process != StateHandlerGlyphs))
            {
                HINT_KillSpecificHint(hint);

                HINT_StartHint(12);
            }
        }
        else if (hint == 12)
        {
            HINT_KillSpecificHint(hint);
        }
    }
    else if (hint == 12)
    {
        HINT_KillSpecificHint(hint);
    }
    else if (hint == 40)
    {
        HINT_KillSpecificHint(hint);
    }

    if (!(Raziel.playerEventHistory & 0x10000))
    {
        if ((Raziel.playerEvent & 0x10000))
        {
            if (hint == -1)
            {
                HINT_StartHint(32);
            }
        }
        else if (hint == 32)
        {
            HINT_KillSpecificHint(hint);
        }
    }
}

void ProcessInteractiveMusic(Instance *instance)
{
    Level *level;
    int temp; // not from decls.h

    level = STREAM_GetLevelWithID(instance->currentStreamUnitID);

    RAZIEL_SetInteractiveMusic(6, level->unitFlags & 0x2);
    RAZIEL_SetInteractiveMusic(9, level->unitFlags & 0x200);
    RAZIEL_SetInteractiveMusic(10, level->unitFlags & 0x100);
    RAZIEL_SetInteractiveMusic(7, level->unitFlags & 0x40);
    RAZIEL_SetInteractiveMusic(8, level->unitFlags & 0x80);
    RAZIEL_SetInteractiveMusic(11, level->unitFlags & 0x400);
    RAZIEL_SetInteractiveMusic(14, level->unitFlags & 0x4000);

    if ((level->unitFlags & 0x2))
    {
        if (!(level->unitFlags & 0xC0))
        {
            temp = GAMELOOP_GetTimeOfDay();

            RAZIEL_SetInteractiveMusic(7, temp != 1900);
            RAZIEL_SetInteractiveMusic(8, temp == 1900);
        }
    }

    RAZIEL_SetInteractiveMusic(12, Raziel.CurrentPlane == 2);
    RAZIEL_SetInteractiveMusic(5, Raziel.Mode & 0x40000);

    if (((level->unitFlags & 0x10)) || ((Raziel.Mode & 0x2000000)))
    {
        Raziel.soundModifier &= ~0x1E;

        RAZIEL_SetInteractiveMusic(3, 1);
        return;
    }

    if (((level->unitFlags & 0x8)) || ((Raziel.Senses.Flags & 0x20)))
    {
        Raziel.soundModifier &= ~0x1E;

        RAZIEL_SetInteractiveMusic(2, 1);
        return;
    }

    if ((level->unitFlags & 0x4))
    {
        Raziel.soundModifier &= ~0x1E;

        RAZIEL_SetInteractiveMusic(1, 1);
        return;
    }

    if ((level->unitFlags & 0x20))
    {
        Raziel.soundModifier &= ~0x1E;

        RAZIEL_SetInteractiveMusic(4, 1);
        return;
    }

    Raziel.soundModifier &= ~0x1E;

    SOUND_SetMusicModifier(0);
}

void ProcessTimers(Instance *instance)
{
    if (Raziel.timeAccumulator > 0)
    {
        Raziel.timeAccumulator -= gameTrackerX.timeMult;

        if (Raziel.timeAccumulator <= 0)
        {
            INSTANCE_Post(instance, 0x100015, -Raziel.timeAccumulator);

            Raziel.timeAccumulator = 0;
        }
    }

    if (Raziel.soundTimerNext > 0)
    {
        Raziel.soundTimerNext -= gameTrackerX.timeMult;

        if (Raziel.soundTimerNext <= 0)
        {
            Raziel.soundTimerNext = 0;

            switch (Raziel.soundTimerData)
            {
            case 1:
                razSetupSoundRamp(instance, (SoundRamp *)&Raziel.soundHandle, 19, 1000, 1000, 120, 120, 4096, 3500);
                razSetupSoundRamp(instance, (SoundRamp *)&Raziel.soundHandle2, 20, 1030, 1030, 120, 120, 4096, 3500);

                Raziel.soundTimerNext = 122880;
                Raziel.soundTimerData = 2;

                Raziel.effectsFlags |= 0x8;
                break;
            case 2:
                razSetupSoundRamp(instance, (SoundRamp *)&Raziel.soundHandle, 19, 1000, 1000, 120, 0, 245760, 3500);
                razSetupSoundRamp(instance, (SoundRamp *)&Raziel.soundHandle2, 20, 1030, 1030, 120, 0, 245760, 3500);

                Raziel.soundTimerNext = 0;
                Raziel.soundTimerData = 0;
                break;
            case 3:
                if (Raziel.soundHandle != 0)
                {
                    SndEndLoop(Raziel.soundHandle);
                }

                Raziel.soundHandle = 0;

                razSetupSoundRamp(instance, (SoundRamp *)&Raziel.soundHandle, 13, 0, 0, 100, 100, 4096, 3500);

                Raziel.soundTimerNext = 40960;
                Raziel.soundTimerData = 4;
                break;
            case 4:
                if (Raziel.soundHandle2 != 0)
                {
                    SndEndLoop(Raziel.soundHandle2);
                }

                Raziel.soundHandle2 = 0;

                razSetupSoundRamp(instance, (SoundRamp *)&Raziel.soundHandle2, 13, 50, 50, 95, 95, 4096, 3500);

                Raziel.soundTimerNext = 28672;
                Raziel.soundTimerData = 5;

                Raziel.effectsFlags |= 0x8;
                break;
            case 5:
                if (Raziel.soundHandle != 0)
                {
                    SndEndLoop(Raziel.soundHandle);
                }

                Raziel.soundHandle = 0;

                razSetupSoundRamp(instance, (SoundRamp *)&Raziel.soundHandle, 13, -20, -20, 80, 80, 4096, 3500);

                Raziel.soundTimerNext = 32768;
                Raziel.soundTimerData = 6;
                break;
            case 6:
                if (Raziel.soundHandle2 != 0)
                {
                    SndEndLoop(Raziel.soundHandle2);
                }

                Raziel.soundHandle2 = 0;

                razSetupSoundRamp(instance, (SoundRamp *)&Raziel.soundHandle2, 13, 100, 100, 75, 75, 4096, 3500);

                Raziel.soundTimerNext = 40960;
                Raziel.soundTimerData = 7;
                break;
            case 7:
                if (Raziel.soundHandle != 0)
                {
                    SndEndLoop(Raziel.soundHandle);
                }

                Raziel.soundHandle = 0;

                razSetupSoundRamp(instance, (SoundRamp *)&Raziel.soundHandle, 13, -100, -100, 65, 65, 4096, 3500);

                Raziel.soundTimerNext = 20480;
                Raziel.soundTimerData = 8;
                break;
            case 8:
                if (Raziel.soundHandle2 != 0)
                {
                    SndEndLoop(Raziel.soundHandle2);
                }

                Raziel.soundHandle2 = 0;

                razSetupSoundRamp(instance, (SoundRamp *)&Raziel.soundHandle2, 13, 30, 30, 60, 60, 4096, 3500);

                Raziel.soundTimerNext = 40960;
                Raziel.soundTimerData = 9;
                break;
            case 9:
                if (Raziel.soundHandle != 0)
                {
                    SndEndLoop(Raziel.soundHandle);
                }

                Raziel.soundHandle = 0;

                if (Raziel.soundHandle2 != 0)
                {
                    SndEndLoop(Raziel.soundHandle2);
                }

                Raziel.soundHandle2 = 0;

                Raziel.effectsFlags &= ~0x4;
                Raziel.effectsFlags &= ~0x8;
                break;
            case 10:
                razSetupSoundRamp(instance, (SoundRamp *)&Raziel.soundHandle, 51, -200, -200, 120, 0, 245760, 3500);

                Raziel.soundTimerNext = 0;
                Raziel.soundTimerData = 0;
                break;
            default:
                if (Raziel.soundHandle != 0)
                {
                    SndEndLoop(Raziel.soundHandle);
                }
            }
        }
    }
}

void SetTimer(int ticks)
{
    Raziel.timeAccumulator = ticks << 12;
}

void ProcessSpecialAbilities(Instance *instance)
{
    unsigned long reaver;

    if ((Raziel.Abilities & 0x8))
    {
        if (debugRazielFlags2 != 0)
        {
            unsigned long temp;

            temp = 1 << (Raziel.currentSoulReaver + 9);

            if (temp != debugRazielFlags2)
            {
                temp = debugRazielFlags2 & ~temp;

                debugRazielFlags2 = temp;
            }

            reaver = razGetReaverFromMask(temp);
        }
        else if (Raziel.CurrentPlane == 1)
        {
            reaver = 2;

            debugRazielFlags2 = 0x800;
        }
        else
        {
            reaver = 1;

            debugRazielFlags2 = 0x400;
        }

        if (Raziel.soulReaver == NULL)
        {
            Object *soulReaverOb;

            soulReaverOb = (Object *)objectAccess[22].object;

            if (soulReaverOb != NULL)
            {
                razReaverPickup(instance, INSTANCE_BirthObject(instance, soulReaverOb, 0));

                RAZIEL_DebugHealthFillUp();
            }
        }
        else if (Raziel.currentSoulReaver != reaver)
        {
            INSTANCE_Post(Raziel.soulReaver, 0x800103, reaver);

            Raziel.currentSoulReaver = reaver;
        }
        else if (reaver == 6)
        {
            Level *level;

            level = STREAM_GetLevelWithID(instance->currentStreamUnitID);

            if (instance->waterFace != NULL)
            {
                if (instance->matrix == NULL)
                {
                    return;
                }

                if (instance->matrix[41].t[2] < instance->splitPoint.z)
                {
                    razReaverImbue(2);
                }
            }
            else
            {
                if (instance->matrix == NULL)
                {
                    return;
                }

                if (instance->matrix[41].t[2] < level->waterZLevel)
                {
                    razReaverImbue(2);
                }
            }
        }
    }
    else if (Raziel.soulReaver != NULL)
    {
        INSTANCE_UnlinkFromParent(Raziel.soulReaver);

        INSTANCE_KillInstance(Raziel.soulReaver);

        Raziel.soulReaver = NULL;
        Raziel.currentSoulReaver = 0;

        debugRazielFlags2 = 0;

        Raziel.Senses.heldClass = 0;
    }
}

int GetControllerMessages(long *controlCommand)
{
    if ((controlCommand[1] & RazielCommands[2]))
    {
        EnMessageQueueData(&Raziel.padCommands, 0x80000000, 0);
    }
    else if ((controlCommand[2] & RazielCommands[2]))
    {
        EnMessageQueueData(&Raziel.padCommands, 0x20000000, 0);
    }

    if ((controlCommand[1] & RazielCommands[1]))
    {
        EnMessageQueueData(&Raziel.padCommands, 0x80000002, 0);
    }
    else if ((controlCommand[2] & RazielCommands[1]))
    {
        EnMessageQueueData(&Raziel.padCommands, 0x20000002, 0);
    }

    if ((controlCommand[1] & RazielCommands[3]))
    {
        EnMessageQueueData(&Raziel.padCommands, 0x80000001, 0);
    }
    else if ((controlCommand[2] & RazielCommands[3]))
    {
        EnMessageQueueData(&Raziel.padCommands, 0x20000001, 0);
    }

    if ((controlCommand[1] & RazielCommands[7]))
    {
        EnMessageQueueData(&Raziel.padCommands, 0x80000004, 0);
    }
    else if ((controlCommand[2] & RazielCommands[7]))
    {
        EnMessageQueueData(&Raziel.padCommands, 0x20000004, 0);
    }

    if ((controlCommand[1] & RazielCommands[6]))
    {
        EnMessageQueueData(&Raziel.padCommands, 0x80000008, 0);
    }
    else if ((controlCommand[2] & RazielCommands[6]))
    {
        EnMessageQueueData(&Raziel.padCommands, 0x20000008, 0);
    }

    if ((controlCommand[1] & RazielCommands[9]))
    {
        EnMessageQueueData(&Raziel.padCommands, 0x80000020, 0);
    }
    else if ((controlCommand[2] & RazielCommands[9]))
    {
        EnMessageQueueData(&Raziel.padCommands, 0x20000020, 0);
    }

    if ((controlCommand[1] & RazielCommands[0]))
    {
        EnMessageQueueData(&Raziel.padCommands, 0x80000010, 0);
    }

    return 0;
}

void RazielAdditionalCollide(Instance *instance, GameTracker *gameTracker)
{
    int rc;
    int Mode;
    short Height;
    evPhysicsEdgeData *Data;
    evPhysicsSwimData *swimData;
    Instance *Inst;

    if ((ControlFlag & 0x8))
    {
        rc = 1;
    }
    else
    {
        Height = 0; // this line is garbage, only needed for matching purposes
        rc = 0;
    }

    if ((ControlFlag & 0x2000))
    {
        PhysicsCheckDropOff(instance, SetPhysicsDropOffData(0, -96, Raziel.dropOffHeight, (short)Raziel.slipSlope, 256), 2);
    }

    if ((rc & 0x1))
    {
        if (gameTrackerX.cheatMode == 1)
        {
            Height = 64;
        }
        else
        {
            Height = 128;
        }

        if ((instance->oldPos.z - instance->position.z) > Height)
        {
            Height = instance->oldPos.z - instance->position.z;
        }

        if ((PhysicsCheckGravity(instance, SetPhysicsGravityData((short)instance->matrix[1].t[2] - instance->matrix[0].t[2], Height, 0, 0, 0, (short)Raziel.slipSlope), 7) & 0x1))
        {
            Raziel.Senses.Flags |= 0x4;
        }
        else
        {
            Raziel.Senses.Flags &= ~0x4;
        }

        FX_UpdateInstanceWaterSplit(instance);
    }
    else
    {
        instance->oldTFace = NULL;
        instance->tface = NULL;
        instance->tfaceLevel = NULL;
        instance->waterFace = NULL;
        instance->waterFaceTerrain = NULL;
    }

    if (((ControlFlag & 0x400)) && (Raziel.Senses.heldClass != 0x3))
    {
        Data = (evPhysicsEdgeData *)SetPhysicsEdgeData(400, -256, 144, 0, -196, 498, &Raziel.Senses.ForwardNormal,
        &Raziel.Senses.AboveNormal, &Raziel.Senses.Delta);

        Mode = PhysicsCheckEdgeGrabbing(instance, gameTracker, (intptr_t)Data, 3);

        if ((Mode & 0x6) == 6)
        {
            SetPhysics(instance, 0, 0, 0, 0);

            Raziel.Senses.Flags |= 0x2;

            if (ExtraRot != NULL)
            {
                G2Anim_DisableController(&instance->anim, 1, 14);

                instance->rotation.z += ExtraRot->z;

                ExtraRot = NULL;
            }

            if (Data != NULL)
            {
                PhysicsDefaultEdgeGrabResponse(instance, Data, 0);
            }
        }
        else
        {
            Raziel.Senses.Flags &= ~0x2;
        }

        if ((Mode & 0x2))
        {
            Raziel.Senses.Flags |= 0x1;
        }
        else
        {
            Raziel.Senses.Flags &= ~0x1;
        }
    }

    if ((ControlFlag & 0x100))
    {
        Inst = razGetHeldWeapon();

        swimData = (evPhysicsSwimData *)SetPhysicsSwimData(((Raziel.Mode & (0x1 << 18))) >> 18, &Raziel.iVelocity, 256, 416, 112);

        WaterStatus = PhysicsCheckSwim(instance, (intptr_t)swimData, 3);

        if (((swimData->rc & 0x10)) && (Inst != NULL) && (INSTANCE_Query(Inst, 4) == 3))
        {
            G2Anim_SetSpeedAdjustment(&instance->anim, 2048);
        }

        if (((swimData->rc & 0x20)) && (Inst != NULL) && (INSTANCE_Query(Inst, 4) == 3))
        {
            G2Anim_SetSpeedAdjustment(&instance->anim, 4096);
        }
    }
    else
    {
        WaterStatus = 32;

        FX_UpdateInstanceWaterSplit(instance);
    }

    if ((ControlFlag & 0x8000))
    {
        Mode = PhysicsCheckBlockers(instance, gameTracker, SetPhysicsEdgeData(256, -256, 80, 0, -104, 0,
            &Raziel.Senses.ForwardNormal, &Raziel.Senses.AboveNormal, &Raziel.Senses.Delta), 3);

        if ((Mode & 0x4))
        {
            Raziel.Senses.Flags |= 0x2;
        }
        else
        {
            Raziel.Senses.Flags &= ~0x2;
        }

        if ((Mode & 0x2))
        {
            Raziel.Senses.Flags |= 0x1;
        }
        else
        {
            Raziel.Senses.Flags &= ~0x1;
        }
    }

    if ((ControlFlag & 0x80000))
    {
        PhysicsFollowWall(instance, gameTracker, SetPhysicsWallCrawlData(0, -448, 160, -186), 7);
    }

    if (((ControlFlag & 0x4000000)) && (Raziel.attachedPlatform != NULL))
    {
        if ((INSTANCE_Query(Raziel.attachedPlatform, 2) & 0x8))
        {
            PhysicsCheckLinkedMove(instance, SetPhysicsLinkedMoveData(Raziel.attachedPlatform, 2, NULL, NULL), 5);
        }
        else
        {
            PhysicsCheckLinkedMove(instance, SetPhysicsLinkedMoveData(Raziel.attachedPlatform, 0, NULL, NULL), 5);
        }
    }
}

int GetEngageEvent(Instance *instance)
{
    int Ability;

    if (instance != NULL)
    {
        Ability = INSTANCE_Query(instance, 2);

        if ((Ability & 0x8))
        {
            return 0x2000000;
        }

        if ((Ability & 0x1))
        {
            return 0x2000001;
        }

        if ((Ability & 0x2))
        {
            return 0x2000004;
        }
    }

    return 0;
}

int SetupReaction(Instance *player, Instance *instance)
{
    int FaceAngle;
    int isEvent;

    player->yVel = 0;
    player->xVel = 0;

    player->work3 = (long)instance;

    isEvent = GetEngageEvent(instance) != 0x2000001;

    FaceAngle = player->position.z;

    if (isEvent != 0)
    {
        PhysicsCheckEdgeGrabbing(player, gameTracker, SetPhysicsEdgeData(400, -256, 144, 0, -196, 498, &Raziel.Senses.ForwardNormal, &Raziel.Senses.AboveNormal, &Raziel.Senses.Delta), 1);

        PhysicsDefaultEdgeGrabResponse(player, (evPhysicsEdgeData *)SetPhysicsEdgeData(0, 0, 0, 0, -141, 0, &Raziel.Senses.ForwardNormal, NULL, &Raziel.Senses.Delta), 1);
    }

    player->position.z = FaceAngle;

    return FaceAngle;
}

int CheckHolding(Instance *instance)
{
    return instance->LinkChild != NULL;
}

void DisableWristCollision(Instance *instance, int Side)
{
    if ((Side & 0x1))
    {
        COLLIDE_SegmentCollisionOff(instance, 31);
    }

    if ((Side & 0x2))
    {
        COLLIDE_SegmentCollisionOff(instance, 41);
    }
}

void EnableWristCollision(Instance *instance, int Side)
{
    if ((Side & 0x1))
    {
        COLLIDE_SegmentCollisionOn(instance, 31);
    }

    if ((Side & 0x2))
    {
        COLLIDE_SegmentCollisionOn(instance, 41);
    }
}

int GetCollisionType(Instance *instance)
{
    CollideInfo *collideInfo;
    HSphere *S;

    collideInfo = instance->collideInfo;

    S = collideInfo->prim0;

    if (S->id == 9)
    {
        if (collideInfo->type1 != 3)
        {
            Instance *inst;

            inst = collideInfo->inst1;

            inst->flags |= 0x4;
        }
        else
        {
            COLLIDE_SetBSPTreeFlag(collideInfo, 0x800);
        }

        if (collideInfo->type1 != 1)
        {
            COLLIDE_SegmentCollisionOff(instance, collideInfo->segment);

            if (Raziel.Senses.HitMonster != NULL)
            {
                INSTANCE_Post(Raziel.Senses.HitMonster, 0x1000024, 0);
            }

            Raziel.Senses.HitMonster = NULL;

            return 1;
        }
    }

    if ((*(int *)&collideInfo->flags & 0xFFFF0000) == 0x1010000) // TODO: this is probably a compiler optimization, find the real form
    {
        if ((ControlFlag & 0x1000))
        {
            return 1;
        }

        if ((((char *)collideInfo->prim0)[4] == 9) && (((char *)collideInfo->prim1)[4] == 8)) // TODO: find what the actual castings of prim0 and prim1 are
        {
            if (Raziel.Senses.HitMonster == NULL)
            {
                Raziel.Senses.HitMonster = collideInfo->inst1;
            }
            else
            {
                if (Raziel.Senses.HitMonster == collideInfo->inst1)
                {
                    return 1;
                }

                printf("MultiHit\n");
            }

            return 1;
        }
    }

    if (((collideInfo->type0 == 5) || (collideInfo->type1 == 5) || (collideInfo->type0 == 2) || (collideInfo->type1 == 2)) && ((ControlFlag & 0x8000000)))
    {
        return 1;
    }

    return 0;
}

void InitGlyphSystem(Instance *instance)
{
    Object *GlyphOb;
    Instance *iGlyph;

    GlyphOb = (Object *)objectAccess[20].object;

    if (GlyphOb != NULL)
    {
        iGlyph = INSTANCE_BirthObject(instance, GlyphOb, 0);

        if (iGlyph != NULL)
        {
            Raziel.GlyphSystem = iGlyph;
        }
    }
}

void mdRazielProcess(Instance *playerInstance, GameTracker *gameTracker, long *controlCommand)
{
    ProcessTimers(playerInstance);

    razClearPlayerEvent();

    ProcessRazControl(controlCommand);

    SetStates(playerInstance, gameTracker, controlCommand, -1);

    ProcessHints();

    CAMERA_Control(&theCamera, playerInstance);

    playerInstance->offset.x = 0;
    playerInstance->offset.y = 0;
    playerInstance->offset.z = 0;

    Raziel.collisionEdgeOffset.x = 0;
    Raziel.collisionEdgeOffset.y = 0;
    Raziel.collisionEdgeOffset.z = 0;

    Raziel.Senses.HitMonster = NULL;

    if (Raziel.GlyphSystem != NULL)
    {
        GlyphProcess(Raziel.GlyphSystem, gameTracker);
    }

    Raziel.Abilities = debugRazielFlags1;

    debugRazielFlags1 |= debugRazielFlags2;
}

void RazielProcess(Instance *playerInstance, GameTracker *gameTracker)
{
    mdRazielProcess(gameTracker->playerInstance, gameTracker, &gameTracker->controlCommand[0][0]);

    DebugProcess(playerInstance, &Raziel);

    Norm.z = 0;
    Norm.y = 0;
    Norm.x = 0;
}

void RazielInit(Instance *instance, GameTracker *gameTracker)
{
    instance->data = instance->object->data;

    gameTracker->playerInstance = instance;

    instance->intro = NULL;

    instance->zVel = 0;
    instance->zAccl = 0;

    instance->yVel = 0;
    instance->yAccl = 0;

    instance->work0 = 0;
    instance->work1 = 0;

    instance->offset.x = 0;
    instance->offset.y = 0;
    instance->offset.z = 0;

    instance->currentMainState = 0;
    instance->currentSubState = 0;

    instance->matrix = NULL;

    instance->maxZVel = 0;

    instance->work3 = 0;

    instance->lightGroup = 0;

    instance->flags = ((instance->flags | 0x400) & ~0x100) | 0x80;
    instance->flags2 |= 0x84;

    OBTABLE_GetInstanceCollideFunc(instance);
    OBTABLE_GetInstanceProcessFunc(instance);

    DisableWristCollision(instance, 2);
    DisableWristCollision(instance, 1);

    InitStates(instance);
}

INCLUDE_ASM("asm/nonmatchings/Game/RAZIEL/RAZIEL", RazielCollide);

void RAZIEL_TurnHead(Instance *instance, short *rotx, short *rotz, GameTracker *gameTracker)
{
    Rotation rot;
    evActionLookAroundData data;
    short rx;
    short rz;

    (void)gameTracker;

    if (((Raziel.Mode & 0x20000)) && (!(Raziel.throwMode & 0x4)))
    {
        *rotx += gameTrackerX.controlData[0][4] / 8;
        *rotz -= gameTrackerX.controlData[0][3] / 6;

        if ((Raziel.extraRot.x != 0) && (*rotx != 0))
        {
            *rotx -= Raziel.throwData->coilRot;
        }

        *rotx += Raziel.extraRot.x - 4096;

        rot.x = *rotx;
        rot.y = 0;
        rot.z = *rotz;

        LimitRotation(&rot);

        *rotx = rot.x - (Raziel.extraRot.x - 4096);
        *rotz = rot.z;

        ThrowSetFocusPoint(instance, &rot);

        if (Raziel.extraRot.x != 0)
        {
            CAMERA_SetLookRot(&theCamera, *rotx + Raziel.throwData->coilRot, *rotz);
        }
    }
    else
    {
        *rotx += gameTrackerX.controlData[0][4] / 4;
        *rotz -= gameTrackerX.controlData[0][3] / 3;

        rx = *rotx & 0xFFF;

        if (rx > 2048)
        {
            rx |= ~0xFFF;
        }

        *rotx = rx;

        rz = *rotz & 0xFFF;

        if (rz > 2048)
        {
            rz |= ~0xFFF;
        }

        *rotz = rz;

        data.minx = -768;
        data.maxx = 512;

        data.rotx = rotx;
        data.rotz = rotz;

        data.minz = -1024;
        data.maxz = 1024;

        razRotateUpperBody(instance, &data);
    }
}

void RAZIEL_SetLookAround(Instance *instance)
{
    G2Anim_EnableController(&instance->anim, 17, 14);
    G2Anim_EnableController(&instance->anim, 16, 14);
    G2Anim_EnableController(&instance->anim, 14, 14);

    INSTANCE_Post(instance, 0x100009, 1);

    Raziel.throwXRot = 0;
    Raziel.throwZRot = 0;
}

void RAZIEL_ResetLookAround(Instance *instance)
{
    G2Anim_InterpDisableController(&instance->anim, 17, 14, 300);
    G2Anim_InterpDisableController(&instance->anim, 16, 14, 300);
    G2Anim_InterpDisableController(&instance->anim, 14, 14, 300);

    INSTANCE_Post(instance, 0x100009, 0);
}

long RAZIEL_OkToLookAround(Instance *playerInstance)
{
    (void)playerInstance;

    if (((Raziel.Senses.Flags & 0x4)) && ((Raziel.State.SectionList[0].Process == StateHandlerIdle) || (Raziel.State.SectionList[0].Process == StateHandlerThrow2)))
    {
        return 1;
    }

    if ((Raziel.State.SectionList[1].Process == StateHandlerSwim) || (Raziel.State.SectionList[1].Process == StateHandlerSwimTread))
    {
        return 1;
    }

    return 0;
}

void TrailWaterFX(Instance *instance, int Segment, int Bubbles, int Type)
{
    SVector Pos;
    SVector Vel;
    SVector Accl = {0};
    int i;
    int j;
    Level *level;
    BubbleParams BP;
    int temp, temp2; // not from decls.h

    level = STREAM_GetLevelWithID(instance->currentStreamUnitID);

    BP.DisperseFrames = 5;

    BP.KillScale = 128;

    BP.MinSplashSize = 2048;

    BP.MaxSpeed = 24;
    BP.MaxSpeedRange = 24;

    BP.ScaleRate = 32;
    BP.ScaleRateRange = 16;

    BP.UniqueBubbles = 4;

    if ((instance->matrix != NULL) && (instance->oldMatrix != NULL))
    {
        for (i = 0; i < 2; i++)
        {
            Pos.x = instance->oldMatrix[Segment].t[0] + (((instance->matrix[Segment].t[0] - instance->oldMatrix[Segment].t[0]) * i) / 2);
            Pos.y = instance->oldMatrix[Segment].t[1] + (((instance->matrix[Segment].t[1] - instance->oldMatrix[Segment].t[1]) * i) / 2);
            Pos.z = instance->oldMatrix[Segment].t[2] + (((instance->matrix[Segment].t[2] - instance->oldMatrix[Segment].t[2]) * i) / 2);

            for (j = 0; j < Bubbles; j++)
            {
                Vel.x = Pos.x - instance->oldMatrix[Segment].t[0] + (rand() % 20) - 10;
                Vel.y = Pos.y - instance->oldMatrix[Segment].t[1] + (rand() % 20) - 10;

                if (Type == 0)
                {
                    Vel.z = Pos.z - instance->oldMatrix[Segment].t[2] + (rand() % 10);

                    if ((Vel.z << 16) <= 0)
                    {
                        Vel.z = 1;
                    }

                    Accl.z = 0;

                    BP.StartScale = 3072;
                    BP.StartScaleRange = 256;

                    FX_MakeWaterBubble(&Pos, &Vel, &Accl, level->waterZLevel, &BP);
                }
                else if (Type == 1)
                {
                    Vel.z = (rand() % 20) + 6;

                    // TODO: the following temp vars aren't really needed, have to find the actual arithmetic operations
                    temp2 = rand();

                    temp = temp2;

                    if (temp < 0)
                    {
                        temp2 = temp + 3;
                    }

                    Accl.z = (temp - ((temp2 >> 2) << 2)) + 1;

                    BP.StartScale = 4096;
                    BP.StartScaleRange = 2048;

                    FX_MakeWaterBubble(&Pos, &Vel, &Accl, level->waterZLevel, &BP);
                }
            }
        }
    }
}
