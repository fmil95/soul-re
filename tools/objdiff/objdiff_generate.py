# Adapted from https://github.com/Vatuu/silent-hill-decomp/tree/master

from pathlib import Path
from argparse import ArgumentParser
from dataclasses import dataclass, asdict
import logging
import yaml
import json

@dataclass
class UnitMetadata:
    progress_categories: list[str]

@dataclass
class Unit:
    name: str
    base_path: str
    target_path: str
    metadata: UnitMetadata

@dataclass
class ProgressCategory:
    id: str
    name: str

@dataclass
class Config:
    build_base: bool
    build_target: bool
    custom_make: str
    custom_args: list[str]
    units: list[Unit]
    progress_categories: list[ProgressCategory]


def _create_config():
    parser = ArgumentParser()
    parser.add_argument("config", type = Path)
    args = parser.parse_args()

    if not args.config.exists() or args.config.is_dir() or args.config.suffix != ".yaml":
        raise ValueError(f"The given path {args.objects} is not pointing towards a valid config.")

    with open(args.config) as stream:
        try:
            return yaml.safe_load(stream)
        except yaml.YAMLError as exc:
            raise exc

EXCLUDED_NAMES = {"data", "rodata", "sdata", "bss"}

def _collect_objects(path: Path, config) -> list[Path]:
    ignored = config["ignored_files"]
    return [
        path for path in path.rglob("*.o")
        if not any(name in path.name for name in EXCLUDED_NAMES ) and not any(file in str(path) for file in ignored)
    ]

def _determine_categories(path: Path, config) -> tuple[UnitMetadata, str]:
    if path.name.endswith(".s.o"):
        modified_path = path.relative_to(config["expected_paths"]["asm"])
    else:
        modified_path = path.relative_to(config["expected_paths"]["src"])

    categories = []
    for category in config["categories"]:
        for prefix in category["paths"]:
            if str(modified_path).startswith(prefix):
                categories.append(category["id"])
    return (UnitMetadata(categories), str(modified_path))

def _get_base_path(path: Path) -> Path:
    name = "build/src/" + path.removesuffix(".s.o").removesuffix(".c.o")
    c_path = name + ".c.o"
    hasm_path = name + ".hasm.s.o"
    if Path(c_path).exists(): return c_path
    if Path(hasm_path).exists(): return hasm_path
    return None

def main():
    logging.basicConfig(level = logging.INFO)
    config = _create_config()

    expected_objects = _collect_objects(Path(config["expected_paths"]["asm"]), config) + _collect_objects(Path(config["expected_paths"]["src"]), config)
    
    logging.info(f"Accounting for {len(expected_objects)} objects.")
    units = []
    for file in expected_objects:
        processed_path = _determine_categories(file, config)
        unit = Unit(
            processed_path[1].removesuffix(".s.o").removesuffix(".c.o"),
            _get_base_path(processed_path[1]),
            str(file),
            processed_path[0])
        units.append(unit)
    
    categories = []
    for category in config["categories"]:
        categories.append(ProgressCategory(category["id"], category["name"]))
    
    with (Path(config["output"])).open("w") as json_file:
        json.dump(asdict(Config(True, False, "make", ["progress"], units, categories)), json_file, indent=2)

if __name__ == "__main__":
    main()