| Policy | Best for | Weak against | Complexity | Notes |
|---|---|---|---|---|
| LRU | General workloads | Large scans | O(1) | Best default choice |
| LFU | Stable hotsets | Bursty traffic | O(1) | Highest hit ratio on skewed loads |
| ARC | Mixed/adaptive workloads | CPU overhead | O(1) | Self-tuning LRU/LFU hybrid |
| FIFO | Simple queues | Locality-heavy access | O(1) | Lowest overhead |
| MRU | Sequential scans | Temporal locality | O(1) | Opposite of LRU |
| TTL | Expiring data | High churn | O(log n) | Time-aware eviction |

## Which policy should I use?

- Use **LRU** if you don't know your workload.
- Use **LFU** for stable hotsets and popularity-based access.
- Use **ARC** when workloads change dynamically.
- Use **FIFO** for ultra-low overhead queues.
- Use **MRU** for scan-heavy workloads.
- Use **TTL** when correctness depends on expiration time.

| Policy | get | put | evict |
|---|---|---|---|
| FIFO | O(1) | O(1) | O(1) |
| LRU | O(1) | O(1) | O(1) |
| MRU | O(1) | O(1) | O(1) |
| LFU | O(1) | O(1) | O(1) |
| ARC | O(1) | O(1) | O(1) |
| TTL | O(log n) | O(log n) | O(log n) |

## Workload characteristics

| Workload | Best policy |
|---|---|
| Temporal locality | LRU |
| Stable popularity | LFU |
| Adaptive mixed load | ARC |
| Sequential scans | MRU |
| Queue-like buffering | FIFO |
| Expiring data | TTL |
