#include "Game/RAZIEL/SPIDER.h"

void StateHandlerWallGrab(CharacterState *In, int CurrentSection, intptr_t Data)
{
    Message *Ptr;
    evPhysicsWallCrawlData *data;

    while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
    {
        switch (Ptr->ID)
        {
        case 0x100001:
            if (CurrentSection == 0)
            {
                Raziel.Mode = 0x4000000;

                SteerSwitchMode(In->CharacterInstance, 7);

                ControlFlag = 0x80A1101;

                In->CharacterInstance->tface = NULL;

                PhysicsMode = 3;

                ResetPhysics(In->CharacterInstance, -16);

                razReaverBladeOff();
            }

            G2EmulationSwitchAnimation(In, CurrentSection, 75, 0, 3, 1);

            PurgeMessageQueue(&In->SectionList[CurrentSection].Event);

            In->CharacterInstance->flags2 &= ~0x40;
            break;
        case 0x8000000:
            if (CurrentSection == 0)
            {
                StateSwitchStateCharacterData(In, StateHandlerWallIdle, 0);
            }

            break;
        case 0x100004:
        case 0x1000001:
        case 0x80000000:
        case 0x80000008:
        case 0x80000020:
            break;
        case 0x4010011:
            data = (evPhysicsWallCrawlData *)Ptr->Data;

            if (data->rc == 0)
            {
                In->CharacterInstance->rotation.x = 0;
                In->CharacterInstance->rotation.y = 0;
                In->CharacterInstance->rotation.z = 0;

                StateSwitchStateData(In, CurrentSection, StateHandlerIdle, SetControlInitIdleData(0, 0, 3));

                In->CharacterInstance->flags2 |= 0x40;
            }
            else if (CurrentSection == 0)
            {
                razSetWallCrawlNodes(In->CharacterInstance, data);
            }

            break;
        default:
            DefaultStateHandler(In, CurrentSection, Data);
        }

        DeMessageQueue(&In->SectionList[CurrentSection].Event);
    }
}

void StateHandlerWallIdle(CharacterState *In, int CurrentSection, intptr_t Data)
{
    int Anim;                    
    Message *Ptr;              
    SVECTOR startVec;            
    SVECTOR endVec;              
    int extraProcess;            
      
    extraProcess = 1;
    
    Anim = G2EmulationQueryAnimation(In, CurrentSection);
    
    while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL) 
    {
        switch (Ptr->ID) 
        {    
        case 0x100001: 
            if (CurrentSection == 2) 
            {
                G2EmulationSwitchAnimation(In, 2, 0, 0, 3, 2);
            } 
            else 
            {
                G2EmulationSwitchAnimation(In, CurrentSection, 76, 0, 3, 2);
            }
            
            ExtraRot->x = 0;
            ExtraRot->y = 0;
            ExtraRot->z = 0;
            
            In->CharacterInstance->tface = In->CharacterInstance->oldTFace = NULL;
            
            In->CharacterInstance->waterFace = NULL;
            
            ControlFlag &= ~0x8001000;
            
            In->CharacterInstance->flags2 &= ~0x40;
            break;
        case 0x1000000:                           
            if (CurrentSection == 0) 
            {
                evMonsterHitData *data;      
                
                data = (evMonsterHitData*)Ptr->Data; 
                
                if ((gameTrackerX.debugFlags2 & 0x800)) 
                {
                    LoseHealth(data->power);
                }
            }
        case 0x80000001:
            Raziel.Mode = 0x8;
            
            ControlFlag |= 0x10;
        
            SetPhysics(In->CharacterInstance, -16, 0, 40, 154);
            
            G2EmulationSwitchAnimation(In, CurrentSection, 35, 0, 0, 1);
            
            StateSwitchStateData(In, CurrentSection, StateHandlerWallDismount, 0);
            
            In->CharacterInstance->yVel = -(In->CharacterInstance->yVel * 2);
            In->CharacterInstance->zVel = 0;
            
            In->CharacterInstance->rotation.x = 0;
            In->CharacterInstance->rotation.y = 0; 
            
            PhysicsMode = 0;
            break;
        case 0x8000001:             
            if ((CurrentSection == 0) && (Anim == 104)) 
            {
                PHYSICS_GenericLineCheckSetup(0, 0, 320, &startVec);
                PHYSICS_GenericLineCheckSetup(0, -320, 320, &endVec);
                
                if (!(PHYSICS_CheckForValidMove(In->CharacterInstance, &startVec, &endVec, 0) & 0x1))
                {
                    G2EmulationSwitchAnimationCharacter(In, 106, 0, 3, 1);
                }
            }
            
            break;
        case 0x8000000:                       
            if (CurrentSection == 2) 
            {
                G2EmulationSwitchAnimation(In, 2, 0, 0, 3, CurrentSection);
                
                G2Anim_SetController_Vector(&In->CharacterInstance->anim, 0, 14, ExtraRot);
            }
            else 
            {
                G2EmulationSwitchAnimation(In, CurrentSection, 76, 0, 3, 2);
            }
            
            break;
        case 0x4010011:       
        {
            evPhysicsWallCrawlData *Data; 
            
            Data = (evPhysicsWallCrawlData*)Ptr->Data;
            
            if (Data->rc == 0) 
            {
                In->CharacterInstance->rotation.x = 0;
                In->CharacterInstance->rotation.y = 0;
                In->CharacterInstance->rotation.z = 0; 
                
                StateSwitchStateData(In, CurrentSection, StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
                
                In->CharacterInstance->flags2 |= 0x40;
                
                razReaverBladeOn();
            } 
            else if (CurrentSection == 0)
            {
                if ((Data->rc & 0x4)) 
                {
                    SteerSwitchMode(In->CharacterInstance, 0);
                    
                    StateSwitchStateCharacterData(In, StateHandlerHang, SetControlInitHangData(NULL, 0, 3));
                    
                    razReaverBladeOn();
                } 
                else
                {
                    razSetWallCrawlNodes(In->CharacterInstance, Data);
                }
            }
            
            break;
        }
        case 0x10000000:                       
            if (CurrentSection == 0) 
            {
                Anim = G2EmulationQueryAnimation(In, 0);
                
                switch (Ptr->Data) 
                {              
                case 0x10000001:          
                {
                    int rc;                      
                        
                    if (Anim != 104) 
                    {
                        PHYSICS_GenericLineCheckSetup(0, 0, 160, &startVec);
                        PHYSICS_GenericLineCheckSetup(0, -320, 160, &endVec);
                        
                        rc = PHYSICS_CheckForValidMove(In->CharacterInstance, &startVec, &endVec, 0);
                        
                        if (((rc & 0x1)) || (rc == 0)) 
                        {
                            razSwitchVAnimCharacterSingle(In->CharacterInstance, 25, NULL, NULL);
                        }
                    }
                    
                    break;
                }
                case 0x10000002:                  
                    if ((Anim != 146) && (razSideMoveSpiderCheck(In->CharacterInstance, -128) == 0)) 
                    {
                        razSwitchVAnimCharacterSingle(In->CharacterInstance, 28, NULL, NULL);
                    }
                    
                    break;
                case 0x10000004:           
                    if ((Anim != 145) && (razSideMoveSpiderCheck(In->CharacterInstance, 128) == 0)) 
                    {
                        razSwitchVAnimCharacterSingle(In->CharacterInstance, 27, NULL, NULL);
                    }
                    
                    break;
                case 0x10000003:                
                {
                    int rc;                     
                    
                    if (Anim != 149) 
                    {
                        PHYSICS_GenericLineCheckSetup(0, 0, -320, &startVec);
                        PHYSICS_GenericLineCheckSetup(0, -320, -320, &endVec);
                        
                        rc = PHYSICS_CheckForValidMove(In->CharacterInstance, &startVec, &endVec, 0);
                        
                        if (((rc & 0x1)) || (rc == 0))
                        {
                            razSwitchVAnimCharacterSingle(In->CharacterInstance, 26, NULL, NULL);
                        }
                    }
                    
                    break;
                }
                }
            }
            
            break;
        case 0x100000:                            
            if (CurrentSection == 0) 
            {
                StateSwitchStateCharacterData(In, StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
                
                In->CharacterInstance->flags2 |= 0x40;
                
                razReaverBladeOn();
            }
            
            extraProcess = 0;
            break;
        case 0:
        case 0x100004:
        case 0x1000001:
        case 0x4000004:
        case 0x80000000:
        case 0x80000008:
        case 0x80000020:
            break;
        default:                                  
            DefaultStateHandler(In, CurrentSection, Data);
            break;
        }
        
        DeMessageQueue(&In->SectionList[CurrentSection].Event);
    }
    
    if (extraProcess != 0)
    {
        if ((!(*PadData & 0x8000000F)) && ((Raziel.passedMask & 0x1))) 
        {
            G2EmulationSwitchAnimationCharacter(In, 106, 0, 3, 1);
            
            Raziel.passedMask = 0;
        }
        
        if (CurrentSection == 0) 
        {
            switch (Anim) 
            {                  
            case 104:                  
            {
                int rc;                    
                    
                PHYSICS_GenericLineCheckSetup(0, 0, 160, &startVec);
                PHYSICS_GenericLineCheckSetup(0, -320, 160, &endVec);
                
                rc = PHYSICS_CheckForValidMove(In->CharacterInstance, &startVec, &endVec, 0);
                
                if ((!(rc & 0x1)) && (rc != 0)) 
                {
                    G2EmulationSwitchAnimationCharacter(In, 106, 0, 3, 1);
                }
                
                break;
            }
            case 146:                            
                if (razSideMoveSpiderCheck(In->CharacterInstance, -128) != 0)
                {
                    G2EmulationSwitchAnimationCharacter(In, 106, 0, 3, 1);
                }
                
                break;
            case 145:                          
                if (razSideMoveSpiderCheck(In->CharacterInstance, 128) != 0)
                {
                    G2EmulationSwitchAnimationCharacter(In, 106, 0, 3, 1);
                }
                
                break;
            case 149:                            
                PHYSICS_GenericLineCheckSetup(0, 0, 0, &startVec);
                PHYSICS_GenericLineCheckSetup(0, 0, -320, &endVec);
                
                if ((PHYSICS_CheckForValidMove(In->CharacterInstance, &startVec, &endVec, 0) != 0) || (In->CharacterInstance->waterFace != NULL)) 
                {
                    EnMessageQueueData(&In->SectionList->Defer, 0x100000, 0);
                    break;
                }
                
                PHYSICS_GenericLineCheckSetup(0, 0, -320, &startVec);
                PHYSICS_GenericLineCheckSetup(0, -320, -320, &endVec);
                
                if (!(PHYSICS_CheckForValidMove(In->CharacterInstance, &startVec, &endVec, 0) & 0x1))
                {
                    G2EmulationSwitchAnimationCharacter(In, 106, 0, 3, 1);
                }
                
                break;
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/Game/RAZIEL/RAZIEL", StateHandlerWallDismount);
