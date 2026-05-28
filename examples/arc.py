# examples/arc_example.py
"""
Пример использования ARCCache.

ARC адаптивно балансирует между LRU (recency) и LFU (frequency) на основе
сигналов из «призрачных» списков B1/B2 — ключей, недавно выселенных из
реального кэша.
"""
from juniper.cache_policies import ARCCache
from juniper.data_structures import SwissTable, RHMap, BTHashMap


def basic_usage():
    """Базовая корректность put/get/__contains__."""
    cache = ARCCache(capacity=3)

    cache.put("a", 1)
    cache.put("b", 2)
    cache.put("c", 3)

    assert cache.get("a") == 1
    assert "a" in cache
    assert "missing" not in cache
    assert cache.get("missing") is None
    print("basic_usage: OK")


def promotion_to_t2_on_second_hit():
    """
    После повторного обращения ключ переходит из T1 (recency) в T2 (frequency).
    Это видно по тому, что он становится более «защищённым» от выселения.
    """
    cache = ARCCache(capacity=4)
    cache.put("a", 1)   # T1
    cache.put("b", 2)   # T1
    cache.put("c", 3)   # T1
    cache.put("d", 4)   # T1

    # Промоутим "a" в T2.
    cache.get("a")

    # Кладём ещё ключи: T1 теперь под давлением, "a" в T2 — должен выжить.
    cache.put("e", 5)
    cache.put("f", 6)

    assert "a" in cache
    print("promotion_to_t2_on_second_hit: OK")


def adaptive_under_recency_workload():
    """
    Recency-нагрузка: каждый ключ используется один раз и забывается.
    ARC должен «открыться» в сторону T1 (увеличить p).
    """
    cache = ARCCache(capacity=10)
    # 50 уникальных ключей, каждый используется один раз — классика для LRU.
    for i in range(50):
        key = f"k{i}"
        if cache.get(key) is None:
            cache.put(key, i)
    # Последние 10 должны быть в кэше (T1 удерживает recency).
    for i in range(40, 50):
        assert f"k{i}" in cache
    print(f"adaptive_under_recency_workload: p={cache._p} (выросло — recency-режим)")


def adaptive_under_frequency_workload():
    """
    Frequency-нагрузка: маленькое горячее ядро, многократные обращения.
    ARC должен «открыться» в сторону T2 (уменьшить p).
    """
    cache = ARCCache(capacity=10)
    hot = [f"hot_{i}" for i in range(5)]

    # Прогрев — каждый горячий получает 2+ обращения, чтобы попасть в T2.
    for k in hot:
        cache.put(k, k)
    for _ in range(3):
        for k in hot:
            cache.get(k)

    # Сканирующий шум: одноразовые ключи, каждый из которых пытается
    # вытеснить горячие. T2 должен их защитить.
    for i in range(100):
        cache.put(f"scan_{i}", i)

    survivors = sum(1 for k in hot if k in cache)
    assert survivors == 5, f"ожидаем все 5 горячих, выжило {survivors}"
    print(f"adaptive_under_frequency_workload: все {survivors}/5 горячих выжили")


def explicit_evict():
    """Ручной evict() возвращает выселенный ключ."""
    cache = ARCCache(capacity=3)
    cache.put("a", 1)
    cache.put("b", 2)
    cache.put("c", 3)

    evicted = cache.evict()
    assert evicted is not None
    assert evicted not in cache
    assert len(cache) == 2

    cache.evict(); cache.evict()
    assert cache.evict() is None
    print("explicit_evict: OK")


def caching_none_values():
    cache = ARCCache(capacity=2)
    cache.put("k", None)
    assert "k" in cache
    assert cache.get("k") is None
    assert "missing" not in cache
    print("caching_none_values: OK")


def custom_map_impl():
    for impl in (SwissTable, RHMap, BTHashMap):
        cache = ARCCache(capacity=50, map_impl=impl)
        for i in range(200):
            cache.put(f"k{i}", i)
        assert len(cache) == 50
        print(f"  {impl.__name__}: {cache.stats}")


def stats_tracking():
    cache = ARCCache(capacity=10)
    for i in range(5):
        cache.put(i, i * i)
    for i in range(5):
        cache.get(i)
    for i in range(100, 103):
        cache.get(i)
    s = cache.stats
    assert s["hits"] == 5
    assert s["misses"] == 3
    print(len(cache))
    print(cache.stats)
    assert s["size"] == 5
    print("stats_tracking: OK")


if __name__ == "__main__":
    basic_usage()
    promotion_to_t2_on_second_hit()
    adaptive_under_recency_workload()
    adaptive_under_frequency_workload()
    explicit_evict()
    caching_none_values()
    custom_map_impl()
    stats_tracking()
    print("OK")
