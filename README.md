# Soul Reaverse

<img src="https://i.imgur.com/RTQyAu8.png"/>

## Description

***Soul Reaverse*** is a project that seeks to reconstruct the source code of the PSX action-adventure classic *Legacy of Kain: Soul Reaver*. This is achieved by disassembling the executable of a prototype of the game that was built two weeks before its final release, which includes a full set of debugging symbols that makes the process of reverse-engineering the binary's MIPS instructions back to C language easier.

Once its fully overhauled, the source code could then be used for many different purposes, including but not limited to: porting the game to other platforms, improving its performance on console hardware, and developing tools for it such as a level editor.  

Next is a benchmark for the progress achieved so far on the decompilation of the main game executable (**SLUS_007.08**): 
```
1911 matched functions / 2274 total (84.04%)
489004 matching bytes / 653008 total (74.88%)
```

Note that the status above is not reflective of the actual completion rate of the project however, as Soul Reaver like many other titles for the PlayStation (especially those near the end of the system's life cycle) distributes a good amount of game logic throughout *overlays*. For those not familiar with the concept, overlays are binaries which contain code that is loaded at specific instances during gameplay by the primary executable. This was designed such to taper the maximum amount of memory that the game consumes at once, which could never be too high due to hardware limitations.

A complete list of overlays will be published on this repository soon.  

## Installation

**Notice:** The process outlined below for setting up Soul Reaverse locally on your machine will only work on Linux, because much of the required software isn't compatible with Windows. The only way to install the development environment on that operating system is through the *Windows Subsystem for Linux (WSL)*, which if you don't have it already enabled you should look for some guide on the net on how to use it.  

### Fetching the ROM

The commercial releases of Soul Reaver don't have debugging symbols, except for the German release. However, the .SYM file that is available on that version's CD-ROM is from an earlier build of the game, which would render reverse-engineering it confusing as the symbols would be mapped to different addresses in memory and have breaking changes in regards to the actual compiled data on the binary. Instead, you must download the **1999-07-14 build** which can be found on Internet Archive, and glean the SLUS_007.08 file from there. 

### Installing the IDE and Plugins

The preferred *Integrated Development Environment (IDE)* for this project is **Visual Studio Code 1.99.3 or higher**, which can be freely obtained on its official website.

After installing Visual Studio Code, it is very much recommended that you add the following Extensions to it:
- C/C++ Extension Pack
- CMake Tools
- Makefile Tools
- Python
- WSL (not necessary if you are on Linux)

### Installing the required packages

You'll need to have the following packets installed on your system:
- git
- build-essential
- binutils-mips-linux-gnu
- python3
- bchunk
- p7zip-full
- p7zip-rar

You might already have some of these but if you don't have any of them or are missing a select few, you can grab them all with the following two Bash commands:
```
sudo apt update
sudo apt install git build-essential binutils-mips-linux-gnu python3 bchunk p7zip-full p7zip-rar
```

### Cloning the repository

In order to clone this repository locally on your desktop, you can enter the next Bash command in the directory where you want to put up the decompile:  
```
git clone --recursive https://github.com/fmil95/soul-re.git
```

If you want to do the cloning by some other means, don't forget to do it recursively or else the submodules in the **tools** folder of the project won't be copied correctly.

### Placing the ROM

Now that you have set up the repository, place the SLUS_007.08 file that you obtained earlier from the prototype's disc in the root folder.  

### Installing the Python 3 prerequisites

On the root directory which is where the **requirements.txt** file is, run this command on your terminal to install the Python 3 essentials for the project: 
```
python3 -m install -r requirements.txt
```

### Building the code

Enter `make setup` on the terminal to extract the source assets from the ROM, then input `make -j12` to compile the binary. If the build process is succesfully performed a folder named `build` will appear, which is where you'll find the result: **KAIN2.EXE**. 

In case of an error, you can use `make clean` to do this step once more from scratch.
 
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
