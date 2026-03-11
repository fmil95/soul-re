#include "Game/G2/POOLMMG2.h"
#include "Game/MEMPACK.h"

void G2PoolMem_InitPool(void *voidPool, int blockCount, int blockSize)
{
    ((G2PoolMemPool *)voidPool)->blockPool = (G2PoolMem *)MEMPACK_Malloc(blockCount * blockSize, MEMORY_TYPE_G2ANIMATION);

    ((G2PoolMemPool *)voidPool)->stack = (unsigned short *)MEMPACK_Malloc(blockCount * 2, MEMORY_TYPE_G2ANIMATION);

    ((G2PoolMemPool *)voidPool)->blockSize = blockSize;
    ((G2PoolMemPool *)voidPool)->stackSize = blockCount;

    G2PoolMem_ResetPool(voidPool);
}

void G2PoolMem_ResetPool(void *voidPool)
{
    G2PoolMemPool *pool;
    int blockIndex;

    pool = (G2PoolMemPool *)voidPool;

    pool->stackTop = 0;

    for (blockIndex = 0; blockIndex < pool->stackSize; blockIndex++)
    {
        pool->stack[blockIndex] = blockIndex;
    }
}

void *G2PoolMem_Allocate(void *voidPool)
{
    int blockIndex;

    if (((G2PoolMemPool *)voidPool)->stackTop >= ((G2PoolMemPool *)voidPool)->stackSize)
    {
        return NULL;
    }

    blockIndex = ((G2PoolMemPool *)voidPool)->stack[((G2PoolMemPool *)voidPool)->stackTop];

    ((G2PoolMemPool *)voidPool)->stackTop++;

    return (char *)((G2PoolMemPool *)voidPool)->blockPool + (((G2PoolMemPool *)voidPool)->blockSize * blockIndex);
}

void G2PoolMem_Free(void *voidPool, void *block)
{
    int blockIndex;

    blockIndex = (((char *)block) - ((char *)((G2PoolMemPool *)voidPool)->blockPool)) / ((G2PoolMemPool *)voidPool)->blockSize;

    ((G2PoolMemPool *)voidPool)->stack[--((G2PoolMemPool *)voidPool)->stackTop] = blockIndex;
}
