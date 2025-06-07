#include "Game/SCRIPT2.h"
#include "Game/GAMELOOP.h"

void LitShaftInit(Instance *instance, GameTracker *gameTracker)
{
    LitShaftProperties *Prop;

    (void)gameTracker;
    Prop = (LitShaftProperties *)instance->data;

    if (Prop != NULL)
    {
        int curTimeOfDay;

        curTimeOfDay = GAMELOOP_GetTimeOfDay();

        switch (curTimeOfDay)
        {
        case 1900:
        case 600:
            instance->fadeValue = 4096;
            break;
        case 700:
        case 1800:
            instance->fadeValue = Prop->fadeValue;
            break;
        }
    }
}
