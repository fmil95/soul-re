#include "Game/MATH3D.h"
#include "Game/GAMELOOP.h"
#include "Game/G2/QUATG2.h"
#include "Game/SPLINE.h"
#include "Game/SCRIPT.h"
#include "Game/G2/QUATG2.h"
#include "Game/HASM.h"
#include "Game/INSTANCE.h"
#include "Game/STREAM.h"
#include "Game/SOUND.h"
#include "Game/SIGNAL.h"
#include "Game/FX.h"
#include "Game/GENERIC.h"

void SCRIPT_CombineEulerAngles(Rotation *combinedRotation, Rotation *inputRotation1, Rotation *inputRotation2)
{
    MATRIX rotMatrix1;
    MATRIX rotMatrix2;
    G2EulerAngles ea;

    RotMatrix((SVECTOR *)inputRotation1, &rotMatrix1);
    RotMatrix((SVECTOR *)inputRotation2, &rotMatrix2);

    MulMatrix2(&rotMatrix2, &rotMatrix1);

    G2EulerAngles_FromMatrix(&ea, (G2Matrix *)&rotMatrix1, 21);

    COPY_SVEC(Rotation, combinedRotation, G2EulerAngles, &ea);
}

void SCRIPT_InstanceSplineInit(Instance *instance)
{
    Spline *spline;
    RSpline *rspline;
    Spline *sspline;
    MultiSpline *multi;
    SplineDef *sd;
    SplineDef *rsd;
    SplineDef *ssd;
    unsigned long isParent;
    unsigned long isClass;

    multi = SCRIPT_GetMultiSpline(instance, &isParent, &isClass);

    if (multi != NULL)
    {
        sd = SCRIPT_GetPosSplineDef(instance, multi, isParent, isClass);
        rsd = SCRIPT_GetRotSplineDef(instance, multi, isParent, isClass);
        ssd = SCRIPT_GetScaleSplineDef(instance, multi, isParent, isClass);

        spline = multi->positional;
        rspline = multi->rotational;
        sspline = multi->scaling;

        if (rspline != NULL)
        {
            G2Quat *q;
            G2EulerAngles ea;

            q = SplineGetFirstRot(rspline, rsd);

            if ((isParent == 0) && (isClass == 0))
            {
                if (1)
                {
                    G2Quat_ToMatrix_S(q, (G2Matrix *)&multi->curRotMatrix);

                    q = (G2Quat *)&instance->rotation;

                    if (instance->intro != NULL)
                    {
                        MATRIX introTransform;

                        RotMatrix((SVECTOR *)&instance->intro->rotation, &introTransform);

                        MulMatrix0(&multi->curRotMatrix, &introTransform, &multi->curRotMatrix);
                    }

                    instance->flags |= 0x1;
                }
            }
            else
            {
                Rotation combinedRotation;

                G2Quat_ToEuler(q, &ea, 0);

                instance->rotation.x = ea.x;
                instance->rotation.y = ea.y;
                instance->rotation.z = ea.z;

                SCRIPT_CombineEulerAngles((Rotation *)&combinedRotation, (Rotation *)&instance->rotation, &instance->intro->rotation);

                COPY_SVEC(Rotation, &instance->rotation, Rotation, &combinedRotation);
            }
        }

        if (spline != NULL)
        {
            SVector *start_point;

            start_point = SplineGetFirstPoint(spline, sd);

            if (start_point != NULL)
            {
                if (isClass != 0)
                {
                    ADD_SVEC(Position, &instance->position, Position, &instance->initialPos, SVector, start_point);
                }
                else
                {
                    instance->position.x = start_point->x;
                    instance->position.y = start_point->y;
                    instance->position.z = start_point->z;
                }
            }
        }

        if (sspline != NULL)
        {
            SVector *start_point;

            start_point = SplineGetFirstPoint(sspline, ssd);

            if (start_point != NULL)
            {
                instance->scale.x = start_point->x;
                instance->scale.y = start_point->y;
                instance->scale.z = start_point->z;
            }
        }
    }
}

short SCRIPTCountFramesInSpline(Instance *instance)
{

    SplineKey *key;
    Spline *spline;
    RSpline *rspline;
    short kf;
    short frames;

    kf = 0;
    frames = 0;

    spline = ScriptGetPosSpline(instance);

    if (spline != NULL)
    {
        for (key = spline->key; kf < spline->numkeys; kf++, key++)
        {
            frames += key->count;
        }
    }
    else
    {

        SplineRotKey *rkey;

        rspline = ScriptGetRotSpline(instance);
        if (rspline != NULL)
        {
            for (rkey = rspline->key; kf < rspline->numkeys; kf++, rkey++)
            {
                frames += rkey->count;
            }
        }
        else
        {

            MultiSpline *multi;
            multi = SCRIPT_GetMultiSpline(instance, 0, 0);

            if (multi != NULL)
            {
                spline = multi->scaling;
                for (key = spline->key; kf < spline->numkeys; kf++, key++)
                {
                    frames += key->count;
                }
            }
        }
    }
    return frames;
}

Spline *ScriptGetPosSpline(Instance *instance)
{
    MultiSpline *multi;

    multi = SCRIPT_GetMultiSpline(instance, NULL, NULL);

    if (multi != NULL)
    {
        return multi->positional;
    }

    return NULL;
}

RSpline *ScriptGetRotSpline(Instance *instance)
{
    MultiSpline *multi;

    multi = SCRIPT_GetMultiSpline(instance, NULL, NULL);

    if (multi != NULL)
    {
        return multi->rotational;
    }

    return NULL;
}

int SCRIPT_GetSplineFrameNumber(Instance *instance, SplineDef *splineDef)
{
    if (ScriptGetPosSpline(instance) != NULL)
    {
        return SplineGetFrameNumber(ScriptGetPosSpline(instance), splineDef);
    }
    else
    {
        return SplineGetFrameNumber((Spline *)ScriptGetRotSpline(instance), splineDef);
    }
}

MultiSpline *SCRIPT_GetMultiSpline(Instance *instance, unsigned long *isParent, unsigned long *isClass)
{
    MultiSpline *multi;

    multi = NULL;

    if (isParent != NULL)
    {
        *isParent = 0;
    }

    if (isClass != NULL)
    {
        *isClass = 0;
    }

    if ((instance != NULL) && (instance->intro != NULL) && (instance->intro->multiSpline != NULL))
    {
        multi = instance->intro->multiSpline;

        if (((instance->flags & 0x100002) == 0x2) && (isParent != NULL))
        {
            *isParent = 1;
        }
    }

    if ((multi == NULL) && (instance->object->modelList[0] != NULL))
    {
        multi = instance->object->modelList[0]->multiSpline;

        if (isClass != NULL)
        {
            *isClass = 1;
        }
    }

    return multi;
}

SplineDef *SCRIPT_GetPosSplineDef(Instance *instance, MultiSpline *multi, unsigned long isParent, unsigned long isClass)
{
    SplineDef *splineDef;

    if ((isParent != 0) || (isClass != 0))
    {
        splineDef = (SplineDef *)&instance->work0;

        return splineDef;
    }

    if (multi == NULL)
    {
        return NULL;
    }

    return &multi->curPositional;
}

SplineDef *SCRIPT_GetRotSplineDef(Instance *instance, MultiSpline *multi, unsigned long isParent, unsigned long isClass)
{
    SplineDef *splineDef;

    if ((isParent != 0) || (isClass != 0))
    {
        splineDef = (SplineDef *)&instance->work2;

        return splineDef;
    }

    if (multi == NULL)
    {
        return NULL;
    }

    return &multi->curRotational;
}

SplineDef *SCRIPT_GetScaleSplineDef(Instance *instance, MultiSpline *multi, unsigned long isParent, unsigned long isClass)
{
    SplineDef *splineDef;

    if ((isParent != 0) || (isClass != 0))
    {
        splineDef = (SplineDef *)&instance->work4;

        return splineDef;
    }

    if (multi == NULL)
    {
        return NULL;
    }

    return &multi->curScaling;
}

void SCRIPT_RelativisticSpline(Instance *instance, SVector *point)
{
    SVector pt;       

    if ((instance->intro != NULL) && ((instance->intro->rotation.x != 0) || (instance->intro->rotation.y != 0) || (instance->intro->rotation.z != 0))) 
    {
        MATRIX segMatrix; 
        Position newPt;  
        Position localPt; 
        
        COPY_SVEC(SVector, &pt, SVector, point);
        
        localPt.x = pt.x;
        localPt.y = pt.y;
        localPt.z = pt.z;
        
        RotMatrix((SVECTOR*)&instance->intro->rotation, &segMatrix);
       
        segMatrix.t[2] = 0;
        segMatrix.t[1] = 0;
        segMatrix.t[0] = 0;
       
        newPt.x = ((localPt.x * segMatrix.m[0][0]) >> 12) + ((localPt.y * segMatrix.m[0][1]) >> 12) + ((localPt.z * segMatrix.m[0][2]) >> 12);
        newPt.y = ((localPt.x * segMatrix.m[1][0]) >> 12) + ((localPt.y * segMatrix.m[1][1]) >> 12) + ((localPt.z * segMatrix.m[1][2]) >> 12);
        newPt.z = ((localPt.x * segMatrix.m[2][0]) >> 12) + ((localPt.y * segMatrix.m[2][1]) >> 12) + ((localPt.z * segMatrix.m[2][2]) >> 12);
       
        ADD_SVEC(Position, &instance->position, Position, &newPt, Position, &instance->initialPos);
    }
    else 
    {
        ADD_SVEC(Position, &instance->position, Position, &instance->initialPos, SVector, point);
    }
}

void SCRIPT_InstanceSplineSet(Instance *instance, short frameNum, SplineDef *splineDef, SplineDef *rsplineDef, SplineDef *ssplineDef)
{
    Spline *spline;         
    RSpline *rspline;       
    Spline *sspline;       
    MultiSpline *multi;      
    unsigned long isClass;   
    unsigned long isParent;   
    SVector point;           

    multi = SCRIPT_GetMultiSpline(instance, &isParent, &isClass);
    
    if (multi != NULL)
    {
        if (frameNum == -1) 
        {
            frameNum = SCRIPTCountFramesInSpline(instance);
        }
        
        if (((isParent != 0) || (isClass != 0)) || ((splineDef == NULL) && (rsplineDef == NULL) && (ssplineDef == NULL))) 
        {
            splineDef = SCRIPT_GetPosSplineDef(instance, multi, isParent, isClass);
            rsplineDef = SCRIPT_GetRotSplineDef(instance, multi, isParent, isClass);
            ssplineDef = SCRIPT_GetScaleSplineDef(instance, multi, isParent, isClass);
        }
        
        spline = multi->positional;
        rspline = multi->rotational;
        sspline = multi->scaling;
        
        if ((splineDef != NULL) && (spline != NULL))
        {
            SplineSetDef2FrameNumber(spline, splineDef, frameNum);
            
            SplineGetData(spline, splineDef, &point);
            
            if (isClass != 0) 
            {
                SCRIPT_RelativisticSpline(instance, &point);
            } 
            else 
            {
                instance->position.x = point.x;
                instance->position.y = point.y;
                instance->position.z = point.z;
            }
        }
        
        if ((rsplineDef != NULL) && (rspline != NULL))
        {
            SplineSetDef2FrameNumber((Spline*)rspline, rsplineDef, frameNum);
            
            if ((instance->flags & 0x1)) 
            {
                G2Quat q; 
                
                if (SplineGetQuatData((Spline*)rspline, rsplineDef, &q) != 0) 
                {
                    MATRIX introTransform;   
                    
                    G2Quat_ToMatrix_S(&q, (G2Matrix*)&multi->curRotMatrix);
                    
                    if (instance->intro != NULL)
                    {
                        RotMatrix((SVECTOR*)&instance->intro->rotation, &introTransform);
                        MulMatrix0(&multi->curRotMatrix, &introTransform, &multi->curRotMatrix);
                    }
                }
            } 
            else 
            {
                Rotation rot;             
                
                if (SplineGetData((Spline*)rspline, rsplineDef, &rot) != 0) 
                {
                    Rotation combinedRotation; 
                    
                    instance->rotation.x = rot.x;
                    instance->rotation.y = rot.y;
                    instance->rotation.z = rot.z;
                    
                    SCRIPT_CombineEulerAngles(&combinedRotation, &instance->rotation, &instance->intro->rotation);
    
                    COPY_SVEC(Rotation, &instance->rotation, Rotation, &combinedRotation); 
                }
            }
        }
        
        if ((ssplineDef != NULL) && (sspline != NULL)) 
        {
            SVector scale;            
            
            SplineSetDef2FrameNumber(sspline, ssplineDef, frameNum);
            
            SplineGetData(sspline, ssplineDef, &scale);
            
            instance->scale.x = scale.x;
            instance->scale.y = scale.z;
            instance->scale.z = scale.y;
        }
    }
}

long SCRIPT_SplineProcess(Instance *instance, MultiSpline *multi, SplineDef *splineDef, SplineDef *rsplineDef, SplineDef *ssplineDef, int direction, int isClass)
{
    long retVal; 
    Spline *spline; 
    RSpline *rspline;
    Spline *sspline; 
    SVector *point; 
    Rotation rot;
    long timeOff; 
    SVector temp; // not from decls.h

    timeOff = gameTrackerX.timeMult;
    
    retVal = -1;
    
    spline = multi->positional;
    sspline = multi->scaling;
    rspline = multi->rotational;
    
    point = NULL;
    
    if (sspline != NULL) 
    {
        //SVector pt; // unused
        
        if (direction > 0)
        {
            point = SplineGetNextPoint(sspline, ssplineDef);
        } 
        else if (direction < 0) 
        {
            point = SplineGetPreviousPoint(sspline, ssplineDef);
        }
        else if (SplineGetData(sspline, ssplineDef, &temp) != 0) 
        {
            point = &temp;
        }
        
        if (point != NULL) 
        {
            retVal = 0;
            
            instance->scale.x = point->x;
            instance->scale.y = point->z;
            instance->scale.z = point->y;
        } 
        else
        {
            retVal = 1;
        }
    }
    
    if (rspline != NULL) 
    {
        retVal = 0;
        
        if (direction > 0) 
        {
            retVal = SplineGetOffsetNext((Spline*)rspline, rsplineDef, timeOff) == 0;
        } 
        else if ((direction < 0) && (SplineGetOffsetPrev((Spline*)rspline, rsplineDef, timeOff) == 0)) 
        {
            retVal = 1;
        }
        
        if (retVal == 0) 
        {
            G2Quat q; 
            
            if ((instance->flags & 0x1)) 
            {
                if (SplineGetQuatData((Spline*)rspline, rsplineDef, &q) != 0) 
                {
                    MATRIX introTransform;
                    
                    G2Quat_ToMatrix_S(&q, (G2Matrix*)&multi->curRotMatrix);
                    
                    if (instance->intro != NULL)
                    {
                        RotMatrix((SVECTOR*)&instance->intro->rotation, &introTransform);
                        MulMatrix0(&multi->curRotMatrix, &introTransform, &multi->curRotMatrix);
                    }
                } 
                else 
                {
                    retVal = 1;
                }
            } 
            else if (SplineGetData((Spline*)rspline, rsplineDef, &rot) != 0)
            {
                Rotation combinedRotation; 
                
                instance->rotation.x = rot.x;
                instance->rotation.y = rot.y;
                instance->rotation.z = rot.z;
                
                SCRIPT_CombineEulerAngles(&combinedRotation, &instance->rotation, &instance->intro->rotation);
                
                COPY_SVEC(Rotation, &instance->rotation, Rotation, &combinedRotation);
            } 
            else 
            {
                retVal = 1;
            }
        }
    }
    
    if (spline != NULL) 
    {
        SVector pt; 
        
        if (direction > 0)
        {
            point = SplineGetOffsetNextPoint(spline, splineDef, timeOff);
        }
        else if (direction < 0) 
        {
            point = SplineGetOffsetPreviousPoint(spline, splineDef, timeOff);
        } 
        else if (SplineGetData(spline, splineDef, &pt) != 0) 
        {
            point = &pt;
        }
        
        retVal = 1;
        
        if (point != NULL) 
        {
            retVal = 0;
            
            if (isClass != 0) 
            {
                SCRIPT_RelativisticSpline(instance, point);
            } 
            else 
            {   
                instance->position.x = point->x;
                instance->position.y = point->y;
                instance->position.z = point->z;
            }
        }
    }
    
    return retVal;
}

long SCRIPT_InstanceSplineProcess(Instance *instance, SplineDef *splineDef, SplineDef *rsplineDef, SplineDef *ssplineDef, int direction)
{
    MultiSpline *multi;
    unsigned long isClass;
    unsigned long isParent;

    multi = SCRIPT_GetMultiSpline(instance, &isParent, &isClass);

    if (multi != NULL)
    {
        if (((isParent != 0) || (isClass != 0)) || ((splineDef == NULL) && (rsplineDef == NULL) && (ssplineDef == NULL)))
        {
            splineDef = SCRIPT_GetPosSplineDef(instance, multi, isParent, isClass);
            rsplineDef = SCRIPT_GetRotSplineDef(instance, multi, isParent, isClass);
            ssplineDef = SCRIPT_GetScaleSplineDef(instance, multi, isParent, isClass);
        }

        return SCRIPT_SplineProcess(instance, multi, splineDef, rsplineDef, ssplineDef, direction, isClass);
    }

    return -1;
}

void SCRIPT_FadeOutProcess(Instance *instance)
{
    instance->fadeValue += (gameTrackerX.timeMult / 32);

    if (instance->fadeValue >= 4096)
    {
        INSTANCE_PlainDeath(instance);
    }
}

void ScriptKillInstance(Instance *instance, int effect)
{
    Object *object;

    object = instance->object;

    if ((object->oflags2 & 0x4))
    {
        SOUND_EndInstanceSounds(object->soundData, instance->soundInstanceTbl);
    }

    if (((object->oflags & 0x40000000)) && (instance->introData != NULL))
    {
        Signal *temp; // not from decls.h

        temp = instance->introData;

        if (temp->id != 0)
        {
            SIGNAL_HandleSignal(instance, (Signal *)(temp->id + 8), 0); // TODO: find the union from the Signal struct that would logically fit in here
        }
    }

    if (effect == 1)
    {
        Object *object;

        object = instance->object;

        if ((object->oflags & 0x8000))
        {
            effect = 5;
        }
    }

    switch (effect)
    {
    case 6:
    {
        SVECTOR v;

        v.vy = v.vz = v.vx = -2;

        FX_Build(instance, NULL, &v, NULL, gFXT, NULL, NULL);
        break;
    }
    case 2:
    case 5:
    {
        FXSplinter *splinterData;
        int facadeFlg;
        short shardFlags;

        splinterData = NULL;

        shardFlags = facadeFlg = 0;

        if ((instance->processFunc == GenericProcess) || (instance->processFunc == NULL))
        {
            GenericTune *tune;

            tune = object->data;

            if (tune != NULL)
            {
                splinterData = tune->shatterData;

                facadeFlg = tune->flags & 0x1;

                if ((tune->flags & 0x2))
                {
                    shardFlags = 0x10;
                }
            }
        }

        _FX_BuildSplinters(instance, NULL, NULL, NULL, splinterData, gFXT, NULL, (facadeFlg != 0) ? FX_DFacadeProcess : 0, shardFlags);
        break;
    }
    case 7:
        instance->processFunc = SCRIPT_FadeOutProcess;
        return;
    case 0:
    case 1:
    case 3:
    case 4:
        break;
    }

    INSTANCE_PlainDeath(instance);
}
