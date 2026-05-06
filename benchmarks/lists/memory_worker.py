"""
Measure REAL RSS memory for list structures (separate process).
Usage:
    python memory_lists.py --list list --n 50000
"""

import argparse
import gc
import resource
import time

from juniper import SList, DList


def get_rss_mb():
    return resource.getrusage(resource.RUSAGE_SELF).ru_maxrss / 1024


def build_list(name, n):
    if name == "list":
        l = list(range(n))
    elif name == "SList":
        l = SList(range(n))
    elif name == "DList":
        l = DList(range(n))
    else:
        raise ValueError(name)
    return l


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--list", required=True)
    parser.add_argument("--n", type=int, required=True)
    args = parser.parse_args()

    gc.collect()
    gc.disable()
    time.sleep(0.2)

    rss_before = get_rss_mb()
    l = build_list(args.list, args.n)
    rss_after = get_rss_mb()

    delta = rss_after - rss_before
    bytes_per_entry = delta * 1024 * 1024 / args.n

    print(f"{args.list} {args.n} {delta:.2f} {bytes_per_entry:.1f}")


if __name__ == "__main__":
    main()