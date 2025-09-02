# Soul Reaverse

![image](https://i.imgur.com/RTQyAu8.png)

## Description

**_Soul Reaverse_** is a project that seeks to reconstruct the source code of the PSX action-adventure classic _Legacy of Kain: Soul Reaver_. This is achieved by disassembling the executable of a prototype of the game that was built two weeks before its final release, which includes a full set of debugging symbols that makes the process of reverse-engineering the binary's MIPS instructions back to C language easier.

Once its fully overhauled, the source code could then be used for many different purposes, including but not limited to: porting the game to other platforms, improving its performance on console hardware, and developing tools for it such as a level editor.

## Progress

| Name | Game ID | Progress |
|------|---------|----------|
| Legacy of Kain: Soul Reaver | SLUS_007.08 | ![image](https://decomp.dev/fmil95/soul-re.svg?mode=shield&label=Code&measure=matched_code_percent) ![image](https://decomp.dev/fmil95/soul-re.svg?mode=shield&label=Functions&measure=matched_functions)|

Note that the status above is not reflective of the actual completion rate of the project however, as Soul Reaver like many other titles for the PlayStation (especially those near the end of the system's life cycle) distributes a good amount of game logic throughout _overlays_. For those not familiar with the concept, overlays are binaries which contain code that is loaded at specific instances during gameplay by the primary executable. This was designed such to taper the maximum amount of memory that the game consumes at once, which could never be too high due to hardware limitations.

A complete list of overlays will be published on this repository soon.

## Installation

**Notice:** If not using Docker, the process outlined below for setting up Soul Reaverse locally on your machine will only work on Linux, because much of the required software isn't compatible with Windows. The only way to install the development environment on that operating system is through the _Windows Subsystem for Linux (WSL)_, which if you don't have it already enabled you should look for some guide on the net on how to use it.

### Developing with Docker

Docker can be used to work on this project independent of your operating system. To do so, first make sure you have Docker installed.
To build the Docker image for the first time, just run:

```bash
./build_image.sh
```

commands can be passed to the container using `docker_run.sh`. Ex:

```bash
./docker_run.sh make setup
```

### Fetching the ROM

The commercial releases of Soul Reaver don't have debugging symbols, except for the German release. However, the .SYM file that is available on that version's CD-ROM is from an earlier build of the game, which would render reverse-engineering it confusing as the symbols would be mapped to different addresses in memory and have breaking changes in regards to the actual compiled data on the binary. Instead, you must download the **1999-07-14 build** which can be found on Internet Archive, and glean the SLUS_007.08 file from there.

### Installing the IDE and Plugins

The preferred _Integrated Development Environment (IDE)_ for this project is **Visual Studio Code 1.99.3 or higher**, which can be freely obtained on its official website.

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
- cpp-mips-linux-gnu
- python3
- bchunk
- p7zip-full
- p7zip-rar

You might already have some of these but if you don't have any of them or are missing a select few, you can grab them all with the following two Bash commands:

```bash
sudo apt update
sudo apt install git build-essential binutils-mips-linux-gnu cpp-mips-linux-gnu python3 bchunk p7zip-full p7zip-rar
```

### Cloning the repository

In order to clone this repository locally on your desktop, you can enter the next Bash command in the directory where you want to put up the decompile:

```bash
git clone --recursive https://github.com/fmil95/soul-re.git
```

If you want to do the cloning by some other means, don't forget to do it recursively or else the submodules in the **tools** folder of the project won't be copied correctly.

### Placing the ROM

Now that you have set up the repository, place the SLUS_007.08 file that you obtained earlier from the prototype's disc in the root folder.

### Installing the Python 3 prerequisites

On the root directory which is where the **requirements.txt** file is, run this command on your terminal to install the Python 3 essentials for the project:

```bash
python3 -m pip install -r requirements.txt
```

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

### Building the code

Enter `make setup` on the terminal to extract the source assets from the ROM, then input `make -j12` to compile the binary. If the build process is succesfully performed a folder named `build` will appear, which is where you'll find the result: **KAIN2.EXE**.

In case of an error, you can use `make clean` to do this step once more from scratch.

## Contact

The **PS1/PS2 Decompilation** Discord server has an exclusive channel for this decompilation titled **#soul-reaver**, which is the main medium of communication for anything in relation to it: [REDACTED].

Alternatively, if you aren't able to join the server for some reason, you can write me an email to frozenburnside@hotmail.com with any query you might have.