#include "Game/CINEMA/CINEPSX.h"
#include "Game/OBTABLE.h"
#include "Game/PSX/MAIN.h"
#include "Game/GAMEPAD.h"
#include "Game/SAVEINFO.h"
#include "Game/LOAD3D.h"
#include "Game/STRMLOAD.h"
#include "Game/STREAM.h"

static cinema_fn_table_t *the_cine_table = NULL;

static ObjectTracker *the_cine_tracker = NULL;

int CINE_CDIntrQuery()
{
    if (StCdIntrFlag != 0)
    {
        StCdIntrFlag = 0;

        return 1;
    }

    return 0;
}

unsigned short CINE_Pad(int pad)
{
    if (pad != 0)
    {
        return readGPBuffer2.data.pad;
    }
    else
    {
        return readGPBuffer1.data.pad;
    }
}

void CINE_Play(char *strfile, unsigned short mask, int buffers)
{
    if (the_cine_table != NULL)
    {
        if (the_cine_table->versionID == monVersion)
        {
            the_cine_table->play(strfile, mask, buffers);

            LOAD_InitCdStreamMode();
        }
        else
        {
            printf("CINEMA : Version number is wrong. Not playing the cinematics.\n");
        }
    }
}

int CINE_Load()
{
    ObjectTracker *tracker;
    int attempts;

    attempts = 0;

    tracker = STREAM_GetObjectTracker("cinemax");

    for (; attempts < 400 && tracker->objectStatus != OBJECTTRACKER_USED; attempts++)
    {
        STREAM_PollLoadQueue();

        VSync(0);
    }

    if (attempts >= 400)
    {
        printf("cinema timeout\n");

        return 0;
    }
    else
    {
        the_cine_tracker = tracker;

        the_cine_table = (cinema_fn_table_t *)tracker->object->relocModule;

        return 1;
    }
}

int CINE_Loaded()
{
    return the_cine_tracker != NULL;
}

void CINE_Unload()
{
    VSyncCallback(VblTick);

    the_cine_table = NULL;

    if (the_cine_tracker != NULL)
    {
        STREAM_DumpObject(the_cine_tracker);

        the_cine_tracker = NULL;
    }
}

void CINE_PlayIngame(int number)
{
    char movie_name[24];

    sprintf(movie_name, "\\CHRONO%d.STR;1", number);

    if (CINE_Load() != 0)
    {
        CINE_Play(movie_name, 0, 2);

        CINE_Unload();
    }
}
