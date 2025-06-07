#include "Game/VM.h"
#include "Game/MEMPACK.h"
#include "Game/GAMELOOP.h"

static long vmRealClock = 0;

static long vmClock = 0;

void VM_Tick(long time)
{
    vmRealClock += time;

    vmClock = vmRealClock >> 8;
}

void VM_UpdateMorph(Level *level, int initFlg)
{
    VMObject *vmobject;
    int i;
    VMOffsetTable *curTable;
    short ratio;
    int j;
    long num;
    //long len; unused
    short dr;
    short dg;
    short db;

    vmobject = level->vmobjectList;

    for (i = level->numVMObjects; i != 0; i--, vmobject++)
    {
        if ((vmobject->materialIdx != vmobject->spectralIdx) || ((vmobject->flags & 0x100)))
        {
            curTable = vmobject->curVMOffsetTable;

            if (gameTrackerX.gameData.asmData.MorphTime == 1000)
            {
                if (curTable != vmobject->vmoffsetTableList[vmobject->currentIdx])
                {
                    MEMPACK_Free((char *)curTable);
                }

                if (initFlg != 0)
                {
                    if (gameTrackerX.gameData.asmData.MorphType == 0)
                    {
                        vmobject->currentIdx = vmobject->materialIdx;
                    }
                    else
                    {
                        vmobject->currentIdx = vmobject->spectralIdx;
                    }
                }
                else if (gameTrackerX.gameData.asmData.MorphType != 0)
                {
                    vmobject->currentIdx = vmobject->materialIdx;
                }
                else
                {
                    vmobject->currentIdx = vmobject->spectralIdx;
                }

                vmobject->curVMOffsetTable = vmobject->vmoffsetTableList[vmobject->currentIdx];
            }
            else
            {
                num = curTable->numVMOffsets;

                ratio = gameTrackerX.spectral_fadeValue;

                if (curTable == vmobject->vmoffsetTableList[vmobject->currentIdx])
                {
                    vmobject->curVMOffsetTable = (VMOffsetTable *)MEMPACK_Malloc(((vmobject->flags & 0x8) ? num * 6 : num * 3) + 4, 40);

                    vmobject->curVMOffsetTable->numVMOffsets = num;
                }

                if ((vmobject->flags & 0x8))
                {
                    VMMoveOffset *material;
                    VMMoveOffset *spectral;
                    VMMoveOffset *offset;

                    material = &vmobject->vmoffsetTableList[vmobject->materialIdx]->offsets.moveOffsets;
                    spectral = &vmobject->vmoffsetTableList[vmobject->spectralIdx]->offsets.moveOffsets;

                    offset = &vmobject->curVMOffsetTable->offsets.moveOffsets;

                    for (j = 0; j < num; j++)
                    {
                        offset[j].dx = material[j].dx + (((spectral[j].dx - material[j].dx) * ratio) >> 12);
                        offset[j].dy = material[j].dy + (((spectral[j].dy - material[j].dy) * ratio) >> 12);
                        offset[j].dz = material[j].dz + (((spectral[j].dz - material[j].dz) * ratio) >> 12);
                    }
                }
                else
                {
                    VMColorOffset *material;
                    VMColorOffset *spectral;
                    VMColorOffset *offset;

                    material = &vmobject->vmoffsetTableList[vmobject->materialIdx]->offsets.colorOffsets;
                    spectral = &vmobject->vmoffsetTableList[vmobject->spectralIdx]->offsets.colorOffsets;

                    offset = &vmobject->curVMOffsetTable->offsets.colorOffsets;

                    for (j = 0; j < num; j++)
                    {
                        if ((vmobject->flags & 0x100))
                        {
                            dr = spectral[j].db - material[j].dr;
                            dg = spectral[j].dg - material[j].dg;
                            db = spectral[j].dr - material[j].db;
                        }
                        else
                        {
                            dr = spectral[j].dr - material[j].dr;
                            dg = spectral[j].dg - material[j].dg;
                            db = spectral[j].db - material[j].db;
                        }

                        offset[j].dr = material[j].dr + ((dr * ratio) >> 12);
                        offset[j].dg = material[j].dg + ((dg * ratio) >> 12);
                        offset[j].db = material[j].db + ((db * ratio) >> 12);
                    }
                }
            }
        }
    }
}

void VM_VMObjectSetTable(Level *level, VMObject *vmobject, int table)
{
    VMOffsetTable *curTable;

    (void)level;
    curTable = vmobject->curVMOffsetTable;

    if (curTable != vmobject->vmoffsetTableList[vmobject->currentIdx])
    {
        MEMPACK_Free((char *)curTable);
    }

    vmobject->currentIdx = table;
    vmobject->curVMOffsetTable = (VMOffsetTable *)vmobject->vmoffsetTableList[vmobject->currentIdx];
}
