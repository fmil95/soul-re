#ifndef _SDK_H_
#define _SDK_H_

#include "common.h"
/*TODO: Remove LIBS/ when adding PSYQ headers*/

#ifndef USE_PSYQ
#define PadStateDiscon 0
#define PadStateFindPad 1
#define PadStateFindCTP1 2
#define PadStateFindCTP2 3
#define PadStateReqInfo 4
#define PadStateExecCmd 5
#define PadStateStable 6
#endif

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef ABS
#define ABS(x) (((x) >= 0) ? (x) : (-(x)))
#endif

#ifndef kabs
#define kabs(a) ((a) > 0 ? (a) : -(a))
#endif

#define _break(CODE) __asm__("break " #CODE)

#ifndef USE_PSYQ
#define getScratchAddr(offset) ((unsigned long *)(0x1f800000 + (offset) * 4))

#define getTPage(tp, abr, x, y)                                                                 \
    ((((tp) & 0x3) << 7) | (((abr) & 0x3) << 5) | (((y) & 0x100) >> 4) | (((x) & 0x3ff) >> 6) | \
     (((y) & 0x200) << 2))

#define getClut(x, y) \
    (((y) << 6) | (((x) >> 4) & 0x3f))
#endif

// custom struct
typedef struct _LINE_SG2
{
    u_long tag;
    u_long drawTPage1;
    // BASIC_LINE_G2
    u_char r0, g0, b0, code;
    short x0, y0;
    u_char r1, g1, b1, p1;
    short x1, y1;
} LINE_SG2;

#ifndef USE_PSYQ
typedef struct
{
    u_long tag;
    u_char r0, g0, b0, code;
    short x0, y0;
    short x1, y1;
    short x2, y2;
} POLY_F3; /* Flat Triangle */

#define setcode(p, _code) (((P_TAG *)(p))->code = (u_char)(_code))

#define getcode(p) (u_char)(((P_TAG *)(p))->code)

#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (uintptr_t)(_addr))

#define nextPrim(p) (void *)((((P_TAG *)(p))->addr) | (intptr_t)0x80000000)

#define setlen(p, _len) (((P_TAG *)(p))->len = (u_char)(_len))

#define _get_mode(dfe, dtd, tpage)         \
    ((0xe1000000) | ((dtd) ? 0x0200 : 0) | \
     ((dfe) ? 0x0400 : 0) | ((tpage) & 0x9ff))

#define getaddr(p) (u_long)(((P_TAG *)(p))->addr)

#define getlen(p) (u_char)(((P_TAG *)(p))->len)

#define isendprim(p) ((((P_TAG *)(p))->addr) == 0xFFFFFF)

#define termPrim(p) setaddr(p, 0xFFFFFF)

#define setDrawTPage(p, dfe, dtd, tpage) \
    setlen(p, 1),                        \
        ((u_long *)(p))[1] = _get_mode(dfe, dtd, tpage)
#endif

/*
 * Set Primitive Colors
 */
#ifndef USE_PSYQ
#define setRGB0(p, _r0, _g0, _b0) \
    (p)->r0 = _r0, (p)->g0 = _g0, (p)->b0 = _b0

#define setRGB1(p, _r1, _g1, _b1) \
    (p)->r1 = _r1, (p)->g1 = _g1, (p)->b1 = _b1

#define setRGB2(p, _r2, _g2, _b2) \
    (p)->r2 = _r2, (p)->g2 = _g2, (p)->b2 = _b2

#define setRGB3(p, _r3, _g3, _b3) \
    (p)->r3 = _r3, (p)->g3 = _g3, (p)->b3 = _b3
#endif

 /*
  * Set Primitive Screen Points
  */
#ifndef USE_PSYQ
#define setXY0(p, _x0, _y0) \
    (p)->x0 = (_x0), (p)->y0 = (_y0)

#define setXY2(p, _x0, _y0, _x1, _y1) \
    (p)->x0 = (_x0), (p)->y0 = (_y0), \
    (p)->x1 = (_x1), (p)->y1 = (_y1)

#define setXY3(p, _x0, _y0, _x1, _y1, _x2, _y2) \
    (p)->x0 = (_x0), (p)->y0 = (_y0),           \
    (p)->x1 = (_x1), (p)->y1 = (_y1),           \
    (p)->x2 = (_x2), (p)->y2 = (_y2)

#define setXY4(p, _x0, _y0, _x1, _y1, _x2, _y2, _x3, _y3) \
    (p)->x0 = (_x0), (p)->y0 = (_y0),                     \
    (p)->x1 = (_x1), (p)->y1 = (_y1),                     \
    (p)->x2 = (_x2), (p)->y2 = (_y2),                     \
    (p)->x3 = (_x3), (p)->y3 = (_y3)

#define setXYWH(p, _x0, _y0, _w, _h)         \
    (p)->x0 = (_x0), (p)->y0 = (_y0),        \
    (p)->x1 = (_x0) + (_w), (p)->y1 = (_y0), \
    (p)->x2 = (_x0), (p)->y2 = (_y0) + (_h), \
    (p)->x3 = (_x0) + (_w), (p)->y3 = (_y0) + (_h)

#define setWH(p, _w, _h) \
    (p)->w = _w, (p)->h = _h

#define setSemiTrans(p, abe) \
	((abe)?setcode(p, getcode(p)|0x02):setcode(p, getcode(p)&~0x02))
    
#define setPolyGT3(p)	setlen(p, 9),  setcode(p, 0x34)
#define setPolyF4(p) setlen(p, 5), setcode(p, 0x28)
#define setPolyG4(p) setlen(p, 8), setcode(p, 0x38)
#endif

  // custom macro
#define setAbr(p, abr) (p)->dr_tpage |= abr << 5

#ifndef USE_PSYQ
typedef void (*CdlCB)(u_char, u_char *);
#endif

/*
 * CD-ROM Primitive Commands
 */
#ifndef USE_PSYQ
#define CdlNop		0x01
#define CdlSetloc	0x02
#define CdlPlay		0x03
#define CdlForward	0x04
#define CdlBackward	0x05
#define CdlReadN	0x06
#define CdlStandby	0x07
#define CdlStop		0x08
#define CdlPause	0x09
#define CdlMute		0x0b
#define CdlDemute	0x0c
#define CdlSetfilter	0x0d
#define CdlSetmode	0x0e
#define CdlGetparam	0x0f
#define CdlGetlocL	0x10
#define CdlGetlocP	0x11
#define CdlGetTN	0x13
#define CdlGetTD	0x14
#define CdlSeekL	0x15
#define CdlSeekP	0x16
#define CdlReadS	0x1B
#endif

 /*
  * CD-ROM Mode (used int CdlSetmode)
  */
#ifndef USE_PSYQ
#define CdlModeStream  0x100    /* Normal Streaming                     */
#define CdlModeStream2 0x120    /* SUB HEADER information includes      */
#define CdlModeSpeed	0x80	/* 0: normal speed	1: double speed	*/
#define CdlModeRT	0x40	/* 0: ADPCM off		1: ADPCM on	*/
#define CdlModeSize1	0x20	/* 0: 2048 byte		1: 2340byte	*/
#define CdlModeSize0	0x10	/* 0: -			1: 2328byte	*/
#define CdlModeSF	0x08	/* 0: Channel off	1: Channel on	*/
#define CdlModeRept  	0x04	/* 0: Report off	1: Report on	*/
#define CdlModeAP	0x02	/* 0: AutoPause off	1: AutoPause on */
#define CdlModeDA	0x01	/* 0: CD-DA off		1: CD-DA on	*/ 
#endif

  /*
   * Interrupts
   */
#ifndef USE_PSYQ
#define CdlNoIntr	0x00	/* No interrupt */
#define CdlDataReady	0x01	/* Data Ready */
#define CdlComplete	0x02	/* Command Complete */
#define CdlAcknowledge	0x03	/* Acknowledge (reserved) */
#define CdlDataEnd	0x04	/* End of Data Detected */
#define CdlDiskError	0x05	/* Error Detected */
#endif

#define RING_SIZE 23

/*
 * Location
 */
typedef struct {
	u_char minute;		/* minute (BCD) */
	u_char second;		/* second (BCD) */
	u_char sector;		/* sector (BCD) */
	u_char track;		/* track (void) */
} DslLOC;

/*
 * Streaming Structures
 */
#ifndef USE_PSYQ
typedef struct {
	u_short id;
	u_short type;
	u_short secCount;
	u_short nSectors;
	u_long frameCount;
	u_long frameSize;

	u_short width;
	u_short height;
	u_long dummy1;
	u_long dummy2;
	DslLOC loc;
} StHEADER;		/* CD-ROM STR structure */
#endif

void MemCardStart(void);   
void MemCardStop(void);
long MemCardSync( long mode, long* cmds, long* rslt ); 
#ifndef USE_PSYQ
u_long	StGetNext(u_long **addr,u_long **header);
u_long	StFreeRing(u_long *base);
void StSetRing( u_long* ring_addr, u_long ring_size );
void StSetStream( u_long mode, u_long start_frame, u_long end_frame, void ( *func1 )(), void ( *func2 )() );
#endif
int DecDCTvlc(u_long *bs, u_long *buf);
void DecDCTReset(int mode);
void DecDCTin(u_long *buf, int mode);
void DecDCTout(u_long *buf, int size);
int DecDCToutCallback(void (*func)());
#ifndef USE_PSYQ
void SpuSetCommonCDMix(long cd_mix);
void SpuSetCommonMasterVolume(short mvol_left, short mvol_right);
long EnableEvent(long);
long OpenEvent(unsigned long, long, long, long (*func)());
long SetRCnt(unsigned long, unsigned short, long);
long StartRCnt(unsigned long);
#endif
int rand();
#ifndef USE_PSYQ
void ApplyMatrix(MATRIX *, SVECTOR *, VECTOR *);
void ApplyMatrixSV(MATRIX *, SVECTOR *, SVECTOR *);
void ApplyMatrixLV(MATRIX *, VECTOR *, VECTOR *);
void TransposeMatrix(MATRIX *, MATRIX *);
void EnterCriticalSection();
void ExitCriticalSection();
int GetRCnt(int);
u_long *ClearOTagR(u_long *ot, int n);
MATRIX *RotMatrix(SVECTOR *, MATRIX *);
long ratan2(long y, long x);
MATRIX *RotMatrixX(long r, MATRIX *m);
MATRIX *RotMatrixY(long r, MATRIX *m);
MATRIX *RotMatrixZ(long r, MATRIX *m);
MATRIX *RotMatrixZYX(SVECTOR *r, MATRIX *m);
MATRIX *MulMatrix0(MATRIX *m0, MATRIX *m1, MATRIX *m2);
MATRIX *MulMatrix2(MATRIX *m0, MATRIX *m1);
void SetBackColor(long rbk, long gbk, long bbk);
void SetGeomScreen(long h);
void SetFogNearFar(long a, long b, long h);
#endif
int checkagain();
#ifndef USE_PSYQ
void PopMatrix();
void SetTransMatrix(MATRIX *m);
void SetRotMatrix(MATRIX *m);
u_short GetTPage(int tp, int abr, int x, int y);
void PushMatrix();
MATRIX *CompMatrix(MATRIX *m0, MATRIX *m1, MATRIX *m2);
MATRIX *TransMatrix(MATRIX *m, VECTOR *v);
int rcos(int);
int rsin(int);
void ScaleMatrix(MATRIX *, VECTOR *);
DRAWENV *SetDefDrawEnv(DRAWENV *env, int x, int y, int w, int h);
DISPENV *SetDefDispEnv(DISPENV *env, int x, int y, int w, int h);
u_long *BreakDraw(void);
int DrawSync(int mode);
int LoadImage(RECT *rect, u_long *p);
void DrawOTag(u_long *p);
int MoveImage(RECT *rect, int x, int y);
void SetDrawStp(DR_STP *p, int pbw);
void DrawPrim(void *p);
DRAWENV *PutDrawEnv(DRAWENV *env);
int VSyncCallback(void (*f)(void));
CdlCB CdSyncCallback(CdlCB func);
int CdPosToInt(CdlLOC *p);
CdlFILE *CdSearchFile(CdlFILE *fp, char *name);
int CdControl(u_char com, u_char *param, u_char *result);
int CdControlB(u_char com, u_char *param, u_char *result);
CdlCB CdSyncCallback(CdlCB func);
void SpuSetCommonAttr(SpuCommonAttr *attr);
int VSync(int mode);
CdlLOC *CdIntToPos(int i, CdlLOC *p);
void RotTrans(SVECTOR *v0, VECTOR *v1, long *flag);
u_long DrawSyncCallback(void (*func)());
DISPENV *PutDispEnv(DISPENV *env);
void LoadAverageShort12(SVECTOR *v0, SVECTOR *v1, long p0, long p1, SVECTOR *v2);
void SetDrawArea(DR_AREA *p, RECT *r);
void SetLightMatrix(MATRIX *m);
void LoadAverageCol(u_char *v0, u_char *v1, long p0, long p1, u_char *v2);
long StopRCnt(unsigned long);
long DisableEvent(long);
long CloseEvent(long);
void SpuSetKey(long on_off, unsigned long voice_bit);
long SpuClearReverbWorkArea(long mode);
void SpuQuit(void);
void SpuSetVoicePitch(int vNum, unsigned short pitch);
void SpuGetVoicePitch(int vNum, unsigned short *pitch);
void SpuSetVoiceStartAddr(int vNum, unsigned long startAddr);
void SpuSetVoiceVolume(int vNum, short volL, short volR);
void SpuSetVoiceADSRAttr(int vNum, unsigned short AR, unsigned short DR, unsigned short SR, unsigned short RR, unsigned short SL, long ARmode, long SRmode, long RRmode);
int PadGetState(int);
void PadInitDirect(unsigned char *, unsigned char *);
void PadStartCom(void);
void PadSetAct(int, unsigned char *, int);
void SetFarColor(long rfc, long gfc, long bfc);
int PadInfoMode(int, int, int);
int PadSetActAlign(int, unsigned char *);
int ResetCallback(void);
int CdSetDebug(int level);
int CdInit(void);
CdlCB CdReadyCallback(CdlCB func);
CdlCB CdSyncCallback(CdlCB func);
int CdControl(u_char com, u_char *param, u_char *result);
int CdRead2(long mode);
void SetDispMask(int mask);
void InitGeom();
void SetGeomOffset(long ofx, long ofy);
int ClearImage(RECT *rect, u_char r, u_char g, u_char b);
int ResetGraph(int mode);
int SetGraphDebug(int level);
unsigned long SpuSetReverbVoice(long on_off, unsigned long voice_bit);
void SpuGetAllKeysStatus(char *status);
int CdReset(int mode);
int CdControlF(u_char com, u_char *param);
int CdSync(int mode, u_char *result);
int CdGetSector(void *madr, int size);
int StopCallback(void);
void PadStopCom(void);
void SetPolyFT4(POLY_FT4 *p);
void SetColorMatrix(MATRIX *m);
long SpuSetReverbModeType(long mode);
void SpuSetReverbModeDepth(short depth_left, short depth_right);
long SpuSetReverb(long on_off);
SpuTransferCallbackProc SpuSetTransferCallback(SpuTransferCallbackProc func);
long SpuIsTransferCompleted(long flag);
unsigned long SpuSetTransferStartAddr(unsigned long addr);
unsigned long SpuRead(unsigned char *addr, unsigned long size);
unsigned long SpuWrite(unsigned char *addr, unsigned long size);
void SpuInit (void);
#endif

extern int StCdIntrFlag;

#endif
