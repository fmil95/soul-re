
import os, struct, csv, json, sys

DEFAULT_BASE = 0x80000000

def read_u32_le(b, o): return struct.unpack_from("<I", b, o)[0]
def read_i32_le(b, o): return struct.unpack_from("<i", b, o)[0]
def read_u16_le(b, o): return struct.unpack_from("<H", b, o)[0]
def read_i16_le(b, o): return struct.unpack_from("<h", b, o)[0]

def parse_top_header(data):
    if len(data) < 8: return []
    count = read_u32_le(data, 0)
    if count <= 0 or 4 + 4*count > len(data): return []
    offs = list(struct.unpack_from("<%dI" % count, data, 4))
    # keep strictly increasing offsets within file
    size=len(data)
    cleaned=[]
    last=0
    for o in offs:
        if 0 < o < size and o > last:
            cleaned.append(o)
            last=o
        else:
            break
    return cleaned

def try_redirectlist(chunk):
    """Detect and extract RedirectList-style relocations from the start of the chunk.
       Returns dict or None.
    """
    if len(chunk) < 8: return None
    num = read_u32_le(chunk, 0)
    # Heuristics: num shouldn't be insane; must fit; pointers region must exist
    max_num = min(200000, (len(chunk)//4)-1)
    if num <= 0 or num > max_num: return None
    # pointers area: next num dwords
    ptrs_off = 4
    ptrs_end = 4 + num*4
    if ptrs_end > len(chunk): return None
    # Compute table size in words (rounded to 512) -> bytes
    table_words = ((num + 512) // 512) * 512
    table_bytes = table_words * 4
    if table_bytes > len(chunk): return None
    module = bytearray(chunk[table_bytes:])
    module_size = len(module)
    # Validate pointer offsets (byte offsets into module)
    ptrs = list(struct.unpack_from("<%dI" % num, chunk, ptrs_off))
    valid= True
    for p in ptrs[: min(num, 4096)]:  # sample up to 4096 to keep perf sane
        if p >= module_size:
            valid=False; break
    if not valid: return None
    return {
        "type": "redirect",
        "num": num,
        "table_bytes": table_bytes,
        "ptrs": ptrs,
        "module_raw": bytes(module),
    }

def apply_redirectlist(module_raw, ptrs, baseaddr):
    out = bytearray(module_raw)
    for off in ptrs:
        if off+4 <= len(out):
            val = read_u32_le(out, off)
            val = (val + baseaddr) & 0xFFFFFFFF
            struct.pack_into("<I", out, off, val)
    return bytes(out)

def decode_relmod_table(rel_bytes, module_size):
    """Return (entries, consumed_bytes) or (None, 0)."""
    pos=0
    entries=[]
    try:
        while pos + 4 <= len(rel_bytes):
            word = read_i32_le(rel_bytes, pos)
            if word == -1:
                pos += 4
                break
            uword = read_u32_le(rel_bytes, pos)
            typ = uword & 0x3
            off = uword & ~0x3
            if typ not in (0,1,2,3): return None, 0
            if off >= module_size: return None, 0
            pos += 4
            extra=None
            if typ == 1:
                if pos + 4 > len(rel_bytes): return None, 0
                extra = read_u32_le(rel_bytes, pos)
                pos += 4
            entries.append((typ, off, extra))
            if len(entries) > 500000: break
        if not entries: return None, 0
        return entries, pos
    except struct.error:
        return None, 0

def try_relmod(chunk):
    """Brute-force search inside chunk for a plausible RELMOD relocation table.
       Assume module == full chunk.
    """
    size = len(chunk)
    best=None
    best_score = -1
    # quick scan windows to limit cost
    for start in range(0, min(size-16, 65536), 4):  # limit where table begins to first 64KB by default
        # quick filter: must contain -1 within next 8KB
        window = chunk[start : min(size, start+8192)]
        if b"\xFF\xFF\xFF\xFF" not in window: continue
        entries, consumed = decode_relmod_table(window, size)
        if not entries: continue
        # score: more entries and shorter table is better
        score = len(entries)*2 - consumed//64
        if score > best_score:
            best_score = score
            best = {"start": start, "entries": entries, "consumed": consumed}
    return best

def apply_relmod(module_raw, entries, baseaddr):
    out = bytearray(module_raw)
    for typ, off, extra in entries:
        if off >= len(out): continue
        if typ == 0:
            val = read_i32_le(out, off)
            if val >= 0:
                struct.pack_into("<I", out, off, (val + baseaddr) & 0xFFFFFFFF)
        elif typ == 1:
            hi = ((baseaddr + (extra or 0)) + 32768) >> 16
            struct.pack_into("<H", out, off, hi & 0xFFFF)
        elif typ == 2:
            val = read_i16_le(out, off)
            struct.pack_into("<H", out, off, ( (val + baseaddr) & 0xFFFF ))
        elif typ == 3:
            val = read_u32_le(out, off)
            adj = ((baseaddr << 4) >> 6) & 0xFFFFFFFF
            struct.pack_into("<I", out, off, (val + adj) & 0xFFFFFFFF)
    return bytes(out)

def process_drm(path, outdir, baseaddr=DEFAULT_BASE):
    with open(path, "rb") as f: data=f.read()
    offs = parse_top_header(data)
    if not offs:
        print("No valid top-level header found.")
        return []
    results=[]
    for i,o in enumerate(offs):
        end = offs[i+1] if i+1 < len(offs) else len(data)
        chunk = data[o:end]
        kind=None
        info=None
        # Try RedirectList first (cheap)
        r = try_redirectlist(chunk)
        if r:
            kind="redirect"
            module_raw = r["module_raw"]
            module_patched = apply_redirectlist(module_raw, r["ptrs"], baseaddr)
            reloc_bytes = chunk[: r["table_bytes"]]
            # export
            base = os.path.join(outdir, f"{os.path.splitext(os.path.basename(path))[0]}_chunk{i:03d}")
            mod_raw_path   = base + "_module_raw.bin"
            mod_pat_path   = base + "_module_patched.bin"
            reloc_bin_path = base + "_reloclist.bin"
            reloc_csv_path = base + "_relocs.csv"
            os.makedirs(outdir, exist_ok=True)
            with open(mod_raw_path,"wb") as f: f.write(module_raw)
            with open(mod_pat_path,"wb") as f: f.write(module_patched)
            with open(reloc_bin_path,"wb") as f: f.write(reloc_bytes)
            with open(reloc_csv_path,"w", newline="") as f:
                w=csv.writer(f); w.writerow(["index","offset(byte)"])
                for idx, off in enumerate(r["ptrs"]):
                    w.writerow([idx, hex(off)])
            results.append({
                "chunk": i, "kind": kind, "baseaddr": hex(baseaddr),
                "module_raw": mod_raw_path, "module_patched": mod_pat_path,
                "reloc_bin": reloc_bin_path, "reloc_csv": reloc_csv_path,
                "num_relocs": r["num"], "module_size": len(module_raw)
            })
            print(f"[+] chunk {i}: RedirectList num={r['num']} module={len(module_raw)} bytes")
            continue
        # Try RELMOD scanning
        rm = try_relmod(chunk)
        if rm:
            kind="relmod"
            module_raw = chunk  # assume module = full chunk data
            module_patched = apply_relmod(module_raw, rm["entries"], baseaddr)
            # serialize reloc list
            reloc_bytes = bytearray()
            for typ, off, extra in rm["entries"]:
                reloc_bytes += struct.pack("<I", (off & ~0x3) | (typ & 0x3))
                if typ == 1:
                    reloc_bytes += struct.pack("<I", (extra or 0) & 0xFFFFFFFF)
            reloc_bytes += struct.pack("<i", -1)
            base = os.path.join(outdir, f"{os.path.splitext(os.path.basename(path))[0]}_chunk{i:03d}")
            mod_raw_path   = base + "_module_raw.bin"
            mod_pat_path   = base + "_module_patched.bin"
            reloc_bin_path = base + "_reloclist.bin"
            reloc_csv_path = base + "_relocs.csv"
            os.makedirs(outdir, exist_ok=True)
            with open(mod_raw_path,"wb") as f: f.write(module_raw)
            with open(mod_pat_path,"wb") as f: f.write(module_patched)
            with open(reloc_bin_path,"wb") as f: f.write(reloc_bytes)
            with open(reloc_csv_path,"w", newline="") as f:
                w=csv.writer(f); w.writerow(["index","type","offset","extra"])
                for idx,(typ, off, extra) in enumerate(rm["entries"]):
                    w.writerow([idx, typ, hex(off), (hex(extra) if extra is not None else "")])
            results.append({
                "chunk": i, "kind": kind, "baseaddr": hex(baseaddr),
                "module_raw": mod_raw_path, "module_patched": mod_pat_path,
                "reloc_bin": reloc_bin_path, "reloc_csv": reloc_csv_path,
                "num_relocs": len(rm["entries"]), "module_size": len(module_raw),
                "reloc_start_guess": rm["start"], "reloc_table_bytes": rm["consumed"]
            })
            print(f"[+] chunk {i}: RELMOD entries={len(rm['entries'])} module={len(module_raw)} bytes")
            continue
        # Unknown / not relocatable
        print(f"[-] chunk {i}: no relocations detected")
    # Write manifest
    manifest = os.path.join(outdir, f"{os.path.splitext(os.path.basename(path))[0]}_manifest.json")
    with open(manifest,"w") as f:
        json.dump(results, f, indent=2)
    print(f"\nWrote manifest: {manifest} ({len(results)} modules)")
    return results

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python extract_overlays_all.py <file.drm> <outdir> [baseaddr]")
        sys.exit(1)
    path = sys.argv[1]
    outdir = sys.argv[2]
    baseaddr = int(sys.argv[3], 0) if len(sys.argv) >= 4 else DEFAULT_BASE
    process_drm(path, outdir, baseaddr)
