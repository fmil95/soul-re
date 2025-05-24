# Soul Reaverse

<img src="https://i.imgur.com/RTQyAu8.png"/>

## Description

***Soul Reaverse*** is a project that seeks to reconstruct the source code of the PSX action-adventure classic *Legacy of Kain: Soul Reaver*. This is achieved by disassembling the executable of a prototype of the game that was built two weeks before its final release, which includes a full set of debugging symbols that makes the process of reverse-engineering the binary's MIPS instructions back to C language easier.

Once its fully overhauled, the source code could then be used for many different purposes, including but not limited to: porting the game to other platforms, improving its performance on console hardware, and developing tools for it such as a level editor.  

Next is a benchmark for the progress achieved so far on the decompilation of the main game executable (**SLUS_007.08**): 
```
1956 matched functions / 2274 total (86.02%)
509776 matching bytes / 653008 total (78.07%)
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
 
## Contact

The **PS1/PS2 Decompilation** Discord server has an exclusive channel for this decompilation titled **#soul-reaver**, which is the main medium of communication for anything in relation to it: https://discord.gg/VwCPdfbxgm

Alternatively, if you aren't able to join the server for some reason, you can write me an email to fmilesi_soft@hotmail.com with any query you might have. 
