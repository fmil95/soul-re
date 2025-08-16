#!/usr/bin/env python3

import argparse
from dataclasses import dataclass
import git
import os
import subprocess
import sys
from colour import Color

script_dir = os.path.dirname(os.path.realpath(__file__))
root_dir = os.path.join(script_dir, "../..")
asm_dir = os.path.join(root_dir, "asm", "nonmatchings")
build_dir = os.path.join(root_dir, "build")
readme = os.path.join(root_dir, "README.md")
elf_path = os.path.join(build_dir, "KAIN2.elf")


@dataclass
class Progress:

    def __init__(self):
        self.func_sizes, self.total_size = self.get_func_sizes()
        self.all_funcs = set(self.func_sizes.keys())

        self.nonmatching_funcs = self.get_nonmatching_funcs()
        self.matching_funcs = self.all_funcs - self.nonmatching_funcs

        self.matching_size, self.nonmatching_size = self.get_funcs_sizes(
            self.func_sizes,
            self.matching_funcs,
            self.nonmatching_funcs)

        if len(self.all_funcs) == 0:
            self.funcs_matching_ratio = 0.0
            self.matching_ratio = 0.0
        else:
            self.funcs_matching_ratio = (
                len(self.matching_funcs) / len(self.all_funcs)) * 100
            self.matching_ratio = (self.matching_size / self.total_size) * 100

    def get_func_sizes(self):
        try:
            result = subprocess.run(
                ['objdump', '-x', elf_path], stdout=subprocess.PIPE)
            nm_lines = result.stdout.decode().split("\n")
        except:
            print(
                f"Error: Could not run objdump on {elf_path} - make sure that the project is built")
            sys.exit(1)

        sizes = {}
        total = 0

        for line in nm_lines:
            if " F " in line:
                components = line.split()
                size = int(components[4], 16)
                name = components[5]
                total += size
                sizes[name] = size

        return sizes, total

    def get_nonmatching_funcs(self):
        funcs = set()
        for _root, _dirs, files in os.walk(asm_dir):
            for f in files:
                if f.endswith(".s"):
                    funcs.add(f[:-2])

        return funcs

    def get_funcs_sizes(self, sizes, matchings, nonmatchings):
        msize = 0
        nmsize = 0

        for func in matchings:
            msize += sizes[func]

        for func in nonmatchings:
            if func in sizes:
                nmsize += sizes[func]

        return msize, nmsize


def lerp(a, b, alpha):
    return a + (b - a) * alpha


def main(args):

    progress = Progress()

    if args.csv:
        version = 1
        git_object = git.Repo().head.object
        timestamp = str(git_object.committed_date)
        git_hash = git_object.hexsha
        csv_list = [str(version), timestamp, git_hash, str(len(progress.all_funcs)), str(len(progress.nonmatching_funcs)),
                    str(len(progress.matching_funcs)), str(progress.total_size), str(progress.nonmatching_size), str(progress.matching_size)]
        print(",".join(csv_list))
    elif args.shield_json:
        import json

        # https://shields.io/endpoint
        color = Color("#50ca22", hue=lerp(
            0, 105/255, progress.matching_ratio / 100))
        print(json.dumps({
            "schemaVersion": 1,
            "label": f"progress",
            "message": f"{progress.matching_ratio:.2f}%",
            "color": color.hex,
        }))
    elif args.update_readme:
        with open(readme, 'r') as file:
            contents = file.readlines()

        for i, line in enumerate(contents):
            if line.find('matched functions') != -1:
                contents[i] = f"{len(progress.matching_funcs)} matched functions / {len(progress.all_funcs)} total ({progress.funcs_matching_ratio:.2f}%)\n"
                contents[i+1] = f"{progress.matching_size} matching bytes / {progress.total_size} total ({progress.matching_ratio:.2f}%)\n"

        with open(readme, 'w') as file:
            file.writelines(contents)

    else:
        if progress.matching_size + progress.nonmatching_size != progress.total_size:
            print("Warning: category/total size mismatch!\n")
        print(f"{len(progress.matching_funcs)} matched functions / {len(progress.all_funcs)} total ({progress.funcs_matching_ratio:.2f}%)")
        print(f"{progress.matching_size} matching bytes / {progress.total_size} total ({progress.matching_ratio:.2f}%)")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Reports progress for the project")
    parser.add_argument("--csv", action="store_true")
    parser.add_argument("--shield-json", action="store_true")
    parser.add_argument("--update-readme", action="store_true")
    args = parser.parse_args()

    main(args)
