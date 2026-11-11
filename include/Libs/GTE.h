#ifndef _GTE_H_
#define _GTE_H_

/*
 * GTE (COP2) macros.
 *
 * This is an extension of the gte_macros header from rabbitizer (MIT), which emits the COP2 encodings directly instead of parsing them through DMPSX.
 * This header makes some modifications on top of it, such as adding lowercase names for certain macros, and adding macros that aren't present there.  
 *
 */
#pragma GCC system_header
#include "Libs/gte_macros.h"

/* Psy-Q style names for gte_macros.h opcodes */
#define gte_nrtps()                   gte_nRTPS()
#define gte_nrtpt()                   gte_nRTPT()
#define gte_nclip()                   gte_nNCLIP()
#define gte_navsz3()                  gte_nAVSZ3()
#define gte_ndpcs()                   gte_nDPCS()
#define gte_nnccs()                   gte_nNCCS()
#define gte_nncds()                   gte_nNCDS()
#define gte_gpf(sf)                   gte_GPF(sf)
#define gte_ngpf(sf)                  gte_nGPF(sf)
#define gte_gpl(sf)                   gte_GPL(sf)
#define gte_ngpl(sf)                  gte_nGPL(sf)
#define gte_sqr(sf)                   gte_SQR(sf)
#define gte_nsqr(sf)                  gte_nSQR(sf)
#define gte_sqr0()                    gte_SQR(0)
#define gte_nsqr0()                   gte_nSQR(0)
#define gte_mvmva(sf, mx, v, cv, lm)  gte_MVMVA(sf, mx, v, cv, lm)
#define gte_nmvmva(sf, mx, v, cv, lm) gte_nMVMVA(sf, mx, v, cv, lm)
#define gte_nrtvx()                   gte_nMVMVA(1, 0, 0, 0, 0)

/* Helpers not provided by gte_macros.h */

#define gte_ldv2_ext(r0) __asm__ volatile( \
    "lw		$12, 0( %0 );"                    \
    "lw		$13, 4( %0 );"                    \
    "ctc2	$12, $3;"                        \
    "ctc2	$13, $4;"                        \
    :                                      \
    : "r"(r0)                              \
    : "$12", "$13")

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

#define gte_stv0(r0) __asm__( \
    "swc2    $0, 0(%0);"      \
    "swc2    $1, 4(%0)"       \
    : : "r"(r0)               \
    : "memory")

#define gte_stv1(r0) __asm__( \
    "swc2    $2, 0(%0);"      \
    "swc2    $3, 4(%0)"       \
    : : "r"(r0)               \
    : "memory")

#define gte_stv2(r0) __asm__( \
    "swc2    $4, 0(%0);"      \
    "swc2    $5, 4(%0)"       \
    : : "r"(r0)               \
    : "memory")

#define gte_lddqa(r0) __asm__ volatile( \
    "ctc2	%0, $27"                      \
    :                                   \
    : "r"(r0))

#define gte_lddqb(r0) __asm__ volatile( \
    "ctc2	%0, $28"                      \
    :                                   \
    : "r"(r0))

#endif
