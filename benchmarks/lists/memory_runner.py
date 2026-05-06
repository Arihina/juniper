"""
REAL memory benchmark for lists.
Runs each test in a fresh Python process.
"""

import subprocess
import sys

LISTS = ["list", "SList", "DList"]
SIZES = [10_000, 50_000, 200_000]

print("\n" + "="*70)
print(" REAL MEMORY CONSUMPTION (Lists)")
print("="*70)
print(f"{'N':>10} {'List':>12} {'RSS MB':>12} {'Bytes/entry':>15}")
print("-"*70)

for n in SIZES:
    for lst in LISTS:
        cmd = [
            "python3.11",
            "memory_worker.py",
            "--list", lst,
            "--n", str(n),
        ]
        out = subprocess.check_output(cmd, text=True).strip()
        name, n_str, rss, bpe = out.split()
        print(f"{n_str:>10} {name:>12} {float(rss):>12.2f} {float(bpe):>15.1f}")