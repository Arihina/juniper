"""
Пример использования TTLCache.

TTL-кэш автоматически удаляет записи по истечении срока их годности.
Удаление происходит лениво — на следующем обращении к кэшу.
"""
import time
from juniper.cache_policies import TTLCache
from juniper.data_structures import (
    SwissTable, RHMap, BTHashMap,
    SkipListSet, RBSet, AVLSet, BTreeSet,
)


def basic_expiration():
    """Базовый сценарий: запись живёт ровно default_ttl, потом исчезает."""
    cache = TTLCache(capacity=10, default_ttl=0.1)  # 100ms

    cache.put("a", 1)
    assert "a" in cache
    assert cache.get("a") == 1

    # Ждём истечения.
    time.sleep(0.15)

    # Запись протухла. __contains__ отвечает False без побочных эффектов.
    assert "a" not in cache
    # get запускает lazy-purge и возвращает None.
    assert cache.get("a") is None

    print("basic_expiration: OK")


def per_call_ttl_override():
    """Можно задать индивидуальный TTL для конкретной записи."""
    cache = TTLCache(capacity=10, default_ttl=10.0)  # default 10 секунд

    cache.put("short", 1, ttl=0.05)   # переопределяем — 50ms
    cache.put("long", 2)              # default — 10s

    time.sleep(0.1)

    assert "short" not in cache
    assert "long" in cache

    print("per_call_ttl_override: OK")


def put_resets_ttl():
    """put существующего ключа полностью обновляет expiry (рестарт TTL)."""
    cache = TTLCache(capacity=10, default_ttl=0.1)

    cache.put("k", 1)
    time.sleep(0.07)
    # До истечения ещё ~30ms; перезаписываем — TTL рестартует с 100ms.
    cache.put("k", 2)
    time.sleep(0.07)
    # С момента второго put прошло ~70ms, ещё 30ms запаса.
    assert cache.get("k") == 2

    # Теперь дождёмся истечения уже второго put.
    time.sleep(0.05)
    assert "k" not in cache

    print("put_resets_ttl: OK")


def get_does_not_extend_ttl():
    """В отличие от LRU/MRU/LFU, get НЕ обновляет expiry."""
    cache = TTLCache(capacity=10, default_ttl=0.1)

    cache.put("k", 1)
    time.sleep(0.05)
    cache.get("k")   # никакого продления
    cache.get("k")
    cache.get("k")
    time.sleep(0.07)
    # С момента put прошло ~120ms, запись протухла независимо от get'ов.
    assert "k" not in cache

    print("get_does_not_extend_ttl: OK")


def capacity_eviction_picks_nearest_expiry():
    """
    При переполнении выселяется ключ с ближайшим expiry —
    тот, который и так протух бы первым.
    """
    cache = TTLCache(capacity=2, default_ttl=10.0)

    cache.put("soon", 1, ttl=0.5)
    cache.put("later", 2, ttl=5.0)
    # Кэш полон. Вставка третьего вытеснит "soon" (min expiry).
    cache.put("third", 3, ttl=10.0)

    assert "soon" not in cache
    assert "later" in cache
    assert "third" in cache

    print("capacity_eviction_picks_nearest_expiry: OK")


def lazy_purge_frees_capacity():
    """
    Если все имеющиеся ключи уже протухли, новый put не должен ничего
    выселять — место освободит сам purge.
    """
    cache = TTLCache(capacity=2, default_ttl=0.05)

    cache.put("a", 1)
    cache.put("b", 2)
    time.sleep(0.1)  # обе протухли

    # Кэш «формально» полон, но purge внутри put удалит обе записи.
    cache.put("c", 3)
    cache.put("d", 4)

    assert "a" not in cache
    assert "b" not in cache
    assert "c" in cache
    assert "d" in cache
    assert len(cache) == 2

    print("lazy_purge_frees_capacity: OK")


def explicit_evict():
    """Ручной evict() возвращает выселенный ключ (с min expiry)."""
    cache = TTLCache(capacity=3, default_ttl=10.0)
    cache.put("a", 1, ttl=5.0)
    cache.put("b", 2, ttl=1.0)   # min expiry
    cache.put("c", 3, ttl=10.0)

    evicted = cache.evict()
    assert evicted == "b"
    assert "b" not in cache

    cache.evict(); cache.evict()
    assert cache.evict() is None

    print("explicit_evict: OK")


def caching_none_values():
    """None как value хранится корректно."""
    cache = TTLCache(capacity=2, default_ttl=10.0)
    cache.put("k", None)
    assert "k" in cache
    assert cache.get("k") is None
    assert "missing" not in cache

    print("caching_none_values: OK")


def all_combinations_of_impls():
    """
    Все 3 map × 4 expiry-set = 12 комбинаций — все должны работать
    идентично с точки зрения наблюдаемого поведения.
    """
    for map_impl in (SwissTable, RHMap, BTHashMap):
        for expiry_impl in (SkipListSet, RBSet, AVLSet, BTreeSet):
            cache = TTLCache(
                capacity=5,
                default_ttl=10.0,
                map_impl=map_impl,
                expiry_impl=expiry_impl,
            )
            for i in range(10):
                cache.put(f"k{i}", i, ttl=10.0)
            # Без get'ов: все freq равны, evict выбрасывал по min expiry,
            # которые в порядке вставки — выживут последние 5.
            assert len(cache) == 5
            assert "k4" not in cache
            assert "k5" in cache
            assert cache.get("k9") == 9
    print("all_combinations_of_impls: OK")


def stats_tracking():
    """hits/misses/hit_rate. Промахи по протухшим тоже считаются как miss."""
    cache = TTLCache(capacity=10, default_ttl=0.05)
    for i in range(3):
        cache.put(f"k{i}", i)

    for i in range(3):
        cache.get(f"k{i}")  # 3 хита

    time.sleep(0.1)

    for i in range(3):
        cache.get(f"k{i}")  # 3 промаха (уже протухли)

    s = cache.stats
    assert s["hits"] == 3
    assert s["misses"] == 3
    assert s["size"] == 0
    assert abs(s["hit_rate"] - 0.5) < 1e-9

    print("stats_tracking: OK")


if __name__ == "__main__":
    basic_expiration()
    per_call_ttl_override()
    put_resets_ttl()
    get_does_not_extend_ttl()
    capacity_eviction_picks_nearest_expiry()
    lazy_purge_frees_capacity()
    explicit_evict()
    caching_none_values()
    all_combinations_of_impls()
    stats_tracking()
    print("OK")
