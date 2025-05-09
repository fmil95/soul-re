#ifndef _MEMPACK_H_
#define _MEMPACK_H_

char *MEMPACK_Malloc(unsigned long allocSize, unsigned char memType);
void MEMPACK_Free(char *address);
long MEMPACK_MemoryValidFunc(char *address);
unsigned long MEMPACK_Size(char *address);
void MEMPACK_DoGarbageCollection();
unsigned long MEMPACK_ReportFreeMemory();
void MEMPACK_Return(char *address, long takeBackSize);
void MEMPACK_SetMemoryDoneStreamed(char *address);
void MEMPACK_SetMemoryBeingStreamed(char *address);
void MEMPACK_RelocateCDMemory(MemHeader *newAddress, long offset, BigFileDir *oldDir);
void MEMPACK_RelocateObjectType(MemHeader *newAddress, long offset, Object *oldObject);
void MEMPACK_RelocateAreaType(MemHeader *newAddress, long offset, Level *oldLevel);
char *MEMPACK_GarbageCollectMalloc(unsigned long *allocSize, unsigned char memType, unsigned long *freeSize);
void MEMPACK_ReportMemory2();
void MEMPACK_Init();
void MEMPACK_FreeByType(unsigned char memType);

#endif
