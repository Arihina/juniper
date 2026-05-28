"""
Пример использования FIFOCache.
"""
from juniper.cache_policies import FIFOCache
from juniper.data_structures import SwissTable, RHMap, BTHashMap


def basic_usage():
    """Базовый сценарий: capacity=3, наблюдаем порядок вытеснения."""
    cache = FIFOCache(capacity=3)

    cache.put("a", 1)
    cache.put("b", 2)
    cache.put("c", 3)
    # Очередь: head -> [a, b, c] <- tail

    # Чтение НЕ продлевает жизнь ключу — это не LRU.
    assert cache.get("a") == 1
    assert "a" in cache

    # Вставка четвёртого ключа выселяет самый старый — "a".
    cache.put("d", 4)
    assert "a" not in cache
    assert cache.get("a") is None
    assert cache.get("d") == 4

    # Перезапись существующего ключа НЕ двигает его в очереди.
    cache.put("b", 999)            # b всё ещё «второй по старшинству»
    cache.put("e", 5)              # выселит "b", а не "c"
    assert "b" not in cache
    assert cache.get("c") == 3

    print("basic_usage:", cache.stats)


def explicit_evict():
    """Ручной evict() возвращает выселенный ключ; результат можно игнорировать."""
    cache = FIFOCache(capacity=2)
    cache.put("x", 10)
    cache.put("y", 20)

    evicted = cache.evict()
    assert evicted == "x"
    assert "x" not in cache

    # Если кэш пуст — None, без исключения.
    cache.evict()
    assert cache.evict() is None


def caching_none_values():
    """Хранение None как value работает корректно."""
    cache = FIFOCache(capacity=2)
    cache.put("missing_in_db", None)

    # `in` отвечает по факту присутствия, не по значению.
    assert "missing_in_db" in cache

    # get() возвращает None и для промаха, и для записанного None —
    # различить их нужно через `in`.
    assert cache.get("missing_in_db") is None
    assert cache.get("never_inserted") is None
    assert "never_inserted" not in cache


def custom_map_impl():
    """Под бенчмарки можно подменить hashmap-имплементацию."""
    for impl in (SwissTable, RHMap, BTHashMap):
        cache = FIFOCache(capacity=100, map_impl=impl)
        for i in range(150):
            cache.put(f"k{i}", i)
        # Из-за вытеснения остаются только последние 100 ключей: k50..k149
        assert "k49" not in cache
        assert "k50" in cache
        assert cache.get("k149") == 149
        print(f"{impl.__name__}: {cache.stats}")


def stats_tracking():
    """hits/misses/hit_rate обновляются на каждый get()."""
    cache = FIFOCache(capacity=10)
    for i in range(5):
        cache.put(i, i * i)

    # 5 хитов
    for i in range(5):
        cache.get(i)

    # 3 промаха
    for i in range(100, 103):
        cache.get(i)

    s = cache.stats
    assert s["hits"] == 5
    assert s["misses"] == 3
    assert s["size"] == 5
    assert abs(s["hit_rate"] - 5 / 8) < 1e-9


if __name__ == "__main__":
    basic_usage()
    explicit_evict()
    caching_none_values()
    custom_map_impl()
    stats_tracking()
    print("OK")