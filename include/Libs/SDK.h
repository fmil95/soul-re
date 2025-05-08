#ifndef _SDK_H_
#define _SDK_H_

#include "common.h"
/*TODO: Remove LIBS/ when adding PSYQ headers*/

#define PadStateDiscon 0
#define PadStateFindPad 1
#define PadStateFindCTP1 2
#define PadStateFindCTP2 3
#define PadStateReqInfo 4
#define PadStateExecCmd 5
#define PadStateStable 6

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

#define getScratchAddr(offset) ((unsigned long *)(0x1f800000 + (offset) * 4))

#define getTPage(tp, abr, x, y)                                                                 \
    ((((tp) & 0x3) << 7) | (((abr) & 0x3) << 5) | (((y) & 0x100) >> 4) | (((x) & 0x3ff) >> 6) | \
     (((y) & 0x200) << 2))

#define getClut(x, y) \
    (((y) << 6) | (((x) >> 4) & 0x3f))

// GTE macros

#define gte_SetRotMatrix(r0) __asm__( \
    "lw    $12, 0(%0);"               \
    "lw    $13, 4(%0);"               \
    "ctc2    $12, $0;"                \
    "ctc2    $13, $1;"                \
    "lw    $12, 8(%0);"               \
    "lw    $13, 12(%0);"              \
    "lw    $14, 16(%0);"              \
    "ctc2    $12, $2;"                \
    "ctc2    $13, $3;"                \
    "ctc2    $14, $4"                 \
    : : "r"(r0)                       \
    : "$12", "$13", "$14")

#define gte_SetTransMatrix(r0) __asm__( \
    "lw    $12, 20(%0);"                \
    "lw    $13, 24(%0);"                \
    "ctc2    $12, $5;"                  \
    "lw    $14, 28(%0);"                \
    "ctc2    $13, $6;"                  \
    "ctc2    $14, $7"                   \
    : : "r"(r0)                         \
    : "$12", "$13", "$14")

#define gte_ldv0(r0) __asm__( \
    "lwc2    $0, 0(%0);"      \
    "lwc2    $1, 4(%0)"       \
    : : "r"(r0))

#define gte_nmvmva(sf, mx, v, cv, lm) __asm__( \
    "nop;"                                     \
    "nop;"                                     \
    ".word %0"                                 \
    : : "g"(0x4A400012 | ((sf) & 0x1) << 19 | ((mx) & 0x3) << 17 | ((v) & 0x3) << 15 | ((cv) & 0x3) << 13 | ((lm) & 0x1) << 10))

#define gte_nrtv0tr() gte_nmvmva(1, 0, 0, 0, 0)

#define gte_mvmva(sf, mx, v, cv, lm) __asm__( \
    ".word %0"                                \
    : : "g"(0x4A400012 | ((sf) & 0x1) << 19 | ((mx) & 0x3) << 17 | ((v) & 0x3) << 15 | ((cv) & 0x3) << 13 | ((lm) & 0x1) << 10))

#define gte_stlvnl(r0) __asm__( \
    "swc2    $25, 0(%0);"       \
    "swc2    $26, 4(%0);"       \
    "swc2    $27, 8(%0)"        \
    : : "r"(r0)                 \
    : "memory")

#define gte_nrtv0() gte_nmvmva(1, 0, 0, 3, 0)

#define gte_ldlvl(r0) __asm__( \
    "lwc2    $9, 0(%0);"       \
    "lwc2    $10, 4(%0);"      \
    "lwc2    $11, 8(%0)"       \
    : : "r"(r0))

#define gte_nsqr(sf) __asm__( \
    "nop;"                    \
    "nop;"                    \
    ".word %0"                \
    : : "g"(0x4AA00428 | ((sf) & 0x1) << 19))

#define gte_nsqr0() gte_nsqr(0)

#define gte_sqr(sf) __asm__( \
    ".word %0"               \
    : : "g"(0x4AA00428 | ((sf) & 0x1) << 19))

#define gte_sqr0() gte_sqr(0)

#define gte_nrtps() __asm__( \
    "nop;"                   \
    "nop;"                   \
    ".word 0x4A180001")

#define gte_stsxy(r0) __asm__( \
    "swc2    $14, 0(%0)"       \
    : : "r"(r0)                \
    : "memory")

#define gte_ldsv(r0) __asm__( \
    "lhu    $12, 0(%0);"      \
    "lhu    $13, 2(%0);"      \
    "lhu    $14, 4(%0);"      \
    "mtc2    $12, $9;"        \
    "mtc2    $13, $10;"       \
    "mtc2    $14, $11"        \
    : : "r"(r0)               \
    : "$12", "$13", "$14")

#define gte_lddp(r0) __asm__( \
    "mtc2    %0, $8"          \
    : : "r"(r0))

#define gte_gpl(sf) __asm__( \
    ".word %0"               \
    : : "g"(0x4BA0003E | ((sf) & 0x1) << 19))

#define gte_gpl12() gte_gpl(1)

#define gte_ldsvrtrow0(r0) __asm__( \
    "lw    $12, 0(%0);"             \
    "lw    $13, 4(%0);"             \
    "ctc2    $12, $0;"              \
    "ctc2    $13, $1"               \
    : : "r"(r0)                     \
    : "$12", "$13")

#define gte_nGPL(sf) __asm__( \
    "nop;"                    \
    "nop;"                    \
    ".word %0"                \
    : : "g"(0x4BA0003E | ((sf) & 0x1) << 19))

#define gte_ngpl12() gte_nGPL(1)

#define gte_stsv(r0) __asm__( \
    "mfc2    $12, $9;"        \
    "mfc2    $13, $10;"       \
    "mfc2    $14, $11;"       \
    "sh    $12, 0(%0);"       \
    "sh    $13, 2(%0);"       \
    "sh    $14, 4(%0)"        \
    : : "r"(r0)               \
    : "$12", "$13", "$14", "memory")

#define gte_stsz(r0) __asm__( \
    "swc2    $19, 0(%0)"      \
    : : "r"(r0)               \
    : "memory")

#define gte_stdp(r0) __asm__( \
    "swc2    $8, 0(%0)"       \
    : : "r"(r0)               \
    : "memory")

// custom macro
#define gte_ldlvnlsv(r0) __asm__ volatile( \
    "lhu	$12, 0( %0 );"                    \
    "lhu	$13, 2 ( %0 );"                   \
    "lhu	$14, 4 ( %0 );"                   \
    "mtc2	$12, $25;"                       \
    "mtc2	$13, $26;"                       \
    "mtc2	$14, $27"                        \
    :                                      \
    : "r"(r0)                              \
    : "$12", "$13", "$14")

// custom macro
#define gte_stlvnlsv(r0) __asm__ volatile( \
    "mfc2	$12, $25;"                       \
    "mfc2	$13, $26;"                       \
    "mfc2	$14, $27;"                       \
    "sh	$12, 0( %0 );"                     \
    "sh	$13, 2( %0 );"                     \
    "sh	$14, 4( %0 )"                      \
    :                                      \
    : "r"(r0)                              \
    : "$12", "$13", "$14", "memory")

// custom macro
#define gte_ldv2_ext(r0) __asm__ volatile( \
    "lw		$12, 0( %0 );"                    \
    "lw		$13, 4( %0 );"                    \
    "ctc2	$12, $3;"                        \
    "ctc2	$13, $4;"                        \
    :                                      \
    : "r"(r0)                              \
    : "$12", "$13")

// custom macro
#define gte_nrtvx() gte_nmvmva(1, 0, 0, 0, 0)

// custom macro
#define gte_ldv0sv(r0) __asm__ volatile( \
    "lhu	$12, 0( %0 );"                  \
    "lhu	$13, 2( %0 );"                  \
    "lhu	$14, 4( %0 );"                  \
    "or     $12, $13;"                   \
    "ctc2	$12, $0;"                      \
    "ctc2	$14, $1;"                      \
    :                                    \
    : "r"(r0)                            \
    : "$12", "$13", "$14")

#define gte_rtir12() gte_mvmva(1, 0, 3, 3, 0)

#define gte_nrtir12() gte_nmvmva(1, 0, 3, 3, 0)

#define gte_stlvnl0(r0) __asm__( \
    "swc2    $25, 0(%0)"         \
    : : "r"(r0)                  \
    : "memory")

#define gte_gpf(sf) __asm__( \
    ".word %0"               \
    : : "g"(0x4B90003D | ((sf) & 0x1) << 19))

#define gte_gpf12() gte_gpf(1)

#define gte_stlvl(r0) __asm__( \
    "swc2    $9, 0(%0);"       \
    "swc2    $10, 4(%0);"      \
    "swc2    $11, 8(%0)"       \
    : : "r"(r0)                \
    : "memory")

#define gte_ngpf(sf) __asm__( \
    "nop;"                    \
    "nop;"                    \
    ".word %0"                \
    : : "g"(0x4B90003D | ((sf) & 0x1) << 19))

#define gte_ngpf12() gte_ngpf(1)

#define gte_ldv3(r0, r1, r2) __asm__( \
    "lwc2    $0, 0(%0);"              \
    "lwc2    $1, 4(%0);"              \
    "lwc2    $2, 0(%1);"              \
    "lwc2    $3, 4(%1);"              \
    "lwc2    $4, 0(%2);"              \
    "lwc2    $5, 4(%2)"               \
    : : "r"(r0), "r"(r1), "r"(r2))

#define gte_nrtpt() __asm__( \
    "nop;"                   \
    "nop;"                   \
    ".word 0x4A280030")

#define gte_stsz3(r0, r1, r2) __asm__( \
    "swc2    $17, 0(%0);"              \
    "swc2    $18, 0(%1);"              \
    "swc2    $19, 0(%2)"               \
    : : "r"(r0), "r"(r1), "r"(r2)      \
    : "memory")

#define gte_stszotz(r0) __asm__( \
    "mfc2    $12, $19;"          \
    "nop;"                       \
    "sra    $12, $12, 2;"        \
    "sw    $12, 0(%0)"           \
    : : "r"(r0)                  \
    : "$12", "memory")

#define gte_stsxy3(r0, r1, r2) __asm__( \
    "swc2    $12, 0(%0);"               \
    "swc2    $13, 0(%1);"               \
    "swc2    $14, 0(%2)"                \
    : : "r"(r0), "r"(r1), "r"(r2)       \
    : "memory")

#define gte_ldcv(r0) __asm__( \
    "lbu    $12, 0(%0);"      \
    "lbu    $13, 1(%0);"      \
    "lbu    $14, 2(%0);"      \
    "mtc2    $12, $9;"        \
    "mtc2    $13, $10;"       \
    "mtc2    $14, $11"        \
    : : "r"(r0)               \
    : "$12", "$13", "$14")

#define gte_stcv(r0) __asm__( \
    "mfc2    $12, $9;"        \
    "mfc2    $13, $10;"       \
    "mfc2    $14, $11;"       \
    "sb    $12, 0(%0);"       \
    "sb    $13, 1(%0);"       \
    "sb    $14, 2(%0)"        \
    : : "r"(r0)               \
    : "$12", "$13", "$14", "memory")

#define gte_stsxy0(r0) __asm__( \
    "swc2    $12, 0(%0)"        \
    : : "r"(r0)                 \
    : "memory")

#define gte_stsxy1(r0) __asm__( \
    "swc2    $13, 0(%0)"        \
    : : "r"(r0)                 \
    : "memory")

#define gte_stopz(r0) __asm__( \
    "swc2    $24, 0(%0)"       \
    : : "r"(r0)                \
    : "memory")

// custom macro
#define gte_lddqb(r0) __asm__ volatile( \
    "ctc2	%0, $28"                      \
    :                                   \
    : "r"(r0))

// custom macro
#define gte_lddqa(r0) __asm__ volatile( \
    "ctc2	%0, $27"                      \
    :                                   \
    : "r"(r0))

#define gte_ldclmv(r0) __asm__ ( \
        "lhu    $12, 0(%0);" \
        "lhu    $13, 6(%0);" \
        "lhu    $14, 12(%0);" \
        "mtc2    $12, $9;" \
        "mtc2    $13, $10;" \
        "mtc2    $14, $11" \
        : : "r"(r0) \
        : "$12", "$13", "$14" \
    )

#define gte_stclmv(r0) __asm__ ( \
        "mfc2    $12, $9;" \
        "mfc2    $13, $10;" \
        "mfc2    $14, $11;" \
        "sh    $12, 0(%0);" \
        "sh    $13, 6(%0);" \
        "sh    $14, 12(%0)" \
        : : "r"(r0) \
        : "$12", "$13", "$14", "memory" \
    )

#define gte_ldlv0(r0) __asm__ ( \
        "lhu    $13, 4(%0);" \
        "lhu    $12, 0(%0);" \
        "sll    $13, $13, 16;" \
        "or    $12, $12, $13;" \
        "mtc2    $12, $0;" \
        "lwc2    $1, 8(%0)" \
        : : "r"(r0) \
        : "$12", "$13" \
    )

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

#define setDrawTPage(p, dfe, dtd, tpage) \
    setlen(p, 1),                        \
        ((u_long *)(p))[1] = _get_mode(dfe, dtd, tpage)

/*
 * Set Primitive Colors
 */
#define setRGB0(p, _r0, _g0, _b0) \
    (p)->r0 = _r0, (p)->g0 = _g0, (p)->b0 = _b0

#define setRGB1(p, _r1, _g1, _b1) \
    (p)->r1 = _r1, (p)->g1 = _g1, (p)->b1 = _b1

#define setRGB2(p, _r2, _g2, _b2) \
    (p)->r2 = _r2, (p)->g2 = _g2, (p)->b2 = _b2

#define setRGB3(p, _r3, _g3, _b3) \
    (p)->r3 = _r3, (p)->g3 = _g3, (p)->b3 = _b3

 /*
  * Set Primitive Screen Points
  */
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

#define setPolyF4(p) setlen(p, 5), setcode(p, 0x28)
#define setPolyG4(p) setlen(p, 8), setcode(p, 0x38)

typedef void (*CdlCB)(u_char, u_char *);

/*
 * CD-ROM Primitive Commands
 */
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

 /*
  * CD-ROM Mode (used int CdlSetmode)
  */
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

void SpuSetCommonCDMix(long cd_mix);
void SpuSetCommonMasterVolume(short mvol_left, short mvol_right);
long EnableEvent(long);
long OpenEvent(unsigned long, long, long, long (*func)());
long SetRCnt(unsigned long, unsigned short, long);
long StartRCnt(unsigned long);
int rand();
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
int checkagain();
void PopMatrix();
void SetTransMatrix(MATRIX *m);
void SetRotMatrix(MATRIX *m);
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

extern int StCdIntrFlag;

#endif
