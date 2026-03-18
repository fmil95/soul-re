#include "Overlays/mcardx/mcardx.h"
#include "Game/MCARD/MEMCARD.h"
#include "Game/MENU/MENUDEFS.h"
#include "Game/MENU/MENUFACE.h"
#include "Game/GAMELOOP.h" 

// this conditional is for the objdiff report
#ifndef SKIP_ASM

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_880003B4);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_880003BC);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_88000438);

void func_88000464(mcpsx_t *mcpsx)
{
	long commands;
	long result;
    
	MemCardSync(0, &commands, &result);

	mcpsx->state.sync = sync_idle;
    
	mcpsx->state.func = func_none;
    
	mcpsx->state.err = mcpsx_err_busy;
}

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_880004A8);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_880004DC);

void func_88000510(mcpsx_t *mcpsx, int err)
{
	mcpsx->state.func = func_none;
    
	mcpsx->state.sync = sync_idle;
    
	mcpsx->state.err = err;
    
	mcpsx->state.observed = 0;
}

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_88000528);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_8800053C);

INCLUDE_RODATA("asm/nonmatchings/Overlays/mcardx/mcardx", D_88000000);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_8800055C);

void func_880006B8(mcpsx_t *mcpsx)
{
	long commands;
	long result;
	long sync;

	sync = MemCardSync(1, &commands, &result);
    
	mcpsx->state.sync = sync;

	if (sync == sync_done)
	{
		func_88000510(mcpsx, result);
	}
}

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_88000700);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_8800076C);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_88000790);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_880007A0);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_880007E0);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_88000820);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_88000890);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_88000900);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_88000960);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_880009B0);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_880009F0);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_88000A30);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_88000A90);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_88000B58);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_88000B70);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_88000B90);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_88000C10);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_88000C34);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_88000C68);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_88000C8C);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_88000EF8);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_88000F04);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_88000F10);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_88000F30);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_88000F40);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_88000F58);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_88000F70);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_88000F90);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_88001014);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_88001038);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_8800107C);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_880010AC);

int func_880010DC(void *opaque, int param, menu_ctrl_t ctrl)
{
	mcmenu_t *mcmenu;
    GameTracker *gt;

    (void)param;
	
	mcmenu = (mcmenu_t*)gt2mcmenu(opaque); 

	if ((ctrl == menu_ctrl_engage) || (ctrl == menu_ctrl_cancel)) 
	{
		memcard_pop(mcmenu->opaque);

        gt = GAMELOOP_GetGT();
        
		gt->gameFlags &= ~0x20000000;
        
		return 1;
	}

	return 0;
}

int func_88001148(void *opaque, int param, menu_ctrl_t ctrl)
{
	mcmenu_t *mcmenu;
    GameTracker *gt;
	
	mcmenu = (mcmenu_t*)gt2mcmenu(opaque); 

	if (ctrl == menu_ctrl_engage)
	{
		mcmenu->state.fsm = (fsm_t)param;
        
		return 1;
	}
	else if (ctrl == menu_ctrl_cancel)
	{
		memcard_pop(mcmenu->opaque);

        gt = GAMELOOP_GetGT();
        
		gt->gameFlags &= ~0x20000000;
        
		return 1;
	}

	return 0;
}

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_880011CC);

int func_8800125C(void *opaque, int param, menu_ctrl_t ctrl)
{
	mcmenu_t *mcmenu;
    GameTracker *gt;
	
	mcmenu = (mcmenu_t*)gt2mcmenu(opaque); 

	if (ctrl == menu_ctrl_engage)
	{
        mcmenu->state.slot = (fsm_t)param;
        mcmenu->state.fsm = 8;
        
		return 1;
	}
	else if (ctrl == menu_ctrl_cancel)
	{
		memcard_pop(mcmenu->opaque);

        gt = GAMELOOP_GetGT();
        
		gt->gameFlags &= ~0x20000000;
        
		return 1;
	}

	return 0;
}

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_880012E8);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_880013DC);

int func_88001D50(mcmenu_t *mcmenu, int index)
{
	GameTracker *gt;
	
	gt = GAMELOOP_GetGT();

	gt->gameFlags |= 0x20000000;

	MENUFACE_ChangeStateRandomly(0);

	do_check_controller(gt);

	return func_880013DC(mcmenu, index, 1);
}

int func_88001DBC(mcmenu_t *mcmenu, int index)
{
	GameTracker *gt;
	
	gt = GAMELOOP_GetGT();
	
	gt->gameFlags |= 0x20000000;

	do_check_controller(gt);

	return func_880013DC(mcmenu, index, 0);
}

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_88001E14);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_88001EF8);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_88001F64);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_8800213C);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_88002164);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_880021D0);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_8800228C);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_88002298);

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_880022DC);

#else

void func_880003B4(void) { };

void func_880003BC(void) { };

void func_88000438(void) { };

void func_88000464(mcpsx_t *mcpsx) { };

void func_880004A8(void) { };

void func_880004DC(void) { };

void func_88000510(mcpsx_t *mcpsx, int err) { };

void func_88000528(void) { };

void func_8800053C(void) { };

void func_8800055C(void) { };

void func_880006B8(mcpsx_t *mcpsx) { };

void func_88000700(void) { };

void func_8800076C(void) { };

void func_88000790(void) { };

void func_880007A0(void) { };

void func_880007E0(void) { };

void func_88000820(void) { };

void func_88000890(void) { };

void func_88000900(void) { };

void func_88000960(void) { };

void func_880009B0(void) { };

void func_880009F0(void) { };

void func_88000A30(void) { };

void func_88000A90(void) { };

void func_88000B58(void) { };

void func_88000B70(void) { };

void func_88000B90(void) { };

void func_88000C10(void) { };

void func_88000C34(void) { };

void func_88000C68(void) { };

void func_88000C8C(void) { };

void func_88000EF8(void) { };

void func_88000F04(void) { };

void func_88000F10(void) { };

void func_88000F30(void) { };

void func_88000F40(void) { };

void func_88000F58(void) { };

void func_88000F70(void) { };

void func_88000F90(void) { };

void func_88001014(void) { };

void func_88001038(void) { };

void func_8800107C(void) { };

void func_880010AC(void) { };

int func_880010DC(void *opaque, int param, menu_ctrl_t ctrl) { };

int func_88001148(void *opaque, int param, menu_ctrl_t ctrl) { };

void func_880011CC(void) { };

int func_8800125C(void *opaque, int param, menu_ctrl_t ctrl) { };

void func_880012E8(void) { };

int func_880013DC(mcmenu_t *mcmenu, int index, int arg2) { };  

int func_88001D50(mcmenu_t *mcmenu, int index) { };

int func_88001DBC(mcmenu_t *mcmenu, int index) { };

void func_88001E14(void) { };

void func_88001EF8(void) { };

void func_88001F64(void) { };

void func_8800213C(void) { };

void func_88002164(void) { };

void func_880021D0(void) { };

void func_8800228C(void) { };

void func_88002298(void) { };

void func_880022DC(void) { };

#endif
