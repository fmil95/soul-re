import argparse
import os
import shutil
import subprocess
import sys
from pathlib import Path


def collect_objs(dir: Path) -> list[Path]:

    items = os.listdir(dir)
    files = []

    for item in items:
        item_path = dir / item

        if item_path.is_dir():
            files.extend(collect_objs(item_path))
        elif item_path.suffix == ".o":
            files.append(item_path)

    return files


def symbols_defined_in_obj(obj_path: Path, target_symbols: list[str]) -> list[str]:

    lines = (
        subprocess.run(
            ["mips-linux-gnu-objdump", "-t", obj_path.absolute()],
            check=False,
            capture_output=True,
            text=True,
        )
        .stdout.strip()
        .splitlines()[3:]
    )  # Discard info lines, only want the symbols

    found_syms = []

    for line in lines:
        *_base_addr_and_flags, section, _size, symbol = line.strip().split()

        if section != "*UND*" and symbol in target_symbols:
            found_syms.append(symbol)

    return found_syms


def main():

    parser = argparse.ArgumentParser(
        description="Given a path to the PsyQ `lib` folder, attempts to find the object file(s) containing target symbols"
    )

    parser.add_argument("lib_path", help="Path to PsyQ `lib` folder")
    parser.add_argument(
        "targets", nargs="+", help="Space-separated list of symbol names to find"
    )
    parser.add_argument(
        "--obj-info",
        default=False,
        action="store_true",
        required=False,
        help="When set, print out the headers and symbols of the found object(s)",
    )

    args = parser.parse_args()

    if shutil.which("mips-linux-gnu-objdump") is None:
        print("Cannot find 'mips-linux-gnu-objdump'!")
        sys.exit(1)

    if not Path(args.lib_path).exists():
        print(f"{args.lib_path} does not exist!")
        sys.exit(1)

    if not Path(args.lib_path).is_dir():
        print(f"{args.lib_path} does is not a folder!")
        sys.exit(1)

    files = collect_objs(Path(args.lib_path))
    objs = set()

    for file in files:
        for found_sym in symbols_defined_in_obj(file, args.targets):
            print(f"Symbol '{found_sym}' found in {file}")
            objs.add(file)

    if args.obj_info:
        for obj in objs:
            print(
                subprocess.run(
                    ["mips-linux-gnu-objdump", "-ht", obj.absolute()],
                    check=False,
                    capture_output=True,
                    text=True,
                ).stdout
            )


if __name__ == "__main__":
    main()
