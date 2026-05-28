"""
Пример использования LRUCache.

LRU — каноническая стратегия для нагрузок с временной локальностью:
недавно использованный ключ остаётся в кэше, давно не использованный
выселяется первым.
"""
from juniper.cache_policies import LRUCache
from juniper.data_structures import SwissTable, RHMap, BTHashMap


def basic_usage():
    """Базовый сценарий: видно, что выселяется самый давно использованный."""
    cache = LRUCache(capacity=3)

    cache.put("a", 1)
    cache.put("b", 2)
    cache.put("c", 3)
    # Front (MRU) -> [c, b, a] <- Back (LRU)
    # При переполнении выселится "a".

    # get обновляет позицию: "a" уходит во front.
    assert cache.get("a") == 1
    # Front -> [a, c, b] <- Back. Теперь LRU = "b".

    # Вставка нового ключа выселяет "b" (самый давно использованный).
    cache.put("d", 4)
    assert "b" not in cache
    assert "a" in cache
    assert "c" in cache
    assert cache.get("d") == 4

    print("basic_usage:", cache.stats)


def touch_on_get_and_put():
    """И get, и put существующего ключа делают touch (двигают в front)."""
    cache = LRUCache(capacity=2)
    cache.put("x", 1)
    cache.put("y", 2)
    # Front -> [y, x] <- Back. LRU = "x".

    # put существующего ключа: touch + обновление value.
    cache.put("x", 100)
    # Front -> [x, y] <- Back. LRU = "y".

    # Вставляем "z" — выселится "y" (LRU).
    cache.put("z", 3)
    assert "y" not in cache
    assert cache.get("x") == 100
    assert cache.get("z") == 3


def temporal_locality_advantage():
    """
    Иллюстрация, зачем LRU: горячий рабочий набор удерживается в кэше.

    Эмулируем нагрузку с локальностью: 80% обращений идут в малый "горячий"
    набор (10 ключей), 20% — в "холодный" (90 ключей). При capacity=15
    LRU должен удерживать почти все горячие ключи.

    Важная деталь модели: при miss мы делаем put — так ведёт себя реальный
    кэш (read-through). Без этого ключи никогда бы не попадали в кэш через
    одни лишь get().

    Проверка стохастическая: при конечной выборке 1-2 горячих ключа могут
    случайно не трогаться долго и быть вытеснены. Поэтому assert сделан
    мягким (>= 8/10), а в конце добавлен детерминированный «cooldown» —
    последовательное обращение ко всем горячим, после которого в кэше
    обязаны быть все 10.
    """
    import random

    random.seed(42)
    cache = LRUCache(capacity=15)

    hot = [f"hot_{i}" for i in range(10)]
    cold = [f"cold_{i}" for i in range(90)]

    # Случайная нагрузка с локальностью + read-through на miss.
    for _ in range(2000):
        key = random.choice(hot) if random.random() < 0.8 else random.choice(cold)
        if cache.get(key) is None:
            cache.put(key, key)

    # Стохастическая проверка: подавляющее большинство горячих в кэше.
    hot_in_cache = sum(1 for k in hot if k in cache)
    print(f"temporal_locality: горячих в кэше {hot_in_cache}/10 (стохастически)")
    print(f"  hit_rate = {cache.hit_rate:.3f}")
    assert hot_in_cache >= 8, (
        f"LRU должен удерживать минимум 8 горячих ключей; держит {hot_in_cache}"
    )

    # Детерминированная проверка: после явного touch всех горячих
    # они ВСЕ должны быть в кэше (capacity=15 >= 10).
    for k in hot:
        if cache.get(k) is None:
            cache.put(k, k)

    hot_after_touch = sum(1 for k in hot if k in cache)
    assert hot_after_touch == 10, (
        f"после touch всех горячих ожидаем 10/10, получили {hot_after_touch}"
    )
    print(f"  после явного touch всех hot: {hot_after_touch}/10 (детерминированно)")


def explicit_evict():
    """Ручной evict() возвращает выселенный ключ."""
    cache = LRUCache(capacity=3)
    cache.put("a", 1)
    cache.put("b", 2)
    cache.put("c", 3)
    # LRU = "a"

    evicted = cache.evict()
    assert evicted == "a"
    assert "a" not in cache

    cache.evict()
    cache.evict()
    assert cache.evict() is None


def caching_none_values():
    """Хранение None как value работает корректно."""
    cache = LRUCache(capacity=2)
    cache.put("k", None)

    assert "k" in cache
    assert cache.get("k") is None
    assert "missing" not in cache


def custom_map_impl():
    """Параметризация map-имплементацией для бенчмарков."""
    for impl in (SwissTable, RHMap, BTHashMap):
        cache = LRUCache(capacity=100, map_impl=impl)
        for i in range(150):
            cache.put(f"k{i}", i)
        # После прогрева вставкой 150 ключей подряд (без get'ов)
        # выживут последние 100: k50..k149.
        assert "k49" not in cache
        assert "k50" in cache
        assert cache.get("k149") == 149
        print(f"{impl.__name__}: {cache.stats}")


def stats_tracking():
    """hits/misses/hit_rate обновляются на каждый get()."""
    cache = LRUCache(capacity=10)
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
    temporal_locality_advantage()
    explicit_evict()
    caching_none_values()
    custom_map_impl()
    stats_tracking()
    print("OK")
