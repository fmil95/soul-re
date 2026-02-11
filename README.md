# Soul Reaverse

![image](https://i.imgur.com/RTQyAu8.png)

## Description

**_Soul Reaverse_** is a project that seeks to reconstruct the source code of the PSX action-adventure classic _Legacy of Kain: Soul Reaver_. This is achieved by disassembling the executable of a prototype of the game that was built two weeks before its final release, which includes a full set of debugging symbols that makes the process of reverse-engineering the binary's MIPS instructions back to C language easier.

Once its fully overhauled, the source code could then be used for many different purposes, including but not limited to: porting the game to other platforms, improving its performance on console hardware, and developing tools for it such as a level editor.

## Progress

| Name | Purpose | Status |
|------|---------|----------|
| SLUS_007.08 | Gex 2 engine + game code | ![image](https://decomp.dev/fmil95/soul-re.svg?mode=shield&label=Code&measure=matched_code_percent) ![image](https://decomp.dev/fmil95/soul-re.svg?mode=shield&label=Functions&measure=matched_functions)|
| cinemax.drm | Video Player overlay | N/A |
| mcardx.drm | Memory Card overlay | N/A |
| ronin.drm | Dumahim overlay | N/A |
| hunter.drm | Human Hunter overlay | N/A |
| skinner.drm | Melchiahim overlay | N/A |
| wallcr.drm | Zephonim overlay | N/A |
| morlock.drm | Turelim overlay | N/A |
| aluka.drm | Rahabim overlay | N/A |
| skinbos.drm | Melchiah overlay | N/A |
| kain.drm | Kain overlay | N/A |
| walboss.drm | Zephon overlay A | N/A |
| walbosb.drm | Zephon overlay B | N/A |
| walbosc.drm | Zephon overlay C | N/A |
| alukabss.drm | Rahab overlay | N/A |
| roninbss.drm | Dumah overlay | N/A |

In case you wonder what an overlay is: Soul Reaver like many other titles for the PlayStation (especially those near the end of the system's life cycle) distributes a good amount of game logic throughout _overlays_. For those not familiar with the concept, overlays are binaries which contain code that is loaded at specific instances during gameplay by the primary executable. This was designed such to taper the maximum amount of memory that the game consumes at once, which could never be too high due to hardware limitations. While overlays are fairly common, Crystal Dynamics designed some noteworthy tech to link DRM files dynamically on runtime, which means that the game's overlays never load at the same address during execution and thus, why none of them have been decompiled yet, since they require their own decompilation workflow different from the PS-EXE.

### DAT Utils

Soul Reaver, like other Crystal Dynamics games for the PSX, stores its overlays in BIGFILE.DAT. There is a provided utility for unpacking, repacking, and comparing BIGFILEs to assist with the overlay work. To get started, place `BIGFILE.DAT` into the root of the project.

To unpack the file:

```bash
python tools/cd-dat-utils/dat_utils.py -c dat-config.json unpack BIFILE.DAT <output_dir>
```

To repackage the file:

```bash
python tools/cd-dat-utils/dat_utils.py -c dat-config.json pack <input_dir> <output_file>
```

To compare between 2 BIGFILEs (packed or unpacked):

```bash
python tools/cd-dat-utils/dat_utils.py -c dat-config.json compare <input_a> <input_b>
```

The compare command will tell you how the BIGFILEs differ and in what way (encryption, hashes of file X in folder Y, etc.)

### Setting Up

For first time setup, check out the [installation guide](docs/INSTALL.MD).

### Contributing

Thinking about contributing to the project? Check out the [contribution guide](docs/CONTRIBUTING.MD) to get started!

### Reference

New to the PSX, decompilation as a whole, or just stuck on a function? Check out the [decompilation reference](/docs/DECOMPILATION.MD) which has several common code patterns broken down and documented.

### Contact

The **PS1/PS2 Decompilation** Discord server has an exclusive channel for this decompilation titled **#soul-reaver**, which is the main medium of communication for anything in relation to it: [REDACTED].

Alternatively, if you aren't able to join the server for some reason, you can write me an email to frozenburnside@hotmail.com with any query you might have.
