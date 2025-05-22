def apply(config, args):
    config["baseimg"] = "SLUS_007.08"
    config["myimg"] = "build/KAIN2.EXE"
    config["mapfile"] = "build/KAIN2.map"
    config["source_directories"] = ["."]
    # config["show_line_numbers_default"] = True
    config["arch"] = "mipsel"
    # config["map_format"] = "gnu" # gnu, mw, ms
    # config["build_dir"] = "build/" # only needed for mw and ms map format
    # config["expected_dir"] = "expected/" # needed for -o
    # config["makeflags"] = []
    # config["objdump_executable"] = ""
