#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "common.h"

void DEBUG_FatalError(const char *fmt, ...);
void DEBUG_DisplayStatus(GameTracker *gameTracker);
void DEBUG_ProcessSecondController(GameTracker *gameTracker);
void DEBUG_ProcessCheat(GameTracker *gameTracker);
void DEBUG_Menu(GameTracker *gt);
void process_cheat_codes(GameTracker *gt, long *ctrl);
void DEBUG_ViewVram(GameTracker *gameTracker);
void DEBUG_CaptureScreen(GameTracker *gameTracker);
void DEBUG_FillUpHealth(long *var);
void DEBUG_FogLoad();
void DEBUG_SetViewVram();
void DEBUG_KeepGameTime(long *var);
void DEBUG_SetGameTime(long *var);
void DEBUG_UpdateFog(long *var);
void DEBUG_FogRestore();
void DEBUG_UpdateHealth(long *var);
void DEBUG_UpdateMana(long *var);
void DEBUG_SendCinematicSwitch();
void DEBUG_LevelSelectNew();
void DEBUG_SendMoveTo();
void DEBUG_ReloadCurrentLevel();
void handle_line_type_bit(GameTracker *gt, DebugMenuLine *line);
void handle_line_type_long(GameTracker *gt, DebugMenuLine *line);
void handle_line_type_action(GameTracker *gt, DebugMenuLine *line);
void handle_line_type_action_with_line(GameTracker *gt, DebugMenuLine *line);
void handle_line_type_menu(GameTracker *gt, DebugMenuLine *line);
void DEBUG_ExitMenus();
void DEBUG_EndViewVram(GameTracker *gameTracker);
void DEBUG_ExitGame();
void DEBUG_Process(GameTracker *gameTracker);
void DEBUG_DrawShrinkCels(unsigned long **polyAddr);
void DEBUG_Draw(GameTracker *gameTracker, unsigned long **ot);
void DEBUG_ContinueGame();
void DEBUG_DoAreaProtection();

extern unsigned long debugRazielFlags1;
extern unsigned long debugRazielFlags2;
extern unsigned long debugRazielFlags3;
extern DebugMenuLine *currentMenu;
extern DebugMenuLine standardMenu[];
extern DebugMenuLine pauseMenu[];

#endif
