# Soul Reaverse

![image](https://i.imgur.com/RTQyAu8.png)

## Description

**_Soul Reaverse_** is a project that seeks to reconstruct the source code of the PSX action-adventure classic _Legacy of Kain: Soul Reaver_. This is achieved by disassembling the executable of a prototype of the game that was built two weeks before its final release, which includes a full set of debugging symbols that makes the process of reverse-engineering the binary's MIPS instructions back to C language easier.

Once its fully overhauled, the source code could then be used for many different purposes, including but not limited to: porting the game to other platforms, improving its performance on console hardware, and developing tools for it such as a level editor.

## Progress

[![Build Status]][actions] [![Code Progress]][progress] [![Data Progress]][progress] 

[Build Status]: https://github.com/fmil95/soul-re/actions/workflows/progress_report.yml/badge.svg
[actions]: https://github.com/fmil95/soul-re/actions/workflows/progress_report.yml

[Code Progress]: https://decomp.dev/fmil95/soul-re.svg?mode=shield&label=Code&measure=matched_code_percent
[Data Progress]: https://decomp.dev/fmil95/soul-re.svg?mode=shield&label=Functions&measure=matched_functions
[progress]: https://decomp.dev/fmil95/soul-re

| Name | Purpose | Status |
|------|---------|----------|
| SLUS_007.08 | Gex 2 engine + game code | ![image](https://decomp.dev/fmil95/soul-re.svg?mode=shield&category=main&label=Code&measure=matched_code_percent) ![image](https://decomp.dev/fmil95/soul-re.svg?mode=shield&category=main&label=Functions&measure=matched_functions) |
| cinemax.drm | Video Player overlay | ![image](https://decomp.dev/fmil95/soul-re.svg?mode=shield&category=cinemax&label=Code&measure=matched_code_percent) ![image](https://decomp.dev/fmil95/soul-re.svg?mode=shield&category=cinemax&label=Functions&measure=matched_functions) |
| mcardx.drm | Memory Card overlay | ![image](https://decomp.dev/fmil95/soul-re.svg?mode=shield&category=mcardx&label=Code&measure=matched_code_percent) ![image](https://decomp.dev/fmil95/soul-re.svg?mode=shield&category=mcardx&label=Functions&measure=matched_functions) |
| hunter.drm | Human Hunter overlay | ![image](https://decomp.dev/fmil95/soul-re.svg?mode=shield&category=hunter&label=Code&measure=matched_code_percent) ![image](https://decomp.dev/fmil95/soul-re.svg?mode=shield&category=hunter&label=Functions&measure=matched_functions) |
| skinner.drm | Melchiahim overlay | ![image](https://decomp.dev/fmil95/soul-re.svg?mode=shield&category=skinner&label=Code&measure=matched_code_percent) ![image](https://decomp.dev/fmil95/soul-re.svg?mode=shield&category=skinner&label=Functions&measure=matched_functions) |
| wallcr.drm | Zephonim overlay | ![image](https://decomp.dev/fmil95/soul-re.svg?mode=shield&category=wallcr&label=Code&measure=matched_code_percent) ![image](https://decomp.dev/fmil95/soul-re.svg?mode=shield&category=wallcr&label=Functions&measure=matched_functions) |
| aluka.drm | Rahabim overlay | ![image](https://decomp.dev/fmil95/soul-re.svg?mode=shield&category=aluka&label=Code&measure=matched_code_percent) ![image](https://decomp.dev/fmil95/soul-re.svg?mode=shield&category=aluka&label=Functions&measure=matched_functions) |
| skinbos.drm | Melchiah overlay | ![image](https://decomp.dev/fmil95/soul-re.svg?mode=shield&category=skinbos&label=Code&measure=matched_code_percent) ![image](https://decomp.dev/fmil95/soul-re.svg?mode=shield&category=skinbos&label=Functions&measure=matched_functions) |
| kain.drm | Kain overlay | ![image](https://decomp.dev/fmil95/soul-re.svg?mode=shield&category=kain&label=Code&measure=matched_code_percent) ![image](https://decomp.dev/fmil95/soul-re.svg?mode=shield&category=kain&label=Functions&measure=matched_functions) |
| walboss.drm | Zephon overlay A | ![image](https://decomp.dev/fmil95/soul-re.svg?mode=shield&category=walboss&label=Code&measure=matched_code_percent) ![image](https://decomp.dev/fmil95/soul-re.svg?mode=shield&category=walboss&label=Functions&measure=matched_functions) |
| walbosb.drm | Zephon overlay B | ![image](https://decomp.dev/fmil95/soul-re.svg?mode=shield&category=walbosb&label=Code&measure=matched_code_percent) ![image](https://decomp.dev/fmil95/soul-re.svg?mode=shield&category=walbosb&label=Functions&measure=matched_functions) |
| alukabss.drm | Rahab overlay | ![image](https://decomp.dev/fmil95/soul-re.svg?mode=shield&category=alukabss&label=Code&measure=matched_code_percent) ![image](https://decomp.dev/fmil95/soul-re.svg?mode=shield&category=alukabss&label=Functions&measure=matched_functions) |
| roninbss.drm | Dumah overlay | ![image](https://decomp.dev/fmil95/soul-re.svg?mode=shield&category=roninbss&label=Code&measure=matched_code_percent) ![image](https://decomp.dev/fmil95/soul-re.svg?mode=shield&category=roninbss&label=Functions&measure=matched_functions) |

In case you wonder what an overlay is: Soul Reaver like many other titles for the PlayStation (especially those near the end of the system's life cycle) distributes a good amount of game logic throughout _overlays_. For those not familiar with the concept, overlays are binaries which contain code that is loaded at specific instances during gameplay by the primary executable. This was designed such to taper the maximum amount of memory that the game consumes at once, which could never be too high due to hardware limitations. 

While overlays are fairly common, Crystal Dynamics designed some noteworthy tech to link DRM files dynamically on runtime, which means that the game's overlays never load at the same address during execution. The [un_drm script](/tools/scripts/un_drm.py) circumvents that to allow matching decompilation of the modules.

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
