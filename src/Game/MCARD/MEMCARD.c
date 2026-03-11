#include "Game/MCARD/MEMCARD.h"
#include "Game/SAVEINFO.h"
#include "Game/MENU/MENU.h"
#include "Game/PSX/MAIN.h"
#include "Game/MEMPACK.h"
#include "Game/RELMOD.h"
#include "Game/STRMLOAD.h"
#include "Game/GAMELOOP.h"

STATIC mcmenu gMcmenu;

int MEMCARD_IsWrongVersion(memcard_t *memcard)
{
    int result;

    result = 1;

    if (memcard != NULL)
    {
        result = memcard->wrongVerison;
    }

    return result;
}

void load(memcard_t *memcard)
{
    Object *object;

    if ((gameTrackerX.gameFlags & 0x8000000))
    {
        object = (Object *)&gameTrackerX.primPool->prim;
    }
    else
    {
        object = (Object *)MEMPACK_Malloc(40000, MEMORY_TYPE_MCARD);
    }

    LOAD_LoadToAddress("\\kain2\\object\\mcardx\\mcardx.drm", object, 1);

    memcard->table = (mcmenu_table_t *)object->relocModule;

    RELMOD_InitModulePointers((uintptr_t)object->relocModule, (int *)object->relocList);

    memcard->object = object;

    if (memcard->table->versionID != monVersion)
    {
        if (!(gameTrackerX.gameFlags & 0x8000000))
        {
            MEMPACK_Free((char *)object);
        }

        memcard->table = NULL;
    }
}

void unload(memcard_t *memcard)
{
    if (memcard->object != NULL)
    {
        if ((char *)memcard->object != (char *)gameTrackerX.primPool->prim)
        {
            MEMPACK_Free((char *)memcard->object);
        }

        memcard->object = NULL;
    }

    memcard->table = NULL;
}

int memcard_data_size()
{
    return sizeof(memcard_t);
}

int memcard_initialize(memcard_t *memcard, void *gt, int nblocks, void *buffer, int nbytes)
{
    int header_size;

    (void)gt;

    memset(memcard, 0, sizeof(memcard_t));

    load(memcard);

    header_size = 0;

    memcard->wrongVerison = 0;

    if (memcard->table != NULL)
    {
        memcard->mcmenu = &gMcmenu.dummy1;

        memcard->table->initialize(memcard->mcmenu, memcard, nblocks);

        header_size = memcard->table->set_buffer(memcard->mcmenu, buffer, nbytes);

        unload(memcard);
    }
    else
    {
        memcard->wrongVerison = 1;
    }

    return header_size;
}

void memcard_end(memcard_t *memcard)
{
    memcard->table->end(memcard->mcmenu);

    unload(memcard);

    memcard->running = 0;
}

int maybe_start(memcard_t *memcard)
{
    if (memcard->running == 0)
    {
        if (memcard->object == NULL)
        {
            load(memcard);
        }

        memcard->running = 1;

        memcard->table->begin(memcard->mcmenu);
    }

    return 1;
}

int memcard_main_menu(void *gt, int index)
{
    memcard_t *memcard;

    memcard = ((GameTracker *)gt)->memcard;

    if (maybe_start(memcard) == 0)
    {
        return -1;
    }

    memcard->table->main(memcard->mcmenu, index);
}

int memcard_pause_menu(void *gt, int index)
{
    memcard_t *memcard;

    memcard = ((GameTracker *)gt)->memcard;

    if (maybe_start(memcard) != 0)
    {
        return memcard->table->pause(memcard->mcmenu, index);
    }

    return -1;
}

void *gt2mcmenu(void *gt)
{
    memcard_t *memcard;

    memcard = ((GameTracker *)gt)->memcard;

    return memcard->mcmenu;
}

void memcard_pop(void *opaque)
{
    GameTracker *temp; // not from decls.h

    (void)opaque;

    temp = &gameTrackerX;

    menu_pop(temp->menu);

    memcard_end(temp->memcard);
}

void memcard_start(void *opaque)
{
    (void)opaque;

    gameTrackerX.streamFlags |= 0x1000000;

    MAIN_StartGame();

    memcard_end(gameTrackerX.memcard);
}

void memcard_load(void *opaque)
{
    (void)opaque;

    gameTrackerX.streamFlags |= 0x200000;

    SAVE_RestoreGame();

    MAIN_StartGame();

    memcard_end(gameTrackerX.memcard);
}

void memcard_save(void *opaque)
{
    (void)opaque;

    SAVE_SaveGame();
}

void memcard_item(void *opaque, int (*fn)(), long parameter, long flags, char *text)
{
    (void)opaque;

    if (flags != 0)
    {
        menu_item_flags(gameTrackerX.menu, fn, parameter, flags, text);
    }
    else
    {
        menu_item(gameTrackerX.menu, fn, parameter, text);
    }
}
