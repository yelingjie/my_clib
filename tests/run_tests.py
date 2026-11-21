"""Use GCC for host tests or Cortex-M4 compilation; --cc selects another compiler."""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess


def main():
    root = Path(__file__).resolve().parents[1]
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cc", help="C compiler path (gcc, clang, or zig)")
    parser.add_argument("--arm-check", action="store_true", help="Compile and archive with arm-none-eabi-gcc; do not run")
    args = parser.parse_args()
    compiler = (args.cc or os.environ.get("ARM_CC" if args.arm_check else "CC") or
                shutil.which("arm-none-eabi-gcc" if args.arm_check else "gcc"))
    if not compiler:
        parser.error("找不到 GCC；请使用 --cc 指定编译器路径，ARM 编译需加 --arm-check。")
    build = root / "build"
    build.mkdir(exist_ok=True)
    env = os.environ.copy()
    command = [str(compiler)]
    if Path(compiler).stem.lower() == "zig":
        command.append("cc")
        env["ZIG_GLOBAL_CACHE_DIR"] = str(build / "zig-global")
        env["ZIG_LOCAL_CACHE_DIR"] = str(build / "zig-local")
    command += ["-std=c99", "-O2", "-fstrict-aliasing", "-Wall", "-Wextra",
                "-Wpedantic", "-Werror", "-I", str(root)]
    sources = sorted(root.glob("lib_*.c"))
    if args.arm_check:
        arm_build = build / "arm"
        arm_build.mkdir(exist_ok=True)
        command += ["-mcpu=cortex-m4", "-mthumb", "-mfpu=fpv4-sp-d16", "-mfloat-abi=hard",
                    "-ffreestanding", "-ffunction-sections", "-fdata-sections"]
        objects = []
        database = []
        for source in sources:
            obj = arm_build / (source.stem + ".o")
            argv = [*command, "-c", str(source), "-o", str(obj)]
            subprocess.run(argv, env=env, check=True)
            objects.append(str(obj))
            database.append({"directory": str(root), "file": str(source), "arguments": argv})
        ar = Path(compiler).with_name(Path(compiler).name.replace("gcc", "ar"))
        archive = arm_build / "libembedded_utils.a"
        subprocess.run([str(ar), "rcs", str(archive), *objects], env=env, check=True)
        (arm_build / "compile_commands.json").write_text(json.dumps(database, indent=2), encoding="utf-8")
        print("Cortex-M4 GCC compilation passed:", archive)
        return
    exe = build / ("test_lib.exe" if os.name == "nt" else "test_lib")
    command += [str(p) for p in sources]
    command += [str(root / "tests/test_lib.c"), "-o", str(exe)]
    subprocess.run(command, env=env, check=True)
    subprocess.run([str(exe)], check=True)


if __name__ == "__main__":
    main()
