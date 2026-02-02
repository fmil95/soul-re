#include "Game/PHYSICS.h"
#include "Game/MATH3D.h"
#include "Game/PHYSICS.h"
#include "Game/GAMELOOP.h"
#include "Game/HASM.h"
#include "Game/COLLIDE.h"
#include "Game/STATE.h"
#include "Game/SIGNAL.h"
#include "Game/PHYSOBS.h"
#include "Game/STREAM.h"
#include "Game/INSTANCE.h"
#include "Game/FX.h"
#include "Game/G2/QUATG2.h"
#include "Game/PSX/MAIN.h"

static inline int PHYSICS_FixedMultiplication(short a, short b, long c)
{
    long r;

    r = a * b * c;

    return r / 4096;
}

void SetNoPtCollideInFamily(Instance *instance)
{
    Instance *child;

    child = instance->LinkChild;

    instance->flags |= 0x40;

    while (child != NULL)
    {
        SetNoPtCollideInFamily(child);

        child = child->LinkSibling;
    }
}

void ResetNoPtCollideInFamily(Instance *instance)
{
    Instance *child;

    child = instance->LinkChild;

    instance->flags &= ~0x40;

    while (child != NULL)
    {
        ResetNoPtCollideInFamily(child);

        child = child->LinkSibling;
    }
}

void PHYSICS_CheckLineInWorld(Instance *instance, PCollideInfo *pcollideInfo)
{
    pcollideInfo->collideType = 63;

    PHYSICS_CheckLineInWorldMask(instance, pcollideInfo);
}

void PHYSICS_CheckLineInWorldMask(Instance *instance, PCollideInfo *pcollideInfo)
{
    Level *level;

    level = STREAM_GetLevelWithID(instance->currentStreamUnitID);

    pcollideInfo->inst = NULL;
    pcollideInfo->instance = instance;

    SetNoPtCollideInFamily(instance);

    if (level != NULL)
    {
        COLLIDE_PointAndWorld(pcollideInfo, level);
    }
    else
    {
        pcollideInfo->type = 0;
    }

    ResetNoPtCollideInFamily(instance);
}

int PhysicsCheckLinkedMove(Instance *instance, intptr_t Data, short Mode)
{
    evPhysicsLinkedMoveData *ptr;
    Instance *on;
    MATRIX work;
    MATRIX *current;
    VECTOR delta;
    long onx;
    long ony;
    long onz;
    long ix;
    long iy;
    long iz;

    if (instance->matrix != NULL)
    {
        Instance *oldOn;

        ptr = (evPhysicsLinkedMoveData *)Data;

        on = ptr->instance;

        current = &on->matrix[ptr->segment];

        onx = current->t[0];
        ony = current->t[1];
        onz = current->t[2];

        ix = instance->position.x;
        iy = instance->position.y;
        iz = instance->position.z;

        if ((on->introUniqueID != instance->attachedID) || (ptr->segment != instance->attachedSegment))
        {
            if (instance->attachedID != on->introUniqueID)
            {
                oldOn = INSTANCE_Find(instance->attachedID);

                if (oldOn != NULL)
                {
                    oldOn->flags2 &= ~0x80;
                }
            }

            on->flags2 |= 0x80;

            instance->attachedID = on->introUniqueID;

            instance->attachedSegment = ptr->segment;

            instance->zAccl = 0;

            instance->zVel = 0;

            TransposeMatrix(current, &work);

            delta.vx = ix - onx;
            delta.vy = iy - ony;
            delta.vz = iz - onz;

            ApplyMatrixLV(&work, &delta, (VECTOR *)&instance->relativeMatrix.t[0]);

            MulMatrix0(&work, instance->matrix, &instance->relativeMatrix);

            ApplyMatrixLV(current, (VECTOR *)&instance->relativeMatrix.t[0], &delta);

            ptr->rotDelta.z = 0;
            ptr->rotDelta.y = 0;
            ptr->rotDelta.x = 0;

            ptr->posDelta.x = (short)(ix - (delta.vx + onx));
            ptr->posDelta.y = (short)(iy - (delta.vy + ony));
            ptr->posDelta.z = (short)(iz - (delta.vz + onz));
        }
        else
        {
            G2EulerAngles newRot;
            VECTOR newRelPos;
            long oldPosX;
            long oldPosY;
            long oldPosZ;
            int rotated;
            int moved;

            moved = 0;

            oldPosX = instance->oldPos.x;
            oldPosY = instance->oldPos.y;
            oldPosZ = instance->oldPos.z;

            rotated = 0;

            if ((instance->rotation.x != instance->oldRotation.x) || (instance->rotation.y != instance->oldRotation.y) || (instance->rotation.z != instance->oldRotation.z))
            {
                rotated = 1;
            }

            if ((ix != oldPosX) || (iy != oldPosY) || (iz != oldPosZ))
            {
                moved = 1;
            }

            if ((moved != 0) || (rotated != 0))
            {
                TransposeMatrix(current, &work);

                if (rotated != 0)
                {
                    MulMatrix0(&work, instance->matrix, &instance->relativeMatrix);
                }

                if (moved != 0)
                {
                    delta.vx = ix - oldPosX;
                    delta.vy = iy - oldPosY;
                    delta.vz = iz - oldPosZ;

                    ApplyMatrixLV(&work, &delta, &delta);

                    instance->relativeMatrix.t[0] += delta.vx;
                    instance->relativeMatrix.t[1] += delta.vy;
                    instance->relativeMatrix.t[2] += delta.vz;
                }
            }

            ApplyMatrixLV(current, (VECTOR *)&instance->relativeMatrix.t[0], &newRelPos);

            MulMatrix0(current, &instance->relativeMatrix, &work);

            G2EulerAngles_FromMatrix(&newRot, (G2Matrix *)&work, 21);

            ptr->posDelta.x = (short)((newRelPos.vx + onx) - ix);
            ptr->posDelta.y = (short)((newRelPos.vy + ony) - iy);
            ptr->posDelta.z = (short)((newRelPos.vz + onz) - iz);

            ptr->rotDelta.x = (newRot.x - instance->oldRotation.x);
            ptr->rotDelta.y = (newRot.y - instance->oldRotation.y);
            ptr->rotDelta.z = (newRot.z - instance->oldRotation.z);
        }

        if ((Mode & 0x4))
        {
            PhysicsDefaultLinkedMoveResponse(instance, ptr, (((unsigned)(Mode & 0xFFFF) >> 3) ^ 1) & 0x1);
        }

        if ((Mode & 0x2))
        {
            INSTANCE_Post(instance, 0x4010008, Data);
        }

        if ((ptr->posDelta.x != 0) || (ptr->posDelta.y != 0) || (ptr->posDelta.z != 0) || (ptr->rotDelta.x != 0) || (ptr->rotDelta.y != 0) || (ptr->rotDelta.z != 0))
        {
            instance->flags2 |= 0x8;
        }

        return 65536;
    }

    instance->attachedID = 0;
    return 0;
}

void PhysicsDefaultLinkedMoveResponse(Instance *instance, evPhysicsLinkedMoveData *Data, int updateTransforms)
{
    instance->position.x += Data->posDelta.x;
    instance->position.y += Data->posDelta.y;
    instance->position.z += Data->posDelta.z;

    if (updateTransforms != 0)
    {
        COLLIDE_UpdateAllTransforms(instance, (SVECTOR *)&Data->posDelta);
    }

    instance->rotation.z += Data->rotDelta.z;
}

int PhysicsCheckGravity(Instance *instance, intptr_t Data, short Mode)
{
    evPhysicsGravityData *Ptr;
    SVECTOR D;
    SVECTOR N;
    short Dot;
    int rc;
    PCollideInfo CInfo;
    SVECTOR Old;
    SVECTOR New;
    int slide;
    int stillOnOldTFace;
    Level *level;
    TFace *tface;
    Instance *oldOn;
    // int dx; unused
    // int dy; unused
    // int dz; unused

    D = (SVECTOR){.vx = 0, .vy = 0, .vz = (short)61440};

    Ptr = (evPhysicsGravityData *)Data;

    rc = 0;

    slide = 0;

    CInfo.oldPoint = &Old;
    CInfo.newPoint = &New;

    Old.vx = New.vx = instance->position.x;
    Old.vy = New.vy = instance->position.y;
    Old.vz = New.vz = instance->position.z;

    Old.vz += Ptr->UpperOffset;
    New.vz -= Ptr->LowerOffset;

    if (Old.vz < New.vz)
    {
        CInfo.type = 0;
    }
    else
    {
        if ((instance->cachedTFace != -1) && (instance->cachedTFaceLevel != NULL))
        {
            tface = NULL;

            level = STREAM_GetLevelWithID(((Level *)instance->cachedTFaceLevel)->streamUnitID);

            gameTrackerX.gameFlags |= 0x8000;

            if (level != NULL)
            {
                tface = &level->terrain->faceList[instance->cachedTFace];

                stillOnOldTFace = COLLIDE_PointAndTfaceFunc(level->terrain, &level->terrain->BSPTreeArray[instance->cachedBSPTree], (SVector *)&New, (SVector *)&Old, tface, 0, 0);
            }
            else
            {
                stillOnOldTFace = 0;
            }

            if (stillOnOldTFace == 0)
            {
                instance->waterFace = NULL;

                PHYSICS_CheckLineInWorld(instance, &CInfo);
            }
            else
            {
                CInfo.type = 3;

                CInfo.prim = tface;

                CInfo.inst = (Instance *)level;

                CInfo.segment = instance->cachedBSPTree;

                COLLIDE_GetNormal((short)tface->normal, (short *)level->terrain->normalList, (SVector *)&CInfo.wNormal);
            }
        }
        else
        {
            instance->waterFace = NULL;

            gameTrackerX.gameFlags |= 0x8000;

            PHYSICS_CheckLineInWorld(instance, &CInfo);
        }

        gameTrackerX.gameFlags &= ~0x8000;
    }

    if (((CInfo.type == 2) || (CInfo.type == 3) || (CInfo.type == 5)) && ((CInfo.wNormal.vz < Ptr->slipSlope) && (CInfo.wNormal.vz > 0)))
    {
        N.vx = PHYSICS_FixedMultiplication(CInfo.wNormal.vx, -CInfo.wNormal.vz, -1);
        N.vy = PHYSICS_FixedMultiplication(CInfo.wNormal.vy, -CInfo.wNormal.vz, -1);
        N.vz = PHYSICS_FixedMultiplication(CInfo.wNormal.vz, -CInfo.wNormal.vz, 1);

        N.vz = D.vz - N.vz;

        Dot = (short)((instance->zVel < -48) ? -instance->zVel : 48);

        Old.vx = New.vx = CInfo.newPoint->vx + ((N.vx * Dot) / 4096);
        Old.vy = New.vy = CInfo.newPoint->vy + ((N.vy * Dot) / 4096);
        Old.vz = New.vz = CInfo.newPoint->vz + ((N.vz * Dot) / 4096);

        Old.vz += Ptr->UpperOffset;
        New.vz -= Ptr->LowerOffset;

        PHYSICS_CheckLineInWorld(instance, &CInfo);

        if ((CInfo.type == 2) || (CInfo.type == 3) || (CInfo.type == 5))
        {
            if ((CInfo.wNormal.vz < Ptr->slipSlope) && (CInfo.wNormal.vz > 0))
            {
                slide = 1;
            }
            else
            {
                instance->position.x = CInfo.newPoint->vx;
                instance->position.y = CInfo.newPoint->vy;
            }
        }
    }

    if (CInfo.type == 3)
    {
        instance->cachedBSPTree = CInfo.segment;

        instance->cachedTFace = (TFace *)CInfo.prim - ((Level *)CInfo.inst)->terrain->faceList;

        instance->cachedTFaceLevel = CInfo.inst;
    }
    else
    {
        instance->cachedTFace = -1;

        instance->cachedTFaceLevel = NULL;
    }

    if ((CInfo.type == 2) || (CInfo.type == 3) || (CInfo.type == 5))
    {
        if ((Mode & 0x7))
        {
            if (slide != 0)
            {
                Ptr->x = CInfo.newPoint->vx - instance->position.x;
                Ptr->y = CInfo.newPoint->vy - instance->position.y;
            }
            else
            {
                Ptr->x = 0;
                Ptr->y = 0;
            }

            Ptr->z = CInfo.newPoint->vz - instance->position.z;
        }

        if (((Mode & 0x2)) && (slide == 0))
        {
            INSTANCE_Post(instance, 0x4010008, Data);
        }

        if (slide == 1)
        {
            if ((Mode & 0x2))
            {
                INSTANCE_Post(instance, 0x4010200, Data);
            }

            rc |= 0x100000;
        }

        if (((Mode & 0x4)) && ((CheckPhysOb(instance) == 0) || (CheckPhysObFamily(instance, 1) == 0) || (instance->attachedID == 0)))
        {
            PhysicsDefaultGravityResponse(instance, Ptr);
        }

        if (CInfo.type != 1)
        {
            instance->wNormal.x = CInfo.wNormal.vx;
            instance->wNormal.y = CInfo.wNormal.vy;
            instance->wNormal.z = CInfo.wNormal.vz;
        }
        else
        {
            instance->wNormal.x = 0;
            instance->wNormal.y = 0;
            instance->wNormal.z = 4096;
        }

        if ((CInfo.type != 3) && (CInfo.inst != NULL) && ((CInfo.inst->object->oflags & 0x400)))
        {
            rc |= PhysicsCheckLinkedMove(instance, SetPhysicsLinkedMoveData(CInfo.inst, CInfo.segment, NULL, NULL), Mode);
        }
        else
        {
            if (instance->attachedID != 0)
            {
                oldOn = INSTANCE_Find(instance->attachedID);

                if (oldOn != NULL)
                {
                    oldOn->flags2 &= ~0x80;
                }
            }

            instance->attachedID = 0;
        }

        if ((instance->flags2 & 0x40))
        {
            instance->shadowPosition = instance->position;

            instance->flags |= 0x8000000;
        }

        if (CInfo.type == 3)
        {
            if (instance->tface != CInfo.prim)
            {
                instance->oldTFace = instance->tface;

                instance->tface = (TFace *)CInfo.prim;

                instance->tfaceLevel = CInfo.inst;

                instance->bspTree = CInfo.segment;

                if (CInfo.segment == 0)
                {
                    rc |= 0x80000;
                }
            }
        }
        else if (instance->tface != NULL)
        {
            instance->oldTFace = instance->tface;

            instance->tface = NULL;

            instance->tfaceLevel = NULL;

            instance->bspTree = 0;
        }

        rc |= 0x1;
    }
    else
    {
        if (instance->tface != NULL)
        {
            instance->oldTFace = instance->tface;

            instance->tface = NULL;

            instance->tfaceLevel = NULL;

            instance->bspTree = 0;
        }

        instance->attachedID = 0;

        if ((Mode & 0x2))
        {
            INSTANCE_Post(instance, 0x4000001, Data);
        }
    }

    return rc;
}

static SVECTOR unused = {.vx = 0, .vy = 160, .vz = 0};

void PhysicsDefaultGravityResponse(Instance *instance, evPhysicsGravityData *Data)
{
    instance->position.x += Data->x;
    instance->position.y += Data->y;

    if ((instance == gameTrackerX.playerInstance) && (Data->z > 128))
    {
        instance->position.z += 128;
    }
    else
    {
        instance->position.z += Data->z;
    }
}

int PhysicsCheckEdgeGrabbing(Instance *instance, GameTracker *gameTracker, intptr_t Data, short Mode)
{
    evPhysicsEdgeData *Ptr;
    int rc;
    VECTOR OutTrans;
    SVECTOR *ExtraRot;
    PCollideInfo CInfo;
    SVECTOR Old;
    SVECTOR New;
    static MATRIX TempMat;
    static MATRIX *pTempMat;
    int wallCrawl;
    int freeSpot;
    char temp; // not from decls.h

    (void)gameTracker;

    rc = 0;

    wallCrawl = 0;

    freeSpot = 1;

    Ptr = (evPhysicsEdgeData *)Data;

    if ((Mode & 0x1))
    {
        Ptr->instance = NULL;
    }

    CInfo.oldPoint = &Old;
    CInfo.newPoint = &New;

    ExtraRot = (SVECTOR *)INSTANCE_Query(instance, 7);

    if (ExtraRot != NULL)
    {
        pTempMat = &TempMat;

        RotMatrix(ExtraRot, &TempMat);
    }
    else
    {
        pTempMat = instance->matrix;
    }

    PHYSICS_GenericLineCheckSetup(0, 0, Ptr->UpperOffset + Ptr->AboveOffset, &Old);
    PHYSICS_GenericLineCheckSetup(0, Ptr->ForwardOffset, Ptr->UpperOffset + Ptr->AboveOffset, &New);

    PHYSICS_GenericLineCheck(instance, instance->matrix, pTempMat, &CInfo);

    // garbage code for reordering
    if (freeSpot != 0)
    {
        temp = -temp;
    }

    if (PHYSICS_CheckFaceStick(&CInfo) != 0)
    {
        wallCrawl++;
    }

    if (CInfo.type == 0)
    {
        PHYSICS_GenericLineCheckSetup(0, 0, Ptr->AboveOffset, &Old);
        PHYSICS_GenericLineCheckSetup(0, 0, (Ptr->UpperOffset + Ptr->AboveOffset) + 128, &New);

        PHYSICS_GenericLineCheck(instance, instance->matrix, pTempMat, &CInfo);
    }

    if (CInfo.type != 0)
    {
        freeSpot = 0;
    }

    if ((freeSpot != 0) || (wallCrawl != 0))
    {
        New.vx = 0;
        New.vy = 0;
        New.vz = Ptr->UpperOffset;

        gte_SetRotMatrix(&pTempMat->m[0][0]);
        gte_ldv0(&New.vx);
        gte_nrtv0();
        gte_stlvnl(&OutTrans);

        Old.vx = instance->position.x + (short)OutTrans.vx;
        Old.vy = instance->position.y + (short)OutTrans.vy;
        Old.vz = instance->position.z + (short)OutTrans.vz;

        New.vx = 0;
        New.vy = Ptr->ForwardOffset;
        New.vz = Ptr->UpperOffset;

        gte_SetRotMatrix(&pTempMat->m[0][0]);
        gte_ldv0(&New.vx);
        gte_nrtv0();
        gte_stlvnl(&OutTrans);

        New.vx = instance->position.x + (short)OutTrans.vx;
        New.vy = instance->position.y + (short)OutTrans.vy;
        New.vz = instance->position.z + (short)OutTrans.vz;

        PHYSICS_CheckLineInWorld(instance, &CInfo);

        if (PHYSICS_CheckDontGrabEdge(&CInfo) == 0)
        {
            if (((CInfo.type == 3) || (CInfo.type == 5) || (CInfo.type == 2)) && (CInfo.wNormal.vz < 2048))
            {
                if (PHYSICS_CheckFaceStick(&CInfo) != 0)
                {
                    wallCrawl++;
                }

                if ((Mode & 0x1))
                {
                    Ptr->Normal1->x = CInfo.wNormal.vx;
                    Ptr->Normal1->y = CInfo.wNormal.vy;
                    Ptr->Normal1->z = CInfo.wNormal.vz;

                    Ptr->Delta->y = CInfo.newPoint->vy - CInfo.oldPoint->vy;
                    Ptr->Delta->x = CInfo.newPoint->vx - CInfo.oldPoint->vx;

                    if ((CInfo.type == 5) || (CInfo.type == 2))
                    {
                        Ptr->instance = CInfo.inst;
                    }
                }

                rc |= 0x2;

                Old.vx = 0;
                Old.vy = -16;
                Old.vz = 0;

                gte_SetRotMatrix(&pTempMat->m[0][0]);
                gte_ldv0(&Old.vx);
                gte_nrtv0();
                gte_stlvnl(&OutTrans);

                New.vx = CInfo.newPoint->vx + (short)OutTrans.vx;
                New.vy = CInfo.newPoint->vy + (short)OutTrans.vy;
                New.vz = CInfo.newPoint->vz + (short)OutTrans.vz;

                Old.vx = New.vx;
                Old.vy = New.vy;
                Old.vz = New.vz + Ptr->AboveOffset;

                PHYSICS_CheckLineInWorld(instance, &CInfo);

                if (PHYSICS_CheckDontGrabEdge(&CInfo) == 0)
                {
                    if (((CInfo.type == 3) || (CInfo.type == 5) || (CInfo.type == 2)) && (CInfo.wNormal.vz >= 3548))
                    {
                        if ((Mode & 0x1))
                        {
                            Ptr->Normal2->x = CInfo.wNormal.vx;
                            Ptr->Normal2->y = CInfo.wNormal.vy;
                            Ptr->Normal2->z = CInfo.wNormal.vz;

                            Ptr->Delta->z = CInfo.newPoint->vz - CInfo.oldPoint->vz;

                            if ((CInfo.type == 5) || (CInfo.type == 2))
                            {
                                Ptr->instance = CInfo.inst;
                            }
                        }

                        if (CInfo.wNormal.vz > 2896)
                        {
                            rc |= 0x4;

                            if ((Mode & 0x2))
                            {
                                INSTANCE_Post(instance, 0x4010010, Data);
                            }

                            if ((Mode & 0x4))
                            {
                                PhysicsDefaultEdgeGrabResponse(instance, Ptr, 0);
                            }
                        }
                    }
                }
            }
        }
    }
    else if ((CInfo.type == 3) || (CInfo.type == 5) || (CInfo.type == 2))
    {
        if ((Mode & 0x1))
        {
            Ptr->Normal1->x = CInfo.wNormal.vx;
            Ptr->Normal1->y = CInfo.wNormal.vy;
            Ptr->Normal1->z = CInfo.wNormal.vz;

            Ptr->Delta->y = CInfo.newPoint->vy - CInfo.oldPoint->vy;
            Ptr->Delta->x = CInfo.newPoint->vx - CInfo.oldPoint->vx;
            Ptr->Delta->z = 0;

            if ((CInfo.type == 5) || (CInfo.type == 2))
            {
                Ptr->instance = CInfo.inst;
            }
        }

        rc |= 0x2;
    }

    if (((Mode & 0x2)) && (wallCrawl == 2))
    {
        INSTANCE_Post(instance, 0x4000011, Data);
    }

    return rc;
}

void PhysicsDefaultEdgeGrabResponse(Instance *instance, evPhysicsEdgeData *Data, int blockFlag)
{
    SVector normal;
    G2EulerAngles ea1;
    VECTOR OutTrans;
    MATRIX TempMat;
    SVECTOR New;
    long dp;

    if (blockFlag != 0)
    {
        normal.z = 0;
        normal.y = 0;
        normal.x = 0;

        if (Data->Normal1->y > 512)
        {
            normal.y = 4096;
            ea1.z = 0;
        }
        else if (Data->Normal1->y < -512)
        {
            normal.y = -4096;
            ea1.z = -2048;
        }

        if (Data->Normal1->x > 512)
        {
            normal.x = 4096;
            ea1.z = -1024;
        }
        else if (Data->Normal1->x < -512)
        {
            normal.x = -4096;
            ea1.z = 1024;
        }

        ea1.y = 0;
        ea1.x = 0;

        MATH3D_RotateAxisToVector(&TempMat, instance->matrix, &normal, AXIS_Y);

        instance->rotation.x = 0;
    }
    else
    {
        normal.x = Data->Normal1->x;
        normal.y = Data->Normal1->y;
        normal.z = 0;

        MATH3D_RotateAxisToVector(&TempMat, &instance->matrix[1], &normal, AXIS_Y);

        G2EulerAngles_FromMatrix(&ea1, (G2Matrix *)&TempMat, 0);

        instance->rotation.x = 0;
    }

    instance->rotation.y = 0;
    instance->rotation.z = ea1.z;

    Data->zRot = ea1.z;

    New.vz = Data->ZDistance;
    New.vx = Data->XDistance;
    New.vy = Data->YDistance;

    RotMatrix((SVECTOR *)&instance->rotation, &TempMat);

    gte_SetRotMatrix(&TempMat);
    gte_ldv0(&New);
    gte_nrtv0();
    gte_stlvnl(&OutTrans);

    dp = ((Data->Delta->x * normal.x) + (Data->Delta->y * normal.y) + (Data->Delta->z * normal.z)) >> 12;

    Data->Delta->x = ((int)dp * normal.x) >> 12;
    Data->Delta->y = ((int)dp * normal.y) >> 12;

    instance->position.z -= (short)OutTrans.vz - (Data->UpperOffset + Data->AboveOffset + Data->Delta->z);
    instance->position.x += Data->Delta->x - (short)OutTrans.vx;
    instance->position.y -= (short)OutTrans.vy - Data->Delta->y;
}

int PhysicsCheckSliding(Instance *instance, intptr_t Data, short Mode)
{
    evPhysicsSlideData *Ptr;
    int rc;
    VECTOR OutTrans;
    SVector normal;
    PCollideInfo CInfo;
    SVECTOR Old;
    SVECTOR New;
    static MATRIX *pTempMat;

    Ptr = (evPhysicsSlideData *)Data;

    CInfo.oldPoint = &Old;
    CInfo.newPoint = &New;

    pTempMat = &instance->matrix[Ptr->Segment];

    Old.vx = pTempMat->t[0];
    Old.vy = pTempMat->t[1];
    Old.vz = pTempMat->t[2] + Ptr->Height;

    rc = 0;

    if (instance->tface != NULL)
    {
        COLLIDE_GetNormal(instance->tface->normal, (short *)STREAM_GetLevelWithID(instance->currentStreamUnitID)->terrain->normalList, &normal);
    }
    else
    {
        normal.x = 0;
        normal.y = 0;
        normal.z = 4096;
    }

    Ptr->Dot = (Ptr->ForwardVector.x * normal.x) + (Ptr->ForwardVector.y * normal.y) + (Ptr->ForwardVector.z * normal.z);

    Ptr->Dot /= 4096;

    {
        int Temp;

        Temp = (Ptr->Dot * normal.x) / 4096;

        OutTrans.vx = Ptr->ForwardVector.x - Temp;

        Temp = (Ptr->Dot * normal.y) / 4096;

        OutTrans.vy = Ptr->ForwardVector.y - Temp;

        Temp = (Ptr->Dot * normal.z) / 4096;

        OutTrans.vz = Ptr->ForwardVector.z - Temp;
    }

    New.vx = Old.vx + OutTrans.vx;
    New.vy = Old.vy + OutTrans.vy;
    New.vz = Old.vz + OutTrans.vz;

    PHYSICS_CheckLineInWorld(instance, &CInfo);

    switch (CInfo.type)
    {
    case 3:
    case 5:
        if ((Mode & 0x1))
        {
            Ptr->ForwardNormal.x = CInfo.wNormal.vx;
            Ptr->ForwardNormal.y = CInfo.wNormal.vy;
            Ptr->ForwardNormal.z = CInfo.wNormal.vz;

            Ptr->ForwardVector.x = Old.vx - New.vx;
            Ptr->ForwardVector.y = Old.vy - New.vy;
            Ptr->ForwardVector.z = Old.vz - New.vz;
        }

        New.vx = pTempMat->t[0] + OutTrans.vx;
        New.vy = pTempMat->t[1] + OutTrans.vy;
        New.vz = (pTempMat->t[2] + Ptr->Height) + OutTrans.vz;

        Old.vx = New.vx;
        Old.vy = New.vy;
        Old.vz = New.vz + Ptr->UpperOffset;

        PHYSICS_CheckLineInWorld(instance, &CInfo);

        rc |= 0x2;

        if ((CInfo.type == 3) || (CInfo.type == 5))
        {
            if ((Mode & 0x1))
            {
                Ptr->DropNormal.x = CInfo.wNormal.vx;
                Ptr->DropNormal.y = CInfo.wNormal.vy;
                Ptr->DropNormal.z = CInfo.wNormal.vz;

                Ptr->Height = Old.vz - New.vz;
            }

            rc |= 0x4;
        }

        CInfo.type = 0;
        break;
    default:
        if (CInfo.type == 0)
        {
            Old.vx = pTempMat->t[0] + OutTrans.vx;
            Old.vy = pTempMat->t[1] + OutTrans.vy;
            Old.vz = (pTempMat->t[2] + Ptr->Height) + OutTrans.vz;

            New.vx = Old.vx;
            New.vy = Old.vy;
            New.vz = Old.vz - Ptr->DropOffset;

            PHYSICS_CheckLineInWorld(instance, &CInfo);

            if ((CInfo.type == 3) || (CInfo.type == 5))
            {
                if ((Mode & 0x1))
                {
                    Ptr->DropNormal.x = CInfo.wNormal.vx;
                    Ptr->DropNormal.y = CInfo.wNormal.vy;
                    Ptr->DropNormal.z = CInfo.wNormal.vz;

                    Ptr->Height = Old.vz - New.vz;
                }

                rc |= 0x4000;
            }
        }
    }

    Old.vx = pTempMat->t[0];
    Old.vy = pTempMat->t[1];
    Old.vz = pTempMat->t[2];

    New.vx = Old.vx;
    New.vy = Old.vy;
    New.vz = Old.vz + Ptr->UpperOffset;

    PHYSICS_CheckLineInWorld(instance, &CInfo);

    if (CInfo.type != 0)
    {
        rc |= 0x8000;
    }

    if (CInfo.type == 5)
    {
        if ((Mode & 0x1))
        {
            Ptr->UpperInstance = CInfo.inst;
        }
    }
    else
    {
        Ptr->UpperInstance = NULL;
    }

    return rc;
}

int PhysicsUpdateTface(Instance *instance, intptr_t Data)
{
    PCollideInfo CInfo;
    SVECTOR Old;
    SVECTOR New;

    CInfo.oldPoint = &Old;
    CInfo.newPoint = &New;

    Old.vx = New.vx = instance->position.x;
    Old.vy = New.vy = instance->position.y;
    Old.vz = New.vz = instance->position.z;

    Old.vz = New.vz + ((short *)Data)[0];

    New.vz -= ((short *)Data)[1];

    PHYSICS_CheckLineInWorld(instance, &CInfo);

    if (CInfo.type == 3)
    {
        if (instance->tface != CInfo.prim)
        {
            instance->oldTFace = instance->tface;
            instance->tface = (TFace *)CInfo.prim;
            instance->tfaceLevel = CInfo.inst;
            instance->bspTree = CInfo.segment;
        }

        return 1;
    }

    if (instance->tface != NULL)
    {
        instance->oldTFace = instance->tface;
        instance->tface = NULL;
        instance->tfaceLevel = NULL;
        instance->bspTree = 0;
    }

    return 0;
}

int PhysicsCheckBlockers(Instance *instance, GameTracker *gameTracker, intptr_t Data, short Mode)
{
    evPhysicsEdgeData *Ptr;
    VECTOR OutTrans;
    PCollideInfo CInfo;
    SVECTOR Old;
    SVECTOR New;
    static MATRIX *pTempMat;
    static long unused;
    TFace *tface;
    int Dot;
    SVECTOR Force;
    // HFace *hface; unused
    char temp[8]; // not from decls.h

    (void)gameTracker;
    (void)temp;
    (void)unused;

    Ptr = (evPhysicsEdgeData *)Data;

    Ptr->rc = 0;

    Ptr->instance = NULL;

    CInfo.oldPoint = &Old;
    CInfo.newPoint = &New;

    PHYSICS_GenericLineCheckSetup(0, 0, 0, &Old);
    PHYSICS_GenericLineCheckSetup(0, Ptr->ForwardOffset, 0, &New);

    PHYSICS_GenericLineCheck(instance, &instance->matrix[1], instance->matrix, &CInfo);

    pTempMat = instance->matrix;

    if ((CInfo.type == 3) || (CInfo.type == 5) || (CInfo.type == 2))
    {
        tface = (TFace *)CInfo.prim;

        Force.vy = 0;
        Force.vx = 0;
        Force.vy = -4096;

        ApplyMatrix(pTempMat, &Force, &OutTrans);

        Dot = CInfo.wNormal.vx * OutTrans.vx;

        Dot += CInfo.wNormal.vy * OutTrans.vy;
        Dot += CInfo.wNormal.vz * OutTrans.vz;

        Dot /= 4096;

        if ((CInfo.type == 3) && (tface->textoff != 0xFFFF) && ((((TextureFT3 *)((char *)((Level *)&CInfo.inst->node)->terrain->StartTextureList + tface->textoff))->attr & 0x1000)))
        {
            if (Dot > -3784)
            {
                return Ptr->rc;
            }
        }
        else if (Dot >= -4032)
        {
            return Ptr->rc;
        }

        if ((Mode & 0x1))
        {
            Ptr->Normal1->x = CInfo.wNormal.vx;
            Ptr->Normal1->y = CInfo.wNormal.vy;
            Ptr->Normal1->z = CInfo.wNormal.vz;

            Ptr->Delta->y = CInfo.newPoint->vy;
            Ptr->Delta->x = CInfo.newPoint->vx;
            Ptr->Delta->z = CInfo.newPoint->vz;
        }

        if ((CInfo.type == 3) && (tface->textoff != 0xFFFF) && ((((TextureFT3 *)((char *)((Level *)&CInfo.inst->node)->terrain->StartTextureList + tface->textoff))->attr & 0x1000)))
        {
            Ptr->rc |= 0x20000;
        }

        Ptr->rc |= 0x2;

        Old.vx = 0;
        Old.vy = -16;
        Old.vz = 0;

        ApplyMatrix(pTempMat, &Old, &OutTrans);

        New.vx = CInfo.newPoint->vx + (short)OutTrans.vx;
        New.vy = CInfo.newPoint->vy + (short)OutTrans.vy;
        New.vz = CInfo.newPoint->vz + (short)OutTrans.vz;

        Old.vx = New.vx;
        Old.vy = New.vy;
        Old.vz = New.vz + Ptr->AboveOffset;

        PHYSICS_CheckLineInWorld(instance, &CInfo);

        if ((CInfo.type == 3) || (CInfo.type == 5))
        {
            if ((Mode & 0x1))
            {
                Ptr->Normal2->x = CInfo.wNormal.vx;
                Ptr->Normal2->y = CInfo.wNormal.vy;
                Ptr->Normal2->z = CInfo.wNormal.vz;
            }

            if (CInfo.wNormal.vz > 2896)
            {
                Ptr->rc |= 0x4;
            }
        }

        if ((Mode & 0x2))
        {
            INSTANCE_Post(instance, 0x4010400, Data);
        }
    }

    return Ptr->rc;
}

int PhysicsCheckSwim(Instance *instance, intptr_t Data, short Mode)
{
    evPhysicsSwimData *Ptr;
    int rc;
    int Depth;
    int WaterDepth;
    PCollideInfo CInfo;
    SVECTOR Old;
    SVECTOR New;
    long waterZLevel;

    Ptr = (evPhysicsSwimData *)Data;

    waterZLevel = STREAM_GetWaterZLevel(STREAM_GetLevelWithID(instance->currentStreamUnitID), instance);

    Depth = instance->matrix[1].t[2] - waterZLevel;

    if (waterZLevel == -32767)
    {
        Depth = 1;
        WaterDepth = -32767;
    }
    else
    {
        WaterDepth = -32767;

        if (waterZLevel == 32767)
        {
            Depth = -32767;
        }
        else if (Ptr->CheckDepth != 0)
        {
            CInfo.oldPoint = &Old;
            CInfo.newPoint = &New;

            Old.vx = New.vx = (short)instance->matrix[1].t[0];
            Old.vy = New.vy = (short)instance->matrix[1].t[1];
            Old.vz = New.vz = (short)instance->matrix[1].t[2];

            New.vz -= Ptr->WadeDepth * 4;
            Old.vz += Ptr->TreadDepth;

            gameTrackerX.gameFlags |= 0x8000;

            instance->waterFace = NULL;

            PHYSICS_CheckLineInWorld(instance, &CInfo);

            gameTrackerX.gameFlags &= ~0x8000;

            if (CInfo.type != 0)
            {
                WaterDepth = (Depth + CInfo.newPoint->vz) - instance->matrix[1].t[2];
            }
            else
            {
                WaterDepth = Depth - (Ptr->WadeDepth * 4);
            }
        }
        else
        {
            Ptr->Depth = Depth;
            Ptr->WaterDepth = WaterDepth = instance->matrix->t[2] - waterZLevel;
        }
    }

    if ((Mode & 0x3))
    {
        Ptr->Depth = Depth;
        Ptr->WaterDepth = WaterDepth;
    }

    Ptr->WaterLevel = waterZLevel;

    Ptr->rc = rc = PhysicsDefaultCheckSwimResponse(instance, (evPhysicsSwimData *)Ptr);

    if ((Mode & 0x2))
    {
        INSTANCE_Post(instance, 0x4020000, Data);
    }

    FX_UpdateInstanceWaterSplit(instance);

    return rc;
}

int PhysicsDefaultCheckSwimResponse(Instance *instance, evPhysicsSwimData *Data)
{
    int rc;
    long waterZLevel;

    STREAM_GetLevelWithID(instance->currentStreamUnitID);

    waterZLevel = Data->WaterLevel;

    if (-Data->WadeDepth >= Data->WaterDepth)
    {
        rc = 0;
    }
    else
    {
        rc = 128;
    }

    if ((-Data->TreadDepth < Data->Depth) && (Data->Depth < 0))
    {
        rc |= 0x40;
    }

    if (Data->Depth < -Data->SwimDepth)
    {
        rc |= 0x10;
    }

    if (Data->Depth > 0)
    {
        rc |= 0x20;
    }

    if ((instance->position.z < waterZLevel) && (instance->oldPos.z > waterZLevel) && (Data->iVelocity->z < 0))
    {
        SIGNAL_InWater(instance);

        rc |= 0x100;
    }

    if (((instance->matrix != NULL) && (instance->oldMatrix != NULL)) && (instance->matrix[1].t[2] > waterZLevel) && (instance->oldMatrix[1].t[2] < waterZLevel))
    {
        SIGNAL_OutOfWater(instance);

        rc |= 0x200;
    }

    if ((instance->position.z > waterZLevel) && (instance->oldPos.z < waterZLevel) && (Data->iVelocity->z > 0))
    {
        rc |= 0x400;
    }

    if (((instance->matrix != NULL) && (instance->oldMatrix != NULL)) && (instance->matrix[1].t[2] < (waterZLevel - Data->SwimDepth)) && (instance->oldMatrix[1].t[2] > (waterZLevel - Data->SwimDepth)) && (Data->iVelocity->z < 0))
    {
        rc |= 0x800;
    }

    return rc;
}

void PhysicsForceSetWater(Instance *instance, int *Time, int Depth, int rate, int maxAmplitude)
{
    int Amplitude;

    Amplitude = Depth;

    if (Amplitude < -3072)
    {
        Amplitude = -3072;
    }

    if (Amplitude > 0)
    {
        Amplitude = 0;
    }

    Amplitude = (Amplitude * maxAmplitude) / -3072;

    *Time += (rate << 12) / gameTrackerX.timeMult;

    if (*Time >= 4097)
    {
        *Time -= 4096;
    }

    if (Depth < 0)
    {
        instance->position.z += (Amplitude * rcos(*Time)) / 4096;
    }
}

int PhysicsCheckLOS(Instance *instance, intptr_t Data, int Mode)
{
    PCollideInfo CInfo;

    (void)Mode;
    CInfo.oldPoint = (SVECTOR *)(Data + 8);
    CInfo.newPoint = (SVECTOR *)Data;

    PHYSICS_CheckLineInWorld(instance, &CInfo);

    return CInfo.type == 0;
}

int PhysicsCheckDropHeight(Instance *instance, intptr_t Data, int Mode)
{
    evPhysicsDropHeightData *data;
    Level *level;
    SVECTOR newPos;
    SVECTOR oldPos;
    int rc;
    int lowZ;
    PCollideInfo CInfo;
    static MATRIX TempMat;
    static MATRIX *pTempMat;
    static long unused1;
    static long unused2;

    (void)unused1;
    (void)unused2;

    data = (evPhysicsDropHeightData *)Data;

    if ((data->mode & 0x20))
    {
        VECTOR outTrans;
        SVECTOR *ExtraRot;

        ExtraRot = (SVECTOR *)INSTANCE_Query(instance, 8);

        if (ExtraRot != NULL)
        {
            pTempMat = &TempMat;

            RotMatrix(ExtraRot, &TempMat);

            MulMatrix2(instance->matrix, pTempMat);
        }
        else
        {
            pTempMat = instance->matrix;
        }

        ApplyMatrix(pTempMat, (SVECTOR *)data, &outTrans);

        oldPos.vx = (short)(instance->position.x + outTrans.vx);
        oldPos.vy = (short)(instance->position.y + outTrans.vy);
        oldPos.vz = (short)(instance->position.z + outTrans.vz);
    }
    else if ((data->mode & 0x10))
    {
        oldPos.vx = instance->position.x + data->origin.x;
        oldPos.vy = instance->position.y + data->origin.y;
        oldPos.vz = instance->position.z + data->origin.z;
    }
    else
    {
        oldPos.vx = data->origin.x;
        oldPos.vy = data->origin.y;
        oldPos.vz = data->origin.z;
    }
    {
        long waterZLevel;

        level = STREAM_GetLevelWithID(instance->currentStreamUnitID);

        if (((data->mode & 0x40)) && (waterZLevel = STREAM_GetWaterZLevel(level, instance), (waterZLevel != -32767)))
        {
            lowZ = oldPos.vz - data->DropOffset;

            if (lowZ < waterZLevel)
            {
                lowZ = waterZLevel - 1;

                rc = 16;
            }
            else
            {
                rc = 0;
            }
        }
        else
        {
            rc = 0;

            lowZ = oldPos.vz - data->DropOffset;
        }
    }

    if (lowZ < oldPos.vz)
    {
        newPos.vz = lowZ;

        CInfo.oldPoint = &oldPos;
        CInfo.newPoint = &newPos;

        newPos.vx = oldPos.vx;
        newPos.vy = oldPos.vy;

        PHYSICS_CheckLineInWorld(instance, &CInfo);

        if ((CInfo.type != 0) && (CInfo.wNormal.vz > 3200))
        {
            if (Mode == 1)
            {
                if (CInfo.type == 3)
                {
                    level = (Level *)CInfo.inst;

                    data->bsp = &level->terrain->BSPTreeArray[CInfo.segment];

                    data->tface = (TFace *)CInfo.prim;
                }
                else
                {
                    data->bsp = NULL;

                    data->tface = NULL;
                }

                data->result = oldPos.vz - newPos.vz;

                data->origin.z = newPos.vz;
            }

            rc = 1;
        }
    }

    return rc;
}

int PhysicsCheckDropOff(Instance *instance, intptr_t Data, short Mode)
{
    evPhysicsDropOffData *Ptr;
    int rc;
    VECTOR OutTrans;
    SVECTOR *ExtraRot;
    PCollideInfo CInfo;
    SVECTOR New;
    SVECTOR Old;
    static MATRIX TempMat;
    static MATRIX *pTempMat;

    Ptr = (evPhysicsDropOffData *)Data;

    CInfo.oldPoint = &Old;
    CInfo.newPoint = &New;

    ExtraRot = (SVECTOR *)INSTANCE_Query(instance, 8);

    rc = 0;

    if (instance->matrix != NULL)
    {
        if (ExtraRot != NULL)
        {
            pTempMat = &TempMat;

            RotMatrix(ExtraRot, pTempMat);

            MulMatrix2(instance->matrix, pTempMat);
        }
        else
        {
            pTempMat = instance->matrix;
        }

        New.vx = Ptr->xOffset;
        New.vy = Ptr->yOffset;
        New.vz = 0;

        ApplyMatrix(pTempMat, &New, &OutTrans);

        Old.vx = instance->position.x + (short)OutTrans.vx;
        Old.vy = instance->position.y + (short)OutTrans.vy;
        Old.vz = instance->position.z + (short)OutTrans.vz;

        New.vx = Old.vx;
        New.vy = Old.vy;
        New.vz = Old.vz - Ptr->DropOffset;

        Old.vz += Ptr->UpperOffset;

        PHYSICS_CheckLineInWorld(instance, &CInfo);

        if ((CInfo.type == 2) || (CInfo.type == 3) || (CInfo.type == 5))
        {
            if ((Mode & 0x2))
            {
                if (CInfo.wNormal.vz > Ptr->slipSlope)
                {
                    INSTANCE_Post(instance, 0x4010080, 1);
                }
                else
                {
                    INSTANCE_Post(instance, 0x4010080, 0);

                    rc = 8;
                }
            }

            if ((CInfo.wNormal.vz < Ptr->slipSlope) && (CInfo.wNormal.vz > 0) && (CInfo.newPoint->vz > instance->position.z))
            {
                INSTANCE_Post(instance, 0x4010401, 0);
            }
        }
        else if (CInfo.type == 0)
        {
            if ((Mode & 0x2))
            {
                INSTANCE_Post(instance, 0x4010080, 0);
            }

            rc = 8;
        }
    }

    return rc;
}

int PhysicsFollowWall(Instance *instance, GameTracker *gameTracker, intptr_t Data, short Mode)
{
    VECTOR OutTrans;
    evPhysicsWallCrawlData *Ptr;
    PCollideInfo CInfo;
    SVECTOR New;
    SVECTOR Old;
    static MATRIX *pTempMat;

    (void)gameTracker;

    Ptr = (evPhysicsWallCrawlData *)Data;

    Ptr->rc = 0;

    CInfo.oldPoint = &Old;
    CInfo.newPoint = &New;

    pTempMat = &instance->matrix[Ptr->Segment];

    PHYSICS_GenericLineCheckSetup(0, 64, 0, &Old);
    PHYSICS_GenericLineCheckSetup(0, Ptr->Length, 0, &New);

    PHYSICS_GenericLineCheck(instance, pTempMat, pTempMat, &CInfo);

    if ((CInfo.type == 3) || (CInfo.type == 5))
    {
        instance->shadowPosition.x = New.vx;
        instance->shadowPosition.y = New.vy;
        instance->shadowPosition.z = New.vz;

        if ((instance->flags2 & 0x40))
        {
            instance->flags |= 0x8000000;
        }

        Ptr->rc |= 0x8;

        if ((Mode & 0x7))
        {
            Ptr->DropNormal.x = CInfo.wNormal.vx;
            Ptr->DropNormal.y = CInfo.wNormal.vy;
            Ptr->DropNormal.z = CInfo.wNormal.vz;
        }

        if ((Mode & 0x4))
        {
            Position A;
            Position B;
            MATRIX mat;

            B.x = B.y = B.z = 0;

            A.x = Ptr->DropNormal.x;
            A.y = Ptr->DropNormal.y;
            A.z = Ptr->DropNormal.z;

            MATH3D_RotationFromPosToPos(&A, &B, &Ptr->DropRotation);

            RotMatrix((SVECTOR *)&Ptr->DropRotation, &mat);

            Ptr->NewPosition.x = New.vx;
            Ptr->NewPosition.y = New.vy;
            Ptr->NewPosition.z = New.vz;

            New.vx = 0;
            New.vy = -Ptr->NormalDistance;
            New.vz = 0;

            ApplyMatrix(&mat, &New, &OutTrans);

            Ptr->NewPosition.x += (short)OutTrans.vx;
            Ptr->NewPosition.y += (short)OutTrans.vy;
            Ptr->NewPosition.z += (short)OutTrans.vz;

            Ptr->NewPosition.x = instance->position.x - Ptr->NewPosition.x;
            Ptr->NewPosition.y = instance->position.y - Ptr->NewPosition.y;
            Ptr->NewPosition.z = instance->position.z - Ptr->NewPosition.z;
        }

        instance->wNormal.x = CInfo.wNormal.vx;
        instance->wNormal.y = CInfo.wNormal.vy;
        instance->wNormal.z = CInfo.wNormal.vz;

        instance->oldTFace = instance->tface;

        instance->tface = (TFace *)CInfo.prim;
        instance->tfaceLevel = CInfo.inst;

        instance->bspTree = CInfo.segment;
    }

    PHYSICS_GenericLineCheckSetup(0, 64, Ptr->ForwardOffset, &Old);
    PHYSICS_GenericLineCheckSetup(0, (short)(Ptr->NormalDistance / 2) + Ptr->Length, Ptr->ForwardOffset, &New);

    PHYSICS_GenericLineCheck(instance, pTempMat, pTempMat, &CInfo);

    if ((CInfo.type == 3) || (CInfo.type == 5))
    {
        Ptr->rc |= 0x2;

        if ((Mode & 0x7))
        {
            Ptr->ForwardNormal.x = CInfo.wNormal.vx;
            Ptr->ForwardNormal.y = CInfo.wNormal.vy;
            Ptr->ForwardNormal.z = CInfo.wNormal.vz;
        }

        if ((Mode & 0x4))
        {
            Ptr->ForwardXRotation = MATH3D_AngleBetweenVectors(&Ptr->DropNormal, &Ptr->ForwardNormal);
        }

        Old.vx = 0;
        Old.vy = -32;
        Old.vz = 0;

        ApplyMatrix(pTempMat, &Old, &OutTrans);

        Old.vx = 0;
        Old.vy = 0;
        Old.vz = 64;

        New.vx += (short)OutTrans.vx;
        New.vy += (short)OutTrans.vy;
        New.vz += (short)OutTrans.vz;

        ApplyMatrix(pTempMat, &Old, &OutTrans);

        Old.vx = New.vx + (short)OutTrans.vx;
        Old.vy = New.vy + (short)OutTrans.vy;
        Old.vz = New.vz + (short)OutTrans.vz;

        PHYSICS_CheckLineInWorld(instance, &CInfo);

        if ((CInfo.type == 3) || (CInfo.type == 5))
        {
            Old.vx = 0;
            Old.vy = 0;
            Old.vz = 64;

            ApplyMatrix(pTempMat, &Old, &OutTrans);

            New.vx += (short)OutTrans.vx;
            New.vy += (short)OutTrans.vy;
            New.vz += (short)OutTrans.vz;

            Old.vx = (short)pTempMat->t[0] + (short)OutTrans.vx;
            Old.vy = (short)pTempMat->t[1] + (short)OutTrans.vy;
            Old.vz = (short)pTempMat->t[2] + (short)OutTrans.vz;

            PHYSICS_CheckLineInWorld(instance, &CInfo);

            if (CInfo.type == 0)
            {
                Ptr->rc |= 0x4;
            }
        }
    }

    if ((Mode & 0x2))
    {
        INSTANCE_Post(instance, 0x4010011, Data);
    }

    return Ptr->rc;
}

void PhysicsMoveLocalZClamp(Instance *instance, long segment, long time, long clamp)
{
    Position pos = {0};
    SVECTOR v;
    SVECTOR dv;

    PhysicsMove(instance, &pos, time);

    v.vx = pos.x;
    v.vy = -pos.y;
    v.vz = pos.z;

    ApplyMatrixSV(&instance->matrix[segment], &v, &dv);

    instance->position.x += dv.vx;
    instance->position.y += dv.vy;

    if (clamp == 0)
    {
        instance->position.z += dv.vz;
    }
}

void PhysicsMove(Instance *instance, Position *position, long time)
{
    long xVel;
    long yVel;
    long zVel;
    long xat;
    long yat;
    long zat;

    xVel = instance->xVel;
    yVel = instance->yVel;
    zVel = instance->zVel;

    xat = (instance->xAccl * time) / 4096;
    yat = (instance->yAccl * time) / 4096;
    zat = (instance->zAccl * time) / 4096;

    position->x += ((xVel * time) / 4096) + ((xat * time) / 8192);
    position->y += ((yVel * time) / 4096) + ((yat * time) / 8192);
    position->z += ((zVel * time) / 4096) + ((zat * time) / 8192);

    xVel += xat;
    yVel += yat;
    zVel += zat;

    if (xVel > instance->maxXVel)
    {
        xVel = instance->maxXVel;
    }
    else if (xVel < -instance->maxXVel)
    {
        xVel = -instance->maxXVel;
    }

    if (yVel > instance->maxYVel)
    {
        yVel = instance->maxYVel;
    }
    else if (yVel < -instance->maxYVel)
    {
        yVel = -instance->maxYVel;
    }

    if (zVel > instance->maxZVel)
    {
        zVel = instance->maxZVel;
    }
    else if (zVel < -instance->maxZVel)
    {
        zVel = -instance->maxZVel;
    }

    instance->xVel = xVel;
    instance->yVel = yVel;
    instance->zVel = zVel;
}

void PhysicsSetVelFromZRot(Instance *instance, short angle, long magnitude)
{
    instance->xVel = (rcos(angle - 1024) * magnitude) >> 12;
    instance->yVel = (rsin(angle - 1024) * magnitude) >> 12;
}

void PhysicsSetVelFromRot(Instance *instance, Rotation *rot, long magnitude)
{
    MATRIX mat;
    SVECTOR flatPt;
    SVECTOR newPt;

    (void)rot;
    flatPt.vx = 0;
    flatPt.vy = -magnitude;
    flatPt.vz = 0;

    RotMatrix((SVECTOR *)&instance->rotation, &mat);

    ApplyMatrixSV(&mat, &flatPt, &newPt);

    instance->xVel = newPt.vx;
    instance->yVel = newPt.vy;
    instance->zVel = newPt.vz;
}

void PHYSICS_SetVAndAFromRot(Instance *instance, Rotation *rot, long v, long a)
{
    SVECTOR flatPt;
    MATRIX mat;

    (void)rot;
    if ((v != 0) || (a != 0))
    {
        RotMatrix((SVECTOR *)&instance->rotation, &mat);

        flatPt.vx = 0;
        flatPt.vz = 0;
    }

    if (v != 0)
    {
        SVECTOR newPt;

        flatPt.vy = -v;

        ApplyMatrixSV(&mat, &flatPt, &newPt);

        instance->xVel = newPt.vx;
        instance->yVel = newPt.vy;
        instance->zVel = newPt.vz;
    }
    else
    {
        instance->xVel = 0;
        instance->yVel = 0;
        instance->zVel = 0;
    }

    if (a != 0)
    {
        SVECTOR newPt;

        flatPt.vy = -a;

        ApplyMatrixSV(&mat, &flatPt, &newPt);

        instance->xAccl = newPt.vx;
        instance->yAccl = newPt.vy;
        instance->zAccl = newPt.vz;
    }
    else
    {
        instance->xAccl = 0;
        instance->yAccl = 0;
        instance->zAccl = 0;
    }
}

long PHYSICS_FindAFromDAndT(long d, long t)
{
    if (t != 0)
    {
        t = (d * 8192) / (t * t);

        return t / 4096;
    }

    return 0;
}

long PHYSICS_FindVFromAAndD(long a, long d)
{
    long vsq;

    vsq = a * 2 * d;

    if (vsq != 0)
    {
        return MATH3D_FastSqrt0(vsq);
    }

    return 0;
}

void PHYSICS_StopIfCloseToTarget(Instance *instance, int x, int y, int z)
{
    if (((instance->xAccl < 0) && (instance->xVel <= x)) || ((instance->xAccl > 0) && (instance->xVel >= x)))
    {
        instance->xAccl = 0;
        instance->xVel = x;
    }

    if (((instance->yAccl < 0) && (instance->yVel <= y)) || ((instance->yAccl > 0) && (instance->yVel >= y)))
    {
        instance->yAccl = 0;
        instance->yVel = y;
    }

    if (((instance->zAccl < 0) && (instance->zVel <= z)) || ((instance->zAccl > 0) && (instance->zVel >= z)))
    {
        instance->zAccl = 0;
        instance->zVel = z;
    }
}

int PHYSICS_CheckForTerrainCollide(Instance *instance, SVECTOR *startVec, SVECTOR *endVec, int segment)
{
    PCollideInfo CInfo;
    MATRIX *pTempMat;

    CInfo.oldPoint = startVec;
    CInfo.newPoint = endVec;

    pTempMat = &instance->matrix[segment];

    CInfo.collideType = 1;

    PHYSICS_GenericLineCheckMask(instance, pTempMat, pTempMat, &CInfo);

    return CInfo.type == 3;
}

int PHYSICS_CheckForObjectCollide(Instance *instance, SVECTOR *startVec, SVECTOR *endVec, int segment)
{
    PCollideInfo CInfo;
    MATRIX *pTempMat;

    CInfo.oldPoint = startVec;
    CInfo.newPoint = endVec;

    pTempMat = instance->matrix;

    CInfo.collideType = 62;

    PHYSICS_GenericLineCheckMask(instance, &pTempMat[segment], &pTempMat[segment], &CInfo);

    return CInfo.type != 0;
}

int PHYSICS_CheckForValidMove(Instance *instance, SVECTOR *startVec, SVECTOR *endVec, int segment)
{
    PCollideInfo CInfo;
    MATRIX *pTempMat;
    int rc;

    CInfo.oldPoint = startVec;
    CInfo.newPoint = endVec;

    pTempMat = &instance->matrix[segment];

    PHYSICS_GenericLineCheck(instance, pTempMat, pTempMat, &CInfo);

    rc = PHYSICS_CheckFaceStick(&CInfo) != 0;

    if ((CInfo.type == 2) || (CInfo.type == 3) || (CInfo.type == 5))
    {
        rc += 2;
    }

    return rc;
}

int PHYSICS_CheckFaceStick(PCollideInfo *CInfo)
{
    int rc;
    TFace *tface;
    HFace *hface;
    char temp[8]; // not from decls.h

    (void)hface;
    (void)temp;

    rc = 0;

    if (CInfo->type == 3)
    {
        tface = (TFace *)CInfo->prim;

        if (tface->textoff != 0xFFFF)
        {
            rc = (unsigned int)rc < (unsigned int)(((TextureFT3 *)((char *)((Level *)&CInfo->inst->node)->terrain->StartTextureList + tface->textoff))->attr & 0x200);
        }
    }

    return rc;
}

int PHYSICS_CheckDontGrabEdge(PCollideInfo *CInfo)
{
    int rc;
    TFace *tface;
    BSPTree *bsp;
    HFace *hface;

    rc = 0;

    if (CInfo->type == 3)
    {
        tface = (TFace *)CInfo->prim;

        if (tface->textoff != 0xFFFF)
        {
            hface = (HFace *)((char *)((Terrain *)CInfo->inst->node.prev)->StartTextureList + tface->textoff);

            if ((hface->n0 & (unsigned int)0x80) > (unsigned int)rc)
            {
                rc = 1;
            }
        }

        bsp = &((Terrain *)CInfo->inst->node.prev)->BSPTreeArray[CInfo->segment];

        if ((bsp->flags & 0x8000))
        {
            rc = 1;
        }
    }
    else if (CInfo->type == 2)
    {
        hface = (HFace *)CInfo->prim;

        if ((hface->attr & 0x80))
        {
            rc = 1;
        }
    }
    else if (CInfo->type == 5)
    {
        if ((!(INSTANCE_Query(CInfo->inst, 1) & 0x20)) || ((INSTANCE_Query(CInfo->inst, 3) & 0x1)))
        {
            rc = 0;
        }
        else
        {
            rc = 1;
        }
    }

    return rc;
}

void PHYSICS_GenericLineCheckSetup(short x, short y, short z, SVECTOR *inVec)
{
    inVec->vx = x;
    inVec->vy = y;
    inVec->vz = z;
}

void PHYSICS_GenericLineCheck(Instance *instance, MATRIX *transMat, MATRIX *rotMat, PCollideInfo *cInfo)
{
    cInfo->collideType = 63;

    PHYSICS_GenericLineCheckMask(instance, transMat, rotMat, cInfo);
}

void PHYSICS_GenericLineCheckMask(Instance *instance, MATRIX *transMat, MATRIX *rotMat, PCollideInfo *cInfo)
{
    SVECTOR *startVec;
    SVECTOR *endVec;
    VECTOR outVec;

    startVec = cInfo->oldPoint;
    endVec = cInfo->newPoint;

    gte_SetRotMatrix(&rotMat->m[0][0]);
    gte_ldv0(&startVec->vx);
    gte_nrtv0();
    gte_stlvnl(&outVec);

    startVec->vx = (short)(transMat->t[0] + outVec.vx);
    startVec->vy = (short)(transMat->t[1] + outVec.vy);
    startVec->vz = (short)(transMat->t[2] + outVec.vz);

    gte_SetRotMatrix(&rotMat->m[0][0]);
    gte_ldv0(&endVec->vx);
    gte_nrtv0();
    gte_stlvnl(&outVec);

    endVec->vx = (short)(transMat->t[0] + outVec.vx);
    endVec->vy = (short)(transMat->t[1] + outVec.vy);
    endVec->vz = (short)(transMat->t[2] + outVec.vz);

    PHYSICS_CheckLineInWorldMask(instance, cInfo);
}
