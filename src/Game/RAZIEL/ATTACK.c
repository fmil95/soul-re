#include "Game/RAZIEL/ATTACK.h"
#include "Game/G2/ANMCTRLR.h"
#include "Game/PHYSOBS.h"

int StateHandlerDecodeHold(int *Message, int *Data)
{
    int rc;
    int WhoAmI;
    Instance *heldInstance;

    rc = 1;

    WhoAmI = 0;

    heldInstance = razGetHeldWeapon();

    if (Raziel.CurrentPlane == 2)
    {
        *Data = rc;

        *Message = 0x80000000;

        if (!(Raziel.Senses.EngagedMask & 0x200))
        {
            rc = 0;

            if ((Raziel.Senses.heldClass == 0x1000) && ((Raziel.Abilities & 0x4)))
            {
                rc = 1;

                *Message = 0x800010;
            }
        }
    }
    else
    {
        if ((Raziel.Senses.EngagedMask & 0x200))
        {
            WhoAmI = INSTANCE_Query(Raziel.Senses.EngagedList[9].instance, 1);
        }

        if ((WhoAmI & 0x8))
        {
            *Data = rc;

            if (Raziel.Senses.heldClass == 0x1000)
            {
                *Message = 0x1000023;
            }
            else
            {
                *Message = 0x80000000;
            }
        }
        else if (((Raziel.Senses.EngagedMask & 0x200)) && (heldInstance != NULL) && ((Raziel.Senses.heldClass != 0x3) && (Raziel.Senses.heldClass != 0x8)))
        {
            int hitState;

            hitState = INSTANCE_Query(Raziel.Senses.EngagedList[9].instance, 0);

            if ((hitState & 0x2000000))
            {
                if (heldInstance == Raziel.soulReaver)
                {
                    *Message = 0x1000023;

                    *Data = rc;
                }
                else
                {
                    goto label;
                }
            }
            else
            {
                if ((hitState & 0x10000000))
                {
                    *Data = rc;

                    *Message = 0x100000A;
                }
                else
                {
                    *Data = 0;

                    *Message = 0x100000A;
                }

                if (heldInstance == Raziel.soulReaver)
                {
                    *Message = 0x1000023;

                    if ((hitState & 0x21000000))
                    {
                        *Data = 1;
                    }
                    else
                    {
                        *Data = 0;
                    }
                }
                else if ((INSTANCE_Query(heldInstance, 2) & 0x20))
                {
                    *Message = 0x1000018;

                    if (*Data != 0)
                    {
                        if ((INSTANCE_Query(heldInstance, 3) & 0x10000))
                        {
                            *Data = 1;
                        }
                        else
                        {
                            *Data = 0;
                        }
                    }
                }
            }
        }
        else if (((Raziel.Senses.EngagedMask & 0x100)) && (heldInstance == NULL))
        {
            if ((int)INSTANCE_Query(Raziel.Senses.EngagedList[8].instance, 0) < 0)
            {
                *Data = 1;

                *Message = 0x1000002;
            }
            else
            {
                *Data = 0;

                *Message = 0x1000002;
            }
        }
        else
        {
            Instance *heldWeapon;

            if (((Raziel.Senses.EngagedMask & 0x200)) && (Raziel.Senses.heldClass == 0x3))
            {
                *Message = 0x80000000;

                *Data = 1;

                return 1;
            }

            heldWeapon = razGetHeldWeapon();

            if ((heldWeapon != NULL) && ((heldWeapon != Raziel.soulReaver) || ((Raziel.Abilities & 0x4))))
            {
                *Message = 0x800010;
            }
            else if ((Raziel.Abilities & 0x4))
            {
                *Message = 0x80000;
            }
            else
            {
            label:
                rc = 0;
            }
        }

        if (Raziel.Senses.heldClass != 0x3)
        {
            return rc;
        }

        if (*Message != 0x800010)
        {
            return 0;
        }
    }

    return rc;
}

void StateHandlerAttack2(CharacterState *In, int CurrentSection, intptr_t Data)
{
    Message *Ptr;
    int message;
    int messageData;
    int Frame;
    int Anim;
    int ignoreHit;
    Instance *ignoreInst;
    Instance *inst;
    Instance *weaponInst;
    evMonsterHitData *data;
    evMonsterHitData *hitData;
    G2AnimSection *animSection;

    Frame = G2EmulationQueryFrame(In, CurrentSection);

    Anim = G2EmulationQueryAnimation(In, CurrentSection);

    ignoreHit = 0;

    ignoreInst = NULL;

    if (CurrentSection == 0)
    {
        if (!(ControlFlag & 0x20))
        {
            if (((ControlFlag & 0x10000000)) && (!(Raziel.Mode & 0x2)))
            {
                if ((*PadData & 0x8000000F))
                {
                    SteerSwitchMode(In->CharacterInstance, 2);
                }
                else
                {
                    SteerSwitchMode(In->CharacterInstance, 0);
                }
            }
        }
        else
        {
            SteerSwitchMode(In->CharacterInstance, 0);
        }
    }

    while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
    {
        switch (Ptr->ID)
        {
        case 0x100001:
            Raziel.currentAttack = Ptr->Data;

            In->SectionList[CurrentSection].Data2 = 0;

            if (razGetHeldWeapon() != NULL)
            {
                switch (Raziel.Senses.heldClass)
                {
                case 0x1:
                    Raziel.currentAttack = Ptr->Data + 1;
                    break;
                case 0x2:
                    Raziel.currentAttack = Ptr->Data + 2;
                    break;
                case 0x1000:
                    Raziel.currentAttack = Ptr->Data + 3;
                    break;
                case 0x3:
                    Raziel.currentAttack = Ptr->Data + 4;
                    break;
                }
            }

            Raziel.attack = PlayerData->attackList[Raziel.currentAttack][In->SectionList[CurrentSection].Data2];

            if (Ptr->Data >= 10)
            {
                if (CurrentSection == 1)
                {
                    G2EmulationSwitchAnimationAlpha(In, 1, Raziel.attack->anim, 0, Raziel.attack->framesIn, 1, Raziel.attack->alphaIn);
                }
            }
            else
            {
                G2EmulationSwitchAnimationAlpha(In, CurrentSection, Raziel.attack->anim, 0, Raziel.attack->framesIn, 1, Raziel.attack->alphaIn);
            }

            if (CurrentSection == 1)
            {
                unsigned long startColor;
                unsigned long endColor;

                startColor = Raziel.attack->ribbonStartColor;
                endColor = Raziel.attack->ribbonEndColor;

                if (razGetHeldWeapon() != NULL)
                {
                    inst = razGetHeldWeapon();

                    if ((INSTANCE_Query(inst, 2) & 0x20))
                    {
                        if ((INSTANCE_Query(inst, 3) & 0x10000))
                        {
                            startColor = PlayerData->nonBurningRibbonStartColor;
                            endColor = PlayerData->nonBurningRibbonEndColor;
                        }
                    }
                    else if (Raziel.Senses.heldClass == 0x1000)
                    {
                        startColor = REAVER_GetGlowColor(inst);
                        endColor = 0;
                    }
                }
                else
                {
                    inst = In->CharacterInstance;
                }

                FX_StartRibbon(inst, Raziel.attack->ribbonStartSegment, Raziel.attack->ribbonEndSegment, 0, Raziel.attack->ribbonLifeTime, Raziel.attack->ribbonFaceLifeTime, (short)Raziel.attack->ribbonStartOpacity, startColor, endColor);

                if (Ptr->Data >= 10)
                {
                    if ((Raziel.Senses.EngagedMask & 0x40))
                    {
                        SteerSwitchMode(In->CharacterInstance, 15);

                        SetTimer(4);
                    }
                }
                else
                {
                    SteerSwitchMode(In->CharacterInstance, 9);
                }

                ControlFlag |= 0x2000;

                if (Ptr->Data == 5)
                {
                    SetTimer(1);
                }
            }

            Raziel.attackLastHit = NULL;
            Raziel.attackCurrentHit = NULL;

            Raziel.glowEffect = NULL;

            Raziel.Mode |= 0x200000;
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
        case 0x100015:
            if ((Raziel.steeringMode == 15) && ((*PadData & 0x8000000F)))
            {
                SteerSwitchMode(In->CharacterInstance, 2);
            }
            else
            {
                SteerSwitchMode(In->CharacterInstance, 0);
            }

            break;
        case 0x2000000:
        case 0x80000000:
            Raziel.attack = PlayerData->attackList[Raziel.currentAttack][In->SectionList[CurrentSection].Data2];

            if ((Anim == Raziel.attack->anim) && (Frame >= Raziel.attack->ignoreDelay))
            {
                Raziel.attackFlags |= 0x4;
            }

            if ((CurrentSection == 1) && (Raziel.glowEffect != NULL))
            {
                FX_StopGlowEffect(Raziel.glowEffect, 0);

                Raziel.glowEffect = NULL;
            }

            break;
        case 0x1000001:
        case 0x4020000:
            break;
        case 0x100004:
            if (CurrentSection == 1)
            {
                DisableWristCollision(In->CharacterInstance, 2);
                DisableWristCollision(In->CharacterInstance, 1);

                Raziel.dropOffHeight = 256;

                Raziel.fallZVelocity = -96;

                weaponInst = razGetHeldWeapon();

                if (weaponInst != NULL)
                {
                    INSTANCE_Post(weaponInst, 0x200005, 0);
                    INSTANCE_Post(weaponInst, 0x200003, 7);
                }
            }

            ControlFlag &= ~0x800000;
            break;
        case 0x8000000:
            if (((Raziel.attackFlags & 0x4)) && (PlayerData->attackList[Raziel.currentAttack][In->SectionList[CurrentSection].Data2 + 1] != NULL))
            {
                if (Raziel.currentAttack >= 10)
                {
                    if ((*PadData & 0x8000000F))
                    {
                        EnMessageQueueData(&In->SectionList[CurrentSection].Event, 0x100000, 0);
                    }
                }
                else
                {
                    EnMessageQueueData(&In->SectionList[CurrentSection].Event, 0x100000, 0);
                }
            }
            else if (Raziel.currentAttack >= 10)
            {
                if ((*PadData & 0x8000000F))
                {
                    StateSwitchStateData(In, CurrentSection, StateHandlerMove, 0);
                }
                else
                {
                    StateSwitchStateCharacterData(In, StateHandlerStopMove, 60);
                }
            }
            else if ((Raziel.nothingCounter < 7) && ((Raziel.Senses.EngagedMask & 0x40)))
            {
                StateSwitchStateData(In, CurrentSection, StateHandlerAutoFace, 0);
            }
            else
            {
                StateSwitchStateData(In, CurrentSection, StateHandlerIdle, SetControlInitIdleData(1, 0, Raziel.attack->framesOut));
            }

            break;
        case 0x80000020:
            if (StateHandlerDecodeHold(&message, &messageData) != 0)
            {
                EnMessageQueueData(&In->SectionList[CurrentSection].Event, message, messageData);

                ControlFlag |= 0x800000;
            }

            break;
        case 0x100000:
            In->SectionList[CurrentSection].Data2++;

            if (PlayerData->attackList[Raziel.currentAttack][In->SectionList[CurrentSection].Data2] == NULL)
            {
                In->SectionList[CurrentSection].Data2 = 1;
            }

            Raziel.attack = PlayerData->attackList[Raziel.currentAttack][In->SectionList[CurrentSection].Data2];

            G2EmulationSwitchAnimationAlpha(In, CurrentSection, Raziel.attack->anim, 0, Raziel.attack->framesIn, 1, Raziel.attack->alphaIn);

            if (CurrentSection == 1)
            {
                unsigned long startColor;
                unsigned long endColor;

                startColor = Raziel.attack->ribbonStartColor;
                endColor = Raziel.attack->ribbonEndColor;

                if (razGetHeldWeapon() != NULL)
                {
                    weaponInst = razGetHeldWeapon();

                    if ((INSTANCE_Query(weaponInst, 2) & 0x20))
                    {
                        if ((INSTANCE_Query(weaponInst, 3) & 0x10000))
                        {
                            startColor = PlayerData->nonBurningRibbonStartColor;
                            endColor = PlayerData->nonBurningRibbonEndColor;
                        }
                    }
                    else if (Raziel.Senses.heldClass == 0x1000)
                    {
                        startColor = REAVER_GetGlowColor(weaponInst);
                        endColor = 0;
                    }
                }
                else
                {
                    weaponInst = In->CharacterInstance;
                }

                Raziel.attackLastHit = Raziel.attackCurrentHit;
                Raziel.attackCurrentHit = NULL;

                FX_StartRibbon(weaponInst, Raziel.attack->ribbonStartSegment, Raziel.attack->ribbonEndSegment, 0, Raziel.attack->ribbonLifeTime, Raziel.attack->ribbonFaceLifeTime, (short)Raziel.attack->ribbonStartOpacity, startColor, endColor);
            }

            EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x100002, 0);

            PurgeMessageQueue(&In->SectionList[CurrentSection].Event);
            break;
        case 0x100002:
            Raziel.attackFlags = 0;
            break;
        case 0x1000002:
            if (Raziel.CurrentPlane == 1)
            {
                StateSwitchStateData(In, CurrentSection, StateHandlerGrab, messageData);
            }

            break;
        case 0x100000A:
            if (Ptr->Data != 0)
            {
                razSetPlayerEventHistory(0x80);

                ControlFlag |= 0x40000;

                if (CurrentSection == 2)
                {
                    G2EmulationSwitchAnimation(In, 2, 0, 0, 3, 2);
                }
                else
                {
                    G2EmulationSwitchAnimation(In, CurrentSection, 59, 0, 3, 1);
                }

                StateSwitchStateData(In, CurrentSection, StateHandlerCannedReaction, 0);

                if ((CurrentSection == 0) && ((Raziel.Senses.EngagedMask & 0x200)))
                {
                    razAlignYRotMoveInterp(In->CharacterInstance, Raziel.Senses.EngagedList[9].instance, 520, 0, 3, 0);
                }
            }
            else
            {
                ControlFlag |= 0x20;

                if (CurrentSection == 1)
                {
                    G2EmulationSwitchAnimationCharacter(In, 72, 0, 3, CurrentSection);

                    if ((Raziel.Senses.EngagedMask & 0x200))
                    {
                        INSTANCE_Post(Raziel.Senses.EngagedList[9].instance, 0x100000A, 0);
                    }
                }
            }

            break;
        case 0x8000004:
            if ((Raziel.Senses.EngagedMask & 0x200))
            {
                Instance *Inst;

                Inst = Raziel.Senses.EngagedList[9].instance;

                if (Raziel.Senses.heldClass == 0x1000)
                {
                    if (Raziel.currentSoulReaver == 6)
                    {
                        INSTANCE_Post(Inst, 0x100000C, 0x20);
                    }
                    else
                    {
                        INSTANCE_Post(Inst, 0x1000023, 0x1000);
                    }
                }
                else
                {
                    INSTANCE_Post(Inst, 0x100000C, 0x20);

                    Inst = razGetHeldWeapon();

                    if (Inst != NULL)
                    {
                        INSTANCE_Post(Inst, 0x800029, 0);
                    }
                }
            }

            break;
        case 0x1000018:
            if (Ptr->Data != 0)
            {
                razSetPlayerEventHistory(0x100);

                ControlFlag |= 0x40000;

                if (CurrentSection == 2)
                {
                    G2EmulationSwitchAnimation(In, 2, 0, 0, 3, CurrentSection);
                }
                else
                {
                    G2EmulationSwitchAnimation(In, CurrentSection, 138, 0, 0, 1);
                }

                if ((CurrentSection == 1) && ((Raziel.Senses.EngagedMask & 0x200)))
                {
                    Raziel.alarmTable = 900;

                    razAlignYRotMoveInterp(In->CharacterInstance, Raziel.Senses.EngagedList[9].instance, 486, 0, 20, 0);

                    In->CharacterInstance->anim.section[CurrentSection].swAlarmTable = &Raziel.alarmTable;
                }
            }
            else
            {
                ControlFlag |= 0x20;

                if (CurrentSection == 1)
                {
                    G2EmulationSwitchAnimationCharacter(In, 138, 0, 3, CurrentSection);

                    if ((Raziel.Senses.EngagedMask & 0x200))
                    {
                        INSTANCE_Post(Raziel.Senses.EngagedList[9].instance, 0x100000A, 0);
                    }
                }
            }

            break;
        case 0x1000023:
            if (Ptr->Data == 0)
            {
                ControlFlag |= 0x20;
            }

            if (CurrentSection == 0)
            {
                razSwitchVAnimCharacterSingle(In->CharacterInstance, 24, NULL, NULL);

                if ((Raziel.Senses.EngagedMask & 0x200))
                {
                    INSTANCE_Post(Raziel.Senses.EngagedList[9].instance, 0x1000023, Ptr->Data);

                    razAlignYRotMoveInterp(In->CharacterInstance, Raziel.Senses.EngagedList[9].instance, 486, 0, 20, 0);

                    if (Ptr->Data != 0)
                    {
                        Raziel.alarmTable = 3500;

                        In->CharacterInstance->anim.section[0].swAlarmTable = &Raziel.alarmTable;
                    }
                }
            }

            break;
        case 0x800010:
            StateSwitchStateData(In, CurrentSection, StateHandlerThrow2, 0);
            break;
        case 0x80000:
            Raziel.playerEvent |= 0x400;

            razSetPlayerEventHistory(0x400);

            StateSwitchStateData(In, CurrentSection, StateHandlerThrow2, 0);

            if (CurrentSection == 1)
            {
                razLaunchForce(In->CharacterInstance);
            }

            break;
        case 0x1000024:
            ignoreHit = 1;
            break;
        case 0x2000002:
            if (CurrentSection == 1)
            {
                inst = (Instance *)Ptr->Data;

                ignoreInst = (Instance *)inst->node.prev;

                data = (evMonsterHitData *)Ptr->Data;

                INSTANCE_Post(data->sender, 0x1000000, SetMonsterHitData(In->CharacterInstance, Raziel.attackLastHit, ((intptr_t)inst->prev * Raziel.attack->hitPowerScale) / 4096, Raziel.attack->knockBackDistance, Raziel.attack->knockBackFrames));

                if ((Raziel.attack->handsToCollide & 0x2))
                {
                    INSTANCE_Post(data->sender, 0x400000, SetFXHitData(In->CharacterInstance, 41, 32, 256));
                }

                if ((Raziel.attack->handsToCollide & 0x1))
                {
                    INSTANCE_Post(data->sender, 0x400000, SetFXHitData(In->CharacterInstance, 31, 32, 256));
                }

                Raziel.attackCurrentHit = data->sender;
            }

            break;
        case 0x80000001:
            if (CurrentSection == 0)
            {
                if ((ControlFlag & 0x10000000))
                {
                    Raziel.Mode = 0x8;

                    if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 0, NULL, NULL) != 0)
                    {
                        G2EmulationSwitchAnimationCharacter(In, 26, 0, 0, 1);
                    }

                    StateSwitchStateCharacterData(In, StateHandlerCompression, 0);

                    ControlFlag &= ~0x2000;
                    break;
                }

                EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x80000001, 0);
            }

            break;
        case 0x100001F:
            if (CurrentSection == 1)
            {
                inst = (Instance *)Ptr->Data;

                if (inst->node.next != NULL)
                {
                    hitData = (evMonsterHitData *)Ptr->Data;

                    INSTANCE_Post(hitData->lastHit, 0x100001F, SetMonsterHitData(hitData->sender, NULL, hitData->power, Raziel.attack->knockBackDistance, Raziel.attack->knockBackFrames));
                }
            }

            break;
        default:
            DefaultStateHandler(In, CurrentSection, Data);
        }

        DeMessageQueue(&In->SectionList[CurrentSection].Event);
    }

    if ((Raziel.attackFlags & 0x4))
    {
        animSection = &In->CharacterInstance->anim.section[CurrentSection & 0xFF];

        if (animSection->elapsedTime >= Raziel.attack->switchDelay * 100)
        {
            EnMessageQueueData(&In->SectionList[CurrentSection].Event, 0x100000, 0);
        }
    }

    if ((ignoreHit != 0) && (ignoreInst != NULL))
    {
        INSTANCE_Post(ignoreInst, 0x1000024, 0);
    }
}

void StateHandlerCannedReaction(CharacterState *In, int CurrentSection, intptr_t Data)
{
    Message *Ptr;

    while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
    {
        switch (Ptr->ID)
        {
        case 0x100001:
            if (CurrentSection == 0)
            {
                Raziel.alarmTable = 4500;

                Raziel.Mode = 0x10000;

                ControlFlag = 0x1041009;

                PhysicsMode = 3;

                SteerSwitchMode(In->CharacterInstance, 0);

                In->CharacterInstance->anim.section[0].swAlarmTable = &Raziel.alarmTable;
            }

            break;
        case 0x100004:
            if (CurrentSection == 1)
            {
                G2EmulationSwitchAnimationSync(In, 2, 1, 4);
            }

            break;
        case 0x100014:
        case 0x8000000:
            StateSwitchStateData(In, CurrentSection, StateHandlerIdle, SetControlInitIdleData(0, 0, 3));

            Raziel.Mode = 1;
            break;
        case 0x8000003:
        {
            Instance *Inst;

            if (CurrentSection == 0)
            {
                Inst = razGetHeldItem();

                if ((Raziel.Senses.EngagedMask & 0x200))
                {
                    INSTANCE_Post(Raziel.Senses.EngagedList[9].instance, 0x100000A, SetMonsterImpaleData(Inst, &In->CharacterInstance->rotation, &In->CharacterInstance->position, 520));
                }
            }

            break;
        }
        case 0x8000004:
        {
            Instance *Inst;

            Inst = razGetHeldItem();

            INSTANCE_Post(Inst, 0x800008, 2);

            razReaverOn();

            if ((Raziel.Senses.EngagedMask & 0x200))
            {
                INSTANCE_Post(Raziel.Senses.EngagedList[9].instance, 0x100000A, SetMonsterImpaleData(Inst, &In->CharacterInstance->rotation, &In->CharacterInstance->position, 520));
            }

            break;
        }
        case 0x1000000:
        case 0x1000001:
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

void StateHandlerStumble(CharacterState *In, int CurrentSection, intptr_t Data)
{
    Message *Ptr;
    int temp; // not from decls.h

    while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
    {
        switch (Ptr->ID)
        {
        case 0x100001:
            if (CurrentSection == 0)
            {
                ControlFlag = 0x8;

                SetPhysics(In->CharacterInstance, -16, 0, 0, 0);

                PhysicsMode = 0;

                SteerSwitchMode(In->CharacterInstance, 0);

                razResetMotion(In->CharacterInstance);

                if (Ptr->Data != 0)
                {
                    temp = ((Ptr->Data * 16) - Ptr->Data) * 2;

                    SetTimer(temp / 4096);
                }
                else
                {
                    SetTimer(1);
                }
            }

            G2Anim_SetSpeedAdjustment(&In->CharacterInstance->anim, 2048);

            G2EmulationSwitchAnimation(In, CurrentSection, 128, 0, 3, 2);
            break;
        case 0x100004:
            G2Anim_SetSpeedAdjustment(&In->CharacterInstance->anim, 4096);
            break;
        case 0x100000:
            if (((*PadData & RazielCommands[5])) && ((*PadData & RazielCommands[4])))
            {
                Raziel.returnState = StateHandlerIdle;

                StateSwitchStateCharacterData(In, StateHandlerIdle, SetControlInitIdleData(0, 0, 12));
                StateSwitchStateCharacterData(In, StateHandlerLookAround, 0);
            }
            else
            {
                StateSwitchStateCharacterData(In, StateHandlerIdle, SetControlInitIdleData(0, 0, 12));
            }

            break;
        case 0x4000001:
            break;
        case 0x10000000:
            if (In->SectionList[CurrentSection].Data2 == 1)
            {
                In->SectionList[CurrentSection].Data2 = 0;
                In->SectionList[CurrentSection].Data1 -= 512;
            }

            break;
        case 0:
            In->SectionList[CurrentSection].Data2 = 1;
            break;
        case 0x40003:
            if (CurrentSection == 0)
            {
                evActionPlayHostAnimationData *data;

                data = (evActionPlayHostAnimationData *)Ptr->Data;

                G2EmulationInstanceToInstanceSwitchAnimationCharacter(data->instance, data->host, data->newAnim, data->newFrame, data->frames, data->mode);
            }

            break;
        case 0x1000000:
            if ((gameTrackerX.debugFlags2 & 0x800))
            {
                evMonsterHitData *data;

                data = (evMonsterHitData *)Ptr->Data;

                LoseHealth(data->power);
            }

            break;
        case 0x100015:
            if (STREAM_IsMorphInProgress() == 0)
            {
                EnMessageQueueData(&Raziel.State.SectionList[CurrentSection].Defer, 0x100000, 0);
            }
            else
            {
                EnMessageQueueData(&Raziel.State.SectionList[CurrentSection].Defer, 0x100015, 0);
            }

            break;
        case 0x40005:
        case 0x100009:
        case 0x1000001:
        case 0x2000000:
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

void StateHandlerHitReaction(CharacterState *In, int CurrentSection, intptr_t Data)
{
    Message *Ptr;         
    
    while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
    {
        switch (Ptr->ID)
        {
        case 0x100001:
        {
            evMonsterHitData *data; 
                
            data = (evMonsterHitData*)Ptr->Data;
            
            if (CurrentSection == 0)
            {
                Instance *weapon;      
                
                Raziel.attackedBy = data->sender;
                
                weapon = razGetHeldWeapon();
                
                if (weapon != NULL) 
                {
                    TurnOffCollisionPhysOb(weapon, 7);
                }
                
                if ((gameTrackerX.debugFlags2 & 0x800))
                {
                    LoseHealth(data->power);
                }
                
                PhysicsMode = 0;
                
                ControlFlag |= 0x1008;
                ControlFlag &= ~0x8000000;
                
                Raziel.Mode |= 0x208000;
                
                ResetPhysics(In->CharacterInstance, -16);
                
                SteerSwitchMode(In->CharacterInstance, 10);
            }
            
            if (data->power > 4096) 
            {
                G2EmulationSwitchAnimation(In, CurrentSection, 56, 0, 2, 1);
            } 
            else
            {
                G2EmulationSwitchAnimation(In, CurrentSection, 125, 0, 2, 1);
            }
            
            break;
        }
        case 0x8000000:
        case 0x8000001:
            if (CurrentSection == 0)
            {
                if ((ControlFlag & 0x100000)) 
                {
                    StateSwitchStateCharacterData(In, StateHandlerGlyphs, 0);
                    break;
                }
                
                if ((Raziel.Mode & 0x40000)) 
                {
                    StateSwitchStateCharacterData(In, StateHandlerSwim, 0);
                } 
                else 
                {
                    StateSwitchStateCharacterData(In, StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
                }
            } 
            
            break;
        case 0x1000000:
        case 0x4020000:
        case 0x80000000:
        case 0x80000001:
        case 0x80000020:
            break;
        default:
            DefaultStateHandler(In, CurrentSection, Data);
            break;
        }
        
        DeMessageQueue(&In->SectionList[CurrentSection].Event);
    }
}

void StateHandlerThrow2(CharacterState *In, int CurrentSection, intptr_t Data)
{
    Message *Ptr;
    int Anim;

    Anim = G2EmulationQueryAnimation(In, CurrentSection);

    while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
    {
        switch (Ptr->ID)
        {
        case 0x100001:
            if (CurrentSection == 0)
            {
                ControlFlag = 0x41001;

                if (!(Raziel.Mode & 0x40000))
                {
                    SteerSwitchMode(In->CharacterInstance, 0);

                    ControlFlag |= 0x8;
                }
                else
                {
                    SteerSwitchMode(In->CharacterInstance, 11);
                }

                PhysicsMode = 3;

                Raziel.Mode |= 0x20000;

                In->SectionList[CurrentSection].Data1 = 0;

                if (razGetHeldWeapon() != NULL)
                {
                    Raziel.throwInstance = razGetHeldItem();

                    switch (Raziel.Senses.heldClass)
                    {
                    case 0x1:
                        Raziel.throwData = PlayerData->throwList[0];
                        break;
                    case 0x2:
                        Raziel.throwData = PlayerData->throwList[1];
                        break;
                    case 0x3:
                        Raziel.throwData = PlayerData->throwList[2];
                        break;
                    case 0x1000:
                        Raziel.throwInstance = NULL;
                        Raziel.throwData = PlayerData->throwList[5];

                        INSTANCE_Post(Raziel.soulReaver, 0x800104, 0);

                        Raziel.effectsFlags |= 0x4;

                        razSetupSoundRamp(In->CharacterInstance, (SoundRamp *)&Raziel.soundHandle, 12, -100, 0, 50, 85, 184320, 10000);

                        Raziel.soundTimerNext = 0;
                        Raziel.soundTimerData = 0;
                        break;
                    case 0x8:
                        In->SectionList[CurrentSection].Data1 = 1;

                        Raziel.throwData = PlayerData->throwList[4];
                        Raziel.throwInstance = NULL;
                        break;
                    }
                }

                Raziel.throwMode = 0;
            }

            if (CurrentSection == 2)
            {
                if ((Raziel.Mode & 0x40000))
                {
                    G2EmulationSwitchAnimation(In, 2, 61, 0, 3, 2);
                }
                else
                {
                    G2EmulationSwitchAnimation(In, 2, 0, 0, 3, 2);
                }
            }
            else if ((!(Raziel.Mode & 0x40000)) || (CurrentSection != 0))
            {
                if (Raziel.throwData[0].prepAnim != 0)
                {
                    G2EmulationSwitchAnimationAlpha(In, CurrentSection, Raziel.throwData[0].prepAnim, 0, Raziel.throwData[0].prepAnimFramesIn, 1, Raziel.throwData[0].prepAnimAlphaIn);
                }
                else
                {
                    G2EmulationSwitchAnimationAlpha(In, CurrentSection, Raziel.throwData[0].coilAnim, 0, Raziel.throwData[0].coilAnimFramesIn, 1, Raziel.throwData[0].coilAnimAlphaIn);
                }
            }
            else if (Anim != 63)
            {
                G2EmulationSwitchAnimation(In, 0, 63, 0, 16, 2);
            }

            In->SectionList[CurrentSection].Data2 = 1;
            break;
        case 0x100004:
            G2Anim_InterpDisableController(&In->CharacterInstance->anim, 14, 14, 300);

            if ((Raziel.effectsFlags & 0x2))
            {
                razSetFadeEffect(In->CharacterInstance->fadeValue, 0, -PlayerData->throwFadeOutRate);
            }

            if ((CurrentSection == 0) && (Raziel.soundHandle != 0))
            {
                SndEndLoop(Raziel.soundHandle);

                Raziel.soundHandle = 0;
            }

            if (Raziel.Senses.heldClass == 0x8)
            {
                INSTANCE_PlainDeath(razGetHeldItem());
            }

            if (Raziel.Senses.heldClass == 0x1000)
            {
                INSTANCE_Post(Raziel.soulReaver, 0x800105, 0);
            }

            break;
        case 0x1000001:
        case 0x4020000:
        case 0x20000002:
        case 0x80000000:
        case 0x80000020:
            break;
        case 0x80000008:
            if (Raziel.throwInstance != NULL)
            {
                EnMessageQueueData(&In->SectionList[CurrentSection].Event, 0x100000, 0);
            }

            break;
        case 0x100000:
            if ((Raziel.Mode & 0x40000))
            {
                StateSwitchStateData(In, CurrentSection, StateHandlerSwim, 0);
                break;
            }

            StateSwitchStateData(In, CurrentSection, StateHandlerIdle, SetControlInitIdleData(0, 0, Raziel.throwData->framesOut));

            if (((*PadData & RazielCommands[5])) && ((*PadData & RazielCommands[4])))
            {
                Raziel.returnState = StateHandlerIdle;

                StateSwitchStateData(In, CurrentSection, StateHandlerLookAround, 0);
            }

            break;
        case 0x1000000:
            if (CurrentSection == 0)
            {
                if ((Raziel.Mode & 0x40000))
                {
                    evMonsterHitData *data;

                    data = (evMonsterHitData *)Ptr->Data;

                    if ((gameTrackerX.debugFlags2 & 0x800))
                    {
                        LoseHealth(data->power);
                    }
                }
                else if ((!(ControlFlag & 0x4000)) && (Raziel.invincibleTimer == 0))
                {
                    StateSwitchStateCharacterDataDefault(In, StateHandlerHitReaction, Ptr->Data);

                    CAMERA_ForceEndLookaroundMode(&theCamera);
                }
            }

            break;
        case 0x8000000:
            Raziel.throwMode &= ~0x4;

            if (Anim == Raziel.throwData->prepAnim)
            {
                if ((!(Raziel.Mode & 0x40000)) || (CurrentSection != 0))
                {
                    G2EmulationSwitchAnimationAlpha(In, CurrentSection, Raziel.throwData->coilAnim, 0, Raziel.throwData->coilAnimFramesIn, 1, Raziel.throwData->coilAnimAlphaIn);
                }
            }
            else if ((Anim == Raziel.throwData->throwAnim) && (CurrentSection == 1))
            {
                INSTANCE_Post(In->CharacterInstance, 0x100000, 0);
            }

            break;
        case 0x20000020:
            if (In->SectionList[CurrentSection].Data2 != 0)
            {
                if (Anim == Raziel.throwData->coilAnim)
                {
                    if ((!(Raziel.Mode & 0x40000)) || (CurrentSection != 0))
                    {
                        G2EmulationSwitchAnimation(In, CurrentSection, Raziel.throwData->throwAnim, 0, Raziel.throwData->throwAnimFramesInFactor, 1);
                    }

                    if (CurrentSection == 1)
                    {
                        Raziel.alarmTable = Raziel.throwData->throwFrameToLaunch * 100;

                        In->CharacterInstance->anim.section[1].swAlarmTable = &Raziel.alarmTable;
                    }

                    In->SectionList[CurrentSection].Data2 = 0;

                    if ((Raziel.playerEvent & 0x200))
                    {
                        razSetPlayerEventHistory(0x200);
                    }
                }
                else
                {
                    EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x20000020, 0);
                }
            }

            break;
        case 0x8000004:
        {
            Instance *weaponInst;

            weaponInst = razGetHeldWeapon();

            if (weaponInst != NULL)
            {
                int spin_type;

                spin_type = Raziel.throwData->selfAdjustingFlag != 0;

                if ((Raziel.throwMode & 0x2))
                {
                    INSTANCE_Post(weaponInst, 0x800010, SetObjectThrowData(&Raziel.throwTarget, NULL, 4, spin_type, Raziel.throwData->velocity, Raziel.throwData->gravity, 0, 0));
                }
                else if ((Raziel.Senses.EngagedMask & 0x80))
                {
                    Position To;
                    Rotation Rot;
                    MATRIX *matrix;

                    INSTANCE_Post(weaponInst, 0x800010, SetObjectThrowData(Raziel.Senses.EngagedList[7].instance, NULL, 1, spin_type, (unsigned short)Raziel.throwData->velocity, Raziel.throwData->gravity, 0, 0));

                    matrix = (MATRIX *)INSTANCE_Query(Raziel.Senses.EngagedList[7].instance, 12);

                    To.x = (short)matrix->t[0];
                    To.y = (short)matrix->t[1];
                    To.z = (short)matrix->t[2];

                    PointAt(In->CharacterInstance, &To, &Rot);
                }
                else
                {
                    INSTANCE_Post(weaponInst, 0x800010, SetObjectThrowData(NULL, NULL, 0, spin_type, Raziel.throwData->velocity, 0, 0, 0));
                }

                razSetFadeEffect(In->CharacterInstance->fadeValue, 0, -PlayerData->throwFadeOutRate);

                razReaverOn();
            }

            break;
        }
        case 0x4000001:
            if (CurrentSection == 0)
            {
                SetDropPhysics(In->CharacterInstance, &Raziel);
            }

            G2EmulationSwitchAnimation(In, CurrentSection, 36, 0, 4, 1);

            StateSwitchStateData(In, CurrentSection, StateHandlerFall, 0);
            break;
        case 0x100009:
            if (CurrentSection == 1)
            {
                if (CurrentSection == Ptr->Data)
                {
                    if ((Raziel.Senses.heldClass != 0x1000) && (Raziel.Senses.heldClass != 0x8))
                    {
                        razSetFadeEffect(In->CharacterInstance->fadeValue, PlayerData->throwFadeValue, PlayerData->throwFadeInRate);
                    }

                    Raziel.returnState = StateHandlerIdle;

                    Raziel.throwMode = 2;
                }
                else
                {
                    razSetFadeEffect(In->CharacterInstance->fadeValue, 0, -PlayerData->throwFadeInRate);

                    G2Anim_InterpDisableController(&In->CharacterInstance->anim, 14, 14, 600);

                    Raziel.throwMode = 0;
                }
            }

            break;
        default:
            DefaultStateHandler(In, CurrentSection, Data);
        }

        DeMessageQueue(&In->SectionList[CurrentSection].Event);
    }

    if ((CurrentSection == 1) && ((Raziel.Senses.EngagedMask & 0x80)))
    {
        Position To;
        Rotation Rot;
        MATRIX *matrix;

        matrix = (MATRIX *)INSTANCE_Query(Raziel.Senses.EngagedList[7].instance, 12);

        To.x = (short)matrix->t[0];
        To.y = (short)matrix->t[1];
        To.z = (short)matrix->t[2];

        PointAt(In->CharacterInstance, &To, &Rot);
    }
}

void PointAt(Instance *instance, Position *Target, Rotation *Rot1)
{
    Position From;
    SVECTOR v1;
    VECTOR v3;
    MATRIX *tempMat;

    tempMat = instance->matrix;

    v1.vx = Raziel.throwData->launchPointX;
    v1.vy = Raziel.throwData->launchPointY;
    v1.vz = Raziel.throwData->launchPointZ;

    ApplyMatrix(tempMat, &v1, &v3);

    v3.vx += instance->position.x;
    v3.vy += instance->position.y;
    v3.vz += instance->position.z;

    From.x = (short)v3.vx;
    From.y = (short)v3.vy;
    From.z = (short)v3.vz;

    MATH3D_RotationFromPosToPos(&From, Target, Rot1);

    Rot1->z -= instance->rotation.z;
    Rot1->y = 0;

    LimitRotation(Rot1);

    if (G2Anim_IsControllerActive(&instance->anim, 14, 14) == G2FALSE)
    {
        G2Anim_EnableController(&instance->anim, 14, 14);
    }

    MATH3D_ZYXtoXYZ(Rot1);

    G2Anim_SetController_Vector(&instance->anim, 14, 14, (G2SVector3 *)Rot1);
}

void ThrowSetFocusPoint(Instance *instance, Rotation *rot)
{
    MATRIX *pTempMat;
    Instance *weaponInstance;
    SVECTOR v1;
    VECTOR v4;

    rot->x = -rot->x;

    pTempMat = theCamera.core.cwTransform2;

    weaponInstance = razGetHeldWeapon();

    if ((weaponInstance != NULL) && (weaponInstance->matrix != NULL))
    {
        v1.vx = 0;
        v1.vy = 0;
        v1.vz = Raziel.throwData->velocity;

        ApplyMatrix(pTempMat, &v1, &v4);

        Raziel.throwTarget.x = (short)v4.vx;
        Raziel.throwTarget.y = (short)v4.vy;
        Raziel.throwTarget.z = (short)v4.vz;

        if (G2Anim_IsControllerActive(&instance->anim, 14, 14) == G2FALSE)
        {
            G2Anim_EnableController(&instance->anim, 14, 14);
        }

        MATH3D_ZYXtoXYZ(rot);

        G2Anim_SetController_Vector(&instance->anim, 14, 14, (G2SVector3 *)rot);

        v4.vx = weaponInstance->matrix[1].t[0];
        v4.vy = weaponInstance->matrix[1].t[1];
        v4.vz = weaponInstance->matrix[1].t[2];

        CAMERA_SetLookFocusAndDistance(&theCamera, &v4, PlayerData->throwManualDistance);
    }
}

void LimitRotation(Rotation *rot)
{
    rot->z &= 0xFFF;
    rot->x &= 0xFFF;

    while (rot->z >= 4097)
    {
        rot->z -= 4096;
    }

    if (rot->z > 0)
    {
        if (rot->z < 2048)
        {
            if (rot->z > Raziel.throwData->maxZRotation)
            {
                rot->z = Raziel.throwData->maxZRotation;
            }
        }
        else if (rot->z < Raziel.throwData->minZRotation)
        {
            rot->z = Raziel.throwData->minZRotation;
        }
    }
    else
    {
        rot->z += 4096;

        if (rot->z < Raziel.throwData->minZRotation)
        {
            rot->z = Raziel.throwData->minZRotation;
        }
    }

    while (rot->x >= 4097)
    {
        rot->x -= 4096;
    }

    if (rot->x > 0)
    {
        if (rot->x < 2048)
        {
            if (rot->x > Raziel.throwData->maxXRotation)
            {
                rot->x = Raziel.throwData->maxXRotation;
            }
        }
        else if (rot->x < Raziel.throwData->minXRotation)
        {
            rot->x = Raziel.throwData->minXRotation;
        }
    }
    else
    {
        rot->x += 4096;

        if (rot->x < Raziel.throwData->minXRotation)
        {
            rot->x = Raziel.throwData->minXRotation;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/Game/RAZIEL/RAZIEL", StateHandlerGrab);
