#include "Game/STREAM.h"
#include "Game/STRMLOAD.h"
#include "Game/GAMELOOP.h"
#include "Game/MEMPACK.h"
#include "Game/INSTANCE.h"
#include "Game/OBTABLE.h"
#include "Game/PSX/AADLIB.h"
#include "Game/TIMER.h"
#include "Game/LIGHT3D.h"
#include "Game/SAVEINFO.h"
#include "Game/EVENT.h"
#include "Game/PLAN/PLANAPI.h"
#include "Game/VRAM.h"
#include "Game/FX.h"
#include "Game/RELMOD.h"
#include "Game/SPLINE.h"
#include "Game/CAMERA.h"
#include "Game/GLYPH.h"
#include "Game/LOAD3D.h"
#include "Game/DEBUG.h"
#include "Game/SIGNAL.h"
#include "Game/MATH3D.h"
#include "Game/PLAN/PLAN.h"
#include "Game/PSX/SUPPORT.h"
#include "Game/COLLIDE.h"
#include "Game/SOUND.h"
#include "Game/VM.h"
#include "Game/GAMEPAD.h"
#include "Game/FONT.h"
#include "Game/DRAW.h"
#include "Game/LIST.h"
#include "Game/HASM.h"
#include "Game/PSX/MAIN.h"

WarpGateLoadInformation WarpGateLoadInfo;

long CurrentWarpNumber = 0;

WarpRoom WarpRoomArray[14] = {
    {"under3", NULL},
    {"clfpil3", NULL},
    {"huba8", NULL},
    {"out10", NULL},
    {"city6", NULL},
    {"cathy58", NULL},
    {"cathy62", NULL},
    {"stone15", NULL},
    {"add6", NULL},
    {"aluka43", NULL},
    {"fill3", NULL},
    {"hubb4", NULL},
    {"oracle1", NULL},
    {"chrono17", NULL},
};

TFace *MORPH_SavedFace;

Level *MORPH_SavedLevel;

short MORPH_Component[3];

short MORPH_Track[2];

short RENDER_currentStreamUnitID;

static int s_zval;

void RelocateLevel(Level *level, SVector *offset);
void RelocateLevelWithInstances(Level *level, SVector *offset);
void RelocateTerrain(Terrain *terrain, SVector *offset);
void RelocateVMObjects(VMObject *vobjectlist, long numvmobjs, SVector *offset);
void RelocateCameras(CameraKey *cameraList, long numCameras, SVector *offset);
void RelocateBGObjects(BGObject *BGObjList, long numBGObjs, SVector *offset);
void RelocateSavedCameras(Camera *camera, Level *level, SVector *offset);
void RelocatePlanMarkers(PlanMkr *planMkrList, int numPlanMkrs, SVector *offset);
void RelocateSFXMarkers(SFXMkr *sfxMkrList, int numSFXMkrs, SVector *offset);
void RelocateInstances(SVector *offset);
void RelocatePlanPool(PlanningNode *planPool, SVector *offset);
void RelocateStreamPortals(StreamUnitPortal *StreamUnitList, int NumStreamUnits, SVector *offset);

void STREAM_FillOutFileNames(char *baseAreaName, char *dramName, char *vramName, char *sfxName)
{
    char text[16];
    char *number;

    strcpy(text, baseAreaName);

    number = strpbrk(text, "0123456789");

    if (number != 0)
    {
        *number = 0;
    }

    if (dramName != NULL)
    {
        sprintf(dramName, "\\kain2\\area\\%s\\bin\\%s.drm", text, baseAreaName);
    }

    if (vramName != NULL)
    {
        sprintf(vramName, "\\kain2\\area\\%s\\bin\\%s.crm", text, baseAreaName);
    }

    if (sfxName != NULL)
    {
        sprintf(sfxName, "\\kain2\\area\\%s\\bin\\%s.snf", text, baseAreaName);
    }
}

void STREAM_AbortAreaLoad(char *baseAreaName)
{
    char vramName[80];

    STREAM_FillOutFileNames(baseAreaName, NULL, vramName, NULL);

    LOAD_AbortDirectoryChange(baseAreaName);

    LOAD_AbortFileLoad(vramName, (void *)VRAM_LoadReturn);
}

void STREAM_Init()
{
    int i;

    for (i = 0; i < 16; i++)
    {
        StreamTracker.StreamList[i].used = 0;

        StreamTracker.StreamList[i].flags = 0;

        StreamTracker.StreamList[i].StreamUnitID = 0;
    }
}

int FindObjectName(char *name)
{
    int i;
    ObjectTracker *otr;

    otr = gameTrackerX.GlobalObjects;

    for (i = 0; i < 48; i++, otr++)
    {
        if ((otr->objectStatus != 0) && (strcmpi(otr->name, name) == 0))
        {
            return i;
        }
    }

    return -1;
}

ObjectTracker *FindObjectInTracker(Object *object)
{
    int i;
    ObjectTracker *otr;

    otr = gameTrackerX.GlobalObjects;

    for (i = 0; i < 48; i++, otr++)
    {
        if ((otr->objectStatus != 0) && (otr->object == object))
        {
            return otr;
        }
    }

    return NULL;
}

StreamUnit *FindStreamUnitFromLevel(Level *level)
{
    StreamUnit *ret;
    long i;

    ret = NULL;

    for (i = 0; i < 16; i++)
    {
        if ((StreamTracker.StreamList[i].used == 2) && (StreamTracker.StreamList[i].level == level))
        {
            ret = &StreamTracker.StreamList[i];
            break;
        }
    }

    return ret;
}

void STREAM_LoadObjectReturn(void *loadData, void *data, void *data2)
{
    Object *object;
    ObjectTracker *objectTracker;

    (void)data2;

    GetRCnt(0xF2000000);

    object = (Object *)loadData;

    objectTracker = (ObjectTracker *)data;

    gameTimer;

    if (((object->oflags & 0x8000000)) && (object->relocList != NULL) && (object->relocModule != NULL))
    {
        RELMOD_InitModulePointers((intptr_t)object->relocModule, (int *)object->relocList);
    }

    STREAM_PackVRAMObject(objectTracker);

    OBTABLE_InitAnimPointers(objectTracker);
    OBTABLE_InitObjectWithID(object);

    if ((object->oflags2 & 0x800000))
    {
        char objDsfxFileName[64];

        sprintf(objDsfxFileName, "\\kain2\\sfx\\object\\%s\\%s.snf", objectTracker->name, objectTracker->name);

        object->sfxFileHandle = 0;

        if (LOAD_DoesFileExist(objDsfxFileName) != 0)
        {
            object->sfxFileHandle = aadLoadDynamicSfx(objectTracker->name, 0, 0);
        }
    }

    if (objectTracker->vramBlock == NULL)
    {
        objectTracker->objectStatus = 2;
    }
    else
    {
        objectTracker->objectStatus = 4;
    }
}

void STREAM_DumpMonster(ObjectTracker *dumpee)
{
    Object *object;
    Instance *instance;

    object = dumpee->object;

    if (dumpee->vramBlock != NULL)
    {
        VRAM_ClearVramBlock((BlockVramEntry *)dumpee->vramBlock);
    }

    OBTABLE_RemoveObjectEntry(object);

    if (((object->oflags2 & 0x800000)) && (object->sfxFileHandle != 0))
    {
        aadFreeDynamicSfx(object->sfxFileHandle);
    }

    instance = gameTrackerX.instanceList->first;

    if (instance != NULL)
    {
        Instance *next;

        while (instance != NULL)
        {
            next = instance->next;

            if (object == instance->object)
            {
                INSTANCE_ReallyRemoveInstance(gameTrackerX.instanceList, instance, 0);
            }

            instance = next;
        }
    }

    MEMPACK_Free((char *)object);

    dumpee->object = NULL;
}

int STREAM_InList(char *name, char **nameList)
{
    char **mon;

    for (mon = nameList; *mon != NULL; mon++)
    {
        if (strcmpi(name, *mon) == 0)
        {
            return 1;
        }
    }

    return 0;
}

int STREAM_IsSpecialMonster(char *name)
{
    static char *mon[] = {"wallcr", "aluka", "ronin", "sluagh", "vwraith", NULL};

    return STREAM_InList(name, mon);
}

void STREAM_DumpSomeMonsters()
{
    ObjectTracker *otr;
    int i;

    otr = gameTrackerX.GlobalObjects;

    for (i = 0; i < 48; i++, otr++)
    {
        if (((otr->objectStatus == 2) && (otr->object != NULL)) && (STREAM_IsSpecialMonster((char *)otr) != 0))
        {
            STREAM_DumpMonster(otr);
        }
    }
}

void STREAM_NoMonsters()
{
    gameTrackerX.gameFlags |= 0x4000000;

    STREAM_DumpSomeMonsters();
}

void STREAM_YesMonsters()
{
    gameTrackerX.gameFlags &= ~0x4000000;
}

int STREAM_IsMonster(char *name)
{
    static char *monnames[] = {
        "skinner",
        "morlock",
        "wallcr",
        "ronin",
        "aluka",
        "sluagh",
        "vwraith",
        "vlgra",
        "vlgrb",
        "vlgrc",
        "hunter",
        "wrshp",
        "roninbss",
        "skinbos",
        "priests",
        "alukabss",
        "morboss",
        "soul",
        NULL,
    };

    return STREAM_InList(name, monnames);
}

int STREAM_TryAndDumpANonResidentObject()
{
    ObjectTracker *otr;
    int i;

    otr = gameTrackerX.GlobalObjects;

    for (i = 0; i < 48; i++, otr++)
    {
        if (STREAM_TryAndDumpNonResident(otr) != 0)
        {
            return i;
        }
    }

    return -1;
}

/*Unused*/ static char D_800D18C4[] = "LOW MEMORY: Dumping monster %s\n";

int InsertGlobalObject(char *name, GameTracker *gameTracker)
{
    char string[64];
    char vramname[64];
    int i;
    ObjectTracker *otr;

    i = -1;

    if (((!(gameTrackerX.gameFlags & 0x4000000)) || (STREAM_IsSpecialMonster(name) == 0)) && ((!(gameTracker->debugFlags2 & 0x8000)) || (STREAM_IsMonster(name) == 0)))
    {
        otr = gameTracker->GlobalObjects;

        for (i = 0; i < 48; i++, otr++)
        {
            if ((otr->objectStatus != 0) && (strcmpi(otr->name, name) == 0))
            {
                break;
            }
        }

        if (i == 48)
        {
            otr = gameTracker->GlobalObjects;

            for (i = 0; i < 48; i++, otr++)
            {
                if (otr->objectStatus == 0)
                {
                    break;
                }
            }

            if (i == 48)
            {
                i = STREAM_TryAndDumpANonResidentObject();

                if (i == -1)
                {
                    DEBUG_FatalError("The Object tracker is full MAX_OBJECTS=%d.\n", 48);
                }
            }

            sprintf(string, "\\kain2\\object\\%s\\%s.drm", name, name);
            sprintf(vramname, "\\kain2\\object\\%s\\%s.crm", name, name);

            strcpy(otr->name, name);

            otr->objectStatus = 1;

            LOAD_NonBlockingBinaryLoad(string, (void *)STREAM_LoadObjectReturn, (void *)otr, NULL, (void **)&otr->object, 1);

            otr->numInUse = 0;
            otr->numObjectsUsing = 0;
        }
    }

    return i;
}

ObjectTracker *STREAM_GetObjectTracker(char *name)
{
    int i;

    i = InsertGlobalObject(name, &gameTrackerX);

    if (i == -1)
    {
        return NULL;
    }
    else
    {
        return &gameTrackerX.GlobalObjects[i];
    }
}

void LoadLevelObjects(StreamUnit *stream)
{
    int objlist_pos;
    char name[20];
    Level *level;
    int i;

    STREAM_NextLoadAsNormal();

    objlist_pos = 0;

    while (((unsigned char *)stream->level->objectNameList)[objlist_pos] != 255)
    {
        strcpy(name, (char *)stream->level->objectNameList + objlist_pos);

        InsertGlobalObject(name, &gameTrackerX);

        objlist_pos += 16;
    }

    level = stream->level;

    for (i = 0; i < level->numIntros; i++)
    {
        if (FindObjectName(level->introList[i].name) != -1)
        {
            level->introList[i].flags &= ~0x4000;
        }
        else
        {
            level->introList[i].flags |= 0x4000;
        }
    }
}

long STREAM_IsAnInstanceUsingObject(Object *object)
{
    Instance *instance;
    Instance *next;
    long ret;

    instance = gameTrackerX.instanceList->first;

    ret = 0;

    while (instance != NULL)
    {
        next = instance->next;

        if (instance->object == object)
        {
            ret = 1;
            break;
        }

        instance = next;
    }

    return ret;
}

void STREAM_StreamLoadObjectAbort(void *loadData, void *data, void *data2)
{
    ObjectTracker *objectTracker;

    (void)data2;

    objectTracker = (ObjectTracker *)data;

    if (loadData != NULL)
    {
        MEMPACK_Free((char *)loadData);
    }

    objectTracker->objectStatus = 0;
}

void STREAM_DumpLoadingObjects()
{
    int i;
    ObjectTracker *tracker;

    tracker = gameTrackerX.GlobalObjects;

    for (i = 0; i < 48; i++, tracker++)
    {
        if (tracker->objectStatus == 1)
        {
            STREAM_DumpObject(tracker);
        }
    }
}

void STREAM_DumpObject(ObjectTracker *objectTracker)
{
    Object *object;
    char dramName[64];

    object = objectTracker->object;

    if (objectTracker->objectStatus == 1)
    {
        sprintf(dramName, "\\kain2\\object\\%s\\%s.drm", objectTracker->name, objectTracker->name);

        LOAD_AbortFileLoad(dramName, (void *)STREAM_StreamLoadObjectAbort);
    }
    else if (object != NULL)
    {
        if (!(object->oflags & 0x2000000))
        {
            if (objectTracker->vramBlock != NULL)
            {
                VRAM_ClearVramBlock((BlockVramEntry *)objectTracker->vramBlock);
            }

            if (((object->oflags2 & 0x800000)) && (object->sfxFileHandle != 0))
            {
                aadFreeDynamicSfx(object->sfxFileHandle);
            }

            OBTABLE_RemoveObjectEntry(object);

            MEMPACK_Free((char *)object);

            objectTracker->objectStatus = 0;
        }

        if (object == NULL)
        {
            objectTracker->objectStatus = 0;
        }
    }
    else
    {
        objectTracker->objectStatus = 0;
    }
}

int STREAM_IsObjectInAnyUnit(ObjectTracker *tracker)
{
    int d;
    unsigned char *objlist;

    for (d = 0; d < 16; d++)
    {
        if ((StreamTracker.StreamList[d].used == 2) && (StreamTracker.StreamList[d].level != NULL))
        {
            for (objlist = (unsigned char *)StreamTracker.StreamList[d].level->objectNameList; *objlist != 255; objlist += 16)
            {
                if (strcmpi(tracker->name, (char *)objlist) == 0)
                {
                    return 1;
                }
            }
        }
    }

    return 0;
}

void STREAM_RemoveAllObjectsNotInUse()
{
    int i;
    int abort;
    ObjectTracker *tracker;
    ObjectTracker *trackerList;

    trackerList = gameTrackerX.GlobalObjects;

    for (tracker = trackerList, i = 0; i < 48; i++, tracker++)
    {
        Object *object;

        if (tracker->objectStatus == 2)
        {
            object = tracker->object;

            if ((!(object->oflags & 0x2000000)) && (STREAM_IsObjectInAnyUnit(tracker) == 0) && (STREAM_IsAnInstanceUsingObject(object) == 0))
            {
                tracker->objectStatus = 3;
            }
        }
    }

    do
    {
        abort = 1;

        for (tracker = trackerList, i = 0; i < 48; i++, tracker++)
        {
            if (tracker->objectStatus == 3)
            {
                int j;

                for (j = 0; j < (char)tracker->numObjectsUsing; j++)
                {
                    if (trackerList[(int)tracker->objectsUsing[j]].objectStatus != 3)
                    {
                        tracker->objectStatus = 2;

                        abort = 0;
                        break;
                    }
                }
            }
        }
    } while (abort == 0);

    for (tracker = trackerList, i = 0; i < 48; i++, tracker++)
    {
        if (tracker->objectStatus == 3)
        {
            int j;
            ObjectTracker *otr;

            for (otr = trackerList, j = 0; j < 48; j++, otr++)
            {
                if ((otr->objectStatus == 1) || (otr->objectStatus == 2) || (otr->objectStatus == 4))
                {
                    int k;

                    for (k = 0; k < (char)otr->numObjectsUsing; k++)
                    {
                        if ((char)otr->objectsUsing[k] == i)
                        {
                            int l;

                            otr->numObjectsUsing--;

                            for (l = k; l < (char)otr->numObjectsUsing; l++)
                            {
                                otr->objectsUsing[l] = otr->objectsUsing[l + 1];
                            }

                            break;
                        }
                    }
                }
            }

            STREAM_DumpObject(tracker);
        }
    }

    for (tracker = trackerList, i = 0; i < 48; i++, tracker++)
    {
        if ((tracker->objectStatus == 1) && (STREAM_IsObjectInAnyUnit(tracker) == 0) && ((char)tracker->numObjectsUsing == 0))
        {
            STREAM_DumpObject(tracker);
        }
    }
}

void RemoveAllObjects(GameTracker *gameTracker)
{
    int i;
    ObjectTracker *tracker;

    for (i = 0, tracker = &gameTracker->GlobalObjects[i]; i < 48; i++, tracker++)
    {
        if (tracker->objectStatus != 0)
        {
            STREAM_DumpObject(tracker);
        }
    }
}

Level *STREAM_GetLevelWithID(long id)
{
    Level *retLevel;
    long i;

    retLevel = NULL;

    for (i = 0; i < 16; i++)
    {
        if ((StreamTracker.StreamList[i].used == 2) && (StreamTracker.StreamList[i].StreamUnitID == id))
        {
            retLevel = StreamTracker.StreamList[i].level;
            break;
        }
    }

    return retLevel;
}

StreamUnit *STREAM_GetStreamUnitWithID(long id)
{
    StreamUnit *retUnit;
    long i;

    retUnit = NULL;

    for (i = 0; i < 16; i++)
    {
        if ((StreamTracker.StreamList[i].used == 2) && (StreamTracker.StreamList[i].StreamUnitID == id))
        {
            retUnit = &StreamTracker.StreamList[i];
            break;
        }
    }

    return retUnit;
}

void STREAM_CalculateWaterLevel(Level *level)
{
    Terrain *terrain;
    int i;
    TFace *tface;
    long waterZLevel;

    waterZLevel = -32767;

    if (level->waterZLevel == 0)
    {
        if ((level->unitFlags & 0x1))
        {
            level->waterZLevel = 32767;
        }
        else
        {
            terrain = level->terrain;

            tface = (TFace *)terrain->faceList;

            for (i = terrain->numFaces; i > 0; i--, tface++)
            {
                if (((tface->attr & 0x8)) && (terrain->vertexList[tface->face.v0].vertex.z == terrain->vertexList[tface->face.v1].vertex.z) && (terrain->vertexList[tface->face.v0].vertex.z == terrain->vertexList[tface->face.v2].vertex.z))
                {
                    if (waterZLevel == -32767)
                    {
                        waterZLevel = terrain->vertexList[tface->face.v0].vertex.z;
                    }
                    else if (waterZLevel != terrain->vertexList[tface->face.v0].vertex.z)
                    {
                        break;
                    }
                }
            }

            if (waterZLevel == -32767)
            {
                level->waterZLevel = -32767;
            }
            else
            {
                level->waterZLevel = waterZLevel + level->terrain->BSPTreeArray[0].globalOffset.z;
            }
        }
    }
}

int STREAM_IsMorphInProgress()
{
    return gameTrackerX.gameData.asmData.MorphTime != 1000;
}

long STREAM_GetWaterZLevel(Level *level, Instance *instance)
{
    int waterZLevel;

    if ((instance->flags2 & 0x8000000))
    {
        if ((!(instance->object->oflags2 & 0x2000000)) && (gameTrackerX.gameData.asmData.MorphTime != 1000))
        {
            waterZLevel = level->waterZLevel;
        }
        else
        {
            waterZLevel = -32767;
        }
    }
    else if ((!(instance->object->oflags2 & 0x2000000)) && (gameTrackerX.gameData.asmData.MorphTime != 1000))
    {
        waterZLevel = -32767;
    }
    else
    {
        waterZLevel = level->waterZLevel;
    }

    return waterZLevel;
}

void STREAM_SetMainFog(StreamUnit *streamUnit)
{
    Level *level;

    level = streamUnit->level;

    if (gameTrackerX.gameData.asmData.MorphType != 0)
    {
        streamUnit->UnitFogNear = level->spectralFogNear;
        streamUnit->UnitFogFar = level->spectralFogFar;
    }
    else
    {
        streamUnit->UnitFogNear = level->holdFogNear;
        streamUnit->UnitFogFar = level->holdFogFar;
    }

    streamUnit->TargetFogNear = streamUnit->UnitFogNear;
    streamUnit->TargetFogFar = streamUnit->UnitFogFar;
}

void STREAM_SetStreamFog(StreamUnit *streamUnit, short fogNear, short fogFar)
{
    short unitFogFar;
    int unitFogHold;

    unitFogFar = FindStreamUnitFromLevel(gameTrackerX.level)->UnitFogFar;

    if (fogFar < unitFogFar)
    {
        unitFogFar = fogFar;
    }

    unitFogHold = unitFogFar - 2000;

    streamUnit->TargetFogFar = unitFogFar;
    streamUnit->UnitFogFar = unitFogFar;

    if (fogNear < unitFogHold)
    {
        unitFogHold = fogNear;
    }

    streamUnit->TargetFogNear = unitFogHold;
    streamUnit->UnitFogNear = unitFogHold;
}

void STREAM_ConnectStream(StreamUnit *streamUnit)
{
    StreamUnit *mainUnit;
    StreamUnitPortal *streamPortal;
    int numportals;
    StreamUnitPortal *streamPortal2;
    int numportals2;
    int i;
    int j;
    char text[16];
    char *commapos;
    int signalID;

    WARPGATE_UpdateAddToArray(streamUnit);

    if (gameTrackerX.StreamUnitID != streamUnit->StreamUnitID)
    {
        mainUnit = STREAM_GetStreamUnitWithID(gameTrackerX.StreamUnitID);

        numportals2 = *(long *)streamUnit->level->terrain->StreamUnits;

        streamPortal2 = (StreamUnitPortal *)((long *)streamUnit->level->terrain->StreamUnits + 1);

        for (j = 0; j < numportals2; j++, streamPortal2++)
        {
            StreamUnit *connectStream;

            strcpy(text, streamPortal2->tolevelname);

            commapos = strchr(text, ',');

            if (commapos != NULL)
            {
                *commapos = 0;

                signalID = atoi(commapos + 1);
            }
            else
            {
                signalID = 0;
            }

            connectStream = STREAM_GetStreamUnitWithID(streamPortal2->streamID);

            if ((strcmpi(text, "warpgate") == 0) && (WARPGATE_IsUnitWarpRoom(mainUnit) != 0))
            {
                connectStream = mainUnit;
            }

            streamPortal2->toStreamUnit = connectStream;

            if ((connectStream == NULL) || (connectStream != mainUnit))
            {
                continue;
            }

            numportals = *(long *)mainUnit->level->terrain->StreamUnits;

            streamPortal = (StreamUnitPortal *)((long *)mainUnit->level->terrain->StreamUnits + 1);

            for (i = 0; i < numportals; i++, streamPortal++)
            {
                if (signalID == streamPortal->MSignalID)
                {
                    SVector offset;

                    offset.x = streamPortal->minx - streamPortal2->minx;
                    offset.y = streamPortal->miny - streamPortal2->miny;
                    offset.z = streamPortal->minz - streamPortal2->minz;

                    RelocateLevel(streamUnit->level, &offset);
                    break;
                }
            }
        }

        {
            long d;
            StreamUnit *connectStream;

            connectStream = StreamTracker.StreamList;

            for (d = 0; d < 16; d++, connectStream++)
            {
                if ((connectStream->used == 2) && (connectStream != streamUnit))
                {
                    numportals2 = *(long *)connectStream->level->terrain->StreamUnits;

                    streamPortal2 = (StreamUnitPortal *)((long *)connectStream->level->terrain->StreamUnits + 1);

                    for (j = 0; j < numportals2; j++, streamPortal2++)
                    {
                        long hookedUp;

                        hookedUp = 0;

                        strcpy(text, streamPortal2->tolevelname);

                        commapos = strchr(text, ',');

                        if (commapos != NULL)
                        {
                            *commapos = 0;

                            signalID = atoi(commapos + 1);
                        }
                        else
                        {
                            signalID = 0;
                        }

                        if (streamPortal2->streamID == streamUnit->StreamUnitID)
                        {
                            streamPortal2->toStreamUnit = streamUnit;

                            hookedUp = 1;
                        }
                        else if ((strcmpi(text, "warpgate") == 0) && (WARPGATE_IsUnitWarpRoom(streamUnit) != 0))
                        {
                            streamPortal2->toStreamUnit = streamUnit;

                            hookedUp = 1;
                        }

                        if ((hookedUp == 1) && (connectStream == mainUnit))
                        {
                            numportals = *(long *)streamUnit->level->terrain->StreamUnits;

                            streamPortal = (StreamUnitPortal *)((long *)streamUnit->level->terrain->StreamUnits + 1);

                            for (i = 0; i < numportals; i++, streamPortal++)
                            {
                                if (signalID == streamPortal->MSignalID)
                                {
                                    SVector offset;

                                    offset.x = streamPortal2->minx - streamPortal->minx;
                                    offset.y = streamPortal2->miny - streamPortal->miny;
                                    offset.z = streamPortal2->minz - streamPortal->minz;

                                    RelocateLevel(streamUnit->level, &offset);
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }

        for (i = 0; i < streamUnit->level->numIntros; i++)
        {
            if (strcmpi(streamUnit->level->introList[i].name, "raziel") == 0)
            {
                streamUnit->level->introList[i].flags |= 0x8;
                break;
            }
        }
    }
}

void STREAM_StreamLoadLevelAbort(void *loadData, void *data, void *data2)
{
    StreamUnit *streamUnit;

    (void)data;

    streamUnit = (StreamUnit *)data2;

    if (loadData != NULL)
    {
        MEMPACK_Free((char *)loadData);
    }

    streamUnit->level = NULL;

    streamUnit->used = 0;

    streamUnit->flags = 0;
}

void STREAM_DoObjectLoadAndDump(StreamUnit *streamUnit)
{
    int i;

    (void)streamUnit;

    for (i = 0; i < 16; i++)
    {
        if (StreamTracker.StreamList[i].used == 1)
        {
            return;
        }
    }

    STREAM_RemoveAllObjectsNotInUse();
}

void STREAM_FinishLoad(StreamUnit *streamUnit)
{
    Level *level;
    char sfxName[80];

    STREAM_FillOutFileNames(streamUnit->baseAreaName, NULL, NULL, sfxName);

    level = streamUnit->level;

    level->morphLastStep = -1;

    streamUnit->sfxFileHandle = 0;

    if (LOAD_DoesFileExist(sfxName) != 0)
    {
        streamUnit->sfxFileHandle = aadLoadDynamicSfx(streamUnit->baseAreaName, streamUnit->StreamUnitID, 1);
    }

    LoadLevelObjects(streamUnit);

    streamUnit->FogColor = *(long *)&level->backColorR;

    level->fogFar = streamUnit->TargetFogFar;
    level->fogNear = streamUnit->TargetFogNear;

    LIGHT_CalcDQPTable(level);

    STREAM_CalculateWaterLevel(level);

    if (gameTrackerX.gameData.asmData.MorphType == 1)
    {
        MORPH_UpdateNormals(level);
    }

    STREAM_ConnectStream(streamUnit);

    streamUnit->used = 2;

    STREAM_DoObjectLoadAndDump(streamUnit);

    EVENT_LoadEventsForLevel(streamUnit->StreamUnitID, level);

    PLANAPI_InitPlanMkrList(streamUnit);

    if (level->startUnitLoadedSignal != NULL)
    {
        level->startUnitLoadedSignal->flags |= 0x1;

        SIGNAL_HandleSignal(gameTrackerX.playerInstance, &level->startUnitLoadedSignal->signalList[0], 0);

        EVENT_AddSignalToReset(level->startUnitLoadedSignal);
    }

    SAVE_IntroForStreamID(streamUnit);

    SAVE_UpdateLevelWithSave(streamUnit);

    EVENT_AddStreamToInstanceList(streamUnit);

    WARPGATE_FixUnit(streamUnit);
}

void STREAM_LoadLevelReturn(void *loadData, void *data, void *data2)
{
    (void)data;

    GetRCnt(0xF2000000);

    gameTimer;

    ((StreamUnit *)data2)->StreamUnitID = ((Level *)loadData)->streamUnitID;

    gameTrackerX.StreamUnitID = ((Level *)loadData)->streamUnitID;

    gameTrackerX.level = (Level *)loadData;

    STREAM_SetMainFog(((StreamUnit *)data2));

    STREAM_FinishLoad(((StreamUnit *)data2));
}

void STREAM_StreamLoadLevelReturn(void *loadData, void *data, void *data2)
{
    Level *level;
    StreamUnit *streamUnit;

    (void)data;

    GetRCnt(0xF2000000);

    gameTimer;

    level = (Level *)loadData;

    streamUnit = (StreamUnit *)data2;

    streamUnit->StreamUnitID = level->streamUnitID;

    if (streamUnit->used == 3)
    {
        streamUnit->used = 0;

        streamUnit->flags = 0;

        MEMPACK_Free((char *)streamUnit->level);

        streamUnit->level = NULL;

        return;
    }

    if (gameTrackerX.gameData.asmData.MorphType != 0)
    {
        STREAM_SetStreamFog(streamUnit, level->spectralFogNear, level->spectralFogFar);
    }
    else
    {
        STREAM_SetStreamFog(streamUnit, level->holdFogNear, level->holdFogFar);
    }

    STREAM_FinishLoad(streamUnit);

    if ((gameTrackerX.playerInstance != NULL) && (level->streamUnitID == gameTrackerX.playerInstance->currentStreamUnitID))
    {
        strcpy(gameTrackerX.baseAreaName, level->worldName);

        STREAM_SetMainFog(streamUnit);

        gameTrackerX.StreamUnitID = level->streamUnitID;

        gameTrackerX.level = level;
    }
}

void STREAM_UpdateLevelPointer(Level *oldLevel, Level *newLevel, long sizeOfLevel)
{
    long i;
    long offset;
    GameTracker *gameTracker;

    offset = (intptr_t)newLevel - (intptr_t)oldLevel;

    for (i = 0; i < 16; i++)
    {
        if ((StreamTracker.StreamList[i].used == 2) && (StreamTracker.StreamList[i].level == oldLevel))
        {
            StreamTracker.StreamList[i].level = newLevel;
            break;
        }
    }

    gameTracker = &gameTrackerX;

    if (gameTrackerX.level == oldLevel)
    {
        gameTrackerX.level = newLevel;
    }

    {
        Instance *instance;

        instance = gameTracker->instanceList->first;

        while (instance != NULL)
        {
            i = (intptr_t)oldLevel + sizeOfLevel;

            if (IN_BOUNDS(instance->intro, oldLevel, i) != 0)
            {
                instance->intro = (Intro *)OFFSET_DATA(instance->intro, offset);
            }

            if (IN_BOUNDS(instance->introData, oldLevel, i) != 0)
            {
                instance->introData = (void *)OFFSET_DATA(instance->introData, offset);
            }

            if (IN_BOUNDS(instance->tface, oldLevel, i) != 0)
            {
                instance->tface = (TFace *)OFFSET_DATA(instance->tface, offset);
            }

            if (IN_BOUNDS(instance->waterFace, oldLevel, i) != 0)
            {
                instance->waterFace = (TFace *)OFFSET_DATA(instance->waterFace, offset);
            }

            if (IN_BOUNDS(instance->waterFaceTerrain, oldLevel, i) != 0)
            {
                instance->waterFaceTerrain = (Terrain *)OFFSET_DATA(instance->waterFaceTerrain, offset);
            }

            if (IN_BOUNDS(instance->oldTFace, oldLevel, i) != 0)
            {
                instance->oldTFace = (TFace *)OFFSET_DATA(instance->oldTFace, offset);
            }

            if (IN_BOUNDS(instance->tfaceLevel, oldLevel, i) != 0)
            {
                instance->tfaceLevel = (void *)OFFSET_DATA(instance->tfaceLevel, offset);
            }

            if (IN_BOUNDS(instance->cachedTFaceLevel, oldLevel, i) != 0)
            {
                instance->cachedTFaceLevel = (void *)OFFSET_DATA(instance->cachedTFaceLevel, offset);
            }

            instance = instance->next;
        }
    }

    if (IN_BOUNDS(theCamera.data.Cinematic.posSpline, oldLevel, (intptr_t)oldLevel + sizeOfLevel) != 0)
    {
        theCamera.data.Cinematic.posSpline = (MultiSpline *)OFFSET_DATA(theCamera.data.Cinematic.posSpline, offset);
    }

    if (IN_BOUNDS(theCamera.data.Cinematic.targetSpline, oldLevel, (intptr_t)oldLevel + sizeOfLevel) != 0)
    {
        theCamera.data.Cinematic.targetSpline = (MultiSpline *)OFFSET_DATA(theCamera.data.Cinematic.targetSpline, offset);
    }

    for (i = 0; i <= theCamera.stack; i++)
    {
        if (IN_BOUNDS(theCamera.savedCinematic[i].posSpline, oldLevel, (intptr_t)oldLevel + sizeOfLevel) != 0)
        {
            theCamera.savedCinematic[i].posSpline = (MultiSpline *)OFFSET_DATA(theCamera.savedCinematic[i].posSpline, offset);
        }

        if (IN_BOUNDS(theCamera.savedCinematic[i].targetSpline, oldLevel, (intptr_t)oldLevel + sizeOfLevel) != 0)
        {
            theCamera.savedCinematic[i].targetSpline = (MultiSpline *)OFFSET_DATA(theCamera.savedCinematic[i].targetSpline, offset);
        }
    }

    EVENT_UpdateResetSignalArrayAndWaterMovement(oldLevel, newLevel, sizeOfLevel);
}

StreamUnit *STREAM_WhichUnitPointerIsIn(void *pointer)
{
    int i;
    int size;
    Level *level;

    for (i = 0; i < 16; i++)
    {
        if (StreamTracker.StreamList[i].used == 2)
        {
            level = StreamTracker.StreamList[i].level;

            if (level != NULL)
            {
                size = MEMPACK_Size((char *)level);

                if ((level <= (Level *)pointer) && (((char *)level + size) >= (char *)pointer))
                {
                    return &StreamTracker.StreamList[i];
                }
            }
        }
    }

    return NULL;
}

void STREAM_UpdateObjectPointer(Object *oldObject, Object *newObject, long sizeOfObject)
{
    long i;
    long d;
    GameTracker *gameTracker;
    long offset;
    ObjectTracker *otr;

    gameTracker = &gameTrackerX;

    offset = (intptr_t)newObject - (intptr_t)oldObject;

    otr = FindObjectInTracker(oldObject);

    if (otr != NULL)
    {
        otr->object = newObject;

        for (i = 0; i < otr->numObjectsUsing; i++)
        {
            int j;
            Object *object;

            object = gameTracker->GlobalObjects[(int)otr->objectsUsing[i]].object;

            if (object != NULL)
            {
                for (j = 0; j < object->numAnims; j++)
                {
                    if (IN_BOUNDS(object->animList[j], oldObject, (intptr_t)oldObject + sizeOfObject) != 0)
                    {
                        object->animList[j] = (G2AnimKeylist *)OFFSET_DATA(object->animList[j], offset);
                    }
                }
            }
        }

        OBTABLE_ChangeObjectAccessPointers(oldObject, newObject);

        if (((newObject->oflags & 0x8000000)) && (newObject->relocList != NULL) && (newObject->relocModule != NULL))
        {
            RELMOD_RelocModulePointers((intptr_t)newObject->relocModule, offset, (int *)newObject->relocList);
        }

        {
            Instance *instance;

            instance = gameTracker->instanceList->first;

            while (instance != NULL)
            {
                if (instance->object == oldObject)
                {
                    instance->object = newObject;

                    if (instance->hModelList != NULL)
                    {
                        for (i = 0; i < instance->object->numModels; i++)
                        {
                            for (d = 0; d < instance->hModelList[i].numHPrims; d++)
                            {
                                instance->hModelList[i].hPrimList[d].data.hsphere = (HSphere *)OFFSET_DATA(instance->hModelList[i].hPrimList[d].data.hsphere, offset);
                            }
                        }
                    }

                    OBTABLE_RelocateInstanceObject(instance, offset);
                }

                if (IN_BOUNDS(instance->data, oldObject, (intptr_t)oldObject + sizeOfObject) != 0)
                {
                    instance->data = (void *)OFFSET_DATA(instance->data, offset);
                }

                instance = instance->next;
            }
        }
    }

    OBTABLE_RelocateObjectTune(newObject, offset);

    if ((newObject->oflags2 & 0x20000000))
    {
        FX_RelocateFXPointers(oldObject, newObject, sizeOfObject);
    }
}

void STREAM_UpdateInstanceCollisionInfo(HModel *oldHModel, HModel *newHModel)
{
    Instance *instance;

    instance = gameTrackerX.instanceList->first;

    while (instance != NULL)
    {
        if (instance->hModelList == oldHModel)
        {
            instance->hModelList = newHModel;
        }

        instance = instance->next;
    }
}

void STREAM_LoadMainVram(GameTracker *gameTracker, char *baseAreaName, StreamUnit *streamUnit)
{
    char dramName[80];
    char vramName[80];
    VramBuffer *vramBuffer;
    Level *level;

    (void)baseAreaName;

    level = streamUnit->level;

    STREAM_FillOutFileNames(gameTracker->baseAreaName, dramName, vramName, NULL);

    vramBuffer = (VramBuffer *)MEMPACK_Malloc((level->vramSize.w << 1) + 20, 35);

    vramBuffer->lineOverFlow = (short *)(vramBuffer + 1);

    vramBuffer->flags = 0;

    vramBuffer->x = level->vramSize.x + 512;
    vramBuffer->y = level->vramSize.y;
    vramBuffer->w = level->vramSize.w;
    vramBuffer->h = level->vramSize.h;

    M_TrackClutUpdate = 0;

    vramBuffer->yOffset = 0;

    vramBuffer->lengthOfLeftOverData = 0;

    LOAD_NonBlockingBufferedLoad(vramName, (void *)VRAM_TransferBufferToVram, vramBuffer, NULL);
}

void STREAM_MoveIntoNewStreamUnit()
{
    gameTrackerX.playerInstance->cachedTFace = -1;
    gameTrackerX.playerInstance->cachedTFaceLevel = NULL;

    gameTrackerX.playerInstance->currentStreamUnitID = gameTrackerX.moveRazielToStreamID;

    INSTANCE_UpdateFamilyStreamUnitID(gameTrackerX.playerInstance);

    GAMELOOP_StreamLevelLoadAndInit(gameTrackerX.S_baseAreaName, &gameTrackerX, gameTrackerX.toSignal, gameTrackerX.fromSignal);

    gameTrackerX.SwitchToNewStreamUnit = 0;

    if (gameTrackerX.SwitchToNewWarpIndex != -1)
    {
        SndPlayVolPan(388, 127, 64, 0);

        CurrentWarpNumber = gameTrackerX.SwitchToNewWarpIndex;
    }
}

StreamUnit *STREAM_LoadLevel(char *baseAreaName, StreamUnitPortal *streamPortal, int loadnext)
{
    int i;
    long streamID;
    StreamUnit *streamUnit;
    Level *level;
    char dramName[80];

    (void)loadnext;

    streamID = -1;

    if (streamPortal != NULL)
    {
        streamID = streamPortal->streamID;
    }

    for (i = 0; i < 16; i++)
    {
        streamUnit = &StreamTracker.StreamList[i];

        if ((streamUnit->used != 0) && (strcmpi(streamUnit->baseAreaName, baseAreaName) == 0))
        {
            if (streamUnit->used == 3)
            {
                streamUnit->used = 1;
                break;
            }
            else if (streamUnit->used != 1)
            {
                streamUnit->FrameCount = 0;

                if (streamPortal == NULL)
                {
                    strcpy(gameTrackerX.baseAreaName, baseAreaName);

                    STREAM_SetMainFog(streamUnit);

                    gameTrackerX.StreamUnitID = streamUnit->StreamUnitID;

                    gameTrackerX.level = streamUnit->level;
                }
                else
                {
                    level = streamUnit->level;

                    STREAM_ConnectStream(streamUnit);

                    if (gameTrackerX.gameData.asmData.MorphType != 0)
                    {
                        STREAM_SetStreamFog(streamUnit, (short)level->spectralFogNear, (short)level->spectralFogFar);
                    }
                    else
                    {
                        STREAM_SetStreamFog(streamUnit, (short)level->holdFogNear, (short)level->holdFogFar);
                    }
                }

                break;
            }
            else
            {
                break;
            }
        }
    }

    if (i == 16)
    {
        for (i = 0; i < 16; i++)
        {
            streamUnit = &StreamTracker.StreamList[i];

            if (streamUnit->used == 0)
            {
                STREAM_FillOutFileNames(baseAreaName, dramName, NULL, NULL);

                streamUnit->used = 1;

                strcpy(streamUnit->baseAreaName, baseAreaName);

                streamUnit->StreamUnitID = streamID;

                streamUnit->FrameCount = 0;

                streamUnit->flags = 0;

                if (streamPortal == NULL)
                {
                    strcpy(gameTrackerX.baseAreaName, baseAreaName);

                    gameTrackerX.StreamUnitID = streamUnit->StreamUnitID;

                    LOAD_NonBlockingBinaryLoad(dramName, (void *)STREAM_LoadLevelReturn, NULL, streamUnit, (void **)&streamUnit->level, 2);

                    break;
                }
                else
                {
                    streamPortal->toStreamUnit = NULL;

                    LOAD_NonBlockingBinaryLoad(dramName, (void *)STREAM_StreamLoadLevelReturn, NULL, streamUnit, (void **)&streamUnit->level, 2);

                    break;
                }
            }
        }
    }

    return streamUnit;
}

void RemoveIntroducedLights(Level *level)
{
    int i;

    LIGHT_Restore(gameTrackerX.lightInfo);

    gameTrackerX.lightInfo->numSavedColors = 0;

    for (i = 0; i < level->numSpotLights; i++)
    {
        if ((level->spotLightList[i].flags & 0x10))
        {
            LIST_DeleteFunc(&level->spotLightList[i].node);
        }
    }

    for (i = 0; i < level->numPointLights; i++)
    {
        if ((level->pointLightList[i].flags & 0x10))
        {
            LIST_DeleteFunc(&level->pointLightList[i].node);
        }
    }
}

void STREAM_RemoveInstancesWithIDInInstanceList(InstanceList *list, long id, Level *level)
{
    Instance *instance;
    Instance *next;

    instance = list->first;

    while (instance != NULL)
    {
        next = instance->next;

        if (instance->currentStreamUnitID == id)
        {
            SAVE_Instance(instance, level);

            INSTANCE_ReallyRemoveInstance(list, instance, 0);

            instance = next;
        }
        else
        {
            if (instance->birthStreamUnitID == id)
            {
                SAVE_Instance(instance, level);

                instance->intro = NULL;
            }

            instance = next;
        }
    }
}

void STREAM_MarkUnitNeeded(long streamID)
{
    int i;

    for (i = 0; i < 16; i++)
    {
        if ((StreamTracker.StreamList[i].used != 0) && (streamID == StreamTracker.StreamList[i].StreamUnitID))
        {
            StreamTracker.StreamList[i].FrameCount = gameTrackerX.displayFrameCount;
            break;
        }
    }
}

void STREAM_DumpUnit(StreamUnit *streamUnit, long doSave)
{
    int i;
    int j;
    int numportals;

    for (i = 0; i < 16; i++)
    {
        if (StreamTracker.StreamList[i].used == 2)
        {
            StreamUnitPortal *p; // not from decls.h

            numportals = *(long *)StreamTracker.StreamList[i].level->terrain->StreamUnits;

            p = (StreamUnitPortal *)((long *)StreamTracker.StreamList[i].level->terrain->StreamUnits + 1);

            for (j = 0; j < numportals; j++, p++)
            {
                if (p->toStreamUnit == streamUnit)
                {
                    p->toStreamUnit = NULL;
                }
            }
        }
    }

    if ((streamUnit->used == 1) || (streamUnit->used == 3))
    {
        char dramName[80];

        STREAM_FillOutFileNames(streamUnit->baseAreaName, dramName, NULL, NULL);

        LOAD_AbortFileLoad(dramName, (void *)STREAM_StreamLoadLevelAbort);

        streamUnit->used = 0;

        streamUnit->flags = 0;
        return;
    }

    if (WARPGATE_IsUnitWarpRoom(streamUnit) != 0)
    {
        WARPGATE_RemoveFromArray(streamUnit);
    }

    EVENT_RemoveStreamToInstanceList(streamUnit);

    for (i = 0; i < streamUnit->level->NumberOfSFXMarkers; i++)
    {
        SFXMkr *sfxMkr;

        sfxMkr = &streamUnit->level->SFXMarkerList[i];

        SOUND_EndInstanceSounds(sfxMkr->soundData, sfxMkr->sfxTbl);
    }

    if (streamUnit->sfxFileHandle != 0)
    {
        aadFreeDynamicSfx(streamUnit->sfxFileHandle);
    }

    PLANAPI_DeleteNodeFromPoolByUnit(streamUnit->StreamUnitID);

    STREAM_RemoveInstancesWithIDInInstanceList(gameTrackerX.instanceList, streamUnit->StreamUnitID, streamUnit->level);

    if (doSave != 0)
    {
        EVENT_SaveEventsFromLevel(streamUnit->StreamUnitID, streamUnit->level);

        SAVE_CreatedSavedLevel(streamUnit->StreamUnitID, streamUnit->level);
    }

    MEMPACK_Free((char *)streamUnit->level);

    streamUnit->level = NULL;

    streamUnit->used = 0;

    streamUnit->flags = 0;
}

void STREAM_DumpAllUnitsNotNeeded()
{
    int i;

    for (i = 0; i < 16; i++)
    {
        if ((StreamTracker.StreamList[i].used != 0) && (StreamTracker.StreamList[i].FrameCount != (long)gameTrackerX.displayFrameCount))
        {
            STREAM_DumpUnit(&StreamTracker.StreamList[i], 1);
        }
    }
}

void STREAM_DumpAllLevels(long IDNoRemove, int DoSave)
{
    int i;

    for (i = 0; i < 16; i++)
    {
        if ((StreamTracker.StreamList[i].used != 0) && (StreamTracker.StreamList[i].StreamUnitID != IDNoRemove))
        {
            STREAM_DumpUnit(&StreamTracker.StreamList[i], DoSave);
        }
    }
}

void STREAM_LoadCurrentWarpRoom(StreamUnitPortal *streamPortal, StreamUnit *mainStreamUnit)
{
    if (strcmpi(mainStreamUnit->level->worldName, WarpRoomArray[CurrentWarpNumber].name) == 0)
    {
        WarpGateLoadInfo.loading = 3;

        WarpGateLoadInfo.curTime = WarpGateLoadInfo.maxTime;
    }

    WarpRoomArray[CurrentWarpNumber].streamUnit = STREAM_LoadLevel(WarpRoomArray[CurrentWarpNumber].name, streamPortal, 0);

    if (WarpRoomArray[CurrentWarpNumber].streamUnit != NULL)
    {
        streamPortal->toStreamUnit = WarpRoomArray[CurrentWarpNumber].streamUnit;

        WarpRoomArray[CurrentWarpNumber].streamUnit->flags |= 0x1;
    }
}

void WARPGATE_RelocateLoadedWarpRooms(StreamUnit *mainUnit, StreamUnitPortal *streamPortal)
{
    int i;

    for (i = 0; i < 16; i++)
    {
        if ((StreamTracker.StreamList[i].used == 2) && (&StreamTracker.StreamList[i] != mainUnit) && ((StreamTracker.StreamList[i].flags & 0x1)))
        {
            STREAM_LoadLevel(StreamTracker.StreamList[i].baseAreaName, streamPortal, 0);
        }
    }
}

long WARPGATE_GetWarpRoomIndex(char *name)
{
    int i;

    for (i = 0; i < 14; i++)
    {
        if (strcmpi(WarpRoomArray[i].name, name) == 0)
        {
            return i;
        }
    }

    return -1;
}

void WARPGATE_UpdateAddToArray(StreamUnit *streamUnit)
{
    int i;

    i = WARPGATE_GetWarpRoomIndex(streamUnit->baseAreaName);

    if (i != -1)
    {
        WarpRoomArray[i].streamUnit = streamUnit;
    }
}

void WARPGATE_RemoveFromArray(StreamUnit *streamUnit)
{
    if (WARPGATE_GetWarpRoomIndex(streamUnit->baseAreaName) == -1)
    {
        CurrentWarpNumber = 0;
    }
}

void WARPGATE_Init()
{
    int n;

    WarpGateLoadInfo.fadeValue = 4096;

    WarpGateLoadInfo.warpgate_in_use = 0;

    WarpGateLoadInfo.loading = 0;

    WarpGateLoadInfo.blocked = 0;

    WarpGateLoadInfo.curTime = 0;
    WarpGateLoadInfo.maxTime = 61440;

    WarpGateLoadInfo.warpFaceInstance = NULL;

    CurrentWarpNumber = 0;

    for (n = 13; n >= 0; n--)
    {
        WarpRoomArray[n].streamUnit = NULL;
    }
}

void WARPGATE_StartUsingWarpgate()
{
    if (WarpGateLoadInfo.warpgate_in_use == 0)
    {
        SndPlayVolPan(367, 127, 64, 0);
    }

    WarpGateLoadInfo.warpgate_in_use = 1;
}

void WARPGATE_EndUsingWarpgate()
{
    if (WarpGateLoadInfo.warpgate_in_use == 1)
    {
        SndPlayVolPan(386, 127, 64, 0);
    }

    WarpGateLoadInfo.warpgate_in_use = 0;
}

int WARPGATE_IsWarpgateInUse()
{
    return WarpGateLoadInfo.warpgate_in_use;
}

int WARPGATE_IsWarpgateActive()
{
    return WarpGateLoadInfo.loading != 0;
}

int WARPGATE_IsWarpgateUsable()
{
    return (WarpGateLoadInfo.loading == 4) && (WarpGateLoadInfo.blocked == 0);
}

int WARPGATE_IsWarpgateReady()
{
    return WarpGateLoadInfo.loading == 4;
}

int WARPGATE_IsWarpgateSpectral()
{
    return strcmpi(WarpRoomArray[CurrentWarpNumber].name, "under3") == 0;
}

int WARPGATE_IsObjectOnWarpSide(Instance *instance)
{
    int side;
    int temp, temp2; // not from decls.h

    if (WarpGateLoadInfo.warpFaceInstance != NULL)
    {
        side = ~(WarpGateLoadInfo.warpFaceInstance->position.y - theCamera.core.position.y);

        temp = side < 0;

        temp2 = temp;

        if ((WarpGateLoadInfo.warpFaceInstance->position.y - instance->position.y) < 0)
        {
            if (temp == 1)
            {
                return 1;
            }
        }
        else if (temp2 == 0)
        {
            return 1;
        }
    }

    return 0;
}

void WARPGATE_IsItActive(StreamUnit *streamUnit)
{
    Level *level;
    int d;
    int temp; // not from decls.h

    level = streamUnit->level;

    streamUnit->flags |= 0x1;

    if (level->PuzzleInstances == NULL)
    {
        return;
    }

    for (d = 0; d < level->PuzzleInstances->numPuzzles; d++)
    {
        temp = level->PuzzleInstances->eventInstances[d]->eventNumber;

        if (temp != 1)
        {
            continue;
        }

        if ((gameTrackerX.streamFlags & 0x400000))
        {
            *level->PuzzleInstances->eventInstances[d]->eventVariables = temp;
        }

        if (*level->PuzzleInstances->eventInstances[d]->eventVariables == temp)
        {
            streamUnit->flags |= 0x8;
        }

        break;
    }
}

long WARPGATE_IsUnitWarpRoom(StreamUnit *streamUnit)
{
    Level *level;
    long isWarpRoom;
    StreamUnitPortal *streamPortal;
    long numPortals;
    long d;

    level = streamUnit->level;

    isWarpRoom = 0;

    numPortals = *(long *)level->terrain->StreamUnits;

    streamPortal = (StreamUnitPortal *)((long *)level->terrain->StreamUnits + 1);

    for (d = 0; d < numPortals; d++, streamPortal++)
    {
        if ((streamPortal->flags & 0x1))
        {
            isWarpRoom = 1;
        }
    }

    return isWarpRoom;
}

void WARPGATE_FixUnit(StreamUnit *streamUnit)
{
    if (WARPGATE_IsUnitWarpRoom(streamUnit) != 0)
    {
        WARPGATE_IsItActive(streamUnit);
    }
}

void STREAM_MarkWarpUnitsNeeded()
{
    int i;

    for (i = 0; i < 16; i++)
    {
        if ((StreamTracker.StreamList[i].flags & 0x1))
        {
            StreamTracker.StreamList[i].FrameCount = gameTrackerX.displayFrameCount;
        }
    }
}

long WARPGATE_IncrementIndex()
{
    long result;

    result = 1;

    if (WarpGateLoadInfo.loading == 4)
    {
        SndPlayVolPan(387, 127, 64, 0);

        WarpGateLoadInfo.loading = 1;

        WarpGateLoadInfo.curTime = 0;

        WarpGateLoadInfo.warpFaceInstance->fadeValue = 4096;

        WarpGateLoadInfo.warpFaceInstance = NULL;

        WarpRoomArray[CurrentWarpNumber].streamUnit = NULL;

        CurrentWarpNumber = (CurrentWarpNumber + 1) % 14;

        if (strcmpi(gameTrackerX.baseAreaName, WarpRoomArray[CurrentWarpNumber].name) == 0)
        {
            CurrentWarpNumber = (CurrentWarpNumber + 1) % 14;
        }

        hud_warp_arrow_flash = -8192;
    }

    return result;
}

void WARPGATE_CalcWarpFade(int timeInc)
{
    WarpGateLoadInfo.warpFaceInstance->fadeValue = WarpGateLoadInfo.fadeValue;

    WarpGateLoadInfo.curTime += timeInc;

    WarpGateLoadInfo.fadeValue = 4096 - (short)((WarpGateLoadInfo.curTime << 12) / WarpGateLoadInfo.maxTime);

    if (WarpGateLoadInfo.fadeValue >= 4097)
    {
        WarpGateLoadInfo.fadeValue = 4096;
    }

    if (WarpGateLoadInfo.fadeValue < 0)
    {
        WarpGateLoadInfo.fadeValue = 0;
    }
}

long WARPGATE_DecrementIndex()
{
    long result;

    result = 1;

    if (WarpGateLoadInfo.loading == 4)
    {
        SndPlayVolPan(387, 127, 64, 0);

        WarpGateLoadInfo.loading = 1;

        WarpGateLoadInfo.curTime = 0;

        WarpGateLoadInfo.warpFaceInstance->fadeValue = 4096;

        WarpGateLoadInfo.warpFaceInstance = NULL;

        WarpRoomArray[CurrentWarpNumber].streamUnit = NULL;

        CurrentWarpNumber--;

        if (CurrentWarpNumber < 0)
        {
            CurrentWarpNumber = 13;
        }

        if (strcmpi(gameTrackerX.baseAreaName, WarpRoomArray[CurrentWarpNumber].name) == 0)
        {
            CurrentWarpNumber--;

            if (CurrentWarpNumber < 0)
            {
                CurrentWarpNumber = 13;
            }
        }

        hud_warp_arrow_flash = 8192;
    }

    return result;
}

void PreloadAllConnectedUnits(StreamUnit *streamUnit, SVector *offset)
{
    int i;
    char text[16];
    int numportals;
    char *commapos;
    StreamUnitPortal *stream;

    gameTrackerX.displayFrameCount++;

    numportals = *(long *)streamUnit->level->terrain->StreamUnits;

    stream = (StreamUnitPortal *)((long *)streamUnit->level->terrain->StreamUnits + 1);

    for (i = 0; i < numportals; i++)
    {
        strcpy(text, stream[i].tolevelname);

        commapos = strchr(text, ',');

        if (commapos != NULL)
        {
            *commapos = 0;
        }

        if (strcmpi(text, "warpgate") == 0)
        {
            STREAM_MarkWarpUnitsNeeded();
        }
        else
        {
            STREAM_MarkUnitNeeded(stream[i].streamID);
        }
    }

    STREAM_MarkUnitNeeded(streamUnit->StreamUnitID);

    STREAM_DumpAllUnitsNotNeeded();

    RelocateLevelWithInstances(streamUnit->level, offset);

    MEMPACK_DoGarbageCollection();

    numportals = *(long *)streamUnit->level->terrain->StreamUnits;

    for (i = 0; i < numportals; i++)
    {
        stream = (StreamUnitPortal *)((long *)streamUnit->level->terrain->StreamUnits + 1) + i;

        strcpy(text, stream->tolevelname);

        commapos = strchr(text, ',');

        if (commapos != NULL)
        {
            *commapos = 0;

            if (strcmpi(text, "warpgate") == 0)
            {
                stream->flags |= 0x1;

                stream->toStreamUnit = NULL;

                WARPGATE_RelocateLoadedWarpRooms(streamUnit, stream);
            }
            else
            {
                STREAM_LoadLevel(text, stream, 1);
            }
        }
    }
}

void RelocateLevel(Level *level, SVector *offset)
{
    int curTree;

    RelocateTerrain(level->terrain, offset);
    RelocateVMObjects(level->vmobjectList, level->numVMObjects, offset);
    RelocateBGObjects(level->bgObjectList, level->numBGObjects, offset);
    RelocateCameras((CameraKey *)level->cameraList, level->numCameras, offset);
    RelocateSavedCameras(&theCamera, level, offset);

    level->deathZ += offset->z;

    RelocatePlanMarkers(level->PlanMarkerList, level->NumberOfPlanMarkers, offset);
    RelocateSFXMarkers(level->SFXMarkerList, level->NumberOfSFXMarkers, offset);

    if ((level->waterZLevel != -32767) && (level->waterZLevel != 32767))
    {
        level->waterZLevel += offset->z;
    }

    for (curTree = 0; curTree < level->terrain->numBSPTrees; curTree++)
    {
        ADD_SVEC(Position, &level->terrain->BSPTreeArray[curTree].globalOffset, Position, &level->terrain->BSPTreeArray[curTree].globalOffset, SVector, offset);
    }
}

void RelocateCameras(CameraKey *cameraList, long numCameras, SVector *offset)
{
    int i;

    for (i = 0; i < numCameras; i++)
    {
        cameraList[i].x += offset->x;
        cameraList[i].y += offset->y;
        cameraList[i].z += offset->z;

        cameraList[i].tx += offset->x;
        cameraList[i].ty += offset->y;
        cameraList[i].tz += offset->z;
    }
}

void RelocateSavedCameras(Camera *camera, Level *level, SVector *offset)
{
    int i;

    for (i = 0; camera->stack >= i; i++)
    {
        if ((camera->savedMode[i] == 5) && (camera->savedCinematic[i].level == level))
        {
            ADD_SVEC(Position, &camera->savedCinematic[i].position, Position, &camera->savedCinematic[i].position, SVector, offset);
            ADD_SVEC(Position, &camera->savedCinematic[i].focusPoint, Position, &camera->savedCinematic[i].focusPoint, SVector, offset);
            ADD_SVEC(Position, &camera->savedCinematic[i].targetPos, Position, &camera->savedCinematic[i].targetPos, SVector, offset);
            ADD_SVEC(Position, &camera->savedCinematic[i].targetFocusPoint, Position, &camera->savedCinematic[i].targetFocusPoint, SVector, offset);
        }
    }
}

void RelocateLevelWithInstances(Level *level, SVector *offset)
{
    RelocateLevel(level, offset);
    RelocateInstances(offset);
    RelocatePlanPool((PlanningNode *)gameTrackerX.planningPool, offset);
}

void RelocateTerrain(Terrain *terrain, SVector *offset)
{
    int i;

    for (i = 0; i < terrain->numIntros; i++)
    {
        terrain->introList[i].position.x += offset->x;
        terrain->introList[i].position.y += offset->y;
        terrain->introList[i].position.z += offset->z;

        if (terrain->introList[i].multiSpline != NULL)
        {
            STREAM_AdjustMultiSpline(terrain->introList[i].multiSpline, offset);
        }
    }

    RelocateStreamPortals((StreamUnitPortal *)((long *)terrain->StreamUnits + 1), *(long *)terrain->StreamUnits, offset);
}

void RelocateVMObjects(VMObject *vobjectlist, long numvmobjs, SVector *offset)
{
    int i;

    for (i = 0; i < numvmobjs; i++)
    {
        vobjectlist[i].position.x += offset->x;
        vobjectlist[i].position.y += offset->y;
        vobjectlist[i].position.z += offset->z;
    }
}

void RelocateBGObjects(BGObject *BGObjList, long numBGObjs, SVector *offset)
{
    int i;
    int d;

    for (i = 0; i < numBGObjs; i++)
    {
        for (d = 0; d < BGObjList[i].numVertices; d++)
        {
            BGObjList[i].vertexList[d].x += offset->x;
            BGObjList[i].vertexList[d].y += offset->y;
            BGObjList[i].vertexList[d].z += offset->z;
        }
    }
}

void RelocatePlanPool(PlanningNode *planPool, SVector *offset)
{
    int i;
    short ox;
    short oy;
    short oz;

    ox = offset->x;
    oy = offset->y;
    oz = offset->z;

    for (i = PLANAPI_NumNodesInPool(planPool); i != 0; i--, planPool++)
    {
        planPool->pos.x += ox;
        planPool->pos.y += oy;
        planPool->pos.z += oz;
    }

    poolManagementData->playerPosAtLastPlanMkrUpdate.x += ox;
    poolManagementData->playerPosAtLastPlanMkrUpdate.y += oy;
    poolManagementData->playerPosAtLastPlanMkrUpdate.z += oz;
}

void RelocatePlanMarkers(PlanMkr *planMkrList, int numPlanMkrs, SVector *offset)
{
    int i;
    short ox;
    short oy;
    short oz;

    ox = offset->x;
    oy = offset->y;
    oz = offset->z;

    for (i = numPlanMkrs; i != 0; i--, planMkrList++)
    {
        planMkrList->pos.x += ox;
        planMkrList->pos.y += oy;
        planMkrList->pos.z += oz;
    }
}

void RelocateSFXMarkers(SFXMkr *sfxMkrList, int numSFXMkrs, SVector *offset)
{
    int i;
    short ox;
    short oy;
    short oz;

    ox = offset->x;
    oy = offset->y;
    oz = offset->z;

    for (i = numSFXMkrs; i != 0; i--, sfxMkrList++)
    {
        sfxMkrList->pos.x += ox;
        sfxMkrList->pos.y += oy;
        sfxMkrList->pos.z += oz;
    }
}

void STREAM_AdjustMultiSpline(MultiSpline *multi, SVector *offset)
{
    int i;

    if (multi->positional != NULL)
    {
        for (i = 0; i < multi->positional->numkeys; i++)
        {
            multi->positional->key[i].point.x += offset->x;
            multi->positional->key[i].point.y += offset->y;
            multi->positional->key[i].point.z += offset->z;
        }
    }
}

void STREAM_RelocateInstance(Instance *instance, SVector *offset)
{
    STREAM_OffsetInstancePosition(instance, offset, 1);

    INSTANCE_Post(instance, 0x100008, (intptr_t)offset);
}

void STREAM_OffsetInstancePosition(Instance *instance, SVector *offset, int streamSignalFlag)
{
    instance->position.x += offset->x;
    instance->position.y += offset->y;
    instance->position.z += offset->z;

    instance->oldPos.x += offset->x;
    instance->oldPos.y += offset->y;
    instance->oldPos.z += offset->z;

    instance->shadowPosition.x += offset->x;
    instance->shadowPosition.y += offset->y;
    instance->shadowPosition.z += offset->z;

    COLLIDE_UpdateAllTransforms(instance, (SVECTOR *)offset);

    COLLIDE_MoveAllTransforms(instance, (Position *)offset);

    if (instance == theCamera.focusInstance)
    {
        CAMERA_Relocate(&theCamera, offset, streamSignalFlag);
    }
}

void STREAM_SetInstancePosition(Instance *instance, evPositionData *data)
{
    SVector offset;

    offset.x = data->x - instance->position.x;
    offset.y = data->y - instance->position.y;
    offset.z = data->z - instance->position.z;

    STREAM_OffsetInstancePosition(instance, &offset, 0);
}

void RelocateInstances(SVector *offset)
{
    Instance *instance;

    instance = gameTrackerX.instanceList->first;

    while (instance != NULL)
    {
        STREAM_RelocateInstance(instance, offset);

        instance = instance->next;
    }

    FX_Relocate(offset);
}

void RelocateStreamPortals(StreamUnitPortal *StreamUnitList, int NumStreamUnits, SVector *offset)
{
    int i;
    int d;

    for (i = 0; i < NumStreamUnits; i++)
    {
        StreamUnitList[i].maxx += offset->x;
        StreamUnitList[i].maxy += offset->y;
        StreamUnitList[i].maxz += offset->z;

        StreamUnitList[i].minx += offset->x;
        StreamUnitList[i].miny += offset->y;
        StreamUnitList[i].minz += offset->z;

        for (d = 0; d < 3; d++)
        {
            StreamUnitList[i].t1[d].x += offset->x;
            StreamUnitList[i].t1[d].y += offset->y;
            StreamUnitList[i].t1[d].z += offset->z;
        }

        for (d = 0; d < 3; d++)
        {
            StreamUnitList[i].t2[d].x += offset->x;
            StreamUnitList[i].t2[d].y += offset->y;
            StreamUnitList[i].t2[d].z += offset->z;
        }
    }
}

void STREAM_PackVRAMObject(ObjectTracker *objectTracker)
{
    VramSize *vramSize;
    VramBuffer *vramBuffer;
    char fileName[64];
    BlockVramEntry *vramBlock;

    vramSize = &objectTracker->object->vramSize;

    if ((vramSize->x != -1) && (VRAM_GetObjectVramSpace(vramSize, objectTracker) != 0))
    {
        vramBlock = (BlockVramEntry *)objectTracker->vramBlock;

        if (vramBlock != NULL)
        {
            AdjustVramCoordsObject(512, 0, vramBlock->x, vramBlock->y, objectTracker->object);
        }

        sprintf(fileName, "\\kain2\\object\\%s\\%s.crm", objectTracker->name, objectTracker->name);

        vramBuffer = (VramBuffer *)MEMPACK_Malloc((vramBlock->w << 1) + sizeof(VramBuffer), 35);

        vramBuffer->lineOverFlow = (short *)(vramBuffer + 1);

        vramBuffer->flags = 0;

        vramBuffer->x = vramBlock->x;
        vramBuffer->y = vramBlock->y;
        vramBuffer->w = vramBlock->w;
        vramBuffer->h = vramBlock->h;

        vramBuffer->yOffset = 0;

        vramBuffer->lengthOfLeftOverData = 0;

        LOAD_NonBlockingBufferedLoad(fileName, (void *)VRAM_TransferBufferToVram, vramBuffer, objectTracker);
    }
    else
    {
        objectTracker->vramBlock = NULL;
    }
}

void MORPH_SetupInstanceFlags(Instance *instance)
{
    if ((instance->object->oflags & 0x80000))
    {
        return;
    }

    switch (gameTrackerX.gameData.asmData.MorphType)
    {
    case 0:
        if (!(instance->flags2 & 0x8000000))
        {
            instance->flags2 &= ~0x10000000;
            instance->flags2 &= ~0x4000000;
            break;
        }

        instance->flags2 |= 0x10000000;

        if (MEMPACK_MemoryValidFunc((char *)instance->object) == 0)
        {
            break;
        }

        if (((instance->object->oflags2 & 0x2000000)) || ((instance->LinkParent != NULL) && ((instance->LinkParent->object->oflags2 & 0x2000000))))
        {
            INSTANCE_Post(instance, 0x40026, 0);

            instance->flags2 |= 0x4000000;
        }

        break;
    default:
        if ((instance->flags2 & 0x8000000))
        {
            instance->flags2 &= ~0x10000000;
            instance->flags2 &= ~0x4000000;
            break;
        }

        instance->flags2 |= 0x10000000;

        if (MEMPACK_MemoryValidFunc((char *)instance->object) == 0)
        {
            break;
        }

        if (((instance->object->oflags2 & 0x2000000)) || ((instance->LinkParent != NULL) && ((instance->LinkParent->object->oflags2 & 0x2000000))))
        {
            INSTANCE_Post(instance, 0x40026, 0);

            instance->flags2 |= 0x4000000;
        }

        break;
    }
}

void MORPH_SetupInstanceListFlags()
{
    Instance *instance;

    instance = gameTrackerX.instanceList->first;

    while (instance != NULL)
    {
        MORPH_SetupInstanceFlags(instance);

        instance = instance->next;
    }
}

void MORPH_InMorphInstanceListFlags()
{
    Instance *instance;

    instance = gameTrackerX.instanceList->first;

    while (instance != NULL)
    {
        if ((instance->flags2 & 0x4000000))
        {
            instance->flags2 &= ~0x10000000;
        }

        instance->flags2 &= ~0x4000000;

        instance = instance->next;
    }
}

void MORPH_InMorphDoFadeValues()
{
    int fade1;
    int fade2;

    fade1 = (gameTrackerX.gameData.asmData.MorphTime << 12) / 1000;
    fade2 = 4096 - fade1;

    if (gameTrackerX.gameData.asmData.MorphType == 0)
    {
        gameTrackerX.spectral_fadeValue = fade1;
        gameTrackerX.material_fadeValue = fade2;
    }
    else
    {
        gameTrackerX.spectral_fadeValue = fade2;
        gameTrackerX.material_fadeValue = fade1;
    }
}

void MORPH_UpdateTimeMult()
{
    short ratio;
    short temp, temp2; // not from decls.h

    if (gameTrackerX.gameData.asmData.MorphTime != 1000)
    {
        temp2 = gameTrackerX.gameData.asmData.MorphTime;

        if (gameTrackerX.gameData.asmData.MorphType != 0)
        {
            ratio = temp2 - 750;

            if (ratio < 0)
            {
                ratio = 0;
            }
        }
        else
        {
            if (temp2 > 250)
            {
                temp2 = 250;
            }

            ratio = 250 - temp2;
        }

        temp = (ratio << 12) / 250;

        gameTrackerX.materialTimeMult = (long)((gameTrackerX.globalTimeMult * temp) << 4) >> 16;

        if (gameTrackerX.materialTimeMult == 0)
        {
            gameTrackerX.materialTimeMult = 1;
        }

        temp2 = gameTrackerX.gameData.asmData.MorphTime;

        if (gameTrackerX.gameData.asmData.MorphType == 0)
        {
            ratio = temp2 - 750;

            if (ratio < 0)
            {
                ratio = 0;
            }
        }
        else
        {
            if (temp2 > 250)
            {
                temp2 = 250;
            }

            ratio = 250 - temp2;
        }

        temp = (ratio << 12) / 250;

        gameTrackerX.spectralTimeMult = (long)((gameTrackerX.globalTimeMult * temp) << 4) >> 16;

        if (gameTrackerX.spectralTimeMult == 0)
        {
            gameTrackerX.spectralTimeMult = 1;
        }
    }
    else if (gameTrackerX.gameData.asmData.MorphType == 0)
    {
        gameTrackerX.spectralTimeMult = 0;
        gameTrackerX.materialTimeMult = gameTrackerX.globalTimeMult;
    }
    else
    {
        gameTrackerX.materialTimeMult = 0;
        gameTrackerX.spectralTimeMult = gameTrackerX.globalTimeMult;
    }
}

void MORPH_UpdateNormals(Level *BaseLevel)
{
    TFace *face;
    long faceCount;
    TVertex *v;
    MorphVertex *mv;
    MorphColor *mc;
    short h1;
    short *morphNormals;

    morphNormals = BaseLevel->terrain->morphNormalIdx;

    face = BaseLevel->terrain->faceList;

    for (faceCount = BaseLevel->terrain->numFaces; faceCount > 0; faceCount--, face++)
    {
        h1 = face->normal;

        face->normal = *morphNormals;

        *morphNormals++ = h1;
    }

    {
        BSPNode *node;
        BSPLeaf *leaf;
        Terrain *terrain;
        long curTree;

        mv = BaseLevel->terrain->MorphDiffList;

        if (mv != NULL)
        {
            for (; mv->vindex >= 0; mv++)
            {
                v = &BaseLevel->terrain->vertexList[mv->vindex];

                v->vertex.x = mv->hx + mv->x;
                v->vertex.y = mv->hy + mv->y;
                v->vertex.z = mv->hz + mv->z;
            }
        }

        mc = BaseLevel->terrain->MorphColorList;

        if (mc != NULL)
        {
            TVertex *endv;

            endv = &BaseLevel->terrain->vertexList[BaseLevel->terrain->numVertices];

            for (v = BaseLevel->terrain->vertexList; v < endv; v++, mc++)
            {
                v->r0 = ((unsigned short)mc->morphColor15 & 0x1F) << 3;
                v->g0 = ((unsigned short)mc->morphColor15 >> 2) & 0xF8;
                v->b0 = ((unsigned short)mc->morphColor15 >> 7) & 0xF8;
            }
        }

        terrain = BaseLevel->terrain;

        for (curTree = 0; curTree < terrain->numBSPTrees; curTree++)
        {
            Sphere_noSq hsphere;
            BoundingBox hbox;

            for (node = terrain->BSPTreeArray[curTree].bspRoot; (BSPLeaf *)node < terrain->BSPTreeArray[curTree].startLeaves; node++)
            {
                hsphere = node->sphere;

                node->sphere = node->spectralSphere;
                node->spectralSphere = hsphere;
            }

            for (leaf = terrain->BSPTreeArray[curTree].startLeaves; leaf < terrain->BSPTreeArray[curTree].endLeaves; leaf++)
            {
                hsphere = leaf->sphere;

                leaf->sphere = leaf->spectralSphere;
                leaf->spectralSphere = hsphere;

                hbox = leaf->box;

                leaf->box = leaf->spectralBox;
                leaf->spectralBox = hbox;
            }
        }

        {
            Instance *instance;

            for (instance = gameTrackerX.instanceList->first; instance != NULL; instance = instance->next)
            {
                Intro *temp; // not from decls.h

                temp = instance->intro;

                if ((temp != NULL) && ((temp->spectralPosition.x != 0) || (temp->spectralPosition.y != 0) || (temp->spectralPosition.z != 0)) && (!(instance->flags2 & 0x8)))
                {
                    SVECTOR realDiff;
                    Position oldPos;

                    oldPos = instance->position;

                    instance->position.x = temp->position.x + temp->spectralPosition.x;
                    instance->position.y = temp->position.y + temp->spectralPosition.y;
                    instance->position.z = temp->position.z + temp->spectralPosition.z;

                    realDiff.vx = instance->position.x - oldPos.x;
                    realDiff.vy = instance->position.y - oldPos.y;
                    realDiff.vz = instance->position.z - oldPos.z;

                    if (realDiff.vx + realDiff.vy + realDiff.vz != 0)
                    {
                        COLLIDE_UpdateAllTransforms(instance, &realDiff);
                    }
                }
            }
        }
    }
}

void MORPH_BringBackNormals(Level *BaseLevel)
{
    TFace *face;
    long faceCount;
    TVertex *v;
    MorphVertex *mv;
    MorphColor *mc;
    short h1;
    short *morphNormals;

    morphNormals = BaseLevel->terrain->morphNormalIdx;

    face = BaseLevel->terrain->faceList;

    for (faceCount = BaseLevel->terrain->numFaces; faceCount > 0; morphNormals++, face++, faceCount--)
    {
        h1 = face->normal;

        face->normal = *morphNormals;

        *morphNormals = h1;
    }

    mv = BaseLevel->terrain->MorphDiffList;

    if (mv != NULL)
    {
        for (; mv->vindex > -1; mv++)
        {
            v = &BaseLevel->terrain->vertexList[mv->vindex];

            v->vertex.x = mv->hx;
            v->vertex.y = mv->hy;
            v->vertex.z = mv->hz;
        }
    }

    mc = BaseLevel->terrain->MorphColorList;

    if (mc != NULL)
    {
        TVertex *endv;

        endv = &BaseLevel->terrain->vertexList[BaseLevel->terrain->numVertices];

        for (v = BaseLevel->terrain->vertexList; v < endv; v++)
        {
            v->r0 = (v->rgb15 & 0x1F) << 3;
            v->g0 = (v->rgb15 >> 2) & 0xF8;
            v->b0 = (v->rgb15 >> 7) & 0xF8;
        }
    }

    {
        BSPNode *node;
        BSPLeaf *leaf;
        Sphere_noSq hsphere;
        BoundingBox hbox;
        Terrain *terrain;
        long curTree;

        terrain = BaseLevel->terrain;

        for (curTree = 0; curTree < terrain->numBSPTrees; curTree++)
        {
            for (node = terrain->BSPTreeArray[curTree].bspRoot; (BSPLeaf *)node < terrain->BSPTreeArray[curTree].startLeaves; node++)
            {
                hsphere = node->sphere;

                node->sphere = node->spectralSphere;
                node->spectralSphere = hsphere;
            }

            for (leaf = terrain->BSPTreeArray[curTree].startLeaves; leaf < terrain->BSPTreeArray[curTree].endLeaves; leaf++)
            {
                hsphere = leaf->sphere;
                leaf->sphere = leaf->spectralSphere;
                leaf->spectralSphere = hsphere;

                hbox = leaf->box;
                leaf->box = leaf->spectralBox;
                leaf->spectralBox = hbox;
            }
        }
    }

    {
        Instance *instance;

        for (instance = gameTrackerX.instanceList->first; instance != NULL; instance = instance->next)
        {
            if (instance->intro != NULL)
            {
                SVECTOR realDiff;
                Position oldPos;
                Intro *temp; // not from decls.h

                temp = instance->intro;

                if (((temp->spectralPosition.x != 0) || (temp->spectralPosition.y != 0) || (temp->spectralPosition.z != 0)) && (!(instance->flags2 & 0x8)))
                {
                    oldPos = instance->position;

                    instance->position.x = temp->position.x;
                    instance->position.y = temp->position.y;
                    instance->position.z = temp->position.z;

                    realDiff.vx = instance->position.x - oldPos.x;
                    realDiff.vy = instance->position.y - oldPos.y;
                    realDiff.vz = instance->position.z - oldPos.z;

                    if (realDiff.vx + realDiff.vy + realDiff.vz != 0)
                    {
                        COLLIDE_UpdateAllTransforms(instance, &realDiff);
                    }
                }
            }
        }
    }
}

void MORPH_AddOffsets(Level *BaseLevel, int time)
{
    TVertex *v;
    MorphVertex *mv;
    MorphColor *mc;
    // long m; unused
    long fixed_time;
    Instance *instance;
    int temp; // not from decls.h

    instance = gameTrackerX.instanceList->first;

    if (time < 501)
    {
        temp = time * 2;

        time = (temp * temp * temp) / 2000000;
    }
    else
    {
        temp = (1000 - time) * 2;

        time = 1000 - ((temp * temp * temp) / 2000000);
    }

    mv = BaseLevel->terrain->MorphDiffList;

    fixed_time = (time << 12) / 1000;

    if (mv != NULL)
    {
        for (; mv->vindex >= 0; mv++)
        {
            v = &BaseLevel->terrain->vertexList[mv->vindex];

            v->vertex.x = (short)(mv->hx + ((mv->x * fixed_time) >> 12));
            v->vertex.y = (short)(mv->hy + ((mv->y * fixed_time) >> 12));
            v->vertex.z = (short)(mv->hz + ((mv->z * fixed_time) >> 12));
        }
    }

    for (; instance != NULL; instance = instance->next)
    {
        Intro *temp2; // not from decls.h

        temp2 = instance->intro;

        if ((temp2 != NULL) && ((temp2->spectralPosition.x != 0) || (temp2->spectralPosition.y != 0) || (temp2->spectralPosition.z != 0)) && (!(instance->flags2 & 0x8)))
        {
            SVECTOR diff;
            SVECTOR realDiff;
            Position oldPos;

            diff.vx = (short)((temp2->spectralPosition.x * fixed_time) >> 12);
            diff.vy = (short)((temp2->spectralPosition.y * fixed_time) >> 12);
            diff.vz = (short)((temp2->spectralPosition.z * fixed_time) >> 12);

            oldPos = instance->position;

            instance->position.x = temp2->position.x + diff.vx;
            instance->position.y = temp2->position.y + diff.vy;
            instance->position.z = temp2->position.z + diff.vz;

            realDiff.vx = instance->position.x - oldPos.x;
            realDiff.vy = instance->position.y - oldPos.y;
            realDiff.vz = instance->position.z - oldPos.z;

            if (realDiff.vx + realDiff.vy + realDiff.vz != 0)
            {
                COLLIDE_UpdateAllTransforms(instance, &realDiff);
            }
        }
    }

    {
        long r0;
        long g0;
        long b0;
        long r1;
        long g1;
        long b1;

        mc = BaseLevel->terrain->MorphColorList;

        if (mc != NULL)
        {
            TVertex *endv;

            endv = &BaseLevel->terrain->vertexList[BaseLevel->terrain->numVertices];

            for (v = BaseLevel->terrain->vertexList; v < endv; v++, mc++)
            {
                r0 = (v->rgb15 & 0x1F) << 3;
                r1 = (mc->morphColor15 & 0x1F) << 3;

                g0 = (v->rgb15 >> 2) & 0xF8;
                g1 = (mc->morphColor15 >> 2) & 0xF8;

                b0 = (v->rgb15 >> 7) & 0xF8;
                b1 = (mc->morphColor15 >> 7) & 0xF8;

                v->r0 = (unsigned char)(r0 + (((r1 - r0) * fixed_time) >> 12));
                v->g0 = (unsigned char)(g0 + (((g1 - g0) * fixed_time) >> 12));
                v->b0 = (unsigned char)(b0 + (((b1 - b0) * fixed_time) >> 12));
            }
        }
    }
}

void MORPH_SubtractOffsets(Level *BaseLevel, int time)
{
    TVertex *v;
    MorphVertex *mv;
    MorphColor *mc;
    // long m; unused
    long fixed_time;
    Instance *instance;
    int temp; // not from decls.h

    instance = gameTrackerX.instanceList->first;

    if (time < 501)
    {
        temp = time * 2;

        time = (temp * temp * temp) / 2000000;
    }
    else
    {
        temp = (1000 - time) * 2;

        time = 1000 - ((temp * temp * temp) / 2000000);
    }

    mv = BaseLevel->terrain->MorphDiffList;

    fixed_time = ((1000 - time) << 12) / 1000;

    if (mv != NULL)
    {
        for (; mv->vindex >= 0; mv++)
        {
            v = &BaseLevel->terrain->vertexList[mv->vindex];

            v->vertex.x = (short)(mv->hx + ((mv->x * fixed_time) >> 12));
            v->vertex.y = (short)(mv->hy + ((mv->y * fixed_time) >> 12));
            v->vertex.z = (short)(mv->hz + ((mv->z * fixed_time) >> 12));
        }
    }

    for (; instance != NULL; instance = instance->next)
    {
        Intro *temp2; // not from decls.h

        temp2 = instance->intro;

        if ((temp2 != NULL) && ((temp2->spectralPosition.x != 0) || (temp2->spectralPosition.y != 0) || (temp2->spectralPosition.z != 0)) && (!(instance->flags2 & 0x8)))
        {
            SVECTOR diff;
            SVECTOR realDiff;
            Position oldPos;

            diff.vx = (short)((temp2->spectralPosition.x * fixed_time) >> 12);
            diff.vy = (short)((temp2->spectralPosition.y * fixed_time) >> 12);
            diff.vz = (short)((temp2->spectralPosition.z * fixed_time) >> 12);

            oldPos = instance->position;

            instance->position.x = temp2->position.x + diff.vx;
            instance->position.y = temp2->position.y + diff.vy;
            instance->position.z = temp2->position.z + diff.vz;

            realDiff.vx = instance->position.x - oldPos.x;
            realDiff.vy = instance->position.y - oldPos.y;
            realDiff.vz = instance->position.z - oldPos.z;

            if (realDiff.vx + realDiff.vy + realDiff.vz != 0)
            {
                COLLIDE_UpdateAllTransforms(instance, &realDiff);
            }
        }
    }

    {
        long r0;
        long g0;
        long b0;
        long r1;
        long g1;
        long b1;

        mc = BaseLevel->terrain->MorphColorList;

        if (mc != NULL)
        {
            TVertex *endv;

            endv = &BaseLevel->terrain->vertexList[BaseLevel->terrain->numVertices];

            for (v = BaseLevel->terrain->vertexList; v < endv; v++, mc++)
            {
                r0 = (v->rgb15 & 0x1F) << 3;
                r1 = (mc->morphColor15 & 0x1F) << 3;

                g0 = (v->rgb15 >> 2) & 0xF8;
                g1 = (mc->morphColor15 >> 2) & 0xF8;

                b0 = (v->rgb15 >> 7) & 0xF8;
                b1 = (mc->morphColor15 >> 7) & 0xF8;

                v->r0 = (unsigned char)(r0 + (((r1 - r0) * fixed_time) >> 12));
                v->g0 = (unsigned char)(g0 + (((g1 - g0) * fixed_time) >> 12));
                v->b0 = (unsigned char)(b0 + (((b1 - b0) * fixed_time) >> 12));
            }
        }
    }
}

void MORPH_GetComponentsForTrackingPoint(TFace *face, Level *level)
{
    SVector *v[3];
    Position *offset;
    Position player;
    int n;
    int saved_div;
    int next;
    int side;
    int track;
    int x[2];
    TVertex *vertexList;

    MORPH_SavedFace = face;

    MORPH_SavedLevel = level;

    if ((face != NULL) && (level != NULL))
    {
        vertexList = level->terrain->vertexList;

        v[0] = (SVector *)&vertexList[face->face.v0].vertex;
        v[1] = (SVector *)&vertexList[face->face.v1].vertex;
        v[2] = (SVector *)&vertexList[face->face.v2].vertex;

        offset = &level->terrain->BSPTreeArray[gameTrackerX.playerInstance->bspTree].globalOffset;

        SUB_SVEC(Position, &player, Position, &gameTrackerX.playerInstance->position, Position, offset);

        for (track = 0; track < 2; track++)
        {
            int div;

            saved_div = 0;

            side = 0;

            for (n = 0; n < 3; n++)
            {
                next = n + 1;

                if (next >= 3)
                {
                    next = 0;
                }

                if (((track != 1) || (n != MORPH_Track[0])) && (((player.y >= v[n]->y) && (player.y <= v[next]->y)) || ((player.y >= v[next]->y) && (player.y <= v[n]->y))))
                {
                    div = v[next]->y - v[n]->y;

                    if (ABS(div) > ABS(saved_div))
                    {
                        saved_div = div;

                        side = n;
                    }
                }
            }

            {
                int next;

                MORPH_Track[track] = side;

                next = side + 1;

                if (next >= 3)
                {
                    next = 0;
                }

                if (saved_div != 0)
                {
                    MORPH_Component[track] = ((player.y - v[side]->y) << 12) / saved_div;
                }
                else
                {
                    MORPH_Component[track] = 0;
                }

                x[track] = v[side]->x + ((MORPH_Component[track] * (v[next]->x - v[side]->x)) / 4096);
            }
        }

        if (x[0] != x[1])
        {
            MORPH_Component[2] = ((player.x - x[0]) << 12) / (x[1] - x[0]);
        }
        else
        {
            MORPH_Component[2] = 0;
        }
    }
}

void MORPH_AveragePoint(SVector *start, SVector *end, int interp, SVector *out)
{
    if (interp >= 0)
    {
        if (interp >= 4097)
        {
            interp = 4096;
        }

        LoadAverageShort12((SVECTOR *)start, (SVECTOR *)end, 4096 - interp, interp, (SVECTOR *)out);
    }
    else
    {
        if (interp < -4096)
        {
            interp = -4096;
        }

        LoadAverageShort12((SVECTOR *)end, (SVECTOR *)start, interp + 4096, -interp, (SVECTOR *)out);
    }
}

void MORPH_UpdateTrackingPoint(TFace *face, Level *level)
{
    SVector *v[3];
    SVector p1;
    SVector p2;
    SVector p3;
    Position *offset;
    int next;
    TVertex *vertexList;

    if ((face != NULL) && (level != NULL))
    {
        vertexList = level->terrain->vertexList;

        v[0] = (SVector *)&vertexList[face->face.v0];
        v[1] = (SVector *)&vertexList[face->face.v1];
        v[2] = (SVector *)&vertexList[face->face.v2];

        next = MORPH_Track[0] + 1;

        if (next >= 3)
        {
            next = 0;
        }

        MORPH_AveragePoint(v[MORPH_Track[0]], v[next], MORPH_Component[0], &p1);

        next = MORPH_Track[1] + 1;

        if (next >= 3)
        {
            next = 0;
        }

        MORPH_AveragePoint(v[MORPH_Track[1]], v[next], MORPH_Component[1], &p2);
        MORPH_AveragePoint(&p1, &p2, MORPH_Component[2], &p3);

        offset = &level->terrain->BSPTreeArray[gameTrackerX.playerInstance->bspTree].globalOffset;

        ADD_SVEC(SVector, &p3, SVector, &p3, Position, offset);

        COPY_SVEC(Position, &gameTrackerX.playerInstance->position, SVector, &p3);

        gameTrackerX.playerInstance->cachedTFace = -1;
        gameTrackerX.playerInstance->cachedTFaceLevel = NULL;
    }
}

void MORPH_ToggleMorph()
{
    Level *level;
    int i;
    StreamUnit *temp; // not from decls.h

    SOUND_PlaneShift(gameTrackerX.gameData.asmData.MorphType == 0);

    INSTANCE_Broadcast(NULL, 0xA, 0x1000020, gameTrackerX.gameData.asmData.MorphType);

    MORPH_GetComponentsForTrackingPoint(gameTrackerX.playerInstance->tface, (Level *)gameTrackerX.playerInstance->tfaceLevel);

    MORPH_UpdateTrackingPoint(gameTrackerX.playerInstance->tface, (Level *)gameTrackerX.playerInstance->tfaceLevel);

    do
    {
    } while (0); // garbage code for reordering

    gameTrackerX.gameData.asmData.MorphTime = 0;

    SOUND_Play3dSound(&gameTrackerX.playerInstance->position, 26, -350, 127, 32767);

    for (temp = StreamTracker.StreamList, i = 16; i > 0; i--, temp++)
    {
        if (temp->used == 2)
        {
            level = temp->level;

            if (gameTrackerX.gameData.asmData.MorphType == 1)
            {
                if (level->materialSignal != NULL)
                {
                    level->materialSignal->flags |= 0x1;

                    SIGNAL_HandleSignal(gameTrackerX.playerInstance, level->materialSignal->signalList, 0);

                    EVENT_AddSignalToReset(level->materialSignal);
                }
            }
            else if (level->spectralSignal != NULL)
            {
                level->spectralSignal->flags |= 0x1;

                SIGNAL_HandleSignal(gameTrackerX.playerInstance, level->spectralSignal->signalList, 0);

                EVENT_AddSignalToReset(level->spectralSignal);
            }
        }
    }
}

void MORPH_DoStep(StreamUnit *streamUnit, char *baseAreaName)
{
    Level *level;

    (void)baseAreaName;

    GAMEPAD_Shock1(64, 61440);

    level = streamUnit->level;

    if (level->terrain->MorphDiffList != NULL)
    {
        VM_UpdateMorph(level, 0);

        if (gameTrackerX.gameData.asmData.MorphTime == 1000)
        {
            if (gameTrackerX.gameData.asmData.MorphType == 0)
            {
                gameTrackerX.gameData.asmData.MorphType ^= 1;

                MORPH_UpdateNormals(level);
            }
            else
            {
                gameTrackerX.gameData.asmData.MorphType ^= 1;

                MORPH_BringBackNormals(level);
            }

            gameTrackerX.gameData.asmData.MorphType ^= 1;

            level->morphLastStep = -1;
        }
        else if (gameTrackerX.gameData.asmData.MorphType == 0)
        {
            MORPH_AddOffsets(level, gameTrackerX.gameData.asmData.MorphTime);
        }
        else
        {
            MORPH_SubtractOffsets(level, gameTrackerX.gameData.asmData.MorphTime);
        }
    }
}

void MORPH_SetFog(StreamUnit *streamUnit, int mainUnitFlag)
{
    long time;
    Level *level;
    int fogNear;
    int fogFar;

    time = (gameTrackerX.gameData.asmData.MorphTime << 12) / 1000;

    level = streamUnit->level;

    if (gameTrackerX.gameData.asmData.MorphType == 0)
    {
        time = 4096 - time;
    }

    fogNear = (((level->holdFogNear - level->spectralFogNear) * time) / 4096) + level->spectralFogNear;
    fogFar = (((level->holdFogFar - level->spectralFogFar) * time) / 4096) + level->spectralFogFar;

    if (mainUnitFlag != 0)
    {
        streamUnit->UnitFogNear = fogNear;
        streamUnit->TargetFogNear = fogNear;

        streamUnit->UnitFogFar = fogFar;
        streamUnit->TargetFogFar = fogFar;
    }
    else
    {
        STREAM_SetStreamFog(streamUnit, (short)fogNear, (short)fogFar);
    }
}

void MORPH_UpdateTextures()
{
    int time;

    time = (gameTrackerX.gameData.asmData.MorphTime << 12) / 1000;

    if (gameTrackerX.gameData.asmData.MorphType == 1)
    {
        time = 4096 - time;
    }

    MORPH_ChangeAreaPalettes(time);
}

void MORPH_Continue()
{
    int i;
    long mode;

    if (gameTrackerX.gameData.asmData.MorphTime == 0)
    {
        MORPH_InMorphInstanceListFlags();
    }

    gameTrackerX.gameData.asmData.MorphTime += 20;

    MORPH_InMorphDoFadeValues();

    for (i = 0; i < 16; i++)
    {
        if (StreamTracker.StreamList[i].used == 2)
        {
            MORPH_DoStep(&StreamTracker.StreamList[i], StreamTracker.StreamList[i].baseAreaName);

            MORPH_SetFog(&StreamTracker.StreamList[i], 0);
        }
    }

    mode = INSTANCE_Query(gameTrackerX.playerInstance, 10);

    if ((gameTrackerX.playerInstance->tface != NULL) && (mode >= 0))
    {
        if (MORPH_SavedFace == gameTrackerX.playerInstance->tface)
        {
            MORPH_UpdateTrackingPoint(MORPH_SavedFace, MORPH_SavedLevel);
        }
        else
        {
            MORPH_GetComponentsForTrackingPoint(gameTrackerX.playerInstance->tface, (Level *)gameTrackerX.playerInstance->tfaceLevel);

            MORPH_UpdateTrackingPoint(gameTrackerX.playerInstance->tface, (Level *)gameTrackerX.playerInstance->tfaceLevel);
        }
    }

    MORPH_UpdateTextures();

    if (gameTrackerX.gameData.asmData.MorphTime == 1000)
    {
        gameTrackerX.gameData.asmData.MorphType ^= 1;

        if (gameTrackerX.gameData.asmData.MorphType == 0)
        {
            gameTrackerX.playerInstance->flags2 &= ~0x8000000;
        }
        else
        {
            gameTrackerX.playerInstance->flags2 |= 0x8000000;
        }

        MORPH_SetupInstanceListFlags();
    }
}

void STREAM_MORPH_Relocate()
{
    MORPH_SavedFace = NULL;
    MORPH_SavedLevel = NULL;
}

int AddVertex(VECTOR *v0, RECT *rect)
{
    SVECTOR v;
    int x;
    int y;
    int z;
    int scr_x;
    int scr_y;

    if (v0->vx < 0)
    {
        v0->vx += -32768;
    }
    else if (v0->vx > 0)
    {
        v0->vx += 32768;
    }

    if (v0->vy < 0)
    {
        v0->vy += -32768;
    }
    else if (v0->vy > 0)
    {
        v0->vy += 32768;
    }

    if (v0->vz < 0)
    {
        v0->vz += -32768;
    }
    else if (v0->vz > 0)
    {
        v0->vz += 32768;
    }

    v.vx = v0->vx >> 16;
    v.vy = v0->vy >> 16;
    v.vz = v0->vz >> 16;

    gte_ldv0(&v);
    gte_nrtv0tr();
    gte_stsv(&v);

    if (v.vz <= 0)
    {
        return -1;
    }

    do
    {
    } while (0); // garbage code for reodering

    x = v.vx;
    y = v.vy;
    z = v.vz;

    scr_x = ((x * 320) / z) + (512 / 2);
    scr_y = ((y * 320) / z) + (240 / 2);

    rect->x = (rect->x < scr_x) ? rect->x : scr_x;
    rect->w = (scr_x >= rect->w) ? scr_x : rect->w;
    rect->y = (rect->y < scr_y) ? rect->y : scr_y;
    rect->h = (scr_y >= rect->h) ? scr_y : rect->h;

    return z;
}

int GetPlaneDist(int k, int j, int i, VECTOR *v)
{
    (void)i;

    return (((v->vx >> 12) - (theCamera.core.position.x << 4)) * theCamera.core.vvNormalWorVecMat[k].m[j][0]) +
        (((v->vy >> 12) - (theCamera.core.position.y << 4)) * theCamera.core.vvNormalWorVecMat[k].m[j][1]) +
        (((v->vz >> 12) - (theCamera.core.position.z << 4)) * theCamera.core.vvNormalWorVecMat[k].m[j][2]);
}

void CalcVert(VECTOR *v, VECTOR *v1, VECTOR *v2, int dist1, int dist2, int k, int j)
{
    int diff;

    (void)k;
    (void)j;

    diff = (dist1 - dist2) >> 12;

    v->vx = (((v2->vx - v1->vx) >> 12) * (dist2 / diff)) + v2->vx;
    v->vy = (((v2->vy - v1->vy) >> 12) * (dist2 / diff)) + v2->vy;
    v->vz = (((v2->vz - v1->vz) >> 12) * (dist2 / diff)) + v2->vz;
}

INCLUDE_ASM("asm/nonmatchings/Game/STREAM", AddClippedTri);

int STREAM_GetClipRect(StreamUnitPortal *portal, RECT *rect)
{
    int v1x;
    int v1y;
    int v1z;
    int v2x;
    int v2y;
    int v2z;
    int nx;
    int ny;
    int nz;
    int nx2;
    int ny2;
    int nz2;
    int len;
    int side;
    int side2;
    int retval;
    int horizontal_flag;
    int fullscreen_flag;
    int minz;
    int minz2;

    fullscreen_flag = 0;
    horizontal_flag = 0;

    if (portal->t1[0].z == portal->t1[1].z)
    {
        horizontal_flag = (portal->t1[0].z == portal->t1[2].z);
    }

    v1y = portal->t1[0].y - portal->t1[1].y;
    v2z = portal->t1[0].z - portal->t1[2].z;
    v1z = portal->t1[0].z - portal->t1[1].z;
    v2y = portal->t1[0].y - portal->t1[2].y;
    v1x = portal->t1[0].x - portal->t1[1].x;
    v2x = portal->t1[0].x - portal->t1[2].x;

    nx = ((v1y * v2z) - (v2y * v1z)) >> 12;
    ny = ((v2x * v1z) - (v1x * v2z)) >> 12;
    nz = ((v1x * v2y) - (v2x * v1y)) >> 12;

    side = -(((portal->t1[0].x - theCamera.core.position.x) * nx) +
             ((portal->t1[0].y - theCamera.core.position.y) * ny) +
             ((portal->t1[0].z - theCamera.core.position.z) * nz));

    len = MATH3D_FastSqrt((nx * nx) + (ny * ny) + (nz * nz));

    if (kabs(side) < len)
    {
        v1y = portal->t2[0].y - portal->t2[1].y;
        v2z = portal->t2[0].z - portal->t2[2].z;
        v1z = portal->t2[0].z - portal->t2[1].z;
        v2y = portal->t2[0].y - portal->t2[2].y;
        v2x = portal->t2[0].x - portal->t2[2].x;
        v1x = portal->t2[0].x - portal->t2[1].x;

        nx2 = ((v1y * v2z) - (v2y * v1z)) >> 12;
        ny2 = ((v2x * v1z) - (v1x * v2z)) >> 12;
        nz2 = ((v1x * v2y) - (v2x * v1y)) >> 12;

        side2 = -(((portal->t2[0].x - theCamera.core.position.x) * nx2) +
                  ((portal->t2[0].y - theCamera.core.position.y) * ny2) +
                  ((portal->t2[0].z - theCamera.core.position.z) * nz2));

        MATH3D_FastSqrt((nx2 * nx2) + (ny2 * ny2) + (nz2 * nz2));

        if (side < side2)
        {
            side = side2;
            nx = nx2;
            ny = ny2;
            nz = nz2;
        }
    }

    if (memcmp(portal->tolevelname, "warpgate", sizeof("warpgate") - 1) != 0)
    {
        if (kabs(side) < len)
        {
            int dot;

            dot = -((((theCamera.focusInstance->matrix + 1)->t[0] - theCamera.core.position.x) * nx) +
                    (((theCamera.focusInstance->matrix + 1)->t[1] - theCamera.core.position.y) * ny) +
                    (((theCamera.focusInstance->matrix + 1)->t[2] - theCamera.core.position.z) * nz));

            if (dot > 0 && horizontal_flag == 0)
            {
                fullscreen_flag = 1;
            }
        }
    }

    if (side < -1024)
    {
        if (fullscreen_flag == 0)
        {
            return 0;
        }
    }

    rect->w += rect->x;
    rect->h += rect->y;

    gte_SetRotMatrix(theCamera.core.wcTransform);
    gte_SetTransMatrix(theCamera.core.wcTransform);

    retval = AddClippedTri((SVECTOR *)&portal->t1[0], rect, &minz);
    retval |= AddClippedTri((SVECTOR *)&portal->t2[0], rect, &minz2);

    if (retval == 0)
    {
        rect->w -= rect->x;
        rect->h -= rect->y;

        return 0;
    }
    else
    {
        if (minz2 < minz)
        {
            minz = minz2;
        }

        if (fullscreen_flag != 0)
        {
            if (minz < 64)
            {
                rect->x = 0;
                rect->y = 0;
                rect->w = 512;
                rect->h = 240;

                return 1;
            }
        }

        if (rect->x < 0)
        {
            rect->x = 0;
        }

        if (rect->y < 0)
        {
            rect->y = 0;
        }

        if (rect->w >= 513)
        {
            rect->w = 512;
        }

        if (rect->h >= 241)
        {
            rect->h = 240;
        }

        rect->w -= rect->x;
        rect->h -= rect->y;

        if (horizontal_flag != 0)
        {
            if (kabs(side) < len)
            {
                if (nz > 0)
                {
                    rect->h = 240 - rect->y;
                }
                else
                {
                    rect->h = rect->y + rect->h;
                    rect->y = 0;
                }
            }
        }

        if (rect->w <= 0)
        {
            return 0;
        }
        else
        {
            return (rect->h > 0);
        }
    }

    return 0;
}

long GetFogColor(struct StreamUnitPortal *portal, struct _StreamUnit *mainStreamUnit, struct Level *mainLevel)
{
    long z1;
    long z2;
    long z3;
    long zval;
    long zval2;
    long interp;
    long Color;
    long MainColor;
    long LevelColor;
    struct Level *level;
    int time;

    MainColor = mainStreamUnit->FogColor;

    level = portal->toStreamUnit->level;

    if (gameTrackerX.gameData.asmData.MorphTime != 1000)
    {
        time = (gameTrackerX.gameData.asmData.MorphTime << 12) / 1000;

        if (gameTrackerX.gameData.asmData.MorphType == 1)
        {
            time = 4096 - time;
        }

        LoadAverageCol(&level->specturalColorR, &level->backColorR, time, 4096 - time, (unsigned char *)&LevelColor);

        LevelColor &= 0xFFFFFF;
    }
    else
    {
        if (gameTrackerX.gameData.asmData.MorphType == 1)
        {
            LevelColor = ((int *)&level->specturalColorR)[0];
        }
        else
        {
            LevelColor = ((int *)&level->backColorR)[0];
        }
    }

    gte_ldv3(&portal->t1[0], &portal->t1[1], &portal->t1[2]);

    gte_nrtpt();

    gte_stsz3(&z1, &z2, &z3);

    if (z1 > z2)
    {
        zval = z3;

        if (z3 < z1)
        {
            zval = z1;
        }
    }
    else
    {
        zval = z3;

        if (z3 < z2)
        {
            zval = z2;
        }
    }

    gte_ldv3(&portal->t2[0], &portal->t2[1], &portal->t2[2]);

    gte_nrtpt();

    gte_stsz3(&z1, &z2, &z3);

    if (z1 > z2)
    {
        zval2 = z3;

        if (z3 < z1)
        {
            zval2 = z1;
        }
    }
    else
    {
        zval2 = z3;

        if (z3 < z2)
        {
            zval2 = z2;
        }
    }

    if (zval < zval2)
    {
        zval = zval2;
    }

    zval += 128;

    zval = zval < mainLevel->fogFar ? zval : mainLevel->fogFar;

    s_zval = zval >> 2;

    if (MainColor == LevelColor)
    {
        Color = LevelColor & 0xFFF8F8F8;

        if (Color != 0)
        {
            Color |= 0x40404;
        }

        return Color;
    }

    zval -= mainLevel->fogNear;

    if (zval < 0)
    {
        return LevelColor;
    }

    interp = ((zval << 16) / (mainLevel->fogFar - mainLevel->fogNear));
    interp = interp >> 4;

    if (interp < 0)
    {
        interp = 0;
    }

    if (interp >= 4097)
    {
        interp = 4096;
    }

    LoadAverageCol((unsigned char *)&MainColor, (unsigned char *)&LevelColor, interp, 4096 - interp, (unsigned char *)&Color);

    if (interp >= 4091)
    {
        Color &= 0xFFF8F8F8;

        if (Color != 0)
        {
            Color |= 0x40404;
        }
    }

    Color &= 0xFFFFFF;

    return Color;
}

void DrawFogRectangle(RECT *cliprect, PrimPool *primPool, int otzpos, unsigned long **drawot, long color)
{
    POLY_G4 *polyg4;
    POLY_G4 *temp; // not from decls.h

    (void)primPool;

    polyg4 = (POLY_G4 *)gameTrackerX.primPool->nextPrim;

    temp = polyg4 + 1;

    if (temp < (POLY_G4 *)gameTrackerX.primPool->lastPrim)
    {
        gameTrackerX.primPool->nextPrim = (uintptr_t *)temp;

        polyg4->x0 = cliprect->x;
        polyg4->y0 = cliprect->y;

        polyg4->x1 = cliprect->x + cliprect->w;
        polyg4->y1 = cliprect->y;

        polyg4->x2 = cliprect->x;
        polyg4->y2 = cliprect->y + cliprect->h;

        polyg4->x3 = cliprect->x + cliprect->w;
        polyg4->y3 = cliprect->y + cliprect->h;

        *(int *)&polyg4->r0 = color;
        *(int *)&polyg4->r1 = color;
        *(int *)&polyg4->r2 = color;
        *(int *)&polyg4->r3 = color;

        setPolyG4(polyg4);

        // addPrim(drawot[otzpos], polyg4);
        *(int *)polyg4 = getaddr(&drawot[otzpos]) | 0x8000000;
        drawot[otzpos] = (unsigned long *)((intptr_t)polyg4 & 0xFFFFFF);
    }
}

void STREAM_RenderAdjacantUnit(unsigned long **curOT, StreamUnitPortal *curStreamPortal, StreamUnit *toStreamUnit, StreamUnit *mainStreamUnit, RECT *cliprect)
{
    long portalFogColor;

    if (MEMPACK_MemoryValidFunc((char *)toStreamUnit->level) != 0)
    {
        RENDER_currentStreamUnitID = (short)toStreamUnit->StreamUnitID;

        portalFogColor = GetFogColor(curStreamPortal, mainStreamUnit, mainStreamUnit->level);

        DrawFogRectangle(cliprect, gameTrackerX.primPool, 3071, curOT, portalFogColor);

        PushMatrix();

        StreamRenderLevel(toStreamUnit, mainStreamUnit->level, curOT, portalFogColor);

        PopMatrix();
    }
}

BSPTree *STREAM_GetBspTree(StreamUnit *streamUnit, long bspNumber)
{
    Level *level;
    Terrain *terrain;
    int d;

    level = streamUnit->level;

    terrain = level->terrain;

    if (terrain->numBSPTrees != 0)
    {
        for (d = 0; d < terrain->numBSPTrees; d++)
        {
            if (terrain->BSPTreeArray[d].ID == bspNumber)
            {
                return &terrain->BSPTreeArray[d];
            }
        }
    }

    return NULL;
}

void WARPGATE_BlockWarpGateEntrance(StreamUnit *streamUnit, long collideOn)
{
    BSPTree *bspTree;

    bspTree = STREAM_GetBspTree(streamUnit, 7);

    if (bspTree != NULL)
    {
        if (collideOn != 0)
        {
            bspTree->flags |= 0x2;

            WarpGateLoadInfo.blocked = 0;
            return;
        }

        bspTree->flags &= ~0x2;

        WarpGateLoadInfo.blocked = 1;
    }
}

void WARPGATE_DrawWarpGateRim(StreamUnit *streamUnit, long drawOn)
{
    BSPTree *bspTree;

    bspTree = STREAM_GetBspTree(streamUnit, 3);

    if (bspTree != NULL)
    {
        bspTree->flags &= ~0x2;

        if (drawOn != 0)
        {
            bspTree->flags &= ~0x1;
        }
        else
        {
            bspTree->flags |= 0x1;
        }
    }
}

void WARPGATE_HideAllCloudCovers()
{
    Object *warpFaceObject;

    warpFaceObject = (Object *)objectAccess[23].object;

    if (warpFaceObject != NULL)
    {
        Instance *instance;
        Instance *next;

        for (instance = gameTrackerX.instanceList->first; instance != NULL; instance = next)
        {
            next = instance->next;

            if (instance->object == warpFaceObject)
            {
                instance->flags |= 0x800;
            }
        }
    }
}

Instance *WARPGATE_UnHideCloudCoverInUnit(long streamUnitID)
{
    Object *warpFaceObject;
    Instance *result;

    warpFaceObject = (Object *)objectAccess[23].object;

    result = NULL;

    if (warpFaceObject != NULL)
    {
        Instance *instance;
        Instance *next;

        for (instance = gameTrackerX.instanceList->first; instance != NULL; instance = next)
        {
            next = instance->next;

            if ((instance->object == warpFaceObject) && (instance->currentStreamUnitID == streamUnitID))
            {
                result = instance;

                result->flags = instance->flags & ~0x800;
                break;
            }
        }
    }

    return result;
}

void STREAM_RenderWarpGate(unsigned long **mainOT, StreamUnitPortal *curStreamPortal, StreamUnit *mainStreamUnit, RECT *cliprect)
{
    StreamUnit *toStreamUnit;

    WARPGATE_DrawWarpGateRim(mainStreamUnit, 1);

    toStreamUnit = curStreamPortal->toStreamUnit;

    if (WarpGateLoadInfo.loading == 1)
    {
        if (WarpGateLoadInfo.curTime <= WarpGateLoadInfo.maxTime)
        {
            if (WarpGateLoadInfo.warpFaceInstance == NULL)
            {
                WARPGATE_HideAllCloudCovers();

                WarpGateLoadInfo.warpFaceInstance = WARPGATE_UnHideCloudCoverInUnit(mainStreamUnit->StreamUnitID);

                WarpGateLoadInfo.fadeValue = 4096;
            }

            WARPGATE_CalcWarpFade(gameTrackerX.timeMult);
        }
        else
        {
            if (toStreamUnit != NULL)
            {
                STREAM_DumpUnit(toStreamUnit, 1);
            }

            WarpGateLoadInfo.loading = 2;

            STREAM_LoadCurrentWarpRoom(curStreamPortal, mainStreamUnit);

            WarpGateLoadInfo.warpFaceInstance->fadeValue = 0;

            return;
        }
    }
    else if (WarpGateLoadInfo.loading == 2)
    {
        WarpGateLoadInfo.fadeValue = 0;

        if ((toStreamUnit != NULL) && (toStreamUnit->used == 2))
        {
            WarpGateLoadInfo.loading = 3;

            WarpGateLoadInfo.curTime = WarpGateLoadInfo.maxTime;
        }
    }
    else if (WarpGateLoadInfo.loading == 3)
    {
        if (WarpGateLoadInfo.curTime >= 0)
        {
            WARPGATE_CalcWarpFade((int)-gameTrackerX.timeMult);
        }
        else
        {
            WARPGATE_HideAllCloudCovers();

            if ((WARPGATE_IsWarpgateInUse() != 0) || (strcmpi(gameTrackerX.baseAreaName, WarpRoomArray[CurrentWarpNumber].name) != 0))
            {
                WarpGateLoadInfo.loading = 4;
            }
            else
            {
                WarpGateLoadInfo.loading = 0;

                CurrentWarpNumber = 0;
            }
        }
    }

    if (WarpGateLoadInfo.loading != 0)
    {
        if ((toStreamUnit != NULL) && (toStreamUnit->used == 2))
        {
            WARPGATE_RenderWarpUnit(mainOT, curStreamPortal, mainStreamUnit, cliprect);
        }

        if ((WarpGateLoadInfo.loading == 4) && (WARPGATE_IsWarpgateInUse() == 0))
        {
            WarpGateLoadInfo.loading = 1;

            WarpGateLoadInfo.curTime = 0;

            WarpGateLoadInfo.warpFaceInstance = NULL;

            WarpRoomArray[CurrentWarpNumber].streamUnit = NULL;

            while (strcmpi(gameTrackerX.baseAreaName, WarpRoomArray[CurrentWarpNumber].name) != 0)
            {
                CurrentWarpNumber = (CurrentWarpNumber + 1) % 14;
            }
        }
    }
    else
    {
        if (WARPGATE_IsWarpgateInUse() != 0)
        {
            if (strcmpi(mainStreamUnit->level->worldName, WarpRoomArray[CurrentWarpNumber].name) == 0)
            {
                CurrentWarpNumber = (CurrentWarpNumber + 1) % 14;
            }

            WarpGateLoadInfo.loading = 1;

            WarpGateLoadInfo.warpFaceInstance = NULL;

            WarpGateLoadInfo.curTime = 0;
        }
        else
        {
            WARPGATE_BlockWarpGateEntrance(mainStreamUnit, 1);
        }
    }
}

void WARPGATE_RenderWarpUnit(unsigned long **mainOT, StreamUnitPortal *curStreamPortal, StreamUnit *mainStreamUnit, RECT *cliprect)
{
    unsigned long **curOT;
    DR_AREA *PortalClip;
    StreamUnit *toStreamUnit;

    toStreamUnit = curStreamPortal->toStreamUnit;

    if ((gameTrackerX.debugFlags2 & 0x1000000))
    {
        FONT_Print("Looking at warp unit =%s\n", &toStreamUnit->baseAreaName);
    }

    if ((toStreamUnit->flags & 0x8))
    {
        WARPGATE_BlockWarpGateEntrance(mainStreamUnit, 1);
    }
    else
    {
        WARPGATE_BlockWarpGateEntrance(mainStreamUnit, 0);
    }

    WARPGATE_DrawWarpGateRim(toStreamUnit, 1);

    if (MEMPACK_MemoryValidFunc((char *)toStreamUnit->level) != 0)
    {
        curOT = (unsigned long **)gameTrackerX.primPool->nextPrim;

        if (curOT < (unsigned long **)(gameTrackerX.primPool->lastPrim - 3072))
        {
            RECT PortalRect;
            long portalFogColor;
            unsigned long *hld;

            gameTrackerX.primPool->nextPrim = (unsigned long *)&curOT[3072];

            ClearOTagR((u_long *)curOT, 3072);

            RENDER_currentStreamUnitID = toStreamUnit->StreamUnitID;

            curStreamPortal->toStreamUnit = toStreamUnit;

            portalFogColor = GetFogColor(curStreamPortal, mainStreamUnit, mainStreamUnit->level);

            DrawFogRectangle(cliprect, gameTrackerX.primPool, 3071, curOT, portalFogColor);

            PushMatrix();

            StreamRenderLevel(toStreamUnit, mainStreamUnit->level, curOT, portalFogColor);

            PopMatrix();

            PortalClip = (DR_AREA *)gameTrackerX.primPool->nextPrim;

            gameTrackerX.primPool->nextPrim = (unsigned long *)(PortalClip + 1);

            PortalRect = *cliprect;

            PortalRect.x += draw[gameTrackerX.drawPage].ofs[0];
            PortalRect.y += draw[gameTrackerX.drawPage].ofs[1];

            SetDrawArea(PortalClip, &PortalRect);

            // addPrim(curOT[3070], PortalClip);
            *(int *)PortalClip = getaddr(&curOT[3070]) | 0x2000000;
            curOT[3070] = (unsigned long *)((intptr_t)PortalClip & 0xFFFFFF);

            if (!(toStreamUnit->flags & 0x8))
            {
                DRAW_TranslucentQuad(cliprect->x, cliprect->y, cliprect->x + cliprect->w, cliprect->y, cliprect->x, (short)(cliprect->y + cliprect->h), (short)(cliprect->x + cliprect->w), (short)(cliprect->y + cliprect->h), 50, 50, 50, 1, gameTrackerX.primPool, &curOT[1]);
                DRAW_TranslucentQuad(cliprect->x, cliprect->y, cliprect->x + cliprect->w, cliprect->y, cliprect->x, (short)(cliprect->y + cliprect->h), (short)(cliprect->x + cliprect->w), (short)(cliprect->y + cliprect->h), 50, 50, 50, 2, gameTrackerX.primPool, &curOT[1]);
            }

            PortalClip = (DR_AREA *)gameTrackerX.primPool->nextPrim;

            gameTrackerX.primPool->nextPrim = (unsigned long *)(PortalClip + 1);

            PortalRect.x = draw[gameTrackerX.drawPage].ofs[0];
            PortalRect.y = draw[gameTrackerX.drawPage].ofs[1];
            PortalRect.w = 512;
            PortalRect.h = 240;

            SetDrawArea(PortalClip, &PortalRect);

            // addPrim(curOT[1], PortalClip);
            *(int *)PortalClip = getaddr(&curOT[1]) | 0x2000000;
            curOT[1] = (unsigned long *)((intptr_t)PortalClip & 0xFFFFFF);

            hld = mainOT[s_zval];

            mainOT[s_zval] = curOT[3071];

            curOT[0] = hld;
        }
    }
}

void STREAM_DumpNonResidentObjects()
{
    Instance *instance;
    Instance *next;
    Object *object;

    for (instance = gameTrackerX.instanceList->first; instance != NULL; instance = next)
    {
        next = instance->next;

        object = instance->object;

        if ((!(object->oflags & 0x2000000)) && (STREAM_GetStreamUnitWithID(instance->birthStreamUnitID) == 0) && (STREAM_IsObjectInAnyUnit(FindObjectInTracker(object)) == 0))
        {
            INSTANCE_ReallyRemoveInstance(gameTrackerX.instanceList, instance, 0);
        }
    }

    STREAM_RemoveAllObjectsNotInUse();
}

int STREAM_TryAndDumpNonResident(ObjectTracker *otr)
{
    Instance *instance;
    Instance *next;

    if ((otr->objectStatus != 2) || ((otr->object->oflags & 0x2000000)))
    {
        return 0;
    }

    if (STREAM_IsObjectInAnyUnit(otr) != 0)
    {
        return 0;
    }

    for (instance = gameTrackerX.instanceList->first; instance != NULL; instance = next)
    {
        next = instance->next;

        if ((instance->object == otr->object) && (instance->LinkParent != NULL) && (!(instance->object->oflags2 & 0x80000)))
        {
            return 0;
        }
    }

    for (instance = gameTrackerX.instanceList->first; instance != NULL; instance = next)
    {
        next = instance->next;

        if (instance->object == otr->object)
        {
            SAVE_DeleteInstance(instance);

            if (instance->LinkChild != NULL)
            {
                INSTANCE_ReallyRemoveAllChildren(instance);

                next = instance->next;
            }

            INSTANCE_ReallyRemoveInstance(gameTrackerX.instanceList, instance, 0);
        }
    }

    STREAM_RemoveAllObjectsNotInUse();

    return otr->objectStatus == 0;
}
