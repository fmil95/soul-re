import argparse
import hashlib
import sys
from dataclasses import dataclass
from enum import IntEnum
from io import BytesIO
from pathlib import Path
from struct import pack, unpack
from typing import BinaryIO

MODULE_BASE = 0x88000000

def read_s16(f: BinaryIO, peek: bool = False) -> int:
    val = unpack("<h", f.read(2))[0]
    if peek:
        f.seek(-2, 1)
    return val
def write_s16(f: BinaryIO, val: int) -> int:
    f.write(pack("<H", val & 0xFFFF))
def read_s32(f: BinaryIO, peek: bool = False) -> int:
    val = unpack("<i", f.read(4))[0]
    if peek:
        f.seek(-4, 1)
    return val
def read_u32(f: BinaryIO, peek: bool = False) -> int:
    val = unpack("<I", f.read(4))[0]
    if peek:
        f.seek(-4, 1)
    return val
def write_u32(f: BinaryIO, val: int) -> int:
    f.write(pack("<I", val & 0xFFFFFFFF))

class RelocType(IntEnum):
    R_MIPS_32 = 0
    R_MIPS_HI16 = 1
    R_MIPS_LO16 = 2
    R_MIPS_26 = 3

@dataclass
class Reloc:
    type: RelocType
    addr: int
    addend: int = 0


def parse_args():
    parser = argparse.ArgumentParser(
        description="Extract RELMOD overlays and make yaml"
    )

    parser.add_argument(
        "--input", 
        type=Path, 
        required=True,
        help="Path to file.drm to extract"
    )
    parser.add_argument(
        "--output",
        type=Path,
        required=True,
        help="Path to folder where to place files"
    )

    return parser.parse_args()


def read_relocs(f: BinaryIO) -> list[Reloc]:
    relocs: list[Reloc] = []
    while True:
        val = read_s32(f)
        if val == -1:
            break
        
        rel_type = val & 0x3
        rel_addr = val & ~0x3
        rel = Reloc(RelocType(rel_type), rel_addr)

        if rel.type == RelocType.R_MIPS_HI16:
            rel.addend = read_s32(f)
        relocs.append(rel)
    
    return relocs


def get_relmod(f: BinaryIO) -> tuple[list[Reloc], bytes]:
    # Skip RedirectList
    count = read_s32(f)
    f.seek(count * 4 + 4)
    while (f.tell() % 0x800) != 0:
        f.read(1)

    # Save RELMOD offset
    relmod_off = f.tell()

    # Only grab relocs and the module
    f.seek(relmod_off + 0x3C)
    rel_off = read_s32(f)
    mod_off = read_s32(f)

    if mod_off == 0:
        print("No MODULE in DRM file")
        sys.exit(-1)

    if rel_off == 0:
        print("No relocs in MODULE")
    
    f.seek(relmod_off + rel_off)
    relocs: list[Reloc] = read_relocs(f)
    
    f.seek(relmod_off + mod_off)
    mod_bytes = f.read()

    return relocs, mod_bytes


def main():
    args = parse_args()

    drm_path: Path = args.input
    out_path: Path = args.output

    with drm_path.open("rb") as f:
       relocs, mod_bytes = get_relmod(f)

    mod_len = len(mod_bytes)
    mod_new = BytesIO(mod_bytes)
    
    # Apply relocs
    for rel in relocs:
        mod_new.seek(rel.addr)
        r = read_u32(mod_new, True)
        top = r & 0xFFFF0000

        if rel.type == RelocType.R_MIPS_32:
            if r >= 0:
                write_u32(mod_new, r + MODULE_BASE)
        elif rel.type == RelocType.R_MIPS_HI16:
            imm = ((rel.addend + MODULE_BASE + 0x8000) >> 16) & 0xFFFF
            write_u32(mod_new, top | imm)
        elif rel.type == RelocType.R_MIPS_LO16:
            imm = (r + MODULE_BASE) & 0xFFFF
            write_u32(mod_new, top | imm)
        elif rel.type == RelocType.R_MIPS_26:
            write_u32(mod_new, r + ((MODULE_BASE // 4) & 0x03FFFFFF))
    
    mod_new.seek(0)
    module_sha = hashlib.file_digest(mod_new, "sha1").hexdigest()

    out_path.mkdir(parents=True, exist_ok=True)

    mod_name = drm_path.stem
    yml_path = out_path / f"{mod_name}.yaml"
    m_mod_path = out_path / f"{mod_name}.bin"
    o_mod_path = out_path / f"{mod_name}.orig.bin"

    with m_mod_path.open("wb") as f:
        f.write(mod_new.read())
    with o_mod_path.open("wb") as f:
        f.write(mod_bytes)
    
    with yml_path.open("w", encoding="utf8") as f:
        s = get_yaml_string(mod_name, mod_len, module_sha)
        f.write(s)


def get_yaml_string(name: str, eof: int, sha_hash: str) -> str:
    out = []
    out.append(f"name: {name} (Overlay)")
    out.append(f"sha1: {sha_hash}")
    out.append(f"options:")
    out.append(f"  basename: KAIN2_{name.upper()}")
    out.append(f"  target_path: {name}.bin")
    out.append(f"  base_path: .")
    out.append(f"  platform: psx")
    out.append(f"  compiler: gcc")
    out.append(f"  build_path: BUILD_PATH")
    out.append(f"  ld_script_path: {name}.ld")
    out.append(f"  find_file_boundaries: False")
    out.append(f"  use_legacy_include_asm: False")
    out.append(f"  gp_value: 0x800D7598")
    out.append(f"  section_order: [\".rodata\", \".text\", \".data\", \".bss\"]")
    out.append(f"  symbol_addrs_path:")
    out.append(f"    - symbol_addrs.txt")
    out.append(f"    - symbol_addrs.{name}.txt")
    out.append(f"  reloc_addrs_path:")
    out.append(f"    - reloc_addrs.{name}.txt")
    out.append(f"  extensions_path: tools/splat_ext")
    out.append(f"  string_encoding: ASCII")
    out.append(f"  rodata_string_guesser_level: 2")
    out.append(f"  data_string_encoding: ASCII")
    out.append(f"  data_string_guesser_level: 2")
    out.append(f"  subalign: 4")
    out.append(f"  migrate_rodata_to_functions: True")
    out.append(f"  hasm_in_src_path: True")
    out.append(f"  make_full_disasm_for_code: True")
    out.append(f"  generate_asm_macros_files: False")
    out.append(f"  asm_data_macro: \"glabel\"")
    out.append(f"  asm_data_end_label: \"\"")
    out.append(f"  asm_end_label: \"\"")
    out.append(f"  asm_function_alt_macro: glabel")
    out.append(f"  asm_jtbl_label_macro: jlabel")
    out.append(f"  asm_nonmatching_label_macro: \"\"")
    out.append(f"  ld_bss_is_noload: False")
    out.append(f"  global_vram_start: 0x80010000")
    out.append(f"  global_vram_end: 0x800E0000")
    # out.append(f"  disasm_unknown: True")
    out.append(f"")
    out.append(f"segments:")
    out.append(f"  - name: main")
    out.append(f"    type: code")
    out.append(f"    start: 0x00")
    out.append(f"    vram: 0x{MODULE_BASE:08X}")
    out.append(f"    align: 4")
    out.append(f"    # bss_size: 0x#### # Fill me!")
    out.append(f"    bss_contains_common: True")
    out.append(f"    subsegments:")
    out.append(f"      # - [0x######, rodata, overlay/] # Pick the correct rodata/text/data splits")
    out.append(f"      - [0x000000, asm, overlay/{name}/asm]")
    out.append(f"  - [0x{eof:06X}]")

    return "\n".join(out)


if __name__ == "__main__":
    main()
