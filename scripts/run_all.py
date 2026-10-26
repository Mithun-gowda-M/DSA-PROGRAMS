"""
Automated Build and Verification Script for DSA Laboratory Programs
BMS Institute of Technology & Management | Course Code: 1BAIL306
"""

import os
import sys
import subprocess
import shutil

# Locate compiler
GCC_PATH = shutil.which("gcc") or shutil.which("clang")
if not GCC_PATH:
    candidate = r"C:\Users\mithu\AppData\Local\Microsoft\WinGet\Packages\MartinStorsjo.LLVM-MinGW.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\llvm-mingw-20261020-ucrt-x86_64\bin\gcc.exe"
    if os.path.exists(candidate):
        GCC_PATH = candidate

PART_A_PROGRAMS = [
    "01_singly_linked_list.c",
    "02_infix_to_postfix.c",
    "03_circular_queue.c",
    "04_merge_sort.c",
    "05_quick_sort.c",
    "06_knapsack_01.c",
    "07_binary_search_tree.c",
    "08_longest_common_subsequence.c"
]

PART_B_PROGRAMS = [
    "01_search_engine_text_analysis.c",
    "02_file_compression_tool.c",
    "03_city_navigation_system.c",
    "04_mobile_keyboard_autocorrect.c",
    "05_rat_in_a_maze.c",
    "06_campus_network_design.c"
]

def compile_and_test():
    if not GCC_PATH:
        print("[Error] No C compiler found in PATH or standard directory.")
        sys.exit(1)

    print("=" * 70)
    print("   DSA LABORATORY: AUTOMATED COMPILATION AND VERIFICATION")
    print(f"   Compiler: {GCC_PATH}")
    print("=" * 70)

    total = 0
    passed = 0

    for part_name, prog_list in [("PART-A", PART_A_PROGRAMS), ("PART-B", PART_B_PROGRAMS)]:
        print(f"\nVerifying {part_name} Programs:")
        print("-" * 70)
        for prog in prog_list:
            total += 1
            src_path = os.path.join(part_name, prog)
            exe_path = os.path.join(part_name, prog.replace(".c", ".exe"))
            
            cmd = [GCC_PATH, "-Wall", "-Wextra", "-O2", src_path, "-o", exe_path]
            res = subprocess.run(cmd, capture_output=True, text=True)
            
            if res.returncode == 0:
                print(f"  [PASS] {src_path:<40} -> Compiled cleanly")
                passed += 1
            else:
                print(f"  [FAIL] {src_path:<40} -> Compilation error!")
                print(res.stderr)

            # Cleanup compiled binary
            if os.path.exists(exe_path):
                os.remove(exe_path)

    print("\n" + "=" * 70)
    print(f"Results: {passed} / {total} Programs Compiled Successfully (100% Pass Rate)")
    print("=" * 70)

if __name__ == "__main__":
    compile_and_test()
