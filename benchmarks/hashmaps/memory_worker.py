import argparse
import gc
import json
import random
import resource
import time
import tracemalloc

from juniper import HashMap, RHMap, BTHashMap, SwissTable


# === ТВОЙ make_map ИЗ БЕНЧА ===
def make_map(name, n):
    if name == "dict":
        return {}
    if name == "HashMap":
        return HashMap(max(16, n * 2))
    if name == "RHMap":
        return RHMap()
    if name == "BTHashMap":
        return BTHashMap()
    if name == "SwissTable":
        return SwissTable()
    raise ValueError(name)


def get_rss_mb():
    return resource.getrusage(resource.RUSAGE_SELF).ru_maxrss / 1024


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--map", required=True)
    parser.add_argument("--n", type=int, required=True)
    args = parser.parse_args()

    N = args.n
    MAP = args.map

    # одинаковые ключи как в твоём бенче
    keys = list(range(N))
    random.shuffle(keys)

    gc.collect()
    time.sleep(0.2)

    tracemalloc.start()
    rss_before = get_rss_mb()

    # ==== создание и заполнение ====
    m = make_map(MAP, N)
    for k in keys:
        m[k] = k

    gc.collect()
    time.sleep(0.2)

    rss_after = get_rss_mb()
    _, peak_py = tracemalloc.get_traced_memory()

    rss_delta = rss_after - rss_before
    bytes_per_entry = (rss_delta * 1024 * 1024) / N

    print(json.dumps({
        "map": MAP,
        "n": N,
        "rss": rss_delta,
        "peak": peak_py / (1024*1024),
        "bpe": bytes_per_entry,
    }))


if __name__ == "__main__":
    main()
    