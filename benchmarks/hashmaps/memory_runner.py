import json
import subprocess
import sys

MAPS = ["dict", "HashMap", "RHMap", "BTHashMap", "SwissTable"]
SIZES = [10_000, 50_000, 200_000]

def run_case(map_name, n):
    cmd = [
        sys.executable,
        "memory_worker.py",
        "--map", map_name,
        "--n", str(n),
    ]
    out = subprocess.check_output(cmd)
    return json.loads(out)

def print_table(rows):
    print("\n" + "="*70)
    print(" REAL MEMORY CONSUMPTION (separate process per test)")
    print("="*70)
    print(f"{'N':>10} {'Map':>12} {'RSS MB':>12} {'Peak MB':>12} {'Bytes/entry':>14}")
    print("-"*70)
    for r in rows:
        print(f"{r['n']:>10} {r['map']:>12} {r['rss']:>12.2f} {r['peak']:>12.2f} {r['bpe']:>14.1f}")

def main():
    rows = []
    for n in SIZES:
        for m in MAPS:
            res = run_case(m, n)
            rows.append(res)
    print_table(rows)

if __name__ == "__main__":
    main()
    