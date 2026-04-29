import subprocess, sys, json

STRUCTS=["set","BSTSet","AVLSet","RBSet","BTreeSet","SkipListSet"]
SIZES=[10000,50000,200000]

def run_case(name,n):
    out=subprocess.check_output([
        sys.executable,
        "memory_worker.py",
        "--map",name,
        "--n",str(n)
    ])
    return json.loads(out)

rows=[]
for n in SIZES:
    for s in STRUCTS:
        rows.append(run_case(s,n))

print("\nREAL MEMORY TREES")
print("N   Struct   RSS MB   Bytes/entry")
for r in rows:
    print(r["n"], r["map"], round(r["rss"],2), round(r["bpe"],1))
