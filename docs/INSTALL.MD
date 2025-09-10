# Installation and Project Setup

## Executable and IDE

### Fetching the Binary

The commercial releases of Soul Reaver don't have debugging symbols, except for the German release. However, the .SYM file that is available on that version's CD-ROM is from an earlier build of the game, which would render reverse-engineering it confusing as the symbols would be mapped to different addresses in memory and have breaking changes in regards to the actual compiled data on the binary. Instead, you must download the **1999-07-14 build** which can be found on Internet Archive, and unpack it to get the `SLUS_007.08` file.

Once `SLUS_007.08` has been extracted, place it at the root of the project.

### IDE and Plugins

The preferred _Integrated Development Environment (IDE)_ for this project is **Visual Studio Code 1.99.3 or higher**, which can be freely obtained on its official website.

After installing Visual Studio Code, it is very much recommended that you add the following Extensions to it:

- C/C++ Extension Pack
- CMake Tools
- Makefile Tools
- Python
- WSL (if using Windows)

## Cloning the Repository

Soul Reaverse has a few submodules, so be sure to clone those with the `--recursive` flag:

```bash
git clone --recursive https://github.com/fmil95/soul-re.git
```

## Development Environment Setup

### Windows

For Windows, make sure **Windows Subsystem for Linux** (WSL) is installed and set up. From there, follow the Linux instructions in the WSL environment.

### Linux (and WSL)

Install the required dependencies:

```bash
sudo apt update
sudo apt install git build-essential binutils-mips-linux-gnu cpp-mips-linux-gnu python3 bchunk p7zip-full p7zip-rar
```

### MacOS

For MacOS development (Intel or ARM), install [Docker Desktop](https://www.docker.com/products/docker-desktop/) and run `./build_image.sh` to build the development container.

To run any of the make commands in the container, you can feed them to the `docker_run.sh` script like so:

```bash
./docker_run.sh make clean
```

### Python Setup

Though not required, it is recommended to create a Python virtual environment for the project:

```bash
python -m venv .venv
source .venv/bin/activate
```

Now that the repo is cloned and the developnent utilities are set up, install the project's Python dependencies with:

```bash
pip install -r requirements.txt
```

## Splitting and Building

With everything in place, run:

```bash
make setup
```

That will split the executable into individual assembly files. As a final test, run the build command:

```bash
make -j
```

If the build runs successfully, then setup is complete!
