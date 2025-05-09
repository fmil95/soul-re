#ifndef _GAMELOOP_H_
#define _GAMELOOP_H_

#include "common.h"

void GAMELOOP_SetGameTime(long timeOfDay);
int GAMELOOP_GetTimeOfDay();
MATRIX *GAMELOOP_GetMatrices(int numMatrices);
void GAMELOOP_Set_Pause_Redraw();
void GAMELOOP_StreamLevelLoadAndInit(char *baseAreaName, GameTracker *gameTracker, int toSignalNum, int fromSignalNum);
void GAMELOOP_RequestLevelChange(char *name, short number, GameTracker *gameTracker);
void ResetPrimPool();
void GAMELOOP_Process(GameTracker *gameTracker);
void ResetDrawPage();
void GAMELOOP_AllocStaticMemory();
void GAMELOOP_InitGameTracker();
void GAMELOOP_AddClearPrim(unsigned long **drawot, int override);
void SaveOT();
void GAMELOOP_Reset24FPS();
void GAMELOOP_Set24FPS();
void GAMELOOP_DisplayFrame(GameTracker *gameTracker);
void GAMELOOP_ChangeMode();
void Switch_For_Redraw();
void MainRenderLevel(StreamUnit *currentUnit, unsigned long **drawot);
void GAMELOOP_DrawSavedOT(unsigned long **newOT);
void GAMELOOP_SetScreenWipe(int time, int maxTime, int type);
void GAMELOOP_FlipScreenAndDraw(GameTracker *gameTracker, unsigned long **drawot);
void GAMELOOP_HandleScreenWipes(unsigned long **drawot);
void GAMELOOP_SystemInit(GameTracker *gameTracker);
void GAMELOOP_LevelLoadAndInit(char *baseAreaName, struct GameTracker *gameTracker);
void PSX_GameLoop(GameTracker *gameTracker);
void GAMELOOP_ResetGameStates();

GameTracker gameTrackerX;
GameTracker *gameTracker;
FXTracker *gFXT;
extern long playerCameraMode;
extern long cameraMode;
DRAWENV draw[2];

#endif
