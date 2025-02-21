# Soul Reaverse

<img src="https://i.imgur.com/RTQyAu8.png"/>

## About

Soul Reaverse is a project that seeks to reconstruct the source code of the PSX action-adventure classic Legacy of Kain: Soul Reaver. This is achieved by disassembling the executable of a prototype of the game that was built two weeks before its final release, which includes a full set of debugging symbols that makes the process of reverse-engineering the binary's MIPS instructions back to C language easier.

The current progress of the project is 77% completion, excluding overlays.

Discord server: https://discord.gg/VwCPdfbxgm

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
Clone `https://github.com/FedericoMilesi/soul-re` in whatever directory you wish. Make sure to clone recursively!
```
git clone --recursive https://github.com/FedericoMilesi/soul-re.git
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
