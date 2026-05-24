from abc import ABC, abstractmethod
from typing import Any, Optional


class CachePolicy(ABC):
    def __init__(self, capacity: int):
        if capacity <= 0:
            raise ValueError("capacity must be > 0")
        self.capacity = capacity
        self._hits = 0
        self._misses = 0

    @abstractmethod
    def get(self, key: Any) -> Optional[Any]:
        """Вернуть значение или None при промахе."""
        ...

    @abstractmethod
    def put(self, key: Any, value: Any) -> None:
        """Добавить/обновить запись, при необходимости вытеснить."""
        ...

    @abstractmethod
    def evict(self) -> Optional[Any]:
        """Вытеснить одну запись, вернуть вытесненный ключ (None если кэш пуст)."""
        ...

    @abstractmethod
    def __contains__(self, key: Any) -> bool:
        """Проверка наличия ключа без побочных эффектов и без обновления статистики."""
        ...

    @abstractmethod
    def __len__(self) -> int: ...


    def _record_hit(self) -> None:
        self._hits += 1

    def _record_miss(self) -> None:
        self._misses += 1


    @property
    def hit_rate(self) -> float:
        total = self._hits + self._misses
        return self._hits / total if total else 0.0

    @property
    def stats(self) -> dict:
        return {
            "hits": self._hits,
            "misses": self._misses,
            "hit_rate": self.hit_rate,
            "size": len(self),
        }
    