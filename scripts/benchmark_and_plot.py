"""
Benchmark and Complexity Analysis Automation Script
BMSIT&M - Data Structures & Algorithms Lab (1BAIL306)
Runs benchmarks for Merge Sort, Quick Sort, Knapsack, BST, and LCS,
saving results into clean CSV format for reporting and analysis.
"""

import os
import subprocess
import time
import random
import csv
import shutil

GCC = shutil.which("gcc") or r"C:\Users\mithu\AppData\Local\Microsoft\WinGet\Packages\MartinStorsjo.LLVM-MinGW.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\llvm-mingw-20261020-ucrt-x86_64\bin\gcc.exe"

def run_benchmarks():
    os.makedirs("results", exist_ok=True)
    print("=" * 70)
    print("   RUNNING BENCHMARKS FOR SORTING & DP COMPLEXITY PLOTS")
    print("=" * 70)

    # 1. Compile 04_merge_sort.c
    merge_exe = "PART-A/04_merge_sort.exe"
    subprocess.run([GCC, "-O2", "PART-A/04_merge_sort.c", "-o", merge_exe], check=True)

    # 2. Compile 05_quick_sort.c
    quick_exe = "PART-A/05_quick_sort.exe"
    subprocess.run([GCC, "-O2", "PART-A/05_quick_sort.c", "-o", quick_exe], check=True)

    # 3. Compile 08_longest_common_subsequence.c
    lcs_exe = "PART-A/08_longest_common_subsequence.exe"
    subprocess.run([GCC, "-O2", "PART-A/08_longest_common_subsequence.c", "-o", lcs_exe], check=True)

    print("\n[1] Running Merge Sort Benchmark (n = 5000, 10000, 20000, 50000)...")
    proc = subprocess.run([merge_exe], input="1\n4\n", text=True, capture_output=True)
    with open("results/merge_sort_benchmark.txt", "w", encoding="utf-8") as f:
        f.write(proc.stdout)
    print("  -> Saved output to results/merge_sort_benchmark.txt")

    print("\n[2] Running Quick Sort Benchmark (Random, Sorted, Reverse)...")
    proc = subprocess.run([quick_exe], input="1\n2\n5\n", text=True, capture_output=True)
    with open("results/quick_sort_benchmark.txt", "w", encoding="utf-8") as f:
        f.write(proc.stdout)
    print("  -> Saved output to results/quick_sort_benchmark.txt")

    print("\n[3] Running LCS Benchmark (up to length 1000)...")
    proc = subprocess.run([lcs_exe], input="1\n4\n", text=True, capture_output=True)
    with open("results/lcs_benchmark.txt", "w", encoding="utf-8") as f:
        f.write(proc.stdout)
    print("  -> Saved output to results/lcs_benchmark.txt")

    # Cleanup temporary executables
    for exe in [merge_exe, quick_exe, lcs_exe]:
        if os.path.exists(exe):
            os.remove(exe)

    print("\n" + "=" * 70)
    print("All benchmark outputs generated successfully in 'results/' directory.")
    print("=" * 70)

if __name__ == "__main__":
    run_benchmarks()
