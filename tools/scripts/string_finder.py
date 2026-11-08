import argparse
import os
import re
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


def strs_defined_in_obj(obj_path: Path, target_strs: list[str]) -> list[str]:

    lines = (
        subprocess.run(
            ["strings", obj_path.absolute()],
            check=False,
            capture_output=True,
            text=True,
        )
        .stdout.strip()
        .splitlines()
    )

    found_strs = []

    for line in lines:
        line = line.strip()

        if any(re.search(target, line) is not None for target in target_strs):
            found_strs.append(line)

    return found_strs


def main():

    parser = argparse.ArgumentParser(
        description="Given a path to the PsyQ `lib` folder, attempts to find the object file(s) containing target symbols"
    )

    parser.add_argument("lib_path", help="Path to PsyQ `lib` folder")
    parser.add_argument(
        "targets", nargs="+", help="Space-separated list of symbol names to find"
    )

    args = parser.parse_args()

    if not Path(args.lib_path).exists():
        print(f"{args.lib_path} does not exist!")
        sys.exit(1)

    if not Path(args.lib_path).is_dir():
        print(f"{args.lib_path} does is not a folder!")
        sys.exit(1)

    files = collect_objs(Path(args.lib_path))

    for file in files:
        for found_str in strs_defined_in_obj(file, args.targets):
            print(f"String '{found_str}' found in {file}")


if __name__ == "__main__":
    main()
