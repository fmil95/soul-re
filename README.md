# Soul Reaverse

<img src="https://i.imgur.com/RTQyAu8.png"/>

## Description

Soul Reaverse is a project that seeks to reconstruct the source code of the PSX action-adventure classic Legacy of Kain: Soul Reaver. This is achieved by disassembling the executable of a prototype of the game that was built two weeks before its final release, which includes a full set of debugging symbols that makes the process of reverse-engineering the binary's MIPS instructions back to C language easier.

Once its fully overhauled, the source code could then be used for many different purposes, including but not limited to: porting the game to other platforms, improving its performance on console hardware, and developing tools for it such as a level editor.  

Next is a benchmark for the progress achieved so far on the decompilation of the main game executable (SLUS_007.08): 
```
1803 matched functions / 2274 total (79.29%)
452112 matching bytes / 653008 total (69.24%)
```

Note that the status above is not reflective of the actual completion rate of the project however, as Soul Reaver like many other titles for the PlayStation (especially those near the end of the system's life cycle) distributes a good amount of game logic throughout overlays. For those not familiar with the concept, overlays are binaries which contain code that is loaded at specific instances during gameplay by the primary executable. This was designed such to taper the maximum amount of memory that the game consumes at once, which could never be too high due to hardware limitations. A complete list of the overlays will be published on this repository soon.  

## Building (Linux)

### Install build dependencies
The build process has the following package requirements:
- git
- build-essential
- binutils-mips-linux-gnu
- python3
- bchunk
- 7z

Under a Debian based distribution, you can install these with the following commands:
```
sudo apt update
sudo apt install git build-essential binutils-mips-linux-gnu python3 bchunk p7zip-full p7zip-rar
```

### Clone the repository
Clone `https://github.com/fmil95/soul-re` in whatever directory you wish. Make sure to clone recursively!
```
git clone --recursive https://github.com/fmil95/soul-re.git
cd soul-re
```

### Install Python3 requirements
Run `python3 -m install -r requirements.txt`

### Placing the ROM
You will need to provide your own rom dump of the game. The required version is the 1999-07-14 Prototype Version of Legacy of Kain: Soul Reaver.
If done correctly, you will end up with a .IMG and a .CUE file that you can mount on your PC to get the required binary, SLUS_007.08, which needs to be placed inside the `soul-re` folder.

### Build the code
Run `make setup` to extract needed stuff from the SLUS_007.08 file, if that succeeds, run `make -j12` to build it.
Once build has finished a folder will be produced with the name `build`, inside this, you will find the output.

## Contributing
Contributions are welcome. If you would like to reserve a function, open a PR with the function or file name(s).

## Community

The primary way to discuss the project is through the `PS1/PS2 Decompilation` server on Discord, which has a dedicated channel for the decomp called `soul-reaver`. See you there!

[![Join the Discord](https://dcbadge.limes.pink/api/server/https://discord.gg/tch8h5Vw8E)](https://discord.gg/https://discord.gg/tch8h5Vw8E)

## Resources

Associated decomp projects:
- [Resident Evil CODE: Veronica X](https://github.com/fmil95/recvx-decomp)
- [Fatal Frame](https://github.com/Mikompilation/Himuro)
- [Silent Hill](https://github.com/Vatuu/silent-hill-decomp)

## Disclaimer

This repository and the code therein, is made public on this site by fmil95 for preservational and educational purposes. I do not claim to hold any affiliation to the Legacy of Kain franchise, Crystal Dynamics, Inc. or any other related party, nor mean to infringe upon the copyright of Legacy of Kain: Soul Reaver.
