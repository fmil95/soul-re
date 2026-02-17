### Build Options ###

BASEEXE          := SLUS_007.08
TARGET           := KAIN2
COMPARE          ?= 1
NON_MATCHING     ?= 0
SKIP_ASM         ?= 0
VERBOSE          ?= 0
BUILD_DIR        ?= build
TOOLS_DIR        := tools
OBJDIFF_DIR      := $(TOOLS_DIR)/objdiff
EXPECTED_DIR     ?= expected
CHECK            ?= 1

CINEMAX_YAML     := cinemax.yaml
CINEMAX_LD       := cinemax.ld
CINEMAX_LD_PP    := $(BUILD_DIR)/cinemax.ld
CINEMAX_BASEBIN  := cinemax.bin
CINEMAX_ELF      := $(BUILD_DIR)/cinemax.elf
CINEMAX_BIN      := $(BUILD_DIR)/cinemax.bin

MCARDX_YAML      := mcardx.yaml
MCARDX_LD        := mcardx.ld
MCARDX_LD_PP     := $(BUILD_DIR)/mcardx.ld
MCARDX_BASEBIN   := mcardx.bin
MCARDX_ELF       := $(BUILD_DIR)/mcardx.elf
MCARDX_BIN       := $(BUILD_DIR)/mcardx.bin

HUNTER_YAML      := hunter.yaml
HUNTER_LD        := hunter.ld
HUNTER_LD_PP     := $(BUILD_DIR)/hunter.ld
HUNTER_BASEBIN   := hunter.bin
HUNTER_ELF       := $(BUILD_DIR)/hunter.elf
HUNTER_BIN       := $(BUILD_DIR)/hunter.bin

SKINNER_YAML     := skinner.yaml
SKINNER_LD       := skinner.ld
SKINNER_LD_PP    := $(BUILD_DIR)/skinner.ld
SKINNER_BASEBIN  := skinner.bin
SKINNER_ELF      := $(BUILD_DIR)/skinner.elf
SKINNER_BIN      := $(BUILD_DIR)/skinner.bin

WALLCR_YAML      := wallcr.yaml
WALLCR_LD        := wallcr.ld
WALLCR_LD_PP     := $(BUILD_DIR)/wallcr.ld
WALLCR_BASEBIN   := wallcr.bin
WALLCR_ELF       := $(BUILD_DIR)/wallcr.elf
WALLCR_BIN       := $(BUILD_DIR)/wallcr.bin

ALUKA_YAML       := aluka.yaml
ALUKA_LD         := aluka.ld
ALUKA_LD_PP      := $(BUILD_DIR)/aluka.ld
ALUKA_BASEBIN    := aluka.bin
ALUKA_ELF        := $(BUILD_DIR)/aluka.elf
ALUKA_BIN        := $(BUILD_DIR)/aluka.bin

SKINBOS_YAML     := skinbos.yaml
SKINBOS_LD       := skinbos.ld
SKINBOS_LD_PP    := $(BUILD_DIR)/skinbos.ld
SKINBOS_BASEBIN  := skinbos.bin
SKINBOS_ELF      := $(BUILD_DIR)/skinbos.elf
SKINBOS_BIN      := $(BUILD_DIR)/skinbos.bin

KAIN_YAML        := kain.yaml
KAIN_LD          := kain.ld
KAIN_LD_PP       := $(BUILD_DIR)/kain.ld
KAIN_BASEBIN     := kain.bin
KAIN_ELF         := $(BUILD_DIR)/kain.elf
KAIN_BIN         := $(BUILD_DIR)/kain.bin

WALBOSS_YAML     := walboss.yaml
WALBOSS_LD       := walboss.ld
WALBOSS_LD_PP    := $(BUILD_DIR)/walboss.ld
WALBOSS_BASEBIN  := walboss.bin
WALBOSS_ELF      := $(BUILD_DIR)/walboss.elf
WALBOSS_BIN      := $(BUILD_DIR)/walboss.bin

WALBOSB_YAML     := walbosb.yaml
WALBOSB_LD       := walbosb.ld
WALBOSB_LD_PP    := $(BUILD_DIR)/walbosb.ld
WALBOSB_BASEBIN  := walbosb.bin
WALBOSB_ELF      := $(BUILD_DIR)/walbosb.elf
WALBOSB_BIN      := $(BUILD_DIR)/walbosb.bin

ALUKABSS_YAML    := alukabss.yaml
ALUKABSS_LD      := alukabss.ld
ALUKABSS_LD_PP   := $(BUILD_DIR)/alukabss.ld
ALUKABSS_BASEBIN := alukabss.bin
ALUKABSS_ELF     := $(BUILD_DIR)/alukabss.elf
ALUKABSS_BIN     := $(BUILD_DIR)/alukabss.bin

RONINBSS_YAML    := roninbss.yaml
RONINBSS_LD      := roninbss.ld
RONINBSS_LD_PP   := $(BUILD_DIR)/roninbss.ld
RONINBSS_BASEBIN := roninbss.bin
RONINBSS_ELF     := $(BUILD_DIR)/roninbss.elf
RONINBSS_BIN     := $(BUILD_DIR)/roninbss.bin

# Fail early if baserom does not exist
ifeq ($(wildcard $(BASEEXE)),)
$(error Baserom `$(BASEEXE)' not found.)
endif

# NON_MATCHING=1 implies COMPARE=0
ifeq ($(NON_MATCHING),1)
override COMPARE=0
endif

ifeq ($(VERBOSE),0)
V := @
endif

ifeq ($(OS),Windows_NT)
  DETECTED_OS=windows
else
  UNAME_S := $(shell uname -s)
  ifeq ($(UNAME_S),Linux)
    DETECTED_OS=linux
  endif
  ifeq ($(UNAME_S),Darwin)
    DETECTED_OS=macos
    MAKE=gmake
    CPPFLAGS += -xc++
  endif
endif

### Output ###

EXE          := $(BUILD_DIR)/$(TARGET).EXE
ELF          := $(BUILD_DIR)/$(TARGET).elf
LD_SCRIPT    := $(TARGET).ld
LD_MAP       := $(BUILD_DIR)/$(TARGET).map

### Tools ###

PYTHON         := python3
EXE_YAML       := $(BASEEXE).yaml
SPLAT          := splat split $(EXE_YAML)
SPLAT_CINEMAX  := splat split $(CINEMAX_YAML)
SPLAT_MCARDX   := splat split $(MCARDX_YAML)
SPLAT_HUNTER   := splat split $(HUNTER_YAML)
SPLAT_SKINNER  := splat split $(SKINNER_YAML)
SPLAT_WALLCR   := splat split $(WALLCR_YAML)
SPLAT_ALUKA    := splat split $(ALUKA_YAML)
SPLAT_SKINBOS  := splat split $(SKINBOS_YAML)
SPLAT_KAIN     := splat split $(KAIN_YAML)
SPLAT_WALBOSS  := splat split $(WALBOSS_YAML)
SPLAT_WALBOSB  := splat split $(WALBOSB_YAML)
SPLAT_ALUKABSS := splat split $(ALUKABSS_YAML)
SPLAT_RONINBSS := splat split $(RONINBSS_YAML)
DIFF           := diff
MASPSX         := $(PYTHON) tools/maspsx/maspsx.py --use-comm-section --aspsx-version=2.81 -G4096
CROSS          := mips-linux-gnu-
AS             := $(CROSS)as -EL
LD             := $(CROSS)ld -EL
OBJCOPY        := $(CROSS)objcopy
STRIP          := $(CROSS)strip
CPP            := $(CROSS)cpp
CC             := tools/gcc-2.8.1-psx/cc1
CC_HOST        := gcc
OBJDIFF        := $(OBJDIFF_DIR)/objdiff

PRINT := printf '
 ENDCOLOR := \033[0m
 WHITE     := \033[0m
 ENDWHITE  := $(ENDCOLOR)
 GREEN     := \033[0;32m
 ENDGREEN  := $(ENDCOLOR)
 BLUE      := \033[0;34m
 ENDBLUE   := $(ENDCOLOR)
 YELLOW    := \033[0;33m
 ENDYELLOW := $(ENDCOLOR)
 PURPLE    := \033[0;35m
 ENDPURPLE := $(ENDCOLOR)
ENDLINE := \n'

### Compiler Options ###

ASFLAGS        := -Iinclude -march=r3000 -mtune=r3000 -no-pad-sections
CFLAGS         := -O2 -G4096 -gcoff -quiet -fsigned-char
CPPFLAGS       := -Iinclude -Isrc -DTARGET_PSX
LDFLAGS        := -T undefined_syms.txt -T undefined_funcs.txt -T $(BUILD_DIR)/$(LD_SCRIPT) -Map $(LD_MAP) \
                  --no-check-sections -nostdlib
CFLAGS_CHECK   := -fsyntax-only -fno-builtin -std=gnu90
CHECK_WARNINGS := -Wall -Wextra

ifeq ($(NON_MATCHING),1)
CPPFLAGS += -DNON_MATCHING
endif

ifeq ($(SKIP_ASM),1)
CPPFLAGS := $(CPPFLAGS) -DSKIP_ASM
endif

### Sources ###

ASM_SRCS := $(shell find asm/Game -type f -name '*.s')
ASM_OBJS := $(ASM_SRCS:.s=.s.o)
ASM_OBJS := $(ASM_OBJS:%=$(BUILD_DIR)/%)

# Object files
OBJECTS := $(shell grep -E 'BUILD_PATH.+\.o' $(LD_SCRIPT) -o)
OBJECTS := $(OBJECTS:BUILD_PATH/%=$(BUILD_DIR)/%)
ifeq ($(SKIP_ASM),1)
OBJECTS += $(ASM_OBJS)
endif
DEPENDS := $(OBJECTS:=.d)

CINEMAX_OBJECTS := \
  $(BUILD_DIR)/src/Overlays/cinemax/cinemax.c.o \
  $(BUILD_DIR)/asm/Overlays/cinemax/cinemax.s.o \
  $(BUILD_DIR)/asm/data/Overlays/cinemax/cinemax.data.s.o \
  $(BUILD_DIR)/asm/data/Overlays/cinemax/cinemax.bss.s.o

MCARDX_OBJECTS := \
  $(BUILD_DIR)/src/Overlays/mcardx/mcardx.c.o \
  $(BUILD_DIR)/asm/Overlays/mcardx/mcardx.s.o \
  $(BUILD_DIR)/asm/data/Overlays/mcardx/mcardx.data.s.o 

HUNTER_OBJECTS := \
  $(BUILD_DIR)/src/Overlays/hunter/hunter.c.o \
  $(BUILD_DIR)/asm/Overlays/hunter/hunter.s.o \
  $(BUILD_DIR)/asm/data/Overlays/hunter/hunter.data.s.o \
  $(BUILD_DIR)/asm/data/Overlays/hunter/hunter.bss.s.o

SKINNER_OBJECTS := \
  $(BUILD_DIR)/src/Overlays/skinner/skinner.c.o \
  $(BUILD_DIR)/asm/Overlays/skinner/skinner.s.o \
  $(BUILD_DIR)/asm/data/Overlays/skinner/skinner.data.s.o \
  $(BUILD_DIR)/asm/data/Overlays/skinner/skinner.bss.s.o

WALLCR_OBJECTS := \
  $(BUILD_DIR)/src/Overlays/wallcr/wallcr.c.o \
  $(BUILD_DIR)/asm/Overlays/wallcr/wallcr.s.o \
  $(BUILD_DIR)/asm/data/Overlays/wallcr/wallcr.data.s.o \
  $(BUILD_DIR)/asm/data/Overlays/wallcr/wallcr.bss.s.o

ALUKA_OBJECTS := \
  $(BUILD_DIR)/src/Overlays/aluka/aluka.c.o \
  $(BUILD_DIR)/asm/Overlays/aluka/aluka.s.o \
  $(BUILD_DIR)/asm/data/Overlays/aluka/aluka.data.s.o \
  $(BUILD_DIR)/asm/data/Overlays/aluka/aluka.bss.s.o

SKINBOS_OBJECTS := \
  $(BUILD_DIR)/src/Overlays/skinbos/skinbos.c.o \
  $(BUILD_DIR)/asm/Overlays/skinbos/skinbos.s.o \
  $(BUILD_DIR)/asm/data/Overlays/skinbos/skinbos.data.s.o \
  $(BUILD_DIR)/asm/data/Overlays/skinbos/skinbos.bss.s.o

KAIN_OBJECTS := \
  $(BUILD_DIR)/src/Overlays/kain/kain.c.o \
  $(BUILD_DIR)/asm/Overlays/kain/kain.s.o \
  $(BUILD_DIR)/asm/data/Overlays/kain/kain.data.s.o \
  $(BUILD_DIR)/asm/data/Overlays/kain/kain.bss.s.o

WALBOSS_OBJECTS := \
  $(BUILD_DIR)/src/Overlays/walboss/walboss.c.o \
  $(BUILD_DIR)/asm/Overlays/walboss/walboss.s.o \
  $(BUILD_DIR)/asm/data/Overlays/walboss/walboss.data.s.o \
  $(BUILD_DIR)/asm/data/Overlays/walboss/walboss.bss.s.o

WALBOSB_OBJECTS := \
  $(BUILD_DIR)/src/Overlays/walbosb/walbosb.c.o \
  $(BUILD_DIR)/asm/Overlays/walbosb/walbosb.s.o \
  $(BUILD_DIR)/asm/data/Overlays/walbosb/walbosb.data.s.o \
  $(BUILD_DIR)/asm/data/Overlays/walbosb/walbosb.bss.s.o

ALUKABSS_OBJECTS := \
  $(BUILD_DIR)/src/Overlays/alukabss/alukabss.c.o \
  $(BUILD_DIR)/asm/Overlays/alukabss/alukabss.s.o \
  $(BUILD_DIR)/asm/data/Overlays/alukabss/alukabss.data.s.o

RONINBSS_OBJECTS := \
  $(BUILD_DIR)/src/Overlays/roninbss/roninbss.c.o \
  $(BUILD_DIR)/asm/Overlays/roninbss/roninbss.s.o \
  $(BUILD_DIR)/asm/data/Overlays/roninbss/roninbss.data.s.o

### Targets ###

$(BUILD_DIR)/src/Game/CINEMA/CINEPSX.c.o: CFLAGS += -G0

$(BUILD_DIR)/src/Game/MCARD/MEMCARD.c.o: CFLAGS += -G0
$(BUILD_DIR)/src/Game/MCARD/MCASSERT.c.o: CFLAGS += -G0

$(BUILD_DIR)/src/Game/MENU/MENUUTIL.c.o: CFLAGS += -G0

$(BUILD_DIR)/src/Game/G2/QUATG2.c.o: CFLAGS += -funsigned-char

$(BUILD_DIR)/src/Game/PLAN/ENMYPLAN.c.o: CFLAGS += -funsigned-char
$(BUILD_DIR)/src/Game/PLAN/PLANPOOL.c.o: CFLAGS += -funsigned-char
$(BUILD_DIR)/src/Game/PLAN/PLANAPI.c.o: CFLAGS += -funsigned-char

$(BUILD_DIR)/src/Game/PSX/AADLIB.c.o: CFLAGS += -funsigned-char
$(BUILD_DIR)/src/Game/PSX/AADSEQEV.c.o: CFLAGS += -funsigned-char
$(BUILD_DIR)/src/Game/PSX/AADSFX.c.o: CFLAGS += -funsigned-char
$(BUILD_DIR)/src/Game/PSX/AADSQCMD.c.o: CFLAGS += -funsigned-char
$(BUILD_DIR)/src/Game/PSX/AADVOICE.c.o: CFLAGS += -funsigned-char
$(BUILD_DIR)/src/Game/PSX/MAIN.c.o: CFLAGS += -funsigned-char
$(BUILD_DIR)/src/Game/PSX/SUPPORT.c.o: CFLAGS += -funsigned-char

$(BUILD_DIR)/src/Game/DRAW.c.o: CFLAGS += -funsigned-char
$(BUILD_DIR)/src/Game/STRMLOAD.c.o: CFLAGS += -funsigned-char
$(BUILD_DIR)/src/Game/INSTANCE.c.o: CFLAGS += -funsigned-char
$(BUILD_DIR)/src/Game/DEBUG.c.o: CFLAGS += -funsigned-char
$(BUILD_DIR)/src/Game/SOUND.c.o: CFLAGS += -funsigned-char
$(BUILD_DIR)/src/Game/MEMPACK.c.o: CFLAGS += -funsigned-char
$(BUILD_DIR)/src/Game/LOAD3D.c.o: CFLAGS += -funsigned-char
$(BUILD_DIR)/src/Game/FONT.c.o: CFLAGS += -funsigned-char
$(BUILD_DIR)/src/Game/EVENT.c.o: CFLAGS += -funsigned-char

$(BUILD_DIR)/src/Game/MONSTER/MONMSG.c.o: CFLAGS += -funsigned-char

$(BUILD_DIR)/src/Game/MENU/MENUFACE.c.o: CFLAGS += -funsigned-char

$(BUILD_DIR)/src/Game/RAZIEL/RAZIEL.c.o: CFLAGS += -funsigned-char

$(BUILD_DIR)/src/Overlays/cinemax/cinemax.c.o: CFLAGS += -G0
$(BUILD_DIR)/src/Overlays/mcardx/mcardx.c.o: CFLAGS += -G0
$(BUILD_DIR)/src/Overlays/hunter/hunter.c.o: CFLAGS += -G0
$(BUILD_DIR)/src/Overlays/skinner/skinner.c.o: CFLAGS += -G0
$(BUILD_DIR)/src/Overlays/wallcr/wallcr.c.o: CFLAGS += -G0
$(BUILD_DIR)/src/Overlays/aluka/aluka.c.o: CFLAGS += -G0
$(BUILD_DIR)/src/Overlays/skinbos/skinbos.c.o: CFLAGS += -G0
$(BUILD_DIR)/src/Overlays/kain/kain.c.o: CFLAGS += -G0
$(BUILD_DIR)/src/Overlays/walboss/walboss.c.o: CFLAGS += -G0
$(BUILD_DIR)/src/Overlays/walbosb/walbosb.c.o: CFLAGS += -G0
$(BUILD_DIR)/src/Overlays/alukabss/alukabss.c.o: CFLAGS += -G0
$(BUILD_DIR)/src/Overlays/roninbss/roninbss.c.o: CFLAGS += -G0

ifeq ($(SKIP_ASM),1)
all: $(OBJECTS) overlays
	@echo "SKIP_ASM=1: Skipping linking, only built objects."
else
all: $(EXE) overlays
endif

-include $(DEPENDS)

clean:
	$(V)rm -rf $(BUILD_DIR)
	$(V)rm -rf $(EXPECTED_DIR)

distclean: clean
	$(V)rm -f $(LD_SCRIPT)
	$(V)rm -rf asm
	$(V)rm -rf *_auto.txt

setup: distclean split

split:
	$(V)$(SPLAT)
	$(V)$(SPLAT_CINEMAX)
	$(V)$(SPLAT_MCARDX)
	$(V)$(SPLAT_HUNTER)
	$(V)$(SPLAT_SKINNER)
	$(V)$(SPLAT_WALLCR)
	$(V)$(SPLAT_ALUKA)
	$(V)$(SPLAT_SKINBOS)
	$(V)$(SPLAT_KAIN)
	$(V)$(SPLAT_WALBOSS)
	$(V)$(SPLAT_WALBOSB)
	$(V)$(SPLAT_ALUKABSS)
	$(V)$(SPLAT_RONINBSS)

reset: clean
	$(V)rm -rf $(EXPECTED_DIR)

regenerate: reset

objdiff-config: regenerate
	@$(MAKE) NON_MATCHING=1 SKIP_ASM=1 expected -j12
	@$(PYTHON) $(OBJDIFF_DIR)/objdiff_generate.py $(OBJDIFF_DIR)/config.yaml

report: objdiff-config
	@$(OBJDIFF) report generate > $(BUILD_DIR)/progress.json

progress:
	$(MAKE) build NON_MATCHING=1 SKIP_ASM=1

expected: all
	@mkdir -p $(EXPECTED_DIR)
	$(V)mv $(BUILD_DIR)/asm $(EXPECTED_DIR)/asm
	$(V)mv $(BUILD_DIR)/src $(EXPECTED_DIR)/src
	$(V)find $(EXPECTED_DIR)/src -name '*.s.o' -delete

overlays: $(CINEMAX_BIN) $(MCARDX_BIN) $(HUNTER_BIN) $(SKINNER_BIN) $(WALLCR_BIN) $(ALUKA_BIN) $(SKINBOS_BIN) $(KAIN_BIN) $(WALBOSS_BIN) $(WALBOSB_BIN) $(ALUKABSS_BIN) $(RONINBSS_BIN)

# Compile .c files
$(BUILD_DIR)/%.c.o: %.c
	@$(PRINT)$(GREEN)Compiling C file: $(ENDGREEN)$(BLUE)$<$(ENDBLUE)$(ENDLINE)
	@mkdir -p $(shell dirname $@)
ifeq ($(CHECK),1)
	@$(CC_HOST) $(CFLAGS_CHECK) $(CHECK_WARNINGS) $(CPPFLAGS) -UTARGET_PSX $<
endif
	$(V)$(CPP) $(CPPFLAGS) -ffreestanding -MMD -MP -MT $@ -MF $@.d $< | $(CC) $(CFLAGS) | $(MASPSX) | $(AS) $(ASFLAGS) -o $@

# Compile .s files
$(BUILD_DIR)/%.s.o: %.s
	@$(PRINT)$(GREEN)Assembling asm file: $(ENDGREEN)$(BLUE)$<$(ENDBLUE)$(ENDLINE)
	@mkdir -p $(shell dirname $@)
	$(V)$(AS) $(ASFLAGS) -o $@ $<

# Create .o files from .bin files.
$(BUILD_DIR)/%.bin.o: %.bin
	@$(PRINT)$(GREEN)objcopying binary file: $(ENDGREEN)$(BLUE)$<$(ENDBLUE)$(ENDLINE)
	@mkdir -p $(shell dirname $@)
	$(V)$(LD) -r -b binary -o $@ $<

$(BUILD_DIR)/$(LD_SCRIPT): $(LD_SCRIPT)
	@$(PRINT)$(GREEN)Preprocessing linker script: $(ENDGREEN)$(BLUE)$<$(ENDBLUE)$(ENDLINE)
	$(V)$(CPP) -P -DBUILD_PATH=$(BUILD_DIR) $< -o $@
#Temporary hack for noload segment wrong alignment
	@sed -r -i 's/\.main_bss \(NOLOAD\) : SUBALIGN\(4\)/.main_bss main_SDATA_END (NOLOAD) : SUBALIGN(4)/g' $@

ifeq ($(SKIP_ASM),1)
# Prevent building ELF if SKIP_ASM != 1
$(BUILD_DIR)/$(TARGET).elf:
	@echo "Skipping linking (SKIP_ASM != 1)"
else
# Link the .o files into the .elf
$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) $(BUILD_DIR)/$(LD_SCRIPT)
	@$(PRINT)$(GREEN)Linking elf file: $(ENDGREEN)$(BLUE)$@$(ENDBLUE)$(ENDLINE)
	$(V)$(LD) $(LDFLAGS) -o $@
endif

ifeq ($(SKIP_ASM),1)
$(EXE):
	@echo "Skipping EXE creation (SKIP_ASM=1)"
else
# Convert the .elf to the final exe
$(EXE): $(BUILD_DIR)/$(TARGET).elf
	@$(PRINT)$(GREEN)Creating EXE: $(ENDGREEN)$(BLUE)$@$(ENDBLUE)$(ENDLINE)
	$(V)$(OBJCOPY) $< $@ -O binary
	$(V)$(OBJCOPY) -O binary --gap-fill 0x00 --pad-to 0x0C3000 $< $@
ifeq ($(COMPARE),1)
	@$(DIFF) $(BASEEXE) $(EXE) && printf "EXE: OK\n" || (echo 'The build succeeded, but did not match the base EXE. This is expected if you are making changes to the game. To skip this check, use "make COMPARE=0".' && false)
endif
endif

$(CINEMAX_LD_PP): $(CINEMAX_LD)
	@$(PRINT)$(GREEN)Preprocessing cinemax overlay ld: $(ENDGREEN)$(BLUE)$<$(ENDBLUE)$(ENDLINE)
	@mkdir -p $(BUILD_DIR)
	$(V)$(CPP) -P -DBUILD_PATH=$(BUILD_DIR) $< -o $@

$(CINEMAX_ELF): $(CINEMAX_OBJECTS) $(CINEMAX_LD_PP)
	$(V)$(LD) \
		--no-check-sections \
		-nostdlib \
		-T undefined_syms_auto.cinemax.txt \
		-T undefined_funcs_auto.cinemax.txt \
		-T $(CINEMAX_LD_PP) \
		-Map $(BUILD_DIR)/cinemax.map \
		-o $@

$(CINEMAX_BIN): $(CINEMAX_ELF)
	@$(PRINT)$(GREEN)Creating cinemax.bin: $(ENDGREEN)$(BLUE)$@$(ENDBLUE)$(ENDLINE)
	$(V)$(OBJCOPY) -O binary $< $@
ifeq ($(COMPARE),1)
	@$(DIFF) $(CINEMAX_BASEBIN) $(CINEMAX_BIN) && printf "cinemax.bin: OK\n" || (echo 'The build succeeded, but did not match the base BIN. This is expected if you are making changes to the game. To skip this check, use "make COMPARE=0".' && false)
endif

$(MCARDX_LD_PP): $(MCARDX_LD)
	@$(PRINT)$(GREEN)Preprocessing mcardx overlay ld: $(ENDGREEN)$(BLUE)$<$(ENDBLUE)$(ENDLINE)
	@mkdir -p $(BUILD_DIR)
	$(V)$(CPP) -P -DBUILD_PATH=$(BUILD_DIR) $< -o $@

$(MCARDX_ELF): $(MCARDX_OBJECTS) $(MCARDX_LD_PP)
	$(V)$(LD) \
		--no-check-sections \
		-nostdlib \
		-T undefined_syms_auto.mcardx.txt \
		-T undefined_funcs_auto.mcardx.txt \
		-T $(MCARDX_LD_PP) \
		-Map $(BUILD_DIR)/mcardx.map \
		-o $@

$(MCARDX_BIN): $(MCARDX_ELF)
	@$(PRINT)$(GREEN)Creating mcardx.bin: $(ENDGREEN)$(BLUE)$@$(ENDBLUE)$(ENDLINE)
	$(V)$(OBJCOPY) -O binary $< $@
ifeq ($(COMPARE),1)
	@$(DIFF) $(MCARDX_BASEBIN) $(MCARDX_BIN) && printf "mcardx.bin: OK\n" || (echo 'The build succeeded, but did not match the base BIN. This is expected if you are making changes to the game. To skip this check, use "make COMPARE=0".' && false)
endif

$(HUNTER_LD_PP): $(HUNTER_LD)
	@$(PRINT)$(GREEN)Preprocessing hunter overlay ld: $(ENDGREEN)$(BLUE)$<$(ENDBLUE)$(ENDLINE)
	@mkdir -p $(BUILD_DIR)
	$(V)$(CPP) -P -DBUILD_PATH=$(BUILD_DIR) $< -o $@

$(HUNTER_ELF): $(HUNTER_OBJECTS) $(HUNTER_LD_PP)
	$(V)$(LD) \
		--no-check-sections \
		-nostdlib \
		-T undefined_syms_auto.hunter.txt \
		-T undefined_funcs_auto.hunter.txt \
		-T $(HUNTER_LD_PP) \
		-Map $(BUILD_DIR)/hunter.map \
		-o $@

$(HUNTER_BIN): $(HUNTER_ELF)
	@$(PRINT)$(GREEN)Creating hunter.bin: $(ENDGREEN)$(BLUE)$@$(ENDBLUE)$(ENDLINE)
	$(V)$(OBJCOPY) -O binary $< $@
ifeq ($(COMPARE),1)
	@$(DIFF) $(HUNTER_BASEBIN) $(HUNTER_BIN) && printf "hunter.bin: OK\n" || (echo 'The build succeeded, but did not match the base BIN. This is expected if you are making changes to the game. To skip this check, use "make COMPARE=0".' && false)
endif

$(SKINNER_LD_PP): $(SKINNER_LD)
	@$(PRINT)$(GREEN)Preprocessing skinner overlay ld: $(ENDGREEN)$(BLUE)$<$(ENDBLUE)$(ENDLINE)
	@mkdir -p $(BUILD_DIR)
	$(V)$(CPP) -P -DBUILD_PATH=$(BUILD_DIR) $< -o $@

$(SKINNER_ELF): $(SKINNER_OBJECTS) $(SKINNER_LD_PP)
	$(V)$(LD) \
		--no-check-sections \
		-nostdlib \
		-T undefined_syms_auto.skinner.txt \
		-T undefined_funcs_auto.skinner.txt \
		-T $(SKINNER_LD_PP) \
		-Map $(BUILD_DIR)/skinner.map \
		-o $@

$(SKINNER_BIN): $(SKINNER_ELF)
	@$(PRINT)$(GREEN)Creating skinner.bin: $(ENDGREEN)$(BLUE)$@$(ENDBLUE)$(ENDLINE)
	$(V)$(OBJCOPY) -O binary $< $@
ifeq ($(COMPARE),1)
	@$(DIFF) $(SKINNER_BASEBIN) $(SKINNER_BIN) && printf "skinner.bin: OK\n" || (echo 'The build succeeded, but did not match the base BIN. This is expected if you are making changes to the game. To skip this check, use "make COMPARE=0".' && false)
endif

$(WALLCR_LD_PP): $(WALLCR_LD)
	@$(PRINT)$(GREEN)Preprocessing wallcr overlay ld: $(ENDGREEN)$(BLUE)$<$(ENDBLUE)$(ENDLINE)
	@mkdir -p $(BUILD_DIR)
	$(V)$(CPP) -P -DBUILD_PATH=$(BUILD_DIR) $< -o $@

$(WALLCR_ELF): $(WALLCR_OBJECTS) $(WALLCR_LD_PP)
	$(V)$(LD) \
		--no-check-sections \
		-nostdlib \
		-T undefined_syms_auto.wallcr.txt \
		-T undefined_funcs_auto.wallcr.txt \
		-T $(WALLCR_LD_PP) \
		-Map $(BUILD_DIR)/wallcr.map \
		-o $@

$(WALLCR_BIN): $(WALLCR_ELF)
	@$(PRINT)$(GREEN)Creating wallcr.bin: $(ENDGREEN)$(BLUE)$@$(ENDBLUE)$(ENDLINE)
	$(V)$(OBJCOPY) -O binary $< $@
ifeq ($(COMPARE),1)
	@$(DIFF) $(WALLCR_BASEBIN) $(WALLCR_BIN) && printf "wallcr.bin: OK\n" || (echo 'The build succeeded, but did not match the base BIN. This is expected if you are making changes to the game. To skip this check, use "make COMPARE=0".' && false)
endif

$(ALUKA_LD_PP): $(ALUKA_LD)
	@$(PRINT)$(GREEN)Preprocessing aluka overlay ld: $(ENDGREEN)$(BLUE)$<$(ENDBLUE)$(ENDLINE)
	@mkdir -p $(BUILD_DIR)
	$(V)$(CPP) -P -DBUILD_PATH=$(BUILD_DIR) $< -o $@

$(ALUKA_ELF): $(ALUKA_OBJECTS) $(ALUKA_LD_PP)
	$(V)$(LD) \
		--no-check-sections \
		-nostdlib \
		-T undefined_syms_auto.aluka.txt \
		-T undefined_funcs_auto.aluka.txt \
		-T $(ALUKA_LD_PP) \
		-Map $(BUILD_DIR)/aluka.map \
		-o $@

$(ALUKA_BIN): $(ALUKA_ELF)
	@$(PRINT)$(GREEN)Creating aluka.bin: $(ENDGREEN)$(BLUE)$@$(ENDBLUE)$(ENDLINE)
	$(V)$(OBJCOPY) -O binary $< $@
ifeq ($(COMPARE),1)
	@$(DIFF) $(ALUKA_BASEBIN) $(ALUKA_BIN) && printf "aluka.bin: OK\n" || (echo 'The build succeeded, but did not match the base BIN. This is expected if you are making changes to the game. To skip this check, use "make COMPARE=0".' && false)
endif

$(SKINBOS_LD_PP): $(SKINBOS_LD)
	@$(PRINT)$(GREEN)Preprocessing skinbos overlay ld: $(ENDGREEN)$(BLUE)$<$(ENDBLUE)$(ENDLINE)
	@mkdir -p $(BUILD_DIR)
	$(V)$(CPP) -P -DBUILD_PATH=$(BUILD_DIR) $< -o $@

$(SKINBOS_ELF): $(SKINBOS_OBJECTS) $(SKINBOS_LD_PP)
	$(V)$(LD) \
		--no-check-sections \
		-nostdlib \
		-T undefined_syms_auto.skinbos.txt \
		-T undefined_funcs_auto.skinbos.txt \
		-T $(SKINBOS_LD_PP) \
		-Map $(BUILD_DIR)/skinbos.map \
		-o $@

$(SKINBOS_BIN): $(SKINBOS_ELF)
	@$(PRINT)$(GREEN)Creating skinbos.bin: $(ENDGREEN)$(BLUE)$@$(ENDBLUE)$(ENDLINE)
	$(V)$(OBJCOPY) -O binary $< $@
ifeq ($(COMPARE),1)
	@$(DIFF) $(SKINBOS_BASEBIN) $(SKINBOS_BIN) && printf "skinbos.bin: OK\n" || (echo 'The build succeeded, but did not match the base BIN. This is expected if you are making changes to the game. To skip this check, use "make COMPARE=0".' && false)
endif

$(KAIN_LD_PP): $(KAIN_LD)
	@$(PRINT)$(GREEN)Preprocessing kain overlay ld: $(ENDGREEN)$(BLUE)$<$(ENDBLUE)$(ENDLINE)
	@mkdir -p $(BUILD_DIR)
	$(V)$(CPP) -P -DBUILD_PATH=$(BUILD_DIR) $< -o $@

$(KAIN_ELF): $(KAIN_OBJECTS) $(KAIN_LD_PP)
	$(V)$(LD) \
		--no-check-sections \
		-nostdlib \
		-T undefined_syms_auto.kain.txt \
		-T undefined_funcs_auto.kain.txt \
		-T $(KAIN_LD_PP) \
		-Map $(BUILD_DIR)/kain.map \
		-o $@

$(KAIN_BIN): $(KAIN_ELF)
	@$(PRINT)$(GREEN)Creating kain.bin: $(ENDGREEN)$(BLUE)$@$(ENDBLUE)$(ENDLINE)
	$(V)$(OBJCOPY) -O binary $< $@
ifeq ($(COMPARE),1)
	@$(DIFF) $(KAIN_BASEBIN) $(KAIN_BIN) && printf "kain.bin: OK\n" || (echo 'The build succeeded, but did not match the base BIN. This is expected if you are making changes to the game. To skip this check, use "make COMPARE=0".' && false)
endif

$(WALBOSS_LD_PP): $(WALBOSS_LD)
	@$(PRINT)$(GREEN)Preprocessing walboss overlay ld: $(ENDGREEN)$(BLUE)$<$(ENDBLUE)$(ENDLINE)
	@mkdir -p $(BUILD_DIR)
	$(V)$(CPP) -P -DBUILD_PATH=$(BUILD_DIR) $< -o $@

$(WALBOSS_ELF): $(WALBOSS_OBJECTS) $(WALBOSS_LD_PP)
	$(V)$(LD) \
		--no-check-sections \
		-nostdlib \
		-T undefined_syms_auto.walboss.txt \
		-T undefined_funcs_auto.walboss.txt \
		-T $(WALBOSS_LD_PP) \
		-Map $(BUILD_DIR)/walboss.map \
		-o $@

$(WALBOSS_BIN): $(WALBOSS_ELF)
	@$(PRINT)$(GREEN)Creating walboss.bin: $(ENDGREEN)$(BLUE)$@$(ENDBLUE)$(ENDLINE)
	$(V)$(OBJCOPY) -O binary $< $@
ifeq ($(COMPARE),1)
	@$(DIFF) $(WALBOSS_BASEBIN) $(WALBOSS_BIN) && printf "walboss.bin: OK\n" || (echo 'The build succeeded, but did not match the base BIN. This is expected if you are making changes to the game. To skip this check, use "make COMPARE=0".' && false)
endif

$(WALBOSB_LD_PP): $(WALBOSB_LD)
	@$(PRINT)$(GREEN)Preprocessing walbosb overlay ld: $(ENDGREEN)$(BLUE)$<$(ENDBLUE)$(ENDLINE)
	@mkdir -p $(BUILD_DIR)
	$(V)$(CPP) -P -DBUILD_PATH=$(BUILD_DIR) $< -o $@

$(WALBOSB_ELF): $(WALBOSB_OBJECTS) $(WALBOSB_LD_PP)
	$(V)$(LD) \
		--no-check-sections \
		-nostdlib \
		-T undefined_syms_auto.walbosb.txt \
		-T undefined_funcs_auto.walbosb.txt \
		-T $(WALBOSB_LD_PP) \
		-Map $(BUILD_DIR)/walbosb.map \
		-o $@

$(WALBOSB_BIN): $(WALBOSB_ELF)
	@$(PRINT)$(GREEN)Creating walbosb.bin: $(ENDGREEN)$(BLUE)$@$(ENDBLUE)$(ENDLINE)
	$(V)$(OBJCOPY) -O binary $< $@
ifeq ($(COMPARE),1)
	@$(DIFF) $(WALBOSB_BASEBIN) $(WALBOSB_BIN) && printf "walbosb.bin: OK\n" || (echo 'The build succeeded, but did not match the base BIN. This is expected if you are making changes to the game. To skip this check, use "make COMPARE=0".' && false)
endif

$(ALUKABSS_LD_PP): $(ALUKABSS_LD)
	@$(PRINT)$(GREEN)Preprocessing alukabss overlay ld: $(ENDGREEN)$(BLUE)$<$(ENDBLUE)$(ENDLINE)
	@mkdir -p $(BUILD_DIR)
	$(V)$(CPP) -P -DBUILD_PATH=$(BUILD_DIR) $< -o $@

$(ALUKABSS_ELF): $(ALUKABSS_OBJECTS) $(ALUKABSS_LD_PP)
	$(V)$(LD) \
		--no-check-sections \
		-nostdlib \
		-T undefined_syms_auto.alukabss.txt \
		-T undefined_funcs_auto.alukabss.txt \
		-T $(ALUKABSS_LD_PP) \
		-Map $(BUILD_DIR)/alukabss.map \
		-o $@

$(ALUKABSS_BIN): $(ALUKABSS_ELF)
	@$(PRINT)$(GREEN)Creating alukabss.bin: $(ENDGREEN)$(BLUE)$@$(ENDBLUE)$(ENDLINE)
	$(V)$(OBJCOPY) -O binary $< $@
ifeq ($(COMPARE),1)
	@$(DIFF) $(ALUKABSS_BASEBIN) $(ALUKABSS_BIN) && printf "alukabss.bin: OK\n" || (echo 'The build succeeded, but did not match the base BIN. This is expected if you are making changes to the game. To skip this check, use "make COMPARE=0".' && false)
endif

$(RONINBSS_LD_PP): $(RONINBSS_LD)
	@$(PRINT)$(GREEN)Preprocessing roninbss overlay ld: $(ENDGREEN)$(BLUE)$<$(ENDBLUE)$(ENDLINE)
	@mkdir -p $(BUILD_DIR)
	$(V)$(CPP) -P -DBUILD_PATH=$(BUILD_DIR) $< -o $@

$(RONINBSS_ELF): $(RONINBSS_OBJECTS) $(RONINBSS_LD_PP)
	$(V)$(LD) \
		--no-check-sections \
		-nostdlib \
		-T undefined_syms_auto.roninbss.txt \
		-T undefined_funcs_auto.roninbss.txt \
		-T $(RONINBSS_LD_PP) \
		-Map $(BUILD_DIR)/roninbss.map \
		-o $@

$(RONINBSS_BIN): $(RONINBSS_ELF)
	@$(PRINT)$(GREEN)Creating alukabss.bin: $(ENDGREEN)$(BLUE)$@$(ENDBLUE)$(ENDLINE)
	$(V)$(OBJCOPY) -O binary $< $@
ifeq ($(COMPARE),1)
	@$(DIFF) $(RONINBSS_BASEBIN) $(RONINBSS_BIN) && printf "roninbss.bin: OK\n" || (echo 'The build succeeded, but did not match the base BIN. This is expected if you are making changes to the game. To skip this check, use "make COMPARE=0".' && false)
endif

### Make Settings ###

.PHONY: all clean distclean overlays setup split

# Remove built-in implicit rules to improve performance
MAKEFLAGS += --no-builtin-rules

# Print target for debugging
print-% : ; $(info $* is a $(flavor $*) variable set to [$($*)]) @true
