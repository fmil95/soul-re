#include "Game/BSP.h"
#include "Game/COLLIDE.h"
#include "Game/GAMELOOP.h"
#include "Game/GAMEPAD.h"
#include "Game/EVENT.h"
#include "Game/LOAD3D.h"
#include "Game/STREAM.h"
#include "Game/STRMLOAD.h"
#include "Game/CAMERA.h"
#include "Game/OBTABLE.h"
#include "Game/INSTANCE.h"
#include "Game/FX.h"
#include "Game/DRAW.h"
#include "Game/MATH3D.h"
#include "Game/SIGNAL.h"
#include "Game/VOICEXA.h"
#include "Game/PSX/MAIN.h"
#include "Game/PSX/SUPPORT.h"
#include "Game/G2/ANIMG2.h"
#include "Game/PLAN/PLANAPI.h"
#include "Game/PLAN/ENMYPLAN.h"
#include "Game/MEMPACK.h"
#include "Game/TIMER.h"
#include "Game/HASM.h"
#include "Game/DEBUG.h"
#include "Game/SOUND.h"
#include "Game/MENU/MENU.h"
#include "Game/MENU/MENUDEFS.h"
#include "Game/LIGHT3D.h"
#include "Game/PIPE3D.h"
#include "Game/SAVEINFO.h"
#include "Game/FONT.h"
#include "Game/VM.h"
#include "Game/VRAM.h"
#include "Game/UNDRWRLD.h"
#include "Game/PLAN/PLANAPI.h"
#include "Game/MONSTER/MONAPI.h"
#include "Game/G2/INSTNCG2.h"
#include "Game/GLYPH.h"

long cameraMode = 0xD;

long playerCameraMode = 0xD;

Object *fontsObject = NULL;

STATIC short pause_redraw_flag;

STATIC PrimPool *primPool[2];

STATIC InstanceList *instanceList;

STATIC InstancePool *instancePool;

STATIC void *planningPool;

STATIC void *enemyPlanPool;

FXTracker *fxTracker;

STATIC void *pause_redraw_prim;

STATIC VertexPool *gVertexPool;

STATIC ObjectTracker *GlobalObjects;

STATIC LightInfo *gLightInfo;

STATIC unsigned long **gOt[2];

char *primBase;

STATIC PolytopeList *gPolytopeList;

FXTracker *gFXT;

int InStreamUnit;

unsigned long **hackOT;

unsigned long StackSave;

#define STACK_SET(x)                       \
    {                                      \
        asm(                               \
            "sw      $sp, 0(%0);"          \
            "li      $t4, 0x1F8003F0;"     \
            "move    $sp, $t4;"            \
            :                              \
            : "r"(&StackSave) : "memory"); \
    }

#define STACK_SAVE() STACK_SET(0x1F8003F0)

#define STACK_RESTORE(x)                          \
    {                                             \
        asm(                                      \
            "lw      $sp, 0(%0);"                 \
            :                                     \
            : "r"(&StackSave) : "$sp", "memory"); \
    }

void GAMELOOP_AllocStaticMemory()
{
    instanceList = (InstanceList *)MEMPACK_Malloc(sizeof(InstanceList), 6);
    instancePool = (InstancePool *)MEMPACK_Malloc(sizeof(InstancePool), 6);

    primBase = MEMPACK_Malloc(216600, 6);

    gOt[0] = (unsigned long **)primBase;
    gOt[1] = (unsigned long **)(primBase + (3072 * 4));

    primPool[0] = (PrimPool *)(primBase + ((3072 * 4) + (3072 * 4)));
    primPool[1] = (PrimPool *)(primBase + ((3072 * 4) + (3072 * 4) + 96012));

    gLightInfo = (LightInfo *)MEMPACK_Malloc(sizeof(LightInfo), 6);

    memset(gLightInfo, 0, sizeof(LightInfo));

    gVertexPool = (VertexPool *)MEMPACK_Malloc(sizeof(VertexPool), 6);

    gPolytopeList = (PolytopeList *)gVertexPool;

    gFXT = fxTracker = (FXTracker *)MEMPACK_Malloc(sizeof(FXTracker), 6);

    planningPool = MEMPACK_Malloc(3000, 6);
    enemyPlanPool = MEMPACK_Malloc(1000, 6);

    GlobalObjects = (ObjectTracker *)MEMPACK_Malloc(1728, 6);

    G2Anim_Install();
}

void GAMELOOP_InitGameTracker()
{
    int i;

    primPool[0]->lastPrim = &primPool[0]->prim[23988];
    primPool[1]->lastPrim = &primPool[1]->prim[23988];

    primPool[0]->nextPrim = &primPool[0]->prim[0];
    primPool[1]->nextPrim = &primPool[1]->prim[0];

    gameTrackerX.instancePool = instancePool;
    gameTrackerX.vertexPool = gVertexPool;
    gameTrackerX.planningPool = planningPool;
    gameTrackerX.enemyPlanPool = enemyPlanPool;

    gameTrackerX.GlobalObjects = GlobalObjects;

    gameTrackerX.instanceList = instanceList;

    gameTrackerX.lightInfo = gLightInfo;

    gameTrackerX.multGameTime = 10;

    gameTrackerX.material_fadeValue = 4096;
    gameTrackerX.spectral_fadeValue = 0;

    gameTrackerX.decoupleGame = 1;

    gameTrackerX.frameRateLock = 1;

    gameTrackerX.primPool = primPool[0];

    gameTrackerX.drawOT = gOt[0];
    gameTrackerX.dispOT = gOt[1];

    gameTrackerX.frameRate24fps = 1;

    for (i = 0; i < 48; i++)
    {
        gameTrackerX.GlobalObjects[i].objectStatus = 0;
    }

    gameTrackerX.gameData.asmData.MorphTime = 1000;

    OBTABLE_ClearObjectReferences();

    EVENT_Init();
}

void GAMELOOP_SystemInit(GameTracker *gameTracker)
{
    (void)gameTracker;

    GAMELOOP_AllocStaticMemory();

    INSTANCE_InitInstanceList(instanceList, instancePool);

    GAMELOOP_InitGameTracker();
}

void GAMELOOP_ResetGameStates()
{
    EVENT_Init();
}

void GAMELOOP_ClearGameTracker()
{
    gameTrackerX.gameData.asmData.MorphTime = 1000;

    gameTrackerX.currentTime = 0;
    gameTrackerX.currentTicks = 0;

    gameTrackerX.gameFlags = 0;

    gameTrackerX.frameCount = 0;
    gameTrackerX.fps30Count = 0;

    gameTrackerX.currentHotSpot = 0;

    gameTrackerX.SwitchToNewStreamUnit = 0;

    gameTrackerX.gameData.asmData.drawBackFaces = 0;

    pause_redraw_flag = 0;
    pause_redraw_prim = NULL;

    gameTrackerX.debugFlags |= 0x40000;
}

void GAMELOOP_CalcGameTime()
{
    long time;

    time = (gameTrackerX.currentTimeOfDayTime * gameTrackerX.multGameTime) / 60000;

    gameTrackerX.timeOfDay = ((((time + 720) / 60) % 24) * 100) + ((time + 720) % 60);
}

void GAMELOOP_SetGameTime(long timeOfDay)
{
    long tim;

    tim = ((timeOfDay / 100) * 60) + (timeOfDay % 100);

    tim -= 720;

    if (tim < 0)
    {
        tim += 1440;
    }

    tim = (tim * 60000) / gameTrackerX.multGameTime;

    gameTrackerX.timeOfDay = timeOfDay;

    gameTrackerX.currentMaterialTime = gameTrackerX.currentTimeOfDayTime = tim;
}

int GAMELOOP_GetTimeOfDay()
{
    int timeOfDay;

    timeOfDay = gameTrackerX.timeOfDay;

    if (((timeOfDay - 601) >= 0) && ((timeOfDay - 601) < 99))
    {
        return 600;
    }
    else if (((timeOfDay - 700) >= 0) && ((timeOfDay - 700) < 1100))
    {
        return 700;
    }
    else if (((timeOfDay - 1800) >= 0) && ((timeOfDay - 1800) < 100))
    {
        return 1800;
    }

    return 1900;
}

int GAMELOOP_GetTimeOfDayIdx(int timeOfDay)
{
    if (((timeOfDay - 601) >= 0) && ((timeOfDay - 601) < 99))
    {
        return 1;
    }

    if (((timeOfDay - 700) >= 0) && ((timeOfDay - 700) < 1100))
    {
        return 2;
    }

    if (((timeOfDay - 1800) >= 0) && ((timeOfDay - 1800) < 100))
    {
        return 3;
    }

    return 0;
}

int GAMELOOP_WaitForLoad()
{
    if ((gameTrackerX.debugFlags & 0x80000))
    {
        VOICEXA_Tick();
    }

    return STREAM_PollLoadQueue();
}

StreamUnit *LoadLevels(char *baseAreaName, GameTracker *gameTracker)
{
    SVector offset;
    StreamUnit *streamUnit;
    static char oldArea[16] = {0};

    if (strlen(oldArea) != 0)
    {
        STREAM_AbortAreaLoad(oldArea);
    }

    strcpy(oldArea, baseAreaName);

    LOAD_ChangeDirectory(baseAreaName);

    streamUnit = STREAM_LoadLevel(baseAreaName, NULL, 0);

    if (streamUnit->used == 1)
    {
        int num;
        int waitFor;

        DRAW_LoadingMessage();

        while (streamUnit->used == 1)
        {
            GAMELOOP_WaitForLoad();
        }

        STREAM_NextLoadFromHead();

        STREAM_LoadMainVram(gameTracker, baseAreaName, streamUnit);

        STREAM_NextLoadAsNormal();

        waitFor = GAMELOOP_WaitForLoad() - 1;

        do
        {
            num = GAMELOOP_WaitForLoad();

            if (num == 0)
            {
                break;
            }
        } while (num >= waitFor);
    }

    else
    {
        STREAM_DumpLoadingObjects();

        STREAM_LoadMainVram(gameTracker, baseAreaName, streamUnit);
    }

    if ((streamUnit->level->startUnitMainSignal != NULL) && (gameTracker->playerInstance != NULL))
    {
        streamUnit->level->startUnitMainSignal->flags |= 0x1;

        SIGNAL_HandleSignal(gameTracker->playerInstance, streamUnit->level->startUnitMainSignal->signalList, 0);

        EVENT_AddSignalToReset(streamUnit->level->startUnitMainSignal);
    }

    ADD_SVEC(SVector, &offset, Position, &streamUnit->level->terrain->BSPTreeArray->bspRoot->sphere.position, Position, &streamUnit->level->terrain->BSPTreeArray->globalOffset);

    offset.x = -offset.x;
    offset.y = -offset.y;
    offset.z = -offset.z;

    PreloadAllConnectedUnits(streamUnit, &offset);

    return streamUnit;
}

void GAMELOOP_InitStandardObjects()
{
    static char *sobjects[] = {
        "raziel",
        "paths",
        "glphicon",
        "sreavr",
        "soul",
        "force",
        "particle",
        "healths",
        "eaggot",
        "eaggots"};

    int i;

    LOAD_DumpCurrentDir();

    for (i = 0; (unsigned int)i < ARRAY_COUNT(sobjects); i++)
    {
        InsertGlobalObject(sobjects[i], &gameTrackerX);
    }
}

void GAMELOOP_LevelLoadAndInit(char *baseAreaName, GameTracker *gameTracker)
{
    long i;
    StreamUnit *streamUnit;
    BLK_FILL *temp; // not from decls.h

    G2Anim_ResetInternalState();
    gameTracker->playerInstance = NULL;

    INSTANCE_InitInstanceList(instanceList, instancePool);
    GAMELOOP_ClearGameTracker();
    CAMERA_Initialize(&theCamera);
    PLANAPI_InitPlanning(planningPool);
    ENMYPLAN_InitEnemyPlanPool(enemyPlanPool);
    FX_Init(fxTracker);
    WARPGATE_Init();
    DRAW_InitShadow();
    GAMELOOP_InitStandardObjects();

    streamUnit = LoadLevels(baseAreaName, gameTracker);

    while (STREAM_PollLoadQueue() != 0)
    {
    }

    fontsObject = (Object *)objectAccess[2].object;

    gameTracker->introFX = (Object *)objectAccess[6].object;

    RENDER_currentStreamUnitID = (unsigned short)gameTracker->StreamUnitID;

    for (i = 0; i < streamUnit->level->numIntros; i++)
    {
        if (strcmpi(streamUnit->level->introList[i].name, "raziel") == 0)
        {
            INSTANCE_IntroduceInstance(&streamUnit->level->introList[i], (short)streamUnit->StreamUnitID);
            break;
        }
    }

    gameTracker->playerInstance->data = gameTracker->playerInstance->object->data;

    CAMERA_SetInstanceFocus(&theCamera, gameTracker->playerInstance);

    COPY_SVEC(Position, &theCamera.core.position, Position, &gameTracker->playerInstance->position);

    SetFogNearFar(streamUnit->level->fogNear, streamUnit->level->fogFar, 320);
    SetFarColor(0, 0, 0);

    temp = clearRect;

    temp->r0 = streamUnit->level->backColorR;
    temp->g0 = streamUnit->level->backColorG;
    temp->b0 = streamUnit->level->backColorB;

    temp++;

    temp->r0 = streamUnit->level->backColorR;
    temp->g0 = streamUnit->level->backColorG;
    temp->b0 = streamUnit->level->backColorB;

    gameTracker->wipeType = 10;
    gameTracker->hideBG = 0;
    gameTracker->wipeTime = 30;
    gameTracker->maxWipeTime = 30;

    if (streamUnit->level->startSignal != NULL)
    {
        streamUnit->level->startSignal->flags |= 0x1;
        SIGNAL_HandleSignal(gameTracker->playerInstance, streamUnit->level->startSignal->signalList, 0);
        EVENT_AddSignalToReset(streamUnit->level->startSignal);
    }

    gameTracker->vblFrames = 0;

    if (streamUnit->level->startUnitMainSignal != NULL && gameTracker->playerInstance != NULL)
    {
        streamUnit->level->startUnitMainSignal->flags |= 0x1;
        SIGNAL_HandleSignal(gameTracker->playerInstance, streamUnit->level->startUnitMainSignal->signalList, 0);
        EVENT_AddSignalToReset(streamUnit->level->startUnitMainSignal);
    }
}

void GAMELOOP_StreamLevelLoadAndInit(char *baseAreaName, GameTracker *gameTracker, int toSignalNum, int fromSignalNum)
{
    (void)toSignalNum;
    (void)fromSignalNum;

    LoadLevels(baseAreaName, gameTracker);
}

void GAMELOOP_SetScreenWipe(int time, int maxTime, int type)
{
    gameTrackerX.maxWipeTime = maxTime;
    gameTrackerX.wipeTime = time;

    gameTrackerX.wipeType = type;
}

void GAMELOOP_HandleScreenWipes(unsigned long **drawot)
{
    long temp;
    PrimPool *primPool;

    primPool = gameTrackerX.primPool;

    if ((GlobalSave->flags & 0x1))
    {
        DRAW_FlatQuad(&gameTrackerX.wipeColor, 0, 0, 512, 0, 0, 30, 512, 30, primPool, drawot);
        DRAW_FlatQuad(&gameTrackerX.wipeColor, 0, 210, 512, 210, 0, 240, 512, 240, primPool, drawot);
    }

    if (gameTrackerX.wipeTime > 0)
    {
        switch (gameTrackerX.wipeType)
        {
        case 10:
            temp = (gameTrackerX.wipeTime * 255) / gameTrackerX.maxWipeTime;

            DRAW_TranslucentQuad(0, 0, 512, 0, 0, 240, 512, 240, (short)temp, (short)temp, (short)temp, 2, primPool, drawot);
            break;
        case 11:
            temp = (gameTrackerX.wipeTime * 255) / gameTrackerX.maxWipeTime;

            DRAW_TranslucentQuad(0, 0, 512, 0, 0, 30, 512, 30, (short)temp, (short)temp, (short)temp, 2, primPool, drawot);
            DRAW_TranslucentQuad(0, 210, 512, 210, 0, 240, 512, 240, (short)temp, (short)temp, (short)temp, 2, primPool, drawot);

            GlobalSave->flags &= ~0x1;
            break;
        }

        if (gameTrackerX.gameFramePassed != 0)
        {
            gameTrackerX.wipeTime--;
        }
    }
    else if (gameTrackerX.wipeTime < -1)
    {
        switch (gameTrackerX.wipeType)
        {
        case 10:
            temp = ((gameTrackerX.maxWipeTime + gameTrackerX.wipeTime + 2) * 255) / gameTrackerX.maxWipeTime;

            DRAW_TranslucentQuad(0, 0, 512, 0, 0, 240, 512, 240, (short)temp, (short)temp, (short)temp, 2, primPool, drawot);
            break;
        case 11:
            temp = ((gameTrackerX.maxWipeTime + gameTrackerX.wipeTime) * 255) / gameTrackerX.maxWipeTime;

            DRAW_TranslucentQuad(0, 0, 512, 0, 0, 30, 512, 30, (short)temp, (short)temp, (short)temp, 2, primPool, drawot);
            DRAW_TranslucentQuad(0, 210, 512, 210, 0, 240, 512, 240, (short)temp, (short)temp, (short)temp, 2, primPool, drawot);

            if (gameTrackerX.wipeTime == -2)
            {
                GlobalSave->flags |= 0x1;
            }

            break;
        }

        if (gameTrackerX.gameFramePassed != 0)
        {
            gameTrackerX.wipeTime++;
        }
    }
    else if (gameTrackerX.wipeTime == -1)
    {
        if (gameTrackerX.wipeType == 11)
        {
            GlobalSave->flags |= 0x1;
        }
        else
        {
            DRAW_FlatQuad(&gameTrackerX.wipeColor, 0, 0, 512, 0, 0, 240, 512, 240, primPool, drawot);
        }
    }
    else
    {
        theCamera.core.screenScale.x = theCamera.core.screenScale.y = theCamera.core.screenScale.z = 4096;
    }
}

void UpdateFogSettings(StreamUnit *currentUnit, Level *level)
{
    int changed;
    int FogFar;
    int FogNear;
    int setflag;

    changed = 0;

    FogNear = currentUnit->TargetFogNear;
    FogFar = currentUnit->TargetFogFar;

    setflag = 0;

    if (level->fogNear > FogNear)
    {
        level->fogNear -= 500;

        changed = 1;

        if (level->fogNear <= FogNear)
        {
            setflag = 1;

            level->fogNear = FogNear;
        }
    }
    else if (level->fogNear < FogNear)
    {
        level->fogNear += 500;

        changed = 1;

        if (level->fogNear >= FogNear)
        {
            setflag = 1;

            level->fogNear = FogNear;
        }
    }
    else
    {
        setflag = 1;
    }

    if (level->fogFar > FogFar)
    {
        level->fogFar -= 500;

        changed = 1;

        if (level->fogFar <= FogFar)
        {
            level->fogFar = FogFar;
        }
        else
        {
            setflag = 0;
        }
    }
    else if (level->fogFar < FogFar)
    {
        level->fogFar += 500;

        changed = 1;

        if (level->fogFar >= FogFar)
        {
            level->fogFar = FogFar;
        }
        else
        {
            setflag = 0;
        }
    }

    if (changed != 0)
    {
        LIGHT_CalcDQPTable(level);

        if (setflag != 0)
        {
            currentUnit->TargetFogNear = level->fogNear;
            currentUnit->TargetFogFar = level->fogFar;
        }
    }
}

int CheckForNoBlend(ColorType *Color)
{
    if ((Color->r < 5) && (Color->g < 5) && (Color->b < 5))
    {
        return 1;
    }

    return 0;
}

void BlendToColor(ColorType *target, ColorType *current, ColorType *dest)
{
    LoadAverageCol((unsigned char *)target, (unsigned char *)current, 512, 3584, (unsigned char *)dest);

    if ((target->r - dest->r) >= 0)
    {
        if ((target->r - dest->r) >= 5)
        {
            dest->code = 0;

            return;
        }
    }
    else if ((dest->r - target->r) >= 5)
    {
        dest->code = 0;

        return;
    }

    if ((target->g - dest->g) >= 0)
    {
        if ((target->g - dest->g) >= 5)
        {
            dest->code = 0;

            return;
        }
    }
    else if ((dest->g - target->g) >= 5)
    {
        dest->code = 0;

        return;
    }

    if ((target->b - dest->b) >= 0)
    {
        if ((target->b - dest->b) >= 5)
        {
            dest->code = 0;

            return;
        }
    }
    else if ((dest->b - target->b) >= 5)
    {
        dest->code = 0;

        return;
    }

    *(int *)dest = *(int *)target;

    dest->code = 0;
}

void MainRenderLevel(StreamUnit *currentUnit, unsigned long **drawot)
{
    Level *level;
    GameTracker *gameTracker;
    Terrain *terrain;
    int curTree;
    long BackColor;
    Position cam_pos_save;
    MATRIX cam_mat_save;
    Instance *saveLightInstance;
    BLK_FILL *temp, *temp2, *temp3; // not from decls.h

    level = currentUnit->level;

    terrain = level->terrain;

    UpdateFogSettings(currentUnit, level);

    gameTracker = &gameTrackerX;

    currentUnit->FrameCount = gameTracker->displayFrameCount;

    SetFogNearFar(level->fogNear, level->fogFar, 320);
    SetFarColor(0, 0, 0);

    temp = clearRect;

    temp->r0 = level->backColorR;
    temp->g0 = level->backColorG;
    temp->b0 = level->backColorB;

    temp++;

    temp->r0 = level->backColorR;
    temp->g0 = level->backColorG;
    temp->b0 = level->backColorB;

    if (gameTrackerX.gameData.asmData.MorphTime != 1000)
    {
        int time;

        time = (gameTrackerX.gameData.asmData.MorphTime << 12) / 1000;

        if (gameTrackerX.gameData.asmData.MorphType == 1)
        {
            time = 4096 - time;
        }

        LoadAverageCol(&level->specturalColorR, &level->backColorR, time, 4096 - time, (unsigned char *)&BackColor);
    }
    else if (gameTrackerX.gameData.asmData.MorphType == 1)
    {
        BackColor = ((long *)&level->specturalColorR)[0];
    }
    else
    {
        BackColor = ((long *)&level->backColorR)[0];
    }

    BlendToColor((ColorType *)&BackColor, (ColorType *)&currentUnit->FogColor, (ColorType *)&currentUnit->FogColor);

    depthQBackColor = currentUnit->FogColor;

    {
        int tod;

        tod = GAMELOOP_GetTimeOfDay();

        if ((((tod != 600) && (tod != 1800)) || (!(level->unitFlags & 0x2))) && (gameTrackerX.gameData.asmData.MorphTime == 1000))
        {
            depthQBackColor = (depthQBackColor & 0xFFF8F8F8) | 0x40404;
        }
    }

    depthQFogStart = level->fogNear;
    depthQFogFar = level->fogFar;

    if (CheckForNoBlend((ColorType *)&depthQBackColor) == 0)
    {
        depthQBlendStart = depthQFogStart;
    }
    else
    {
        depthQBlendStart = 65535;
    }

    temp2 = &clearRect[0];

    temp2->r0 = ((ColorType *)&depthQBackColor)->r;
    temp2->g0 = ((ColorType *)&depthQBackColor)->g;
    temp2->b0 = ((ColorType *)&depthQBackColor)->b;

    temp3 = &clearRect[1];

    temp3->r0 = ((ColorType *)&depthQBackColor)->r;
    temp3->g0 = ((ColorType *)&depthQBackColor)->g;
    temp3->b0 = ((ColorType *)&depthQBackColor)->b;

    PIPE3D_AnimateTerrainTextures(terrain->aniList, gameTracker->frameCount, gameTracker->primPool, drawot);
    PIPE3D_AnimateTerrainTextures(level->bgAniList, gameTracker->frameCount, gameTracker->primPool, drawot);

    gLightInfo->numSavedColors = 0;

    PIPE3D_InstanceListTransformAndDraw(currentUnit, gameTracker, drawot, &theCamera.core);

    cam_pos_save = theCamera.core.position;
    cam_mat_save = *theCamera.core.wcTransform;

    for (curTree = 0; curTree < terrain->numBSPTrees; curTree++)
    {
        SVector tmp;
        BSPTree *bsp;

        bsp = &terrain->BSPTreeArray[curTree];

        if ((bsp->ID >= 0) && (!(bsp->flags & 0x1)))
        {
            saveLightInstance = NULL;

            if ((bsp->flags & 0x40))
            {
                saveLightInstance = gameTrackerX.gameData.asmData.lightInstances[0].lightInstance;

                gameTrackerX.gameData.asmData.lightInstances[0].lightInstance = NULL;
            }

            theCamera.core.position.x = cam_pos_save.x - bsp->globalOffset.x;
            theCamera.core.position.y = cam_pos_save.y - bsp->globalOffset.y;
            theCamera.core.position.z = cam_pos_save.z - bsp->globalOffset.z;

            tmp.x = -theCamera.core.position.x;
            tmp.y = -theCamera.core.position.y;
            tmp.z = -theCamera.core.position.z;

            ApplyMatrix(&cam_mat_save, (SVECTOR *)&tmp, (VECTOR *)theCamera.core.wcTransform->t);

            BSP_MarkVisibleLeaves_S(bsp, &theCamera, gPolytopeList);

            gameTracker->primPool->nextPrim = ((DrawDisplayPolytopeListFunc *)gameTracker->drawDisplayPolytopeListFunc)(gPolytopeList, terrain, &theCamera, gameTracker->primPool, drawot, &bsp->globalOffset);

            if ((bsp->flags & 0x40))
            {
                gameTrackerX.gameData.asmData.lightInstances[0].lightInstance = saveLightInstance;
            }
        }
    }

    theCamera.core.position = cam_pos_save;
    *theCamera.core.wcTransform = cam_mat_save;

    SBSP_IntroduceInstancesAndLights(terrain, &theCamera.core, gLightInfo, RENDER_currentStreamUnitID);

    STACK_SAVE();

    FX_DrawList(fxTracker, &gameTrackerX, gameTrackerX.drawOT, theCamera.core.wcTransform);

    if (gameTrackerX.playerInstance->currentStreamUnitID == currentUnit->StreamUnitID)
    {
        FX_DrawReaver(gameTrackerX.primPool, gameTrackerX.drawOT, theCamera.core.wcTransform);
    }

    STACK_RESTORE();
}

void StreamIntroInstancesForUnit(StreamUnit *currentUnit)
{
    if (currentUnit->used == 2)
    {
        SBSP_IntroduceInstances(currentUnit->level->terrain, currentUnit->StreamUnitID);
    }
}

long StreamRenderLevel(StreamUnit *currentUnit, Level *mainLevel, unsigned long **drawot, long portalFogColor)
{
    GameTracker *gameTracker;
    Level *level;
    Terrain *terrain;
    int curTree;
    int farplanesave;
    Position cam_pos_save;
    MATRIX cam_mat_save;
    SVector tmp;
    BSPTree *bsp;

    (void)mainLevel;

    level = currentUnit->level;

    farplanesave = theCamera.core.farPlane;

    terrain = level->terrain;

    SetFarColor(0, 0, 0);

    UpdateFogSettings(currentUnit, level);

    gameTracker = &gameTrackerX;

    depthQBackColor = portalFogColor;

    depthQFogFar = level->fogFar;
    depthQFogStart = level->fogNear;

    currentUnit->FogColor = portalFogColor;

    theCamera.core.farPlane = depthQFogFar;

    if (CheckForNoBlend((ColorType *)&depthQBackColor) != 0)
    {
        depthQBlendStart = 65535;
    }
    else
    {
        depthQBlendStart = depthQFogStart;
    }

    SetFogNearFar(depthQFogStart, depthQFogFar, 320);

    PIPE3D_AnimateTerrainTextures(terrain->aniList, gameTracker->frameCount, gameTracker->primPool, drawot);
    PIPE3D_AnimateTerrainTextures(level->bgAniList, gameTracker->frameCount, gameTracker->primPool, drawot);

    PIPE3D_InstanceListTransformAndDraw(currentUnit, gameTracker, drawot, &theCamera.core);

    cam_pos_save = theCamera.core.position;
    cam_mat_save = *theCamera.core.wcTransform;

    for (curTree = 0; curTree < terrain->numBSPTrees; curTree++)
    {
        bsp = &terrain->BSPTreeArray[curTree];

        if ((bsp->ID >= 0) && (!(bsp->flags & 0x1)))
        {
            theCamera.core.position.x = cam_pos_save.x - bsp->globalOffset.x;
            theCamera.core.position.y = cam_pos_save.y - bsp->globalOffset.y;
            theCamera.core.position.z = cam_pos_save.z - bsp->globalOffset.z;

            tmp.x = -theCamera.core.position.x;
            tmp.y = -theCamera.core.position.y;
            tmp.z = -theCamera.core.position.z;

            ApplyMatrix(&cam_mat_save, (SVECTOR *)&tmp, (VECTOR *)&theCamera.core.wcTransform->t[0]);

            BSP_MarkVisibleLeaves_S(bsp, &theCamera, gPolytopeList);

            gameTracker->primPool->nextPrim = ((DrawDisplayPolytopeListFunc *)gameTracker->drawDisplayPolytopeListFunc)(gPolytopeList, terrain, &theCamera, gameTracker->primPool, drawot, &bsp->globalOffset);
        }
    }

    theCamera.core.position = cam_pos_save;
    *theCamera.core.wcTransform = cam_mat_save;

    InStreamUnit = 1;

    SBSP_IntroduceInstancesAndLights(terrain, &theCamera.core, gLightInfo, RENDER_currentStreamUnitID);

    theCamera.core.farPlane = farplanesave;

    InStreamUnit = 0;

    if (gameTrackerX.playerInstance->currentStreamUnitID == currentUnit->StreamUnitID)
    {
        hackOT = drawot;

        STACK_SET();

        FX_DrawReaver(gameTrackerX.primPool, hackOT, theCamera.core.wcTransform);

        STACK_RESTORE();
    }

    return 0;
}

void GAMELOOP_FlipScreenAndDraw(GameTracker *gameTracker, unsigned long **drawot)
{
    DrawOTag((unsigned long *)(drawot + 3071));

    while (CheckVolatile(gameTracker->drawTimerReturn) != 0)
        ;

    ResetPrimPool();

    PutDrawEnv(&draw[gameTracker->drawPage]);

    while (CheckVolatile(gameTracker->reqDisp) != 0)
        ;

    gameTracker->usecsStartDraw = (GetRCnt(0xF2000000) & 0xFFFF) | (gameTimer << 16);

    gameTracker->drawTimerReturn = (long *)&gameTracker->drawTime;

    gameTracker->gameData.asmData.dispPage = 1 - gameTracker->gameData.asmData.dispPage;
}

void GAMELOOP_AddClearPrim(unsigned long **drawot, int override)
{
    if ((!(gameTrackerX.gameFlags & 0x8000000)) || (override != 0))
    {
        BLK_FILL *blkfill;
        int *temp; // not from decls.h

        blkfill = (BLK_FILL *)gameTrackerX.primPool->nextPrim;

        *blkfill = clearRect[gameTrackerX.drawPage];

        gameTrackerX.primPool->nextPrim = (unsigned long *)(blkfill + 1);

        // addPrim(drawot[3071], blkfill);

        temp = (int *)&drawot[3071];

        *(int *)blkfill = getaddr(temp) | 0x3000000;
        *(int *)temp = getaddr(&blkfill);
    }
    else
    {
        BLK_FILL *blkfill;

        blkfill = (BLK_FILL *)gameTrackerX.savedOTStart;

        blkfill->y0 = clearRect[gameTrackerX.drawPage].y0;
    }
}

void GAMELOOP_SwitchTheDrawBuffer(unsigned long **drawot)
{
    GAMELOOP_AddClearPrim(drawot, 0);

    DrawSync(0);

    if (gameTrackerX.drawTimerReturn != NULL)
    {
        gameTrackerX.drawTimerReturn = NULL;

        gameTrackerX.reqDisp = (DISPENV *)gameTrackerX.disp + gameTrackerX.gameData.asmData.dispPage;
    }

    PutDrawEnv(&draw[gameTrackerX.drawPage]);
}

void GAMELOOP_SetupRenderFunction(GameTracker *gameTracker)
{
    gameTracker->drawAnimatedModelFunc = &DRAW_AnimatedModel_S;
    gameTracker->drawDisplayPolytopeListFunc = &DRAW_DisplayPolytopeList_S;
}

StreamUnit *GAMELOOP_GetMainRenderUnit()
{
    StreamUnit *streamUnit;
    Instance *focusInstance;
    StreamUnit *cameraUnit;

    if (theCamera.mode == 5)
    {
        streamUnit = STREAM_WhichUnitPointerIsIn(theCamera.data.Cinematic.posSpline);
    }
    else
    {
        focusInstance = theCamera.focusInstance;

        if ((focusInstance == gameTrackerX.playerInstance) && (gameTrackerX.SwitchToNewStreamUnit != 0))
        {
            streamUnit = STREAM_GetStreamUnitWithID(gameTrackerX.moveRazielToStreamID);

            if (streamUnit == NULL)
            {
                return STREAM_GetStreamUnitWithID(focusInstance->currentStreamUnitID);
            }
        }
        else
        {
            streamUnit = STREAM_GetStreamUnitWithID(focusInstance->currentStreamUnitID);
        }

        cameraUnit = COLLIDE_CameraWithStreamSignals(&theCamera);

        if (cameraUnit != NULL)
        {
            streamUnit = cameraUnit;
        }
    }

    return streamUnit;
}

void GAMELOOP_DisplayFrame(GameTracker *gameTracker)
{
    unsigned long **drawot;
    Level *mainLevel;
    StreamUnitPortal *streamPortal;
    int numportals;
    int d;
    StreamUnit *mainStreamUnit;
    void *savedNextPrim;

    drawot = gameTracker->drawOT;

    if ((!(gameTrackerX.gameFlags & 0x8000000)) || (pause_redraw_flag != 0))
    {
        if (pause_redraw_flag != 0)
        {
            savedNextPrim = gameTrackerX.primPool->nextPrim;

            DrawSync(0);

            Switch_For_Redraw();

            drawot = gameTracker->drawOT;

            ClearOTagR((unsigned long *)gameTrackerX.drawOT, 3072);

            if (pause_redraw_prim != NULL)
            {
                gameTrackerX.primPool->nextPrim = pause_redraw_prim;
            }
            else
            {
                gameTrackerX.primPool->nextPrim = gameTrackerX.primPool->prim;
            }
        }
        else
        {
            pause_redraw_prim = gameTrackerX.primPool->nextPrim;
        }

        gameTrackerX.visibleInstances = 0;

        gameTrackerX.displayFrameCount++;

        GAMELOOP_SetupRenderFunction(&gameTrackerX);

        if ((!(GlobalSave->flags & 0x1)) && ((gameTracker->wipeType != 11) || (gameTracker->wipeTime == 0)) && ((gameTracker->debugFlags2 & 0x800)))
        {
            FX_Spiral(gameTrackerX.primPool, drawot);
        }

        if (pause_redraw_flag == 0)
        {
            HUD_Draw();
        }

        mainStreamUnit = GAMELOOP_GetMainRenderUnit();

        mainLevel = mainStreamUnit->level;

        if ((gameTracker->debugFlags & 0x4))
        {
            FONT_Print("Cameraunit: %s\n", mainStreamUnit->level->worldName);
        }

        RENDER_currentStreamUnitID = mainStreamUnit->StreamUnitID;

        theCamera.core.leftX = 0;
        theCamera.core.rightX = 320;

        theCamera.core.topY = 0;
        theCamera.core.bottomY = 240;

        CAMERA_SetViewVolume(&theCamera);

        if (MEMPACK_MemoryValidFunc((char *)mainLevel) != 0)
        {
            if (mainLevel->fogFar != theCamera.core.farPlane)
            {
                theCamera.core.farPlane = mainLevel->fogFar;
            }

            if (!(gameTracker->debugFlags & 0x8000))
            {
                MainRenderLevel(mainStreamUnit, drawot);
            }
        }

        numportals = *(long *)mainLevel->terrain->StreamUnits;

        streamPortal = (StreamUnitPortal *)((long *)mainLevel->terrain->StreamUnits + 1);

        for (d = 0; d < numportals; d++, streamPortal++)
        {
            StreamUnit *toStreamUnit;
            long toStreamUnitID;
            StreamUnitPortal *streamPortal2;
            int i;
            int draw;
            RECT cliprect;

            toStreamUnit = streamPortal->toStreamUnit;

            toStreamUnitID = streamPortal->streamID;

            if ((toStreamUnit != NULL) && ((unsigned long)toStreamUnit->FrameCount == gameTrackerX.displayFrameCount))
            {
                continue;
            }

            cliprect.x = 512;
            cliprect.y = 240;
            cliprect.w = -512;
            cliprect.h = -240;

            theCamera.core.leftX = 0;
            theCamera.core.rightX = 320;

            theCamera.core.topY = 0;
            theCamera.core.bottomY = 240;

            CAMERA_SetViewVolume(&theCamera);

            draw = 0;

            streamPortal2 = (StreamUnitPortal *)((long *)mainLevel->terrain->StreamUnits + 1);

            for (i = 0; i < numportals; i++, streamPortal2++)
            {
                if (streamPortal2->streamID != toStreamUnitID)
                {
                    continue;
                }

                if (STREAM_GetClipRect(streamPortal2, &cliprect) != 0)
                {
                    draw = 1;
                }
                else if ((theCamera.instance_mode & 0x2000000))
                {
                    int streamID;
                    Instance *instance;

                    streamID = streamPortal2->toStreamUnit->StreamUnitID;

                    instance = (Instance *)INSTANCE_Query(gameTrackerX.playerInstance, 34);

                    if ((streamID == gameTrackerX.playerInstance->currentStreamUnitID) || ((instance != NULL) && (streamID == instance->currentStreamUnitID)))
                    {
                        draw = 1;

                        cliprect.w = 512;
                        cliprect.x = 0;
                        cliprect.y = 0;
                        cliprect.h = 240;
                    }
                }
            }

            if (draw != 0)
            {
                theCamera.core.leftX = (cliprect.x * 320) / 512;
                theCamera.core.topY = cliprect.y;

                theCamera.core.rightX = ((cliprect.x + cliprect.w) * 320) / 512;
                theCamera.core.bottomY = cliprect.y + cliprect.h;

                CAMERA_SetViewVolume(&theCamera);

                SetRotMatrix(theCamera.core.wcTransform);
                SetTransMatrix(theCamera.core.wcTransform);

                if ((streamPortal->flags & 0x1))
                {
                    if ((mainStreamUnit->flags & 0x8))
                    {
                        if (toStreamUnit != NULL)
                        {
                            if ((unsigned long)toStreamUnit->FrameCount == gameTrackerX.displayFrameCount)
                            {
                                continue;
                            }

                            toStreamUnit->FrameCount = gameTrackerX.displayFrameCount;
                        }

                        STREAM_RenderWarpGate(drawot, streamPortal, mainStreamUnit, &cliprect);
                    }
                    else
                    {
                        WARPGATE_IsItActive(mainStreamUnit);
                    }
                }
                else if ((toStreamUnit != NULL) && ((unsigned long)toStreamUnit->FrameCount != gameTrackerX.displayFrameCount))
                {
                    toStreamUnit->FrameCount = gameTrackerX.displayFrameCount;

                    STREAM_RenderAdjacantUnit(drawot, streamPortal, toStreamUnit, mainStreamUnit, &cliprect);
                }
            }
            else if ((toStreamUnit != NULL) && ((unsigned long)toStreamUnit->FrameCount != gameTrackerX.displayFrameCount))
            {
                toStreamUnit->FrameCount = gameTrackerX.displayFrameCount;

                StreamIntroInstancesForUnit(toStreamUnit);
            }
        }

        for (d = 0; d < 16; d++)
        {
            if ((StreamTracker.StreamList[d].used == 2) && ((unsigned long)StreamTracker.StreamList[d].FrameCount != gameTrackerX.displayFrameCount))
            {
                StreamTracker.StreamList[d].FrameCount = gameTrackerX.displayFrameCount;

                StreamIntroInstancesForUnit(&StreamTracker.StreamList[d]);
            }
        }

        theCamera.core.rightX = 320;
        theCamera.core.leftX = 0;

        theCamera.core.topY = 0;
        theCamera.core.bottomY = 240;

        CAMERA_SetViewVolume(&theCamera);

        if (pause_redraw_flag != 0)
        {
            GAMELOOP_AddClearPrim(drawot, 1);

            SaveOT();

            ClearOTagR((unsigned long *)gameTrackerX.drawOT, 3072);

            Switch_For_Redraw();

            drawot = gameTracker->drawOT;

            pause_redraw_flag = 0;

            gameTrackerX.primPool->nextPrim = savedNextPrim;
        }
    }

    if ((gameTrackerX.gameFlags & 0x8000000))
    {
        HUD_Draw();
    }

    DEBUG_Draw(gameTracker, drawot);

    FONT_Flush();

    GAMELOOP_SwitchTheDrawBuffer(drawot);

    gameTracker->idleTime = (GetRCnt(0xF2000000) & 0xFFFF) | (gameTimer << 16);

    if (gameTracker->vblFrames <= (unsigned long)gameTracker->frameRateLock)
    {
        while (CheckVolatile(gameTracker->reqDisp) != 0)
            ;
    }
    else if (gameTracker->reqDisp != NULL)
    {
        PutDispEnv(gameTracker->reqDisp);

        gameTracker->reqDisp = NULL;

        gameTracker->vblFrames = 0;
    }

    gameTracker->idleTime = TIMER_TimeDiff(gameTracker->idleTime);

    gameTracker->gameData.asmData.dispPage = 1 - gameTracker->gameData.asmData.dispPage;

    DEBUG_DrawShrinkCels(drawot + 3071);

    GAMELOOP_HandleScreenWipes(drawot);

    gameTracker->usecsStartDraw = (GetRCnt(0xF2000000) & 0xFFFF) | (gameTimer << 16);

    gameTracker->drawTimerReturn = (long *)&gameTracker->drawTime;

    if ((gameTrackerX.gameFlags & 0x8000000))
    {
        GAMELOOP_DrawSavedOT(drawot);
    }
    else
    {
        DrawOTag((unsigned long *)drawot + 3071);
    }
}

void GAMELOOP_DrawSavedOT(unsigned long **newOT)
{
    void *tag;
    int y;

    y = draw[gameTrackerX.drawPage].ofs[1];
    for (tag = gameTrackerX.savedOTStart; tag != (P_TAG *)gameTrackerX.savedOTEnd; tag = (P_TAG *)nextPrim(tag))
    {
        int mask = ~0x10;

        if ((getcode(tag) & ~3) == 0x34)
        {
            int tpage;
            tpage = ((POLY_GT3 *)tag)->tpage;

            if ((tpage & 0xF) < 8)
            {
                if (y != 0)
                {
                    ((POLY_GT3 *)tag)->tpage |= 0x10;
                }
                else
                {
                    ((POLY_GT3 *)tag)->tpage &= mask;
                }
            }
        }
        else if ((getcode(tag) & ~3) == 0x24)
        {
            int tpage;
            tpage = ((POLY_FT3 *)tag)->tpage;

            if ((tpage & 0xF) < 8)
            {
                if (y != 0)
                {
                    ((POLY_FT3 *)tag)->tpage |= 0x10;
                }
                else
                {
                    ((POLY_FT3 *)tag)->tpage &= mask;
                }
            }
        }
        else if (getcode(tag) == 0xE3)
        {
            if (y != 0)
            {
                ((DR_TPAGE *)tag)->code[0] |= 0x40000;
                ((DR_TPAGE *)tag)->code[1] |= 0x40000;
            }
            else
            {
                ((DR_TPAGE *)tag)->code[0] &= ~0x40000;
                ((DR_TPAGE *)tag)->code[1] &= ~0x40000;
            }
        }
    }

    tag = gameTrackerX.savedOTEnd;
    setaddr(tag, &newOT[3071]);

    DrawOTag((unsigned long *)gameTrackerX.savedOTStart);
}

void ResetPrimPool()
{
    ResetDrawPage();

    if (!(gameTrackerX.gameFlags & 0x8000000))
    {
        if (gameTrackerX.primPool == primPool[0])
        {
            gameTrackerX.primPool = primPool[1];
        }
        else
        {
            gameTrackerX.primPool = primPool[0];
        }

        gameTrackerX.primPool->nextPrim = gameTrackerX.primPool->prim;

        gameTrackerX.primPool->numPrims = 0;
    }
    else
    {
        if (gameTrackerX.drawPage != 0)
        {
            gameTrackerX.primPool->nextPrim = &gameTrackerX.primPool->prim[16500];
        }
        else
        {
            gameTrackerX.primPool->nextPrim = &gameTrackerX.primPool->prim[9000];
        }

        gameTrackerX.primPool->numPrims = 0;
    }
}

void Switch_For_Redraw()
{
    unsigned long **temp;

    temp = gameTrackerX.drawOT;

    gameTrackerX.drawOT = gameTrackerX.dispOT;
    gameTrackerX.dispOT = temp;

    if (gameTrackerX.drawPage != 0)
    {
        gameTrackerX.drawPage = 0;

        gameTrackerX.gameData.asmData.dispPage = 1;
    }
    else
    {
        gameTrackerX.drawPage = 1;

        gameTrackerX.gameData.asmData.dispPage = 0;
    }

    if (gameTrackerX.primPool == primPool[0])
    {
        gameTrackerX.primPool = primPool[1];
    }
    else
    {
        gameTrackerX.primPool = primPool[0];
    }

    gameTrackerX.primPool->nextPrim = gameTrackerX.primPool->prim;

    gameTrackerX.primPool->numPrims = 0;
}

void GAMELOOP_Set_Pause_Redraw()
{
    pause_redraw_flag = 1;
}

INCLUDE_ASM("asm/nonmatchings/Game/GAMELOOP", SaveOT);

void ResetDrawPage()
{
    unsigned long **temp;

    temp = gameTrackerX.drawOT;

    gameTrackerX.drawOT = gameTrackerX.dispOT;
    gameTrackerX.dispOT = temp;

    gameTrackerX.drawPage = 1 - gameTrackerX.drawPage;

    ClearOTagR((unsigned long *)gameTrackerX.drawOT, 3072);
}

void GAMELOOP_Set24FPS()
{
    gameTrackerX.frameRate24fps = 1;
}

void GAMELOOP_Reset24FPS()
{
    gameTrackerX.frameRate24fps = 0;
}

void GAMELOOP_DoTimeProcess()
{
    int holdTime;

    holdTime = TIMER_GetTimeMS();

    if (!(gameTrackerX.gameFlags & 0x10000000))
    {
        gameTrackerX.totalTime = TIMER_TimeDiff(gameTrackerX.currentTicks);

        gameTrackerX.currentTicks = (GetRCnt(0xF2000000) & 0xFFFF) | (gameTimer << 16);

        if (gameTrackerX.frameRateLock <= 0)
        {
            gameTrackerX.frameRateLock = 1;
        }

        if (gameTrackerX.frameRateLock >= 3)
        {
            gameTrackerX.frameRateLock = 2;
        }

        if ((gameTrackerX.decoupleGame == 0) || ((gameTrackerX.gameFlags & 0x10000000)))
        {
            if (gameTrackerX.frameRateLock == 1)
            {
                gameTrackerX.lastLoopTime = 33;
            }
            else if (gameTrackerX.frameRateLock == 2)
            {
                gameTrackerX.lastLoopTime = 50;
            }

            gameTrackerX.timeMult = (gameTrackerX.lastLoopTime << 12) / 33;
        }
        else
        {
            int lockRate;
            unsigned long last;

            lockRate = 33;

            if (gameTrackerX.frameRateLock != 1)
            {
                if (gameTrackerX.frameRateLock == 2)
                {
                    lockRate = 50;
                }
            }

            last = lockRate;

            if (gameTrackerX.lastLoopTime != -1U)
            {
                last = holdTime - gameTrackerX.currentTime;
            }

            if ((gameTrackerX.frameRateLock == 1) && (gameTrackerX.frameRate24fps != 0))
            {
                last -= 9;
            }

            if ((last < (unsigned int)lockRate) || (gameTrackerX.gameData.asmData.MorphTime != 1000))
            {
                last = lockRate;
            }
            else if (last > 66)
            {
                last = 66;
            }

            gameTrackerX.timeMult = (last << 12) / 33;

            gameTrackerX.lastLoopTime = last;
        }

        gameTrackerX.gameFramePassed = 0;

        gameTrackerX.globalTimeMult = gameTrackerX.timeMult;

        gameTrackerX.timeSinceLastGameFrame += gameTrackerX.timeMult;

        while (gameTrackerX.timeSinceLastGameFrame >= 4097)
        {
            gameTrackerX.gameFramePassed = 1;

            gameTrackerX.timeSinceLastGameFrame -= 4096;

            gameTrackerX.fps30Count++;
        }
    }
    else
    {
        gameTrackerX.lastLoopTime = -1;
    }

    gameTrackerX.currentTime = holdTime;
}

void GAMELOOP_Process(GameTracker *gameTracker)
{
    int d;
    int useTime;
    Level *level;
    int i;
    SFXMkr *sfxMkr;

    if (gEndGameNow != 0)
    {
        DEBUG_ExitGame();

        gameTracker->levelDone = 3;
    }
    else
    {
        GAMELOOP_DoTimeProcess();

        if ((gameTrackerX.gameMode != 6) && (!(gameTrackerX.streamFlags & 0x100000)))
        {
            MORPH_UpdateTimeMult();

            GAMELOOP_CalcGameTime();

            if (gameTracker->gameData.asmData.MorphType != 0)
            {
                gameTracker->currentSpectralTime += gameTracker->lastLoopTime;
            }
            else
            {
                useTime = 1;

                if (gameTrackerX.playerInstance != NULL)
                {
                    level = STREAM_GetLevelWithID(gameTrackerX.playerInstance->currentStreamUnitID);

                    if (level != NULL)
                    {
                        useTime = (((unsigned long)level->unitFlags & 0x2000)) < (unsigned int)useTime;
                    }
                }

                if (useTime != 0)
                {
                    gameTracker->currentTimeOfDayTime += gameTracker->lastLoopTime;
                }

                gameTracker->currentMaterialTime += gameTracker->lastLoopTime;
            }
        }

        gameTracker->numGSignals = 0;

        GAMELOOP_ChangeMode();

        ResetPrimPool();

        memset(gameTracker->overrideData, 0, sizeof(gameTracker->overrideData));

        if (gameTrackerX.gameMode != 6)
        {
            if (!(gameTrackerX.streamFlags & 0x100000))
            {
                if (gameTrackerX.SwitchToNewStreamUnit == 1)
                {
                    INSTANCE_Post(gameTrackerX.playerInstance, 0x4000006, 0);

                    STREAM_MoveIntoNewStreamUnit();
                }

                if ((VRAM_NeedToUpdateMorph != 0) && (STREAM_IsCdBusy(NULL) == 0))
                {
                    VRAM_UpdateMorphPalettes();

                    VRAM_NeedToUpdateMorph = 0;
                }

                if (gameTracker->gameData.asmData.MorphTime < 1000)
                {
                    MORPH_Continue();
                }

                if ((gameTracker->streamFlags & 0x80000))
                {
                    gameTracker->streamFlags &= ~0x80000;

                    UNDERWORLD_StartProcess();
                }

                EVENT_DoProcess();

                for (i = 0; i < 16; i++)
                {
                    if (StreamTracker.StreamList[i].used == 2)
                    {
                        if (StreamTracker.StreamList[i].level->PuzzleInstances != NULL)
                        {
                            if ((gameTrackerX.debugFlags2 & 0x100))
                            {
                                FONT_Print("Processing unit %s\n", StreamTracker.StreamList[i].baseAreaName);
                            }

                            EVENT_ProcessEvents(StreamTracker.StreamList[i].level->PuzzleInstances, StreamTracker.StreamList[i].level);
                        }

                        EVENT_BSPProcess(&StreamTracker.StreamList[i]);
                    }
                }

                EVENT_ResetAllOneTimeVariables();
            }

            EVENT_ProcessHints();

            for (d = 0; d < 16; d++)
            {
                if (StreamTracker.StreamList[d].used == 2)
                {
                    for (i = 0; i < StreamTracker.StreamList[d].level->NumberOfSFXMarkers; i++)
                    {
                        sfxMkr = &StreamTracker.StreamList[d].level->SFXMarkerList[i];

                        if ((sfxMkr != NULL) && (sfxMkr->soundData != NULL))
                        {
                            SOUND_ProcessInstanceSounds(sfxMkr->soundData, sfxMkr->sfxTbl, &sfxMkr->pos, sfxMkr->livesInOnePlace, sfxMkr->inSpectral, 0, 0, NULL);
                        }
                    }
                }
            }

            if (!(gameTrackerX.streamFlags & 0x100000))
            {
                INSTANCE_SpatialRelationships(instanceList);
            }

            INSTANCE_ProcessFunctions(gameTracker->instanceList);

            INSTANCE_CleanUpInstanceList(gameTracker->instanceList, 0);

            INSTANCE_DeactivateFarInstances(gameTracker);

            MONAPI_ProcessGenerator();

            getScratchAddr(0)[0] = ((unsigned long *)&theCamera.core.position.x)[0];
            getScratchAddr(1)[0] = ((unsigned long *)&theCamera.core.position.z)[0];

            STACK_SAVE();

            G2Instance_BuildTransformsForList(gameTracker->instanceList->first);

            STACK_RESTORE();

            if (!(gameTrackerX.streamFlags & 0x100000))
            {
                STACK_SAVE();

                FX_ProcessList(fxTracker);

                STACK_RESTORE();

                if (!(gameTrackerX.streamFlags & 0x100000))
                {
                    VM_Tick(256);

                    if ((gameTracker->debugFlags2 & 0x10000))
                    {
                        FONT_Print("Military Time %04d\n", gameTrackerX.timeOfDay);
                    }

                    for (d = 0; d < 16; d++)
                    {
                        if (StreamTracker.StreamList[d].used == 2)
                        {
                            VM_ProcessVMObjectList_S(StreamTracker.StreamList[d].level, &theCamera);
                        }
                    }

                    if (!(gameTrackerX.streamFlags & 0x100000))
                    {
                        PLANAPI_UpdatePlanningDatabase(gameTracker, gameTrackerX.playerInstance);
                    }
                }
            }

            DEBUG_Process(gameTracker);

            COLLIDE_InstanceList(gameTracker->instanceList);
            COLLIDE_InstanceListTerrain(gameTracker->instanceList);

            INSTANCE_AdditionalCollideFunctions(instanceList);

            COLLIDE_InstanceListWithSignals(instanceList);

            if (!(gameTrackerX.streamFlags & 0x100000))
            {
                LIGHT_CalcShadowPositions(gameTracker);

                INSTANCE_CleanUpInstanceList(gameTracker->instanceList, 16);
            }

            CAMERA_Process(&theCamera);

            PIPE3D_CalculateWCTransform(&theCamera.core);

            theCamera.core.wcTransform2->m[2][3] = 0;

            PIPE3D_InvertTransform(theCamera.core.cwTransform2, theCamera.core.wcTransform2);

            CAMERA_CalcVVClipInfo(&theCamera);

            if (gameTracker->levelChange != 0)
            {
                gameTracker->levelChange = 0;
            }

            SAVE_IntroduceBufferIntros();
        }
        else
        {
            getScratchAddr(0)[0] = ((unsigned long *)&theCamera.core.position.x)[0];
            getScratchAddr(1)[0] = ((unsigned long *)&theCamera.core.position.z)[0];

            STACK_SAVE();

            G2Instance_BuildTransformsForList(gameTracker->instanceList->first);

            STACK_RESTORE();

            DEBUG_Process(gameTracker);
        }

        if (gameTracker->levelDone == 0)
        {
            GAMELOOP_DisplayFrame(gameTracker);
        }
        else
        {
            ResetDrawPage();
        }

        gameTracker->frameCount++;

        gameTracker->debugFlags &= ~0x8000000;
    }
}

void GAMELOOP_ModeStartRunning()
{
    if ((gameTrackerX.gameMode == 4) || (gameTrackerX.gameMode == 6))
    {
        DEBUG_ExitMenus();

        if (gameTrackerX.gameMode == 6)
        {
            currentMenu = standardMenu;

            SOUND_ResumeAllSound();

            VOICEXA_Resume();
        }
    }

    if ((gameTrackerX.gameFlags & 0x8000000))
    {
        gameTrackerX.gameFlags &= ~0x8000000;

        gameTrackerX.savedOTStart = NULL;

        DrawSync(0);
    }

    gameTrackerX.gameMode = 0;
    gameTrackerX.gameFlags &= ~0x10000000;

    gameTrackerX.playerInstance->flags &= ~0x100;

    gameTrackerX.gameMode = 0;

    GAMEPAD_RestoreControllers();

    INSTANCE_Post(gameTrackerX.playerInstance, 0x10000A, 0);
}

void GAMELOOP_ModeStartPause()
{
    gameTrackerX.gameMode = 6;

    INSTANCE_Post(gameTrackerX.playerInstance, 0x10000A, 1);

    currentMenu = pauseMenu;

    menu_set(gameTrackerX.menu, menudefs_pause_menu);

    SOUND_PauseAllSound();

    VOICEXA_Pause();

    SndPlay(5);

    gameTrackerX.gameFlags |= 0x10000000;

    GAMEPAD_SaveControllers();

    gameTrackerX.gameFlags |= 0x8000000;

    if (gameTrackerX.primPool == primPool[0])
    {
        gameTrackerX.primPool = primPool[1];
    }
    else
    {
        gameTrackerX.primPool = primPool[0];
    }

    gameTrackerX.primPool->nextPrim = &gameTrackerX.primPool->prim[0];

    gameTrackerX.primPool->numPrims = 0;

    SaveOT();

    pause_redraw_flag = 1;
}

void GAMELOOP_ChangeMode()
{
    long *controlCommand;
    int temp; // not from decls.h

    controlCommand = gameTrackerX.controlCommand[0];

    if (!(gameTrackerX.debugFlags & 0x40000))
    {
        if (!(gameTrackerX.debugFlags & 0x200000))
        {
            if ((gameTrackerX.controlCommand[0][0] & 0xA01) == 0xA01)
            {
                theCamera.forced_movement = 1;

                gameTrackerX.playerInstance->position.z += 100;

                gameTrackerX.playerInstance->zVel = 0;

                gameTrackerX.cheatMode = 1;

                INSTANCE_Post(gameTrackerX.playerInstance, 0x100010, 1);

                gameTrackerX.playerInstance->flags &= ~0x800;
            }
            else if ((gameTrackerX.controlCommand[0][0] & 0xA02) == 0xA02)
            {
                theCamera.forced_movement = 1;

                gameTrackerX.playerInstance->position.z -= 100;

                gameTrackerX.playerInstance->zVel = 0;

                gameTrackerX.cheatMode = 0;

                INSTANCE_Post(gameTrackerX.playerInstance, 0x100010, 0);

                gameTrackerX.gameMode = 0;
            }
        }
    }

    if ((!(gameTrackerX.debugFlags & 0x40000)) || ((gameTrackerX.playerCheatFlags & 0x2)))
    {
        if (((controlCommand[1] & 0x60) == 0x60) && (!(controlCommand[0] & 0xF)))
        {
            if (gameTrackerX.gameMode == 0)
            {
                gameTrackerX.gameMode = 4;

                currentMenu = (DebugMenuLine *)&standardMenu;

                if ((unsigned char)gameTrackerX.sound.gVoiceOn != 0)
                {
                    gameTrackerX.debugFlags |= 0x80000;
                }
                else
                {
                    gameTrackerX.debugFlags &= ~0x80000;
                }

                if ((unsigned char)gameTrackerX.sound.gMusicOn != 0)
                {
                    gameTrackerX.debugFlags2 |= 0x1000;
                }
                else
                {
                    gameTrackerX.debugFlags2 &= ~0x1000;
                }

                if ((unsigned char)gameTrackerX.sound.gSfxOn != 0)
                {
                    gameTrackerX.debugFlags2 |= 0x2000;
                }
                else
                {
                    gameTrackerX.debugFlags2 &= ~0x2000;
                }
            }
            else if (gameTrackerX.gameMode == 7)
            {
                DEBUG_EndViewVram(&gameTrackerX);

                gameTrackerX.gameMode = 0;
            }
            else
            {
                GAMELOOP_ModeStartRunning();
            }
        }
    }

    if ((((controlCommand[1] & 0x4000)) || (gamePadControllerOut >= 6)) && (gameTrackerX.gameMode == 0) && (!(gameTrackerX.gameFlags & 0x80)) && ((gameTrackerX.wipeTime == 0) || ((gameTrackerX.wipeType != 11) && (gameTrackerX.wipeTime == -1))))
    {
        GAMELOOP_ModeStartPause();
    }
    else
    {
        temp = controlCommand[1] & 0x4000;

        if (((temp != 0) || ((gameTrackerX.gameFlags & 0x40000000))) && (gameTrackerX.gameMode != 0) && (!(gameTrackerX.gameFlags & 0x20000000)) && ((gameTrackerX.wipeTime == 0) || ((gameTrackerX.wipeType != 11) && (gameTrackerX.wipeTime == -1))))
        {
            if (temp != 0)
            {
                if (!(gameTrackerX.gameFlags & 0x40000000))
                {
                    SndPlay(5);
                }
            }

            gameTrackerX.gameFlags &= ~0x40000000;

            GAMELOOP_ModeStartRunning();
        }
    }

    if ((gameTrackerX.controlCommand[0][0] & 0x40000000))
    {
        gameTrackerX.playerInstance->flags |= 0x100;
    }
    else if ((gameTrackerX.controlCommand[0][2] & 0x40000000))
    {
        gameTrackerX.playerInstance->flags &= ~0x100;
    }
}

void GAMELOOP_RequestLevelChange(char *name, short number, GameTracker *gameTracker)
{
    if (gameTracker->levelChange == 0)
    {
        gameTracker->gameFlags |= 0x1;

        SOUND_ResetAllSound();

        sprintf(gameTracker->baseAreaName, "%s%d", name, number);

        gameTracker->levelChange = 1;

        gameTracker->levelDone = 1;
    }
}

void PSX_GameLoop(GameTracker *gameTracker)
{
    GAMEPAD_Process(gameTracker);

    GAMELOOP_Process(gameTracker);
}

MATRIX *GAMELOOP_GetMatrices(int numMatrices)
{
    MATRIX *matrix;
    PrimPool *pool;

    pool = gameTrackerX.primPool;

    matrix = (MATRIX *)pool->nextPrim;

    if (&matrix[numMatrices] < (MATRIX *)pool->lastPrim)
    {
        pool->nextPrim = (unsigned long *)&matrix[numMatrices];

        return matrix;
    }

    return NULL;
}

GameTracker *GAMELOOP_GetGT()
{
    return &gameTrackerX;
}
