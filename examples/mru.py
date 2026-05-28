"""
Пример использования MRUCache.

Главное отличие MRU от FIFO/LRU: при переполнении выселяется НЕДАВНО
использованный ключ. Это контринтуитивно, но полезно при последовательных
сканах больших датасетов и в циклических нагрузках.
"""
from juniper.cache_policies import MRUCache
from juniper.data_structures import SwissTable, RHMap, BTHashMap


def basic_usage():
    """Базовый сценарий: видно, что выселяется самый недавний, а не самый старый."""
    cache = MRUCache(capacity=3)

    cache.put("a", 1)
    cache.put("b", 2)
    cache.put("c", 3)
    # Front (MRU) -> [c, b, a] <- Back (LRU)
    # При следующем переполнении выселится "c" (или то, что станет MRU после touch).

    # get обновляет MRU: теперь самый недавно использованный — "a".
    assert cache.get("a") == 1
    # Front -> [a, c, b] <- Back

    # Вставка нового ключа выселяет MRU = "a", и НА ЕГО МЕСТО встаёт "d".
    cache.put("d", 4)
    assert "a" not in cache
    assert "b" in cache
    assert "c" in cache
    assert cache.get("d") == 4

    print("basic_usage:", cache.stats)


def touch_on_get_and_put():
    """И get, и put существующего ключа делают touch (двигают в front)."""
    cache = MRUCache(capacity=2)
    cache.put("x", 1)
    cache.put("y", 2)
    # Front -> [y, x] <- Back

    # put существующего ключа: touch + обновление value.
    cache.put("x", 100)
    # Front -> [x, y] <- Back. Теперь MRU = "x".

    # Вставляем "z" — выселится "x" (MRU).
    cache.put("z", 3)
    assert "x" not in cache
    assert cache.get("y") == 2
    assert cache.get("z") == 3


def sequential_scan_advantage():
    """
    Иллюстрация, зачем нужен MRU: последовательный скан больших данных.

    Представим, что мы по очереди читаем страницы, и каждая страница после
    прочтения нам не нужна (скан в одну сторону), а старые могут пригодиться
    при повторном проходе. MRU выкидывает только что прочитанные —
    это правильное поведение для такого паттерна.
    """
    cache = MRUCache(capacity=3)

    # «Прогрев»: положили три старые страницы.
    for page in ("p1", "p2", "p3"):
        cache.put(page, f"data_{page}")

    # Скан новых страниц. Каждая тут же становится MRU и выселяется
    # следующей вставкой, а старые p1/p2/p3 остаются (с поправкой
    # на первое выселение, которое снимет самый свежий из p1/p2/p3).
    for page in ("p4", "p5", "p6", "p7"):
        cache.put(page, f"data_{page}")

    # Старые страницы (по крайней мере две из трёх) уцелели — это и есть смысл MRU.
    survivors = sum(1 for p in ("p1", "p2", "p3") if p in cache)
    assert survivors >= 2, f"ожидаем минимум 2 выживших, получили {survivors}"
    print(f"sequential_scan: уцелело старых страниц {survivors}/3")


def explicit_evict():
    """Ручной evict() возвращает выселенный ключ; результат можно игнорировать."""
    cache = MRUCache(capacity=3)
    cache.put("a", 1)
    cache.put("b", 2)
    cache.put("c", 3)
    # Front (MRU) = "c"

    evicted = cache.evict()
    assert evicted == "c"
    assert "c" not in cache

    # Кэш пуст — None, без исключения.
    cache.evict()
    cache.evict()
    assert cache.evict() is None


def caching_none_values():
    """Хранение None как value работает корректно (отличаем `in` от get())."""
    cache = MRUCache(capacity=2)
    cache.put("k", None)

    assert "k" in cache
    assert cache.get("k") is None
    assert "missing" not in cache


def custom_map_impl():
    """Параметризация map-имплементацией для бенчмарков."""
    for impl in (SwissTable, RHMap, BTHashMap):
        cache = MRUCache(capacity=50, map_impl=impl)
        for i in range(200):
            cache.put(f"k{i}", i)
        assert len(cache) == 50
        print(f"{impl.__name__}: {cache.stats}")


def stats_tracking():
    """hits/misses/hit_rate обновляются на каждый get()."""
    cache = MRUCache(capacity=10)
    for i in range(5):
        cache.put(i, i * i)

    for i in range(5):
        cache.get(i)
    for i in range(100, 103):
        cache.get(i)

    s = cache.stats
    assert s["hits"] == 5
    assert s["misses"] == 3
    assert s["size"] == 5
    assert abs(s["hit_rate"] - 5 / 8) < 1e-9


if __name__ == "__main__":
    basic_usage()
    touch_on_get_and_put()
    sequential_scan_advantage()
    explicit_evict()
    caching_none_values()
    custom_map_impl()
    stats_tracking()
    print("OK")
