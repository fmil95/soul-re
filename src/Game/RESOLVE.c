#include "Game/RESOLVE.h"

void RESOLVE_Pointers(RedirectList *redirectList, long *data, long *baseAddr)
{
    long *rdList;
    int i;
    long *handle;

    rdList = redirectList->data;

    for (i = redirectList->numPointers; i != 0; i--, rdList++)
    {
        handle = (long *)((char *)data + *rdList);

        *handle += (long)((char *)baseAddr);
    }
}
