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


mcard_err_entry_t D_8800571C[] = {
    { mcpsx_err_none, fsmcard_error, mcard_status_error },
    { mcpsx_err_card_not_exist, fsmcard_card_not_exist, mcard_status_card_not_exist },
    { mcpsx_err_card_invalid, fsmcard_card_invalid, mcard_status_card_invalid },
    { mcpsx_err_new_card, fsmcard_new_card, mcard_status_new_card },
    { mcpsx_err_not_format, fsmcard_not_format, mcard_status_not_format },
    { mcpsx_err_file_not_exist, fsmcard_error, mcard_status_error },
    { mcpsx_err_already_exists, fsmcard_error, mcard_status_error },
    { mcpsx_err_block_full, fsmcard_error, mcard_status_error },
};

// unused
char D_8800577C[3][16] = { "soul reaver a", "soul reaver b", "soul reaver c" };

mcmenu_fsm_entry_t D_880057AC[] = {
    { mcard_status_card_not_exist, fsm_card_not_exist },
    { mcard_status_card_invalid, fsm_card_invalid },
    { mcard_status_not_format, fsm_not_format },
    { mcard_status_new_card, fsm_new_card },
    { mcard_status_ready, fsm_ready },
};

mcardx_ascii_t D_880057D4[3] = {
    { 0x824F, '0' },
    { 0x8260, 'A' },
    { 0x8281, 'a' },
};

unsigned short D_880057E0[] = {
    0x8140, 0x8149, 0x8168, 0x8194, 0x8190, 0x8193, 0x8195, 0x8166,
    0x8169, 0x816A, 0x8196, 0x817B, 0x8143, 0x817C, 0x8144, 0x815E,
    0x8146, 0x8147, 0x8171, 0x8181, 0x8172, 0x8148, 0x8197, 0x816D,
    0x818F, 0x816E, 0x814F, 0x8151, 0x8165, 0x816F, 0x8162, 0x8170,
    0x8150, 0x0000,
};

char D_88005824[5][13] = { "BASLUS-00708", "BESLES-02024", "BESLES-02025", "BESLES-02026", "BESLES-02027" };

mcardx_icon_t D_88005868[] = {
    {
        "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x10\x11\x02\x00\x00\x00\x00\x00\x31\x12\x24\x00\x00\x00"
        "\x00\x00\x35\x15\x64\x01\x00\x00\x00\x10\x75\x25\x44\x02\x00\x00"
        "\x00\x30\x75\x28\x86\x04\x00\x00\x00\x10\x77\x92\x5A\x05\xAA\xAA"
        "\x00\x00\x87\x9B\xAA\xA3\xCA\xCC\x00\x00\x8A\xED\xAC\xA7\xCC\xCC"
        "\x00\x00\xDA\xE8\xA9\x3A\xCC\xCC\x00\x00\xBE\xB8\xCE\xAA\xC3\xCC"
        "\x00\x00\xB9\xD8\x9E\xAA\xC6\xCC\x00\x00\xEB\x89\xEB\xAC\x3A\xCC"
        "\x00\xE0\xDD\x6E\xE8\xAC\xAA\xC6\x00\xDE\xDD\xBD\xB8\xC9\xAA\x6A"
    },
};

mcardx_clut_t D_880058E8[] = {
    {
        "\x00\x80\x61\x10\xA4\x18\x10\x32\x87\x35\x06\x21\x2A\x1D\x14\x47"
        "\xDE\x5F\x0C\x09\x43\x08\x16\x1E\x65\x08\x59\x22\x70\x0D\xFF\x7F"
    },
};


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

void func_8800055C(mcpsx_t *mcpsx)
{
	switch ((int)mcpsx->state.func)
	{
	case func_exist:
		func_88000528(mcpsx, MemCardExist(mcpsx->params.channel));
		break;
	case func_accept:
		func_88000528(mcpsx, MemCardAccept(mcpsx->params.channel));
		break;
	case func_read_file:
		func_88000528(mcpsx, MemCardReadFile(mcpsx->params.channel, mcpsx->params.filename, mcpsx->params.buffer, mcpsx->params.offset, mcpsx->params.nbytes));
		break;
	case func_write_file:
		func_88000528(mcpsx, MemCardWriteFile(mcpsx->params.channel, mcpsx->params.filename, mcpsx->params.buffer, mcpsx->params.offset, mcpsx->params.nbytes));
		break;
	case func_create_file:
		func_8800053C(mcpsx, MemCardCreateFile(mcpsx->params.channel, mcpsx->params.filename, mcpsx->params.nblocks));
		break;
	case func_delete_file:
		func_8800053C(mcpsx, MemCardDeleteFile(mcpsx->params.channel, mcpsx->params.filename)); 
		break;
	case func_format:
		func_8800053C(mcpsx, MemCardFormat(mcpsx->params.channel));
		break;
	case func_unformat:
		func_8800053C(mcpsx, MemCardUnformat(mcpsx->params.channel)); 
		break;
	case func_directory:
		func_8800053C(mcpsx, func_88000A90(mcpsx->params.channel, mcpsx->params.directory, mcpsx->params.nfilesptr));
		break;
	}
}

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

mcard_status_t func_88000C8C(mcard_t *mcard)
{
	mcpsx_err_t err;
	mcard_err_entry_t *entry;
	int ok;

	ok = 1;

	err = func_8800076C(mcard->mcpsx);

	if ((err != mcard->state.err) && (err != mcpsx_err_none) && (err < mcpsx_err_busy))
	{
		entry = &D_8800571C[err];

		if ((err == mcpsx_err_card_not_exist) && (mcard->state.not_exists < 0))
		{
			mcard->state.not_exists++;
		}
		else
		{
			mcard->state.err    = err;
			mcard->state.fsm    = entry->fsm;
			mcard->state.status = entry->status;

			mcard->state.not_exists = 0;
		}
	}

	if (mcard->state.fsm >= fsmcard_format)
	{
		mcard->state.status = mcard_status_accessing;
	}

	switch (mcard->state.fsm)
	{
	case 0:
		if (err == mcpsx_err_none)
		{
			mcard->state.fsm = fsmcard_new_card;
		}

		break;
	case 1:
	case fsmcard_not_format:
		break;
	case fsmcard_new_card:
		ok = func_880007E0(mcard->mcpsx);

		if (ok != 0)
		{
			mcard->state.fsm = fsmcard_directory;
		}

		break;
	case fsmcard_directory:
		ok = func_88000A30(mcard->mcpsx, mcard->params.directory, mcard->params.nfilesptr);

		if (ok != 0)
		{
			mcard->state.fsm = fsmcard_ready;
		}

		break;
	case fsmcard_ready:
		if (err == mcpsx_err_none)
		{
			mcard->state.status = mcard_status_ready;
		}

		if (err == mcpsx_err_new_card)
		{
			mcard->state.fsm = (fsmcard_t)err;
		}

		break;
	case fsmcard_format:
		ok = func_880009B0(mcard->mcpsx);

		if (ok != 0)
		{
			mcard->state.fsm = fsmcard_directory;
		}

		break;
	case fsmcard_unformat:
		ok = func_880009F0(mcard->mcpsx);

		if (ok != 0)
		{
			mcard->state.fsm = fsmcard_directory;
		}

		break;
	case fsmcard_create:
		ok = func_88000900(mcard->mcpsx, mcard->params.filename, mcard->params.nblocks);

		if (ok != 0)
		{
			mcard->state.fsm = fsmcard_write;
		}

		break;
	case fsmcard_delete:
		ok = func_88000960(mcard->mcpsx, mcard->params.filename);

		if (ok != 0)
		{
			mcard->state.fsm = fsmcard_ready;
		}

		break;
	case fsmcard_write:
		ok = func_88000890(mcard->mcpsx, mcard->params.filename, mcard->params.buffer, mcard->params.nbytes);

		if (ok != 0)
		{
			mcard->state.fsm = fsmcard_ready;
		}

		break;
	case fsmcard_read:
		ok = func_88000820(mcard->mcpsx, mcard->params.filename, mcard->params.buffer, mcard->params.nbytes);

		if (ok != 0)
		{
			mcard->state.fsm = fsmcard_ready;
		}

		break;
	default:
		mcard->state.status = mcard_status_new_card;
		mcard->state.fsm    = fsmcard_new_card;
		break;
	}

	if (ok != 0)
	{
		func_880007A0(mcard->mcpsx);
	}

	return mcard->state.status;
}

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

// differs only in register allocation and some instruction scheduling
#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/Overlays/mcardx/mcardx", func_880013DC);
#else
int func_880013DC(mcmenu_t *mcmenu, int index, int load)
{
	char title[32];
	GameTracker *gt;
	mcard_status_t status;
	mcpsx_directory_t *entry;
	int i;
	int nused;
	int nblocks;
	int slots;
	mcpsx_directory_t *e2;

	gt = GAMELOOP_GetGT();

	status = func_88000C8C(mcmenu->mcard);

	if ((status != mcmenu->state.status) && (status < mcard_status_ready))
	{
		mcmenu->state.status = status;
		mcmenu->state.fsm    = D_880057AC[status].fsm;

		index = -1;
	}

again:
	if (mcmenu->state.fsm != mcmenu->state.fsm_prev)
	{
		index = -1;
	}

	mcmenu->state.fsm_prev = mcmenu->state.fsm;

	switch (mcmenu->state.fsm)
	{
	case fsm_card_not_exist:
		if (load != 0)
		{
			index = func_880012E8(mcmenu, load, index, localstr_get(LOCALSTR_no_card), localstr_get(LOCALSTR_warning_one));
			break;
		}

		index = func_880012E8(mcmenu, 0, index, localstr_get(LOCALSTR_no_card), localstr_get(LOCALSTR_insert_card));
		break;
	case fsm_card_invalid:
		index = func_880012E8(mcmenu, load, index, localstr_get(LOCALSTR_invalid_card), localstr_get(LOCALSTR_try_card_again));
		break;
	case fsm_not_format:
		if (load != 0)
		{
			index = func_880012E8(mcmenu, load, index, localstr_get(LOCALSTR_unformatted_card), localstr_get(LOCALSTR_warning_one));
			break;
		}

		memcard_item(mcmenu->opaque, NULL,          0,          4, localstr_get(LOCALSTR_unformatted_card));
		memcard_item(mcmenu->opaque, func_88001148, fsm_format, 0, localstr_get(LOCALSTR_format_card));
		memcard_item(mcmenu->opaque, func_880010DC, 0,          0, localstr_get(LOCALSTR_go_back));

		index = (index < 0) ? 2 : index;
		break;
	case fsm_format:
		memcard_item(mcmenu->opaque, NULL, 0, 4, localstr_get(LOCALSTR_formatting));
		memcard_item(mcmenu->opaque, NULL, 0, 0, localstr_get(LOCALSTR_do_not_remove_card));

		index = -1;

		if (status == mcard_status_not_format)
		{
			func_88000EF8(mcmenu->mcard);
		}

		if (status == mcard_status_ready)
		{
			mcmenu->state.slot = 0;
			mcmenu->state.fsm  = fsm_create;
		}

		break;
	case fsm_new_card:
		gt->gameFlags |= 0x20000000;

		memcard_item(mcmenu->opaque, NULL, 0, 0, localstr_get(LOCALSTR_accessing_card));

		index = -1;

		if (status == mcard_status_ready)
		{
			mcmenu->state.fsm = fsm_ready;
		}

		break;
	case fsm_ready:
		nused   = 0;
		slots   = 0;
		nblocks = mcmenu->params.nblocks;

		for (i = 0; i < mcmenu->params.nfiles; i++)
		{
			entry = &mcmenu->params.directory[i];
			
			e2 = entry;

			nblocks += (entry->nbytes + 8191) / 8192;

			if (func_88002164(entry->name) != 0)
			{
				if (nused == 0)
				{
					memcard_item(mcmenu->opaque, NULL, 0, 4, localstr_get((load != 0) ? LOCALSTR_choose_load_file : LOCALSTR_choose_save_file));
				}

				memcard_item(mcmenu->opaque, (load != 0) ? func_880011CC : func_8800125C, i, 0, D_8800516C[((unsigned char *)e2->name)[17]]);

				nused++;

				slots |= 1 << func_8800228C(entry->name);
			}
		}

		if (nused > 0)
		{
			if (load != 0)
			{
				memcard_item(mcmenu->opaque, func_88001148, fsm_start,  0, localstr_get(LOCALSTR_start_new_game));
			}
			else if ((nblocks < 16) && (mcmenu->params.nfiles < 15) && (nused < 3))
			{
				memcard_item(mcmenu->opaque, func_88001148, fsm_create, 0, localstr_get(LOCALSTR_create_file));

				index = (index < 0) ? nused + 1 : index;

				for (i = 0; (i < 15) && ((slots >> i) & 1); i++);

				mcmenu->state.slot = i;

				goto go_back;
			}

			index = (index < 0) ? nused + 1 : index;
		}
		else
		{
			if (nblocks == (mcmenu->params.nblocks + 15))
			{
				index = func_880012E8(mcmenu, load, index, localstr_get(LOCALSTR_card_full), localstr_get(LOCALSTR_insert_card_free));
				break;
			}

			if (nblocks >= 16)
			{
				index = func_880012E8(mcmenu, load, index, localstr_get(LOCALSTR_card_not_enough_blocks), localstr_get(LOCALSTR_insert_card_free));
				break;
			}

			if (load != 0)
			{
				mcmenu->state.fsm = fsm_start;

				goto again;
			}

			memcard_item(mcmenu->opaque, NULL,          0,          4, localstr_get(LOCALSTR_choose_save_file));
			memcard_item(mcmenu->opaque, func_88001148, fsm_create, 0, localstr_get(LOCALSTR_create_file));

			index = (index < 0) ? 1 : index;
		}

	go_back:
		memcard_item(mcmenu->opaque, func_880010DC, 0, 0, localstr_get(LOCALSTR_go_back));

		break;
	case fsm_start:
		memcard_item(mcmenu->opaque, NULL, 0, 0, " ");

		memcard_start(mcmenu->opaque);

		index = -1;
		break;
	case fsm_loading:
		memcard_item(mcmenu->opaque, NULL, 0, 4, localstr_get(LOCALSTR_loading));
		memcard_item(mcmenu->opaque, NULL, 0, 0, localstr_get(LOCALSTR_do_not_remove_card));

		index = -1;

		if (status == mcard_status_ready)
		{
			gt->gameFlags &= ~0x20000000;

			memcard_load(mcmenu->opaque);

			break;
		}

		break;
	case fsm_query_overwrite:
		memcard_item(mcmenu->opaque, NULL,          0,             0, D_8800516C[((unsigned char *)mcmenu->params.directory[mcmenu->state.slot].name)[17]]);
		memcard_item(mcmenu->opaque, NULL,          0,             0, localstr_get(LOCALSTR_query_overwrite));
		memcard_item(mcmenu->opaque, func_88001148, fsm_overwrite, 0, localstr_get(LOCALSTR_overwrite_game));
		memcard_item(mcmenu->opaque, func_88001148, fsm_ready,     0, localstr_get(LOCALSTR_go_back));

		index = (index < 0) ? 3 : index;
		break;
	case fsm_overwrite:
		func_880021D0(mcmenu->params.directory[mcmenu->params.nfiles].name, mcmenu->state.slot);

		sprintf(title, "Legacy Of Kain: Soul Reaver %c", mcmenu->state.slot + 'A');

		func_880022DC(mcmenu->params.buffer, title);

		memcard_item(mcmenu->opaque, NULL, 0, 4, localstr_get(LOCALSTR_saving));
		memcard_item(mcmenu->opaque, NULL, 0, 0, localstr_get(LOCALSTR_do_not_remove_card));

		index = -1;

		memcard_save(mcmenu->opaque);

		gt->gameFlags |= 0x20000800;

		func_88000F40(mcmenu->mcard, mcmenu->params.directory[mcmenu->state.slot].name, mcmenu->params.buffer, mcmenu->params.nbytes);

		mcmenu->state.fsm = fsm_saving;
		break;
	case fsm_saving:
		memcard_item(mcmenu->opaque, NULL, 0, 4, localstr_get(LOCALSTR_saving));
		memcard_item(mcmenu->opaque, NULL, 0, 0, localstr_get(LOCALSTR_do_not_remove_card));

		index = -1;

		if (status == mcard_status_ready)
		{
			mcmenu->state.fsm = fsm_save_complete;
		}

		break;
	case fsm_save_complete:
		memcard_item(mcmenu->opaque, NULL,          0, 4, localstr_get(LOCALSTR_save_ok));
		memcard_item(mcmenu->opaque, func_880010DC, 0, 0, localstr_get(LOCALSTR_continue));

		index = 1;

		gt->gameFlags &= ~0x800;
		break;
	case fsm_create:
		entry = &mcmenu->params.directory[mcmenu->params.nfiles];

		func_880021D0(entry->name, mcmenu->state.slot);

		sprintf(title, "Legacy Of Kain: Soul Reaver %c", mcmenu->state.slot + 'A');

		func_880022DC(mcmenu->params.buffer, title);

		index = -1;

		memcard_save(mcmenu->opaque);

		func_88000F10(mcmenu->mcard, entry->name, mcmenu->params.nblocks, mcmenu->params.buffer, mcmenu->params.nbytes);

		memcard_item(mcmenu->opaque, NULL, 0, 4, localstr_get(LOCALSTR_saving));
		memcard_item(mcmenu->opaque, NULL, 0, 0, localstr_get(LOCALSTR_do_not_remove_card));

		gt->gameFlags |= 0x20000800;

		mcmenu->state.fsm = fsm_saving;
		break;
	default:
		gt->gameFlags |= 0x20000000;

		mcmenu->state.fsm = fsm_new_card;

		goto again;
	}

	return index;
}
#endif

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

unsigned short func_88001E14(unsigned char ascii_code)
{
	mcardx_ascii_t *entry;
    int stmp;
	int stmp2;

    stmp  = 0;
    stmp2 = 0;

	if (ascii_code < ' ') 
    {
        return 0;
    }
    else if (ascii_code < '0') 
    {
        stmp2 = 1;
    }
    else if (ascii_code < ':')  
    {
        stmp = 0;
    }
    else if (ascii_code < 'A')  
    {
        stmp2 = 11;
    }
    else if (ascii_code < '[') 
    {
        stmp = 1;
    }
    else if (ascii_code < 'a') 
    {  
        stmp2 = 37;
    }
    else if (ascii_code < '{')  
    {
        stmp = 2;
    }
    else if (ascii_code < 0x7F) 
    {
        stmp2 = 63;
    }
    else
    {
        return 0;
    }

    if (stmp2 != 0) 
    {
        return D_880057E0[(ascii_code - ' ') - (stmp2 - 1)];
    }
    else
    {
        entry = &D_880057D4[stmp];

        return (entry->sjis + ascii_code) - entry->ascii;
    }
}

void func_88001EF8(unsigned char *src, unsigned short *dst)
{
	unsigned char code;
	unsigned short sjis;

loop: // TODO: find the actual loop structure
	code = *src++;

	if (code != 0)
	{
		sjis = func_88001E14(code);

		*dst++ = ((sjis & 0xFF00) >> 8) | ((sjis & 0xFF) << 8);

		goto loop;
	}

	*dst = 0;
}

int func_88001F64(mcardx_header_t *header, unsigned char *title, int nblocks, int nicons, mcardx_clut_t *clut, mcardx_icon_t *icons)
{
	int i;
	int size;

	size = (nicons * 128) + 256;

	memset(header, 0, 256);

	header->magic[0] = 'S';
	header->magic[1] = 'C';
    
	header->type = nicons + 16;

	header->nblocks = nblocks;

	func_88001EF8(title, (unsigned short *)header->title);

	header->clut = *clut;

	for (i = 0; i < nicons; i++)
	{
		header->icon[i] = icons[i];
	}

	return size;
}

void func_8800213C(void *prm0, void *prm1)
{
	func_88001EF8(prm1, (unsigned short *)((char *)prm0 + 4));
}

int func_88002164(char *name)
{
	int i;
	char *id;

    i = 0;

	for (id = D_88005824[0]; i < 5; i++, id += sizeof(D_88005824[0]))
	{
		if (strncmp(id, name, 12) == 0)
		{
			return 1;
		}
	}

	return 0;
}

void func_880021D0(char *name, int slot)
{
	int language;

	language = localstr_get_language();

	memset(name, 0, 20);

	*(mcardx_id_t *)name = *(mcardx_id_t *)D_88005824[language];

	sprintf(&name[12], "kain %c", slot + 'a');
    
	printf("SetName: %s\n", name);
}

int func_8800228C(char *name)
{
	return ((unsigned char *)name)[17] - 'a';
}

int func_88002298(void *buffer, int nblocks)
{
	return func_88001F64(buffer, D_880000F0, nblocks, 1, D_880058E8, D_88005868); 
}

void func_880022DC(void *prm0, void *prm1)
{
	func_8800213C(prm0, prm1);
}
