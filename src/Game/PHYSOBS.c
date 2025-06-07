#include "Game/PHYSOBS.h"
#include "Game/MATH3D.h"
#include "Game/PHYSICS.h"
#include "Game/STATE.h"
#include "Game/STREAM.h"
#include "Game/GLYPH.h"
#include "Game/SCRIPT.h"
#include "Game/MEMPACK.h"
#include "Game/MONSTER/MBMISS.h"
#include "Game/G2/ANIMG2.h"
#include "Game/G2/INSTNCG2.h"
#include "Game/GAMELOOP.h"
#include "Game/OBTABLE.h"
#include "Game/INSTANCE.h"
#include "Game/SOUND.h"
#include "Game/FX.h"
#include "Game/COLLIDE.h"
#include "Game/G2/QUATG2.h"
#include "Game/HASM.h"
#include "Game/G2/ANMCTRLR.h"
#include "Game/G2/ANMG2ILF.h"

void PHYSOB_PlayDropSound(Instance *instance)
{
    PhysObProperties *Prop;
    int sound;

    Prop = (PhysObProperties *)instance->data;

    if (Prop->family == 1)
    {
        sound = (short)((PhysObInteractProperties *)Prop)->auxConditions; // TODO: likely wrong, physob type should be different
    }
    else if (Prop->family == 0)
    {
        sound = (char)((PhysObInteractProperties *)Prop)->razielAuxAnim; // TODO: likely wrong, physob type should be different
    }
    else if (Prop->family == 3)
    {
        sound = 0;

        if (((PhysObInteractProperties *)Prop)->weapon != NULL)
        {
            sound = ((PhysObInteractProperties *)Prop)->weapon->dropSound;
        }
    }
    else
    {
        sound = 0;
    }

    switch (sound)
    {
    case 1:
        SOUND_Play3dSound(&instance->position, 10, -100, 120, 30000);
        break;
    case 2:
        SOUND_Play3dSound(&instance->position, 282, -400, 90, 14000);
        SOUND_Play3dSound(&instance->position, 282, -800, 100, 14000);
        break;
    case 3:
        SOUND_Play3dSound(&instance->position, 178, -800, 120, 30000);
        break;
    }
}

Instance *PHYSOBS_IsAPushBlockAttached(Instance *block)
{
    Instance *instance;
    Instance *next;
    Instance *result;

    result = NULL;

    for (instance = gameTrackerX.instanceList->first; instance != NULL; instance = next)
    {
        next = instance->next;

        if ((instance->attachedID == block->introUniqueID) && (CheckPhysObAbility(instance, 2) != 0))
        {
            result = instance;
            break;
        }
    }

    return result;
}

Instance *PHYSOBS_IsAnythingAttached(Instance *block)
{
    Instance *instance;
    Instance *next;
    Instance *result;

    result = NULL;

    for (instance = gameTrackerX.instanceList->first; instance != NULL; instance = next)
    {
        next = instance->next;

        if (instance->attachedID == block->introUniqueID)
        {
            result = instance;
            break;
        }
    }

    return result;
}

int CheckPhysOb(Instance *instance)
{
    PhysObProperties *Prop;

    Prop = (PhysObProperties *)instance->data;

    if (Prop != NULL)
    {
        return (Prop->ID ^ 0xB00B) == 0;
    }

    return 0;
}

int CheckPhysObAbility(Instance *instance, unsigned short ability)
{
    PhysObProperties *prop;
    PhysObInteractProperties *temp; // not from decls.h

    prop = (PhysObProperties *)instance->data;

    if (prop != NULL)
    {
        if (prop->ID != 0xB00B)
        {
            return 0;
        }
        else if (prop->family == 3)
        {
            temp = (PhysObInteractProperties *)instance->extraData;

            if (((short)temp->Properties.ID & (ability & 0xFFFF)))
            {
                return 1;
            }
        }
        else if ((prop->Type & ability))
        {
            return 1;
        }
    }

    return 0;
}

int CheckPhysObFamily(Instance *instance, unsigned short family)
{
    PhysObProperties *Prop;

    Prop = (PhysObProperties *)instance->data;

    if (Prop != NULL)
    {
        if (Prop->ID != 0xB00B)
        {
            return 0;
        }

        if (Prop->family == family)
        {
            return 1;
        }
    }

    return 0;
}

int GetPhysicalAbility(Instance *instance)
{
    PhysObProperties *Prop;
    PhysObInteractProperties *temp; // not from decls.h

    Prop = (PhysObProperties *)instance->data;

    if ((Prop != NULL) && (Prop->ID == 0xB00B))
    {
        if (Prop->family == 3)
        {
            temp = (PhysObInteractProperties *)instance->extraData;

            return (short)temp->Properties.ID;
        }

        return Prop->Type;
    }

    return 0;
}

int AnyBlocksInMotion()
{
    Instance *instance;

    for (instance = gameTrackerX.instanceList->first; instance != NULL; instance = instance->next)
    {
        if ((instance->object->oflags2 & 0x40000))
        {
            PhysObProperties *Prop;
            PhysObData *Data;

            Prop = (PhysObProperties *)instance->data;

            if (((Prop->Type & 0x8)) && (Prop->ID == 0xB00B))
            {
                Data = (PhysObData *)instance->extraData;

                if ((Data->Mode & 0x14E))
                {
                    return 1;
                }
            }
        }
    }

    return 0;
}

void SetThrowDirection(Instance *instance, Instance *parent, evObjectThrowData *throwData, PhysObData *Data)
{
    long val;
    Instance *itarget;
    MATRIX *matrix;

    switch (throwData->type)
    {
    case 0:
        if (parent->LinkParent != NULL)
        {
            PhysicsSetVelFromZRot(instance, parent->LinkParent->rotation.z, throwData->speed);
        }
        else
        {
            PhysicsSetVelFromZRot(instance, parent->rotation.z, throwData->speed);
        }

        instance->zVel = throwData->zVel;
        break;
    case 1:
        itarget = throwData->data.target;

        Data->Force = itarget;

        PhysicsSetVelFromZRot(instance, MATH3D_AngleFromPosToPos(&instance->position, &itarget->position), throwData->speed);

        instance->zVel = 0;

        matrix = (MATRIX *)INSTANCE_Query(itarget, 12);

        if (ABS(instance->xVel) > ABS(instance->yVel))
        {
            val = matrix->t[0] - instance->position.x;

            if ((instance->xVel != 0) && (val != 0))
            {
                instance->zVel = (((matrix->t[2] - instance->position.z) * instance->xVel) / val) - (((instance->zAccl * val) / instance->xVel) >> 1);
            }
        }
        else
        {
            val = matrix->t[1] - instance->position.y;

            if ((instance->yVel != 0) && (val != 0))
            {
                instance->zVel = (((matrix->t[2] - instance->position.z) * instance->yVel) / val) - (((instance->zAccl * val) / instance->yVel) >> 1);
            }
        }

        break;
    case 2:
    {
        long val;
        Position *ptarget;

        ptarget = &throwData->data.position;

        PhysicsSetVelFromZRot(instance, MATH3D_AngleFromPosToPos(&instance->position, ptarget), throwData->speed);

        instance->zVel = 0;

        if (ABS(instance->xVel) > ABS(instance->yVel))
        {
            val = ptarget->x - instance->position.x;

            if ((instance->xVel != 0) && (val != 0))
            {
                instance->zVel = (((ptarget->z - instance->position.z) * instance->xVel) / val) - (((instance->zAccl * val) / instance->xVel) >> 1);
            }
        }
        else
        {
            val = ptarget->y - instance->position.y;

            if ((instance->yVel != 0) && (val != 0))
            {
                instance->zVel = (((ptarget->z - instance->position.z) * instance->yVel) / val) - (((instance->zAccl * val) / instance->yVel) >> 1);
            }
        }

        break;
    }
    case 3:
        PhysicsSetVelFromRot(instance, &throwData->data.direction, throwData->speed);
        break;
    case 4:
        instance->xVel = (short)throwData->data.throwVector.x;
        instance->yVel = (short)throwData->data.throwVector.y;
        instance->zVel = (short)throwData->data.throwVector.z;

        instance->zAccl = (short)throwData->gravity;
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/Game/PHYSOBS", ThrowPhysOb);

int PushPhysOb(Instance *instance, short x, short y, short PathNumber, Instance *Force)
{
    PhysObData *Data;
    PhysObProperties *Prop;
    int result;

    Prop = (PhysObProperties *)instance->data;

    result = 0;

    if (((Prop->Type & 0x8)) && (Prop->ID == 0xB00B))
    {
        Data = (PhysObData *)instance->extraData;

        if ((!(Data->Mode & 0x214A)) && (AnyBlocksInMotion() == 0))
        {
            PHYSOB_NormalToAxis(&x, &y);

            Data->Force = Force;

            Data->PathNumber = PathNumber;

            Data->xForce = x;
            Data->yForce = y;

            Data->Mode &= ~0x8;

            instance->zVel = 0;
            instance->yVel = 0;
            instance->xVel = 0;

            instance->zAccl = 0;
            instance->yAccl = 0;
            instance->xAccl = 0;

            result = PHYSOBS_CheckForValidMove(instance);

            if ((result & 0x1))
            {
                if (!(Data->Mode & 0x100))
                {
                    Data->px = instance->position.x;
                    Data->py = instance->position.y;
                    Data->pz = instance->position.z;
                }

                Data->Mode |= 0x1000;

                instance->flags2 |= 0x8;
            }
        }
    }

    return result;
}

void ResetSwitchPhysOb(Instance *instance)
{
    PhysObData *Data;
    PhysObSwitchProperties *Prop;
    PhysObSwitchProperties *temp; // not from decls.h

    if (CheckPhysObFamily(instance, 2) != 0)
    {
        Data = (PhysObData *)instance->extraData;

        Prop = (PhysObSwitchProperties *)instance->data;

        Data->Mode |= 0x800;

        temp = (PhysObSwitchProperties *)instance->extraData;

        if (((temp->Properties.ID & 0x2)) && (Prop->shutAnim != 0))
        {
            G2EmulationInstanceSetAnimation(instance, 0, Prop->shutAnim, 0, 0);
            G2EmulationInstanceSetMode(instance, 0, 1);
        }
    }
}

int SwitchPhysOb(Instance *instance)
{
    PhysObData *Data;
    PhysObSwitchProperties *Prop;
    SwitchData *switchData;
    PhysObSwitchProperties *temp; // not from decls.h

    temp = (PhysObSwitchProperties *)instance->data;

    if (CheckPhysObFamily(instance, 2) != 0)
    {
        Data = (PhysObData *)instance->extraData;

        Data->Mode |= 0x800;

        Prop = (PhysObSwitchProperties *)instance->extraData;

        switchData = (SwitchData *)&Prop->Properties.ID; // TODO: this is probably wrong

        if ((Prop->Properties.ID & 0x1))
        {
            if ((Prop->Properties.ID & 0x2))
            {
                Prop->Properties.ID = (Prop->Properties.ID & ~0x1) | 0x8;

                G2EmulationInstanceSetAnimation(instance, 0, temp->offAnim, 0, 0);
                G2EmulationInstanceSetMode(instance, 0, 1);

                instance->rotation.z += switchData->accumulator;

                if (temp->Class == 7)
                {
                    switchData->accumulator = 1024;
                }
                else
                {
                    switchData->accumulator = 0;
                }

                instance->flags |= 0x8;
            }
            else if (temp->enableAnim != 255)
            {
                G2EmulationInstanceSetAnimation(instance, 0, temp->enableAnim, 0, 0);
                G2EmulationInstanceSetMode(instance, 0, 1);

                Prop->Properties.ID |= 0x2;
            }
        }
        else
        {
            if ((Prop->Properties.ID & 0x2))
            {
                Prop->Properties.ID |= 0x5;

                G2EmulationInstanceSetAnimation(instance, 0, temp->onAnim, 0, 0);

                if (temp->Class == 7)
                {
                    switchData->accumulator = 1024;
                }
                else
                {
                    switchData->accumulator = 0;
                }

                instance->rotation.z += switchData->accumulator;

                instance->flags |= 0x8;
            }
            else if (temp->failedOnAnim != 255)
            {
                G2EmulationInstanceSetAnimation(instance, 0, temp->failedOnAnim, 0, 0);
            }

            G2EmulationInstanceSetMode(instance, 0, 1);
        }

        return 0;
    }

    return 1;
}

INCLUDE_ASM("asm/nonmatchings/Game/PHYSOBS", InteractPhysOb);

void ResetOrientation(Instance *instance)
{
    G2EulerAngles ea;
    PhysObData *Data;
    G2SVector3 vec;
    G2SVector3 vec2;
    int dp;
    int fixxy;
    int fixz;
    int dx;
    int dx2;
    int dy;
    int dy2;
    int dz;
    int dz2;
    short temp; // not from decls.h

    G2Anim_DisableController(&instance->anim, 0, 76);

    Data = (PhysObData *)instance->extraData;

    fixxy = 0;
    fixz = 0;

    dp = instance->position.x - Data->px;

    if (dp != 0)
    {
        if (dp > 0)
        {
            dp = ((dp + 320) / 640) * 640;
        }
        else
        {
            dp = ((dp - 320) / 640) * 640;
        }

        instance->position.x = dp + Data->px;
    }

    dp = instance->position.y - Data->py;

    if (dp != 0)
    {
        if (dp > 0)
        {
            dp -= fixxy;

            dp = ((dp + 320) / 640) * 640;

            dp += fixxy;
        }
        else
        {
            dp += fixxy;

            dp = ((dp - 320) / 640) * 640;

            dp -= fixxy;
        }

        instance->position.y = dp + Data->py;
    }

    dp = instance->position.z - Data->pz;

    if (dp != 0)
    {
        if (dp >= 0)
        {
            dp -= fixz;

            dp = ((dp + 80) / 160) * 160;

            dp += fixz;
        }
        else
        {
            dp += fixz;

            dp = ((dp - 80) / 160) * 160;

            dp -= fixz;
        }

        instance->position.z = dp + Data->pz;
    }

    Data->px = instance->position.x;
    Data->py = instance->position.y;
    Data->pz = instance->position.z;

    G2EulerAngles_FromMatrix(&ea, (G2Matrix *)&instance->matrix[2], 21);

    if (ea.x < 0)
    {
        temp = ((ea.x - 512) / 1024) * 1024;
    }
    else
    {
        temp = ((ea.x + 512) / 1024) * 1024;
    }

    vec2.x = temp;

    if (ea.y < 0)
    {
        temp = ((ea.y - 512) / 1024) * 1024;
    }
    else
    {
        temp = ((ea.y + 512) / 1024) * 1024;
    }

    vec2.y = temp;

    if (ea.z < 0)
    {
        temp = ((ea.z - 512) / 1024) * 1024;
    }
    else
    {
        temp = ((ea.z + 512) / 1024) * 1024;
    }

    vec2.z = temp;

    SUB_SVEC(G2SVector3, &vec, G2EulerAngles, &ea, G2SVector3, &vec2);

    vec.x = (((vec.x < 0) ? vec.x - 150 : vec.x + 150) / 301) * 301;
    vec.y = (vec.y < 0) ? ((vec.y - 150) / 301) * 301 : ((vec.y + 150) / 301) * 301;
    vec.z = (vec.z < 0) ? ((vec.z - 150) / 301) * 301 : ((vec.z + 150) / 301) * 301;

    ADD_SVEC(G2SVector3, &vec, G2SVector3, &vec, G2SVector3, &vec2);

    dx = vec.x - ea.x;
    dx2 = vec2.x - ea.x;

    dx = abs(dx);
    dx2 = abs(dx2);

    if (dx > dx2)
    {
        vec.x = vec2.x;
    }

    dy = vec.y - ea.y;
    dy2 = vec2.y - ea.y;

    dy = abs(dy);
    dy2 = abs(dy2);

    if (dy > dy2)
    {
        vec.y = vec2.y;
    }

    dz = vec.z - ea.z;
    dz2 = vec2.z - ea.z;

    dz = abs(dz);
    dz2 = abs(dz2);

    if (dz > dz2)
    {
        vec.z = vec2.z;
    }

    G2Anim_SetController_Vector(&instance->anim, 2, 14, &vec);

    G2Instance_RebuildTransforms(instance);

    G2Anim_SwitchToKeylist(&instance->anim, *instance->object->animList, 0);
}

void FinishPush(Instance *instance)
{
    ResetOrientation(instance);
}

void PhysOb_AlignPush(Instance *instance, int x, int y, int path, PhysObData *Data)
{
    G2SVector3 vec;
    int rotZ;
    short temp[3][3];
    short temp2[3][3];
    G2EulerAngles ea;

    G2Anim_EnableController(&instance->anim, 0, 76);
    G2Anim_EnableController(&instance->anim, 2, 14);

    Data->xForce = x;
    Data->yForce = y;

    Data->PathNumber = path;

    Data->Mode &= ~0x100000;

    rotZ = 2048;

    if (y <= 0)
    {
        rotZ = 0;

        if (y >= 0)
        {
            rotZ = 1024;

            if (x < 0)
            {
                rotZ = -1024;
            }
        }
    }

    vec.x = 0;
    vec.y = 0;
    vec.z = rotZ;

    G2Anim_SetController_Vector(&instance->anim, 0, 76, &vec);

    vec.z = -rotZ;

    RotMatrixZYX((SVECTOR *)&vec, (MATRIX *)&temp);

    MulMatrix0((MATRIX *)&temp, &instance->matrix[2], (MATRIX *)&temp2);

    G2EulerAngles_FromMatrix(&ea, (G2Matrix *)&temp2, 21);

    vec.x = ea.x;
    vec.y = ea.y;
    vec.z = ea.z;

    G2Anim_SetController_Vector(&instance->anim, 2, 14, &vec);

    G2Anim_SwitchToKeylist(&instance->anim, instance->object->animList[path], path);
}

int FlipPhysOb(Instance *instance, short x, short y, Instance *Force)
{
    PhysObData *Data;
    PhysObProperties *Prop;
    int result;

    result = 0;

    Prop = (PhysObProperties *)instance->data;

    if (((Prop->Type & 0x2)) && (PHYSOBS_IsAPushBlockAttached(instance) == NULL))
    {
        Data = (PhysObData *)instance->extraData;

        if ((!(Data->Mode & 0x2000)) && (AnyBlocksInMotion() == 0))
        {
            PHYSOB_NormalToAxis(&x, &y);

            Data->PathNumber = 1;

            Data->Force = Force;

            Data->Mode |= 0x8;

            Data->xForce = x;
            Data->yForce = y;

            instance->zVel = 0;
            instance->yVel = 0;
            instance->xVel = 0;

            instance->zAccl = 0;
            instance->yAccl = 0;
            instance->xAccl = 0;

            ResetOrientation(instance);

            result = PHYSOBS_CheckForValidMove(instance);

            if ((result & 0x1))
            {
                Data->px = instance->position.x;
                Data->py = instance->position.y;
                Data->pz = instance->position.z;

                Data->Mode |= 0x1000;

                instance->flags2 |= 0x8;
            }
        }
    }

    return result;
}

int CanBePickedUp(Instance *instance, Instance *Force, int LinkNode)
{
    PCollideInfo pcollideInfo;
    Position newPos;
    Position oldPos;

    if (Force == NULL)
    {
        return 0;
    }
    else if ((Force->object->oflags2 & 0x80000))
    {
        return 1;
    }
    else if (Force->matrix == NULL)
    {
        return 1;
    }

    if (CheckPhysObAbility(instance, 1) != 0)
    {
        if (instance->matrix != NULL)
        {
            oldPos.x = (short)instance->matrix[2].t[0];
            oldPos.y = (short)instance->matrix[2].t[1];
            oldPos.z = (short)instance->matrix[2].t[2];

            newPos.x = (short)Force->matrix[LinkNode].t[0];
            newPos.y = (short)Force->matrix[LinkNode].t[1];
            newPos.z = (short)Force->matrix[LinkNode].t[2];

            pcollideInfo.newPoint = (SVECTOR *)&newPos;
            pcollideInfo.oldPoint = (SVECTOR *)&oldPos;

            if ((CheckPhysObAbility(instance, 32) != 0) && (newPos.z < oldPos.z))
            {
                return 1;
            }

            PHYSICS_CheckLineInWorld(instance, &pcollideInfo);

            if (((pcollideInfo.type < 0) || (pcollideInfo.type >= 2)) && (MATH3D_LengthXYZ(newPos.x - oldPos.x, newPos.y - oldPos.y, newPos.z - oldPos.z) >= 21))
            {
                return 0;
            }
        }

        return 1;
    }

    return 0;
}

int PickUpPhysOb(Instance *instance, short Steps, Instance *Force, int LinkNode)
{
    PhysObData *Data;

    Data = (PhysObData *)instance->extraData;

    if (instance->LinkParent == NULL)
    {
        if ((Data->Mode & 0x2000000))
        {
            return 1;
        }

        TurnOffCollisionPhysOb(instance, 7);

        if (CanBePickedUp(instance, Force, LinkNode) != 0)
        {
            Data->Steps = Steps;
            Data->Step = 0;

            Data->Force = Force;

            Data->LinkNode = LinkNode;

            Data->Mode = (Data->Mode & ~0x600000) | 0x4000;

            if ((CheckPhysObAbility(instance, 32) != 0) && ((Data->Mode & 0x10000)))
            {
                PhysObLight *pLight;

                pLight = PhysObGetLight(instance);

                if (pLight != NULL)
                {
                    PHYSOB_StartLighting(instance, pLight);
                }
            }

            return 0;
        }
    }

    return 1;
}

Instance *PHYSOB_BirthCollectible(Instance *parent, int x, int y, int z, int type, int lifeTime)
{
    Object *object;
    Instance *instance;

    instance = NULL;

    if (type != 0)
    {
        object = (Object *)objectAccess[26].object;
    }
    else
    {
        object = (Object *)objectAccess[25].object;
    }

    if (object != NULL)
    {
        instance = INSTANCE_BirthObject(parent, object, 0);

        if (instance != NULL)
        {
            ((Signal *)instance->extraData)->data.introFX.time = lifeTime * 122880; // TODO: double-check whether lvalue is accessing the right union

            instance->fadeValue = 4096;

            instance->position.x = x;
            instance->position.y = y;
            instance->position.z = z;
        }
    }

    return instance;
}

evObjectBirthProjectileData *PHYSOB_BirthProjectile(Instance *parent, int joint, int type)
{
    evObjectBirthProjectileData *rc;

    rc = (evObjectBirthProjectileData *)SetObjectBirthProjectileData(parent, joint, type);

    rc->birthInstance = BirthProjectilePhysOb(parent, joint, type);

    return rc;
}

struct _Instance *BirthProjectilePhysOb(struct _Instance *instance, int grabJoint, int type)
{
    struct Object *ForceOb; // $a1
    struct _Instance *iForce; // $s0
    struct PhysObData *Data; // $s2
    struct PhysObProperties *Prop; // $s1

    ForceOb = (struct Object *)objectAccess[19].object;

    iForce = NULL;

    if (ForceOb != 0)
    {
        iForce = INSTANCE_BirthObject(instance, ForceOb, 0);
        if (iForce != NULL)
        {
            struct __PhysObProjectileProperties *ProjProp; // $v1 line 15
            struct __PhysObProjectileData *ProjData; // $v1 line 15
            struct evObjectBirthProjectileData *temp;

            temp = iForce->extraData;

            ProjProp = iForce->data;

            temp->joint = type;

            ProjData = &ProjProp->data[type];

            iForce->currentModel = ProjData->model;

            Data = iForce->extraData;

            if (ProjData->model == 0)
            {
                struct _Position offset; // stack offset -32 line 30
                struct _FXForceFieldEffect *field; // $v1 line 30
                offset.x = 0;
                offset.y = 0;
                offset.z = 0;
                field = FX_StartFField(iForce, 0x78, &offset, 0x20, 0x10, 0x80, 0x202020);
                if (field != NULL)
                {
                    field->type = 1;
                }
            }
            INSTANCE_LinkToParent(iForce, instance, grabJoint);
            TurnOffCollisionPhysOb(iForce, 7);
            Prop = iForce->data;
            if (Prop->family == 7)
            {
                struct __PhysObProjectileData *ProjData; // $v1 line 51
                struct evObjectBirthProjectileData *temp;

                temp = ((struct evObjectBirthProjectileData *)iForce->extraData);
                ProjProp = iForce->data;
                ProjData = &ProjProp->data[temp->joint];
                if (ProjData->startAnim != -1)
                {
                    G2EmulationInstanceSetAnimation(iForce, 0, ProjData->startAnim, 0, 0);
                    G2EmulationInstanceSetMode(iForce, 0, 2);
                }
            }
            if (Prop->Type & 1)
            {
                Data->Mode = 0x1080;
                Data->Force = NULL;
                Data->Step = 0;
                Data->Steps = 0;
                iForce->xVel = 0;
                iForce->yVel = 0;
                iForce->zVel = 0;
                iForce->xAccl = 0;
                iForce->yAccl = 0;
                iForce->zAccl = 0;
                return iForce;
            }
            Data->physObTimer = 0x96000;
        }
    }
    return iForce;
}

void PHYSOB_SetLightTable(PhysObLight *pLight, LightInstance *li, short burnAmplitude)
{
    long idx;
    long frac;
    LightTableEntry *ltable;
    LightTableEntry *ltable2;
    LightTableEntry *temp; // not from decls.h

    temp = pLight->lightTable;

    idx = (unsigned short)gameTrackerX.vblCount;

    idx = (idx * pLight->speed) % (pLight->length << 12);

    frac = idx & 0xFFF;

    idx >>= 12;

    ltable = &temp[idx++];

    if (idx == pLight->length)
    {
        idx = 0;
    }

    li->segment = pLight->segment;

    ltable2 = &temp[idx];

    li->radius = ((short)(ltable->radius + (((ltable2->radius - ltable->radius) * frac) >> 12)) * burnAmplitude) >> 12;

    if ((li->radius << 16) <= 0)
    {
        li->radius = 1;
    }

    li->r = (short)(((ltable->r << 12) + ((ltable2->r - ltable->r) * frac)) / li->radius);
    li->g = (short)(((ltable->g << 12) + ((ltable2->g - ltable->g) * frac)) / li->radius);
    li->b = (short)(((ltable->b << 12) + ((ltable2->b - ltable->b) * frac)) / li->radius);
}

void PHYSOB_EndLighting(Instance *instance, PhysObLight *pLight)
{
    if ((pLight != NULL) && (gameTrackerX.gameData.asmData.lightInstances[0].lightInstance == instance))
    {
        gameTrackerX.gameData.asmData.lightInstances[0].lightInstance = NULL;
    }
}

void PHYSOB_StartLighting(Instance *instance, PhysObLight *pLight)
{
    PhysObData *Data;

    Data = (PhysObData *)instance->extraData;

    if (Data->burnAmpl < 0)
    {
        Data->burnAmpl = -Data->burnAmpl;
    }

    Data->Mode |= 0x18000;

    if (pLight != NULL)
    {
        LightInstance *li;

        gameTrackerX.gameData.asmData.lightInstances[0].lightInstance = instance;

        li = gameTrackerX.gameData.asmData.lightInstances;

        if (li != NULL)
        {
            PHYSOB_SetLightTable(pLight, li, Data->burnAmpl);
        }
    }
}

void PHYSOB_StartBurnFX(Instance *instance)
{
    PhysObData *Data;
    Object *object;
    int i;

    object = instance->object;

    Data = (PhysObData *)instance->extraData;

    if ((Data->burnAmpl == 0) && (object->numberOfEffects != 0))
    {
        for (i = 0; i < object->numberOfEffects; i++)
        {
            FX_StartInstanceEffect(instance, &object->effectList[i], 1);
        }
    }
}

void PHYSOB_StartBurning(Instance *instance, PhysObLight *pLight)
{
    PHYSOB_StartBurnFX(instance);
    PHYSOB_StartLighting(instance, pLight);
}

void PHYSOB_StopLighting(Instance *instance, PhysObLight *pLight)
{
    PhysObData *Data;

    (void)pLight;

    Data = (PhysObData *)instance->extraData;

    Data->burnAmpl = -4096;

    Data->Mode |= 0x8000;
}

void PHYSOB_StopBurning(Instance *instance, PhysObLight *pLight)
{
    PhysObData *Data;

    (void)pLight;

    Data = (PhysObData *)instance->extraData;

    Data->burnAmpl = -4096;

    Data->Mode |= 0x48000;
}

void PHYSOB_EndBurning(Instance *instance, PhysObLight *pLight)
{
    PhysObData *Data;

    Data = (PhysObData *)instance->extraData;

    FX_EndInstanceEffects(instance);

    PHYSOB_EndLighting(instance, pLight);

    Data->burnAmpl = 0;

    Data->Mode &= ~0x10000;
}

void StopPhysOb(Instance *instance)
{
    PhysObData *Data;

    Data = (PhysObData *)instance->extraData;

    Data->Steps = 0;
    Data->Step = 0;

    Data->Segment1 = 1;

    ResetOrientation(instance);

    Data->Mode = 0x1;
}

void InitPhysicalObject(Instance *instance, GameTracker *gameTracker)
{
    PhysObData *Data;
    PhysObProperties *Prop;
    PhysObCollectibleProperties *collectibleProp;
    PhysObInteractProperties *interact;
    int s;
    G2SVector3 vec;
    G2Matrix mat;
    G2EulerAngles ea;
    SwitchData *switchData;
    PhysObSwitchProperties *switchProp;
    PhysObLight *pLight;
    INICommand *index;

    if ((instance->flags & 0x20000))
    {
        PhysObInteractProperties *interactProp;

        interactProp = (PhysObInteractProperties *)instance->data;

        if (interactProp->mode == 2)
        {
            INSTANCE_Post(gameTracker->playerInstance, 0x800024, (intptr_t)SetObjectIdleData(0, NULL));
        }

        if (CheckPhysObAbility(instance, 8) != 0)
        {
            G2Anim_DetachControllerFromSeg(&instance->anim, 0, 76);
            G2Anim_DetachControllerFromSeg(&instance->anim, 2, 14);
        }

        MEMPACK_Free((char *)instance->extraData);
        return;
    }

    Prop = (PhysObProperties *)instance->data;

    Data = (PhysObData *)MEMPACK_Malloc(sizeof(PhysObData), 26);

    instance->extraData = Data;

    Data->Mode = 0x1;

    Data->Segment1 = 4097;
    Data->Segment2 = 4098;

    Data->Pad1 = -1;

    Data->burnAmpl = 4096;

    Data->xForce = 0;
    Data->yForce = 0;

    Data->Force = NULL;

    Data->px = 0;
    Data->py = 0;
    Data->pz = 0;

    Data->rx1 = 0;
    Data->ry1 = 0;
    Data->rz1 = 0;

    Data->rx2 = 0;
    Data->ry2 = 0;
    Data->rz2 = 0;

    Data->Steps = 0;
    Data->Step = 0;

    Data->RightCollision = NULL;
    Data->LeftCollision = NULL;

    instance->maxXVel = 640;
    instance->maxYVel = 640;
    instance->maxZVel = 640;

    Data->xRotVel = 0;
    Data->yRotVel = 0;
    Data->zRotVel = 0;

    if ((instance->object != NULL) && (!(instance->object->oflags & 0x1)))
    {
        Data->Mode |= 0x1000;
    }

    if ((Prop->Type & 0x40))
    {
        Prop->Type |= 0x8000;

        instance->flags2 |= 0x4;
    }

    if ((Prop->Type & 0x8000))
    {
        G2EmulationInstanceSetTotalSections(instance, 1);
        G2EmulationInstanceSetStartAndEndSegment(instance, 0, 0, (int)instance->object->modelList[instance->currentModel]->numSegments - 1);
        G2EmulationInstanceSetAnimation(instance, 0, 0, 0, 0);
        G2EmulationInstanceSetMode(instance, 0, 2);
    }

    if (Prop->family == 5)
    {
        collectibleProp = (PhysObCollectibleProperties *)instance->data;

        G2EmulationInstanceInitSection(instance, 0, PhysobAnimCallback, instance);

        if (collectibleProp->idleAnim != 255)
        {
            G2EmulationInstanceSetAnimation(instance, 0, collectibleProp->idleAnim, 0, 0);
            G2EmulationInstanceSetMode(instance, 0, 2);
        }

        if ((collectibleProp->collectClass == 2) && (gameTrackerX.playerInstance != NULL) && (!(INSTANCE_Query(gameTrackerX.playerInstance, 36) & 0xFC0000)))
        {
            instance->flags |= 0x20;
        }

        if (collectibleProp->collectClass != 3)
        {
            instance->flags2 |= 0x20000;
        }
    }

    if ((Prop->Type & 0x8D00))
    {
        Data->Mode |= 0x1000;
    }

    if (Prop->family == 3)
    {
        PhysObInteractProperties *interactProp;

        interactProp = (PhysObInteractProperties *)instance->extraData;

        interactProp->Properties.ID = Prop->Type | 0x80;
        interactProp->Properties.Type = 0;

        interact = (PhysObInteractProperties *)instance->data;

        if ((interact->newType & 0x80))
        {
            instance->flags2 |= 0x20000;
        }
        else if (!(interact->newType & 0x1))
        {
            instance->flags2 |= 0x4;
        }

        if (instance->object->modelList != NULL)
        {
            if ((interact->startAnimMode & 0x8))
            {
                for (s = 0; s < instance->object->modelList[0]->numSegments; s++)
                {
                    COLLIDE_SegmentCollisionOff(instance, s);
                }
            }
            else
            {
                for (s = 0; s < instance->object->modelList[0]->numSegments; s++)
                {
                    COLLIDE_SegmentCollisionOn(instance, s);
                }
            }
        }

        TurnOffCollisionPhysOb(instance, 7);

        if ((Prop->Type & 0x8000))
        {
            switch (interact->startAnimMode & 0x3)
            {
            case 0:
                G2EmulationInstanceSetMode(instance, 0, 2);
                break;
            case 1:
                G2EmulationInstanceSetMode(instance, 0, 1);
                break;
            case 2:
                G2EmulationInstanceSetMode(instance, 0, 0);
                break;
            }
        }

        if ((interact->startAnim != 255) && ((Prop->Type & 0x8000)))
        {
            G2EmulationInstanceSetAnimation(instance, 0, interact->startAnim, 0, 0);
        }

        if (interact->mode == 2)
        {
            INSTANCE_Post(gameTracker->playerInstance, 0x800024, SetObjectIdleData(1, instance));
        }
    }
    else if (CheckPhysObAbility(instance, 1) != 0)
    {
        TurnOffCollisionPhysOb(instance, 7);
    }
    else if (CheckPhysObAbility(instance, 8) != 0)
    {
        instance->object->oflags |= 0x40000;

        Data->px = instance->position.x;
        Data->py = instance->position.y;
        Data->pz = instance->position.z;

        instance->lightMatrix = 2;

        instance->object->oflags |= 0x400;

        G2EmulationInstanceInitSection(instance, 0, PhysobAnimCallback, instance);

        G2Anim_AttachControllerToSeg(&instance->anim, 0, 76);
        G2Anim_AttachControllerToSeg(&instance->anim, 2, 14);

        G2Anim_DisableController(&instance->anim, 0, 76);

        G2Anim_EnableController(&instance->anim, 2, 14);

        vec.x = instance->rotation.x;
        vec.y = instance->rotation.y;
        vec.z = instance->rotation.z;

        RotMatrixZYX((SVECTOR *)&vec, (MATRIX *)&mat);

        G2EulerAngles_FromMatrix(&ea, &mat, 21);

        vec.x = ea.x;
        vec.y = ea.y;
        vec.z = ea.z;

        instance->rotation.x = 0;
        instance->rotation.y = 0;
        instance->rotation.z = 0;

        G2Anim_SetController_Vector(&instance->anim, 2, 14, &vec);
    }
    else
    {
        if (CheckPhysObAbility(instance, 64) != 0)
        {
            instance->flags2 |= 0x4;

            switchProp = (PhysObSwitchProperties *)instance->data;

            switchData = (SwitchData *)instance->extraData + 1;

            switchData->state = switchProp->startMode;

            switchData->accumulator = 0;

            if (switchProp->startAnim == 255)
            {
                switchProp->startAnim = 0;
            }

            G2EmulationInstanceSetAnimation(instance, 0, switchProp->startAnim, 0, 0);

            G2EmulationInstanceInitSection(instance, 0, PhysobAnimCallback, instance);

            if (switchProp->Class == 7)
            {
                switchData->accumulator = 1024;

                G2EmulationInstanceSetAnimation(instance, 0, switchProp->startAnim, 90, 0);
                G2EmulationInstanceSetMode(instance, 0, 1);

                G2EmulationInstancePlayAnimation(instance);
            }
        }

        if (CheckPhysObFamily(instance, 7) != 0)
        {
            G2EmulationInstanceInitSection(instance, 0, PhysobAnimCallback, instance);

            instance->flags2 |= 0x20000;
        }
    }

    if (CheckPhysObAbility(instance, 32) != 0)
    {
        index = INSTANCE_FindIntroCommand(instance, 21);

        pLight = PhysObGetLight(instance);

        if ((pLight != NULL) && ((index == NULL) || (!(index->parameter[0] & 0x1))))
        {
            Data->burnAmpl = 0;

            PHYSOB_StartBurnFX(instance);

            Data->Mode |= 0x10000;
        }
        else
        {
            PHYSOB_EndBurning(instance, pLight);
        }
    }

    instance->flags |= 0x10000;

    if (CheckPhysObFamily(instance, 6) != 0)
    {
        instance->flags2 |= 0x20000;
    }

    if (CheckPhysObFamily(instance, 0) != 0)
    {
        instance->flags2 |= 0x100;
    }
}

void ProcessPhysicalObject(Instance *instance, GameTracker *gameTracker)
{
    PhysObData *Data;

    Data = (PhysObData *)instance->extraData;

    if (instance == gameTracker->gameData.asmData.lightInstances[0].lightInstance)
    {
        PhysObLight *pLight;

        pLight = PhysObGetLight(instance);

        if (pLight != NULL)
        {
            LightInstance *li;

            li = gameTracker->gameData.asmData.lightInstances;

            if (li->lightInstance == instance)
            {
                PHYSOB_SetLightTable(pLight, li, ((Data->Mode & 0x8000)) ? abs(Data->burnAmpl) : 4096);
            }
        }
    }

    if (CheckPhysObAbility(instance, 2) != 0)
    {
        if (PHYSOBS_IsAnythingAttached(instance) != NULL)
        {
            instance->flags2 |= 0x80;
        }
        else
        {
            instance->flags2 &= ~0x80;
        }
    }

    if ((Data->Mode & 0x2000000))
    {
        instance->fadeValue += (short)(gameTrackerX.timeMult / 32);

        if (instance->fadeValue >= 4096)
        {
            instance->flags |= 0x20;
        }
    }

    if ((Data->Mode & 0x8000))
    {
        PhysObWeaponProperties *properties;

        if (Data->burnAmpl < 0)
        {
            Data->burnAmpl += 136;

            if (Data->burnAmpl >= 0)
            {
                Data->burnAmpl = 0;

                Data->Mode &= ~0x8000;

                properties = (PhysObWeaponProperties *)instance->data;

                if ((properties->Properties.Type & 0x20))
                {
                    PhysObLight *pLight;

                    pLight = PhysObGetLight(instance);

                    if ((Data->Mode & 0x40000))
                    {
                        PHYSOB_EndBurning(instance, pLight);
                    }
                    else
                    {
                        PHYSOB_EndLighting(instance, pLight);
                    }

                    Data->Mode &= ~0x40000;
                }
            }
        }
        else
        {
            Data->burnAmpl += 136;

            if (Data->burnAmpl >= 4096)
            {
                Data->burnAmpl = 4096;

                Data->Mode &= ~0x8000;
            }
        }
    }

    if (CheckPhysObFamily(instance, 5) != 0)
    {
        Dummy2 *temp; // not from decls.h

        temp = (Dummy2 *)instance->extraData;

        if (temp->unknown > 0)
        {
            temp->unknown -= gameTrackerX.timeMult;

            if (temp->unknown < 0)
            {
                temp->unknown = 0;

                INSTANCE_KillInstance(instance);
            }
            else if (temp->unknown <= 122879)
            {
                instance->fadeValue = 4096 - (temp->unknown / 30);
            }
            else if (instance->fadeValue != 0)
            {
                instance->fadeValue -= 64;

                if (instance->fadeValue < 0)
                {
                    instance->fadeValue = 0;
                }
            }
        }
    }

    {
        PhysObAnimatedProperties *Prop;

        if (CheckPhysObFamily(instance, 6) != 0)
        {
            Prop = (PhysObAnimatedProperties *)instance->data;

            if ((Prop->flags & 0x1))
            {
                if ((INSTANCE_Query(gameTrackerX.playerInstance, 36) & 0x10))
                {
                    instance->flags &= ~0x800;
                }
                else
                {
                    instance->flags |= 0x800;
                }
            }
        }
    }

    if (CheckPhysObAbility(instance, 64) != 0)
    {
        PhysObSwitchProperties *temp; // not from decls.h

        temp = (PhysObSwitchProperties *)instance->extraData;

        if ((Data->Mode & 0x100000))
        {
            temp->Properties.ID &= ~0xC;

            Data->Mode &= ~0x100800;
        }

        if ((Data->Mode & 0x800))
        {
            G2EmulationInstancePlayAnimation(instance);
        }
    }
    else
    {
        if (CheckPhysObAbility(instance, 0x8000) != 0)
        {
            G2EmulationInstancePlayAnimation(instance);
        }

        if (!(Data->Mode & 0x1))
        {
            if ((Data->Mode & 0x8))
            {
                ExecuteFlip(instance);
            }

            if (!(Data->Mode & 0x1))
            {
                if ((Data->Mode & 0x40))
                {
                    ExecuteDrag(instance);
                }
            }
        }

        if ((Data->Mode & 0x2))
        {
            ExecuteSlideToStop(instance);
        }

        if ((Data->Mode & 0x4000))
        {
            ExecuteGravitate(instance);
        }

        if ((Data->Mode & 0x10))
        {
            ExecuteThrow(instance);
        }

        if ((Data->Mode & 0x20))
        {
            ExecuteFollow(instance);
        }

        if (((Data->Mode & 0x100000)) && (CheckPhysObFamily(instance, 5) != 0))
        {
            PhysObCollectibleProperties *collectibleProp;

            collectibleProp = (PhysObCollectibleProperties *)instance->data;

            if (collectibleProp->idleAnim != 255)
            {
                G2EmulationInstanceSetAnimation(instance, 0, collectibleProp->idleAnim, 0, 0);
                G2EmulationInstanceSetMode(instance, 0, 2);
            }

            Data->Mode &= ~0x100000;
        }

        if ((Data->Mode & 0x10000))
        {
            Level *level;

            level = STREAM_GetLevelWithID(instance->currentStreamUnitID);

            if ((level != NULL) && (instance->matrix != NULL))
            {
                PhysObLight *pLight;

                pLight = PhysObGetLight(instance);

                if (level->waterZLevel > instance->matrix[pLight->segment].t[2])
                {
                    PHYSOB_EndBurning(instance, pLight);
                }
            }
        }

        if (CheckPhysObFamily(instance, 7) != 0)
        {
            evObjectBirthProjectileData *temp; // not from decls.h

            temp = (evObjectBirthProjectileData *)instance->extraData;

            if (temp->joint == 8)
            {
                Level *level;

                level = STREAM_GetLevelWithID(instance->currentStreamUnitID);

                if ((level != NULL) && (instance->position.z < level->waterZLevel))
                {
                    PhysObProjectileProperties *ProjProp;
                    PhysObProjData *ProjData;

                    ProjProp = (PhysObProjectileProperties *)instance->data;

                    if (instance->currentModel == 0)
                    {
                        FX_EndInstanceEffects(instance);
                    }

                    ProjData = &ProjProp->data[temp->joint];

                    if ((signed char)ProjData->endAnim != -1)
                    {
                        if (!(Data->Mode & 0x80000))
                        {
                            G2EmulationInstanceSetAnimation(instance, 0, (signed char)ProjData->endAnim, 0, 0);
                            G2EmulationInstanceSetMode(instance, 0, 1);

                            Data->Mode |= 0x81001;
                        }
                    }
                    else
                    {
                        INSTANCE_KillInstance(instance);
                    }
                }
            }
        }

        if ((!(Data->Mode & 0x1000)) && (instance->matrix != NULL))
        {
            evPhysicsGravityData *gravityData;
            int rc;
            int upperOffset;
            short lowerOffset;
            int wasFalling;
            int temp, temp2; // not from decls.h

            wasFalling = Data->Mode & 0x4;

            upperOffset = instance->oldPos.z - instance->position.z;

            if (upperOffset < 160)
            {
                upperOffset = 160;
            }

            temp = instance->zAccl * gameTrackerX.timeMult;

            temp2 = (instance->zVel + (short)(temp / 4096)) * gameTrackerX.timeMult;

            lowerOffset = -(temp2 / 4096) + 64; // TODO: looks a bit hacky, needs rewriting

            if (lowerOffset < 160)
            {
                lowerOffset = 160;
            }

            if (lowerOffset < upperOffset)
            {
                lowerOffset = upperOffset;
            }
            else
            {
                upperOffset = lowerOffset;
            }

            gravityData = (evPhysicsGravityData *)SetPhysicsGravityData((short)upperOffset, lowerOffset, 0, 0, 0, 2896);

            rc = PhysicsCheckGravity(instance, (intptr_t)gravityData, 13);

            if (((rc & 0x1)) && ((instance->attachedID == 0) || (wasFalling != 0)) && (instance->wNormal.z >= 2897))
            {
                wasFalling = instance->zVel < -50;

                if ((Data->Mode & 0x14))
                {
                    PHYSOB_PlayDropSound(instance);
                }

                Data->Mode &= ~0x4;

                if ((Data->Mode & 0x400000))
                {
                    Data->Mode = (Data->Mode & ~0x400010) | 0x200000;

                    Data->physObTimer = 10;

                    TurnOnCollisionPhysOb(instance, 7);
                }
                else
                {
                    Data->Mode &= ~0x800000;

                    if (!(Data->Mode & 0x4000000))
                    {
                        Data->Mode |= 0x1;
                    }
                }

                if ((Data->Mode & 0x10))
                {
                    instance->flags2 &= ~0x80;
                }

                Data->Mode &= ~0x10;

                if (!(Data->Mode & 0x200000))
                {
                    TurnOffCollisionPhysOb(instance, 4);
                }

                if ((rc & 0x80000))
                {
                    Data->Mode |= 0x1000;

                    instance->zAccl = 0;
                    instance->zVel = 0;
                }
                else
                {
                    instance->zAccl = -1;
                    instance->zVel = -1;
                }

                if ((instance->oldMatrix != NULL) && ((Data->Mode & 0x18000) == 0x10000))
                {
                    PHYSOB_StopLighting(instance, PhysObGetLight(instance));
                }

                if ((CheckPhysObAbility(instance, 8) != 0) && (!(Data->Mode & 0x100)))
                {
                    if (wasFalling != 0)
                    {
                        instance->zAccl = 0;

                        ResetOrientation(instance);
                    }

                    Data->pz = instance->position.z;
                }
            }
            else if ((instance->attachedID == 0) && (instance->oldMatrix != NULL))
            {
                if ((rc & 0x1))
                {
                    instance->position.x += gravityData->x;
                    instance->position.y += gravityData->y;
                }

                if ((instance->zVel == 0) && ((instance->object->oflags & 0x400)))
                {
                    instance->zVel = -50;
                }

                instance->zAccl = -10;

                Data->Mode |= 0x4;
            }
            else
            {
                instance->zAccl = 0;
                instance->zVel = 0;
            }

            if ((!(instance->object->oflags & 0x80000)) && (CheckPhysObAbility(instance, 8) != 0) && (gameTrackerX.gameData.asmData.MorphType == 0) && (gameTrackerX.gameData.asmData.MorphTime != 1000) && (!(instance->flags2 & 0x8000000)))
            {
                PCollideInfo CInfo;
                SVECTOR Old;
                SVECTOR New;
                short len;
                // short mult; unused
                int temp; // not from decls.h

                CInfo.oldPoint = &Old;
                CInfo.newPoint = &New;

                Old.vx = New.vx = instance->position.x;
                Old.vy = New.vy = instance->position.y;
                Old.vz = New.vz = instance->position.z;

                New.vz -= 1280;

                Old.vz += 320;

                PHYSICS_CheckLineInWorld(instance, &CInfo);

                len = (Old.vz - New.vz) - 320;

                instance->zAccl = 0;

                temp = 1000 - gameTrackerX.gameData.asmData.MorphTime;

                temp *= 50;

                if (len >= 960)
                {
                    short temp2; // not from decls.h

                    temp2 = Data->pz - instance->position.z;

                    len = instance->position.z - (Data->pz - ((temp2 / 640) + 1) * 640); // TODO: looks a bit hacky, needs rewriting
                }

                len = (len * 1000) / temp;

                len = (len << 12) / gameTrackerX.timeMult; // TODO: looks a bit hacky, needs rewriting

                instance->zVel = -len;
            }
        }
        else
        {
            instance->tface = NULL;
            instance->tfaceLevel = NULL;
        }

        if ((Data->Mode & 0x200000))
        {
            Data->physObTimer--;

            if ((PHYSOB_ReAlignFalling(instance, -900) != 0) || (Data->physObTimer <= 0))
            {
                Data->Mode = (Data->Mode & ~0xA00000) | 0x1;

                TurnOffCollisionPhysOb(instance, 7);
            }
        }

        if (((instance->object->oflags & 0x80000)) || (CheckPhysObAbility(instance, 8) != 0) || ((gameTrackerX.gameData.asmData.MorphTime == 1000) && (gameTrackerX.gameData.asmData.MorphType == 0)))
        {
            PhysicsMove(instance, &instance->position, gameTracker->timeMult);
        }
    }
}

unsigned long PhysicalObjectQuery(Instance *instance, unsigned long Query)
{
    PhysObData *Data;
    int trueValue;

    Data = (PhysObData *)instance->extraData;

    trueValue = 1;

    switch (Query)
    {
    case 1:
    {
        PhysObAnimatedProperties *temp; // not from decls.h

        if (CheckPhysOb(instance) == 0)
        {
            return 0;
        }

        if (CheckPhysObFamily(instance, 6) == 0)
        {
            return 32;
        }

        temp = (PhysObAnimatedProperties *)instance->data;

        if (((temp->flags & 0x1)) && ((instance->flags & 0x800)))
        {
            break;
        }

        return 32;
    }
    case 2:
        return GetPhysicalAbility(instance);
    case 3:
        return Data->Mode;
    case 5:
    {
        PhysObSwitchProperties *temp; // not from decls.h

        temp = (PhysObSwitchProperties *)instance->extraData;

        if (CheckPhysObAbility(instance, 64) != 0)
        {
            return (short)temp->Properties.ID;
        }

        return 0;
    }
    case 6:
        return SetPositionData(instance->position.x, instance->position.y, instance->position.z);
    case 7:
    {
        PhysObData *temp; // not from decls.h

        if (CheckPhysObAbility(instance, 64) != 0)
        {
            temp = (PhysObData *)instance->extraData;

            return SetPositionData(0, 0, (short)(instance->rotation.z + temp->yForce));
        }

        if (CheckPhysObFamily(instance, 1) != 0)
        {
            G2EulerAngles ea;

            if (instance->matrix != NULL)
            {
                G2EulerAngles_FromMatrix(&ea, (G2Matrix *)&instance->matrix[2], 0);
            }
            else
            {
                ea.x = 0;
                ea.y = 0;
                ea.z = 0;
            }

            return SetPositionData(ea.x, ea.y, ea.z);
        }

        return SetPositionData(instance->rotation.x, instance->rotation.y, instance->rotation.z);
    }
    case 4:
    {
        PhysObProperties *Prop;
        PhysObInteractProperties *temp; // not from decls.h

        Prop = (PhysObProperties *)instance->data;

        if (Prop->family == 3)
        {
            temp = (PhysObInteractProperties *)instance->extraData;

            return (short)temp->Properties.Type;
        }
        else if (Prop->family == 0)
        {
            return (signed char)((PhysObWeaponProperties *)Prop)->WeaponAttributes.Class;
        }
        else if (Prop->family == 2)
        {
            return ((PhysObSwitchProperties *)Prop)->Class;
        }

        return (Prop->family == 7) << 3;
    }
    case 11:
        return 3;
    case 12:
    case 13:
        return (unsigned long)instance->matrix;
    case 14:
    {
        PhysObCollectibleProperties *collectibleProp;

        if ((instance->matrix != NULL) && (CheckPhysObFamily(instance, 5) != 0))
        {
            collectibleProp = (PhysObCollectibleProperties *)instance->data;

            return (unsigned long)&instance->matrix[collectibleProp->spatialNode];
        }

        return 0;
    }
    case 21:
        if (CheckPhysObFamily(instance, 3) != 0)
        {
            return (unsigned long)instance->data;
        }

        return 0;
    case 29:
        if (CheckPhysObFamily(instance, 5) != 0)
        {
            return (unsigned long)instance->data;
        }

        return 0;
    case 28:
        if (CheckPhysObAbility(instance, 64) != 0)
        {
            return (unsigned long)instance->data;
        }

        return 0;
    case 22:
    {
        PhysObDraftProperties *DraftProp;
        INICommand *index;

        if (CheckPhysObAbility(instance, 256) != 0)
        {
            index = INSTANCE_FindIntroCommand(instance, 19);

            if (index != NULL)
            {
                DraftProp = (PhysObDraftProperties *)instance->data;

                DraftProp += index->parameter[0];
            }
            else
            {
                DraftProp = (PhysObDraftProperties *)instance->data;
            }

            return SetObjectDraftData(DraftProp->force, DraftProp->radius, DraftProp->radiusCoef, DraftProp->height, DraftProp->maxVelocity);
        }

        return 0;
    }
    case 24:
    {
        evControlSaveDataData *pdata;
        PhysObSaveData *data;

        pdata = (evControlSaveDataData *)CIRC_Alloc(sizeof(evControlSaveDataData) * 2);

        pdata->length = 8;

        pdata->data = pdata + 1; // TODO: something else should be assigned here

        memcpy(pdata + 1, (char *)&((evControlSaveDataData *)instance->extraData)->data, sizeof(SwitchData)); // TODO: parameters look wrong

        data = (PhysObSaveData *)pdata + 1; // TODO: looks wrong

        if ((instance->LinkParent != NULL) && ((instance->LinkParent->object->oflags2 & 0x80000)) && (instance->ParentLinkNode != 3))
        {
            data->Mode = Data->Mode | 0x2000000;
        }
        else if ((instance->LinkParent != NULL) && (instance->LinkParent == gameTrackerX.playerInstance))
        {
            data->Mode = (Data->Mode & ~0x1090) | 0x400004;
        }
        else
        {
            data->Mode = Data->Mode;
        }

        return (unsigned long)pdata;
    }
    case 27:
        trueValue = 0;
    case 26:
    {
        PhysObSwitchProperties *Prop;

        if (CheckPhysObFamily(instance, 2) != 0)
        {
            Prop = (PhysObSwitchProperties *)instance->extraData;

            if ((Prop->Properties.ID & 0x2))
            {
                return trueValue;
            }

            return trueValue == 0;
        }
        {
            PhysObData *Data;

            Prop = (PhysObSwitchProperties *)instance->data;

            if (Prop->Class == 0)
            {
                Data = (PhysObData *)instance->extraData;

                if ((Data->Mode & 0x2000))
                {
                    return trueValue == 0;
                }
            }

            return trueValue;
        }
    }
    case 23:
        if (CheckPhysObAbility(instance, 64) != 0)
        {
            return (unsigned long)instance->data;
        }

        return 0;
    case 47:
    {
        PhysObData *physobData;

        physobData = (PhysObData *)instance->extraData;

        if (physobData != NULL)
        {
            if ((Data->Mode & 0x800))
            {
                return 1;
            }

            if ((Data->Mode & 0x14E))
            {
                return 1;
            }
        }

        break;
    }
    case 17:
        return G2EmulationInstanceQueryAnimation(instance, 0);
    }

    return 0;
}

void PhysicalObjectPost(Instance *instance, unsigned long Message, unsigned long Data)
{
    evObjectData *Ptr;
    PhysObData *PData;

    PData = (PhysObData *)instance->extraData;

    Ptr = (evObjectData *)Data;

    switch (Message)
    {
    case 0x800025:
        break;
    case 0x800000:
        if ((STREAM_IsMorphInProgress() != 0) || (gameTrackerX.gameData.asmData.MorphType == 0))
        {
            Ptr->rc = PushPhysOb(instance, Ptr->x, Ptr->y, Ptr->PathNumber, Ptr->Force);
        }

        break;
    case 0x800001:
        if ((STREAM_IsMorphInProgress() != 0) || (gameTrackerX.gameData.asmData.MorphType == 0))
        {
            Ptr->rc = FlipPhysOb(instance, Ptr->x, Ptr->y, Ptr->Force);
        }

        break;
    case 0x800002:
        if ((STREAM_IsMorphInProgress() != 0) || (gameTrackerX.gameData.asmData.MorphType == 0) || (Ptr->PathNumber == 0))
        {
            Ptr->LinkNode = PickUpPhysOb(instance, Ptr->PathNumber, Ptr->Force, Ptr->LinkNode);

            if ((Ptr->PathNumber == 0) && ((PData->Mode & 0x4000)))
            {
                ExecuteGravitate(instance);
            }
        }

        break;
    case 0x80002E:
        if (CanBePickedUp(instance, Ptr->Force, Ptr->LinkNode) != 0)
        {
            Ptr->LinkNode = 0;
        }

        break;
    case 0x800004:
        StopPhysOb(instance);
        break;
    case 0x800008:
        DropPhysOb(instance, (int)Data);

        if (((int)Data & 0x1))
        {
            PData->Mode |= 0x2000000;
        }

        break;
    case 0x800010:
        ThrowPhysOb(instance, (evObjectThrowData *)Data);
        break;
    case 0x800020:
        SwitchPhysOb(instance);
        break;
    case 0x80002D:
        ResetSwitchPhysOb(instance);
        break;
    case 0x800023:
        if ((STREAM_IsMorphInProgress() != 0) || (gameTrackerX.gameData.asmData.MorphType == 0))
        {
            evObjectBreakOffData *temp; // not from decls.h

            temp = (evObjectBreakOffData *)Data;

            InteractPhysOb(instance, temp->force, temp->node, temp->action);
        }

        break;
    case 0x800021:
    case 0x800022:
    {
        PhysObSwitchProperties *Prop;
        PhysObSwitchProperties *temp; // not from decls.h

        if (CheckPhysOb(instance) != 0)
        {
            Prop = (PhysObSwitchProperties *)instance->data;

            if (CheckPhysObFamily(instance, 2) != 0)
            {
                temp = (PhysObSwitchProperties *)instance->extraData;

                if (Message == 0x800021)
                {
                    temp->Properties.ID |= 0x2;
                }
                else
                {
                    temp->Properties.ID &= ~0x2;
                }

                break;
            }

            if (Prop->Class == 0)
            {
                PhysObData *Data;

                Data = (PhysObData *)instance->extraData;

                if (Message == 0x800021)
                {
                    Data->Mode &= ~0x2000;
                    break;
                }

                Data->Mode |= 0x2000;
            }
        }

        break;
    }
    case 0x100007:
    {
        PhysObSaveData *saveData;

        saveData = ((evControlSaveDataData *)Data)->data;

        *(SwitchData *)&PData->xForce = saveData->switchData; // TODO: very illogical, needs rewriting

        PData->Mode = saveData->Mode & ~0x100800;

        if ((PData->Mode & 0x2000000))
        {
            instance->flags |= 0x20;
        }

        if (CheckPhysObAbility(instance, 32) != 0)
        {
            PhysObLight *pLight;

            pLight = PhysObGetLight(instance);

            if (!(PData->Mode & 0x10000))
            {
                PHYSOB_EndBurning(instance, pLight);
            }
        }

        if (CheckPhysObFamily(instance, 2) != 0)
        {
            PhysObSwitchProperties *Prop;
            PhysObSwitchProperties *temp; // not from decls.h

            temp = (PhysObSwitchProperties *)instance->extraData;

            Prop = (PhysObSwitchProperties *)instance->data;

            if ((temp->Properties.ID & 0x1))
            {
                G2EmulationInstanceSetAnimation(instance, 0, Prop->offAnim, 0, 0);
                G2EmulationInstanceSetMode(instance, 0, 1);

                G2EmulationInstancePlayAnimation(instance);
            }
        }

        if (CheckPhysObFamily(instance, 3) != 0)
        {
            PhysObInteractProperties *temp; // not from decls.h

            temp = (PhysObInteractProperties *)instance->extraData;

            if ((short)temp->Properties.Type != 0)
            {
                G2EmulationInstanceSetAnimation(instance, 0, 0, 0, 0);
                G2EmulationInstanceSetMode(instance, 0, 1);

                G2EmulationInstancePlayAnimation(instance);
            }
        }

        break;
    }
    case 0x100008:
        if (PData != NULL)
        {
            PData->px += Ptr->x;
            PData->py += Ptr->y;
            PData->pz += Ptr->PathNumber;
        }

        break;
    case 0x800026:
    {
        PhysObCollectibleProperties *collectibleProp;

        collectibleProp = (PhysObCollectibleProperties *)instance->data;

        switch (collectibleProp->collectClass)
        {
        case 1:
            HEALTHU_Pickup(instance);
            break;
        case 2:
        case 5:
            MANNA_Pickup();
        case 3:
        case 4:
            INSTANCE_PlainDeath(instance);
            break;
        case 7:
            MANNA_Pickup();
        case 6:
            if ((collectibleProp->collectAnim != 0xFF) && (G2EmulationInstanceQueryAnimation(instance, 0) != collectibleProp->collectAnim))
            {
                printf("Collect %s\n", instance->object->name);

                G2EmulationInstanceSetAnimation(instance, 0, collectibleProp->collectAnim, 0, 0);
                G2EmulationInstanceSetMode(instance, 0, 1);
            }

            break;
        }

        break;
    }
    case 0x800027:
        if ((uintptr_t *)Data != NULL)
        {
            PData->Mode &= ~0x1000;
        }
        else
        {
            PData->Mode |= 0x1000;
        }

        break;
    case 0x80002C:
        PData->Mode |= 0x800000;
        break;
    case 0x200002:
        TurnOnCollisionPhysOb(instance, (int)Data);
        break;
    case 0x200003:
        TurnOffCollisionPhysOb(instance, (int)Data);
        break;
    case 0x200005:
        PData->Mode |= 0x1000000;
        break;
    case 0x200006:
        PData->Mode &= ~0x1000000;
        break;
    case 0x80002b:
        PData->Mode &= ~0x800000;
        break;
    case 0x800029:
    {
        PhysObLight *pLight;

        pLight = PhysObGetLight(instance);

        if (pLight != NULL)
        {
            switch ((unsigned long)Data)
            {
            case 0:
                PHYSOB_StopBurning(instance, pLight);
                break;
            case 1:
                PHYSOB_StartBurning(instance, pLight);
                break;
            case 2:
            {
                PhysObData *podata;

                podata = (PhysObData *)instance->extraData;

                podata->burnAmpl = 0;

                podata->Mode &= ~0x8000;

                PHYSOB_EndBurning(instance, pLight);
                break;
            }
            }
        }

        break;
    }
    case 0x80005:
        ScriptKillInstance(instance, 5);
        break;
    case 0x40002:
        ScriptKillInstance(instance, (int)Data);
        break;
    case 0x80002A:
    {
        PhysObSwitchProperties *temp; // not from decls.h

        if (CheckPhysObFamily(instance, 2) != 0)
        {
            temp = (PhysObSwitchProperties *)instance->extraData;

            temp->Properties.ID = (unsigned short)Data;
        }

        break;
    }
    case 0x8000008:
    {
        evAnimationInstanceSwitchData *temp; // not from decls.h

        temp = (evAnimationInstanceSwitchData *)Data;

        G2EmulationInstanceSetAnimation(instance, 0, temp->anim, temp->frame, temp->frames);
        G2EmulationInstanceSetMode(instance, 0, temp->mode);
        break;
    }
    case 0x4000E:
    {
        PhysObProperties *Prop;

        if ((uintptr_t *)Data != NULL)
        {
            Prop = (PhysObProperties *)instance->data;

            if ((Prop->family == 7) && (instance->parent != NULL) && ((INSTANCE_Query(instance->parent, 1) & 0xA)))
            {
                instance->flags |= 0x20;
            }
        }

        break;
    }
    }
}

long PhysobAnimCallback(G2Anim *anim, int sectionID, G2AnimCallbackMsg message, long messageDataA, long messageDataB, void *data)
{
    PhysObData *Data;
    Instance *temp; // not from decls.h

    if (message == 1)
    {
        temp = (Instance *)data;

        Data = (PhysObData *)temp->extraData;

        G2AnimSection_SetPaused(&anim->section[sectionID]);

        Data->Mode |= 0x100000;

        if ((Data->Mode & 0x80000))
        {
            INSTANCE_KillInstance(temp);
        }

        return messageDataA;
    }

    return INSTANCE_DefaultAnimCallback(anim, sectionID, message, messageDataA, messageDataB, (Instance *)data);
}

void CheckForceCollision(struct _Instance *instance, struct _Instance *hitinst, struct _TFace *tface, struct _CollideInfo *collideInfo, struct _SVector *dir, int obliqueFlg)
{
    int killImmediately; /* var s5 */
    struct PhysObData *data; /* temp s3*/
    struct PhysObData *data2; // ??
    struct evObjectBirthProjectileData *temp;
    struct __PhysObProjectileProperties *ProjProp;
    struct __PhysObProjectileData *ProjData;

    data = instance->extraData; // struct PhysObData

    if (CheckPhysObFamily(instance, 7) == 0)
    {
        return;
    }

    killImmediately = 1;

    ProjProp = instance->data; // struct __PhysObProjectileProperties
    temp = instance->extraData; // struct evObjectBirthProjectileData

    COLLIDE_SegmentCollisionOff(instance, 0);

    data2 = (struct PhysObData *)((intptr_t)temp+4); // ??

    ProjData = &ProjProp->data[temp->joint];

    if (ProjData->endAnim != -1)
    {

        data->Mode = (data->Mode & 0xFFBFFFFB) | 0x1001;

        instance->xVel = 0;
        instance->yVel = 0;
        instance->zVel = 0;
        instance->xAccl = 0;
        instance->yAccl = 0;
        instance->zAccl = 0;

        if ((data->Mode & 0x80000))
        {
            return;
        }

        if (instance->currentModel == 0)
        {
            FX_EndInstanceEffects(instance);
        }

        G2EmulationInstanceSetAnimation(instance, 0, ProjData->endAnim, 0, 0);
        G2EmulationInstanceSetMode(instance, 0, 1);

        killImmediately = 0;

        data->Mode |= 0x80000 | 0x1000 | 0x1;
    }

    if ((instance->parent != 0) && (INSTANCE_Query(instance->parent, 1) & 1))
    {
        if ((tface != 0) && (gameTrackerX.gameData.asmData.MorphType == 0))
        {
            COLLIDE_SetBSPTreeFlag(collideInfo, 0x400);
        }
        else if ((hitinst != gameTrackerX.playerInstance) && (gameTrackerX.gameData.asmData.MorphType == 0))
        {
            if ((CheckPhysOb(hitinst) != 0) && (hitinst->introUniqueID != gameTrackerX.playerInstance->attachedID) && (obliqueFlg == 0))
            {
                INSTANCE_Post(hitinst, 0x800000, SetObjectData(dir->x, dir->y, 6, 0, 0));
            }
            if (data2->Mode == 1)
            { // ??
                hitinst->flags2 |= 0x10000;
            }
            else
            {
                hitinst->flags |= 0x80000000;
            }
        }
    }

    if (killImmediately != 0)
    {
        INSTANCE_KillInstance(instance);
    }
}

int GetObliqueDirection(Instance *instance, SVector *dir)
{
    SVector vel;
    int obliqueFlg;
    long axVel;
    long ayVel;
    int temp; // not from decls.h

    obliqueFlg = 0;

    vel.x = instance->xVel;
    vel.y = instance->yVel;
    vel.z = instance->zVel;

    if (((vel.x << 16) != 0) || ((vel.y << 16) != 0))
    {
        axVel = abs(instance->xVel);
        ayVel = abs(instance->yVel);

        dir->z = 0;

        if (axVel > ayVel)
        {
            if (instance->xVel > 0)
            {
                dir->x = 4096;
            }
            else
            {
                dir->x = -4096;
            }

            dir->y = 0;

            temp = (axVel >> 1) > ayVel;
        }
        else
        {
            if (instance->yVel > 0)
            {
                dir->y = 4096;
            }
            else
            {
                dir->y = -4096;
            }

            dir->x = 0;

            temp = axVel < (ayVel >> 1);
        }

        if (temp == 0)
        {
            obliqueFlg = 1;
        }
    }
    else
    {
        dir->z = 0;
        dir->y = 0;
        dir->x = 0;
    }

    return obliqueFlg;
}

INCLUDE_ASM("asm/nonmatchings/Game/PHYSOBS", CollidePhysicalObject);

PhysObWeaponAttributes *PhysObGetWeapon(Instance *instance)
{
    PhysObWeaponAttributes *weapon;

    weapon = NULL;

    if (CheckPhysObFamily(instance, 0) != 0)
    {
        PhysObWeaponProperties *Prop;

        Prop = (PhysObWeaponProperties *)instance->data;

        weapon = &Prop->WeaponAttributes;
    }

    if (CheckPhysObFamily(instance, 3) != 0)
    {
        PhysObInteractProperties *Prop;

        Prop = (PhysObInteractProperties *)instance->data;

        weapon = Prop->weapon;
    }

    if (CheckPhysObFamily(instance, 7) != 0)
    {
        PhysObProjectileProperties *Prop;
        PhysObProjData *temp;               // not from decls.h
        evObjectBirthProjectileData *temp2; // not from decls.h

        Prop = (PhysObProjectileProperties *)instance->data;

        temp2 = (evObjectBirthProjectileData *)instance->extraData;

        temp = Prop->data;

        weapon = temp[temp2->joint].weapon;
    }

    return weapon;
}

PhysObLight *PhysObGetLight(Instance *instance)
{
    PhysObWeaponAttributes *weapon;
    PhysObLight *pLight;

    pLight = NULL;

    weapon = PhysObGetWeapon(instance);

    if (weapon != NULL)
    {
        pLight = weapon->Light;
    }

    return pLight;
}

PhysObSplinter *PhysObGetSplinter(Instance *instance)
{
    PhysObWeaponAttributes *weapon;
    PhysObSplinter *pSplinter;

    pSplinter = NULL;

    weapon = PhysObGetWeapon(instance);

    if (weapon != NULL)
    {
        pSplinter = &weapon->splinter;
    }

    return pSplinter;
}

void TurnOnCollisionPhysOb(Instance *instance, int coll)
{
    PhysObWeaponAttributes *weapon;
    PhysObData *temp; // not from decls.h

    temp = (PhysObData *)instance->extraData;

    temp->Mode &= ~0x1000000;

    weapon = PhysObGetWeapon(instance);

    if (weapon != NULL)
    {
        if ((coll & 0x2))
        {
            COLLIDE_SegmentCollisionOn(instance, (signed char)weapon->LeftHandSphere);
        }

        if ((coll & 0x1))
        {
            COLLIDE_SegmentCollisionOn(instance, (signed char)weapon->RightHandSphere);
        }

        if ((coll & 0x4))
        {
            COLLIDE_SegmentCollisionOn(instance, (signed char)weapon->ThrowSphere);
        }
    }
}

void TurnOffCollisionPhysOb(Instance *instance, int coll)
{
    PhysObWeaponAttributes *weapon;

    weapon = PhysObGetWeapon(instance);

    if (weapon != NULL)
    {
        if ((coll & 0x2))
        {
            COLLIDE_SegmentCollisionOff(instance, (signed char)weapon->LeftHandSphere);
        }

        if ((coll & 0x1))
        {
            COLLIDE_SegmentCollisionOff(instance, (signed char)weapon->RightHandSphere);
        }

        if ((coll & 0x4))
        {
            COLLIDE_SegmentCollisionOff(instance, (signed char)weapon->ThrowSphere);
        }
    }
}

int GetPhysObCollisionType(Instance *instance)
{
    CollideInfo *collideInfo;
    // HSphere *S; unused
    Instance *target;
    PhysObWeaponAttributes *weapon;
    PhysObData *Data;

    Data = (PhysObData *)instance->extraData;

    collideInfo = (CollideInfo *)instance->collideInfo;

    if (!(Data->Mode & 0x1600001))
    {
        weapon = (PhysObWeaponAttributes *)collideInfo->prim0;

        if ((instance->LinkParent == gameTrackerX.playerInstance) && ((unsigned char)weapon->AltDamage == 9))
        {
            if ((unsigned char)collideInfo->type1 != 3)
            {
                Instance *inst;

                inst = (Instance *)collideInfo->inst1;

                inst->flags |= 0x4;
            }
            else
            {
                COLLIDE_SetBSPTreeFlag(collideInfo, 0x800);
            }

            if ((unsigned char)collideInfo->type1 != 1)
            {
                COLLIDE_SegmentCollisionOff(instance, (unsigned char)collideInfo->segment);

                if (instance->LinkParent == gameTrackerX.playerInstance)
                {
                    INSTANCE_Post(instance->LinkParent, 0x1000024, 0);
                }
            }
        }

        if (((collideInfo->type0 == 1) && (collideInfo->type1 == 1)) && ((unsigned char)weapon->AltDamage == 9) && (weapon = (PhysObWeaponAttributes *)collideInfo->prim1, ((HSphere *)weapon)->id == 8)) // TODO: weapon should be S here
        {
            target = (Instance *)collideInfo->inst1;

            weapon = PhysObGetWeapon(instance);

            if (weapon != NULL)
            {
                TurnOffCollisionPhysOb(instance, 7);

                if (instance->LinkParent == NULL)
                {
                    if ((INSTANCE_Query(target, 0) & 0x10000000))
                    {
                        if (weapon->Class == 1)
                        {
                            INSTANCE_Post(target, 0x1000019, (intptr_t)instance);

                            Data->Mode &= ~0x10;
                        }
                        else if ((CheckPhysObAbility(instance, 32) != 0) && ((Data->Mode & 0x10000)))
                        {
                            INSTANCE_Post(target, 0x100000C, 0x20);
                            INSTANCE_Post(instance, 0x800029, 0);
                        }
                        else
                        {
                            INSTANCE_Post(target, 0x1000000, SetMonsterHitData(instance, NULL, weapon->Damage, weapon->knockBackDistance, weapon->knockBackFrames));
                        }
                    }
                    else
                    {
                        INSTANCE_Post(target, 0x1000000, SetMonsterHitData(instance, NULL, weapon->Damage, weapon->knockBackDistance, weapon->knockBackFrames));
                    }

                    if ((gameTrackerX.gameData.asmData.MorphType == 0) && (CheckPhysObFamily(instance, 7) != 0) && (instance->parent == gameTrackerX.playerInstance))
                    {
                        INSTANCE_Post(target, 0x80001, 0);
                    }
                }
                else
                {
                    int damage;

                    damage = weapon->Damage;

                    if (CheckPhysObAbility(instance, 32) != 0)
                    {
                        if (!(Data->Mode & 0x10000))
                        {
                            damage = weapon->AltDamage;
                        }
                    }

                    INSTANCE_Post(instance->LinkParent, 0x2000002, SetMonsterHitData(target, NULL, damage, 0, 0));
                }

                if ((CheckPhysObAbility(instance, 32) != 0) && ((Data->Mode & 0x10000)))
                {
                    INSTANCE_Post(target, 0x400000, SetFXHitData(instance, (unsigned char)collideInfo->segment, weapon->Damage >> 7, 32));
                }
                else
                {
                    INSTANCE_Post(target, 0x400000, SetFXHitData(instance, (unsigned char)collideInfo->segment, weapon->Damage >> 7, 256));
                }

                return 1;
            }
        }
    }

    return 0;
}

void ExecuteThrow(Instance *instance)
{
    PhysObData *Data;

    Data = (PhysObData *)instance->extraData;

    if ((Data->throwFlags & 0x1))
    {
        short angley;
        Position zero = {0};
        Position velocity;

        velocity.x = (short)instance->xVel;
        velocity.y = (short)instance->yVel;
        velocity.z = (short)instance->zVel;

        angley = MATH3D_AngleFromPosToPos(&zero, &velocity);

        instance->rotation.x = Data->initialXRot + MATH3D_ElevationFromPosToPos(&zero, &velocity);
        instance->rotation.z = angley;
        instance->rotation.y = 0;

        MATH3D_ZYXtoXYZ(&instance->rotation);
    }
    else
    {
        instance->rotation.x += Data->xRotVel;
        instance->rotation.y += Data->yRotVel;
        instance->rotation.z += Data->zRotVel;
    }

    Data->physObTimer -= gameTrackerX.timeMult;

    if (Data->physObTimer < 0)
    {
        INSTANCE_KillInstance(instance);
    }
}

void ExecuteDrag(Instance *instance)
{
    PhysObData *Data;

    Data = (PhysObData *)instance->extraData;

    if ((Data->Mode & 0x2))
    {
        Data->Mode &= ~0x2;
    }

    if ((Data->Mode & 0x100000))
    {
        Data->Force = NULL;

        FinishPush(instance);

        Data->Mode = (Data->Mode & ~0x10014A) | 0x1;

        instance->initialPos = instance->position;
    }
    else
    {
        G2EmulationInstancePlayAnimation(instance);
    }
}

void ExecuteSlideToStop(Instance *instance)
{
    PhysObData *Data;

    Data = (PhysObData *)instance->extraData;

    if ((Data->Mode & 0x100000))
    {
        if ((Data->Mode & 0x4000000))
        {
            Data->Mode &= ~0x410114A;

            if (!(Data->Mode & 0x4))
            {
                Data->Mode |= 0x1;
            }
        }
        else
        {
            Data->Force = NULL;

            FinishPush(instance);

            Data->Mode = (Data->Mode & ~0x1000) | 0x4000000;

            instance->initialPos = instance->position;
        }
    }
    else
    {
        G2EmulationInstancePlayAnimation(instance);
    }
}

void ExecuteFlip(Instance *instance)
{
    PhysObData *Data;

    Data = (PhysObData *)instance->extraData;

    if ((Data->Mode & 0x2))
    {
        Data->Mode &= ~0x2;
    }

    if ((Data->Mode & 0x100000))
    {
        if ((Data->Mode & 0x4000000))
        {
            Data->Mode &= ~0x410114A;

            if (!(Data->Mode & 0x4))
            {
                Data->Mode |= 0x1;
            }
        }
        else
        {
            if (Data->PathNumber == 14)
            {
                instance->zVel = -50;
            }

            ResetOrientation(instance);

            Data->Force = NULL;

            Data->Mode = (Data->Mode & ~0x1000) | 0x4000000;

            instance->initialPos = instance->position;
        }
    }
    else
    {
        G2EmulationInstancePlayAnimation(instance);
    }
}

int CheckSlope(int Znormal, int Slope, int Slop)
{
    int temp; // not from decls.h

    temp = 0;

    if ((Slope - Slop) < Znormal)
    {
        temp = Znormal < (Slope + Slop);
    }

    return temp;
}

int CheckBlockSlope(Instance *instance, int x, int y, int hOff, int vOff, int dH, int dV)
{
    PCollideInfo CInfo;
    SVECTOR Old;
    SVECTOR New;
    int xOff;
    int yOff;
    int dX;
    int dY;
    MATRIX *mat;

    CInfo.oldPoint = &Old;
    CInfo.newPoint = &New;

    mat = instance->matrix;

    Old.vx = mat->t[0];
    Old.vy = mat->t[1];
    Old.vz = mat->t[2];

    xOff = x * hOff;
    yOff = y * hOff;

    Old.vx += xOff;
    Old.vy += yOff;
    Old.vz += vOff;

    dX = x * dH;
    dY = y * dH;

    New.vx = Old.vx + dX;
    New.vy = Old.vy + dY;
    New.vz = Old.vz + dV;

    PHYSICS_CheckLineInWorld(instance, &CInfo);

    return CInfo.type == 5;
}

int PHYSOBS_FigureDragForSlope(Instance *instance, int pathNumber, int *result)
{
    int didSomething;

    didSomething = 0;

    do
    {
    } while (0); // garbage code for reodering

    if ((PHYSOBS_IsAPushBlockAttached(instance) != NULL) || (pathNumber == 5) || (pathNumber == 1) || (pathNumber == 7) || (pathNumber == 4) || (pathNumber == 2) || (didSomething = 0, pathNumber == 3))
    {
        *result = 0;

        didSomething = 1;
    }

    return didSomething;
}

INCLUDE_ASM("asm/nonmatchings/Game/PHYSOBS", PHYSOB_CheckThrownLineCollision);

INCLUDE_ASM("asm/nonmatchings/Game/PHYSOBS", PHYSOB_CheckDroppedLineCollision);

int PHYSOB_CheckDirectedLineCollision(Instance *instance, int xoffset, int yoffset, int startZOffset)
{
    PCollideInfo pcollideInfo;
    Position newPos;
    Position oldPos;
    MATRIX *mat;

    mat = &instance->matrix[1];

    oldPos.x = mat->t[0];
    oldPos.y = mat->t[1];
    oldPos.z = mat->t[2] + startZOffset;

    newPos.x = oldPos.x + xoffset;
    newPos.y = oldPos.y + yoffset;
    newPos.z = oldPos.z;

    pcollideInfo.newPoint = (SVECTOR *)&newPos;
    pcollideInfo.oldPoint = (SVECTOR *)&oldPos;

    PHYSICS_CheckLineInWorld(instance, &pcollideInfo);

    return pcollideInfo.type;
}

long PHYSOBS_CheckForStackedForwardHits(Instance *block, long xoffset, long yoffset)
{
    Instance *attachedBlock;
    long result;

    result = 0;

    attachedBlock = PHYSOBS_IsAPushBlockAttached(block);

    while (attachedBlock != NULL)
    {
        if (PHYSOB_CheckDirectedLineCollision(attachedBlock, xoffset, yoffset, 0) != 0)
        {
            result = 1;
            break;
        }

        attachedBlock = PHYSOBS_IsAPushBlockAttached(attachedBlock);
    }

    return result;
}

void PHYSOBS_SetNewAnim(Instance *instance, PhysObData *Data, unsigned long modeBits, int path, int resetflg)
{
    Data->PathNumber = (short)path;

    Data->Mode = (Data->Mode & ~0x1) | modeBits;

    if (resetflg != 0)
    {
        ResetOrientation(instance);
    }

    PhysOb_AlignPush(instance, (int)Data->xForce, (int)Data->yForce, path, Data);
}

long PHYSOB_CheckForEnemyInBlkSpot(Instance *instance, int dx, int dy)
{
    int x;
    int y;
    int z;
    int x0;
    int x1;
    int y0;
    int y1;
    int z0;
    int z1;
    Instance *inst;

    inst = gameTrackerX.instanceList->first;

    x = instance->position.x + dx;
    y = instance->position.y + dy;
    z = instance->position.z;

    x0 = x - 320;
    x1 = x + 320;

    y0 = y - 320;
    y1 = y + 320;

    z0 = z - 320;
    z1 = z + 320;

    while (inst != NULL)
    {
        if (((!(inst->object->oflags2 & 0x80000)) || ((inst->flags2 & 0x8000000))) || ((INSTANCE_Query(inst, 0) & 0x40000000)) || ((inst->position.x < x0) || (inst->position.x > x1)) || ((inst->position.y < y0) || (inst->position.y > y1)) || ((inst->position.z < z0) || (inst->position.z > z1)))
        {
            inst = inst->next;
        }
        else
        {
            return 1;
        }
    }

    return 0;
}

long PHYSOBS_CheckForValidMove(Instance *instance)
{
    int rc;
    PhysObData *Data;
    evPhysicsSlideData *Ptr;
    int result;
    // int Height; unused (line 86)
    short temp; // not from decls.h

    gameTrackerX.block_collide_override = 1;

    Data = (PhysObData *)instance->extraData;

    result = 7;

    if (((Data->Mode & 0x1)) && (PhysicsUpdateTface(instance, SetPhysicsGravityData(160, 640, 0, 0, 0, 2896)), (PHYSOBS_CheckForStackedForwardHits(instance, Data->xForce * 704, Data->yForce * 704) == 0)))
    {
        rc = PHYSOB_CheckSlide(instance, Data->xForce, Data->yForce, &Ptr);

        if ((rc & 0x2))
        {
            if (((CheckSlope(Ptr->ForwardNormal.z, 3663, 4) == 0) && (CheckSlope(Ptr->ForwardNormal.z, 4096, 4) == 0)) && ((Data->Mode & 0x8)) && ((rc & 0x4)) && (PHYSOB_CheckDirectedLineCollision(instance, Data->xForce * 704, Data->yForce * 704, 640) == 0) && (((169 - Ptr->Height) >= 0) && ((169 - Ptr->Height) < 19)))
            {
                PHYSOBS_SetNewAnim(instance, Data, 2, 7, 1);

                result = (result & ~4) | 8;
            }
            else
            {
                result = 0;
            }
        }
        else if ((rc & 0x4000))
        {
            if ((Data->Mode & 0x8))
            {
                if ((Data->Mode & 0x200))
                {
                    result = 0;
                }

                if (((rc & 0x8000)) || (PHYSOB_CheckDirectedLineCollision(instance, Data->xForce * 704, Data->yForce * 704, 640) != 0))
                {
                    result = 0;
                }
            }

            if (result != 0)
            {
                if ((CheckSlope(Ptr->Dot, -314, 4) == 0) && (CheckSlope(Ptr->Dot, 314, 4) == 0) && (!(Data->Mode & 0x200)))
                {
                    if (CheckSlope(Ptr->DropNormal.z, 3663, 4) != 0)
                    {
                        if (((Ptr->DropNormal.x > 10) && (Data->xForce > 0)) || ((Ptr->DropNormal.x < -10) && (Data->xForce < 0)) || ((Ptr->DropNormal.y > 10) && (Data->yForce > 0)) || ((Ptr->DropNormal.y < -10) && (Data->yForce < 0)))
                        {
                            if (CheckBlockSlope(instance, Data->xForce, Data->yForce, 732, -173, 625, -312) == 0)
                            {
                                if (PHYSOBS_FigureDragForSlope(instance, Data->PathNumber, &result) == 0)
                                {
                                    result = 0;
                                }
                            }
                            else
                            {
                                result = 0;
                            }
                        }
                        else
                        {
                            result = 0;
                        }
                    }
                    else if (((PHYSOB_CheckForEnemyInBlkSpot(instance, Data->xForce * 640, Data->yForce * 640) == 0) || (PHYSOB_CheckDirectedLineCollision(instance, Data->xForce * 1344, Data->yForce * 1344, 0) == 0)) && (CheckSlope(Ptr->DropNormal.z, 4096, 4) != 0))
                    {
                        PHYSOBS_SetNewAnim(instance, Data, 2, Data->PathNumber, 0);
                    }
                    else
                    {
                        result = 0;
                    }
                }
                else
                {
                    result = 0;
                }
            }
        }
        else
        {
            evPhysicsSlideData *Ptr;

            if ((!(PHYSOB_CheckDropOnSlope(instance, Data->xForce, Data->yForce, &Ptr) & 0x4000)) || (Ptr->DropNormal.z > 3900))
            {
                if (((Data->Mode & 0x8)) && ((rc & 0x8000)))
                {
                    result = 0;
                }

                temp = Data->PathNumber;

                if (((temp != 2) && (temp != 3)) && (result != 0))
                {
                    if (temp == 5)
                    {
                        result &= ~5;
                    }
                    else if ((Data->Mode & 0x8))
                    {
                        Data->PathNumber = 14;

                        PHYSOBS_SetNewAnim(instance, Data, 2, Data->PathNumber, 1);
                    }
                    else
                    {
                        PHYSOBS_SetNewAnim(instance, Data, 2, temp, 1);
                    }
                }
                else
                {
                    result = 0;
                }
            }
            else
            {
                result = 0;
            }
        }
    }
    else
    {
        result = 0;
    }

    gameTrackerX.block_collide_override = 0;

    if (result == 0)
    {
        Data->Mode &= ~0x14A;
    }

    return result;
}

void ExecuteGravitate(Instance *instance)
{
    PhysObData *Data;
    Instance *Force;
    G2EulerAngles ea;
    Vector Position;
    Model *model;
    MATRIX *forceMatrix;
    int scale;

    Data = (PhysObData *)instance->extraData;

    Force = Data->Force;

    model = instance->object->modelList[0];

    forceMatrix = &Force->matrix[Data->LinkNode];

    if (CheckPhysObAbility(instance, 1) != 0)
    {
        COLLIDE_SegmentCollisionOff(instance, 1);
    }

    if (Data->Steps != 0)
    {
        G2EulerAngles_FromMatrix(&ea, (G2Matrix *)forceMatrix, 21);

        if (Data->Step == 0)
        {
            Data->rx1 = instance->rotation.x;
            Data->ry1 = instance->rotation.y;
            Data->rz1 = instance->rotation.z;

            Data->px = instance->position.x;
            Data->py = instance->position.y;
            Data->pz = instance->position.z;
        }

        instance->rotation.x = Data->rx1 + (((ea.x - Data->rx1) * Data->Step) / Data->Steps);
        instance->rotation.y = Data->ry1 + (((ea.y - Data->ry1) * Data->Step) / Data->Steps);
        instance->rotation.z = Data->rz1 + (((ea.z - Data->rz1) * Data->Step) / Data->Steps);

        ApplyMatrix(&instance->matrix[2], (SVECTOR *)&model->segmentList[2].px, (VECTOR *)&Position);

        Position.x = forceMatrix->t[0] - Position.x;
        Position.y = forceMatrix->t[1] - Position.y;
        Position.z = forceMatrix->t[2] - Position.z;

        instance->position.x = (short)(Data->px + (((Position.x - Data->px) * Data->Step) / Data->Steps));
        instance->position.y = (short)(Data->py + (((Position.y - Data->py) * Data->Step) / Data->Steps));
        instance->position.z = (short)(Data->pz + (((Position.z - Data->pz) * Data->Step) / Data->Steps));

        Data->Step++;
    }

    if ((Data->Mode & 0x20000))
    {
        if (Data->Steps == 0)
        {
            scale = 4096;
        }
        else
        {
            scale = 4096 - ((Data->Step << 12) / Data->Steps);
        }

        instance->scale.x = instance->scale.y = instance->scale.z = scale;
    }

    if (Data->Step == Data->Steps)
    {
        if ((Data->Mode & 0x20000))
        {
            INSTANCE_KillInstance(instance);
        }
        else
        {
            Data->Mode = (Data->Mode & ~0x1024005) | 0x1080;

            TurnOffCollisionPhysOb(instance, 7);

            INSTANCE_LinkToParent(instance, Force, Data->LinkNode);

            Data->Force = NULL;

            Data->Step = 0;
            Data->Steps = 0;

            instance->xVel = 0;
            instance->yVel = 0;
            instance->zVel = 0;

            instance->xAccl = 0;
            instance->yAccl = 0;
            instance->zAccl = 0;
        }
    }
}

void ExecuteFollow(Instance *instance)
{
    PhysObData *Data;
    Instance *Force;

    Data = (PhysObData *)instance->extraData;

    Force = Data->Force;

    instance->position.x = Force->matrix[Data->Segment1].t[0] + Data->px;
    instance->position.y = Force->matrix[Data->Segment1].t[1] + Data->py;
}

void PHYSOB_Normalize(SVector *v)
{
    long len;

    len = MATH3D_SquareLength(v->x, v->y, v->z);

    if (len != 0)
    {
        len = MATH3D_FastSqrt0(len);
    }
    else
    {
        len = 1;
    }

    v->x = ((v->x << 12) / len);
    v->y = ((v->y << 12) / len);
    v->z = ((v->z << 12) / len);
}

int PHYSOB_MoveTowardsAlign(Instance *instance, SVector *orgVec, SVector *endVec)
{
    MATRIX xform;
    MATRIX dest;
    MATRIX *src;
    G2Quat rot;
    long len;
    int theta;
    int sintheta;
    G2EulerAngles ea;
    short temp; // not from decls.h

    temp = ((long)gameTrackerX.timeMult * 50) >> 12;

    PHYSOB_Normalize(orgVec);
    PHYSOB_Normalize(endVec);

    src = instance->matrix;

    rot.x = ((orgVec->y * endVec->z) - (orgVec->z * endVec->y)) / 4096;
    rot.y = ((orgVec->z * endVec->x) - (orgVec->x * endVec->z)) / 4096;
    rot.z = ((orgVec->x * endVec->y) - (orgVec->y * endVec->x)) / 4096;

    theta = MATH3D_racos_S(((orgVec->x * endVec->x) + (orgVec->y * endVec->y) + (orgVec->z * endVec->z)) / 4096) / 2;

    if (theta < -temp)
    {
        theta = -temp;
    }

    if (theta > temp)
    {
        theta = temp;
    }

    len = MATH3D_SquareLength(rot.x, rot.y, rot.z);

    if (len <= 0)
    {
        len = 4096;
    }
    else
    {
        len = MATH3D_FastSqrt0(len);
    }

    sintheta = rsin(theta);

    rot.x = (rot.x * sintheta) / len;
    rot.y = (rot.y * sintheta) / len;
    rot.z = (rot.z * sintheta) / len;
    rot.w = rcos(theta);

    G2Quat_ToMatrix_S(&rot, (G2Matrix *)&xform);

    MulMatrix0(&xform, src, &dest);

    G2EulerAngles_FromMatrix(&ea, (G2Matrix *)&dest, 21);

    instance->rotation.x = ea.x;
    instance->rotation.y = ea.y;
    instance->rotation.z = ea.z;

    return theta < 20;
}

INCLUDE_ASM("asm/nonmatchings/Game/PHYSOBS", PHYSOB_ReAlignFalling);

void DropPhysOb(Instance *instance, int flags)
{
    PhysObData *Data;
    Instance *parent;
    int temp; // not from decls.h

    parent = instance->LinkParent;

    Data = (PhysObData *)instance->extraData;

    if (parent != NULL)
    {
        INSTANCE_UnlinkFromParent(instance);
    }
    else
    {
        Data->Mode &= ~0x800000;
    }

    PHYSOB_CheckDroppedLineCollision(instance, parent);

    if ((flags & 0x2))
    {
        Data->Mode = (Data->Mode & ~0x90) | 0x1000;

        instance->zAccl = 0;
    }
    else
    {
        if (!(flags & 0x4))
        {
            temp = 0;

            if (parent != NULL)
            {
                temp = -900;
            }

            PHYSOB_ReAlignFalling(instance, temp);
        }

        Data->throwingInstance = parent;

        Data->Mode = (Data->Mode & ~0x1090) | 0x400004;

        instance->zAccl = -10;
    }

    instance->xVel = 0;
    instance->yVel = 0;
    instance->zVel = 0;

    instance->xAccl = 0;
    instance->yAccl = 0;
}

void PhysicalRelocateTune(Object *object, long offset)
{
    PhysObWeaponProperties *properties;
    PhysObLight *pLight;

    properties = (PhysObWeaponProperties *)object->data;

    if (properties != NULL)
    {
        if (properties->Properties.family == 0)
        {
            pLight = (PhysObLight *)OFFSET_DATA(properties->WeaponAttributes.Light, offset);

            properties->WeaponAttributes.Light = pLight;

            if (pLight != NULL)
            {
                pLight->lightTable = (LightTableEntry *)OFFSET_DATA(pLight->lightTable, offset);
            }

            properties->WeaponAttributes.splinter.splinterData = (void *)OFFSET_DATA(properties->WeaponAttributes.splinter.splinterData, offset);
        }

        if (properties->Properties.family == 3)
        {
            PhysObInteractProperties *prop;
            PhysObWeaponAttributes *weapon;

            prop = (PhysObInteractProperties *)object->data;

            prop->weapon = (PhysObWeaponAttributes *)OFFSET_DATA(prop->weapon, offset);

            weapon = prop->weapon;

            if (weapon != NULL)
            {
                weapon->Light = (PhysObLight *)OFFSET_DATA(weapon->Light, offset);

                pLight = weapon->Light;

                if (pLight != NULL)
                {
                    pLight->lightTable = (LightTableEntry *)OFFSET_DATA(pLight->lightTable, offset);
                }

                weapon->splinter.splinterData = (void *)OFFSET_DATA(weapon->splinter.splinterData, offset);
            }
        }

        if (properties->Properties.family == 7)
        {
            PhysObProjectileProperties *prop;
            PhysObProjData *temp; // not from decls.h

            prop = (PhysObProjectileProperties *)object->data;

            prop->data = (PhysObProjData *)OFFSET_DATA(prop->data, offset);

            if (prop->data != NULL)
            {
                int i;

                temp = prop->data;

                for (i = 0; i < prop->totalProjectiles; i++)
                {
                    temp[i].weapon = (PhysObWeaponAttributes *)OFFSET_DATA(temp[i].weapon, offset);

                    if (temp[i].weapon != NULL)
                    {
                        pLight = temp[i].weapon->Light;

                        if (pLight != NULL)
                        {
                            pLight->lightTable = (LightTableEntry *)OFFSET_DATA(pLight->lightTable, offset);
                        }
                    }
                }
            }
        }
    }
}

int PHYSOBS_CheckObjectAxisAlignment(MATRIX *m0, MATRIX *m1, SVector *axis)
{
    VECTOR r0;
    VECTOR r1;
    SVector v0;
    SVector v1;

    ApplyMatrix(m0, (SVECTOR *)axis, &r0);
    ApplyMatrix(m1, (SVECTOR *)axis, &r1);

    v0.x = r0.vx;
    v0.y = r0.vy;
    v0.z = r0.vz;

    v1.x = r1.vx;
    v1.y = r1.vy;
    v1.z = r1.vz;

    return (short)(((v0.x * v1.x) + (v0.y * v1.y) + (v0.z * v1.z)) >> 12);
}

int PHYSOB_CheckSlide(Instance *instance, int x, int y, evPhysicsSlideData **data)
{
    *data = (evPhysicsSlideData *)SetPhysicsSlideData(1, (short)(x * 704), (short)(y * 704), 0, 640, 640, -160);

    return PhysicsCheckSliding(instance, (intptr_t)*data, 1);
}

int PHYSOB_CheckSlide2(Instance *instance, int x, int y, evPhysicsSlideData **data)
{
    *data = (evPhysicsSlideData *)SetPhysicsSlideData(1, (short)(x * 704), (short)(y * 704), 0, 640, 640, -150);

    return PhysicsCheckSliding(instance, (intptr_t)*data, 1);
}

int PHYSOB_CheckDropOnSlope(Instance *instance, int x, int y, evPhysicsSlideData **data)
{
    *data = (evPhysicsSlideData *)SetPhysicsSlideData(1, (short)(x * 704), (short)(y * 704), 0, 2560, 640, -160);

    return PhysicsCheckSliding(instance, (intptr_t)*data, 1);
}

int PHYSOB_CheckSlideNormal(Instance *instance, int x, int y, evPhysicsSlideData **data)
{
    PHYSOB_NormalToAxis((short *)&x, (short *)&y);

    return PHYSOB_CheckSlide2(instance, x, y, data);
}

void PHYSOB_NormalToAxis(short *x, short *y)
{
    int x1;
    int y1;

    x1 = *x;
    y1 = *y;

    if (x1 >= 1025)
    {
        *x = 1;
    }
    else if (x1 < -1024)
    {
        *x = -1;
    }
    else
    {
        *x = 0;
    }

    if (y1 >= 1025)
    {
        *y = 1;
        return;
    }

    if (y1 < -1024)
    {
        *y = -1;
        return;
    }

    *y = 0;
}
