#!/usr/bin/env python3
"""
Post-process maspsx output so that the given common symbols (`.comm`) become
real global definitions, laid out the way ccpsx/psylink did it:

    <file's static (.lcomm) symbols>  <listed common symbols, in the given order>

GNU ld allocates COMMON symbols in hash-table order (and maspsx gives them
power-of-two alignment), so plain `.comm` cannot reproduce the original
layout. Each listed symbol is emitted in the section maspsx placed it in
(.sbss / .bss), after that section's `.lcomm` entries, word aligned with its
size rounded up to a multiple of 4 (same as aspsx does for `.lcomm`).

Only the listed symbols are converted: tentative definitions that come from
headers (e.g. `FontTracker fontTracker;` included everywhere) stay `.comm` in
every other file and simply resolve to the strong definition.

Usage: maspsx ... | comm_to_sbss.py SYMBOL[:SIZE] [SYMBOL[:SIZE] ...] | as ...

SIZE overrides the size from the `.comm` directive, for symbols that are
deliberately declared with a different size in C (e.g. `x[0]`, to keep the
compiler from addressing them gp-relative).
"""

import re
import sys

COMM_RE = re.compile(r"^\s*\.comm\s+([^,\s]+)\s*,\s*(\d+)(?:\s*,\s*\d+)?\s*$")
SECTION_RE = re.compile(r"^\s*\.section\s+(\.\w+)")


def main() -> None:
    sizes: dict[str, int] = {}
    wanted = []
    for arg in sys.argv[1:]:
        symbol, _, size = arg.partition(":")
        wanted.append(symbol)
        if size:
            sizes[symbol] = int(size, 0)
    lines = sys.stdin.read().split("\n")

    out = []
    found: dict[str, tuple[str, int]] = {}
    section = ".text"

    for line in lines:
        m = SECTION_RE.match(line)
        if m:
            section = m.group(1)

        m = COMM_RE.match(line)
        if m and m.group(1) in wanted:
            target = section if section in (".sbss", ".bss") else ".sbss"
            found[m.group(1)] = (target, int(m.group(2)))
            continue

        out.append(line)

    missing = [s for s in wanted if s not in found]
    if missing:
        sys.exit(f"comm_to_sbss.py: no .comm found for: {' '.join(missing)}")

    current = None
    for symbol in wanted:
        target, size = found[symbol]
        size = sizes.get(symbol, size)
        if target != current:
            out.append(f".section {target}")
            current = target
        out.append(f"\t.globl {symbol}")
        out.append("\t.align 2")
        out.append(f"{symbol}:")
        out.append(f"\t.space {(size + 3) & ~3}")

    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
