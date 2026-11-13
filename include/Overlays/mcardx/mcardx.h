#ifndef _MCARDX_H_
#define _MCARDX_H_

#include "common.h"

typedef struct mcardx_ascii_t
{
	unsigned short sjis;
	unsigned char  ascii;
} mcardx_ascii_t;

typedef struct mcardx_clut_t
{
	char data[32];
} mcardx_clut_t;

typedef struct mcardx_icon_t
{
	char data[128];
} mcardx_icon_t;

typedef struct mcardx_header_t
{         
	char          magic[2];     
	char          type;        
	char          nblocks;
	char          title[64];    
	char          reserved[28];
	mcardx_clut_t clut;
	mcardx_icon_t icon[3];
} mcardx_header_t;

typedef struct mcardx_id_t
{
	char id[12];
} mcardx_id_t;

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
void func_88000C68(mcard_t *mcard);
void func_88000EF8(mcard_t *mcard);
void func_88000F04(mcard_t *mcard);
void func_88000F40(mcard_t *mcard, char *filename, void *buffer, int nbytes);
void func_88000F58(mcard_t *mcard, char *filename, void *buffer, int nbytes);
int func_88000F70();
void func_88000F90(mcmenu_t *mcmenu, void *opaque, int nblocks);
void func_88001014(mcmenu_t *mcmenu);
void func_88001038(mcmenu_t *mcmenu);
void func_8800107C(mcmenu_t *mcmenu);
int func_880010AC(mcmenu_t *mcmenu, void *buffer, int nbytes);
int func_880010DC(void *opaque, int param, menu_ctrl_t ctrl);
int func_88001148(void *opaque, int param, menu_ctrl_t ctrl);
int func_880011CC(void *opaque, int param, menu_ctrl_t ctrl);
int func_8800125C(void *opaque, int param, menu_ctrl_t ctrl);
int func_880012E8(mcmenu_t *mcmenu, int nfiles, int index, char *title, char *subtitle);
int func_880013DC(mcmenu_t *mcmenu, int index, int arg2);
int func_88001D50(mcmenu_t *mcmenu, int index);
int func_88001DBC(mcmenu_t *mcmenu, int index);
unsigned short func_88001E14(unsigned char ascii_code);
void func_88001EF8(unsigned char *src, unsigned short *dst);
int func_88001F64(mcardx_header_t *header, unsigned char *title, int nblocks, int nicons, mcardx_clut_t *clut, mcardx_icon_t *icons);
void func_8800213C(void *prm0, void *prm1);
int func_88002164(char *name);
void func_880021D0(char *name, int slot);
int func_8800228C(char *name);
int func_88002298(void *buffer, int nblocks);
void func_880022DC(void *prm0, void *prm1);

extern unsigned char D_880000F0[];
extern mcardx_ascii_t D_880057D4[3];
extern unsigned short D_880057E0[];
extern char D_88005824[5][13];
extern mcardx_icon_t D_88005868[];
extern mcardx_clut_t D_880058E8[];

#endif
