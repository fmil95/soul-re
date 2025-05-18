#ifndef _VM_H_
#define _VM_H_

#include "common.h"

void VM_UpdateMorph(Level *level, int initFlg);
void VM_Tick(long time);
void VM_VMObjectSetTable(Level *level, VMObject *vmobject, int table);

#endif
