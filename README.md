# Soul Reaverse

![image](https://i.imgur.com/RTQyAu8.png)

## Description

**_Soul Reaverse_** is a project that seeks to reconstruct the source code of the PSX action-adventure classic _Legacy of Kain: Soul Reaver_. This is achieved by disassembling the executable of a prototype of the game that was built two weeks before its final release, which includes a full set of debugging symbols that makes the process of reverse-engineering the binary's MIPS instructions back to C language easier.

Once its fully overhauled, the source code could then be used for many different purposes, including but not limited to: porting the game to other platforms, improving its performance on console hardware, and developing tools for it such as a level editor.

For the decompilation of the next game in the series, check the [Soul Reaver 2 decompilation project](https://github.com/fmil95/soul-re2).

## Progress

[![Build Status]][actions] [![Code Progress]][progress] [![Data Progress]][progress]

[Build Status]: https://github.com/fmil95/soul-re/actions/workflows/progress_report.yml/badge.svg
[actions]: https://github.com/fmil95/soul-re/actions/workflows/progress_report.yml
[Code Progress]: https://fmil95.github.io/soul-re/badges/all_code.svg
[Data Progress]: https://fmil95.github.io/soul-re/badges/all_functions.svg
[progress]: https://fmil95.github.io/soul-re/

| Name         | Purpose                  | Status                                                                                                                                                                                                                                        |
| ------------ | ------------------------ | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| SLUS_007.08  | Gex 2 engine + game code | ![image](https://fmil95.github.io/soul-re/badges/main_code.svg) ![image](https://fmil95.github.io/soul-re/badges/main_functions.svg)         |
| cinemax.drm  | Video Player overlay     | ![image](https://fmil95.github.io/soul-re/badges/cinemax_code.svg) ![image](https://fmil95.github.io/soul-re/badges/cinemax_functions.svg)   |
| mcardx.drm   | Memory Card overlay      | ![image](https://fmil95.github.io/soul-re/badges/mcardx_code.svg) ![image](https://fmil95.github.io/soul-re/badges/mcardx_functions.svg)     |
| hunter.drm   | Human Hunter overlay     | ![image](https://fmil95.github.io/soul-re/badges/hunter_code.svg) ![image](https://fmil95.github.io/soul-re/badges/hunter_functions.svg)     |
| skinner.drm  | Melchiahim overlay       | ![image](https://fmil95.github.io/soul-re/badges/skinner_code.svg) ![image](https://fmil95.github.io/soul-re/badges/skinner_functions.svg)   |
| wallcr.drm   | Zephonim overlay         | ![image](https://fmil95.github.io/soul-re/badges/wallcr_code.svg) ![image](https://fmil95.github.io/soul-re/badges/wallcr_functions.svg)     |
| aluka.drm    | Rahabim overlay          | ![image](https://fmil95.github.io/soul-re/badges/aluka_code.svg) ![image](https://fmil95.github.io/soul-re/badges/aluka_functions.svg)       |
| skinbos.drm  | Melchiah overlay         | ![image](https://fmil95.github.io/soul-re/badges/skinbos_code.svg) ![image](https://fmil95.github.io/soul-re/badges/skinbos_functions.svg)   |
| kain.drm     | Kain overlay             | ![image](https://fmil95.github.io/soul-re/badges/kain_code.svg) ![image](https://fmil95.github.io/soul-re/badges/kain_functions.svg)         |
| walboss.drm  | Zephon overlay A         | ![image](https://fmil95.github.io/soul-re/badges/walboss_code.svg) ![image](https://fmil95.github.io/soul-re/badges/walboss_functions.svg)   |
| walbosb.drm  | Zephon overlay B         | ![image](https://fmil95.github.io/soul-re/badges/walbosb_code.svg) ![image](https://fmil95.github.io/soul-re/badges/walbosb_functions.svg)   |
| alukabss.drm | Rahab overlay            | ![image](https://fmil95.github.io/soul-re/badges/alukabss_code.svg) ![image](https://fmil95.github.io/soul-re/badges/alukabss_functions.svg) |
| roninbss.drm | Dumah overlay            | ![image](https://fmil95.github.io/soul-re/badges/roninbss_code.svg) ![image](https://fmil95.github.io/soul-re/badges/roninbss_functions.svg) |

In case you wonder what an overlay is: Soul Reaver like many other titles for the PlayStation (especially those near the end of the system's life cycle) distributes a good amount of game logic throughout _overlays_. For those not familiar with the concept, overlays are binaries which contain code that is loaded at specific instances during gameplay by the primary executable. This was designed such to taper the maximum amount of memory that the game consumes at once, which could never be too high due to hardware limitations.

While overlays are fairly common, Crystal Dynamics designed some noteworthy tech to link DRM files dynamically on runtime, which means that the game's overlays never load at the same address during execution. The `undrm` function of [cd-dat-utils](https://github.com/StableCrimson/cd-dat-utils) circumvents that, allowing decompilation of the modules.

### Setting Up

For first time setup, check out the [installation guide](docs/INSTALL.MD).

### Contributing

Thinking about contributing to the project? Check out the [contribution guide](docs/CONTRIBUTING.MD) to get started!

### Reference

New to the PSX, decompilation as a whole, or just stuck on a function? Check out the [decompilation reference](/docs/DECOMPILATION.MD) which has several common code patterns broken down and documented.

### Contact

The **PS1/PS2 Decompilation** Discord server has an exclusive channel for this decompilation titled **#soul-reaver**, which is the main medium of communication for anything in relation to it: [REDACTED].

Alternatively, if you aren't able to join the server for some reason, you can write me an email to <frozenburnside@hotmail.com> with any query you might have.
