#include "Game/RAZCNTRL.h"

long RazielCommands[10] = {0x8000, 0x40, 0x20, 0x80, 0x400, 0x800, 0x100, 0x200, 0xC00, 0x10};

static int Pending = 0;

static int Up = 0;

static int Down = 0;

void ProcessRazControl(long *command)
{
    if ((*command & RazielCommands[2]))
    {
        Up = 0;

        Down++;
    }
    else
    {
        Up++;

        if (Down != 0)
        {
            Pending = Down;
        }

        Down = 0;
    }

    if (Down >= 6)
    {
        Pending = Down;
    }

    if (Up >= 3)
    {
        Pending = 0;
    }
}
