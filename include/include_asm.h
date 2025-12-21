#ifndef __INCLUDE_ASM_H__
#define __INCLUDE_ASM_H__

#if !defined(SPLAT) && !defined(__CTX__) && !defined(PERMUTER) && !defined(SKIP_ASM)
#ifndef INCLUDE_ASM
#define INCLUDE_ASM(FOLDER, NAME) \
   __asm__( \
        ".section .text\n" \
        "\t.set noat\n" \
        "\t.set noreorder\n" \
        "\t.align\t2\n" \
        "\t.globl\t"#NAME"\n" \
        "\t.type "#NAME", @function\n" \
        "\t.ent\t"#NAME"\n" \
        #NAME ":\n" \
        "\t.include \""FOLDER"/"#NAME".s\"\n" \
        "\t.set reorder\n" \
        "\t.set at\n" \
        "\t.end\t"#NAME"\n" \
        ".end"#NAME":\n" \
        "\t.size\t"#NAME",.end"#NAME"-"#NAME \
    )
#endif

#ifndef INCLUDE_RODATA
#define INCLUDE_RODATA(FOLDER, NAME) \
   __asm__( \
        ".section .rodata\n" \
        "\t.include \""FOLDER"/"#NAME".s\"\n" \
        ".section .text" \
    )
#endif

#ifndef FORCE_RODATA_ALIGNMENT
#define FORCE_RODATA_ALIGNMENT \
   __asm__( \
        ".section .rodata\n" \
        ".word 0x00000000" \
    )
#endif

__asm__(".include \"include/macro.inc\"\n");
#else
#define INCLUDE_ASM(FOLDER, NAME)
#define INCLUDE_RODATA(FOLDER, NAME)
#define FORCE_RODATA_ALIGNMENT
#endif

#endif
