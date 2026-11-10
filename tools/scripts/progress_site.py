#!/usr/bin/env python3
"""
Build a self-hosted progress site from an objdiff report (build/progress.json).

Replaces decomp.dev: everything is generated here and published with GitHub Pages.

    progress_site.py site REPORT OUTDIR [--history HISTORY.json] [--commit SHA] [--repo OWNER/NAME]
        Writes OUTDIR/index.html, OUTDIR/progress.json, OUTDIR/history.json and
        OUTDIR/badges/<category>_{code,functions}.svg (category "all" = whole project).
        The new data point is appended to the given history (if any).

    progress_site.py summary REPORT [--base BASE_REPORT.json]
        Prints a Markdown table (for $GITHUB_STEP_SUMMARY), with deltas against BASE if given.

Only the Python standard library is used.
"""

import argparse
import datetime
import html
import json
import os
import sys

MEASURES = ("total_code", "matched_code", "total_functions", "matched_functions", "total_data", "matched_data")


def num(v):
    try:
        return float(v)
    except (TypeError, ValueError):
        return 0.0


def pct(m, part, total):
    t = num(m.get(total))
    return 100.0 * num(m.get(part)) / t if t else 0.0


def load_report(path):
    with open(path) as f:
        d = json.load(f)
    cats = [("all", "All", d.get("measures", {}))]
    for c in d.get("categories", []):
        cats.append((c["id"], c.get("name", c["id"]), c.get("measures", {})))
    out = []
    for cid, name, m in cats:
        out.append({
            "id": cid,
            "name": name,
            "code_percent": pct(m, "matched_code", "total_code"),
            "functions_percent": pct(m, "matched_functions", "total_functions"),
            "data_percent": pct(m, "matched_data", "total_data"),
            **{k: int(num(m.get(k))) for k in MEASURES},
        })
    return out


# --- badges -------------------------------------------------------------------

def text_width(s):
    # Rough Verdana 11px metrics, good enough for short badge labels.
    return int(sum(7.0 if ch.isupper() or ch in "%MW" else 6.0 if ch.isdigit() else 5.5 for ch in s)) + 10


def badge_color(p):
    if p >= 100:
        return "#4c1"
    if p >= 75:
        return "#97ca00"
    if p >= 50:
        return "#dfb317"
    if p >= 25:
        return "#fe7d37"
    return "#e05d44"


def badge_svg(label, value, color):
    lw, vw = text_width(label), text_width(value)
    w = lw + vw
    esc = html.escape
    return f"""<svg xmlns="http://www.w3.org/2000/svg" width="{w}" height="20" role="img" aria-label="{esc(label)}: {esc(value)}">
<title>{esc(label)}: {esc(value)}</title>
<linearGradient id="s" x2="0" y2="100%"><stop offset="0" stop-color="#bbb" stop-opacity=".1"/><stop offset="1" stop-opacity=".1"/></linearGradient>
<clipPath id="r"><rect width="{w}" height="20" rx="3" fill="#fff"/></clipPath>
<g clip-path="url(#r)"><rect width="{lw}" height="20" fill="#555"/><rect x="{lw}" width="{vw}" height="20" fill="{color}"/><rect width="{w}" height="20" fill="url(#s)"/></g>
<g fill="#fff" text-anchor="middle" font-family="Verdana,Geneva,DejaVu Sans,sans-serif" font-size="11">
<text x="{lw / 2}" y="15" fill="#010101" fill-opacity=".3">{esc(label)}</text><text x="{lw / 2}" y="14">{esc(label)}</text>
<text x="{lw + vw / 2}" y="15" fill="#010101" fill-opacity=".3">{esc(value)}</text><text x="{lw + vw / 2}" y="14">{esc(value)}</text>
</g></svg>
"""


# --- history chart -------------------------------------------------------------

def chart_svg(history, key, width=760, height=220):
    pts = [(e["date"], e["categories"].get("all", {}).get(key, 0.0)) for e in history]
    if not pts:
        return ""
    pad_l, pad_r, pad_t, pad_b = 40, 10, 10, 25
    iw, ih = width - pad_l - pad_r, height - pad_t - pad_b
    n = len(pts)
    def x(i):
        return pad_l + (iw * i / (n - 1) if n > 1 else iw / 2)
    def y(v):
        return pad_t + ih * (1 - v / 100.0)
    grid = "".join(
        f'<line x1="{pad_l}" x2="{width - pad_r}" y1="{y(g)}" y2="{y(g)}" stroke="var(--grid)"/>'
        f'<text x="{pad_l - 6}" y="{y(g) + 4}" text-anchor="end" font-size="11" fill="var(--muted)">{g}%</text>'
        for g in (0, 25, 50, 75, 100))
    line = " ".join(f"{x(i):.1f},{y(v):.1f}" for i, (_, v) in enumerate(pts))
    first, last = pts[0][0][:10], pts[-1][0][:10]
    return f"""<svg viewBox="0 0 {width} {height}" width="100%" role="img" aria-label="{key} history">
{grid}
<polyline points="{line}" fill="none" stroke="var(--accent)" stroke-width="2"/>
<text x="{pad_l}" y="{height - 6}" font-size="11" fill="var(--muted)">{first}</text>
<text x="{width - pad_r}" y="{height - 6}" font-size="11" fill="var(--muted)" text-anchor="end">{last}</text>
</svg>"""


# --- site ----------------------------------------------------------------------

def write_site(args):
    cats = load_report(args.report)
    os.makedirs(os.path.join(args.outdir, "badges"), exist_ok=True)

    for c in cats:
        for kind, label in (("code", "Code"), ("functions", "Functions")):
            p = c[kind + "_percent"]
            with open(os.path.join(args.outdir, "badges", f"{c['id']}_{kind}.svg"), "w") as f:
                f.write(badge_svg(label, f"{p:.2f}%", badge_color(p)))

    history = []
    if args.history and os.path.exists(args.history):
        try:
            with open(args.history) as f:
                history = json.load(f)
        except (OSError, ValueError):
            print("warning: could not read previous history, starting a new one", file=sys.stderr)
            history = []

    entry = {
        "date": datetime.datetime.now(datetime.timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ"),
        "commit": args.commit or "",
        "categories": {c["id"]: {k: round(c[k], 4) for k in ("code_percent", "functions_percent", "data_percent")} for c in cats},
    }
    if not history or history[-1].get("commit") != entry["commit"] or not entry["commit"]:
        history.append(entry)

    with open(os.path.join(args.outdir, "history.json"), "w") as f:
        json.dump(history, f, indent=1)
    with open(args.report) as src, open(os.path.join(args.outdir, "progress.json"), "w") as dst:
        dst.write(src.read())

    rows = []
    for c in cats:
        rows.append(
            f"<tr><td>{html.escape(c['name'])}</td>"
            f"<td><div class=bar><span style=\"width:{c['code_percent']:.2f}%\"></span></div>{c['code_percent']:.2f}%</td>"
            f"<td>{c['matched_functions']} / {c['total_functions']} ({c['functions_percent']:.2f}%)</td>"
            f"<td>{c['matched_code']:,} / {c['total_code']:,}</td>"
            f"<td><img alt=code src=\"badges/{c['id']}_code.svg\"> <img alt=functions src=\"badges/{c['id']}_functions.svg\"></td></tr>")

    repo = html.escape(args.repo or "")
    commit = html.escape(args.commit or "")
    commit_link = f'<a href="https://github.com/{repo}/commit/{commit}">{commit[:8]}</a>' if repo and commit else commit[:8]
    page = f"""<!doctype html>
<html lang="en"><head><meta charset="utf-8"><meta name="viewport" content="width=device-width, initial-scale=1">
<title>Soul Reaverse Progress</title>
<style>
:root{{--bg:#fff;--fg:#1b1f23;--muted:#6a737d;--grid:#e1e4e8;--accent:#2f81f7;--bar:#e1e4e8}}
@media (prefers-color-scheme: dark){{:root{{--bg:#0d1117;--fg:#e6edf3;--muted:#8b949e;--grid:#30363d;--accent:#58a6ff;--bar:#30363d}}}}
body{{background:var(--bg);color:var(--fg);font:15px/1.5 system-ui,sans-serif;max-width:960px;margin:0 auto;padding:16px}}
table{{border-collapse:collapse;width:100%}} td,th{{padding:6px 8px;border-bottom:1px solid var(--grid);text-align:left;vertical-align:middle}}
.bar{{display:inline-block;width:120px;height:8px;background:var(--bar);border-radius:4px;margin-right:8px;vertical-align:middle}}
.bar span{{display:block;height:100%;background:var(--accent);border-radius:4px}}
.muted{{color:var(--muted)}} .wrap{{overflow-x:auto}}
</style></head><body>
<h1>Soul Reaverse Progress</h1>
<p class=muted>Last updated {entry['date']} &middot; commit {commit_link} &middot; <a href="progress.json">progress.json</a> &middot; <a href="history.json">history.json</a></p>
<h2>Matched code (whole project)</h2>
{chart_svg(history, "code_percent")}
<h2>By category</h2>
<div class=wrap><table><thead><tr><th>Category</th><th>Code</th><th>Functions</th><th>Bytes</th><th>Badges</th></tr></thead>
<tbody>{''.join(rows)}</tbody></table></div>
</body></html>
"""
    with open(os.path.join(args.outdir, "index.html"), "w") as f:
        f.write(page)


def write_summary(args):
    cats = load_report(args.report)
    base = {c["id"]: c for c in load_report(args.base)} if args.base and os.path.exists(args.base) else {}

    def delta(c, key):
        if c["id"] not in base:
            return ""
        d = c[key] - base[c["id"]][key]
        return f" ({'+' if d >= 0 else ''}{d:.2f})" if abs(d) >= 0.005 else ""

    print("## Progress\n")
    print("| Category | Code | Functions |")
    print("| --- | ---: | ---: |")
    for c in cats:
        print(f"| {c['name']} | {c['code_percent']:.2f}%{delta(c, 'code_percent')} "
              f"| {c['matched_functions']}/{c['total_functions']}{delta(c, 'functions_percent')} |")
    if args.base and not base:
        print("\n_No base report available; deltas omitted._")


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    s = sub.add_parser("site")
    s.add_argument("report")
    s.add_argument("outdir")
    s.add_argument("--history")
    s.add_argument("--commit")
    s.add_argument("--repo")
    m = sub.add_parser("summary")
    m.add_argument("report")
    m.add_argument("--base")
    args = ap.parse_args()
    (write_site if args.cmd == "site" else write_summary)(args)


if __name__ == "__main__":
    main()
