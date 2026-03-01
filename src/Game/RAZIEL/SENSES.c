#include "Game/RAZIEL/SENSES.h"

int SetEngagedInstance(Player *player, evCollideInstanceStatsData *Ptr, unsigned long Attribute)
{
    int rc;

    rc = 0;

    if ((player->Senses.EngagedMask & (1 << Attribute)))
    {
        if (player->Senses.EngagedList[Attribute].distance > Ptr->distance)
        {
            rc = 1;

            player->Senses.EngagedList[Attribute].instance = Ptr->instance;
            player->Senses.EngagedList[Attribute].distance = Ptr->distance;
        }
    }
    else
    {
        rc = 1;

        player->Senses.EngagedMask |= 1 << Attribute;

        player->Senses.EngagedList[Attribute].instance = Ptr->instance;
        player->Senses.EngagedList[Attribute].distance = Ptr->distance;
    }

    return rc;
}

void UpdateEngagementList(evCollideInstanceStatsData *Ptr, Player *player)
{
    int WhoAmI;
    int Ability;
    int hidden;
    PhysObCollectibleProperties *collectibleProp;

    WhoAmI = INSTANCE_Query(Ptr->instance, queryWhatAmI);

    hidden = Ptr->instance->flags & 0x800;

    if ((WhoAmI & 0x20))
    {
        Ability = INSTANCE_Query(Ptr->instance, queryPhysicalAbility);

        if (((Ability & 0x100)) && (!(Ptr->instance->flags & 0x800)))
        {
            evObjectDraftData *draft;

            draft = (evObjectDraftData *)INSTANCE_Query(Ptr->instance, queryPhysicalDraftData);

            if ((Ptr->xyDistance < draft->radius) && ((Ptr->zDelta < 0) && (Ptr->zDelta > -draft->height)))
            {
                draft->force = (short)((((((4096 - ((Ptr->xyDistance * 4096) / draft->radius)) * draft->force) / 4096) * draft->radiusCoef) / 4096) + (((((4096 - ((-Ptr->zDelta * 4096) / draft->height)) * draft->force) / 4096) * (4096 - draft->radiusCoef)) / 4096));

                INSTANCE_Post(player->State.CharacterInstance, 0x4000007, (intptr_t)draft);
            }
        }

        if ((Ability & 0x1))
        {
            if (player->Mode == 0x40000)
            {
                Ptr->xyDistance /= 2;
                Ptr->zDelta /= 2;
            }

            if (Ptr->instance->LinkParent == NULL || INSTANCE_Query(Ptr->instance->LinkParent, queryHitState) & 0x40000000)
            {
                if ((Ptr->xyDistance < 512) && ((Ptr->zDelta > -256) && (Ptr->zDelta < 640)))
                {
                    SetEngagedInstance(player, Ptr, 5);
                }
            }
        }

        if (((Ability & 0xE)) && (hidden == 0))
        {
            SVECTOR startVec;
            SVECTOR endVec;

            if (MATH3D_ConeDetect(&Ptr->relativePosition, 967, 967) != 0)
            {
                PHYSICS_GenericLineCheckSetup(0, 0, 0, &startVec);
                PHYSICS_GenericLineCheckSetup(0, -320, 0, &endVec);

                if (PHYSICS_CheckForTerrainCollide(gameTrackerX.playerInstance, &startVec, &endVec, 1) == 0)
                {
                    if ((Ptr->xyDistance < 640) && ((Ptr->zDelta > -16) && (Ptr->zDelta < 16)))
                    {
                        if ((Ability & 0x4))
                        {
                            SetEngagedInstance(player, Ptr, 1);
                        }

                        if ((Ability & 0x2))
                        {
                            SetEngagedInstance(player, Ptr, 2);
                        }
                    }

                    if ((Ptr->xyDistance < 525) && ((Ptr->zDelta > -256) && (Ptr->zDelta < 256)) && ((Ability & 0x8)))
                    {
                        SetEngagedInstance(player, Ptr, 0);
                    }
                }
            }
        }

        if ((Ability & 0x40))
        {
            PhysObSwitchProperties *prop;

            prop = (PhysObSwitchProperties *)INSTANCE_Query(Ptr->instance, queryPhysicalSwitchProperties);

            if ((Ptr->xyDistance < prop->engageXYDistance) && (Ptr->zDelta > (signed char)prop->engageZMinDelta) && (Ptr->zDelta < (signed char)prop->engageZMaxDelta))
            {
                if (MATH3D_ConeDetect(&Ptr->relativePosition, (prop->engageYCone << 12) / 360, (prop->engageZCone << 12) / 360) != 0)
                {
                    SetEngagedInstance(player, Ptr, 3);
                }

                INSTANCE_Query(Ptr->instance, queryPhysicalStatus);
            }
        }

        if ((Ability & 0x80))
        {
            PhysObInteractProperties *interactProp;
            int testConditions;

            interactProp = (PhysObInteractProperties *)INSTANCE_Query(Ptr->instance, queryPhysicalInteractProperties);

            testConditions = 0;

            if (interactProp->conditions & 0x1 || interactProp->auxConditions & 0x1)
            {
                testConditions = 0xFFFF;
            }
            else
            {
                Instance *heldInst;

                heldInst = razGetHeldItem();

                if (heldInst == NULL)
                {
                    testConditions = 0x800;

                    if (Raziel.HitPoints == GetMaxHealth())
                    {
                        if (Raziel.soulReaver != 0)
                        {
                            testConditions = (((Raziel.Abilities & 0x3FC00) >> 9) & interactProp->conditions) | 0x800;
                        }

                        if (Raziel.currentSoulReaver == 6)
                        {
                            testConditions |= 0x200;
                        }
                    }
                }
                else if (INSTANCE_Query(heldInst, queryPhysicalAbility) & 0x20)
                {
                    if (INSTANCE_Query(heldInst, queryPhysicalMode) & 0x10000)
                    {
                        testConditions = 0x200;
                    }
                    else
                    {
                        testConditions = 0x400;
                    }
                }
            }

            if ((((testConditions & interactProp->conditions)) || ((testConditions & interactProp->auxConditions))) && (interactProp->mode != 2) && (Ptr->xyDistance < interactProp->engageXYDistance) && ((Ptr->zDelta > interactProp->engageZMinDelta) && (Ptr->zDelta < interactProp->engageZMaxDelta)) && (MATH3D_ConeDetect(&Ptr->relativePosition, (interactProp->engageYCone << 12) / 360, (interactProp->engageZCone << 12) / 360) != 0))
            {
                if ((testConditions & interactProp->conditions))
                {
                    SetEngagedInstance(player, Ptr, 4);
                }
                else
                {
                    SetEngagedInstance(player, Ptr, 13);
                }
            }
        }

        collectibleProp = (PhysObCollectibleProperties *)INSTANCE_Query(Ptr->instance, queryPhysicalCollectibleData);

        if ((collectibleProp != NULL) && (hidden == 0))
        {
            MATRIX *collectMat;
            Position headPos;
            Position collectPos;
            long headDistance;
            long rootDistance;

            collectMat = (MATRIX *)INSTANCE_Query(Ptr->instance, queryIDMatrix);

            if (collectMat != NULL)
            {
                collectPos.x = (short)collectMat->t[0];
                collectPos.y = (short)collectMat->t[1];
                collectPos.z = (short)collectMat->t[2];
            }

            headPos.x = (short)gameTrackerX.playerInstance->matrix[18].t[0];
            headPos.y = (short)gameTrackerX.playerInstance->matrix[18].t[1];
            headPos.z = (short)gameTrackerX.playerInstance->matrix[18].t[2];

            headDistance = MATH3D_DistanceBetweenPositions(&headPos, &collectPos);

            headPos.x = (short)gameTrackerX.playerInstance->matrix[1].t[0];
            headPos.y = (short)gameTrackerX.playerInstance->matrix[1].t[1];
            headPos.z = (short)gameTrackerX.playerInstance->matrix[1].t[2];

            rootDistance = MATH3D_DistanceBetweenPositions(&headPos, &collectPos);

            if ((Ptr->distance < collectibleProp->distance) || ((unsigned long)headDistance < collectibleProp->distance) || ((unsigned long)rootDistance < collectibleProp->distance))
            {
                switch (collectibleProp->collectClass)
                {
                case 1:
                    Raziel.HealthBalls++;

                    if (Raziel.HealthScale < ((Raziel.HealthBalls / 5) + 1))
                    {
                        BumpUpHealth();
                    }

                    INSTANCE_Post(Ptr->instance, 0x800026, Raziel.HealthBalls);
                    break;
                case 2:
                    if (Raziel.GlyphManaBalls < Raziel.GlyphManaMax)
                    {
                        Raziel.GlyphManaBalls += collectibleProp->collectValue;

                        if (Raziel.GlyphManaBalls > Raziel.GlyphManaMax)
                        {
                            Raziel.GlyphManaBalls = Raziel.GlyphManaMax;
                        }

                        INSTANCE_Post(Ptr->instance, 0x800026, Raziel.GlyphManaBalls);
                    }

                    break;
                case 3:
                {
                    unsigned long ability_bit;

                    INSTANCE_Post(Ptr->instance, 0x800026, 0);

                    ability_bit = (collectibleProp->collectValue & 0xFF) << 17;

                    if (!(Raziel.Abilities & ability_bit))
                    {
                        Raziel.Abilities |= ability_bit;

                        debugRazielFlags1 = Raziel.Abilities;

                        Raziel.GlyphManaMax += (collectibleProp->collectValue & 0xFF00) >> 8;
                        Raziel.GlyphManaBalls = Raziel.GlyphManaMax;
                    }

                    break;
                }
                case 5:
                    Raziel.GlyphManaMax += collectibleProp->collectValue;
                    Raziel.GlyphManaBalls = Raziel.GlyphManaMax;

                    INSTANCE_Post(Ptr->instance, 0x800026, 0);
                    break;
                case 4:
                    INSTANCE_Post(Ptr->instance, 0x800026, 0);
                    break;
                case 6:
                    INSTANCE_Post(Ptr->instance, 0x800026, Raziel.GlyphManaBalls);

                    RAZIEL_DebugHealthFillUp();
                    break;
                case 7:
                    INSTANCE_Post(Ptr->instance, 0x800026, Raziel.GlyphManaBalls);

                    SetMana(1);
                    break;
                }
            }
        }

        if (((Ptr->instance->object->oflags2 & 0x400000)) && (Ptr->distance < 3200) && (MATH3D_ConeDetect(&Ptr->relativePosition, 967, 967) != 0))
        {
            SetEngagedInstance(player, Ptr, 10);
        }
    }
    else if (((WhoAmI & 0xA) || (Ptr->instance->flags2 & 0x20)) && !(Ptr->instance->flags2 & 0x20000000))
    {
        if (!(INSTANCE_Query(Ptr->instance, queryHitState) & 0x44000000))
        {
            if (Ptr->instance->flags2 & 0x20)
            {
                if ((Ptr->xyDistance < 0x780) && ((Ptr->zDelta > -512) && (Ptr->zDelta < 512)))
                {
                    if (ControlFlag & 4)
                    {
                        if (Raziel.Senses.CurrentAutoFace != 0)
                        {
                            if (Ptr->instance == Raziel.Senses.CurrentAutoFace)
                            {
                                if (SetEngagedInstance(player, Ptr, 6) == 0)
                                {
                                    Raziel.Senses.CurrentAutoFace = 0;
                                }
                                else
                                {
                                    AutoFaceAngle = UpdateAutoFaceAngle(player->State.CharacterInstance, Ptr->instance);
                                }
                            }
                        }
                        else if (SetEngagedInstance(player, Ptr, 6) != 0)
                        {
                            AutoFaceAngle = UpdateAutoFaceAngle(player->State.CharacterInstance, Ptr->instance);
                        }
                    }
                    else if ((Ptr->instance != Raziel.Senses.LastAutoFace) || (Raziel.Senses.CurrentAutoFace == 0))
                    {
                        if (SetEngagedInstance(player, Ptr, 6) != 0)
                        {
                            AutoFaceAngle = UpdateAutoFaceAngle(player->State.CharacterInstance, Ptr->instance);
                        }
                    }
                }

                if (Ptr->instance->object->oflags2 >= 0)
                {
                    if (Ptr->distance >= 0x281)
                    {
                        if (MATH3D_ConeDetect(&Ptr->relativePosition, 0x2AA, 0x2AA) != 0)
                        {
                            SetEngagedInstance(player, Ptr, 7);
                        }
                    }
                }
            }
            if (WhoAmI & 0xA)
            {
                if (Ptr->xyDistance < 0x280)
                {
                    if (((Ptr->zDelta > -256) && (Ptr->zDelta < 256)) && (MATH3D_ConeDetect(&Ptr->relativePosition, 0x555, 0x38E) != 0))
                    {
                        SetEngagedInstance(player, Ptr, 8);
                    }
                }
                if ((WhoAmI & 0xA) && (Ptr->xyDistance < 0x3C0) && ((Ptr->zDelta > -256) && (Ptr->zDelta < 256)) && (MATH3D_ConeDetect(&Ptr->relativePosition, 0x555, 0x38E) != 0))
                {
                    SetEngagedInstance(player, Ptr, 9);
                }
            }
            if ((Ptr->instance->object->oflags2 & 0x400000) && (Ptr->distance < 0xC80) && (MATH3D_ConeDetect(&Ptr->relativePosition, 0x3C7, 0x3C7) != 0))
            {
                SetEngagedInstance(player, Ptr, 0xA);
            }
            if ((Ptr->distance < 0x1E00) && (INSTANCE_Query(Ptr->instance, 0x25) != 0))
            {
                Raziel.Senses.Flags = (Raziel.Senses.Flags | 0x20);
            }
        }
    }
    else if ((WhoAmI & 0x20000) && (Raziel.soulReaver == 0))
    {
        if ((Ptr->instance->LinkParent != player->State.CharacterInstance) && (Ptr->xyDistance < 0x200) && ((Ptr->zDelta > -100) && (Ptr->zDelta < 640)))
        {
            SetEngagedInstance(player, Ptr, 0xB);
        }
        if (Raziel.Abilities & 8)
        {
            razReaverPickup(player->State.CharacterInstance, Ptr->instance);
        }
    }

    if (INSTANCE_Query(Ptr->instance, queryHitState) & 0x08000000)
    {
        if (Ptr->xyDistance < 0x200)
        {
            if (((Ptr->zDelta > -100) && (Ptr->zDelta < 640)) && (MATH3D_ConeDetect(&Ptr->relativePosition, 0x2AA, 0x2AA) != 0))
            {
                SetEngagedInstance(player, Ptr, 0xC);
            }
        }
    }
    if (WhoAmI & 4)
    {
        Raziel.playerEvent = (Raziel.playerEvent | 0x1000);
        if (ControlFlag & 0x40)
        {
            Instance *Inst;

            Inst = player->State.CharacterInstance;
            INSTANCE_Post(Ptr->instance, 0x01000009, SetMonsterSoulSuckData(Inst, Inst->matrix[15].t[0], Inst->matrix[15].t[1], Inst->matrix[15].t[2]));
        }
    }

    if ((Ptr->instance->object->oflags2 & 0x4000000))
    {
        if ((Raziel.constrictFlag & 0x20))
        {
            VECTOR in;
            VECTOR out;

            in.vx = Ptr->instance->position.x - Raziel.constrictCenter.x;
            in.vy = Ptr->instance->position.y - Raziel.constrictCenter.y;
            in.vz = 0;

            gte_ldlvl(&in);
            gte_nsqr(0);
            gte_stlvnl(&out);

            if ((out.vx + out.vy) <= 409599)
            {
                int rc;

                rc = razConstrictAngle(Ptr->instance);

                if (((Raziel.constrictFlag & 0x10)) && (rc != 0))
                {
                    if (rc > 0)
                    {
                        if (Ptr->instance->constrictAngle < 0)
                        {
                            Ptr->instance->constrictAngle = 0;
                        }

                        Ptr->instance->constrictAngle++;
                    }
                    else
                    {
                        if (Ptr->instance->constrictAngle > 0)
                        {
                            Ptr->instance->constrictAngle = 0;
                        }

                        Ptr->instance->constrictAngle--;
                    }

                    if ((Ptr->instance->constrictAngle == 1) || (Ptr->instance->constrictAngle == -1))
                    {
                        FX_StartConstrict(Raziel.State.CharacterInstance, &Raziel.constrictCenter, 1, 18);

                        INSTANCE_Post(Ptr->instance, 0x40009, 0);
                    }
                    else if ((Ptr->instance->constrictAngle != -1) && (Ptr->instance->constrictAngle != 0) && (Ptr->instance->constrictAngle != 1))
                    {
                        gameTrackerX.streamFlags |= 0x4;

                        FX_EndConstrict(1, Ptr->instance);

                        Raziel.constrictFlag = 1;

                        INSTANCE_Post(Ptr->instance, 0x1000000, SetMonsterHitData(player->State.CharacterInstance, Ptr->instance, 12288, 0, 0));

                        razSetupSoundRamp(gameTrackerX.playerInstance, (SoundRamp *)&Raziel.soundHandle, 48, 100, 100, 100, 100, 0, 3500);
                    }
                }
                else
                {
                    Ptr->instance->constrictAngle = 0;

                    FX_EndConstrict(0, NULL);
                }
            }
        }

        if (Raziel.constrictGoodCircle == 0)
        {
            Ptr->instance->constrictAngle = 0;
        }
    }

    if (((Ptr->instance->object->oflags2 & 0x1000000)) && (!(Ptr->instance->flags & 0x800)) && (gameTrackerX.gameData.asmData.MorphType != 0) && (Raziel.CurrentPlane == 2) && (Ptr->xyDistance < 450) && ((Ptr->zDelta > -64) && (Ptr->zDelta < 64)))
    {
        Raziel.Senses.Flags |= 0x40;
        Raziel.Senses.Portal = Ptr->instance;
    }

    if (((Ptr->instance->object->oflags & 0x100000)) && (Ptr->distance >= 641) && (MATH3D_ConeDetect(&Ptr->relativePosition, 682, 682) != 0))
    {
        SetEngagedInstance(player, Ptr, 7);
    }

    if (((Ptr->instance->object->oflags2 & 0x20)) && (Ptr->distance < 256))
    {
        SetEngagedInstance(player, Ptr, 14);
    }
}

long UpdateAutoFaceAngle(Instance *player, Instance *instance)
{
    long rc;

    rc = MATH3D_FastAtan2(player->position.y - instance->position.y, player->position.x - instance->position.x);

    return (rc + 3072) & 0xFFF;
}
