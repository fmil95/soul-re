#include "Overlays/mcardx/mcardx.h"
#include "Game/LOCAL/LOCALSTR.h" 
#include "Game/MCARD/MEMCARD.h"
#include "Game/MENU/MENUDEFS.h"
#include "Game/MENU/MENUFACE.h"
#include "Game/GAMELOOP.h" 


// ---- PsyQ library symbols ----

#define	McErrNone		(0)
#define	McErrCardNotExist	(1)
#define	McErrCardInvalid	(2)

// TODO: Remove once PsyQ inclusion has been updated
extern void _card_open(long); // Not part of the public API
extern void _card_close();   // Not part of the public API
extern long MemCardExist(long chan);
extern long MemCardAccept(long chan);
extern long MemCardReadFile(long chan, char *file, unsigned long *adrs, long ofs, long bytes);
extern long MemCardWriteFile(long chan, char *file, unsigned long *adrs, long ofs, long bytes);
extern long MemCardCreateFile(long chan, char *file, long blocks);
extern long MemCardFormat(long chan);
extern long MemCardDeleteFile(long chan, char *file);
extern long MemCardUnformat(long chan);
extern long MemCardGetDirentry(long chan, char *name, struct DIRENTRY *dir, long *files, long ofs, long max);


// this conditional is for the objdiff report
#ifndef SKIP_ASM

int func_880003B4()
{
    return sizeof(mcpsx_t);
}

void func_880003BC(mcpsx_t *mcpsx, void *opaque)
{

    memset(mcpsx, 0, sizeof(mcpsx_t));

    PadStopCom();
    _card_open(1);
    PadStartCom();

    mcpsx->state.mode = mode_initialized;
    mcpsx->state.sync = sync_idle;
    mcpsx->state.func = func_none;
    mcpsx->state.err = mcpsx_err_busy;
    mcpsx->state.observed = 0;
    mcpsx->params.channel = 0;
    mcpsx->params.offset = 0;
    mcpsx->opaque = opaque;

}

void func_88000438(mcpsx_t *mcpsx)
{
    _card_close();
    mcpsx->state.mode = mode_terminated;
}

void func_88000464(mcpsx_t *mcpsx)
{
    long commands;
    long result;

    MemCardSync(0, &commands, &result);

    mcpsx->state.sync = sync_idle;
    mcpsx->state.func = func_none;
    mcpsx->state.err = mcpsx_err_busy;
}

void func_880004A8(mcpsx_t *mcpsx)
{
    MemCardStart();
    func_88000464(mcpsx);
    mcpsx->state.mode = mode_running;
}

void func_880004DC(mcpsx_t *mcpsx)
{
    func_88000464(mcpsx);
    MemCardStop();
    mcpsx->state.mode = mode_initialized;
}

void func_88000510(mcpsx_t *mcpsx, mcpsx_err_t err)
{
    mcpsx->state.func = func_none;
    mcpsx->state.sync = sync_idle;
    mcpsx->state.err = err;
    mcpsx->state.observed = 0;
}

void func_88000528(mcpsx_t *mcpsx, mcpsx_sync_t sync)
{
    if (sync != sync_busy)
    {
        mcpsx->state.sync = sync_busy;
    }
}

void func_8800053C(mcpsx_t *mcpsx, mcpsx_err_t err)
{
    func_88000510(mcpsx, err);
}

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

int func_88000700(mcpsx_t *mcpsx) 
{
	if (mcpsx->state.observed == 0)
	{
		return 0;
	}

	switch ((int)mcpsx->state.sync)
	{
	case sync_func:
		func_8800055C(mcpsx);
		
        return 0;
	case sync_busy:
		func_880006B8(mcpsx);

		return 0;
	case sync_idle:
		return 1;
	}

	return 1;
}

int func_8800076C(mcpsx_t *mcpsx)
{
	if (mcpsx->state.sync == sync_idle)
	{
		mcpsx->state.observed = 1;

		return mcpsx->state.err;
	}

	return mcpsx_err_busy;
}

void func_88000790(mcpsx_t *mcpsx, mcpsx_func_t func)
{
    mcpsx->state.func = func;
    mcpsx->state.sync = sync_func;
}

int func_880007A0(mcpsx_t *mcpsx)
{
	if (func_88000700(mcpsx) != 0)
	{
		func_88000790(mcpsx, func_exist);

		return 1;
	}

	return 0;
}

int func_880007E0(mcpsx_t *mcpsx)
{
	if (func_88000700(mcpsx) != 0)
	{
		func_88000790(mcpsx, func_accept);

		return 1;
	}

	return 0;
}

int func_88000820(mcpsx_t *mcpsx, char *filename, void *buffer, long nbytes)
{
	if (func_88000700(mcpsx) != 0)
	{
		func_88000790(mcpsx, func_read_file);

		mcpsx->params.filename = filename;
		mcpsx->params.buffer   = buffer;

		mcpsx->params.nbytes = nbytes;

		return 1;
	}

	return 0;
}

int func_88000890(mcpsx_t *mcpsx, char *filename, void *buffer, long nbytes)
{
	if (func_88000700(mcpsx) != 0)
	{
		func_88000790(mcpsx, func_write_file);

		mcpsx->params.filename = filename;
		mcpsx->params.buffer   = buffer;

		mcpsx->params.nbytes = nbytes;

		return 1;
	}

	return 0;
}

int func_88000900(mcpsx_t *mcpsx, char *filename, long nblocks)
{
	if (func_88000700(mcpsx) != 0)
	{
		func_88000790(mcpsx, func_create_file);

		mcpsx->params.filename = filename;

		mcpsx->params.nblocks = nblocks;

		return 1;
	}

	return 0;
}

int func_88000960(mcpsx_t *mcpsx, char *filename)
{
	if (func_88000700(mcpsx) != 0)
	{
		func_88000790(mcpsx, func_delete_file);

		mcpsx->params.filename = filename;

		return 1;
	}

	return 0;
}

int func_880009B0(mcpsx_t *mcpsx)
{
	if (func_88000700(mcpsx) != 0)
	{
		func_88000790(mcpsx, func_format);

		return 1;
	}

	return 0;
}

int func_880009F0(mcpsx_t *mcpsx)
{
	if (func_88000700(mcpsx) != 0)
	{
		func_88000790(mcpsx, func_unformat);

		return 1;
	}

	return 0;
}

int func_88000A30(mcpsx_t *mcpsx, mcpsx_directory_t *directory, long *nfilesptr)
{
	if (func_88000700(mcpsx) != 0)
	{
		func_88000790(mcpsx, func_directory);

		mcpsx->params.directory = directory;
		mcpsx->params.nfilesptr = nfilesptr;

		return 1;
	}

	return 0;
}

long func_88000A90(long channel, mcpsx_directory_t *directory, long *nfilesptr)
{

    struct DIRENTRY dirEntries[15];
    int result;
    int i;
    struct DIRENTRY *currentDirEntry;
    mcpsx_directory_t *currentDirectory;

    result = MemCardGetDirentry(channel, "*", dirEntries, nfilesptr, 0, 15);

    if (result == McErrNone)
    {

        i = 0;

        if (result < *nfilesptr)
        {

            currentDirectory = directory;
            currentDirEntry = dirEntries;

            for (; i < *nfilesptr; currentDirEntry++, i++, currentDirectory++)
            {
                memcpy(currentDirectory->name, currentDirEntry->name, sizeof(currentDirectory->name));
                currentDirectory->nbytes = currentDirEntry->size;
            }
        }
    }

    return result;

}

void func_88000B58(mcard_t *mcard)
{
	mcard->state.err    = mcpsx_err_new_card;
	mcard->state.fsm    = fsmcard_new_card;
	mcard->state.status = mcard_status_new_card;

	mcard->state.not_exists = 0;
}

int func_88000B70()
{
	return func_880003B4() + sizeof(mcard_t);
}

void func_88000B90(mcard_t *mcard, void *opaque, mcpsx_directory_t *directory, long *nfilesptr)
{
	mcpsx_t *mcpsx;

	mcpsx = (mcpsx_t *)&mcard[1];

	memset(mcard, 0, sizeof(mcard_t));

	mcard->mcpsx  = mcpsx;
	mcard->opaque = opaque;

	mcard->params.nfilesptr = nfilesptr;
	mcard->params.directory = directory;

	func_880003BC(mcard->mcpsx, mcard);

	func_88000B58(mcard);
}

void func_88000C10(mcard_t *mcard)
{
	func_88000438(mcard->mcpsx);
}

void func_88000C34(mcard_t *mcard)
{
	func_880004A8(mcard->mcpsx);

	func_88000B58(mcard);
}

void func_88000C68(mcard_t *mcard)
{
	func_880004DC(mcard->mcpsx);
}

INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_88000C8C);

void func_88000EF8(mcard_t *mcard)
{
    mcard->state.fsm = fsmcard_format;
};

void func_88000F04(mcard_t *mcard)
{
	mcard->state.fsm = fsmcard_unformat;
}

void func_88000F10(mcard_t *mcard, char *filename, int nblocks, void *buffer, int nbytes)
{
    mcard->state.fsm = fsmcard_create;
    mcard->params.filename = filename;
    mcard->params.nblocks = nblocks;
    mcard->params.buffer = buffer;
    mcard->params.nbytes = nbytes;
}

void func_88000F30(mcard_t *mcard, char *filename)
{
	mcard->state.fsm = fsmcard_delete;

	mcard->params.filename = filename;
}

void func_88000F40(mcard_t *mcard, char *filename, void *buffer, int nbytes)
{
	mcard->state.fsm = fsmcard_write;

	mcard->params.filename = filename;
	mcard->params.buffer   = buffer;

	mcard->params.nbytes = nbytes;
}

void func_88000F58(mcard_t *mcard, char *filename, void *buffer, int nbytes)
{
	mcard->state.fsm = fsmcard_read;

	mcard->params.filename = filename;
	mcard->params.buffer   = buffer;

	mcard->params.nbytes = nbytes;
}

int func_88000F70()
{
	return func_88000B70() + sizeof(mcmenu_t);
}

void func_88000F90(mcmenu_t *mcmenu, void *opaque, int nblocks)
{
	mcard_t *mcard;

	mcard = (mcard_t *)&mcmenu[1];

	memset(mcmenu, 0, sizeof(mcmenu_t));

	mcmenu->state.status   = mcard_status_new_card;
	mcmenu->state.fsm      = fsm_new_card;
	mcmenu->state.fsm_prev = fsm_new_card;

	mcmenu->state.running = 0;

	mcmenu->params.nblocks = nblocks;

	mcmenu->mcard  = mcard;
	mcmenu->opaque = opaque;

	func_88000B90(mcard, mcmenu, mcmenu->params.directory, &mcmenu->params.nfiles);
}

void func_88001014(mcmenu_t *mcmenu)
{
	func_88000C10(mcmenu->mcard);
}

void func_88001038(mcmenu_t *mcmenu)
{
	func_88000C34(mcmenu->mcard);

	mcmenu->state.status   = mcard_status_new_card;
	mcmenu->state.fsm      = fsm_new_card;
	mcmenu->state.fsm_prev = fsm_new_card;

	mcmenu->state.running = 1;
}

void func_8800107C(mcmenu_t *mcmenu)
{
	func_88000C68(mcmenu->mcard);

	mcmenu->state.running = 0;
}

int func_880010AC(mcmenu_t *mcmenu, void *buffer, int nbytes)
{
	mcmenu->params.buffer = buffer;
	mcmenu->params.nbytes = nbytes;

	return func_88002298(buffer, mcmenu->params.nblocks);
}

int func_880010DC(void *opaque, int param, menu_ctrl_t ctrl)
{
    mcmenu_t *mcmenu;
    GameTracker *gt;

    (void)param;

    mcmenu = (mcmenu_t *)gt2mcmenu(opaque);

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

    mcmenu = (mcmenu_t *)gt2mcmenu(opaque);

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

int func_880011CC(void *opaque, int param, menu_ctrl_t ctrl)
{
	mcmenu_t *mcmenu;
	GameTracker *gt;

	mcmenu = (mcmenu_t*)gt2mcmenu(opaque);

	if (ctrl == menu_ctrl_engage)
	{
		gt = GAMELOOP_GetGT();

		gt->gameFlags |= 0x20000000;

		func_88000F58(mcmenu->mcard, mcmenu->params.directory[param].name, mcmenu->params.buffer, mcmenu->params.nbytes);

		mcmenu->state.fsm = fsm_loading;

		return 1;
	}

	return 0;
}

int func_8800125C(void *opaque, int param, menu_ctrl_t ctrl)
{
    mcmenu_t *mcmenu;
    GameTracker *gt;

    mcmenu = (mcmenu_t *)gt2mcmenu(opaque);

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

int func_880012E8(mcmenu_t *mcmenu, int nfiles, int index, char *title, char *subtitle)
{
	memcard_item(mcmenu->opaque, NULL, 0, 4, title);

	if (subtitle != NULL)
	{
		memcard_item(mcmenu->opaque, NULL, 0, 4, subtitle);
	}

	if (nfiles != 0)
	{
		memcard_item(mcmenu->opaque, func_88001148, fsm_start, 0, localstr_get(LOCALSTR_start_game));
	}

	memcard_item(mcmenu->opaque, func_880010DC, 0, 0, localstr_get(LOCALSTR_go_back));

	if (nfiles != 0)
	{
		index = (index < 0) ? 3 : index;
	}
	else
	{
		index = (index < 0) ? 2 : index;
	}

	return index;
}

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

void func_880022DC(void *prm0, void *prm1)
{
	func_8800213C(prm0, prm1);
}

#else

int func_880003B4()
{
    return sizeof(mcpsx_t);
}

void func_880003BC(mcpsx_t *mcpsx, void *opaque)
{

    memset(mcpsx, 0, sizeof(mcpsx_t));

    PadStopCom();
    _card_open(1);
    PadStartCom();

    mcpsx->state.mode = mode_initialized;
    mcpsx->state.sync = sync_idle;
    mcpsx->state.func = func_none;
    mcpsx->state.err = mcpsx_err_busy;
    mcpsx->state.observed = 0;
    mcpsx->params.channel = 0;
    mcpsx->params.offset = 0;
    mcpsx->opaque = opaque;

}

void func_88000438(mcpsx_t *mcpsx)
{
    _card_close();
    mcpsx->state.mode = mode_terminated;
}

void func_88000464(mcpsx_t *mcpsx)
{
    long commands;
    long result;

    MemCardSync(0, &commands, &result);

    mcpsx->state.sync = sync_idle;
    mcpsx->state.func = func_none;
    mcpsx->state.err = mcpsx_err_busy;
}

void func_880004A8(mcpsx_t *mcpsx)
{
    MemCardStart();
    func_88000464(mcpsx);
    mcpsx->state.mode = mode_running;
}

void func_880004DC(mcpsx_t *mcpsx)
{
    func_88000464(mcpsx);
    MemCardStop();
    mcpsx->state.mode = mode_initialized;
}

void func_88000510(mcpsx_t *mcpsx, mcpsx_err_t err)
{
    mcpsx->state.func = func_none;
    mcpsx->state.sync = sync_idle;
    mcpsx->state.err = err;
    mcpsx->state.observed = 0;
}

void func_88000528(mcpsx_t *mcpsx, mcpsx_sync_t sync)
{
    if (sync != sync_busy)
    {
        mcpsx->state.sync = sync_busy;
    }
}

void func_8800053C(mcpsx_t *mcpsx, mcpsx_err_t err)
{
    func_88000510(mcpsx, err);
}

void func_8800055C(mcpsx_t *mcpsx) {};

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

int func_88000700(mcpsx_t *mcpsx) 
{
	if (mcpsx->state.observed == 0)
	{
		return 0;
	}

	switch ((int)mcpsx->state.sync)
	{
	case sync_func:
		func_8800055C(mcpsx);
		
        return 0;
	case sync_busy:
		func_880006B8(mcpsx);

		return 0;
	case sync_idle:
		return 1;
	}

	return 1;
}

int func_8800076C(mcpsx_t *mcpsx)
{
	if (mcpsx->state.sync == sync_idle)
	{
		mcpsx->state.observed = 1;

		return mcpsx->state.err;
	}

	return mcpsx_err_busy;
}

void func_88000790(mcpsx_t *mcpsx, mcpsx_func_t func)
{
    mcpsx->state.func = func;
    mcpsx->state.sync = sync_func;
}

int func_880007A0(mcpsx_t *mcpsx)
{
	if (func_88000700(mcpsx) != 0)
	{
		func_88000790(mcpsx, func_exist);

		return 1;
	}

	return 0;
}

int func_880007E0(mcpsx_t *mcpsx)
{
	if (func_88000700(mcpsx) != 0)
	{
		func_88000790(mcpsx, func_accept);

		return 1;
	}

	return 0;
}

int func_88000820(mcpsx_t *mcpsx, char *filename, void *buffer, long nbytes)
{
	if (func_88000700(mcpsx) != 0)
	{
		func_88000790(mcpsx, func_read_file);

		mcpsx->params.filename = filename;
		mcpsx->params.buffer   = buffer;

		mcpsx->params.nbytes = nbytes;

		return 1;
	}

	return 0;
}

int func_88000890(mcpsx_t *mcpsx, char *filename, void *buffer, long nbytes)
{
	if (func_88000700(mcpsx) != 0)
	{
		func_88000790(mcpsx, func_write_file);

		mcpsx->params.filename = filename;
		mcpsx->params.buffer   = buffer;

		mcpsx->params.nbytes = nbytes;

		return 1;
	}

	return 0;
}

int func_88000900(mcpsx_t *mcpsx, char *filename, long nblocks)
{
	if (func_88000700(mcpsx) != 0)
	{
		func_88000790(mcpsx, func_create_file);

		mcpsx->params.filename = filename;

		mcpsx->params.nblocks = nblocks;

		return 1;
	}

	return 0;
}

int func_88000960(mcpsx_t *mcpsx, char *filename)
{
	if (func_88000700(mcpsx) != 0)
	{
		func_88000790(mcpsx, func_delete_file);

		mcpsx->params.filename = filename;

		return 1;
	}

	return 0;
}

int func_880009B0(mcpsx_t *mcpsx)
{
	if (func_88000700(mcpsx) != 0)
	{
		func_88000790(mcpsx, func_format);

		return 1;
	}

	return 0;
}

int func_880009F0(mcpsx_t *mcpsx)
{
	if (func_88000700(mcpsx) != 0)
	{
		func_88000790(mcpsx, func_unformat);

		return 1;
	}

	return 0;
}

int func_88000A30(mcpsx_t *mcpsx, mcpsx_directory_t *directory, long *nfilesptr)
{
	if (func_88000700(mcpsx) != 0)
	{
		func_88000790(mcpsx, func_directory);

		mcpsx->params.directory = directory;
		mcpsx->params.nfilesptr = nfilesptr;

		return 1;
	}

	return 0;
}

long func_88000A90(long channel, mcpsx_directory_t *directory, long *nfilesptr)
{

    struct DIRENTRY dirEntries[15];
    int result;
    int i;
    struct DIRENTRY *currentDirEntry;
    mcpsx_directory_t *currentDirectory;

    result = MemCardGetDirentry(channel, "*", dirEntries, nfilesptr, 0, 15);

    if (result == McErrNone)
    {

        i = 0;

        if (result < *nfilesptr)
        {

            currentDirectory = directory;
            currentDirEntry = dirEntries;

            for (; i < *nfilesptr; currentDirEntry++, i++, currentDirectory++)
            {
                memcpy(currentDirectory->name, currentDirEntry->name, sizeof(currentDirectory->name));
                currentDirectory->nbytes = currentDirEntry->size;
            }
        }
    }

    return result;

}

void func_88000B58(mcard_t *mcard)
{
	mcard->state.err    = mcpsx_err_new_card;
	mcard->state.fsm    = fsmcard_new_card;
	mcard->state.status = mcard_status_new_card;

	mcard->state.not_exists = 0;
}

int func_88000B70()
{
	return func_880003B4() + sizeof(mcard_t);
}

void func_88000B90(mcard_t *mcard, void *opaque, mcpsx_directory_t *directory, long *nfilesptr)
{
	mcpsx_t *mcpsx;

	mcpsx = (mcpsx_t *)&mcard[1];

	memset(mcard, 0, sizeof(mcard_t));

	mcard->mcpsx  = mcpsx;
	mcard->opaque = opaque;

	mcard->params.nfilesptr = nfilesptr;
	mcard->params.directory = directory;

	func_880003BC(mcard->mcpsx, mcard);

	func_88000B58(mcard);
}

void func_88000C10(mcard_t *mcard)
{
	func_88000438(mcard->mcpsx);
}

void func_88000C34(mcard_t *mcard)
{
	func_880004A8(mcard->mcpsx);

	func_88000B58(mcard);
}

void func_88000C68(mcard_t *mcard)
{
	func_880004DC(mcard->mcpsx);
}

mcard_status_t func_88000C8C(mcard_t *mcard) {};

void func_88000EF8(mcard_t *mcard)
{
    mcard->state.fsm = fsmcard_format;
};

void func_88000F04(mcard_t *mcard)
{
	mcard->state.fsm = fsmcard_unformat;
}

void func_88000F10(mcard_t *mcard, char *filename, int nblocks, void *buffer, int nbytes)
{
    mcard->state.fsm = fsmcard_create;
    mcard->params.filename = filename;
    mcard->params.nblocks = nblocks;
    mcard->params.buffer = buffer;
    mcard->params.nbytes = nbytes;
}

void func_88000F30(mcard_t *mcard, char *filename)
{
	mcard->state.fsm = fsmcard_delete;

	mcard->params.filename = filename;
}

void func_88000F40(mcard_t *mcard, char *filename, void *buffer, int nbytes)
{
	mcard->state.fsm = fsmcard_write;

	mcard->params.filename = filename;
	mcard->params.buffer   = buffer;
    
	mcard->params.nbytes = nbytes;
}

void func_88000F58(mcard_t *mcard, char *filename, void *buffer, int nbytes)
{
	mcard->state.fsm = fsmcard_read;

	mcard->params.filename = filename;
	mcard->params.buffer   = buffer;
    
	mcard->params.nbytes = nbytes;
}

int func_88000F70()
{
	return func_88000B70() + sizeof(mcmenu_t);
}

void func_88000F90(mcmenu_t *mcmenu, void *opaque, int nblocks)
{
	mcard_t *mcard;

	mcard = (mcard_t *)&mcmenu[1];

	memset(mcmenu, 0, sizeof(mcmenu_t));

	mcmenu->state.status   = mcard_status_new_card;
	mcmenu->state.fsm      = fsm_new_card;
	mcmenu->state.fsm_prev = fsm_new_card;

	mcmenu->state.running = 0;

	mcmenu->params.nblocks = nblocks;

	mcmenu->mcard  = mcard;
	mcmenu->opaque = opaque;

	func_88000B90(mcard, mcmenu, mcmenu->params.directory, &mcmenu->params.nfiles);
}

void func_88001014(mcmenu_t *mcmenu)
{
	func_88000C10(mcmenu->mcard);
}

void func_88001038(mcmenu_t *mcmenu)
{
	func_88000C34(mcmenu->mcard);

	mcmenu->state.status   = mcard_status_new_card;
	mcmenu->state.fsm      = fsm_new_card;
	mcmenu->state.fsm_prev = fsm_new_card;

	mcmenu->state.running = 1;
}

void func_8800107C(mcmenu_t *mcmenu)
{
	func_88000C68(mcmenu->mcard);

	mcmenu->state.running = 0;
}

int func_880010AC(mcmenu_t *mcmenu, void *buffer, int nbytes)
{
	mcmenu->params.buffer = buffer;
	mcmenu->params.nbytes = nbytes;

	return func_88002298(buffer, mcmenu->params.nblocks);
}

int func_880010DC(void *opaque, int param, menu_ctrl_t ctrl)
{
    mcmenu_t *mcmenu;
    GameTracker *gt;

    (void)param;

    mcmenu = (mcmenu_t *)gt2mcmenu(opaque);

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

    mcmenu = (mcmenu_t *)gt2mcmenu(opaque);

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

int func_880011CC(void *opaque, int param, menu_ctrl_t ctrl)
{
	mcmenu_t *mcmenu;
	GameTracker *gt;

	mcmenu = (mcmenu_t*)gt2mcmenu(opaque);

	if (ctrl == menu_ctrl_engage)
	{
		gt = GAMELOOP_GetGT();

		gt->gameFlags |= 0x20000000;

		func_88000F58(mcmenu->mcard, mcmenu->params.directory[param].name, mcmenu->params.buffer, mcmenu->params.nbytes);

		mcmenu->state.fsm = fsm_loading;

		return 1;
	}

	return 0;
}

int func_8800125C(void *opaque, int param, menu_ctrl_t ctrl)
{
    mcmenu_t *mcmenu;
    GameTracker *gt;

    mcmenu = (mcmenu_t *)gt2mcmenu(opaque);

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

int func_880012E8(mcmenu_t *mcmenu, int nfiles, int index, char *title, char *subtitle)
{
	memcard_item(mcmenu->opaque, NULL, 0, 4, title);

	if (subtitle != NULL)
	{
		memcard_item(mcmenu->opaque, NULL, 0, 4, subtitle);
	}

	if (nfiles != 0)
	{
		memcard_item(mcmenu->opaque, func_88001148, fsm_start, 0, localstr_get(LOCALSTR_start_game));
	}

	memcard_item(mcmenu->opaque, func_880010DC, 0, 0, localstr_get(LOCALSTR_go_back));

	if (nfiles != 0)
	{
		index = (index < 0) ? 3 : index;
	}
	else
	{
		index = (index < 0) ? 2 : index;
	}

	return index;
}

int func_880013DC(mcmenu_t *mcmenu, int index, int arg2) {};

int func_88001D50(mcmenu_t *mcmenu, int index) {};

int func_88001DBC(mcmenu_t *mcmenu, int index)
{
    GameTracker *gt;

    gt = GAMELOOP_GetGT();

    gt->gameFlags |= 0x20000000;

    do_check_controller(gt);

    return func_880013DC(mcmenu, index, 0);
}

void func_88001E14(void) {};

void func_88001EF8(void) {};

void func_88001F64(void) {};

void func_8800213C(void *prm0, void *prm1) {};

void func_88002164(void) {};

void func_880021D0(void) {};

void func_8800228C(void) {};

int func_88002298(void *buffer, int nblocks) {};

void func_880022DC(void *prm0, void *prm1)
{
	func_8800213C(prm0, prm1);
}

#endif
