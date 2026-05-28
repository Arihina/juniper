# examples/lfu_example.py
"""
Пример использования LFUCache.

LFU выселяет ключ с НАИМЕНЬШЕЙ частотой обращений. При равенстве частот
работает LRU-правило: среди равночастотных уходит самый давно
использованный.
"""
from juniper.cache_policies import LFUCache
from juniper.data_structures import SwissTable, RHMap, BTHashMap


def basic_freq_eviction():
    """
    Базовый сценарий: ключ с минимальной частотой выселяется первым,
    независимо от того, насколько недавно его трогали.
    """
    cache = LFUCache(capacity=3)

    cache.put("a", 1)   # freq=1
    cache.put("b", 2)   # freq=1
    cache.put("c", 3)   # freq=1

    # Поднимаем частоты "a" и "b". "c" остаётся с freq=1.
    cache.get("a"); cache.get("a")    # freq("a")=3
    cache.get("b")                    # freq("b")=2
    # freq: a=3, b=2, c=1

    # Вставка нового ключа выселяет ключ с min freq — "c".
    cache.put("d", 4)
    assert "c" not in cache
    assert "a" in cache
    assert "b" in cache
    assert "d" in cache               # d вошёл с freq=1

    # Если теперь вставить ещё один — выселится "d" (freq=1, как самый редкий).
    cache.put("e", 5)
    assert "d" not in cache
    assert "e" in cache

    print("basic_freq_eviction: OK")


def lru_tiebreaker():
    """
    При равенстве частот работает LRU-правило:
    среди равночастотных выселяется самый давно использованный.
    """
    cache = LFUCache(capacity=3)

    cache.put("a", 1)   # freq=1
    cache.put("b", 2)   # freq=1
    cache.put("c", 3)   # freq=1

    # Все три имеют freq=1. Touch'аем "a" и "b" — у них станет freq=2,
    # а "c" останется с freq=1 (=> уйдёт первым). Это понятный случай.
    # Интереснее: НЕ повышаем частоты, а просто положим четвёртый ключ.
    # Тогда среди {a, b, c} с одинаковой freq=1 уйдёт "a" — самый старый.
    cache.put("d", 4)
    assert "a" not in cache, "среди равночастотных уйти должен самый давний (a)"
    assert "b" in cache
    assert "c" in cache
    assert "d" in cache

    print("lru_tiebreaker: OK")


def update_value_increments_freq():
    """
    put существующего ключа НЕ просто обновляет value — он также
    повышает частоту (ключ был использован).
    """
    cache = LFUCache(capacity=2)
    cache.put("x", 1)   # freq=1
    cache.put("y", 2)   # freq=1

    # Перезапись "x" — это touch.
    cache.put("x", 100) # freq("x")=2

    # Теперь "y" имеет min freq. Вставка "z" выселит "y".
    cache.put("z", 3)
    assert "y" not in cache
    assert cache.get("x") == 100
    assert cache.get("z") == 3

    print("update_value_increments_freq: OK")


def stable_hot_set_under_lfu():
    """
    Демонстрация преимущества LFU над LRU: «вечные хиты», которые накопили
    высокую частоту, не вытесняются временной волной новых ключей.

    Сценарий: 3 «популярных» ключа сильно прогреты, потом приходит
    серия одноразовых запросов. Популярные должны выжить.
    """
    cache = LFUCache(capacity=5)

    # Прогреваем «популярные»: каждый получает freq=10.
    populars = ["pop_1", "pop_2", "pop_3"]
    for k in populars:
        cache.put(k, k)
        for _ in range(9):
            cache.get(k)
    # freq: pop_1=10, pop_2=10, pop_3=10, размер=3

    # Серия одноразовых запросов — у всех freq=1.
    for i in range(20):
        cache.put(f"once_{i}", i)
    # capacity=5, поэтому в кэше будут 3 популярных + 2 последних one-shot.

    survivors = sum(1 for k in populars if k in cache)
    assert survivors == 3, f"ожидаем все 3 популярных, выжило {survivors}"

    # И последние два one-shot тоже на месте (last-in среди freq=1).
    assert "once_19" in cache
    assert "once_18" in cache

    print(f"stable_hot_set_under_lfu: все {survivors}/3 популярных выжили")


def explicit_evict():
    """Ручной evict() возвращает выселенный ключ."""
    cache = LFUCache(capacity=3)
    cache.put("a", 1)
    cache.put("b", 2)
    cache.put("c", 3)
    # Все freq=1. evict снимет самый давний: "a".

    evicted = cache.evict()
    assert evicted == "a"

    # Кэш пуст -> None.
    cache.evict(); cache.evict()
    assert cache.evict() is None

    print("explicit_evict: OK")


def caching_none_values():
    """Хранение None как value работает корректно."""
    cache = LFUCache(capacity=2)
    cache.put("k", None)

    assert "k" in cache
    assert cache.get("k") is None
    assert "missing" not in cache

    print("caching_none_values: OK")


def custom_map_impl():
    """Параметризация map-имплементацией для бенчмарков."""
    for impl in (SwissTable, RHMap, BTHashMap):
        cache = LFUCache(capacity=50, map_impl=impl)
        for i in range(200):
            cache.put(f"k{i}", i)
        # Без get'ов все ключи имеют freq=1; LRU-tiebreaker оставит последние 50.
        assert len(cache) == 50
        assert "k150" in cache
        assert "k149" not in cache
        print(f"  {impl.__name__}: {cache.stats}")


def stats_tracking():
    """hits/misses/hit_rate обновляются на каждый get()."""
    cache = LFUCache(capacity=10)
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
    print("stats_tracking: OK")


if __name__ == "__main__":
    basic_freq_eviction()
    lru_tiebreaker()
    update_value_increments_freq()
    stable_hot_set_under_lfu()
    explicit_evict()
    caching_none_values()
    custom_map_impl()
    stats_tracking()
    print("OK")