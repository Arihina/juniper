import argparse, json, resource, random, gc
from juniper import BSTSet, AVLSet, RBSet, BTreeSet, SkipListSet

def get_rss():
    return resource.getrusage(resource.RUSAGE_SELF).ru_maxrss/1024

def make(name):
    return {
        "set": set(),
        "BSTSet": BSTSet(),
        "AVLSet": AVLSet(),
        "RBSet": RBSet(),
        "BTreeSet": BTreeSet(),
        "SkipListSet": SkipListSet(),
    }[name]

def add(s,x):
    s.add(x)

parser=argparse.ArgumentParser()
parser.add_argument("--map")
parser.add_argument("--n",type=int)
args=parser.parse_args()

gc.collect()
rss_before=get_rss()

s=make(args.map)
for i in range(args.n):
    add(s,i)

rss_after=get_rss()
rss=rss_after-rss_before

print(json.dumps({
    "map":args.map,
    "n":args.n,
    "rss":rss,
    "bpe":rss*1024*1024/args.n
}))
