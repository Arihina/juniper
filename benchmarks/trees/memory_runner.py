import subprocess, sys, json

STRUCTS=["AVLSet","RBSet","BTreeSet","SkipListSet"]
SIZES=[10000,50000,200000]

def run_case(name,n):
    out=subprocess.check_output([
        sys.executable,
        "memory_worker.py",
        "--map",name,
        "--n",str(n)
    ])
    return json.loads(out)

def print_table(rows):
    print("\n" + "="*70)
    print(" MEMORY CONSUMPTION ")
    print("="*70)
    print(f"{'N':>10} {'Struct':>12} {'RSS MB':>12} {'Bytes/entry':>14}")
    print("-"*70)
    for r in rows:
        print(f"{r['n']:>10} {r['map']:>12} {r['rss']:>12.2f} {r['bpe']:>14.1f}")

def main():
    rows = []
    for n in SIZES:
        for m in STRUCTS:
            res = run_case(m, n)
            rows.append(res)
    print_table(rows)

if __name__ == "__main__":
    main()
