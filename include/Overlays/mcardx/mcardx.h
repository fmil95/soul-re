#ifndef _MCARDX_H_
#define _MCARDX_H_

#include "common.h"

int func_880003B4();
void func_880003BC(mcpsx_t *mcpsx, void *opaque);
void func_88000438(mcpsx_t *mcpsx);
void func_88000464(mcpsx_t *mcpsx);
void func_880004A8(mcpsx_t *mcpsx);
void func_88000464(mcpsx_t *mcpsx);
void func_88000510(mcpsx_t *mcpsx, mcpsx_err_t err);
void func_8800055C(mcpsx_t *mcpsx);
void func_880006B8(mcpsx_t *mcpsx);
int func_88000700(mcpsx_t *mcpsx);
int func_8800076C(mcpsx_t *mcpsx);
void func_88000790(mcpsx_t *mcpsx, mcpsx_func_t func);
int func_880007A0(mcpsx_t *mcpsx);
int func_880007E0(mcpsx_t *mcpsx);
int func_88000820(mcpsx_t *mcpsx, char *filename, void *buffer, long nbytes);
int func_88000890(mcpsx_t *mcpsx, char *filename, void *buffer, long nbytes);
int func_88000900(mcpsx_t *mcpsx, char *filename, long nblocks);
int func_88000960(mcpsx_t *mcpsx, char *filename);
int func_880009B0(mcpsx_t *mcpsx);
int func_880009F0(mcpsx_t *mcpsx);
int func_88000A30(mcpsx_t *mcpsx, mcpsx_directory_t *directory, long *nfilesptr);
long func_88000A90(long channel, mcpsx_directory_t *directory, long *nfilesptr);
void func_88000B58(mcard_t *mcard);
int func_88000B70();
void func_88000B90(mcard_t *mcard, void *opaque, mcpsx_directory_t *directory, long *nfilesptr);
void func_88000C10(mcard_t *mcard);
void func_88000C34(mcard_t *mcard);
int func_880010DC(void *opaque, int param, menu_ctrl_t ctrl);
int func_88001148(void *opaque, int param, menu_ctrl_t ctrl);
int func_8800125C(void *opaque, int param, menu_ctrl_t ctrl);
int func_880013DC(mcmenu_t *mcmenu, int index, int arg2);
int func_88001D50(mcmenu_t *mcmenu, int index);
int func_88001DBC(mcmenu_t *mcmenu, int index);

#endif
