#include "Game/RAZIEL/SWIM.h"

void StateHandlerInitSwim(CharacterState *In, int CurrentSection, intptr_t Data)
{
    (void)Data;

    G2EmulationSwitchAnimation(In, CurrentSection, 61, 0, 9, 2);

    StateSwitchStateData(In, CurrentSection, &StateHandlerSwimTread, 0);

    In->SectionList[CurrentSection].Data2 |= 0x2;

    if ((*PadData & RazielCommands[3]))
    {
        EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x80000001, 0);
    }

    if (CurrentSection == 2)
    {
        Raziel.RotationSegment = 1;

        Raziel.Mode = 0x40000;

        SteerSwitchMode(In->CharacterInstance, 6);

        ControlFlag = 0x191;

        CAMERA_ChangeToUnderWater(&theCamera, In->CharacterInstance);

        ExtraRot = &ExtraRotData;

        ExtraRotData.x = 128;

        ExternalForces[0].Friction = 3968;
    }
}

INCLUDE_ASM("asm/nonmatchings/Game/RAZIEL/RAZIEL", StateHandlerSwimCoil);

INCLUDE_ASM("asm/nonmatchings/Game/RAZIEL/RAZIEL", StateHandlerSwimTread);

void StateHandlerSwimDiveIn(CharacterState *In, int CurrentSection, intptr_t Data)
{

    Message *Ptr;
    evPhysicsSwimData *SwimData;

    (void)Data;
    SwimData = NULL;

    while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
    {
        switch (Ptr->ID)
        {
        case 0x100001:
            if (Raziel.Mode != 0x40000 && CurrentSection == 0)
            {
                CAMERA_ChangeToUnderWater(&theCamera, In->CharacterInstance);
            }
            if (CurrentSection == 0)
            {
                Raziel.Mode = 0x40000;
                ControlFlag = 0x191;
                PhysicsMode = 6;
                SteerSwitchMode(In->CharacterInstance, 0x11);
            }
            break;
        case 0x100004:
            if (!(Raziel.Mode & 0x40000) && (CurrentSection == 0))
            {
                CAMERA_ChangeToOutOfWater(&theCamera, In->CharacterInstance);
                Raziel.passedMask = 0;
            }
            break;
        case 0x8000000:
            if (*PadData & RazielCommands[3])
            {
                EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x80000001, 0);
            }
            if (SwimData->rc & 0x40)
            {
                StateSwitchStateData(In, CurrentSection, &StateHandlerSwimTread, 0);
            }
            else
            {
                StateSwitchStateData(In, CurrentSection, &StateHandlerSwim, 0);
                In->SectionList[CurrentSection].Data2 |= 2;
            }
            break;
        case 0x4020000:
            SwimData = (evPhysicsSwimData *)Ptr->Data;
            if (SwimData->rc & 0x80)
            {
                StateSwitchStateData(In, CurrentSection, &StateHandlerIdle, SetControlInitIdleData(0, 0, 9));
            }
            break;
        case 0x4010200:
        case 0x80000004:
        case 0x100000:
            break;
        case 0x100014:
            StateSwitchStateData(In, CurrentSection, &StateHandlerIdle, SetControlInitIdleData(0, 0, 9));
            if (CurrentSection == 0)
            {
                CAMERA_ChangeToOutOfWater(&theCamera, In->CharacterInstance);
            }
            break;
        case 0x1000000: {
            evMonsterHitData *data;

            if (CurrentSection == 0)
            {
                data = (evMonsterHitData *)Ptr->Data;
                if (gameTrackerX.debugFlags2 & 0x800)
                {
                    LoseHealth(data->power);
                }
            }
            break;
        }
        }
        DeMessageQueue(&In->SectionList[CurrentSection].Event);
    }

    if (CurrentSection == 0)
    {
        In->CharacterInstance->position.z -= 0x10;
        if (Raziel.passedMask & 1)
        {
            if (Raziel.passedMask & 2 || PadData[0] & 1)
            {
                Raziel.extraRot.x += 0x30;
            }
            if (Raziel.extraRot.x >= 0x401)
            {
                PhysicsMode = 5;
            }
            if (Raziel.passedMask & 4)
            {
                razLaunchBubbles(3, 1, 0);
            }
        }
    }
}

void StateHandlerSwim(CharacterState *In, int CurrentSection, intptr_t Data)
{
    Message *Ptr;
    evPhysicsSwimData *SwimData;
    int rc;
    int release;
    Instance *temp; // not from decls.h

    (void)release;

    rc = 1;

    if (CurrentSection == 0)
    {
        if (In->CharacterInstance->zVel > 0)
        {
            In->SectionList[CurrentSection].Data2 |= 0x2;
        }

        if (Raziel.iVelocity.z < -96)
        {
            razLaunchBubbles(3, 3, 1);
        }

        if ((Raziel.passedMask & 0x1))
        {
            razLaunchBubbles(2, 1, 0);
        }

        if ((Raziel.passedMask & 0x2))
        {
            razLaunchBubbles(2, 1, 0);
        }
    }

    while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)))
    {
        switch (Ptr->ID)
        {
        case 0x100001:
            if (Raziel.Mode != 0x40000)
            {
                if (CurrentSection == 0)
                {
                    CAMERA_ChangeToUnderWater(&theCamera, In->CharacterInstance);
                }
            }

            if (CurrentSection == 0)
            {
                Raziel.Mode = 0x40000;

                Raziel.RotationSegment = 1;

                ControlFlag = 0x190;

                SteerSwitchMode(In->CharacterInstance, 6);

                razSetSwimVelocity(In->CharacterInstance, 0, PlayerData->SwimPhysicsUnderDeceleration);

                G2Anim_SetSpeedAdjustment(&In->CharacterInstance->anim, 4096);
            }

            if (CurrentSection == 2)
            {
                G2EmulationSwitchAnimation(In, 2, 61, 0, 3, 2);
            }
            else
            {
                G2EmulationSwitchAnimation(In, CurrentSection, 63, 0, 16, 2);
            }

            PhysicsMode = 0x5;

            In->SectionList[CurrentSection].Data1 = 0;
            In->SectionList[CurrentSection].Data2 = 0;

            Raziel.passedMask = 0;
            break;
        case 0x100004:
            if (!(Raziel.Mode & 0x40000))
            {
                if (CurrentSection == 0)
                {
                    G2Anim_SetSpeedAdjustment(&In->CharacterInstance->anim, 4096);

                    CAMERA_ChangeToOutOfWater(&theCamera, In->CharacterInstance);
                }
            }

            if (CurrentSection == 0)
            {
                Raziel.passedMask = 0;
            }

            break;
        case 0x4010200:
        case 0x20000001:
        case 0x8000000:
        case 0x1000001:
        case 0x4000001:
        case 0x80000000:
            break;
        case 0x80000002:
            if (CurrentSection == 1)
            {
                Raziel.returnState = In->SectionList[1].Process;

                StateSwitchStateData(In, 1, &StateHandlerSoulSuck, 0);
            }

            break;
        case 0x80000020:
            if (razGetHeldWeapon() != NULL)
            {
                EnMessageQueueData(&In->SectionList[CurrentSection].Event, 0x800010, 1);
            }
            else if ((Raziel.Abilities & 0x4))
            {
                EnMessageQueueData(&In->SectionList[CurrentSection].Event, 0x80000, 1);
            }

            break;
        case 0x800010:
            if (CurrentSection == 0)
            {
                StateSwitchStateCharacterData(In, &StateHandlerThrow2, 0);
            }

            break;
        case 0x80000:
            if (CurrentSection == 0)
            {
                razLaunchForce(In->CharacterInstance);

                StateSwitchStateCharacterData(In, &StateHandlerThrow2, 0);
            }

            break;
        case 0x8000001:
            if ((Ptr->Data == 68) || (Ptr->Data == 66))
            {
                if (!(In->SectionList[CurrentSection].Data2 & 0x1))
                {
                    if (((PadData[0] & RazielCommands[3])) && (Raziel.steeringMode == 6))
                    {
                        if (CurrentSection == 2)
                        {
                            G2EmulationSwitchAnimation(In, 2, 0, 0, 3, 2);
                            break;
                        }
                        else
                        {
                            G2EmulationSwitchAnimation(In, CurrentSection, 60, 0, 12, 2);
                            break;
                        }
                    }

                    if (CurrentSection == 0)
                    {
                        razSetSwimVelocity(In->CharacterInstance, 0, PlayerData->SwimPhysicsUnderDeceleration);
                    }

                    if (CurrentSection == 2)
                    {
                        G2EmulationSwitchAnimation(In, 2, 61, 0, 3, 2);
                        break;
                    }
                    else
                    {
                        G2EmulationSwitchAnimation(In, CurrentSection, 63, 0, 8, 2);
                        break;
                    }
                }

                razSetSwimVelocity(In->CharacterInstance, PlayerData->SwimPhysicsUnderKickVelocity, PlayerData->SwimPhysicsUnderKickAccel);

                In->SectionList[CurrentSection].Data2 &= ~0x1;
                break;
            }

            if ((Ptr->Data != 63) && (!(PadData[0] & RazielCommands[3])))
            {
                if (CurrentSection == 0)
                {
                    razSetSwimVelocity(In->CharacterInstance, 0, PlayerData->SwimPhysicsUnderDeceleration);
                }

                if (CurrentSection == 2)
                {
                    G2EmulationSwitchAnimation(In, 2, 61, 0, 3, 2);
                }
                else
                {
                    G2EmulationSwitchAnimation(In, CurrentSection, 63, 0, 16, 2);
                }
            }

            break;
        case 0x80000008:
            if (CurrentSection == 0)
            {
                if (G2EmulationQueryFrame(In, 0) >= 2)
                {
                    StateSwitchStateCharacterData(In, &StateHandlerSwimCoil, 0);
                }
                else if (rc != 0)
                {
                    rc = 0;

                    EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x80000008, 0);
                }
            }

            break;
        case 0x80000001:
            if (Raziel.steeringMode == 6)
            {
                if (G2EmulationQueryAnimation(In, CurrentSection) != 68)
                {
                    if (CurrentSection == 0)
                    {
                        razSetSwimVelocity(In->CharacterInstance, PlayerData->SwimPhysicsUnderKickVelocity, PlayerData->SwimPhysicsUnderKickAccel);

                        Raziel.alarmTable = 1500;

                        In->CharacterInstance->anim.section[0].swAlarmTable = &Raziel.alarmTable;
                    }

                    G2EmulationSwitchAnimation(In, CurrentSection, 68, 0, 5, 2);
                    break;
                }

                In->SectionList[CurrentSection].Data2 |= 0x1;
            }

            break;
        case 0x8000004:
            if ((PadData[0] & RazielCommands[3]))
            {
                razSetSwimVelocity(In->CharacterInstance, PlayerData->SwimPhysicsUnderVelocity, PlayerData->SwimPhysicsUnderKickDecel);
            }
            else
            {
                razSetSwimVelocity(In->CharacterInstance, 0, PlayerData->SwimPhysicsUnderDeceleration);
            }

            break;
        case 0x100009:
            if (CurrentSection == 0)
            {
                if (Ptr->Data != 0)
                {
                    SteerSwitchMode(In->CharacterInstance, 0);

                    razSetSwimVelocity(In->CharacterInstance, 0, PlayerData->SwimPhysicsUnderDeceleration);
                }
                else
                {
                    SteerSwitchMode(In->CharacterInstance, 6);
                }
            }

            break;
        case 0x4020000:
            SwimData = (evPhysicsSwimData *)Ptr->Data;

            if ((Raziel.CurrentPlane == 1) && (!(Raziel.Abilities & 0x10)))
            {
                Raziel.HitPoints = 100000;
            }
            else
            {
                if (CurrentSection == 0)
                {
                    temp = razGetHeldWeapon();

                    PhysicsForceSetWater(In->CharacterInstance, &In->SectionList[CurrentSection].Data1, SwimData->Depth, PlayerData->SwimPhysicsWaterFrequency, PlayerData->SwimPhysicsWaterAmplitude);

                    if (SwimData->Depth >= -63)
                    {
                        ControlFlag |= 0x400;
                    }
                    else
                    {
                        ControlFlag &= ~0x400;
                    }

                    if ((temp != NULL) && (INSTANCE_Query(temp, 4) == 3))
                    {
                        CAMERA_ChangeToOutOfWater(&theCamera, In->CharacterInstance);

                        StateSwitchStateCharacterData(In, &StateHandlerIdle, SetControlInitIdleData(0, 0, 9));
                    }
                }

                if (((SwimData->rc & 0x200)) && (CurrentSection == 0))
                {
                    ExternalForces[0].Friction = 48;
                }

                if (((SwimData->rc & 0x40)) && (Raziel.Mode == 0x40000) && (Raziel.iVelocity.z > 0))
                {
                    StateHandlerInitSwim(In, CurrentSection, Data);
                }

                if ((SwimData->rc & 0x400))
                {
                    if (CurrentSection == 0)
                    {
                        Raziel.Mode = 0x10;

                        SetExternalForce(&ExternalForces[2], 0, 0, -16, 1, 4096);

                        In->CharacterInstance->xVel = Raziel.iVelocity.x;
                        In->CharacterInstance->yVel = Raziel.iVelocity.y;
                        In->CharacterInstance->zVel = Raziel.iVelocity.z;

                        ExternalForces[0].Friction = 56;
                    }

                    G2EmulationSwitchAnimation(In, CurrentSection, 35, 0, 0, 1);

                    SteerSwitchMode(In->CharacterInstance, 4);

                    StateSwitchStateData(In, CurrentSection, &StateHandlerFall, 0);
                }

                if ((SwimData->rc & 0x20))
                {
                    Raziel.Mode = 0x100000;

                    G2EmulationSwitchAnimation(In, CurrentSection, 35, 0, 0, 1);

                    SteerSwitchMode(In->CharacterInstance, 4);

                    StateSwitchStateData(In, CurrentSection, &StateHandlerFall, 0);

                    SetExternalForce(&ExternalForces[0], 0, 0, -16, 1, 4096);
                }
            }

            break;
        case 0x20000004:
            G2Anim_SetSpeedAdjustment(&In->CharacterInstance->anim, 4096);
            break;
        case 0x80000004:
            G2Anim_SetSpeedAdjustment(&In->CharacterInstance->anim, PlayerData->SwimPhysicsUnderStealthAdjust);
            break;
        case 0x2000000:
            razPickupAndGrab(In, CurrentSection);
            break;
        case 0x80000010:
            if ((!(Raziel.Mode & 0x10000000)) && (CurrentSection == 0))
            {
                StateSwitchStateCharacterData(In, &StateHandlerGlyphs, 0);

                G2EmulationSwitchAnimation(In, 2, 61, 0, 3, 2);

                G2EmulationSwitchAnimation(In, 0, 63, 0, 16, 2);

                G2EmulationSwitchAnimation(In, 1, 63, 0, 16, 2);
            }

            break;
        case 0x100014:
            StateSwitchStateData(In, CurrentSection, &StateHandlerIdle, SetControlInitIdleData(0, 0, 9));

            if (CurrentSection == 0)
            {
                CAMERA_ChangeToOutOfWater(&theCamera, In->CharacterInstance);
            }

            break;
        case 0x1000000:
        {
            if (CurrentSection == 0)
            {
                evMonsterHitData *data = (evMonsterHitData *)Ptr->Data;
                if ((gameTrackerX.debugFlags2 & 0x800))
                {
                    LoseHealth(data->power);
                }
            }

            break;
        }
        default:
            DefaultStateHandler(In, CurrentSection, Data);
        }

        DeMessageQueue(&In->SectionList[CurrentSection].Event);
    }
}
